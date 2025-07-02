#pragma once
#include <cstdint>
#include "../../Hal/I2cBus.hpp"
#include "main.h"

/* lcd1602  */
class Lcd1602
{
public:
    /* construct with shared i2c bus and 7-bit device address. */
    Lcd1602(I2cBus& bus, uint8_t address7 = 0x27);

    bool init();
    bool setCursor(uint8_t col, uint8_t row);
    /* can print both string and int */
    bool print(const char* str);
    bool print(int16_t value);

private:
    /*  helpers return false on i2c error) */
    bool writeNibble(uint8_t nibble, bool rs);
    bool writeCmd(uint8_t cmd);
    bool writeData(uint8_t data);

    void delayMs(uint32_t ms) { HAL_Delay(ms); }

    I2cBus&  bus_;
    /* left-shifted address! */
    uint8_t  addr8bit_;
    static constexpr uint8_t kEN = 0x04;
    static constexpr uint8_t kRS = 0x01;
    static constexpr uint8_t kBL = 0x08;
};
