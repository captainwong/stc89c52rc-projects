#ifndef __LCD_1602_H__
#define __LCD_1602_H__

#include "../lib/delay.h"
#include "../lib/types.h"

#define LCD1602_MODE_4BIT 0
#define LCD1602_MODE_8BIT 1

typedef struct {
    uint8_t mode;  // 0 for 4-bit mode, 1 for 8-bit mode
    void (*write_rw)(uint8_t rw); // if NULL, use delay instead of busy flag
    void (*write_rs)(uint8_t rs); // must not be NULL
    void (*write_en)(uint8_t en); // must not be NULL
    union {
        // this struct is used for 4-bit mode,
        // the low 4 bits of dat or return value is valid
        struct {
            void (*write_4bit)(uint8_t dat);
            uint8_t (*read_4bit)();
        } lcd4bit;

        // this struct is used for 8-bit mode
        struct {
            void (*write_byte)(uint8_t dat);
            uint8_t (*read_byte)();
        } lcd8bit;
    } io; // choose one of the two structs
} lcd1602_t;

void lcd1602_init(const lcd1602_t* lcd);
void lcd1602_clear(const lcd1602_t* lcd);
// set cursor position, x: 0~15, y: 0~1
void lcd1602_set_cursor_pos(const lcd1602_t* lcd, uint8_t x, uint8_t y);
void lcd1602_show_cursor(const lcd1602_t* lcd, bit show);
void lcd1602_putc(const lcd1602_t* lcd, char c);
// display one digit, range 0~9
void lcd1602_putd(const lcd1602_t* lcd, uint8_t d);
// display two chars for hex, range 0x00~0xff
void lcd1602_puth(const lcd1602_t* lcd, uint8_t h);
// display null-terminated string
void lcd1602_puts(const lcd1602_t* lcd, const char* s);
// shift 1 char to left/right, dir='R' for right, 'L' for left
void lcd1602_shift(const lcd1602_t* lcd, char dir);

#endif
