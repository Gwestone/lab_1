#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "blink_led.h"
#include "output_counter.h"

void app_main(void)
{
    //xTaskCreate(blink_led_task, "Blink LED", 2048, NULL, 5, NULL);
    xTaskCreate(output_counter_task, "Output Counter", 2048, NULL, 5, NULL);
}