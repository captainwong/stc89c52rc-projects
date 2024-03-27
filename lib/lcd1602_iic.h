/**
 * @file lcd1602_iic.h
 * @author captainwong (qtk.jack#gmail.com)
 * @brief A library for LCD1602 with IIC interface
 * @note This implementation is inspired by the Arduino LiquidCrystal_I2C library
 * @note The LCD1602 module has a PCF8574T chip on board, which is an IIC to 8-bit parallel port expander
 * @version 0.1
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __LCD1602_IIC_H__
#define __LCD1602_IIC_H__

#include "delay.h"
#include "hd44780.h"
#include "iic.h"

/*
 * Pin configurations:
 * PCF8547T P7 P6 P5 P4 P3 P2 P1 P0
 * LCD1602  D7 D6 D5 D4 K  EN RW RS
 * P3 is connected to a 8050 NPN transistor to control the backlight with a 4.7K ohm pull-up resistor
 * P3=1, the backlight is on, P3=0, the backlight is off
 */
#define LCD1602_IIC_RS_BIT 0x01  // Enable bit, 1 for data, 0 for command
#define LCD1602_IIC_RW_BIT 0x02  // Read/Write bit, 1 for read, 0 for write
#define LCD1602_IIC_EN_BIT 0x04  // Enable bit, 1 for enable, 0 for disable

// flags for backlight control, note that P3 is used to control the backlight
#define LCD1602_IIC_BACKLIGHT 0x08
#define LCD1602_IIC_NO_BACKLIGHT 0x00

typedef struct {
    // The following are public parts, should be initialized before calling `lcd1602_iic_init`
    iic_t* iic;         // The initialized IIC instance
    uint8_t addr;       // The IIC address of the LCD1602, e.g. 0x27 for PCF8574T
    uint8_t backlight;  // The backlight status
    // The following are private parts, should not be accessed directly
    uint8_t display_function;  // The display function
    uint8_t display_control;   // The display control
    uint8_t display_mode;      // The display mode
} lcd1602_iic_t;

/**
 * @brief Initialize the LCD1602 with IIC
 *
 * @param lcd The LCD1602 with IIC instance
 * @param lcd.iic The initialized IIC instance
 * @param lcd.addr The IIC address of the LCD1602, e.g. `0x27` for `PCF8574T`
 * @param lcd.backlight The backlight status, e.g. `LCD1602_IIC_BACKLIGHT`, `LCD1602_IIC_NO_BACKLIGHT`
 * @param lines The number of lines, e.g. `HD44780_1LINE`, `HD44780_2LINE`
 * @param dotsize The size of the characters, e.g. `HD44780_5x8_DOTS`, `HD44780_5x10_DOTS`
 * @note Don't touch other fields of the `lcd1602_iic_t struct`,
 *      they are ignored and will be initialized automatically
 */
void lcd1602_iic_init(lcd1602_iic_t* lcd, uint8_t lines, uint8_t dotsize);
// clear the display
void lcd1602_iic_clear(lcd1602_iic_t* lcd);
// return cursor to home position
void lcd1602_iic_home(lcd1602_iic_t* lcd);
// turn the display off (quickly)
void lcd1602_iic_no_display(lcd1602_iic_t* lcd);
// turn the display on (quickly)
void lcd1602_iic_display(lcd1602_iic_t* lcd);
// turn the blinking cursor off
void lcd1602_iic_no_blink(lcd1602_iic_t* lcd);
// turn the blinking cursor on
void lcd1602_iic_blink(lcd1602_iic_t* lcd);
// turn the cursor off
void lcd1602_iic_no_cursor(lcd1602_iic_t* lcd);
// turn the cursor on
void lcd1602_iic_cursor(lcd1602_iic_t* lcd);
// scroll the display left without changing the RAM
void lcd1602_iic_scroll_display_left(lcd1602_iic_t* lcd);
// scroll the display right without changing the RAM
void lcd1602_iic_scroll_display_right(lcd1602_iic_t* lcd);
// text flows Left to Right
void lcd1602_iic_left_to_right(lcd1602_iic_t* lcd);
// text flows Right to Left
void lcd1602_iic_right_to_left(lcd1602_iic_t* lcd);
// turn the backlight off
void lcd1602_iic_no_backlight(lcd1602_iic_t* lcd);
// turn the backlight on
void lcd1602_iic_backlight(lcd1602_iic_t* lcd);
// 'left justify' text from the cursor
void lcd1602_iic_no_autoscroll(lcd1602_iic_t* lcd);
// 'right justify' text from the cursor
void lcd1602_iic_autoscroll(lcd1602_iic_t* lcd);
// set cursor position, x: 0~15, y: 0~1
void lcd1602_iic_set_cursor(lcd1602_iic_t* lcd, uint8_t x, uint8_t y);
// display a char
void lcd1602_iic_putc(lcd1602_iic_t* lcd, char c);
// display one digit, range 0~9
void lcd1602_iic_putd(lcd1602_iic_t* lcd, uint8_t d);
// display two chars for hex, range 0x00~0xff
void lcd1602_iic_puth(lcd1602_iic_t* lcd, uint8_t h);
// display null-terminated string
void lcd1602_iic_puts(lcd1602_iic_t* lcd, char* s);

/**
 * @brief Create a custom character (glyph) for use on the LCD
 *
 * @param lcd The LCD1602 with IIC instance
 * @param location The location where the character is stored, 0~7
 * @param charmap The data for the custom character, 8 bytes
 */
void lcd1602_iic_create_char(lcd1602_iic_t* lcd, uint8_t location, uint8_t charmap[]);

/******************** Low-Level write functions *******************/

/**
 * @brief Write a command to the LCD1602 through IIC
 *
 * @note This is the low-level function to write a command to the LCD1602 through IIC
 * @param lcd The LCD1602 with IIC instance
 * @param cmd The command to write
 */
void lcd1602_iic_write_cmd(lcd1602_iic_t* lcd, uint8_t cmd);

/**
 * @brief Write a data to the LCD1602 through IIC
 *
 * @note This is the low-level function to write a data to the LCD1602 through IIC
 * @param lcd The LCD1602 with IIC instance
 * @param dat The data to write
 */
void lcd1602_iic_write_data(lcd1602_iic_t* lcd, uint8_t dat);

#endif
