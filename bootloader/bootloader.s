.set PROT_MODE_CSEG,        0x8		# protected mode code section
.set PROT_MODE_DSEG,        0x10	# protected mode data section
.set SECTOR_NUMBER,         0xff	# read to the sector number from sector 2 to this
.code16

.section .text
.org 0x7c00
.globl _start
_start:
jmp entry
entry:
nop
cli					# Disable hardware interrupts
xorw %ax,%ax
movw %ax,%ss
movw %ax,%ds
movw %ax,%es

#movw $0x4f02,%ax
#movw $0x103,%bx
movb $0x00,%ah
movb $0x13,%al
int $0x10			#set the display mod 800*600 256 color
xorw %ax,%ax
#movw %ax,%bx

open20step1:		# open A20 gate by i8042 chip,step1,test the chip is busy or not,then tell the chip A20 bit will be set
inb $0x64,%al		# port 0x64,the state and command port
testb $0x2,%al
jnz open20step1
movb $0xd1,%al		# command 0xd1,tell i8042 data will go to out buffer
outb %al,$0x64

open20step2:		# open A20 gate step2,set the A20 bit
inb $0x64,%al
testb $0x2,%al
jnz open20step2
movb $0xdf,%al
outb %al,$0x60		# port 0x60,the data buffer port


lgdt gdtdesc		# load gdt address to GDTR
movl %cr0,%eax		# switch to 32-bit protected mode
orl $0x01,%eax
movl %eax,%cr0		# enable 32bits protected mode
ljmp $PROT_MODE_CSEG, $protect		# set the CS:IP register,jump to the 32bits code.
									# why 0x8,it`s 0000000000001|000 section ,the second table of gdt,first is 0000000000000|000
									# jmp set the IP,ljmp set the CS:IP.
.code32
protect:
movl $PROT_MODE_DSEG, %eax			# Data segment selector,0000000000010|000,can only be written.
movw %ax, %ds						# DS: Data Segment
movw %ax, %es						# ES: Extra Segment
movw %ax, %fs						# FS
movw %ax, %gs						# GS
movw %ax, %ss						# SS: Stack Segment

movl $0x0000,%ebp			# set up the stack base at 0x00000000
movl $0x7c00,%esp			# set up the stack top at 0x00007c00

#ready to read the second sector from IDE disk 0
movl    $0x100000,%edi		# the memory address to put kernel,1M upper
movl    $0x02,%ebx		

wait:						# wait to the disk ready
movl	$0x1f7,%edx
inb     (%dx),%al
and     $0xffffffc0,%eax
cmp     $0x40,%al
jne     wait

setread:					# set disk read parameters
movl    $0x1,%eax
movl	$0x1f2,%edx
outb    %al,(%dx)			# count=1
movl    %ebx,%eax
movb	$0xf3,%dl
outb    %al,(%dx)			# the sector number
shrl    $0x08,%eax
movb	$0xf4,%dl
outb    %al,(%dx)			# the low 8 bits of cylinder
shrl    $0x08,%eax
movb	$0xf5,%dl
outb    %al,(%dx)			# the high 8 bits of cylinder
shrl    $0x08,%eax
movb	$0xf6,%dl
outb    %al,(%dx)			# the head 0
movl	$0x20,%eax
movb	$0xf7,%dl
outb	%al,(%dx)			# cmd 0x20,read disk

wait2:
movb	$0xf7,%dl
inb     (%dx),%al
and     $0xffffffc0,%eax
cmp     $0x40,%al
jne     wait2

readsct:
movl    $0x80,%ecx			# read 128 times 4B,512B in total,a sector
cld							# set %edi go increase
movw	$0x1f0,%dx			# disk data port
repnz   insl				# read 4B per time form disk to ES:EDI
cmp		$SECTOR_NUMBER,%ebx
je		jmptoker
addl	$0x1,%ebx
jmp		wait

jmptoker:
movl    $0x100372,%eax		# the kernel program entry address,should be changed accordinglly
call    *%eax				# * means force jump to the address

.p2align 2					# force 4 byte alignment
.org 0x7d00
gdt:
.word 0x0,0x0				# null seg
.byte 0x0,0x0,0x0,0x0
.word 0xffff,0x0;			# second seg,for kernel code.
.byte 0x0,0x9a,0xcf,0x00	# executabl and readable from 0x0 with limit 0xffffffff code seg for bootloader and kernel from 0 to 4 GB
.word 0xffff,0x0;			# third seg,for kernel data
.byte 0x0,0x92,0xcf,0x00    # writeable from 0x0 with limit 0xffffffff data seg
							# format is :low 16bits of limit,low 16bits of base
							# middle 8 bits of base,low 8 atrribute ,high 4 ar,high 4 limit,high 8 base.

gdtdesc:					# gdt describe table
.word 0x17					# 3 index,8 byte each,so limit is 3*8-1=0x17
.long gdt					# gdt index address



.org 0x7dbe					# reserved patition information from my usb
.word 0x0480,0x1205,0x0f0b,0x7f07,0x0800,0x0000,0x3000,0x0077
.org 0x7dfe
.word 0xaa55				# bootable sign


