/*================================================================
*                source for rtos_learn
*   
*   filename   : pin.c
*   author     : chenjiang
*   date       : 2018-08-04
*   description: 对 IO 初始化，操作
*
================================================================*/

#include "pin.h"

#define PIN_INVALID (-1)
#define PIN_DEFAULT {-1, 0, NULL, 0}
#define MAP_PIN(IDX, RCC, GPIOX, PINX) { \
    IDX, RCC_##RCC##Periph_GPIO##GPIOX, GPIO##GPIOX, GPIO_Pin_##PINX \
}

struct pin_instance {
    int idx;
    uint32_t rcc;
    GPIO_TypeDef *gpiox; // 是哪个 portx
    uint16_t pinx; // portx 的哪个 pinx
};

static const struct pin_instance pin_instance_map[] = {
#if (STM32F10X_PIN_NUMBERS == 48)
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(2, APB2, C, 13),
    MAP_PIN(3, APB2, C, 14),
    MAP_PIN(4, APB2, C, 15),
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(10, APB2, A, 0),
    MAP_PIN(11, APB2, A, 1),
    MAP_PIN(12, APB2, A, 2),
    MAP_PIN(13, APB2, A, 3),
    MAP_PIN(14, APB2, A, 4),
    MAP_PIN(15, APB2, A, 5),
    MAP_PIN(16, APB2, A, 6),
    MAP_PIN(17, APB2, A, 7),
    MAP_PIN(18, APB2, B, 0),
    MAP_PIN(19, APB2, B, 1),
    MAP_PIN(20, APB2, B, 2),
    MAP_PIN(21, APB2, B, 10),
    MAP_PIN(22, APB2, B, 11),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(25, APB2, B, 12),
    MAP_PIN(26, APB2, B, 13),
    MAP_PIN(27, APB2, B, 14),
    MAP_PIN(28, APB2, B, 15),
    MAP_PIN(29, APB2, A, 8),
    MAP_PIN(30, APB2, A, 9),
    MAP_PIN(31, APB2, A, 10),
    MAP_PIN(32, APB2, A, 11),
    MAP_PIN(33, APB2, A, 12),
    MAP_PIN(34, APB2, A, 13),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(37, APB2, A, 14),
    MAP_PIN(38, APB2, A, 15),
    MAP_PIN(39, APB2, B, 3),
    MAP_PIN(40, APB2, B, 4),
    MAP_PIN(41, APB2, B, 5),
    MAP_PIN(42, APB2, B, 6),
    MAP_PIN(43, APB2, B, 7),
    PIN_DEFAULT,
    MAP_PIN(45, APB2, B, 8),
    MAP_PIN(46, APB2, B, 9),
    PIN_DEFAULT,
    PIN_DEFAULT,
#endif
#if (STM32F10X_PIN_NUMBERS == 64)
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(2, APB2, C, 13),
    MAP_PIN(3, APB2, C, 14),
    MAP_PIN(4, APB2, C, 15),
    MAP_PIN(5, APB2, D, 0),
    MAP_PIN(6, APB2, D, 1),
    PIN_DEFAULT,
    MAP_PIN(8, APB2, C, 0),
    MAP_PIN(9, APB2, C, 1),
    MAP_PIN(10, APB2, C, 2),
    MAP_PIN(11, APB2, C, 3),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(14, APB2, A, 0),
    MAP_PIN(15, APB2, A, 1),
    MAP_PIN(16, APB2, A, 2),
    MAP_PIN(17, APB2, A, 3),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(20, APB2, A, 4),
    MAP_PIN(21, APB2, A, 5),
    MAP_PIN(22, APB2, A, 6),
    MAP_PIN(23, APB2, A, 7),
    MAP_PIN(24, APB2, C, 4),
    MAP_PIN(25, APB2, C, 5),
    MAP_PIN(26, APB2, B, 0),
    MAP_PIN(27, APB2, B, 1),
    MAP_PIN(28, APB2, B, 2),
    MAP_PIN(29, APB2, B, 10),
    MAP_PIN(30, APB2, B, 11),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(33, APB2, B, 12),
    MAP_PIN(34, APB2, B, 13),
    MAP_PIN(35, APB2, B, 14),
    MAP_PIN(36, APB2, B, 15),
    MAP_PIN(37, APB2, C, 6),
    MAP_PIN(38, APB2, C, 7),
    MAP_PIN(39, APB2, C, 8),
    MAP_PIN(40, APB2, C, 9),
    MAP_PIN(41, APB2, A, 8),
    MAP_PIN(42, APB2, A, 9),
    MAP_PIN(43, APB2, A, 10),
    MAP_PIN(44, APB2, A, 11),
    MAP_PIN(45, APB2, A, 12),
    MAP_PIN(46, APB2, A, 13),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(49, APB2, A, 14),
    MAP_PIN(50, APB2, A, 15),
    MAP_PIN(51, APB2, C, 10),
    MAP_PIN(52, APB2, C, 11),
    MAP_PIN(53, APB2, C, 12),
    MAP_PIN(54, APB2, D, 2),
    MAP_PIN(55, APB2, B, 3),
    MAP_PIN(56, APB2, B, 4),
    MAP_PIN(57, APB2, B, 5),
    MAP_PIN(58, APB2, B, 6),
    MAP_PIN(59, APB2, B, 7),
    PIN_DEFAULT,
    MAP_PIN(61, APB2, B, 8),
    MAP_PIN(62, APB2, B, 9),
    PIN_DEFAULT,
    PIN_DEFAULT,
#endif
#if (STM32F10X_PIN_NUMBERS == 100)
    PIN_DEFAULT,
    MAP_PIN(1, APB2, E, 2),
    MAP_PIN(2, APB2, E, 3),
    MAP_PIN(3, APB2, E, 4),
    MAP_PIN(4, APB2, E, 5),
    MAP_PIN(5, APB2, E, 6),
    PIN_DEFAULT,
    MAP_PIN(7, APB2, C, 13),
    MAP_PIN(8, APB2, C, 14),
    MAP_PIN(9, APB2, C, 15),
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(15, APB2, C, 0),
    MAP_PIN(16, APB2, C, 1),
    MAP_PIN(17, APB2, C, 2),
    MAP_PIN(18, APB2, C, 3),
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(23, APB2, A, 0),
    MAP_PIN(24, APB2, A, 1),
    MAP_PIN(25, APB2, A, 2),
    MAP_PIN(26, APB2, A, 3),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(29, APB2, A, 4),
    MAP_PIN(30, APB2, A, 5),
    MAP_PIN(31, APB2, A, 6),
    MAP_PIN(32, APB2, A, 7),
    MAP_PIN(33, APB2, C, 4),
    MAP_PIN(34, APB2, C, 5),
    MAP_PIN(35, APB2, B, 0),
    MAP_PIN(36, APB2, B, 1),
    MAP_PIN(37, APB2, B, 2),
    MAP_PIN(38, APB2, E, 7),
    MAP_PIN(39, APB2, E, 8),
    MAP_PIN(40, APB2, E, 9),
    MAP_PIN(41, APB2, E, 10),
    MAP_PIN(42, APB2, E, 11),
    MAP_PIN(43, APB2, E, 12),
    MAP_PIN(44, APB2, E, 13),
    MAP_PIN(45, APB2, E, 14),
    MAP_PIN(46, APB2, E, 15),
    MAP_PIN(47, APB2, B, 10),
    MAP_PIN(48, APB2, B, 11),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(51, APB2, B, 12),
    MAP_PIN(52, APB2, B, 13),
    MAP_PIN(53, APB2, B, 14),
    MAP_PIN(54, APB2, B, 15),
    MAP_PIN(55, APB2, D, 8),
    MAP_PIN(56, APB2, D, 9),
    MAP_PIN(57, APB2, D, 10),
    MAP_PIN(58, APB2, D, 11),
    MAP_PIN(59, APB2, D, 12),
    MAP_PIN(60, APB2, D, 13),
    MAP_PIN(61, APB2, D, 14),
    MAP_PIN(62, APB2, D, 15),
    MAP_PIN(63, APB2, C, 6),
    MAP_PIN(64, APB2, C, 7),
    MAP_PIN(65, APB2, C, 8),
    MAP_PIN(66, APB2, C, 9),
    MAP_PIN(67, APB2, A, 8),
    MAP_PIN(68, APB2, A, 9),
    MAP_PIN(69, APB2, A, 10),
    MAP_PIN(70, APB2, A, 11),
    MAP_PIN(71, APB2, A, 12),
    MAP_PIN(72, APB2, A, 13),
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(76, APB2, A, 14),
    MAP_PIN(77, APB2, A, 15),
    MAP_PIN(78, APB2, C, 10),
    MAP_PIN(79, APB2, C, 11),
    MAP_PIN(80, APB2, C, 12),
    MAP_PIN(81, APB2, D, 0),
    MAP_PIN(82, APB2, D, 1),
    MAP_PIN(83, APB2, D, 2),
    MAP_PIN(84, APB2, D, 3),
    MAP_PIN(85, APB2, D, 4),
    MAP_PIN(86, APB2, D, 5),
    MAP_PIN(87, APB2, D, 6),
    MAP_PIN(88, APB2, D, 7),
    MAP_PIN(89, APB2, B, 3),
    MAP_PIN(90, APB2, B, 4),
    MAP_PIN(91, APB2, B, 5),
    MAP_PIN(92, APB2, B, 6),
    MAP_PIN(93, APB2, B, 7),
    PIN_DEFAULT,
    MAP_PIN(95, APB2, B, 8),
    MAP_PIN(96, APB2, B, 9),
    MAP_PIN(97, APB2, E, 0),
    MAP_PIN(98, APB2, E, 1),
    PIN_DEFAULT,
    PIN_DEFAULT,
#endif
#if (STM32F10X_PIN_NUMBERS == 144)
    PIN_DEFAULT,
    MAP_PIN(1, APB2, E, 2),
    MAP_PIN(2, APB2, E, 3),
    MAP_PIN(3, APB2, E, 4),
    MAP_PIN(4, APB2, E, 5),
    MAP_PIN(5, APB2, E, 6),
    PIN_DEFAULT,
    MAP_PIN(7, APB2, C, 13),
    MAP_PIN(8, APB2, C, 14),
    MAP_PIN(9, APB2, C, 15),
    MAP_PIN(10, APB2, F, 0),
    MAP_PIN(11, APB2, F, 1),
    MAP_PIN(12, APB2, F, 2),
    MAP_PIN(13, APB2, F, 3),
    MAP_PIN(14, APB2, F, 4),
    MAP_PIN(15, APB2, F, 5),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(18, APB2, F, 6),
    MAP_PIN(19, APB2, F, 7),
    MAP_PIN(20, APB2, F, 8),
    MAP_PIN(21, APB2, F, 9),
    MAP_PIN(22, APB2, F, 10),
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(26, APB2, C, 0),
    MAP_PIN(27, APB2, C, 1),
    MAP_PIN(28, APB2, C, 2),
    MAP_PIN(29, APB2, C, 3),
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(34, APB2, A, 0),
    MAP_PIN(35, APB2, A, 1),
    MAP_PIN(36, APB2, A, 2),
    MAP_PIN(37, APB2, A, 3),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(40, APB2, A, 4),
    MAP_PIN(41, APB2, A, 5),
    MAP_PIN(42, APB2, A, 6),
    MAP_PIN(43, APB2, A, 7),
    MAP_PIN(44, APB2, C, 4),
    MAP_PIN(45, APB2, C, 5),
    MAP_PIN(46, APB2, B, 0),
    MAP_PIN(47, APB2, B, 1),
    MAP_PIN(48, APB2, B, 2),
    MAP_PIN(49, APB2, F, 11),
    MAP_PIN(50, APB2, F, 12),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(53, APB2, F, 13),
    MAP_PIN(54, APB2, F, 14),
    MAP_PIN(55, APB2, F, 15),
    MAP_PIN(56, APB2, G, 0),
    MAP_PIN(57, APB2, G, 1),
    MAP_PIN(58, APB2, E, 7),
    MAP_PIN(59, APB2, E, 8),
    MAP_PIN(60, APB2, E, 9),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(63, APB2, E, 10),
    MAP_PIN(64, APB2, E, 11),
    MAP_PIN(65, APB2, E, 12),
    MAP_PIN(66, APB2, E, 13),
    MAP_PIN(67, APB2, E, 14),
    MAP_PIN(68, APB2, E, 15),
    MAP_PIN(69, APB2, B, 10),
    MAP_PIN(70, APB2, B, 11),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(73, APB2, B, 12),
    MAP_PIN(74, APB2, B, 13),
    MAP_PIN(75, APB2, B, 14),
    MAP_PIN(76, APB2, B, 15),
    MAP_PIN(77, APB2, D, 8),
    MAP_PIN(78, APB2, D, 9),
    MAP_PIN(79, APB2, D, 10),
    MAP_PIN(80, APB2, D, 11),
    MAP_PIN(81, APB2, D, 12),
    MAP_PIN(82, APB2, D, 13),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(85, APB2, D, 14),
    MAP_PIN(86, APB2, D, 15),
    MAP_PIN(87, APB2, G, 2),
    MAP_PIN(88, APB2, G, 3),
    MAP_PIN(89, APB2, G, 4),
    MAP_PIN(90, APB2, G, 5),
    MAP_PIN(91, APB2, G, 6),
    MAP_PIN(92, APB2, G, 7),
    MAP_PIN(93, APB2, G, 8),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(96, APB2, C, 6),
    MAP_PIN(97, APB2, C, 7),
    MAP_PIN(98, APB2, C, 8),
    MAP_PIN(99, APB2, C, 9),
    MAP_PIN(100, APB2, A, 8),
    MAP_PIN(101, APB2, A, 9),
    MAP_PIN(102, APB2, A, 10),
    MAP_PIN(103, APB2, A, 11),
    MAP_PIN(104, APB2, A, 12),
    MAP_PIN(105, APB2, A, 13),
    PIN_DEFAULT,
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(109, APB2, A, 14),
    MAP_PIN(110, APB2, A, 15),
    MAP_PIN(111, APB2, C, 10),
    MAP_PIN(112, APB2, C, 11),
    MAP_PIN(113, APB2, C, 12),
    MAP_PIN(114, APB2, D, 0),
    MAP_PIN(115, APB2, D, 1),
    MAP_PIN(116, APB2, D, 2),
    MAP_PIN(117, APB2, D, 3),
    MAP_PIN(118, APB2, D, 4),
    MAP_PIN(119, APB2, D, 5),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(122, APB2, D, 6),
    MAP_PIN(123, APB2, D, 7),
    MAP_PIN(124, APB2, G, 9),
    MAP_PIN(125, APB2, G, 10),
    MAP_PIN(126, APB2, G, 11),
    MAP_PIN(127, APB2, G, 12),
    MAP_PIN(128, APB2, G, 13),
    MAP_PIN(129, APB2, G, 14),
    PIN_DEFAULT,
    PIN_DEFAULT,
    MAP_PIN(132, APB2, G, 15),
    MAP_PIN(133, APB2, B, 3),
    MAP_PIN(134, APB2, B, 4),
    MAP_PIN(135, APB2, B, 5),
    MAP_PIN(136, APB2, B, 6),
    MAP_PIN(137, APB2, B, 7),
    PIN_DEFAULT,
    MAP_PIN(139, APB2, B, 8),
    MAP_PIN(140, APB2, B, 9),
    MAP_PIN(141, APB2, E, 0),
    MAP_PIN(142, APB2, E, 1),
    PIN_DEFAULT,
    PIN_DEFAULT,
#endif
};

