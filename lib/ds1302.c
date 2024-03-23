#include "ds1302.h"

/******************** local helpers ********************/

static void write_byte(const ds1302_t *ds1302, uint8_t dat) {
    uint8_t mask = 0x01;
    while (mask) {
        ds1302->write(dat & mask);
        ds1302->sclk(1);
        ds1302->sclk(0);
        mask <<= 1;
    }
    ds1302->write(1);
}

static uint8_t read_byte(const ds1302_t *ds1302) {
    uint8_t dat = 0;
    uint8_t mask = 0x01;
    while (mask) {
        if (ds1302->read()) {
            dat |= mask;
        }
        ds1302->sclk(1);
        ds1302->sclk(0);
        mask <<= 1;
    }
    return dat;
}

/******************** public funtions ********************/

void ds1302_init(const ds1302_t *ds1302) {
    uint8_t dat = 0;
    ds1302_time_t time = DS1302_INIT_TIME;
    ds1302->ce(0);
    ds1302->sclk(0);
    dat = ds1302_read_byte(ds1302, DS1302_REG_SEC);
    if (dat & 0x80) { // clock halt
        ds1302_write_byte(ds1302, DS1302_REG_WP, 0x00); // enable write
        ds1302_write_time(ds1302, &time);
    }
}

uint8_t ds1302_read_byte(const ds1302_t *ds1302, uint8_t reg){
    uint8_t dat = 0;
    ds1302->ce(1);
    write_byte(ds1302, (reg << 1) | 0x81);
    dat = read_byte(ds1302);
    ds1302->ce(0);
    return dat;
}

void ds1302_write_byte(const ds1302_t *ds1302, uint8_t reg, uint8_t dat){
    ds1302->ce(1);
    write_byte(ds1302, (reg << 1) | 0x80);
    write_byte(ds1302, dat);
    ds1302->ce(0);
}

void ds1302_burst_read(const ds1302_t *ds1302, uint8_t *buf) {
    uint8_t i;
    ds1302->ce(1);    
    write_byte(ds1302, 0xBF); // 10111111
    for (i = 0; i < DS1302_BURST_LEN; i++) {
        buf[i] = read_byte(ds1302);
    }
    ds1302->ce(0);
}

void ds1302_burst_write(const ds1302_t *ds1302, const uint8_t *buf) {
    uint8_t i;
    ds1302->ce(1);
    write_byte(ds1302, 0xBE); // 10111110
    for (i = 0; i < DS1302_BURST_LEN; i++) {
        write_byte(ds1302, buf[i]);
    }
    ds1302->ce(0);
}

void ds1302_read_time(const ds1302_t *ds1302, ds1302_time_t *time) {
    ds1302_burst_read(ds1302, (uint8_t*)time);
    time->second = bcd2dec(time->second);
    time->minute = bcd2dec(time->minute);
    time->hour = bcd2dec(time->hour);
    time->date = bcd2dec(time->date);
    time->month = bcd2dec(time->month);
    time->day = bcd2dec(time->day);
    time->year = bcd2dec(time->year);
}

void ds1302_write_time(const ds1302_t *ds1302, ds1302_time_t *time) {
    time->second = dec2bcd(time->second);
    time->minute = dec2bcd(time->minute);
    time->hour = dec2bcd(time->hour);
    time->date = dec2bcd(time->date);
    time->month = dec2bcd(time->month);
    time->day = dec2bcd(time->day);
    time->year = dec2bcd(time->year);
    time->wp = 0;
    ds1302_burst_write(ds1302, (uint8_t*)time);
}
