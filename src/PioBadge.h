#ifndef PioBadge_h
#define PioBadge_h

#if !defined(ADAFRUIT_PYBADGE_M4_EXPRESS)
#error This library is currently only made for PyBadge M4 Express!
#endif

#include <Arduino.h>
#include <Adafruit_LIS3DH.h>

#include "PioBadgeAudio.h"
#include "PioBadgeDisplay.h"
#include "PioBadgeFilesystem.h"
#include "PioBadgeInput.h"
#include "PioBadgeSensor.h"

#define PIOBADGE_NEOPIXEL_PIN 8
#define PIOBADGE_NEOPIXEL_NUM 5

class PioBadge
{
public:
    PioBadgeAudio audio;
    PioBadgeDisplay *display;
    PioBadgeFilesystem filesystem;
    PioBadgeInput input;
    PioBadgeSensor sensor;

    bool begin();

    void update();
};

#endif