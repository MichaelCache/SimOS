#include <print.h>
void main()
{
	outb(0x03c8,0);
	outb(0x03c9,0);
	outb(0x03c9,0);
	outb(0x03c9,0);
	hlt();
}
