#ifndef INTERRUPTS_H_
#define INTERRUPTS_H_

// pointer to the idt base declared in os/boot/src/interrupts.asm
// basically pointer to idt structure
extern uint8_t* idt_base; 
char* exception_msg[] = 
{
	"Divide By Zero\0",			// 0
	"Debug\0",					// 1
	"Non-Maskable Interrupt\0",	// 2
	"Breakpoint\0",				// 3
	"Overflow\0",				// 4
	"Round Range Exceeded\0", 	// 5
	"Invalid Opcode\0", 		// 6
	"Device not Available\0", 	// 7
	"Double Fault\0",			// 8
	"n/a\0",					// 9
	"Invalid TSS\0",			// A
	"Segment Not Present\0", 	// B
	"Stack Segment Fault\0", 	// C
	"General Protection Fault\0",// D
	"Page Fault\0", 			// E
	"n/a"						// F
	"x87-Floating Point Excp.\0",// 10	
	"Alignment Check\0", 		// 11
	"Machine Check\0", 			// 12
	"Floating Point Exception\0", // 13
};

// idt entry struct
typedef struct {
	uint16_t offset_lo; 
	uint16_t selector; 
	uint8_t zero; 
	uint8_t attributes; 
	uint16_t offset_hi; 
} idt_entry;  

typedef struct 
{
	uint32_t gs, fs, es, ds;
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
	uint32_t int_no, err_code;
	uint32_t eip, cs, cflags, useresp, ss;
} regs;


// handles cpu faults and dumps relavent info on screen 
void fault_handler(regs* r); 

// assembly routines defined in asm/isr_exceptions.asm
// they all call fault_handler()
extern void isr_divide_error();
extern void isr_debug_trap();
extern void isr_non_maskable_interrupt();
extern void isr_breakpoint();
extern void isr_overflow();
extern void isr_round_range_exceeded();
extern void isr_invalid_opcode();
extern void isr_device_not_available();
extern void isr_double_fault();
extern void isr_invalid_tss();
extern void isr_segment_not_present();
extern void isr_stack_segment_fault();
extern void isr_general_protection_fault();
extern void isr_page_fault();
extern void isr_x87_floating_point_exception();
extern void isr_alignment_check();
extern void isr_machine_check();
extern void isr_simd_floating_point_exception();

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

// installs cpu exception handlers
void traps_init(); 

// umasks pic irqs
void enable_hardware_interrupts(); 

#include "src/interrupts.c"
#endif
