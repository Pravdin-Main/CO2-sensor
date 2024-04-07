#include "disp.h"

SevSeg display;
GTimer_ms delay_check(CHECK);

void disp_init(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G, uint8_t DP, uint8_t G1, uint8_t G2, uint8_t G3, uint8_t G4) {
    byte digit_pins[] = {G1, G2, G3, G4};
    byte segment_pins[] = {A, B, C, D, E, F, G, DP};

    display.begin(CONFIG, DIGITS, digit_pins, segment_pins, RESISTORS);
    display.setBrightness(FULL_BRI);
    display.setNumber(CHECK);

    while (!delay_check.isReady()) {
        display.refreshDisplay();
    }
}

void disp_post(uint16_t val) {
    _value = val;
    display.setNumber(val);
}

void disp_setBrightness(uint8_t val) {
    val = map(val, 0, 255, 0, 100);
    display.setBrightness(val);
}

void disp_upd() {
    if(!load_flag) load_flag = !load_flag;
    for(uint8_t count = 0; count < 64; count++){
        display.refreshDisplay();
    }
}

void disp_err(uint16_t err_type){
    display.setNumber(err_type);
    while (!delay_check.isReady()) {
        display.refreshDisplay();
    }
}

void disp_load(){
    if(!load_flag) {
        display.setNumber(LOAD);
        load_flag = !load_flag;
    }
    display.refreshDisplay();
}