#include "IRSEARCH.h"
#include "motor.h"
#include "keysacn.h"
// delay.h ? stm32f10x.h ?????,HAL_Delay ? main.h ????

char ctrl_comm      = COMM_STOP;  // ??????
char ctrl_comm_last = COMM_STOP;  // ????????

// ===================== 1. ???? GPIO ???(HAL ?) =====================
void IRSearchInit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    // ?? GPIOA / GPIOB ??(?????? RCC_APB2PeriphClockCmd)
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    // ???:SEARCH_R_PIN(PA7),???? GPIO_Mode_IPU
    GPIO_InitStruct.Pin  = SEARCH_R_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    // ???????????,???????
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SEARCH_R_GPIO, &GPIO_InitStruct);

    // ???:SEARCH_L_PIN(PB0),???? GPIO_Mode_IPU
    GPIO_InitStruct.Pin  = SEARCH_L_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(SEARCH_L_GPIO, &GPIO_InitStruct);
}

// ===================== 2. ??????(????) =====================
void SearchRun(void)
{
    uint8_t current_speed = 60; // ????
    
    // ===================== 1. ???? =====================
    // ?? PC1 ? SET ??????(????????)
    if (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_SET) 
    {
        BEEP_SET;           // ????
        current_speed = 40;  // ??????? 40
    }
    else 
    {
        BEEP_RESET;
        current_speed = 60;  // ?????? 60
    }

    // ===================== 2. ???? (????) =====================
    if (SEARCH_L_IO == WHITE_AREA && SEARCH_R_IO == WHITE_AREA)
        ctrl_comm = COMM_UP;
    else if (SEARCH_L_IO == BLACK_AREA && SEARCH_R_IO == WHITE_AREA)
        ctrl_comm = COMM_LEFT;
    else if (SEARCH_R_IO == BLACK_AREA && SEARCH_L_IO == WHITE_AREA) 
        ctrl_comm = COMM_RIGHT;
    else
        ctrl_comm = COMM_UP;

    // ===================== 3. ???? =====================
    // ??:????? ctrl_comm_last ???,??????????
    // ??????????????,???????? last ??
    
    switch (ctrl_comm)
    {
        case COMM_UP:
            ZYSTM32_run(current_speed, 10); 
            break;

        case COMM_LEFT:
            // ???????????????,??????
            ZYSTM32_Left(current_speed + 20, 10); 
            break;

        case COMM_RIGHT:
            ZYSTM32_Right(current_speed + 20, 10);
            break;

        case COMM_STOP:
            ZYSTM32_brake(10);
            break;

        default:
            break;
    }
}
