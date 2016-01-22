bootblock:bootloader.S
	gcc -c -I ./ -m32 -o bootloader bootloader.S
	objcopy bootloader -O binary bootblock
	dd if=bootblock of=os.img skip=62
	dd if=../test/bootinfo of=os.img conv=notrunc seek=1
#	dd if=/dev/zero of=os.img conv=notrunc oflag=append count=415 obs=1 ibs=1
