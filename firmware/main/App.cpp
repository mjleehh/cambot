#include "App.hpp"

#include <esp_log.h>
#include <mfl/helpers/pinFromInt.h>
#include "sdkconfig.h"
#include "additional_response_types.hpp"

using mfl::helpers::pinFromInt;

namespace cambot {

namespace {

// ---------------------------------------------------------------------------------------------------------------------

const char* tag = "cambot::App";

// ---------------------------------------------------------------------------------------------------------------------

}

// ---------------------------------------------------------------------------------------------------------------------

App::App() :
        state_(State::initial),
        display_(
                mfl::Display::ControllerType::ssd1306,
                pinFromInt<CONFIG_CAMBOT_DISPLAY_CLOCK_PIN>(),
                pinFromInt<CONFIG_CAMBOT_DISPLAY_DATA_PIN>(),
                pinFromInt<CONFIG_CAMBOT_DISPLAY_RESET_PIN>()),
        wifi_(CONFIG_CAMBOT_HOSTNAME, CONFIG_CAMBOT_WIFI_SSID, CONFIG_CAMBOT_WIFI_PASSWORD),
        server_(router_)
{

}

// ---------------------------------------------------------------------------------------------------------------------

void App::start() {
    initializeDisplay();
}

// ---------------------------------------------------------------------------------------------------------------------

void App::initializeDisplay() {
    if (state_ != State::initial) {
        ESP_LOGE(tag, "trying to start display for non initial app state %d", static_cast<int>(state_));
        return;
    }

    display_.start();
    setStatusMessage("starting up...");

    state_ = State::displayReady;
    initializeSteppers();
}

// ---------------------------------------------------------------------------------------------------------------------

void App::initializeSteppers() {
    if (state_ != State::displayReady) {
        ESP_LOGE(tag, "trying to start steppers for non display ready app state %d", static_cast<int>(state_));
        return;
    }

    auto h = display_.handle();
    u8g2_SetFont(h, u8g2_font_profont17_tr);
    u8g2_DrawStr(h, 2, 34, "starting steppers...");
    u8g2_SendBuffer(h);

    auto &yaw = steppers_.add(CONFIG_CAMBOT_YAW_NUM_STEPS,
                              pinFromInt<CONFIG_CAMBOT_YAW_DIRECTION_PIN>(),
                              pinFromInt<CONFIG_CAMBOT_YAW_CLOCK_PIN>());
    auto &lowerVert = steppers_.addGroup(CONFIG_CAMBOT_LOWER_VERT_NUM_STEPS, {
            {
                    pinFromInt<CONFIG_CAMBOT_LOWER_VERT_1_DIRECTION_PIN>(),
                    pinFromInt<CONFIG_CAMBOT_LOWER_VERT_1_CLOCK_PIN>(),
                    false,
            },
            {
                    pinFromInt<CONFIG_CAMBOT_LOWER_VERT_2_DIRECTION_PIN>(),
                    pinFromInt<CONFIG_CAMBOT_LOWER_VERT_2_CLOCK_PIN>(),
                    true,
            }});
    auto &upperVert = steppers_.add(CONFIG_CAMBOT_UPPER_VERT_NUM_STEPS, pinFromInt<CONFIG_CAMBOT_UPPER_VERT_DIRECTION_PIN>(),
                                    pinFromInt<CONFIG_CAMBOT_UPPER_VERT_CLOCK_PIN>());
    auto &roll = steppers_.add(CONFIG_CAMBOT_ROLL_NUM_STEPS, pinFromInt<CONFIG_CAMBOT_ROLL_DIRECTION_PIN>(),
                               pinFromInt<CONFIG_CAMBOT_ROLL_CLOCK_PIN>());
    auto &pitch = steppers_.add(CONFIG_CAMBOT_PITCH_NUM_STEPS, pinFromInt<CONFIG_CAMBOT_PITCH_DIRECTION_PIN>(),
                                pinFromInt<CONFIG_CAMBOT_PITCH_CLOCK_PIN>());

    actuators_.reset(new ActuatorReferences{
            .yaw = yaw,
            .lowerVert = lowerVert,
            .upperVert = upperVert,
            .roll = roll,
            .pitch = pitch,
    });

    setStatusMessage("steppers configured");
    state_ = State::steppersReady;
    startWifi();
}

void App::startWifi() {
    if (state_ != State::steppersReady) {
        ESP_LOGE(tag, "trying to start steppers for non steppers ready app state %d", static_cast<int>(state_));
        return;
    }
    wifi_.start([this](const esp_ip4_addr& addr) {
        char ipString[16];
        sprintf(ipString, IPSTR, IP2STR(&addr));
        ESP_LOGI(tag, "got ip %s", ipString);
        setAddress(ipString);
        setStatusMessage("wifi ready");
        state_ = State::wifiReady;
        initializeCalibrationPhase();
    });
}

void App::initializeCalibrationPhase() {
    if (state_ != State::wifiReady) {
        ESP_LOGE(tag, "trying to start steppers for non wifi ready app state %d", static_cast<int>(state_));
        return;
    }

    auto stepYawLeft = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate yaw left");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->yaw.rotateSteps(stepper::Actuator::Direction::cw, steps, speed);
    };

