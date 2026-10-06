#include "keysacn.h"

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitStruct.Pin  = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.Pin   = GPIO_PIN_3;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    BEEP_RESET;
}

void keysacn(void)
{
    int val;
    val = KEY;

    // while(!GPIO_ReadInputDataBit(...)) -> while(!KEY)
    while(!KEY)
    {
        val = KEY;
    }

    // while(GPIO_ReadInputDataBit(...)) -> while(KEY)
    while(KEY)
    {
        HAL_Delay(10);     // delay_ms(10) -> HAL_Delay(10)

        val = KEY;
        if(val == 1)
        {
            BEEP_SET;

            // while(!GPIO_ReadInputDataBit(...)) -> while(!KEY)
            while(!KEY)
            {
                BEEP_RESET;
            }
        }
        else
        {
            BEEP_RESET;
        }
    }
}
