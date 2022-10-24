#define VERSION 'j'

#include <avr/pgmspace.h>
#include "Arduino.h"
#include <Wire.h>
#include "FlowSerialRead.h"
#include "SHCustomProtocol.h"
#include "SHDebouncer.h"
#include "SHRotaryEncoder.h"
#include "SHCommands.h"
#include "SHButtonMatrix.h"
#include "SHRGBLedsNeoPixelFastLed.h"


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------GAMEPAD----------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

#define MICRO_GAMEPAD_ENCODERPRESSTIME 50

#include "SHGamepadAxis.h"

#define GAMEPAD_AXIS_01_ENABLED 0
#define GAMEPAD_AXIS_02_ENABLED 0
#define GAMEPAD_AXIS_03_ENABLED 0


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------ROTARY ENCODERS-----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

#define ENCODER1_CLK_PIN 20
#define ENCODER1_DT_PIN 21
#define ENCODER1_BUTTON_PIN (-1)
#define ENCODER1_ENABLE_PULLUP 1
#define ENCODER1_REVERSE_DIRECTION 0
#define ENCODER1_ENABLE_HALFSTEPS 0

#define ENCODER2_CLK_PIN 18
#define ENCODER2_DT_PIN 19
#define ENCODER2_BUTTON_PIN (-1)
#define ENCODER2_ENABLE_PULLUP 1
#define ENCODER2_REVERSE_DIRECTION 0
#define ENCODER2_ENABLE_HALFSTEPS 0

#define ENCODER3_CLK_PIN 22
#define ENCODER3_DT_PIN 23
#define ENCODER3_BUTTON_PIN (-1)
#define ENCODER3_ENABLE_PULLUP 1
#define ENCODER3_REVERSE_DIRECTION 0
#define ENCODER3_ENABLE_HALFSTEPS 0

#define ENCODER4_CLK_PIN 12
#define ENCODER4_DT_PIN 11
#define ENCODER4_BUTTON_PIN (-1)
#define ENCODER4_ENABLE_PULLUP 1
#define ENCODER4_REVERSE_DIRECTION 0
#define ENCODER4_ENABLE_HALFSTEPS 0


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// ---------------------------------------------------------------------------BUTTON MATRIX------------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

#define BMATRIX_COLS 4
#define BMATRIX_ROWS 3

#define BMATRIX_COL1 7
#define BMATRIX_COL2 8
#define BMATRIX_COL3 9
#define BMATRIX_COL4 10
#define BMATRIX_COL5 2
#define BMATRIX_COL6 2
#define BMATRIX_COL7 2
#define BMATRIX_COL8 2

#define BMATRIX_ROW1 2
#define BMATRIX_ROW2 3
#define BMATRIX_ROW3 4
#define BMATRIX_ROW4 10
#define BMATRIX_ROW5 2
#define BMATRIX_ROW6 2
#define BMATRIX_ROW7 2
#define BMATRIX_ROW8 2

byte BMATRIX_COLSDEF[8] = { BMATRIX_COL1, BMATRIX_COL2, BMATRIX_COL3, BMATRIX_COL4, BMATRIX_COL5, BMATRIX_COL6, BMATRIX_COL7, BMATRIX_COL8 };
byte BMATRIX_ROWSDEF[8] = { BMATRIX_ROW1, BMATRIX_ROW2, BMATRIX_ROW3, BMATRIX_ROW4, BMATRIX_ROW5, BMATRIX_ROW6, BMATRIX_ROW7, BMATRIX_ROW8 };


// ----------------------------------------------------------------------------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------WS2812B RGB LEDS----------------------------------------------------------------------
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------

#define WS2812B_RGBLEDCOUNT 16
#define WS2812B_RIGHTTOLEFT 1




Joystick_ Joystick(JOYSTICK_DEFAULT_REPORT_ID,JOYSTICK_TYPE_JOYSTICK,
                   128, 0,
                   false, false, false,
                   false, false, false,
                   false,
                   GAMEPAD_AXIS_01_ENABLED, GAMEPAD_AXIS_02_ENABLED, GAMEPAD_AXIS_03_ENABLED,
                   false);

SHDebouncer ButtonsDebouncer(10);

SHRGBLedsNeoPixelFastLed shRGBLedsWS2812B;

SHButtonMatrix shButtonMatrix;

SHRotaryEncoder encoder1, encoder2, encoder3, encoder4;
SHRotaryEncoder* SHRotaryEncoders[] = { &encoder1, &encoder2, &encoder3, &encoder4 };

SHCustomProtocol shCustomProtocol;

void buttonMatrixStatusChanged(int buttonId, byte Status) {
    Joystick.setButton(buttonId - 1, Status);
    Joystick.sendState();
}

