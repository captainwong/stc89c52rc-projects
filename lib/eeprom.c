#include "eeprom.h"

bit eeprom_read(const iic_io_t* iic, uint8_t dev, uint8_t addr, uint8_t* buf, uint8_t len) {
    // wait for the device to be ready
    uint8_t retries = EEPROM_RETRY_TIMES;
    do {
        iic_start(iic);
        if (IIC_ACK == iic_write_byte(iic, dev << 1)) {
            break;
        }
        iic_stop(iic);
    } while (retries--);
    // device not ready
    if (retries == 0) {
        return IIC_NACK;
    }
    // now the device is ready, send the address
    iic_write_byte(iic, addr);
    // restart
    iic_start(iic);
    // send the device address again
    iic_write_byte(iic, (dev << 1) | 1);
    // read data
    while (len--) {
        *buf++ = iic_read_byte(iic, len ? IIC_ACK : IIC_NACK);
    }
    iic_stop(iic);
    return IIC_ACK;
}

bit eeprom_write(const iic_io_t* iic, uint8_t dev, uint8_t addr, const uint8_t* buf, uint8_t len) {
    while (len) {
        // wait for the device to be ready and last time write is finished
        uint8_t retries = EEPROM_RETRY_TIMES;
        do {
            iic_start(iic);
            if (IIC_ACK == iic_write_byte(iic, dev << 1)) {
                break;
            }
            iic_stop(iic);
        } while (retries--);
        // device not ready
        if (retries == 0) {
            return IIC_NACK;
        }

        // now the device is ready, write the buf to the device by page

        // send the address
        iic_write_byte(iic, addr);
        // write data by page
        do {
            iic_write_byte(iic, *buf++);
            addr++;
        } while (--len && (addr & EEPROM_PAGE_SIZE_MASK));
        iic_stop(iic);
    }
    return IIC_ACK;
}
