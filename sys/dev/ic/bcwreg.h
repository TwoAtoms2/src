/*	$OpenBSD: bcwreg.h,v 1.20 2007/03/16 12:16:31 mglocker Exp $ */

/*
 * Copyright (c) 2006 Jon Simola <jsimola@gmail.com>
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
 *
 */

/* Broadcom BCM43xx */

/*
 * PCI registers defined in the PCI 2.2 specs
 */
#define BCW_PCI_BAR0			0x10

/*
 * SPROM control register
 */
#define BCW_SPROM_CONTROL		0x88

/*
 * MMIO offsets
 */
#define BCW_MMIO_DMA0_INT_MASK		0x24
#define BCW_MMIO_DMA1_INT_MASK		0x2c
#define BCW_MMIO_DMA2_INT_MASK		0x34
#define BCW_MMIO_DMA3_INT_MASK		0x3c
#define BCW_MMIO_DMA4_INT_MASK		0x44
#define BCW_MMIO_DMA5_INT_MASK		0x4c
#define BCW_MMIO_SBF			0x120	/* Status Bit Field */
#define BCW_MMIO_GIR			0x128	/* Generic Interrupt Reason */
#define BCW_MMIO_GIM			0x12c	/* Generic Interrupt Mask */
#define BCW_MMIO_RAM_CONTROL		0x130
#define BCW_MMIO_RAM_DATA		0x134
#define BCW_MMIO_SHM_CONTROL		0x160	/* Control */
#define BCW_MMIO_SHM_DATA		0x164	/* Data - 32bit */
#define BCW_MMIO_SHM_DATALOW		0x164	/* Data Low - 16bit */
#define BCW_MMIO_SHM_DATAHIGH		0x166	/* Data High - 16 bit */

#define BCW_MMIO_PHY_RADIO		0x3e2
#define BCW_MMIO_CHANNEL		0x3f0
#define BCW_MMIO_CHANNEL_EXT		0x3f4
#define BCW_MMIO_RADIO_DATA_LOW		0x3fa
#define BCW_MMIO_RADIO_CONTROL		0x3f6
#define BCW_MMIO_RADIO_DATA_HIGH	0x3f8
#define BCW_MMIO_PHY_CONTROL		0x3fc
#define BCW_MMIO_PHY_DATA		0x3fe
#define BCW_MMIO_GPIO_CONTROL		0x49c
#define BCW_MMIO_GPIO_MASK		0x49e

/*
 * XXX SPROM registers are 16 bit and based at MMIO offset 0x1000
 */
#define BCW_SPROM_BASE			0x1000
#define BCW_SPROM_SUBPRODID		0x1004	/* Subsystem Product ID */
#define BCW_SPROM_SUBVENID		0x1006	/* Subsystem Vendor ID */
#define BCW_SPROM_PRODID		0x1008	/* Product ID */
#define BCW_SPROM_IL0MACADDR		0x1048	/* 802.11b/g MAC */
#define BCW_SPROM_ET0MACADDR		0x104e	/* ethernet MAC */
#define BCW_SPROM_ET1MACADDR		0x1054	/* 802.11a MAC */
#define BCW_SPROM_PA0B0			0x105e
#define BCW_SPROM_PA0B1			0x1060
#define BCW_SPROM_PA0B2			0x1062
#define BCW_SPROM_PAMAXPOWER		0x1066	/* 7-0 for A, 15-8 for B/G */
#define BCW_SPROM_PA1B0			0x106a
#define BCW_SPROM_PA1B1			0x106c
#define BCW_SPROM_PA1B2			0x106e
#define BCW_SPROM_IDLETSSI		0x1070  /* As below */
#define BCW_SPROM_BOARDFLAGS		0x1072	/* lower 16 bits */

/*
 * SPROM Board Flags
 */
