#pragma once

#include "driver/gpio.h"
#include "freertos/idf_additions.h"
#include <stdbool.h>

#define LED_PIN GPIO_NUM_2

void blink_led_task(void *pvParameters);
void enable_blink_led_task(void);