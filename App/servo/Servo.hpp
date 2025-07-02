#pragma once
#include <cstdint>
#include "main.h"

class Servo {
public:
    void attach(TIM_HandleTypeDef& timer, uint32_t channel);
    // write pulse in microseconds 1100–1900 µs
    void writeUs(uint16_t us);

private:
    TIM_HandleTypeDef* timer_   = nullptr;
    uint32_t           channel_ = 0;
};
