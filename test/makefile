bootinfo:bootinfo.s
	gcc -c -m32 -o bootinfo.o bootinfo.s
	objcopy -j .text bootinfo.o -O binary bootinfo.tmp
	dd if=bootinfo.tmp of=bootinfo skip=65
