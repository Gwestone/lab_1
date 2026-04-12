#pragma once

#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include <stdint.h>

void shift_out_lsb(gpio_num_t data_pin, gpio_num_t clk_pin, uint16_t data);
