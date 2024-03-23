/**
 * @file ds1302.h
 * @author captainwong (qtk.jack#gmail.com)
 * @brief DS1302 driver
 * @version 0.1
 * @date 2024-03-23
 *
 * @copyright Copyright (c) 2024
 * @todo 1. Support RAM read/write
 *       2. Support trickle charger
 *       3. Support 12-hour mode
 */

#ifndef __DS1302_H__
#define __DS1302_H__

#include "bcd.h"
#include "types.h"

#define DS1302_REG_SEC 0    // Second
#define DS1302_REG_MIN 1    // Minute
#define DS1302_REG_HOUR 2   // Hour
#define DS1302_REG_DATE 3   // Day of month
#define DS1302_REG_MONTH 4  // Month
#define DS1302_REG_DAY 5    // Day of week
#define DS1302_REG_YEAR 6   // Year
#define DS1302_REG_WP 7     // Write Protect
#define DS1302_BURST_LEN 8  // Burst length

/**
 * @brief The ds1302 time structure
 * @note The time is stored in BCD format in DS1302, and this structure is used to store the time in decimal format
 */
typedef struct {
    uint8_t second;  // 0-59
    uint8_t minute;  // 0-59
    uint8_t hour;    // 1-12/0-23
    uint8_t date;    // 1-31
    uint8_t month;   // 1: January, 2: February, ..., 12: December
    uint8_t day;     // 1: Sunday, 2: Monday, ..., 7: Saturday
    uint8_t year;    // 0-99, means 2000-2099
    uint8_t wp;      // Write Protect
} ds1302_time_t;

#ifndef DS1302_INIT_TIME
// 2024-3-23 16:05:59 Saturday
#define DS1302_INIT_TIME \
    { 59, 5, 16, 23, 3, 7, 24 }
#endif

typedef struct {
    void (*ce)(uint8_t);
    void (*sclk)(uint8_t);
    void (*write)(uint8_t);
    uint8_t (*read)(void);
} ds1302_t;

/**
 * @brief Initialize DS1302
 *
 * @note If the DS1302 is not initialized, the time will be reset to the initial time
 * @param ds1302 The DS1302 instance
 */
void ds1302_init(const ds1302_t *ds1302);

/**
 * @brief Read one byte from DS1302
 *
 * @note The read operation is done by sending 8 clock pulses to DS1302, and read the data bit by bit
 * @note This is the non-burst read operation
 * @param ds1302 The DS1302 instance
 * @param reg The register to read from, see DS1302_REG_*
 * @return uint8_t
 */
uint8_t ds1302_read_byte(const ds1302_t *ds1302, uint8_t reg);

/**
 * @brief Write one byte to DS1302
 *
 * @note The write operation is done by sending 8 clock pulses to DS1302, and write the data bit by bit
 * @note This is the non-burst write operation
 * @param ds1302 The DS1302 instance
 * @param reg The register to write to, see DS1302_REG_*
 * @param dat The data to write
 */
void ds1302_write_byte(const ds1302_t *ds1302, uint8_t reg, uint8_t dat);

/**
 * @brief Burst read 8 bytes from DS1302
 *
 * @param ds1302 The DS1302 instance
 * @param buf The buffer to store the read data, the buffer should have at least 8 bytes
 */
void ds1302_burst_read(const ds1302_t *ds1302, uint8_t *buf);

/**
 * @brief Burst write 8 bytes to DS1302
 *
 * @param ds1302 The DS1302 instance
 * @param buf The buffer to write to DS1302, the buffer should have at least 8 bytes
 */
void ds1302_burst_write(const ds1302_t *ds1302, const uint8_t *buf);

/**
 * @brief Read time from DS1302
 *
 * @param ds1302 The DS1302 instance
 * @param time The time structure to store the read time
 */
void ds1302_read_time(const ds1302_t *ds1302, ds1302_time_t *time);

/**
 * @brief Write time to DS1302
 *
 * @param ds1302 The DS1302 instance
 * @param time The time structure to write to DS1302
 */
void ds1302_write_time(const ds1302_t *ds1302, ds1302_time_t *time);

#endif
