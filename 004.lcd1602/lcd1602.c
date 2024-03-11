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
    unsigned char res;
    LCD1602_RS = LCD1602_RS_CMD;
    LCD1602_RW = LCD1602_RW_READ;
    LCD1602_EN = 1;
    delay_5us();
    res = LCD1602_D0;
    LCD1602_EN = 0;
    return res;
}
#endif  // LCD1602_USE_RW

static void lcd_write(unsigned char d, bit rs) {
#ifdef LCD1602_USE_RW
    while (lcd_is_busy())
        _nop_();
    LCD1602_RW = LCD1602_RW_WRITE;
#else
    delay_ms(2);
#endif

    LCD1602_RS = rs;
    LCD1602_EN = 1;

#ifdef LCD1602_USE_4PIN
    // write high 4-bit data
    LCD1602_D4 = (d >> 4) & 0x01;
    LCD1602_D5 = (d >> 5) & 0x01;
    LCD1602_D6 = (d >> 6) & 0x01;
    LCD1602_D7 = (d >> 7) & 0x01;
    delay_2us();
    LCD1602_EN = 1;
    delay_1us();
    LCD1602_EN = 0;

    // write low 4-bit data
    LCD1602_D0 = d & 0x01;
    LCD1602_D1 = (d >> 1) & 0x01;
    LCD1602_D2 = (d >> 2) & 0x01;
    LCD1602_D3 = (d >> 3) & 0x01;
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

    delay_2us();
    LCD1602_EN = 1;
    delay_1us();
    LCD1602_EN = 0;
}

/**************************functions***********************/

void lcd1602_init() {
    delay_ms(5);
#ifdef LCD1602_USE_4PIN
    lcd_write_cmd(0x28);
#else
    lcd_write_cmd(0x38);
#endif

    // display on, cursor off, blink off
    lcd_write_cmd(0x0c);
}

void lcd1602_clear() {
    lcd_write_cmd(0x01);
}

void lcd1602_set_cursor_pos(unsigned char x, unsigned char y) {
    unsigned char addr = 0x80;
    if (y == 1)
        addr += 0x40;
    addr += x;
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
