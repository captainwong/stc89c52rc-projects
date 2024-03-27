/**
 * @file hd44780.h
 * @author captainwong (qtk.jack#gmail.com)
 * @brief Definitions of the HD44780 LCD controller
 * @version 0.1
 * @date 2024-03-27
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef __HD44780_H__
#define __HD44780_H__

// commands
#define HD44780_CLEAR_DISPLAY 0x01
#define HD44780_RETURN_HOME 0x02
#define HD44780_ENTRY_MODE_SET 0x04
#define HD44780_DISPLAY_CONTROL 0x08
#define HD44780_CURSOR_SHIFT 0x10
#define HD44780_FUNCTION_SET 0x20
#define HD44780_SET_CGRAM_ADDR 0x40
#define HD44780_SET_DDRAM_ADDR 0x80

// flags for display entry mode
#define HD44780_ENTRY_RIGHT 0x00
#define HD44780_ENTRY_LEFT 0x02
#define HD44780_ENTRY_SHIFT_INCREMENT 0x01
#define HD44780_ENTRY_SHIFT_DECREMENT 0x00

// flags for display on/off control
#define HD44780_DISPLAY_ON 0x04
#define HD44780_DISPLAY_OFF 0x00
#define HD44780_CURSOR_ON 0x02
#define HD44780_CURSOR_OFF 0x00
#define HD44780_BLINK_ON 0x01
#define HD44780_BLINK_OFF 0x00

// flags for display/cursor shift
#define HD44780_DISPLAY_MOVE 0x08
#define HD44780_CURSOR_MOVE 0x00
#define HD44780_MOVE_RIGHT 0x04
#define HD44780_MOVE_LEFT 0x00

// flags for function set
#define HD44780_8BIT_MODE 0x10
#define HD44780_4BIT_MODE 0x00
#define HD44780_2LINE 0x08
#define HD44780_1LINE 0x00
#define HD44780_5x10_DOTS 0x04
#define HD44780_5x8_DOTS 0x00

#endif
