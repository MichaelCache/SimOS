static inline void outb(unsigned char color,unsigned short port);
static inline void hlt();
void int_handler21();	//driver for PS/2 keyboard
void int_handler2c();	//driver for PS/2 mouse
struct GATE_DESCRIPTOR	//IDT gate descriptor
{
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};
struct IDT_DESC		//IDT index descriptor
{
	short limit;
	int address;
}idt_dessc;
void set_gatedec(struct GATE_DESCRIPTOR *gd,int offset,short selector,short atrribute);
void ldidt(struct IDT_DESC *idt_desc);
void sti();

void main()
{
	/*set the screen to be light blue form VGA port*/
	outb(0,0x03c8);
	int i;
	for(i=1;i<=10;i++)
	{
		/* 0x00ffff is the RGB code for light blue*/
		outb(0x00,0x03c9);
		outb(0xff,0x03c9);
		outb(0xff,0x03c9);
	}

	/*initial PIC i8059a chip*/
	outb(0xff,0x21);	//mask all master PIC interrupt
	outb(0xff,0xa1);	//mask all slave PIC interrupt

	outb(0x11,0x20);	//edge trigger mode
	outb(0x20,0x21);	//set IRQ0-7 map to INT 0x20-0x27
	outb(1<<2,0x21);	//slave connected to the IRQ2 port of master,00000100
	outb(0x1,0x21);		//no buffer mode

	outb(0x11,0xa0);
	outb(0x28,0xa1);
	outb(2,0xa1);
	outb(0x1,0xa1);

	outb(0x00, 0x21);
	outb(0x00, 0xa1);

	/*initial IDT*/

	struct GATE_DESCRIPTOR *idt= (struct GATE_DESCRIPTOR *)0x26f800;	//IDT address is 0x26f800


	struct IDT_DESC idt_desc=
	{
			.limit=0x7ff,
			.address=0x26f800,
	};
	ldidt(&idt_desc);	//load IDT limit and address

	for(i=0;i<=255;i++)		//create 256 empty IDT index
	{
		set_gatedec(idt+i,0,0,0);
	}

	set_gatedec(idt+0x21,int_handler21,2*8,0x8e);	//create the INT 0x21 index
	set_gatedec(idt+0x21,int_handler2c,2*8,0x8e);	//create the INT 0x21 index
	sti();
	hlt();
}


void set_gatedec(struct GATE_DESCRIPTOR *gd,int offset,short selector,short atrribute)
{
	gd->offset_low	= offset & 0xffff;
	gd->selector	= selector;
	gd->dw_count	= (atrribute >> 8) & 0xff;
	gd->access_right= atrribute & 0xff;
	gd->offset_high	= (offset >> 16) & 0xffff;
}

static inline void outb(unsigned char color,unsigned short port)
{
	asm volatile ("outb %0,%1"::"a"(color),"d"(port));
}

static inline void hlt()
{
	asm volatile ("hlt"::);
}

void int_handler21()
{
	outb(0,0x03c8);
	int i;
	for(i=1;i<=10;i++)
	{
		/* 0xffff00 is the RGB code for yellow,this function means when mouse clicked the
		 * screen will turn yellow*/
		outb(0xff,0x03c9);
		outb(0xff,0x03c9);
		outb(0x00,0x03c9);
	}
}

void int_handler2c()
{
	outb(0,0x03c8);
	int i;
	for(i=1;i<=10;i++)
	{
		/* 0xff0000 is the RGB code for red,this function means when mouse clicked the
		 * screen will turn red*/
		outb(0xff,0x03c9);
		outb(0x00,0x03c9);
		outb(0x00,0x03c9);
	}
}

void ldidt(struct IDT_DESC *idt_desc)
{
	asm volatile("lidt (%0);"::"a"(idt_desc));
}

void sti()
{
	asm volatile("sti;");
}
