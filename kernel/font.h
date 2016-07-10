#ifndef __PRINT_FONT__
#define __PRINT_FONT__
//#include "palette.h"

static void print_font(int x,int y,char font[]);

static char font_A[16]=
{
	0x00,0x18,0x18,0x18,0x18,0x24,0x24,0x24,
	0x24,0x7e,0x42,0x42,0x42,0xe7,0x00,0x00,
};

static void print_font(int x,int y,char font[])
{
        unsigned char *vram=(unsigned char *)0x000a0000;
        int i,j,start;
        start=y*320+x;
	for(i=0;i<16;i++)
	{
		j=start+320*i;
		if((font[i]&0x80)!=0)
		{
			*(vram+j)=0;
		}
		if((font[i]&0x40)!=0)
		{
			*(vram+j+1)=0;
		}
		if((font[i]&0x20)!=0)
		{
			*(vram+j+2)=0;
		}
		if((font[i]&0x10)!=0)
		{
			*(vram+j+3)=0;
		}
		if((font[i]&0x08)!=0)
		{
			*(vram+j+4)=0;
		}
		if((font[i]&0x04)!=0)
		{
			*(vram+j+5)=0;
		}
		if((font[i]&0x02)!=0)
		{
			*(vram+j+6)=0;
		}
		if((font[i]&0x01)!=0)
		{
			*(vram+j+7)=0;
		}
	}
}
#endif
