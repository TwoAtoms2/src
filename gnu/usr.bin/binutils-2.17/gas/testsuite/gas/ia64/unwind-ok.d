#readelf: -u
#name: ia64 unwind descriptors

Unwind section '\.IA_64\.unwind' at offset 0x[[:xdigit:]]+ contains 8 entries:

<full1>: \[0x[[:xdigit:]]*0-0x[[:xdigit:]]*0\], info at \+0x[[:xdigit:]]*[08]
[[:space:]]*v[[:digit:]]+, flags=0x3 \( ?ehandler uhandler\), len=[[:digit:]]+ bytes
[[:space:]]*R1:prologue\(rlen=8\)
[[:space:]]*P6:fr_mem\(frmask=\[f2,f5\]\)
[[:space:]]*P6:gr_mem\(grmask=\[r4,r7\]\)
[[:space:]]*P1:br_mem\(brmask=\[b1,b5\]\)
[[:space:]]*P4:spill_mask\(imask=\[rfb,rfb,--\]\)
[[:space:]]*P7:spill_base\(pspoff=0x10-0x10\)
[[:space:]]*P3:rp_br\(reg=b7\)
[[:space:]]*P10:unwabi\(abi=@svr4,context=0x00\)
[[:space:]]*R1:body\(rlen=25\)
[[:space:]]*X2:spill_reg\(t=0,reg=r4,treg=r2\)
[[:space:]]*X4:spill_reg_p\(qp=p1,t=1,reg=r7,treg=r31\)
[[:space:]]*X1:spill_sprel\(reg=b1,t=2,spoff=0x8\)
[[:space:]]*X3:spill_sprel_p\(qp=p2,t=3,reg=b5,spoff=0x10\)
[[:space:]]*X1:spill_psprel\(reg=f2,t=4,pspoff=0x10-0x28\)
[[:space:]]*X3:spill_psprel_p\(qp=p4,t=5,reg=f5,pspoff=0x10-0x30\)
[[:space:]]*X2:restore\(t=6,reg=f16\)
[[:space:]]*X4:restore_p\(qp=p8,t=7,reg=f31\)
[[:space:]]*X2:spill_reg\(t=8,reg=ar\.bsp,treg=r16\)
[[:space:]]*X2:spill_reg\(t=9,reg=ar\.bspstore,treg=r17\)
[[:space:]]*X2:spill_reg\(t=10,reg=ar\.fpsr,treg=r18\)
[[:space:]]*X2:spill_reg\(t=11,reg=ar\.lc,treg=r19\)
[[:space:]]*X2:spill_reg\(t=12,reg=ar\.pfs,treg=r20\)
[[:space:]]*X2:spill_reg\(t=13,reg=ar\.rnat,treg=r21\)
[[:space:]]*X2:spill_reg\(t=14,reg=ar\.unat,treg=r22\)
[[:space:]]*X2:spill_reg\(t=15,reg=psp,treg=r23\)
[[:space:]]*X2:spill_reg\(t=16,reg=pr,treg=r24\)
[[:space:]]*X2:spill_reg\(t=17,reg=rp,treg=r25\)
[[:space:]]*X2:spill_reg\(t=18,reg=@priunat,treg=r26\)
[[:space:]]*B1:label_state\(label=1\)
[[:space:]]*B2:epilogue\(t=4,ecount=0\)
[[:space:]]*B1:copy_state\(label=1\)
#...
<full2>: \[0x[[:xdigit:]]*0-0x[[:xdigit:]]*0\], info at \+0x[[:xdigit:]]*[08]
[[:space:]]*v[[:digit:]]+, flags=0x0( \(\))?, len=[[:digit:]]+ bytes
[[:space:]]*R2:prologue_gr\(mask=\[rp,psp,pr\],grsave=r8,rlen=14\)
[[:space:]]*P5:frgr_mem\(grmask=\[r4,r7\],frmask=\[f2,f31\]\)
[[:space:]]*P4:spill_mask\(imask=\[frb,bfr,---,---,--\]\)
[[:space:]]*P7:spill_base\(pspoff=0x10-0x10\)
[[:space:]]*P2:br_gr\(brmask=\[b1,b5\],gr=r32\)
[[:space:]]*X2:spill_reg\(t=6,reg=f31,treg=f31\)
[[:space:]]*X4:spill_reg_p\(qp=p63,t=7,reg=f16,treg=f0\)
[[:space:]]*X1:spill_sprel\(reg=f5,t=8,spoff=0x20\)
[[:space:]]*X3:spill_sprel_p\(qp=p31,t=9,reg=f2,spoff=0x18\)
[[:space:]]*X1:spill_psprel\(reg=b5,t=10,pspoff=0x10-0x20\)
[[:space:]]*X3:spill_psprel_p\(qp=p15,t=11,reg=b1,pspoff=0x10-0x18\)
[[:space:]]*X2:restore\(t=12,reg=r7\)
[[:space:]]*X4:restore_p\(qp=p7,t=13,reg=r4\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=0\)
[[:space:]]*R1:prologue\(rlen=0\)
[[:space:]]*R1:body\(rlen=7\)
[[:space:]]*B4:label_state\(label=32\)
[[:space:]]*B3:epilogue\(t=4,ecount=32\)
[[:space:]]*B4:copy_state\(label=32\)
#...
<full3>: \[0x[[:xdigit:]]*0-0x[[:xdigit:]]*0\], info at \+0x[[:xdigit:]]*[08]
[[:space:]]*v[[:digit:]]+, flags=0x0( \(\))?, len=[[:digit:]]+ bytes
[[:space:]]*R3:prologue\(rlen=33\)
[[:space:]]*P4:spill_mask\(imask=\[rrb,brr,bb-,---,---,---,---,---,---,---,---\]\)
[[:space:]]*P7:spill_base\(pspoff=0x10-0x10\)
[[:space:]]*P9:gr_gr\(grmask=\[r4,r5\],r32\)
[[:space:]]*P2:br_gr\(brmask=\[b1,b2\],gr=r34\)
[[:space:]]*P9:gr_gr\(grmask=\[r6,r7\],r124\)
[[:space:]]*P2:br_gr\(brmask=\[b4,b5\],gr=r126\)
[[:space:]]*R3:body\(rlen=33\)
#...
<fframe>: \[0x[[:xdigit:]]*0-0x[[:xdigit:]]*0\], info at \+0x[[:xdigit:]]*[08]
[[:space:]]*v[[:digit:]]+, flags=0x0( \(\))?, len=[[:digit:]]+ bytes
[[:space:]]*R1:prologue\(rlen=1\)
[[:space:]]*P7:mem_stack_f\(t=0,size=0\)
[[:space:]]*R1:body\(rlen=2\)
#...
<vframe>: \[0x[[:xdigit:]]*0-0x[[:xdigit:]]*0\], info at \+0x[[:xdigit:]]*[08]
[[:space:]]*v[[:digit:]]+, flags=0x0( \(\))?, len=[[:digit:]]+ bytes
[[:space:]]*R1:prologue\(rlen=11\)
[[:space:]]*P7:mem_stack_v\(t=0\)
[[:space:]]*P3:psp_gr\(reg=r16\)
[[:space:]]*P8:bsp_when\(t=1\)
[[:space:]]*P3:bsp_gr\(reg=r17\)
[[:space:]]*P8:bspstore_when\(t=2\)
[[:space:]]*P3:bspstore_gr\(reg=r18\)
[[:space:]]*P7:fpsr_when\(t=3\)
[[:space:]]*P3:fpsr_gr\(reg=r19\)
[[:space:]]*P7:lc_when\(t=4\)
[[:space:]]*P3:lc_gr\(reg=r20\)
[[:space:]]*P7:pfs_when\(t=5\)
[[:space:]]*P3:pfs_gr\(reg=r21\)
[[:space:]]*P8:rnat_when\(t=6\)
[[:space:]]*P3:rnat_gr\(reg=r22\)
[[:space:]]*P7:unat_when\(t=7\)
[[:space:]]*P3:unat_gr\(reg=r23\)
[[:space:]]*P7:pr_when\(t=8\)
[[:space:]]*P3:pr_gr\(reg=r24\)
[[:space:]]*P8:priunat_when_gr\(t=9\)
[[:space:]]*P3:priunat_gr\(reg=r25\)
[[:space:]]*P7:rp_when\(t=10\)
[[:space:]]*P3:rp_gr\(reg=r26\)
[[:space:]]*R1:body\(rlen=1\)
#...
<vframesp>: \[0x[[:xdigit:]]*0-0x[[:xdigit:]]*0\], info at \+0x[[:xdigit:]]*[08]
[[:space:]]*v[[:digit:]]+, flags=0x0( \(\))?, len=[[:digit:]]+ bytes
[[:space:]]*R1:prologue\(rlen=11\)
[[:space:]]*P7:mem_stack_v\(t=0\)
[[:space:]]*P7:psp_sprel\(spoff=0x0\)
[[:space:]]*P8:bsp_when\(t=1\)
[[:space:]]*P8:bsp_sprel\(spoff=0x8\)
[[:space:]]*P8:bspstore_when\(t=2\)
[[:space:]]*P8:bspstore_sprel\(spoff=0x10\)
[[:space:]]*P7:fpsr_when\(t=3\)
[[:space:]]*P8:fpsr_sprel\(spoff=0x18\)
[[:space:]]*P7:lc_when\(t=4\)
[[:space:]]*P8:lc_sprel\(spoff=0x20\)
[[:space:]]*P7:pfs_when\(t=5\)
[[:space:]]*P8:pfs_sprel\(spoff=0x28\)
[[:space:]]*P8:rnat_when\(t=6\)
[[:space:]]*P8:rnat_sprel\(spoff=0x30\)
[[:space:]]*P7:unat_when\(t=7\)
[[:space:]]*P8:unat_sprel\(spoff=0x38\)
[[:space:]]*P7:pr_when\(t=8\)
[[:space:]]*P8:pr_sprel\(spoff=0x40\)
[[:space:]]*P8:priunat_when_mem\(t=9\)
[[:space:]]*P8:priunat_sprel\(spoff=0x48\)
[[:space:]]*P7:rp_when\(t=10\)
[[:space:]]*P8:rp_sprel\(spoff=0x50\)
[[:space:]]*R1:body\(rlen=1\)
#...
<psp>: \[0x[[:xdigit:]]*0-0x[[:xdigit:]]*0\], info at \+0x[[:xdigit:]]*[08]
[[:space:]]*v[[:digit:]]+, flags=0x0( \(\))?, len=[[:digit:]]+ bytes
[[:space:]]*R1:prologue\(rlen=11\)
[[:space:]]*P7:mem_stack_v\(t=0\)
[[:space:]]*P7:psp_sprel\(spoff=0x0\)
[[:space:]]*P8:bsp_when\(t=1\)
[[:space:]]*P8:bsp_psprel\(pspoff=0x10-0x18\)
[[:space:]]*P8:bspstore_when\(t=2\)
[[:space:]]*P8:bspstore_psprel\(pspoff=0x10-0x20\)
[[:space:]]*P7:fpsr_when\(t=3\)
[[:space:]]*P7:fpsr_psprel\(pspoff=0x10-0x28\)
[[:space:]]*P7:lc_when\(t=4\)
[[:space:]]*P7:lc_psprel\(pspoff=0x10-0x30\)
[[:space:]]*P7:pfs_when\(t=5\)
[[:space:]]*P7:pfs_psprel\(pspoff=0x10-0x38\)
[[:space:]]*P8:rnat_when\(t=6\)
[[:space:]]*P8:rnat_psprel\(pspoff=0x10-0x40\)
[[:space:]]*P7:unat_when\(t=7\)
[[:space:]]*P7:unat_psprel\(pspoff=0x10-0x48\)
[[:space:]]*P7:pr_when\(t=8\)
[[:space:]]*P7:pr_psprel\(pspoff=0x10-0x50\)
[[:space:]]*P8:priunat_when_mem\(t=9\)
[[:space:]]*P8:priunat_psprel\(pspoff=0x10-0x58\)
[[:space:]]*P7:rp_when\(t=10\)
[[:space:]]*P7:rp_psprel\(pspoff=0x10-0x60\)
[[:space:]]*R1:body\(rlen=1\)
#...
<simple>: \[0x[[:xdigit:]]*0-0x[[:xdigit:]]*0\], info at \+0x[[:xdigit:]]*[08]
[[:space:]]*v[[:digit:]]+, flags=0x0( \(\))?, len=[[:digit:]]+ bytes
#pass
