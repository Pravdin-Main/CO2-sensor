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
#define LED_R 11
#define LED_G 10
#define LED_B 9
// ### 4-digits display ###
#define DISP_CLK 6
#define DISP_RST 8
#define DISP_OFF 7
#define DISP_D1 A0
#define DISP_D2 A1
#define DISP_D3 A2
#define DISP_D4 A3
// ### CO2 sensor ###
#define CO2_TX 12
#define CO2_RX 13
#define CO2_A A4
// ### Touch button ###
#define BUTTON_LEVEL HIGH
#define TOUCN_IO 4
// ### Bazzer ###
#define BUZ_IO 3
#define WORK true
#define OFF false
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
#define NORMAL_MODE 1
#define SATISFACTORY_MODE 2
#define EXCESS_MODE 3
#define N_COLOR GGreen
#define S_COLOR GBlue //GYellow
#define E_COLOR GRed
// ##########

// ### Timers ###
// #define DISP_REF 1         // sec
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
bool switcher;
bool buz_status;

uint8_t button_react();
uint8_t check_ppm(int ppm);
void set_mode(uint8_t mode);

#endif