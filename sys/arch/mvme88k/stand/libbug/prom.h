/*	$OpenBSD: prom.h,v 1.1.8.2 2004/02/19 10:49:09 niklas Exp $	*/

#define MVMEPROM_CALL(x) \
	__asm__ __volatile__ (__CONCAT("or r9,r0,", __STRING(x))); \
	__asm__ __volatile__ ("tb0 0,r0,496")
