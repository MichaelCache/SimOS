.section .text
.global _ldidt
_ldidt:
lidt idt_desc
ret

idt_desc:
.word 0x7ff		#IDT limit 2048(wich is 256 index with 8B each)-1=0x7ff
.long 0xf800	#IDT base address
