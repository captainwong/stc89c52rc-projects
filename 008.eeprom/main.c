#include "delay.h"
#include "eeprom.h"
#include "iic.h"
#include "uart.h"

sbit IIC_SCL = P3 ^ 2;
sbit IIC_SDA = P3 ^ 3;

#define EEPROM_DEV_ADDR 0x50
#define EEPROM_BUF_LEN 128

static uint8_t eeprom_buf[EEPROM_BUF_LEN];

static void iic_write_sda(uint8_t dat) {
    IIC_SDA = dat;
}

static void iic_write_scl(uint8_t dat) {
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
    uint8_t i = 0;
    EA = 1;
    uart_init();
    while (1) {
        if (uart_rx == 'r') {  // read low 128 bytes
            uart_rx = 0;
            eeprom_read(&iic_io, EEPROM_DEV_ADDR, 0, eeprom_buf, EEPROM_BUF_LEN);
            uart_send_bytes(eeprom_buf, EEPROM_BUF_LEN);
        } else if (uart_rx == 'R') {  // read high 128 bytes
            uart_rx = 0;
            eeprom_read(&iic_io, EEPROM_DEV_ADDR, EEPROM_BUF_LEN, eeprom_buf, EEPROM_BUF_LEN);
            uart_send_bytes(eeprom_buf, EEPROM_BUF_LEN);
        } else if (uart_rx == 'w') {  // write low 128 bytes
            uart_rx = 0;
            for (i = 0; i < EEPROM_BUF_LEN; i++) {
                eeprom_buf[i] = i;
            }
            eeprom_write(&iic_io, EEPROM_DEV_ADDR, 0, eeprom_buf, EEPROM_BUF_LEN);
        } else if (uart_rx == 'W') {  // write high 128 bytes
            uart_rx = 0;
            for (i = 0; i < EEPROM_BUF_LEN; i++) {
                eeprom_buf[i] = EEPROM_BUF_LEN + i;
            }
            eeprom_write(&iic_io, EEPROM_DEV_ADDR, EEPROM_BUF_LEN, eeprom_buf, EEPROM_BUF_LEN);
        } else if (uart_rx == 'F') {  // read all 256 bytes
            uart_rx = 0;
            eeprom_read(&iic_io, EEPROM_DEV_ADDR, 0, eeprom_buf, EEPROM_BUF_LEN);
            uart_send_bytes(eeprom_buf, EEPROM_BUF_LEN);
            eeprom_read(&iic_io, EEPROM_DEV_ADDR, EEPROM_BUF_LEN, eeprom_buf, EEPROM_BUF_LEN);
            uart_send_bytes(eeprom_buf, EEPROM_BUF_LEN);
        }
    }
}
