
.syntax unified
.cpu cortex-m3
.fpu softvfp
.thumb
.text

    .global asm_task_start
    .type asm_task_start, %function
asm_task_start:
    @ ; 获取 task 的 sp
    ldr r0, = p_curr_sp
    ldr r1, [r0]
    ldr sp, [r1]
    @ ; 切换到 task_one 
    pop {r0}
    msr xpsr, r0
    pop {r0 - r12, r15}

    .global asm_task_switch
    .type asm_task_switch, %function
asm_task_switch:
    @ ; 保存 curr task 的环境到当前任务栈
    push {r0 - r12, r14}
    @ ; 保存 curr task xpsr
    mrs r0, xpsr
    push {r0}
    @ ; 保存 curr task sp
    ldr r0, = p_curr_sp
    ldr r1, [r0]
    str sp, [r1]
    @ ; 获取 next task sp
    ldr r0, = next_sp
    ldr sp, [r0]
    @ ; 切换到 next task
    pop {r0}
    msr xpsr, r0
    pop {r0 - r12, r15}
