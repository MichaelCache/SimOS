#include "asm.h"
.code16
.org 0x7c00
.section .text
.globl _start
_start:
jmp entry
nop
.ascii "testos.1"
.word 0x0200,0x8608,0x0204,0x0000,0x0000,0x00f8,0x3f00,0xff00,0x0000,0x0008,0x0000,0x7730,0xbd00,0x001d,0x0000,0x0000,0x0200,0x0000,0x0100,0x0600,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x8000,0x2901,0x353b,0x484a,0x4f4e,0x4e20,0x4d41,0x2045,0x2020
.byte 0x20	/* all this data are copied from usb bios info */
.ascii "FAT32   "
entry:
movw $0,%ax
movw %ax,%ss
movw %ax,%ds

cli				# Disable interrupts
open20step1:  /*open A20 gate by i8042 */
inb $0x64,%al
testb $0x2,%al
jnz open20step1
movb $0xd1,%al
outb %al,$0x64

open20step2:
inb $0x64,%al
testb $0x2,%al
jnz open20step2
movb $0xdf,%al
outb %al,$0x60


lgdt gdtdesc   # load gdt address to GDTR
movl %cr0,%eax   # switch to 32-bit mode
orl $0x1,%eax
movl %eax,%cr0

ljmp $ox8,$portect   #why 0x8,it`s 0000000000001|000 section ,the first section of gdt

.code32
movw $010, %ax        # Our data segment selector,0000000000010|000
movw %ax, %ds                                   # -> DS: Data Segment
movw %ax, %es                                   # -> ES: Extra Segment
movw %ax, %fs                                   # -> FS
movw %ax, %gs                                   # -> GS
movw %ax, %ss

movl $0x0,%ebp		#set up the stack base at 0x00000000
movl $_start,%esp	#set up the stack top at 0x00007c00
call bootstep2




.p2align 2                       # force 4 byte alignment
gdt:
SEG_NULLASM                           # null seg
SEG_ASM(STA_X|STA_R, 0x0, 0xffffffff) #code seg for bootloader and kernel
SEG_ASM(STA_W, 0x0, 0xffffffff)
gdtdesc:
.word 0x17    # 3 index,8 byte each,so linit is 3*8-1=0x17
.long gdt



.org 0x7dbe
.word 0x0480,0x1205,0x0f0b,0x7f07,0x0800,0x0000,0x3000,0x0077
.org 0x7dfe
.word 0xaa55
