bootblock:bootloader.S bootstep2.c asm.h x86.h defs.h elf.h 
	gcc -I ./ -m32 -o bootloader bootloader.S
	objcopy bootloader -O binary bootblock
	dd if=bootblock of=bootMRB skip=62
