#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "uvm32.h"
#include "../common/uvm32_common_custom.h"

// Run multiple scheduled VMs in parallel until all have ended
#define NUM_VM 4   // number of vms
#define SCHEDULE SCHEDULE_RANDOM    // scheduling algorithm

// scheduling algorithms
#define SCHEDULE_ROUNDROBIN()   scheduler_index = (scheduler_index + 1) % NUM_VM
#define SCHEDULE_RANDOM()       scheduler_index = rand()%NUM_VM

#include "fib.h"

int main(int argc, char *argv[]) {
    uvm32_state_t vmst[NUM_VM];
    uvm32_evt_t evt;
    int numVmRunning = NUM_VM;
    int scheduler_index = 0;

    for (int i=0;i<NUM_VM;i++) {
        uvm32_init(&vmst[i]);
        uvm32_load(&vmst[i], fib, fib_len);
    }

    while(numVmRunning > 0) {
        if (uvm32_hasEnded(&vmst[scheduler_index])) {
            // this vm has already completed, pick another
            SCHEDULE();
            continue;
        }
        uvm32_run(&vmst[scheduler_index], &evt, 100);   // num instructions before vm considered hung

        switch(evt.typ) {
            case UVM32_EVT_END:
                printf("[VM %d ended]\n", scheduler_index);
                numVmRunning--;
            break;
            case UVM32_EVT_SYSCALL:    // vm has paused to handle UVM32_SYSCALL
                switch(evt.data.syscall.code) {
                    case UVM32_SYSCALL_YIELD:
                    break;
                    case UVM32_SYSCALL_PRINTLN: {
                        const char *str = uvm32_getcstr(&vmst[scheduler_index], &evt, ARG0);
                        if (str[0] != 0) {
                            printf("[VM %d] %s\n", scheduler_index, str);
                        }
                    } break;
                    case UVM32_SYSCALL_PRINTDEC:
                        printf("[VM %d] %d\n", scheduler_index, uvm32_getval(&vmst[scheduler_index], &evt, ARG0));
                    break;
                    default:
                        printf("Unhandled syscall 0x%08x\n", evt.data.syscall.code);
                    break;
                }
            break;
            case UVM32_EVT_ERR:
                printf("UVM32_EVT_ERR '%s' (%d)\n", evt.data.err.errstr, (int)evt.data.err.errcode);
            break;
            default:
            break;
        }
        
        SCHEDULE();
    }

    return 0;
}
