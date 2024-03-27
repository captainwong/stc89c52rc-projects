#include "../lib/delay.h"
#include "lcd1.h"
#include "lcd2.h"

static const char* code messages[] = {
    "Hi! I'm BATMAN!",    // 1
    "NO! I'm BATMAN!",    // 2
    "How you doing?",     // 1
    "I'm fine, you?",     // 2
    "I'm BATMAN!",        // 1
    "NO! I'm BATMAN!",    // 2
    "NO! I'm BATMAN!",    // 1
    "I'm BATMAN!",        // 2
    "OK, I'm ROBIN!",     // 1
    "NO! I'm BATMAN!",    // 2
    "Whaaaaaat?",         // 1
    "I'm BATMAN!",        // 2
    "Fine.........",      // 1
    "I'm YOUR FATHER!",   // 2
    "Freaking idiot...",  // 1
    "I'm BATMAN!",        // 2
    "Kill me now...",     // 1
    "Haha! Gotcha!",      // 2
};

void main(void) {
    uint8_t i = 0;
    lcd1_init();
    lcd2_init();

    while (1) {
        // make them talk
        lcd1_print(messages[i]);
        delay_ms(2000);
        lcd1_clear();
        lcd2_print(messages[i + 1]);
        delay_ms(2000);
        lcd2_clear();
        i += 2;
        if (i >= sizeof(messages) / sizeof(messages[0])) {
            i = 0;
        }
    }
}
