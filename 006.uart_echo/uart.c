#include "uart.h"
#include "../lib/types.h"

void uart_init() {
    // set UART mode to mode 1, 8-bit data, 1 stop bit, no parity
    SCON = 0x50;
    TMOD = 0x20;  // timer1 mode 2, 8-bit auto-reload
    // set baud rate
    TH1 = (uint8_t)(256 - (MAIN_FOSC / 12 / 32 / UART_BAUD));
    TL1 = TH1;
    ET1 = 0;  // disable timer1 interrupt
    ES = 1;   // enable UART interrupt
    TR1 = 1;  // start timer1
}

void uart_interrupt() interrupt 4 {
    if (RI) {
        RI = 0;
        SBUF = SBUF;
    }
    if (TI) {
        TI = 0;
    }
}
