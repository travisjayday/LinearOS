section .data
global system_time
system_time: 
	dd 0x0

section .text
global isr_pit_channel0
isr_pit_channel0: 
	push eax	
	inc dword[system_time]

	; acknowledge pic
	mov	al, 0x20
	out	0x20, al

	pop eax
	iret
