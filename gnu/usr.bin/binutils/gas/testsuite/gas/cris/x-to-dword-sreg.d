#objdump: -dr
#name: x-to-dword-sreg-@OR@

.*:     file format .*-cris

Disassembly of section \.text:
00000000 <notstart>:
[	 ]+0:[	 ]+0000[ 	]+bcc ( 0x2|\.\+2)
^[ 	]+\.\.\.
00000004 <start>:
[	 ]+4:[	 ]+@IR+3a06@[ 	]+move[ ]+r10,.*
[	 ]+6:[	 ]+@IM+3a0a@[ 	]+move[ ]+\[r10\],.*
[	 ]+8:[	 ]+@IM+3a0e@[ 	]+move[ ]+\[r10\+\],.*
[	 ]+a:[	 ]+@IM+3f0e@ 0000 0000[ 	]+move[ ]+(0x0|0 <notstart>),.*
[	 ]+10:[	 ]+@IM+3f0e@ 0100 0000[ 	]+move[ ]+(0x1|1 <notstart\+0x1>),.*
[	 ]+16:[	 ]+@IM+3f0e@ 7f00 0000[ 	]+move[ ]+(0x7f|7f <start\+0x7b>),.*
[	 ]+1c:[	 ]+@IM+3f0e@ 8000 0000[ 	]+move[ ]+(0x80|80 <start\+0x7c>),.*
[	 ]+22:[	 ]+@IM+3f0e@ ffff ffff[ 	]+move[ ]+0xffffffff,.*
[	 ]+28:[	 ]+@IM+3f0e@ 81ff ffff[ 	]+move[ ]+0xffffff81,.*
[	 ]+2e:[	 ]+@IM+3f0e@ 80ff ffff[ 	]+move[ ]+0xffffff80,.*
[	 ]+34:[	 ]+@IM+3f0e@ ff00 0000[ 	]+move[ ]+(0xff|ff <start\+0xfb>),.*
[	 ]+3a:[	 ]+@IM+3f0e@ 01ff ffff[ 	]+move[ ]+0xffffff01,.*
[	 ]+40:[	 ]+@IM+3f0e@ 0001 0000[ 	]+move[ ]+(0x[0]?100|100 <start\+0xfc>),.*
[	 ]+46:[	 ]+@IM+3f0e@ 68dd ffff[ 	]+move[ ]+0xffffdd68,.*
[	 ]+4c:[	 ]+@IM+3f0e@ 9822 0000[ 	]+move[ ]+(0x2298|2298 <end\+0x1ef0>),.*
[	 ]+52:[	 ]+@IM+3f0e@ ac72 2a00[ 	]+move[ ]+(0x2a72ac|2a72ac <two701867\+0x13881>),.*
[	 ]+58:[	 ]+@IM+3f0e@ d5c5 d6ff[ 	]+move[ ]+0xffd6c5d5,.*
[	 ]+5e:[	 ]+@IM+3f0e@ acce c09e[ 	]+move[ ]+(0x9ec0ceac|9ec0ceac <const_int_m32\+0x70281ef7>),.*
[	 ]+64:[	 ]+@IM+3f0e@ 5331 3f81[ 	]+move[ ]+(0x813f3153|813f3153 <const_int_m32\+0x52a6819e>),.*
[	 ]+6a:[	 ]+@IM+3f0e@ b5af 982e[ 	]+move[ ]+(0x2e98afb5|2e98afb5 <const_int_m32>),.*
[	 ]+70:[	 ]+@IM+3f0e@ 2b45 941b[ 	]+move[ ]+(0x1b94452b|1b94452b <const_int_32>),.*
[	 ]+76:[	 ]+@IM+3f0e@ 2a00 0000[ 	]+move[ ]+(0x2a|2a <start\+0x26>),.*
[	 ]+7c:[	 ]+@IM+3f0e@ d6ff ffff[ 	]+move[ ]+0xffffffd6,.*
[	 ]+82:[	 ]+@IM+3f0e@ 2a00 0000[ 	]+move[ ]+(0x2a|2a <start\+0x26>),.*
[	 ]+88:[	 ]+@IM+3f0e@ d6ff ffff[ 	]+move[ ]+0xffffffd6,.*
[	 ]+8e:[	 ]+@IM+3f0e@ d6ff ffff[ 	]+move[ ]+0xffffffd6,.*
[	 ]+94:[	 ]+@IM+3f0e@ 2a00 0000[ 	]+move[ ]+(0x2a|2a <start\+0x26>),.*
[	 ]+9a:[	 ]+@IM+3f0e@ ff7f 0000[ 	]+move[ ]+(0x7fff|7fff <three2767>),.*
[	 ]+a0:[	 ]+@IM+3f0e@ 0080 0000[ 	]+move[ ]+(0x8000|8000 <three2767\+0x1>),.*
[	 ]+a6:[	 ]+@IM+3f0e@ 0180 0000[ 	]+move[ ]+(0x8001|8001 <three2767\+0x2>),.*
[	 ]+ac:[	 ]+@IM+3f0e@ 0180 ffff[ 	]+move[ ]+0xffff8001,.*
[	 ]+b2:[	 ]+@IM+3f0e@ 0080 ffff[ 	]+move[ ]+0xffff8000,.*
[	 ]+b8:[	 ]+@IM+3f0e@ ff7f ffff[ 	]+move[ ]+0xffff7fff,.*
[	 ]+be:[	 ]+@IM+3f0e@ ffff 0000[ 	]+move[ ]+(0xffff|ffff <six5535>),.*
[	 ]+c4:[	 ]+@IM+3f0e@ 0000 0100[ 	]+move[ ]+(0x[0]?10000|10000 <six5535\+0x1>),.*
[	 ]+ca:[	 ]+@IM+3f0e@ 2b3a 2900[ 	]+move[ ]+(0x293a2b|293a2b <two701867>),.*
[	 ]+d0:[	 ]+@IM+3f0e@ d5c5 d6ff[ 	]+move[ ]+(0xffd6c5d5|ffd6c5d5 <const_int_m32\+0xd13e1620>),.*
[	 ]+d6:[	 ]+@IM+3f0e@ 0000 0000[ 	]+move[ ]+(0x0|0 <notstart>),.*
[ 	]+d8:[ 	]+(R_CRIS_)?32[ 	]+externalsym
[	 ]+dc:[	 ]+42a5 @IM+300a@[ 	]+move[ ]+\[r2\+r10\.b\],.*
[	 ]+e0:[	 ]+4a29 @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\]\.b\],.*
[	 ]+e4:[	 ]+4a2d @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\+\]\.b\],.*
[	 ]+e8:[	 ]+4a2d @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\+\]\.b\],.*
[	 ]+ec:[	 ]+52a5 @IM+300a@[ 	]+move[ ]+\[r2\+r10\.w\],.*
[	 ]+f0:[	 ]+5a29 @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\]\.w\],.*
[	 ]+f4:[	 ]+5a2d @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\+\]\.w\],.*
[	 ]+f8:[	 ]+5a2d @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\+\]\.w\],.*
[	 ]+fc:[	 ]+62a5 @IM+300a@[ 	]+move[ ]+\[r2\+r10\.d\],.*
[	 ]+100:[	 ]+6a29 @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\]\.d\],.*
[	 ]+104:[	 ]+6a2d @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\+\]\.d\],.*
[	 ]+108:[	 ]+6a2d @IM+300a@[ 	]+move[ ]+\[r2\+\[r10\+\]\.d\],.*
[	 ]+10c:[	 ]+0021 @IM+300a@[ 	]+move[ ]+\[r2\+0\],.*
[	 ]+110:[	 ]+0121 @IM+300a@[ 	]+move[ ]+\[r2\+1\],.*
[	 ]+114:[	 ]+7f21 @IM+300a@[ 	]+move[ ]+\[r2\+127\],.*
[	 ]+118:[	 ]+5f2d 8000 @IM+300a@[ 	]+move[ ]+\[r2\+128\],.*
[	 ]+11e:[	 ]+ff21 @IM+300a@[ 	]+move[ ]+\[r2-1\],.*
[	 ]+122:[	 ]+ff21 @IM+300a@[ 	]+move[ ]+\[r2-1\],.*
[	 ]+126:[	 ]+8121 @IM+300a@[ 	]+move[ ]+\[r2-127\],.*
[	 ]+12a:[	 ]+8021 @IM+300a@[ 	]+move[ ]+\[r2-128\],.*
[	 ]+12e:[	 ]+8121 @IM+300a@[ 	]+move[ ]+\[r2-127\],.*
[	 ]+132:[	 ]+8021 @IM+300a@[ 	]+move[ ]+\[r2-128\],.*
[	 ]+136:[	 ]+5f2d ff00 @IM+300a@[ 	]+move[ ]+\[r2\+255\],.*
[	 ]+13c:[	 ]+5f2d 01ff @IM+300a@[ 	]+move[ ]+\[r2-255\],.*
[	 ]+142:[	 ]+5f2d 01ff @IM+300a@[ 	]+move[ ]+\[r2-255\],.*
[	 ]+148:[	 ]+5f2d 0001 @IM+300a@[ 	]+move[ ]+\[r2\+256\],.*
[	 ]+14e:[	 ]+5f2d 00ff @IM+300a@[ 	]+move[ ]+\[r2-256\],.*
[	 ]+154:[	 ]+5f2d 68dd @IM+300a@[ 	]+move[ ]+\[r2-8856\],.*
[	 ]+15a:[	 ]+5f2d 00ff @IM+300a@[ 	]+move[ ]+\[r2-256\],.*
[	 ]+160:[	 ]+5f2d 68dd @IM+300a@[ 	]+move[ ]+\[r2-8856\],.*
[	 ]+166:[	 ]+5f2d 9822 @IM+300a@[ 	]+move[ ]+\[r2\+8856\],.*
[	 ]+16c:[	 ]+6f2d ac72 2a00 @IM+300a@[ 	]+move[ ]+\[r2\+(2781868|2a72ac <two701867\+0x13881>)\],.*
[	 ]+174:[	 ]+6f2d d5c5 d6ff @IM+300a@[ 	]+move[ ]+\[r2\+(0xffd6c5d5|ffd6c5d5 <const_int_m32\+0xd13e1620>)\],.*
[	 ]+17c:[	 ]+6f2d acce c09e @IM+300a@[ 	]+move[ ]+\[r2\+(0x9ec0ceac|9ec0ceac <const_int_m32\+0x70281ef7>)\],.*
[	 ]+184:[	 ]+6f2d 5331 3f81 @IM+300a@[ 	]+move[ ]+\[r2\+(0x813f3153|813f3153 <const_int_m32\+0x52a6819e>)\],.*
[	 ]+18c:[	 ]+6f2d 5331 3f81 @IM+300a@[ 	]+move[ ]+\[r2\+(0x813f3153|813f3153 <const_int_m32\+0x52a6819e>)\],.*
[	 ]+194:[	 ]+6f2d b5af 982e @IM+300a@[ 	]+move[ ]+\[r2\+(0x2e98afb5|2e98afb5 <const_int_m32>)\],.*
[	 ]+19c:[	 ]+6f2d 2b45 941b @IM+300a@[ 	]+move[ ]+\[r2\+(0x1b94452b|1b94452b <const_int_32>)\],.*
[	 ]+1a4:[	 ]+2a21 @IM+300a@[ 	]+move[ ]+\[r2\+42\],.*
[	 ]+1a8:[	 ]+d621 @IM+300a@[ 	]+move[ ]+\[r2-42\],.*
[	 ]+1ac:[	 ]+d621 @IM+300a@[ 	]+move[ ]+\[r2-42\],.*
[	 ]+1b0:[	 ]+2a21 @IM+300a@[ 	]+move[ ]+\[r2\+42\],.*
[	 ]+1b4:[	 ]+d621 @IM+300a@[ 	]+move[ ]+\[r2-42\],.*
[	 ]+1b8:[	 ]+d621 @IM+300a@[ 	]+move[ ]+\[r2-42\],.*
[	 ]+1bc:[	 ]+2a21 @IM+300a@[ 	]+move[ ]+\[r2\+42\],.*
[	 ]+1c0:[	 ]+d621 @IM+300a@[ 	]+move[ ]+\[r2-42\],.*
[	 ]+1c4:[	 ]+2a21 @IM+300a@[ 	]+move[ ]+\[r2\+42\],.*
[	 ]+1c8:[	 ]+5f2d ff7f @IM+300a@[ 	]+move[ ]+\[r2\+32767\],.*
[	 ]+1ce:[	 ]+6f2d 0080 0000 @IM+300a@[ 	]+move[ ]+\[r2\+(32768|8000 <three2767\+0x1>)\],.*
[	 ]+1d6:[	 ]+6f2d 0180 0000 @IM+300a@[ 	]+move[ ]+\[r2\+(32769|8001 <three2767\+0x2>)\],.*
[	 ]+1de:[	 ]+5f2d 0180 @IM+300a@[ 	]+move[ ]+\[r2-32767\],.*
[	 ]+1e4:[	 ]+5f2d 0080 @IM+300a@[ 	]+move[ ]+\[r2-32768\],.*
[	 ]+1ea:[	 ]+6f2d ff7f ffff @IM+300a@[ 	]+move[ ]+\[r2\+(0xffff7fff|ffff7fff <const_int_m32\+0xd166d04a>)\],.*
[	 ]+1f2:[	 ]+5f2d 0180 @IM+300a@[ 	]+move[ ]+\[r2-32767\],.*
[	 ]+1f8:[	 ]+5f2d 0080 @IM+300a@[ 	]+move[ ]+\[r2-32768\],.*
[	 ]+1fe:[	 ]+6f2d ff7f ffff @IM+300a@[ 	]+move[ ]+\[r2\+(0xffff7fff|ffff7fff <const_int_m32\+0xd166d04a>)\],.*
[	 ]+206:[	 ]+6f2d ffff 0000 @IM+300a@[ 	]+move[ ]+\[r2\+(65535|ffff <six5535>)\],.*
[	 ]+20e:[	 ]+6f2d 0000 0100 @IM+300a@[ 	]+move[ ]+\[r2\+(65536|10000 <six5535\+0x1>)\],.*
[	 ]+216:[	 ]+6f2d 2b3a 2900 @IM+300a@[ 	]+move[ ]+\[r2\+(2701867|293a2b <two701867>)\],.*
[	 ]+21e:[	 ]+6f2d d5c5 d6ff @IM+300a@[ 	]+move[ ]+\[r2\+(0xffd6c5d5|ffd6c5d5 <const_int_m32\+0xd13e1620>)\],.*
[	 ]+226:[	 ]+6f2d d5c5 d6ff @IM+300a@[ 	]+move[ ]+\[r2\+(0xffd6c5d5|ffd6c5d5 <const_int_m32\+0xd13e1620>)\],.*
[	 ]+22e:[	 ]+6f2d 0000 0000 @IM+300a@[ 	]+move[ ]+\[r2\+0( <notstart>)?\],.*
[ 	]+230:[ 	]+(R_CRIS_)?32[ 	]+externalsym
[	 ]+236:[	 ]+42a5 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+r10\.b\],.*
[	 ]+23a:[	 ]+4a29 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\]\.b\],.*
[	 ]+23e:[	 ]+4a2d @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\+\]\.b\],.*
[	 ]+242:[	 ]+4a2d @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\+\]\.b\],.*
[	 ]+246:[	 ]+52a5 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+r10\.w\],.*
[	 ]+24a:[	 ]+5a29 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\]\.w\],.*
[	 ]+24e:[	 ]+5a2d @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\+\]\.w\],.*
[	 ]+252:[	 ]+5a2d @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\+\]\.w\],.*
[	 ]+256:[	 ]+62a5 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+r10\.d\],.*
[	 ]+25a:[	 ]+6a29 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\]\.d\],.*
[	 ]+25e:[	 ]+6a2d @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\+\]\.d\],.*
[	 ]+262:[	 ]+6a2d @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+\[r10\+\]\.d\],.*
[	 ]+266:[	 ]+0021 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+0\],.*
[	 ]+26a:[	 ]+0121 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+1\],.*
[	 ]+26e:[	 ]+7f21 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+127\],.*
[	 ]+272:[	 ]+5f2d 8000 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+128\],.*
[	 ]+278:[	 ]+ff21 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-1\],.*
[	 ]+27c:[	 ]+ff21 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-1\],.*
[	 ]+280:[	 ]+8121 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-127\],.*
[	 ]+284:[	 ]+8021 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-128\],.*
[	 ]+288:[	 ]+8121 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-127\],.*
[	 ]+28c:[	 ]+8021 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-128\],.*
[	 ]+290:[	 ]+5f2d ff00 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+255\],.*
[	 ]+296:[	 ]+5f2d 01ff @IM+3c0e@[ 	]+move[ ]+\[r12=r2-255\],.*
[	 ]+29c:[	 ]+5f2d 01ff @IM+3c0e@[ 	]+move[ ]+\[r12=r2-255\],.*
[	 ]+2a2:[	 ]+5f2d 0001 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+256\],.*
[	 ]+2a8:[	 ]+5f2d 00ff @IM+3c0e@[ 	]+move[ ]+\[r12=r2-256\],.*
[	 ]+2ae:[	 ]+5f2d 68dd @IM+3c0e@[ 	]+move[ ]+\[r12=r2-8856\],.*
[	 ]+2b4:[	 ]+5f2d 00ff @IM+3c0e@[ 	]+move[ ]+\[r12=r2-256\],.*
[	 ]+2ba:[	 ]+5f2d 68dd @IM+3c0e@[ 	]+move[ ]+\[r12=r2-8856\],.*
[	 ]+2c0:[	 ]+5f2d 9822 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+8856\],.*
[	 ]+2c6:[	 ]+6f2d ac72 2a00 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(2781868|2a72ac <two701867\+0x13881>)\],.*
[	 ]+2ce:[	 ]+6f2d d5c5 d6ff @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0xffd6c5d5|ffd6c5d5 <const_int_m32\+0xd13e1620>)\],.*
[	 ]+2d6:[	 ]+6f2d acce c09e @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0x9ec0ceac|9ec0ceac <const_int_m32\+0x70281ef7>)\],.*
[	 ]+2de:[	 ]+6f2d 5331 3f81 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0x813f3153|813f3153 <const_int_m32\+0x52a6819e>)\],.*
[	 ]+2e6:[	 ]+6f2d 5331 3f81 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0x813f3153|813f3153 <const_int_m32\+0x52a6819e>)\],.*
[	 ]+2ee:[	 ]+6f2d b5af 982e @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0x2e98afb5|2e98afb5 <const_int_m32>)\],.*
[	 ]+2f6:[	 ]+6f2d 2b45 941b @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0x1b94452b|1b94452b <const_int_32>)\],.*
[	 ]+2fe:[	 ]+2a21 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+42\],.*
[	 ]+302:[	 ]+d621 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-42\],.*
[	 ]+306:[	 ]+d621 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-42\],.*
[	 ]+30a:[	 ]+2a21 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+42\],.*
[	 ]+30e:[	 ]+d621 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-42\],.*
[	 ]+312:[	 ]+d621 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-42\],.*
[	 ]+316:[	 ]+2a21 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+42\],.*
[	 ]+31a:[	 ]+d621 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-42\],.*
[	 ]+31e:[	 ]+2a21 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+42\],.*
[	 ]+322:[	 ]+5f2d ff7f @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+32767\],.*
[	 ]+328:[	 ]+6f2d 0080 0000 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(32768|8000 <three2767\+0x1>)\],.*
[	 ]+330:[	 ]+6f2d 0180 0000 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(32769|8001 <three2767\+0x2>)\],.*
[	 ]+338:[	 ]+5f2d 0180 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-32767\],.*
[	 ]+33e:[	 ]+5f2d 0080 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-32768\],.*
[	 ]+344:[	 ]+6f2d ff7f ffff @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0xffff7fff|ffff7fff <const_int_m32\+0xd166d04a>)\],.*
[	 ]+34c:[	 ]+5f2d 0180 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-32767\],.*
[	 ]+352:[	 ]+5f2d 0080 @IM+3c0e@[ 	]+move[ ]+\[r12=r2-32768\],.*
[	 ]+358:[	 ]+6f2d ff7f ffff @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0xffff7fff|ffff7fff <const_int_m32\+0xd166d04a>)\],.*
[	 ]+360:[	 ]+6f2d ffff 0000 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(65535|ffff <six5535>)\],.*
[	 ]+368:[	 ]+6f2d 0000 0100 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(65536|10000 <six5535\+0x1>)\],.*
[	 ]+370:[	 ]+6f2d 2b3a 2900 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(2701867|293a2b <two701867>)\],.*
[	 ]+378:[	 ]+6f2d d5c5 d6ff @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0xffd6c5d5|ffd6c5d5 <const_int_m32\+0xd13e1620>)\],.*
[	 ]+380:[	 ]+6f2d d5c5 d6ff @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+(0xffd6c5d5|ffd6c5d5 <const_int_m32\+0xd13e1620>)\],.*
[	 ]+388:[	 ]+6f2d 0000 0000 @IM+3c0e@[ 	]+move[ ]+\[r12=r2\+0( <notstart>)?\],.*
[ 	]+38a:[ 	]+(R_CRIS_)?32[ 	]+externalsym
[	 ]+390:[	 ]+7309 @IM+300a@[ 	]+move[ ]+\[\[r3\]\],.*
[	 ]+394:[	 ]+710d @IM+300a@[ 	]+move[ ]+\[\[r1\+\]\],.*
[	 ]+398:[	 ]+7f0d 0000 0000 @IM+300a@[ 	]+move[ ]+\[(0x0|0 <notstart>)\],.*
[ 	]+39a:[ 	]+(R_CRIS_)?32[ 	]+externalsym
[	 ]+3a0:[	 ]+7f0d 0000 0000 @IM+300a@[ 	]+move[ ]+\[(0x0|0 <notstart>)\],.*
[ 	]+3a2:[ 	]+(R_CRIS_)?32[ 	]+\.text

