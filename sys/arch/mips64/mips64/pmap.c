/*	$OpenBSD: pmap.c,v 1.46 2010/01/05 06:44:58 syuu Exp $	*/

/*
 * Copyright (c) 2001-2004 Opsycon AB  (www.opsycon.se / www.opsycon.com)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS
 * OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	XXX This code needs some major rewriting.
 */

#include <sys/param.h>
#include <sys/systm.h>
#include <sys/proc.h>
#include <sys/malloc.h>
#include <sys/user.h>
#include <sys/buf.h>
#include <sys/pool.h>
#ifdef SYSVSHM
#include <sys/shm.h>
#endif

#include <machine/cpu.h>
#include <machine/autoconf.h>
#include <machine/vmparam.h>
#include <mips64/archtype.h>

#include <uvm/uvm.h>

extern void mem_zero_page(vaddr_t);

struct pool pmap_pmap_pool;
struct pool pmap_pv_pool;
struct pool pmap_pg_pool;

#define pmap_pv_alloc()		(pv_entry_t)pool_get(&pmap_pv_pool, PR_NOWAIT)
#define pmap_pv_free(pv)	pool_put(&pmap_pv_pool, (pv))

#ifndef PMAP_PV_LOWAT
#define PMAP_PV_LOWAT   16
#endif
int	pmap_pv_lowat = PMAP_PV_LOWAT;

uint	 pmap_alloc_tlbpid(struct proc *);
int	 pmap_enter_pv(pmap_t, vaddr_t, vm_page_t, pt_entry_t *);
void	 pmap_page_cache(vm_page_t, int);
void	 pmap_remove_pv(pmap_t, vaddr_t, paddr_t);
void	*pmap_pg_alloc(struct pool *, int, int *);
void	 pmap_pg_free(struct pool *, void *);

struct pool_allocator pmap_pg_allocator = {
	pmap_pg_alloc, pmap_pg_free
};

static void pmap_invalidate_kernel_page(vaddr_t);
static void pmap_invalidate_user_page(pmap_t, vaddr_t);
#ifdef MULTIPROCESSOR
static void pmap_invalidate_kernel_page_action(void *);
static void pmap_invalidate_user_page_action(void *);
static void pmap_update_kernel_page_action(void *);
static void pmap_update_user_page_action(void *);
#endif

#ifdef PMAPDEBUG
struct {
	int kernel;	/* entering kernel mapping */
	int user;	/* entering user mapping */
	int ptpneeded;	/* needed to allocate a PT page */
	int pwchange;	/* no mapping change, just wiring or protection */
	int wchange;	/* no mapping change, just wiring */
	int mchange;	/* was mapped but mapping to different page */
	int managed;	/* a managed page */
	int firstpv;	/* first mapping for this PA */
	int secondpv;	/* second mapping for this PA */
	int ci;		/* cache inhibited */
	int unmanaged;	/* not a managed page */
	int flushes;	/* cache flushes */
	int cachehit;	/* new entry forced valid entry out */
} enter_stats;
struct {
	int calls;
	int removes;
	int flushes;
	int pidflushes;	/* HW pid stolen */
	int pvfirst;
	int pvsearch;
} remove_stats;

#define PDB_FOLLOW	0x0001
#define PDB_INIT	0x0002
#define PDB_ENTER	0x0004
#define PDB_REMOVE	0x0008
#define PDB_CREATE	0x0010
#define PDB_PTPAGE	0x0020
#define PDB_PVENTRY	0x0040
#define PDB_BITS	0x0080
#define PDB_COLLECT	0x0100
#define PDB_PROTECT	0x0200
#define PDB_TLBPID	0x0400
#define PDB_PARANOIA	0x2000
#define PDB_WIRING	0x4000
#define PDB_PVDUMP	0x8000

#define DPRINTF(flag, printdata)	\
	if (pmapdebug & (flag)) 	\
		printf printdata;

#define stat_count(what)	(what)++
int pmapdebug = PDB_ENTER|PDB_FOLLOW;

#else

#define DPRINTF(flag, printdata)
#define stat_count(what)

#endif	/* PMAPDEBUG */

static struct pmap	kernel_pmap_store
	[(PMAP_SIZEOF(MAXCPUS) + sizeof(struct pmap) - 1)
		/ sizeof(struct pmap)];
struct pmap *const kernel_pmap_ptr = kernel_pmap_store;

psize_t	mem_size;	/* memory size in bytes */
vaddr_t	virtual_start;  /* VA of first avail page (after kernel bss)*/
vaddr_t	virtual_end;	/* VA of last avail page (end of kernel AS) */

static struct pmap_asid_info pmap_asid_info[MAXCPUS];

pt_entry_t	*Sysmap;		/* kernel pte table */
u_int		Sysmapsize;		/* number of pte's in Sysmap */
const vaddr_t	Sysmapbase = VM_MIN_KERNEL_ADDRESS;	/* for libkvm */


#ifdef MULTIPROCESSOR

struct pmap_invalidate_page_arg {
	pmap_t pmap;
	vaddr_t va;
};

static void
pmap_invalidate_kernel_page(vaddr_t va)
{
	struct pmap_invalidate_page_arg arg;
	unsigned int cpumask = 0;
	struct cpu_info *ci;
	CPU_INFO_ITERATOR cii;

	CPU_INFO_FOREACH(cii, ci) 
		if (cpuset_isset(&cpus_running, ci))
			cpumask |= 1 << ci->ci_cpuid;
	arg.va = va;

	smp_rendezvous_cpus(cpumask, pmap_invalidate_kernel_page_action, &arg);
}

static void
pmap_invalidate_kernel_page_action(void *arg)
{
	vaddr_t va = ((struct pmap_invalidate_page_arg *)arg)->va;

	tlb_flush_addr(va);
}

static void
pmap_invalidate_user_page(pmap_t pmap, vaddr_t va)
{
	struct pmap_invalidate_page_arg arg;
	unsigned int cpumask = 0;
	struct cpu_info *ci;
	CPU_INFO_ITERATOR cii;

	CPU_INFO_FOREACH(cii, ci) 
		if (cpuset_isset(&cpus_running, ci)) {
			unsigned int i = ci->ci_cpuid;
			unsigned int m = 1 << i;
			if (pmap->pm_asid[i].pma_asidgen !=
			    pmap_asid_info[i].pma_asidgen)
				continue;
			else if (ci->ci_curpmap != pmap) {
				pmap->pm_asid[i].pma_asidgen = 0;
				continue;
			}
			cpumask |= m;
		}

	arg.pmap = pmap;
	arg.va = va;

	smp_rendezvous_cpus(cpumask, pmap_invalidate_user_page_action, &arg);
}

