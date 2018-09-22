/*================================================================
*                source for rtos_learn
*   
*   filename   : timer.h
*   author     : chenjiang
*   date       : 2018-08-04
*   description: timer
*
================================================================*/

#ifndef TIMER_H__
#define TIMER_H__

#include "bsp_public.h"

enum timer_idx {timer_1, timer_2};

struct timer_cfg {
    enum timer_idx idx;
    struct {
       uint32_t tick; 
       enum {COUNTER_UP = 0x0, COUNTER_DOWN} mode;
    } base;
    struct {
        enum { // 同时输出波个数
            TIMER_OUT_NUMBER_1, TIMER_OUT_NUMBER_2, 
            TIMER_OUT_NUMBER_3, TIMER_OUT_NUMBER_4} number;
        enum {POLARITY_HIGH, POLARITY_LOW} polarity;
    } out;
};

#endif /* TIMER_H__ */


