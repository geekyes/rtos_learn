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


void xianshi(void);//��ʾ��������
void jianpan(void);//���̺���
extern float weight;//ad�ɼ���������ȫ�ֱ���
extern u8 b;//�궨�����
void fell_PB8(void);//��������������
void biao_ding(void);



#endif

