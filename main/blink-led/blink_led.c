#include "blink_led.h"

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

void enable_blink_led_task(void){
    xTaskCreate(blink_led_task, "Blink LED", 2048, NULL, 5, NULL);
}
