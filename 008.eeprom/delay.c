#include "delay.h"

void delay_us(unsigned int us) {
    while (us--) {
        _nop_();
    }
}

void delay_ms(unsigned int ms) {
    unsigned int i;
    while (ms--) {
        for (i = 0; i < 70; i++) {
            _nop_();
        }
    }
}
