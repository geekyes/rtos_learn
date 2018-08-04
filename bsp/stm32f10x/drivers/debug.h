/*================================================================
*                source for peripheral_module_test
*   
*   filename   : debug.h
*   author     : chenjiang
*   date       : 2018-07-24
*   description: µ˜ ‘œ‡πÿ
*
================================================================*/

#ifndef DEBUG_H__
#define DEBUG_H__

#include <stdio.h>
#include <stdint.h>

#define DEBUG_ASSERT(expr) \
    ((expr) ? (void)0 : assert_failed((uint8_t *)__FILE__, __LINE__))

void assert_failed(uint8_t *file_name, size_t line);

#endif /* DEBUG_H__ */


