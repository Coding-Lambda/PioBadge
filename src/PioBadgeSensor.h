#ifndef PioBadgeSensor_h
#define PioBadgeSensor_h

#include <Arduino.h>
#include <Adafruit_LIS3DH.h>

#define PIOBADGE_LIGHT_SENSOR A7
#define PIOBADGE_BATTERY_SENSOR A6

struct Vector3F : public Printable
{
    float x = 0;
    float y = 0;
    float z = 0;

    virtual size_t printTo(Print &p) const;
};

class PioBadgeSensor
{
public:
    Adafruit_LIS3DH *accel = NULL;

    void begin();

    /// @brief Reads the light level of the sensor.
    /// @return The light level in range of 0 to 1023 (the higher, the brighter)
    uint16_t getLightLevel();

    /// @brief Reads the charge level of the battery
    /// @return The charge level in Volts.
    float getBatteryLevel();

    /// @brief Reads the acceleration sensor.
    /// @return The current acceleration as a vector of 3 floats.
    Vector3F getAcceleration();
};

#endif