void UpdateGamepadEncodersState(bool sendState) {
    int btnidx = BMATRIX_COLS * BMATRIX_ROWS;
    unsigned long refTime = millis();
    for (auto & SHRotaryEncoder : SHRotaryEncoders) {
        uint8_t dir = SHRotaryEncoder->getDirection(MICRO_GAMEPAD_ENCODERPRESSTIME, refTime);
        Joystick.setButton(btnidx, dir == 0);
        Joystick.setButton(btnidx + 1, dir == 1);
        Joystick.setButton(btnidx + 2, SHRotaryEncoder->getPressed());

        btnidx += 3;
    }

    if (sendState) Joystick.sendState();

}

void EncoderPositionChanged(int encoderId, int position, byte direction) {
    UpdateGamepadEncodersState(true);
}

void InitEncoders() {
    encoder1.begin(ENCODER1_CLK_PIN, ENCODER1_DT_PIN, ENCODER1_BUTTON_PIN,
                   ENCODER1_REVERSE_DIRECTION, ENCODER1_ENABLE_PULLUP, 1,
                   ENCODER1_ENABLE_HALFSTEPS, EncoderPositionChanged);

    encoder2.begin(ENCODER2_CLK_PIN, ENCODER2_DT_PIN, ENCODER2_BUTTON_PIN,
                   ENCODER2_REVERSE_DIRECTION, ENCODER2_ENABLE_PULLUP, 2,
                   ENCODER2_ENABLE_HALFSTEPS, EncoderPositionChanged);

    encoder3.begin(ENCODER3_CLK_PIN, ENCODER3_DT_PIN, ENCODER3_BUTTON_PIN,
                   ENCODER3_REVERSE_DIRECTION, ENCODER3_ENABLE_PULLUP, 3,
                   ENCODER3_ENABLE_HALFSTEPS, EncoderPositionChanged);

    encoder4.begin(ENCODER4_CLK_PIN, ENCODER4_DT_PIN, ENCODER4_BUTTON_PIN,
                   ENCODER4_REVERSE_DIRECTION, ENCODER4_ENABLE_PULLUP, 4,
                   ENCODER4_ENABLE_HALFSTEPS, EncoderPositionChanged);
}

void UpdateGamepadState() {
    UpdateGamepadEncodersState(false);
    Joystick.sendState();
}

void idle() {
    for (auto & SHRotaryEncoder : SHRotaryEncoders) {
        SHRotaryEncoder->read();
    }

    shButtonMatrix.read();
    if (ButtonsDebouncer.Debounce()) {
        shCustomProtocol.idle();
    }
}

char loop_opt;

void setup() {
    FlowSerialBegin(19200);
    Joystick.begin(false);

    shRGBLedsWS2812B.begin(WS2812B_RGBLEDCOUNT, WS2812B_RIGHTTOLEFT);
    shButtonMatrix.begin(BMATRIX_COLS, BMATRIX_ROWS, BMATRIX_COLSDEF, BMATRIX_ROWSDEF, buttonMatrixStatusChanged);

    InitEncoders();

    shCustomProtocol.setup();
    arqserial.setIdleFunction(reinterpret_cast<IdleFunction>(idle));

    Joystick.sendState();
}

void loop() {
    UpdateGamepadState();
    shCustomProtocol.loop();

    // Wait for data
    if (FlowSerialAvailable() > 0 && FlowSerialTimedRead() == MESSAGE_HEADER) {
         {

            // Read command
            loop_opt = FlowSerialTimedRead();
            if (loop_opt == '1') Command_Hello();
            else if (loop_opt == '2') Command_ModulesCount();
            else if (loop_opt == '8') Command_SetBaudrate();
            else if (loop_opt == 'J') Command_ButtonsCount();
            else if (loop_opt == 'B') Command_SimpleModulesCount();
            else if (loop_opt == 'A') Command_Acq();
            else if (loop_opt == 'N') Command_DeviceName();
            else if (loop_opt == 'I') Command_UniqueId();
            else if (loop_opt == '0') Command_Features();
            else if (loop_opt == '4') Command_RGBLEDSCount();
            else if (loop_opt == '6') {
                shRGBLedsWS2812B.read();
                shRGBLedsWS2812B.show();
                FlowSerialWrite(0x15);
            } else if (loop_opt == 'P') {
                shCustomProtocol.read();
                FlowSerialWrite(0x15);
            } else if (loop_opt == 'X') {
                String xaction = FlowSerialReadStringUntil(' ', '\n');
                if (xaction == F("list")) Command_ExpandedCommandsList();
                else if (xaction == F("mcutype")) Command_MCUType();
                else if (xaction == F("encoderscount")) Command_EncodersCount();
            }
        }
    }
}
