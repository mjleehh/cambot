#include <cstdint>
#include <esp_log.h>
#include <cmath>
#include <mfl/thread/LockGuard.hpp>
#include "StepperGroupTd6560.hpp"
#include "stepper-common.hpp"

using mfl::thread::LockGuard;

namespace cambot::stepper {

namespace {

// ---------------------------------------------------------------------------------------------------------------------

const char* tag = "mfl::StepperTd6560";

constexpr int sign(int value) {
    return (0 < value) - (value < 0);
}

// ---------------------------------------------------------------------------------------------------------------------

} // namespace anonymous

// ---------------------------------------------------------------------------------------------------------------------

StepperGroupTd6560::StepperGroupTd6560(uint numSteps, std::vector<StepperTd6560>&& steppers, SemaphoreHandle_t dataLock)
        : numSteps_(numSteps), steppers_(std::move(steppers)),
          stepPos_(0), delta_(0), cyclesPerStep_(0), cyclesSinceLastStep_(0), dataLock_(dataLock)
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

void StepperGroupTd6560::moveToStep(int step, uint speed) {
    auto numSteps = static_cast<int>(numSteps_);
    auto stepPos  = static_cast<int>(stepPos_);
    int delta = (step - stepPos) % numSteps;
    auto direction = delta >= 0 ? Direction::ccw : Direction::cw;
    rotateSteps(direction, delta, speed);
}

// ---------------------------------------------------------------------------------------------------------------------

void StepperGroupTd6560::moveToAngle(float angle, float angularSpeed) {
    moveToStep(angleToSteps(angle), angleToSteps(angularSpeed));
}

// ---------------------------------------------------------------------------------------------------------------------

void StepperGroupTd6560::moveToRad(float rad, float radSpeed) {
    moveToStep(radToSteps(rad), radToSteps(radSpeed));
}

// ---------------------------------------------------------------------------------------------------------------------

void StepperGroupTd6560::rotateSteps(StepperGroupTd6560::Direction direction, uint steps, uint speed) {
    if (speed < millisPerCycle()) {
        throw stepper::InvalidSpeed();
    }

    ESP_LOGI(tag, "requesting move");
    LockGuard dataLock(dataLock_);

    for (auto& stepper : steppers_) {
        auto pinLevel = (direction == Direction::cw && !stepper.inverted) || (direction == Direction::ccw && stepper.inverted);
        ESP_LOGI(tag, "setting directin level to %d, %d", pinLevel, static_cast<uint32_t>(pinLevel));
        gpio_set_level(stepper.directionPin, pinLevel);
    }

    delta_ = static_cast<int>(direction) * steps;
    cyclesPerStep_ = 1000u / speed / millisPerCycle();
    cyclesSinceLastStep_ = cyclesPerStep_;
    ESP_LOGI(tag, "delta %d, %d, %d", delta_, cyclesPerStep_, cyclesSinceLastStep_);
}

// ---------------------------------------------------------------------------------------------------------------------

void StepperGroupTd6560::rotateAngle(float angle, float angularSpeed) {
    auto direction = angle >= 0 ? Direction::ccw : Direction::cw;
    auto steps = std::abs(angleToSteps(angle));
    auto stepSpeed = angleToSteps(angularSpeed);
    rotateSteps(direction, steps, stepSpeed);
}

// ---------------------------------------------------------------------------------------------------------------------

void StepperGroupTd6560::rotateRad(float rad, float radSpeed) {
    auto direction = rad >= 0 ? Direction::ccw : Direction::cw;
    auto steps = std::abs(radToSteps(rad));
    auto stepSpeed = radToSteps(radSpeed);
    rotateSteps(direction, steps, stepSpeed);
}

// ---------------------------------------------------------------------------------------------------------------------

uint StepperGroupTd6560::numSteps() const {
    return numSteps_;
}

// ---------------------------------------------------------------------------------------------------------------------

uint8_t StepperGroupTd6560::positionSteps() const {
    return stepPos_;
}

// ---------------------------------------------------------------------------------------------------------------------

float StepperGroupTd6560::positionAngle() const {
    return stepsToAngle(stepPos_);
}

// ---------------------------------------------------------------------------------------------------------------------

float StepperGroupTd6560::positionRad() const {
    return stepsToRad(stepPos_);
}

// ---------------------------------------------------------------------------------------------------------------------

bool StepperGroupTd6560::isMoving() const {
    return delta_ != 0;
}

// ---------------------------------------------------------------------------------------------------------------------

float StepperGroupTd6560::stepsToAngle(float steps) const {
    return steps / numSteps_ * 360;
}

// ---------------------------------------------------------------------------------------------------------------------

float StepperGroupTd6560::angleToSteps(float angle) const {
    return angle / 360 * numSteps_;
}

// ---------------------------------------------------------------------------------------------------------------------

float StepperGroupTd6560::stepsToRad(float steps) const {
    return steps / numSteps_ * 2 * M_PI;
}

// ---------------------------------------------------------------------------------------------------------------------

float StepperGroupTd6560::radToSteps(float rad) const {
    return rad / (2 * M_PI) * numSteps_;
}

// ---------------------------------------------------------------------------------------------------------------------

void StepperGroupTd6560::clockUp() {
    if (needToStep()) {
        stepPos_ = (numSteps_ + stepPos_ + delta_) % numSteps_; // rotation step values should be positive
        for (auto& stepper : steppers_) {
            gpio_set_level(stepper.clockPin, 1);
        }
        delta_ -= sign(delta_);
        cyclesSinceLastStep_ = 0;
    } else if (isMoving()) {
        ++cyclesSinceLastStep_;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void StepperGroupTd6560::clockDown() {
    if (needToStep()) {
        for (auto& stepper : steppers_) {
            gpio_set_level(stepper.clockPin, 0);
        }
    }
}

bool StepperGroupTd6560::needToStep() const {
    return isMoving() && cyclesSinceLastStep_ >= cyclesPerStep_;
}

// ---------------------------------------------------------------------------------------------------------------------

}
