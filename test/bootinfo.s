.section .text
.globl _start
_start:
.code32
.org 0x8200
movl $0,%eax
movl $msg,%esi
putloop:
movb (%si),%al
add  $1,%si
cmp  $0,%al
je   fin
movb $0x0e,%ah
movl $15,%ebx
int  $0x10
jmp  putloop
fin:
hlt
jmp  fin
.org 0x83cb
msg:
.byte 0x0a,0x0a
.ascii "This is a test OS. the system is boot secuceesly."
.byte 0x0a,0x00
