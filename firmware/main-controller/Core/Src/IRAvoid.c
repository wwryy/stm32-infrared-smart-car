#include "IRAvoid.h"
#include "motor.h"
#include "keysacn.h"
// ???? "stm32f10x.h" ? "delay.h",?????????? HAL_Delay ?

int SR_2;    // ???????????
int SL_2;    // ???????????

void IRAvoidInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // ?????? RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOA , ENABLE);
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // ????:PB1,???? GPIO_Mode_IPU
    GPIO_InitStruct.Pin  = AVOID_RIGHT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AVOID_RIGHT_PIN_GPIO, &GPIO_InitStruct);

    // ????:PA8,???? GPIO_Mode_IPU
    GPIO_InitStruct.Pin  = AVOID_LEFT_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(AVOID_LEFT_PIN_GPIO, &GPIO_InitStruct);
}

void AVoidRun(void)
{
    // ===== ??????????,????? =====
    SR_2 = AVOID_RIGHT_IO;
    SL_2 = AVOID_LEFT_IO;

    if (SL_2 == 1 && SR_2 == 1)
    {
        ZYSTM32_run(60, 10);
        BEEP_RESET;
        LED_D3_RESET;
    }
    else if (SL_2 == 1 && SR_2 == 0)
    {
        ZYSTM32_Left(90, 1000);
			  ZYSTM32_Right(90, 1000);
    }
    else if (SR_2 == 1 && SL_2 == 0)
    {
        
			  ZYSTM32_Right(90, 1000);
			ZYSTM32_Left(90, 1000);
    }
    else
    {
        BEEP_SET;
        LED_D3_SET;
        ZYSTM32_brake(300);       // ?? 300ms
        ZYSTM32_back(70, 1500);   // ?? 1000ms
			  ZYSTM32_brake(700);
        ZYSTM32_Spin(3600); // ??? 500ms
			  BEEP_RESET;
			  
    }
}
