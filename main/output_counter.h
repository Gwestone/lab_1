#pragma once

#include "driver/gpio.h"
#include "esp_attr.h"
#include "freertos/idf_additions.h"
#include "shift_out_lsb.h"
#include <stdint.h>
#include "digits.h"
#include "esp_log.h"

#define BUTTON_PIN GPIO_NUM_7
#define DATA_PIN GPIO_NUM_4
#define SHIFT_PIN GPIO_NUM_6 //tells shift register to save current value at data pin
#define SYNC_LATCH_PIN GPIO_NUM_5 //tells shift register to output new value instead of old one
#define ORDER_NUMBER (uint16_t)14;

QueueHandle_t button_queue, display_queue;
bool useConstant = false;

void buttonTask(void* pvParameters){
    uint32_t level;
    uint16_t valueToDisplay = ORDER_NUMBER;
    while(1){
        //read last value from queue
        int valueCount = uxQueueMessagesWaiting(button_queue);
        if(valueCount > 0){
            for(int i = 0; i < valueCount; i++){
                xQueueReceive(button_queue, &level, portMAX_DELAY);
            }
            ESP_LOGI("buttonTask", "Button pressed: %d", level);
            useConstant = !level;
            if(useConstant){
                xQueueSend(display_queue, &valueToDisplay, portMAX_DELAY);
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void display_task(void* pvParameters){
    uint16_t valueToDisplay;
    while(1){
        if(xQueueReceive(display_queue, &valueToDisplay, portMAX_DELAY)){
            ESP_LOGI("display_task", "Displaying: %d", valueToDisplay);
            int first_digit = valueToDisplay / 10;
            int second_digit = valueToDisplay % 10;
            uint16_t binary_data = digits[second_digit] | digits[first_digit] << 8;

            gpio_set_level(SYNC_LATCH_PIN, 0);
            shift_out_lsb(DATA_PIN, SHIFT_PIN, ~binary_data);
            gpio_set_level(SYNC_LATCH_PIN, 1);
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}

void IRAM_ATTR button_isr(void *args){
    uint32_t level = gpio_get_level(BUTTON_PIN);
    xQueueSendFromISR(button_queue, &level, NULL);
}

void enable_button_interupt(){
    button_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(buttonTask, "buttonTask", 2048, NULL, 5, NULL);

    gpio_install_isr_service(0);
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(BUTTON_PIN, button_isr, NULL);
}

void enable_display_task(){
    display_queue = xQueueCreate(10, sizeof(uint16_t));
    xTaskCreate(display_task, "display_task", 2048, NULL, 5, NULL);
}

void output_counter_task(void *pvParameters){
    enable_button_interupt();
    enable_display_task();

    gpio_reset_pin(DATA_PIN);
    gpio_reset_pin(SHIFT_PIN);
    gpio_reset_pin(SYNC_LATCH_PIN);
    gpio_set_direction(DATA_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SHIFT_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(SYNC_LATCH_PIN, GPIO_MODE_OUTPUT);

    uint16_t counter = 0;

    shift_out_lsb(DATA_PIN, SHIFT_PIN, 0xff);

    while(1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        if(useConstant) continue;

        counter++;
        if (counter >= 100) {
            counter = 0;
        }
        xQueueSend(display_queue, &counter, portMAX_DELAY);
    }
}

