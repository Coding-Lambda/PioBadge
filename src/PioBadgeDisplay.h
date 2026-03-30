#ifndef PioBadgeDisplay_h
#define PioBadgeDisplay_h

#include <Adafruit_ST7735.h>

#ifdef USE_SPI_DMA
#error USE_SPI_DMA should not be defined. SPI DMA does not seem to work with PlatformIO! (crashes on 'static volatile bool dma_busy' variable access)
#endif

#define PIOBADGE_TFT_SPI SPI1
#define PIOBADGE_TFT_CS 44  // Display CS Arduino pin number
#define PIOBADGE_TFT_DC 45  // Display D/C Arduino pin number
#define PIOBADGE_TFT_RST 46 // Display reset Arduino pin number
#define PIOBADGE_TFT_LITE 47
#define PIOBADGE_TFT_ROTATION 1
#define PIOBADGE_TFT_DEFAULTFILL 0x7BEF
#define PIOBADGE_TFT_WIDTH 160
#define PIOBADGE_TFT_HEIGHT 128

class PioBadgeDisplay : public Adafruit_ST7735
{
public:
    PioBadgeDisplay() : Adafruit_ST7735(&PIOBADGE_TFT_SPI, PIOBADGE_TFT_CS, PIOBADGE_TFT_DC, PIOBADGE_TFT_RST) {}

    void begin();
    void setBacklight(uint8_t brightness);
};

#endif