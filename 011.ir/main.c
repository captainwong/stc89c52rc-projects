#include "../lib/lcd1602.h"
#include "../lib/stc89.h"
#include "../lib/uart.h"
#include "ir.h"

sbit LED = P2 ^ 0;

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
static void lcd1602_write_4bit(uint8_t dat) {
    LCD1602_D7 = (dat >> 3) & 0x01;
    LCD1602_D6 = (dat >> 2) & 0x01;
    LCD1602_D5 = (dat >> 1) & 0x01;
    LCD1602_D4 = (dat >> 0) & 0x01;
}
static uint8_t lcd1602_read_4bit() {
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

static void display_ir_value() {
    uint8_t i;
    lcd1602_clear(&lcd);
    lcd1602_set_cursor_pos(&lcd, 0, 0);
    for (i = 0; i < sizeof(ir_value); i++) {
        lcd1602_puth(&lcd, ir_value[i]);
        lcd1602_putc(&lcd, ' ');
    }
    
    lcd1602_set_cursor_pos(&lcd, 0, 1);
    switch(ir_value[2]) {
        case 0x45:
            lcd1602_puts(&lcd, "CH-");
            break;
        case 0x46:
            lcd1602_puts(&lcd, "CH");
            break;
        case 0x47:
            lcd1602_puts(&lcd, "CH+");
            break;
        case 0x44:
            lcd1602_puts(&lcd, "PREV");
            break;
        case 0x40:
            lcd1602_puts(&lcd, "NEXT");
            break;
        case 0x43:
            lcd1602_puts(&lcd, "PLAY/PAUSE");
            break;
        case 0x07:
            lcd1602_puts(&lcd, "VOL-");
            break;
        case 0x15:
            lcd1602_puts(&lcd, "VOL+");
            break;
        case 0x09:
            lcd1602_puts(&lcd, "EQ");
            break;
        case 0x16:
            lcd1602_puts(&lcd, "0");
            break;
        case 0x19:
            lcd1602_puts(&lcd, "100+");
            break;
        case 0x0d:
            lcd1602_puts(&lcd, "200+");
            break;
        case 0x0c:
            lcd1602_puts(&lcd, "1");
            break;
        case 0x18:
            lcd1602_puts(&lcd, "2");
            break;
        case 0x5e:
            lcd1602_puts(&lcd, "3");
            break;
        case 0x08:
            lcd1602_puts(&lcd, "4");
            break;
        case 0x1c:
            lcd1602_puts(&lcd, "5");
            break;
        case 0x5a:
            lcd1602_puts(&lcd, "6");
            break;
        case 0x42:
            lcd1602_puts(&lcd, "7");
            break;
        case 0x52:
            lcd1602_puts(&lcd, "8");
            break;
        case 0x4a:
            lcd1602_puts(&lcd, "9");
            break;
        default:
            lcd1602_puts(&lcd, "Unknown");
            break;
    }
}

void main(void) {
#if LCD1602_USE_4BIT_BUS
    lcd.mode = LCD1602_MODE_4BIT;
#else
    lcd.mode = LCD1602_MODE_8BIT;
#endif
    lcd.write_rs = lcd1602_write_rs;
    lcd.write_en = lcd1602_write_en;
    lcd.write_rw = lcd1602_write_rw;
#if LCD1602_USE_4BIT_BUS
    lcd.io.lcd4bit.write_4bit = lcd1602_write_4bit;
    lcd.io.lcd4bit.read_4bit = lcd1602_read_4bit;
#else
    lcd.io.lcd8bit.write_byte = lcd1602_io_write;
    lcd.io.lcd8bit.read_byte = lcd1602_io_read;
#endif
    lcd1602_init(&lcd);

    uart_init();
    ir_init();
    EA = 1;

    while (1) {
        LED = IR_PIN;

        if (ir_valid) {
            ir_valid = 0;
            uart_send_bytes(ir_value, sizeof(ir_value));
            display_ir_value();
        }

        if (ir_error_line) {
            uart_send_byte(ir_error_line);
            ir_error_line = 0;
            uart_send_byte(ir_counter >> 8);
            uart_send_byte(ir_counter);
        }
    }
}
