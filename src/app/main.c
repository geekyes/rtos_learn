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

#define TASK_STACK_SIZE ((uint8_t)110)

struct task_runtime_data {
    uint8_t number;
    uint8_t stack[TASK_STACK_SIZE];
    void (*func)(void);
};

/* usart_rcv handler */
static void usart_rcv(uint8_t *p_data, uint8_t size);
static void usart_rcv_task(void);

static void task_one(void);
static void task_two(void);

static void task_create(struct task_runtime_data *p_task);
static void task_start(void);
static void task_switch(struct task_runtime_data *p_task);

static struct {
    volatile uint8_t size;
    uint8_t data[RCV_BUFF_SIZE];
} rcv = {0, {0}};

struct task_runtime_data task1 = {1, {0}, task_one};
struct task_runtime_data task2 = {2, {0}, task_two};
struct task_runtime_data root_task = {3, {0}, usart_rcv_task};

int main(void)
{
    /* init */
    (void)usart_driver.init(usart_rcv, 115200);
    task_create(&task1);
    task_create(&task2);

    task_start();

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

static void usart_rcv_task(void)
{
    while(1)
    {
        if (rcv.size)
        {
            (void)usart_driver.write((uint8_t*)"\r\nrcv result: \r\n", 16);
            (void)usart_driver.write(rcv.data, rcv.size);
            rcv.size = 0;
        }
    }
}

static void task_one(void)
{
    while (1)
    {
        usart_driver.write((uint8_t*)"task1 run...\r\n", 14);
        /* delay */
        /* TODO task switch */
    }
}

static void task_two(void)
{
    while (1)
    {
        usart_driver.write((uint8_t*)"task2 run...\r\n", 14);
        /* delay */
        /* TODO task switch */
    }
}

static void task_create(struct task_runtime_data *p_task)
{
}

static void task_start(void)
{
    asm volatile (
            /* 设置 sp */
            "ldr r13, =%0\r\n
            bl %1"
            : "m" (&task_one.stack[TASK_STACK_SIZE / 2]), "m" (task_one.func)
            );
}

static void task_switch(struct task_runtime_data *p_task)
{
}

