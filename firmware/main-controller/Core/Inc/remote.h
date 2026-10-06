#ifndef __REMOTE_H
#define __REMOTE_H

#include "stm32f1xx_hal.h"
#include <stdint.h>

#define REMOTE_GPIO_PORT   GPIOA
#define REMOTE_GPIO_PIN    GPIO_PIN_1

#define RDATA  (HAL_GPIO_ReadPin(REMOTE_GPIO_PORT, REMOTE_GPIO_PIN))

#define REMOTE_ID  0

extern uint8_t RmtCnt;

void Remote_Init(void);

uint8_t Remote_Scan(void);

void Remote_EXTI_Handler(uint16_t GPIO_Pin);

#endif
