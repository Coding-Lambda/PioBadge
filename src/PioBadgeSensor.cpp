#include "PioBadgeSensor.h"

void PioBadgeSensor::begin()
{
    accel = new Adafruit_LIS3DH();
    if (!accel->begin(0x18) && !accel->begin(0x19))
    {
        delete accel;

        accel = nullptr;
    }
    else
    {
        accel->setRange(LIS3DH_RANGE_4_G); // 2, 4, 8 or 16 G!
    }
}

uint16_t PioBadgeSensor::getLightLevel()
{
    return analogRead(PIOBADGE_LIGHT_SENSOR);
}

float PioBadgeSensor::getBatteryLevel()
{
    return ((float)analogRead(PIOBADGE_BATTERY_SENSOR) / 1023.0) * 2.0 * 3.3; // ?
}

Vector3F PioBadgeSensor::getAcceleration()
{
    if (this->accel == nullptr)
        return {};

    sensors_event_t data;

    if (!this->accel->getEvent(&data))
        return {};

    Vector3F result;
    result.x = data.acceleration.x;
    result.y = data.acceleration.y;
    result.z = data.acceleration.z;

    return result;
}

size_t Vector3F::printTo(Print &p) const
{
    size_t total = p.print(F("{ x = "));
    total += p.print(x);
    total += p.print(F(", y = "));
    total += p.print(y);
    total += p.print(F(", z = "));
    total += p.print(z);
    total += p.print(F(" }"));

    return total;
}
