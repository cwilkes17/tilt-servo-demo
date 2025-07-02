#pragma once
#include <cstdint>

/*
 * I2cBus abstraction/contract for any I²C link.
 * Concrete classes (e.g., STM32 HAL or a desktop mock) will inherit
 * from this class and implement the two pure-virtual functions.
 */
class I2cBus
{
public:
    virtual ~I2cBus() = default;

    // Write one byte to a device register.
    virtual bool writeByte(uint8_t device7bitAddress,
                           uint8_t registerAddress,
                           uint8_t value) = 0;

    // Read ‘length’ bytes starting at a device register.
    virtual bool readBytes(uint8_t  device7bitAddress,
                           uint8_t  startRegister,
                           uint8_t* buffer,
                           uint8_t  length) = 0;
};
