/*	$OpenBSD: bus.h,v 1.7.4.3 2002/03/28 11:23:52 niklas Exp $	*/
/*	$NetBSD: bus.h,v 1.31 2001/09/21 15:30:41 wiz Exp $	*/

/*-
 * Copyright (c) 1996, 1997, 1998, 2001 The NetBSD Foundation, Inc.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Jason R. Thorpe of the Numerical Aerospace Simulation Facility,
 * NASA Ames Research Center.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the NetBSD
 *	Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * Copyright (c) 1997-1999, 2001 Eduardo E. Horvath. All rights reserved.
 * Copyright (c) 1996 Charles M. Hannum.  All rights reserved.
 * Copyright (c) 1996 Christopher G. Demetriou.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by Christopher G. Demetriou
 *	for the NetBSD Project.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef _SPARC_BUS_H_
#define _SPARC_BUS_H_

#include <machine/types.h>
#include <machine/ctlreg.h>

#define	__HAS_NEW_BUS_DMAMAP_SYNC

/*
 * Debug hooks
 */

#define	BSDB_ACCESS	0x01
#define BSDB_MAP	0x02
extern int bus_space_debug;

/*
 * UPA and SBUS spaces are non-cached and big endian
 * (except for RAM and PROM)
 *
 * PCI spaces are non-cached and little endian
 */

enum bus_type { 
	UPA_BUS_SPACE,
	SBUS_BUS_SPACE,
	PCI_CONFIG_BUS_SPACE,
	PCI_IO_BUS_SPACE,
	PCI_MEMORY_BUS_SPACE,
	LAST_BUS_SPACE
}; 
extern int bus_type_asi[];
extern int bus_stream_asi[];
/* For backwards compatibility */
#define SPARC_BUS_SPACE	UPA_BUS_SPACE

#define __BUS_SPACE_HAS_STREAM_METHODS	1

/*
 * Bus address and size types
 */
typedef	u_int64_t	bus_space_handle_t;
typedef enum bus_type	bus_type_t;
typedef u_int64_t	bus_addr_t;
typedef u_int64_t	bus_size_t;

/* For buses which have an iospace. */
#define	BUS_ADDR_IOSPACE(x)	((x)>>32)
#define	BUS_ADDR_PADDR(x)	((x)&0xffffffff)
#define	BUS_ADDR(io, pa)	(((io)<<32)|(pa))

/*
 * Access methods for bus resources and address space.
 */
typedef struct sparc_bus_space_tag	*bus_space_tag_t;

struct sparc_bus_space_tag {
	void		*cookie;
	bus_space_tag_t	parent;
	int		type;

	int     (*sparc_bus_alloc)(bus_space_tag_t, 
		bus_addr_t, bus_addr_t,
		bus_size_t, bus_size_t, bus_size_t, 
		int, bus_addr_t *, bus_space_handle_t *);

	void	(*sparc_bus_free)(bus_space_tag_t, 
		bus_space_handle_t, bus_size_t);

	int	(*sparc_bus_map)(bus_space_tag_t,
		bus_type_t, bus_addr_t,	bus_size_t,
		int, vaddr_t, bus_space_handle_t *);

	int	(*sparc_bus_unmap)(bus_space_tag_t,
		bus_space_handle_t, bus_size_t);

	int	(*sparc_bus_subregion)(bus_space_tag_t,
		bus_space_handle_t, bus_size_t,
		bus_size_t, bus_space_handle_t *);

	void	(*sparc_bus_barrier)(bus_space_tag_t,
		bus_space_handle_t, bus_size_t,
		bus_size_t, int);

	paddr_t	(*sparc_bus_mmap)(bus_space_tag_t,
		bus_addr_t, off_t, int, int);

	void	*(*sparc_intr_establish)(bus_space_tag_t,
		int, int, int,
		int (*)(void *), void *);

};

#if 0
/*
 * The following macro could be used to generate the bus_space*() functions
 * but it uses a gcc extension and is ANSI-only.
#define PROTO_bus_space_xxx(bus_space_tag_t t, ...)
#define RETURNTYPE_bus_space_xxx	void *
#define BUSFUN(name, returntype, t, args...)			\
	__inline__ RETURNTYPE_##name				\
	bus_##name PROTO_##name					\
	{							\
		while (t->sparc_##name == NULL)			\
			t = t->parent;				\
		return (*(t)->sparc_##name)(t, args);		\
	}
 */
#endif

/*
 * Bus space function prototypes.
 */
static int	bus_space_alloc(
				bus_space_tag_t,
				bus_addr_t,		/* reg start */
				bus_addr_t,		/* reg end */
				bus_size_t,		/* size */
				bus_size_t,		/* alignment */
				bus_size_t,		/* boundary */
				int,			/* flags */
				bus_addr_t *, 
				bus_space_handle_t *);
static void	bus_space_free(
				bus_space_tag_t,
				bus_space_handle_t,
				bus_size_t);
static int	bus_space_map(
				bus_space_tag_t,
				bus_addr_t,
				bus_size_t,
				int,			/*flags*/
				bus_space_handle_t *);
static int	bus_space_map2(
				bus_space_tag_t,
				bus_type_t,
				bus_addr_t,
				bus_size_t,
				int,			/*flags*/
				vaddr_t,		/*preferred vaddr*/
				bus_space_handle_t *);
static int	bus_space_unmap(
				bus_space_tag_t,
				bus_space_handle_t,
				bus_size_t);
static int	bus_space_subregion(
				bus_space_tag_t,
				bus_space_handle_t,
				bus_size_t,
				bus_size_t,
				bus_space_handle_t *);
static void	bus_space_barrier(
				bus_space_tag_t,
				bus_space_handle_t,
				bus_size_t,
				bus_size_t,
				int);
static paddr_t	bus_space_mmap(
				bus_space_tag_t,
				bus_addr_t,		/*addr*/
				off_t,			/*offset*/
				int,			/*prot*/
				int);			/*flags*/
static void	*bus_intr_establish(
				bus_space_tag_t,
				int,			/*bus-specific intr*/
				int,			/*device class level,
							  see machine/intr.h*/
				int,			/*flags*/
				int (*)(void *),	/*handler*/
				void *);		/*handler arg*/


