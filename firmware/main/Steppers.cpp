#include "Steppers.hpp"
#include "stepper-common.hpp"
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

Steppers::Steppers() : state_(idle), updateTask_(nullptr) {
    // avoid the pit
    vSemaphoreCreateBinary(dataLock_);
}

// ---------------------------------------------------------------------------------------------------------------------

Steppers::~Steppers() {
    for (auto stepper : steppers_) {
        delete stepper;
    }
}

// ---------------------------------------------------------------------------------------------------------------------

stepper::StepperGroupTd6560& Steppers::add(uint numSteps, gpio_num_t directionPin, gpio_num_t clockPin) {
    steppers_.push_back(new stepper::StepperGroupTd6560(numSteps, {{directionPin, clockPin, false}}, dataLock_));
    return *steppers_.back();
}

// ---------------------------------------------------------------------------------------------------------------------

stepper::StepperGroupTd6560& Steppers::addGroup(uint numSteps, std::vector<stepper::StepperTd6560>&& steppers) {
    steppers_.push_back(new stepper::StepperGroupTd6560(numSteps, std::move(steppers), dataLock_));
    return *steppers_.back();
}

// ---------------------------------------------------------------------------------------------------------------------

[[noreturn]] void Steppers::updateTaskWrapper(void* param) {
    auto self = static_cast<Steppers*>(param);
    self->runUpdateLoop();
    while (true) {
        vTaskDelay(1000);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Steppers::run() {
    ESP_LOGI(tag, "requesting stepper module startup");
    LockGuard dataLock(dataLock_);
    if (state_ != State::idle) {
        throw stepper::StartupFailure();
    }
    state_ = State::startRequested;
    if (xTaskCreatePinnedToCore(updateTaskWrapper, "steppers_task", 4096, this, tskIDLE_PRIORITY, &updateTask_, 1) != pdPASS) {
        throw stepper::StartupFailure();
    }
}

// ---------------------------------------------------------------------------------------------------------------------

void Steppers::stop() {
    ESP_LOGI(tag, "requesting stepper module shutdown");
    LockGuard dataLock(dataLock_);
    if (state_ != active) {
        throw stepper::StopFailure();
    }
    state_ = State::stopRequested;

    for (auto i = 0; i < MAX_NUM_DELAYS_WAITING_FOR_TASK; ++i) {
        // idle until killed
        vTaskDelay(1000);
        LockGuard dataLock(dataLock_);
        if (state_ == State::idle) {
            return;
        }
    }
    throw stepper::StopFailure();
}

// ---------------------------------------------------------------------------------------------------------------------

void Steppers::runUpdateLoop() {
    ESP_LOGI(tag, "entering stepper control loop");
    LockGuard lock(dataLock_);
    state_ = State::active;
    lock.release();

    for (;;) {
        LockGuard lock(dataLock_);
        if (state_ != State::active) {
            state_ = State::idle;
            ESP_LOGI(tag, "exiting stepper control loop");
            return;
        }

        for (auto stepper : steppers_) {
            stepper->clockUp();
        }
        vTaskDelay(CONFIG_CAMBOT_STEPPER_CLOCK_DELAY);
        for (auto stepper : steppers_) {
            stepper->clockDown();
        }
        lock.release();
        vTaskDelay(CONFIG_CAMBOT_STEPPER_MOVE_DELAY);
    }
}

// ---------------------------------------------------------------------------------------------------------------------

}