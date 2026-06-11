
/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "psa/service.h"
#include "psa_manifest/covert.h"
#include "tfm/tfm_spm_services.h"
#include "tfm_sp_log.h"
#include <assert.h>
#include "cmsis.h"
#include "covert_src/covert.h"


// Change the cache states in this TA just can be done in Level 1
//#define STM32L552_CACHE_INVALIDATION

#ifdef STM32L552_CACHE_INVALIDATION
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

void config_mpu_all_non_cacheable(){
    ARM_MPU_Disable_NS();
    ARM_MPU_SetMemAttr_NS(0UL, ARM_MPU_ATTR(       /* Normal memory */
      ARM_MPU_ATTR_NON_CACHEABLE, /* Outer non-cacheable, normal memory*/
      ARM_MPU_ATTR_NON_CACHEABLE  /* Inner non-cacheable, normal memory */
    ));

    ARM_MPU_SetRegion_NS(0UL,
       ARM_MPU_RBAR(0x08044000UL, ARM_MPU_SH_NON, 1UL, 1UL, 0UL),  /* Non-shareable, read only, non-privileged, execute code */
       ARM_MPU_RLAR(0x080447ffU, 0UL)                             /* 4kB memory block using Attr 0 */
     );
//  // 0 -> memory region 0 | (8h44) 0100 -> Normal memory, Outer non-cacheable | 0100 ->  Normal memory, Inner non-cacheable.
//  ARM_MPU_SetMemAttr(0, 0x44);
//  //RBAR = 0x08060006 (27h080600 + 5b00110) --> 0x08060000 -> base addr | 00 -> Non-shareable | 11 -> Read-only by any privilege level | 0 -> executable
//  //RLAR = 0x08061001 (27h08061FE + 4b0001)  --> 0x08061FFF (depois é acrescentado 1F; 27h08061FE + 5h1F)-> addr limit | 000 -> region number | 1 -> region enable
//  ARM_MPU_SetRegion(0, 0x08060006, 0x08061fE1); // region 0,
    ARM_MPU_Enable_NS(4); // 4 -> enables use of the default memory map for regions non configured
}


/**
 * \brief An example service implementation that prints out a message.
 */
static void tfm_covert_service1(void)
{
   const int BUFFER_LEN = 32;
    psa_msg_t msg;
    psa_status_t r;
    int i;
    uint8_t lines; //needs to be a buffer (or tf-m will fail)
    uint8_t rec_buf[BUFFER_LEN];
    uint8_t send_buf[32] = "Hello World";

    //LOG_INFFMT("Hello from  covert service\n");

    psa_get(TFM_COVERT_SERVICE1_SIGNAL, &msg);
    switch (msg.type) {
    case PSA_IPC_CONNECT:
/*        if (service_in_use & TFM_COVERT_SERVICE1_SIGNAL) {
            r = PSA_ERROR_CONNECTION_REFUSED;
        } else {
            service_in_use |= TFM_COVERT_SERVICE1_SIGNAL;
            r = PSA_SUCCESS;
        }*/

        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_CALL:
            if (msg.in_size[i] != 0) {
                psa_read(msg.handle, 0, &lines, 1);
            } 
        // TROJAN ---------------------------------------
        touch_s(lines);
        // ----------------------------------------------
        #ifdef STM32L552_CACHE_INVALIDATION
            ICache_invalidation();
        #endif
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    case PSA_IPC_DISCONNECT:
/*        assert((service_in_use & TFM_COVERT_SERVICE1_SIGNAL) != 0);
        service_in_use &= ~TFM_COVERT_SERVICE1_SIGNAL;*/
        psa_reply(msg.handle, PSA_SUCCESS);
        break;
    default:
        /* cannot get here [broken SPM] */
        psa_panic();
        break;
    }
}

/**
 * \brief The example FFM-1.1 partition's entry function.
 */
void covert_main(void)
{
    psa_signal_t signals;

    //LOG_INFFMT("Covert Main\n");
    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_COVERT_SERVICE1_SIGNAL) {
             //LOG_INFFMT("TFM_COVERT_SERVICE1_SIGNAL \n");
            tfm_covert_service1();
        }else{
            /* Should not come here */
            psa_panic();
        }
    }
}
