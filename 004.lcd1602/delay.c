#include "delay.h"

void delay_ms(unsigned int ms) {
    unsigned int i;
    while (ms--) {
        for (i = 0; i < 101; i++) {
            _nop_();
        }
    }
}
