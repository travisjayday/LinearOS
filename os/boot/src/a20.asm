attempt_init_a20: 
	mov	si, test_a20_str
	call	write_str

	call 	status_a20	; if a20 is set, al = 1, else al = 0
	or	al, al
	jne	skip_enable_a20

	; enable a20 lien
	call	enable_a20	; if successful, al = 1; else al = 0
	or	al, al		; if al is zero
	jne	skip_enable_a20 ; if not zero, skip

	mov	si, a20_fatal	; else display fatal error 
	call	write_str

	cli
	hlt	

   skip_enable_a20:
	; a20 is enabled
	mov	si, a20_enabled
	call	write_str
	
	ret


; enables a20 line, then chekcs if enabling was successful
; if succesful, al = 1, else al = 0;
enable_a20:
        cli
 
        call    a20wait
        mov     al,0xAD
        out     0x64,al
 
        call    a20wait
        mov     al,0xD0
        out     0x64,al
 
        call    a20wait2
        in      al,0x60
        push    eax
 
        call    a20wait
        mov     al,0xD1
        out     0x64,al
 
        call    a20wait
        pop     eax
        or      al,2
        out     0x60,al
 
        call    a20wait
        mov     al,0xAE
        out     0x64,al
 
        call    a20wait
        sti
 
   	call 	status_a20	; if a20 is set, al = 1, else al = 0
	ret

   a20wait:
        in      al,0x64
        test    al,2
        jnz     a20wait
        ret
 
 
   a20wait2:
        in      al,0x64
        test    al,1
        jz      a20wait2
        ret



; checks if a20 is enabled
; if it is al is 1 else al is 0 
status_a20:   
	pushf
    	push ds
	push es
	push di
	push si
	 
    	cli
 
    	xor ax, ax ; ax = 0
    	mov es, ax
 
    	not ax ; ax = 0xFFFF
    	mov ds, ax
 
   	mov di, 0x0500
  	mov si, 0x0510
 
   	mov al, byte [es:di]
    	push ax
 
    	mov al, byte [ds:si]
    	push ax
 
    	mov byte [es:di], 0x00
    	mov byte [ds:si], 0xFF
 
    	cmp byte [es:di], 0xFF
 
    	pop ax
    	mov byte [ds:si], al
 
    	pop ax
    	mov byte [es:di], al
 
    	mov ax, 0
    	je check_a20__exit
 
    	mov ax, 1
 
   check_a20__exit:
   	pop si
    	pop di
   	pop es
    	pop ds
    	popf
 
    	ret

test_a20_str:
	db "Testing if A20 line is enabled...", 0xA, 0xD, 0x0

a20_enabled: 
	db "Enabled A20 line!", 0xA, 0xD, 0x0

a20_fatal:
	db "Fatal Error: Failed to enable a20 line..."
	
