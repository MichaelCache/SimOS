#include <asm.h>
.set PROT_MODE_CSEG,        0x8 

.code16
.org 0x7c00
.section .text
.globl _start
_start:
jmp entry
nop
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
movl %cr0,%eax   # switch to 32-bit protected mode
orl $0x1,%eax
movl %eax,%cr0
.code32
ljmp $PROT_MODE_CSEG,$protect   #set the CS register,then the instruction might be CS:IP,which means the net pseudocode.
                     #why 0x8,it`s 0000000000001|000 section ,the second section of gdt
                    #jmp set the IP,ljmp set the CS,CS:IP is the instruction,s address
#.code32
protect:
movw $0x10, %ax        # Our data segment selector,0000000000010|000
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
.word 0xffff,0x0;
.byte 0x0,0x9a,0xdf,0xff        #SEG_ASM(STA_X|STA_R,0x0,0xffffffff)   code seg for bootloader and kernel from 0 to 4 GB
.word 0xffff,0x0;
.byte 0x0,0x92,0xdf,0xff#SEG_ASM(STA_W,0x0,0xffffffff)        data seg
gdtdesc:
.word 0x17    # 3 index,8 byte each,so linit is 3*8-1=0x17
.long gdt



.org 0x7dbe
.word 0x0480,0x1205,0x0f0b,0x7f07,0x0800,0x0000,0x3000,0x0077
.org 0x7dfe
.word 0xaa55
