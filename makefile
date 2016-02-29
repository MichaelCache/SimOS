bootblock:bootloader/bootloader.s
	gcc -c -ggdb -m32 -o bootloader.o bootloader/bootloader.s
	objcopy bootloader.o -O binary bootblock
system:kernel/init.c kernel/palette.h kernel/basicfunc.h
	gcc -c -ggdb -m32 -o system.o kernel/init.c
	ld -Ttext=0x100000 system.o
	objcopy a.out -O binary system
all:system bootblock
	dd if=bootblock of=SimOS.img skip=62
	dd if=system of=SimOS.img conv=notrunc seek=1
	qemu-img convert SimOS.img -O vdi DSB.vdi
qemu_debug:
	qemu-system-i386 -hda SimOS.img -d in_asm
run:all
	qemu-system-i386 -hda SimOS.img
clean:
	rm SimOS.img
	rm system.o
	rm system
	rm bootblock
	rm bootloader.o
	rm a.out
	rm DSB.vdi

