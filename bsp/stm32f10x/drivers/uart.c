/*================================================================
*                source for rtos_learn
*   
*   filename   : uart.c
*   author     : chenjiang
*   date       : 2018-06-22
*   description: uart 
*
================================================================*/

#include "uart.h"
#include "pin.h"

struct uart_instance {
    USART_TypeDef *name;
    uint32_t rcc;
    uint8_t tx_pin_idx;
    uint8_t rx_pin_idx;
};

struct rcv {
    uint8_t index;
    uint8_t data[RCV_BUFF_SIZE];
    uart_rcv_indicate_t indicate;
};

/* TODO 没有 uart_2 的 io */
static const struct uart_instance uart_inst_map[] = {
#if (STM32F10X_PIN_NUMBERS == 48)
    {USART1, RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, 30, 31}, 
    {USART2, RCC_APB1Periph_USART2 | RCC_APB2Periph_AFIO, 9, 10}, 
#endif
#if (STM32F10X_PIN_NUMBERS == 64)
    {USART1, RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, 42, 43}, 
    {USART2, RCC_APB1Periph_USART2 | RCC_APB2Periph_AFIO, 9, 10}, 
#endif
#if (STM32F10X_PIN_NUMBERS == 100)
    {USART1, RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, 68, 69}, 
    {USART2, RCC_APB1Periph_USART2 | RCC_APB2Periph_AFIO, 9, 10}, 
#endif
#if (STM32F10X_PIN_NUMBERS == 144)
    {USART1, RCC_APB2Periph_USART1 | RCC_APB2Periph_AFIO, 101, 102}, 
    {USART2, RCC_APB1Periph_USART2 | RCC_APB2Periph_AFIO, 9, 10}, 
#endif
};

static struct rcv rcv_buff[UART_NUMBER] = {0};

/* init */
void uart_init(struct uart_cfg *cfg)
{
    DEBUG_ASSERT(cfg);
    DEBUG_ASSERT(cfg->idx < UART_NUMBER);

	USART_InitTypeDef USART_InitStructure;
    struct uart_instance inst = uart_inst_map[cfg->idx];
    struct pin_cfg tx_pin = {inst.tx_pin_idx, AF_OUT};
    struct pin_cfg rx_pin = {inst.rx_pin_idx, INPUT};

    pin_init(&tx_pin);
    pin_init(&rx_pin);
    /* 使能 uart 外设时钟 */
    if (UART_1 == cfg->idx)
    {
        RCC_APB2PeriphClockCmd(inst.rcc, ENABLE);
    }
    else 
    {
        RCC_APB1PeriphClockCmd(inst.rcc, ENABLE);
    }
    /* USART 初始化设置 */
	USART_InitStructure.USART_BaudRate = cfg->baud_rate;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.\
            USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(inst.name, &USART_InitStructure);
    if (NULL != cfg->rcv_indicate) // 中断接收
    {
        NVIC_InitTypeDef NVIC_InitStructure;
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
        NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
        NVIC_Init(&NVIC_InitStructure);
        USART_ITConfig(inst.name, USART_IT_RXNE, ENABLE);
        rcv_buff[cfg->idx].indicate = cfg->rcv_indicate;
        rcv_buff[cfg->idx].index = 0;
    }
    else
    {
        /* 不处理 */
    }
    USART_Cmd(inst.name, ENABLE);
}

/* write */
void uart_write(enum uart_idx idx, uint8_t *p_data, uint8_t size)
{
    DEBUG_ASSERT(size > 0 && p_data);

    struct uart_instance inst = uart_inst_map[idx];

    for (uint8_t i = 0; i < size; i++)
    {
        USART_SendData(inst.name, p_data[i]);
        while (RESET == USART_GetFlagStatus(inst.name, USART_FLAG_TXE)) ;
    }
}

void USART1_IRQHandler(void)
{
    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
        static uint8_t finish_flag = 0;
        rcv_buff[UART_1].data[ \
            rcv_buff[UART_1].index] = (uint8_t)USART_ReceiveData(USART1);
        
        /* 判断接收结束第一步 */
        if ((uint8_t)'\r' == rcv_buff[UART_1].data[rcv_buff[UART_1].index])
        {
            finish_flag++;
        }
        rcv_buff[UART_1].index++;
        
        /* 判断接收结束第二步 */
        if (finish_flag == 1 || rcv_buff[UART_1].index > RCV_BUFF_SIZE)
        {
            finish_flag = 0;
            rcv_buff[UART_1].indicate(
                    rcv_buff[UART_1].data, rcv_buff[UART_1].index - 1);
            rcv_buff[UART_1].index = 0;
        }
    }
}

void USART2_IRQHandler(void)
{
    if (USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
    {
        static uint8_t finish_flag = 0;
        rcv_buff[UART_2].data[ \
            rcv_buff[UART_2].index] = (uint8_t)USART_ReceiveData(USART2);
        
        /* 判断接收结束第一步 */
        if ((uint8_t)'\r' == rcv_buff[UART_2].data[rcv_buff[UART_2].index])
        {
            finish_flag++;
        }
        rcv_buff[UART_2].index++;
        
        /* 判断接收结束第二步 */
        if (finish_flag == 1 || rcv_buff[UART_2].index > RCV_BUFF_SIZE)
        {
            finish_flag = 0;
            rcv_buff[UART_2].indicate( \
                    rcv_buff[UART_2].data, rcv_buff[UART_2].index - 1);
            rcv_buff[UART_2].index = 0;
        }
    }
}

