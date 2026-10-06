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
#include "Server.h"
//#include "UltrasonicWave.h"
#include "remote.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t g_bt_data = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static uint32_t last_remote_time = 0; // ??????????
volatile uint8_t g_ir_exti_hit = 0;

typedef enum { MODE_TRACK = 0, MODE_AVOID, MODE_REMOTE } CarMode;

static CarMode g_mode = MODE_TRACK;
//static CarMode g_mode = MODE_REMOTE;

static uint32_t g_obs_t0 = 0;    // ??????

// ?? IRAvoid.c ?:1=???,0=???(????)
// ??????????? (AVOID_LEFT_IO==0 || AVOID_RIGHT_IO==0)
static inline uint8_t IR_AnyBlocked(void)
{
    return (AVOID_LEFT_IO == BARRIER_Y) || (AVOID_RIGHT_IO == BARRIER_Y); // BARRIER_Y=0
}

// ??:???????>=30ms ??????
static uint8_t ObstacleDebounced(uint32_t now)
{
    if (IR_AnyBlocked())
    {
        if (g_obs_t0 == 0) g_obs_t0 = now;
        return (now - g_obs_t0) >= 30;
    }
    g_obs_t0 = 0;
    return 0;
}

// ?????????(?? BSP,?? main ???????????????)
//static void TrackStep(void)
//{
//	if (SEARCH_L_IO == WHITE_AREA && SEARCH_R_IO == WHITE_AREA)
//    {
//        ZYSTM32_run(60, 0); // ???? 0,???
//    }
//    else if (SEARCH_L_IO == BLACK_AREA && SEARCH_R_IO == WHITE_AREA)
//    {
//        ZYSTM32_Left(100, 0); 
//    }
//    else if (SEARCH_R_IO == BLACK_AREA && SEARCH_L_IO == WHITE_AREA)
//    {
//        ZYSTM32_Right(100, 0);
//    }
//    else
//    {
//        ZYSTM32_run(60, 0);
//    }
//    // ?? IRSEARCH.h:BLACK_AREA=1, WHITE_AREA=0(???? BSP ????)
//    if (SEARCH_L_IO == WHITE_AREA && SEARCH_R_IO == WHITE_AREA)
//    {
//        ZYSTM32_run(60, 20);
//    }
//    else if (SEARCH_L_IO == BLACK_AREA && SEARCH_R_IO == WHITE_AREA)
//    {
//        ZYSTM32_Left(100, 80);
//    }
//    else if (SEARCH_R_IO == BLACK_AREA && SEARCH_L_IO == WHITE_AREA)
//    {
//        ZYSTM32_Right(100, 80);
//    }
//    else
//    {
//        ZYSTM32_run(60, 20);
//    }
//}
static void RemoteExec(uint8_t key)
{
    switch (key)
    {
        case 70:  ZYSTM32_run(80,0);        break;
//        case 21:   ZYSTM32_brake(0);         break;
			case 21: // ???
    g_mode = MODE_REMOTE; // ?????????
    ZYSTM32_brake(0);     // ??
    BEEP_SET; HAL_Delay(50); BEEP_RESET; // ?????????????
    break;
        case 68: ZYSTM32_Left(80,0);       break;
        case 67:  ZYSTM32_Right(80,0);      break;
        case 224: ZYSTM32_Spin_Left(80,0);  break;
        case 64: ZYSTM32_back(80,0);       break;
        case 144: ZYSTM32_Spin_Right(80,0); break;
			  case 12: // ?? 12 ??????
    if (g_mode == MODE_TRACK) {
        g_mode = MODE_REMOTE;
        ZYSTM32_brake(0); // ????????????
    } else {
        g_mode = MODE_TRACK;
        // ????????,????????,
        // ?????????? TrackStep()
    }
    // ??????????
    BEEP_SET; HAL_Delay(200); BEEP_RESET; 
    break;
        default: break;
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
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

  TIM5_PWM_Init(0, 0);   // ???????,????? CubeMX
  SetJointAngle(90.0f);  // ??????

  //UltrasonicWave_Configuration();

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
    uint8_t rk = Remote_Scan();

    if (rk > 0) 
{
    last_remote_time = now;
    RemoteExec(rk);
    // BEEP_SET; // ??????????,?????????
    // HAL_Delay(10); 
    // BEEP_RESET; 
}

    // --- ??????? ---
    
    // 1. ?????????????(???? 800ms ?)
    if ((now - last_remote_time) < 800) 
    {
        // ?? 800ms ?,?????,?? RemoteExec ???????
			
        // ??????????????,????????,?????????
    }
    // 2. ????? MODE_TRACK ?,?????????/??
    else if (g_mode == MODE_TRACK) 
    {
        if (ObstacleDebounced(now)) {
            AVoidRun(); 
        } else {
            SearchRun(); 
        }
    }
    // 3. ??? MODE_REMOTE ??????,????????
    else 
    {
        ZYSTM32_brake(0); // ????????,?????
    }
 
		
		
		
		
		
		
//		uint32_t now = HAL_GetTick();
//    uint8_t rk = Remote_Scan();

//    if (rk > 0) 
//    {
//        // ??????,??????????
//        last_remote_time = now;
//        RemoteExec(rk);
//        
//        // ????????????(???? Delay)
//        BEEP_SET; HAL_Delay(10); BEEP_RESET; 
//    }

//    // --- ?????? ---
//    // ?????????? 800 ??,???????????,?????
//    if ((now - last_remote_time) < 800) 
//    {
//        // ???????,????? RemoteExec ?????
//    }
//    else 
//    {
//        // ?? 800ms ????,??????/??
//        if (ObstacleDebounced(now)) {
//            AVoidRun(); 
//        } else {
//            TrackStep(); // ??? TrackStep ?????????? 0
//        }
//    }
		
		
//		uint8_t rk = Remote_Scan();
//if (rk > 0) 
//{
//    // 1. ??? 1 ?,??????????
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
//    HAL_Delay(1000);
//    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
//    HAL_Delay(500);

//    // 2. ???? (????)
//    for(int i = 0; i < (rk / 10); i++) {
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); HAL_Delay(400);
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); HAL_Delay(400);
//    }
//    
//    HAL_Delay(1000); // ????????? 1 ?