#define BCW_BF_BTCOEXIST		0x0001	/* Bluetooth Co-existance */
#define BCW_BF_PACTRL			0x0002	/* GPIO 9 controls PA */
#define BCW_BF_AIRLINEMODE		0x0004	/* GPIO13 -> radio off LED */
#define BCW_BF_RSSI			0x0008	/* "something RSSI related */
#define BCW_BF_ENETSPI			0x0010	/* EPHY Roboswitch SPI */
#define BCW_BF_XTAL			0x0020	/* XTAL doesn't slow clock */
#define BCW_BF_CCKHIPWR			0x0040	/* HiPower CCK xmission */
#define BCW_BF_ENETADM			0x0080	/* has ADMtek Switch */
#define BCW_BF_ENETVLAN			0x0100	/* VLAN capable - huh? */
#define BCW_BF_AFTERBURNER		0x0200	/* Afterburner capable */
#define BCW_BF_NOPCI			0x0400	/* Board leaves PCI floating */
#define BCW_BF_FEM			0x0800	/* Front End Module support */
#define BCW_BF_EXTLNA			0x1000	/* has external LNA */
#define BCW_BF_HGPA			0x2000	/* has High Gain PA */
#define BCW_BF_BTCMOD			0x4000	/* BTCOEXIST in alt GPIOs */
#define BCW_BF_ALTIQ			0x8000	/* Alternate I/Q settings */

/*
 * GPIO register offset, in both ChipCommon and PCI core
 */
#define BCW_GPIO_CTRL			0x6c

/*
 * SHM Routing
 */
#define BCW_SHM_SHARED			0x0001
#define BCW_SHM_80211			0x0002
#define BCW_SHM_PCM			0x0003
#define BCW_SHM_HWMAC			0x0004
#define BCW_SHM_UCODE			0x0300

/*
 * Power control
 */
#define BCW_PCTL_IN			0xb0
#define BCW_PCTL_OUT			0xb4
#define BCW_PCTL_OUTENABLE		0xb8
#define BCW_PCTL_XTAL_POWERUP		0x40
#define BCW_PCTL_PLL_POWERDOWN		0x80

/*
 * Core IDs
 */
#define BCW_CORE_COMMON_CHIPID		0x0
#define BCW_CORE_COMMON			0x800
#define BCW_CORE_PCI			0x804
#define BCW_CORE_ENET			0x806
#define BCW_CORE_PCMCIA			0x80d
#define BCW_CORE_80211			0x812
#define BCW_CORE_PCIE			0x820
#define BCW_CORE_MIMOPHY		0x821
#define BCW_CORE_NONEXIST		0x8ff

/*
 * Core information registers
 */
#define BCW_CIR_BASE			0xf00
#define BCW_CIR_SBTMSTATELOW		(BCW_CIR_BASE + 0x98)

/*
 * PHY Versioning
 */
#define BCW_PHY_TYPEA			0x0	/* 802.11a PHY */
#define BCW_PHY_TYPEB			0x1	/* 802.11b PHY */
#define BCW_PHY_TYPEG			0x2	/* 802.11g PHY */
#define BCW_PHY_TYPEN			0x4	/* 802.11n PHY */

/*
 * PHY registers
 */
#define BCW_PHY_ILT_A_CTRL		0x0072
#define BCW_PHY_ILT_A_DATA1		0x0073
#define BCW_PHY_G_LO_CONTROL		0x0810
#define BCW_PHY_ILT_G_CTRL		0x0472
#define BCW_PHY_ILT_G_DATA1		0x0473
#define BCW_PHY_G_CRS			0x0429
#define BCW_PHY_A_CRS			0x0029
#define BCW_PHY_RADIO_BITFIELD		0x0401
#define BCW_PHY_A_PCTL			0x007b
#define BCW_PHY_G_PCTL			0x0029
#define BCW_PHY_NRSSILT_CTRL		0x0803
#define BCW_PHY_NRSSILT_DATA		0x0804

/*
 * Radio Registers
 */
#define BCW_RADIO_ID			0x01

/*
 * Status Bit Fields
 */
