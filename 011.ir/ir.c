#include "ir.h"

bit ir_valid;
uint8_t ir_value[4];
uint8_t ir_error_line;
uint16_t ir_counter;

void ir_init() {
    ir_valid = 0;
    ir_value[0] = ir_value[1] = ir_value[2] = ir_value[3] = 0;
    ir_error_line = 0;
    IR_PIN = 1;

    // configure timer 0
    TMOD &= 0xF0;      // clear timer 0 mode bits
    TMOD |= 0x01;      // set timer 0 mode 1, 16-bit ir_counter, no interrupt
    TH0 = TL0 = 0x00;  // set initial value to 0 as we are using it as a ir_counter
    TR0 = 0;           // stop timer 0
    ET0 = 0;           // disable timer 0 interrupt

    // configure external interrupt 0
    IT0 = 1;  // falling edge trigger
              // according to the datasheet
              // CPU will not trigger the interrupt again if it is still executing the ISR
              // until we set IE0 to 0 in the ISR
              // so we don't need to worry about the ISR taking too long
              // because we will count the number of cycles of the IR signal in the ISR
    EX0 = 1;  // enable external interrupt 0
}

// count the number of cycles the IR signal is high
// return the number of cycles
static uint16_t count_ir_high() {
    TH0 = TL0 = 0x00;
    TR0 = 1;
    while (IR_PIN) {
        if (TH0 > TH0_TIMEOUT) {
            break;
        }
    }
    TR0 = 0;
    return TH0 << 8 | TL0;
}

// count the number of cycles the IR signal is low
// return the number of cycles
static uint16_t count_ir_low() {
    TH0 = TL0 = 0x00;
    TR0 = 1;
    while (!IR_PIN) {
        if (TH0 > TH0_TIMEOUT) {
            break;
        }
    }
    TR0 = 0;
    return TH0 << 8 | TL0;
}

void ir_isr() interrupt 0 {
    uint8_t i, j, value;

    // leading burst
    ir_counter = count_ir_low();
    if (ir_counter < IR_LEADING_BURST_MIN || ir_counter > IR_LEADING_BURST_MAX) {
        ir_error_line = __LINE__;
        goto fail;
    }

    // leading space
    ir_counter = count_ir_high();
    if (ir_counter < IR_LEADING_SPACE_MIN || ir_counter > IR_LEADING_SPACE_MAX) {
        ir_error_line = __LINE__;
        goto fail;
    }

    // data bits
    for (i = 0; i < 4; i++) {
        value = 0;
        for (j = 0; j < 8; j++) {
            ir_counter = count_ir_low();
            if (ir_counter < IR_LOGICAL_BURST_MIN || ir_counter > IR_LOGICAL_BURST_MAX) {
                ir_error_line = __LINE__;
                goto fail;
            }

            ir_counter = count_ir_high();
            if (IR_LOGICAL_SPACE_MIN_0 <= ir_counter && ir_counter <= IR_LOGICAL_SPACE_MAX_0) {
                // logical 0
                value >>= 1;
            } else if (IR_LOGICAL_SPACE_MIN_1 <= ir_counter && ir_counter <= IR_LOGICAL_SPACE_MAX_1) {
                // logical 1
                value >>= 1;
                value |= 0x80;
            } else {
                ir_error_line = __LINE__;
                goto fail;
            }
        }
        ir_value[i] = value;
    }

    // success
    ir_valid = 1;
    goto clean;

fail:
    ir_valid = 0;

clean:
    // clear external interrupt 0 flag to enable the interrupt again
    IE0 = 0;
}
