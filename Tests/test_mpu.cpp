// Tests/test_mpu.cpp

// ─────── Disable Catch2 signal handlers ───────
#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_NO_POSIX_SIGNALS
#define CATCH_INTERNAL_CONFIG_NO_POSIX_SIGNALS

#include "catch.hpp"
#include "I2cBusMock.hpp"
#include "../App/imu/Mpu6050.hpp"

TEST_CASE("MPU6050 readAccelX_raw returns correct combined value", "[Mpu6050]")
{
    I2cBusMock mockBus;
    const uint8_t dev   = 0x68;
    const uint8_t regHi = 0x3B;
    const uint8_t regLo = 0x3C;

    // preset high and low bytes
    mockBus.preset(dev, regHi, 0x12);
    mockBus.preset(dev, regLo, 0x34);

    Mpu6050 imu(mockBus);
    REQUIRE( imu.readAccelX_raw() == static_cast<int16_t>(0x1234) );
}

TEST_CASE("MPU6050 readAccelX_raw returns 0 on read failure", "[Mpu6050]")
{
    struct BadBus : I2cBusMock {
        bool readBytes(uint8_t, uint8_t, uint8_t*, uint8_t) override {
            return false;
        }
    } badBus;

    Mpu6050 imu(badBus);
    REQUIRE( imu.readAccelX_raw() == 0 );
}