#define BCW_SBF_MAC_ENABLED		0x00000001
#define BCW_SBF_CORE_READY		0x00000004
#define BCW_SBF_400_MAGIC		0x00000400
#define BCW_SBF_REGISTER_BYTESWAP	0x00010000
#define BCW_SBF_ADHOC			0x00020000
#define BCW_SBF_AP			0x00040000
#define BCW_SBF_RADIOREG_LOCK		0x00080000
#define BCW_SBF_MONITOR			0x00400000 
#define BCW_SBF_PROMISC			0x01000000
#define BCW_SBF_PS1			0x02000000
#define BCW_SBF_PS2			0x04000000
#define BCW_SBF_SSID_BCAST		0x08000000
#define BCW_SBF_TIME_UPDATE		0x10000000

/*
 * Microcode
 */
#define BCW_UCODE_REVISION		0x0000
#define BCW_UCODE_PATCHLEVEL		0x0002
#define BCW_UCODE_DATE			0x0004
#define BCW_UCODE_TIME			0x0006
#define BCW_UCODE_STATUS		0x0040

/*
 * Microcode Flags Bitfield
 *
 * http://bcm-specs.sipsolutions.net/MicrocodeFlagsBitfield
 */
#define BCW_SHM_MICROCODEFLAGSLOW	0x005e
#define BCW_SHM_MICROCODEFLAGSAUTODIV	0x0001

/*
 * Generic interrupt reasons
 */
#define BCW_INTR_READY			(1 << 0)
#define BCW_INTR_BEACON			(1 << 1)
#define BCW_INTR_PS			(1 << 2)
#define BCW_INTR_REG124			(1 << 5)
#define BCW_INTR_PMQ			(1 << 6)
#define BCW_INTR_PIO_WORKAROUND		(1 << 8)
#define BCW_INTR_XMIT_ERROR		(1 << 11)
#define BCW_INTR_RX			(1 << 15)
#define BCW_INTR_SCAN			(1 << 16)
#define BCW_INTR_NOISE			(1 << 18)
#define BCW_INTR_XMIT_STATUS		(1 << 29)

#define BCW_INTR_ALL			0xffffffff
#define BCW_INTR_INITIAL		(BCW_INTR_PS |		\
					 BCW_INTR_REG124 |	\
					 BCW_INTR_PMQ |		\
					 BCW_INTR_XMIT_ERROR |	\
					 BCW_INTR_RX |		\
					 BCW_INTR_SCAN |	\
					 BCW_INTR_NOISE |	\
					 BCW_INTR_XMIT_STATUS)

/*
 * XXX 0x300 PIO Register space
 */
#define BCW_RADIO_BASEBAND		0x3e6	/* Baseband Attenuation */

/*
 * XXX Do we really need those ...
 */
#define BCW_GPIOI			0xb0
#define BCW_GPIOO			0xb4
#define BCW_GPIOE			0xb8
#define BCW_ADDR_SPACE0			0x80
/* transmit control bits */
#define BCW_DMA_TXADDR			0x0204
#define BCW_DMA_RXADDR			0x0214
/* receive status bits */
#define RS_CD_MASK			0x0fff
/* SiliconBackplane registers 0xea8 through 0xffc */
#define SBIM_REJECT			0x0200000
#define SBIM_INBANDERR			0x20000
#define SBIM_TIMEOUT			0x40000
#define SBTML_RESET			0x1
#define SBTML_REJ			0x6
#define SBTML_REJ22			0x2
#define SBTML_REJ23			0x4
#define SBTML_CLK			0x10000
#define SBTML_FGC			0x20000
#define SBTML_80211FLAG			0x40000
#define SBTML_80211PHY			0x20000000
#define SBTMH_SERR			0x1
#define SBTMH_BUSY			0x4
#define SBREV_MASK			0xf0000000
#define SBREV_MASK_SHIFT		28
#define SBTPS_BACKPLANEFLAGMASK		0x3f
#define BCW_SBTMSTATEHI			0x0f9C
#define BCW_SBTPSFLAG			0x0f18
#define BCW_SBIMSTATE			0x0f90
#define BCW_SBTMSTATELOW		0x0f98
#define BCW_CIR_SBID_LO			0xff8
#define BCW_CIR_SBID_LO_INITIATOR	0x80
#define BCW_CIR_SBID_HI			0xffc
#define BCW_SPROM_ANTGAIN		0x1074

