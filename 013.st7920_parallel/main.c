#include "../lib/delay.h"
#include "../lib/stc89.h"
#include "../lib/types.h"

sbit ST7920_RS = P2 ^ 7;
sbit ST7920_RW = P2 ^ 6;
sbit ST7920_EN = P2 ^ 5;
sbit ST7920_PSB = P2 ^ 4;
// sbit ST7920_RST = P2 ^ 3;
#define ST7920_IO P0

void pulse_en() {
    ST7920_EN = 1;
    delay_us(1);
    ST7920_EN = 0;
    delay_us(1);
}

void write_byte(uint8_t dat, bit rs) {
    ST7920_RS = rs;
    ST7920_RW = 0;
    ST7920_IO = dat;
    pulse_en();
}

void write_cmd(uint8_t cmd) {
    write_byte(cmd, 0);
}

void write_data(uint8_t dat) {
    write_byte(dat, 1);
}

void init() {
    delay_ms(100);
    ST7920_PSB = 1;
    ST7920_EN = 0;
    write_cmd(0x30);
    write_cmd(0x0c);
    write_cmd(0x06);
    write_cmd(0x01);
    delay_ms(2);
}

void clear() {
    write_cmd(0x01);
}

void print(uint8_t x, uint8_t y, char* str) {
    uint8_t code addrs[4] = {0x80, 0x90, 0x88, 0x98};
    y &= 0x03;
    x = addrs[y] + x;
    write_cmd(x);
    while (*str) {
        write_data(*str++);
    }
}

void main(void) {
    init();
    print(0, 0, "Hello, World!");
    print(0, 1, "This is ST7920.");
    print(0, 2, "8-bit Parallel.");
    print(0, 3, "Happy Coding!!!!");

    while (1) {
    }
}
