#include "remote.h"

static uint32_t s_cycles_per_us = 0;

static void DWT_Init(void)
{
    SystemCoreClockUpdate();
    s_cycles_per_us = SystemCoreClock / 1000000UL;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static inline uint32_t now_us(void)
{
    return (uint32_t)(DWT->CYCCNT / s_cycles_per_us);
}

static volatile uint8_t  got_guide = 0;
static volatile uint8_t  bit_cnt   = 0;
static volatile uint8_t  frame_ok  = 0;
static volatile uint32_t RmtRec    = 0;

uint8_t RmtCnt = 0;

static uint32_t rise_us = 0;
static uint32_t last_edge_ms = 0;

static void remote_reset(void)
{
    got_guide = 0;
    bit_cnt   = 0;
    frame_ok  = 0;
    RmtRec    = 0;
    RmtCnt    = 0;
}

void Remote_Init(void)
{
    DWT_Init();
    remote_reset();
}

void Remote_EXTI_Handler(uint16_t GPIO_Pin)
{
    if (GPIO_Pin != REMOTE_GPIO_PIN) return;

    uint32_t ms = HAL_GetTick();
    if (got_guide && (ms - last_edge_ms) > 30U)
    {
        remote_reset();
    }
    last_edge_ms = ms;

    if (RDATA == GPIO_PIN_SET)
    {
        rise_us = now_us();
    }
    else
    {
        uint32_t high_us = now_us() - rise_us;

        if (!got_guide)
        {
            if (high_us > 4200 && high_us < 4700)
            {
                got_guide = 1;
                bit_cnt   = 0;
                frame_ok  = 0;
                RmtRec    = 0;
                RmtCnt    = 0;
            }
            return;
        }

        if (high_us > 2200 && high_us < 2600)
        {
            RmtCnt++;
            return;
        }

       if (high_us > 300 && high_us < 800)              // 0
{
    bit_cnt++;
}
else if (high_us > 1400 && high_us < 1800)       // 1
{
    RmtRec |= (1UL << bit_cnt);
    bit_cnt++;
}

        else
        {
            remote_reset();
            return;
        }

        if (bit_cnt >= 32)
        {
            frame_ok = 1;
            got_guide = 0;
        }
    }
}

uint8_t Remote_Scan(void)
{
    if (frame_ok)
    {
        uint8_t cmd = (uint8_t)((RmtRec >> 16) & 0xFF);
        frame_ok = 0U;
        return cmd;
    }
    return 0U;
}
