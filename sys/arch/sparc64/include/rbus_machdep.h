/*	$OpenBSD: rbus_machdep.h,v 1.3 2009/07/21 21:20:05 miod Exp $	*/

/*
 * Copyright (c) 2007 Mark Kettenis
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _SPARC64_RBUS_MACHDEP_H_
#define _SPARC64_RBUS_MACHDEP_H_

struct pci_attach_args;

rbus_tag_t rbus_pccbb_parent_io(struct device *, struct pci_attach_args *);
rbus_tag_t rbus_pccbb_parent_mem(struct device *, struct pci_attach_args *);

#define md_space_map(rbt, addr, size, flags, hp) \
	bus_space_map((rbt)->rb_bt, (addr), (size), (flags), (hp))
#define md_space_unmap(rbt, h, size, addrp) \
	do { \
		*addrp = (rbt)->rb_bt->sparc_bus_addr((rbt)->rb_bt, \
		    (rbt)->rb_bt, (h)); \
		bus_space_unmap((rbt)->rb_bt, (h), (size)); \
	} while (0)

void pccbb_attach_hook(struct device *, struct device *,
	struct pci_attach_args *);

#endif /* _SPARC64_RBUS_MACHDEP_H_ */