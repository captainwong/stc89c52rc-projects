#include "lcd1602.h"

/**************************local helpers*******************/

#define LCD1602_RS_CMD 0
#define LCD1602_RS_DATA 1
#define LCD1602_RW_WRITE 0
#define LCD1602_RW_READ 1

// pulse enable pin
static void lcd_pulse_enable(lcd1602_t* lcd) {
    lcd->write_en(0);
    delay_us(1);
    lcd->write_en(1);
    delay_us(1);  // enable pulse must be >450ns
    lcd->write_en(0);
    delay_us(50);  // commands need > 37us to settle
}

static void lcd_write_4bits(lcd1602_t* lcd, uint8_t value) {
    lcd->io.lcd4bit.write_4bits(value);
    lcd_pulse_enable(lcd);
}

static void lcd_write_8bits(lcd1602_t* lcd, uint8_t value) {
    lcd->io.lcd8bit.write_8bits(value);
    lcd_pulse_enable(lcd);
}

static void lcd_write(lcd1602_t* lcd, uint8_t d, bit rs) {
    lcd->write_rs(rs);
    if (lcd->write_rw) {
        lcd->write_rw(LCD1602_RW_WRITE);
    }

    if (lcd->display_mode & HD44780_8BIT_MODE) {
        lcd_write_8bits(lcd, d);
    } else {
        lcd_write_4bits(lcd, d >> 4);
        lcd_write_4bits(lcd, d);
    }
}

static void lcd_wait_ready(lcd1602_t* lcd) {
    if (lcd->write_rw) {
        uint8_t busy = 1;
        do {
            lcd->write_rs(LCD1602_RS_CMD);
            lcd->write_rw(LCD1602_RW_READ);
            lcd->write_en(1);
            if (lcd->display_mode & HD44780_8BIT_MODE) {
                busy = lcd->io.lcd8bit.read_8bits() & 0x80;
            } else {
                busy = lcd->io.lcd4bit.read_4bits() & 0x08;
            }
            lcd->write_en(0);
        } while (busy);
    } else {
        delay_ms(2);
    }
}

/**************************public functions***********************/

void lcd1602_write_cmd(lcd1602_t* lcd, uint8_t cmd) {
    lcd_wait_ready(lcd);
    lcd_write(lcd, cmd, LCD1602_RS_CMD);
}

void lcd1602_write_data(lcd1602_t* lcd, uint8_t dat) {
    lcd_wait_ready(lcd);
    lcd_write(lcd, dat, LCD1602_RS_DATA);
}

// When the display powers up, it is configured as follows:
//
// 1. Display clear
// 2. Function set:
//    DL = 1; 8-bit interface data
//    N = 0; 1-line display
//    F = 0; 5x8 dot character font
// 3. Display on/off control:
//    D = 0; Display off
//    C = 0; Cursor off
//    B = 0; Blinking off
// 4. Entry mode set:
//    I/D = 1; Increment by 1
//    S = 0; No shift
//
// Note, however, that resetting the board doesn't reset the LCD, so we
// can't assume that its in that state when a sketch starts (and the
// lcd1602_init is called).

