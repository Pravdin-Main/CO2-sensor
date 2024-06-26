#include "GyverTimer.h"
#include <Arduino.h> 

GTimer_ms::GTimer_ms() {}

GTimer_ms::GTimer_ms(uint32_t interval) {
	_interval = interval;
	_timer = millis();
}

void GTimer_ms::start() {
	_state = true;
}
void GTimer_ms::stop() {
	_state = false;
}
boolean GTimer_ms::isReady() {
	if (!_state) return false;
	if ((long)millis() - _timer >= _interval) {
		if (_mode) _timer = millis();
		return true;
	} else {
		return false;
	}
}

void GTimer_ms::reset() {
	_timer = millis();
    _state = true;
}

GTimer_us::GTimer_us() {}

GTimer_us::GTimer_us(uint32_t interval) {
	_interval = interval;
	_timer = micros();
}

void GTimer_us::start() {
	_state = true;
}
void GTimer_us::stop() {
	_state = false;
}
boolean GTimer_us::isReady() {
	if (!_state) return false;
	if ((long)micros() - _timer >= _interval) {
		if (_mode) _timer = micros();
		return true;
	} else {
		return false;
	}
}

void GTimer_us::reset() {
	_timer = micros();
    _state = true;
}