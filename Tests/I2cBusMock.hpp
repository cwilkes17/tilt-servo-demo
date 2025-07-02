#pragma once
#include "../Hal/I2cBus.hpp"
#include <cstdint>
#include <map>

/**
 * I2cBusMock  —  desktop mock for I2cBus interface.
 * Stores register values in a map so you can preset and verify behavior.
 */
class I2cBusMock : public I2cBus
{
public:
    I2cBusMock() = default;

    /**
     * Preset a register value before running tests.
     */
    void preset(uint8_t device7bitAddr,
                uint8_t registerAddr,
                uint8_t value)
    {
        memory_[key(device7bitAddr, registerAddr)] = value;
    }

    bool writeByte(uint8_t device7bitAddr,
                   uint8_t registerAddr,
                   uint8_t value) override
    {
        memory_[key(device7bitAddr, registerAddr)] = value;
        return true;
    }

    bool readBytes(uint8_t device7bitAddr,
                   uint8_t startRegister,
                   uint8_t* buffer,
                   uint8_t length) override
    {
        for (uint8_t i = 0; i < length; ++i)
        {
            auto it = memory_.find(key(device7bitAddr, startRegister + i));
            buffer[i] = (it != memory_.end()) ? it->second : 0xFF;
        }
        return true;
    }

private:
    using Key = uint16_t;
    static Key key(uint8_t dev, uint8_t reg)
    {
        return static_cast<Key>(dev) << 8 | reg;
    }

    std::map<Key, uint8_t> memory_;
};
