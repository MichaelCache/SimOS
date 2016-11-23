#include "basicfunc.h"
#include "palette.h"
#include "font.h"

void int_handler21();            //driver for PS/2 keyboard
void int_handler2c(); 

void set_gatedec(struct GATE_DESCRIPTOR *gd,int offset,short selector,short atrribute);

void mouse_decode();
unsigned char mouse_phase,mouse_code[3];
int mouse_info[3];
int mouse_x,mouse_y;

extern struct FIFO *fifo_mouse;

void main()
{

	init_palette();
	init_screen();
	draw_windows(40,30,200,160,214);

	//print_font(50,50,font_E);
	//print_font(58,50,font_F);
	print_mouse(140,70);
	/*initial PIC i8059a chip*/
	outb(0xff,PIC0_IMR);	//mask all master PIC interrupt
	outb(0xff,PIC1_IMR);	//mask all slave PIC interrupt

	outb(0x11,PIC0_ICW1);	//edge trigger mode
	outb(0x20,PIC0_ICW2);	//set IRQ0-7 map to INT 0x20-0x27
	outb(1<<2,PIC0_ICW3);	//slave connected to the IRQ2 port of master,00000100
	outb(0x01,PIC0_ICW4);		//no buffer mode

	outb(0x11,PIC1_ICW1);
	outb(0x28,PIC1_ICW2);	//set IRQ8-15 map to INT 0x28-0x2f
	outb(2,PIC1_ICW3);
	outb(0x01,PIC1_ICW4);

	/*initial IDT*/

	struct GATE_DESCRIPTOR *idt= (struct GATE_DESCRIPTOR *)0xf800;	//IDT address is 0xf800

	int j;
	for(j=0;j<=255;j++)		//create 256 empty IDT index
		{
			set_gatedec(idt+j,0,1<<3,0x8e);
		}

	set_gatedec(idt+0x21,enter_int_21,1<<3,0x8e);	//create the INT 0x21 index
	set_gatedec(idt+0x2c,enter_int_2c,1<<3,0x8e);	//create the INT 0x2c index

	ldidt();				//load IDT limit and address

	outb(0xf9,PIC0_IMR);	//unmask some PIC interrupt,enable int 0x21
	outb(0xef,PIC1_IMR);	//enable PIC int 0x2c
	sti();					//enable CPU accept interrupt

        fifo_mouse_init(fifo_mouse);		//size 256B

	//enable mouse
	mouse_phase=0;
	enable_mouse();
	mouse_x=140;
	mouse_y=70;
	for(;;)
	{
		cli();
		if(fifo_mouse->flag==0)
		{
			sti();
		}
		else
		{
			sti();
			mouse_decode();
		}
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

void mouse_decode()
{
	unsigned char data;
        data=fifo_mouse_read(fifo_mouse);
        if(mouse_phase==1)
        {
                mouse_code[0]=data;
                mouse_phase=2;
		return;
        }
        if(mouse_phase==2)
        {
                mouse_code[1]=data;
                mouse_phase=3;
		return;
        }
        if(mouse_phase==3)
        {
                mouse_code[2]=data;
                mouse_phase=1;
                mouse_info[0]=mouse_code[0];//& 0x07;
                mouse_info[1]=mouse_code[1];
                mouse_info[2]=mouse_code[2];
        	if((mouse_info[0] & 0x10) != 0)
        	{
                	mouse_info[1] |=0xffffff00;
        	}
        	if((mouse_info[0] & 0x20) != 0)
        	{
                	mouse_info[2] |=0xffffff00;
        	}
        	mouse_x+=mouse_info[1];
		mouse_y+=mouse_info[2]*(-1);
		print_mouse(mouse_x,mouse_y);
		return;
        }
        if(mouse_phase==0)
        {
                if(data==0xfa)
        	{
                        mouse_phase=1;
                }
		return;
        }
}
