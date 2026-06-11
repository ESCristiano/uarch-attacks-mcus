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
#include "covert_src/covert.h"

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

#ifdef STM32L552
    void config_mpu_half_cacheable(){
        ARM_MPU_Disable();
        ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR(       /* Normal memory */
          ARM_MPU_ATTR_NON_CACHEABLE, /* Outer non-cacheable, normal memory*/
          ARM_MPU_ATTR_NON_CACHEABLE  /* Inner non-cacheable, normal memory */
        ));

        ARM_MPU_SetMemAttr(1UL, ARM_MPU_ATTR(       /* Normal memory */
          ARM_MPU_ATTR_NON_CACHEABLE, /* Outer non-cacheable, normal memory*/
          ARM_MPU_ATTR_NON_CACHEABLE  /* Inner non-cacheable, normal memory */
        ));

        ARM_MPU_SetRegion(0UL,
           ARM_MPU_RBAR(0x08044000UL, ARM_MPU_SH_NON, 1UL, 1UL, 0UL),  /* Non-shareable, read only, non-privileged, execute code */
           ARM_MPU_RLAR(0x08044800UL, 0UL)                             /* 4kB memory block using Attr 0 */
         );

        ARM_MPU_SetRegion(1UL,
           ARM_MPU_RBAR(0x08045000UL, ARM_MPU_SH_NON, 1UL, 1UL, 0UL),  /* Non-shareable, read only, non-privileged, execute code */
           ARM_MPU_RLAR(0x08045800UL, 0UL)                             /* 4kB memory block using Attr 0 */
         );
    //  // 0 -> memory region 0 | (8h44) 0100 -> Normal memory, Outer non-cacheable | 0100 ->  Normal memory, Inner non-cacheable.
    //  ARM_MPU_SetMemAttr(0, 0x44);
    //  //RBAR = 0x08060006 (27h080600 + 5b00110) --> 0x08060000 -> base addr | 00 -> Non-shareable | 11 -> Read-only by any privilege level | 0 -> executable
    //  //RLAR = 0x08061001 (27h08061FE + 4b0001)  --> 0x08061FFF (depois é acrescentado 1F; 27h08061FE + 5h1F)-> addr limit | 000 -> region number | 1 -> region enable
    //  ARM_MPU_SetRegion(0, 0x08060006, 0x08061fE1); // region 0,
        ARM_MPU_Enable(4); // 4 -> enables use of the default memory map for regions non configured
    }
#endif

void config_mpu_only_prime_code_cacheable(unsigned long addr_ns, unsigned long size_ns, unsigned long addr_prime, unsigned long size_prime){
    ARM_MPU_Disable();
    
    // ALL NS memory Non Cacheble ----------------------------------------------------------------------------------------
    ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR(       /* Normal memory */
      ARM_MPU_ATTR_NON_CACHEABLE, /* Outer non-cacheable, normal memory*/
      ARM_MPU_ATTR_NON_CACHEABLE  /* Inner non-cacheable, normal memory */
    ));

    ARM_MPU_SetRegion(0UL,
       ARM_MPU_RBAR(addr_ns, ARM_MPU_SH_NON, 1UL, 1UL, 0UL),  /* Non-shareable, read only, non-privileged, execute code */
       ARM_MPU_RLAR(size_ns, 0UL)                             
     ); 
    //-------------------------------------------------------------------------------------------------------------------

    // Memory for prime the cache cacheable, i.e., block_jumps_x_x. ----------------------------------------------------
    ARM_MPU_SetMemAttr(1UL, ARM_MPU_ATTR(       /* Normal memory */
     ARM_MPU_ATTR_MEMORY_(0UL, 1UL, 1UL, 1UL), /* Outer Write-Back transient with read and write allocate */
    ARM_MPU_ATTR_MEMORY_(0UL, 1UL, 1UL, 1UL)  /* Inner Write-Back transient with read and write allocate */
    ));

    ARM_MPU_SetRegion(1UL,
      ARM_MPU_RBAR(addr_prime, ARM_MPU_SH_OUTER , 1UL, 1UL, 0UL),  /* shareable, read only, non-privileged, execute code */
      ARM_MPU_RLAR(size_prime, 0UL)                            
    );
    //-------------------------------------------------------------------------------------------------------------------

    ARM_MPU_Enable(4); // 4 -> enables use of the default memory map for regions non configured
}

