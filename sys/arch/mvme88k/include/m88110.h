/*	$OpenBSD: m88110.h,v 1.1.4.5 2002/03/28 10:36:02 niklas Exp $ */

#ifndef	__MACHINE_M88110_H__
#define	__MACHINE_M88110_H__

#include <uvm/uvm_extern.h>
#ifndef _LOCORE
# include <machine/mmu.h>		 /* batc_template_t */
#endif

/*
 *	88110 CMMU definitions
 */
#define CMMU_ICMD 0
#define CMMU_ICTL 1
#define CMMU_ISAR 2
#define CMMU_ISAP 3
#define CMMU_IUAP 4
#define CMMU_IIR  5
#define CMMU_IBP  6
#define CMMU_IPPU 7
#define CMMU_IPPL 8
#define CMMU_ISR  9
#define CMMU_ILAR 10
#define CMMU_IPAR 11

#define CMMU_DCMD 12
#define CMMU_DCTL 13
#define CMMU_DSAR 14
#define CMMU_DSAP 15
#define CMMU_DUAP 16
#define CMMU_DIR  17
#define CMMU_DBP  18
#define CMMU_DPPU 19
#define CMMU_DPPL 20
#define CMMU_DSR  21
#define CMMU_DLAR 22
#define CMMU_DPAR 23

#define CMMU_ICMD_INV_ITIC       0x001    /* Invalidate Inst Cache & TIC */
#define CMMU_ICMD_INV_TIC        0x002    /* Invalidate TIC */
#define CMMU_ICMD_INV_LINE       0x005    /* Invalidate Inst Cache Line */
#define CMMU_ICMD_PRB_SUPR       0x008    /* MMU Probe Supervisor */
#define CMMU_ICMD_PRB_USER       0x009    /* MMU Probe User */
#define CMMU_ICMD_INV_SATC       0x00A    /* Invalidate All Supervisor ATCs */
#define CMMU_ICMD_INV_UATC       0x00B    /* Invalidate All User ATCs */

#define CMMU_ICTL_DID            0x8000   /* Double instruction disable */
#define CMMU_ICTL_PREN           0x4000   /* Branch Prediction Enable */
#define CMMU_ICTL_FRZ0           0x0100   /* Inst Cache Freeze Bank 0 */
#define CMMU_ICTL_FRZ1           0x0080   /* Inst Cache Freeze Bank 1 */
#define CMMU_ICTL_HTEN           0x0040   /* Hardware Table Search Enable */
#define CMMU_ICTL_MEN            0x0020   /* Inst MMU Enable */
#define CMMU_ICTL_BEN            0x0004   /* TIC Cache Enable */
#define CMMU_ICTL_CEN            0x0001   /* Inst Cache Enable */

#define CMMU_ISR_TBE             0x200000 /* Table Search Bus Error */
#define CMMU_ISR_SI              0x100000 /* Segment Fault*/
#define CMMU_ISR_PI              0x080000 /* Page Fault */
#define CMMU_ISR_SP              0x040000 /* Supervisor Protection Violation */
#define CMMU_ISR_PH              0x000800 /* PATC Hit */
#define CMMU_ISR_BH              0x000400 /* BATC Hit */
#define CMMU_ISR_SU              0x000200 /* Supervisor Bit */
#define CMMU_ISR_BE              0x000001 /* Bus Error */

#define CMMU_DCMD_FLUSH_PG       0x000    /* Flush Data Cache Page (sync) */
#define CMMU_DCMD_INV_ALL        0x001    /* Invalidate Data Cache All */
#define CMMU_DCMD_FLUSH_ALL      0x002    /* Flush Data Cache All (sync) */
#define CMMU_DCMD_FLUSH_ALL_INV  0x003    /* Flush Data Cache All (sync & inval) */
#define CMMU_DCMD_FLUSH_PG_INV   0x004    /* Flush Data Cache Page (sync & inval) */
#define CMMU_DCMD_INV_LINE       0x005    /* Invalidate Data Cache Line */
#define CMMU_DCMD_FLUSH_LINE     0x006    /* Flush Data Cache Line (sync)*/
#define CMMU_DCMD_FLUSH_LINE_INV 0x007    /* Flush Data Cache Line (sync & inval)*/
#define CMMU_DCMD_PRB_SUPR       0x008    /* MMU Probe Supervisor */
#define CMMU_DCMD_PRB_USER       0x009    /* MMU Probe User */
#define CMMU_DCMD_INV_SATC       0x00A    /* Invalidate All Supervisor ATCs */
#define CMMU_DCMD_INV_UATC       0x00B    /* Invalidate All User ATCs */

