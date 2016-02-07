bootblock:bootloader/bootloader.s
	gcc -c -ggdb -m32 -o bootloader.o bootloader/bootloader.s
	objcopy bootloader.o -O binary bootblock
printsc:print/print.h print/print.c
	gcc -c -ggdb -I ./print -Wl,-Ttext=0x8200 -o print.o print/print.c
	objcopy print.o -O binary print.tmp
	dd if=print.tmp of=printsc #skip=65
bootinfo:test/bootinfo.s
	gcc -c -ggdb -o bootinfo.o test/bootinfo.s
	objcopy bootinfo.o -O binary bootinfo.tmp
	dd if=bootinfo.tmp of=bootinfo skip=65
os:bootblock bootinfo	
	dd if=bootblock of=os.img skip=62
	dd if=bootinfo of=os.img conv=notrunc seek=1
printf:~/labos/ucore_os_lab/labcodes_answer/lab1_result/bin/bootblock hello.o
	dd if=bootblock of=printf.img skip=62
	dd if=hello.o of=printf.img conv=notrunc seek=1
print.img:printsc bootblock
	dd if=bootblock of=print.img skip=62
	dd if=printsc of=print.img conv=notrunc seek=1
clean:
	rm print.img
	rm print.o
	rm printsc
	rm print.tmp
	rm bootblock
#	dd if=/dev/zero of=os.img conv=notrunc oflag=append count=415 obs=1 ibs=1
