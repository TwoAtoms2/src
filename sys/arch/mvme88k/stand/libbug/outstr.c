/*	$OpenBSD: outstr.c,v 1.1.8.1 2001/10/31 03:01:20 nate Exp $	*/

/*
 * bug routines -- assumes that the necessary sections of memory
 * are preserved.
 */
#include <sys/types.h>
#include <machine/prom.h>

void
mvmeprom_outstr(start, end)
	char *start, *end;
{
	asm volatile ("or r2,r0,%0": : "r" (start));
	asm volatile ("or r3,r0,%0": : "r" (end));
	MVMEPROM_CALL(MVMEPROM_OUTSTR);
}
