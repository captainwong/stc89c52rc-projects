#include "lcd1602_iic.h"

bit lcd1602_iic_write_byte(const lcd1602_iic_t* lcd, uint8_t dat) {
    // wait for the device to be ready and last time write is finished
    uint8_t retries = LCD1602_IIC_RETRY_TIMES;
    do {
        iic_start(lcd->iic);
        if (IIC_ACK == iic_write_byte(lcd->iic, lcd->addr << 1)) {
            break;
        }
        iic_stop(lcd->iic);
    } while (retries--);
    // device not ready
    if (retries == 0) {
        return IIC_NACK;
    }

    // now the device is ready, write the buf to the device
    iic_write_byte(lcd->iic, dat);
    iic_stop(lcd->iic);
    return IIC_ACK;
}

bit lcd1602_iic_write_cmd(const lcd1602_iic_t* lcd, uint8_t cmd) {
    bit ack = lcd1602_iic_write_byte(lcd, cmd & 0xfc);
    delay_us(2);
    ack = lcd1602_iic_write_byte(lcd, cmd & 0xf8);
    delay_us(2);
    ack = lcd1602_iic_write_byte(lcd, (cmd << 4) | 0x0c);
    delay_us(2);
    ack = lcd1602_iic_write_byte(lcd, (cmd << 4) | 0x08);
    delay_us(2);
    return ack;
}

bit lcd1602_iic_write_data(const lcd1602_iic_t* lcd, uint8_t dat) {
    bit ack = lcd1602_iic_write_byte(lcd, dat & 0xfd);
    delay_us(2);
    ack = lcd1602_iic_write_byte(lcd, dat & 0xf9);
    delay_us(2);
    ack = lcd1602_iic_write_byte(lcd, (dat << 4) | 0x0d);
    delay_us(2);
    ack = lcd1602_iic_write_byte(lcd, (dat << 4) | 0x09);
    return ack;
}

void lcd1602_iic_init(const lcd1602_iic_t* lcd) {
    /*
     * Wait for more than 15ms after Vcc rises to 4.5V
     * Or wait for more than 40ms after Vcc rises to 2.7V
     */
    delay_ms(40);
    // send 0x3 3 times
    lcd1602_iic_write_cmd(lcd, 0x33);
    delay_ms(5);
    lcd1602_iic_write_cmd(lcd, 0x32);
    delay_ms(5);
    // set 4-bit mode
    lcd1602_iic_write_cmd(lcd, 0x28);
    delay_ms(5);
    // display off
    // lcd1602_iic_write_cmd(lcd, 0x08);
    delay_us(50);
    // clear display
    lcd1602_iic_write_cmd(lcd, 0x01);
    delay_ms(3);
    // entry mode, auto increment with no shift
    lcd1602_iic_write_cmd(lcd, 0x06);
    delay_us(50);
    // display on, cursor off, blink off
    lcd1602_iic_write_cmd(lcd, 0x0c);
    delay_us(50);
}

void lcd1602_iic_clear(const lcd1602_iic_t* lcd) {
    lcd1602_iic_write_cmd(lcd, 0x01);
}

// set cursor position, x: 0~15, y: 0~1
void lcd1602_iic_set_cursor_pos(const lcd1602_iic_t* lcd, uint8_t x, uint8_t y) {
    uint8_t addr = 0x80;
    if (y == 1)
        addr += 0x40;
    addr += x & 0x0f;
    lcd1602_iic_write_cmd(lcd, addr);
}

void lcd1602_iic_show_cursor(const lcd1602_iic_t* lcd, bit show) {
    if (show)
        lcd1602_iic_write_cmd(lcd, 0x0e);
    else
        lcd1602_iic_write_cmd(lcd, 0x0c);
}

void lcd1602_iic_putc(const lcd1602_iic_t* lcd, char c) {
    lcd1602_iic_write_data(lcd, c);
}

// display one digit, range 0~9
void lcd1602_iic_putd(const lcd1602_iic_t* lcd, uint8_t d) {
    lcd1602_iic_write_data(lcd, d + '0');
}

// display two chars for hex, range 0x00~0xff
void lcd1602_iic_puth(const lcd1602_iic_t* lcd, uint8_t h) {
    uint8_t hi = h >> 4;
    uint8_t lo = h & 0x0f;
    lcd1602_iic_write_data(lcd, hi > 9 ? 'A' - 10 + hi : '0' + hi);
    lcd1602_iic_write_data(lcd, lo > 9 ? 'A' - 10 + lo : '0' + lo);
}

// display null-terminated string
void lcd1602_iic_puts(const lcd1602_iic_t* lcd, const char* s) {
    while (*s)
        lcd1602_iic_write_data(lcd, *s++);
}

// shift 1 char to left/right, dir='R' for right, 'L' for left
void lcd1602_iic_shift(const lcd1602_iic_t* lcd, char dir) {
    lcd1602_iic_write_cmd(lcd, dir == 'R' ? 0x1c : 0x18);
}
