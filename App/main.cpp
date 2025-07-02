// App/main.cpp  –  Pot-controlled tilt loop with MPU-6050 and ES08MAII servo
#include <cmath>        // asinf
#include <cstdio>       // snprintf
#include <cstdint>      // uint*_t

#include "../Hal/I2cBusHal.hpp"
#include "imu/Mpu6050.hpp"
#include "lcd/Lcd1602.hpp"
#include "input/Potentiometer.hpp"
#include "servo/Servo.hpp"

/* limits and tuning*/
static constexpr float     SET_RANGE_DEG   = 30.0f;
/* 0.5 seems to be the best */
static constexpr float     Kp              = 0.5f;
/* safe range of servo */
static constexpr uint16_t  PULSE_MIN_US    = 1100;
static constexpr uint16_t  PULSE_MAX_US    = 1900;
static constexpr float     RAD2DEG         = 57.29578f;

/* clamp 1100-1900 for safe range of servo*/
static float clamp(float value, float lowerBound, float upperBound)
{
    if (value < lowerBound)
        return lowerBound;

    if (value > upperBound)
        return upperBound;

    return value;
}
/* C++ entry point */
extern "C" void appMain(void)
{
    /* hardware interface objs */
    static I2cBusHal     i2cBus;
    static Mpu6050       imu(i2cBus);
    static Lcd1602       lcd(i2cBus, 0x27);
    static Potentiometer pot;
    static Servo         servo;

    /* initialize peripherals */
    imu.wake();
    lcd.init();
    /* dont need to init pot, doesn't do anything */
    pot.init();
    servo.attach(htim3, TIM_CHANNEL_1);

    /* lcd static headers */
    lcd.setCursor(0,0); lcd.print("Set     Tilt");
    lcd.setCursor(0,1); lcd.print("Err     PWM ");


    /*  loops every 100ms */
    while (true)
    {
        /*  pot set point −30 to +30 deg */
        float setDeg = (pot.readNorm() * 2.0f - 1.0f) * SET_RANGE_DEG;

        /*  imu tilt */
        int16_t axRaw = imu.readAccelX_raw();
        float   g     = -axRaw / 16384.0f;
        float   tiltDeg = asinf(clamp(g, -1.0f, 1.0f)) * RAD2DEG;

        /*  proportional control */
        float error = setDeg - tiltDeg;
        float pulse = 1500.0f - Kp * error * (400.0f / SET_RANGE_DEG);
        pulse = clamp(pulse, PULSE_MIN_US, PULSE_MAX_US);

        servo.writeUs(static_cast<uint16_t>(pulse));

        /* update lcd stats */
        lcd.setCursor(4, 0);
        lcd.print(static_cast<int16_t>(setDeg)); lcd.print(" ");

        lcd.setCursor(13,0);
        lcd.print(static_cast<int16_t>(tiltDeg)); lcd.print(" ");

        lcd.setCursor(4, 1);
        lcd.print( static_cast<int16_t>(error));   lcd.print(" ");

        lcd.setCursor(12,1);
        lcd.print(static_cast<int16_t>(pulse));   lcd.print(" ");

        /* ten times a second */
        HAL_Delay(100);
    }
}
