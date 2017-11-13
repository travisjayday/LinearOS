;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; Sector 2
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
bits 16
;org 0x1000
extern kernel_main

section .text
global bootstrapper
bootsrapper: 

	; initialize vital componenets 	
	call	attempt_init_a20
	call 	init_fpu

	cli	
	; setup interrupts
	call repgrogram_pic
	lidt idt_desc

	; wait a little
	mov	cx, 0x003E
	mov	dx, 0x8480
	mov	ah, 0x86

	int	0x15

	; set VGA 640x48 16 color graphics
	mov 	si, vga_str
	call 	write_str

	mov	ah, 0x00
	mov	al, 0x13
	int 	0x10

	; enter protected mode
	cli	
	lgdt	[gdtr]		; load gdt
	
	mov	eax, cr0
	or	eax, 1
	mov	cr0, eax

	jmp 	(CODE_DESC - NULL_DESC) : protect_mode 	

	hlt

write_str: 
	mov 	ah, 0x0E	
   write_ch:	
	mov	al, [si]
	int 	0x10
	inc 	si
	cmp	byte [si], 0x00
	jne	write_ch

	ret

; includes
%include "/root/Programming/OS/LinearOS/boot/src/a20.asm"
%include "/root/Programming/OS/LinearOS/boot/src/fpu.asm"
%include "/root/Programming/OS/LinearOS/boot/src/interrupts.asm"

vga_str:
	db "Switching to VGA...", 0x0

NULL_DESC: 
	dd 0
	dd 0 

CODE_DESC:
	dw 0xFFFF	; limit low
	dw 0 		; base low
	db 0 		; base middle
	db 10011010b	; access
	db 11001111b	; granularity
	db 0 		; base high 

DATA_DESC:
	dw 0xFFFF	; limit low
	dw 0		; base low
	db 0		; base middle
	db 10010010b	; access
	db 11001111b	; granularity
	db 0 		; base high

gdtr: 
	lmiit 	dw gdtr - NULL_DESC - 1 	; size of gdt -1
	base	dd NULL_DESC			; start of gdt

bits 32
protect_mode: 
	mov	ax, DATA_DESC - NULL_DESC
	mov 	ds, ax	; update data segment

	call	kernel_main
	hlt

