bootblock:bootloader/bootloader.s
	#compile bootloader on 32bit mode with GDB information
	gcc -c -ggdb -m32 -o bootloader.o bootloader/bootloader.s
	objcopy bootloader.o -O binary bootblock
system:kernel/init.c kernel/palette.h kernel/basicfunc.h kernel/font.h 
	gcc -c -ggdb -m32 -o system.o kernel/init.c
	#set the system start address at 0x100000
	#use objdump -D to inspect out file
	#use nm to list symbols from object file
	ld -e main -m elf_i386 -Ttext=0x100000 system.o
	objcopy a.out -O binary system
all:system bootblock
	#skip 62*512 bit off from bootblock, whose code start from 0x7c00
	dd if=bootblock of=SimOS.img skip=62
	#skip first sector bootloader of SimOS.img
	dd if=system of=SimOS.img conv=notrunc seek=1
convert_vdi:SimOS.img
	qemu-img convert SimOS.img -O vdi DSB.vdi
gdb_qemu:
	#use gdb to debug image
	#gdb>target remote [localhost]:1234
	#gdb>file a.out
	qemu-system-i386 -s -S -hda SimOS.img -d in_asm
run:all
	qemu-system-i386 -hda SimOS.img
clean:
	rm SimOS.img
	rm system.o
	rm system
	rm bootblock
	rm bootloader.o
	rm a.out
	#rm DSB.vdi

