#ifndef __MOTOR_H_
#define __MOTOR_H_

#include "main.h"

void TIM4_PWM_Init(void);
void SetMotorSpeed(uint8_t ucChannel, int8_t cSpeed);

void ZYSTM32_run(int8_t speed,int time);
void ZYSTM32_brake(int time);
void ZYSTM32_Left(int8_t speed,int time);
void ZYSTM32_Spin_Left(int8_t speed,int time);
void ZYSTM32_Right(int8_t speed,int time);
void ZYSTM32_Spin_Right(int8_t speed,int time);
void ZYSTM32_back(int8_t speed,int time);
void ZYSTM32_Spin(int time);

/* Motor interface mapping:
 * LEFT_MOTOR_GO   PB7  direction output
 * LEFT_MOTOR_PWM  PB8  TIM4_CH3 PWM output
 * RIGHT_MOTOR_GO  PA4  direction output
 * RIGHT_MOTOR_PWM PB9  TIM4_CH4 PWM output
 */

#define LEFT_MOTOR_GO_GPIO    GPIOB
#define LEFT_MOTOR_GO_PIN     GPIO_PIN_7
#define LEFT_MOTOR_GO_SET     HAL_GPIO_WritePin(LEFT_MOTOR_GO_GPIO, LEFT_MOTOR_GO_PIN, GPIO_PIN_SET)
#define LEFT_MOTOR_GO_RESET   HAL_GPIO_WritePin(LEFT_MOTOR_GO_GPIO, LEFT_MOTOR_GO_PIN, GPIO_PIN_RESET)

#define LEFT_MOTOR_PWM_GPIO   GPIOB
#define LEFT_MOTOR_PWM_PIN    GPIO_PIN_8
#define LEFT_MOTOR_PWM_SET    HAL_GPIO_WritePin(LEFT_MOTOR_PWM_GPIO, LEFT_MOTOR_PWM_PIN, GPIO_PIN_SET)
#define LEFT_MOTOR_PWM_RESET  HAL_GPIO_WritePin(LEFT_MOTOR_PWM_GPIO, LEFT_MOTOR_PWM_PIN, GPIO_PIN_RESET)

#define RIGHT_MOTOR_GO_GPIO   GPIOA
#define RIGHT_MOTOR_GO_PIN    GPIO_PIN_4
#define RIGHT_MOTOR_GO_SET    HAL_GPIO_WritePin(RIGHT_MOTOR_GO_GPIO, RIGHT_MOTOR_GO_PIN, GPIO_PIN_SET)
#define RIGHT_MOTOR_GO_RESET  HAL_GPIO_WritePin(RIGHT_MOTOR_GO_GPIO, RIGHT_MOTOR_GO_PIN, GPIO_PIN_RESET)

#define RIGHT_MOTOR_PWM_GPIO  GPIOB
#define RIGHT_MOTOR_PWM_PIN   GPIO_PIN_9
#define RIGHT_MOTOR_PWM_SET   HAL_GPIO_WritePin(RIGHT_MOTOR_PWM_GPIO, RIGHT_MOTOR_PWM_PIN, GPIO_PIN_SET)
#define RIGHT_MOTOR_PWM_RESET HAL_GPIO_WritePin(RIGHT_MOTOR_PWM_GPIO, RIGHT_MOTOR_PWM_PIN, GPIO_PIN_RESET)

#endif