static void
pmap_invalidate_user_page_action(void *arg)
{
	pmap_t pmap = ((struct pmap_invalidate_page_arg *)arg)->pmap;
	vaddr_t va = ((struct pmap_invalidate_page_arg *)arg)->va;
	unsigned int cpuid = cpu_number();
	u_long asid;

	asid = pmap->pm_asid[cpuid].pma_asid << VMTLB_PID_SHIFT;
	tlb_flush_addr(va | asid);
}

struct pmap_update_page_arg {
	pmap_t pmap;
	vaddr_t va;
	pt_entry_t entry;
};

void
pmap_update_kernel_page(vaddr_t va, pt_entry_t entry)
{
	struct pmap_update_page_arg arg;
	unsigned long cpumask = 0;
	struct cpu_info *ci;
	CPU_INFO_ITERATOR cii;

	CPU_INFO_FOREACH(cii, ci) 
		if (cpuset_isset(&cpus_running, ci))
			cpumask |= 1 << ci->ci_cpuid;

	arg.va = va;
	arg.entry = entry;
	smp_rendezvous_cpus(cpumask,
			    pmap_update_kernel_page_action, &arg);
}

static void
pmap_update_kernel_page_action(void *arg)
{
	vaddr_t va = ((struct pmap_update_page_arg *)arg)->va;
	pt_entry_t entry = ((struct pmap_update_page_arg *)arg)->entry;

	tlb_update(va, entry);
}

void
pmap_update_user_page(pmap_t pmap, vaddr_t va, pt_entry_t entry)
{
	struct pmap_update_page_arg arg;
	unsigned long cpumask = 0;
	struct cpu_info *ci;
	CPU_INFO_ITERATOR cii;

	CPU_INFO_FOREACH(cii, ci) 
		if (cpuset_isset(&cpus_running, ci)) {
			unsigned int i = ci->ci_cpuid;
			unsigned int m = 1 << i;
      		if (pmap->pm_asid[i].pma_asidgen != 
			    pmap_asid_info[i].pma_asidgen)
				continue;
			else if (ci->ci_curpmap != pmap) {
				pmap->pm_asid[i].pma_asidgen = 0;
				continue;
			}
			cpumask |= m;
		}

	arg.pmap = pmap;
	arg.va = va;
	arg.entry = entry;
	smp_rendezvous_cpus(cpumask,
			    pmap_update_user_page_action, &arg);
}

static void
pmap_update_user_page_action(void *arg)
{
	pmap_t pmap = ((struct pmap_update_page_arg *)arg)->pmap;
	vaddr_t va = ((struct pmap_update_page_arg *)arg)->va;
	pt_entry_t entry = ((struct pmap_update_page_arg *)arg)->entry;
	unsigned int cpuid = cpu_number();
	u_long asid;

	asid = pmap->pm_asid[cpuid].pma_asid << VMTLB_PID_SHIFT;
	tlb_update(va | asid, entry);
}
#else
static void
pmap_invalidate_kernel_page(vaddr_t va)
{
	tlb_flush_addr(va);
}

static void
pmap_invalidate_user_page(pmap_t pmap, vaddr_t va)
{
	u_long cpuid = cpu_number();
	u_long asid = pmap->pm_asid[cpuid].pma_asid << VMTLB_PID_SHIFT;

	if (pmap->pm_asid[cpuid].pma_asidgen ==
	    pmap_asid_info[cpuid].pma_asidgen)
		tlb_flush_addr(va | asid);
}

void
pmap_update_kernel_page(vaddr_t va, pt_entry_t entry)
{
	tlb_update(va, entry);
}

void
pmap_update_user_page(pmap_t pmap, vaddr_t va, pt_entry_t entry)
{
	u_long cpuid = cpu_number();
	u_long asid = pmap->pm_asid[cpuid].pma_asid << VMTLB_PID_SHIFT;

	if (pmap->pm_asid[cpuid].pma_asidgen ==
	    pmap_asid_info[cpuid].pma_asidgen)
		tlb_update(va | asid, entry);
}
#endif

/*
 *	Bootstrap the system enough to run with virtual memory.
 */
void
pmap_bootstrap(void)
{
	u_int i;
	pt_entry_t *spte;

	/*
	 * Create a mapping table for kernel virtual memory. This
	 * table is a linear table in contrast to the user process
	 * mapping tables which are built with segment/page tables.
	 * Create 1GB of map (this will only use 1MB of memory).
	 */
	virtual_start = VM_MIN_KERNEL_ADDRESS;
	virtual_end = VM_MAX_KERNEL_ADDRESS;

	Sysmapsize = (VM_MAX_KERNEL_ADDRESS - VM_MIN_KERNEL_ADDRESS) /
	    PAGE_SIZE;
	if (Sysmapsize & 1)
		Sysmapsize++;	/* force even number of pages */

	Sysmap = (pt_entry_t *)
	    uvm_pageboot_alloc(sizeof(pt_entry_t) * Sysmapsize);

	pool_init(&pmap_pmap_pool, PMAP_SIZEOF(ncpusfound), 0, 0, 0,"pmappl", NULL);
	pool_init(&pmap_pv_pool, sizeof(struct pv_entry), 0, 0, 0,"pvpl", NULL);
	pool_init(&pmap_pg_pool, PMAP_L2SIZE, PMAP_L2SIZE, 0, 0, "pmappgpl",
	    &pmap_pg_allocator);

	simple_lock_init(&pmap_kernel()->pm_lock);
	pmap_kernel()->pm_count = 1;

	/*
	 * The 64 bit Mips architecture stores the AND result
	 * of the Global bits in the pte pair in the on chip
	 * translation lookaside buffer. Thus invalid entries
	 * must have the Global bit set so when Entry LO and
	 * Entry HI G bits are ANDed together they will produce
	 * a global bit to store in the tlb.
	 */
	for (i = Sysmapsize, spte = Sysmap; i != 0; i--, spte++)
		*spte = PG_G;

	for (i = 0; i < MAXCPUS; i++) {
		pmap_asid_info[i].pma_asidgen = 1;
		pmap_asid_info[i].pma_asid = 2;
	}
}

/*
 *  Page steal allocator used during bootup.
 */
