.code16

.extern _kernel_main

entry: 
jmp	_start
nop
.org .+100

.text
.global _start
_start:
	xorw	%ax, %ax
	mov 	%ds, %ax
	mov	%es, %ax
	int	$0x10

	leaw	text_found, %di 
	call 	write_string
	
	call 	_kernel_main	

	hlt

write_string: 
	movb 	$0x0E, %ah	# select write char
	xorw	%bx, %bx	# xor for formatting
	movb	$0x01, %bl
	
   write_char: 
	movb	(%di), %al
	incw	%di
	int 	$0x10
	cmpb	$0x00, (%di)	
	jne 	write_char

	ret

text_found: 
	.asciz "Bootloader Found\n\rKerneling..."
. = entry + 510
.word 0xAA55
