#include "lcd1602.h"

#include "delay.h"

/**************************local helpers*******************/

#define LCD1602_RS_CMD 0
#define LCD1602_RS_DATA 1
#define LCD1602_RW_WRITE 0
#define LCD1602_RW_READ 1

static void lcd_wait_ready(const lcd1602_t* lcd) {
    if (lcd->write_rw) {
        uint8_t busy = 0;
        // if (lcd->mode == LCD1602_MODE_4BIT) {
        //     lcd->io.lcd4bit.write_4bit(0xFF);
        // } else {
        //     lcd->io.lcd8bit.write_byte(0xFF);
        // }
        lcd->write_rs(LCD1602_RS_CMD);
        lcd->write_rw(LCD1602_RW_READ);
        do {
            lcd->write_en(1);
            if (lcd->mode == LCD1602_MODE_4BIT) {
                busy = lcd->io.lcd4bit.read_4bit() & 0x08;
            } else {
                busy = lcd->io.lcd8bit.read_byte() & 0x80;
            }
            lcd->write_en(0);
        } while (busy);
    } else {
        delay_ms(2);
    }
}

static void lcd_write_nowait(const lcd1602_t* lcd, uint8_t d, bit rs) {
    lcd->write_rs(rs);
    if (lcd->write_rw) {
        lcd->write_rw(LCD1602_RW_WRITE);
    }

    if (lcd->mode == LCD1602_MODE_4BIT) {
        // write high 4-bit data
        lcd->io.lcd4bit.write_4bit(d >> 4);
        lcd->write_en(1);
        lcd->write_en(0);

        // write low 4-bit data
        lcd->io.lcd4bit.write_4bit(d);
    } else {
        lcd->io.lcd8bit.write_byte(d);
    }

    lcd->write_en(1);
    lcd->write_en(0);
}

static void lcd_write(const lcd1602_t* lcd, uint8_t d, bit rs) {
    lcd_wait_ready(lcd);
    lcd_write_nowait(lcd, d, rs);
}

static void lcd_write_data(const lcd1602_t* lcd, uint8_t d) {
    lcd_write(lcd, d, LCD1602_RS_DATA);
}

static void lcd_write_cmd(const lcd1602_t* lcd, uint8_t c) {
    lcd_write(lcd, c, LCD1602_RS_CMD);
}

/**************************public functions***********************/

void lcd1602_init(const lcd1602_t* lcd) {
    // WARNING: this delay is not enough for some LCD/MCU,
    // you may need to increase it
    delay_ms(50);
    lcd->write_en(0);

    // The same command is sent three times,
    // Function Set with 8-bit interface D7–D4 = binary 0011,
    // the lower four bits are "don't care", using single enable pulses.
    lcd_write_nowait(lcd, 0x30, LCD1602_RS_CMD);
    delay_us(50);
    lcd_write_nowait(lcd, 0x30, LCD1602_RS_CMD);
    delay_us(50);
    lcd_write_nowait(lcd, 0x30, LCD1602_RS_CMD);
    delay_us(50);

    if (lcd->mode == LCD1602_MODE_4BIT) {
        // Function Set: 4-bit, 2-line, 5x7 dots
        lcd_write_nowait(lcd, 0x28, LCD1602_RS_CMD);
    } else {
        // Function Set: 8-bit, 2-line, 5x7 dots
        lcd_write_nowait(lcd, 0x38, LCD1602_RS_CMD);
    }
    delay_us(50);

    // display on, cursor off, blink off
    lcd_write_nowait(lcd, 0x0c, LCD1602_RS_CMD);
    delay_us(50);
    // entry mode, auto increment with no shift
    lcd_write_nowait(lcd, 0x06, LCD1602_RS_CMD);
    delay_us(50);
    // clear display
    lcd_write_nowait(lcd, 0x01, LCD1602_RS_CMD);
    delay_us(50);
    // cursor home
    lcd_write_nowait(lcd, 0x02, LCD1602_RS_CMD);
    delay_us(50);
}

void lcd1602_clear(const lcd1602_t* lcd) {
    lcd_write_cmd(lcd, 0x01);
}

// set cursor position, x: 0~15, y: 0~1
void lcd1602_set_cursor_pos(const lcd1602_t* lcd, uint8_t x, uint8_t y) {
    uint8_t addr = 0x80;
    if (y == 1)
        addr += 0x40;
    addr += x & 0x0f;
    lcd_write_cmd(lcd, addr);
}

void lcd1602_show_cursor(const lcd1602_t* lcd, bit show) {
    if (show)
        lcd_write_cmd(lcd, 0x0e);
    else
        lcd_write_cmd(lcd, 0x0c);
}

void lcd1602_putc(const lcd1602_t* lcd, char c) {
    lcd_write_data(lcd, c);
}

// display one digit, range 0~9
void lcd1602_putd(const lcd1602_t* lcd, uint8_t d) {
    lcd_write_data(lcd, d + '0');
}

// display two chars for hex, range 0x00~0xff
void lcd1602_puth(const lcd1602_t* lcd, uint8_t h) {
    lcd_write_data(lcd, (h >> 4) + (h >> 4) > 9 ? 'A' + (h >> 4) - 10 : '0' + (h >> 4));
    lcd_write_data(lcd, (h & 0x0f) > 9 ? 'A' + (h & 0x0f) - 10 : '0' + (h & 0x0f));
}

// display null-terminated string
void lcd1602_puts(const lcd1602_t* lcd, const char* s) {
    while (*s)
        lcd_write_data(lcd, *s++);
}

// shift 1 char to left/right, dir='R' for right, 'L' for left
void lcd1602_shift(const lcd1602_t* lcd, char dir) {
    lcd_write_cmd(lcd, dir == 'R' ? 0x1c : 0x18);
}
