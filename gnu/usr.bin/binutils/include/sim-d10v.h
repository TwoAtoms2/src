/* This file defines the interface between the d10v simulator and gdb.
   Copyright (C) 1999 Free Software Foundation, Inc.

This file is part of GDB.

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#if !defined (SIM_D10V_H)
#define SIM_D10V_H

#ifdef __cplusplus
extern "C" { // }
#endif

/* GDB interprets addresses as:

   0x00xxxxxx: Physical unified memory segment     (Unified memory)
   0x01xxxxxx: Physical instruction memory segment (On-chip insn memory)
   0x02xxxxxx: Physical data memory segment        (On-chip data memory)
   0x10xxxxxx: Logical data address segment        (DMAP translated memory)
   0x11xxxxxx: Logical instruction address segment (IMAP translated memory)

   The remote d10v board interprets addresses as:

   0x00xxxxxx: Physical unified memory segment     (Unified memory)
   0x01xxxxxx: Physical instruction memory segment (On-chip insn memory)
   0x02xxxxxx: Physical data memory segment        (On-chip data memory)

   The following translate a virtual DMAP/IMAP offset into a physical
   memory segment assigning the translated address to PHYS.  Since a
   memory access may cross a page boundrary the number of bytes for
   which the translation is applicable (or 0 for an invalid virtual
   offset) is returned. */

enum
  {
    SIM_D10V_MEMORY_UNIFIED = 0x00000000,
    SIM_D10V_MEMORY_INSN = 0x01000000,
    SIM_D10V_MEMORY_DATA = 0x02000000,
    SIM_D10V_MEMORY_DMAP = 0x10000000,
    SIM_D10V_MEMORY_IMAP = 0x11000000
  };

extern unsigned long sim_d10v_translate_dmap_addr
  (unsigned long offset,
   int nr_bytes,
   unsigned long *phys,
   unsigned long (*dmap_register) (int reg_nr));

extern unsigned long sim_d10v_translate_imap_addr
  (unsigned long offset,
   int nr_bytes,
   unsigned long *phys,
   unsigned long (*imap_register) (int reg_nr));

extern unsigned long sim_d10v_translate_addr
  (unsigned long vaddr,
   int nr_bytes,
   unsigned long *phys,
   unsigned long (*dmap_register) (int reg_nr),
   unsigned long (*imap_register) (int reg_nr));


/* The simulator makes use of the following register information. */

enum
  {
    SIM_D10V_R0_REGNUM = 0,
    SIM_D10V_CR0_REGNUM = 16,
    SIM_D10V_A0_REGNUM = 32,
    SIM_D10V_SPI_REGNUM = 34,
    SIM_D10V_SPU_REGNUM = 35,
    SIM_D10V_IMAP0_REGNUM = 36,
    SIM_D10V_DMAP0_REGNUM = 38,
    SIM_D10V_TS2_DMAP_REGNUM = 40
  };

enum
  {
    SIM_D10V_NR_R_REGS = 16,
    SIM_D10V_NR_A_REGS = 2,
    SIM_D10V_NR_IMAP_REGS = 2,
    SIM_D10V_NR_DMAP_REGS = 4,
    SIM_D10V_NR_CR_REGS = 16
  };

#ifdef __cplusplus
}
#endif

#endif
