#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"
#include "Steppers.hpp"


#include <mfl/Display.hpp>
#include <mfl/helpers/pinFromInt.h>
#include <esp_log.h>
#include <mfl/EspHttpd.hpp>
#include <mfl/Wifi.hpp>
#include <nvs_flash.h>
#include <sstream>

using namespace mfl;

auto tag = "main";

void f(httpd::Context<void, void> &c) {

}

using namespace cambot;

extern "C" void app_main(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(nvs_flash_init());

    Display display(
            mfl::Display::ControllerType::ssd1306,
            helpers::pinFromInt<CONFIG_CAMBOT_DISPLAY_CLOCK_PIN>(),
            helpers::pinFromInt<CONFIG_CAMBOT_DISPLAY_DATA_PIN>(),
            helpers::pinFromInt<CONFIG_CAMBOT_DISPLAY_RESET_PIN>());

    auto h = display.handle();
    u8g2_ClearBuffer(h);
    u8g2_SetFont(h, u8g2_font_profont17_tr);
    u8g2_DrawStr(h, 2, 17, "initializing...");
    u8g2_SendBuffer(h);

    Steppers steppers;
    auto& yaw = steppers.add(4076,
                             static_cast<gpio_num_t>(CONFIG_CAMBOT_YAW_DIRECTION_PIN),
                             static_cast<gpio_num_t>(CONFIG_CAMBOT_YAW_CLOCK_PIN));
    auto& lowerVert = steppers.addGroup(4076,{
            {
                    static_cast<gpio_num_t>(CONFIG_CAMBOT_LOWER_VERT_1_DIRECTION_PIN),
                    static_cast<gpio_num_t>(CONFIG_CAMBOT_LOWER_VERT_1_CLOCK_PIN),
                    false,
            }, {
                    static_cast<gpio_num_t>(CONFIG_CAMBOT_LOWER_VERT_2_DIRECTION_PIN),
                    static_cast<gpio_num_t>(CONFIG_CAMBOT_LOWER_VERT_2_CLOCK_PIN),
                    true,
            }});
    auto& upperVert = steppers.add(4076, static_cast<gpio_num_t>(CONFIG_CAMBOT_UPPER_VERT_DIRECTION_PIN),
                                   static_cast<gpio_num_t>(CONFIG_CAMBOT_UPPER_VERT_CLOCK_PIN));
    auto& roll = steppers.add(4076, static_cast<gpio_num_t>(CONFIG_CAMBOT_ROLL_DIRECTION_PIN),
                              static_cast<gpio_num_t>(CONFIG_CAMBOT_ROLL_CLOCK_PIN));
    auto& pitch = steppers.add(4076, static_cast<gpio_num_t>(CONFIG_CAMBOT_PITCH_DIRECTION_PIN),
                               static_cast<gpio_num_t>(CONFIG_CAMBOT_PITCH_CLOCK_PIN));
    steppers.run();

    auto stepYawLeft = [&yaw](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate yaw left");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        yaw.rotateSteps(stepper::StepperGroupTd6560::Direction::cw, steps, speed);
    };

    auto stepYawRight = [&yaw](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate yaw right");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        yaw.rotateSteps(stepper::StepperGroupTd6560::Direction::ccw, steps, speed);
    };

    auto stepLowerVertUp = [&lowerVert](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate lower vert up");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        lowerVert.rotateSteps(stepper::StepperGroupTd6560::Direction::cw, steps, speed);
    };

    auto stepLowerVertDown = [&lowerVert](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate lower vert down");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        lowerVert.rotateSteps(stepper::StepperGroupTd6560::Direction::ccw, steps, speed);
    };

    auto stepUpperVertUp = [&upperVert](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate upper vert up");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        upperVert.rotateSteps(stepper::StepperGroupTd6560::Direction::cw, steps, speed);
    };

    auto stepUpperVertDown = [&upperVert](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate uppper vert down");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        upperVert.rotateSteps(stepper::StepperGroupTd6560::Direction::ccw, steps, speed);
    };

    auto stepRollCcw = [&roll](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate roll counter clock wise");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        roll.rotateSteps(stepper::StepperGroupTd6560::Direction::ccw, steps, speed);
    };

    auto stepRollCw = [&roll](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate roll clock wise");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        roll.rotateSteps(stepper::StepperGroupTd6560::Direction::cw, steps, speed);
    };

    auto stepPitchUp = [&pitch](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate pitch up");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        pitch.rotateSteps(stepper::StepperGroupTd6560::Direction::cw, steps, speed);
    };

    auto stepPitchDown = [&pitch](httpd::Context<void, std::string>& c) {
        ESP_LOGI(tag, "rotate pitch down");
        auto steps = c.params.get<uint>("steps");
        auto speed = c.params.get<uint>("speed");
        pitch.rotateSteps(stepper::StepperGroupTd6560::Direction::ccw, steps, speed);
    };


    Wifi wifi(CONFIG_CAMBOT_HOSTNAME, CONFIG_CAMBOT_WIFI_SSID, CONFIG_CAMBOT_WIFI_PASSWORD);
    wifi.start([&](const esp_ip4_addr& addr){
        ESP_LOGI(tag, "got ip " IPSTR "\n\n", IP2STR(&addr));
        httpd::Router router;
        router.post("/yaw/left/:steps/:speed", stepYawLeft);
        router.post("/yaw/right/:steps/:speed", stepYawRight);

        router.post("/lv/up/:steps/:speed", stepLowerVertUp);
        router.post("/lv/down/:steps/:speed", stepLowerVertDown);

        router.post("/uv/up/:steps/:speed", stepUpperVertUp);
        router.post("/uv/down/:steps/:speed", stepUpperVertDown);

        router.post("/roll/ccw/:steps/:speed", stepRollCcw);
        router.post("/roll/cw/:steps/:speed", stepRollCw);

        router.post("/pitch/up/:steps/:speed", stepPitchUp);
        router.post("/pitch/down/:steps/:speed", stepPitchDown);

        EspHttpd server(router);
        server.start();
    });

    for (;;) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
