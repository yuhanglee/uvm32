#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "uvm32.h"
#include "../common/uvm32_common_custom.h"

__AFL_FUZZ_INIT();

int main(int argc, char *argv[]) {
    __AFL_INIT();
    uvm32_evt_t evt;

    uvm32_state_t *vmst = malloc(sizeof(uvm32_state_t));

    while (__AFL_LOOP(100000)) {
        memset(vmst, 0x00, sizeof(uvm32_state_t));
        uvm32_init(vmst);
        unsigned char *rom = __AFL_FUZZ_TESTCASE_BUF;
        uvm32_load(vmst, rom, __AFL_FUZZ_TESTCASE_LEN);

        uvm32_extram(vmst, rom, __AFL_FUZZ_TESTCASE_LEN);

        memset(&evt, 0x00, sizeof(evt));
        for (int i=0;i<10;i++) {
            uvm32_run(vmst, &evt, 1000);
        }
    }

    return 0;
}
