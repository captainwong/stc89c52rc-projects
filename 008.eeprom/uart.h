#ifndef __UART_H__
#define __UART_H__

#include "../stc89.h"

#define MAIN_FOSC 11059200
#define UART_BAUD 9600

void uart_init();
void uart_send_byte(unsigned char byte);
void uart_send_string(const unsigned char *str);

#endif
