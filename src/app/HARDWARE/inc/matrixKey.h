#ifndef __MATRIX_KEY
#define __MATRIX_KEY

#include "stm32f10x.h"

#define KEY_IO GPIOD

//extern volatile uint8_t gvu8KeyDisVolFlag;


//���󰴼��ĳ�ʼ��
void MatrixKeyInit(uint8_t u8Mode);


//���󰴼�ɨ��
//return: ��ֵ
//u8Mode == 0:��֧���������� u8Mode == 1:֧����������
uint8_t MatrixKeyScan(uint8_t u8Mode);





#endif


