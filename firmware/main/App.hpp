#pragma once

#include <mfl/EspHttpd.hpp>
#include <memory>
#include <mfl/Wifi.hpp>
#include <mfl/Display.hpp>

#include "Actuators.hpp"
#include "Actuator.hpp"
#include "Kinematics.h"

namespace cambot {

// ---------------------------------------------------------------------------------------------------------------------

struct App {
    enum class State {
        initial,
        displayReady,
        steppersReady,
        wifiReady,
        setHome,
        ready,
    };

    App();

    void start();

private:
    void setStatusMessage(const std::string& msg);
    void setAddress(const std::string& msg);

    struct ActuatorReferences {
        stepper::Actuator& yaw;
        stepper::Actuator& lowerVert;
        stepper::Actuator& upperVert;
        stepper::Actuator& roll;
        stepper::Actuator& pitch;
    };

    void initializeDisplay();
    void initializeSteppers();
    void startWifi();
    void initializeCalibrationPhase();
    void setReady();

    State state_;
    std::string address_;

    mfl::Display display_;
    Actuators steppers_;
    std::unique_ptr<ActuatorReferences> actuators_;
    std::unique_ptr<Kinematics> kinematics_;
    mfl::Wifi wifi_;
    mfl::httpd::Router router_;
    mfl::EspHttpd server_;


};

// ---------------------------------------------------------------------------------------------------------------------

}