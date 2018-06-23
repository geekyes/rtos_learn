/*================================================================
*                source for rtos_learn
*   
*   filename   : usart.c
*   author     : chenjiang
*   date       : 2018-06-22
*   description: usart 
*
================================================================*/

#include "public.h"

struct rcv_buff {
    uint8_t data[RCV_BUFF_SIZE];
    usart_rcv_handler_t handler;
};

static struct rcv_buff rcv;

/* init */
static uint8_t init(usart_rcv_handler_t handler, uint32_t baud_rate)
{
    /* GPIO端口设置 */
    GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

    /* 使能外设时钟 */
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA
            | RCC_APB2Periph_AFIO, ENABLE);
    /* USART1_TX   PA.9 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    /* USART1_RX	  PA.10 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);  
    /* USART1 NVIC 配置 */
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
    /* USART 初始化设置 */
	USART_InitStructure.USART_BaudRate = baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART1, ENABLE);

    if (NULL != handler)
    {
        rcv.handler = handler;
    }
    else
    {
        /* 不处理 */
    }

    return 0;
}

/* write */
static uint8_t write(uint8_t *p_data, uint8_t size)
{
    if (size == 0 || NULL == p_data)
    {
        return 1;
    }

    for (uint8_t i = 0; i < size; i++)
    {
        USART_SendData(USART1, p_data[i]);
        while (RESET == USART_GetFlagStatus(USART1, USART_FLAG_TXE)) ;
    }

    return 0;
}

void USART1_IRQHandler(void)
{
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        (void)USART_ReceiveData(USART1);
    }
}

const struct mcu_driver usart_driver = {
    init,
    write
};

