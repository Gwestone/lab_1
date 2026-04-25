#pragma once

#include "freertos/idf_additions.h"
#include <stdint.h>
#include <stdbool.h>

#define ORDER_NUMBER (uint16_t)14

extern volatile bool useConstant;
extern uint16_t counter;

void output_counter_task(void *pvParameters);
void enable_output_counter_task(void);
