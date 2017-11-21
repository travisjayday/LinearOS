global idt_base
idt_base: times 256 * 8 db 0

idt_descr: 
	dw 	256 * 8 - 1	; contains 256 entries
	dd 	idt_base		; base address of idt

; reprograms the PIC such that it remaps old bios interrupts
reprogram_pic: 
	mov	al,0x11		; initialization sequence
	out	0x20,al		; send it to 8259A-1
	;dw	0x00eb,0x00eb		; jmp $+2, jmp $+2
	out	0xA0,al		; and to 8259A-2
	;dw	0x00eb,0x00eb
	mov	al,0x20		; start of hardware int's master pic (0x20)
	out	0x21,al
	;dw	0x00eb,0x00eb
	mov	al,0x28		; start of hardware int's 2 slave pic (0x28)
	out	0xA1,al
	;dw	0x00eb,0x00eb
	mov	al,0x04		; 8259-1 is master
	out	0x21,al
	;dw	0x00eb,0x00eb
	mov	al,0x02		; 8259-2 is slave
	out	0xA1,al
	;dw	0x00eb,0x00eb
	mov	al,0x01		; 8086 mode for both
	out	0x21,al
	;dw	0x00eb,0x00eb
	out	0xA1,al
	;dw	0x00eb,0x00eb
	mov	al,0xFF		; mask off all interrupts for now
	out	0x21,al
	;dw	0x00eb,0x00eb
	out	0xA1,al

	ret


