#include "main.h"

// ########## Hardware initialization ##########
CO2 co2;
LED led(COMMON_CATHODE, LED_R, LED_G, LED_B);
Button touch(TOUCN_IO);
// ####################

// ########## Timers ##########
GTimer_ms disp_refresh(DISP_REFRESH);
GTimer_ms co2_refresh(CO2_REFRESH);
GTimer_ms heating(LAUNCH_HEATING_DELAY);
// ####################

// ################################# Setup ##########################################
void setup() {
    #if(DEBUG)
        Serial.begin(9600);
        Serial.println("DEBUG mode activated!");
    #endif

    // ########## Display ##########
    disp_init(DISP_A, DISP_B, DISP_C, DISP_D, DISP_E, DISP_F, DISP_G, DISP_DP, DISP_G1, DISP_G2, DISP_G3, DISP_G4);

    #if(DEBUG)
        Serial.println("Display ready!");
    #endif
    // ###################

    // ########## CO2 ##########
    if(!co2.start(CO2_RX, CO2_TX)) disp_err(CO2_ERR);

    #if(DEBUG)
        ppm = co2.getPPM();
        Serial.println("Current ppm is: " + (String)ppm);
    #endif
    // ####################

    // ########## RGB ##########
    led.setColor(GPurple, MAX_LEVEL);

    #if(DEBUG)
        Serial.print("LED color set. ");
    #endif

    led.set_mode(SHIMMER);

    #if(DEBUG)
        Serial.println("LED mode set to " + (String)SHIMMER);
    #endif
    // ###################

    // ########## Touch button ##########
    touch.setBtnLevel(BUTTON_LEVEL);
    // ####################

    // ########## Buzzer ##########
    set_buzzer(BUZ_IO);
    if(!buz_check()) disp_err(BUZ_ERR);

    #if(DEBUG)
        Serial.println("Buzzer pin set!");
    #endif
    // ####################

    // ########## CO2 heating delay ###########
    #if(DEBUG)
        Serial.println("Staring CO2 sensor heating... ");
    #endif

    while (!co2.warm()) {
        #if(DEBUG)
            Serial.print("LED is " + (String)led.work() + " ");
        #else
            led.work();
        #endif

        #if(DEBUG)
            Serial.print("Touch is " + (String)touch.tick() + " ");
        #else
            touch.tick();
        #endif

        #if(DEBUG)
            Serial.println("Button status is " + (String)button_react());
        #else
            button_react();
        #endif

        #if(DEBUG)
            Serial.println();
        #else
            disp_load();
        #endif

        buz_work();
    }

    #if(DEBUG)
        Serial.println("CO2 warmed up!");
    #endif
    // ####################

    set_mode(NORMAL_MODE);

    #if(DEBUG)
        Serial.println("Mode set to " + (String)NORMAL_MODE);
    #endif
}

// ###################################### Loop ########################################
void loop() {
    // ########## Hardware upd ##########
    #if(DEBUG)
        if (touch.tick()) Serial.println("Button touched");
    #else
        touch.tick();
    #endif
    led.work();
    button_react();
    buz_work();
    disp_upd();
    // ####################
    // ########## CO2 check ##########
    if(co2_refresh.isReady()) {
        #if(DEBUG)
            Serial.print("Checking ppm... ");
        #endif

        ppm = co2.getPPM();
        temp = co2.getTemperature();

        #if(DEBUG)
            Serial.println("Temperature (C): " + (String)Temp);
        #endif

        #if(DEBUG)
            Serial.print("Current PPM is: " + (String)CO2 + " ");
            Serial.print("Raw PPM is: " + (String)co2.raw_ppm() + " ");
        #endif

        set_mode(check_ppm(ppm));

        #if(DEBUG)
            Serial.println("Mode set to " + (String)check_ppm(CO2));
        #endif
    }
    // ####################

    // ########## Sending value to display ##########
    if(disp_refresh.isReady()) {
        disp_post(ppm);

        #if(DEBUG)
            Serial.println("PPM sent to display");
        #endif
        //disp_upd();
    }
    // ###################
}

uint8_t button_react() {
    switch (touch.action()) {
    case EB_CLICKS:
        if(touch.getClicks() == 1){
            #if(DEBUG)
                Serial.println("Button clicked once!");
            #endif

            do_beep(1);

            static uint8_t disp_level = 0;
            disp_setBrightness(level_set[disp_level]);
            disp_level++;

            if (disp_level == sizeof(level_set)/sizeof(level_set[0])) {
                disp_level = 0;
            }
            return 1;
        } else if(touch.getClicks() == 2){
            #if(DEBUG)
                Serial.println("Button clicked twice!");
            #endif

            do_beep(2);

            static uint8_t led_level = 0;
            led.setBrightness(level_set[led_level]);
            led_level++;

            if (led_level == sizeof(level_set)/sizeof(level_set[0])) {
                led_level = 0;
            }
            return 2;
        } else {
            #if(DEBUG)
                Serial.println("Button clicked more then twice!");
            #endif
            do_beep(5);
            return 4;
        }

    // case EB_CLICKS:
    //     #if(DEBUG)
    //     Serial.println("Button clicked few times!");
    //     #endif

    //     static uint8_t led_level = 0;
    //     led.setBrightness(level_set[led_level]);
    //     led_level++;
    //     if (led_level == sizeof(level_set)/sizeof(level_set[0])) {
    //         led_level = 0;
    //     }
    //     return 2;
    case EB_HOLD:
        #if(DEBUG)
            Serial.println("Button holded!");
        #endif

        do_beep(3);

        static uint8_t buz_level = 0;
        // buz_setBrightness(level_set[disp_level]);
        buz_level++;

        if (buz_level == sizeof(level_set)/sizeof(level_set[0])) {
            buz_level = 0;
        }
        return 3;
    default:
        return 0;
    }
}

uint8_t check_ppm(int ppm) {
    if (ppm <= GREEN_LIMIT) {
        return NORMAL_MODE;
    } else if (ppm <= BLUE_LIMIT) {
        return SATISFACTORY_MODE;
    } else {
        return EXCESS_MODE;
    }
}

void set_mode(uint8_t mode) {
    if (last_mode != mode) {
        last_mode = mode;
        switch (mode) {
        case NORMAL_MODE:
            led.set_mode(LIGHT);
            led.set_color(N_COLOR);
            do_beep(3);
            break;
        case SATISFACTORY_MODE:
            led.set_mode(LIGHT);
            led.set_color(S_COLOR);
            do_beep(1);
            break;
        case EXCESS_MODE:
            led.set_mode(LIGHT);
            led.set_color(E_COLOR);
            do_beep(7);
            break;
        default:
            break;
        }
    }
}