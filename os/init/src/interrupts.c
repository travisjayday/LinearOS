// registers interrupt in idt. See header file for descirption
void register_isr(int n, uint8_t dpl, uint8_t type, uint32_t* isr_addr)
{
        idt_entry entry;
        entry.offset_lo = (uint32_t)(isr_addr);               // low word of interrupt service routine address
        entry.selector = 8;                             // gdt + 8 is the code segment selector
        entry.zero = 0;
        entry.attributes =  1 << 7 | dpl << 5 | type;
        entry.offset_hi = (uint32_t)(isr_addr) >> 16;       // high word of interrupt service routine address
		memcpy8_t((uint8_t*) &idt_base + 8 * n,  &entry, sizeof(entry)); 
}

// wrapper to register interrupt gate
void register_isr_gate(int n, uint32_t* isr_addr) 
{
	// 0xE is the magic type for gate interrupts
	register_isr(n, 0, 0xE, (uint32_t*) isr_addr); 
}

// wrapper to register interrupt trap
void register_isr_trap(int n, uint32_t* isr_addr) 
{
	// 0xF is the magic type for trap interrupts
	register_isr(n, 0, 0xF, (uint32_t*) isr_addr); 
}

void fault_handler(regs* r)
{
	// blue screen :-)
	fill_buffer(VGA_COLOR_BLUE);

	uint8_t i = 4; 
	draw_string(10, 10, "Kernel Panic!\n\n----------------\n\n", VGA_COLOR_WHITE);
	draw_string(10, ++i*7, strcat("CPU Exception: \0", exception_msg[r->int_no]), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("ERROR: \0", int2hex(r->err_code)), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("EIP: \0", int2hex(r->eip)), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("ESP: \0", int2hex(r->esp)), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("USR-ESP: \0", int2hex(r->useresp)), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("EBP: \0", int2hex(r->ebp)), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("SS: \0", int2hex(r->ss)), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("CS: \0", int2hex(r->cs)), VGA_COLOR_WHITE); 
	i++; 
	draw_string(10, ++i*7, "Stack Trace", VGA_COLOR_WHITE); 
	// not very pretty
	draw_string(10, ++i*7, strcat("ESP + 08: \0", int2hex(*((uint32_t*)(r->esp + 8)))), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("ESP - 00: \0", int2hex(*((uint32_t*)(r->esp - 0)))), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("ESP - 08: \0", int2hex(*((uint32_t*)(r->esp - 8)))), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("ESP - 16: \0", int2hex(*((uint32_t*)(r->esp - 16)))), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("ESP - 24: \0", int2hex(*((uint32_t*)(r->esp - 24)))), VGA_COLOR_WHITE); 
	draw_string(10, ++i*7, strcat("ESP - 32: \0", int2hex(*((uint32_t*)(r->esp - 32)))), VGA_COLOR_WHITE); 
	flip_buffers(); 
	for (;;); 
}

// install interrupt service routines
void isr_traps_init()
{	register_isr_trap(0x0, (uint32_t*) isr_divide_error); 
	register_isr_trap(0x1, (uint32_t*) isr_debug_trap); 
	register_isr_trap(0x2, (uint32_t*) isr_non_maskable_interrupt); 
	register_isr_trap(0x3, (uint32_t*) isr_breakpoint); 
	register_isr_trap(0x4, (uint32_t*) isr_overflow); 
	register_isr_trap(0x5, (uint32_t*) isr_round_range_exceeded); 
	register_isr_trap(0x6, (uint32_t*) isr_invalid_opcode); 
	register_isr_trap(0x7, (uint32_t*) isr_device_not_available); 
	register_isr_trap(0x8, (uint32_t*) isr_double_fault); 
	// no coprocessor segment overrun (int 0x9)  
	register_isr_trap(0xA, (uint32_t*) isr_invalid_tss); 
	register_isr_trap(0xB, (uint32_t*) isr_segment_not_present); 
	register_isr_trap(0xC, (uint32_t*) isr_stack_segment_fault); 
	register_isr_trap(0xD, (uint32_t*) isr_general_protection_fault); 
	register_isr_trap(0xE, (uint32_t*) isr_page_fault); 
	// int 0xF is reserved
	register_isr_trap(0x10, (uint32_t*) isr_x87_floating_point_exception); 
	register_isr_trap(0x11, (uint32_t*) isr_alignment_check); 
	register_isr_trap(0x12, (uint32_t*) isr_machine_check); 
	register_isr_trap(0x13, (uint32_t*) isr_simd_floating_point_exception);
}

void enable_hardware_interrupts()
{
	// enable only keyboard interrupt
	outb(0x21, 0b11111101); 
/*	asm volatile(
	"movb	$0b11111101, %al # // 0 = enabled, 1 = disabled, irq 0 = lsb, irq 8 = msb\n"\
	"out	%al, $0x21 \n"\
	"out	%al, $0xA1 \n"); */

}
