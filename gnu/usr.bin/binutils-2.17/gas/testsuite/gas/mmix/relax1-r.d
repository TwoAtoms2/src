#objdump: -dr
#as: -linkrelax -x
#source: relax1.s
.*:     file format elf64-mmix

Disassembly of section \.text:

0000000000000000 <Main>:
       0:	f0020019 	jmp 80064 <l6>
			0: R_MMIX_ADDR27	\.text\+0x80064

0000000000000004 <l0>:
       4:	f0020018 	jmp 80064 <l6>
			4: R_MMIX_ADDR27	\.text\+0x80064

0000000000000008 <l1>:
       8:	f0020017 	jmp 80064 <l6>
			8: R_MMIX_ADDR27	\.text\+0x80064

000000000000000c <l01>:
       c:	f0020016 	jmp 80064 <l6>
			c: R_MMIX_ADDR27	\.text\+0x80064
      10:	f407ffff 	geta \$7,4000c <nearfar1>
			10: R_MMIX_ADDR19	\.text\+0x4000c
      14:	f2bfffff 	pushj \$191,40010 <nearfar2>
			14: R_MMIX_ADDR19	\.text\+0x40010

0000000000000018 <l2>:
      18:	f000fffe 	jmp 40010 <nearfar2>
			18: R_MMIX_ADDR27	\.text\+0x40010
	\.\.\.
   40004:	4d480000 	bnp \$72,4 <l0>
			40004: R_MMIX_ADDR19	\.text\+0x4
   40008:	f5040000 	geta \$4,8 <l1>
			40008: R_MMIX_ADDR19	\.text\+0x8

000000000004000c <nearfar1>:
   4000c:	f3050000 	pushj \$5,c <l01>
			4000c: R_MMIX_ADDR19	\.text\+0xc

0000000000040010 <nearfar2>:
   40010:	f4090000 	geta \$9,40010 <nearfar2>
			40010: R_MMIX_GETA	\.text\+0x8
   40014:	fd000000 	swym 0,0,0
   40018:	fd000000 	swym 0,0,0
   4001c:	fd000000 	swym 0,0,0
   40020:	f20b0000 	pushj \$11,40020 <nearfar2\+0x10>
			40020: R_MMIX_PUSHJ	\.text\+0x80030
   40024:	fd000000 	swym 0,0,0
   40028:	fd000000 	swym 0,0,0
   4002c:	fd000000 	swym 0,0,0
   40030:	fd000000 	swym 0,0,0

0000000000040034 <l4>:
   40034:	4437ffff 	bp \$55,80030 <l3>
			40034: R_MMIX_ADDR19	\.text\+0x80030
	\.\.\.
   8002c:	f1fdfff7 	jmp 8 <l1>
			8002c: R_MMIX_ADDR27	\.text\+0x8

0000000000080030 <l3>:
   80030:	f1fdfff5 	jmp 4 <l0>
			80030: R_MMIX_ADDR27	\.text\+0x4
   80034:	47580000 	bod \$88,40034 <l4>
			80034: R_MMIX_ADDR19	\.text\+0x40034
   80038:	46580000 	bod \$88,80038 <l3\+0x8>
			80038: R_MMIX_CBRANCH	\.text\+0x40034
   8003c:	fd000000 	swym 0,0,0
   80040:	fd000000 	swym 0,0,0
   80044:	fd000000 	swym 0,0,0
   80048:	fd000000 	swym 0,0,0
   8004c:	fd000000 	swym 0,0,0
   80050:	f0000000 	jmp 80050 <l3\+0x20>
			80050: R_MMIX_JMP	\.text\+0x4080060
   80054:	fd000000 	swym 0,0,0
   80058:	fd000000 	swym 0,0,0
   8005c:	fd000000 	swym 0,0,0
   80060:	fd000000 	swym 0,0,0

0000000000080064 <l6>:
   80064:	f0ffffff 	jmp 4080060 <l5>
			80064: R_MMIX_ADDR27	\.text\+0x4080060
   80068:	436ffff2 	bz \$111,80030 <l3>
			80068: R_MMIX_ADDR19	\.text\+0x80030
	\.\.\.

0000000004080060 <l5>:
 4080060:	f000000d 	jmp 4080094 <l8>
			4080060: R_MMIX_ADDR27	\.text\+0x4080094
 4080064:	f1000000 	jmp 80064 <l6>
			4080064: R_MMIX_ADDR27	\.text\+0x80064
 4080068:	f0000000 	jmp 4080068 <l5\+0x8>
			4080068: R_MMIX_JMP	\.text\+0x80064
 408006c:	fd000000 	swym 0,0,0
 4080070:	fd000000 	swym 0,0,0
 4080074:	fd000000 	swym 0,0,0
 4080078:	fd000000 	swym 0,0,0
 408007c:	482c0000 	bnn \$44,408007c <l5\+0x1c>
			408007c: R_MMIX_CBRANCH	\.text\+0x40c0090
 4080080:	fd000000 	swym 0,0,0
 4080084:	fd000000 	swym 0,0,0
 4080088:	fd000000 	swym 0,0,0
 408008c:	fd000000 	swym 0,0,0
 4080090:	fd000000 	swym 0,0,0

0000000004080094 <l8>:
 4080094:	482cffff 	bnn \$44,40c0090 <l9>
			4080094: R_MMIX_ADDR19	\.text\+0x40c0090
 4080098:	f1fffff2 	jmp 4080060 <l5>
			4080098: R_MMIX_ADDR27	\.text\+0x4080060
 408009c:	f1fffff1 	jmp 4080060 <l5>
			408009c: R_MMIX_ADDR27	\.text\+0x4080060
	\.\.\.

00000000040c008c <l10>:
 40c008c:	f1fefff5 	jmp 4080060 <l5>
			40c008c: R_MMIX_ADDR27	\.text\+0x4080060

00000000040c0090 <l9>:
 40c0090:	f0000007 	jmp 40c00ac <l11>
			40c0090: R_MMIX_ADDR27	\.text\+0x40c00ac

00000000040c0094 <l7>:
 40c0094:	f3210000 	pushj \$33,4080094 <l8>
			40c0094: R_MMIX_ADDR19	\.text\+0x4080094
 40c0098:	f2210000 	pushj \$33,40c0098 <l7\+0x4>
			40c0098: R_MMIX_PUSHJ	\.text\+0x4080094
 40c009c:	fd000000 	swym 0,0,0
 40c00a0:	fd000000 	swym 0,0,0
 40c00a4:	fd000000 	swym 0,0,0
 40c00a8:	fd000000 	swym 0,0,0

00000000040c00ac <l11>:
 40c00ac:	f1feffed 	jmp 4080060 <l5>
			40c00ac: R_MMIX_ADDR27	\.text\+0x4080060
 40c00b0:	f1fefff9 	jmp 4080094 <l8>
			40c00b0: R_MMIX_ADDR27	\.text\+0x4080094
	\.\.\.
 41000ac:	f53d0000 	geta \$61,40c00ac <l11>
			41000ac: R_MMIX_ADDR19	\.text\+0x40c00ac
 41000b0:	f4480000 	geta \$72,41000b0 <l11\+0x40004>
			41000b0: R_MMIX_GETA	\.text\+0x40c00ac
 41000b4:	fd000000 	swym 0,0,0
 41000b8:	fd000000 	swym 0,0,0
 41000bc:	fd000000 	swym 0,0,0
