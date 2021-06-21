#include <cstdint>
#include <esp_log.h>
#include <cmath>
#include <mfl/thread/LockGuard.hpp>
#include "Actuator.hpp"
#include "math_utils.hpp"


using mfl::thread::LockGuard;

namespace cambot::stepper {

namespace {

// ---------------------------------------------------------------------------------------------------------------------

const char* tag = "mfl::StepperTd6560";

// ---------------------------------------------------------------------------------------------------------------------

}

// ---------------------------------------------------------------------------------------------------------------------

Actuator::Actuator(uint numSteps, std::vector<StepperTd6560>&& steppers, SemaphoreHandle_t dataLock)
        : numSteps_(numSteps), steppers_(std::move(steppers)),
          stepPos_(0), isHomed_(false), delta_(0), deltaSign_(0), cyclesPerStep_(0), cyclesSinceLastStep_(0), dataLock_(dataLock)
{
    uint64_t mask = 0;
    for (auto& stepper : steppers_) {
        mask |= BIT64(stepper.directionPin) | BIT64(stepper.clockPin);
    }

    gpio_config_t config = {
            .pin_bit_mask = mask,
            .mode = GPIO_MODE_OUTPUT,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .intr_type = GPIO_INTR_DISABLE,
    };
    gpio_config(&config);

    for (auto& stepper : steppers_) {
        gpio_set_level(stepper.directionPin, 0);
        gpio_set_level(stepper.clockPin, 0);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::moveToStep(int step, uint speed) {
    auto numSteps = static_cast<int>(numSteps_);
    int delta = (step - stepPos_) % numSteps;
    auto direction = delta >= 0 ? Direction::ccw : Direction::cw;
    speed = (speed < 1) + speed; // ensure speed to be at least 1
    ESP_LOGI(tag, "move to %d, %d, %d, %d", step, speed, stepPos_, delta);
    rotateSteps(direction, mutil::absi(delta), speed);
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::moveToAngle(float angle, float angularSpeed) {
    moveToStep(angleToSteps(angle), angleToSteps(angularSpeed));
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::moveToRad(float rad, float radSpeed) {
    moveToStep(radToSteps(rad), radToSteps(radSpeed));
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::rotateSteps(Actuator::Direction direction, uint steps, uint speed) {
    uint cyclesPerMillisecond = 1000000u / CONFIG_CAMBOT_STEPPER_CYCLE_TIME;

//    if (speed > cyclesPerMillisecond) {
//        ESP_LOGE(tag, "speed too high!\nspeed: %d\ncycles per ms: %d", speed, cyclesPerMillisecond);
//        throw stepper::InvalidSpeed();
//    }

    LockGuard dataLock(dataLock_);
    for (auto& stepper : steppers_) {
        auto pinLevel = (direction == Direction::cw && !stepper.inverted) || (direction == Direction::ccw && stepper.inverted);
        gpio_set_level(stepper.directionPin, pinLevel);
    }

    delta_ = static_cast<int>(direction) * steps;
    cyclesPerStep_ = cyclesPerMillisecond / speed;
    cyclesPerStep_ = cyclesPerStep_ * (cyclesPerStep_ > 1) + (cyclesPerStep_ <= 1);
    cyclesSinceLastStep_ = cyclesPerStep_;

    ESP_LOGI(tag, "rotate steps: \ncycles per ms:  %d\nspeed:          %d\ndelta:          %d\ncycles per step:%d", cyclesPerMillisecond, speed, delta_, cyclesPerStep_);
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::rotateAngle(float angle, float angularSpeed) {
    auto direction = angle >= 0 ? Direction::ccw : Direction::cw;
    auto steps = std::abs(angleToSteps(angle));
    auto stepSpeed = angleToSteps(angularSpeed);
    rotateSteps(direction, steps, stepSpeed);
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::rotateRad(float rad, float radSpeed) {
    auto direction = rad >= 0 ? Direction::ccw : Direction::cw;
    auto steps = std::abs(radToSteps(rad));
    auto stepSpeed = radToSteps(radSpeed);
    rotateSteps(direction, steps, stepSpeed);
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::stop() {
    LockGuard dataLock(dataLock_);
    delta_ = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::setHome() {
    LockGuard dataLock(dataLock_);
    if (isMoving()) {
        throw ErrorCalibrating();
    }
    stepPos_ = 0;
}

// ---------------------------------------------------------------------------------------------------------------------

bool Actuator::isHomed() const {
    return isHomed_;
}

// ---------------------------------------------------------------------------------------------------------------------

uint Actuator::numSteps() const {
    return numSteps_;
}

// ---------------------------------------------------------------------------------------------------------------------

int Actuator::positionSteps() const {
    return stepPos_;
}

// ---------------------------------------------------------------------------------------------------------------------

float Actuator::positionAngle() const {
    return stepsToAngle(stepPos_);
}

// ---------------------------------------------------------------------------------------------------------------------

float Actuator::positionRad() const {
    return stepsToRad(stepPos_);
}

// ---------------------------------------------------------------------------------------------------------------------

bool Actuator::isMoving() const {
    return deltaSign_ != 0;
}

// ---------------------------------------------------------------------------------------------------------------------

float Actuator::stepsToAngle(float steps) const {
    return steps / numSteps_ * 360;
}

// ---------------------------------------------------------------------------------------------------------------------

float Actuator::angleToSteps(float angle) const {
    return angle / 360.f * numSteps_;
}

// ---------------------------------------------------------------------------------------------------------------------

float Actuator::stepsToRad(float steps) const {
    return 2.f * M_PI * steps / numSteps_;
}

// ---------------------------------------------------------------------------------------------------------------------

float Actuator::radToSteps(float rad) const {
    return rad / (2.f * M_PI) * numSteps_;
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::clockUp() {
    // only clock up can start and end moving
    deltaSign_ = mutil::signi(delta_);
    if (needToStep()) {
        for (auto& stepper : steppers_) {
            gpio_set_level(stepper.clockPin, 1);
        }
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuator::clockDown() {
    if (needToStep()) {
        for (auto& stepper : steppers_) {
            gpio_set_level(stepper.clockPin, 0);
        }
        stepPos_ += deltaSign_;
        delta_ -= deltaSign_;
        cyclesSinceLastStep_ = 0;
    } else if (isMoving()) {
        ++cyclesSinceLastStep_;
    }
}

bool Actuator::needToStep() const {
    return isMoving() && cyclesSinceLastStep_ >= cyclesPerStep_;
}

// ---------------------------------------------------------------------------------------------------------------------

}
