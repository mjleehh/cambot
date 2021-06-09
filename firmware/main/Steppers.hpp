#pragma once

#include "StepperGroupTd6560.hpp"
#include <vector>
#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>

namespace cambot::stepper {

// ---------------------------------------------------------------------------------------------------------------------

struct StartupFailure : std::runtime_error {
    explicit StartupFailure() : std::runtime_error("failed to start stepper loop") {};
};

// ---------------------------------------------------------------------------------------------------------------------

struct StopFailure : std::runtime_error {
    explicit StopFailure() : std::runtime_error("failed to stop stepper loop") {};
};

// ---------------------------------------------------------------------------------------------------------------------

}

namespace cambot {

// ---------------------------------------------------------------------------------------------------------------------

struct Steppers {
    Steppers();
    ~Steppers();

    /**
     * add an actuator moved by a single stepper
     */
    stepper::StepperGroupTd6560& add(uint numSteps, gpio_num_t directionPin, gpio_num_t clockPin);

    /**
     * add an actuator that is powered by a group of steppers
     */
    stepper::StepperGroupTd6560& addGroup(uint numSteps, std::vector<stepper::StepperTd6560>&& steppers);


    void run();
    void stop();

private:
    enum State {
        idle,
        startRequested,
        active,
        stopRequested,
    } state_;

    void runUpdateLoop();
    [[noreturn]] static void updateTaskWrapper(void* param);

    SemaphoreHandle_t dataLock_;
    std::vector<stepper::StepperGroupTd6560*> steppers_;
    TaskHandle_t updateTask_;
};

// ---------------------------------------------------------------------------------------------------------------------

}

