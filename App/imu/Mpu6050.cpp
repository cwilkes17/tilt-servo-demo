#include "Mpu6050.hpp"

/* constructor gets shared bus  */
Mpu6050::Mpu6050(I2cBus& bus) : bus_(bus) {}

/* clear sleep bit in 0x6B  */
bool Mpu6050::wake()
{
    constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
    constexpr uint8_t VALUE_WAKE     = 0x00;

    return bus_.writeByte(kAddr, REG_PWR_MGMT_1, VALUE_WAKE);
}


int16_t Mpu6050::readAccelX_raw()
{
    constexpr uint8_t REG_AXOUT_H = 0x3B;
    uint8_t raw[2] {0, 0};

    bool ok = bus_.readBytes(kAddr, REG_AXOUT_H, raw, 2);
    if (!ok) return 0;

    return static_cast<int16_t>((raw[0] << 8) | raw[1]);
}
