#include "../lib/stc89.h"

sbit IR = P3 ^ 2;
sbit LED = P2 ^ 0;

void main(void) {
    while (1) {
        LED = IR;
    }
}
