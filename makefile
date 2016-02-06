bootblock:bootloader/bootloader.s
	gcc -c -ggdb -m32 -o bootloader.o bootloader/bootloader.s
	objcopy bootloader.o -O binary bootblock
print:print/print.h print/print.c
	gcc -c -ggdb -I ./print -o print.o print.c
bootinfo:test/bootinfo.s
	gcc -c -ggdb -o bootinfo.o test/bootinfo.s
	objcopy bootinfo.o -O binary bootinfo.tmp
	dd if=bootinfo.tmp of=bootinfo skip=65
os:bootblock bootinfo	
	dd if=bootblock of=os.img skip=62
	dd if=bootinfo of=os.img conv=notrunc seek=1
hello.o:hello/hello.c
	gcc -static -ggdb -m32 -Wl,-Ttext=0x100000 -o hello.o hello/hello.c
printf:~/labos/ucore_os_lab/labcodes_answer/lab1_result/bin/bootblock hello.o
	dd if=bootblock of=printf.img skip=62
	dd if=hello.o of=printf.img conv=notrunc seek=1



#	dd if=/dev/zero of=os.img conv=notrunc oflag=append count=415 obs=1 ibs=1
