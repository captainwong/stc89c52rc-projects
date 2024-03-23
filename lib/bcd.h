#ifndef __BCD_H__
#define __BCD_H__

#define bcd2dec(bcd) (((bcd) & 0x0F) + ((bcd) >> 4) * 10)
#define dec2bcd(dec) ((((dec) / 10) << 4) + (dec) % 10)

#endif
