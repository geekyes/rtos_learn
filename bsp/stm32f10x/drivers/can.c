/*================================================================
*                source for rtos_learn
*   
*   filename   : can.c
*   author     : chenjiang
*   date       : 2018-08-25
*   description: test can
*
================================================================*/

#include "can.h"
#include "pin.h"
#include "uart.h"

void can_init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#if (STM32F10X_PIN_NUMBERS == 48)
    struct pin_cfg tx_pin = {33, AF_OUT};
    struct pin_cfg rx_pin = {32, INPUT_UP};
#endif
#if (STM32F10X_PIN_NUMBERS == 64)
    struct pin_cfg tx_pin = {45, AF_OUT};
    struct pin_cfg rx_pin = {44, INPUT_UP};
#endif
    pin_init(&rx_pin);
    pin_init(&tx_pin);

    CAN_InitTypeDef CAN_InitStructure = {0};
    /* CAN1 Periph clocks enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);

    /* CAN1 register init */
    CAN_DeInit(CAN1);

    /* Struct init*/
    CAN_StructInit(&CAN_InitStructure);

    /* CAN1 cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = ENABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_3tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_2tq;
    CAN_InitStructure.CAN_Prescaler = 12;

    /*Initializes the CAN1 */
    DEBUG_ASSERT(CAN_Init(CAN1, &CAN_InitStructure));

    CAN_FilterInitTypeDef CAN_Filter = {0};
    /* CAN1 filter init */
    CAN_Filter.CAN_FilterNumber = 1;
    CAN_Filter.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_Filter.CAN_FilterScale = CAN_FilterScale_32bit;
    #ifdef STM32F10X_HD
    CAN_Filter.CAN_FilterIdHigh = (uint16_t)0x100 << 5;
    #endif
    #ifdef STM32F10X_MD
    CAN_Filter.CAN_FilterIdHigh = (uint16_t)0x100 << 5;
    #endif
    CAN_Filter.CAN_FilterIdLow = 0x0000;
    CAN_Filter.CAN_FilterMaskIdHigh = 0x0000;
    CAN_Filter.CAN_FilterMaskIdLow = 0x0000;
    CAN_Filter.CAN_FilterFIFOAssignment = CAN_Filter_FIFO0;
    CAN_Filter.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_Filter);

    NVIC_InitTypeDef  NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);

}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
    if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) != RESET)
    {
        CanRxMsg rx_msg = {0};
        CAN_Receive(CAN1, CAN_FIFO0, &rx_msg);
        uart_write(UART_1, rx_msg.Data, 8);
    }
}
