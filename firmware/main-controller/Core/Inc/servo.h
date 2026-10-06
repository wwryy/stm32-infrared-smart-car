#ifndef SMART_CAR_SERVO_H
#define SMART_CAR_SERVO_H

#include "main.h"

void TIM5_PWM_Init(uint16_t arr, uint16_t psc);
void SetJointAngle(float angle);

#endif /* SMART_CAR_SERVO_H */
