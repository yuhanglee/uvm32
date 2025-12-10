#include "uvm32_target.h"

void stall(void) {
    volatile uint32_t i;
    for (i=0;i<100;i++) {
    }
}

void main(void) {
    for (int i=0;i<100;i++) {
        stall();
        printdec(i);
    }
}

