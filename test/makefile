hello:hello.c
	gcc -static -m32 -Wl,-Ttest=0x100000 -o hello-static hello.c

bootinfo:bootinfo.s
	gcc -c -m32 -o bootinfo.o bootinfo.s
	objcopy -j .text bootinfo.o -O binary bootinfo.tmp
	dd if=bootinfo.tmp of=bootinfo skip=65

