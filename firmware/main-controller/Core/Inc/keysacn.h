#ifndef __KEYSACN_H_
#define __KEYSACN_H_

#include "main.h"   // ????? stm32f1xx_hal.h

void KEY_Init(void);   // ?????
void keysacn(void);    // ??????

// ??? IO ??(?????:PC3)
#define BEEP_PIN         GPIO_PIN_3
#define BEEP_GPIO        GPIOC
#define BEEP_SET         HAL_GPIO_WritePin(BEEP_GPIO, BEEP_PIN, GPIO_PIN_SET)
#define BEEP_RESET       HAL_GPIO_WritePin(BEEP_GPIO, BEEP_PIN, GPIO_PIN_RESET)

// ???? PC2(?????? KEY)
#define KEY              HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_2)

#define LED_D3_SET       HAL_GPIO_WritePin(LED_D3_GPIO_Port, LED_D3_Pin, GPIO_PIN_SET)
#define LED_D3_RESET     HAL_GPIO_WritePin(LED_D3_GPIO_Port, LED_D3_Pin, GPIO_PIN_RESET)

#endif
