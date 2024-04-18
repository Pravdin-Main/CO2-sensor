#include "disp.h"
#if(SEVSEG)
    SevSeg display;
#endif

#if(CD4026)
    GTimer_ms freq(FREQ);
    GTimer_ms wait(WAIT);
    GTimer_ms switching(SWITCHING);
#endif

#if(SEVSEG)
void disp_init(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G, uint8_t DP, uint8_t G1, uint8_t G2, uint8_t G3, uint8_t G4) {
    byte digit_pins[] = {G1, G2, G3, G4};
    byte segment_pins[] = {A, B, C, D, E, F, G, DP};
    display.begin(CONFIG, DIGITS, digit_pins, segment_pins, RESISTORS);
    display.setBrightness(FULL_BRI);
};

void disp_post(int value) {
    display.setNumber(value);
}

void disp_setBrightness(uint8_t value) {
    value = map(value, 0, 255, 0, 100);
    display.setBrightness(value);
}

void disp_upd() {
    // for (uint8_t count = 0; count < 8; count++){
        display.refreshDisplay();
    // }
}
#endif

#if(CD4026)
disp::disp(uint8_t clk, uint8_t rst, uint8_t off, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4){
    _pin_clk = clk;
    _pin_rst = rst;
    _pin_off = off;
    _pin_d1 = d1;
    _pin_d2 = d2;
    _pin_d3 = d3;
    _pin_d4 = d4;

    pinMode(_pin_clk, OUTPUT);
    pinMode(_pin_rst, OUTPUT);
    pinMode(_pin_off, OUTPUT);
    pinMode(_pin_d1, OUTPUT);
    pinMode(_pin_d2, OUTPUT);
    pinMode(_pin_d3, OUTPUT);
    pinMode(_pin_d4, OUTPUT);
    // Serial.println("Disp: Pins set");

    digitalWrite(_pin_off, HIGH);
    digitalWrite(_pin_rst, HIGH);
    digitalWrite(_pin_rst, LOW);
    // Serial.println("Disp: CD4026 prepared");

    digitalWrite(_pin_d1, HIGH);
    digitalWrite(_pin_d2, HIGH);
    digitalWrite(_pin_d3, HIGH);
    digitalWrite(_pin_d4, HIGH);
    // Serial.println("Disp: Indicator prerared");
}

void disp::set_number(uint16_t num){
    if(num > CHECK){
        for(uint8_t count = 0; count != DIGITS; count++){
            _numbers[count] = 9;
        }
        // Serial.println("Disp: ERROR! Number is over " + (String)CHECK);
        return;
    }
    if (num == 0){
        for(uint8_t count = 0; count != DIGITS; count++){
            _numbers[count] = 0;
        }
        // Serial.print("Disp: got " + (String)num + " wrote ");
        // for(uint8_t count = 0; count < sizeof(_numbers)/sizeof(_numbers[0]);count++){
        //     Serial.print((String)_numbers[count]);
        // }
        // Serial.println("");
        return;
    }
    // Serial.print("Disp: got " + (String)num);
    for (uint8_t count = 0; count != DIGITS; count++){
        _numbers[count] = num / cats[count];
        num = num - (_numbers[count] * cats[count]);
    } 
    // Serial.print(" wrote ");
    // for(uint8_t count = 0; count < sizeof(_numbers)/sizeof(_numbers[0]);count++){
    //     // Serial.print((String)_numbers[count] + " ");
    // }
    // Serial.println("");
}

bool disp::work(){
    if(_status){
        _prt_num();
        // Serial.println("Status is 1");
        return true;
    }
        else{
            _switch_off();
            return false;
        }
}

void disp::off(){
    _status = false;
    _flag = true;
    digitalWrite(_pin_off, LOW);
}

void disp::on(){
    _status = true;
    _flag = true;
    digitalWrite(_pin_off, HIGH);
}

void disp::loading(){
    if(_flag){
        _flag = false;
        freq.reset();
        switching.reset();
        
        for (uint8_t count = 0; count != DIGITS; count++){
            _set_digit(count);
            digitalWrite(_pin_off, LOW);
        }

        _cur_d = 0;
        _set_digit(_cur_d);
        digitalWrite(_pin_off, HIGH);
        digitalWrite(_pin_rst, HIGH);
        digitalWrite(_pin_rst, LOW);
    }

    if(switching.isReady()){
        digitalWrite(_pin_off, LOW);
        _cur_d++;
        if(_cur_d >= DIGITS) {
            _cur_d = 0;
            _zero_flag = false;
        }

        _set_digit(_cur_d);
        digitalWrite(_pin_off, HIGH);

        digitalWrite(_pin_rst, HIGH);
        digitalWrite(_pin_rst, LOW);
    }
}

void disp::_switch_off(){
    if(_flag) _flag = false;
}

void disp::_prt_num(){
    if(_flag){
        _flag = false;
        freq.reset();
        _cur_d = 0;
        _set_digit(_cur_d);
        _prt_digit(_numbers[_cur_d]);
        // Serial.println("Disp: First digit printed: " + (String)_numbers[_cur_d]);
    }

    if(freq.isReady()){
        _cur_d++;
        if(_cur_d >= DIGITS) {
            _cur_d = 0;
            _zero_flag = false;
        }

        _set_digit(_cur_d);
        _prt_digit(_numbers[_cur_d]);
        // Serial.println("Disp: Number printed on digit " + (String)_cur_d);
        // Serial.println("Disp: Number printed on digit " + (String)_cur_d + " Number is: " + (String)_numbers[_cur_d]);
    }
}

void disp::_prt_digit(uint8_t num){
    // reset the indicator
    digitalWrite(_pin_rst, HIGH);
    digitalWrite(_pin_rst, LOW);
    // Serial.println("Disp: digit reseted");
    // Serial.println("Disp: _prt_digit: number is " + (String)num);

    if(num == 0) {
        // Serial.println("Disp: _prt_digit: number is ZERO");
        if(!_zero_flag) {
            digitalWrite(_pin_off, LOW);
        }
        return;
    }
    digitalWrite(_pin_off, HIGH);
    if(num > 9){
        // digitalWrite(_pin_off, HIGH);
        for (uint8_t count = 9; num != 0; count--){
            digitalWrite(_pin_clk, HIGH);
            // delay(1);
            digitalWrite(_pin_clk, LOW);
        }
        // Serial.println("Disp: _prt_digit: number is over 9 - " + (String)num);
        return;
    }
    while (num--) {
        digitalWrite(_pin_clk, HIGH);
        digitalWrite(_pin_clk, LOW);
        // Serial.println("Disp: _prt_digit: number is " + (String)num);
    }
    _zero_flag = true;
    
    // for (uint8_t count = num; num != 0; count--){
    //     digitalWrite(_pin_clk, HIGH);
    //     // delay(1);
    //     digitalWrite(_pin_clk, LOW);
    //     Serial.println("Disp: _prt_digit: number is " + (String)num);
    // }
}

void disp::_set_digit(uint8_t d){
    digitalWrite(_pin_d1, HIGH);
    digitalWrite(_pin_d2, HIGH);
    digitalWrite(_pin_d3, HIGH);
    digitalWrite(_pin_d4, HIGH);
    switch (d){
        case ZERO:
            digitalWrite(_pin_d1, LOW);
            break;
        case ONE:
            digitalWrite(_pin_d2, LOW);
            break;
        case TWO:
            digitalWrite(_pin_d3, LOW);
            break;
        case TREE:
            digitalWrite(_pin_d4, LOW);
            break;
        default:
            break;
    }
}
#endif