/* This macro finds the first "upstream" implementation of method `f' */
#define _BS_CALL(t,f)			\
	while (t->f == NULL)		\
		t = t->parent;		\
	return (*(t)->f)

__inline__ int
bus_space_alloc(t, rs, re, s, a, b, f, ap, hp)
	bus_space_tag_t t;
	bus_addr_t	rs;
	bus_addr_t	re;
	bus_size_t	s;
	bus_size_t	a;
	bus_size_t	b;
	int		f;
	bus_addr_t	*ap;
	bus_space_handle_t *hp;
{
	_BS_CALL(t, sparc_bus_alloc)(t, rs, re, s, a, b, f, ap, hp);
}

__inline__ void
bus_space_free(t, h, s)
	bus_space_tag_t	t;
	bus_space_handle_t	h;
	bus_size_t	s;
{
	_BS_CALL(t, sparc_bus_free)(t, h, s);
}

__inline__ int
bus_space_map(t, a, s, f, hp)
	bus_space_tag_t	t;
	bus_addr_t	a;
	bus_size_t	s;
	int		f;
	bus_space_handle_t *hp;
{
	_BS_CALL(t, sparc_bus_map)(t, 0, a, s, f, 0, hp);
}

__inline__ int
bus_space_map2(t, bt, a, s, f, v, hp)
	bus_space_tag_t	t;
	bus_type_t	bt;
	bus_addr_t	a;
	bus_size_t	s;
	int		f;
	vaddr_t	v;
	bus_space_handle_t *hp;
{
	_BS_CALL(t, sparc_bus_map)(t, bt, a, s, f, v, hp);
}

__inline__ int
bus_space_unmap(t, h, s)
	bus_space_tag_t t;
	bus_space_handle_t h;
	bus_size_t	s;
{
	_BS_CALL(t, sparc_bus_unmap)(t, h, s);
}

__inline__ int
bus_space_subregion(t, h, o, s, hp)
	bus_space_tag_t	t;
	bus_space_handle_t h;
	bus_size_t	o;
	bus_size_t	s;
	bus_space_handle_t *hp;
{
	_BS_CALL(t, sparc_bus_subregion)(t, h, o, s, hp);
}

__inline__ paddr_t
bus_space_mmap(t, a, o, p, f)
	bus_space_tag_t	t;
	bus_addr_t	a;
	off_t		o;
	int		p;
	int		f;
{
	_BS_CALL(t, sparc_bus_mmap)(t, a, o, p, f);
}

__inline__ void *
bus_intr_establish(t, p, l, f, h, a)
	bus_space_tag_t t;
	int	p;
	int	l;
	int	f;
	int	(*h)(void *);
	void	*a;
{
	_BS_CALL(t, sparc_intr_establish)(t, p, l, f, h, a);
}

__inline__ void
bus_space_barrier(t, h, o, s, f)
	bus_space_tag_t t;
	bus_space_handle_t h;
	bus_size_t o;
	bus_size_t s;
	int f;
{
	_BS_CALL(t, sparc_bus_barrier)(t, h, o, s, f);
}

#if 1
/* XXXX Things get complicated if we use unmapped register accesses. */
#define	bus_space_vaddr(t, h)	(vaddr_t)(h)
#else
void * bus_space_vaddr(bus_space_tag_t space, bus_space_handle_t handle);
#endif

/* flags for bus space map functions */
#define BUS_SPACE_MAP_CACHEABLE		0x0001
#define BUS_SPACE_MAP_LINEAR		0x0002
#define BUS_SPACE_MAP_READONLY		0x0004
#define BUS_SPACE_MAP_PREFETCHABLE	0x0008
#define BUS_SPACE_MAP_BUS1	0x0100	/* placeholders for bus functions... */
#define BUS_SPACE_MAP_BUS2	0x0200
#define BUS_SPACE_MAP_BUS3	0x0400
#define BUS_SPACE_MAP_BUS4	0x0800


/* flags for intr_establish() */
#define BUS_INTR_ESTABLISH_FASTTRAP	1
#define BUS_INTR_ESTABLISH_SOFTINTR	2

/* flags for bus_space_barrier() */
#define	BUS_SPACE_BARRIER_READ	0x01		/* force read barrier */
#define	BUS_SPACE_BARRIER_WRITE	0x02		/* force write barrier */

/*
 * Device space probe assistant.
 * The optional callback function's arguments are:
 *	the temporary virtual address
 *	the passed `arg' argument
 */
int bus_space_probe(
		bus_space_tag_t,
		bus_type_t,
		bus_addr_t,
		bus_size_t,			/* probe size */
		size_t,				/* offset */
		int,				/* flags */
		int (*)(void *, void *),	/* callback function */
		void *);			/* callback arg */


/*
 *	u_intN_t bus_space_read_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset);
 *
 * Read a 1, 2, 4, or 8 byte quantity from bus space
 * described by tag/handle/offset.
 */
#ifndef BUS_SPACE_DEBUG
#define	bus_space_read_1(t, h, o)					\
	    lduba((h) + (o), bus_type_asi[(t)->type])

#define	bus_space_read_2(t, h, o)					\
	    lduha((h) + (o), bus_type_asi[(t)->type])

#define	bus_space_read_4(t, h, o)					\
	    lda((h) + (o), bus_type_asi[(t)->type])

#define	bus_space_read_8(t, h, o)					\
	    ldxa((h) + (o), bus_type_asi[(t)->type])
#else
#define	bus_space_read_1(t, h, o) ({					\
	unsigned char __bv =				      		\
	    lduba((h) + (o), bus_type_asi[(t)->type]);			\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsr1(%llx + %llx, %x) -> %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_type_asi[(t)->type], (unsigned int) __bv);		\
	__bv; })

#define	bus_space_read_2(t, h, o) ({					\
	unsigned short __bv =				      		\
	    lduha((h) + (o), bus_type_asi[(t)->type]);			\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsr2(%llx + %llx, %x) -> %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_type_asi[(t)->type], (unsigned int)__bv);		\
	__bv; })

#define	bus_space_read_4(t, h, o) ({					\
	unsigned int __bv =				      		\
	    lda((h) + (o), bus_type_asi[(t)->type]);			\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsr4(%llx + %llx, %x) -> %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_type_asi[(t)->type], __bv);				\
	__bv; })

