#include <intrins.h>

#include "../lib/stc89.h"
#include "../lib/delay.h"
#include "../lib/lcd1602.h"

void main(void) {
    delay_ms(1000);
    lcd1602_init();

    while (1) {
        lcd1602_clear();
        lcd1602_set_cursor_pos(0, 0);
        lcd1602_puts("Hello, Bilibili!");
        lcd1602_set_cursor_pos(0, 1);
        lcd1602_puts("Hello, Zhihu!");
        delay_ms(3000);

        lcd1602_clear();
        lcd1602_set_cursor_pos(0, 0);
        lcd1602_puts("Hello, world!");
        lcd1602_set_cursor_pos(0, 1);
        lcd1602_puts("Hello, world!2");
        delay_ms(3000);
    }
}