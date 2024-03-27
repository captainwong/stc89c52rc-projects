#ifndef __LCD1602_IIC_H__
#define __LCD1602_IIC_H__

#include "../lib/iic.h"
#include "../lib/delay.h"

/* 
 * Pin configurations:
 * PCF8547T  P0 P1 P2 P3 P4 P5 P6 P7
 * LCD1602   RS RW EN K  D4 D5 D6 D7
 * P3 is connected to a 8050 NPN transistor to control the backlight with a 4.7K ohm pull-up resistor
 * P3=1, the backlight is on, P3=0, the backlight is off
 */

#define LCD1602_IIC_RETRY_TIMES 10

typedef struct {
    iic_t* iic;    // The initialized IIC instance
    uint8_t addr;  // The IIC address of the LCD1602
} lcd1602_iic_t;

// initialize the LCD1602 with IIC
void lcd1602_iic_init(const lcd1602_iic_t* lcd);
// clear the display
void lcd1602_iic_clear(const lcd1602_iic_t* lcd);
// set cursor position, x: 0~15, y: 0~1
void lcd1602_iic_set_cursor_pos(const lcd1602_iic_t* lcd, uint8_t x, uint8_t y);
void lcd1602_iic_show_cursor(const lcd1602_iic_t* lcd, bit show);
void lcd1602_iic_putc(const lcd1602_iic_t* lcd, char c);
// display one digit, range 0~9
void lcd1602_iic_putd(const lcd1602_iic_t* lcd, uint8_t d);
// display two chars for hex, range 0x00~0xff
void lcd1602_iic_puth(const lcd1602_iic_t* lcd, uint8_t h);
// display null-terminated string
void lcd1602_iic_puts(const lcd1602_iic_t* lcd, const char* s);
// shift 1 char to left/right, dir='R' for right, 'L' for left
void lcd1602_iic_shift(const lcd1602_iic_t* lcd, char dir);


/******************** Low-Level write functions *******************/

/**
 * @brief Write a byte to the LCD1602 through IIC
 * 
 * @note This is the low-level function to write a byte to the LCD1602 through IIC
 * @param lcd The LCD1602 with IIC instance
 * @param dat The byte to write
 * @return bit IIC_ACK if success, IIC_NACK if failed
 */
bit lcd1602_iic_write_byte(const lcd1602_iic_t* lcd, uint8_t dat);

/**
 * @brief Write a command to the LCD1602 through IIC
 * 
 * @note This is the low-level function to write a command to the LCD1602 through IIC
 * @param lcd The LCD1602 with IIC instance
 * @param cmd The command to write
 * @return bit IIC_ACK if success, IIC_NACK if failed
 */
bit lcd1602_iic_write_cmd(const lcd1602_iic_t* lcd, uint8_t cmd);

/**
 * @brief Write a data to the LCD1602 through IIC
 * 
 * @note This is the low-level function to write a data to the LCD1602 through IIC
 * @param lcd The LCD1602 with IIC instance
 * @param dat The data to write
 * @return bit IIC_ACK if success, IIC_NACK if failed
 */
bit lcd1602_iic_write_data(const lcd1602_iic_t* lcd, uint8_t dat);

#endif
