/*	$OpenBSD: putchar.c,v 1.2.6.1 2004/02/19 10:49:09 niklas Exp $ */

/*
 * putchar: easier to do this with outstr than to add more macros to
 * handle byte passing on the stack
 */

#include <sys/types.h>
#include <machine/prom.h>

#include "stand.h"
#include "prom.h"

void
putchar(c)

int c;

{
    char ca;
    ca = (char)c & 0xFF;
    if (ca == '\n') 
	putchar('\r');
    asm  volatile ("or r2,r0,%0" : : "r" (ca));
    MVMEPROM_CALL(MVMEPROM_OUTCHR);
}