#define CMMU_DCTL_RSVD7          0x40000   /* Reserved */
#define CMMU_DCTL_RSVD6          0x20000   /* Reserved */
#define CMMU_DCTL_RSVD5          0x10000   /* Reserved */
#define CMMU_DCTL_RSVD4          0x8000   /* Reserved */
#define CMMU_DCTL_RSVD3          0x4000   /* Reserved */
#define CMMU_DCTL_XMEM           0x2000   /* store -> load sequence */
#define CMMU_DCTL_DEN            0x1000   /* Decoupled Cache Access Enable */
#define CMMU_DCTL_FWT            0x0800   /* Force Write Through */
#define CMMU_DCTL_BPEN1          0x0400   /* Break Point Enable 1 */
#define CMMU_DCTL_BPEN0          0x0200   /* Break Point Enable 0 */
#define CMMU_DCTL_FRZ0           0x0100   /* Data Cache Freeze Bank 0 */
#define CMMU_DCTL_FRZ1           0x0080   /* Data Cache Freeze Bank 1 */
#define CMMU_DCTL_HTEN           0x0040   /* Hardware Table Search Enable */
#define CMMU_DCTL_MEN            0x0020   /* Data MMU Enable */
#define CMMU_DCTL_RSVD2          0x0010   /* Reserved */
#define CMMU_DCTL_ADS            0x0008   /* Allocat Disable */
#define CMMU_DCTL_RSVD1          0x0004   /* Reserved */
#define CMMU_DCTL_SEN            0x0002   /* Data Cache Snoop Enable */
#define CMMU_DCTL_CEN            0x0001   /* Data Cache Enable */

#define CMMU_DSR_TBE             0x200000 /* Table Search Bus Error */
#define CMMU_DSR_SI              0x100000 /* Segment Fault*/
#define CMMU_DSR_PI              0x080000 /* Page Fault */
#define CMMU_DSR_SP              0x040000 /* Supervisor Protection Violation */
#define CMMU_DSR_WE              0x020000 /* Write Protection Violation */
#define CMMU_DSR_BPE             0x010000 /* Break Point Exception */
#define CMMU_DSR_PH              0x000800 /* PATC Hit */
#define CMMU_DSR_BH              0x000400 /* BATC Hit */
#define CMMU_DSR_SU              0x000200 /* Supervisor Bit */
#define CMMU_DSR_RW              0x000100 /* Read Bit */
#define CMMU_DSR_CP              0x000004 /* Copyback Error */
#define CMMU_DSR_WA              0x000002 /* Write-Allocate Bus Error */
#define CMMU_DSR_BE              0x000001 /* Bus Error */

#define CMMU_READ 0
#define CMMU_WRITE 1
#define CMMU_DATA 1
#define CMMU_INST 0

/* definitions for use of the BATC */
#define BATC_512K	(0x00 << 19)
#define BATC_1M		(0x01 << 19)
#define BATC_2M		(0x03 << 19)
#define BATC_4M		(0x07 << 19)
#define BATC_8M		(0x0F << 19)
#define BATC_16M	(0x1F << 19)
#define BATC_32M	(0x3F << 19)
#define BATC_64M	(0x7F << 19)
#define BATC_ADDR_MASK	0xFFF80000
#define BATC_ADDR_SHIFT	13
#define BATC_LBA_SHIFT	19
#define BATC_PBA_SHIFT	6
#define BATC_SU		0x20
#define BATC_WT		0x10
#define BATC_G		0x08
#define BATC_CI		0x04
#define BATC_WP		0x02
#define BATC_V		0x01

#define CLINE_MASK	0x1F
#define CLINE_SIZE	(8 * 32)

#ifndef	_LOCORE

/*
 * Prototypes from "mvme88k/mvme88k/m88110_cmmu.c"
 */
void m88110_show_apr(unsigned);
void m88110_show_sctr(unsigned);
void m88110_setup_board_config(void);
void m88110_setup_cmmu_config(void);
void m88110_cmmu_dump_config(void);
void m88110_cpu_configuration_print(int);
void m88110_cmmu_shutdown_now(void);
void m88110_cmmu_parity_enable(void);
unsigned m88110_cmmu_cpu_number(void);
unsigned m88110_cmmu_get_idr(unsigned);
void m88110_cmmu_set_sapr(unsigned);
void m88110_cmmu_remote_set_sapr(unsigned, unsigned);
void m88110_cmmu_set_uapr(unsigned);
void m88110_cmmu_set_batc_entry(unsigned, unsigned, unsigned, unsigned);
void m88110_cmmu_set_pair_batc_entry(unsigned, unsigned, unsigned);
void m88110_cmmu_flush_remote_tlb(unsigned, unsigned, vm_offset_t, int);
void m88110_cmmu_flush_tlb(unsigned, vm_offset_t, int);
void m88110_cmmu_pmap_activate(unsigned, unsigned, 
				  batc_template_t i_batc[BATC_MAX],
				  batc_template_t d_batc[BATC_MAX]);
void m88110_cmmu_flush_remote_cache(int, vm_offset_t, int);
void m88110_cmmu_flush_cache(vm_offset_t, int);
void m88110_cmmu_flush_remote_inst_cache(int, vm_offset_t, int);
void m88110_cmmu_flush_inst_cache(vm_offset_t, int);
void m88110_cmmu_flush_remote_data_cache(int, vm_offset_t, int);
void m88110_cmmu_flush_data_cache(vm_offset_t, int);
void m88110_dma_cachectl(vm_offset_t, int, int);

