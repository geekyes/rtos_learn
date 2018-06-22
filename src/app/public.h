/*================================================================
*                source for rtos_learn
*   
*   filename   : public.h
*   author     : chenjiang
*   date       : 2018-06-22
*   description: TODO
*
================================================================*/

#ifndef PUBLIC_H__
#define PUBLIC_H__

#include <stdint.h>

#include "stm32f10x_conf.h"

struct mcu_driver {
    uint8_t (*init)(void);
    uint8_t (*write)(uint8_t *, uint8_t);
    uint8_t (*read)(uint8_t *, uint8_t);
};

extern const struct mcu_driver usart_driver;

#endif /* PUBLIC_H__ */