vaddr_t
pmap_steal_memory(vsize_t size, vaddr_t *vstartp, vaddr_t *vendp)
{
	int i, j;
	uint npg;
	vaddr_t va;
	paddr_t pa;

#ifdef DIAGNOSTIC
	if (uvm.page_init_done) {
		panic("pmap_steal_memory: too late, vm is running!");
	}
#endif

	size = round_page(size);
	npg = atop(size);
	va = 0;

	for(i = 0; i < vm_nphysseg && va == 0; i++) {
		if (vm_physmem[i].avail_start != vm_physmem[i].start ||
		    vm_physmem[i].avail_start >= vm_physmem[i].avail_end) {
			continue;
		}

		if ((vm_physmem[i].avail_end - vm_physmem[i].avail_start) < npg)
			continue;

		pa = ptoa(vm_physmem[i].avail_start);
		vm_physmem[i].avail_start += npg;
		vm_physmem[i].start += npg;

		if (vm_physmem[i].avail_start == vm_physmem[i].end) {
			if (vm_nphysseg == 1)
				panic("pmap_steal_memory: out of memory!");

			vm_nphysseg--;
			for (j = i; j < vm_nphysseg; j++)
				vm_physmem[j] = vm_physmem[j + 1];
		}
		if (vstartp)
			*vstartp = round_page(virtual_start);
		if (vendp)
			*vendp = virtual_end;

#ifdef __sgi__
		/*
		 * If we are running with a 32 bit ARCBios (i.e. kernel
		 * linked in CKSEG0), return a CKSEG0 address whenever possible.
		 */
		if (IS_XKPHYS((vaddr_t)&pmap_steal_memory) ||
		    pa + size >= CKSEG_SIZE)
			va = PHYS_TO_XKPHYS(pa, CCA_CACHED);
		else
			va = PHYS_TO_CKSEG0(pa);
#else
			va = PHYS_TO_XKPHYS(pa, CCA_CACHED);
#endif

		bzero((void *)va, size);
		return (va);
	}

	panic("pmap_steal_memory: no memory to steal");
}

/*
 *	Initialize the pmap module.
 *	Called by vm_init, to initialize any structures that the pmap
 *	system needs to map virtual memory.
 */
void
pmap_init()
{

	DPRINTF(PDB_FOLLOW|PDB_INIT, ("pmap_init()\n"));

#if 0 /* too early */
	pool_setlowat(&pmap_pv_pool, pmap_pv_lowat);
#endif
}

static pv_entry_t pg_to_pvh(struct vm_page *);
static __inline pv_entry_t
pg_to_pvh(struct vm_page *pg)
{
	return &pg->mdpage.pv_ent;
}

/*
 *	Create and return a physical map.
 */
pmap_t
pmap_create()
{
	pmap_t pmap;
	int i, s;

extern struct vmspace vmspace0;
extern struct user *proc0paddr;

	DPRINTF(PDB_FOLLOW|PDB_CREATE, ("pmap_create()\n"));

	s = splvm();
	pmap = pool_get(&pmap_pmap_pool, PR_WAITOK | PR_ZERO);
	splx(s);

	simple_lock_init(&pmap->pm_lock);
	pmap->pm_count = 1;

	pmap->pm_segtab = (struct segtab *)pool_get(&pmap_pg_pool,
	    PR_WAITOK | PR_ZERO);

	if (pmap == vmspace0.vm_map.pmap) {
		/*
		 * The initial process has already been allocated a TLBPID
		 * in mach_init().
		 */
		for (i = 0; i < ncpusfound; i++) {
			pmap->pm_asid[i].pma_asid = 1;
			pmap->pm_asid[i].pma_asidgen =
				pmap_asid_info[i].pma_asidgen;
		}
		proc0paddr->u_pcb.pcb_segtab = pmap->pm_segtab;
	} else {
		for (i = 0; i < ncpusfound; i++) {
			pmap->pm_asid[i].pma_asid = 0;
			pmap->pm_asid[i].pma_asidgen = 0;
		}
	}

	return (pmap);
}

/*
 *	Retire the given physical map from service.
 *	Should only be called if the map contains
 *	no valid mappings.
 */
void
pmap_destroy(pmap_t pmap)
{
	int s, count;

	DPRINTF(PDB_FOLLOW|PDB_CREATE, ("pmap_destroy(%x)\n", pmap));

	simple_lock(&pmap->pm_lock);
	count = --pmap->pm_count;
	simple_unlock(&pmap->pm_lock);
	if (count > 0)
		return;

	if (pmap->pm_segtab) {
		pt_entry_t *pte;
		int i;
#ifdef PARANOIA
		int j;
#endif

		for (i = 0; i < PMAP_SEGTABSIZE; i++) {
			/* get pointer to segment map */
			pte = pmap->pm_segtab->seg_tab[i];
			if (!pte)
				continue;
#ifdef PARANOIA
			for (j = 0; j < NPTEPG; j++) {
				if (pte[j] != PG_NV)
					panic("pmap_destroy: segmap not empty");
			}
#endif
			pool_put(&pmap_pg_pool, pte);
#ifdef PARANOIA
			pmap->pm_segtab->seg_tab[i] = NULL;
#endif
		}
		pool_put(&pmap_pg_pool, pmap->pm_segtab);
#ifdef PARANOIA
		pmap->pm_segtab = NULL;
#endif
	}

	s = splvm();
	pool_put(&pmap_pmap_pool, pmap);
	splx(s);
}

/*
 *	Add a reference to the specified pmap.
 */
void
pmap_reference(pmap_t pmap)
{

	DPRINTF(PDB_FOLLOW, ("pmap_reference(%x)\n", pmap));

	if (pmap) {
		simple_lock(&pmap->pm_lock);
		pmap->pm_count++;
		simple_unlock(&pmap->pm_lock);
	}
}

/*
 *      Make a new pmap (vmspace) active for the given process.
 */
void
pmap_activate(struct proc *p)
{
	pmap_t pmap = p->p_vmspace->vm_map.pmap;
	struct cpu_info *ci = curcpu();

	ci->ci_curpmap = pmap;
	p->p_addr->u_pcb.pcb_segtab = pmap->pm_segtab;
	pmap_alloc_tlbpid(p);
}

/*
 *      Make a previously active pmap (vmspace) inactive.
 */
void
pmap_deactivate(struct proc *p)
{
	struct cpu_info *ci = curcpu();

	ci->ci_curpmap = NULL;
}

/*
 *	Remove the given range of addresses from the specified map.
 *
 *	It is assumed that the start and end are properly
 *	rounded to the page size.
 */
