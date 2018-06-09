#ifndef __HX711_H
#define __HX711_H

#include "stm32f10x.h"

//

extern int32_t ADValA[17];
//HX711 24λAD��GPIO Init
extern void HX711Init(void);

//��ȡ HX711 ��ת��ֵ
extern uint32_t HX711ReadADVal(void);

//���������
extern float TestWeight(void);

//��ȡƽ����
extern int32_t Filter(void);


#endif
