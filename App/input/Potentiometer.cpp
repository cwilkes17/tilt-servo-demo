#include "main.h"
#include "Potentiometer.hpp"

void Potentiometer::init() { }

uint16_t Potentiometer::readRaw() {

    HAL_ADC_Start(&hadc1);
    HAL_ADC_PollForConversion(&hadc1, HAL_MAX_DELAY);

    return HAL_ADC_GetValue(&hadc1);
}

float Potentiometer::readNorm() {

    return readRaw() * (1.0f / 4095.0f);
}
