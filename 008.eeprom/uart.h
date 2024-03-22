#ifndef __UART_H__
#define __UART_H__

#include "../stc89.h"
#include "types.h"

#define MAIN_FOSC 11059200
#define UART_BAUD 9600

extern uint8_t uart_rx;
void uart_init();
void uart_send_byte(uint8_t byte);
void uart_send_bytes(const uint8_t *bytes, uint8_t len);
void uart_send_string(const uint8_t *str);

#endif
