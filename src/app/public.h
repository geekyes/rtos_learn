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
#include <string.h>

#include "stm32f10x_conf.h"

#undef NULL
#define NULL ((void*)0)
#define RCV_BUFF_SIZE ((uint8_t)(100U))

typedef void (*usart_rcv_handler_t)(uint8_t *, uint8_t);

struct mcu_driver {
    uint8_t (*init)(usart_rcv_handler_t, uint32_t);
    uint8_t (*write)(uint8_t *, uint8_t);
};

extern const struct mcu_driver usart_driver;

#endif /* PUBLIC_H__ */


