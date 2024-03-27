#include "iic.h"

void iic_start(const iic_t* io) {
    io->write_sda(1);
    io->write_scl(1);
    iic_delay();
    io->write_sda(0);
    iic_delay();
    io->write_scl(0);
}

void iic_stop(const iic_t* io) {
    io->write_sda(0);
    io->write_scl(0);
    iic_delay();
    io->write_scl(1);
    iic_delay();
    io->write_sda(1);
    iic_delay();
}

bit iic_write_byte(const iic_t* io, uint8_t dat) {
    bit ack = IIC_NACK;
    uint8_t mask = 0x80;
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

bit iic_test(const iic_t* io, uint8_t addr) {
    bit ack = IIC_NACK;
    iic_start(io);
    ack = iic_write_byte(io, (addr << 1));
    iic_stop(io);
    return ack;
}

uint8_t iic_read_byte(const iic_t* io, bit ack) {
    uint8_t dat = 0;
    uint8_t mask = 0x80;
    io->write_sda(1);
    while (mask) {
        iic_delay();
        io->write_scl(1);
        iic_delay();
        if (io->read_sda()) {
            dat |= mask;
        }
        iic_delay();
        io->write_scl(0);
        mask >>= 1;
    }
    io->write_sda(ack);
    iic_delay();
    io->write_scl(1);
    iic_delay();
    io->write_scl(0);
    return dat;
}
