#ifndef rgb_h
#define rgb_h

#include <Arduino.h>
#include "GyverTimer.h"
#include "GRGB.h"

// #define STD_COLOR 'r'
#define STD_FADING_SPEED 5
#define STD_DELAY 500
#define CRT_MODE 1

#ifndef SERIAL_LIB
    #define SERIAL_LIB 0
#endif

// ### Modes ###
#define STOP 0
#define SHIMMER 1
#define BLINK 2
#define LIGHT 3
#define ALARM 4
// ##########

// ### Colors ###
// #define RED 'r'
// #define GREEN 'g'
// #define BLUE 'b'
// #define ALL 111
// #define BLACK 110
// ##########

// ### Timers ###
#define FADE_PERIOD 1000
#define BRIGHTNESS_STEP 5
#define SHIMMER_STEP FADE_PERIOD*2
// ##########

// ### Limits ###
#define GREEN_LIMIT 800
#define BLUE_LIMIT 1200
#define RED_LIMIT 1600
// ##########

// const uint32_t queue[] {GMaroon, GOlive, GLime, GAqua, GTeal};
const uint32_t queue[] {GRed, GGreen, GBlue};

class LED: public GRGB {
    private:
        uint8_t _mode;
        // uint16_t _green_limit = GREEN_LIMIT;
        // uint16_t _blue_limit = BLUE_LIMIT;
        // uint16_t _red_limit = RED_LIMIT;
        uint32_t _color;
        // uint32_t _queue{};
        byte _mode_flag = false;
        void _shimmer();
        void _blinking();
        void _lighting();
        void _alarm();

    public:
        using GRGB::GRGB;
        void set_mode(uint8_t mode);
        void set_color(uint32_t color);
        // void set_queue(uint32_t queue[]);
        // void set_limits(uint16_t green, uint16_t blue, uint16_t red);
        bool work();
};


#endif