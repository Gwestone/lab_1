#include "display.h"

QueueHandle_t display_queue;

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

void enable_display_task(void){
    display_queue = xQueueCreate(10, sizeof(uint16_t));
    xTaskCreate(display_task, "display_task", 2048, NULL, 5, NULL);
}
