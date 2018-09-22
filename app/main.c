/*================================================================
*                source for rtos_learn
*   
*   filename   : main.c
*   author     : chenjiang
*   date       : 2018-06-19
*   description: test
*
================================================================*/

#include <stdint.h>

#include "uart.h"
#include "can.h"

#define TASK_STACK_SIZE ((uint8_t)200 / 4)
/* THUMB指令集USR工作模式掩码 */
#define MODE_USR 0x01000000

typedef void (*task_t)(void);

struct task_runtime_data {
    uint8_t number;
    uint32_t *sp;
    uint32_t stack[TASK_STACK_SIZE];
};

/* usart_rcv handler */
static void usart_rcv(uint8_t *p_data, uint8_t size);
static void usart_rcv_task(void);

static void delay(uint32_t cnt);

static void task_one(void);
static void task_two(void);

static void task_create(uint8_t num, task_t task,\
        struct task_runtime_data *p_task);
static void task_start(void);
static void task_switch(struct task_runtime_data *p_curr_task,\
        struct task_runtime_data *p_next_task);

extern void asm_task_start(void);
extern void asm_task_switch(void);

static struct {
    volatile uint8_t size;
    uint8_t data[RCV_BUFF_SIZE];
} rcv = {0, {0}};

struct task_runtime_data root_task = {0};
struct task_runtime_data task1 = {0};
struct task_runtime_data task2 = {0};
uint32_t **p_curr_sp = NULL;
uint32_t *next_sp = NULL;

int main(void)
{
    /* init */
    struct uart_cfg cfg = {UART_1, 115200, usart_rcv};
    uart_init(&cfg);
    can_init();
    task_create(0, usart_rcv_task, &root_task);
    task_create(1, task_one, &task1);
    task_create(2, task_two, &task2);
    
    task_start();
    
    return 0;
}

/* usart_rcv handler */
static void usart_rcv(uint8_t *p_data, uint8_t size)
{
    DEBUG_ASSERT(size > 0 && p_data);
    
    memcpy(rcv.data, p_data, size);
    rcv.size = size;
}

static void usart_rcv_task(void)
{
    while(1)
    {
        if (rcv.size)
        {
            uart_write(UART_1, (uint8_t*)"rcv result: \r\n", 16);
            uart_write(UART_1, rcv.data, rcv.size);
            uart_write(UART_1, (uint8_t*)"\r\n", 2);
            rcv.size = 0;
            task_switch(&root_task, &task1);
        }
    }
}

static void task_one(void)
{
    while (1)
    {
        uart_write(UART_1, (uint8_t*)"can send run...\r\n", 17);
        CanTxMsg tx_msg = {
            #ifdef STM32F10X_HD
            .StdId = 0x100,
            .Data ={0x31, 0x63, 0x68, 0x65, 0x6e, 0x6a, 0xd, 0xa},
            #endif
            #ifdef STM32F10X_MD
            .StdId = 0x101,
            .Data ={0x32, 0x63, 0x68, 0x65, 0x6e, 0x6a, 0xd, 0xa},
            #endif
            .ExtId = 0x0,
            .IDE = CAN_Id_Standard,
            .RTR = CAN_RTR_Data,
            .DLC = 8,
        };
        (void)CAN_Transmit(CAN1, &tx_msg);
        /* delay */
        delay(1000);
        /* task switch */
        /* task_switch(&task1, &task2); */
    }
}

static void task_two(void)
{
    while (1)
    {
        uart_write(UART_1, (uint8_t*)"task2 run...\r\n", 14);
        /* delay */
        delay(1000);
        /* task switch */
        task_switch(&task2, &root_task);
    }
}

static void task_create(uint8_t num, task_t task,\
        struct task_runtime_data *p_task)
{
    DEBUG_ASSERT(p_task);

    uint8_t i = 0;

    p_task->number = num;
    p_task->stack[TASK_STACK_SIZE - ++i] = (uint32_t)(task); /* r14 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r12 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r11 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r10 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r9 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r8 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r7 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r6 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r5 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r4 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r3 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r2 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r1 */
    p_task->stack[TASK_STACK_SIZE - ++i] = 0; /* r0 */
    p_task->stack[TASK_STACK_SIZE - ++i] = MODE_USR; /* cpsr */
    p_task->sp = &p_task->stack[TASK_STACK_SIZE - i]; /* r13 */
}

static void task_start(void)
{
    /* push 操作行为：先减少后操作 */
    /* pop 操作行为：先操作后增加 */
    p_curr_sp = &task1.sp;
    asm_task_start();
}

static void task_switch(struct task_runtime_data *p_curr_task,\
        struct task_runtime_data *p_next_task)
{
    DEBUG_ASSERT(p_curr_task && p_next_task);

    p_curr_sp = &p_curr_task->sp;
    next_sp = p_next_task->sp;
    asm_task_switch();
}

static void delay(uint32_t cnt)
{
    for (volatile uint32_t i = 0; i < cnt; i++)
    {
        for (volatile uint32_t j = 0; j < 7200; j++)
        {
            ;
        }
    }
}

