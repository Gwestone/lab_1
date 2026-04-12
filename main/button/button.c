#include "button.h"
#include "output_counter.h"  // for useConstant, counter, ORDER_NUMBER
#include "display.h"         // for display_queue
 
QueueHandle_t button_queue;
uint64_t last_isr_time = 0;

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

void IRAM_ATTR button_isr(void *args){
    uint64_t current_time = esp_timer_get_time();
    
    // 500ms (500,000 microseconds) debounce window
    if (current_time - last_isr_time > 500000) {
        uint32_t level = gpio_get_level(BUTTON_PIN);
        xQueueSendFromISR(button_queue, &level, NULL);
        last_isr_time = current_time;
    }
}

void enable_button_interupt(void){
    button_queue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(buttonTask, "buttonTask", 2048, NULL, 5, NULL);

    gpio_install_isr_service(0);
    gpio_reset_pin(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BUTTON_PIN, GPIO_INTR_ANYEDGE);
    gpio_isr_handler_add(BUTTON_PIN, button_isr, NULL);
}
