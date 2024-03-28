#include "../lib/delay.h"
#include "../lib/stc89.h"
#include "../lib/types.h"

sbit ST7920_RS = P2 ^ 7;
sbit ST7920_RW = P2 ^ 6;
sbit ST7920_EN = P2 ^ 5;
sbit ST7920_PSB = P2 ^ 4;
// sbit ST7920_RST = P2 ^ 3;
sbit ST7920_P4 = P0 ^ 4;
sbit ST7920_P5 = P0 ^ 5;
sbit ST7920_P6 = P0 ^ 6;
sbit ST7920_P7 = P0 ^ 7;

void pulse_en() {
    ST7920_EN = 1;
    delay_us(1);
    ST7920_EN = 0;
    delay_us(1);
}

void write_4bit(uint8_t dat, bit rs) {
    ST7920_RS = rs;
    ST7920_RW = 0;
    ST7920_P7 = (dat >> 3) & 0x01;
    ST7920_P6 = (dat >> 2) & 0x01;
    ST7920_P5 = (dat >> 1) & 0x01;
    ST7920_P4 = dat & 0x01;
    pulse_en();
}

void write_byte(uint8_t dat, bit rs) {
    write_4bit(dat >> 4, rs);
    write_4bit(dat, rs);
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

    write_4bit(0x02, 0);
    delay_us(100);
    write_cmd(0x20);
    delay_us(100);
    write_cmd(0x0c);
    delay_us(100);
    write_cmd(0x01);
    delay_ms(10);
    write_cmd(0x06);
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
    print(0, 2, "4-bit Parallel.");
    print(0, 3, "Happy Coding!!!!");

    while (1) {
    }
}
