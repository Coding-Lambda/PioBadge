#ifndef PioBadge_h
#define PioBadge_h

#if !defined(ADAFRUIT_PYBADGE_M4_EXPRESS)
#error This library is currently only made for PyBadge M4 Express!
#endif

#include <Arduino.h>
#include <Adafruit_LIS3DH.h>

#include "PioBadgeFilesystem.h"
#include "PioBadgeDisplay.h"
#include "PioBadgeInput.h"

#define PIOBADGE_SPEAKER_ENABLE 51
#define PIOBADGE_NEOPIXEL_PIN 8
#define PIOBADGE_NEOPIXEL_NUM 5
#define PIOBADGE_AUDIO_OUT A0

#define PIOBADGE_LIGHT_SENSOR A7
#define PIOBADGE_BATTERY_SENSOR A6

#define PIOBADGE_RIGHT_AUDIO_PIN A0
#define PIOBADGE_LEFT_AUDIO_PIN A1

class PioBadge
{
public:
    PioBadgeInput input;
    PioBadgeFilesystem filesystem;
    PioBadgeDisplay *display;

    bool begin();

    void update();

    void enableSpeaker(bool on);
};

#endif