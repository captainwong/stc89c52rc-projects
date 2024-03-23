#include "../lib/stc89.h"
#include "../lib/uart.h"
#include "ir.h"

sbit LED = P2 ^ 0;

void main(void) {
    EA = 1;
    uart_init();
    ir_init();

    while (1) {
        LED = IR_PIN;

        if (ir_valid) {
            ir_valid = 0;
            uart_send_bytes(ir_value, sizeof(ir_value));
        }

        if (ir_error_line) {
            uart_send_byte(ir_error_line);
            ir_error_line = 0;
            uart_send_byte(ir_counter >> 8);
            uart_send_byte(ir_counter);
        }
    }
}
