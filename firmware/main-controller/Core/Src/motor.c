#include "motor.h"
#include "tim.h"
#include <stdlib.h>

extern TIM_HandleTypeDef htim4;

//================= 1. TIM4 PWM ??? =================
void TIM4_PWM_Init(void)
{
    // ? CubeMX ???? TIM4_CH3/CH4 ?? PWM2 ??
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);   // PB8 ? PWM
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);   // PB9 ? PWM

    LEFT_MOTOR_GO_RESET;
    RIGHT_MOTOR_GO_RESET;
}

//================= 2. ???????? =================
// ucChannel: 0 = ??, 1 = ??
// cSpeed   : -100 ~ +100
void SetMotorSpeed(uint8_t ucChannel, int8_t cSpeed)
{
    uint32_t period = __HAL_TIM_GET_AUTORELOAD(&htim4);  // ARR
    uint32_t pulse;
    int16_t  spd;

    if (cSpeed > 100)  cSpeed = 100;
    if (cSpeed < -100) cSpeed = -100;

    // ?????? abs,?? char ??
    spd = cSpeed;
    if (spd < 0) spd = -spd;

    // ???????:sPWM = (ARR+1) - |speed|*(ARR+1)/100  (PWM2)
    pulse = (period + 1U) - ((uint32_t)spd * (period + 1U) / 100U);

    switch (ucChannel)
    {
        case 0: // ?? -> TIM4_CH3
            __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_3, pulse);
            if (cSpeed > 0)
                RIGHT_MOTOR_GO_RESET;    // ???:>0 RESET
            else if (cSpeed < 0)
                RIGHT_MOTOR_GO_SET;      // ???:<0 SET
            break;

        case 1: // ?? -> TIM4_CH4
            __HAL_TIM_SET_COMPARE(&htim4, TIM_CHANNEL_4, pulse);
            if (cSpeed > 0)
                LEFT_MOTOR_GO_SET;       // ???:>0 SET
            else if (cSpeed < 0)
                LEFT_MOTOR_GO_RESET;     // ???:<0 RESET
            break;

        default:
            break;
    }
}

//================= 3. ????(??????) =================

// ??
void ZYSTM32_run(int8_t speed,int time)
{
    int8_t f_speed = -speed;
    SetMotorSpeed(1, f_speed);  // ?? ?
    SetMotorSpeed(0, speed);    // ?? ?
    HAL_Delay((uint32_t)time);
}

// ??
void ZYSTM32_brake(int time)
{
    SetMotorSpeed(1, 0);
    SetMotorSpeed(0, 0);
    RIGHT_MOTOR_GO_RESET;
    LEFT_MOTOR_GO_RESET;
    HAL_Delay((uint32_t)time);
}

// ??(???,???)
void ZYSTM32_Left(int8_t speed,int time)
{
    SetMotorSpeed(1, 0);        // ???
    SetMotorSpeed(0, speed);    // ????
    HAL_Delay((uint32_t)time);
}

// ?????
void ZYSTM32_Spin_Left(int8_t speed,int time)
{
    int8_t u_speed = 100 - speed;
    SetMotorSpeed(1, speed);    // ???
    SetMotorSpeed(0, u_speed);  // ???(??)
    HAL_Delay((uint32_t)time);
}

// ??(???,???)
void ZYSTM32_Right(int8_t speed,int time)
{
    int8_t f_speed = -speed;
    SetMotorSpeed(1, f_speed);  // ???
    SetMotorSpeed(0, 0);        // ???
    HAL_Delay((uint32_t)time);
}

// ?????
void ZYSTM32_Spin_Right(int8_t speed,int time)
{
    int8_t u_speed = 100 - speed;
    int8_t f_speed = -speed;
    SetMotorSpeed(1, -u_speed); // ???
    SetMotorSpeed(0, f_speed);  // ???
    HAL_Delay((uint32_t)time);
}

// ??
void ZYSTM32_back(int8_t speed,int time)
{
    int8_t u_speed = 100 - speed;
    int8_t f_speed = -u_speed;
    SetMotorSpeed(1, u_speed);  // ???
    SetMotorSpeed(0, f_speed);  // ???
    HAL_Delay((uint32_t)time);
}

void ZYSTM32_Spin(int time)
{
  
    SetMotorSpeed(1, 60);  // ?? ?
    SetMotorSpeed(0, 30);    // ?? ?
    HAL_Delay((uint32_t)time);
}
