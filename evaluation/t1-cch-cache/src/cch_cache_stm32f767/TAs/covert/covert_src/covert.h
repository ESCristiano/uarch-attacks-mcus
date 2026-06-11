#ifndef COVERT_H
#define COVERT_H
  
#include "board.h"

  //--------- MACROS for NS world Configs -----------------------
    // Macro REVERSE_ENG_CACHE is used to activate the test
    // to determine the cache configuration. Use the flag 
    // REVERSE_ENG_CACHE with  TEST_DIRECT_MAPPED or TEST_2_WAYS 
     
      //#define REVERSE_ENG_CACHE
      //  // #define TEST_DIRECT_MAPPED
      //  #define TEST_2_WAYS
    //------------------------------------------------------
    // Macro TROJAN_S used to activate the meausre of the 
    // covert-channel between S and NS world. If commented
    // it will measure the natice channel of the board without
    // the S world interference 
      //  #define TROJAN_S 
    //--------------------------------------------------------
    // Macro NS_NON_CACHEABLE put all the NS code non-cacheanle
    // and Macro ONLY_PRIME_CACHEABLE put only code that will 
    // be mapped to cache as cacheable. The rest of the NS code
    // is setted as non-cacheable to reduce noise
      //#define NS_NON_CACHEABLE
      //#define ONLY_PRIME_CACHEABLE 
  //-------------------------------------------------------------- 

  //-----------------TEST CONFIGS --------------------------------
  //-------- Uncomment this to measure the Native Channel
    #define ONLY_PRIME_CACHEABLE 
  
  //-------- Uncomment this to measure the covert channel with tfm
    //#define TROJAN_S 
    //#define ONLY_PRIME_CACHEABLE 
 
  //-------- Uncomment this to measure the covert channel with cache invalidation
  //...... Not appliable to Nuvuton m2351
  //...... Nordic -> Only appliable if tfm level 1
    //#define TROJAN_S 
    //#define ONLY_PRIME_CACHEABLE 
    //#ifdef NORDIC_NRF9160
        //# define NORDIC_NRF9160_CACHE_INVALIDATION
    //#endif
 
  //-------- Uncomment this to measure the covert channel with NS non cacheable
  //...... Not appliable to Nuvuton m2351
    //#define TROJAN_S 
    //#define NS_NON_CACHEABLE
  
  //-------- Uncomment this to measure the covert channel with cache disable
  //...... Not appliable to Nuvuton m2351
  //...... Nordic -> Only appliable if tfm level 1
    //#define TROJAN_S 
    //#ifdef NORDIC_NRF9160
        //# define NORDIC_NRF9160_CACHE_DISABLE 
    //#endif

  #ifndef REVERSE_ENG_CACHE
    #define N_CACHE_SETS    CACHE_SIZE/(CACHE_LINE_SIZE*N_WAYS)
  #else
    #define N_CACHE_SETS    CACHE_SIZE/(CACHE_LINE_SIZE*(N_WAYS/N_WAYS)) // assume that the cahce is direct-mapped for test porpuses 
  #endif

  extern void block_jumps_0_0();
  extern void second_block_jumps_0_0();
  extern void block_bx_lr_way_0();
  extern void block_bx_lr_last_way();
  // touch_s uses the same block of jumps of the prime 1 
  __attribute__ ((noinline)) void touch_s(int secret){
    void (*func_ptr)(void)=(&block_jumps_0_0+1+((N_CACHE_SETS-secret-1)*CACHE_LINE_SIZE));
    func_ptr();
  }

  // touch_ns uses the same block of jumps of the prime 2
  __attribute__ ((noinline)) void touch_ns(int secret){
    void (*func_ptr)(void)=(&second_block_jumps_0_0+1+((N_CACHE_SETS-secret-1)*CACHE_LINE_SIZE));
    func_ptr();
  }

  // ensure that the code is compiled withou otimizations otherwise we can have false results
  __attribute__((optimize(0))) __attribute__ ((noinline)) volatile int probe_all(){
    volatile int time1, time2, result;
    SysTick->CTRL = 0x05; // starts the systick
    time1 = SysTick->VAL;
    #ifndef REVERSE_ENG_CACHE
      block_jumps_0_0();
    #else
      touch_s(N_CACHE_SETS/2);
    #endif
    time2 = SysTick->VAL;
    result = time1 - time2; // inverted subtraction because it is a down counter
    SysTick->CTRL = 0x0; // stops the systick
    SysTick->VAL = 0; // reset the val  counter
    return result;
  }

  // ensure that the code is compiled withou otimizations otherwise we can have false results
  __attribute__((optimize(0))) __attribute__ ((noinline)) volatile int probe_line(int line, int way){
    volatile int time1, time2, result;
    void (*func_ptr)(void);
    if(!way)
      func_ptr = (&block_bx_lr_way_0+1+((line)*CACHE_LINE_SIZE));
    else
      func_ptr = (&block_bx_lr_last_way+1+((line)*CACHE_LINE_SIZE));
    SysTick->CTRL = 0x05; // starts the systick
    time1 = SysTick->VAL;
    func_ptr();
    time2 = SysTick->VAL;
    result = time1 - time2; // inverted subtraction because it is a down counter
    SysTick->CTRL = 0x0; // stops the systick
    SysTick->VAL = 0; // reset the val  counter
    return result;
  }

  inline void prime(){
    block_jumps_0_0();
  }

  inline void prime_2(){
    second_block_jumps_0_0();
  }

  //   volatile int time1, time2, result;
  // __attribute__((optimize(0))) __attribute__ ((noinline)) volatile int fetch_1_way(){
  //   __asm("push {lr}");
  //   // second push from memory 
  //  //SysTick->CTRL = 0x05; // starts the systick
  //   __asm("ldr r0, =0xE000E010"); 
  //   __asm("mov r1, #5"); 
  //   __asm("str r1, [r0]");
    
  //   __asm("ldr r0, =0xE000E018"); 
  //   //read systick
  //   __asm("ldr r1, [r0]"); 
  //     //jump
  //     __asm("bl touch_2_ways"); // last instruction "Page 1" (bx)
  //   //read systick
  //   __asm("ldr r2, [r0]"); 
  //   //time1 = SysTick->VAL
  //   __asm("ldr r3, =time1");
  //   __asm("str r1, [r3]"); 
  //   //time2 = SysTick->VAL;
  //   __asm("ldr r3, =time2");
  //   __asm("str r2, [r3]"); 
  //   SysTick->CTRL = 0x0; // stops the systick
  //   SysTick->VAL = 0; // reset the val  counter
  //   result = (time1 - time2); // inverted subtraction because it is a down counter
  //   __asm("pop {lr}"); // This instruction in m23 gives a compiler error
  //   return result;
  // }


  // __attribute__((optimize(0))) __attribute__ ((noinline)) volatile int fetch_2_ways(){
  //   volatile int time1, time2, result;
  //   __asm("push {lr}");
  //   // second push from memory 
  //   SysTick->CTRL = 0x05; // starts the systick
  //   time1 = SysTick->VAL;
  //   __asm("bl touch_2_ways"); // last instruction "Page 1" (bx)
  //   time2 = SysTick->VAL;
  //   SysTick->CTRL = 0x0; // stops the systick
  //   SysTick->VAL = 0; // reset the val  counter
  //   result = (time1 - time2); // inverted subtraction because it is a down counter
  //   __asm("pop {lr}"); // This instruction in m23 gives a compiler error
  //   return result;
  // }

  // __attribute__((optimize(0))) __attribute__ ((noinline)) volatile int probe_2_without_delay(){
  //   volatile int time1, time2, time3, time4, result;
  //   __asm("push {lr}");
  //   // second push from memory 
  //   SysTick->CTRL = 0x05; // starts the systick
  //   time1 = SysTick->VAL;
  //   __asm("bl ret_way_1"); // last instruction "Page 1" (bx)
  //   time2 = SysTick->VAL;
  //   SysTick->CTRL = 0x0; // stops the systick
  //   SysTick->VAL = 0; // reset the val  counter
  //   // necessário ter este time3, para o tempo ficar o mesmo da  probe_2_with_delay
  //   //porque tenho de ter um delay nessa 
  //   SysTick->CTRL = 0x05; // starts the systick
  //   time3 = SysTick->VAL;
  //   __asm("bl ret_way_2"); // last instruction "Page 2" (bx)
  //   time4 = SysTick->VAL;
  //   SysTick->CTRL = 0x0; // stops the systick
  //   SysTick->VAL = 0; // reset the val  counter
  //   result = (time1 - time2) + (time3 - time4); // inverted subtraction because it is a down counter
  //   __asm("pop {lr}"); // This instruction in m23 gives a compiler error
  //   return result;
  // }

  // __attribute__((optimize(0))) __attribute__ ((noinline)) volatile int probe_2_with_delay(){
  //   volatile int time1, time2, time3, time4, result;
  //   __asm("push {lr}");
  //   // second push from memory 
  //   SysTick->CTRL = 0x05; // starts the systick
  //   time1 = SysTick->VAL;
  //   __asm("bl ret_way_1"); // last instruction "Page 1" (bx)
  //   time2 = SysTick->VAL;
  //   SysTick->CTRL = 0x0; // stops the systick
  //   SysTick->VAL = 0; // reset the val  counter
  //   // second push from memory after delay
  //   //for (int i = 0; i < 24; ++i);
  //   /*__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
  //   __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");
  //   __asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");__asm("nop");*/
  //   SysTick->CTRL = 0x05; // starts the systick
  //   time3 = SysTick->VAL;
  //   __asm("bl ret_way_2"); // last instruction "Page 2" (bx)
  //   time4 = SysTick->VAL;
  //   SysTick->CTRL = 0x0; // stops the systick
  //   SysTick->VAL = 0; // reset the val  counter
  //   result = (time1 - time2) + (time3 - time4); // inverted subtraction because it is a down counter
  //   __asm("pop {lr}"); // This instruction in m23 gives a compiler error
  //   return result;
  // }


#endif