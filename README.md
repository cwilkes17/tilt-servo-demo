# NUCLEO-F446RE demo – Pot-controlled tilt stabilizer

* **MPU-6050 IMU** on I2C (7 bit address 0x68)  
* **1602 LCD** on I2C backpack (7 bit address 0x27)  
* **Potentiometer** on ADC
* **ES08MA II servo** on PWM

  
* Proportional loop: pot sets target +/-30 deg, servo counters tilt

* Hardware abstraction layer, 2 classes (LCD and IMU) share a I2C referenced bus

## Unit tests on host 

```bash
g++ -std=c++17 -I Tests -I App/imu \
    Tests/test_mpu.cpp App/imu/Mpu6050.cpp \
    -o test_mpu && ./test_mpu
