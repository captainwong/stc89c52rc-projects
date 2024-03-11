#include <intrins.h>

#include "../stc89.h"
#include "delay.h"
#include "lcd1602.h"

void main(void) {
    // display cad_table's content on P1
    unsigned int i = 0, j = 0;
    P0 = P1 = P2 = P3 = 0xff;
    delay_ms(255);
    delay_ms(255);
    lcd1602_init();
    
    while (1) {
        lcd1602_clear();
        lcd1602_set_cursor_pos(0, 0);
        lcd1602_puts("Hello, Bilibili!");
        lcd1602_set_cursor_pos(0, 1);
        lcd1602_puts("Hello, Zhihu!");
        delay_ms(255);
        delay_ms(255);
        delay_ms(255);
        delay_ms(255);
        
        lcd1602_clear();
        lcd1602_set_cursor_pos(0, 0);
        lcd1602_puts("Hello, world!");
        lcd1602_set_cursor_pos(0, 1);
        lcd1602_puts("Hello, world!2");
        delay_ms(255);
        delay_ms(255);
        delay_ms(255);
        delay_ms(255);
    }
}