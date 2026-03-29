#include "PioBadge.h"

bool PioBadge::begin()
{
    

    return true;
}

void PioBadge::enableSpeaker(bool on)
{
    digitalWrite(PIOBADGE_SPEAKER_ENABLE, on);
}
