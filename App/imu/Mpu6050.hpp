#pragma once
#include <cstdint>
#include "../../Hal/I2cBus.hpp"

/* simple imu class for mpu6050 */
class Mpu6050
{
public:
    explicit Mpu6050(I2cBus& bus);

    /* returns false on i2c error */
    bool   wake();
    /* raw register value from sensor */
    int16_t readAccelX_raw();

private:
    /* reference to shared bus */
    I2cBus& bus_;
    /* 7-bit device address */
    static constexpr uint8_t kAddr = 0x68;
};
