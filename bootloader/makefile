bootblock:bootloader.S bootstep2.c asm.h x86.h def.h elf.h 
	gcc -I ./ -c -Os -m32 -o bootstep2.o bootstep2.c
	gdd -I ./ -c -Os -m32 -o bootloader.o bootloader.S
	ld -e _start -Ttext 0x7c00 -o boot bootloader.o bootstep2.o
	objcopy boot -O binary bootblock
	dd if=bootblock of=bootMRB skip=62
