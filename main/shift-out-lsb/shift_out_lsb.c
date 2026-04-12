#include "shift_out_lsb.h"

void shift_out_lsb(gpio_num_t data_pin, gpio_num_t clk_pin, uint16_t data) {
    for (int i = 0; i < 16; i++) {
        // Output the bit (LSB first)
        gpio_set_level(data_pin, (data >> i) & 0x01);
        
        // Pulse the clock
        gpio_set_level(clk_pin, 1);
        esp_rom_delay_us(10);
        gpio_set_level(clk_pin, 0);
    }
}
