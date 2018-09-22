/*================================================================
*                source for rtos_learn
*   
*   filename   : public.h
*   author     : chenjiang
*   date       : 2018-06-22
*   description: 每个模块都需要的头文件
*
================================================================*/

#ifndef PUBLIC_H__
#define PUBLIC_H__

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "stm32f10x_conf.h"
#include "debug.h"

#ifdef STM32F10X_HD
#define STM32F10X_PIN_NUMBERS (64)
#endif
#ifdef STM32F10X_MD
#define STM32F10X_PIN_NUMBERS (48)
#endif

#endif /* PUBLIC_H__ */


