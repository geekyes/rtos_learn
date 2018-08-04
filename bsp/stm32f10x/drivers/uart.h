/*================================================================
*                source for rtos_learn
*   
*   filename   : uart.h
*   author     : chenjiang
*   date       : 2018-08-04
*   description: uart
*
================================================================*/

#ifndef UART_H__
#define UART_H__

#include "bsp_public.h"

#define RCV_BUFF_SIZE (100)


typedef void (*uart_rcv_indicate_t)(uint8_t *, uint8_t);

// TODO 
enum uart_idx {UART_1 = 0x0, UART_2, UART_NUMBER};

struct uart_cfg {
    enum uart_idx idx;
    uint32_t baud_rate;
    uart_rcv_indicate_t rcv_indicate;
};

void uart_init(struct uart_cfg *cfg);

void uart_write(enum uart_idx idx, uint8_t *p_data, uint8_t size);

#endif /* UART_H__ */