static const GPIOMode_TypeDef mode[] = {
    GPIO_Mode_AIN, /* 0 */
    GPIO_Mode_IN_FLOATING, /* 1 */
    GPIO_Mode_IPD, /* 2 */
    GPIO_Mode_IPU, /* 3 */
    GPIO_Mode_Out_OD, /* 4 */
    GPIO_Mode_Out_PP, /* 5 */
    GPIO_Mode_AF_OD, /* 6 */
    GPIO_Mode_AF_PP /* 7 */
};


/* init */
void pin_init(struct pin_cfg *cfg)
{
    DEBUG_ASSERT(cfg);
    DEBUG_ASSERT(pin_instance_map[cfg->idx].idx != PIN_INVALID);

    struct pin_instance inst = pin_instance_map[cfg->idx];
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(inst.rcc, ENABLE);

    GPIO_InitStructure.GPIO_Pin = inst.pinx;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = mode[cfg->mode];
    GPIO_Init(inst.gpiox, &GPIO_InitStructure);
}

void pin_write(uint8_t idx, enum out_level level)
{
    DEBUG_ASSERT(pin_instance_map[idx].idx != PIN_INVALID);

    struct pin_instance inst = pin_instance_map[idx];
    BitAction bit_val = Bit_RESET;

    switch (level)
    {
        case BIT_HIGH:
            bit_val = Bit_SET;
            break;
        case BIT_LOW:
            bit_val = Bit_RESET;
            break;
        default: 
            break;
    }
    GPIO_WriteBit(inst.gpiox, inst.pinx, bit_val);
}

enum out_level pin_read(uint8_t idx)
{
    DEBUG_ASSERT(pin_instance_map[idx].idx != PIN_INVALID);

    struct pin_instance inst = pin_instance_map[idx];
    enum out_level bit_val = BIT_LOW;
    
    switch (GPIO_ReadInputDataBit(inst.gpiox, inst.pinx))
    {
        case Bit_SET:
            bit_val = BIT_HIGH;
        case Bit_RESET:
            bit_val = BIT_LOW;
        default: 
            break;
    }

    return bit_val;
}
