#pragma once

#include "driver/gpio.h"
#include "esp_attr.h"
#include "esp_timer.h"
#include "freertos/idf_additions.h"
#include "esp_log.h"
#include <stdint.h>

#define BUTTON_PIN GPIO_NUM_47

extern QueueHandle_t button_queue;
extern uint64_t last_isr_time;

void buttonTask(void *pvParameters);
void button_isr(void *args);
void enable_button_interupt(void);
