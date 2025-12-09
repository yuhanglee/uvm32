#include "uvm32_target.h"

// provide main, with setup()/loop() flow
void setup(void);
bool loop(void);


uint32_t count;

bool loop(void) {
    printdec(count);
    if (count++ >= 10) {
        return false;
    } else {
        return true;
    }
}

void setup(void) {
    count = 0;
}


void main(void) {
    setup();
    while(loop()) {
        yield();
    }
}

