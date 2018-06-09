#ifndef __HX711_H
#define __HX711_H

#include "stm32f10x.h"

//

extern int32_t ADValA[17];
//HX711 24位AD的GPIO Init
extern void HX711Init(void);

//读取 HX711 的转换值
extern uint32_t HX711ReadADVal(void);

//对物体称重
extern float TestWeight(void);

//求取平均数
extern int32_t Filter(void);


#endif
