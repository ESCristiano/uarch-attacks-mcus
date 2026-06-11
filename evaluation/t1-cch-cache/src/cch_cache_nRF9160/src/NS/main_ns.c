/*
 * Copyright (c) 2017-2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include "tfm_api.h"
#include "tfm_ns_interface.h"
#include "tfm_plat_ns.h"
#include "driver/Driver_USART.h"
#include "device_cfg.h"
#include "tfm_log.h"
#include "uart_stdout.h"
#include "cmsis.h"

#define SYSTICK_AVAILABLE
#include "uArchChannel_lib.h"

/**
 * \brief Platform peripherals and devices initialization.
 *        Can be overridden for platform specific initialization.
 *
 * \return  ARM_DRIVER_OK if the initialization succeeds
 */
__attribute__((weak)) int32_t tfm_ns_platform_init(void)
{
    stdio_init();

    return ARM_DRIVER_OK;
}

/**
 * \brief Platform peripherals and devices de-initialization.
 *        Can be overridden for platform specific initialization.
 *
 * \return  ARM_DRIVER_OK if the de-initialization succeeds
 */
__attribute__((weak)) int32_t tfm_ns_platform_uninit(void)
{
    stdio_uninit();

    return ARM_DRIVER_OK;
}


//__attribute__((weak))
int32_t tfm_ns_interface_dispatch(veneer_fn fn,
                                  uint32_t arg0, uint32_t arg1,
                                  uint32_t arg2, uint32_t arg3)
{
    int32_t result;

    /* TFM request protected by NS lock */
//    while (os_wrapper_mutex_acquire(ns_lock_handle, OS_WRAPPER_WAIT_FOREVER)
//            != OS_WRAPPER_SUCCESS);

    result = fn(arg0, arg1, arg2, arg3);

   // while (os_wrapper_mutex_release(ns_lock_handle) != OS_WRAPPER_SUCCESS);

    return result;
}
//__attribute__((weak))
enum tfm_status_e tfm_ns_interface_init(void)
{
//    void *handle;
//
//    handle = os_wrapper_mutex_create();
//    if (!handle) {
//        return TFM_ERROR_GENERIC;
//    }
//
//    ns_lock_handle = handle;
    return TFM_SUCCESS;
}

#include "psa_manifest/sid.h"

//------------------------------------------------------------------------------
//---------------------------- TF-M Interact w/ covert Secure App  -------------
//------------------------------------------------------------------------------
int covert_channel_test() 
{
    char lines;
    int probe_time;
    #ifdef TROJAN_S
        char str3[128], str4[128];
        struct psa_invec invecs[1] = {{&lines, sizeof(lines)}};

        struct psa_outvec outvecs[2] = {{str3, sizeof(str3)},
                                        {str4, sizeof(str4)}};
        psa_handle_t handle;
    #endif

    SysTick->CTRL = 0x0;
    SysTick->CTRL = 0x0;
    SysTick->LOAD  = (uint32_t)(1000000 - 1);/* set reload register */
    SysTick->VAL = 0;

    #ifdef REVERSE_ENG_CACHE
        // Put in cache config, direct mapped (i.e., I_N_CACHE_SETS = CACHE_SIZE/CACHE_LINE_SIZE)
        // Run this (i) with TEST_DIRECT_MAPPED and (ii) TEST_2_WAYS 
        // if the time (i) is much less then (ii), the cache is in direct mapped configuration
        // if time (i) is close to or equal time (ii), the cache is in 2-way configuration 
        // - (it maybe dificulte to be exactly equal due to cache interferences in the code outside of the prime/probe, like calls and etc... )
        #ifdef TEST_DIRECT_MAPPED
            touch_s(I_N_CACHE_SETS/2); 
        #elif defined(TEST_2_WAYS)
            touch_ns(I_N_CACHE_SETS/2);
            touch_s(I_N_CACHE_SETS/2);
        #endif
        probe_time = probe_all();
        LOG_MSG("%d\n", probe_time);
    #else
        for (int input_symbol = 0; input_symbol < I_N_CACHE_SETS; input_symbol++)
        {
          for(int sample = 0; sample < 1000; sample++)
            {
              lines = input_symbol;
              //prime all I-cache 
              prime();
              // touche n_lines
              #ifdef TROJAN_S
                  // Call the S world trojan
                  psa_call(TFM_COVERT_SERVICE1_HANDLE, PSA_IPC_CALL, invecs, 1, outvecs, 2);
              #else
                  // Call the NS world trojan, to measure the channel without ns interference
                  touch_ns(input_symbol);
              #endif
              // measure the time to access the N_CAHCE_SETS I-cache line buffer
              probe_time = probe_all();
              #ifdef GNUPLOT
                LOG_MSG("%d %d\n", input_symbol, probe_time);
              #endif

              #ifdef LEAKIEST
                LOG_MSG("(%d, %d)\r\n", input_symbol, probe_time);
              #endif

            }
        }
    #endif

    return 1;
}


/**
 * \brief main() function
 */
#ifndef __GNUC__
__attribute__((noreturn))
#endif
int main(void)
{

    if (tfm_ns_platform_init() != ARM_DRIVER_OK) {
        /* Avoid undefined behavior if platform init failed */
        while(1);
    }
    
    #ifdef STM32L552
        if (READ_BIT(ICACHE->CR, ICACHE_CR_EN) != 0U)
         {
           LOG_MSG("Error! I-Cache already enabled\n");
         }
         else
         {
             SET_BIT(ICACHE->CR, ICACHE_CR_EN);
         }
    #endif

    LOG_MSG("Non-Secure system starting | Covert Channel...\r\n");
   
    covert_channel_test();

    for (;;) {
    }
}
