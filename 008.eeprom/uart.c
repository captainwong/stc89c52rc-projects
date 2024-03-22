#include "uart.h"

uint8_t uart_rx = 0;
static bit tx_busy = 0;

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

void uart_send_byte(uint8_t dat) {
    while (tx_busy)
        ;
    tx_busy = 1;
    SBUF = dat;
}

void uart_send_bytes(const uint8_t *bytes, uint8_t len) {
    while (len--) {
        uart_send_byte(*bytes++);
    }
}

void uart_send_string(const uint8_t *str) {
    while (*str) {
        uart_send_byte(*str++);
    }
}

void uart_interrupt() interrupt 4 {
    if (RI) {
        RI = 0;
        uart_rx = SBUF;
        SBUF = uart_rx;
    }
    if (TI) {
        TI = 0;
        tx_busy = 0;
    }
}
