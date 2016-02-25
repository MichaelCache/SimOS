bootblock:bootloader/bootloader.s
	gcc -c -ggdb -m32 -o bootloader.o bootloader/bootloader.s
	objcopy bootloader.o -O binary bootblock
kernel:init/init.c init/asmfunc.s init/palette.h init/print.h
	gcc -c -ggdb -m32 -o kernel.o init/init.c
	gcc -c -ggdb -m32 -o asmfunc.o init/asmfunc.s
	ld -Ttext=0x100000 kernel.o asmfunc.o
	objcopy a.out -O binary kernel
all:kernel bootblock
	dd if=bootblock of=SimOS.img skip=62
	dd if=kernel of=SimOS.img conv=notrunc seek=1
	qemu-img convert SimOS.img -O vdi DSB.vdi
qemu_debug:
	qemu-system-i386 -hda SimOS.img -d in_asm
clean:
	rm SimOS.img
	rm kernel.o
	rm kernel
	rm bootblock
	rm bootloader.o
	rm a.out
	rm asmfunc.o