void config_mpu_all_non_cacheable(unsigned long addr_ns, unsigned long size_ns){
    ARM_MPU_Disable();

    // ALL NS memory Non Cacheble ----------------------------------------------------------------------------------------
    ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR(       /* Normal memory */
      ARM_MPU_ATTR_NON_CACHEABLE, /* Outer non-cacheable, normal memory*/
      ARM_MPU_ATTR_NON_CACHEABLE  /* Inner non-cacheable, normal memory */
    ));
    ARM_MPU_SetRegion(0UL,
       ARM_MPU_RBAR(addr_ns, ARM_MPU_SH_NON, 1UL, 1UL, 0UL),  /* Non-shareable, read only, non-privileged, execute code */
       ARM_MPU_RLAR(size_ns, 0UL)                             
     );
    //-------------------------------------------------------------------------------------------------------------------
    
    ARM_MPU_Enable(4); // 4 -> enables use of the default memory map for regions non configured
}

#ifdef STM32L552
    void ICache_invalidation(){
         if (READ_BIT(ICACHE->SR, ICACHE_SR_BUSYF) != 0U)
              {
                return -1;
              }
              else
              {
                /* Make sure BSYENDF is reset before to start cache invalidation */
                CLEAR_BIT(ICACHE->FCR, ICACHE_FCR_CBSYENDF);

                /* Launch cache invalidation */
                SET_BIT(ICACHE->CR, ICACHE_CR_CACHEINV);

                 while (READ_BIT(ICACHE->SR, ICACHE_SR_BSYENDF) == 0U);

              }
              /* Clear BSYENDF */
             WRITE_REG(ICACHE->FCR, ICACHE_FCR_CBSYENDF);
    }
#endif

//------------------------------------------------------------------------------------------------
//---------------------------- TF-M Interact w/ covert Secure App  -------------------------------
//------------------------------------------------------------------------------------------------
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

    #ifdef TROJAN_S
        handle = psa_connect(0x0000F130U, 1);
    #endif

    #ifdef REVERSE_ENG_CACHE
        // Put in cache config, direct mapped (i.e., N_CACHE_SETS = CACHE_SIZE/CACHE_LINE_SIZE)
        // Run this (i) with TEST_DIRECT_MAPPED and (ii) TEST_2_WAYS 
        // if the time (i) is much less then (ii), the cache is in direct mapped configuration
        // if time (i) is close to or equal time (ii), the cache is in 2-way configuration 
        // - (it maybe dificulte to be exactly equal due to cache interferences in the code outside of the prime/probe, like calls and etc... )
        #ifdef TEST_DIRECT_MAPPED
            touch_s(N_CACHE_SETS/2); 
        #elif defined(TEST_2_WAYS)
            touch_ns(N_CACHE_SETS/2);
            touch_s(N_CACHE_SETS/2);
        #endif
        probe_time = probe_all();
        LOG_MSG("%d\n", probe_time);
    #else
        for (int touch_n_lines = 0; touch_n_lines < N_CACHE_SETS; touch_n_lines++)
        {
        // for(int sample = 0; sample < 1000; sample++)
          {
            lines = touch_n_lines;
            //prime all I-cache 
            prime();
            // touche n_lines
            #ifdef TROJAN_S
                // Call the S world trojan
                psa_call(handle, PSA_IPC_CALL, invecs, 1, outvecs, 2);
            #else
                // Call the NS world trojan, to measure the channel without ns interference
                touch_ns(touch_n_lines);
            #endif
            // measure the time to access the N_CAHCE_SETS I-cache line buffer
            probe_time = probe_all();
            LOG_MSG("%d %d\n", touch_n_lines, probe_time);
          }
        }
    #endif

    #ifdef TROJAN_S
        psa_close(handle);
    #endif

    // //printf("outvec1 is: %s\r\n", outvecs[0].base);
    // //printf("outvec2 is: %s\r\n", outvecs[1].base);
    return 1;
}


#include "region_defs.h"
/**
 * \brief main() function
 */
#ifndef __GNUC__
__attribute__((noreturn))
#endif
int main(void)
{
    #ifdef ONLY_PRIME_CACHEABLE
        extern unsigned char __start_covert;
        extern unsigned char __stop_covert;
    #endif
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

    #ifdef ONLY_PRIME_CACHEABLE
        // __stop_ and __start_ are added to our section by the linker to mark the start and end of the section
        config_mpu_only_prime_code_cacheable(NS_CODE_START,NS_CODE_SIZE, &__start_covert,(&__stop_covert) - (&__start_covert) + 12); // + 12 beacause in last cache line we only put a BX (4 bytes)
    #elif defined(NS_NON_CACHEABLE)
        config_mpu_all_non_cacheable(NS_CODE_START,NS_CODE_SIZE);
    #endif
    LOG_MSG("Non-Secure system starting...\r\n");
    covert_channel_test();
    LOG_MSG("End\r\n");
    for (;;) {
    }
}
