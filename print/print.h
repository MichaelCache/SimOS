#ifndef __PRINT_H__
#define __PRINT_H__

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
};
void set_idt(struct IDT_DESC idtable,short limit,int address);
static inline void ldidt(struct IDT_DESC *idt_desc);
void enter_int_21();
void enter_int_2c();
static inline void sti();


static inline void outb(unsigned char color,unsigned short port)
{
	asm volatile ("outb %0,%1"::"a"(color),"d"(port));
}

static inline void hlt()
{
	asm volatile ("hlt"::);
}

void enter_int_21()
{
    asm volatile ("pushl %ds;"
    		"pushl %es;"
    		"pushl %fs;"
    		"pushl %gs;"
    		"pushal;"
    		"call int_handler21;"
    		"popal;"
			"popl %gs;"
			"popl %fs;"
			"popl %es;"
			"popl %ds;"
			"iret;"
			);
}

void enter_int_2c()
{
    asm volatile ("pushl %ds;"
    		"pushl %es;"
    		"pushl %fs;"
    		"pushl %gs;"
    		"pushal;"
    		"call int_handler2c;"
    		"popal;"
			"popl %gs;"
			"popl %fs;"
			"popl %es;"
			"popl %ds;"
			"iret;"
			);
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

void set_idt(struct IDT_DESC idtable,short limit,int address)
{
	idtable.limit=limit;
	idtable.address=address;
}

static inline void ldidt(struct IDT_DESC *idt_desc)
{
	asm volatile("lidt (%0)"::"r"(idt_desc));
}

static inline void sti()
{
	asm volatile("sti;");
}

#endif
