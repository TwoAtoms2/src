/*	$OpenBSD: sysconfunc.h,v 1.1.2.2 2002/03/28 10:36:02 niklas Exp $	*/

#ifndef _MVME88K_SYSCON_H_
#define _MVME88K_SYSCON_H_

int sysconintr_establish(int vec, struct intrhand *ih);

#endif	/* _MVME88K_SYSCON_H_ */
