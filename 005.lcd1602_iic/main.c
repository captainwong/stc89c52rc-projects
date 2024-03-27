#include "../lib/delay.h"
#include "../lib/iic.h"
#include "../lib/stc89.h"

// manipulate the LCD1602 with raw commands
// or comment this macro to use the lcd1602_iic_t interface
// #define SIMPLE_TEST

/**************************iic configurations*************************/
sbit IIC_SCL = P3 ^ 2;
sbit IIC_SDA = P3 ^ 3;

#define IIC_LCD_ADDR 0x27

static void iic_write_sda(uint8_t dat) {
    IIC_SDA = dat;
}

static void iic_write_scl(uint8_t dat) {
    IIC_SCL = dat;
}

static bit iic_read_sda() {
    return IIC_SDA;
}

static const iic_t iic = {
    iic_write_scl,
    iic_write_sda,
    iic_read_sda,
};

#ifdef SIMPLE_TEST

// this implementation is based on the Arduino LiquidCrystal_I2C library

#define RS_BIT 0x01
#define RW_BIT 0x02
#define EN_BIT 0x04

// flags for backlight control
#define LCD_BACKLIGHT 0x08
#define LCD_NOBACKLIGHT 0x00

static void expanderWrite(uint8_t dat) {
    iic_start(&iic);
    iic_write_byte(&iic, IIC_LCD_ADDR << 1);
    iic_write_byte(&iic, dat | LCD_BACKLIGHT);
    iic_stop(&iic);
}

static void pulseEnable(uint8_t dat) {
    expanderWrite(dat | EN_BIT);   // EN high
    delay_us(1);                   // enable pulse must be >450ns
    expanderWrite(dat & ~EN_BIT);  // EN low
    delay_us(50);                  // commands need > 37us to settle
}

static void write4bits(uint8_t value) {
    expanderWrite(value);
    pulseEnable(value);
}

static void send(uint8_t value, uint8_t mode) {
    uint8_t hi = value & 0xf0;
    uint8_t lo = (value << 4) & 0xf0;
    write4bits(hi | mode);
    write4bits(lo | mode);
}

static void send_cmd(uint8_t cmd) {  // 写命令函数
    send(cmd, 0);
}

static void send_data(uint8_t dat) {  // 写数据函数
    send(dat, RS_BIT);
}

static void init_lcd() {
    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands. Arduino can turn on way befer 4.5V so we'll wait 50
    delay_ms(50);

    // Now we pull both RS and R/W low to begin commands
    expanderWrite(LCD_NOBACKLIGHT);  // reset expander and turn backlight off (Bit 8 =1)
    delay_ms(1000);

    // put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write4bits(0x03 << 4);
    delay_ms(5);  // wait min 4.1ms

    // second try
    write4bits(0x03 << 4);
    delay_ms(5);  // wait min 4.1ms

    // third go!
    write4bits(0x03 << 4);
    delay_ms(1);

    // finally, set to 4-bit interface
    write4bits(0x02 << 4);

    // set # lines, font size, etc.
    send_cmd(0x28);  // 4-bit mode, 2-line, 5x8 dots

    // turn the display on with no cursor or blinking default
    send_cmd(0x0c);

    // clear it off
    send_cmd(0x01);  // clear display, set cursor position to zero
    delay_us(2000);  // this command takes a long time!

    // Initialize to default text direction (for roman languages)
    send_cmd(0x06);

    // home
    send_cmd(0x02);  // set cursor position to zero
    delay_us(2000);  // this command takes a long time!
}

static void print(uint8_t x, uint8_t y, uint8_t *str) {
    if (y == 1) {
        send_cmd(0xc0 | (x & 0x0f));
    } else {
        send_cmd(0x80 | (x & 0x0f));
    }
    while (*str) {
        send_data(*str++);
    }
}
#else  // SIMPLE_TEST
#include "lcd1602_iic.h"
/**************************lcd1602_iic configurations*********************/

static lcd1602_iic_t lcd = {
    &iic,
    IIC_LCD_ADDR,
    16,
    2,
    HD44780_5x8_DOTS,
    LCD1602_IIC_BACKLIGHT,
};

#endif

main() {
#ifdef SIMPLE_TEST
    init_lcd();

    while (1) {
        send_cmd(0x01);
        print(0, 0, "Hello, Bilibili!");
        print(0, 1, "Hello, Zhihu!");
        delay_ms(2000);

        send_cmd(0x01);
        print(0, 0, "Hello, world!");
        print(0, 1, "Hello, github!");
        delay_ms(2000);
    }

#else
    lcd1602_iic_init(&lcd);

    while (1) {
        lcd1602_iic_clear(&lcd);
        lcd1602_iic_set_cursor(&lcd, 0, 0);
        lcd1602_iic_puts(&lcd, "Hello, Bilibili!");
        lcd1602_iic_set_cursor(&lcd, 0, 1);
        lcd1602_iic_puts(&lcd, "Hello, Zhihu!");
        delay_ms(2000);

        lcd1602_iic_clear(&lcd);
        lcd1602_iic_set_cursor(&lcd, 0, 0);
        lcd1602_iic_puts(&lcd, "Hello, world!");
        lcd1602_iic_set_cursor(&lcd, 0, 1);
        lcd1602_iic_puts(&lcd, "Hello, github!");
        delay_ms(2000);
    }
#endif
}