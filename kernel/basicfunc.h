#ifndef __PRINT_H__
#define __PRINT_H__

static inline void outb(unsigned char color,unsigned short port);
static inline unsigned char inb(unsigned short port);
static inline void hlt();
void int_handler21();		//driver for PS/2 keyboard
void int_handler2c();		//driver for PS/2 mouse
struct GATE_DESCRIPTOR		//IDT gate descriptor
{
	short offset_low, selector;
	char dw_count, access_right;
	short offset_high;
};

struct FIFO
{
	unsigned char *fifo,*write,*read;
	int size,flag;
};
static void fifo_init(struct FIFO *fifo);
static inline void ldidt();
void enter_int_21();		//int 0x21
void enter_int_2c();
static inline void sti();	//enable interrupt
static void enable_mouse();

//PIC port
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

//keyboard port
#define KEY_STAT		0x0064
#define KEY_CMD			0x0064
#define KEY_DATA		0x0060

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

static void fifo_init(struct FIFO *fifo)
{

}

static inline void hlt()
{
	asm volatile ("hlt;"
			"jmp hlt;");
}

static inline void ldidt()
{
	unsigned short *idt_limit=(unsigned short *)0x8200;	//make a idt_desc_table data structure
	unsigned int *idt_base=(unsigned int *)0x8202;
	*idt_limit=0x7ff;				//IDT limit 2048(wich is 256 index with 8B each)-1=0x7ff
	*idt_base=0xf800;				//IDT base address
	asm volatile("lidt 0x8200");
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
    		"movl %ebp,%esp;"
    		"popl %ebp;"		//for call this function,pushl %ebp and movw %esp,%ebp had be done
 			"iret;"				//iret only pop out the %cs,%ip and EFLAG,
			);					//without instruction upper,the stack will chaos
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
    		"movl %ebp,%esp;"
    		"popl %ebp;"
    		"iret;"
			);
}
void int_handler21()
{
	unsigned char key;
	key=inb(KEY_DATA);				//get the key code from keyboard port
	if(key==0x1e)					//that's if A is pressed
	{
		/*outb(0,0x03c8);
		 0xffff00 is the RGB code for yellow,this function means when keyboard is pressed the
		 * screen will turn yellow
		*outb(0xff,0x03c9);
		*outb(0xff,0x03c9);
		*outb(0x00,0x03c9);*/
		draw_windows(0,0,50,50,0);	//draw a white box
	}
	else
	{
		/*outb(0,0x03c8);
		 0xffff00 is the RGB code for yellow,this function means when keyboard is pressed the
		 * screen will turn red
		outb(0xff,0x03c9);
		outb(0x00,0x03c9);
		outb(0x00,0x03c9);*/
		draw_windows(0,0,50,50,1);	//draw a white box
	}
	outb(0x61,PIC0_OCW2);		//tell PIC interrupt is handled
}

void int_handler2c()
{
	/*outb(0,0x03c8);
	 0xff0000 is the RGB code for red,this function means when mouse(PS/2) clicked
	 * screen will turn black
	*outb(0x00,0x03c9);
	*outb(0x00,0x03c9);
	*outb(0x00,0x03c9);*/
	draw_windows(0,0,50,50,5);	//draw a yello box
}

static inline void sti()
{
	asm volatile("sti;");
}

static void enable_mouse()
{
	while(inb(KEY_STAT)&0x2!=0);	//wait to keyboard ready
	outb(0xd4,KEY_CMD);				//tell i8042 the next CMD will be wrote to mouse
	while(inb(KEY_STAT)&0x2!=0);
	outb(0xf4,KEY_DATA);			//enable mouse
	while(inb(KEY_STAT)&0x2!=0);	//wait to keyboard ready
	outb(0x60,KEY_CMD);				//tell i8042,the next CMD will be wrote to it's command buffer
	while(inb(KEY_STAT)&0x2!=0);
	outb(0x47,KEY_DATA);			//enable mouse and keyboard interrupt
}
#endif
