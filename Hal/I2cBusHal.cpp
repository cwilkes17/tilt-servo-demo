#include <cstdint>          // uint8_t
#include "I2cBusHal.hpp"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

/* writeByte : write one byte to a device register using STM32 HAL */
bool I2cBusHal::writeByte(uint8_t device7bitAddress,
                          uint8_t registerAddress,
                          uint8_t value)
{
    /* HAL expects the 7-bit address left-shifted by 1 (bit-0 = R/W). */
    const uint16_t addr8bit = static_cast<uint16_t>(device7bitAddress) << 1;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(&hi2c1,
                                                 addr8bit,
                                                 registerAddress,
                                                 I2C_MEMADD_SIZE_8BIT,
                                                 &value,
                                                 1,
                                                 HAL_MAX_DELAY);
    return (status == HAL_OK);
}

/* ------------------------------------------------------------------
 * readBytes  read length bytes starting at a register.
 * ----------------------------------------------------------------*/
bool I2cBusHal::readBytes(uint8_t  device7bitAddress,
                          uint8_t  startRegister,
                          uint8_t* buffer,
                          uint8_t  length)
{
    const uint16_t addr8bit = static_cast<uint16_t>(device7bitAddress) << 1;

    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(&hi2c1,
                                                addr8bit,
                                                startRegister,
                                                I2C_MEMADD_SIZE_8BIT,
                                                buffer,
                                                length,
                                                HAL_MAX_DELAY);
    return (status == HAL_OK);
}
