#include <defs.h>
#include <x86.h>
#include <elf.h>

#define SECTSIZE        512
#define ELFHDR          ((struct elfhdr *)0x100000)      //kernel put at the address from 1MB

/* waitdisk - wait for disk ready */
static void
waitdisk(void) {
    while ((inb(0x1F7) & 0xC0) != 0x40)     //check the IDE disk 0 state,0x1f7 data and command buffer
        /* do nothing */;
}

/* readsect - read a single sector at @secno into @dst */
static void
readsect(void *dst, uint32_t secno) {
    // wait for disk to be ready
    waitdisk();

    outb(0x1F2, 1);                         // sector count = 1
    outb(0x1F3, secno & 0xFF);              //set sector number
    outb(0x1F4, (secno >> 8) & 0xFF);       //low 8 bit of track
    outb(0x1F5, (secno >> 16) & 0xFF);      //high 8 bits of track
    outb(0x1F6, ((secno >> 24) & 0xF) | 0xE0);  //head
    outb(0x1F7, 0x20);                      // cmd 0x20 - read sectors

    // wait for disk to be ready
    waitdisk();

    // read a sector
    insl(0x1F0, dst, SECTSIZE / 4);     //read a Dword at dst for 128 times which 512B in total from disk 0
}

/* *
 * readseg - read @count bytes at @offset from kernel into virtual address @va,
 * might copy more than asked.
 * */
static void
readseg(uintptr_t va, uint32_t count, uint32_t offset) {
    uintptr_t end_va = va + count;

    // round down to sector boundary
    va -= offset % SECTSIZE;

    // translate from bytes to sectors; kernel starts at sector 1
    uint32_t secno = (offset / SECTSIZE) + 1;

    // If this is too slow, we could read lots of sectors at a time.
    // We'd write more to memory than asked, but it doesn't matter --
    // we load in increasing order.
    for (; va < end_va; va += SECTSIZE, secno ++) {
        readsect((void *)va, secno);
    }
}

/* bootmain - the entry of bootloader */
void main()
{

asm volatile (
.set PROT_MODE_CSEG,        0x8 

.code16
.org 0x7c00
.section .text
.globl _start
_start:
jmp entry
nop
entry:
cli			# Disable interrupts
movw $0,%ax
movw %ax,%ss
movw %ax,%ds

			
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
orl $0x1,%eax
movl %eax,%cr0
ljmp $PROT_MODE_CSEG,$protect   #set the CS:IP register,then the instruction might be CS:IP,which means the next pseudocode.
                     #why 0x8,it`s 0000000000001|000 section ,the second section of gdt
                    #jmp set the IP,ljmp set the CS:IP is the instruction's address
.code32
protect:
movw $0x10, %ax        # Our data segment selector,0000000000010|000
movw %ax, %ds                                   # -> DS: Data Segment
movw %ax, %es                                   # -> ES: Extra Segment
movw %ax, %fs                                   # -> FS
movw %ax, %gs                                   # -> GS
movw %ax, %ss

movl $0x0500,%ebp		#set up the stack base at 0x00000500
movl $_start,%esp	#set up the stack top at 0x00007c00
sti		#enable interrupts after SS SP is set
call bootstep2



#read the second sector from disk
#wait:
#inb  $0x1f7, %ax
#and  $0xc0, %ax
#testb $0x40, %ax		#check the IDE disk 0 state,0x1f7 data and command buffer
#jnz  $wait

#readdisk:
#movw $0x0201,%ax
#movw $0x0020,%bx
#outb %al,$0x1f2		  # sector count = 1
#outb %ah,$0x1F3          #set sector number=2
#outb %bh,$0x1F4          #low 8 bit of track 0
#outb %bh,$0x1F5    	  #high 8 bits of track 0
#outb %bh,$0x1F6  	      #head 0
#outb %bl,$0x1F7  		  #cmd 0x20 - read sectors
#cld
#movl $0x000100000,%edi
#repne insl

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
);


void
bootstep2(void) {
	
    // read the 1st page off disk
    readseg((uintptr_t)ELFHDR, SECTSIZE * 8, 0);

    // is this a valid ELF?
    if (ELFHDR->e_magic != ELF_MAGIC) {
        goto bad;
    }

    struct proghdr *ph, *eph;

    // load each program segment (ignores ph flags)
    ph = (struct proghdr *)((uintptr_t)ELFHDR + ELFHDR->e_phoff);
    eph = ph + ELFHDR->e_phnum;
    for (; ph < eph; ph ++) {
        readseg(ph->p_va & 0xFFFFFF, ph->p_memsz, ph->p_offset);
    }

    // call the entry point from the ELF header
    // note: does not return
    ((void (*)(void))(ELFHDR->e_entry & 0xFFFFFF))();

bad:
    outw(0x8A00, 0x8A00);
    outw(0x8A00, 0x8E00);

    /* do nothing */
    while (1);
}
}

