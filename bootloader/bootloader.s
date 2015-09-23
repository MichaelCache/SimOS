.code16
.org 0x7c00
.section .text
.globl _start
_start:
jmp entry
nop
.ascii "testos.1"
.word 0x0200,0x8608,0x0204,0x0000,0x0000,0x00f8,0x3f00,0xff00,0x0000,0x0008,0x0000,0x7730,0xbd00,0x001d,0x0000,0x0000,0x0200,0x0000,0x0100,0x0600,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x8000,0x2901,0x353b,0x484a,0x4f4e,0x4e20,0x4d41,0x2045,0x2020
.byte 0x20	/* all this data are copy from usb bios info */
.ascii "FAT32   "
entry:
movw $0,%ax
movw %ax,%ss
movw $0x7c00,%sp
movw %ax,%ds
/* movw %ax,%es
movw $msg,%si
putloop:
movb (%si),%al
add  $1,%si
cmp  $0,%al
je   readdisk
movb $0x0e,%ah
movw $15,%bx
int  $0x10
jmp  putloop
readdisk: */
movw $0x0820,%ax
movw %ax,%es
movb $0,%ch	/* clinder,it depends on the real boot file's location on the disk or usb */
movb $0,%dh	/* head,it depends 	76	*/
movb $2,%cl	/* sector,it depends 	33	*/
movb $0x00,%dl
movb $0x02,%ah
movb $1,%al
movw $0,%bx
movb $0x00,%dl	/* change to 0x80 if use usb */
int  $0x13
#movw %es,%ax
#add  $0x0020,%ax
nop
movw $0x8200,%ax
jmp  %ax
fin:
hlt
jmp  fin
msg:
.byte 0x0a,0x0a
.ascii "This is a test OS"
.byte 0x0a,0x0a
.org 0x7dbe
.word 0x0480,0x1205,0x0f0b,0x7f07,0x0800,0x0000,0x3000,0x0077
.org 0x7dfe
.word 0xaa55
