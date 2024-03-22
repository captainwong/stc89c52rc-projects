#ifndef __IIC_H__
#define __IIC_H__

#include <intrins.h>

#include "../stc89.h"

#define IIC_ACK 0
#define IIC_NACK 1

#define iic_delay() \
    {               \
        _nop_();    \
        _nop_();    \
        _nop_();    \
        _nop_();    \
    }

typedef struct {
    // actually, the parameter is bit
    void (*write_scl)(unsigned char);
    void (*write_sda)(unsigned char);
    bit (*read_sda)();
} iic_io_t;

void iic_start(const iic_io_t* io);
void iic_stop(const iic_io_t* io);
// return IIC_ACK for success, IIC_NACK for error
bit iic_send_byte(const iic_io_t* io, unsigned char dat);
// rw: 0 for read, 1 for write
// return IIC_ACK for success, IIC_NACK for error
bit iic_address(const iic_io_t* io, unsigned char addr, bit rw);

#endif
