#ifndef __SHRGBLEDSNEOPIXELFASTLED_H__
#define __SHRGBLEDSNEOPIXELFASTLED_H__
#define FASTLED_ALLOW_INTERRUPTS 0


#include <Arduino.h>
#include "SHRGBLedsBase.h"
#include <FastLED.h>

CRGB CSWRGBLedsNeoPixelFastLeds_leds[16]; // WS2812B_RGBLEDCOUNT

class SHRGBLedsNeoPixelFastLed : public SHRGBLedsBase {

public:
    void begin(int ledsCount, int rightToLeft) override {
		SHRGBLedsBase::begin(ledsCount, rightToLeft);
		CFastLED::addLeds<NEOPIXEL, 6>(CSWRGBLedsNeoPixelFastLeds_leds, ledsCount); // 6 = WS2812B_DATAPIN
		show();
	}

    void test(int ledsCount, int rightToLeft) {
        SHRGBLedsBase::begin(ledsCount, rightToLeft);
        CFastLED::addLeds<NEOPIXEL, 6>(CSWRGBLedsNeoPixelFastLeds_leds, ledsCount);

        for (int i = 0; i < ledsCount; i++) {
            setPixelColor(i, 255, 0, 0);
        }
        show();
    }

	void show() override {
		FastLED.show();
	}

protected:
	void setPixelColor(uint8_t lednumber, uint8_t r, uint8_t g, uint8_t b) override {
        CSWRGBLedsNeoPixelFastLeds_leds[lednumber].setRGB(r, g, b);
	}
};

#endif
