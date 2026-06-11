/*
 * Copyright (c) 2018-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#ifndef __TFM_PERIPHERALS_DEF_H__
#define __TFM_PERIPHERALS_DEF_H__

//------added by cris------------------------
#include "platform_irq.h"
//--------------------------------------------

struct platform_data_t;

//------added by cris------------------------
#define TFM_TIMER_IRQ           (TIM7_IRQn) // TIM7 global interrupt | "platform_irq.h" -> stm32l5xx.h -> "stm32l552xx.h"
//--------------------------------------------
#define TFM_PERIPHERAL_STD_UART     (0)
#define TFM_PERIPHERAL_TIMER0       (0)




//------added by cris------------------------
    extern struct platform_data_t timer6;
    extern struct platform_data_t timer7;
    extern struct platform_data_t rcc;
    
    #define TFM_PERIPHERAL_WALL_CLOCK &timer6
    #define TFM_PERIPHERAL_TIMER &timer7
    #define TFM_PERIPHERAL_RCC &rcc
//--------------------------------------------

#endif /* __TFM_PERIPHERALS_DEF_H__ */
