#ifndef __PRINT_H__
#define __PRINT_H__

static inline void outb(unsigned char color,unsigned short port);
static inline unsigned char inb(unsigned short port);
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
	unsigned char buff[256];
	int flag,write,read;
};

static void fifo_mouse_init(struct FIFO *f);
static void fifo_mouse_write(struct FIFO *f,unsigned char data);
static unsigned char fifo_mouse_read(struct FIFO *f);


//extern struct FIFO fifo_mouse;

static inline void ldidt();
void enter_int_21();		//some register handle before enter int 0x21
void enter_int_2c();
static inline void sti();	//enable interrupt
static inline void cli();	//disable interrupt
static inline void hlt();	//halt CPU
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

static void fifo_mouse_init(struct FIFO *f)
{
        f->write=0;     //write point at begin of the buffer
        f->read=0;      //read point at begin of the buffer
        f->flag=0;                      //fifo is empty,its flag is 0
}


static void fifo_mouse_write(struct FIFO *f,unsigned char data)
{
        if(f->write>255)
        {
                f->write=0;     //write point at begin of the buffer
        }
        f->buff[f->write]=data;
        f->write++;
}

unsigned char fifo_mouse_read(struct FIFO *f)
{
        if(f->read>255)
        {
                f->read=0;      //write point at begin of the buffer
        }
        unsigned char data;
        data=f->buff[f->read];
        f->read++;
	if(f->read==f->write)
	{
		f->flag=0;
	}
        return data;
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

void enter_int_21()			//enter keyboard interrupt 
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
 		"iret;"			//iret only pop out the %cs,%ip and EFLAG,
		);			//without instruction upper,the stack will chaos
}

void enter_int_2c()			//enter mouse interrupt
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
        key=inb(KEY_DATA);                              //get the key code from keyboard port
        if(key==0x1e)                                   //that's if A is pressed
        {
                draw_windows(0,0,50,50,0);      //draw a white box
        }
        else
        {
                draw_windows(0,0,50,50,1);      //draw a white box
        }
        outb(0x61,PIC0_OCW2);           //tell PIC interrupt is handled
}


struct FIFO *fifo_mouse=(struct FIFO *)0x102520;

void int_handler2c()
{
        outb(0x64,PIC1_OCW2);           //tell PIC1_OCW2 port,int-12(0x4) is handled.Code is 0x60+IRQ.
        outb(0x62,PIC0_OCW2);           //tell PIC0_OCW2 port,int-2(0x2) is handled.Code is 0x60+IRQ.
        unsigned char data;
        data=inb(KEY_DATA);
        fifo_mouse_write(fifo_mouse,data);         //write mouse data to mouse fifo
        fifo_mouse->flag=1;
}


static inline void sti()
{
	asm volatile("sti;");
}

static inline void cli()
{
	asm volatile("cli;");
}

static void enable_mouse()
{
        while(inb(KEY_STAT)&0x2!=0);  //wait to keyboard ready
        outb(0x60,KEY_CMD);           //tell i8042,the next CMD will be wrote to it's command buffer
        while(inb(KEY_STAT)&0x2!=0);	//enable keyboard
        outb(0x47,KEY_DATA);
	while(inb(KEY_STAT)&0x2!=0);	//wait to keyboard ready
	outb(0xd4,KEY_CMD);		//tell i8042 the next CMD will be wrote to mouse
	while(inb(KEY_STAT)&0x2!=0);
	outb(0xf4,KEY_DATA);		//enable mouse
}


#endif
