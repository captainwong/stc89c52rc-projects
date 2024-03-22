#include "delay.h"

void delay_us(uint16_t us) {
    while (us--) {
        _nop_();
    }
}

void delay_ms(uint16_t ms) {
    uint16_t i;
    while (ms--) {
        for (i = 0; i < 70; i++) {
            _nop_();
        }
    }
}
