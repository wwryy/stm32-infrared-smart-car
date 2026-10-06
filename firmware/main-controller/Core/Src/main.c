/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "keysacn.h"
#include "IRSEARCH.h"
#include "IRAvoid.h"
#include "servo.h"
#include "remote.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
  MODE_TRACK = 0,
  MODE_REMOTE
} CarMode;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define REMOTE_HOLD_MS          800U
#define OBSTACLE_DEBOUNCE_MS     30U

#define REMOTE_KEY_FORWARD       70U
#define REMOTE_KEY_STOP          21U
#define REMOTE_KEY_LEFT          68U
#define REMOTE_KEY_RIGHT         67U
#define REMOTE_KEY_SPIN_LEFT    224U
#define REMOTE_KEY_REVERSE       64U
#define REMOTE_KEY_SPIN_RIGHT   144U
#define REMOTE_KEY_TOGGLE_MODE   12U
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */
uint8_t g_bt_data = 0;

static CarMode g_mode = MODE_TRACK;
static uint32_t last_remote_time = 0;
static uint32_t obstacle_start_time = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static uint8_t AnyObstacleDetected(void);
static uint8_t ObstacleDebounced(uint32_t now);
static void RemoteExec(uint8_t key);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static uint8_t AnyObstacleDetected(void)
{
  return (AVOID_LEFT_IO == BARRIER_Y) ||
         (AVOID_RIGHT_IO == BARRIER_Y);
}

static uint8_t ObstacleDebounced(uint32_t now)
{
  if (AnyObstacleDetected())
  {
    if (obstacle_start_time == 0U)
    {
      obstacle_start_time = now;
    }
    return (now - obstacle_start_time) >= OBSTACLE_DEBOUNCE_MS;
  }

  obstacle_start_time = 0U;
  return 0U;
}

static void RemoteExec(uint8_t key)
{
  switch (key)
  {
    case REMOTE_KEY_FORWARD:
      ZYSTM32_run(80, 0);
      break;

    case REMOTE_KEY_STOP:
      g_mode = MODE_REMOTE;
      ZYSTM32_brake(0);
      BEEP_SET;
      HAL_Delay(50);
      BEEP_RESET;
      break;

    case REMOTE_KEY_LEFT:
      ZYSTM32_Left(80, 0);
      break;

    case REMOTE_KEY_RIGHT:
      ZYSTM32_Right(80, 0);
      break;

    case REMOTE_KEY_SPIN_LEFT:
      ZYSTM32_Spin_Left(80, 0);
      break;

    case REMOTE_KEY_REVERSE:
      ZYSTM32_back(80, 0);
      break;

    case REMOTE_KEY_SPIN_RIGHT:
      ZYSTM32_Spin_Right(80, 0);
      break;

    case REMOTE_KEY_TOGGLE_MODE:
      if (g_mode == MODE_TRACK)
      {
        g_mode = MODE_REMOTE;
        ZYSTM32_brake(0);
      }
      else
      {
        g_mode = MODE_TRACK;
      }
      BEEP_SET;
      HAL_Delay(200);
      BEEP_RESET;
      break;

    default:
      break;
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  HAL_Init();
  SystemClock_Config();

  MX_GPIO_Init();
  MX_TIM2_Init();
  MX_TIM4_Init();
  MX_TIM5_Init();
  MX_USART1_UART_Init();

  /* USER CODE BEGIN 2 */
  KEY_Init();
  TIM4_PWM_Init();
  IRSearchInit();
  IRAvoidInit();

  TIM5_PWM_Init(0, 0);
  SetJointAngle(90.0f);

  ZYSTM32_brake(500);
  keysacn();
  Remote_Init();
  HAL_UART_Receive_IT(&huart1, &g_bt_data, 1);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
    uint32_t now = HAL_GetTick();
    uint8_t remote_key = Remote_Scan();

    if (remote_key > 0U)
    {
      last_remote_time = now;
      RemoteExec(remote_key);
    }

    /* A recent remote command temporarily owns the motors. */
    if ((now - last_remote_time) >= REMOTE_HOLD_MS)
    {
      if (g_mode == MODE_TRACK)
      {
        if (ObstacleDebounced(now))
        {
          AVoidRun();
        }
        else
        {
          SearchRun();
        }
      }
      else
      {
        ZYSTM32_brake(0);
      }
    }
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  if (GPIO_Pin == GPIO_PIN_1)
  {
    Remote_EXTI_Handler(GPIO_Pin);
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  BEEP_SET;
  if (huart->Instance == USART1)
  {
    BEEP_SET;
    ZYSTM32_run(60, 100);
    BEEP_RESET;

    HAL_UART_Receive_IT(&huart1, &g_bt_data, 1);
    BEEP_SET;
    ZYSTM32_run(60, 100);
    BEEP_RESET;
  }
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the line number.
  * @param  file Source file name
  * @param  line Source line number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  (void)file;
  (void)line;
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
