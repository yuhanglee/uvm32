#include "config.h"
#include "uvm32.h"
#include "uvm32_common_custom.h"

#include "mandel.h"

uvm32_state_t vmst;
uvm32_evt_t evt;
bool isrunning = false;
uint32_t led_time = 0;
bool led_state = false;

void setup(void) {
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);
    isrunning = false;
}

void loop(void) {
    // flash LED rapidly to show main loop is still running
    if (millis() > led_time + 50) {
        digitalWrite(LED_BUILTIN, led_state);
        led_state = !led_state;
        led_time = millis();
    }

    if (isrunning) {
        uvm32_run(&vmst, &evt, 0xFFFFFFFF);   // num instructions before vm considered hung

        switch(evt.typ) {
            case UVM32_EVT_END:
                isrunning = false;
            break;
            case UVM32_EVT_SYSCALL:    // vm has paused to handle UVM32_SYSCALL
                switch(evt.data.syscall.code) {
                    case UVM32_SYSCALL_YIELD:
                    break;
                    case UVM32_SYSCALL_PUTC:
                        Serial.print((char)uvm32_getval(&vmst, &evt, ARG0));
                    break;
                    case UVM32_SYSCALL_PRINTLN: {
                        const char *str = uvm32_getcstr(&vmst, &evt, ARG0);
                        Serial.print(str);
                        Serial.println("");
                    } break;
                    case UVM32_SYSCALL_PRINT: {
                        const char *str = uvm32_getcstr(&vmst, &evt, ARG0);
                        Serial.print(str);
                    } break;
                    case UVM32_SYSCALL_MILLIS: {
                        uvm32_setval(&vmst, &evt, RET, millis());
                    } break;
                    default:
                        Serial.print("Unhandled syscall: ");
                        Serial.print(evt.data.syscall.code);
                        Serial.println("");
                    break;
                }
            break;
            case UVM32_EVT_ERR:
                Serial.print("Error: ");
                Serial.println(evt.data.err.errstr);
                isrunning = false;
            break;
        }
    } else {
        Serial.println("Running VM");
        // setup vm
        uvm32_init(&vmst);
        uvm32_load(&vmst, mandel, mandel_len);
        isrunning = true;
    }
}
