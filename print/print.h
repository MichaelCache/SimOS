#ifndef __PRINT_H__
#define __PRINT_H__

static inline void outb(unsigned short port,unsigned char color);
static inline void hlt();

static inline void outb(unsigned short port,unsigned char color)
{
	asm volatile ("outb %0,%1"::"a"(color),"d"(port));
}

static inline void hlt()
{
	asm volatile ("hlt"::);
}

#endif
