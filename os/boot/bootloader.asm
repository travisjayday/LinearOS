bits 	16 
org	0x7C00

jmp start
nop
;------------------------------------------;
;  Standard BIOS Parameter Block, "BPB".   ;
;------------------------------------------;
     bpbOEM    		db  'MSDOS5.0'
     bpbSectSize	dw  512
     bpbClustSize	db  1
     bpbReservedSe	dw  1
     bpbFats   		db  2
     bpbRootSize	dw  224
     bpbTotalSect	dw  2880
     bpbMedia  		db  240
     bpbFatSize 	dw  9
     bpbTrackSect	dw  18
     bpbHeads 		dw  2
     bpbHiddenSect	dd  0
     bpbLargeSect	dd  0
     ;---------------------------------;
     ;  extended BPB for FAT12/FAT16   ;
     ;---------------------------------;
     bpbDriveNo		db  0
     bpbReserved	db  0
     bpbSignature	db  41            
     bpbID    		dd  1
     bpbVolumeLabel	db  'BOOT FLOPPY'
     bpbFileSystem	db  'FAT12   '


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
	mov	al, 0x41	; num sectors
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

times 0x1b4 - ($-$$) db 0	; start of partition table

; 0x1b4
db "12345678", 0x0, 0x0		; 10 byte unique id

; 0x1be 		; Partition 1 
db 0x80			; boot indicator flag = on

; start sector
db 0			; starting head = 0
db 0b00000001	; cyilinder = 0, sector = 1 (2 cylinder high bits, and sector. 00 000001 = high bits db 0x00)
db 0			; 7-0 bits of cylinder (insgesamt 9 bits) 

; filesystem type
db 1			; filesystem type = fat12

; end sector = 2880th sector? I doubt this matters much...
db 1			; ending head = 1
db 18			; cyilinder = 79, sector = 18 (2 cylinder high bits, and sector. 00 000001 = high bits db 0x00)
db 79			; 7-0 bits of cylinder (insgesamt 9 bits) 

dd 0			; 32 bit value of number of sectors between MBR and partition

; dd 2880
db 0			; 32 bit value 
db 0			; = 2880 (0xb40)
db 0x0B
db 0x40			; total num of sectors in partition

; 0x1ce			; Partition 2
times 16 db 0

; 0x1de			; Partition 3
times 16 db 0

; 0x1ee			; Parititon 4
times 16 db 0

; 0x1fe			; Signature
dw	0xAA55


