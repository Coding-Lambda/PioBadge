#ifndef PioRandom_h
#define PioRandom_h

#include <Arduino.h>

/**
 * A random number generator class based on xoshiro256**.
 */
class PioRandom
{
private:
    uint64_t state[4];

    void initState(uint64_t seed);

    uint64_t rotateLeft64(uint64_t x, int k);

public:
    PioRandom();

    PioRandom(uint64_t seed);

    uint64_t nextUInt64();    

    double nextDouble();

    float nextFloat();    

    uint64_t nextUInt64(uint64_t maxValue);
    
    uint64_t nextUInt64(uint64_t minValue, uint64_t maxValue);
};

#endif