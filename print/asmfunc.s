.section .text
.global _ldidt
_ldidt:
lidt idt_desc
ret

idt_desc:
.word 0x7ff
.long 0xf800
