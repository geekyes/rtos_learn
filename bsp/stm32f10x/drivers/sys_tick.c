/*================================================================
*                source for rtos_learn
*   
*   filename   : sys_tick.c
*   author     : chenjiang
*   date       : 2018-08-05
*   description: system tick ，单位： 1ms
*
================================================================*/

#include "sys_tick.h"

static sys_tick_indicate_t irq_hook = NULL;

void sys_tick_init(sys_tick_indicate_t indicate)
{
    DEBUG_ASSERT(indicate);

    /* 设置为 1ms */
    (void)SysTick_Config(SystemCoreClock / 1000);
    irq_hook = indicate;
}

uint32_t tick_cnt = 0;
void delay_ms(uint32_t cnt)
{
    tick_cnt = cnt;

    while (tick_cnt > 0);
}

void SysTick_Handler(void)
{
    if (tick_cnt > 0)
    {
        tick_cnt--;
    }
    irq_hook(NULL, 0);
}

