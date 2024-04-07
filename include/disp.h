#ifndef disp_h
#define disp_h

#include <Arduino.h>
#include "SevSeg.h"
#include "GyverTimer.h"

#define DELAY 1000                  // delay of CHECK displaying, ms
#define DIGITS 4                    // number of digits
#define RESISTORS false             // resistors on pins
#define CONFIG COMMON_CATHODE       // cathode/anode

// ### Display codes ###
#define CHECK 9999
#define LOAD 6666
// ##########

#define FULL_BRI 100
#define HALF_BRI 50
#define NULL_BRI 0

static uint16_t _value;
static bool load_flag = false;

// Display pins initialization in the following order: from A to G symbol, DP, from G1 to G4

void disp_init(uint8_t A, uint8_t B, uint8_t C, uint8_t D, uint8_t E, uint8_t F, uint8_t G, uint8_t DP, uint8_t G1, uint8_t G2, uint8_t G3, uint8_t G4);
void disp_post(uint16_t val);
void disp_setBrightness(uint8_t val);
void disp_upd();
void disp_err(uint16_t err_type);
void disp_load();

#endif