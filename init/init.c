#include "print.h"

void set_gatedec(struct GATE_DESCRIPTOR *gd,int offset,short selector,short atrribute);

void main()
{
	/*set the screen to be light blue form VGA port*/
	outb(0,0x03c8);
	/* 0x00ffff is the RGB code for light blue*/
	outb(0x00,0x03c9);
	outb(0xff,0x03c9);
	outb(0xff,0x03c9);

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

	outb(0xfb,0x21);
	outb(0xff,0xa1);
	outb(0x68, 0x20);
	outb(0x0a, 0x20);
	outb(0x68, 0xa0);
	outb(0x0a, 0xa0);

	/*initial IDT*/

	struct GATE_DESCRIPTOR *idt= (struct GATE_DESCRIPTOR *)0xf800;	//IDT address is 0xf800

	int j;
	for(j=0;j<=255;j++)		//create 256 empty IDT index
		{
			set_gatedec(idt+j,0,1<<3,0x8e);
		}

	set_gatedec(idt+0x21,enter_int_21,1<<3,0x8e);	//create the INT 0x21 index
	set_gatedec(idt+0x2c,enter_int_2c,1<<3,0x8e);	//create the INT 0x2c index

	_ldidt();	//load IDT limit and address

	outb(0xf9,0x21);	//unmask some PIC interrupt
	outb(0xef,0xa1);
	sti();		//enable interrupt

	//enable mouse
	//enable_mouse();
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

