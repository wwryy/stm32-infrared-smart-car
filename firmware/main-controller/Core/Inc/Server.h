#ifndef __SERVER_H
#define __SERVER_H

#include "main.h"      // ??????? stm32f1xx_hal.h

void TIM5_PWM_Init(uint16_t arr, uint16_t psc);
void SetJointAngle(float angle);

#endif