    auto stepYawRight = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate yaw right");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->yaw.rotateSteps(stepper::Actuator::Direction::ccw, steps, speed);
    };

    auto stepLowerVertUp = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate lower vert up");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->lowerVert.rotateSteps(stepper::Actuator::Direction::cw, steps, speed);
    };

    auto stepLowerVertDown = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate lower vert down");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->lowerVert.rotateSteps(stepper::Actuator::Direction::ccw, steps, speed);
    };

    auto stepUpperVertUp = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate upper vert up");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->upperVert.rotateSteps(stepper::Actuator::Direction::cw, steps, speed);
    };

    auto stepUpperVertDown = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate uppper vert down");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->upperVert.rotateSteps(stepper::Actuator::Direction::ccw, steps, speed);
    };

    auto stepRollCcw = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate roll counter clock wise");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->roll.rotateSteps(stepper::Actuator::Direction::ccw, steps, speed);
    };

    auto stepRollCw = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate roll clock wise");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->roll.rotateSteps(stepper::Actuator::Direction::cw, steps, speed);
    };

    auto stepPitchUp = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate pitch up");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->pitch.rotateSteps(stepper::Actuator::Direction::cw, steps, speed);
    };

    auto stepPitchDown = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate pitch down");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        actuators_->pitch.rotateSteps(stepper::Actuator::Direction::ccw, steps, speed);
    };

    auto setHome = [this](mfl::httpd::Context<void, void>& c) {
        ESP_LOGI(tag, "set home");
        if (state_ == State::setHome) {
            steppers_.setHome();
            setStatusMessage("ready");
            state_ = State::ready;
            setReady();
            return;
        } else if (state_ == State::ready) {
            steppers_.setHome();
        } else {
            ESP_LOGE(tag, "trying to set home non calibrate or ready state %d", static_cast<int>(state_));
        }
    };

    auto rotations = [this](mfl::httpd::Context<void, nlohmann::json>& c) {
        c.res.body = {
              {"yaw", {
                      {"deg", actuators_->yaw.positionAngle()},
                      {"steps", actuators_->yaw.positionSteps()}
              }},
              {"lowerVert", {
                      {"deg", actuators_->lowerVert.positionAngle()},
                      {"steps", actuators_->lowerVert.positionSteps()}
              }},
              {"upperVert", {
                      {"deg", actuators_->upperVert.positionAngle()},
                      {"steps", actuators_->upperVert.positionSteps()}
              }},
              {"roll", {
                      {"deg", actuators_->roll.positionAngle()},
                      {"steps", actuators_->roll.positionSteps()}
              }},
              {"pitch", {
                      {"deg", actuators_->pitch.positionAngle()},
                      {"steps", actuators_->pitch.positionSteps()}
              }},
        };
    };

    router_.post("/calibrate/yaw/left/:steps/:speed", stepYawLeft);
    router_.post("/calibrate/yaw/right/:steps/:speed", stepYawRight);

    router_.post("/calibrate/lv/up/:steps/:speed", stepLowerVertUp);
    router_.post("/calibrate/lv/down/:steps/:speed", stepLowerVertDown);

    router_.post("/calibrate/uv/up/:steps/:speed", stepUpperVertUp);
    router_.post("/calibrate/uv/down/:steps/:speed", stepUpperVertDown);

    router_.post("/calibrate/roll/ccw/:steps/:speed", stepRollCcw);
    router_.post("/calibrate/roll/cw/:steps/:speed", stepRollCw);

    router_.post("/calibrate/pitch/up/:steps/:speed", stepPitchUp);
    router_.post("/calibrate/pitch/down/:steps/:speed", stepPitchDown);

    router_.post("/calibrate/set-home", setHome);

    router_.get("/rotations", rotations);

    server_.start();
    steppers_.run();

    setStatusMessage("set home...");
    state_ = State::setHome;
}

void App::setReady() {
    kinematics_ = std::make_unique<Kinematics>(actuators_->yaw, actuators_->lowerVert, actuators_->upperVert, actuators_->roll, actuators_->pitch);
    auto position = [this](mfl::httpd::Context<void, vecs::int3>& c){
        c.res.body = kinematics_->position();
    };
    router_.get("/position", position);

    auto moveYaw = [this](mfl::httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "move yaw");
        auto angle = c.params.get<int>("angle");
        auto speed = c.params.get<uint>("speed");
        actuators_->yaw.moveToAngle(angle, speed);
    };
    router_.post("/move/yaw/:angle/:speed", moveYaw);
//
//    router_.post("/move/lv/:steps/:speed", stepLowerVertUp);
//
//    router_.post("/move/uv/:steps/:speed", stepUpperVertUp);
//
//    router_.post("/move/roll/:steps/:speed", stepRollCcw);
//
//    router_.post("/move/pitch/:steps/:speed", stepPitchUp);
}

// ---------------------------------------------------------------------------------------------------------------------

void App::setStatusMessage(const std::string& msg) {
    auto h = display_.handle();
    u8g2_ClearBuffer(h);
    u8g2_SetFont(h, u8g2_font_profont17_tr);
    u8g2_DrawStr(h, 2, 17, "Kimbot");
    u8g2_DrawStr(h, 2, 34, msg.c_str());
    u8g2_DrawStr(h, 2, 51, address_.c_str());
    u8g2_SendBuffer(h);
}

void App::setAddress(const std::string &msg) {
    address_ = msg;
}

// ---------------------------------------------------------------------------------------------------------------------

}