#include "main.h"

// ########## Hardware initialization ##########
CO2 co2;
LED led(COMMON_CATHODE, LED_R, LED_G, LED_B);
Button touch(TOUCN_IO);
disp dsp(DISP_CLK, DISP_RST, DISP_OFF, DISP_D1, DISP_D2, DISP_D3, DISP_D4);
// ####################

// ########## Timers ##########
// GTimer_ms disp_ref(DISP_REF*1000);
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
    dsp.on();

    #if(DEBUG)
        Serial.println("Display ready!");
    #endif
    // ###################

    // ########## CO2 ##########
    if(!co2.start(CO2_RX, CO2_TX)) dsp.set_number(CO2_ERR);

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
    buz_status = WORK;
    if(!buz_check()) dsp.set_number(BUZ_ERR);

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
            dsp.loading();
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
    dsp.work();
    // ####################
    // ########## CO2 check ##########
    if(co2_refresh.isReady()) {
        #if(DEBUG)
            Serial.print("Checking ppm... ");
        #endif

        if(switcher){
            ppm = co2.getPPM();
            switcher = !switcher;

            #if(DEBUG)
                Serial.print("Current PPM is: " + (String)CO2 + " ");
                Serial.print("Raw PPM is: " + (String)co2.raw_ppm() + " ");
            #endif

            set_mode(check_ppm(ppm));
            dsp.set_number(ppm);

            #if(DEBUG)
                Serial.println("Mode set to " + (String)check_ppm(CO2));
            #endif
        } else {
            temp = co2.getTemperature();
            switcher = !switcher;

            #if(DEBUG)
                Serial.println("Temperature (C): " + (String)Temp);
            #endif

            dsp.set_number(temp);
        }
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
            
            if(buz_status) do_beep(1);

            static uint8_t disp_level = 0;
            // disp_setBrightness(level_set[disp_level]);
            disp_level++;

            if (disp_level == sizeof(level_set)/sizeof(level_set[0])) {
                disp_level = 0;
            }
            return 1;
        }
        if(touch.getClicks() == 2){
            #if(DEBUG)
                Serial.println("Button clicked twice!");
            #endif

            if(buz_status) do_beep(2);

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
            if(buz_status) do_beep(5);
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

        buz_status = !buz_status;
        return 3;
    default:
        return 0;
    }
}

uint8_t check_ppm(int ppm) {
    if (ppm <= GREEN_LIMIT) return NORMAL_MODE;
    if (ppm <= BLUE_LIMIT) return SATISFACTORY_MODE;
    return EXCESS_MODE;
}

void set_mode(uint8_t mode) {
    if (last_mode != mode) {
        last_mode = mode;
        switch (mode) {
        case NORMAL_MODE:
            led.set_mode(LIGHT);
            led.set_color(N_COLOR);
            if(buz_status) do_beep(2);
            break;
        case SATISFACTORY_MODE:
            led.set_mode(LIGHT);
            led.set_color(S_COLOR);
            if(buz_status) do_beep(3);
            break;
        case EXCESS_MODE:
            led.set_mode(LIGHT);
            led.set_color(E_COLOR);
            if(buz_status) do_beep(7);
            break;
        default:
            break;
        }
    }
}