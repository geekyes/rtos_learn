#ifndef __MATRIX_KEY
#define __MATRIX_KEY

#include "stm32f10x.h"

#define KEY_IO GPIOD

//extern volatile uint8_t gvu8KeyDisVolFlag;


//矩阵按键的初始化
void MatrixKeyInit(uint8_t u8Mode);


//矩阵按键扫描
//return: 键值
//u8Mode == 0:不支持连续按键 u8Mode == 1:支持连续按键
uint8_t MatrixKeyScan(uint8_t u8Mode);





#endif


