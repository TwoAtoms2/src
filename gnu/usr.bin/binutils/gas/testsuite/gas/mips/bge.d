#objdump: -dr --prefix-addresses
#name: MIPS bge

# Test the bge macro.

.*: +file format .*mips.*

Disassembly of section .text:
0+0000 <[^>]*> slt	\$at,\$a0,\$a1
0+0004 <[^>]*> beqz	\$at,0+0000 <text_label>
0+0008 <[^>]*> nop
0+000c <[^>]*> bgez	\$a0,0+0000 <text_label>
0+0010 <[^>]*> nop
0+0014 <[^>]*> blez	\$a1,0+0000 <text_label>
0+0018 <[^>]*> nop
0+001c <[^>]*> bgez	\$a0,0+0000 <text_label>
0+0020 <[^>]*> nop
0+0024 <[^>]*> bgtz	\$a0,0+0000 <text_label>
0+0028 <[^>]*> nop
0+002c <[^>]*> slti	\$at,\$a0,2
0+0030 <[^>]*> beqz	\$at,0+0000 <text_label>
0+0034 <[^>]*> nop
0+0038 <[^>]*> li	\$at,0x8000
0+003c <[^>]*> slt	\$at,\$a0,\$at
0+0040 <[^>]*> beqz	\$at,0+0000 <text_label>
0+0044 <[^>]*> nop
0+0048 <[^>]*> slti	\$at,\$a0,-32768
0+004c <[^>]*> beqz	\$at,0+0000 <text_label>
0+0050 <[^>]*> nop
0+0054 <[^>]*> lui	\$at,0x1
0+0058 <[^>]*> slt	\$at,\$a0,\$at
0+005c <[^>]*> beqz	\$at,0+0000 <text_label>
0+0060 <[^>]*> nop
0+0064 <[^>]*> lui	\$at,0x1
0+0068 <[^>]*> ori	\$at,\$at,0xa5a5
0+006c <[^>]*> slt	\$at,\$a0,\$at
0+0070 <[^>]*> beqz	\$at,0+0000 <text_label>
0+0074 <[^>]*> nop
0+0078 <[^>]*> slt	\$at,\$a1,\$a0
0+007c <[^>]*> bnez	\$at,0+0000 <text_label>
0+0080 <[^>]*> nop
0+0084 <[^>]*> bgtz	\$a0,0+0000 <text_label>
0+0088 <[^>]*> nop
0+008c <[^>]*> bltz	\$a1,0+0000 <text_label>
0+0090 <[^>]*> nop
0+0094 <[^>]*> bgtz	\$a0,0+0000 <text_label>
0+0098 <[^>]*> nop
0+009c <[^>]*> slt	\$at,\$a0,\$a1
0+00a0 <[^>]*> beqzl	\$at,0+0000 <text_label>
0+00a4 <[^>]*> nop
0+00a8 <[^>]*> slt	\$at,\$a1,\$a0
0+00ac <[^>]*> bnezl	\$at,0+0000 <text_label>
	...
