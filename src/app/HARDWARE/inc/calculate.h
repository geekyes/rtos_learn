#ifndef __CALCULATE_KEY
#define __CALCULATE_KEY
#include "stm32f10x.h"
#include "lcd12864.h"
#include "delay.h"
#include "matrixKey.h"
#include "usart.h"


#define  fell_set GPIO_SetBits(GPIOB,GPIO_Pin_8)
#define  fell_reset GPIO_ResetBits(GPIOB,GPIO_Pin_8)

#define  LED_SET GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define  LED_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_14)

#define  LED1_SET GPIO_SetBits(GPIOB,GPIO_Pin_15)
#define  LED1_RESET GPIO_ResetBits(GPIOB,GPIO_Pin_15)


void xianshi(void);//显示函数定义
void jianpan(void);//键盘函数
extern float weight;//ad采集回来数据全局变量
extern u8 b;//标定点计数
void fell_PB8(void);//蜂鸣器函数定义
void biao_ding(void);



#endif

