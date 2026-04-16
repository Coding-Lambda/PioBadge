#include "PioRandom.h"

void PioRandom::initState(uint64_t seed)
{
    uint64_t splitMixState = seed;

    for (size_t index = 0; index < 4; index++)
    {
        // quick splitmix64 implementation to generate the 4 states
        uint64_t temp = splitMixState + 0x9e3779b97f4a7c15;
        splitMixState = temp;
        temp = (temp ^ (temp >> 30)) * 0xbf58476d1ce4e5b9;
        temp = (temp ^ (temp >> 27)) * 0x94d049bb133111eb;
        state[index] = temp ^ (temp >> 31);
    }
}

uint64_t PioRandom::rotateLeft64(uint64_t x, int k)
{
    return (x << k) | (x >> (64 - k));
}

PioRandom::PioRandom() : PioRandom(random() + micros()) // TODO: better random seed
{
}

PioRandom::PioRandom(uint64_t seed)
{
    initState(seed);
}

uint64_t PioRandom::nextUInt64()
{
    const uint64_t result = rotateLeft64(state[1] * 5, 7) * 9;
    const uint64_t t = state[1] << 17;

    state[2] ^= state[0];
    state[3] ^= state[1];
    state[1] ^= state[2];
    state[0] ^= state[3];

    state[2] ^= t;
    state[3] = rotateLeft64(state[3], 45);

    return result;
}

double PioRandom::nextDouble()
{
    return (nextUInt64() >> 11) * (1.0 / (1ULL << 53));
}

float PioRandom::nextFloat()
{
    return (nextUInt64() >> 40) * (1.0f / (1UL << 24));
}

uint64_t PioRandom::nextUInt64(uint64_t maxValue)
{
    return nextDouble() * maxValue;
}

uint64_t PioRandom::nextUInt64(uint64_t minValue, uint64_t maxValue)
{
    return (nextDouble() * (maxValue - minValue)) + minValue;
}
