#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

extern uint8_t* idt_base; 
//uint8_t* idt = (uint8_t*) 0x7fe6; 

typedef struct {
	uint16_t offset_lo; 
	uint16_t selector; 
	uint8_t zero; 
	uint8_t attributes; 
	uint16_t offset_hi; 
} idt_entry;  

/* Registers an interrupt service routine in the interrupt descriptor table 
 * dpl: 0 = exception and hardware interrupt handler; 3 = software interrupt handler
 * type:
	0	Reserved
	1	available 16-bit TSS	System segment
	2	LDT			System segment
	3	active 16-bit TSS	System segment
	4	16-bit Call Gate	Gate
	5	Task Gate		Gate
	6	16-bit Interrupt Gate	Gate
	7	16-bit Trap Gate	Gate
	8	Reserved
	9	available 32-bit TSS	System segment
	10	Reserved
	11	active 32-bit TSS	System segment
	12	32-bit Call Gate	Gate
	13	Reserved
	14	32-bit Interrupt Gate	Gate
	15	32-bit Trap Gate	Gate
 * isr_addr: address of the handler function
 */
void register_isr(int n, uint8_t dpl, uint8_t type, uint32_t* isr_addr);

// wrapper to register interrupt gate 
void register_isr_gate(int n, uint32_t* isr_addr); 

// wrapper to register interrupt trap
void register_isr_trap(int n, uint32_t* isr_addr); 

void traps_init(); 

void enable_hardware_interrupts(); 

#include "src/interrupts.c"
#endif
