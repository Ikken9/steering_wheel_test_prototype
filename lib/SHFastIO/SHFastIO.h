#ifndef __SHFASTIO_H__
#define __SHFASTIO_H__

#include <Arduino.h>

class FastDigitalPin {
private:
	uint8_t bit = 0;
	uint8_t port = 0;

public:
    void begin(int pin) {
        bit = digitalPinToBitMask(pin);
        port = digitalPinToPort(pin);
	}

	int digitalRead() const {
		if (*portInputRegister(port) & bit) return HIGH;
		return LOW;
	}
};

#endif