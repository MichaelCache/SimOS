#include "print.h"

void set_gatedec(struct GATE_DESCRIPTOR *gd,int offset,short selector,short atrribute);

void main()
{
	/*set the screen to be light blue form VGA port*/
	outb(0,0x03c8);
	int i;
	//for(i=1;i<=3;i++)
	//{
		/* 0x00ffff is the RGB code for light blue*/
		outb(0x00,0x03c9);
		outb(0xff,0x03c9);
		outb(0xff,0x03c9);
	//}

	/*initial PIC i8059a chip*/
	outb(0xff,0x21);	//mask all master PIC interrupt
	outb(0xff,0xa1);	//mask all slave PIC interrupt

	outb(0x11,0x20);	//edge trigger mode
	outb(0x20,0x21);	//set IRQ0-7 map to INT 0x20-0x27
	outb(1<<2,0x21);	//slave connected to the IRQ2 port of master,00000100
	outb(0x3,0x21);		//no buffer mode

	outb(0x11,0xa0);
	outb(0x28,0xa1);
	outb(2,0xa1);
	outb(0x3,0xa1);

	outb(0xf9,0x21);
	outb(0xef,0xa1);
	//outb(0x68, 0x20);
	//outb(0x0a, 0x20);
	//outb(0x68, 0xa0);
	//outb(0x0a, 0xa0);

	/*initial IDT*/

	struct GATE_DESCRIPTOR *idt= (struct GATE_DESCRIPTOR *)0x26f800;	//IDT address is 0x26f800

	int j;
	for(j=0;j<=44;j++)		//create 256 empty IDT index
		{
			set_gatedec(idt+j,0,0,0);
		}

	set_gatedec(idt+0x21,enter_int_21,2*8,0x8e);	//create the INT 0x21 index
	set_gatedec(idt+0x2c,enter_int_2c,2*8,0x8e);	//create the INT 0x2c index

	struct IDT_DESC idt_desc;
	//set_idt(idt_desc,45,0x0026f800);
	idt_desc.address=idt;
	idt_desc.limit=45;
	ldidt(&idt_desc);	//load IDT limit and address

	sti();
	//while(1);
	for(;;)
	{
		hlt();
	}
}

void set_gatedec(struct GATE_DESCRIPTOR *gd,int offset,short selector,short atrribute)
{
	gd->offset_low	= offset & 0xffff;
	gd->selector	= selector;
	gd->dw_count	= (atrribute >> 8) & 0xff;
	gd->access_right= atrribute & 0xff;
	gd->offset_high	= (offset >> 16) & 0xffff;
}

