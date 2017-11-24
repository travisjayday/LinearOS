#ifndef PITDRIVER_H_ 
#define PITDRIVER_H_ 

#include "interrupts.h"

extern uint32_t* system_time; 
extern void init_pit_channel0_ms();
extern void isr_pit_channel0(); 
void init_pit(); 

#include "src/pit_driver.c"

#endif
