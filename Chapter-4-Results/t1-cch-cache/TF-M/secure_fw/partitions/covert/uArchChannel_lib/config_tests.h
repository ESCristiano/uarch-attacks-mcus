#ifndef CONFIG_TESTS_H
#define CONFIG_TESTS_H

    // Uncomment this to measure the covert channel with uTEE
    #define TROJAN_S 

    // Output for Gnuplot
    // #define GNUPLOT

    // Output for Leakiest
    #define LEAKIEST

//------RENESAS-----------------------------------------------------------------
// acho que não posso usar este pk nos files da board há um que se chama board.h
// #define EKRA6M3

// // I_CACHE 
// #define I_CACHE_SIZE 256
// #define I_CACHE_LINE_SIZE 16
// #define I_N_WAYS 8
// #define I_N_CACHE_SETS 2
// #define I_REPLACEMENT_POLICY LRU

// // D_CACHE 
// #define NO_D_CACHE 
// // D_CACHE
// #define NO_D_CACHE
//------------------------------------------------------------------------------

//#include "test_configs.h"
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
      // #define TROJAN_S 
    //--------------------------------------------------------
    // Macro NS_NON_CACHEABLE put all the NS code non-cacheanle
    // and Macro ONLY_PRIME_CACHEABLE put only code that will 
    // be mapped to cache as cacheable. The rest of the NS code
    // is setted as non-cacheable to reduce noise
      //#define NS_NON_CACHEABLE
      // #define ONLY_PRIME_CACHEABLE 
  //-------------------------------------------------------------- 

  //-----------------TEST CONFIGS TEST 1 --------------------------------
  //-------- Uncomment this to measure the Native Channel
    //#define ONLY_PRIME_CACHEABLE 
  
  //-------- Uncomment this to measure the covert channel with tfm
    // #define TROJAN_S 
    // #define ONLY_PRIME_CACHEABLE 
 
  //-------- Uncomment this to measure the covert channel with cache invalidation
  //...... Not appliable to Nuvuton m2351
  //...... Nordic -> Only appliable if tfm level 1
    // #define TROJAN_S 
    // #define ONLY_PRIME_CACHEABLE 
    // #ifdef NORDIC_NRF9160
    //     # define NORDIC_NRF9160_CACHE_INVALIDATION
    // #endif
    // #ifdef STM32L552
    //     #define STM32L552_CACHE_INVALIDATION
    // #endif
    // #ifdef MUSCA_B1
    //     #define MUSCA_B1_CACHE_INVALIDATION
    // #endif
    
  //-------- Uncomment this to measure the covert channel with NS non cacheable
  //...... Not appliable to Nuvuton m2351
    // #define TROJAN_S 
    // #define NS_NON_CACHEABLE
  
  //-------- Uncomment this to measure the covert channel with cache disable
  //...... Not appliable to Nuvuton m2351
  //...... Nordic -> Only appliable if tfm level 1
    //#define TROJAN_S 
    //#ifdef NORDIC_NRF9160
        //# define NORDIC_NRF9160_CACHE_DISABLE 
    //#endif

  //-----------------TEST CONFIGS TEST 2 --------------------------------
    //#define TEST_2_SEC_1
  //-------- Uncomment this to measure the impact to do or not a printf 
  //--------in terms of cache pattern (cache usage)
    // #define ONLY_PRIME_CACHEABLE 
    //#define PRINTF_CACHE_PATTERN
  //-------- Uncomment this to get the pattern to execute two hash operations 
  //--------with different messages and results, one valid other not
    //  #define HASH_CACHE_PATTERN
     // #define ONLY_PRIME_CACHEABLE 
  //-------- Uncomment this to get the pattern to execute two hash operations 
  //--------with different messages 
    // #define ONLY_PRIME_CACHEABLE 
    //#define AES_CACHE_PATTERN
  
  //-----------TEST 3
//   #define TEST_3_SEC_1

#endif