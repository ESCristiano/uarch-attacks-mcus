
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

static void tfm_covert_service1(void)
{
 psa_status_t status;
    uint32_t arg;
    psa_msg_t msg;
    size_t num;
    int i;
    uint8_t lines;
    /* Retrieve the message corresponding to the example service signal */
    status = psa_get(TFM_COVERT_SERVICE1_SIGNAL, &msg);
    if (status != PSA_SUCCESS) {
        return;
    }

    /* Decode the message */
    switch (msg.type) {
    case PSA_IPC_CALL:
        if (msg.in_size[i] != 0) {
            psa_read(msg.handle, 0, &lines, 1);
        } 
        touch_s(lines);
        status = PSA_SUCCESS;
        break;
    default:
        /* Invalid message type */
        status = PSA_ERROR_PROGRAMMER_ERROR;
        break;
    }
    /* Reply with the message result status to unblock the client */
    psa_reply(msg.handle, status);
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
