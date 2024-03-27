/**
 * @file lcd1602.h
 * @author captainwong (qtk.jack#gmail.com)
 * @brief A library for LCD1602
 * @note This implementation is inspired by the Arduino LiquidCrystal library
 * @version 0.1
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LCD_1602_H__
#define __LCD_1602_H__

#include "delay.h"
#include "hd44780.h"
#include "types.h"

typedef struct {
    // The following are public parts, should be initialized before calling `lcd1602_init`
    uint8_t lines;                 // The number of lines, e.g. HD44780_1LINE, HD44780_2LINE
    void (*write_rw)(uint8_t rw);  // if NULL, use delay instead of busy flag
    void (*write_rs)(uint8_t rs);  // must not be NULL
    void (*write_en)(uint8_t en);  // must not be NULL
    union {
        // this struct is used for 4-bit mode,
        // the low 4 bits of dat or return value is valid
        struct {
            void (*write_4bits)(uint8_t dat);
            uint8_t (*read_4bits)();
        } lcd4bit;

        // this struct is used for 8-bit mode
        struct {
            void (*write_8bits)(uint8_t dat);
            uint8_t (*read_8bits)();
        } lcd8bit;
    } io;  // choose one of the two structs
    // The following are private parts, should not be accessed directly
    uint8_t display_function;  // The display function
    uint8_t display_control;   // The display control
    uint8_t display_mode;      // The display mode
} lcd1602_t;

/**
 * @brief Initialize the LCD1602
 *
 * @param lcd The LCD1602 instance
 * @param lcd.write_rw The function to write the RW pin, if NULL, use delay instead of busy flag
 * @param lcd.write_rs The function to write the RS pin, must not be NULL
 * @param lcd.write_en The function to write the EN pin, must not be NULL
 * @param lcd.io The IO functions, choose one of the two structs and assign the functions
 * @param four_bit_mode 1 for 4-bit mode, 0 for 8-bit mode
 * @param lines The number of lines, e.g. HD44780_1LINE, HD44780_2LINE
 * @param dotsize The size of the characters, e.g. HD44780_5x8_DOTS, HD44780_5x10_DOTS
 * @note Don't touch other fields of the `lcd1602_t struct`,
 *      they are ignored and will be initialized automatically
 */
void lcd1602_init(lcd1602_t* lcd, bit four_bit_mode, uint8_t lines, uint8_t dotsize);
// clear the display
void lcd1602_clear(lcd1602_t* lcd);
// return cursor to home position
void lcd1602_home(lcd1602_t* lcd);
// turn the display off (quickly)
void lcd1602_no_display(lcd1602_t* lcd);
// turn the display on (quickly)
void lcd1602_display(lcd1602_t* lcd);
// turn the blinking cursor off
void lcd1602_no_blink(lcd1602_t* lcd);
// turn the blinking cursor on
void lcd1602_blink(lcd1602_t* lcd);
// turn the cursor off
void lcd1602_no_cursor(lcd1602_t* lcd);
// turn the cursor on
void lcd1602_cursor(lcd1602_t* lcd);
// scroll the display left without changing the RAM
void lcd1602_scroll_display_left(lcd1602_t* lcd);
// scroll the display right without changing the RAM
void lcd1602_scroll_display_right(lcd1602_t* lcd);
// text flows Left to Right
void lcd1602_left_to_right(lcd1602_t* lcd);
// text flows Right to Left
void lcd1602_right_to_left(lcd1602_t* lcd);
// 'left justify' text from the cursor
void lcd1602_no_autoscroll(lcd1602_t* lcd);
// 'right justify' text from the cursor
void lcd1602_autoscroll(lcd1602_t* lcd);
// set cursor position, x: 0~15, y: 0~1
void lcd1602_set_cursor(lcd1602_t* lcd, uint8_t x, uint8_t y);
// display a char
void lcd1602_putc(lcd1602_t* lcd, char c);
// display one digit, range 0~9
void lcd1602_putd(lcd1602_t* lcd, uint8_t d);
// display two chars for hex, range 0x00~0xff
void lcd1602_puth(lcd1602_t* lcd, uint8_t h);
// display null-terminated string
void lcd1602_puts(lcd1602_t* lcd, char* s);

/**
 * @brief Create a custom character (glyph) for use on the LCD
 *
 * @param lcd The LCD1602 instance
 * @param location The location where the character is stored, 0~7
 * @param charmap The data for the custom character, 8 bytes
 */
void lcd1602_create_char(lcd1602_t* lcd, uint8_t location, uint8_t charmap[]);

/******************** Low-Level write functions *******************/

/**
 * @brief Write a command to the LCD1602
 *
 * @note This is the low-level function to write a command to the LCD1602
 * @param lcd The LCD1602 instance
 * @param cmd The command to write
 */
void lcd1602_write_cmd(lcd1602_t* lcd, uint8_t cmd);

/**
 * @brief Write a data to the LCD1602
 *
 * @note This is the low-level function to write a data to the LCD1602
 * @param lcd The LCD1602 instance
 * @param dat The data to write
 */
void lcd1602_write_data(lcd1602_t* lcd, uint8_t dat);

#endif
