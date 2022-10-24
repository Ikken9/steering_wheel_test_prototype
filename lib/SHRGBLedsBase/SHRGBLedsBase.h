#ifndef __SHRGBLEDSBASE_H__
#define __SHRGBLEDSBASE_H__

#include <Arduino.h>

class SHRGBLedsBase {
protected:
	int _maxLeds{};
	int _rightToLeft{};

    virtual void begin(int maxLeds, int rightToLeft) {
		_maxLeds = maxLeds;
		_rightToLeft = rightToLeft;
	}

	virtual void setPixelColor(uint8_t lednumber, uint8_t r, uint8_t g, uint8_t b);

public:

	virtual void show();

	void clear() {
		for (int led = 0; led < _maxLeds; led++) {
			setPixelColor(led, 0, 0, 0);
		}
		show();
	}

	void read() {
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t j;
		int mode = FlowSerialTimedRead();
		while (mode > 0) {
			// Read all
			if (mode == 1) {
				for (j = 0; j < _maxLeds; j++) {
					r = FlowSerialTimedRead();
					g = FlowSerialTimedRead();
					b = FlowSerialTimedRead();

					if (_rightToLeft == 1) {
						setPixelColor(_maxLeds - j - 1, r, g, b);
					} else {
						setPixelColor(j, r, g, b);
					}
				}
			}

			// partial led data
			else if (mode == 2) {
				int startled = FlowSerialTimedRead();
				int numleds = FlowSerialTimedRead();

				for (j = startled; j < startled + numleds; j++) {
					/*	if (ENABLE_BLUETOOTH == 0) {*/
					r = FlowSerialTimedRead();
					g = FlowSerialTimedRead();
					b = FlowSerialTimedRead();

					if (_rightToLeft == 1) {
						setPixelColor(_maxLeds - j - 1, r, g, b);
					} else {
						setPixelColor(j, r, g, b);
					}
				}
			}

			// repeated led data
			else if (mode == 3) {
				int startled = FlowSerialTimedRead();
				int numleds = FlowSerialTimedRead();

				r = FlowSerialTimedRead();
				g = FlowSerialTimedRead();
				b = FlowSerialTimedRead();

				for (j = startled; j < startled + numleds; j++) {
					if (_rightToLeft == 1) {
						setPixelColor(_maxLeds - j - 1, r, g, b);
					} else {
						setPixelColor(j, r, g, b);
					}
				}
			}

			mode = FlowSerialTimedRead();
		}

		if (_maxLeds > 0) show();
	}
};

#endif