#include <print.h>
void main()
{
	out8(0x03c8,0);
	out8(0x03c9,0);
	out8(0x03c9,0);
	out8(0x03c9,0);
	hlt();
}