#define	bus_space_read_8(t, h, o) ({					\
	u_int64_t __bv =				      		\
	    ldxa((h) + (o), bus_type_asi[(t)->type]);			\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsr8(%llx + %llx, %x) -> %llx\n", (long long)(h),	\
		(long long)(o),						\
		bus_type_asi[(t)->type], (long long)__bv);		\
	__bv; })
#endif
/*
 *	void bus_space_read_multi_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t *addr, size_t count);
 *
 * Read `count' 1, 2, 4, or 8 byte quantities from bus space
 * described by tag/handle/offset and copy into buffer provided.
 */

#define	bus_space_read_multi_1(t, h, o, a, c) do {			\
	int i = c;							\
	u_int8_t *p = (u_int8_t *)a;					\
	while (i-- > 0)							\
		*p++ = bus_space_read_1(t, h, o);			\
} while (0)

#define	bus_space_read_multi_2(t, h, o, a, c) do {			\
	int i = c;							\
	u_int16_t *p = (u_int16_t *)a;					\
	while (i-- > 0)							\
		*p++ = bus_space_read_2(t, h, o);			\
} while (0)

#define	bus_space_read_multi_4(t, h, o, a, c) do {			\
	int i = c;							\
	u_int32_t *p = (u_int32_t *)a;					\
	while (i-- > 0)							\
		*p++ = bus_space_read_4(t, h, o);			\
} while (0)

#define	bus_space_read_multi_8(t, h, o, a, c) do {			\
	int i = c;							\
	u_int64_t *p = (u_int64_t *)a;					\
	while (i-- > 0)							\
		*p++ = bus_space_read_8(t, h, o);			\
} while (0)

/*
 *	void bus_space_write_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t value);
 *
 * Write the 1, 2, 4, or 8 byte value `value' to bus space
 * described by tag/handle/offset.
 */
#ifndef BUS_SPACE_DEBUG
#define	bus_space_write_1(t, h, o, v)					\
	((void)(stba((h) + (o), bus_type_asi[(t)->type], (v))))

#define	bus_space_write_2(t, h, o, v)					\
	((void)(stha((h) + (o), bus_type_asi[(t)->type], (v))))

#define	bus_space_write_4(t, h, o, v)					\
	((void)(sta((h) + (o), bus_type_asi[(t)->type], (v))))

#define	bus_space_write_8(t, h, o, v)					\
	((void)(stxa((h) + (o), bus_type_asi[(t)->type], (v))))
#else
#define	bus_space_write_1(t, h, o, v) ({				\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsw1(%llx + %llx, %x) <- %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_type_asi[(t)->type], (unsigned int) v);		\
	((void)(stba((h) + (o), bus_type_asi[(t)->type], (v))));  })

#define	bus_space_write_2(t, h, o, v) ({				\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsw2(%llx + %llx, %x) <- %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_type_asi[(t)->type], (unsigned int) v);		\
	((void)(stha((h) + (o), bus_type_asi[(t)->type], (v)))); })

#define	bus_space_write_4(t, h, o, v) ({				\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsw4(%llx + %llx, %x) <- %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_type_asi[(t)->type], (unsigned int) v);		\
	((void)(sta((h) + (o), bus_type_asi[(t)->type], (v)))); })

#define	bus_space_write_8(t, h, o, v) ({				\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsw8(%llx + %llx, %x) <- %llx\n", (long long)(h),	\
		(long long)(o),						\
		bus_type_asi[(t)->type], (long long) v);		\
	((void)(stxa((h) + (o), bus_type_asi[(t)->type], (v)))); })
#endif
/*
 *	void bus_space_write_multi_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    const u_intN_t *addr, size_t count);
 *
 * Write `count' 1, 2, 4, or 8 byte quantities from the buffer
 * provided to bus space described by tag/handle/offset.
 */

#define	bus_space_write_multi_1(t, h, o, a, c) do {			\
	int i = c;							\
	u_int8_t *p = (u_int8_t *)a;					\
	while (i-- > 0)							\
		bus_space_write_1(t, h, o, *p++);			\
} while (0)

#define bus_space_write_multi_2(t, h, o, a, c) do {			\
	int i = c;							\
	u_int16_t *p = (u_int16_t *)a;					\
	while (i-- > 0)							\
		bus_space_write_2(t, h, o, *p++);			\
} while (0)

#define bus_space_write_multi_4(t, h, o, a, c) do {			\
	int i = c;							\
	u_int32_t *p = (u_int32_t *)a;					\
	while (i-- > 0)							\
		bus_space_write_4(t, h, o, *p++);			\
} while (0)

#define bus_space_write_multi_8(t, h, o, a, c) do {			\
	int i = c;							\
	u_int64_t *p = (u_int64_t *)a;					\
	while (i-- > 0)							\
		bus_space_write_8(t, h, o, *p++);			\
} while (0)

/*
 *	void bus_space_set_multi_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset, u_intN_t val,
 *	    size_t count);
 *
 * Write the 1, 2, 4, or 8 byte value `val' to bus space described
 * by tag/handle/offset `count' times.
 */

#define bus_space_set_multi_1(t, h, o, v, c) do {			\
	int i = c;							\
	while (i-- > 0)							\
		bus_space_write_1(t, h, o, v);				\
} while (0)

#define bus_space_set_multi_2(t, h, o, v, c) do {			\
	int i = c;							\
	while (i-- > 0)							\
		bus_space_write_2(t, h, o, v);				\
} while (0)

#define bus_space_set_multi_4(t, h, o, v, c) do {			\
	int i = c;							\
	while (i-- > 0)							\
		bus_space_write_4(t, h, o, v);				\
} while (0)

#define bus_space_set_multi_8(t, h, o, v, c) do {			\
	int i = c;							\
	while (i-- > 0)							\
		bus_space_write_8(t, h, o, v);				\
} while (0)

/*
 *	void bus_space_read_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t off,
 *	    u_intN_t *addr, bus_size_t count);
 *
 */
static void bus_space_read_region_1(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	u_int8_t *,
	bus_size_t);