void
pmap_remove(pmap_t pmap, vaddr_t sva, vaddr_t eva)
{
	vaddr_t nssva;
	pt_entry_t *pte, entry;

	DPRINTF(PDB_FOLLOW|PDB_REMOVE|PDB_PROTECT,
		("pmap_remove(%x, %x, %x)\n", pmap, sva, eva));

	stat_count(remove_stats.calls);

	if (pmap == NULL)
		return;

	if (pmap == pmap_kernel()) {
		/* remove entries from kernel pmap */
#ifdef DIAGNOSTIC
		if (sva < VM_MIN_KERNEL_ADDRESS ||
		    eva >= VM_MAX_KERNEL_ADDRESS || eva < sva)
			panic("pmap_remove(%p, %p): not in range", sva, eva);
#endif
		pte = kvtopte(sva);
		for(; sva < eva; sva += NBPG, pte++) {
			entry = *pte;
			if (!(entry & PG_V))
				continue;
			pmap->pm_stats.resident_count--;
			pmap_remove_pv(pmap, sva, pfn_to_pad(entry));
			*pte = PG_NV | PG_G;
			/*
			 * Flush the TLB for the given address.
			 */
			pmap_invalidate_kernel_page(sva);
			stat_count(remove_stats.flushes);
		}
		return;
	}

#ifdef DIAGNOSTIC
	if (eva > VM_MAXUSER_ADDRESS)
		panic("pmap_remove: uva not in range");
#endif
	while (sva < eva) {
		nssva = mips_trunc_seg(sva) + NBSEG;
		if (nssva == 0 || nssva > eva)
			nssva = eva;
		/*
		 * If VA belongs to an unallocated segment,
		 * skip to the next segment boundary.
		 */
		if (!(pte = pmap_segmap(pmap, sva))) {
			sva = nssva;
			continue;
		}
		/*
		 * Invalidate every valid mapping within this segment.
		 */
		pte += uvtopte(sva);
		for (; sva < nssva; sva += NBPG, pte++) {
			entry = *pte;
			if (!(entry & PG_V))
				continue;
			pmap->pm_stats.resident_count--;
			pmap_remove_pv(pmap, sva, pfn_to_pad(entry));
			*pte = PG_NV;
			/*
			 * Flush the TLB for the given address.
			 */
			pmap_invalidate_user_page(pmap, sva);
			stat_count(remove_stats.flushes);
		}
	}
}

/*
 *	pmap_page_protect:
 *
 *	Lower the permission for all mappings to a given page.
 */
void
pmap_page_protect(struct vm_page *pg, vm_prot_t prot)
{
	pv_entry_t pv;
	vaddr_t va;
	int s;

	if (prot == VM_PROT_NONE) {
		DPRINTF(PDB_REMOVE, ("pmap_page_protect(%p, %p)\n", pg, prot));
	} else {
		DPRINTF(PDB_FOLLOW|PDB_PROTECT,
			("pmap_page_protect(%p, %p)\n", pg, prot));
	}

	switch (prot) {
	case VM_PROT_READ|VM_PROT_WRITE:
	case VM_PROT_ALL:
		break;

	/* copy_on_write */
	case VM_PROT_READ:
	case VM_PROT_READ|VM_PROT_EXECUTE:
		pv = pg_to_pvh(pg);
		s = splvm();
		/*
		 * Loop over all current mappings setting/clearing as apropos.
		 */
		if (pv->pv_pmap != NULL) {
			for (; pv; pv = pv->pv_next) {
				va = pv->pv_va;
				pmap_protect(pv->pv_pmap, va, va + PAGE_SIZE,
				    prot);
			}
		}
		splx(s);
		break;

	/* remove_all */
	default:
		pv = pg_to_pvh(pg);
		s = splvm();
		while (pv->pv_pmap != NULL) {
			va = pv->pv_va;
			pmap_remove(pv->pv_pmap, va, va + PAGE_SIZE);
		}
		splx(s);
	}
}

/*
 *	Set the physical protection on the
 *	specified range of this map as requested.
 */
void
pmap_protect(pmap_t pmap, vaddr_t sva, vaddr_t eva, vm_prot_t prot)
{
	vaddr_t nssva;
	pt_entry_t *pte, entry;
	u_int p;

	DPRINTF(PDB_FOLLOW|PDB_PROTECT,
		("pmap_protect(%p, %p, %p, %p)\n", pmap, sva, eva, prot));

	if ((prot & VM_PROT_READ) == VM_PROT_NONE) {
		pmap_remove(pmap, sva, eva);
		return;
	}

	p = (prot & VM_PROT_WRITE) ? PG_M : PG_RO;

	if (pmap == pmap_kernel()) {
		/*
		 * Change entries in kernel pmap.
		 * This will trap if the page is writeable (in order to set
		 * the dirty bit) even if the dirty bit is already set. The
		 * optimization isn't worth the effort since this code isn't
		 * executed much. The common case is to make a user page
		 * read-only.
		 */
#ifdef DIAGNOSTIC
		if (sva < VM_MIN_KERNEL_ADDRESS ||
		    eva >= VM_MAX_KERNEL_ADDRESS || eva < sva)
			panic("pmap_protect(%p, %p): not in range", sva, eva);
#endif
		pte = kvtopte(sva);
		for (; sva < eva; sva += NBPG, pte++) {
			entry = *pte;
			if (!(entry & PG_V))
				continue;
			if ((entry & PG_M) != 0 /* && p != PG_M */)
				if ((entry & PG_CACHEMODE) == PG_CACHED)
					Mips_HitSyncDCache(sva,
					    pfn_to_pad(entry), PAGE_SIZE);
			entry = (entry & ~(PG_M | PG_RO)) | p;
			*pte = entry;
			/*
			 * Update the TLB if the given address is in the cache.
			 */
			pmap_update_kernel_page(sva, entry);
		}
		return;
	}

#ifdef DIAGNOSTIC
	if (eva > VM_MAXUSER_ADDRESS)
		panic("pmap_protect: uva not in range");
#endif
	while (sva < eva) {
		nssva = mips_trunc_seg(sva) + NBSEG;
		if (nssva == 0 || nssva > eva)
			nssva = eva;
		/*
		 * If VA belongs to an unallocated segment,
		 * skip to the next segment boundary.
		 */
		if (!(pte = pmap_segmap(pmap, sva))) {
			sva = nssva;
			continue;
		}
		/*
		 * Change protection on every valid mapping within this segment.
		 */
		pte += uvtopte(sva);
		for (; sva < nssva; sva += NBPG, pte++) {
			entry = *pte;
			if (!(entry & PG_V))
				continue;
			if ((entry & PG_M) != 0 /* && p != PG_M */)
				if ((entry & PG_CACHEMODE) == PG_CACHED)
					Mips_SyncDCachePage(sva,
					    pfn_to_pad(entry));
			entry = (entry & ~(PG_M | PG_RO)) | p;
			*pte = entry;
			pmap_update_user_page(pmap, sva, entry);
		}
	}
}

/*
 *	Insert the given physical page (p) at
 *	the specified virtual address (v) in the
 *	target physical map with the protection requested.
 *
 *	NB:  This is the only routine which MAY NOT lazy-evaluate
 *	or lose information.  That is, this routine must actually
 *	insert this page into the given map NOW.
 */
