#include "PioBadgeInput.h"

void PioBadgeInput::begin()
{
    pinMode(PIOBADGE_BUTTON_CLOCK, OUTPUT);
    digitalWrite(PIOBADGE_BUTTON_CLOCK, HIGH);

    pinMode(PIOBADGE_BUTTON_LATCH, OUTPUT);
    digitalWrite(PIOBADGE_BUTTON_LATCH, HIGH);

    pinMode(PIOBADGE_BUTTON_DATA, INPUT);
}

void PioBadgeInput::update()
{
    uint8_t buttons = 0;
    uint8_t shift_buttons = 0;

    digitalWrite(PIOBADGE_BUTTON_LATCH, LOW);
    delayMicroseconds(1);
    digitalWrite(PIOBADGE_BUTTON_LATCH, HIGH);
    delayMicroseconds(1);

    for (int i = 0; i < 8; i++)
    {
        shift_buttons <<= 1;
        shift_buttons |= digitalRead(PIOBADGE_BUTTON_DATA);
        digitalWrite(PIOBADGE_BUTTON_CLOCK, HIGH);
        delayMicroseconds(1);
        digitalWrite(PIOBADGE_BUTTON_CLOCK, LOW);
        delayMicroseconds(1);
    }

    if (shift_buttons & PIOBADGE_BUTTON_SHIFTMASK_B)
        buttons |= PioBadgeButton::B;
    if (shift_buttons & PIOBADGE_BUTTON_SHIFTMASK_A)
        buttons |= PioBadgeButton::A;
    if (shift_buttons & PIOBADGE_BUTTON_SHIFTMASK_SELECT)
        buttons |= PioBadgeButton::SELECT;
    if (shift_buttons & PIOBADGE_BUTTON_SHIFTMASK_START)
        buttons |= PioBadgeButton::START;
    if (shift_buttons & PIOBADGE_BUTTON_SHIFTMASK_UP)
        buttons |= PioBadgeButton::UP;
    if (shift_buttons & PIOBADGE_BUTTON_SHIFTMASK_DOWN)
        buttons |= PioBadgeButton::DOWN;
    if (shift_buttons & PIOBADGE_BUTTON_SHIFTMASK_LEFT)
        buttons |= PioBadgeButton::LEFT;
    if (shift_buttons & PIOBADGE_BUTTON_SHIFTMASK_RIGHT)
        buttons |= PioBadgeButton::RIGHT;

    // // GPIO buttons!
    // if ((_start_button >= 0) && !digitalRead(_start_button))
    //     buttons |= ARCADA_BUTTONMASK_START;

    // if ((_select_button >= 0) && !digitalRead(_select_button))
    //     buttons |= ARCADA_BUTTONMASK_SELECT;

    // if ((_a_button >= 0) && !digitalRead(_a_button))
    //     buttons |= ARCADA_BUTTONMASK_A;

    // if ((_b_button >= 0) && !digitalRead(_b_button))
    //     buttons |= ARCADA_BUTTONMASK_B;

    // if ((_up_button >= 0) && !digitalRead(_up_button))
    //     buttons |= ARCADA_BUTTONMASK_UP;
    // if ((_down_button >= 0) && !digitalRead(_down_button))
    //     buttons |= ARCADA_BUTTONMASK_DOWN;
    // if ((_left_button >= 0) && !digitalRead(_left_button))
    //     buttons |= ARCADA_BUTTONMASK_LEFT;
    // if ((_right_button >= 0) && !digitalRead(_right_button))
    //     buttons |= ARCADA_BUTTONMASK_RIGHT;

    Serial.println(buttons, 2);

    lastButtons = currentButtons;
    currentButtons = buttons;
    newlyPressedButtons = (lastButtons ^ currentButtons) & currentButtons;
    newlyReleasedButtons = (lastButtons ^ currentButtons) & lastButtons;
}

bool PioBadgeInput::isPressed(PioBadgeButton button)
{
    return (this->currentButtons & button) == button;
}

bool PioBadgeInput::isNewlyPressed(PioBadgeButton button)
{
    return (this->newlyPressedButtons & button) == button;
}

bool PioBadgeInput::isNewlyReleased(PioBadgeButton button)
{
    return (this->newlyReleasedButtons & button) == button;
}
