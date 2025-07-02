#include <cstdio>

#include "Lcd1602.hpp"

/* constructor */
Lcd1602::Lcd1602(I2cBus& bus, uint8_t address7)
    : bus_(bus), addr8bit_(static_cast<uint8_t>(address7) << 1) {}

/*  write nibble */
bool Lcd1602::writeNibble(uint8_t nibble, bool rs)
{
    uint8_t data = (nibble & 0xF0) | (rs ? kRS : 0) | kBL;
    // pulse EN high to low
    if (!bus_.writeByte(addr8bit_ >> 1, 0, data | kEN))       return false;
    delayMs(1);
    if (!bus_.writeByte(addr8bit_ >> 1, 0, data & ~kEN))      return false;
    delayMs(1);
    return true;
}

/* wrappers */
bool Lcd1602::writeCmd(uint8_t cmd)
{
    return writeNibble(cmd, false) && writeNibble(cmd << 4, false);
}
bool Lcd1602::writeData(uint8_t data)
{
    return writeNibble(data, true) && writeNibble(data << 4, true);
}

/* public API */
bool Lcd1602::init()
{
    delayMs(50);
    // reset sequence
    for (int i = 0; i < 3; ++i) {               // reset sequence
        if (!writeNibble(0x30, false))          return false;
        delayMs(5);
    }
    if (!writeNibble(0x20, false))              return false; // 4-bit
    if (!writeCmd(0x28)) return false;          // 4-bit, 2-line
    if (!writeCmd(0x0C)) return false;          // display on, cursor off
    if (!writeCmd(0x06)) return false;          // entry mode
    return writeCmd(0x01);                      // clear display
}

bool Lcd1602::setCursor(uint8_t col, uint8_t row)
{
    static const uint8_t rowAddr[2] = {0x00, 0x40};
    return writeCmd(0x80 | (rowAddr[row & 1] + col));
}
/* both string and int16 acceptable */
bool Lcd1602::print(const char* str)
{
    while (*str)
        if (!writeData(static_cast<uint8_t>(*str++))) return false;
    return true;
}
bool Lcd1602::print(int16_t value)
{
	char buf[7];
	int len = std::snprintf(buf, sizeof(buf), "%d", value);
	if (len <= 0 ) return false;
	return print(buf);
}
