/*	$OpenBSD: m8820x.h,v 1.3.2.1 2002/01/31 22:55:18 niklas Exp $ */
/* 
 * Mach Operating System
 * Copyright (c) 1993-1992 Carnegie Mellon University
 * All Rights Reserved.
 * 
 * Permission to use, copy, modify and distribute this software and its
 * documentation is hereby granted, provided that both the copyright
 * notice and this permission notice appear in all copies of the
 * software, derivative works or modified versions, and any portions
 * thereof, and that both notices appear in supporting documentation.
 * 
 * CARNEGIE MELLON ALLOWS FREE USE OF THIS SOFTWARE IN ITS "AS IS"
 * CONDITION.  CARNEGIE MELLON DISCLAIMS ANY LIABILITY OF ANY KIND FOR
 * ANY DAMAGES WHATSOEVER RESULTING FROM THE USE OF THIS SOFTWARE.
 * 
 * Carnegie Mellon requests users of this software to return to
 * 
 *  Software Distribution Coordinator  or  Software.Distribution@CS.CMU.EDU
 *  School of Computer Science
 *  Carnegie Mellon University
 *  Pittsburgh PA 15213-3890
 * 
 * any improvements or extensions that they make and grant Carnegie Mellon 
 * the rights to redistribute these changes.
 */
/*
 * HISTORY
 * 
 */


#ifndef	__MACHINE_M8820X_H__
#define	__MACHINE_M8820X_H__

/*
 *	88200 CMMU definitions
 */
#define CMMU_IDR	0x000	/* CMMU id register */
#define CMMU_SCR	0x004	/* system command register */
#define CMMU_SSR	0x008	/* system status register */
#define CMMU_SAR	0x00C	/* system address register */
#define CMMU_SCTR	0x104	/* system control register */
#define CMMU_PFSR	0x108	/* P bus fault status register */
#define CMMU_PFAR	0x10C	/* P bus fault address register */
#define CMMU_SAPR	0x200	/* supervisor area pointer register */
#define CMMU_UAPR	0x204	/* user area pointer register */
#define CMMU_BWP0	0x400	/* block ATC writer port 0 */
#define CMMU_BWP1	0x404	/* block ATC writer port 1 */
#define CMMU_BWP2	0x408	/* block ATC writer port 2 */
#define CMMU_BWP3	0x40C	/* block ATC writer port 3 */
#define CMMU_BWP4	0x410	/* block ATC writer port 4 */
#define CMMU_BWP5	0x414	/* block ATC writer port 5 */
#define CMMU_BWP6	0x418	/* block ATC writer port 6 */
#define CMMU_BWP7	0x41C	/* block ATC writer port 7 */
#define CMMU_CDP0	0x800	/* cache data port 0 */
#define CMMU_CDP1	0x804	/* cache data port 1 */
#define CMMU_CDP2	0x808	/* cache data port 2 */
#define CMMU_CDP3	0x80C	/* cache data port 3 */
#define CMMU_CTP0	0x840	/* cache tag port 0 */
#define CMMU_CTP1	0x844	/* cache tag port 1 */
#define CMMU_CTP2	0x848	/* cache tag port 2 */
#define CMMU_CTP3	0x84C	/* cache tag port 3 */
#define CMMU_CSSP	0x880	/* cache set status register */

#define CMMU_BWP(_X_) \
	(((_X_) < 7) ? \
	 (((_X_) < 6) ? \
	  (((_X_) < 5) ? \
	   (((_X_) < 4) ? \
	    (((_X_) < 3) ? \
	     (((_X_) < 2) ? \
	      (((_X_) < 1) ? \
	       CMMU_BWP0 : \
	       CMMU_BWP1) : \
	      CMMU_BWP2) : \
	     CMMU_BWP3) : \
	    CMMU_BWP4) : \
	   CMMU_BWP5) : \
	  CMMU_BWP6) : \
	 CMMU_BWP7)

/* 88204 CMMU definitions  */
#define CMMU_CSSP0	0x880	/* cache set status register */
#define CMMU_CSSP1	0x890	/* cache set status register */
#define CMMU_CSSP2	0x8A0	/* cache set status register */
#define CMMU_CSSP3	0x8B0	/* cache set status register */

/* CMMU systerm commands */
#define CMMU_FLUSH_USER_LINE		0x30	/* flush PATC */
#define CMMU_FLUSH_USER_PAGE		0x31
#define CMMU_FLUSH_USER_SEGMENT		0x32
#define CMMU_FLUSH_USER_ALL		0x33
#define CMMU_FLUSH_SUPER_LINE		0x34
#define CMMU_FLUSH_SUPER_PAGE		0x35
#define CMMU_FLUSH_SUPER_SEGMENT	0x36
#define CMMU_FLUSH_SUPER_ALL		0x37
#define CMMU_PROBE_USER			0x20	/* probe user address */
#define CMMU_PROBE_SUPER		0x24	/* probe supervisor address */
#define CMMU_FLUSH_CACHE_INV_LINE	0x14	/* data cache invalidate */
#define CMMU_FLUSH_CACHE_INV_PAGE	0x15
#define CMMU_FLUSH_CACHE_INV_SEGMENT	0x16
#define CMMU_FLUSH_CACHE_INV_ALL	0x17
#define CMMU_FLUSH_CACHE_CB_LINE	0x18	/* data cache copyback */
#define CMMU_FLUSH_CACHE_CB_PAGE	0x19
#define CMMU_FLUSH_CACHE_CB_SEGMENT	0x1A
#define CMMU_FLUSH_CACHE_CB_ALL		0x1B
#define CMMU_FLUSH_CACHE_CBI_LINE	0x1C	/* copyback and invalidate */
#define CMMU_FLUSH_CACHE_CBI_PAGE	0x1D
#define CMMU_FLUSH_CACHE_CBI_SEGMENT	0x1E
#define CMMU_FLUSH_CACHE_CBI_ALL	0x1F

