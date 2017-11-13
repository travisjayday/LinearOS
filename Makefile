C_files = os/kernel.c os/src/*
Build_Dir = /root/Programming/OS/LinearOS/

debug: main
	bochs -q

main: 
	nasm boot/bootloader.asm -f bin -o build/loader.bin
	nasm boot/bootstrapper.asm -f elf -o build/strapper.o
	gcc -c -g -Os -march=i686 -ffreestanding -Wall -Werror -m32 os/kernel.c -o $(Build_Dir)/build/kernel.o
	ld -Tlinker.ld -m elf_i386 build/strapper.o build/kernel.o -o build/kernel.elf
	objcopy -O binary build/kernel.elf build/kernel.bin
	dd if=/dev/zero of=build/floppy.img bs=1024 count=1440
	dd if=build/loader.bin of=build/floppy.img bs=512 seek=0 count=1 conv=notrunc
	dd if=build/kernel.bin of=build/floppy.img bs=512 seek=1 conv=notrunc

# writes OS to USB automatically at /dev/sdb 
usb:	main	
	@echo ""
	@echo "[Warning] This operation will overwrite the bootsector of physical disk /dev/sdb"
	@echo "Press [Enter] to continue, [^C] to abort"
	@read input
	dd if=build/floppy.img of=/dev/sdb bs=512
	
clean:
	rm build/*


