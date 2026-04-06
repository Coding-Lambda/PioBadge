#include "PioBadgeAudio.h"

void PioBadgeAudio::enableSpeaker(bool on)
{
    digitalWrite(PIOBADGE_SPEAKER_ENABLE, on);
}

void PioBadgeAudio::playToneBlocking(float frequency, uint16_t duration)
{
    const float stepVal = frequency / this->sampleRate;
    float radVal = 0;

    uint32_t prior = micros();
    uint32_t usec = 1000000L / this->sampleRate;

    analogWriteResolution(8);

    int32_t microsLeft = duration * 1000;

    while (microsLeft > 0)
    {
        uint8_t audio = (sinf(radVal) + 1.0F) * 0xFF;

        radVal += stepVal;

        while (radVal >= radianMax)
        {
            radVal -= radianMax;
        }

        uint32_t t;
        while ((t = micros()) - prior < usec)
            ;

        analogWrite(PIOBADGE_AUDIO_OUT, (uint16_t)audio / 8);

        microsLeft -= (t - prior);
        prior = t;
    }
}

/// @brief Plays melodies based on the "Arduino Songs" format from https://github.com/robsoncouto/arduino-songs
/// @param melody 
/// @param melodySize 
/// @param wholeNoteDuration 
void PioBadgeAudio::playMelodyBlocking(int *melody, int melodySize, int wholeNoteDuration)
{
    int notes = melodySize / sizeof(melody[0]) / 2;

    for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2)
    {
        // calculates the duration of each note
        int divider = melody[thisNote + 1];

        int noteDuration;
        if (divider > 0)
        {
            // regular note, just proceed
            noteDuration = wholeNoteDuration / divider;
        }
        else if (divider < 0)
        {
            // dotted notes are represented with negative durations!!
            noteDuration = wholeNoteDuration / abs(divider);
            noteDuration *= 1.5; // increases the duration in half for dotted notes
        }
        else
        {
            continue;
        }

        // we only play the note for 90% of the duration, leaving 10% as a pause
        this->playToneBlocking(melody[thisNote] << 2, noteDuration);

        // Wait for the specief duration before playing the next note.
        delay(noteDuration);
    }
}
