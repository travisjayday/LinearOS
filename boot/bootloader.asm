bits 	16 
org	0x7C00

jmp start
nop
resb 8+45

start: 
	; setup segments
	xor	ax, ax
	mov	ds, ax
	mov	es, ax

	; setup stack
	cli
	mov	ss, ax
	mov	sp, 0x7C00
	sti

	; write start string
	mov 	si,start_str 
	call	write_str

	; read bootstrapper into memory
	mov	dl, 0x00	; drive 0 (todo: find boot drive)
	mov	dh, 0x00	; head (base = 0)
	mov	ch, 0x00	; track /cylinder = 0
	mov	cl, 0x02	; (1= bootloader, 2=start of kernel
	mov	bx, 0x7E00	; location to load bootstrapper	
	mov	si, 0x04

	; attempt read 4 times 
  read_floppy:
	; reset floppy disk
	xor	ax, ax
	int	0x13

	; check if attempts to read remain
	test	si, si
	je	fail_read
	dec	si

	; attempt read 
	mov	ah, 0x02	; select read
	mov	al, 0x1F	; num sectors
	int 	0x13
	jc	read_floppy

	mov	si, strapper_loaded_str 
	call	write_str

	; jump to bootsrapper
	jmp	0x0000:0x7E00; far jump to bootstrapper
	
fail_read:
	mov	si, fatal_read_str
	call	write_str
	
	hlt
	jmp	fail_read

write_str: 
	mov 	ah, 0x0E	
	xor	bx, bx
   write_ch:	
	mov	al, [si]
	int 	0x10
	inc 	si
	cmp	byte [si], 0x00
	jne	write_ch

	ret
	
fatal_read_str:
	db "Fatal Error: Read Failure", 0x00
start_str:
	db "Bootloader Found...", 0xA, 0xD, 0x00
strapper_loaded_str:
	db "Starting Stage 2 Loader...", 0xA, 0xD, 0x00

times 510 - ($-$$) db 0
dw	0xAA55


