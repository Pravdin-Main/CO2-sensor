#ifndef buzzer_h
#define buzzer_h

#include <Arduino.h>
#include "Beeper.h"
#include "GyverTimer.h"

#define ON_DELAY 100    // ms
#define OFF_DELAY 25    // ms
#define CHECK 5         // quantity of signals
#define ERR_DELAY 3000  // ms

void set_buzzer(uint8_t pin);
void do_beep(uint8_t num);
void buz_work();
bool buz_check();

#endif