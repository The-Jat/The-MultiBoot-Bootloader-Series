#ifndef __SYSTEM_H
#define __SYSTEM_H

/*

Include Guard
The include guard ensures that the contents of the header file are only included once during compilation, preventing multiple definition errors.

*/


/*
They are declared as extern to indicate that their definitions are provided elsewhere.

*/
/* Kernel Main */
extern unsigned char *memcpy(unsigned char *dest, const unsigned char *src, int count);
extern unsigned char *memset(unsigned char *dest, unsigned char val, int count);
extern unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);
extern int strlen(const char *str);
extern unsigned char inportb (unsigned short _port);
extern void outportb (unsigned short _port, unsigned char _data);

/* VGA driver */
extern void cls();
extern void putch(unsigned char c);
extern void puts(unsigned char *str);
extern void settextcolor(unsigned char forecolor, unsigned char backcolor);
extern void init_video();

#endif