void lcd1602_init(lcd1602_t* lcd, bit four_bit_mode, uint8_t lines, uint8_t dotsize) {
    if (four_bit_mode) {
        lcd->display_function = HD44780_4BIT_MODE | HD44780_1LINE | HD44780_5x8_DOTS;
    } else {
        lcd->display_function = HD44780_8BIT_MODE | HD44780_1LINE | HD44780_5x8_DOTS;
    }

    if (lines == HD44780_2LINE) {
        lcd->display_function |= HD44780_2LINE;
    }

    // for some 1 line displays you can select a 10 pixel high font
    if (dotsize == HD44780_5x10_DOTS && lines == HD44780_1LINE) {
        lcd->display_function |= HD44780_5x10_DOTS;
    }

    // SEE PAGE 45/46 FOR INITIALIZATION SPECIFICATION!
    // according to datasheet, we need at least 40ms after power rises above 2.7V
    // before sending commands.
    delay_ms(50);

    // Now we pull both RS and R/W low to begin commands
    lcd->write_en(0);
    lcd->write_rs(0);
    if (lcd->write_rw) {
        lcd->write_rw(0);
    }

    // put the LCD into 4 bit or 8 bit mode
    if (four_bit_mode) {
        // this is according to the hitachi HD44780 datasheet
        // figure 24, pg 46

        // we start in 8bit mode, try to set 4 bit mode
        lcd_write_4bits(lcd, 0x03);
        delay_ms(5);  // wait min 4.1ms

        // second try
        lcd_write_4bits(lcd, 0x03);
        delay_ms(5);  // wait min 4.1ms

        // third go!
        lcd_write_4bits(lcd, 0x03);
        delay_ms(1);

        // finally, set to 4-bit interface
        lcd_write_4bits(lcd, 0x02);

    } else {
        // this is according to the hitachi HD44780 datasheet
        // page 45 figure 23

        // Send function set command sequence
        lcd1602_write_cmd(lcd, HD44780_FUNCTION_SET | lcd->display_function);
        delay_ms(5);  // wait more than 4.1ms

        // second try
        lcd1602_write_cmd(lcd, HD44780_FUNCTION_SET | lcd->display_function);
        delay_ms(1);

        // third go
        lcd1602_write_cmd(lcd, HD44780_FUNCTION_SET | lcd->display_function);
    }

    // finally, set # lines, font size, etc.
    lcd1602_write_cmd(lcd, HD44780_FUNCTION_SET | lcd->display_function);

    // turn the display on with no cursor or blinking default
    lcd->display_control = HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_BLINK_OFF;
    lcd1602_display(lcd);

    // clear it off
    lcd1602_clear(lcd);

    // Initialize to default text direction (for roman languages)
    lcd->display_mode = HD44780_ENTRY_LEFT | HD44780_ENTRY_SHIFT_DECREMENT;
    // set the entry mode
    lcd1602_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

void lcd1602_clear(lcd1602_t* lcd) {
    lcd1602_write_cmd(lcd, HD44780_CLEAR_DISPLAY);
    delay_ms(2);  // this command takes a long time!
}

void lcd1602_home(lcd1602_t* lcd) {
    lcd1602_write_cmd(lcd, HD44780_RETURN_HOME);
    delay_ms(2);  // this command takes a long time!
}

void lcd1602_no_display(lcd1602_t* lcd) {
    lcd->display_control &= ~HD44780_DISPLAY_ON;
    lcd1602_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

void lcd1602_display(lcd1602_t* lcd) {
    lcd->display_control |= HD44780_DISPLAY_ON;
    lcd1602_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

void lcd1602_no_blink(lcd1602_t* lcd) {
    lcd->display_control &= ~HD44780_BLINK_ON;
    lcd1602_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

void lcd1602_blink(lcd1602_t* lcd) {
    lcd->display_control |= HD44780_BLINK_ON;
    lcd1602_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

void lcd1602_no_cursor(lcd1602_t* lcd) {
    lcd->display_control &= ~HD44780_CURSOR_ON;
    lcd1602_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

void lcd1602_cursor(lcd1602_t* lcd) {
    lcd->display_control |= HD44780_CURSOR_ON;
    lcd1602_write_cmd(lcd, HD44780_DISPLAY_CONTROL | lcd->display_control);
}

void lcd1602_scroll_display_left(lcd1602_t* lcd) {
    lcd1602_write_cmd(lcd, HD44780_CURSOR_SHIFT | HD44780_DISPLAY_MOVE | HD44780_MOVE_LEFT);
}

void lcd1602_scroll_display_right(lcd1602_t* lcd) {
    lcd1602_write_cmd(lcd, HD44780_CURSOR_SHIFT | HD44780_DISPLAY_MOVE | HD44780_MOVE_RIGHT);
}

void lcd1602_left_to_right(lcd1602_t* lcd) {
    lcd->display_mode |= HD44780_ENTRY_LEFT;
    lcd1602_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

void lcd1602_right_to_left(lcd1602_t* lcd) {
    lcd->display_mode &= ~HD44780_ENTRY_LEFT;
    lcd1602_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

void lcd1602_autoscroll(lcd1602_t* lcd) {
    lcd->display_mode |= HD44780_ENTRY_SHIFT_INCREMENT;
    lcd1602_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

void lcd1602_no_autoscroll(lcd1602_t* lcd) {
    lcd->display_mode &= ~HD44780_ENTRY_SHIFT_INCREMENT;
    lcd1602_write_cmd(lcd, HD44780_ENTRY_MODE_SET | lcd->display_mode);
}

void lcd1602_set_cursor(lcd1602_t* lcd, uint8_t x, uint8_t y) {
    uint8_t addr = 0x80 + (x & 0x0f);
    if (y == 1 && lcd->display_function & HD44780_2LINE)
        addr += 0x40;
    lcd1602_write_cmd(lcd, addr);
}

void lcd1602_putc(lcd1602_t* lcd, char c) {
    lcd1602_write_data(lcd, c);
}

void lcd1602_putd(lcd1602_t* lcd, uint8_t d) {
    lcd1602_write_data(lcd, d + '0');
}

void lcd1602_puth(lcd1602_t* lcd, uint8_t h) {
    uint8_t hi = h >> 4;
    uint8_t lo = h & 0x0f;
    lcd1602_write_data(lcd, hi > 9 ? 'A' - 10 + hi : '0' + hi);
    lcd1602_write_data(lcd, lo > 9 ? 'A' - 10 + lo : '0' + lo);
}

void lcd1602_puts(lcd1602_t* lcd, char* s) {
    while (*s) {
        lcd1602_write_data(lcd, *s++);
    }
}

void lcd1602_create_char(lcd1602_t* lcd, uint8_t location, uint8_t charmap[]) {
    uint8_t i;
    location &= 0x7;  // we only have 8 locations 0-7
    lcd1602_write_cmd(lcd, HD44780_SET_CGRAM_ADDR | (location << 3));
    for (i = 0; i < 8; i++) {
        lcd1602_write_data(lcd, charmap[i]);
    }
}
