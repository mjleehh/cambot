#include "Actuators.hpp"
#include <mfl/thread/LockGuard.hpp>
#include <esp_log.h>

using mfl::thread::LockGuard;


namespace cambot {

// ---------------------------------------------------------------------------------------------------------------------

namespace {

const char* tag = "mfl::Steppers";

const uint MAX_NUM_DELAYS_WAITING_FOR_TASK = 10;

}

// ---------------------------------------------------------------------------------------------------------------------

Actuators::Actuators() : state_(State::idle), currentCyclePhase_(CyclePhase::up), timer_(nullptr) {
    // avoid the pit
    vSemaphoreCreateBinary(dataLock_);
}

// ---------------------------------------------------------------------------------------------------------------------

Actuators::~Actuators() {
    for (auto stepper : actuators_) {
        delete stepper;
    }
    if (timer_ != nullptr) {
        esp_timer_delete(timer_);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

stepper::Actuator& Actuators::add(uint numSteps, gpio_num_t directionPin, gpio_num_t clockPin, bool inverted) {
    actuators_.push_back(new stepper::Actuator(numSteps, {{directionPin, clockPin, inverted}}, dataLock_));
    return *actuators_.back();
}

// ---------------------------------------------------------------------------------------------------------------------

stepper::Actuator& Actuators::addGroup(uint numSteps, std::vector<stepper::StepperTd6560>&& steppers) {
    actuators_.push_back(new stepper::Actuator(numSteps, std::move(steppers), dataLock_));
    return *actuators_.back();
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuators::timerCallbackWrapper(void* param) {
    auto self = static_cast<Actuators*>(param);
    self->timerCallback();
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuators::run() {
    ESP_LOGI(tag, "requesting stepper module startup");
    LockGuard dataLock(dataLock_);
    if (state_ != State::idle) {
        throw stepper::StartupFailure();
    }

    if (timer_ == nullptr) {
        const esp_timer_create_args_t timerArgs = {
                .callback = &timerCallbackWrapper,
                .arg = this,
                .name = "Steppers::timerCallback",
        };

        if (esp_timer_create(&timerArgs, &timer_) != ESP_OK) {
            throw stepper::StartupFailure();
        }
    }

    if (esp_timer_start_periodic(timer_, CONFIG_CAMBOT_STEPPER_CYCLE_TIME / 2) != ESP_OK) {
        throw stepper::StartupFailure();
    }

    state_ = State::active;
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuators::shutdown() {
    ESP_LOGI(tag, "requesting stepper module shutdown");
    LockGuard dataLock(dataLock_);
    if (state_ != State::active) {
        throw stepper::StopFailure();
    }
    state_ = State::stopRequested;

    for (auto i = 0; i < MAX_NUM_DELAYS_WAITING_FOR_TASK; ++i) {
        // idle until killed
        vTaskDelay(1000);
        LockGuard dataLock(dataLock_);
        if (state_ == State::idle) {
            esp_timer_stop(timer_);
            return;
        }
    }
    throw stepper::StopFailure();
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuators::stop() {
    for (auto stepper : actuators_) {
        stepper->stop();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuators::setHome() {
    for (auto stepper : actuators_) {
        stepper->setHome();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Actuators::timerCallback() {
    //REG_WRITE(GPIO_OUT_W1TS_REG, 0x00010001)

    LockGuard lock(dataLock_);
    switch (currentCyclePhase_) {
        case CyclePhase::up:
            // only confirm exit prior to cycle
            if (state_ != State::active) {
                state_ = State::idle;
                ESP_LOGI(tag, "exiting stepper control loop");
                return;
            }

            for (auto stepper : actuators_) {
                stepper->clockUp();
            }
            currentCyclePhase_ = CyclePhase::down;
            break;
        case CyclePhase::down:
            for (auto stepper : actuators_) {
                stepper->clockDown();
            }
            currentCyclePhase_ = CyclePhase::up;
            break;
        default:
            ESP_LOGE(tag, "corrupted cycle phase value");
    }
    lock.release();
}

// ---------------------------------------------------------------------------------------------------------------------

}