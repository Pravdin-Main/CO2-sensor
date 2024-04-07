#ifndef co2_h
#define co2_h

#include <Arduino.h>
// #include <MHZ19_uart.h>
#include "MHZ19_uart.h"
#include "GyverTimer.h"

#define K1 6.4995
#define K2 590.53

#define WARMING 10     // sec

#define ANALOG 0
#define UART 1
// #define CONNECT_MODE UART       // 0 - analog, 1 - UART (RX/TX)

class CO2: public MHZ19_uart {
    public:
        using MHZ19_uart::MHZ19_uart;
        #if(ANALOG)
            bool start(uint8_t pin);
        #endif
        #if(UART)
            bool start(uint8_t rx, uint8_t tx);
        #endif
        bool warm();
};
#endif