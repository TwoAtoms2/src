/*	$OpenBSD: assert.h,v 1.4.6.3 2001/11/13 21:04:15 niklas Exp $ */
#ifndef __MACHINE_ASSERT_H__
#define __MACHINE_ASSERT_H__
#ifndef assert
#define assert(x) \
({\
	if (!(x)) {\
		printf("assertion failure \"%s\" line %d file %s\n", \
		#x, __LINE__, __FILE__); \
		panic("assertion"); \
	} \
})
#endif /* assert */
#endif __MACHINE_ASSERT_H__

