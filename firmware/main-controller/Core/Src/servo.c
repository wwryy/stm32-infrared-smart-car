#include "servo.h"
#include "tim.h"

void TIM5_PWM_Init(uint16_t arr, uint16_t psc)
{
    (void)arr; (void)psc;
    HAL_TIM_PWM_Start(&htim5, TIM_CHANNEL_1);
}

void SetJointAngle(float angle)
{
    if (angle < 0.0f) angle = 0.0f;
    if (angle > 180.0f) angle = 180.0f;

    uint16_t ccr = (uint16_t)(50.0f * angle / 9.0f + 249.0f);

    __HAL_TIM_SET_COMPARE(&htim5, TIM_CHANNEL_1, ccr);
}