int
pmap_enter(pmap_t pmap, vaddr_t va, paddr_t pa, vm_prot_t prot, int flags)
{
	pt_entry_t *pte, npte;
	vm_page_t pg;
	u_long cpuid = cpu_number();

	DPRINTF(PDB_FOLLOW|PDB_ENTER,
		("pmap_enter(%p, %p, %p, %p, %p)\n", pmap, va, pa, prot, flags));

#ifdef DIAGNOSTIC
	if (pmap == pmap_kernel()) {
		stat_count(enter_stats.kernel);
		if (va < VM_MIN_KERNEL_ADDRESS ||
		    va >= VM_MAX_KERNEL_ADDRESS)
			panic("pmap_enter: kva %p", va);
	} else {
		stat_count(enter_stats.user);
		if (va >= VM_MAXUSER_ADDRESS)
			panic("pmap_enter: uva %p", va);
	}
#endif

	pg = PHYS_TO_VM_PAGE(pa);

	if (pg != NULL) {
		if (!(prot & VM_PROT_WRITE)) {
			npte = PG_ROPAGE;
		} else {
			if (pmap == pmap_kernel()) {
				/*
				 * Don't bother to trap on kernel writes,
				 * just record page as dirty.
				 */
				npte = PG_RWPAGE;
			} else {
				if (pg->pg_flags & PV_ATTR_MOD) {
					npte = PG_RWPAGE;
				} else {
					npte = PG_CWPAGE;
				}
			}
		}

		/* Set page referenced/modified status based on flags */
		if (flags & VM_PROT_WRITE)
			atomic_setbits_int(&pg->pg_flags,
			    PV_ATTR_MOD | PV_ATTR_REF);
		else if (flags & VM_PROT_ALL)
			atomic_setbits_int(&pg->pg_flags, PV_ATTR_REF);

		stat_count(enter_stats.managed);
	} else {
		/*
		 * Assumption: if it is not part of our managed memory
		 * then it must be device memory which may be volatile.
		 */
		stat_count(enter_stats.unmanaged);
		if (prot & VM_PROT_WRITE) {
			npte = PG_IOPAGE & ~PG_G;
		} else {
			npte = (PG_IOPAGE | PG_RO) & ~(PG_G | PG_M);
		}
	}

	if (pmap == pmap_kernel()) {
		if (pg != NULL) {
			if (pmap_enter_pv(pmap, va, pg, &npte) != 0) {
				if (flags & PMAP_CANFAIL)
					return ENOMEM;
				panic("pmap_enter: pmap_enter_pv() failed");
			}
		}

		pte = kvtopte(va);
		npte |= vad_to_pfn(pa) | PG_G;
		if ((*pte & PG_V) == 0) {
			pmap->pm_stats.resident_count++;
		}
		if ((*pte & PG_V) && pa != pfn_to_pad(*pte)) {
			pmap_remove(pmap, va, va + NBPG);
			stat_count(enter_stats.mchange);
		}

		/*
		 * Update the same virtual address entry.
		 */
		*pte = npte;
		pmap_update_kernel_page(va, npte);
		return 0;
	}

	/*
	 *  User space mapping. Do table build.
	 */
	if (!(pte = pmap_segmap(pmap, va))) {
		pt_entry_t *ptepg;
		unsigned int wflags = PR_WAITOK | PR_ZERO;

		if (flags & PMAP_CANFAIL)
			wflags |= PR_LIMITFAIL;
	
		ptepg = (pt_entry_t *)pool_get(&pmap_pg_pool, wflags);
		if (ptepg == NULL)
			return ENOMEM;	/* can only happen if PMAP_CANFAIL */

		pmap_segmap(pmap, va) = pte = ptepg;
	}

	if (pg != NULL) {
		if (pmap_enter_pv(pmap, va, pg, &npte) != 0) {
			if (flags & PMAP_CANFAIL)
				return ENOMEM;
			panic("pmap_enter: pmap_enter_pv() failed");
		}
	}

	pte += uvtopte(va);

	/*
	 * Now validate mapping with desired protection/wiring.
	 * Assume uniform modified and referenced status for all
	 * MIPS pages in a OpenBSD page.
	 */
	npte |= vad_to_pfn(pa);
	if (pmap->pm_asid[cpuid].pma_asidgen == 
	    pmap_asid_info[cpuid].pma_asidgen) {
		DPRINTF(PDB_ENTER, ("pmap_enter: new pte 0x%08x tlbpid %u\n",
			npte, pmap->pm_asid[cpuid].pma_asid));
	} else {
		DPRINTF(PDB_ENTER, ("pmap_enter: new pte 0x%08x\n", npte));
	}

	if ((*pte & PG_V) && pa != pfn_to_pad(*pte)) {
		pmap_remove(pmap, va, va + NBPG);
		stat_count(enter_stats.mchange);
	}

	if ((*pte & PG_V) == 0) {
		pmap->pm_stats.resident_count++;
	}
	*pte = npte;
	pmap_update_user_page(pmap, va, npte);

	/*
	 *  If mapping a memory space address invalidate ICache.
	 */
	if (pg != NULL && (prot & VM_PROT_EXECUTE))
		Mips_InvalidateICache(va, PAGE_SIZE);

	return 0;
}

void
pmap_kenter_pa(vaddr_t va, paddr_t pa, vm_prot_t prot)
{
	pt_entry_t *pte, npte;

	DPRINTF(PDB_FOLLOW|PDB_ENTER,
		("pmap_kenter_pa(%p, %p, 0x%x)\n", va, pa, prot));

#ifdef DIAGNOSTIC
	if (va < VM_MIN_KERNEL_ADDRESS ||
	    va >= VM_MAX_KERNEL_ADDRESS)
		panic("pmap_kenter_pa: kva %p", va);
#endif

	npte = vad_to_pfn(pa) | PG_G;
	if (prot & VM_PROT_WRITE)
		npte |= PG_RWPAGE;
	else
		npte |= PG_ROPAGE;
	pte = kvtopte(va);
	*pte = npte;
	pmap_update_kernel_page(va, npte);
}

/*
 *  Remove a mapping from the kernel map table. When doing this
 *  the cache must be synced for the VA mapped since we mapped
 *  pages behind the back of the VP tracking system. 
 */
