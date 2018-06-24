/*================================================================
*                source for rtos_learn
*   
*   filename   : main.c
*   author     : chenjiang
*   date       : 2018-06-19
*   description: test
*
================================================================*/

#include "public.h"

static struct {
    volatile uint8_t size;
    uint8_t data[RCV_BUFF_SIZE];
} rcv = {0, {0}};

/* usart_rcv handler */
static void usart_rcv(uint8_t *p_data, uint8_t size);

int main(void)
{
    /* init */
    (void)usart_driver.init(usart_rcv, 115200);
    (void)usart_driver.write((uint8_t*)"input: \r\n", 9);

    while(1)
    {
        if (rcv.size)
        {
            (void)usart_driver.write((uint8_t*)"rcv result: \r\n", 14);
            (void)usart_driver.write(rcv.data, rcv.size);
            (void)usart_driver.write((uint8_t*)"\r\ninput: \r\n", 11);
            rcv.size = 0;
        }
    }

    return 0;
}

/* usart_rcv handler */
static void usart_rcv(uint8_t *p_data, uint8_t size)
{
    if (size == 0 || NULL == p_data)
    {
        return ;
    }

    memcpy(rcv.data, p_data, size);
    rcv.size = size;
}
