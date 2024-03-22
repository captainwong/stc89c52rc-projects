#include "iic.h"

void iic_start(const iic_io_t* io) {
    io->write_sda(1);
    io->write_scl(1);
    iic_delay();
    io->write_sda(0);
    iic_delay();
    io->write_scl(0);
}

void iic_stop(const iic_io_t* io) {
    io->write_sda(0);
    io->write_scl(0);
    iic_delay();
    io->write_scl(1);
    iic_delay();
    io->write_sda(1);
    iic_delay();
}

bit iic_send_byte(const iic_io_t* io, unsigned char dat) {
    bit ack = IIC_NACK;
    unsigned char mask = 0x80;
    while (mask) {
        io->write_sda(dat & mask);
        iic_delay();
        io->write_scl(1);
        iic_delay();
        io->write_scl(0);
        mask >>= 1;
    }
    io->write_sda(1);
    iic_delay();
    io->write_scl(1);
    iic_delay();
    ack = io->read_sda();
    iic_delay();
    io->write_scl(0);
    return ack;
}

bit iic_address(const iic_io_t* io, unsigned char addr, bit rw) {
    bit ack = IIC_NACK;
    iic_start(io);
    ack = iic_send_byte(io, (addr << 1) | rw);
    iic_stop(io);
    return ack;
}
