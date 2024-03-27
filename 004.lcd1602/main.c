#include <intrins.h>

#include "../lib/delay.h"
#include "../lib/lcd1602.h"
#include "../lib/stc89.h"

#define LCD1602_USE_4BIT_BUS 1
sbit LCD1602_RS = P2 ^ 7;
sbit LCD1602_EN = P2 ^ 6;
sbit LCD1602_RW = P2 ^ 5;
#define LCD1602_IO P0

#if LCD1602_USE_4BIT_BUS
sbit LCD1602_D4 = P0 ^ 4;
sbit LCD1602_D5 = P0 ^ 5;
sbit LCD1602_D6 = P0 ^ 6;
sbit LCD1602_D7 = P0 ^ 7;
#endif

static void lcd1602_write_rs(uint8_t rs) {
    LCD1602_RS = rs;
}

static void lcd1602_write_en(uint8_t en) {
    LCD1602_EN = en;
}

static void lcd1602_write_rw(uint8_t rw) {
    LCD1602_RW = rw;
}

#if LCD1602_USE_4BIT_BUS
static void lcd1602_write_4bits(uint8_t dat) {
    LCD1602_D7 = (dat >> 3) & 0x01;
    LCD1602_D6 = (dat >> 2) & 0x01;
    LCD1602_D5 = (dat >> 1) & 0x01;
    LCD1602_D4 = (dat >> 0) & 0x01;
}
static uint8_t lcd1602_read_4bits() {
    uint8_t dat = LCD1602_D7;
    dat = (dat << 1) | LCD1602_D6;
    dat = (dat << 1) | LCD1602_D5;
    dat = (dat << 1) | LCD1602_D4;
    return dat;
}
#else
static void lcd1602_io_write(uint8_t dat) {
    LCD1602_IO = dat;
}
static uint8_t lcd1602_io_read() {
    return LCD1602_IO;
}
#endif

static lcd1602_t lcd;

void main(void) {
    lcd.write_rs = lcd1602_write_rs;
    lcd.write_en = lcd1602_write_en;
    lcd.write_rw = lcd1602_write_rw;
#if LCD1602_USE_4BIT_BUS
    lcd.io.lcd4bit.write_4bits = lcd1602_write_4bits;
    lcd.io.lcd4bit.read_4bits = lcd1602_read_4bits;
#else
    lcd.io.lcd8bit.write_byte = lcd1602_io_write;
    lcd.io.lcd8bit.read_byte = lcd1602_io_read;
#endif

    lcd1602_init(&lcd,
#if LCD1602_USE_4BIT_BUS
                 1,
#else
                 0,
#endif
                 HD44780_2LINE,
                 HD44780_5x8_DOTS);

    while (1) {
        lcd1602_clear(&lcd);
        lcd1602_set_cursor(&lcd, 0, 0);
        lcd1602_puts(&lcd, "Hi, I'm LCD1602!");
        lcd1602_set_cursor(&lcd, 0, 1);
        lcd1602_puts(&lcd, "No, I'm BATMAN!");
        delay_ms(2000);

        lcd1602_clear(&lcd);
        lcd1602_set_cursor(&lcd, 0, 0);
        lcd1602_puts(&lcd, "Hello, world!");
        lcd1602_set_cursor(&lcd, 0, 1);
        lcd1602_puts(&lcd, "Hello, github!");
        delay_ms(2000);
    }
}
