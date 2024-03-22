#include "uart.h"

void main(void) {
    EA = 1;
    uart_init();
    while (1) {
    }
}
