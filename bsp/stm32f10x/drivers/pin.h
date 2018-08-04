/*================================================================
*                source for rtos_learn
*   
*   filename   : pin.h
*   author     : chenjiang
*   date       : 2018-08-04
*   description: 对 IO 初始化，操作
*
================================================================*/

#ifndef PIN_H__
#define PIN_H__

#include "bsp_public.h"

#define STM32F10X_PIN_NUMBERS 48

enum out_level {BIT_LOW = 0x0, BIT_HIGH};

struct pin_cfg {
    uint8_t idx;
    enum {AIN = 0x0, INPUT = 0x1, OUTPUT = 0x5, AF_OUT = 0x7} mode;
};

// init
void pin_init(struct pin_cfg *cfg);

void pin_write(uint8_t idx, enum out_level level);

enum out_level pin_read(uint8_t idx);

#endif /* PIN_H__ */


