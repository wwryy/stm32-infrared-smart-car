#ifndef __IRAVOID_H_
#define __IRAVOID_H_

#include "main.h"   // ?????? stm32f1xx_hal.h

void IRAvoidInit(void);
void AVoidRun(void);

// ===================== ????????? =====================
// ????:PB1
#define AVOID_RIGHT_PIN         GPIO_PIN_1
#define AVOID_RIGHT_PIN_GPIO    GPIOB
#define AVOID_RIGHT_IO          HAL_GPIO_ReadPin(AVOID_RIGHT_PIN_GPIO, AVOID_RIGHT_PIN)

// ????:PA8
#define AVOID_LEFT_PIN          GPIO_PIN_8
#define AVOID_LEFT_PIN_GPIO     GPIOA
#define AVOID_LEFT_IO           HAL_GPIO_ReadPin(AVOID_LEFT_PIN_GPIO, AVOID_LEFT_PIN)

// ?/?????(????)
#define BARRIER_Y 0      // ????
#define BARRIER_N 1      // ????

#endif
