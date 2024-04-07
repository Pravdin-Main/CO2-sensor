#include "buzzer.h"

Beeper buz;
GTimer_ms err_delay(ERR_DELAY);

void set_buzzer(uint8_t pin) {
    buz.init(pin);
}

void do_beep(uint8_t num) {
    buz.beepNote(NOTE_G1, num, ON_DELAY, OFF_DELAY);
}

void buz_work() {
    buz.tick();
}

bool buz_check(){
    err_delay.reset();
    buz.beepNote(NOTE_G1, CHECK, ON_DELAY, OFF_DELAY);
    while (!buz.ready()) {
        buz.tick();
        if(err_delay.isReady()){
            return false;
        }
    }
    return true;
}