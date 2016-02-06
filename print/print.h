static inline void out8(int port,int color);
static inline void hlt();

static inline void out8(int port,int color)
{
	asm volatile ("outb %0,%1"::"a"(color),"d"(port));
}

static inline void hlt()
{
	asm volatile ("hlt"::);
}
