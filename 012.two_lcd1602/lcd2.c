#include "lcd2.h"

#include "../lib/delay.h"
#include "../lib/iic.h"
#include "../lib/lcd1602_iic.h"
#include "../lib/stc89.h"

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

/**************************lcd1602_iic configurations*********************/

static lcd1602_iic_t lcd = {
    &iic,
    IIC_LCD_ADDR,
    LCD1602_IIC_BACKLIGHT,
};

void lcd2_init() {
    lcd1602_iic_init(&lcd, HD44780_1LINE, HD44780_5x10_DOTS);
}

void lcd2_print(char *str) {
    lcd1602_iic_clear(&lcd);
    lcd1602_iic_puts(&lcd, str);
}

void lcd2_clear() {
    lcd1602_iic_clear(&lcd);
}
