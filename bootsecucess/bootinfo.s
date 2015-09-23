.code16
.org 0x8200
.section .text
.globl _start
_start:
movw $0,%ax
movw %ax,%ss
movw %ax,%ds
movw %ax,%es
movw $msg,%si
putloop:
movb (%si),%al
add  $1,%si
cmp  $0,%al
je   fin
movb $0x0e,%ah
movw $15,%bx
int  $0x10
jmp  putloop
fin:
hlt
jmp  fin
msg:
.byte 0x0a,0x0a
.ascii "This is a test OS. the system is booted secuceesly."
.byte 0x0a,0x00
.org 0x83ff
.byte 0x00