static void bus_space_read_region_2(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	u_int16_t *,
	bus_size_t);
static void bus_space_read_region_4(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	u_int32_t *,
	bus_size_t);
static void bus_space_read_region_8(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	u_int64_t *,
	bus_size_t);

static __inline__ void
bus_space_read_region_1(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	u_int8_t		*a;
{
	for (; c; a++, c--, o++)
		*a = bus_space_read_1(t, h, o);
}
static __inline__ void
bus_space_read_region_2(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	u_int16_t		*a;
{
	for (; c; a++, c--, o+=2)
		*a = bus_space_read_2(t, h, o);
 }
static __inline__ void
bus_space_read_region_4(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	u_int32_t		*a;
{
	for (; c; a++, c--, o+=4)
		*a = bus_space_read_4(t, h, o);
}
static __inline__ void
bus_space_read_region_8(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	u_int64_t		*a;
{
	for (; c; a++, c--, o+=8)
		*a = bus_space_read_8(t, h, o);
}

/*
 *	void bus_space_write_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t off,
 *	    u_intN_t *addr, bus_size_t count);
 *
 */
static void bus_space_write_region_1(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int8_t *,
	bus_size_t);
static void bus_space_write_region_2(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int16_t *,
	bus_size_t);
static void bus_space_write_region_4(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int32_t *,
	bus_size_t);
static void bus_space_write_region_8(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int64_t *,
	bus_size_t);
static __inline__ void
bus_space_write_region_1(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int8_t		*a;
{
	for (; c; a++, c--, o++)
		bus_space_write_1(t, h, o, *a);
}

static __inline__ void
bus_space_write_region_2(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int16_t		*a;
{
	for (; c; a++, c--, o+=2)
		bus_space_write_2(t, h, o, *a);
}

static __inline__ void
bus_space_write_region_4(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int32_t		*a;
{
	for (; c; a++, c--, o+=4)
		bus_space_write_4(t, h, o, *a);
}

static __inline__ void
bus_space_write_region_8(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int64_t		*a;
{
	for (; c; a++, c--, o+=8)
		bus_space_write_8(t, h, o, *a);
}


/*
 *	void bus_space_set_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t off,
 *	    u_intN_t *addr, bus_size_t count);
 *
 */
static void bus_space_set_region_1(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int8_t,
	bus_size_t);
static void bus_space_set_region_2(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int16_t,
	bus_size_t);
static void bus_space_set_region_4(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int32_t,
	bus_size_t);
static void bus_space_set_region_8(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int64_t,
	bus_size_t);

static __inline__ void
bus_space_set_region_1(t, h, o, v, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int8_t		v;
{
	for (; c; c--, o++)
		bus_space_write_1(t, h, o, v);
}

static __inline__ void
bus_space_set_region_2(t, h, o, v, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int16_t		v;
{
	for (; c; c--, o+=2)
		bus_space_write_2(t, h, o, v);
}

static __inline__ void
bus_space_set_region_4(t, h, o, v, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int32_t		v;
{
	for (; c; c--, o+=4)
		bus_space_write_4(t, h, o, v);
}

static __inline__ void
bus_space_set_region_8(t, h, o, v, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int64_t		v;
{
	for (; c; c--, o+=8)
		bus_space_write_8(t, h, o, v);
}


/*
 *	void bus_space_copy_region_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh1, bus_size_t off1,
 *	    bus_space_handle_t bsh2, bus_size_t off2,
 *	    bus_size_t count);
 *
 * Copy `count' 1, 2, 4, or 8 byte values from bus space starting
 * at tag/bsh1/off1 to bus space starting at tag/bsh2/off2.
 */
static void bus_space_copy_region_1(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	bus_space_handle_t,
	bus_size_t,
	bus_size_t);
static void bus_space_copy_region_2(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	bus_space_handle_t,
	bus_size_t,
	bus_size_t);
static void bus_space_copy_region_4(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	bus_space_handle_t,
	bus_size_t,
	bus_size_t);
static void bus_space_copy_region_8(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	bus_space_handle_t,
	bus_size_t,
	bus_size_t);


static __inline__ void
bus_space_copy_region_1(t, h1, o1, h2, o2, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h1, h2;
	bus_size_t		o1, o2;
	bus_size_t		c;
{
	for (; c; c--, o1++, o2++)
	    bus_space_write_1(t, h1, o1, bus_space_read_1(t, h2, o2));
}

static __inline__ void
bus_space_copy_region_2(t, h1, o1, h2, o2, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h1, h2;
	bus_size_t		o1, o2;
	bus_size_t		c;
{
	for (; c; c--, o1+=2, o2+=2)
	    bus_space_write_2(t, h1, o1, bus_space_read_2(t, h2, o2));
}

static __inline__ void
bus_space_copy_region_4(t, h1, o1, h2, o2, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h1, h2;
	bus_size_t		o1, o2;
	bus_size_t		c;
{
	for (; c; c--, o1+=4, o2+=4)
	    bus_space_write_4(t, h1, o1, bus_space_read_4(t, h2, o2));
}

static __inline__ void
bus_space_copy_region_8(t, h1, o1, h2, o2, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h1, h2;
	bus_size_t		o1, o2;
	bus_size_t		c;
{
	for (; c; c--, o1+=8, o2+=8)
	    bus_space_write_8(t, h1, o1, bus_space_read_8(t, h2, o2));
}

/*
 *	u_intN_t bus_space_read_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset);
 *
 * Read a 1, 2, 4, or 8 byte quantity from bus space
 * described by tag/handle/offset.
 */
#ifndef BUS_SPACE_DEBUG
#define	bus_space_read_stream_1(t, h, o)				\
	    lduba((h) + (o), bus_stream_asi[(t)->type])

#define	bus_space_read_stream_2(t, h, o)				\
	    lduha((h) + (o), bus_stream_asi[(t)->type])

#define	bus_space_read_stream_4(t, h, o)				\
	    lda((h) + (o), bus_stream_asi[(t)->type])

#define	bus_space_read_stream_8(t, h, o)				\
	    ldxa((h) + (o), bus_stream_asi[(t)->type])
#else
#define	bus_space_read_stream_1(t, h, o) ({				\
	unsigned char __bv =				      		\
	    lduba((h) + (o), bus_stream_asi[(t)->type]);		\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsr1(%llx + %llx, %x) -> %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_stream_asi[(t)->type], (unsigned int) __bv);	\
	__bv; })

#define	bus_space_read_stream_2(t, h, o) ({				\
	unsigned short __bv =				      		\
	    lduha((h) + (o), bus_stream_asi[(t)->type]);		\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsr2(%llx + %llx, %x) -> %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_stream_asi[(t)->type], (unsigned int)__bv);		\
	__bv; })

#define	bus_space_read_stream_4(t, h, o) ({				\
	unsigned int __bv =				      		\
	    lda((h) + (o), bus_stream_asi[(t)->type]);			\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsr4(%llx + %llx, %x) -> %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_stream_asi[(t)->type], __bv);			\
	__bv; })

#define	bus_space_read_stream_8(t, h, o) ({				\
	u_int64_t __bv =				      		\
	    ldxa((h) + (o), bus_stream_asi[(t)->type]);			\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsr8(%llx + %llx, %x) -> %llx\n", (long long)(h),	\
		(long long)(o),						\
		bus_stream_asi[(t)->type], (long long)__bv);		\
	__bv; })
#endif
/*
 *	void bus_space_read_multi_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t *addr, size_t count);
 *
 * Read `count' 1, 2, 4, or 8 byte quantities from bus space
 * described by tag/handle/offset and copy into buffer provided.
 */

#define	bus_space_read_multi_stream_1(t, h, o, a, c) do {		\
	int i = c;							\
	u_int8_t *p = (u_int8_t *)a;					\
	while (i-- > 0)							\
		*p++ = bus_space_read_stream_1(t, h, o);		\
} while (0)

#define	bus_space_read_multi_stream_2(t, h, o, a, c) do {		\
	int i = c;							\
	u_int16_t *p = (u_int16_t *)a;					\
	while (i-- > 0)							\
		*p++ = bus_space_read_stream_2(t, h, o);		\
} while (0)

#define	bus_space_read_multi_stream_4(t, h, o, a, c) do {		\
	int i = c;							\
	u_int32_t *p = (u_int32_t *)a;					\
	while (i-- > 0)							\
		*p++ = bus_space_read_stream_4(t, h, o);		\
} while (0)

#define	bus_space_read_multi_stream_8(t, h, o, a, c) do {		\
	int i = c;							\
	u_int64_t *p = (u_int64_t *)a;					\
	while (i-- > 0)							\
		*p++ = bus_space_read_stream_8(t, h, o);		\
} while (0)

/*
 *	void bus_space_write_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    u_intN_t value);
 *
 * Write the 1, 2, 4, or 8 byte value `value' to bus space
 * described by tag/handle/offset.
 */
#ifndef BUS_SPACE_DEBUG
#define	bus_space_write_stream_1(t, h, o, v)				\
	((void)(stba((h) + (o), bus_stream_asi[(t)->type], (v))))

#define	bus_space_write_stream_2(t, h, o, v)				\
	((void)(stha((h) + (o), bus_stream_asi[(t)->type], (v))))

#define	bus_space_write_stream_4(t, h, o, v)				\
	((void)(sta((h) + (o), bus_stream_asi[(t)->type], (v))))

#define	bus_space_write_stream_8(t, h, o, v)				\
	((void)(stxa((h) + (o), bus_stream_asi[(t)->type], (v))))
#else
#define	bus_space_write_stream_1(t, h, o, v) ({				\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsw1(%llx + %llx, %x) <- %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_stream_asi[(t)->type], (unsigned int) v);		\
	((void)(stba((h) + (o), bus_stream_asi[(t)->type], (v))));  })

#define	bus_space_write_stream_2(t, h, o, v) ({				\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsw2(%llx + %llx, %x) <- %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_stream_asi[(t)->type], (unsigned int) v);		\
	((void)(stha((h) + (o), bus_stream_asi[(t)->type], (v)))); })

