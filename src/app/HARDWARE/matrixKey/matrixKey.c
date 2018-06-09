#include "matrixKey.h"
#include "sys.h"
#include "delay.h"

//volatile uint8_t gvu8KeyDisVolFlag = 0;


/******矩阵按键的初始化
 *
 * 注：无移植性，
 * */
void MatrixKeyInit(uint8_t u8Mode)
{
    GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	if (u8Mode == 1) {
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(KEY_IO, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(KEY_IO, &GPIO_InitStructure);
	}
	else if (u8Mode == 2) {
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_Init(KEY_IO, &GPIO_InitStructure);
		
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(KEY_IO, &GPIO_InitStructure);
	}
}

/******矩阵按键的扫描
 * u8Mode == 0 :不支持连续按键
 * u8Mode == 1 :支持连续按键 有的问题
 * return : 按键的键值
 * 
 * */
uint8_t MatrixKeyScan(uint8_t u8Mode)
{
	uint8_t u8KeyVal = 0;
	uint8_t u8High = 0;
	uint8_t u8Low = 0;
	static uint8_t su8KeyUpFlag = 1;

	if (u8Mode) su8KeyUpFlag = 1;
	
	MatrixKeyInit(1);
	KEY_IO->ODR &= 0xfff0;//把0~3设置为低
	u8High = KEY_IO->IDR & 0x00f0;//读取4~7位
	if (su8KeyUpFlag && (u8High != 0xf0)) {
		delay_us(20);
		u8High = KEY_IO->IDR & 0x00f0;//在读取4~7位
		MatrixKeyInit(2);
		KEY_IO->ODR &= 0xff0f;//把4~7设置为低
		u8Low = KEY_IO->IDR & 0x000f;//读取1~3位
		if ((u8Low != 0x0f)) {
			delay_us(20);
			u8Low = KEY_IO->IDR & 0x000f;//再读0~3位
			u8KeyVal = u8High | u8Low;
			su8KeyUpFlag = 0;
		}
	}
	else if (u8High == 0xf0 || u8Low == 0x0f) {
		su8KeyUpFlag = 1;
		u8KeyVal = 0;
	}
	
	return ( (u8KeyVal == 0xff)?0:u8KeyVal );//我不知道为什么！
}
