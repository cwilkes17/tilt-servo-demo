#include "../../Core/Inc/main.h"
#include "Servo.hpp"

/* determine real clock in hz */
static uint32_t getTimerClock(TIM_HandleTypeDef* t)
{
    bool apb2 = (t->Instance == TIM1 || t->Instance == TIM8
              || t->Instance == TIM9 || t->Instance == TIM10
              || t->Instance == TIM11);

    uint32_t pclk = apb2 ? HAL_RCC_GetPCLK2Freq()
                         : HAL_RCC_GetPCLK1Freq();

    /* if pre-scaler not 1, timer clock = 2xPCLK */
    if (apb2)
    {
        if ((RCC->CFGR & RCC_CFGR_PPRE2_Msk) != RCC_CFGR_PPRE2_DIV1) pclk *= 2;
    } else
    {
        if ((RCC->CFGR & RCC_CFGR_PPRE1_Msk) != RCC_CFGR_PPRE1_DIV1) pclk *= 2;
    }
    return pclk / (t->Init.Prescaler + 1);
}

void Servo::attach(TIM_HandleTypeDef& timer, uint32_t channel)
{
    timer_   = &timer;
    channel_ = channel;
    HAL_TIM_PWM_Start(timer_, channel_);
}

void Servo::writeUs(uint16_t us)
{
    if (!timer_) return;
    // 1 tick a µs at 1 mhz
    uint32_t ticksPerUs = getTimerClock(timer_) / 1'000'000U;
    uint32_t compare = us * ticksPerUs;
    __HAL_TIM_SET_COMPARE(timer_, channel_, compare);
}
