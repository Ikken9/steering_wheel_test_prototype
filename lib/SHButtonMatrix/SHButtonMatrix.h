#ifndef __SHBUTTONMATRIX_H__
#define __SHBUTTONMATRIX_H__

#include <Arduino.h>
#include "SHDebouncer.h"

typedef void(*SHButtonMatrixChanged) (int, byte);

class SHButtonMatrix {

private:
	int lastPressedButton{};
	unsigned long buttonLastStateChanged{};

	SHButtonMatrixChanged shButtonChangedCallback{};
	SHDebouncer debouncer;

	byte rowCount{};
	byte colCount{};
	byte * colPins{};
	byte * rowPins{};

public:
	void begin(byte cols, byte rows, byte * col, byte * row, SHButtonMatrixChanged changedcallback) {
		debouncer.begin(10);
		rowCount = rows;
		colCount = cols;
		colPins = col;
		rowPins = row;

		for (int x = 0; x < rowCount; x++) {
			pinMode(rowPins[x], INPUT);
		}

		for (int x = 0; x < colCount; x++) {
			pinMode(colPins[x], INPUT_PULLUP);
		}

		shButtonChangedCallback = changedcallback;
	}

	void read() {
		if (debouncer.Debounce()) {
			int pressedButton = -1;
			if (buttonLastStateChanged - millis() > 50) {
				for (int colIndex = 0; colIndex < colCount; colIndex++) {
					byte curCol = colPins[colIndex];
					pinMode(curCol, OUTPUT);
					digitalWrite(curCol, LOW);

					for (int rowIndex = 0; rowIndex < rowCount; rowIndex++) {
						byte rowCol = rowPins[rowIndex];
						pinMode(rowCol, INPUT_PULLUP);
						if (digitalRead(rowCol) == LOW) {
							pressedButton = rowIndex * colCount + colIndex + 1;
						}
					}

					pinMode(curCol, INPUT);
				}

				if (pressedButton != lastPressedButton) {

					if (lastPressedButton != -1) {
						shButtonChangedCallback(lastPressedButton, 0);
					}
					if (pressedButton != -1) {
						shButtonChangedCallback(pressedButton, 1);
					}
					buttonLastStateChanged = millis();
				}

				lastPressedButton = pressedButton;
			}
		}
	}
};

#endif