/*
 * XXX Dead code (also dead in bcw.c), please remove me soon
 */
#if 0
#define BCW_INT_STS			0x20
#define BCW_REG0_WIN			0x80			/* XXX */
#define BCW_PCICR			0x94			/* XXX */
#define BCW_SONICS_WIN			0x18002000		/* XXX */
#define BCW_SPCI_TR2			0x0108			/* XXX */
#define SBTOPCI_PREF			0x4			/* XXX */
#define SBTOPCI_BURST			0x8			/* XXX */
#define BCW_SBINTVEC			0x0f94			/* XXX */
#define SBIV_ENET0			0x02			/* XXX */
#define I_TO				0x00000080		/* XXX */
#define I_PC				0x00000400		/* XXX */
#define I_PD				0x00000800		/* XXX */
#define I_DE				0x00001000		/* XXX */
#define I_RU				0x00002000 		/* XXX */
#define I_RO				0x00004000		/* XXX */
#define I_XU				0x00008000		/* XXX */
#define I_RI				0x00010000		/* XXX */
#define I_XI				0x01000000		/* XXX */
#define BCW_MACCTL			0x00A8			/* XXX */
#define BCW_EMC_CRC32_ENAB		0x00000001		/* XXX */
#define BCW_EMC_PDOWN			0x00000004		/* XXX */
#define BCW_EMC_LED			0x000000e0		/* XXX */
#define BCW_DMAI_CTL			0x0100			/* XXX */
#define BCW_DMA_TXCTL			0x0200			/* XXX */
#define XC_XE				0x1			/* XXX */
#define BCW_DMA_DPTR			0x0208			/* XXX */
#define BCW_DMA_RXCTL			0x0210			/* XXX */
#define BCW_DMA_RXDPTR			0x0218			/* XXX */
#define BCW_RX_CTL			0x0400			/* XXX */
#define ERC_DB				0x00000001		/* XXX */
#define ERC_AM				0x00000002		/* XXX */
#define ERC_PE				0x00000008		/* XXX */
#define BCW_RX_MAX			0x0404			/* XXX */
#define BCW_TX_MAX			0x0408			/* XXX */
#define BCW_FILT_CTL			0x0428			/* XXX */
#define BCW_ENET_CTL			0x042C			/* XXX */
#define EC_EE				0x00000001		/* XXX */
#define EC_ED				0x00000002		/* XXX */
#define BCW_TX_WATER			0x0434			/* XXX */
#define BCW_DMA_TXSTATUS(x)		(0x20c + (x * 0x20))	/* XXX */
#endif

/*
 * XXX Do we really need those ...
 */
#define BCW_DMA_TXCONTROL(x)		(0x200 + (x * 0x20))
#define BCW_DMA_RXCONTROL(x)		(0x200 + (x * 0x20))
#define BCW_DMA_RXSTATUS(x)		(0x20c + (x * 0x20))

/*
 * Core select address macro
 */
#define BCW_CORE_SELECT(x)		(0x18000000 + (x * 0x1000))

/*
 * 16-bit and 32-bit register read/write
 */
#define BCW_READ16(sc, reg) \
	bus_space_read_2((sc)->sc_iot, (sc)->sc_ioh, (reg))
#define BCW_READ(sc, reg) \
	bus_space_read_4((sc)->sc_iot, (sc)->sc_ioh, (reg))
#define BCW_WRITE16(sc, reg, val) \
	bus_space_write_2((sc)->sc_iot, (sc)->sc_ioh, (reg), (val))
#define BCW_WRITE(sc, reg, val) \
	bus_space_write_4((sc)->sc_iot, (sc)->sc_ioh, (reg), (val))
