#include "output_counter.h"
#include "button.h"
#include "display.h"
#include "shift_out_lsb.h"
#include "driver/gpio.h"

volatile bool useConstant = false;
uint16_t counter = 0;

void output_counter_task(void *pvParameters){

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

void enable_output_counter_task(void){
    xTaskCreate(output_counter_task, "Output Counter", 2048, NULL, 5, NULL);
}
