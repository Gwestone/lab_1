#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "output_counter.h"
#include "button.h"
#include "display.h"

void app_main(void)
{
    enable_display_task();
    enable_button_interupt();
    enable_output_counter_task();
}