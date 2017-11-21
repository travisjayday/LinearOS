init_fpu:
	; initialize FPU 	
	mov	si, fpu_str 
	call	write_str
    
	mov eax, cr0
    	and al, 11110011b
    	or al, 00100010b
    	mov cr0, eax
    	mov eax, cr4
    	or eax, 0x200
    	mov cr4, eax
    	fninit

	ret

fpu_str:
	db "Initialzing FPU...", 0xA, 0xD, 0x0

