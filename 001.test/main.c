#include <reg52.h>
#include <intrins.h>
#include "../lib/types.h"

#define MAIN_Fosc 11059200L

sbit LED = P1 ^ 0;
sbit buzzer = P2 ^ 0;
enum LedState { ON,
                OFF };

//@11.0592MHz
void Delay1ms(void) {
    uint8_t data i = 2, j = 199;
    _nop_();
    j = 199;
    do {
        while (--j)
            ;
    } while (--i);
}

void delay_ms(uint8_t ms) {
    while (ms--) {
        Delay1ms();
    }
}

void shift_right() {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        P1 = ~(0x01 << i);
        delay_ms(255);
    }
}

void shift_left() {
    uint8_t i;
    for (i = 0; i < 8; i++) {
        P1 = ~(0x80 >> i);
        delay_ms(255);
    }
}

void main(void) {
    while (1) {
        // LED = 0;
        P1 = 0xFF;
        delay_ms(255);
        // buzzer = 0;
        shift_right();

        // P1 = 0xFF;
        // // LED = 1;
        // // buzzer = 1;
        // shift_left();
        // delay_ms(255);
        // delay_ms(255);
    }
}