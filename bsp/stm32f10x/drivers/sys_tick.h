/*================================================================
*                source for rtos_learn
*   
*   filename   : sys_tick.h
*   author     : chenjiang
*   date       : 2018-08-04
*   description: system tick ，单位： 1ms
*
================================================================*/

#ifndef SYS_TICK_H__
#define SYS_TICK_H__

#include "bsp_public.h"

typedef void (*sys_tick_indicate_t)(uint8_t *, uint8_t);

void sys_tick_init(sys_tick_indicate_t indicate);

#endif /* SYS_TICK_H__ */


