#include "co2.h"

GTimer_ms warming(WARMING*1000);
GTimer_ms period(PERIOD*1000);

#if(ANALOG)
bool CO2::start(uint8_t pin){
    setAnalogPin(pin);
    return true;
}
#endif

#if(UART)
bool CO2::start(uint8_t rx, uint8_t tx){
    begin(rx, tx);
    setAutoCalibration(false);
    delay(500);
    if(getStatus() == 0){
        return true;
    } else {
        return false;
    }
}
#endif

bool CO2::warm(){
    #if(UART)
    // if(warming.isReady()){
    //     return true;
    // } else {
    //     return !isWarming();
    // }
    if(period.isReady()){
        if(getPPM() > 500) return true;
         else return !isWarming();
    }
    if(warming.isReady()){
        return true;
    }
    #elif(ANALOG)
        warming.isReady() ? return true : return false;
    #endif
}