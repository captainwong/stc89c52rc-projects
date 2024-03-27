#include "lcd1602_iic.h"

/**************************local helpers*******************/

// write data to expander, with backlight on
static void expander_write(lcd1602_iic_t* lcd, uint8_t dat) {
    iic_start(lcd->iic);
    iic_write_byte(lcd->iic, lcd->addr << 1);
    iic_write_byte(lcd->iic, dat | lcd->backlight);
    iic_stop(lcd->iic);
}

// pulse enable pin
static void pulse_enable(lcd1602_iic_t* lcd, uint8_t dat) {
    expander_write(lcd, dat | LCD1602_IIC_EN_BIT);   // EN high
    delay_us(1);                                     // enable pulse must be >450ns
    expander_write(lcd, dat & ~LCD1602_IIC_EN_BIT);  // EN low
    delay_us(50);                                    // commands need > 37us to settle
}

// write 4 bits to expander
static void write_4bits(lcd1602_iic_t* lcd, uint8_t value) {
    expander_write(lcd, value);
    pulse_enable(lcd, value);
}

// write 8 bits to expander
static void write_byte(lcd1602_iic_t* lcd, uint8_t value, uint8_t mode) {
    uint8_t hi = value & 0xf0;
    uint8_t lo = (value << 4) & 0xf0;
    write_4bits(lcd, hi | mode);
    write_4bits(lcd, lo | mode);
}

/**************************public functions***********************/

void lcd1602_iic_write_cmd(lcd1602_iic_t* lcd, uint8_t cmd) {
    write_byte(lcd, cmd, 0);
}

void lcd1602_iic_write_data(lcd1602_iic_t* lcd, uint8_t dat) {
    write_byte(lcd, dat, LCD1602_IIC_RS_BIT);
}

void lcd1602_iic_init(lcd1602_iic_t* lcd) {
    uint8_t backlight = lcd->backlight;
    lcd->backlight = LCD1602_IIC_NO_BACKLIGHT;
    lcd->display_function = HD44780_4BIT_MODE | HD44780_2LINE | HD44780_5x8_DOTS;
    if (lcd->lines > 1) {
        lcd->lines = 1;
        lcd->display_function |= HD44780_2LINE;
    }

    // for some 1 line displays you can select a 10 pixel high font
    if (lcd->dotsize != HD44780_5x8_DOTS && lcd->lines == 1) {
        lcd->dotsize = HD44780_5x10_DOTS;
        lcd->display_function |= HD44780_5x10_DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands.
    delay_ms(50);

    // Now we pull both RS and R/W low to begin commands
    expander_write(lcd, LCD1602_IIC_NO_BACKLIGHT);  // reset expander and turn backlight off (Bit 8 =1)
    delay_ms(1000);

    // put the LCD into 4 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 24, pg 46

    // we start in 8bit mode, try to set 4 bit mode
    write_4bits(lcd, 0x03 << 4);
    delay_ms(5);  // wait min 4.1ms

    // second try
    write_4bits(lcd, 0x03 << 4);
    delay_ms(5);  // wait min 4.1ms

    // third go!
    write_4bits(lcd, 0x03 << 4);
    delay_ms(1);

    // finally, set to 4-bit interface
    write_4bits(lcd, 0x02 << 4);

    // set # lines, font size, etc.
    lcd1602_iic_write_cmd(lcd, HD44780_FUNCTION_SET | lcd->display_function);

    // turn the display on with no cursor or blinking default
    lcd->display_control = HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_BLINK_OFF;
    lcd1602_iic_display(lcd);

    // clear it off
    lcd1602_iic_clear(lcd);

    // Initialize to default text direction (for roman languages)
    lcd->display_mode = HD44780_ENTRY_LEFT | HD44780_ENTRY_SHIFT_DECREMENT;

    // set the entry mode
    lcd1602_iic_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);

    // set back the backlight
    lcd->backlight = backlight;

    lcd1602_iic_home(lcd);
}

// clear the display
void lcd1602_iic_clear(lcd1602_iic_t* lcd) {
    lcd1602_iic_write_cmd(lcd, HD44780_CLEAR_DISPLAY);
    delay_ms(2);  // this command takes a long time!
}

// return cursor to home position
void lcd1602_iic_home(lcd1602_iic_t* lcd) {
    lcd1602_iic_write_cmd(lcd, HD44780_RETURN_HOME);
    delay_ms(2);  // this command takes a long time!
}

