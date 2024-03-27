#ifndef __IIC_H__
#define __IIC_H__

#include <intrins.h>
#include "types.h"

#define IIC_ACK 0
#define IIC_NACK 1

#ifndef iic_delay
#define iic_delay() \
    {               \
        _nop_();    \
        _nop_();    \
        _nop_();    \
        _nop_();    \
    }
#endif

typedef struct {
    // actually, the parameter is bit
    void (*write_scl)(uint8_t);
    void (*write_sda)(uint8_t);
    bit (*read_sda)();
} iic_t;

/*
 * Test if the device with the specified address exists
 * @param io: IIC IO operations
 * @param addr: device address
 * @return IIC_ACK for device exists, IIC_NACK for device not exists
 */
bit iic_test(const iic_t* io, uint8_t addr);
void iic_start(const iic_t* io);
void iic_stop(const iic_t* io);
// return IIC_ACK for success, IIC_NACK for error
bit iic_write_byte(const iic_t* io, uint8_t dat);
// read one byte from IIC bus, send ack if ack is IIC_ACK, otherwise send nack
uint8_t iic_read_byte(const iic_t* io, bit ack);

#endif