void
pmap_kremove(vaddr_t va, vsize_t len)
{
	pt_entry_t *pte, entry;
	vaddr_t eva;

	DPRINTF(PDB_FOLLOW|PDB_REMOVE, ("pmap_kremove(%p, %p)\n", va, len));

	eva = va + len;
#ifdef DIAGNOSTIC
	if (va < VM_MIN_KERNEL_ADDRESS ||
	    eva >= VM_MAX_KERNEL_ADDRESS || eva < va)
		panic("pmap_kremove: va %p len %p", va, len);
#endif
	pte = kvtopte(va);
	for (; va < eva; va += PAGE_SIZE, pte++) {
		entry = *pte;
		if (!(entry & PG_V))
			continue;
		Mips_HitSyncDCache(va, pfn_to_pad(entry), PAGE_SIZE);
		*pte = PG_NV | PG_G;
		pmap_invalidate_kernel_page(va);
	}
}

void
pmap_unwire(pmap_t pmap, vaddr_t va)
{
	/* XXX this pmap does not handle wired mappings yet... */
}

/*
 *	Routine:	pmap_extract
 *	Function:
 *		Extract the physical page address associated
 *		with the given map/virtual_address pair.
 */
boolean_t
pmap_extract(pmap_t pmap, vaddr_t va, paddr_t *pap)
{
	boolean_t rv = TRUE;
	paddr_t pa;
	pt_entry_t *pte;

	if (pmap == pmap_kernel()) {
		if (IS_XKPHYS(va))
			pa = XKPHYS_TO_PHYS(va);
		else if (va >= (vaddr_t)CKSEG0_BASE &&
		    va < (vaddr_t)CKSEG0_BASE + CKSEG_SIZE)
			pa = CKSEG0_TO_PHYS(va);
		else if (va >= (vaddr_t)CKSEG1_BASE &&
		    va < (vaddr_t)CKSEG1_BASE + CKSEG_SIZE)
			pa = CKSEG1_TO_PHYS(va);
		else {
#ifdef DIAGNOSTIC
			if (va < VM_MIN_KERNEL_ADDRESS ||
			    va >= VM_MAX_KERNEL_ADDRESS)
				panic("pmap_extract(%p, %p)", pmap, va);
#endif
			pte = kvtopte(va);
			if (*pte & PG_V)
				pa = pfn_to_pad(*pte) | (va & PAGE_MASK);
			else
				rv = FALSE;
		}
	} else {
		if (!(pte = pmap_segmap(pmap, va)))
			rv = FALSE;
		else {
			pte += uvtopte(va);
			pa = pfn_to_pad(*pte) | (va & PAGE_MASK);
		}
	}
	if (rv != FALSE)
		*pap = pa;

	DPRINTF(PDB_FOLLOW, ("pmap_extract(%p, %p)=%p(%d)", pmap, va, pa, rv));

	return (rv);
}

/*
 * Find first virtual address >= *vap that
 * will not cause cache aliases.
 */
void
pmap_prefer(paddr_t foff, vaddr_t *vap)
{
	if (CpuCacheAliasMask != 0) {
#if 1
		*vap += (foff - *vap) & (CpuCacheAliasMask | PAGE_MASK);
#else
		*vap += (*vap ^ foff) & CpuCacheAliasMask;
#endif
	}
}

/*
 *	Copy the range specified by src_addr/len
 *	from the source map to the range dst_addr/len
 *	in the destination map.
 *
 *	This routine is only advisory and need not do anything.
 */
void
pmap_copy(dst_pmap, src_pmap, dst_addr, len, src_addr)
	pmap_t dst_pmap;
	pmap_t src_pmap;
	vaddr_t dst_addr;
	vsize_t len;
	vaddr_t src_addr;
{

	DPRINTF(PDB_FOLLOW, ("pmap_copy(%p, %p, %p, %p, %p)\n",
	       dst_pmap, src_pmap, dst_addr, len, src_addr));
}

/*
 *	pmap_zero_page zeros the specified (machine independent) page.
 */
void
pmap_zero_page(struct vm_page *pg)
{
	paddr_t phys = VM_PAGE_TO_PHYS(pg);
	vaddr_t va;
	pv_entry_t pv;

	DPRINTF(PDB_FOLLOW, ("pmap_zero_page(%p)\n", phys));

	va = (vaddr_t)PHYS_TO_XKPHYS(phys, CCA_CACHED);
	pv = pg_to_pvh(pg);
	if ((pg->pg_flags & PV_CACHED) &&
	    ((pv->pv_va ^ va) & CpuCacheAliasMask) != 0) {
		Mips_SyncDCachePage(pv->pv_va, phys);
	}
	mem_zero_page(va);
	Mips_HitSyncDCache(va, phys, PAGE_SIZE);
}

/*
 *	pmap_copy_page copies the specified (machine independent) page.
 *
 *	We do the copy phys to phys and need to check if there may be
 *	a virtual coherence problem. If so flush the cache for the
 *	areas before copying, and flush afterwards.
 */
void
pmap_copy_page(struct vm_page *srcpg, struct vm_page *dstpg)
{
	paddr_t src, dst;
	vaddr_t s, d;
	int df = 1;
	int sf = 1;
	pv_entry_t pv;

	src = VM_PAGE_TO_PHYS(srcpg);
	dst = VM_PAGE_TO_PHYS(dstpg);
	s = (vaddr_t)PHYS_TO_XKPHYS(src, CCA_CACHED);
	d = (vaddr_t)PHYS_TO_XKPHYS(dst, CCA_CACHED);

	DPRINTF(PDB_FOLLOW, ("pmap_copy_page(%p, %p)\n", src, dst));

	pv = pg_to_pvh(srcpg);
	if ((srcpg->pg_flags & PV_CACHED) &&
	    (sf = ((pv->pv_va ^ s) & CpuCacheAliasMask) != 0)) {
		Mips_SyncDCachePage(pv->pv_va, src);
	}
	pv = pg_to_pvh(dstpg);
	if ((dstpg->pg_flags & PV_CACHED) &&
	    (df = ((pv->pv_va ^ d) & CpuCacheAliasMask) != 0)) {
		Mips_SyncDCachePage(pv->pv_va, dst);
	}

	memcpy((void *)d, (void *)s, PAGE_SIZE);

	if (sf) {
		Mips_HitSyncDCache(s, src, PAGE_SIZE);
	}
	Mips_HitSyncDCache(d, dst, PAGE_SIZE);
}

/*
 *  Clear the modify bits on the specified physical page.
 *  Also sync the cache so it reflects the new clean state of the page.
 */
