#pragma once

#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_timer.h"
#include "freertos/idf_additions.h"
#include "shift_out_lsb.h"
#include <stdint.h>
#include "digits.h"
#include "esp_log.h"

#define BUTTON_PIN GPIO_NUM_47
#define DATA_PIN GPIO_NUM_0
#define SHIFT_PIN GPIO_NUM_48 //tells shift register to save current value at data pin
#define SYNC_LATCH_PIN GPIO_NUM_45 //tells shift register to output new value instead of old one
#define ORDER_NUMBER (uint16_t)14;

QueueHandle_t button_queue, display_queue;
bool useConstant = false;
uint64_t last_isr_time = 0;
uint16_t counter = 0;

void buttonTask(void* pvParameters){
    uint32_t level;
    uint16_t valueToDisplay = ORDER_NUMBER;
    
    while(1){
        if(xQueueReceive(button_queue, &level, portMAX_DELAY)){
            ESP_LOGI("buttonTask", "Button state changed: %d", (int)level);
            
            // If level is 0 (pulled to GND), button is pressed. 
            // If level is 1 (pulled HIGH), button is released.
            useConstant = !useConstant;
            
            // If the button is currently being held down, send the static order number
            if(useConstant){
                xQueueSend(display_queue, &valueToDisplay, portMAX_DELAY);
            } else {
                xQueueSend(display_queue, &counter, portMAX_DELAY);
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
    uint64_t current_time = esp_timer_get_time();
    
    // 500ms (500,000 microseconds) debounce window
    if (current_time - last_isr_time > 500000) {
        uint32_t level = gpio_get_level(BUTTON_PIN);
        xQueueSendFromISR(button_queue, &level, NULL);
        last_isr_time = current_time;
    }
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

    counter = 0;

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

