#include <print.h>
void main()
{
	outb(0x03c8,0);
	int i;
	for(i=1;i<=10;i++)
	{
		outb(0x03c9,0x00);
		outb(0x03c9,0xff);
		outb(0x03c9,0xff);
	}
	hlt();
}