boolean_t
pmap_clear_modify(struct vm_page *pg)
{
	pv_entry_t pv;
	pt_entry_t *pte, entry;
	boolean_t rv = FALSE;
	int s;

	DPRINTF(PDB_FOLLOW, ("pmap_clear_modify(%p)\n", VM_PAGE_TO_PHYS(pg)));

	pv = pg_to_pvh(pg);
	s = splvm();
	if (pg->pg_flags & PV_ATTR_MOD) {
		atomic_clearbits_int(&pg->pg_flags, PV_ATTR_MOD);
		rv = TRUE;
	}
	if (pg->pg_flags & PV_CACHED)
		Mips_SyncDCachePage(pv->pv_va, VM_PAGE_TO_PHYS(pg));

	for (; pv != NULL; pv = pv->pv_next) {
		if (pv->pv_pmap == pmap_kernel()) {
#ifdef DIAGNOSTIC
			if (pv->pv_va < VM_MIN_KERNEL_ADDRESS ||
			    pv->pv_va >= VM_MAX_KERNEL_ADDRESS)
				panic("pmap_clear_modify(%p)", pv->pv_va);
#endif
			pte = kvtopte(pv->pv_va);
			entry = *pte;
			if ((entry & PG_V) != 0 && (entry & PG_M) != 0) {
				rv = TRUE;
				entry &= ~PG_M;
				*pte = entry;
				pmap_update_kernel_page(pv->pv_va, entry);
			}
		} else if (pv->pv_pmap != NULL) {
			if ((pte = pmap_segmap(pv->pv_pmap, pv->pv_va)) == NULL)
				continue;
			pte += uvtopte(pv->pv_va);
			entry = *pte;
			if ((entry & PG_V) != 0 && (entry & PG_M) != 0) {
				rv = TRUE;
				entry &= ~PG_M;
				*pte = entry;
				pmap_update_user_page(pv->pv_pmap, pv->pv_va, entry);
			}
		}
	}
	splx(s);

	return rv;
}

void
pmap_set_modify(struct vm_page *pg)
{
	atomic_setbits_int(&pg->pg_flags, PV_ATTR_MOD | PV_ATTR_REF);
}

/*
 *	pmap_clear_reference:
 *
 *	Clear the reference bit on the specified physical page.
 */
boolean_t
pmap_clear_reference(struct vm_page *pg)
{
	boolean_t rv;

	DPRINTF(PDB_FOLLOW, ("pmap_clear_reference(%p)\n", VM_PAGE_TO_PHYS(pg)));

	rv = (pg->pg_flags & PV_ATTR_REF) != 0;
	atomic_clearbits_int(&pg->pg_flags, PV_ATTR_REF);
	return rv;
}

/*
 *	pmap_is_referenced:
 *
 *	Return whether or not the specified physical page is referenced
 *	by any physical maps.
 */
boolean_t
pmap_is_referenced(struct vm_page *pg)
{
	return (pg->pg_flags & PV_ATTR_REF) != 0;
}

/*
 *	pmap_is_modified:
 *
 *	Return whether or not the specified physical page is modified
 *	by any physical maps.
 */
boolean_t
pmap_is_modified(struct vm_page *pg)
{
	return (pg->pg_flags & PV_ATTR_MOD) != 0;
}

/*
 * Miscellaneous support routines not part of the pmap API
 */

/*
 *	Return RO protection of page.
 */
int
pmap_is_page_ro(pmap_t pmap, vaddr_t va, pt_entry_t entry)
{
	return (entry & PG_RO);
}


/*
 *  Walk the PV tree for a physical page and change all its
 *  mappings to cached or uncached.
 */
void
pmap_page_cache(vm_page_t pg, int mode)
{
	pv_entry_t pv;
	pt_entry_t *pte, entry;
	pt_entry_t newmode;
	int s;

	DPRINTF(PDB_FOLLOW|PDB_ENTER, ("pmap_page_uncache(%p)\n", pg));

	newmode = mode & PV_UNCACHED ? PG_UNCACHED : PG_CACHED;
	pv = pg_to_pvh(pg);

	s = splvm();
	for (; pv != NULL; pv = pv->pv_next) {
		if (pv->pv_pmap == pmap_kernel()) {
#ifdef DIAGNOSTIC
			if (pv->pv_va < VM_MIN_KERNEL_ADDRESS ||
			    pv->pv_va >= VM_MAX_KERNEL_ADDRESS)
				panic("pmap_page_cache(%p)", pv->pv_va);
#endif
			pte = kvtopte(pv->pv_va);
			entry = *pte;
			if (entry & PG_V) {
				entry = (entry & ~PG_CACHEMODE) | newmode;
				*pte = entry;
				pmap_update_kernel_page(pv->pv_va, entry);
			}
		} else {
			if ((pte = pmap_segmap(pv->pv_pmap, pv->pv_va))) {
				pte += uvtopte(pv->pv_va);
				entry = *pte;
				if (entry & PG_V) {
					entry = (entry & ~PG_CACHEMODE) | newmode;
					*pte = entry;
					pmap_update_user_page(pv->pv_pmap, pv->pv_va, entry);
				}
			}
		}
	}
	atomic_clearbits_int(&pg->pg_flags, PV_CACHED | PV_UNCACHED);
	atomic_setbits_int(&pg->pg_flags, mode);
	splx(s);
}

/*
 * Allocate a hardware PID and return it.
 * It takes almost as much or more time to search the TLB for a
 * specific PID and flush those entries as it does to flush the entire TLB.
 * Therefore, when we allocate a new PID, we just take the next number. When
 * we run out of numbers, we flush the TLB, increment the generation count
 * and start over. PID zero is reserved for kernel use.
 * This is called only by switch().
 */
uint
pmap_alloc_tlbpid(struct proc *p)
{
	pmap_t pmap;
	uint id;
	u_long cpuid = cpu_number();

	pmap = p->p_vmspace->vm_map.pmap;
	if (pmap->pm_asid[cpuid].pma_asidgen != 
	    pmap_asid_info[cpuid].pma_asidgen) {
		id = pmap_asid_info[cpuid].pma_asid;
		if (id >= VMNUM_PIDS) {
			tlb_flush(sys_config.cpu[0].tlbsize);
			/* reserve tlbpid_gen == 0 to alway mean invalid */
			if (++pmap_asid_info[cpuid].pma_asidgen == 0)
				pmap_asid_info[cpuid].pma_asidgen = 1;
			id = 1;
		}
		pmap_asid_info[cpuid].pma_asid = id + 1;
		pmap->pm_asid[cpuid].pma_asid = id;
		pmap->pm_asid[cpuid].pma_asidgen = 
			pmap_asid_info[cpuid].pma_asidgen;
	} else {
		id = pmap->pm_asid[cpuid].pma_asid;
	}

	if (curproc) {
		DPRINTF(PDB_FOLLOW|PDB_TLBPID, 
			("pmap_alloc_tlbpid: curproc %d '%s' ",
				curproc->p_pid, curproc->p_comm));
	} else {
		DPRINTF(PDB_FOLLOW|PDB_TLBPID, 
			("pmap_alloc_tlbpid: curproc <none> "));
	}
	DPRINTF(PDB_FOLLOW|PDB_TLBPID, ("segtab %p tlbpid %u pid %d '%s'\n",
			pmap->pm_segtab, id, p->p_pid, p->p_comm));

	return (id);
}