//    // 3. ???? (??????)
//    for(int i = 0; i < (rk % 10); i++) {
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET); HAL_Delay(200);
//        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET); HAL_Delay(200);
//    }
//}
		
		
//		uint32_t now = HAL_GetTick();
//    uint8_t rk = Remote_Scan(); // ?????????

//    if (rk)
//    {
//        // ????????????,?????????????
//        g_remote_last_ms = now;
//        
//        // ?????????
//        BEEP_SET; 
//        
//        // ?????? (??:RemoteExec ?????? time ??? 0)
//        RemoteExec(rk);
//        
//        // ??????????(????????)
//        HAL_Delay(10); // ????
//        BEEP_RESET;
//    }

//    // --- ??????? ---
//    
//    // ?????????????? 500ms,???????????????
//    if ((now - g_remote_last_ms) < 500) 
//    {
//        // ???? RemoteExec ???????,??????
//        // ?????????
//    }
//    else 
//    {
//        // ?? 500ms ?????,?????????
//        if (ObstacleDebounced(now)) {
//            AVoidRun(); 
//        } else {
//            TrackStep(); // ??:TrackStep ???? time ???? 0
//        }
//    }
		
		
		
//		if (g_ir_exti_hit)
//{
//    g_ir_exti_hit = 0;
//    BEEP_SET; HAL_Delay(20); BEEP_RESET;
//}
//		uint32_t now = HAL_GetTick();

///* ? ????:???????? ? ????????? */
//uint8_t rk = Remote_Scan();
//if (rk)
//{
//	
//    g_mode = MODE_REMOTE;
//    g_remote_last_ms = now;

//    BEEP_SET; HAL_Delay(10); BEEP_RESET;   // ? ????????,?????????
//    RemoteExec(rk);

//    // ????(????/??)
//    continue;
//}

///* ? ????:??????? ? ???????/?? */
//if (g_mode == MODE_REMOTE)
//{
//    if ((now - g_remote_last_ms) > REMOTE_TIMEOUT_MS)
//    {
//        g_mode = IR_AnyBlocked() ? MODE_AVOID : MODE_TRACK;
//    }
//    else
//    {
//        // ??????????:?????/??
//        continue;
//    }
//}

