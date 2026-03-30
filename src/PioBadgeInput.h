#ifndef PioBadgeInput_h
#define PioBadgeInput_h

#include <Arduino.h>

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

enum PioBadgeButton : uint8_t
{
    NONE = 0x00,
    A = 0x01,
    B = 0x02,
    SELECT = 0x04,
    START = 0x08,
    UP = 0x10,
    DOWN = 0x20,
    LEFT = 0x40,
    RIGHT = 0x80,
};

class PioBadgeInput
{
    uint8_t currentButtons;
    uint8_t lastButtons;
    uint8_t newlyPressedButtons;
    uint8_t newlyReleasedButtons;

public:
    void begin();

    void update();

    bool isPressed(PioBadgeButton button);

    bool isNewlyPressed(PioBadgeButton button);

    bool isNewlyReleased(PioBadgeButton button);
};

#endif