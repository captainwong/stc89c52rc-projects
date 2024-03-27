#include <intrins.h>

#include "../lib/delay.h"
#include "../lib/ds1302.h"
#include "../lib/lcd1602.h"
#include "../lib/stc89.h"

/***************************** lcd1602 configurations ********************/

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
static void lcd1602_write_8bits(uint8_t dat) {
    LCD1602_IO = dat;
}
static uint8_t lcd1602_read_8bits() {
    return LCD1602_IO;
}
#endif

static lcd1602_t lcd;

/***************************** ds1302 configurations ********************/

sbit DS1302_SCLK = P3 ^ 2;
sbit DS1302_IO = P3 ^ 3;
sbit DS1302_CE = P3 ^ 4;

static void ds1302_ce(uint8_t ce) {
    DS1302_CE = ce;
}

static void ds1302_sclk(uint8_t sclk) {
    DS1302_SCLK = sclk;
}

static void ds1302_write(uint8_t dat) {
    DS1302_IO = dat;
}

static uint8_t ds1302_read() {
    return DS1302_IO;
}

static const ds1302_t ds1302 = {
    ds1302_ce,
    ds1302_sclk,
    ds1302_write,
    ds1302_read,
};

/***************************** ds1302-lcd1602 helpers ********************/

static const char* get_weekday(uint8_t weekday) {
    switch (weekday) {
        case 1:
            return "Sun";
        case 2:
            return "Mon";
        case 3:
            return "Tue";
        case 4:
            return "Wed";
        case 5:
            return "Thu";
        case 6:
            return "Fri";
        case 7:
            return "Sat";
        default:
            return "Err";
    }
}

static void display_time() {
    char buf[20];
    ds1302_time_t time = {0};
    ds1302_read_time(&ds1302, &time);
    lcd1602_clear(&lcd);
    buf[0] = '2';
    buf[1] = '0';
    buf[2] = time.year / 10 + '0';
    buf[3] = time.year % 10 + '0';
    buf[4] = '-';
    buf[5] = time.month / 10 + '0';
    buf[6] = time.month % 10 + '0';
    buf[7] = '-';
    buf[8] = time.date / 10 + '0';
    buf[9] = time.date % 10 + '0';
    buf[10] = '\0';
    lcd1602_puts(&lcd, buf);
    lcd1602_puts(&lcd, " ");
    lcd1602_puts(&lcd, get_weekday(time.day));

    lcd1602_set_cursor(&lcd, 0, 1);
    buf[0] = time.hour / 10 + '0';
    buf[1] = time.hour % 10 + '0';
    buf[2] = ':';
    buf[3] = time.minute / 10 + '0';
    buf[4] = time.minute % 10 + '0';
    buf[5] = ':';
    buf[6] = time.second / 10 + '0';
    buf[7] = time.second % 10 + '0';
    buf[8] = '\0';
    lcd1602_puts(&lcd, buf);
}

void main(void) {
    lcd.write_rs = lcd1602_write_rs;
    lcd.write_en = lcd1602_write_en;
    lcd.write_rw = lcd1602_write_rw;
#if LCD1602_USE_4BIT_BUS
    lcd.io.lcd4bit.write_4bits = lcd1602_write_4bits;
    lcd.io.lcd4bit.read_4bits = lcd1602_read_4bits;
#else
    lcd.io.lcd8bit.write_8bits = lcd1602_write_8bits;
    lcd.io.lcd8bit.read_8bits = lcd1602_read_8bits;
#endif

    lcd1602_init(&lcd,
#if LCD1602_USE_4BIT_BUS
                 1,
#else
                 0,
#endif
                 HD44780_2LINE,
                 HD44780_5x8_DOTS);
    ds1302_init(&ds1302);

    while (1) {
        display_time();
        delay_ms(1000);
    }
}
