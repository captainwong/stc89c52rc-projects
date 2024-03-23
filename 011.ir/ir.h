/**
 * @file ir.h
 * @author captainwong (qtk.jack#gmail.com)
 * @brief Infrared sensor library for STC89C52RC
 *   - This library uses the NEC protocol to read the digital value from the sensor
 *   - The sensor's VOUT pin should be connected to the P3.2 pin with a pull-up resistor, typically 4.7k ohm
 *   - This library uses timer 0 to calculate the pulse width of the signal
 * @version 0.1
 * @date 2024-03-24
 *
 * @copyright Copyright (c) 2024
 *
 */

#ifndef __IR_H__
#define __IR_H__

#include "../lib/stc89.h"
#include "../lib/types.h"

#define MACHINE_CYCLE 12  // 12T
#define MAIN_FOSC 11059200
#define MACHINE_CYCLE_US 1.085f  // 1000000us / (MAIN_FOSC / MACHINE_CYCLE) = 1.085us
#define TH0_TIMEOUT 0x40         // abount 15ms, 15 * 1000 * 1.085 = 16312.5 = 0x3FD8
// a 9ms leading pulse burst (16 times the pulse burst length used for a logical data bit)
#define IR_LEADING_BURST_MIN 0x1E9A  // 8500us / 1.085 = 7834 = 0x1E9A
#define IR_LEADING_BURST_MAX 0x2233  // 9500us / 1.085 = 8755 = 0x2233
// a 4.5ms space
#define IR_LEADING_SPACE_MIN 0xE67   // 4000us / 1.085 = 3687 = 0xE67
#define IR_LEADING_SPACE_MAX 0x10B2  // 5000us / 1.085 = 4603 = 0x10B2
// Logical '0' – a 562.5µs pulse burst followed by a 562.5µs space, with a total transmit time of 1.125ms
// Logical '1' – a 562.5µs pulse burst followed by a 1.6875ms space, with a total transmit time of 2.25ms
#define IR_LOGICAL_BURST_MIN 0x0A2  // 360us / 1.085 = 332 = 0x14C
#define IR_LOGICAL_BURST_MAX 0x2BC  // 760us / 1.085 = 700 = 0x2BC
#define IR_LOGICAL_SPACE_MIN_0 IR_LOGICAL_BURST_MIN
#define IR_LOGICAL_SPACE_MAX_0 IR_LOGICAL_BURST_MAX
#define IR_LOGICAL_SPACE_MIN_1 0x4B9  // 1600us / 1.085 = 1474 = 0x4B9
#define IR_LOGICAL_SPACE_MAX_1 0x6F4  // 2250us / 1.085 = 2078 = 0x6F4

sbit IR_PIN = P3 ^ 2;        // The pin for the IR sensor
extern bit ir_valid;         // 1 if the ir_value is valid, otherwise 0
extern uint8_t ir_value[4];  // 4 bytes for the NEC protocol
extern uint8_t ir_error_line;
extern uint16_t ir_counter;

/**
 * @brief The initialization function for the IR sensor
 * @note It configures the external interrupt 0 and timer 0
 */
void ir_init();

#endif