#define	bus_space_write_stream_4(t, h, o, v) ({				\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsw4(%llx + %llx, %x) <- %x\n", (long long)(h),		\
		(long long)(o),						\
		bus_stream_asi[(t)->type], (unsigned int) v);		\
	((void)(sta((h) + (o), bus_stream_asi[(t)->type], (v)))); })

#define	bus_space_write_stream_8(t, h, o, v) ({				\
	if (bus_space_debug & BSDB_ACCESS)				\
	printf("bsw8(%llx + %llx, %x) <- %llx\n", (long long)(h),	\
		(long long)(o),						\
		bus_stream_asi[(t)->type], (long long) v);		\
	((void)(stxa((h) + (o), bus_stream_asi[(t)->type], (v)))); })
#endif
/*
 *	void bus_space_write_multi_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset,
 *	    const u_intN_t *addr, size_t count);
 *
 * Write `count' 1, 2, 4, or 8 byte quantities from the buffer
 * provided to bus space described by tag/handle/offset.
 */

#define	bus_space_write_multi_stream_1(t, h, o, a, c) do {		\
	int i = c;							\
	u_int8_t *p = (u_int8_t *)a;					\
	while (i-- > 0)							\
		bus_space_write_stream_1(t, h, o, *p++);		\
} while (0)

#define bus_space_write_multi_stream_2(t, h, o, a, c) do {		\
	int i = c;							\
	u_int16_t *p = (u_int16_t *)a;					\
	while (i-- > 0)							\
		bus_space_write_stream_2(t, h, o, *p++);		\
} while (0)

#define bus_space_write_multi_stream_4(t, h, o, a, c) do {		\
	int i = c;							\
	u_int32_t *p = (u_int32_t *)a;					\
	while (i-- > 0)							\
		bus_space_write_stream_4(t, h, o, *p++);		\
} while (0)

#define bus_space_write_multi_stream_8(t, h, o, a, c) do {		\
	int i = c;							\
	u_int64_t *p = (u_int64_t *)a;					\
	while (i-- > 0)							\
		bus_space_write_stream_8(t, h, o, *p++);		\
} while (0)

/*
 *	void bus_space_set_multi_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t offset, u_intN_t val,
 *	    size_t count);
 *
 * Write the 1, 2, 4, or 8 byte value `val' to bus space described
 * by tag/handle/offset `count' times.
 */

