#include "rgb.h"

GTimer_ms brightness(BRIGHTNESS_STEP);
GTimer_ms shimmer(SHIMMER_STEP);
static uint8_t pos = 1;

void LED::_shimmer() {
    if(_mode_flag){
        _mode_flag = false;

        fadeMode(true);
        setFadePeriod(FADE_PERIOD);

        #if(CRT_MODE == 1)
            setCRT(true);
        #endif

        setColor(queue[0]);
        // setColor(_color);
        Serial.println("LED: shimmer is running");
        shimmer.reset();
    }

    if(shimmer.isReady()){
        setColor(queue[pos]);
        pos++;
        if(pos == sizeof(queue)/sizeof(queue[0])){
            pos = 0;
        }
    }
}

void LED::_blinking() {
    if(_mode_flag){
        _mode_flag = false;

        fadeMode(true);
        setFadePeriod(FADE_PERIOD/2);

        #if(CRT_MODE == 1)
            setCRT(true);
        #endif

        setColor(_color);
        Serial.println("LED: blinking is running");
        brightness.reset();
    }

    if(brightness.isReady()) {
        static int8_t dir = 1;
        static uint8_t val = 0;

        val += dir;
        if(val == 255 || val == 0) dir = -dir;
        setBrightness(val);
    }
}

void LED::_lighting() {
    if(_mode_flag){
        _mode_flag = false;

        fadeMode(true);
        setFadePeriod(FADE_PERIOD);

        #if(CRT_MODE == 1)
            setCRT(true);
        #endif

        setColor(_color);
        Serial.println("LED: lighting is running");
    }
}

void LED::_alarm() {
    if(_mode_flag){
        _mode_flag = false;
        fadeMode(false);
        shimmer.reset();
        setColor(_color);
        Serial.println("LED: alarm is running");
    }

    if(shimmer.isReady()){
        static bool flag;
        if (flag){
            enable();
        } else {
            disable();
        }
        flag = !flag;
    }
}

void LED::set_mode(uint8_t value) {
    _mode = value;
    _mode_flag = true;
}

void LED::set_color(uint32_t color) {
    _color = color;
    setColor(_color);
}

// void LED::set_queue(uint32_t queue[]){
//     _queue = queue;
// }

// void LED::set_limits(uint16_t green, uint16_t blue, uint16_t red) {
//     _green_limit = green;
//     _blue_limit = blue;
//     _red_limit = red;
// }

bool LED::work() {
    switch (_mode) {
    case STOP:
            disable();
        if(_mode_flag){
            Serial.println("LED: stopped");
            _mode_flag = false;
        }
        break;
    case SHIMMER:
        _shimmer();
        break;
    case BLINK:
        _blinking();
        break;
    case LIGHT:
        _lighting();
        break;
    case ALARM:
        _alarm();
        break;
    default:
        break;
    }
    return tick();
}