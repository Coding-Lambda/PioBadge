#include "PioBadge.h"

bool PioBadge::begin()
{
    display = new PioBadgeDisplay();

    if (display != nullptr)
    {
        display->begin();
    }

    input.begin();
    sensor.begin();

    return true;
}

void PioBadge::update()
{
    input.update();
}