/*
 * Enter the pmap and virtual address into the physical to virtual map table.
 */
int
pmap_enter_pv(pmap_t pmap, vaddr_t va, vm_page_t pg, pt_entry_t *npte)
{
	pv_entry_t pv, npv;
	int s;

	pv = pg_to_pvh(pg);

	s = splvm();
	if (pv->pv_pmap == NULL) {
		/*
		 * No entries yet, use header as the first entry
		 */

		DPRINTF(PDB_PVENTRY,
			("pmap_enter: first pv: pmap %p va %p pa %p\n",
				pmap, va, VM_PAGE_TO_PHYS(pg)));

		stat_count(enter_stats.firstpv);

		pv->pv_va = va;
		atomic_setbits_int(&pg->pg_flags, PV_CACHED);
		pv->pv_pmap = pmap;
		pv->pv_next = NULL;
	} else {
		if (pg->pg_flags & PV_UNCACHED) {
			/*
			 * If page is mapped uncached it's either because
			 * an uncached mapping was requested or we have a
			 * VAC situation. Map this page uncached as well.
			 */
			*npte = (*npte & ~PG_CACHEMODE) | PG_UNCACHED;
		} else if (CpuCacheAliasMask != 0) {
			/*
			 * We have a VAC possibility.  Check if virtual
			 * address of current mappings are compatible
			 * with this new mapping. Only need to check first
			 * since all others have been checked compatible
			 * when added. If they are incompatible, remove
			 * all mappings, flush the cache and set page
			 * to be mapped uncached.
			 */
			if (((pv->pv_va ^ va) & CpuCacheAliasMask) != 0) {
#ifdef PMAP_DEBUG
				printf("pmap_enter: VAC for pa %p, %p !=  %p\n",
				    VM_PAGE_TO_PHYS(pg), npv->pv_va, va);
#endif
				pmap_page_cache(pg, PV_UNCACHED);
				Mips_SyncDCachePage(pv->pv_va,
				    VM_PAGE_TO_PHYS(pg));
				*npte = (*npte & ~PG_CACHEMODE) | PG_UNCACHED;
			}
		}

		/*
		 * There is at least one other VA mapping this page.
		 * Place this entry after the header.
		 *
		 * Note: the entry may already be in the table if
		 * we are only changing the protection bits.
		 */
		for (npv = pv; npv; npv = npv->pv_next) {
			if (pmap == npv->pv_pmap && va == npv->pv_va) {
				return 0;
			}
		}

		DPRINTF(PDB_PVENTRY,
			("pmap_enter: new pv: pmap %x va %x pg %p\n",
			    pmap, va, VM_PAGE_TO_PHYS(pg)));

		npv = pmap_pv_alloc();
		if (npv == NULL) {
			splx(s);
			return ENOMEM;
		}
		npv->pv_va = va;
		npv->pv_pmap = pmap;
		npv->pv_next = pv->pv_next;
		pv->pv_next = npv;

		if (!npv->pv_next)
			stat_count(enter_stats.secondpv);
	}

	splx(s);
	return 0;
}

/*
 * Remove a physical to virtual address translation from the PV table.
 */
void
pmap_remove_pv(pmap_t pmap, vaddr_t va, paddr_t pa)
{
	pv_entry_t pv, npv;
	vm_page_t pg;
	int s;

	DPRINTF(PDB_FOLLOW|PDB_PVENTRY,
		("pmap_remove_pv(%p, %p, %p)\n", pmap, va, pa));

	/*
	 * Remove page from the PV table
	 */
	pg = PHYS_TO_VM_PAGE(pa);
	if (pg == NULL)
		return;

	pv = pg_to_pvh(pg);
	s = splvm();
	/*
	 * If we are removing the first entry on the list, copy up
	 * the next entry, if any, and free that pv item since the
	 * first root item can't be freed. Else walk the list.
	 */
	if (pmap == pv->pv_pmap && va == pv->pv_va) {
		if (pg->pg_flags & PV_CACHED)
			Mips_SyncDCachePage(va, pa);
		npv = pv->pv_next;
		if (npv) {
			*pv = *npv;
			pmap_pv_free(npv);
		} else {
			pv->pv_pmap = NULL;
			atomic_clearbits_int(&pg->pg_flags,
			    (PG_PMAP0 | PG_PMAP1 | PG_PMAP2 | PG_PMAP3) &
			    ~PV_PRESERVE);
		}
		stat_count(remove_stats.pvfirst);
	} else {
		for (npv = pv->pv_next; npv; pv = npv, npv = npv->pv_next) {
			stat_count(remove_stats.pvsearch);
			if (pmap == npv->pv_pmap && va == npv->pv_va)
				break;
		}
		if (npv != NULL) {
			if (pg->pg_flags & PV_CACHED)
				Mips_SyncDCachePage(va, pa);
			pv->pv_next = npv->pv_next;
			pmap_pv_free(npv);
		} else {
#ifdef DIAGNOSTIC
			panic("pmap_remove_pv(%x, %x, %x) not found",
			    pmap, va, pa);
#endif
		}
	}
	splx(s);
}

/*
 * Allocator for smaller-than-a-page structures pool (pm_segtab, and
 * second level page tables).  Pages backing this poll are mapped in
 * XKPHYS to avoid additional page faults when servicing a TLB miss.
 */

void *
pmap_pg_alloc(struct pool *pp, int flags, int *slowdown)
{
	vm_page_t pg;

	*slowdown = 0;
	for (;;) {
		pg = uvm_pagealloc(NULL, 0, NULL,
		    UVM_PGA_USERESERVE | UVM_PGA_ZERO);
		if (pg != NULL)
			break;

		*slowdown = 1;
		if (flags & PR_WAITOK)
			uvm_wait(__func__);
		else
			break;
	}

	if (pg != NULL)
		return (void *)PHYS_TO_XKPHYS(VM_PAGE_TO_PHYS(pg), CCA_CACHED);
	else
		return NULL;
}

void
pmap_pg_free(struct pool *pp, void *item)
{
	vaddr_t va = (vaddr_t)item;
	paddr_t pa = XKPHYS_TO_PHYS(va);
	vm_page_t pg = PHYS_TO_VM_PAGE(pa);

	Mips_HitInvalidateDCache(va, pa, PAGE_SIZE);
	uvm_pagefree(pg);
}
