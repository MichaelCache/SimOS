bootblock:bootloader/bootloader.s
	gcc -c -ggdb -m32 -o bootloader.o bootloader/bootloader.s
	objcopy bootloader.o -O binary bootblock
printsc:print/print.c print/asmfunc.s
	gcc -c -ggdb -o print.o print/print.c
	gcc -c -ggdb -o asmfunc.o print/asmfunc.s
	#gcc -c -ggdb -I ./print -o print.o print/print.c
	ld -Ttext=0x8200 print.o asmfunc.o
	objcopy a.out -O binary printsc
all:printsc bootblock
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