// turn the display off (quickly)
void lcd1602_iic_no_display(lcd1602_iic_t* lcd) {
    lcd->display_control &= ~HD44780_DISPLAY_ON;
    lcd1602_iic_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

// turn the display on (quickly)
void lcd1602_iic_display(lcd1602_iic_t* lcd) {
    lcd->display_control |= HD44780_DISPLAY_ON;
    lcd1602_iic_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

// turn the blinking cursor off
void lcd1602_iic_no_blink(lcd1602_iic_t* lcd) {
    lcd->display_control &= ~HD44780_BLINK_ON;
    lcd1602_iic_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

// turn the blinking cursor on
void lcd1602_iic_blink(lcd1602_iic_t* lcd) {
    lcd->display_control |= HD44780_BLINK_ON;
    lcd1602_iic_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

// turn the cursor off
void lcd1602_iic_no_cursor(lcd1602_iic_t* lcd) {
    lcd->display_control &= ~HD44780_CURSOR_ON;
    lcd1602_iic_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

// turn the cursor on
void lcd1602_iic_cursor(lcd1602_iic_t* lcd) {
    lcd->display_control |= HD44780_CURSOR_ON;
    lcd1602_iic_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

// scroll the display left without changing the RAM
void lcd1602_iic_scroll_display_left(lcd1602_iic_t* lcd) {
    lcd1602_iic_write_cmd(lcd, HD44780_CURSOR_SHIFT | HD44780_DISPLAY_MOVE | HD44780_MOVE_LEFT);
}

// scroll the display right without changing the RAM
void lcd1602_iic_scroll_display_right(lcd1602_iic_t* lcd) {
    lcd1602_iic_write_cmd(lcd, HD44780_CURSOR_SHIFT | HD44780_DISPLAY_MOVE | HD44780_MOVE_RIGHT);
}

// text flows Left to Right
void lcd1602_iic_left_to_right(lcd1602_iic_t* lcd) {
    lcd->display_mode |= HD44780_ENTRY_LEFT;
    lcd1602_iic_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

// text flows Right to Left
void lcd1602_iic_right_to_left(lcd1602_iic_t* lcd) {
    lcd->display_mode &= ~HD44780_ENTRY_LEFT;
    lcd1602_iic_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

// turn the backlight off
void lcd1602_iic_no_backlight(lcd1602_iic_t* lcd) {
    lcd->backlight = LCD1602_IIC_NO_BACKLIGHT;
    expander_write(lcd, 0);
}

// turn the backlight on
void lcd1602_iic_backlight(lcd1602_iic_t* lcd) {
    lcd->backlight = LCD1602_IIC_BACKLIGHT;
    expander_write(lcd, 0);
}

// 'left justify' text from the cursor
void lcd1602_iic_no_autoscroll(lcd1602_iic_t* lcd) {
    lcd->display_mode &= ~HD44780_ENTRY_SHIFT_INCREMENT;
    lcd1602_iic_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

// 'right justify' text from the cursor
void lcd1602_iic_autoscroll(lcd1602_iic_t* lcd) {
    lcd->display_mode |= HD44780_ENTRY_SHIFT_INCREMENT;
    lcd1602_iic_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

// set cursor position, x: 0~15, y: 0~1
void lcd1602_iic_set_cursor(lcd1602_iic_t* lcd, uint8_t x, uint8_t y) {
    uint8_t addr = 0x80;
    if (y == 1)
        addr += 0x40;
    addr += x & 0x0f;
    lcd1602_iic_write_cmd(lcd, addr);
}

// display a char
void lcd1602_iic_putc(lcd1602_iic_t* lcd, char c) {
    lcd1602_iic_write_data(lcd, c);
}

// display one digit, range 0~9
void lcd1602_iic_putd(lcd1602_iic_t* lcd, uint8_t d) {
    lcd1602_iic_write_data(lcd, d + '0');
}

// display two chars for hex, range 0x00~0xff
void lcd1602_iic_puth(lcd1602_iic_t* lcd, uint8_t h) {
    uint8_t hi = h >> 4;
    uint8_t lo = h & 0x0f;
    lcd1602_iic_write_data(lcd, hi > 9 ? 'A' - 10 + hi : '0' + hi);
    lcd1602_iic_write_data(lcd, lo > 9 ? 'A' - 10 + lo : '0' + lo);
}

// display null-terminated string
void lcd1602_iic_puts(lcd1602_iic_t* lcd, char* s) {
    while (*s)
        lcd1602_iic_write_data(lcd, *s++);
}

/**
 * @brief Create a custom character (glyph) for use on the LCD
 *
 * @param lcd The LCD1602 with IIC instance
 * @param location The location where the character is stored, 0~7
 * @param charmap The data for the custom character, 8 bytes
 */
void lcd1602_iic_create_char(lcd1602_iic_t* lcd, uint8_t location, uint8_t charmap[]) {
    uint8_t i;
    location &= 0x7;  // we only have 8 locations 0-7
    lcd1602_iic_write_cmd(lcd, HD44780_SET_CGRAM_ADDR | (location << 3));
    for (i = 0; i < 8; i++)
        lcd1602_iic_write_data(lcd, charmap[i]);
}
