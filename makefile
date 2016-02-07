bootblock:bootloader/bootloader.s
	gcc -c -ggdb -m32 -o bootloader.o bootloader/bootloader.s
	objcopy bootloader.o -O binary bootblock
printsc:print/print.h print/print.c
	gcc -c -ggdb -I ./print -o print.o print/print.c
	ld -Ttext=0x8200 print.o
	objcopy a.out -O binary printsc
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
	rm bootblock
	rm bootloader.o
	rm a.out
#	dd if=/dev/zero of=os.img conv=notrunc oflag=append count=415 obs=1 ibs=1
