#pragma once

#include "Actuator.hpp"
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

struct Actuators {
    Actuators();
    ~Actuators();

    /**
     * add an actuator moved by a single stepper
     */
    stepper::Actuator& add(uint numSteps, gpio_num_t directionPin, gpio_num_t clockPin, bool inverted = false);

    /**
     * add an actuator that is powered by a group of steppers
     */
    stepper::Actuator& addGroup(uint numSteps, std::vector<stepper::StepperTd6560>&& steppers);


    void run();
    void shutdown();

    /**
     * stop all steppers
     */
    void stop();

    /**
     * make all the steppers current position the 0 position
     */
    void setHome();

private:
    enum class State {
        idle,
        active,
        stopRequested,
    } state_;

    enum class CyclePhase {
        up,
        down,
    } currentCyclePhase_;

    void timerCallback();
    static void timerCallbackWrapper(void* param);

    SemaphoreHandle_t dataLock_;
    std::vector<stepper::Actuator*> actuators_;
    esp_timer_handle_t timer_;
};

// ---------------------------------------------------------------------------------------------------------------------

}

