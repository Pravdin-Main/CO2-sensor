#ifndef disp_h
#define disp_h

// --- Connection type (choose only one) ---
#define SEVSEG 0
#define CD4026 1
#define _74HC595 0
// ---------

#include <Arduino.h>

#if(SEVSEG)
    #include "SevSeg.h"
#endif

#if(CD4026)
    #include "GyverTimer.h"
#endif

#define DIGITS 4                    // number of digits
#define CHECK 9999
#define FULL_BRI 255
#define HALF_BRI 125
#define NULL_BRI 0

#if(SEVSEG)
    #define RESISTORS false             // resistors on pins
    #define CONFIG COMMON_CATHODE       // cathode/anode
#endif

#if(CD4026)
    // --- Timers ---
    #define FREQ 3  // The frequence of display refresh (digit chenging), ms
    #define WAIT 1  // Implementation delay, ms
    #define SWITCHING 500 // Digits switching during loading display, ms
    // ---------- 
    enum{ZERO, ONE, TWO, TREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE};
    const uint16_t cats[DIGITS] {1000, 100,  10, 1};
#endif

#if(_74HC595)
    byte numbers[10] {B11111100, B01100000, B11011010, B11110010, B01100110, B10110110, B10111110, B11100000, B11111110, B11110110};    // numbers 0-9
#endif
// Display pins initialization in the following order: from A to G symbol, DP, from G1 to G4
#if(SEVSEG)
    void disp_init(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G, uint8_t DP, uint8_t G1, uint8_t G2, uint8_t G3, uint8_t G4);
    void disp_post(int value);
    void disp_setBrightness(uint8_t value);
    void disp_upd();
#endif

#if(CD4026)
    class disp {
        private:
            uint8_t _pin_clk;
            uint8_t _pin_rst;
            uint8_t _pin_off;
            uint8_t _pin_d1;
            uint8_t _pin_d2;
            uint8_t _pin_d3;
            uint8_t _pin_d4;
            
            bool _status = true;
            bool _flag = false;
            bool _zero_flag = false;
            uint8_t _cur_d = 0;
            uint8_t _fr = FREQ;
            uint8_t _numbers[DIGITS];

            void _prt_digit(uint8_t num);
            void _switch_off();
            void _prt_num();
            void _set_digit(uint8_t d);
        public:
            disp(uint8_t clk, uint8_t rst, uint8_t off, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4);
            void set_number(uint16_t num);
            bool work();
            void off();
            void on();
            void loading();
    };
#endif

#endif