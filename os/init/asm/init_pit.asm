; initializes pit timer frequency channel 0 to 1 interrupt per milisecond
global init_pit_channel0_ms
init_pit_channel0_ms: 
	mov	al, 0110100b	; channel 0, accessmode lo/hi byte, mode rate gen, mode bin 16 bit
	out 0x43, al

	; pit ticks at 1193180	
	; divisor = 11931
	mov ax, 1193180 / 100
	; lo byte of divisor
	out 0x40, al		; channel 0 data-port, aka. rate divisor = 1193 (0x4A9)
	mov	al, ah
	; high byte divisor
    out 0x40, al		; thus, every 1193181 / 1193 hertz, an interrupt at irq 0 will happen
						; thus, every 1000.1517 seconds IRQ0 
	
	ret
