#ifndef __IRSEARCH_H_
#define __IRSEARCH_H_

#include "main.h"   // ?????? stm32f1xx_hal.h

void IRSearchInit(void);
void SearchRun(void);

// ===================== ????????? =====================
// ???:SEARCH_R_PIN  PA7
// ???:SEARCH_L_PIN  PB0

#define SEARCH_R_PIN         GPIO_PIN_7
#define SEARCH_R_GPIO        GPIOA
#define SEARCH_R_IO          HAL_GPIO_ReadPin(SEARCH_R_GPIO, SEARCH_R_PIN)

#define SEARCH_L_PIN         GPIO_PIN_0
#define SEARCH_L_GPIO        GPIOB
#define SEARCH_L_IO          HAL_GPIO_ReadPin(SEARCH_L_GPIO, SEARCH_L_PIN)

// ????:????????(?=1,?=0)
#define BLACK_AREA 1
#define WHITE_AREA 0

// ===================== ??????(????) =====================
#define COMM_STOP  'I'  // ??
#define COMM_UP    'A'  // ??
#define COMM_DOWN  'B'  // ??
#define COMM_LEFT  'C'  // ??
#define COMM_RIGHT 'D'  // ??

#endif
