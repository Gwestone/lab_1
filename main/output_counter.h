#pragma once

#include "driver/gpio.h"
#include "freertos/idf_additions.h"
#include "shift_out_lsb.h"
#include <stdint.h>
#include "digits.h"
#define DATA_PIN GPIO_NUM_4
#define SHIFT_PIN GPIO_NUM_6 //tells shift register to save current value at data pin
#define SYNC_LATCH_PIN GPIO_NUM_5 //tells shift register to output new value instead of old one

void output_counter_task(void *pvParameters){
    gpio_reset_pin(DATA_PIN);
    gpio_reset_pin(SHIFT_PIN);
    gpio_reset_pin(SYNC_LATCH_PIN);
    gpio_set_direction(DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SHIFT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SYNC_LATCH_PIN, GPIO_MODE_OUTPUT);

    uint16_t value = 0;
    shift_out_lsb(DATA_PIN, SHIFT_PIN, 0xff);

    while(1) {
        int first_digit = value / 10;
        int second_digit = value % 10;
        uint16_t binary_data = digits[second_digit] | digits[first_digit] << 8;

        gpio_set_level(SYNC_LATCH_PIN, 0);
        shift_out_lsb(DATA_PIN, SHIFT_PIN, ~binary_data);
        gpio_set_level(SYNC_LATCH_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(1000));
        value++;
        if (value >= 100) {
            value = 0;
        }
    }
}