/* CMMU system control command */
#define CMMU_SCTR_PE	0x00008000	/* parity enable */
#define CMMU_SCTR_SE	0x00004000	/* snoop enable */
#define CMMU_SCTR_PR	0x00002000	/* priority arbitration */

/* CMMU P bus fault status */
#define CMMU_PFSR_SUCCESS	0	/* no fault */
#define CMMU_PFSR_BERROR	3	/* bus error */
#define CMMU_PFSR_SFAULT	4	/* segment fault */
#define CMMU_PFSR_PFAULT	5	/* page fault */
#define CMMU_PFSR_SUPER		6	/* supervisor violation */
#define CMMU_PFSR_WRITE		7	/* writer violation */

#ifndef	_LOCORE

/*
 * Prototypes from "mvme88k/mvme88k/m8820x.c"
 */
void m8820x_show_apr __P((unsigned));
void m8820x_setup_board_config __P((void));
void m8820x_setup_cmmu_config __P((void));
void m8820x_cmmu_dump_config __P((void));
void m8820x_cpu_configuration_print __P((int));
void m8820x_cmmu_shutdown_now __P((void));
void m8820x_cmmu_parity_enable __P((void));
unsigned m8820x_cmmu_cpu_number __P((void));
unsigned m8820x_cmmu_get_idr __P((unsigned));
void m8820x_cmmu_set_sapr __P((unsigned));
void m8820x_cmmu_remote_set_sapr __P((unsigned, unsigned));
void m8820x_cmmu_set_uapr __P((unsigned));
void m8820x_cmmu_set_batc_entry __P((unsigned, unsigned, unsigned, unsigned));
void m8820x_cmmu_set_pair_batc_entry __P((unsigned, unsigned, unsigned));
void m8820x_cmmu_flush_remote_tlb __P((unsigned, unsigned, vm_offset_t, int));
void m8820x_cmmu_flush_tlb __P((unsigned, vm_offset_t, int));
void m8820x_cmmu_pmap_activate __P((unsigned, unsigned, 
    batc_template_t i_batc[BATC_MAX],
				  batc_template_t d_batc[BATC_MAX]));
void m8820x_cmmu_flush_remote_cache __P((int, vm_offset_t, int));
void m8820x_cmmu_flush_cache __P((vm_offset_t, int));
void m8820x_cmmu_flush_remote_inst_cache __P((int, vm_offset_t, int));
void m8820x_cmmu_flush_inst_cache __P((vm_offset_t, int));
void m8820x_cmmu_flush_remote_data_cache __P((int, vm_offset_t, int));
void m8820x_cmmu_flush_data_cache __P((vm_offset_t, int));
void m8820x_dma_cachectl __P((vm_offset_t, int, int));

#if DDB
unsigned m8820x_cmmu_get_by_mode __P((int, int));
void m8820x_cmmu_show_translation __P((unsigned, unsigned, unsigned, int));
void m8820x_cmmu_cache_state __P((unsigned, unsigned));
void m8820x_show_cmmu_info __P((unsigned));
#endif 

void m8820x_cmmu_init __P((void));

#endif	/* _LOCORE */

/*
 * Possible MVME188 board configurations
 */
#define CONFIG_0		0x0
#define CONFIG_1		0x1
#define CONFIG_2		0x2
#define CONFIG_5		0x5
#define CONFIG_6		0x6
#define CONFIG_A		0xA

/*
 * Address masks for MMU configs
 */
#define CMMU_SRAM		      (1<<31)
#define CMMU_A12_MASK		(1<<12)
#define CMMU_A14_MASK		(1<<14)
#define CMMU_SRAM_MASK		((1<<31)|(1<<30))

#define INST_CMMU 0
#define DATA_CMMU 1
#define BOTH_CMMU 2

#define CMMU_MODE_INST		0
#define CMMU_MODE_DATA		1
#define CMMU_MODE_BOTH		2

#define CMMU_ACS_USER		0
#define CMMU_ACS_SUPER		1
#define CMMU_ACS_BOTH		2

#define CMMU_SPLIT_ADDRESS	0x0
#define CMMU_SPLIT_SPV		0x1
#define CMMU_SPLIT_SRAM_SPV	0x2
#define CMMU_SPLIT_SRAM_ALL	0x3

#define CMMU_SPLIT_MASK		0x3

#define CMMU_NSTRATEGIES	4

/*
 * Flags passed to cmmu_set()
 */
#define NUM_CMMU		0x01
#define NUM_CPU			0x02 /* notyetused */
#define MODE_VAL		0x04
#define ACCESS_VAL		0x08
#define ADDR_VAL		0x10

#define NBSG    (4*1024*1024) /* segment size */

#endif	/* __MACHINE_M8820X_H__ */
