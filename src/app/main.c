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

int main(void)
{
    /* init */
    (void)usart_driver.init(NULL, 115200);
    (void)usart_driver.write((uint8_t*)"chenjiang\n", 10);

    while(1)
    {
        /* code  */
    }

    return 0;
}

