#ifndef __PRINT_H__
#define __PRINT_H__

static inline void outb(unsigned char color,unsigned short port);
static inline unsigned char inb(unsigned short port);
static inline void hlt();
void int_handler21();	//driver for PS/2 keyboard
void int_handler2c();	//driver for PS/2 mouse
struct GATE_DESCRIPTOR	//IDT gate descriptor
{
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

static inline void _ldidt();
void enter_int_21();	//int 0x21
void enter_int_2c();
static inline void sti();	//enable interrupt
static void enable_mouse();

#define PIC0_ICW1		0x0020
#define PIC0_OCW2		0x0020
#define PIC0_IMR		0x0021
#define PIC0_ICW2		0x0021
#define PIC0_ICW3		0x0021
#define PIC0_ICW4		0x0021
#define PIC1_ICW1		0x00a0
#define PIC1_OCW2		0x00a0
#define PIC1_IMR		0x00a1
#define PIC1_ICW2		0x00a1
#define PIC1_ICW3		0x00a1
#define PIC1_ICW4		0x00a1


static inline void outb(unsigned char color,unsigned short port)
{
	asm volatile ("outb %0,%1"::"a"(color),"d"(port));
}

static inline unsigned char inb(unsigned short port)
{
	unsigned char data;
	asm volatile ("inb %1,%0":"=a"(data):"d"(port));
	return data;
}
static inline void hlt()
{
	asm volatile ("hlt;"
			"jmp hlt;");
}

void enter_int_21()
{
    asm volatile ("pushl %ds;"
    		"pushl %es;"
    		"pushl %fs;"
    		"pushl %gs;"
    		"pushal;"
    		"call int_handler21;"	//call the real int 21 handler function
    		"popal;"
			"popl %gs;"
			"popl %fs;"
			"popl %es;"
			"popl %ds;"
    		"popl %ebp;"
    		//"leave;"		//don't ask why,it's the gcc'c fault.gcc will insert push %ebp when call function
			"iret;"			//so if I don't pop %ebp,iret will case cllapse.
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
    		"popl %ebp;"
    		//"leave;"
    		"iret;"
			);
}
void int_handler21()
{
	unsigned char key;
	key=inb(0x60);
	if(key==0x1e)
	{
		outb(0,0x03c8);
		/* 0xffff00 is the RGB code for yellow,this function means when keyboard is pressed the
		 * screen will turn yellow*/
		outb(0xff,0x03c9);
		outb(0xff,0x03c9);
		outb(0x00,0x03c9);
	}
	else
	{
		outb(0,0x03c8);
		/* 0xffff00 is the RGB code for yellow,this function means when keyboard is pressed the
		 * screen will turn red*/
		outb(0xff,0x03c9);
		outb(0x00,0x03c9);
		outb(0x00,0x03c9);
	}
	outb(PIC0_OCW2,0x61);
}

void int_handler2c()
{
	outb(0,0x03c8);
	/* 0xff0000 is the RGB code for red,this function means when mouse(PS/2) clicked
	 * screen will turn red*/
	outb(0xff,0x03c9);
	outb(0x00,0x03c9);
	outb(0x00,0x03c9);
}

static inline void sti()
{
	asm volatile("sti;");
}

static void enable_mouse()
{
	while(inb(0x64)&0x2!=0);
	outb(0xd4,0x64);
	while(inb(0x64)&0x2!=0);
	outb(0x47,0x60);
}
#endif
