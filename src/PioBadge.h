#ifndef PioBadge_h
#define PioBadge_h

#if !defined(ADAFRUIT_PYBADGE_M4_EXPRESS)
#error This library is currently only made for PyBadge M4 Express!
#endif

#include <Arduino.h>
#include <Adafruit_LIS3DH.h>

#include "PioBadgeDisplay.h"
#include "PioBadgeFilesystem.h"
#include "PioBadgeInput.h"
#include "PioBadgeSensor.h"

#define PIOBADGE_SPEAKER_ENABLE 51
#define PIOBADGE_NEOPIXEL_PIN 8
#define PIOBADGE_NEOPIXEL_NUM 5
#define PIOBADGE_AUDIO_OUT A0

#define PIOBADGE_RIGHT_AUDIO_PIN A0
#define PIOBADGE_LEFT_AUDIO_PIN A1

class PioBadge
{
public:
    PioBadgeDisplay *display;
    PioBadgeFilesystem filesystem;
    PioBadgeInput input;
    PioBadgeSensor sensor;

    bool begin();

    void update();

    void enableSpeaker(bool on);
};

#endif