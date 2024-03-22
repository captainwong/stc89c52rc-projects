#include <intrins.h>

#include "../lib/stc89.h"
#include "../lib/types.h"

// 7-segment common anode display

// define the CAD display table
static uint8_t code cad_table[] = {
    0xc0, // "0"
    0xf9, // "1"
    0xa4, // "2"
    0xb0, // "3"
    0x99, // "4"
    0x92, // "5"
    0x82, // "6"
    0xf8, // "7"
    0x80, // "8"
    0x90, // "9"
    0x88, // "A"
    0x83, // "b"
    0xc6, // "C"
    0xa1, // "d"
    0x86, // "E"
    0x8e, // "F"
};

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

void main(void) {
    // display cad_table's content on P1
    uint8_t i = 0;
    while (1) {
        P1 = cad_table[i];
        delay_ms(255);
        delay_ms(255);
        delay_ms(255);
        if (++i == sizeof(cad_table) / sizeof(cad_table[0])) {
            i = 0;
        }
    }
}