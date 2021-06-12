#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_spi_flash.h"


#include <mfl/Display.hpp>
#include <mfl/helpers/pinFromInt.h>
#include <esp_log.h>

#include <mfl/Wifi.hpp>
#include <nvs_flash.h>

#include "App.hpp"

using namespace mfl;

auto tag = "main";

cambot::App app;

extern "C" [[noreturn]] void app_main(void) {
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(nvs_flash_init());

    app.start();

    while (true) {
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