///* ? ???:????????????????? */
//if (g_mode == MODE_TRACK)
//{
//    if (ObstacleDebounced(now))
//    {
//        g_mode = MODE_AVOID;
//        ZYSTM32_brake(50);
//    }
//    else
//    {
//        TrackStep();
//    }
//}
//else // MODE_AVOID
//{
//    AVoidRun();

//    if (ClearDebounced(now))
//    {
//        g_mode = MODE_TRACK;
//        ZYSTM32_brake(30);
//    }
//}


		// ===== ?????:??? -> ?? -> ???? -> ???? =====
//uint32_t now = HAL_GetTick();

//if (g_mode == MODE_TRACK)
//{
//    // ??????
//    if (ObstacleDebounced(now))
//    {
//        // ???????? -> ????
//        g_mode = MODE_AVOID;
//        ZYSTM32_brake(50);
//    }
//    else
//    {
//        TrackStep();
//    }
//}
//else // MODE_AVOID
//{
//    // ????:??????? AVoidRun()
//    AVoidRun();

//    // ???,?????????????,???????
//    if (ClearDebounced(HAL_GetTick()))
//    {
//        g_mode = MODE_TRACK;
//        ZYSTM32_brake(30);
//    }
//}

    // ????????? SearchRun();

//		int dist_mm = UltrasonicWave_StartMeasure();
//		
//		HAL_Delay(50);

//		 if (dist_mm > 0 && dist_mm < 200)   // ?? 20cm ???????
//    {
//        // ????????:?? + ?? + ??,???????????
//        ZYSTM32_brake(200);
//        SetJointAngle(45.0f);           // ??????

//        ZYSTM32_back(60, 500);
//        ZYSTM32_Left(60, 500);
//    }
//    else
//    {
//        // ????:?????,??????(IRAvoid)
//        SetJointAngle(90.0f);
//        AVoidRun();
//        // ???????,???????? SearchRun();
//        // SearchRun();
//    }
//     
//		// void
//		 AVoidRun(); 
		//search
		 //SearchRun();
		 // forward 1s
    //ZYSTM32_run(60, 1000);
    // stop 0.5s
   // ZYSTM32_brake(500);
    // right 0.8s
   // ZYSTM32_Right(60, 800);
    // back 1s
    //ZYSTM32_back(60, 1000);
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

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
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
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
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
    if (GPIO_Pin == GPIO_PIN_1)   // PA1 = EXTI1
    {
			
		
        g_ir_exti_hit = 1;
        Remote_EXTI_Handler(GPIO_Pin);  // ??:????????
			
    }
}


void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BEEP_SET;
    if(huart->Instance == USART1)
    {
        // ????????,??????,???? 100ms
        BEEP_SET;
        ZYSTM32_run(60, 100);
        BEEP_RESET;

        // ??????
        HAL_UART_Receive_IT(&huart1, &g_bt_data, 1);
			BEEP_SET;
        ZYSTM32_run(60, 100);
        BEEP_RESET;
    }
}

//void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
//{
//    if(huart->Instance == USART1)
//    {
//        // 1. ??????,????
//        last_remote_time = HAL_GetTick();
//        
//        // 2. ?? 16 ??????
//        // ??:???????,???????,??????
//        switch(g_bt_data)
//        {
//            case 0x01: ZYSTM32_run(80, 0); break;    // ??
//            case 0x02: ZYSTM32_back(80, 0); break;   // ??
//            case 0x03: ZYSTM32_Left(80, 0); break;   // ??
//            case 0x04: ZYSTM32_Right(80, 0); break;  // ??
//            case 0x00: 
//                g_mode = MODE_REMOTE; 
//                ZYSTM32_brake(0); 
//                break;  						// ??
//            case 0x05: 
//    g_mode = MODE_TRACK; // ?????????
//    BEEP_SET; HAL_Delay(200); BEEP_RESET;
//    break;
//						default: break;
//        }

//        // 3. ????????
//        HAL_UART_Receive_IT(&huart1, &g_bt_data, 1);
//    }
//}


/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
