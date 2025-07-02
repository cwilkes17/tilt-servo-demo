# NUCLEO-F446RE demo – Pot-controlled tilt stabilizer

### Hardware
* **MPU-6050 IMU** on I2C (7 bit address 0x68)  
* **1602 LCD** on I2C backpack (7 bit address 0x27)  
* **Potentiometer** on ADC
* **ES08MA II servo** on PWM

### Description
  
* Proportional control loop: pot sets target +/-30 deg, servo counters tilt, imu is mounted on servo arm for feedback

* Hardware abstraction layer, 2 classes (LCD and IMU) share a I2C referenced bus

* can run unit tests on host-pc w/ dependency injection

### Project layout

- **Core/** – CubeMX-generated startup and HAL init
  - **Inc/** – headers (`main.h`, `stm32f4xx_it.h`, …)
  - **Src/** – sources (`main.c`, `MX_*_Init.c`, `startup.s`)
- **App/** –  C++ application code
  - **imu/** – MPU6050 driver  
  - **lcd/** – 16×2 1602LCD (+ PCF8574 I2C daughterboard) driver  
  - **input/** – potentiometer (ADC) helper  
  - **servo/** – PWM-servo helper  
  - **main.cpp** – main control loop with proptional 
- **Hal/** – thin C++ wrappers around STM32 HAL
  - **I2cBus.hpp** – abstract I2C bus interface  
  - **I2cBusHal.hpp** – concrete implementation using `hi2c1`
- **Tests/** – host-side catch2 unit tests uses mock I2C bus
- **.gitignore** – should be clean atm






## Unit tests on host 

```bash
g++ -std=c++17 -I Tests -I App/imu \
    Tests/test_mpu.cpp App/imu/Mpu6050.cpp \
    -o test_mpu && ./test_mpu
