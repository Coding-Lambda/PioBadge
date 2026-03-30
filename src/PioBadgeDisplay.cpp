#include "PioBadgeDisplay.h"

void PioBadgeDisplay::begin()
{
    pinMode(PIOBADGE_TFT_LITE, OUTPUT);
    setBacklight(0);

    pinMode(PIOBADGE_TFT_CS, OUTPUT);
    digitalWrite(PIOBADGE_TFT_CS, HIGH);

    initR(INITR_BLACKTAB);
    setRotation(PIOBADGE_TFT_ROTATION);
    fillScreen(PIOBADGE_TFT_DEFAULTFILL);
}

void PioBadgeDisplay::setBacklight(uint8_t brightness)
{
    if (brightness == 0)
    {
        digitalWrite(PIOBADGE_TFT_LITE, LOW);
    }
    else
    {
        analogWrite(PIOBADGE_TFT_LITE, brightness);
    }
}