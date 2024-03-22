#include "uart.h"

void uart_init() {
    // set UART mode to mode 1, 8-bit data, 1 stop bit, no parity
    SCON = 0x50;
    // timer 2 16-bit auto-reload
    TL2 = RCAP2L = (65536 - (MAIN_FOSC / 32 / UART_BAUD));  // set auto-reload vaule
    TH2 = RCAP2H = (65536 - (MAIN_FOSC / 32 / UART_BAUD)) >> 8;
    // 00110100, TR2=1, C/T2#=0, CP/RL2#=1, EXEN2=0, TCLK0=1, RCLK0=1, T2S0=0, T2S1=0
    // start timer2, set as 16-bit auto-reload timer
    T2CON = 0x34;
    ES = 1;  // enable UART interrupt
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
