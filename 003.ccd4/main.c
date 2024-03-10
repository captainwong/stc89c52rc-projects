#include <intrins.h>

#include "../stc89.h"

// 4 digit 7-segment common cathode display

sbit digit1 = P0 ^ 0;
sbit digit2 = P0 ^ 1;
sbit digit3 = P0 ^ 2;
sbit digit4 = P0 ^ 3;

// define the CCD display table
static unsigned char code ccd_table[] = {
    0x3f,  // '0'
    0x06,  // '1'
    0x5b,  // '2'
    0x4f,  // '3'
    0x66,  // '4'
    0x6d,  // '5'
    0x7d,  // '6'
    0x07,  // '7'
    0x7f,  // '8'
    0x6f,  // '9'
    0x77,  // 'A'
    0x7c,  // 'b'
    0x39,  // 'C'
    0x5e,  // 'd'
    0x79,  // 'E'
    0x71,  // 'F'
};

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
    while (ms--) {
        Delay1ms();
    }
}

void display_one_digit(unsigned char digit, unsigned char value) {
    digit1 = digit2 = digit3 = digit4 = 1;
    switch (digit) {
        case 1:
            digit1 = 0;
            break;
        case 2:
            digit2 = 0;
            break;
        case 3:
            digit3 = 0;
            break;
        case 4:
            digit4 = 0;
            break;
        default:
            return;
    }
    if (value < sizeof(ccd_table) / sizeof(ccd_table[0]))
        P1 = ccd_table[value];
}

// value range: 0 - 9999
void display_four_digits(unsigned int value) {
    if (value > 9999) {
        return;
    }
    display_one_digit(1, value / 1000);
    delay_ms(1);
    display_one_digit(2, (value % 1000) / 100);
    delay_ms(1);
    display_one_digit(3, (value % 100) / 10);
    delay_ms(1);
    display_one_digit(4, value % 10);
    delay_ms(1);
}

void main(void) {
    // display cad_table's content on P1
    unsigned int i = 0, j = 0;
    while (1) {
        display_four_digits(i);
        if (++j == 10) {
            j = 0;
            if (++i == 9999) {
                i = 0;
            }
        }
    }
}