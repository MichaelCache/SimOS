boot:bootloader.s 
	as -o bootloader.o bootloader.s
	objcopy -j .text bootloader.o -O binary bootloader
	dd if=bootloader of=boot skip=62
