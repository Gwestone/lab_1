#pragma once

#include "driver/gpio.h"
#include "freertos/idf_additions.h"
#define LED_PIN GPIO_NUM_2

void blink_led_task(void *pvParameters){
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    bool led_state = false;
    while(1) {
        gpio_set_level(LED_PIN, led_state);
        vTaskDelay(pdMS_TO_TICKS(1000));
        led_state = !led_state;
    }
}