#define bus_space_set_multi_stream_1(t, h, o, v, c) do {		\
	int i = c;							\
	while (i-- > 0)							\
		bus_space_write_stream_1(t, h, o, v);			\
} while (0)

#define bus_space_set_multi_stream_2(t, h, o, v, c) do {		\
	int i = c;							\
	while (i-- > 0)							\
		bus_space_write_stream_2(t, h, o, v);			\
} while (0)

#define bus_space_set_multi_stream_4(t, h, o, v, c) do {		\
	int i = c;							\
	while (i-- > 0)							\
		bus_space_write_stream_4(t, h, o, v);			\
} while (0)

#define bus_space_set_multi_stream_8(t, h, o, v, c) do {		\
	int i = c;							\
	while (i-- > 0)							\
		bus_space_write_stream_8(t, h, o, v);			\
} while (0)

/*
 *	void bus_space_read_region_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t off,
 *	    u_intN_t *addr, bus_size_t count);
 *
 */
static void bus_space_read_region_stream_1(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	u_int8_t *,
	bus_size_t);
static void bus_space_read_region_stream_2(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	u_int16_t *,
	bus_size_t);
static void bus_space_read_region_stream_4(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	u_int32_t *,
	bus_size_t);
static void bus_space_read_region_stream_8(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	u_int64_t *,
	bus_size_t);

