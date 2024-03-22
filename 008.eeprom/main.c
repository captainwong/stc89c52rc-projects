#include "uart.h"
#include "iic.h"
#include "delay.h"

sbit IIC_SCL = P3 ^ 2;
sbit IIC_SDA = P3 ^ 3;

static void iic_write_sda(unsigned char dat) {
    IIC_SDA = dat;
}

static void iic_write_scl(unsigned char dat) {
    IIC_SCL = dat;
}

static bit iic_read_sda() {
    return IIC_SDA;
}

static const iic_io_t iic_io = {
    iic_write_scl,
    iic_write_sda,
    iic_read_sda,
};

void main(void) {
    EA = 1;
    uart_init();
    while (1) {
        unsigned char ack = iic_address(&iic_io, 0x50, 0);
        //uart_send_byte('A');
        //uart_send_string("test");
        uart_send_string("addr=0x50, ack: ");
        uart_send_byte('0' + ack);
        delay_ms(3000);
        
        ack = iic_address(&iic_io, 0xA0, 0);
        uart_send_string("addr=0xA0, ack: ");
        uart_send_byte('0' + ack);
        delay_ms(3000);
    }
}
