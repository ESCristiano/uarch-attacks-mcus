
/*
 * Copyright (c) 2021, Arm Limited. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 *
 */

#include <stdint.h>
#include "psa/service.h"
#include "psa_manifest/covert.h"
//#include "tfm/tfm_spm_services.h"
#include "tfm_sp_log.h"
#include <assert.h>
#include "cmsis.h"
#include "uArchChannel_lib.h"
#include "psa_manifest/sid.h"

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
        probe_time = probe_all_s();
        LOG_INFFMT("%d\n", probe_time);
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
                  psa_call(TFM_APPROT_SERVICE1_HANDLE, PSA_IPC_CALL, invecs, 1, outvecs, 2);
              #else
                  // Call the NS world trojan, to measure the channel without ns interference
                  touch_ns(input_symbol);
              #endif
              // measure the time to access the N_CAHCE_SETS I-cache line buffer
              probe_time = probe_all_s();
              #ifdef GNUPLOT
                LOG_INFFMT("%d %d\n", input_symbol, probe_time);
              #endif

              #ifdef LEAKIEST
                LOG_INFFMT("(%d, %d)\r\n", input_symbol, probe_time);
              #endif

            }
        }
    #endif

    return 1;
}

/**
 * \brief An example service implementation that prints out a message.
 */
static void tfm_covert_service1(void)
{
   const int BUFFER_LEN = 32;
    int counter1 = 0, counter2 = 0;
    psa_msg_t msg;
    psa_status_t r;
    int i;
    uint8_t lines; //needs to be a buffer (or tf-m will fail)
    uint8_t rec_buf[BUFFER_LEN];
    uint8_t send_buf[32] = "Hello World";

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
        covert_channel_test();
        //status = PSA_SUCCESS;
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
    // LOG_INFFMT("Covert Main\n");
    while (1) {
        signals = psa_wait(PSA_WAIT_ANY, PSA_BLOCK);
        if (signals & TFM_COVERT_SERVICE1_SIGNAL) {
            tfm_covert_service1();   
        }else{
            /* Should not come here */
            psa_panic();
        }
    }
}
