#ifndef PioBadge_h
#define PioBadge_h

#if !defined(ADAFRUIT_PYBADGE_M4_EXPRESS)
#error This library is currently only made for PyBadge M4 Express!
#endif

// Ugly hack to avoid SPI DMA
#undef ADAFRUIT_PYBADGE_M4_EXPRESS
#include <Adafruit_SPITFT.h>
#define ADAFRUIT_PYBADGE_M4_EXPRESS 1

#ifdef USE_SPI_DMA
#error USE_SPI_DMA should not be defined. SPI DMA does not seem to work with PlatformIO! (crashes on 'static volatile bool dma_busy' variable access)
#endif
// END HACK

#include <Arduino.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_ST7735.h>

#include "PioBadgeFilesystem.h"

#define PIOBADGE_TFT_SPI SPI1
#define PIOBADGE_TFT_CS 44  // Display CS Arduino pin number
#define PIOBADGE_TFT_DC 45  // Display D/C Arduino pin number
#define PIOBADGE_TFT_RST 46 // Display reset Arduino pin number
#define PIOBADGE_TFT_LITE 47
#define PIOBADGE_TFT_ROTATION 1
#define PIOBADGE_TFT_DEFAULTFILL 0x7BEF
#define PIOBADGE_TFT_WIDTH 160
#define PIOBADGE_TFT_HEIGHT 128

#define PIOBADGE_SPEAKER_ENABLE 51
#define PIOBADGE_NEOPIXEL_PIN 8
#define PIOBADGE_NEOPIXEL_NUM 5
#define PIOBADGE_AUDIO_OUT A0
#define PIOBADGE_BUTTON_CLOCK 48
#define PIOBADGE_BUTTON_DATA 49
#define PIOBADGE_BUTTON_LATCH 50
#define PIOBADGE_BUTTON_SHIFTMASK_B 0x80
#define PIOBADGE_BUTTON_SHIFTMASK_A 0x40
#define PIOBADGE_BUTTON_SHIFTMASK_START 0x20
#define PIOBADGE_BUTTON_SHIFTMASK_SELECT 0x10
#define PIOBADGE_BUTTON_SHIFTMASK_LEFT 0x01
#define PIOBADGE_BUTTON_SHIFTMASK_UP 0x02
#define PIOBADGE_BUTTON_SHIFTMASK_DOWN 0x04
#define PIOBADGE_BUTTON_SHIFTMASK_RIGHT 0x08

#define PIOBADGE_LIGHT_SENSOR A7
#define PIOBADGE_BATTERY_SENSOR A6

#define PIOBADGE_RIGHT_AUDIO_PIN A0
#define PIOBADGE_LEFT_AUDIO_PIN A1

class PioBadge
{
public:
    PioBadgeFilesystem filesystem;

    bool begin();
    void enableSpeaker(bool on);
};

#endif