static __inline__ void
bus_space_read_region_stream_1(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	u_int8_t		*a;
{
	for (; c; a++, c--, o++)
		*a = bus_space_read_stream_1(t, h, o);
}
static __inline__ void
bus_space_read_region_stream_2(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	u_int16_t		*a;
{
	for (; c; a++, c--, o+=2)
		*a = bus_space_read_stream_2(t, h, o);
 }
static __inline__ void
bus_space_read_region_stream_4(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	u_int32_t		*a;
{
	for (; c; a++, c--, o+=4)
		*a = bus_space_read_stream_4(t, h, o);
}
static __inline__ void
bus_space_read_region_stream_8(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	u_int64_t		*a;
{
	for (; c; a++, c--, o+=8)
		*a = bus_space_read_stream_8(t, h, o);
}

/*
 *	void bus_space_write_region_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t off,
 *	    u_intN_t *addr, bus_size_t count);
 *
 */
static void bus_space_write_region_stream_1(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int8_t *,
	bus_size_t);
static void bus_space_write_region_stream_2(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int16_t *,
	bus_size_t);
static void bus_space_write_region_stream_4(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int32_t *,
	bus_size_t);
static void bus_space_write_region_stream_8(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int64_t *,
	bus_size_t);
static __inline__ void
bus_space_write_region_stream_1(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int8_t		*a;
{
	for (; c; a++, c--, o++)
		bus_space_write_stream_1(t, h, o, *a);
}

static __inline__ void
bus_space_write_region_stream_2(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int16_t		*a;
{
	for (; c; a++, c--, o+=2)
		bus_space_write_stream_2(t, h, o, *a);
}

static __inline__ void
bus_space_write_region_stream_4(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int32_t		*a;
{
	for (; c; a++, c--, o+=4)
		bus_space_write_stream_4(t, h, o, *a);
}

static __inline__ void
bus_space_write_region_stream_8(t, h, o, a, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int64_t		*a;
{
	for (; c; a++, c--, o+=8)
		bus_space_write_stream_8(t, h, o, *a);
}


/*
 *	void bus_space_set_region_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh, bus_size_t off,
 *	    u_intN_t *addr, bus_size_t count);
 *
 */
static void bus_space_set_region_stream_1(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int8_t,
	bus_size_t);
static void bus_space_set_region_stream_2(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int16_t,
	bus_size_t);
static void bus_space_set_region_stream_4(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int32_t,
	bus_size_t);
static void bus_space_set_region_stream_8(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	const u_int64_t,
	bus_size_t);

static __inline__ void
bus_space_set_region_stream_1(t, h, o, v, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int8_t		v;
{
	for (; c; c--, o++)
		bus_space_write_stream_1(t, h, o, v);
}

static __inline__ void
bus_space_set_region_stream_2(t, h, o, v, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int16_t		v;
{
	for (; c; c--, o+=2)
		bus_space_write_stream_2(t, h, o, v);
}

static __inline__ void
bus_space_set_region_stream_4(t, h, o, v, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int32_t		v;
{
	for (; c; c--, o+=4)
		bus_space_write_stream_4(t, h, o, v);
}

static __inline__ void
bus_space_set_region_stream_8(t, h, o, v, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h;
	bus_size_t		o, c;
	const u_int64_t		v;
{
	for (; c; c--, o+=8)
		bus_space_write_stream_8(t, h, o, v);
}


/*
 *	void bus_space_copy_region_stream_N(bus_space_tag_t tag,
 *	    bus_space_handle_t bsh1, bus_size_t off1,
 *	    bus_space_handle_t bsh2, bus_size_t off2,
 *	    bus_size_t count);
 *
 * Copy `count' 1, 2, 4, or 8 byte values from bus space starting
 * at tag/bsh1/off1 to bus space starting at tag/bsh2/off2.
 */
static void bus_space_copy_region_stream_1(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	bus_space_handle_t,
	bus_size_t,
	bus_size_t);
static void bus_space_copy_region_stream_2(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	bus_space_handle_t,
	bus_size_t,
	bus_size_t);
static void bus_space_copy_region_stream_4(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	bus_space_handle_t,
	bus_size_t,
	bus_size_t);
static void bus_space_copy_region_stream_8(bus_space_tag_t,
	bus_space_handle_t,
	bus_size_t,
	bus_space_handle_t,
	bus_size_t,
	bus_size_t);


static __inline__ void
bus_space_copy_region_stream_1(t, h1, o1, h2, o2, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h1, h2;
	bus_size_t		o1, o2;
	bus_size_t		c;
{
	for (; c; c--, o1++, o2++)
	    bus_space_write_stream_1(t, h1, o1, bus_space_read_stream_1(t, h2, o2));
}

static __inline__ void
bus_space_copy_region_stream_2(t, h1, o1, h2, o2, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h1, h2;
	bus_size_t		o1, o2;
	bus_size_t		c;
{
	for (; c; c--, o1+=2, o2+=2)
	    bus_space_write_stream_2(t, h1, o1, bus_space_read_stream_2(t, h2, o2));
}

static __inline__ void
bus_space_copy_region_stream_4(t, h1, o1, h2, o2, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h1, h2;
	bus_size_t		o1, o2;
	bus_size_t		c;
{
	for (; c; c--, o1+=4, o2+=4)
	    bus_space_write_stream_4(t, h1, o1, bus_space_read_stream_4(t, h2, o2));
}

static __inline__ void
bus_space_copy_region_stream_8(t, h1, o1, h2, o2, c)
	bus_space_tag_t		t;
	bus_space_handle_t	h1, h2;
	bus_size_t		o1, o2;
	bus_size_t		c;
{
	for (; c; c--, o1+=8, o2+=8)
	    bus_space_write_stream_8(t, h1, o1, bus_space_read_8(t, h2, o2));
}

/* OpenBSD "raw" wrappers around the NetBSD "stream" methods. */
/* XXXART - I'm lazy so I'll only implement the ones I need. */
#define BUS_DMA_RAW	BUS_DMA_STREAMING
#define bus_space_read_raw_multi_2(t, h, o, a, c) \
	bus_space_read_multi_stream_2(t, h, o, (u_int8_t *)(a), c/2)
#define bus_space_write_raw_multi_2(t, h, o, a, c) \
	bus_space_write_multi_stream_2(t, h, o, (u_int8_t *)(a), c/2)
#define bus_space_read_raw_multi_4(t, h, o, a, c) \
	bus_space_read_multi_stream_4(t, h, o, (u_int8_t *)(a), c/4)
#define bus_space_write_raw_multi_4(t, h, o, a, c) \
	bus_space_write_multi_stream_4(t, h, o, (u_int8_t *)(a), c/4)

#define BUS_SPACE_ALIGNED_POINTER(p, t) ALIGNED_POINTER(p, t)

/*
 * Flags used in various bus DMA methods.
 */
#define	BUS_DMA_WAITOK		0x000	/* safe to sleep (pseudo-flag) */
#define	BUS_DMA_NOWAIT		0x001	/* not safe to sleep */
#define	BUS_DMA_ALLOCNOW	0x002	/* perform resource allocation now */
#define	BUS_DMA_COHERENT	0x004	/* hint: map memory DMA coherent */
#define	BUS_DMA_NOWRITE		0x008	/* I suppose the following two should default on */
#define	BUS_DMA_BUS1		0x010	/* placeholders for bus functions... */
#define	BUS_DMA_BUS2		0x020
#define	BUS_DMA_BUS3		0x040
#define	BUS_DMA_BUS4		0x080
#define	BUS_DMA_STREAMING	0x100	/* hint: sequential, unidirectional */
#define	BUS_DMA_READ		0x200	/* mapping is device -> memory only */
#define	BUS_DMA_WRITE		0x400	/* mapping is memory -> device only */

#define	BUS_DMA_NOCACHE		BUS_DMA_BUS1
#define	BUS_DMA_DVMA		BUS_DMA_BUS2	/* Don't bother with alignment */

/* Forwards needed by prototypes below. */
struct mbuf;
struct uio;

/*
 * Operations performed by bus_dmamap_sync().
 */
#define	BUS_DMASYNC_PREREAD	0x01	/* pre-read synchronization */
#define	BUS_DMASYNC_POSTREAD	0x02	/* post-read synchronization */
#define	BUS_DMASYNC_PREWRITE	0x04	/* pre-write synchronization */
#define	BUS_DMASYNC_POSTWRITE	0x08	/* post-write synchronization */

#define __HAVE_NEW_BUS_DMAMAP_SYNC

typedef struct sparc_bus_dma_tag	*bus_dma_tag_t;
typedef struct sparc_bus_dmamap		*bus_dmamap_t;

/*
 *	bus_dma_segment_t
 *
 *	Describes a single contiguous DMA transaction.  Values
 *	are suitable for programming into DMA registers.
 */
struct sparc_bus_dma_segment {
	bus_addr_t	ds_addr;	/* DVMA address */
	bus_size_t	ds_len;		/* length of transfer */
	bus_size_t	_ds_boundary;	/* don't cross this */
	bus_size_t	_ds_align;	/* align to this */
	void		*_ds_mlist;	/* XXX - dmamap_alloc'ed pages */
};
typedef struct sparc_bus_dma_segment	bus_dma_segment_t;


/*
 *	bus_dma_tag_t
 *
 *	A machine-dependent opaque type describing the implementation of
 *	DMA for a given bus.
 */
struct sparc_bus_dma_tag {
	void	*_cookie;		/* cookie used in the guts */
	struct sparc_bus_dma_tag* _parent;

	/*
	 * DMA mapping methods.
	 */
	int	(*_dmamap_create)(bus_dma_tag_t, bus_size_t, int,
		    bus_size_t, bus_size_t, int, bus_dmamap_t *);
	void	(*_dmamap_destroy)(bus_dma_tag_t, bus_dmamap_t);
	int	(*_dmamap_load)(bus_dma_tag_t, bus_dmamap_t, void *,
		    bus_size_t, struct proc *, int);
	int	(*_dmamap_load_mbuf)(bus_dma_tag_t, bus_dmamap_t,
		    struct mbuf *, int);
	int	(*_dmamap_load_uio)(bus_dma_tag_t, bus_dmamap_t,
		    struct uio *, int);
	int	(*_dmamap_load_raw)(bus_dma_tag_t, bus_dmamap_t,
		    bus_dma_segment_t *, int, bus_size_t, int);
	void	(*_dmamap_unload)(bus_dma_tag_t, bus_dmamap_t);
	void	(*_dmamap_sync)(bus_dma_tag_t, bus_dmamap_t,
		    bus_addr_t, bus_size_t, int);

	/*
	 * DMA memory utility functions.
	 */
	int	(*_dmamem_alloc)(bus_dma_tag_t, bus_size_t, bus_size_t,
		    bus_size_t, bus_dma_segment_t *, int, int *, int);
	void	(*_dmamem_free)(bus_dma_tag_t,
		    bus_dma_segment_t *, int);
	int	(*_dmamem_map)(bus_dma_tag_t, bus_dma_segment_t *,
		    int, size_t, caddr_t *, int);
	void	(*_dmamem_unmap)(bus_dma_tag_t, caddr_t, size_t);
	paddr_t	(*_dmamem_mmap)(bus_dma_tag_t, bus_dma_segment_t *,
		    int, off_t, int, int);
};

#define	bus_dmamap_create(t, s, n, m, b, f, p)			\
	(*(t)->_dmamap_create)((t), (s), (n), (m), (b), (f), (p))
#define	bus_dmamap_destroy(t, p)				\
	(*(t)->_dmamap_destroy)((t), (p))
#define	bus_dmamap_load(t, m, b, s, p, f)			\
	(*(t)->_dmamap_load)((t), (m), (b), (s), (p), (f))
#define	bus_dmamap_load_mbuf(t, m, b, f)			\
	(*(t)->_dmamap_load_mbuf)((t), (m), (b), (f))
#define	bus_dmamap_load_uio(t, m, u, f)				\
	(*(t)->_dmamap_load_uio)((t), (m), (u), (f))
#define	bus_dmamap_load_raw(t, m, sg, n, s, f)			\
	(*(t)->_dmamap_load_raw)((t), (m), (sg), (n), (s), (f))
#define	bus_dmamap_unload(t, p)					\
	(*(t)->_dmamap_unload)((t), (p))
#define	bus_dmamap_sync(t, p, o, l, ops)			\
	(void)((t)->_dmamap_sync ?				\
	    (*(t)->_dmamap_sync)((t), (p), (o), (l), (ops)) : (void)0)

#define	bus_dmamem_alloc(t, s, a, b, sg, n, r, f)		\
	(*(t)->_dmamem_alloc)((t), (s), (a), (b), (sg), (n), (r), (f))
#define	bus_dmamem_free(t, sg, n)				\
	(*(t)->_dmamem_free)((t), (sg), (n))
#define	bus_dmamem_map(t, sg, n, s, k, f)			\
	(*(t)->_dmamem_map)((t), (sg), (n), (s), (k), (f))
#define	bus_dmamem_unmap(t, k, s)				\
	(*(t)->_dmamem_unmap)((t), (k), (s))
#define	bus_dmamem_mmap(t, sg, n, o, p, f)			\
	(*(t)->_dmamem_mmap)((t), (sg), (n), (o), (p), (f))

/*
 *	bus_dmamap_t
 *
 *	Describes a DMA mapping.
 */
struct sparc_bus_dmamap {
	/*
	 * PRIVATE MEMBERS: not for use by machine-independent code.
	 */
	bus_addr_t	_dm_dvmastart;	/* start and size of allocated */
	bus_size_t	_dm_dvmasize;	/* DVMA segment for this map */

	bus_size_t	_dm_size;	/* largest DMA transfer mappable */
	bus_size_t	_dm_maxsegsz;	/* largest possible segment */
	bus_size_t	_dm_boundary;	/* don't cross this */
	int		_dm_segcnt;	/* number of segs this map can map */
	int		_dm_flags;	/* misc. flags */
#define _DM_TYPE_LOAD	0
#define _DM_TYPE_SEGS	1
#define _DM_TYPE_UIO	2
#define _DM_TYPE_MBUF	3
	int		_dm_type;	/* type of mapping: raw, uio, mbuf, etc */
	void		*_dm_source;	/* source mbuf, uio, etc. needed for unload */

	void		*_dm_cookie;	/* cookie for bus-specific functions */

	/*
	 * PUBLIC MEMBERS: these are used by machine-independent code.
	 */
	bus_size_t	dm_mapsize;	/* size of the mapping */
	int		dm_nsegs;	/* # valid segments in mapping */
	bus_dma_segment_t dm_segs[1];	/* segments; variable length */
};

#ifdef _SPARC_BUS_DMA_PRIVATE
int	_bus_dmamap_create(bus_dma_tag_t, bus_size_t, int, bus_size_t,
	    bus_size_t, int, bus_dmamap_t *);
void	_bus_dmamap_destroy(bus_dma_tag_t, bus_dmamap_t);
int	_bus_dmamap_load(bus_dma_tag_t, bus_dmamap_t, void *,
	    bus_size_t, struct proc *, int);
int	_bus_dmamap_load_mbuf(bus_dma_tag_t, bus_dmamap_t,
	    struct mbuf *, int);
int	_bus_dmamap_load_uio(bus_dma_tag_t, bus_dmamap_t,
	    struct uio *, int);
int	_bus_dmamap_load_raw(bus_dma_tag_t, bus_dmamap_t,
	    bus_dma_segment_t *, int, bus_size_t, int);
void	_bus_dmamap_unload(bus_dma_tag_t, bus_dmamap_t);
void	_bus_dmamap_sync(bus_dma_tag_t, bus_dmamap_t, bus_addr_t,
	    bus_size_t, int);

int	_bus_dmamem_alloc(bus_dma_tag_t tag, bus_size_t size,
	    bus_size_t alignment, bus_size_t boundary,
	    bus_dma_segment_t *segs, int nsegs, int *rsegs, int flags);
void	_bus_dmamem_free(bus_dma_tag_t tag, bus_dma_segment_t *segs,
	    int nsegs);
int	_bus_dmamem_map(bus_dma_tag_t tag, bus_dma_segment_t *segs,
	    int nsegs, size_t size, caddr_t *kvap, int flags);
void	_bus_dmamem_unmap(bus_dma_tag_t tag, caddr_t kva,
	    size_t size);
paddr_t	_bus_dmamem_mmap(bus_dma_tag_t tag, bus_dma_segment_t *segs,
	    int nsegs, off_t off, int prot, int flags);

int	_bus_dmamem_alloc_range(bus_dma_tag_t tag, bus_size_t size,
	    bus_size_t alignment, bus_size_t boundary,
	    bus_dma_segment_t *segs, int nsegs, int *rsegs, int flags,
	    vaddr_t low, vaddr_t high);
#endif /* _SPARC_BUS_DMA_PRIVATE */

#endif /* _SPARC_BUS_H_ */
