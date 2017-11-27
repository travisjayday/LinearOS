C_files = os/kernel.c os/src/*
Build_Dir = $(shell pwd)

elf_sources = os/boot/bootstrapper.asm \
			  os/init/isr_keyboard.asm \
			  os/init/isr_pit.asm \
			  os/init/init_pit.o

elf_objects = build/bootstrapper.o \
			  build/isr_exceptions.o\
		  build/isr_keyboard.o \
		  build/isr_pit.o \
		  build/init_pit.o \
		  build/kernel.o
		  
WERROR = -Werror
debug: main
	cd bochs && bochs -q -rc debug.rc

main: make_obj
	# link objs
	#ld -Tlinker.ld -m elf_i386 build/strapper.o  build/keyboard.o build/isr_pit.o build/kernel.o -o build/kernel.elf
	ld -Tlinker.ld -m elf_i386 $(elf_objects) -o build/kernel.elf
	objcopy -O binary build/kernel.elf build/kernel.bin
	# write to disk
	dd if=/dev/zero of=build/floppy.img bs=1024 count=1440
	dd if=build/bootloader.bin of=build/floppy.img bs=512 seek=0 count=1 conv=notrunc
	dd if=build/kernel.bin of=build/floppy.img bs=512 seek=1 conv=notrunc

make_obj: 
	nasm os/boot/bootloader.asm $(WERROR) -f bin -o build/bootloader.bin
	# TODO: find a better way to assemble the following elf objects...
	nasm os/boot/bootstrapper.asm $(WERROR) -f elf -o build/bootstrapper.o
	nasm os/init/asm/isr_exceptions.asm $(WERROR) -f elf -o build/isr_exceptions.o
	nasm os/init/asm/isr_keyboard.asm $(WERROR) -f elf -o build/isr_keyboard.o
	nasm os/init/asm/isr_pit.asm $(WERROR) -f elf -o build/isr_pit.o
	nasm os/init/asm/init_pit.asm $(WERROR) -f elf -o build/init_pit.o
	# kernel main
	#gcc -c -Os -std=gnu99 -march=i686 -ffreestanding -Wall $(WERROR) -m32 os/kernel.c -o $(Build_Dir)/build/kernel.o
	gcc -c -std=gnu99 -march=i686 -ffreestanding -Wall $(WERROR) -m32 os/kernel.c -o $(Build_Dir)/build/kernel.o

# writes OS to USB automatically at /dev/sdb 
usb:	main	
	@echo ""
	@echo "[Warning] This operation will overwrite the bootsector of physical disk /dev/sdb"
	@echo "Press [Enter] to continue, [^C] to abort"
	@read input
	dd if=build/floppy.img of=/dev/sdb bs=512
	
clean:
	rm build/*


