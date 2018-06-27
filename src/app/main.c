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

#define TASK_STACK_SIZE ((uint8_t)110 / 4)
/* THUMB指令集USR工作模式掩码 */
#define MODE_USR 0x01000000

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

static void task_create(uint8_t num, struct task_runtime_data *p_task);
static void task_start(void);
static void task_switch(struct task_runtime_data *p_curr_task);

/* test */
static void asm_test(void);

static struct {
    volatile uint8_t size;
    uint8_t data[RCV_BUFF_SIZE];
} rcv = {0, {0}};

struct task_runtime_data root_task = {0};
struct task_runtime_data task1 = {0};
struct task_runtime_data task2 = {0};

int main(void)
{
    /* init */
    (void)usart_driver.init(usart_rcv, 115200);
    task_create(0, &root_task);
    task_create(1, &task1);
    task_create(2, &task2);
    
    task_start();
    
    while (1)
    {
        /* space */
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
        delay(100);
        /* TODO task switch */
    }
}

static void task_two(void)
{
    while (1)
    {
        usart_driver.write((uint8_t*)"task2 run...\r\n", 14);
        /* delay */
        delay(100);
        /* TODO task switch */
    }
}

static void task_create(uint8_t num, struct task_runtime_data *p_task)
{
    if (NULL == p_task)
    {
        return ;
    }

    uint8_t i = 0;

    p_task->number = num;
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r0 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r1 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r2 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r3 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r4 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r5 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r6 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r7 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r8 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r9 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r10 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r11 */
    p_task->data[TASK_STACK_SIZE - ++i] = 0; /* r12 */
    p_task->data[TASK_STACK_SIZE - ++i] = (uint32_t)(p_task->func); /* r14 */
    p_task->data[TASK_STACK_SIZE - ++i] = MODE_USR; /* cpsr */
    p_task->sp = &p_task->data[TASK_STACK_SIZE - i]; /* r13 */
}

static void task_start(void)
{
    /* push 操作行为：先减少后操作 */
    /* pop 操作行为：先操作后增加 */
    asm volatile (
            /* 获取 task 的 sp */
            "mov sp, %0\n\t"
            /* cpsr 还原 */
            "push r0"
            "msr cpsr, r0\n\t"
            /* 初始化 r0 - r12, r14 寄存器 */
            "ldmia sp!, {r0 - r12, r14}\n\t"
            "mov pc, %1"
            :: "r" (root_task.sp), "r" (root_task.func)
            );
}

static void task_switch(struct task_runtime_data *p_curr_task)
{
    asm volatile (
            /* 保存当前任务的环境到当前任务栈 */
            "mrs r0, cpsr\n\t"
            "\n\t"
            "stmdb r13, {r0 - r12, r14}\n\t"
            );
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

/* test */
static void asm_test(void)
{
    asm volatile ("nop");
}

