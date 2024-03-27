// at24cxx

#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "iic.h"

// change these comments to match the actual eeprom
#define EEPROM_PAGE_SIZE 8
#define EEPROM_PAGE_SIZE_MASK 0x07
// #define EEPROM_PAGE_SIZE 16
// #define EEPROM_PAGE_SIZE_MASK 0x0F

// we retry 10 times if failed
#ifndef EEPROM_RETRY_TIMES
#define EEPROM_RETRY_TIMES 10
#endif

/*
 * read bytes from eeprom
 * @iic: iic bus
 * @dev: eeprom device address
 * @addr: eeprom address
 * @buf: buffer to store data
 * @len: number of bytes to read
 * @return: IIC_ACK if success, IIC_NACK if failed
 */
bit eeprom_read(const iic_t* iic, uint8_t dev, uint8_t addr, uint8_t* buf, uint8_t len);

/*
 * write bytes to eeprom
 * @iic: iic bus
 * @dev: eeprom device address
 * @addr: eeprom address
 * @buf: buffer to store data
 * @len: number of bytes to write
 * @return: IIC_ACK if success, IIC_NACK if failed
 */
bit eeprom_write(const iic_t* iic, uint8_t dev, uint8_t addr, const uint8_t* buf, uint8_t len);

#endif
