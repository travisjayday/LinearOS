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
	push byte 0 
;	cli
%if 0
	pushad
	cld
	xor eax, eax
	; get and convert scancode to ascii
	in al, 0x60		; if msb is set, it means the key was released

	mov cl, al
	and cl, 0x80	; if cl == 0x80 then the msb was set, so skip 
	jnz iret_now
;	xchg bx, bx
	; move ascii code into stdin and increment stdin index 
	mov al, byte [kblayout + eax] 

	;cmp al, 0x00 
	;jz iret_now

	xor ebx, ebx
	mov bl, byte [stdin_index]
	mov	[stdin + ebx], al
	
	inc byte [stdin_index]
	;add byte [stdin_index], 0x01

	; satisfy PIC for next interrutps
  iret_now: 

	; acknowledge pic
	mov	al, 0x20
	out	0x20, al
%endif
	; return 
;	popad
	;mov [esp], dword 0xdeadbeef
	;mov eax, [esp]
	;jmp $
	;push byte 2
	iret	