#if DDB
unsigned m88110_cmmu_get_by_mode(int, int);
void m88110_cmmu_show_translation(unsigned, unsigned, unsigned, int);
void m88110_cmmu_cache_state(unsigned, unsigned);
void m88110_show_cmmu_info(unsigned);
#endif

void m88110_cmmu_init(void);
int m88110_table_search(pmap_t, vm_offset_t, int, int, int);

void set_icmd(unsigned value);
void set_ictl(unsigned value);
void set_isar(unsigned value);
void set_isap(unsigned value);
void set_iuap(unsigned value);
void set_iir(unsigned value);
void set_ibp(unsigned value);
void set_ippu(unsigned value);
void set_ippl(unsigned value);
void set_isr(unsigned value);
void set_ilar(unsigned value);
void set_ipar(unsigned value);
void set_dcmd(unsigned value);
void set_dctl(unsigned value);
void set_dsar(unsigned value);
void set_dsap(unsigned value);
void set_duap(unsigned value);
void set_dir(unsigned value);
void set_dbp(unsigned value);
void set_dppu(unsigned value);
void set_dppl(unsigned value);
void set_dsr(unsigned value);
void set_dlar(unsigned value);
void set_dpar(unsigned value);

/* get routines */
unsigned get_icmd(void);
unsigned get_ictl(void);
unsigned get_isar(void);
unsigned get_isap(void);
unsigned get_iuap(void);
unsigned get_iir(void);
unsigned get_ibp(void);
unsigned get_ippu(void);
unsigned get_ippl(void);
unsigned get_isr(void);
unsigned get_ilar(void);
unsigned get_ipar(void);
unsigned get_dcmd(void);
unsigned get_dctl(void);
unsigned get_dsar(void);
unsigned get_dsap(void);
unsigned get_duap(void);
unsigned get_dir(void);
unsigned get_dbp(void);
unsigned get_dppu(void);
unsigned get_dppl(void);
unsigned get_dsr(void);
unsigned get_dlar(void);
unsigned get_dpar(void);

/* Cache inlines */

#define line_addr(x)	(vm_offset_t)((x) & ~CLINE_MASK)
#define page_addr(x)	(vm_offset_t)((x) & ~PAGE_MASK)

static __inline__ void mc88110_flush_data_line(vm_offset_t x)
{
	unsigned dctl = get_dctl();
	if (dctl & CMMU_DCTL_CEN){
		set_dsar(line_addr(x)); 
		set_dcmd(CMMU_DCMD_FLUSH_LINE);
	}
}

static __inline__ void mc88110_flush_data_page(vm_offset_t x)
{
	unsigned dctl = get_dctl();
	if (dctl & CMMU_DCTL_CEN){
		set_dsar(page_addr(x)); 
		set_dcmd(CMMU_DCMD_FLUSH_PG);
	}
}

static __inline__ void mc88110_flush_data(void)
{
	unsigned dctl = get_dctl();
	if (dctl & CMMU_DCTL_CEN){
		set_dsar(0x00);
		set_dcmd(CMMU_DCMD_FLUSH_ALL);
	}
}

static __inline__ void mc88110_inval_data_line(vm_offset_t x)
{
	set_dsar(line_addr(x));
	set_dcmd(CMMU_DCMD_INV_LINE);
}

static __inline__ void mc88110_inval_data(void)
{
	set_dsar(0x00);
	set_dcmd(CMMU_DCMD_INV_ALL);
}

static __inline__ void mc88110_sync_data_line(vm_offset_t x)
{
	unsigned dctl = get_dctl();
	if (dctl & CMMU_DCTL_CEN){
		set_dsar(line_addr(x)); 
		set_dcmd(CMMU_DCMD_FLUSH_LINE_INV);
	}
}

static __inline__ void mc88110_sync_data_page(vm_offset_t x)
{
	unsigned dctl = get_dctl();
	if (dctl & CMMU_DCTL_CEN){
		set_dsar(page_addr(x)); 
		set_dcmd(CMMU_DCMD_FLUSH_PG_INV);
	}
}

static __inline__ void mc88110_sync_data(void)
{
	unsigned dctl = get_dctl();
	if (dctl & CMMU_DCTL_CEN){
		set_dsar(0x00);
		set_dcmd(CMMU_DCMD_FLUSH_ALL_INV);
	}
}

static __inline__ void mc88110_inval_inst_line(vm_offset_t x)
{
	set_isar(line_addr(x));
	set_icmd(CMMU_ICMD_INV_LINE);
}

static __inline__ void mc88110_inval_inst(void)
{
	set_isar(0x00);
	set_icmd(CMMU_ICMD_INV_ITIC);
}

#endif	/* _LOCORE */
#endif /* __MACHINE_M88110_H__ */
