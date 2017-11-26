global isr_divide_error
global isr_debug_trap
global isr_non_maskable_interrupt
global isr_breakpoint
global isr_overflow
global isr_round_range_exceeded
global isr_invalid_opcode
global isr_device_not_available
global isr_double_fault
global isr_invalid_tss
global isr_segment_not_present
global isr_stack_segment_fault
global isr_general_protection_fault
global isr_page_fault
global isr_x87_floating_point_exception
global isr_alignment_check
global isr_machine_check
global isr_simd_floating_point_exception

isr_divide_error:
	cli
	push byte 0 
	push byte 0
	jmp isr_common

isr_debug_trap:
	cli
	push byte 0 
	push byte 1
	jmp isr_common

isr_non_maskable_interrupt:
	cli
	push byte 0
	push byte 2
	jmp isr_common

isr_breakpoint:
	cli
	push byte 0
	push byte 3 
	jmp isr_common

isr_overflow:
	cli
	push byte 0
	push byte 4 
	jmp isr_common

isr_round_range_exceeded:
	cli
	push byte 0
	push byte 5 
	jmp isr_common

isr_invalid_opcode:
	cli
	push byte 0
	push byte 6 
	jmp isr_common

isr_device_not_available:
	cli
	push byte 0
	push byte 7 
	jmp isr_common

isr_double_fault:
	cli	
	push byte 8
	jmp isr_common

isr_invalid_tss:
	cli 
	push byte 10 
	jmp isr_common

isr_segment_not_present: 
	cli 
	push byte 11
	jmp isr_common

isr_stack_segment_fault:
	cli 
	push byte 12
	jmp isr_common

isr_general_protection_fault:
	cli 
	push byte 13
	jmp isr_common

isr_page_fault:
	cli 
	push byte 14
	jmp isr_common

isr_x87_floating_point_exception:
	cli
	push byte 0
	push byte 15
	jmp isr_common

isr_alignment_check:
	cli 
	push byte 17 
	jmp isr_common

isr_machine_check:
	cli
	push byte 0
	push byte 18
	jmp isr_common

isr_simd_floating_point_exception:
	cli
	push byte 0
	push byte 19
	jmp isr_common

extern fault_handler

isr_common: 
	pusha
	push ds
	push es
	push fs
	push gs
	mov	 eax, esp
	push eax
	;mov	esp, 0x200
	mov eax, fault_handler
	call eax
	pop eax
	pop gs
	pop fs
	pop es
	pop ds
	popa
	add esp, 8
	iret 


