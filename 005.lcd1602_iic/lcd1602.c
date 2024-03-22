#include "lcd1602.h"

#include "delay.h"

/**************************local helpers*******************/

#define LCD1602_RS_CMD 0
#define LCD1602_RS_DATA 1
#define LCD1602_RW_WRITE 0
#define LCD1602_RW_READ 1
#define lcd_write_data(d) lcd_write(d, LCD1602_RS_DATA)
#define lcd_write_cmd(c) lcd_write(c, LCD1602_RS_CMD)

#ifdef LCD1602_USE_RW
static unsigned char lcd_is_busy() {
    unsigned char busy = 1;
    delay_us(100);
    LCD1602_RS = LCD1602_RS_CMD;
    LCD1602_RW = LCD1602_RW_READ;
    LCD1602_EN = 1;
    delay_us(100);
    busy = LCD1602_D7;
    LCD1602_EN = 0;
    return busy;
}
#endif // LCD1602_USE_RW

static void lcd_write_nowait(unsigned char d, bit rs) {
    LCD1602_RS = rs;
#ifdef LCD1602_USE_RW
    LCD1602_RW = LCD1602_RW_WRITE;
#endif

#ifdef LCD1602_USE_4PIN
    // write high 4-bit data
    LCD1602_D7 = (d >> 7) & 0x01;
    LCD1602_D6 = (d >> 6) & 0x01;
    LCD1602_D5 = (d >> 5) & 0x01;
    LCD1602_D4 = (d >> 4) & 0x01;

    LCD1602_EN = 1;
    LCD1602_EN = 0;

    // write low 4-bit data
    LCD1602_D7 = (d >> 3) & 0x01;
    LCD1602_D6 = (d >> 2) & 0x01;
    LCD1602_D5 = (d >> 1) & 0x01;
    LCD1602_D4 = d & 0x01;
#else
    LCD1602_D0 = d & 0x01;
    LCD1602_D1 = (d >> 1) & 0x01;
    LCD1602_D2 = (d >> 2) & 0x01;
    LCD1602_D3 = (d >> 3) & 0x01;
    LCD1602_D4 = (d >> 4) & 0x01;
    LCD1602_D5 = (d >> 5) & 0x01;
    LCD1602_D6 = (d >> 6) & 0x01;
    LCD1602_D7 = (d >> 7) & 0x01;
#endif

    LCD1602_EN = 1;
    LCD1602_EN = 0;
}

static void lcd_write(unsigned char d, bit rs) {
#ifdef LCD1602_USE_RW
    while (lcd_is_busy())
        _nop_();
#else
    delay_ms(2);
#endif

    lcd_write_nowait(d, rs);
}

/**************************functions***********************/

void lcd1602_init() {
    // WARNING: this delay is not enough for some LCD/MCU,
    // you may need to increase it
    delay_ms(50);
    LCD1602_EN = 0;

    // The same command is sent three times, 
    // Function Set with 8-bit interface D7–D4 = binary 0011, 
    // the lower four bits are "don't care", using single enable pulses. 
    lcd_write_nowait(0x30, LCD1602_RS_CMD);
    delay_us(50);
    lcd_write_nowait(0x30, LCD1602_RS_CMD);
    delay_us(50);
    lcd_write_nowait(0x30, LCD1602_RS_CMD);
    delay_us(50);

#ifdef LCD1602_USE_4PIN
    // Function Set: 4-bit, 2-line, 5x8 dots
    lcd_write_nowait(0x28, LCD1602_RS_CMD);
#else
    // Function Set: 8-bit, 2-line, 5x8 dots
    lcd_write_nowait(0x38, LCD1602_RS_CMD);
#endif
    delay_us(50);

    // clear display
    lcd_write_nowait(0x01, LCD1602_RS_CMD);
    delay_us(50);
    // display on, cursor off, blink off
    lcd_write_nowait(0x0c, LCD1602_RS_CMD);
    delay_us(50);
    // entry mode, auto increment with no shift
    lcd_write_nowait(0x06, LCD1602_RS_CMD);
    delay_us(50);
    // cursor home
    lcd_write_nowait(0x02, LCD1602_RS_CMD);
    delay_us(50);
}

void lcd1602_clear() {
    lcd_write_cmd(0x01);
}

// set cursor position, x: 0~15, y: 0~1
void lcd1602_set_cursor_pos(unsigned char x, unsigned char y) {
    unsigned char addr = 0x80;
    if (y == 1)
        addr += 0x40;
    addr += x & 0x0f;
    lcd_write_cmd(addr);
}

void lcd1602_show_cursor(bit show) {
    if (show)
        lcd_write_cmd(0x0e);
    else
        lcd_write_cmd(0x0c);
}

void lcd1602_putc(char c) {
    lcd_write_data(c);
}

// display one digit, range 0~9
void lcd1602_putd(unsigned char d) {
    lcd_write_data(d + '0');
}

// display two chars for hex, range 0x00~0xff
void lcd1602_puth(unsigned char h) {
    lcd_write_data((h >> 4) + (h >> 4) > 9 ? 'A' + (h >> 4) - 10 : '0' + (h >> 4));
    lcd_write_data((h & 0x0f) > 9 ? 'A' + (h & 0x0f) - 10 : '0' + (h & 0x0f));
}

// display null-terminated string
void lcd1602_puts(const char* s) {
    while (*s)
        lcd_write_data(*s++);
}

// shift 1 char to left/right, dir='R' for right, 'L' for left
void lcd1602_shift(char dir) {
    lcd_write_cmd(dir == 'R' ? 0x1c : 0x18);
}
