#ifndef __DELAY_H__
#define __DELAY_H__

#include <intrins.h>

// @11.0592MHz
#define delay_1us() _nop_()
#define delay_2us() { _nop_(); _nop_(); }
#define delay_3us() { _nop_(); _nop_(); _nop_(); }
#define delay_5us() { _nop_(); _nop_(); _nop_(); _nop_(); _nop_(); }
#define delay_10us() { delay_5us(); delay_5us(); }
void delay_us(unsigned int us);
void delay_ms(unsigned int ms);

#endif
