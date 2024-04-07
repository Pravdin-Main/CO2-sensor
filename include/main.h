#ifndef main_h
#define main_h

#include <Arduino.h>
#include "rgb.h"
#include "co2.h"
#include "disp.h"
#include "buzzer.h"
#include "GyverTimer.h"
#include "EncButton.h"
#include "MHZ19.h"
#include <SoftwareSerial.h>

#define DEBUG 0

// --- Pins set -------------------------------------
// ### LED ###
#define LED_R 0
#define LED_G 1
#define LED_B 10
// ### 4-digits display ###
#define DISP_A 2
#define DISP_B A0
#define DISP_C 5
#define DISP_D 7
#define DISP_E 8
#define DISP_F A3
#define DISP_G 13
#define DISP_DP 6
#define DISP_G1 12
#define DISP_G2 A1
#define DISP_G3 A2
#define DISP_G4 4
// ### CO2 sensor ###
#define CO2_TX 11
#define CO2_RX 9
#define CO2_A A5
// ### Touch button ###
#define BUTTON_LEVEL HIGH
#define TOUCN_IO A4
// ### Bazzer ###
#define BUZ_IO 3
// ##########
// -------------------------------------------------

// ### LED, Display and buzzer level set ###
#define MAX_LEVEL 255
#define HALF_LEVEL 127
#define MIN_LEVEL 10
// ##########

// ### CO2 limits ###
#define GREEN_LIMIT 800
#define BLUE_LIMIT 1200
#define RED_LIMIT 1600
// ##########

// ### Conditions ###
#define NORMAL_MODE 0
#define SATISFACTORY_MODE 1
#define EXCESS_MODE 2
#define N_COLOR GGreen
#define S_COLOR GYellow
#define E_COLOR GRed
// ##########

// ### Timers ###
#define DISP_REFRESH 2000        // ms
#define CO2_REFRESH 2000         // ms
#define LAUNCH_HEATING_DELAY 7*1000
// ##########

// ### Errors ###
#define CO2_ERR 8888
#define BUZ_ERR 7777
// ##########

const uint8_t level_set[] {MIN_LEVEL, HALF_LEVEL, MAX_LEVEL};
uint16_t ppm;
uint16_t temp;
uint8_t last_mode = 0;

uint8_t button_react();
uint8_t check_ppm(int ppm);
void set_mode(uint8_t mode);

#endif