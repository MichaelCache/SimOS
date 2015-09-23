bootfun:bootinfo.s 
	as -o bootinfo.o bootinfo.s
	objcopy -j .text bootinfo.o -O binary bootinfo
	dd if=bootinfo of=bootfun skip=65
