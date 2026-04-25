#pragma once

#include "driver/gpio.h" 
#include "freertos/idf_additions.h"
#include "shift_out_lsb.h"
#include "digits.h"
#include "esp_log.h"
#include <stdint.h>

#define DATA_PIN      GPIO_NUM_0
#define SHIFT_PIN     GPIO_NUM_48  
#define SYNC_LATCH_PIN GPIO_NUM_45 

extern QueueHandle_t display_queue;

void display_task(void *pvParameters);
void enable_display_task(void);
