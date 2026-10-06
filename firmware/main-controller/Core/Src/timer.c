#include "timer.h"

TIM_HandleTypeDef htim2;

void Timerx_Init(uint16_t arr, uint16_t psc)
{
    __HAL_RCC_TIM2_CLK_ENABLE();

    htim2.Instance = TIM2;
    htim2.Init.Prescaler         = psc;
    htim2.Init.CounterMode       = TIM_COUNTERMODE_UP;
    htim2.Init.Period            = arr;
    htim2.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
    htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

    HAL_TIM_Base_Init(&htim2);

    __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
    __HAL_TIM_ENABLE_IT(&htim2, TIM_IT_UPDATE);

    HAL_NVIC_SetPriority(TIM2_IRQn, 0, 3);
    HAL_NVIC_EnableIRQ(TIM2_IRQn);

    // HAL_TIM_Base_Start_IT(&htim2);
}

void TIM2_IRQHandler(void)
{
    if (__HAL_TIM_GET_IT_SOURCE(&htim2, TIM_IT_UPDATE) != RESET
        && __HAL_TIM_GET_FLAG(&htim2, TIM_FLAG_UPDATE) != RESET)
    {
        __HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
    }
}
