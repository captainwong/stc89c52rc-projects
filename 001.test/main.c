#include <reg52.h>
#include <intrins.h>

#define MAIN_Fosc 11059200L

sbit LED = P1 ^ 0;
sbit buzzer = P2 ^ 0;
enum LedState { ON,
                OFF };

//@11.0592MHz
void Delay1ms(void) {
    unsigned char data i = 2, j = 199;
    _nop_();
    j = 199;
    do {
        while (--j)
            ;
    } while (--i);
}

void delay_ms(unsigned char ms) {
    // unsigned int i;
    // do {
    //     i = MAIN_Fosc / 13000;
    //     while (--i)
    //         ;  // 14T per loop
    // } while (--ms);

    while (ms--) {
        Delay1ms();
    }
}

void shift_right() {
    unsigned char i;
    for (i = 0; i < 8; i++) {
        P1 = ~(0x01 << i);
        delay_ms(255);
    }
}

void shift_left() {
    unsigned char i;
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