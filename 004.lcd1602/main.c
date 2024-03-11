#include <intrins.h>

#include "../stc89.h"
#include "delay.h"
#include "lcd1602.h"

void main(void) {
    // display cad_table's content on P1
    unsigned int i = 0, j = 0;
    lcd1602_init();
    lcd1602_set_cursor_pos(0, 0);
    lcd1602_puts("Hello, world!");
    lcd1602_set_cursor_pos(0, 1);
    lcd1602_puts("Hello, world!2");
    while (1) {
        
    }
}