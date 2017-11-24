// registers interrupt in idt. See header file for descirption
void register_isr(int n, uint8_t dpl, uint8_t type, uint32_t* isr_addr)
{
        idt_entry entry;
        entry.offset_lo = (uint32_t)((uint32_t*) isr_addr);               // low word of interrupt service routine address
        entry.selector = 8;                             // gdt + 8 is the code segment selector
        entry.zero = 0;
        entry.attributes =  1 << 7 | dpl << 5 | type;
        entry.offset_hi = ((uint32_t)(uint32_t*) isr_addr) >> 16;       // high word of interrupt service routine address
		memcpy8_t((uint8_t*) &idt_base + 8 * n, (uint8_t*) &entry, sizeof(entry)); 
	/*register uint8_t** a asm ("eax");   
	a = &idt_base; 
	asm("hlt"); 
	a += 2;*/
}

// wrapper to register interrupt gate
void register_isr_gate(int n, uint32_t* isr_addr) 
{
	// 0xE is the magic type for gate interrupts
	register_isr(n, 0, 0xE, isr_addr); 
}

// wrapper to register interrupt trap
void register_isr_trap(int n, uint32_t* isr_addr) 
{
	// 0xF is the magic type for trap interrupts
	register_isr(n, 0, 0xF, isr_addr); 
}

/***************************************************/
/* Start of Trap Handlers                          */ 
/***************************************************/
void cpu_exception(uint8_t* name)
{
	uint32_t d = (uint32_t)__builtin_return_address(0); 
	uint8_t len = strlen(name); 
	uint8_t buf[40]; 
	memcpy8_t(buf, (uint8_t*)"CPU Exception: ", 15); 
	memcpy8_t(buf + 15, name, len); 
	memcpy8_t(buf + 15 + len, (uint8_t*)"\n\nAt: 0x", 8); 
	memcpy8_t(buf + 15 + len + 8, int2hex(d), 9); 
	panic(buf); 
}

void isr_divide_error()
{
	cpu_exception((uint8_t*)"Divide by Zero"); 
}

void isr_debug_trap()
{
	cpu_exception((uint8_t*)"Debug Fault/Trap"); 
}

void isr_non_maskable_interrupt()
{
	cpu_exception((uint8_t*)"Non-maskable Interrupt"); 
}

void isr_breakpoint()
{
	cpu_exception((uint8_t*)"Breakpoint"); 
}

void isr_overflow()
{	
	cpu_exception((uint8_t*)"Overflow"); 
}

void isr_round_range_exceeded()
{	
	cpu_exception((uint8_t*)"Round Range Exceeded"); 
}

void isr_invalid_opcode()
{	
	cpu_exception((uint8_t*)"Invalid Opcode"); 
}

void isr_device_not_available()
{	
	cpu_exception((uint8_t*)"Device Not Available"); 
}

void isr_double_fault()
{	
	cpu_exception((uint8_t*)"Double Fault"); 
}

void isr_invalid_tss()
{	
	cpu_exception((uint8_t*)"Invalid TSS"); 
}

void isr_segment_not_present()
{	
	cpu_exception((uint8_t*)"Segment Not Present"); 
}

void isr_stack_segment_fault()
{	
	cpu_exception((uint8_t*)"Stack Segment Fault"); 
}

void isr_general_protection_fault()
{	
	cpu_exception((uint8_t*)"General Protection Fault"); 
}

void isr_page_fault()
{	
	cpu_exception((uint8_t*)"Page Fault"); 
}

void isr_x87_floating_point_exception()
{	
	cpu_exception((uint8_t*)"x87 Floating Point Exception"); 
}

void isr_alignment_check()
{	
	cpu_exception((uint8_t*)"Alignment Check"); 
}

void isr_machine_check()
{	
	cpu_exception((uint8_t*)"Machine Check"); 
}

void isr_simd_floating_point_exception()
{	
	cpu_exception((uint8_t*)"SIMD Floating-Point Exception"); 
}

void isr_keyboard()
{	
	cpu_exception((uint8_t*)"KEYBOARD PRESSED!!!"); 
}

void isr_traps_init()
{
	register_isr_trap(0x0, (void*) isr_divide_error); 
	register_isr_trap(0x1, (void*) isr_debug_trap); 
	register_isr_trap(0x2, (void*) isr_non_maskable_interrupt); 
	register_isr_trap(0x3, (void*) isr_breakpoint); 
	register_isr_trap(0x4, (void*) isr_overflow); 
	register_isr_trap(0x5, (void*) isr_round_range_exceeded); 
	register_isr_trap(0x6, (void*) isr_invalid_opcode); 
	register_isr_trap(0x7, (void*) isr_device_not_available); 
	register_isr_trap(0x8, (void*) isr_double_fault); 
	// no coprocessor segment overrun (int 0x9)  
	register_isr_trap(0xA, (void*) isr_invalid_tss); 
	register_isr_trap(0xB, (void*) isr_segment_not_present); 
	register_isr_trap(0xC, (void*) isr_stack_segment_fault); 
	register_isr_trap(0xD, (void*) isr_general_protection_fault); 
	register_isr_trap(0xE, (void*) isr_page_fault); 
	// int 0xF is reserved
	register_isr_trap(0x10, (void*) isr_x87_floating_point_exception); 
	register_isr_trap(0x11, (void*) isr_alignment_check); 
	register_isr_trap(0x12, (void*) isr_machine_check); 
	register_isr_trap(0x13, (void*) isr_simd_floating_point_exception); 
}

void enable_hardware_interrupts()
{
	asm volatile(
	"movb	$11111100, %al # // 0 = enabled, 1 = disabled, irq 0 = lsb, irq 8 = msb\n"\
	"out	%al, $0x21 \n"\
	"out	%al, $0xA1 \n"); 

}
