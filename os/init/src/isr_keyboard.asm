stdin_index: 
	db 0

global stdin 
stdin: 
	db 0x41
	db 0x41
	db 0x41
	times 253 db 0

extern kblayout		; table with scan code to ascii conversion

global isr_keyboard_handler
isr_keyboard_handler: 
	pusha

	xor eax, eax
	; get and convert scancode to ascii
	in al, 0x60
	xchg bx, bx
	; move ascii code into stdin and increment stdin index 
	mov al, byte [kblayout + eax] 

	cmp al, 0x00 
	jz iret_now


	xor ebx, ebx
	mov bl, byte [stdin_index]
	nop
	nop
	nop
	mov	[stdin + ebx], al
	
	inc byte [stdin_index]
	;add byte [stdin_index], 0x01

	; satisfy PIC for next interrutps
  iret_now: 
	mov	al, 0x20
	out	0x20, al

	; return 
	popa
	iret	

