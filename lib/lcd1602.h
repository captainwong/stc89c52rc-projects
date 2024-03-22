#ifndef __LCD_1602_H__
#define __LCD_1602_H__

#include "stc89.h"
#include "types.h"

/**************************config*************************/

/* uncomment line below to enable RW
 * otherwise RW is tied to GND
 */
#define LCD1602_USE_RW

/* uncomment line below to enable 4-pin mode */
// #define LCD1602_USE_4PIN

/* change PINs if you need
 * default PINs:
 * P2.7 as RS
 * P2.6 as EN
 * P0 as D0-D7
 */
sbit LCD1602_RS = P2 ^ 7;
sbit LCD1602_EN = P2 ^ 6;

#ifdef LCD1602_USE_RW
sbit LCD1602_RW = P2 ^ 5;
#endif

#ifndef LCD1602_USE_4PIN
sbit LCD1602_D0 = P0 ^ 0;
sbit LCD1602_D1 = P0 ^ 1;
sbit LCD1602_D2 = P0 ^ 2;
sbit LCD1602_D3 = P0 ^ 3;
#endif

sbit LCD1602_D4 = P0 ^ 4;
sbit LCD1602_D5 = P0 ^ 5;
sbit LCD1602_D6 = P0 ^ 6;
sbit LCD1602_D7 = P0 ^ 7;

/**************************config end**********************/

/**************************functions***********************/
void lcd1602_init();
void lcd1602_clear();
// set cursor position, x: 0~15, y: 0~1
void lcd1602_set_cursor_pos(unsigned char x, unsigned char y);
void lcd1602_show_cursor(bit show);
void lcd1602_putc(char c);
// display one digit, range 0~9
void lcd1602_putd(unsigned char d);
// display two chars for hex, range 0x00~0xff
void lcd1602_puth(unsigned char h);
// display null-terminated string
void lcd1602_puts(const char* s);
// shift 1 char to left/right, dir='R' for right, 'L' for left
void lcd1602_shift(char dir);

#endif
