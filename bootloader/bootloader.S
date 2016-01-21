.set PROT_MODE_CSEG,        0x8
.set SECTOR_NUMBER,         0x4
.code16

.section .text
.org 0x7c00
.globl _start
_start:
jmp entry
entry:
nop
cli			# Disable interrupts
xorw %ax,%ax
movw %ax,%ss
movw %ax,%ds
movw %ax,%es
			
open20step1:  #open A20 gate by i8042
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
orl $0x01,%eax
movl %eax,%cr0
ljmp $PROT_MODE_CSEG, $protect   	#set the CS:IP register,then the instruction might be CS:IP,which means the next pseudocode.
                     				#why 0x8,it`s 0000000000001|000 section ,the second section of gdt
                    				#jmp set the IP,ljmp set the CS:IP is the instruction's address
.code32
protect:
movl $0x10, %eax        # Our data segment selector,0000000000010|000
movw %ax, %ds                                   # -> DS: Data Segment
movw %ax, %es                                   # -> ES: Extra Segment
movw %ax, %fs                                   # -> FS
movw %ax, %gs                                   # -> GS
movw %ax, %ss

movl $0x0500,%ebp		#set up the stack base at 0x00000500
movl $0x7c00,%esp		#set up the stack top at 0x00007c00
#sti					#enable interrupts after SS SP is set

#read the second sector from disk
movl    $0x00100000,%edi
#push	%eax
#pop		%edi
movl    $0x02,%ebx

wait:
movl	$0x1f7,%edx
inb     (%dx),%al
and     $0xffffffc0,%eax
cmp     $0x40,%al
jne     wait

setread:
movl    $0x1,%eax
movl	$0x1f2,%edx
outb    %al,(%dx)
movl    %ebx,%eax
movb	$0xf3,%dl
outb    %al,(%dx)
shrl    $0x08,%eax
movb	$0xf4,%dl
outb    %al,(%dx)
shrl    $0x08,%eax
movb	$0xf5,%dl
outb    %al,(%dx)
shrl    $0x08,%eax
movb	$0xf6,%dl
outb    %al,(%dx)
movl	$0x20,%eax
movb	$0xf7,%dl
outb	%al,(%dx)

wait2:
movb	$0xf7,%dl
inb     (%dx),%al
and     $0xffffffc0,%eax
cmp     $0x40,%al
jne     wait2

readdisk:
movl    $0x80,%ecx
cld
movw	$0x1f0,%dx
repnz   insl
cmp		$SECTOR_NUMBER,%ebx
je		jumptokernel
addl	$0x1,%ebx
#addl	$0x200,%edi
jmp		wait

jumptokernel:
movl    $0x100034,%eax
call    *%eax



#.data
.p2align 2                       # force 4 byte alignment
.org 0x7d00
gdt:
.word 0x0,0x0
.byte 0x0,0x0,0x0,0x0       # null seg
.word 0xffff,0x0;
.byte 0x0,0x9a,0xdf,0x00        #executabl and readable from 0x0 with limit 0xffffffff code seg for bootloader and kernel from 0 to 4 GB
.word 0xffff,0x0;
.byte 0x0,0x92,0xdf,0x00    #writeable from 0x0 with limit 0xffffffff)        data seg
gdtdesc:
.word 0x17    # 3 index,8 byte each,so linit is 3*8-1=0x17
.long gdt



.org 0x7dbe
.word 0x0480,0x1205,0x0f0b,0x7f07,0x0800,0x0000,0x3000,0x0077
.org 0x7dfe
.word 0xaa55


