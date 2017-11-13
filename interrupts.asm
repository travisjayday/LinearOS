_idt: .fill 256, 8, 0

idt_descr: 
	.word 256 * 8 - 1	; contains 256 entries
	.long _idt		; base address of idt

; reprograms the PIC such that it remaps old bios interrupts
reprogram_pic: 
	mov	al,0x11		; initialization sequence
	out	0x20,al		; send it to 8259A-1
	.word	0x00eb,0x00eb		; jmp $+2, jmp $+2
	out	0xA0,al		; and to 8259A-2
	.word	0x00eb,0x00eb
	mov	al,0x20		; start of hardware int's (0x20)
	out	0x21,al
	.word	0x00eb,0x00eb
	mov	al,0x28		; start of hardware int's 2 (0x28)
	out	0xA1,al
	.word	0x00eb,0x00eb
	mov	al,0x04		; 8259-1 is master
	out	0x21,al
	.word	0x00eb,0x00eb
	mov	al,0x02		; 8259-2 is slave
	out	0xA1,al
	.word	0x00eb,0x00eb
	mov	al,0x01		; 8086 mode for both
	out	0x21,al
	.word	0x00eb,0x00eb
	out	0xA1,al
	.word	0x00eb,0x00eb
	mov	al,0xFF		; mask off all interrupts for now
	out	0x21,al
	.word	0x00eb,0x00eb
	out	0xA1,al

	ret


