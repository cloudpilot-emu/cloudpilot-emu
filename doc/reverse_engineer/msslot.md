# get_int: 0x0070 - 0x00c2


```

      70:	4e56 fffe      	linkw %fp,#-2
      74:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
      78:	266e 0008      	moveal %fp@(8),%a3
      7c:	246e 000c      	moveal %fp@(12),%a2
      80:	7603           	moveq #3,%d3
      82:	2f0a           	movel %a2,%sp@-
      84:	486e fffe      	pea %fp@(-2)
      88:	3f3c 0001      	movew #1,%sp@-
      8c:	1f3c 0007      	moveb #7,%sp@-
      90:	4eba 20c6      	jsr %pc@(0x2158)
      94:	4a00           	tstb %d0
      96:	4fef 000c      	lea %sp@(12),%sp
      9a:	6706           	beqs 0xa2
      9c:	303c 2000      	movew #8192,%d0
      a0:	601a           	bras 0xbc
      a2:	1012           	moveb %a2@,%d0
      a4:	0240 0007      	andiw #7,%d0
      a8:	6608           	bnes 0xb2
      aa:	16ae fffe      	moveb %fp@(-2),%a3@
      ae:	7000           	moveq #0,%d0
      b0:	600a           	bras 0xbc
      b2:	5343           	subqw #1,%d3
      b4:	6ecc           	bgts 0x82
      b6:	16ae fffe      	moveb %fp@(-2),%a3@
      ba:	7008           	moveq #8,%d0
      bc:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
      c0:	4e5e           	unlk %fp
      c2:	4e75           	rts

```

# write_reg: 0x00ce - 0x011c


```

      ce:	4e56 0000      	linkw %fp,#0
      d2:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
      d6:	266e 0008      	moveal %fp@(8),%a3
      da:	182e 000c      	moveb %fp@(12),%d4
      de:	246e 000e      	moveal %fp@(14),%a2
      e2:	7603           	moveq #3,%d3
      e4:	1f04           	moveb %d4,%sp@-
      e6:	2f0a           	movel %a2,%sp@-
      e8:	2f0b           	movel %a3,%sp@-
      ea:	3f3c 000f      	movew #15,%sp@-
      ee:	1f3c 000b      	moveb #11,%sp@-
      f2:	4eba 1e50      	jsr %pc@(0x1f44)
      f6:	4a00           	tstb %d0
      f8:	4fef 000e      	lea %sp@(14),%sp
      fc:	6706           	beqs 0x104
      fe:	303c 2000      	movew #8192,%d0
     102:	6012           	bras 0x116
     104:	1012           	moveb %a2@,%d0
     106:	0240 0005      	andiw #5,%d0
     10a:	6604           	bnes 0x110
     10c:	7000           	moveq #0,%d0
     10e:	6006           	bras 0x116
     110:	5343           	subqw #1,%d3
     112:	6ed0           	bgts 0xe4
     114:	7008           	moveq #8,%d0
     116:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
     11a:	4e5e           	unlk %fp
     11c:	4e75           	rts

```

# read_reg: 0x012a - 0x0172


```

     12a:	4e56 0000      	linkw %fp,#0
     12e:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
     132:	266e 0008      	moveal %fp@(8),%a3
     136:	246e 000c      	moveal %fp@(12),%a2
     13a:	7603           	moveq #3,%d3
     13c:	2f0a           	movel %a2,%sp@-
     13e:	2f0b           	movel %a3,%sp@-
     140:	3f3c 001f      	movew #31,%sp@-
     144:	1f3c 0004      	moveb #4,%sp@-
     148:	4eba 200e      	jsr %pc@(0x2158)
     14c:	4a00           	tstb %d0
     14e:	4fef 000c      	lea %sp@(12),%sp
     152:	6706           	beqs 0x15a
     154:	303c 2000      	movew #8192,%d0
     158:	6012           	bras 0x16c
     15a:	1012           	moveb %a2@,%d0
     15c:	0240 0007      	andiw #7,%d0
     160:	6604           	bnes 0x166
     162:	7000           	moveq #0,%d0
     164:	6006           	bras 0x16c
     166:	5343           	subqw #1,%d3
     168:	6ed2           	bgts 0x13c
     16a:	7008           	moveq #8,%d0
     16c:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
     170:	4e5e           	unlk %fp
     172:	4e75           	rts

```

# set_cmd: 0x0180 - 0x01ea


```

     180:	4e56 fffe      	linkw %fp,#-2
     184:	48e7 1c20      	moveml %d3-%d5/%a2,%sp@-
     188:	1a2e 000a      	moveb %fp@(10),%d5
     18c:	246e 000c      	moveal %fp@(12),%a2
     190:	7000           	moveq #0,%d0
     192:	102e 0008      	moveb %fp@(8),%d0
     196:	e148           	lslw #8,%d0
     198:	3d40 fffe      	movew %d0,%fp@(-2)
     19c:	7803           	moveq #3,%d4
     19e:	1f05           	moveb %d5,%sp@-
     1a0:	2f0a           	movel %a2,%sp@-
     1a2:	486e fffe      	pea %fp@(-2)
     1a6:	3f3c 0001      	movew #1,%sp@-
     1aa:	1f3c 000e      	moveb #14,%sp@-
     1ae:	4eba 1d94      	jsr %pc@(0x1f44)
     1b2:	4a00           	tstb %d0
     1b4:	4fef 000e      	lea %sp@(14),%sp
     1b8:	6706           	beqs 0x1c0
     1ba:	303c 2000      	movew #8192,%d0
     1be:	6024           	bras 0x1e4
     1c0:	7000           	moveq #0,%d0
     1c2:	1012           	moveb %a2@,%d0
     1c4:	3600           	movew %d0,%d3
     1c6:	0240 0005      	andiw #5,%d0
     1ca:	6716           	beqs 0x1e2
     1cc:	3003           	movew %d3,%d0
     1ce:	0240 0004      	andiw #4,%d0
     1d2:	6706           	beqs 0x1da
     1d4:	303c 2000      	movew #8192,%d0
     1d8:	600a           	bras 0x1e4
     1da:	5344           	subqw #1,%d4
     1dc:	6ec0           	bgts 0x19e
     1de:	7008           	moveq #8,%d0
     1e0:	6002           	bras 0x1e4
     1e2:	7000           	moveq #0,%d0
     1e4:	4cdf 0438      	moveml %sp@+,%d3-%d5/%a2
     1e8:	4e5e           	unlk %fp
     1ea:	4e75           	rts

```

# wait_msInt: 0x01f6 - 0x027c


```

     1f6:	4e56 0000      	linkw %fp,#0
     1fa:	2f0a           	movel %a2,%sp@-
     1fc:	2f03           	movel %d3,%sp@-
     1fe:	246e 000a      	moveal %fp@(10),%a2
     202:	1012           	moveb %a2@,%d0
     204:	0240 0050      	andiw #80,%d0
     208:	664a           	bnes 0x254
     20a:	7000           	moveq #0,%d0
     20c:	102e 0008      	moveb %fp@(8),%d0
     210:	670e           	beqs 0x220
     212:	5340           	subqw #1,%d0
     214:	670e           	beqs 0x224
     216:	5340           	subqw #1,%d0
     218:	6712           	beqs 0x22c
     21a:	5340           	subqw #1,%d0
     21c:	6716           	beqs 0x234
     21e:	601c           	bras 0x23c
     220:	7600           	moveq #0,%d3
     222:	601e           	bras 0x242
     224:	263c 0000 1388 	movel #5000,%d3
     22a:	6016           	bras 0x242
     22c:	263c 0000 2710 	movel #10000,%d3
     232:	600e           	bras 0x242
     234:	263c 0001 86a0 	movel #100000,%d3
     23a:	6006           	bras 0x242
     23c:	263c 0000 03e8 	movel #1000,%d3
     242:	2f03           	movel %d3,%sp@-
     244:	2f0a           	movel %a2,%sp@-
     246:	4eba 2016      	jsr %pc@(0x225e)
     24a:	4a00           	tstb %d0
     24c:	504f           	addqw #8,%sp
     24e:	6704           	beqs 0x254
     250:	7000           	moveq #0,%d0
     252:	6022           	bras 0x276
     254:	7000           	moveq #0,%d0
     256:	1012           	moveb %a2@,%d0
     258:	3600           	movew %d0,%d3
     25a:	0240 0010      	andiw #16,%d0
     25e:	6706           	beqs 0x266
     260:	4212           	clrb %a2@
     262:	7002           	moveq #2,%d0
     264:	6010           	bras 0x276
     266:	3003           	movew %d3,%d0
     268:	0240 0040      	andiw #64,%d0
     26c:	6706           	beqs 0x274
     26e:	4212           	clrb %a2@
     270:	7001           	moveq #1,%d0
     272:	6002           	bras 0x276
     274:	7000           	moveq #0,%d0
     276:	261f           	movel %sp@+,%d3
     278:	245f           	moveal %sp@+,%a2
     27a:	4e5e           	unlk %fp
     27c:	4e75           	rts

```

# read_page_data: 0x028c - 0x02e0


```

     28c:	4e56 0000      	linkw %fp,#0
     290:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
     294:	266e 0008      	moveal %fp@(8),%a3
     298:	246e 000c      	moveal %fp@(12),%a2
     29c:	7803           	moveq #3,%d4
     29e:	2f0a           	movel %a2,%sp@-
     2a0:	2f0b           	movel %a3,%sp@-
     2a2:	3f3c 0200      	movew #512,%sp@-
     2a6:	1f3c 0002      	moveb #2,%sp@-
     2aa:	4eba 1eac      	jsr %pc@(0x2158)
     2ae:	4a00           	tstb %d0
     2b0:	4fef 000c      	lea %sp@(12),%sp
     2b4:	6706           	beqs 0x2bc
     2b6:	303c 2000      	movew #8192,%d0
     2ba:	601e           	bras 0x2da
     2bc:	7000           	moveq #0,%d0
     2be:	1012           	moveb %a2@,%d0
     2c0:	3600           	movew %d0,%d3
     2c2:	0240 0007      	andiw #7,%d0
     2c6:	6604           	bnes 0x2cc
     2c8:	7000           	moveq #0,%d0
     2ca:	600e           	bras 0x2da
     2cc:	3003           	movew %d3,%d0
     2ce:	0240 0006      	andiw #6,%d0
     2d2:	6604           	bnes 0x2d8
     2d4:	5344           	subqw #1,%d4
     2d6:	6ec6           	bgts 0x29e
     2d8:	7008           	moveq #8,%d0
     2da:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
     2de:	4e5e           	unlk %fp
     2e0:	4e75           	rts

```

# write_page_data: 0x02f4 - 0x0350


```

     2f4:	4e56 0000      	linkw %fp,#0
     2f8:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
     2fc:	266e 0008      	moveal %fp@(8),%a3
     300:	246e 000c      	moveal %fp@(12),%a2
     304:	7803           	moveq #3,%d4
     306:	4227           	clrb %sp@-
     308:	2f0a           	movel %a2,%sp@-
     30a:	2f0b           	movel %a3,%sp@-
     30c:	3f3c 0200      	movew #512,%sp@-
     310:	1f3c 000d      	moveb #13,%sp@-
     314:	4eba 1c2e      	jsr %pc@(0x1f44)
     318:	4a00           	tstb %d0
     31a:	4fef 000e      	lea %sp@(14),%sp
     31e:	6706           	beqs 0x326
     320:	303c 2000      	movew #8192,%d0
     324:	6024           	bras 0x34a
     326:	7000           	moveq #0,%d0
     328:	1012           	moveb %a2@,%d0
     32a:	3600           	movew %d0,%d3
     32c:	0240 0005      	andiw #5,%d0
     330:	6604           	bnes 0x336
     332:	7000           	moveq #0,%d0
     334:	6014           	bras 0x34a
     336:	3003           	movew %d3,%d0
     338:	0240 0004      	andiw #4,%d0
     33c:	6706           	beqs 0x344
     33e:	303c 2000      	movew #8192,%d0
     342:	6006           	bras 0x34a
     344:	5344           	subqw #1,%d4
     346:	6ebe           	bgts 0x306
     348:	7008           	moveq #8,%d0
     34a:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
     34e:	4e5e           	unlk %fp
     350:	4e75           	rts

```

# job_msDetectBoot: 0x0398 - 0x047e


```

     398:	4e56 fff6      	linkw %fp,#-10
     39c:	2f0a           	movel %a2,%sp@-
     39e:	2f03           	movel %d3,%sp@-
     3a0:	246e 0008      	moveal %fp@(8),%a2
     3a4:	357c ffff 4376 	movew #-1,%a2@(17270)
     3aa:	357c ffff 4378 	movew #-1,%a2@(17272)
     3b0:	7600           	moveq #0,%d3
     3b2:	486e fff7      	pea %fp@(-9)
     3b6:	486e fffc      	pea %fp@(-4)
     3ba:	486e fff8      	pea %fp@(-8)
     3be:	42a7           	clrl %sp@-
     3c0:	486a 4158      	pea %a2@(16728)
     3c4:	1f3c 0001      	moveb #1,%sp@-
     3c8:	4227           	clrb %sp@-
     3ca:	3f03           	movew %d3,%sp@-
     3cc:	4eba 2aea      	jsr %pc@(0x2eb8)
     3d0:	4a80           	tstl %d0
     3d2:	4fef 001a      	lea %sp@(26),%sp
     3d6:	6664           	bnes 0x43c
     3d8:	0c6a 0001 4158 	cmpiw #1,%a2@(16728)
     3de:	666c           	bnes 0x44c
     3e0:	0c6a ffff 4376 	cmpiw #-1,%a2@(17270)
     3e6:	6706           	beqs 0x3ee
     3e8:	3543 4378      	movew %d3,%a2@(17272)
     3ec:	6074           	bras 0x462
     3ee:	302a 42fa      	movew %a2@(17146),%d0
     3f2:	5140           	subqw #8,%d0
     3f4:	6706           	beqs 0x3fc
     3f6:	5140           	subqw #8,%d0
     3f8:	670a           	beqs 0x404
     3fa:	6010           	bras 0x40c
     3fc:	157c 0010 437d 	moveb #16,%a2@(17277)
     402:	600c           	bras 0x410
     404:	157c 0020 437d 	moveb #32,%a2@(17277)
     40a:	6004           	bras 0x410
     40c:	7010           	moveq #16,%d0
     40e:	6068           	bras 0x478
     410:	486e fff7      	pea %fp@(-9)
     414:	486e fffc      	pea %fp@(-4)
     418:	486e fff8      	pea %fp@(-8)
     41c:	42a7           	clrl %sp@-
     41e:	486a 0018      	pea %a2@(24)
     422:	1f2a 437d      	moveb %a2@(17277),%sp@-
     426:	4227           	clrb %sp@-
     428:	3f03           	movew %d3,%sp@-
     42a:	4eba 2a8c      	jsr %pc@(0x2eb8)
     42e:	4a80           	tstl %d0
     430:	4fef 001a      	lea %sp@(26),%sp
     434:	6616           	bnes 0x44c
     436:	3543 4376      	movew %d3,%a2@(17270)
     43a:	6010           	bras 0x44c
     43c:	202e fffc      	movel %fp@(-4),%d0
     440:	0280 0000 6000 	andil #24576,%d0
     446:	6704           	beqs 0x44c
     448:	7009           	moveq #9,%d0
     44a:	602c           	bras 0x478
     44c:	5243           	addqw #1,%d3
     44e:	0c43 0010      	cmpiw #16,%d3
     452:	6300 ff5e      	blsw 0x3b2
     456:	0c6a ffff 4376 	cmpiw #-1,%a2@(17270)
     45c:	6604           	bnes 0x462
     45e:	7010           	moveq #16,%d0
     460:	6016           	bras 0x478
     462:	4878 0200      	pea 0x200
     466:	486a 0018      	pea %a2@(24)
     46a:	486a 4158      	pea %a2@(16728)
     46e:	4e4f           	trap #15
     470:	a026           	0120046
     472:	7000           	moveq #0,%d0
     474:	4fef 000c      	lea %sp@(12),%sp
     478:	261f           	movel %sp@+,%d3
     47a:	245f           	moveal %sp@+,%a2
     47c:	4e5e           	unlk %fp
     47e:	4e75           	rts

```

# job_msMakeTable: 0x0494 - 0x088a


```

     494:	4e56 ffb0      	linkw %fp,#-80
     498:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
     49c:	206e 0008      	moveal %fp@(8),%a0
     4a0:	41e8 0018      	lea %a0@(24),%a0
     4a4:	2808           	movel %a0,%d4
     4a6:	206e 0008      	moveal %fp@(8),%a0
     4aa:	41e8 0218      	lea %a0@(536),%a0
     4ae:	2d48 ffd8      	movel %a0,%fp@(-40)
     4b2:	266e 0008      	moveal %fp@(8),%a3
     4b6:	47eb 0618      	lea %a3@(1560),%a3
     4ba:	41fa fea8      	lea %pc@(0x364),%a0
     4be:	2d48 ffb0      	movel %a0,%fp@(-80)
     4c2:	2d7c ffff f000 	movel #-4096,%fp@(-44)
     4c8:	ffd4
     4ca:	206e 0008      	moveal %fp@(8),%a0
     4ce:	70ef           	moveq #-17,%d0
     4d0:	c1a8 4382      	andl %d0,%a0@(17282)
     4d4:	3028 42fc      	movew %a0@(17148),%d0
     4d8:	7209           	moveq #9,%d1
     4da:	e268           	lsrw %d1,%d0
     4dc:	7200           	moveq #0,%d1
     4de:	3200           	movew %d0,%d1
     4e0:	2d41 ffb8      	movel %d1,%fp@(-72)
     4e4:	0c28 0001 42d0 	cmpib #1,%a0@(17104)
     4ea:	6706           	beqs 0x4f2
     4ec:	7010           	moveq #16,%d0
     4ee:	6000 0394      	braw 0x884
     4f2:	206e 0008      	moveal %fp@(8),%a0
     4f6:	4aa8 42c8      	tstl %a0@(17096)
     4fa:	6706           	beqs 0x502
     4fc:	7010           	moveq #16,%d0
     4fe:	6000 0384      	braw 0x884
     502:	206e 0008      	moveal %fp@(8),%a0
     506:	2a28 42cc      	movel %a0@(17100),%d5
     50a:	e285           	asrl #1,%d5
     50c:	486e ffe4      	pea %fp@(-28)
     510:	42a7           	clrl %sp@-
     512:	2f04           	movel %d4,%sp@-
     514:	1f3c 0001      	moveb #1,%sp@-
     518:	3f28 4376      	movew %a0@(17270),%sp@-
     51c:	4eba 2794      	jsr %pc@(0x2cb2)
     520:	4a80           	tstl %d0
     522:	4fef 0010      	lea %sp@(16),%sp
     526:	6706           	beqs 0x52e
     528:	7010           	moveq #16,%d0
     52a:	6000 0358      	braw 0x884
     52e:	7600           	moveq #0,%d3
     530:	2d44 ffd0      	movel %d4,%fp@(-48)
     534:	2444           	moveal %d4,%a2
     536:	0c52 ffff      	cmpiw #-1,%a2@
     53a:	670c           	beqs 0x548
     53c:	548a           	addql #2,%a2
     53e:	5283           	addql #1,%d3
     540:	0c83 0000 0100 	cmpil #256,%d3
     546:	65ee           	bcss 0x536
     548:	b685           	cmpl %d5,%d3
     54a:	671c           	beqs 0x568
     54c:	206e 0008      	moveal %fp@(8),%a0
     550:	117c 0001 437c 	moveb #1,%a0@(17276)
     556:	117c 0001 437b 	moveb #1,%a0@(17275)
     55c:	7002           	moveq #2,%d0
     55e:	81a8 0004      	orl %d0,%a0@(4)
     562:	7010           	moveq #16,%d0
     564:	81a8 4382      	orl %d0,%a0@(17282)
     568:	426e ffc2      	clrw %fp@(-62)
     56c:	426e ffbc      	clrw %fp@(-68)
     570:	2d6e ffb0 ffc4 	movel %fp@(-80),%fp@(-60)
     576:	42ae ffbe      	clrl %fp@(-66)
     57a:	6000 02f4      	braw 0x870
     57e:	7c00           	moveq #0,%d6
     580:	1f3c 00ff      	moveb #-1,%sp@-
     584:	4878 03e0      	pea 0x3e0
     588:	2f2e ffd8      	movel %fp@(-40),%sp@-
     58c:	4e4f           	trap #15
     58e:	a027           	0120047
     590:	204b           	moveal %a3,%a0
     592:	5088           	addql #8,%a0
     594:	5888           	addql #4,%a0
     596:	2688           	movel %a0,%a3@
     598:	426e ffee      	clrw %fp@(-18)
     59c:	3d6e ffbc ffec 	movew %fp@(-68),%fp@(-20)
     5a2:	2e2e ffc4      	movel %fp@(-60),%d7
     5a6:	302e ffc2      	movew %fp@(-62),%d0
     5aa:	5240           	addqw #1,%d0
     5ac:	7200           	moveq #0,%d1
     5ae:	3200           	movew %d0,%d1
     5b0:	d281           	addl %d1,%d1
     5b2:	206e ffb0      	moveal %fp@(-80),%a0
     5b6:	d1c1           	addal %d1,%a0
     5b8:	2d48 ffdc      	movel %a0,%fp@(-36)
     5bc:	4fef 000a      	lea %sp@(10),%sp
     5c0:	362e ffec      	movew %fp@(-20),%d3
     5c4:	7800           	moveq #0,%d4
     5c6:	4a85           	tstl %d5
     5c8:	6714           	beqs 0x5de
     5ca:	246e ffd0      	moveal %fp@(-48),%a2
     5ce:	b652           	cmpw %a2@,%d3
     5d0:	6604           	bnes 0x5d6
     5d2:	5286           	addql #1,%d6
     5d4:	6008           	bras 0x5de
     5d6:	548a           	addql #2,%a2
     5d8:	5284           	addql #1,%d4
     5da:	b885           	cmpl %d5,%d4
     5dc:	65f0           	bcss 0x5ce
     5de:	b885           	cmpl %d5,%d4
     5e0:	6500 0192      	bcsw 0x774
     5e4:	486e ffe4      	pea %fp@(-28)
     5e8:	486e fff6      	pea %fp@(-10)
     5ec:	4227           	clrb %sp@-
     5ee:	3f03           	movew %d3,%sp@-
     5f0:	4eba 20fa      	jsr %pc@(0x26ec)
     5f4:	4a80           	tstl %d0
     5f6:	4fef 000c      	lea %sp@(12),%sp
     5fa:	671a           	beqs 0x616
     5fc:	206e ffd4      	moveal %fp@(-44),%a0
     600:	1028 041c      	moveb %a0@(1052),%d0
     604:	0240 0008      	andiw #8,%d0
     608:	6706           	beqs 0x610
     60a:	7054           	moveq #84,%d0
     60c:	6000 0276      	braw 0x884
     610:	5286           	addql #1,%d6
     612:	6000 0160      	braw 0x774
     616:	7000           	moveq #0,%d0
     618:	102e fff6      	moveb %fp@(-10),%d0
     61c:	3d40 ffe0      	movew %d0,%fp@(-32)
     620:	0240 0080      	andiw #128,%d0
     624:	0c40 0080      	cmpiw #128,%d0
     628:	6706           	beqs 0x630
     62a:	5286           	addql #1,%d6
     62c:	6000 0146      	braw 0x774
     630:	7000           	moveq #0,%d0
     632:	102e fff7      	moveb %fp@(-9),%d0
     636:	3800           	movew %d0,%d4
     638:	0240 0004      	andiw #4,%d0
     63c:	6700 0136      	beqw 0x774
     640:	3004           	movew %d4,%d0
     642:	0240 0008      	andiw #8,%d0
     646:	6612           	bnes 0x65a
     648:	486e fff2      	pea %fp@(-14)
     64c:	3f03           	movew %d3,%sp@-
     64e:	4eba 2c60      	jsr %pc@(0x32b0)
     652:	5286           	addql #1,%d6
     654:	5c4f           	addqw #6,%sp
     656:	6000 011c      	braw 0x774
     65a:	41ee fff6      	lea %fp@(-10),%a0
     65e:	5488           	addql #2,%a0
     660:	2448           	moveal %a0,%a2
     662:	0c50 ffff      	cmpiw #-1,%a0@
     666:	660a           	bnes 0x672
     668:	2053           	moveal %a3@,%a0
     66a:	3083           	movew %d3,%a0@
     66c:	5493           	addql #2,%a3@
     66e:	6000 0104      	braw 0x774
     672:	3812           	movew %a2@,%d4
     674:	3d44 fff0      	movew %d4,%fp@(-16)
     678:	2047           	moveal %d7,%a0
     67a:	3d50 ffe2      	movew %a0@,%fp@(-30)
     67e:	b850           	cmpw %a0@,%d4
     680:	650a           	bcss 0x68c
     682:	206e ffdc      	moveal %fp@(-36),%a0
     686:	3004           	movew %d4,%d0
     688:	b050           	cmpw %a0@,%d0
     68a:	6516           	bcss 0x6a2
     68c:	486e fff2      	pea %fp@(-14)
     690:	3f03           	movew %d3,%sp@-
     692:	4eba 2c1c      	jsr %pc@(0x32b0)
     696:	2053           	moveal %a3@,%a0
     698:	3083           	movew %d3,%a0@
     69a:	5493           	addql #2,%a3@
     69c:	5c4f           	addqw #6,%sp
     69e:	6000 00d4      	braw 0x774
     6a2:	7000           	moveq #0,%d0
     6a4:	302e ffe2      	movew %fp@(-30),%d0
     6a8:	d080           	addl %d0,%d0
     6aa:	7200           	moveq #0,%d1
     6ac:	322e fff0      	movew %fp@(-16),%d1
     6b0:	d281           	addl %d1,%d1
     6b2:	206e ffd8      	moveal %fp@(-40),%a0
     6b6:	d1c1           	addal %d1,%a0
     6b8:	2448           	moveal %a0,%a2
     6ba:	91c0           	subal %d0,%a0
     6bc:	2d48 ffe8      	movel %a0,%fp@(-24)
     6c0:	3810           	movew %a0@,%d4
     6c2:	0c44 ffff      	cmpiw #-1,%d4
     6c6:	6700 00a6      	beqw 0x76e
     6ca:	302e ffe0      	movew %fp@(-32),%d0
     6ce:	0240 0010      	andiw #16,%d0
     6d2:	6778           	beqs 0x74c
     6d4:	486e ffe4      	pea %fp@(-28)
     6d8:	486e fff6      	pea %fp@(-10)
     6dc:	4227           	clrb %sp@-
     6de:	3f04           	movew %d4,%sp@-
     6e0:	4eba 200a      	jsr %pc@(0x26ec)
     6e4:	4a80           	tstl %d0
     6e6:	4fef 000c      	lea %sp@(12),%sp
     6ea:	6720           	beqs 0x70c
     6ec:	486e fff2      	pea %fp@(-14)
     6f0:	3f04           	movew %d4,%sp@-
     6f2:	4eba 2bbc      	jsr %pc@(0x32b0)
     6f6:	2053           	moveal %a3@,%a0
     6f8:	3084           	movew %d4,%a0@
     6fa:	5493           	addql #2,%a3@
     6fc:	2047           	moveal %d7,%a0
     6fe:	7000           	moveq #0,%d0
     700:	3010           	movew %a0@,%d0
     702:	d080           	addl %d0,%d0
     704:	204a           	moveal %a2,%a0
     706:	91c0           	subal %d0,%a0
     708:	3083           	movew %d3,%a0@
     70a:	5c4f           	addqw #6,%sp
     70c:	102e fff6      	moveb %fp@(-10),%d0
     710:	0240 0010      	andiw #16,%d0
     714:	6722           	beqs 0x738
     716:	486e fff2      	pea %fp@(-14)
     71a:	3f04           	movew %d4,%sp@-
     71c:	4eba 2b92      	jsr %pc@(0x32b0)
     720:	2053           	moveal %a3@,%a0
     722:	3084           	movew %d4,%a0@
     724:	5493           	addql #2,%a3@
     726:	2047           	moveal %d7,%a0
     728:	7000           	moveq #0,%d0
     72a:	3010           	movew %a0@,%d0
     72c:	d080           	addl %d0,%d0
     72e:	204a           	moveal %a2,%a0
     730:	91c0           	subal %d0,%a0
     732:	3083           	movew %d3,%a0@
     734:	5c4f           	addqw #6,%sp
     736:	603c           	bras 0x774
     738:	486e fff2      	pea %fp@(-14)
     73c:	3f03           	movew %d3,%sp@-
     73e:	4eba 2b70      	jsr %pc@(0x32b0)
     742:	2053           	moveal %a3@,%a0
     744:	3083           	movew %d3,%a0@
     746:	5493           	addql #2,%a3@
     748:	5c4f           	addqw #6,%sp
     74a:	6028           	bras 0x774
     74c:	486e fff2      	pea %fp@(-14)
     750:	3f04           	movew %d4,%sp@-
     752:	4eba 2b5c      	jsr %pc@(0x32b0)
     756:	2053           	moveal %a3@,%a0
     758:	3084           	movew %d4,%a0@
     75a:	5493           	addql #2,%a3@
     75c:	2047           	moveal %d7,%a0
     75e:	7000           	moveq #0,%d0
     760:	3010           	movew %a0@,%d0
     762:	d080           	addl %d0,%d0
     764:	204a           	moveal %a2,%a0
     766:	91c0           	subal %d0,%a0
     768:	3083           	movew %d3,%a0@
     76a:	5c4f           	addqw #6,%sp
     76c:	6006           	bras 0x774
     76e:	206e ffe8      	moveal %fp@(-24),%a0
     772:	3083           	movew %d3,%a0@
     774:	526e ffec      	addqw #1,%fp@(-20)
     778:	526e ffee      	addqw #1,%fp@(-18)
     77c:	0c6e 0200 ffee 	cmpiw #512,%fp@(-18)
     782:	6500 fe3c      	bcsw 0x5c0
     786:	204b           	moveal %a3,%a0
     788:	5088           	addql #8,%a0
     78a:	5888           	addql #4,%a0
     78c:	2253           	moveal %a3@,%a1
     78e:	93c8           	subal %a0,%a1
     790:	2009           	movel %a1,%d0
     792:	e088           	lsrl #8,%d0
     794:	e088           	lsrl #8,%d0
     796:	e088           	lsrl #8,%d0
     798:	ee88           	lsrl #7,%d0
     79a:	d089           	addl %a1,%d0
     79c:	e280           	asrl #1,%d0
     79e:	2740 0004      	movel %d0,%a3@(4)
     7a2:	202e ffb8      	movel %fp@(-72),%d0
     7a6:	5380           	subql #1,%d0
     7a8:	b0ae ffb4      	cmpl %fp@(-76),%d0
     7ac:	6612           	bnes 0x7c0
     7ae:	53ab 0004      	subql #1,%a3@(4)
     7b2:	5286           	addql #1,%d6
     7b4:	202b 0004      	movel %a3@(4),%d0
     7b8:	d080           	addl %d0,%d0
     7ba:	37bc ffff 080c 	movew #-1,%a3@(0000000c,%d0:l)
     7c0:	7010           	moveq #16,%d0
     7c2:	bc80           	cmpl %d0,%d6
     7c4:	6d1c           	blts 0x7e2
     7c6:	206e 0008      	moveal %fp@(8),%a0
     7ca:	117c 0001 437c 	moveb #1,%a0@(17276)
     7d0:	117c 0001 437b 	moveb #1,%a0@(17275)
     7d6:	7002           	moveq #2,%d0
     7d8:	81a8 0004      	orl %d0,%a0@(4)
     7dc:	7010           	moveq #16,%d0
     7de:	81a8 4382      	orl %d0,%a0@(17282)
     7e2:	2746 0008      	movel %d6,%a3@(8)
     7e6:	206e 0008      	moveal %fp@(8),%a0
     7ea:	2d68 0014 ffcc 	movel %a0@(20),%fp@(-52)
     7f0:	4878 03e0      	pea 0x3e0
     7f4:	2f2e ffd8      	movel %fp@(-40),%sp@-
     7f8:	206e ffc4      	moveal %fp@(-60),%a0
     7fc:	7000           	moveq #0,%d0
     7fe:	3010           	movew %a0@,%d0
     800:	d080           	addl %d0,%d0
     802:	206e ffcc      	moveal %fp@(-52),%a0
     806:	d1c0           	addal %d0,%a0
     808:	2008           	movel %a0,%d0
     80a:	90ae ffcc      	subl %fp@(-52),%d0
     80e:	2f00           	movel %d0,%sp@-
     810:	2f2e ffcc      	movel %fp@(-52),%sp@-
     814:	4e4f           	trap #15
     816:	a076           	0120166
     818:	206e 0008      	moveal %fp@(8),%a0
     81c:	2068 0014      	moveal %a0@(20),%a0
     820:	d1ee ffbe      	addal %fp@(-66),%a0
     824:	41e8 400c      	lea %a0@(16396),%a0
     828:	2688           	movel %a0,%a3@
     82a:	206e 0008      	moveal %fp@(8),%a0
     82e:	2d68 0014 ffc8 	movel %a0@(20),%fp@(-56)
     834:	4878 040c      	pea 0x40c
     838:	2f0b           	movel %a3,%sp@-
     83a:	206e ffc8      	moveal %fp@(-56),%a0
     83e:	d1ee ffbe      	addal %fp@(-66),%a0
     842:	41e8 4000      	lea %a0@(16384),%a0
     846:	2008           	movel %a0,%d0
     848:	90ae ffc8      	subl %fp@(-56),%d0
     84c:	2f00           	movel %d0,%sp@-
     84e:	2f2e ffc8      	movel %fp@(-56),%sp@-
     852:	4e4f           	trap #15
     854:	a076           	0120166
     856:	066e 0200 ffbc 	addiw #512,%fp@(-68)
     85c:	54ae ffc4      	addql #2,%fp@(-60)
     860:	06ae 0000 040c 	addil #1036,%fp@(-66)
     866:	ffbe
     868:	526e ffc2      	addqw #1,%fp@(-62)
     86c:	4fef 0020      	lea %sp@(32),%sp
     870:	7000           	moveq #0,%d0
     872:	302e ffc2      	movew %fp@(-62),%d0
     876:	2d40 ffb4      	movel %d0,%fp@(-76)
     87a:	b0ae ffb8      	cmpl %fp@(-72),%d0
     87e:	6d00 fcfe      	bltw 0x57e
     882:	7000           	moveq #0,%d0
     884:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
     888:	4e5e           	unlk %fp
     88a:	4e75           	rts

```

# job_mount: 0x089e - 0x0a80


```

     89e:	4e56 ffde      	linkw %fp,#-34
     8a2:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
     8a6:	246e 0008      	moveal %fp@(8),%a2
     8aa:	47ee ffe0      	lea %fp@(-32),%a3
     8ae:	42aa 4382      	clrl %a2@(17282)
     8b2:	2f0a           	movel %a2,%sp@-
     8b4:	4eba fae2      	jsr %pc@(0x398)
     8b8:	2600           	movel %d0,%d3
     8ba:	584f           	addqw #4,%sp
     8bc:	6706           	beqs 0x8c4
     8be:	2003           	movel %d3,%d0
     8c0:	6000 01b8      	braw 0xa7a
     8c4:	302a 415a      	movew %a2@(16730),%d0
     8c8:	0240 ff00      	andiw #-256,%d0
     8cc:	0c40 0100      	cmpiw #256,%d0
     8d0:	6706           	beqs 0x8d8
     8d2:	7010           	moveq #16,%d0
     8d4:	6000 01a4      	braw 0xa7a
     8d8:	0c2a 0001 42f8 	cmpib #1,%a2@(17144)
     8de:	6706           	beqs 0x8e6
     8e0:	7010           	moveq #16,%d0
     8e2:	6000 0196      	braw 0xa7a
     8e6:	0c2a 0002 42f9 	cmpib #2,%a2@(17145)
     8ec:	6706           	beqs 0x8f4
     8ee:	7010           	moveq #16,%d0
     8f0:	6000 0188      	braw 0xa7a
     8f4:	302a 42fa      	movew %a2@(17146),%d0
     8f8:	5140           	subqw #8,%d0
     8fa:	670a           	beqs 0x906
     8fc:	5140           	subqw #8,%d0
     8fe:	6706           	beqs 0x906
     900:	7010           	moveq #16,%d0
     902:	6000 0176      	braw 0xa7a
     906:	302a 42fc      	movew %a2@(17148),%d0
     90a:	0440 0200      	subiw #512,%d0
     90e:	671e           	beqs 0x92e
     910:	0440 0200      	subiw #512,%d0
     914:	6718           	beqs 0x92e
     916:	0440 0400      	subiw #1024,%d0
     91a:	6712           	beqs 0x92e
     91c:	0440 0800      	subiw #2048,%d0
     920:	670c           	beqs 0x92e
     922:	0440 1000      	subiw #4096,%d0
     926:	6706           	beqs 0x92e
     928:	7010           	moveq #16,%d0
     92a:	6000 014e      	braw 0xa7a
     92e:	302a 42fe      	movew %a2@(17150),%d0
     932:	0440 01f0      	subiw #496,%d0
     936:	671e           	beqs 0x956
     938:	0440 01f0      	subiw #496,%d0
     93c:	6718           	beqs 0x956
     93e:	0440 03e0      	subiw #992,%d0
     942:	6712           	beqs 0x956
     944:	0440 07c0      	subiw #1984,%d0
     948:	670c           	beqs 0x956
     94a:	0440 0f80      	subiw #3968,%d0
     94e:	6706           	beqs 0x956
     950:	7010           	moveq #16,%d0
     952:	6000 0126      	braw 0xa7a
     956:	0c2a 0001 432e 	cmpib #1,%a2@(17198)
     95c:	6706           	beqs 0x964
     95e:	7010           	moveq #16,%d0
     960:	6000 0118      	braw 0xa7a
     964:	0c2a 0001 42d0 	cmpib #1,%a2@(17104)
     96a:	6706           	beqs 0x972
     96c:	7010           	moveq #16,%d0
     96e:	6000 010a      	braw 0xa7a
     972:	7040           	moveq #64,%d0
     974:	81aa 4382      	orl %d0,%a2@(17282)
     978:	0c6a 1001 4320 	cmpiw #4097,%a2@(17184)
     97e:	6618           	bnes 0x998
     980:	2078 0304      	moveal 0x304,%a0
     984:	2628 0010      	movel %a0@(16),%d3
     988:	6708           	beqs 0x992
     98a:	2043           	moveal %d3,%a0
     98c:	7004           	moveq #4,%d0
     98e:	81a8 001c      	orl %d0,%a0@(28)
     992:	7002           	moveq #2,%d0
     994:	81aa 4382      	orl %d0,%a2@(17282)
     998:	486e ffdf      	pea %fp@(-33)
     99c:	2f0b           	movel %a3,%sp@-
     99e:	4eba f78a      	jsr %pc@(0x12a)
     9a2:	4a40           	tstw %d0
     9a4:	504f           	addqw #8,%sp
     9a6:	6706           	beqs 0x9ae
     9a8:	7010           	moveq #16,%d0
     9aa:	6000 00ce      	braw 0xa7a
     9ae:	102b 0002      	moveb %a3@(2),%d0
     9b2:	0240 0001      	andiw #1,%d0
     9b6:	6720           	beqs 0x9d8
     9b8:	157c 0001 437b 	moveb #1,%a2@(17275)
     9be:	2078 0304      	moveal 0x304,%a0
     9c2:	2668 0010      	moveal %a0@(16),%a3
     9c6:	200b           	movel %a3,%d0
     9c8:	6706           	beqs 0x9d0
     9ca:	7008           	moveq #8,%d0
     9cc:	81ab 001c      	orl %d0,%a3@(28)
     9d0:	7020           	moveq #32,%d0
     9d2:	81aa 4382      	orl %d0,%a2@(17282)
     9d6:	6004           	bras 0x9dc
     9d8:	422a 437b      	clrb %a2@(17275)
     9dc:	422a 437c      	clrb %a2@(17276)
     9e0:	2f0a           	movel %a2,%sp@-
     9e2:	4eba fab0      	jsr %pc@(0x494)
     9e6:	2600           	movel %d0,%d3
     9e8:	584f           	addqw #4,%sp
     9ea:	6706           	beqs 0x9f2
     9ec:	2003           	movel %d3,%d0
     9ee:	6000 008a      	braw 0xa7a
     9f2:	357c 0200 4360 	movew #512,%a2@(17248)
     9f8:	357c 0010 4362 	movew #16,%a2@(17250)
     9fe:	362a 42fa      	movew %a2@(17146),%d3
     a02:	0c43 0008      	cmpiw #8,%d3
     a06:	660a           	bnes 0xa12
     a08:	357c 0002 4366 	movew #2,%a2@(17254)
     a0e:	7602           	moveq #2,%d3
     a10:	603c           	bras 0xa4e
     a12:	0c43 0010      	cmpiw #16,%d3
     a16:	662e           	bnes 0xa46
     a18:	362a 42fc      	movew %a2@(17148),%d3
     a1c:	0c43 1f00      	cmpiw #7936,%d3
     a20:	6308           	blss 0xa2a
     a22:	357c 0010 4366 	movew #16,%a2@(17254)
     a28:	6014           	bras 0xa3e
     a2a:	0c43 0f80      	cmpiw #3968,%d3
     a2e:	6308           	blss 0xa38
     a30:	357c 0008 4366 	movew #8,%a2@(17254)
     a36:	6006           	bras 0xa3e
     a38:	357c 0004 4366 	movew #4,%a2@(17254)
     a3e:	362a 4366      	movew %a2@(17254),%d3
     a42:	e24b           	lsrw #1,%d3
     a44:	6008           	bras 0xa4e
     a46:	426a 4366      	clrw %a2@(17254)
     a4a:	7010           	moveq #16,%d0
     a4c:	602c           	bras 0xa7a
     a4e:	302a 42fe      	movew %a2@(17150),%d0
     a52:	0640 fffe      	addiw #-2,%d0
     a56:	7200           	moveq #0,%d1
     a58:	3200           	movew %d0,%d1
     a5a:	82c3           	divuw %d3,%d1
     a5c:	3541 4364      	movew %d1,%a2@(17252)
     a60:	2078 0304      	moveal 0x304,%a0
     a64:	2668 0010      	moveal %a0@(16),%a3
     a68:	200b           	movel %a3,%d0
     a6a:	6706           	beqs 0xa72
     a6c:	7002           	moveq #2,%d0
     a6e:	81ab 001c      	orl %d0,%a3@(28)
     a72:	7001           	moveq #1,%d0
     a74:	81aa 4382      	orl %d0,%a2@(17282)
     a78:	7000           	moveq #0,%d0
     a7a:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
     a7e:	4e5e           	unlk %fp
     a80:	4e75           	rts

```

# job_msMoveWrite: 0x0a8e - 0x16a2


```

     a8e:	4e56 ff60      	linkw %fp,#-160
     a92:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
     a96:	3e2e 0008      	movew %fp@(8),%d7
     a9a:	2c2e 000e      	movel %fp@(14),%d6
     a9e:	246e 0016      	moveal %fp@(22),%a2
     aa2:	41fa f8c0      	lea %pc@(0x364),%a0
     aa6:	2d48 ff60      	movel %a0,%fp@(-160)
     aaa:	206e 0012      	moveal %fp@(18),%a0
     aae:	10ae 000a      	moveb %fp@(10),%a0@
     ab2:	7a01           	moveq #1,%d5
     ab4:	206e ff60      	moveal %fp@(-160),%a0
     ab8:	5488           	addql #2,%a0
     aba:	2608           	movel %a0,%d3
     abc:	2043           	moveal %d3,%a0
     abe:	be50           	cmpw %a0@,%d7
     ac0:	650a           	bcss 0xacc
     ac2:	5483           	addql #2,%d3
     ac4:	5285           	addql #1,%d5
     ac6:	7011           	moveq #17,%d0
     ac8:	ba80           	cmpl %d0,%d5
     aca:	6df0           	blts 0xabc
     acc:	5385           	subql #1,%d5
     ace:	2605           	movel %d5,%d3
     ad0:	3d47 ff64      	movew %d7,%fp@(-156)
     ad4:	2003           	movel %d3,%d0
     ad6:	223c 0000 040c 	movel #1036,%d1
     adc:	4eba 410c      	jsr %pc@(0x4bea)
     ae0:	2d40 ffb6      	movel %d0,%fp@(-74)
     ae4:	2a2a 0014      	movel %a2@(20),%d5
     ae8:	2045           	moveal %d5,%a0
     aea:	d1c0           	addal %d0,%a0
     aec:	4aa8 4004      	tstl %a0@(16388)
     af0:	6606           	bnes 0xaf8
     af2:	7014           	moveq #20,%d0
     af4:	6000 0ba6      	braw 0x169c
     af8:	7000           	moveq #0,%d0
     afa:	3007           	movew %d7,%d0
     afc:	d080           	addl %d0,%d0
     afe:	2045           	moveal %d5,%a0
     b00:	d1c0           	addal %d0,%a0
     b02:	2608           	movel %a0,%d3
     b04:	0c50 ffff      	cmpiw #-1,%a0@
     b08:	6600 03ce      	bnew 0xed8
     b0c:	262e ffb6      	movel %fp@(-74),%d3
     b10:	7000           	moveq #0,%d0
     b12:	102e 000a      	moveb %fp@(10),%d0
     b16:	7209           	moveq #9,%d1
     b18:	e368           	lslw %d1,%d0
     b1a:	48c0           	extl %d0
     b1c:	2d40 ffba      	movel %d0,%fp@(-70)
     b20:	7000           	moveq #0,%d0
     b22:	102e 000c      	moveb %fp@(12),%d0
     b26:	e368           	lslw %d1,%d0
     b28:	48c0           	extl %d0
     b2a:	2d40 ffae      	movel %d0,%fp@(-82)
     b2e:	7001           	moveq #1,%d0
     b30:	c086           	andl %d6,%d0
     b32:	2d40 ff9c      	movel %d0,%fp@(-100)
     b36:	4a2a 437c      	tstb %a2@(17276)
     b3a:	6706           	beqs 0xb42
     b3c:	7014           	moveq #20,%d0
     b3e:	6000 0b5c      	braw 0x169c
     b42:	2003           	movel %d3,%d0
     b44:	d0aa 0014      	addl %a2@(20),%d0
     b48:	2640           	moveal %d0,%a3
     b4a:	2040           	moveal %d0,%a0
     b4c:	2a28 4004      	movel %a0@(16388),%d5
     b50:	2d6b 4000 fffc 	movel %a3@(16384),%fp@(-4)
     b56:	603a           	bras 0xb92
     b58:	54ae fffc      	addql #2,%fp@(-4)
     b5c:	282a 0014      	movel %a2@(20),%d4
     b60:	2003           	movel %d3,%d0
     b62:	d084           	addl %d4,%d0
     b64:	2040           	moveal %d0,%a0
     b66:	2028 4004      	movel %a0@(16388),%d0
     b6a:	d080           	addl %d0,%d0
     b6c:	2044           	moveal %d4,%a0
     b6e:	d1c3           	addal %d3,%a0
     b70:	2648           	moveal %a0,%a3
     b72:	d1c0           	addal %d0,%a0
     b74:	41e8 400c      	lea %a0@(16396),%a0
     b78:	b1ee fffc      	cmpal %fp@(-4),%a0
     b7c:	660a           	bnes 0xb88
     b7e:	204b           	moveal %a3,%a0
     b80:	41e8 400c      	lea %a0@(16396),%a0
     b84:	2d48 fffc      	movel %a0,%fp@(-4)
     b88:	5385           	subql #1,%d5
     b8a:	6606           	bnes 0xb92
     b8c:	7014           	moveq #20,%d0
     b8e:	6000 0b0c      	braw 0x169c
     b92:	206e fffc      	moveal %fp@(-4),%a0
     b96:	0c50 ffff      	cmpiw #-1,%a0@
     b9a:	67bc           	beqs 0xb58
     b9c:	2d6a 0014 ffd6 	movel %a2@(20),%fp@(-42)
     ba2:	4878 0004      	pea 0x4
     ba6:	486e fffc      	pea %fp@(-4)
     baa:	206e ffd6      	moveal %fp@(-42),%a0
     bae:	d1c3           	addal %d3,%a0
     bb0:	41e8 4000      	lea %a0@(16384),%a0
     bb4:	2008           	movel %a0,%d0
     bb6:	90ae ffd6      	subl %fp@(-42),%d0
     bba:	2f00           	movel %d0,%sp@-
     bbc:	2f2e ffd6      	movel %fp@(-42),%sp@-
     bc0:	4e4f           	trap #15
     bc2:	a076           	0120166
     bc4:	2003           	movel %d3,%d0
     bc6:	d0aa 0014      	addl %a2@(20),%d0
     bca:	2040           	moveal %d0,%a0
     bcc:	2068 4000      	moveal %a0@(16384),%a0
     bd0:	3d50 ffee      	movew %a0@,%fp@(-18)
     bd4:	486e fff6      	pea %fp@(-10)
     bd8:	3f2e ffee      	movew %fp@(-18),%sp@-
     bdc:	4eba 26d2      	jsr %pc@(0x32b0)
     be0:	4a80           	tstl %d0
     be2:	4fef 0016      	lea %sp@(22),%sp
     be6:	6700 00bc      	beqw 0xca4
     bea:	202e fff6      	movel %fp@(-10),%d0
     bee:	0280 0000 4000 	andil #16384,%d0
     bf4:	6706           	beqs 0xbfc
     bf6:	7009           	moveq #9,%d0
     bf8:	6000 0aa2      	braw 0x169c
     bfc:	1d7c 007f fffb 	moveb #127,%fp@(-5)
     c02:	486e fffb      	pea %fp@(-5)
     c06:	4227           	clrb %sp@-
     c08:	3f2e ffee      	movew %fp@(-18),%sp@-
     c0c:	4eba 1c5e      	jsr %pc@(0x286c)
     c10:	2d6a 0014 ffea 	movel %a2@(20),%fp@(-22)
     c16:	1f3c 00ff      	moveb #-1,%sp@-
     c1a:	4878 0002      	pea 0x2
     c1e:	2003           	movel %d3,%d0
     c20:	d0ae ffea      	addl %fp@(-22),%d0
     c24:	2040           	moveal %d0,%a0
     c26:	2028 4000      	movel %a0@(16384),%d0
     c2a:	90ae ffea      	subl %fp@(-22),%d0
     c2e:	2f00           	movel %d0,%sp@-
     c30:	2f2e ffea      	movel %fp@(-22),%sp@-
     c34:	4e4f           	trap #15
     c36:	a07e           	0120176
     c38:	2003           	movel %d3,%d0
     c3a:	d0aa 0014      	addl %a2@(20),%d0
     c3e:	2040           	moveal %d0,%a0
     c40:	2028 4008      	movel %a0@(16392),%d0
     c44:	5280           	addql #1,%d0
     c46:	2d40 fff2      	movel %d0,%fp@(-14)
     c4a:	2d6a 0014 ffd2 	movel %a2@(20),%fp@(-46)
     c50:	4878 0004      	pea 0x4
     c54:	486e fff2      	pea %fp@(-14)
     c58:	206e ffd2      	moveal %fp@(-46),%a0
     c5c:	d1c3           	addal %d3,%a0
     c5e:	41e8 4008      	lea %a0@(16392),%a0
     c62:	2008           	movel %a0,%d0
     c64:	90ae ffd2      	subl %fp@(-46),%d0
     c68:	2f00           	movel %d0,%sp@-
     c6a:	2f2e ffd2      	movel %fp@(-46),%sp@-
     c6e:	4e4f           	trap #15
     c70:	a076           	0120166
     c72:	2003           	movel %d3,%d0
     c74:	d0aa 0014      	addl %a2@(20),%d0
     c78:	2040           	moveal %d0,%a0
     c7a:	7010           	moveq #16,%d0
     c7c:	b0a8 4008      	cmpl %a0@(16392),%d0
     c80:	4fef 0026      	lea %sp@(38),%sp
     c84:	6200 feb0      	bhiw 0xb36
     c88:	157c 0001 437c 	moveb #1,%a2@(17276)
     c8e:	157c 0001 437b 	moveb #1,%a2@(17275)
     c94:	7002           	moveq #2,%d0
     c96:	81aa 0004      	orl %d0,%a2@(4)
     c9a:	7010           	moveq #16,%d0
     c9c:	81aa 4382      	orl %d0,%a2@(17282)
     ca0:	6000 fe94      	braw 0xb36
     ca4:	7a00           	moveq #0,%d5
     ca6:	264a           	moveal %a2,%a3
     ca8:	6034           	bras 0xcde
     caa:	177c 00f8 4018 	moveb #-8,%a3@(16408)
     cb0:	177c 00ff 4019 	moveb #-1,%a3@(16409)
     cb6:	3747 401a      	movew %d7,%a3@(16410)
     cba:	177c 00ff 401c 	moveb #-1,%a3@(16412)
     cc0:	177c 00ff 401d 	moveb #-1,%a3@(16413)
     cc6:	177c 00ff 401e 	moveb #-1,%a3@(16414)
     ccc:	177c 00ff 401f 	moveb #-1,%a3@(16415)
     cd2:	177c 00ff 4020 	moveb #-1,%a3@(16416)
     cd8:	508b           	addql #8,%a3
     cda:	548b           	addql #2,%a3
     cdc:	5285           	addql #1,%d5
     cde:	7000           	moveq #0,%d0
     ce0:	102a 437d      	moveb %a2@(17277),%d0
     ce4:	3d40 fff0      	movew %d0,%fp@(-16)
     ce8:	48c0           	extl %d0
     cea:	ba80           	cmpl %d0,%d5
     cec:	6dbc           	blts 0xcaa
     cee:	4a2e 000a      	tstb %fp@(10)
     cf2:	674c           	beqs 0xd40
     cf4:	264a           	moveal %a2,%a3
     cf6:	47eb 0018      	lea %a3@(24),%a3
     cfa:	1f3c 00ff      	moveb #-1,%sp@-
     cfe:	2f2e ffba      	movel %fp@(-70),%sp@-
     d02:	2f0b           	movel %a3,%sp@-
     d04:	4e4f           	trap #15
     d06:	a027           	0120047
     d08:	4a86           	tstl %d6
     d0a:	4fef 000a      	lea %sp@(10),%sp
     d0e:	6618           	bnes 0xd28
     d10:	4227           	clrb %sp@-
     d12:	2f2e ffae      	movel %fp@(-82),%sp@-
     d16:	202e ffba      	movel %fp@(-70),%d0
     d1a:	4873 0800      	pea %a3@(00000000,%d0:l)
     d1e:	4e4f           	trap #15
     d20:	a027           	0120047
     d22:	4fef 000a      	lea %sp@(10),%sp
     d26:	6068           	bras 0xd90
     d28:	2f2e ffae      	movel %fp@(-82),%sp@-
     d2c:	2f06           	movel %d6,%sp@-
     d2e:	202e ffba      	movel %fp@(-70),%d0
     d32:	4873 0800      	pea %a3@(00000000,%d0:l)
     d36:	4e4f           	trap #15
     d38:	a026           	0120046
     d3a:	4fef 000c      	lea %sp@(12),%sp
     d3e:	6050           	bras 0xd90
     d40:	4a86           	tstl %d6
     d42:	6622           	bnes 0xd66
     d44:	4227           	clrb %sp@-
     d46:	302e fff0      	movew %fp@(-16),%d0
     d4a:	7209           	moveq #9,%d1
     d4c:	e368           	lslw %d1,%d0
     d4e:	48c0           	extl %d0
     d50:	2f00           	movel %d0,%sp@-
     d52:	486a 0018      	pea %a2@(24)
     d56:	4e4f           	trap #15
     d58:	a027           	0120047
     d5a:	264a           	moveal %a2,%a3
     d5c:	47eb 0018      	lea %a3@(24),%a3
     d60:	4fef 000a      	lea %sp@(10),%sp
     d64:	602a           	bras 0xd90
     d66:	4aae ff9c      	tstl %fp@(-100)
     d6a:	6722           	beqs 0xd8e
     d6c:	302e fff0      	movew %fp@(-16),%d0
     d70:	7209           	moveq #9,%d1
     d72:	e368           	lslw %d1,%d0
     d74:	48c0           	extl %d0
     d76:	2f00           	movel %d0,%sp@-
     d78:	2f06           	movel %d6,%sp@-
     d7a:	486a 0018      	pea %a2@(24)
     d7e:	4e4f           	trap #15
     d80:	a026           	0120046
     d82:	264a           	moveal %a2,%a3
     d84:	47eb 0018      	lea %a3@(24),%a3
     d88:	4fef 000c      	lea %sp@(12),%sp
     d8c:	6002           	bras 0xd90
     d8e:	2646           	moveal %d6,%a3
     d90:	2f2e 0012      	movel %fp@(18),%sp@-
     d94:	486e fff6      	pea %fp@(-10)
     d98:	486a 4018      	pea %a2@(16408)
     d9c:	2f0b           	movel %a3,%sp@-
     d9e:	1f2a 437d      	moveb %a2@(17277),%sp@-
     da2:	4227           	clrb %sp@-
     da4:	3f2e ffee      	movew %fp@(-18),%sp@-
     da8:	4eba 1c0e      	jsr %pc@(0x29b8)
     dac:	4a80           	tstl %d0
     dae:	4fef 0016      	lea %sp@(22),%sp
     db2:	6700 00c2      	beqw 0xe76
     db6:	202e fff6      	movel %fp@(-10),%d0
     dba:	0280 0000 4000 	andil #16384,%d0
     dc0:	6706           	beqs 0xdc8
     dc2:	7009           	moveq #9,%d0
     dc4:	6000 08d6      	braw 0x169c
     dc8:	1d7c 007f fffb 	moveb #127,%fp@(-5)
     dce:	486e fffb      	pea %fp@(-5)
     dd2:	4227           	clrb %sp@-
     dd4:	3f2e ffee      	movew %fp@(-18),%sp@-
     dd8:	4eba 1a92      	jsr %pc@(0x286c)
     ddc:	2d6a 0014 ffe6 	movel %a2@(20),%fp@(-26)
     de2:	1f3c 00ff      	moveb #-1,%sp@-
     de6:	4878 0002      	pea 0x2
     dea:	2003           	movel %d3,%d0
     dec:	d0ae ffe6      	addl %fp@(-26),%d0
     df0:	2040           	moveal %d0,%a0
     df2:	2028 4000      	movel %a0@(16384),%d0
     df6:	90ae ffe6      	subl %fp@(-26),%d0
     dfa:	2f00           	movel %d0,%sp@-
     dfc:	2f2e ffe6      	movel %fp@(-26),%sp@-
     e00:	4e4f           	trap #15
     e02:	a07e           	0120176
     e04:	2003           	movel %d3,%d0
     e06:	d0aa 0014      	addl %a2@(20),%d0
     e0a:	2040           	moveal %d0,%a0
     e0c:	2028 4008      	movel %a0@(16392),%d0
     e10:	5280           	addql #1,%d0
     e12:	2d40 fff2      	movel %d0,%fp@(-14)
     e16:	2d6a 0014 ffce 	movel %a2@(20),%fp@(-50)
     e1c:	4878 0004      	pea 0x4
     e20:	486e fff2      	pea %fp@(-14)
     e24:	206e ffce      	moveal %fp@(-50),%a0
     e28:	d1c3           	addal %d3,%a0
     e2a:	41e8 4008      	lea %a0@(16392),%a0
     e2e:	2008           	movel %a0,%d0
     e30:	90ae ffce      	subl %fp@(-50),%d0
     e34:	2f00           	movel %d0,%sp@-
     e36:	2f2e ffce      	movel %fp@(-50),%sp@-
     e3a:	4e4f           	trap #15
     e3c:	a076           	0120166
     e3e:	2003           	movel %d3,%d0
     e40:	d0aa 0014      	addl %a2@(20),%d0
     e44:	2040           	moveal %d0,%a0
     e46:	7010           	moveq #16,%d0
     e48:	b0a8 4008      	cmpl %a0@(16392),%d0
     e4c:	4fef 0026      	lea %sp@(38),%sp
     e50:	6218           	bhis 0xe6a
     e52:	157c 0001 437c 	moveb #1,%a2@(17276)
     e58:	157c 0001 437b 	moveb #1,%a2@(17275)
     e5e:	7002           	moveq #2,%d0
     e60:	81aa 0004      	orl %d0,%a2@(4)
     e64:	7010           	moveq #16,%d0
     e66:	81aa 4382      	orl %d0,%a2@(17282)
     e6a:	206e 0012      	moveal %fp@(18),%a0
     e6e:	10ae 000a      	moveb %fp@(10),%a0@
     e72:	6000 fcc2      	braw 0xb36
     e76:	3d6e ffee ff92 	movew %fp@(-18),%fp@(-110)
     e7c:	2d6a 0014 ff7e 	movel %a2@(20),%fp@(-130)
     e82:	4878 0002      	pea 0x2
     e86:	486e ff92      	pea %fp@(-110)
     e8a:	7000           	moveq #0,%d0
     e8c:	302e ff64      	movew %fp@(-156),%d0
     e90:	d080           	addl %d0,%d0
     e92:	206e ff7e      	moveal %fp@(-130),%a0
     e96:	d1c0           	addal %d0,%a0
     e98:	2008           	movel %a0,%d0
     e9a:	90ae ff7e      	subl %fp@(-130),%d0
     e9e:	2f00           	movel %d0,%sp@-
     ea0:	2f2e ff7e      	movel %fp@(-130),%sp@-
     ea4:	4e4f           	trap #15
     ea6:	a076           	0120166
     ea8:	2d6a 0014 ff8a 	movel %a2@(20),%fp@(-118)
     eae:	1f3c 00ff      	moveb #-1,%sp@-
     eb2:	4878 0002      	pea 0x2
     eb6:	206e ff8a      	moveal %fp@(-118),%a0
     eba:	d1ee ffb6      	addal %fp@(-74),%a0
     ebe:	2028 4000      	movel %a0@(16384),%d0
     ec2:	90ae ff8a      	subl %fp@(-118),%d0
     ec6:	2f00           	movel %d0,%sp@-
     ec8:	2f2e ff8a      	movel %fp@(-118),%sp@-
     ecc:	4e4f           	trap #15
     ece:	a07e           	0120176
     ed0:	4fef 001e      	lea %sp@(30),%sp
     ed4:	6000 075a      	braw 0x1630
     ed8:	42ae ffb2      	clrl %fp@(-78)
     edc:	102e 000c      	moveb %fp@(12),%d0
     ee0:	b02a 437d      	cmpb %a2@(17277),%d0
     ee4:	6700 0160      	beqw 0x1046
     ee8:	2043           	moveal %d3,%a0
     eea:	3a10           	movew %a0@,%d5
     eec:	7600           	moveq #0,%d3
     eee:	486e ffa9      	pea %fp@(-87)
     ef2:	486e fff6      	pea %fp@(-10)
     ef6:	486e ff94      	pea %fp@(-108)
     efa:	2003           	movel %d3,%d0
     efc:	720a           	moveq #10,%d1
     efe:	4eba 3cea      	jsr %pc@(0x4bea)
     f02:	204a           	moveal %a2,%a0
     f04:	d1c0           	addal %d0,%a0
     f06:	2808           	movel %a0,%d4
     f08:	4868 4018      	pea %a0@(16408)
     f0c:	2003           	movel %d3,%d0
     f0e:	7209           	moveq #9,%d1
     f10:	e3a8           	lsll %d1,%d0
     f12:	4872 0818      	pea %a2@(00000018,%d0:l)
     f16:	7000           	moveq #0,%d0
     f18:	1003           	moveb %d3,%d0
     f1a:	7200           	moveq #0,%d1
     f1c:	122a 437d      	moveb %a2@(17277),%d1
     f20:	9240           	subw %d0,%d1
     f22:	1f01           	moveb %d1,%sp@-
     f24:	1f03           	moveb %d3,%sp@-
     f26:	3f05           	movew %d5,%sp@-
     f28:	4eba 1f8e      	jsr %pc@(0x2eb8)
     f2c:	4a80           	tstl %d0
     f2e:	4fef 001a      	lea %sp@(26),%sp
     f32:	6734           	beqs 0xf68
     f34:	7002           	moveq #2,%d0
     f36:	c0ae fff6      	andl %fp@(-10),%d0
     f3a:	6606           	bnes 0xf42
     f3c:	7009           	moveq #9,%d0
     f3e:	6000 075c      	braw 0x169c
     f42:	2044           	moveal %d4,%a0
     f44:	117c 0098 4018 	moveb #-104,%a0@(16408)
     f4a:	7001           	moveq #1,%d0
     f4c:	2d40 ffb2      	movel %d0,%fp@(-78)
     f50:	162e ffa9      	moveb %fp@(-87),%d3
     f54:	4883           	extw %d3
     f56:	3803           	movew %d3,%d4
     f58:	5243           	addqw #1,%d3
     f5a:	48c3           	extl %d3
     f5c:	7000           	moveq #0,%d0
     f5e:	102a 437d      	moveb %a2@(17277),%d0
     f62:	5340           	subqw #1,%d0
     f64:	b840           	cmpw %d0,%d4
     f66:	6686           	bnes 0xeee
     f68:	7800           	moveq #0,%d4
     f6a:	260a           	movel %a2,%d3
     f6c:	6048           	bras 0xfb6
     f6e:	4aae ffb2      	tstl %fp@(-78)
     f72:	670a           	beqs 0xf7e
     f74:	2043           	moveal %d3,%a0
     f76:	0c28 0098 4018 	cmpib #-104,%a0@(16408)
     f7c:	6708           	beqs 0xf86
     f7e:	2043           	moveal %d3,%a0
     f80:	117c 00f8 4018 	moveb #-8,%a0@(16408)
     f86:	2043           	moveal %d3,%a0
     f88:	117c 00ff 4019 	moveb #-1,%a0@(16409)
     f8e:	3147 401a      	movew %d7,%a0@(16410)
     f92:	117c 00ff 401c 	moveb #-1,%a0@(16412)
     f98:	117c 00ff 401d 	moveb #-1,%a0@(16413)
     f9e:	117c 00ff 401e 	moveb #-1,%a0@(16414)
     fa4:	117c 00ff 401f 	moveb #-1,%a0@(16415)
     faa:	117c 00ff 4020 	moveb #-1,%a0@(16416)
     fb0:	700a           	moveq #10,%d0
     fb2:	d680           	addl %d0,%d3
     fb4:	5284           	addql #1,%d4
     fb6:	7000           	moveq #0,%d0
     fb8:	102a 437d      	moveb %a2@(17277),%d0
     fbc:	48c0           	extl %d0
     fbe:	b880           	cmpl %d0,%d4
     fc0:	6dac           	blts 0xf6e
     fc2:	4aae ffb2      	tstl %fp@(-78)
     fc6:	6700 00c6      	beqw 0x108e
     fca:	1d7c 007f fffb 	moveb #127,%fp@(-5)
     fd0:	486e fffb      	pea %fp@(-5)
     fd4:	4227           	clrb %sp@-
     fd6:	3f05           	movew %d5,%sp@-
     fd8:	4eba 1892      	jsr %pc@(0x286c)
     fdc:	206a 0014      	moveal %a2@(20),%a0
     fe0:	d1ee ffb6      	addal %fp@(-74),%a0
     fe4:	2028 4008      	movel %a0@(16392),%d0
     fe8:	5280           	addql #1,%d0
     fea:	2d40 fff2      	movel %d0,%fp@(-14)
     fee:	2d6a 0014 ff7a 	movel %a2@(20),%fp@(-134)
     ff4:	4878 0004      	pea 0x4
     ff8:	486e fff2      	pea %fp@(-14)
     ffc:	206e ff7a      	moveal %fp@(-134),%a0
    1000:	d1ee ffb6      	addal %fp@(-74),%a0
    1004:	41e8 4008      	lea %a0@(16392),%a0
    1008:	2008           	movel %a0,%d0
    100a:	90ae ff7a      	subl %fp@(-134),%d0
    100e:	2f00           	movel %d0,%sp@-
    1010:	2f2e ff7a      	movel %fp@(-134),%sp@-
    1014:	4e4f           	trap #15
    1016:	a076           	0120166
    1018:	206a 0014      	moveal %a2@(20),%a0
    101c:	d1ee ffb6      	addal %fp@(-74),%a0
    1020:	7010           	moveq #16,%d0
    1022:	b0a8 4008      	cmpl %a0@(16392),%d0
    1026:	4fef 0018      	lea %sp@(24),%sp
    102a:	6262           	bhis 0x108e
    102c:	157c 0001 437c 	moveb #1,%a2@(17276)
    1032:	157c 0001 437b 	moveb #1,%a2@(17275)
    1038:	7002           	moveq #2,%d0
    103a:	81aa 0004      	orl %d0,%a2@(4)
    103e:	7010           	moveq #16,%d0
    1040:	81aa 4382      	orl %d0,%a2@(17282)
    1044:	6048           	bras 0x108e
    1046:	7800           	moveq #0,%d4
    1048:	260a           	movel %a2,%d3
    104a:	6036           	bras 0x1082
    104c:	2043           	moveal %d3,%a0
    104e:	117c 00f8 4018 	moveb #-8,%a0@(16408)
    1054:	117c 00ff 4019 	moveb #-1,%a0@(16409)
    105a:	3147 401a      	movew %d7,%a0@(16410)
    105e:	117c 00ff 401c 	moveb #-1,%a0@(16412)
    1064:	117c 00ff 401d 	moveb #-1,%a0@(16413)
    106a:	117c 00ff 401e 	moveb #-1,%a0@(16414)
    1070:	117c 00ff 401f 	moveb #-1,%a0@(16415)
    1076:	117c 00ff 4020 	moveb #-1,%a0@(16416)
    107c:	700a           	moveq #10,%d0
    107e:	d680           	addl %d0,%d3
    1080:	5284           	addql #1,%d4
    1082:	7000           	moveq #0,%d0
    1084:	102a 437d      	moveb %a2@(17277),%d0
    1088:	48c0           	extl %d0
    108a:	b880           	cmpl %d0,%d4
    108c:	6dbe           	blts 0x104c
    108e:	262e ffb6      	movel %fp@(-74),%d3
    1092:	7000           	moveq #0,%d0
    1094:	302e ff64      	movew %fp@(-156),%d0
    1098:	d080           	addl %d0,%d0
    109a:	2d40 ffa0      	movel %d0,%fp@(-96)
    109e:	2d40 ffa4      	movel %d0,%fp@(-92)
    10a2:	7001           	moveq #1,%d0
    10a4:	c086           	andl %d6,%d0
    10a6:	2d40 ff98      	movel %d0,%fp@(-104)
    10aa:	7e00           	moveq #0,%d7
    10ac:	1e2e 000c      	moveb %fp@(12),%d7
    10b0:	7209           	moveq #9,%d1
    10b2:	e36f           	lslw %d1,%d7
    10b4:	48c7           	extl %d7
    10b6:	7000           	moveq #0,%d0
    10b8:	102e 000a      	moveb %fp@(10),%d0
    10bc:	e368           	lslw %d1,%d0
    10be:	204a           	moveal %a2,%a0
    10c0:	d0c0           	addaw %d0,%a0
    10c2:	41e8 0018      	lea %a0@(24),%a0
    10c6:	2d48 ffaa      	movel %a0,%fp@(-86)
    10ca:	4a2a 437c      	tstb %a2@(17276)
    10ce:	6706           	beqs 0x10d6
    10d0:	7014           	moveq #20,%d0
    10d2:	6000 05c8      	braw 0x169c
    10d6:	2003           	movel %d3,%d0
    10d8:	d0aa 0014      	addl %a2@(20),%d0
    10dc:	2a00           	movel %d0,%d5
    10de:	2040           	moveal %d0,%a0
    10e0:	2828 4004      	movel %a0@(16388),%d4
    10e4:	2045           	moveal %d5,%a0
    10e6:	2d68 4000 fffc 	movel %a0@(16384),%fp@(-4)
    10ec:	603a           	bras 0x1128
    10ee:	54ae fffc      	addql #2,%fp@(-4)
    10f2:	266a 0014      	moveal %a2@(20),%a3
    10f6:	2003           	movel %d3,%d0
    10f8:	d08b           	addl %a3,%d0
    10fa:	2040           	moveal %d0,%a0
    10fc:	2028 4004      	movel %a0@(16388),%d0
    1100:	d080           	addl %d0,%d0
    1102:	204b           	moveal %a3,%a0
    1104:	d1c3           	addal %d3,%a0
    1106:	2a08           	movel %a0,%d5
    1108:	d1c0           	addal %d0,%a0
    110a:	41e8 400c      	lea %a0@(16396),%a0
    110e:	b1ee fffc      	cmpal %fp@(-4),%a0
    1112:	660a           	bnes 0x111e
    1114:	2045           	moveal %d5,%a0
    1116:	41e8 400c      	lea %a0@(16396),%a0
    111a:	2d48 fffc      	movel %a0,%fp@(-4)
    111e:	5384           	subql #1,%d4
    1120:	6606           	bnes 0x1128
    1122:	7014           	moveq #20,%d0
    1124:	6000 0576      	braw 0x169c
    1128:	206e fffc      	moveal %fp@(-4),%a0
    112c:	0c50 ffff      	cmpiw #-1,%a0@
    1130:	67bc           	beqs 0x10ee
    1132:	2d6a 0014 ffca 	movel %a2@(20),%fp@(-54)
    1138:	4878 0004      	pea 0x4
    113c:	486e fffc      	pea %fp@(-4)
    1140:	206e ffca      	moveal %fp@(-54),%a0
    1144:	d1c3           	addal %d3,%a0
    1146:	41e8 4000      	lea %a0@(16384),%a0
    114a:	2008           	movel %a0,%d0
    114c:	90ae ffca      	subl %fp@(-54),%d0
    1150:	2f00           	movel %d0,%sp@-
    1152:	2f2e ffca      	movel %fp@(-54),%sp@-
    1156:	4e4f           	trap #15
    1158:	a076           	0120166
    115a:	2003           	movel %d3,%d0
    115c:	d0aa 0014      	addl %a2@(20),%d0
    1160:	2040           	moveal %d0,%a0
    1162:	2068 4000      	moveal %a0@(16384),%a0
    1166:	3810           	movew %a0@,%d4
    1168:	486e fff6      	pea %fp@(-10)
    116c:	3f04           	movew %d4,%sp@-
    116e:	4eba 2140      	jsr %pc@(0x32b0)
    1172:	4a80           	tstl %d0
    1174:	4fef 0016      	lea %sp@(22),%sp
    1178:	6700 00ba      	beqw 0x1234
    117c:	202e fff6      	movel %fp@(-10),%d0
    1180:	0280 0000 4000 	andil #16384,%d0
    1186:	6706           	beqs 0x118e
    1188:	7009           	moveq #9,%d0
    118a:	6000 0510      	braw 0x169c
    118e:	1d7c 007f fffb 	moveb #127,%fp@(-5)
    1194:	486e fffb      	pea %fp@(-5)
    1198:	4227           	clrb %sp@-
    119a:	3f04           	movew %d4,%sp@-
    119c:	4eba 16ce      	jsr %pc@(0x286c)
    11a0:	2d6a 0014 ffe2 	movel %a2@(20),%fp@(-30)
    11a6:	1f3c 00ff      	moveb #-1,%sp@-
    11aa:	4878 0002      	pea 0x2
    11ae:	2003           	movel %d3,%d0
    11b0:	d0ae ffe2      	addl %fp@(-30),%d0
    11b4:	2040           	moveal %d0,%a0
    11b6:	2028 4000      	movel %a0@(16384),%d0
    11ba:	90ae ffe2      	subl %fp@(-30),%d0
    11be:	2f00           	movel %d0,%sp@-
    11c0:	2f2e ffe2      	movel %fp@(-30),%sp@-
    11c4:	4e4f           	trap #15
    11c6:	a07e           	0120176
    11c8:	2003           	movel %d3,%d0
    11ca:	d0aa 0014      	addl %a2@(20),%d0
    11ce:	2040           	moveal %d0,%a0
    11d0:	2028 4008      	movel %a0@(16392),%d0
    11d4:	5280           	addql #1,%d0
    11d6:	2d40 fff2      	movel %d0,%fp@(-14)
    11da:	2d6a 0014 ffc6 	movel %a2@(20),%fp@(-58)
    11e0:	4878 0004      	pea 0x4
    11e4:	486e fff2      	pea %fp@(-14)
    11e8:	206e ffc6      	moveal %fp@(-58),%a0
    11ec:	d1c3           	addal %d3,%a0
    11ee:	41e8 4008      	lea %a0@(16392),%a0
    11f2:	2008           	movel %a0,%d0
    11f4:	90ae ffc6      	subl %fp@(-58),%d0
    11f8:	2f00           	movel %d0,%sp@-
    11fa:	2f2e ffc6      	movel %fp@(-58),%sp@-
    11fe:	4e4f           	trap #15
    1200:	a076           	0120166
    1202:	2003           	movel %d3,%d0
    1204:	d0aa 0014      	addl %a2@(20),%d0
    1208:	2040           	moveal %d0,%a0
    120a:	7010           	moveq #16,%d0
    120c:	b0a8 4008      	cmpl %a0@(16392),%d0
    1210:	4fef 0026      	lea %sp@(38),%sp
    1214:	6200 feb4      	bhiw 0x10ca
    1218:	157c 0001 437c 	moveb #1,%a2@(17276)
    121e:	157c 0001 437b 	moveb #1,%a2@(17275)
    1224:	7002           	moveq #2,%d0
    1226:	81aa 0004      	orl %d0,%a2@(4)
    122a:	7010           	moveq #16,%d0
    122c:	81aa 4382      	orl %d0,%a2@(17282)
    1230:	6000 fe98      	braw 0x10ca
    1234:	1d7c 00ef fffb 	moveb #-17,%fp@(-5)
    123a:	486e fffb      	pea %fp@(-5)
    123e:	4227           	clrb %sp@-
    1240:	206e ffa4      	moveal %fp@(-92),%a0
    1244:	202a 0014      	movel %a2@(20),%d0
    1248:	3f30 0800      	movew %a0@(00000000,%d0:l),%sp@-
    124c:	4eba 161e      	jsr %pc@(0x286c)
    1250:	1a2a 437d      	moveb %a2@(17277),%d5
    1254:	ba2e 000c      	cmpb %fp@(12),%d5
    1258:	504f           	addqw #8,%sp
    125a:	6600 0138      	bnew 0x1394
    125e:	4a86           	tstl %d6
    1260:	6624           	bnes 0x1286
    1262:	4227           	clrb %sp@-
    1264:	7000           	moveq #0,%d0
    1266:	1005           	moveb %d5,%d0
    1268:	7209           	moveq #9,%d1
    126a:	e368           	lslw %d1,%d0
    126c:	48c0           	extl %d0
    126e:	2f00           	movel %d0,%sp@-
    1270:	486a 0018      	pea %a2@(24)
    1274:	4e4f           	trap #15
    1276:	a027           	0120047
    1278:	204a           	moveal %a2,%a0
    127a:	41e8 0018      	lea %a0@(24),%a0
    127e:	2a08           	movel %a0,%d5
    1280:	4fef 000a      	lea %sp@(10),%sp
    1284:	602c           	bras 0x12b2
    1286:	4aae ff98      	tstl %fp@(-104)
    128a:	6724           	beqs 0x12b0
    128c:	7000           	moveq #0,%d0
    128e:	1005           	moveb %d5,%d0
    1290:	7209           	moveq #9,%d1
    1292:	e368           	lslw %d1,%d0
    1294:	48c0           	extl %d0
    1296:	2f00           	movel %d0,%sp@-
    1298:	2f06           	movel %d6,%sp@-
    129a:	486a 0018      	pea %a2@(24)
    129e:	4e4f           	trap #15
    12a0:	a026           	0120046
    12a2:	204a           	moveal %a2,%a0
    12a4:	41e8 0018      	lea %a0@(24),%a0
    12a8:	2a08           	movel %a0,%d5
    12aa:	4fef 000c      	lea %sp@(12),%sp
    12ae:	6002           	bras 0x12b2
    12b0:	2a06           	movel %d6,%d5
    12b2:	2f2e 0012      	movel %fp@(18),%sp@-
    12b6:	486e fff6      	pea %fp@(-10)
    12ba:	486a 4018      	pea %a2@(16408)
    12be:	2f05           	movel %d5,%sp@-
    12c0:	1f2a 437d      	moveb %a2@(17277),%sp@-
    12c4:	4227           	clrb %sp@-
    12c6:	3f04           	movew %d4,%sp@-
    12c8:	4eba 16ee      	jsr %pc@(0x29b8)
    12cc:	4a80           	tstl %d0
    12ce:	4fef 0016      	lea %sp@(22),%sp
    12d2:	6700 01ca      	beqw 0x149e
    12d6:	202e fff6      	movel %fp@(-10),%d0
    12da:	0280 0000 4000 	andil #16384,%d0
    12e0:	6706           	beqs 0x12e8
    12e2:	7009           	moveq #9,%d0
    12e4:	6000 03b6      	braw 0x169c
    12e8:	1d7c 007f fffb 	moveb #127,%fp@(-5)
    12ee:	486e fffb      	pea %fp@(-5)
    12f2:	4227           	clrb %sp@-
    12f4:	3f04           	movew %d4,%sp@-
    12f6:	4eba 1574      	jsr %pc@(0x286c)
    12fa:	2d6a 0014 ffde 	movel %a2@(20),%fp@(-34)
    1300:	1f3c 00ff      	moveb #-1,%sp@-
    1304:	4878 0002      	pea 0x2
    1308:	2003           	movel %d3,%d0
    130a:	d0ae ffde      	addl %fp@(-34),%d0
    130e:	2040           	moveal %d0,%a0
    1310:	2028 4000      	movel %a0@(16384),%d0
    1314:	90ae ffde      	subl %fp@(-34),%d0
    1318:	2f00           	movel %d0,%sp@-
    131a:	2f2e ffde      	movel %fp@(-34),%sp@-
    131e:	4e4f           	trap #15
    1320:	a07e           	0120176
    1322:	2003           	movel %d3,%d0
    1324:	d0aa 0014      	addl %a2@(20),%d0
    1328:	2040           	moveal %d0,%a0
    132a:	2028 4008      	movel %a0@(16392),%d0
    132e:	5280           	addql #1,%d0
    1330:	2d40 fff2      	movel %d0,%fp@(-14)
    1334:	2d6a 0014 ffc2 	movel %a2@(20),%fp@(-62)
    133a:	4878 0004      	pea 0x4
    133e:	486e fff2      	pea %fp@(-14)
    1342:	206e ffc2      	moveal %fp@(-62),%a0
    1346:	d1c3           	addal %d3,%a0
    1348:	41e8 4008      	lea %a0@(16392),%a0
    134c:	2008           	movel %a0,%d0
    134e:	90ae ffc2      	subl %fp@(-62),%d0
    1352:	2f00           	movel %d0,%sp@-
    1354:	2f2e ffc2      	movel %fp@(-62),%sp@-
    1358:	4e4f           	trap #15
    135a:	a076           	0120166
    135c:	2003           	movel %d3,%d0
    135e:	d0aa 0014      	addl %a2@(20),%d0
    1362:	2040           	moveal %d0,%a0
    1364:	7010           	moveq #16,%d0
    1366:	b0a8 4008      	cmpl %a0@(16392),%d0
    136a:	4fef 0026      	lea %sp@(38),%sp
    136e:	6218           	bhis 0x1388
    1370:	157c 0001 437c 	moveb #1,%a2@(17276)
    1376:	157c 0001 437b 	moveb #1,%a2@(17275)
    137c:	7002           	moveq #2,%d0
    137e:	81aa 0004      	orl %d0,%a2@(4)
    1382:	7010           	moveq #16,%d0
    1384:	81aa 4382      	orl %d0,%a2@(17282)
    1388:	206e 0012      	moveal %fp@(18),%a0
    138c:	10ae 000a      	moveb %fp@(10),%a0@
    1390:	6000 fd38      	braw 0x10ca
    1394:	4a86           	tstl %d6
    1396:	6612           	bnes 0x13aa
    1398:	4227           	clrb %sp@-
    139a:	2f07           	movel %d7,%sp@-
    139c:	2f2e ffaa      	movel %fp@(-86),%sp@-
    13a0:	4e4f           	trap #15
    13a2:	a027           	0120047
    13a4:	4fef 000a      	lea %sp@(10),%sp
    13a8:	6010           	bras 0x13ba
    13aa:	2f07           	movel %d7,%sp@-
    13ac:	2f06           	movel %d6,%sp@-
    13ae:	2f2e ffaa      	movel %fp@(-86),%sp@-
    13b2:	4e4f           	trap #15
    13b4:	a026           	0120046
    13b6:	4fef 000c      	lea %sp@(12),%sp
    13ba:	2f2e 0012      	movel %fp@(18),%sp@-
    13be:	486e fff6      	pea %fp@(-10)
    13c2:	486a 4018      	pea %a2@(16408)
    13c6:	486a 0018      	pea %a2@(24)
    13ca:	1f2a 437d      	moveb %a2@(17277),%sp@-
    13ce:	4227           	clrb %sp@-
    13d0:	3f04           	movew %d4,%sp@-
    13d2:	4eba 15e4      	jsr %pc@(0x29b8)
    13d6:	4a80           	tstl %d0
    13d8:	4fef 0016      	lea %sp@(22),%sp
    13dc:	6700 00c0      	beqw 0x149e
    13e0:	202e fff6      	movel %fp@(-10),%d0
    13e4:	0280 0000 4000 	andil #16384,%d0
    13ea:	6706           	beqs 0x13f2
    13ec:	7009           	moveq #9,%d0
    13ee:	6000 02ac      	braw 0x169c
    13f2:	1d7c 007f fffb 	moveb #127,%fp@(-5)
    13f8:	486e fffb      	pea %fp@(-5)
    13fc:	4227           	clrb %sp@-
    13fe:	3f04           	movew %d4,%sp@-
    1400:	4eba 146a      	jsr %pc@(0x286c)
    1404:	2d6a 0014 ffda 	movel %a2@(20),%fp@(-38)
    140a:	1f3c 00ff      	moveb #-1,%sp@-
    140e:	4878 0002      	pea 0x2
    1412:	2003           	movel %d3,%d0
    1414:	d0ae ffda      	addl %fp@(-38),%d0
    1418:	2040           	moveal %d0,%a0
    141a:	2028 4000      	movel %a0@(16384),%d0
    141e:	90ae ffda      	subl %fp@(-38),%d0
    1422:	2f00           	movel %d0,%sp@-
    1424:	2f2e ffda      	movel %fp@(-38),%sp@-
    1428:	4e4f           	trap #15
    142a:	a07e           	0120176
    142c:	2003           	movel %d3,%d0
    142e:	d0aa 0014      	addl %a2@(20),%d0
    1432:	2040           	moveal %d0,%a0
    1434:	2028 4008      	movel %a0@(16392),%d0
    1438:	5280           	addql #1,%d0
    143a:	2d40 fff2      	movel %d0,%fp@(-14)
    143e:	2d6a 0014 ffbe 	movel %a2@(20),%fp@(-66)
    1444:	4878 0004      	pea 0x4
    1448:	486e fff2      	pea %fp@(-14)
    144c:	206e ffbe      	moveal %fp@(-66),%a0
    1450:	d1c3           	addal %d3,%a0
    1452:	41e8 4008      	lea %a0@(16392),%a0
    1456:	2008           	movel %a0,%d0
    1458:	90ae ffbe      	subl %fp@(-66),%d0
    145c:	2f00           	movel %d0,%sp@-
    145e:	2f2e ffbe      	movel %fp@(-66),%sp@-
    1462:	4e4f           	trap #15
    1464:	a076           	0120166
    1466:	2003           	movel %d3,%d0
    1468:	d0aa 0014      	addl %a2@(20),%d0
    146c:	2040           	moveal %d0,%a0
    146e:	7010           	moveq #16,%d0
    1470:	b0a8 4008      	cmpl %a0@(16392),%d0
    1474:	4fef 0026      	lea %sp@(38),%sp
    1478:	6218           	bhis 0x1492
    147a:	157c 0001 437c 	moveb #1,%a2@(17276)
    1480:	157c 0001 437b 	moveb #1,%a2@(17275)
    1486:	7002           	moveq #2,%d0
    1488:	81aa 0004      	orl %d0,%a2@(4)
    148c:	7010           	moveq #16,%d0
    148e:	81aa 4382      	orl %d0,%a2@(17282)
    1492:	206e 0012      	moveal %fp@(18),%a0
    1496:	10ae 000a      	moveb %fp@(10),%a0@
    149a:	6000 fc2e      	braw 0x10ca
    149e:	4aae ffb2      	tstl %fp@(-78)
    14a2:	675a           	beqs 0x14fe
    14a4:	2d6a 0014 ff86 	movel %a2@(20),%fp@(-122)
    14aa:	1f3c 00ff      	moveb #-1,%sp@-
    14ae:	4878 0002      	pea 0x2
    14b2:	206e ff86      	moveal %fp@(-122),%a0
    14b6:	d1ee ffb6      	addal %fp@(-74),%a0
    14ba:	2028 4000      	movel %a0@(16384),%d0
    14be:	90ae ff86      	subl %fp@(-122),%d0
    14c2:	2f00           	movel %d0,%sp@-
    14c4:	2f2e ff86      	movel %fp@(-122),%sp@-
    14c8:	4e4f           	trap #15
    14ca:	a07e           	0120176
    14cc:	3d44 ff92      	movew %d4,%fp@(-110)
    14d0:	2d6a 0014 ff76 	movel %a2@(20),%fp@(-138)
    14d6:	4878 0002      	pea 0x2
    14da:	486e ff92      	pea %fp@(-110)
    14de:	206e ff76      	moveal %fp@(-138),%a0
    14e2:	d1ee ffa0      	addal %fp@(-96),%a0
    14e6:	2008           	movel %a0,%d0
    14e8:	90ae ff76      	subl %fp@(-138),%d0
    14ec:	2f00           	movel %d0,%sp@-
    14ee:	2f2e ff76      	movel %fp@(-138),%sp@-
    14f2:	4e4f           	trap #15
    14f4:	a076           	0120166
    14f6:	4fef 001e      	lea %sp@(30),%sp
    14fa:	6000 0134      	braw 0x1630
    14fe:	486e fff6      	pea %fp@(-10)
    1502:	206a 0014      	moveal %a2@(20),%a0
    1506:	202e ffa0      	movel %fp@(-96),%d0
    150a:	3f30 0800      	movew %a0@(00000000,%d0:l),%sp@-
    150e:	4eba 1da0      	jsr %pc@(0x32b0)
    1512:	4a80           	tstl %d0
    1514:	5c4f           	addqw #6,%sp
    1516:	6700 00be      	beqw 0x15d6
    151a:	202e fff6      	movel %fp@(-10),%d0
    151e:	0280 0000 4000 	andil #16384,%d0
    1524:	6706           	beqs 0x152c
    1526:	7009           	moveq #9,%d0
    1528:	6000 0172      	braw 0x169c
    152c:	1d7c 007f fffb 	moveb #127,%fp@(-5)
    1532:	486e fffb      	pea %fp@(-5)
    1536:	4227           	clrb %sp@-
    1538:	206a 0014      	moveal %a2@(20),%a0
    153c:	202e ffa0      	movel %fp@(-96),%d0
    1540:	3f30 0800      	movew %a0@(00000000,%d0:l),%sp@-
    1544:	4eba 1326      	jsr %pc@(0x286c)
    1548:	2d6a 0014 ff72 	movel %a2@(20),%fp@(-142)
    154e:	1f3c 00ff      	moveb #-1,%sp@-
    1552:	4878 0002      	pea 0x2
    1556:	206e ff72      	moveal %fp@(-142),%a0
    155a:	d1ee ffa0      	addal %fp@(-96),%a0
    155e:	2008           	movel %a0,%d0
    1560:	90ae ff72      	subl %fp@(-142),%d0
    1564:	2f00           	movel %d0,%sp@-
    1566:	2f2e ff72      	movel %fp@(-142),%sp@-
    156a:	4e4f           	trap #15
    156c:	a07e           	0120176
    156e:	206a 0014      	moveal %a2@(20),%a0
    1572:	d1ee ffb6      	addal %fp@(-74),%a0
    1576:	2028 4008      	movel %a0@(16392),%d0
    157a:	5280           	addql #1,%d0
    157c:	2d40 fff2      	movel %d0,%fp@(-14)
    1580:	2d6a 0014 ff6e 	movel %a2@(20),%fp@(-146)
    1586:	4878 0004      	pea 0x4
    158a:	486e fff2      	pea %fp@(-14)
    158e:	206e ff6e      	moveal %fp@(-146),%a0
    1592:	d1ee ffb6      	addal %fp@(-74),%a0
    1596:	41e8 4008      	lea %a0@(16392),%a0
    159a:	2008           	movel %a0,%d0
    159c:	90ae ff6e      	subl %fp@(-146),%d0
    15a0:	2f00           	movel %d0,%sp@-
    15a2:	2f2e ff6e      	movel %fp@(-146),%sp@-
    15a6:	4e4f           	trap #15
    15a8:	a076           	0120166
    15aa:	206a 0014      	moveal %a2@(20),%a0
    15ae:	d1ee ffb6      	addal %fp@(-74),%a0
    15b2:	7010           	moveq #16,%d0
    15b4:	b0a8 4008      	cmpl %a0@(16392),%d0
    15b8:	4fef 0026      	lea %sp@(38),%sp
    15bc:	6218           	bhis 0x15d6
    15be:	157c 0001 437c 	moveb #1,%a2@(17276)
    15c4:	157c 0001 437b 	moveb #1,%a2@(17275)
    15ca:	7002           	moveq #2,%d0
    15cc:	81aa 0004      	orl %d0,%a2@(4)
    15d0:	7010           	moveq #16,%d0
    15d2:	81aa 4382      	orl %d0,%a2@(17282)
    15d6:	3d44 ff92      	movew %d4,%fp@(-110)
    15da:	2d6a 0014 ff8e 	movel %a2@(20),%fp@(-114)
    15e0:	4878 0002      	pea 0x2
    15e4:	206e ff8e      	moveal %fp@(-114),%a0
    15e8:	202e ffa0      	movel %fp@(-96),%d0
    15ec:	4870 0800      	pea %a0@(00000000,%d0:l)
    15f0:	d1ee ffb6      	addal %fp@(-74),%a0
    15f4:	2028 4000      	movel %a0@(16384),%d0
    15f8:	90ae ff8e      	subl %fp@(-114),%d0
    15fc:	2f00           	movel %d0,%sp@-
    15fe:	2f2e ff8e      	movel %fp@(-114),%sp@-
    1602:	4e4f           	trap #15
    1604:	a076           	0120166
    1606:	2d6a 0014 ff6a 	movel %a2@(20),%fp@(-150)
    160c:	4878 0002      	pea 0x2
    1610:	486e ff92      	pea %fp@(-110)
    1614:	206e ff6a      	moveal %fp@(-150),%a0
    1618:	d1ee ffa0      	addal %fp@(-96),%a0
    161c:	2008           	movel %a0,%d0
    161e:	90ae ff6a      	subl %fp@(-150),%d0
    1622:	2f00           	movel %d0,%sp@-
    1624:	2f2e ff6a      	movel %fp@(-150),%sp@-
    1628:	4e4f           	trap #15
    162a:	a076           	0120166
    162c:	4fef 0020      	lea %sp@(32),%sp
    1630:	206a 0014      	moveal %a2@(20),%a0
    1634:	d1ee ffb6      	addal %fp@(-74),%a0
    1638:	2068 4000      	moveal %a0@(16384),%a0
    163c:	5488           	addql #2,%a0
    163e:	2d48 fffc      	movel %a0,%fp@(-4)
    1642:	206a 0014      	moveal %a2@(20),%a0
    1646:	d1ee ffb6      	addal %fp@(-74),%a0
    164a:	2d48 ff82      	movel %a0,%fp@(-126)
    164e:	2028 4004      	movel %a0@(16388),%d0
    1652:	d080           	addl %d0,%d0
    1654:	d1c0           	addal %d0,%a0
    1656:	41e8 400c      	lea %a0@(16396),%a0
    165a:	b1ee fffc      	cmpal %fp@(-4),%a0
    165e:	660c           	bnes 0x166c
    1660:	206e ff82      	moveal %fp@(-126),%a0
    1664:	41e8 400c      	lea %a0@(16396),%a0
    1668:	2d48 fffc      	movel %a0,%fp@(-4)
    166c:	2d6a 0014 ff66 	movel %a2@(20),%fp@(-154)
    1672:	4878 0004      	pea 0x4
    1676:	486e fffc      	pea %fp@(-4)
    167a:	206e ff66      	moveal %fp@(-154),%a0
    167e:	d1ee ffb6      	addal %fp@(-74),%a0
    1682:	41e8 4000      	lea %a0@(16384),%a0
    1686:	2008           	movel %a0,%d0
    1688:	90ae ff66      	subl %fp@(-154),%d0
    168c:	2f00           	movel %d0,%sp@-
    168e:	2f2e ff66      	movel %fp@(-154),%sp@-
    1692:	4e4f           	trap #15
    1694:	a076           	0120166
    1696:	7000           	moveq #0,%d0
    1698:	4fef 0010      	lea %sp@(16),%sp
    169c:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    16a0:	4e5e           	unlk %fp
    16a2:	4e75           	rts

```

# job_writeSector: 0x16b6 - 0x1830


```

    16b6:	4e56 fff6      	linkw %fp,#-10
    16ba:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    16be:	246e 0014      	moveal %fp@(20),%a2
    16c2:	266e 000c      	moveal %fp@(12),%a3
    16c6:	206e 0010      	moveal %fp@(16),%a0
    16ca:	2610           	movel %a0@,%d3
    16cc:	4a2a 437c      	tstb %a2@(17276)
    16d0:	6708           	beqs 0x16da
    16d2:	4290           	clrl %a0@
    16d4:	7012           	moveq #18,%d0
    16d6:	6000 0152      	braw 0x182a
    16da:	7000           	moveq #0,%d0
    16dc:	102a 437d      	moveb %a2@(17277),%d0
    16e0:	3e00           	movew %d0,%d7
    16e2:	48c0           	extl %d0
    16e4:	2c00           	movel %d0,%d6
    16e6:	2200           	movel %d0,%d1
    16e8:	202e 0008      	movel %fp@(8),%d0
    16ec:	4eba 3528      	jsr %pc@(0x4c16)
    16f0:	3800           	movew %d0,%d4
    16f2:	202e 0008      	movel %fp@(8),%d0
    16f6:	2206           	movel %d6,%d1
    16f8:	4eba 3574      	jsr %pc@(0x4c6e)
    16fc:	2d40 fff6      	movel %d0,%fp@(-10)
    1700:	1a00           	moveb %d0,%d5
    1702:	7000           	moveq #0,%d0
    1704:	102e fff9      	moveb %fp@(-7),%d0
    1708:	3207           	movew %d7,%d1
    170a:	9240           	subw %d0,%d1
    170c:	48c1           	extl %d1
    170e:	b283           	cmpl %d3,%d1
    1710:	640e           	bccs 0x1720
    1712:	7000           	moveq #0,%d0
    1714:	1005           	moveb %d5,%d0
    1716:	3207           	movew %d7,%d1
    1718:	9240           	subw %d0,%d1
    171a:	48c1           	extl %d1
    171c:	2001           	movel %d1,%d0
    171e:	6002           	bras 0x1722
    1720:	2003           	movel %d3,%d0
    1722:	1c00           	moveb %d0,%d6
    1724:	2f0a           	movel %a2,%sp@-
    1726:	486e ffff      	pea %fp@(-1)
    172a:	2f0b           	movel %a3,%sp@-
    172c:	1f06           	moveb %d6,%sp@-
    172e:	1f05           	moveb %d5,%sp@-
    1730:	3f04           	movew %d4,%sp@-
    1732:	4eba f35a      	jsr %pc@(0xa8e)
    1736:	2e00           	movel %d0,%d7
    1738:	4fef 0012      	lea %sp@(18),%sp
    173c:	671a           	beqs 0x1758
    173e:	7000           	moveq #0,%d0
    1740:	1005           	moveb %d5,%d0
    1742:	122e ffff      	moveb %fp@(-1),%d1
    1746:	4881           	extw %d1
    1748:	9240           	subw %d0,%d1
    174a:	48c1           	extl %d1
    174c:	206e 0010      	moveal %fp@(16),%a0
    1750:	2081           	movel %d1,%a0@
    1752:	2007           	movel %d7,%d0
    1754:	6000 00d4      	braw 0x182a
    1758:	7000           	moveq #0,%d0
    175a:	1006           	moveb %d6,%d0
    175c:	9680           	subl %d0,%d3
    175e:	6606           	bnes 0x1766
    1760:	7000           	moveq #0,%d0
    1762:	6000 00c6      	braw 0x182a
    1766:	4aae 000c      	tstl %fp@(12)
    176a:	670c           	beqs 0x1778
    176c:	7000           	moveq #0,%d0
    176e:	1006           	moveb %d6,%d0
    1770:	7209           	moveq #9,%d1
    1772:	e368           	lslw %d1,%d0
    1774:	48c0           	extl %d0
    1776:	d7c0           	addal %d0,%a3
    1778:	7000           	moveq #0,%d0
    177a:	102a 437d      	moveb %a2@(17277),%d0
    177e:	3a00           	movew %d0,%d5
    1780:	48c0           	extl %d0
    1782:	2200           	movel %d0,%d1
    1784:	2003           	movel %d3,%d0
    1786:	4eba 348e      	jsr %pc@(0x4c16)
    178a:	2e00           	movel %d0,%d7
    178c:	3005           	movew %d5,%d0
    178e:	7209           	moveq #9,%d1
    1790:	e368           	lslw %d1,%d0
    1792:	48c0           	extl %d0
    1794:	2d40 fffa      	movel %d0,%fp@(-6)
    1798:	7a00           	moveq #0,%d5
    179a:	4a87           	tstl %d7
    179c:	6f4c           	bles 0x17ea
    179e:	5244           	addqw #1,%d4
    17a0:	2f0a           	movel %a2,%sp@-
    17a2:	486e ffff      	pea %fp@(-1)
    17a6:	2f0b           	movel %a3,%sp@-
    17a8:	1f2a 437d      	moveb %a2@(17277),%sp@-
    17ac:	4227           	clrb %sp@-
    17ae:	3f04           	movew %d4,%sp@-
    17b0:	4eba f2dc      	jsr %pc@(0xa8e)
    17b4:	2c00           	movel %d0,%d6
    17b6:	4fef 0012      	lea %sp@(18),%sp
    17ba:	6716           	beqs 0x17d2
    17bc:	102e ffff      	moveb %fp@(-1),%d0
    17c0:	4880           	extw %d0
    17c2:	48c0           	extl %d0
    17c4:	2203           	movel %d3,%d1
    17c6:	9280           	subl %d0,%d1
    17c8:	206e 0010      	moveal %fp@(16),%a0
    17cc:	9390           	subl %d1,%a0@
    17ce:	2006           	movel %d6,%d0
    17d0:	6058           	bras 0x182a
    17d2:	4aae 000c      	tstl %fp@(12)
    17d6:	6704           	beqs 0x17dc
    17d8:	d7ee fffa      	addal %fp@(-6),%a3
    17dc:	7000           	moveq #0,%d0
    17de:	102a 437d      	moveb %a2@(17277),%d0
    17e2:	9680           	subl %d0,%d3
    17e4:	5285           	addql #1,%d5
    17e6:	ba87           	cmpl %d7,%d5
    17e8:	6db4           	blts 0x179e
    17ea:	4a83           	tstl %d3
    17ec:	6604           	bnes 0x17f2
    17ee:	7000           	moveq #0,%d0
    17f0:	6038           	bras 0x182a
    17f2:	1c03           	moveb %d3,%d6
    17f4:	2f0a           	movel %a2,%sp@-
    17f6:	486e ffff      	pea %fp@(-1)
    17fa:	2f0b           	movel %a3,%sp@-
    17fc:	1f06           	moveb %d6,%sp@-
    17fe:	4227           	clrb %sp@-
    1800:	3004           	movew %d4,%d0
    1802:	5240           	addqw #1,%d0
    1804:	3f00           	movew %d0,%sp@-
    1806:	4eba f286      	jsr %pc@(0xa8e)
    180a:	2a00           	movel %d0,%d5
    180c:	4fef 0012      	lea %sp@(18),%sp
    1810:	6716           	beqs 0x1828
    1812:	102e ffff      	moveb %fp@(-1),%d0
    1816:	4880           	extw %d0
    1818:	48c0           	extl %d0
    181a:	2203           	movel %d3,%d1
    181c:	9280           	subl %d0,%d1
    181e:	206e 0010      	moveal %fp@(16),%a0
    1822:	9390           	subl %d1,%a0@
    1824:	2005           	movel %d5,%d0
    1826:	6002           	bras 0x182a
    1828:	7000           	moveq #0,%d0
    182a:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    182e:	4e5e           	unlk %fp
    1830:	4e75           	rts

```

# job_readSector: 0x1844 - 0x1cc8


```

    1844:	4e56 ffda      	linkw %fp,#-38
    1848:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    184c:	246e 0014      	moveal %fp@(20),%a2
    1850:	41fa eb12      	lea %pc@(0x364),%a0
    1854:	2d48 ffe6      	movel %a0,%fp@(-26)
    1858:	266e 000c      	moveal %fp@(12),%a3
    185c:	206e 0010      	moveal %fp@(16),%a0
    1860:	2d50 ffde      	movel %a0@,%fp@(-34)
    1864:	2a10           	movel %a0@,%d5
    1866:	7000           	moveq #0,%d0
    1868:	102a 437d      	moveb %a2@(17277),%d0
    186c:	3600           	movew %d0,%d3
    186e:	48c0           	extl %d0
    1870:	2c00           	movel %d0,%d6
    1872:	2200           	movel %d0,%d1
    1874:	202e 0008      	movel %fp@(8),%d0
    1878:	4eba 339c      	jsr %pc@(0x4c16)
    187c:	3800           	movew %d0,%d4
    187e:	202e 0008      	movel %fp@(8),%d0
    1882:	2206           	movel %d6,%d1
    1884:	4eba 33e8      	jsr %pc@(0x4c6e)
    1888:	2d40 ffe2      	movel %d0,%fp@(-30)
    188c:	1e00           	moveb %d0,%d7
    188e:	7000           	moveq #0,%d0
    1890:	102e ffe5      	moveb %fp@(-27),%d0
    1894:	3203           	movew %d3,%d1
    1896:	9240           	subw %d0,%d1
    1898:	48c1           	extl %d1
    189a:	b2ae ffde      	cmpl %fp@(-34),%d1
    189e:	640e           	bccs 0x18ae
    18a0:	7000           	moveq #0,%d0
    18a2:	1007           	moveb %d7,%d0
    18a4:	3203           	movew %d3,%d1
    18a6:	9240           	subw %d0,%d1
    18a8:	48c1           	extl %d1
    18aa:	2001           	movel %d1,%d0
    18ac:	6002           	bras 0x18b0
    18ae:	2005           	movel %d5,%d0
    18b0:	1d40 fff5      	moveb %d0,%fp@(-11)
    18b4:	7c01           	moveq #1,%d6
    18b6:	206e ffe6      	moveal %fp@(-26),%a0
    18ba:	5488           	addql #2,%a0
    18bc:	2608           	movel %a0,%d3
    18be:	2043           	moveal %d3,%a0
    18c0:	b850           	cmpw %a0@,%d4
    18c2:	650a           	bcss 0x18ce
    18c4:	5483           	addql #2,%d3
    18c6:	5286           	addql #1,%d6
    18c8:	7011           	moveq #17,%d0
    18ca:	bc80           	cmpl %d0,%d6
    18cc:	6df0           	blts 0x18be
    18ce:	7000           	moveq #0,%d0
    18d0:	3004           	movew %d4,%d0
    18d2:	2d40 ffda      	movel %d0,%fp@(-38)
    18d6:	d080           	addl %d0,%d0
    18d8:	206a 0014      	moveal %a2@(20),%a0
    18dc:	d1c0           	addal %d0,%a0
    18de:	2608           	movel %a0,%d3
    18e0:	0c50 ffff      	cmpiw #-1,%a0@
    18e4:	661e           	bnes 0x1904
    18e6:	4227           	clrb %sp@-
    18e8:	7000           	moveq #0,%d0
    18ea:	102e fff5      	moveb %fp@(-11),%d0
    18ee:	7209           	moveq #9,%d1
    18f0:	e368           	lslw %d1,%d0
    18f2:	48c0           	extl %d0
    18f4:	2f00           	movel %d0,%sp@-
    18f6:	2f0b           	movel %a3,%sp@-
    18f8:	4e4f           	trap #15
    18fa:	a027           	0120047
    18fc:	4fef 000a      	lea %sp@(10),%sp
    1900:	6000 010e      	braw 0x1a10
    1904:	4aae 000c      	tstl %fp@(12)
    1908:	6654           	bnes 0x195e
    190a:	486e fffa      	pea %fp@(-6)
    190e:	7000           	moveq #0,%d0
    1910:	1007           	moveb %d7,%d0
    1912:	c0fc 000a      	muluw #10,%d0
    1916:	204a           	moveal %a2,%a0
    1918:	d1c0           	addal %d0,%a0
    191a:	2c08           	movel %a0,%d6
    191c:	4868 4018      	pea %a0@(16408)
    1920:	1f07           	moveb %d7,%sp@-
    1922:	2043           	moveal %d3,%a0
    1924:	3f10           	movew %a0@,%sp@-
    1926:	4eba 0dc4      	jsr %pc@(0x26ec)
    192a:	4a80           	tstl %d0
    192c:	4fef 000c      	lea %sp@(12),%sp
    1930:	670c           	beqs 0x193e
    1932:	206e 0010      	moveal %fp@(16),%a0
    1936:	4290           	clrl %a0@
    1938:	7009           	moveq #9,%d0
    193a:	6000 0386      	braw 0x1cc2
    193e:	2046           	moveal %d6,%a0
    1940:	7000           	moveq #0,%d0
    1942:	1028 4019      	moveb %a0@(16409),%d0
    1946:	0240 0030      	andiw #48,%d0
    194a:	0c40 0030      	cmpiw #48,%d0
    194e:	6700 00c0      	beqw 0x1a10
    1952:	206e 0010      	moveal %fp@(16),%a0
    1956:	9b90           	subl %d5,%a0@
    1958:	7013           	moveq #19,%d0
    195a:	6000 0366      	braw 0x1cc2
    195e:	7001           	moveq #1,%d0
    1960:	c0ae 000c      	andl %fp@(12),%d0
    1964:	674a           	beqs 0x19b0
    1966:	486e ffff      	pea %fp@(-1)
    196a:	486e fff6      	pea %fp@(-10)
    196e:	486e fffa      	pea %fp@(-6)
    1972:	486a 4018      	pea %a2@(16408)
    1976:	486a 0018      	pea %a2@(24)
    197a:	1f2e fff5      	moveb %fp@(-11),%sp@-
    197e:	1f07           	moveb %d7,%sp@-
    1980:	2043           	moveal %d3,%a0
    1982:	3f10           	movew %a0@,%sp@-
    1984:	4eba 1532      	jsr %pc@(0x2eb8)
    1988:	4a80           	tstl %d0
    198a:	4fef 001a      	lea %sp@(26),%sp
    198e:	6600 02ae      	bnew 0x1c3e
    1992:	7000           	moveq #0,%d0
    1994:	102e fff5      	moveb %fp@(-11),%d0
    1998:	7209           	moveq #9,%d1
    199a:	e368           	lslw %d1,%d0
    199c:	48c0           	extl %d0
    199e:	2f00           	movel %d0,%sp@-
    19a0:	486a 0018      	pea %a2@(24)
    19a4:	2f0b           	movel %a3,%sp@-
    19a6:	4e4f           	trap #15
    19a8:	a026           	0120046
    19aa:	4fef 000c      	lea %sp@(12),%sp
    19ae:	602a           	bras 0x19da
    19b0:	486e ffff      	pea %fp@(-1)
    19b4:	486e fff6      	pea %fp@(-10)
    19b8:	486e fffa      	pea %fp@(-6)
    19bc:	486a 4018      	pea %a2@(16408)
    19c0:	2f0b           	movel %a3,%sp@-
    19c2:	1f2e fff5      	moveb %fp@(-11),%sp@-
    19c6:	1f07           	moveb %d7,%sp@-
    19c8:	2043           	moveal %d3,%a0
    19ca:	3f10           	movew %a0@,%sp@-
    19cc:	4eba 14ea      	jsr %pc@(0x2eb8)
    19d0:	4a80           	tstl %d0
    19d2:	4fef 001a      	lea %sp@(26),%sp
    19d6:	6600 0266      	bnew 0x1c3e
    19da:	7000           	moveq #0,%d0
    19dc:	1007           	moveb %d7,%d0
    19de:	c0fc 000a      	muluw #10,%d0
    19e2:	204a           	moveal %a2,%a0
    19e4:	d1c0           	addal %d0,%a0
    19e6:	7000           	moveq #0,%d0
    19e8:	1028 4019      	moveb %a0@(16409),%d0
    19ec:	0240 0030      	andiw #48,%d0
    19f0:	0c40 0030      	cmpiw #48,%d0
    19f4:	671a           	beqs 0x1a10
    19f6:	4227           	clrb %sp@-
    19f8:	7000           	moveq #0,%d0
    19fa:	102e fff5      	moveb %fp@(-11),%d0
    19fe:	7209           	moveq #9,%d1
    1a00:	e368           	lslw %d1,%d0
    1a02:	48c0           	extl %d0
    1a04:	2f00           	movel %d0,%sp@-
    1a06:	2f0b           	movel %a3,%sp@-
    1a08:	4e4f           	trap #15
    1a0a:	a027           	0120047
    1a0c:	4fef 000a      	lea %sp@(10),%sp
    1a10:	7000           	moveq #0,%d0
    1a12:	102e fff5      	moveb %fp@(-11),%d0
    1a16:	9a80           	subl %d0,%d5
    1a18:	6706           	beqs 0x1a20
    1a1a:	4aae 000c      	tstl %fp@(12)
    1a1e:	6606           	bnes 0x1a26
    1a20:	7000           	moveq #0,%d0
    1a22:	6000 029e      	braw 0x1cc2
    1a26:	7000           	moveq #0,%d0
    1a28:	102a 437d      	moveb %a2@(17277),%d0
    1a2c:	3600           	movew %d0,%d3
    1a2e:	48c0           	extl %d0
    1a30:	2200           	movel %d0,%d1
    1a32:	2005           	movel %d5,%d0
    1a34:	4eba 31e0      	jsr %pc@(0x4c16)
    1a38:	2d40 fff0      	movel %d0,%fp@(-16)
    1a3c:	7000           	moveq #0,%d0
    1a3e:	102e fff5      	moveb %fp@(-11),%d0
    1a42:	7209           	moveq #9,%d1
    1a44:	e368           	lslw %d1,%d0
    1a46:	48c0           	extl %d0
    1a48:	d7c0           	addal %d0,%a3
    1a4a:	3003           	movew %d3,%d0
    1a4c:	e368           	lslw %d1,%d0
    1a4e:	48c0           	extl %d0
    1a50:	2d40 ffec      	movel %d0,%fp@(-20)
    1a54:	7e00           	moveq #0,%d7
    1a56:	4aae fff0      	tstl %fp@(-16)
    1a5a:	6f00 00fc      	blew 0x1b58
    1a5e:	2c2e ffda      	movel %fp@(-38),%d6
    1a62:	dc86           	addl %d6,%d6
    1a64:	5486           	addql #2,%d6
    1a66:	5244           	addqw #1,%d4
    1a68:	206a 0014      	moveal %a2@(20),%a0
    1a6c:	d1c6           	addal %d6,%a0
    1a6e:	2608           	movel %a0,%d3
    1a70:	0c50 ffff      	cmpiw #-1,%a0@
    1a74:	661e           	bnes 0x1a94
    1a76:	4227           	clrb %sp@-
    1a78:	7000           	moveq #0,%d0
    1a7a:	102a 437d      	moveb %a2@(17277),%d0
    1a7e:	7209           	moveq #9,%d1
    1a80:	e368           	lslw %d1,%d0
    1a82:	48c0           	extl %d0
    1a84:	2f00           	movel %d0,%sp@-
    1a86:	2f0b           	movel %a3,%sp@-
    1a88:	4e4f           	trap #15
    1a8a:	a027           	0120047
    1a8c:	4fef 000a      	lea %sp@(10),%sp
    1a90:	6000 00b0      	braw 0x1b42
    1a94:	7001           	moveq #1,%d0
    1a96:	220b           	movel %a3,%d1
    1a98:	c280           	andl %d0,%d1
    1a9a:	674e           	beqs 0x1aea
    1a9c:	486e ffff      	pea %fp@(-1)
    1aa0:	486e fff6      	pea %fp@(-10)
    1aa4:	486e fffa      	pea %fp@(-6)
    1aa8:	486a 4018      	pea %a2@(16408)
    1aac:	486a 0018      	pea %a2@(24)
    1ab0:	1f2a 437d      	moveb %a2@(17277),%sp@-
    1ab4:	4227           	clrb %sp@-
    1ab6:	2043           	moveal %d3,%a0
    1ab8:	3f10           	movew %a0@,%sp@-
    1aba:	4eba 13fc      	jsr %pc@(0x2eb8)
    1abe:	4a80           	tstl %d0
    1ac0:	4fef 001a      	lea %sp@(26),%sp
    1ac4:	6706           	beqs 0x1acc
    1ac6:	7e00           	moveq #0,%d7
    1ac8:	6000 0174      	braw 0x1c3e
    1acc:	7000           	moveq #0,%d0
    1ace:	102a 437d      	moveb %a2@(17277),%d0
    1ad2:	7209           	moveq #9,%d1
    1ad4:	e368           	lslw %d1,%d0
    1ad6:	48c0           	extl %d0
    1ad8:	2f00           	movel %d0,%sp@-
    1ada:	486a 0018      	pea %a2@(24)
    1ade:	2f0b           	movel %a3,%sp@-
    1ae0:	4e4f           	trap #15
    1ae2:	a026           	0120046
    1ae4:	4fef 000c      	lea %sp@(12),%sp
    1ae8:	602e           	bras 0x1b18
    1aea:	486e ffff      	pea %fp@(-1)
    1aee:	486e fff6      	pea %fp@(-10)
    1af2:	486e fffa      	pea %fp@(-6)
    1af6:	486a 4018      	pea %a2@(16408)
    1afa:	2f0b           	movel %a3,%sp@-
    1afc:	1f2a 437d      	moveb %a2@(17277),%sp@-
    1b00:	4227           	clrb %sp@-
    1b02:	2043           	moveal %d3,%a0
    1b04:	3f10           	movew %a0@,%sp@-
    1b06:	4eba 13b0      	jsr %pc@(0x2eb8)
    1b0a:	4a80           	tstl %d0
    1b0c:	4fef 001a      	lea %sp@(26),%sp
    1b10:	6706           	beqs 0x1b18
    1b12:	7e00           	moveq #0,%d7
    1b14:	6000 0128      	braw 0x1c3e
    1b18:	7000           	moveq #0,%d0
    1b1a:	102a 4019      	moveb %a2@(16409),%d0
    1b1e:	0240 0030      	andiw #48,%d0
    1b22:	0c40 0030      	cmpiw #48,%d0
    1b26:	671a           	beqs 0x1b42
    1b28:	4227           	clrb %sp@-
    1b2a:	7000           	moveq #0,%d0
    1b2c:	102a 437d      	moveb %a2@(17277),%d0
    1b30:	7209           	moveq #9,%d1
    1b32:	e368           	lslw %d1,%d0
    1b34:	48c0           	extl %d0
    1b36:	2f00           	movel %d0,%sp@-
    1b38:	2f0b           	movel %a3,%sp@-
    1b3a:	4e4f           	trap #15
    1b3c:	a027           	0120047
    1b3e:	4fef 000a      	lea %sp@(10),%sp
    1b42:	d7ee ffec      	addal %fp@(-20),%a3
    1b46:	7000           	moveq #0,%d0
    1b48:	102a 437d      	moveb %a2@(17277),%d0
    1b4c:	9a80           	subl %d0,%d5
    1b4e:	5287           	addql #1,%d7
    1b50:	beae fff0      	cmpl %fp@(-16),%d7
    1b54:	6d00 ff0e      	bltw 0x1a64
    1b58:	4a85           	tstl %d5
    1b5a:	6606           	bnes 0x1b62
    1b5c:	7000           	moveq #0,%d0
    1b5e:	6000 0162      	braw 0x1cc2
    1b62:	1605           	moveb %d5,%d3
    1b64:	5244           	addqw #1,%d4
    1b66:	7000           	moveq #0,%d0
    1b68:	3004           	movew %d4,%d0
    1b6a:	d080           	addl %d0,%d0
    1b6c:	206a 0014      	moveal %a2@(20),%a0
    1b70:	d1c0           	addal %d0,%a0
    1b72:	2c08           	movel %a0,%d6
    1b74:	0c50 ffff      	cmpiw #-1,%a0@
    1b78:	661c           	bnes 0x1b96
    1b7a:	4227           	clrb %sp@-
    1b7c:	7000           	moveq #0,%d0
    1b7e:	1003           	moveb %d3,%d0
    1b80:	7209           	moveq #9,%d1
    1b82:	e368           	lslw %d1,%d0
    1b84:	48c0           	extl %d0
    1b86:	2f00           	movel %d0,%sp@-
    1b88:	2f0b           	movel %a3,%sp@-
    1b8a:	4e4f           	trap #15
    1b8c:	a027           	0120047
    1b8e:	4fef 000a      	lea %sp@(10),%sp
    1b92:	6000 00a4      	braw 0x1c38
    1b96:	7001           	moveq #1,%d0
    1b98:	220b           	movel %a3,%d1
    1b9a:	c280           	andl %d0,%d1
    1b9c:	6748           	beqs 0x1be6
    1b9e:	486e ffff      	pea %fp@(-1)
    1ba2:	486e fff6      	pea %fp@(-10)
    1ba6:	486e fffa      	pea %fp@(-6)
    1baa:	486a 4018      	pea %a2@(16408)
    1bae:	486a 0018      	pea %a2@(24)
    1bb2:	1f03           	moveb %d3,%sp@-
    1bb4:	4227           	clrb %sp@-
    1bb6:	2046           	moveal %d6,%a0
    1bb8:	3f10           	movew %a0@,%sp@-
    1bba:	4eba 12fc      	jsr %pc@(0x2eb8)
    1bbe:	4a80           	tstl %d0
    1bc0:	4fef 001a      	lea %sp@(26),%sp
    1bc4:	6704           	beqs 0x1bca
    1bc6:	7e00           	moveq #0,%d7
    1bc8:	6074           	bras 0x1c3e
    1bca:	7000           	moveq #0,%d0
    1bcc:	1003           	moveb %d3,%d0
    1bce:	7209           	moveq #9,%d1
    1bd0:	e368           	lslw %d1,%d0
    1bd2:	48c0           	extl %d0
    1bd4:	2f00           	movel %d0,%sp@-
    1bd6:	486a 0018      	pea %a2@(24)
    1bda:	2f0b           	movel %a3,%sp@-
    1bdc:	4e4f           	trap #15
    1bde:	a026           	0120046
    1be0:	4fef 000c      	lea %sp@(12),%sp
    1be4:	602a           	bras 0x1c10
    1be6:	486e ffff      	pea %fp@(-1)
    1bea:	486e fff6      	pea %fp@(-10)
    1bee:	486e fffa      	pea %fp@(-6)
    1bf2:	486a 4018      	pea %a2@(16408)
    1bf6:	2f0b           	movel %a3,%sp@-
    1bf8:	1f03           	moveb %d3,%sp@-
    1bfa:	4227           	clrb %sp@-
    1bfc:	2046           	moveal %d6,%a0
    1bfe:	3f10           	movew %a0@,%sp@-
    1c00:	4eba 12b6      	jsr %pc@(0x2eb8)
    1c04:	4a80           	tstl %d0
    1c06:	4fef 001a      	lea %sp@(26),%sp
    1c0a:	6704           	beqs 0x1c10
    1c0c:	7e00           	moveq #0,%d7
    1c0e:	602e           	bras 0x1c3e
    1c10:	7000           	moveq #0,%d0
    1c12:	102a 4019      	moveb %a2@(16409),%d0
    1c16:	0240 0030      	andiw #48,%d0
    1c1a:	0c40 0030      	cmpiw #48,%d0
    1c1e:	6718           	beqs 0x1c38
    1c20:	4227           	clrb %sp@-
    1c22:	7000           	moveq #0,%d0
    1c24:	1003           	moveb %d3,%d0
    1c26:	7209           	moveq #9,%d1
    1c28:	e368           	lslw %d1,%d0
    1c2a:	48c0           	extl %d0
    1c2c:	2f00           	movel %d0,%sp@-
    1c2e:	2f0b           	movel %a3,%sp@-
    1c30:	4e4f           	trap #15
    1c32:	a027           	0120047
    1c34:	4fef 000a      	lea %sp@(10),%sp
    1c38:	7000           	moveq #0,%d0
    1c3a:	6000 0086      	braw 0x1cc2
    1c3e:	7002           	moveq #2,%d0
    1c40:	c0ae fff6      	andl %fp@(-10),%d0
    1c44:	661c           	bnes 0x1c62
    1c46:	7000           	moveq #0,%d0
    1c48:	1007           	moveb %d7,%d0
    1c4a:	122e ffff      	moveb %fp@(-1),%d1
    1c4e:	4881           	extw %d1
    1c50:	9240           	subw %d0,%d1
    1c52:	48c1           	extl %d1
    1c54:	2005           	movel %d5,%d0
    1c56:	9081           	subl %d1,%d0
    1c58:	206e 0010      	moveal %fp@(16),%a0
    1c5c:	9190           	subl %d0,%a0@
    1c5e:	7009           	moveq #9,%d0
    1c60:	6060           	bras 0x1cc2
    1c62:	102e ffff      	moveb %fp@(-1),%d0
    1c66:	4880           	extw %d0
    1c68:	c1fc 000a      	mulsw #10,%d0
    1c6c:	204a           	moveal %a2,%a0
    1c6e:	d1c0           	addal %d0,%a0
    1c70:	1028 4018      	moveb %a0@(16408),%d0
    1c74:	0240 0060      	andiw #96,%d0
    1c78:	672e           	beqs 0x1ca8
    1c7a:	0440 0020      	subiw #32,%d0
    1c7e:	6728           	beqs 0x1ca8
    1c80:	0440 0020      	subiw #32,%d0
    1c84:	6722           	beqs 0x1ca8
    1c86:	1d7c 00bf ffeb 	moveb #-65,%fp@(-21)
    1c8c:	486e ffeb      	pea %fp@(-21)
    1c90:	1f2e ffff      	moveb %fp@(-1),%sp@-
    1c94:	7000           	moveq #0,%d0
    1c96:	3004           	movew %d4,%d0
    1c98:	d080           	addl %d0,%d0
    1c9a:	206a 0014      	moveal %a2@(20),%a0
    1c9e:	3f30 0800      	movew %a0@(00000000,%d0:l),%sp@-
    1ca2:	4eba 0bc8      	jsr %pc@(0x286c)
    1ca6:	504f           	addqw #8,%sp
    1ca8:	7000           	moveq #0,%d0
    1caa:	1007           	moveb %d7,%d0
    1cac:	122e ffff      	moveb %fp@(-1),%d1
    1cb0:	4881           	extw %d1
    1cb2:	9240           	subw %d0,%d1
    1cb4:	48c1           	extl %d1
    1cb6:	2005           	movel %d5,%d0
    1cb8:	9081           	subl %d1,%d0
    1cba:	206e 0010      	moveal %fp@(16),%a0
    1cbe:	9190           	subl %d0,%a0@
    1cc0:	7020           	moveq #32,%d0
    1cc2:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    1cc6:	4e5e           	unlk %fp
    1cc8:	4e75           	rts

```

# job_format: 0x1cdc - 0x1ec4


```

    1cdc:	4e56 fff2      	linkw %fp,#-14
    1ce0:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    1ce4:	266e 000c      	moveal %fp@(12),%a3
    1ce8:	41fa e67a      	lea %pc@(0x364),%a0
    1cec:	2e08           	movel %a0,%d7
    1cee:	2d7c ffff f000 	movel #-4096,%fp@(-6)
    1cf4:	fffa
    1cf6:	0c2b 0001 437b 	cmpib #1,%a3@(17275)
    1cfc:	6606           	bnes 0x1d04
    1cfe:	7012           	moveq #18,%d0
    1d00:	6000 01bc      	braw 0x1ebe
    1d04:	4a2b 437c      	tstb %a3@(17276)
    1d08:	6706           	beqs 0x1d10
    1d0a:	7012           	moveq #18,%d0
    1d0c:	6000 01b0      	braw 0x1ebe
    1d10:	7a00           	moveq #0,%d5
    1d12:	1a2b 4214      	moveb %a3@(16916),%d5
    1d16:	48c5           	extl %d5
    1d18:	2805           	movel %d5,%d4
    1d1a:	7600           	moveq #0,%d3
    1d1c:	4a84           	tstl %d4
    1d1e:	6752           	beqs 0x1d72
    1d20:	244b           	moveal %a3,%a2
    1d22:	0c2a 0001 42d0 	cmpib #1,%a2@(17104)
    1d28:	663e           	bnes 0x1d68
    1d2a:	2003           	movel %d3,%d0
    1d2c:	720c           	moveq #12,%d1
    1d2e:	4eba 2eba      	jsr %pc@(0x4bea)
    1d32:	204b           	moveal %a3,%a0
    1d34:	d1c0           	addal %d0,%a0
    1d36:	2828 42cc      	movel %a0@(17100),%d4
    1d3a:	6604           	bnes 0x1d40
    1d3c:	7a00           	moveq #0,%d5
    1d3e:	6032           	bras 0x1d72
    1d40:	2a04           	movel %d4,%d5
    1d42:	e285           	asrl #1,%d5
    1d44:	486e fff2      	pea %fp@(-14)
    1d48:	42a7           	clrl %sp@-
    1d4a:	2f2e 0008      	movel %fp@(8),%sp@-
    1d4e:	1f3c 0001      	moveb #1,%sp@-
    1d52:	3f2b 4376      	movew %a3@(17270),%sp@-
    1d56:	4eba 0f5a      	jsr %pc@(0x2cb2)
    1d5a:	4a80           	tstl %d0
    1d5c:	4fef 0010      	lea %sp@(16),%sp
    1d60:	6710           	beqs 0x1d72
    1d62:	7010           	moveq #16,%d0
    1d64:	6000 0158      	braw 0x1ebe
    1d68:	508a           	addql #8,%a2
    1d6a:	588a           	addql #4,%a2
    1d6c:	5283           	addql #1,%d3
    1d6e:	b685           	cmpl %d5,%d3
    1d70:	65b0           	bcss 0x1d22
    1d72:	362b 4376      	movew %a3@(17270),%d3
    1d76:	0c43 ffff      	cmpiw #-1,%d3
    1d7a:	6606           	bnes 0x1d82
    1d7c:	7010           	moveq #16,%d0
    1d7e:	6000 013e      	braw 0x1ebe
    1d82:	3c2b 4378      	movew %a3@(17272),%d6
    1d86:	0c46 ffff      	cmpiw #-1,%d6
    1d8a:	660a           	bnes 0x1d96
    1d8c:	3003           	movew %d3,%d0
    1d8e:	5240           	addqw #1,%d0
    1d90:	7800           	moveq #0,%d4
    1d92:	3800           	movew %d0,%d4
    1d94:	6008           	bras 0x1d9e
    1d96:	3006           	movew %d6,%d0
    1d98:	5240           	addqw #1,%d0
    1d9a:	7800           	moveq #0,%d4
    1d9c:	3800           	movew %d0,%d4
    1d9e:	177c 00f8 4018 	moveb #-8,%a3@(16408)
    1da4:	177c 00ff 4019 	moveb #-1,%a3@(16409)
    1daa:	177c 00ff 401c 	moveb #-1,%a3@(16412)
    1db0:	177c 00ff 401d 	moveb #-1,%a3@(16413)
    1db6:	177c 00ff 401e 	moveb #-1,%a3@(16414)
    1dbc:	177c 00ff 401f 	moveb #-1,%a3@(16415)
    1dc2:	177c 00ff 4020 	moveb #-1,%a3@(16416)
    1dc8:	7c00           	moveq #0,%d6
    1dca:	6000 00d0      	braw 0x1e9c
    1dce:	2004           	movel %d4,%d0
    1dd0:	0280 0000 01ff 	andil #511,%d0
    1dd6:	660e           	bnes 0x1de6
    1dd8:	2004           	movel %d4,%d0
    1dda:	7209           	moveq #9,%d1
    1ddc:	e2a0           	asrl %d1,%d0
    1dde:	d080           	addl %d0,%d0
    1de0:	2047           	moveal %d7,%a0
    1de2:	3c30 0800      	movew %a0@(00000000,%d0:l),%d6
    1de6:	7600           	moveq #0,%d3
    1de8:	4a85           	tstl %d5
    1dea:	671e           	beqs 0x1e0a
    1dec:	246e 0008      	moveal %fp@(8),%a2
    1df0:	7000           	moveq #0,%d0
    1df2:	102a 0001      	moveb %a2@(1),%d0
    1df6:	7200           	moveq #0,%d1
    1df8:	1212           	moveb %a2@,%d1
    1dfa:	e149           	lslw #8,%d1
    1dfc:	d240           	addw %d0,%d1
    1dfe:	b841           	cmpw %d1,%d4
    1e00:	6708           	beqs 0x1e0a
    1e02:	548a           	addql #2,%a2
    1e04:	5283           	addql #1,%d3
    1e06:	b685           	cmpl %d5,%d3
    1e08:	65e6           	bcss 0x1df0
    1e0a:	b685           	cmpl %d5,%d3
    1e0c:	6500 008c      	bcsw 0x1e9a
    1e10:	486e fff6      	pea %fp@(-10)
    1e14:	3f04           	movew %d4,%sp@-
    1e16:	4eba 1498      	jsr %pc@(0x32b0)
    1e1a:	4a80           	tstl %d0
    1e1c:	5c4f           	addqw #6,%sp
    1e1e:	672a           	beqs 0x1e4a
    1e20:	206e fffa      	moveal %fp@(-6),%a0
    1e24:	1028 041c      	moveb %a0@(1052),%d0
    1e28:	0240 0008      	andiw #8,%d0
    1e2c:	6706           	beqs 0x1e34
    1e2e:	7054           	moveq #84,%d0
    1e30:	6000 008c      	braw 0x1ebe
    1e34:	1d7c 007f ffff 	moveb #127,%fp@(-1)
    1e3a:	486e ffff      	pea %fp@(-1)
    1e3e:	4227           	clrb %sp@-
    1e40:	3f04           	movew %d4,%sp@-
    1e42:	4eba 0a28      	jsr %pc@(0x286c)
    1e46:	504f           	addqw #8,%sp
    1e48:	6050           	bras 0x1e9a
    1e4a:	2004           	movel %d4,%d0
    1e4c:	7209           	moveq #9,%d1
    1e4e:	e2a0           	asrl %d1,%d0
    1e50:	d080           	addl %d0,%d0
    1e52:	2047           	moveal %d7,%a0
    1e54:	bc70 0802      	cmpw %a0@(00000002,%d0:l),%d6
    1e58:	6440           	bccs 0x1e9a
    1e5a:	3746 401a      	movew %d6,%a3@(16410)
    1e5e:	486b 4018      	pea %a3@(16408)
    1e62:	4227           	clrb %sp@-
    1e64:	3f04           	movew %d4,%sp@-
    1e66:	4eba 0732      	jsr %pc@(0x259a)
    1e6a:	4a80           	tstl %d0
    1e6c:	504f           	addqw #8,%sp
    1e6e:	6728           	beqs 0x1e98
    1e70:	206e fffa      	moveal %fp@(-6),%a0
    1e74:	1028 041c      	moveb %a0@(1052),%d0
    1e78:	0240 0008      	andiw #8,%d0
    1e7c:	6704           	beqs 0x1e82
    1e7e:	7054           	moveq #84,%d0
    1e80:	603c           	bras 0x1ebe
    1e82:	1d7c 007f ffff 	moveb #127,%fp@(-1)
    1e88:	486e ffff      	pea %fp@(-1)
    1e8c:	4227           	clrb %sp@-
    1e8e:	3f04           	movew %d4,%sp@-
    1e90:	4eba 09da      	jsr %pc@(0x286c)
    1e94:	504f           	addqw #8,%sp
    1e96:	6002           	bras 0x1e9a
    1e98:	5246           	addqw #1,%d6
    1e9a:	5284           	addql #1,%d4
    1e9c:	7000           	moveq #0,%d0
    1e9e:	302b 42fc      	movew %a3@(17148),%d0
    1ea2:	b880           	cmpl %d0,%d4
    1ea4:	6d00 ff28      	bltw 0x1dce
    1ea8:	4e4f           	trap #15
    1eaa:	a130           	0120460
    1eac:	2f0b           	movel %a3,%sp@-
    1eae:	4eba e5e4      	jsr %pc@(0x494)
    1eb2:	2600           	movel %d0,%d3
    1eb4:	584f           	addqw #4,%sp
    1eb6:	6704           	beqs 0x1ebc
    1eb8:	2003           	movel %d3,%d0
    1eba:	6002           	bras 0x1ebe
    1ebc:	7000           	moveq #0,%d0
    1ebe:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    1ec2:	4e5e           	unlk %fp
    1ec4:	4e75           	rts

```

# write_protocol: 0x1f44 - 0x20d2


```

    1f44:	4e56 fffe      	linkw %fp,#-2
    1f48:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    1f4c:	1e2e 0008      	moveb %fp@(8),%d7
    1f50:	3a2e 000a      	movew %fp@(10),%d5
    1f54:	246e 000c      	moveal %fp@(12),%a2
    1f58:	266e 0010      	moveal %fp@(16),%a3
    1f5c:	3638 f110      	movew 0xfffffffffffff110,%d3
    1f60:	e24b           	lsrw #1,%d3
    1f62:	0243 0007      	andiw #7,%d3
    1f66:	223c 0002 0000 	movel #131072,%d1
    1f6c:	e7a9           	lsll %d3,%d1
    1f6e:	7600           	moveq #0,%d3
    1f70:	3638 f100      	movew 0xfffffffffffff100,%d3
    1f74:	740d           	moveq #13,%d2
    1f76:	e5ab           	lsll %d2,%d3
    1f78:	d681           	addl %d1,%d3
    1f7a:	2003           	movel %d3,%d0
    1f7c:	5680           	addql #3,%d0
    1f7e:	2040           	moveal %d0,%a0
    1f80:	0210 00e8      	andib #-24,%a0@
    1f84:	4a2e 0014      	tstb %fp@(20)
    1f88:	660a           	bnes 0x1f94
    1f8a:	2003           	movel %d3,%d0
    1f8c:	5680           	addql #3,%d0
    1f8e:	2040           	moveal %d0,%a0
    1f90:	0010 0004      	orib #4,%a0@
    1f94:	0c45 0002      	cmpiw #2,%d5
    1f98:	621a           	bhis 0x1fb4
    1f9a:	2003           	movel %d3,%d0
    1f9c:	5880           	addql #4,%d0
    1f9e:	2040           	moveal %d0,%a0
    1fa0:	3092           	movew %a2@,%a0@
    1fa2:	7000           	moveq #0,%d0
    1fa4:	1007           	moveb %d7,%d0
    1fa6:	720c           	moveq #12,%d1
    1fa8:	e368           	lslw %d1,%d0
    1faa:	d045           	addw %d5,%d0
    1fac:	2043           	moveal %d3,%a0
    1fae:	3080           	movew %d0,%a0@
    1fb0:	6000 009e      	braw 0x2050
    1fb4:	0c45 0008      	cmpiw #8,%d5
    1fb8:	6228           	bhis 0x1fe2
    1fba:	7800           	moveq #0,%d4
    1fbc:	7c00           	moveq #0,%d6
    1fbe:	3c05           	movew %d5,%d6
    1fc0:	600c           	bras 0x1fce
    1fc2:	2003           	movel %d3,%d0
    1fc4:	5880           	addql #4,%d0
    1fc6:	2040           	moveal %d0,%a0
    1fc8:	30b2 4800      	movew %a2@(00000000,%d4:l),%a0@
    1fcc:	5484           	addql #2,%d4
    1fce:	b886           	cmpl %d6,%d4
    1fd0:	6df0           	blts 0x1fc2
    1fd2:	7000           	moveq #0,%d0
    1fd4:	1007           	moveb %d7,%d0
    1fd6:	720c           	moveq #12,%d1
    1fd8:	e368           	lslw %d1,%d0
    1fda:	d045           	addw %d5,%d0
    1fdc:	2043           	moveal %d3,%a0
    1fde:	3080           	movew %d0,%a0@
    1fe0:	606e           	bras 0x2050
    1fe2:	7800           	moveq #0,%d4
    1fe4:	2003           	movel %d3,%d0
    1fe6:	5880           	addql #4,%d0
    1fe8:	2040           	moveal %d0,%a0
    1fea:	30b2 4800      	movew %a2@(00000000,%d4:l),%a0@
    1fee:	5484           	addql #2,%d4
    1ff0:	7008           	moveq #8,%d0
    1ff2:	b880           	cmpl %d0,%d4
    1ff4:	6dee           	blts 0x1fe4
    1ff6:	7000           	moveq #0,%d0
    1ff8:	1007           	moveb %d7,%d0
    1ffa:	720c           	moveq #12,%d1
    1ffc:	e368           	lslw %d1,%d0
    1ffe:	d045           	addw %d5,%d0
    2000:	2043           	moveal %d3,%a0
    2002:	3080           	movew %d0,%a0@
    2004:	0c45 0200      	cmpiw #512,%d5
    2008:	6614           	bnes 0x201e
    200a:	2003           	movel %d3,%d0
    200c:	5880           	addql #4,%d0
    200e:	2040           	moveal %d0,%a0
    2010:	4850           	pea %a0@
    2012:	486a 0008      	pea %a2@(8)
    2016:	4eba febc      	jsr %pc@(0x1ed4)
    201a:	504f           	addqw #8,%sp
    201c:	6032           	bras 0x2050
    201e:	0c45 000f      	cmpiw #15,%d5
    2022:	6614           	bnes 0x2038
    2024:	2003           	movel %d3,%d0
    2026:	5880           	addql #4,%d0
    2028:	2040           	moveal %d0,%a0
    202a:	4850           	pea %a0@
    202c:	486a 0008      	pea %a2@(8)
    2030:	4eba fed8      	jsr %pc@(0x1f0a)
    2034:	504f           	addqw #8,%sp
    2036:	6018           	bras 0x2050
    2038:	7808           	moveq #8,%d4
    203a:	7c00           	moveq #0,%d6
    203c:	3c05           	movew %d5,%d6
    203e:	600c           	bras 0x204c
    2040:	2003           	movel %d3,%d0
    2042:	5880           	addql #4,%d0
    2044:	2040           	moveal %d0,%a0
    2046:	30b2 4800      	movew %a2@(00000000,%d4:l),%a0@
    204a:	5484           	addql #2,%d4
    204c:	b886           	cmpl %d6,%d4
    204e:	6df0           	blts 0x2040
    2050:	0c2e 0001 0014 	cmpib #1,%fp@(20)
    2056:	663e           	bnes 0x2096
    2058:	4878 0514      	pea 0x514
    205c:	2f0b           	movel %a3,%sp@-
    205e:	4eba 01fe      	jsr %pc@(0x225e)
    2062:	4a00           	tstb %d0
    2064:	504f           	addqw #8,%sp
    2066:	670e           	beqs 0x2076
    2068:	486e ffff      	pea %fp@(-1)
    206c:	4eba 02ee      	jsr %pc@(0x235c)
    2070:	7001           	moveq #1,%d0
    2072:	584f           	addqw #4,%sp
    2074:	6056           	bras 0x20cc
    2076:	1013           	moveb %a3@,%d0
    2078:	0240 0001      	andiw #1,%d0
    207c:	6736           	beqs 0x20b4
    207e:	486e ffff      	pea %fp@(-1)
    2082:	4eba 02d8      	jsr %pc@(0x235c)
    2086:	4a00           	tstb %d0
    2088:	584f           	addqw #4,%sp
    208a:	6704           	beqs 0x2090
    208c:	7001           	moveq #1,%d0
    208e:	603c           	bras 0x20cc
    2090:	0013 0080      	orib #-128,%a3@
    2094:	601e           	bras 0x20b4
    2096:	4878 03e8      	pea 0x3e8
    209a:	2f0b           	movel %a3,%sp@-
    209c:	4eba 01c0      	jsr %pc@(0x225e)
    20a0:	4a00           	tstb %d0
    20a2:	504f           	addqw #8,%sp
    20a4:	670e           	beqs 0x20b4
    20a6:	486e ffff      	pea %fp@(-1)
    20aa:	4eba 02b0      	jsr %pc@(0x235c)
    20ae:	7001           	moveq #1,%d0
    20b0:	584f           	addqw #4,%sp
    20b2:	6018           	bras 0x20cc
    20b4:	1013           	moveb %a3@,%d0
    20b6:	0240 0080      	andiw #128,%d0
    20ba:	660e           	bnes 0x20ca
    20bc:	486e ffff      	pea %fp@(-1)
    20c0:	4eba 029a      	jsr %pc@(0x235c)
    20c4:	7001           	moveq #1,%d0
    20c6:	584f           	addqw #4,%sp
    20c8:	6002           	bras 0x20cc
    20ca:	7000           	moveq #0,%d0
    20cc:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    20d0:	4e5e           	unlk %fp
    20d2:	4e75           	rts

```

# read_protocol: 0x2158 - 0x224c


```

    2158:	4e56 fffe      	linkw %fp,#-2
    215c:	48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-
    2160:	3c2e 000a      	movew %fp@(10),%d6
    2164:	246e 000c      	moveal %fp@(12),%a2
    2168:	266e 0010      	moveal %fp@(16),%a3
    216c:	3838 f110      	movew 0xfffffffffffff110,%d4
    2170:	e24c           	lsrw #1,%d4
    2172:	0244 0007      	andiw #7,%d4
    2176:	223c 0002 0000 	movel #131072,%d1
    217c:	e9a9           	lsll %d4,%d1
    217e:	7800           	moveq #0,%d4
    2180:	3838 f100      	movew 0xfffffffffffff100,%d4
    2184:	740d           	moveq #13,%d2
    2186:	e5ac           	lsll %d2,%d4
    2188:	d881           	addl %d1,%d4
    218a:	2004           	movel %d4,%d0
    218c:	5680           	addql #3,%d0
    218e:	2040           	moveal %d0,%a0
    2190:	0210 00e8      	andib #-24,%a0@
    2194:	2004           	movel %d4,%d0
    2196:	5680           	addql #3,%d0
    2198:	2040           	moveal %d0,%a0
    219a:	0010 0004      	orib #4,%a0@
    219e:	7000           	moveq #0,%d0
    21a0:	102e 0008      	moveb %fp@(8),%d0
    21a4:	720c           	moveq #12,%d1
    21a6:	e368           	lslw %d1,%d0
    21a8:	d046           	addw %d6,%d0
    21aa:	2044           	moveal %d4,%a0
    21ac:	3080           	movew %d0,%a0@
    21ae:	7600           	moveq #0,%d3
    21b0:	2004           	movel %d4,%d0
    21b2:	5480           	addql #2,%d0
    21b4:	2040           	moveal %d0,%a0
    21b6:	1010           	moveb %a0@,%d0
    21b8:	0240 0040      	andiw #64,%d0
    21bc:	674a           	beqs 0x2208
    21be:	0c46 0200      	cmpiw #512,%d6
    21c2:	6612           	bnes 0x21d6
    21c4:	2004           	movel %d4,%d0
    21c6:	5880           	addql #4,%d0
    21c8:	2040           	moveal %d0,%a0
    21ca:	4850           	pea %a0@
    21cc:	2f0a           	movel %a2,%sp@-
    21ce:	4eba ff16      	jsr %pc@(0x20e6)
    21d2:	504f           	addqw #8,%sp
    21d4:	603a           	bras 0x2210
    21d6:	0c46 001f      	cmpiw #31,%d6
    21da:	6612           	bnes 0x21ee
    21dc:	2004           	movel %d4,%d0
    21de:	5880           	addql #4,%d0
    21e0:	2040           	moveal %d0,%a0
    21e2:	4850           	pea %a0@
    21e4:	2f0a           	movel %a2,%sp@-
    21e6:	4eba ff36      	jsr %pc@(0x211e)
    21ea:	504f           	addqw #8,%sp
    21ec:	6022           	bras 0x2210
    21ee:	7600           	moveq #0,%d3
    21f0:	7a00           	moveq #0,%d5
    21f2:	3a06           	movew %d6,%d5
    21f4:	600c           	bras 0x2202
    21f6:	2004           	movel %d4,%d0
    21f8:	5880           	addql #4,%d0
    21fa:	2040           	moveal %d0,%a0
    21fc:	3590 3800      	movew %a0@,%a2@(00000000,%d3:l)
    2200:	5483           	addql #2,%d3
    2202:	b685           	cmpl %d5,%d3
    2204:	6df0           	blts 0x21f6
    2206:	6008           	bras 0x2210
    2208:	5283           	addql #1,%d3
    220a:	7003           	moveq #3,%d0
    220c:	b680           	cmpl %d0,%d3
    220e:	6da0           	blts 0x21b0
    2210:	4878 03e8      	pea 0x3e8
    2214:	2f0b           	movel %a3,%sp@-
    2216:	4eba 0046      	jsr %pc@(0x225e)
    221a:	4a00           	tstb %d0
    221c:	504f           	addqw #8,%sp
    221e:	670e           	beqs 0x222e
    2220:	486e ffff      	pea %fp@(-1)
    2224:	4eba 0136      	jsr %pc@(0x235c)
    2228:	7001           	moveq #1,%d0
    222a:	584f           	addqw #4,%sp
    222c:	6018           	bras 0x2246
    222e:	1013           	moveb %a3@,%d0
    2230:	0240 0080      	andiw #128,%d0
    2234:	660e           	bnes 0x2244
    2236:	486e ffff      	pea %fp@(-1)
    223a:	4eba 0120      	jsr %pc@(0x235c)
    223e:	7001           	moveq #1,%d0
    2240:	584f           	addqw #4,%sp
    2242:	6002           	bras 0x2246
    2244:	7000           	moveq #0,%d0
    2246:	4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3
    224a:	4e5e           	unlk %fp
    224c:	4e75           	rts

```

# wait_int: 0x225e - 0x22e6


```

    225e:	4e56 0000      	linkw %fp,#0
    2262:	48e7 1e20      	moveml %d3-%d6/%a2,%sp@-
    2266:	246e 0008      	moveal %fp@(8),%a2
    226a:	2c2e 000c      	movel %fp@(12),%d6
    226e:	3838 f110      	movew 0xfffffffffffff110,%d4
    2272:	e24c           	lsrw #1,%d4
    2274:	0244 0007      	andiw #7,%d4
    2278:	223c 0002 0000 	movel #131072,%d1
    227e:	e9a9           	lsll %d4,%d1
    2280:	7800           	moveq #0,%d4
    2282:	3838 f100      	movew 0xfffffffffffff100,%d4
    2286:	740d           	moveq #13,%d2
    2288:	e5ac           	lsll %d2,%d4
    228a:	d881           	addl %d1,%d4
    228c:	4a86           	tstl %d6
    228e:	660c           	bnes 0x229c
    2290:	2004           	movel %d4,%d0
    2292:	5c80           	addql #6,%d0
    2294:	2040           	moveal %d0,%a0
    2296:	1490           	moveb %a0@,%a2@
    2298:	7000           	moveq #0,%d0
    229a:	6044           	bras 0x22e0
    229c:	7a00           	moveq #0,%d5
    229e:	7600           	moveq #0,%d3
    22a0:	2004           	movel %d4,%d0
    22a2:	5480           	addql #2,%d0
    22a4:	2040           	moveal %d0,%a0
    22a6:	1010           	moveb %a0@,%d0
    22a8:	0240 0080      	andiw #128,%d0
    22ac:	670c           	beqs 0x22ba
    22ae:	2004           	movel %d4,%d0
    22b0:	5c80           	addql #6,%d0
    22b2:	2040           	moveal %d0,%a0
    22b4:	1490           	moveb %a0@,%a2@
    22b6:	7000           	moveq #0,%d0
    22b8:	6026           	bras 0x22e0
    22ba:	4878 0064      	pea 0x64
    22be:	4e4f           	trap #15
    22c0:	a249 0643      	macw %a1u,%a1l,>>
    22c4:	0064 5245      	oriw #21061,%a4@-
    22c8:	3043           	moveaw %d3,%a0
    22ca:	b1c6           	cmpal %d6,%a0
    22cc:	584f           	addqw #4,%sp
    22ce:	65d0           	bcss 0x22a0
    22d0:	2004           	movel %d4,%d0
    22d2:	5c80           	addql #6,%d0
    22d4:	2040           	moveal %d0,%a0
    22d6:	1010           	moveb %a0@,%d0
    22d8:	0000 0001      	orib #1,%d0
    22dc:	1480           	moveb %d0,%a2@
    22de:	7000           	moveq #0,%d0
    22e0:	4cdf 0478      	moveml %sp@+,%d3-%d6/%a2
    22e4:	4e5e           	unlk %fp
    22e6:	4e75           	rts

```

# init_mshif: 0x22f4 - 0x234c


```

    22f4:	4e56 0000      	linkw %fp,#0
    22f8:	2f03           	movel %d3,%sp@-
    22fa:	3638 f110      	movew 0xfffffffffffff110,%d3
    22fe:	e24b           	lsrw #1,%d3
    2300:	0243 0007      	andiw #7,%d3
    2304:	223c 0002 0000 	movel #131072,%d1
    230a:	e7a9           	lsll %d3,%d1
    230c:	7600           	moveq #0,%d3
    230e:	3638 f100      	movew 0xfffffffffffff100,%d3
    2312:	740d           	moveq #13,%d2
    2314:	e5ab           	lsll %d2,%d3
    2316:	d681           	addl %d1,%d3
    2318:	7009           	moveq #9,%d0
    231a:	d083           	addl %d3,%d0
    231c:	2040           	moveal %d0,%a0
    231e:	4210           	clrb %a0@
    2320:	2003           	movel %d3,%d0
    2322:	5080           	addql #8,%d0
    2324:	2040           	moveal %d0,%a0
    2326:	0210 00f1      	andib #-15,%a0@
    232a:	7009           	moveq #9,%d0
    232c:	d083           	addl %d3,%d0
    232e:	2040           	moveal %d0,%a0
    2330:	10bc 000e      	moveb #14,%a0@
    2334:	2f2e 0008      	movel %fp@(8),%sp@-
    2338:	4eba 0022      	jsr %pc@(0x235c)
    233c:	2003           	movel %d3,%d0
    233e:	5680           	addql #3,%d0
    2340:	2040           	moveal %d0,%a0
    2342:	10bc 0004      	moveb #4,%a0@
    2346:	584f           	addqw #4,%sp
    2348:	261f           	movel %sp@+,%d3
    234a:	4e5e           	unlk %fp
    234c:	4e75           	rts

```

# reset_mshif: 0x235c - 0x23fe


```

    235c:	4e56 0000      	linkw %fp,#0
    2360:	2f0a           	movel %a2,%sp@-
    2362:	2f03           	movel %d3,%sp@-
    2364:	246e 0008      	moveal %fp@(8),%a2
    2368:	3638 f110      	movew 0xfffffffffffff110,%d3
    236c:	e24b           	lsrw #1,%d3
    236e:	0243 0007      	andiw #7,%d3
    2372:	223c 0002 0000 	movel #131072,%d1
    2378:	e7a9           	lsll %d3,%d1
    237a:	7600           	moveq #0,%d3
    237c:	3638 f100      	movew 0xfffffffffffff100,%d3
    2380:	740d           	moveq #13,%d2
    2382:	e5ab           	lsll %d2,%d3
    2384:	d681           	addl %d1,%d3
    2386:	42a7           	clrl %sp@-
    2388:	4eba 0084      	jsr %pc@(0x240e)
    238c:	2003           	movel %d3,%d0
    238e:	5e80           	addql #7,%d0
    2390:	2040           	moveal %d0,%a0
    2392:	0010 0080      	orib #-128,%a0@
    2396:	2003           	movel %d3,%d0
    2398:	5680           	addql #3,%d0
    239a:	2040           	moveal %d0,%a0
    239c:	0010 0080      	orib #-128,%a0@
    23a0:	2003           	movel %d3,%d0
    23a2:	5680           	addql #3,%d0
    23a4:	2040           	moveal %d0,%a0
    23a6:	0210 007f      	andib #127,%a0@
    23aa:	4878 0514      	pea 0x514
    23ae:	2f0a           	movel %a2,%sp@-
    23b0:	4eba feac      	jsr %pc@(0x225e)
    23b4:	4a00           	tstb %d0
    23b6:	4fef 000c      	lea %sp@(12),%sp
    23ba:	6704           	beqs 0x23c0
    23bc:	7001           	moveq #1,%d0
    23be:	6038           	bras 0x23f8
    23c0:	2003           	movel %d3,%d0
    23c2:	5480           	addql #2,%d0
    23c4:	2040           	moveal %d0,%a0
    23c6:	1010           	moveb %a0@,%d0
    23c8:	0240 0040      	andiw #64,%d0
    23cc:	6704           	beqs 0x23d2
    23ce:	0012 0020      	orib #32,%a2@
    23d2:	7000           	moveq #0,%d0
    23d4:	1012           	moveb %a2@,%d0
    23d6:	3600           	movew %d0,%d3
    23d8:	0240 0001      	andiw #1,%d0
    23dc:	6704           	beqs 0x23e2
    23de:	7001           	moveq #1,%d0
    23e0:	6016           	bras 0x23f8
    23e2:	3003           	movew %d3,%d0
    23e4:	0240 0080      	andiw #128,%d0
    23e8:	6708           	beqs 0x23f2
    23ea:	3003           	movew %d3,%d0
    23ec:	0240 0020      	andiw #32,%d0
    23f0:	6704           	beqs 0x23f6
    23f2:	7001           	moveq #1,%d0
    23f4:	6002           	bras 0x23f8
    23f6:	7000           	moveq #0,%d0
    23f8:	261f           	movel %sp@+,%d3
    23fa:	245f           	moveal %sp@+,%a2
    23fc:	4e5e           	unlk %fp
    23fe:	4e75           	rts

```

# power_save_mode: 0x240e - 0x2452


```

    240e:	4e56 0000      	linkw %fp,#0
    2412:	2f03           	movel %d3,%sp@-
    2414:	3638 f110      	movew 0xfffffffffffff110,%d3
    2418:	e24b           	lsrw #1,%d3
    241a:	0243 0007      	andiw #7,%d3
    241e:	223c 0002 0000 	movel #131072,%d1
    2424:	e7a9           	lsll %d3,%d1
    2426:	7600           	moveq #0,%d3
    2428:	3638 f100      	movew 0xfffffffffffff100,%d3
    242c:	740d           	moveq #13,%d2
    242e:	e5ab           	lsll %d2,%d3
    2430:	d681           	addl %d1,%d3
    2432:	4aae 0008      	tstl %fp@(8)
    2436:	670c           	beqs 0x2444
    2438:	2003           	movel %d3,%d0
    243a:	5680           	addql #3,%d0
    243c:	2040           	moveal %d0,%a0
    243e:	0010 0040      	orib #64,%a0@
    2442:	600a           	bras 0x244e
    2444:	2003           	movel %d3,%d0
    2446:	5680           	addql #3,%d0
    2448:	2040           	moveal %d0,%a0
    244a:	0210 00bf      	andib #-65,%a0@
    244e:	261f           	movel %sp@+,%d3
    2450:	4e5e           	unlk %fp
    2452:	4e75           	rts

```

# card_power: 0x2466 - 0x24e2


```

    2466:	4e56 0000      	linkw %fp,#0
    246a:	2f03           	movel %d3,%sp@-
    246c:	3638 f110      	movew 0xfffffffffffff110,%d3
    2470:	e24b           	lsrw #1,%d3
    2472:	0243 0007      	andiw #7,%d3
    2476:	223c 0002 0000 	movel #131072,%d1
    247c:	e7a9           	lsll %d3,%d1
    247e:	7600           	moveq #0,%d3
    2480:	3638 f100      	movew 0xfffffffffffff100,%d3
    2484:	740d           	moveq #13,%d2
    2486:	e5ab           	lsll %d2,%d3
    2488:	d681           	addl %d1,%d3
    248a:	4a2e 0008      	tstb %fp@(8)
    248e:	661c           	bnes 0x24ac
    2490:	2003           	movel %d3,%d0
    2492:	5680           	addql #3,%d0
    2494:	2040           	moveal %d0,%a0
    2496:	0210 00df      	andib #-33,%a0@
    249a:	2003           	movel %d3,%d0
    249c:	5080           	addql #8,%d0
    249e:	2040           	moveal %d0,%a0
    24a0:	0210 00fd      	andib #-3,%a0@
    24a4:	0238 00df f409 	andib #-33,0xfffffffffffff409
    24aa:	6032           	bras 0x24de
    24ac:	2003           	movel %d3,%d0
    24ae:	5080           	addql #8,%d0
    24b0:	2040           	moveal %d0,%a0
    24b2:	1010           	moveb %a0@,%d0
    24b4:	0240 0002      	andiw #2,%d0
    24b8:	6624           	bnes 0x24de
    24ba:	2003           	movel %d3,%d0
    24bc:	5080           	addql #8,%d0
    24be:	2040           	moveal %d0,%a0
    24c0:	0010 0002      	orib #2,%a0@
    24c4:	0038 0020 f409 	orib #32,0xfffffffffffff409
    24ca:	2003           	movel %d3,%d0
    24cc:	5680           	addql #3,%d0
    24ce:	2040           	moveal %d0,%a0
    24d0:	0010 0020      	orib #32,%a0@
    24d4:	4878 0bb8      	pea 0xbb8
    24d8:	4e4f           	trap #15
    24da:	a249 584f      	macl %a1,%a1
    24de:	261f           	movel %sp@+,%d3
    24e0:	4e5e           	unlk %fp
    24e2:	4e75           	rts

```

# access_led: 0x24f2 - 0x2536


```

    24f2:	4e56 0000      	linkw %fp,#0
    24f6:	2f03           	movel %d3,%sp@-
    24f8:	3638 f110      	movew 0xfffffffffffff110,%d3
    24fc:	e24b           	lsrw #1,%d3
    24fe:	0243 0007      	andiw #7,%d3
    2502:	223c 0002 0000 	movel #131072,%d1
    2508:	e7a9           	lsll %d3,%d1
    250a:	7600           	moveq #0,%d3
    250c:	3638 f100      	movew 0xfffffffffffff100,%d3
    2510:	740d           	moveq #13,%d2
    2512:	e5ab           	lsll %d2,%d3
    2514:	d681           	addl %d1,%d3
    2516:	4a2e 0008      	tstb %fp@(8)
    251a:	660c           	bnes 0x2528
    251c:	2003           	movel %d3,%d0
    251e:	5080           	addql #8,%d0
    2520:	2040           	moveal %d0,%a0
    2522:	0210 00fb      	andib #-5,%a0@
    2526:	600a           	bras 0x2532
    2528:	2003           	movel %d3,%d0
    252a:	5080           	addql #8,%d0
    252c:	2040           	moveal %d0,%a0
    252e:	0010 0004      	orib #4,%a0@
    2532:	261f           	movel %sp@+,%d3
    2534:	4e5e           	unlk %fp
    2536:	4e75           	rts

```

# warning_led: 0x2546 - 0x258a


```

    2546:	4e56 0000      	linkw %fp,#0
    254a:	2f03           	movel %d3,%sp@-
    254c:	3638 f110      	movew 0xfffffffffffff110,%d3
    2550:	e24b           	lsrw #1,%d3
    2552:	0243 0007      	andiw #7,%d3
    2556:	223c 0002 0000 	movel #131072,%d1
    255c:	e7a9           	lsll %d3,%d1
    255e:	7600           	moveq #0,%d3
    2560:	3638 f100      	movew 0xfffffffffffff100,%d3
    2564:	740d           	moveq #13,%d2
    2566:	e5ab           	lsll %d2,%d3
    2568:	d681           	addl %d1,%d3
    256a:	4a2e 0008      	tstb %fp@(8)
    256e:	660c           	bnes 0x257c
    2570:	2003           	movel %d3,%d0
    2572:	5080           	addql #8,%d0
    2574:	2040           	moveal %d0,%a0
    2576:	0210 00f7      	andib #-9,%a0@
    257a:	600a           	bras 0x2586
    257c:	2003           	movel %d3,%d0
    257e:	5080           	addql #8,%d0
    2580:	2040           	moveal %d0,%a0
    2582:	0010 0008      	orib #8,%a0@
    2586:	261f           	movel %sp@+,%d3
    2588:	4e5e           	unlk %fp
    258a:	4e75           	rts

```

# WriteRedundantData: 0x259a - 0x26d4


```

    259a:	4e56 ffde      	linkw %fp,#-34
    259e:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    25a2:	45ee ffde      	lea %fp@(-34),%a2
    25a6:	14bc 0080      	moveb #-128,%a2@
    25aa:	422a 0001      	clrb %a2@(1)
    25ae:	302e 0008      	movew %fp@(8),%d0
    25b2:	e048           	lsrw #8,%d0
    25b4:	1540 0002      	moveb %d0,%a2@(2)
    25b8:	156e 0009 0003 	moveb %fp@(9),%a2@(3)
    25be:	157c 0040 0004 	moveb #64,%a2@(4)
    25c4:	156e 000a 0005 	moveb %fp@(10),%a2@(5)
    25ca:	7600           	moveq #0,%d3
    25cc:	266e 000c      	moveal %fp@(12),%a3
    25d0:	1593 3806      	moveb %a3@,%a2@(00000006,%d3:l)
    25d4:	528b           	addql #1,%a3
    25d6:	5283           	addql #1,%d3
    25d8:	700a           	moveq #10,%d0
    25da:	b680           	cmpl %d0,%d3
    25dc:	65f2           	bcss 0x25d0
    25de:	486e ffff      	pea %fp@(-1)
    25e2:	4227           	clrb %sp@-
    25e4:	2f0a           	movel %a2,%sp@-
    25e6:	4eba dae6      	jsr %pc@(0xce)
    25ea:	7200           	moveq #0,%d1
    25ec:	3200           	movew %d0,%d1
    25ee:	2601           	movel %d1,%d3
    25f0:	4fef 000a      	lea %sp@(10),%sp
    25f4:	6710           	beqs 0x2606
    25f6:	2803           	movel %d3,%d4
    25f8:	2f04           	movel %d4,%sp@-
    25fa:	4eba 0e42      	jsr %pc@(0x343e)
    25fe:	70ff           	moveq #-1,%d0
    2600:	584f           	addqw #4,%sp
    2602:	6000 00ca      	braw 0x26ce
    2606:	486e ffff      	pea %fp@(-1)
    260a:	4227           	clrb %sp@-
    260c:	1f3c 0055      	moveb #85,%sp@-
    2610:	4eba db6e      	jsr %pc@(0x180)
    2614:	7200           	moveq #0,%d1
    2616:	3200           	movew %d0,%d1
    2618:	2601           	movel %d1,%d3
    261a:	504f           	addqw #8,%sp
    261c:	6712           	beqs 0x2630
    261e:	7810           	moveq #16,%d4
    2620:	8883           	orl %d3,%d4
    2622:	2f04           	movel %d4,%sp@-
    2624:	4eba 0e18      	jsr %pc@(0x343e)
    2628:	70ff           	moveq #-1,%d0
    262a:	584f           	addqw #4,%sp
    262c:	6000 00a0      	braw 0x26ce
    2630:	486e ffff      	pea %fp@(-1)
    2634:	1f3c 0002      	moveb #2,%sp@-
    2638:	4eba dbbc      	jsr %pc@(0x1f6)
    263c:	7200           	moveq #0,%d1
    263e:	3200           	movew %d0,%d1
    2640:	2601           	movel %d1,%d3
    2642:	7001           	moveq #1,%d0
    2644:	b680           	cmpl %d0,%d3
    2646:	5c4f           	addqw #6,%sp
    2648:	6714           	beqs 0x265e
    264a:	4a83           	tstl %d3
    264c:	660c           	bnes 0x265a
    264e:	7810           	moveq #16,%d4
    2650:	8883           	orl %d3,%d4
    2652:	2f04           	movel %d4,%sp@-
    2654:	4eba 0de8      	jsr %pc@(0x343e)
    2658:	584f           	addqw #4,%sp
    265a:	70ff           	moveq #-1,%d0
    265c:	6070           	bras 0x26ce
    265e:	486e ffff      	pea %fp@(-1)
    2662:	486e fffe      	pea %fp@(-2)
    2666:	4eba da08      	jsr %pc@(0x70)
    266a:	7200           	moveq #0,%d1
    266c:	3200           	movew %d0,%d1
    266e:	2601           	movel %d1,%d3
    2670:	504f           	addqw #8,%sp
    2672:	6710           	beqs 0x2684
    2674:	7810           	moveq #16,%d4
    2676:	8883           	orl %d3,%d4
    2678:	2f04           	movel %d4,%sp@-
    267a:	4eba 0dc2      	jsr %pc@(0x343e)
    267e:	70ff           	moveq #-1,%d0
    2680:	584f           	addqw #4,%sp
    2682:	604a           	bras 0x26ce
    2684:	7000           	moveq #0,%d0
    2686:	102e fffe      	moveb %fp@(-2),%d0
    268a:	3600           	movew %d0,%d3
    268c:	0240 0080      	andiw #128,%d0
    2690:	671a           	beqs 0x26ac
    2692:	3003           	movew %d3,%d0
    2694:	0240 0040      	andiw #64,%d0
    2698:	6604           	bnes 0x269e
    269a:	7000           	moveq #0,%d0
    269c:	6030           	bras 0x26ce
    269e:	7601           	moveq #1,%d3
    26a0:	2f03           	movel %d3,%sp@-
    26a2:	4eba 0d9a      	jsr %pc@(0x343e)
    26a6:	70ff           	moveq #-1,%d0
    26a8:	584f           	addqw #4,%sp
    26aa:	6022           	bras 0x26ce
    26ac:	3003           	movew %d3,%d0
    26ae:	0240 0001      	andiw #1,%d0
    26b2:	670e           	beqs 0x26c2
    26b4:	7608           	moveq #8,%d3
    26b6:	2f03           	movel %d3,%sp@-
    26b8:	4eba 0d84      	jsr %pc@(0x343e)
    26bc:	70ff           	moveq #-1,%d0
    26be:	584f           	addqw #4,%sp
    26c0:	600c           	bras 0x26ce
    26c2:	7610           	moveq #16,%d3
    26c4:	2f03           	movel %d3,%sp@-
    26c6:	4eba 0d76      	jsr %pc@(0x343e)
    26ca:	70ff           	moveq #-1,%d0
    26cc:	584f           	addqw #4,%sp
    26ce:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
    26d2:	4e5e           	unlk %fp
    26d4:	4e75           	rts

```

# ReadRedundantData: 0x26ec - 0x2856


```

    26ec:	4e56 ffbe      	linkw %fp,#-66
    26f0:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    26f4:	282e 0010      	movel %fp@(16),%d4
    26f8:	47ee ffde      	lea %fp@(-34),%a3
    26fc:	45ee ffbe      	lea %fp@(-66),%a2
    2700:	2044           	moveal %d4,%a0
    2702:	4290           	clrl %a0@
    2704:	14bc 0080      	moveb #-128,%a2@
    2708:	422a 0001      	clrb %a2@(1)
    270c:	302e 0008      	movew %fp@(8),%d0
    2710:	e048           	lsrw #8,%d0
    2712:	1540 0002      	moveb %d0,%a2@(2)
    2716:	156e 0009 0003 	moveb %fp@(9),%a2@(3)
    271c:	157c 0040 0004 	moveb #64,%a2@(4)
    2722:	156e 000a 0005 	moveb %fp@(10),%a2@(5)
    2728:	486e ffff      	pea %fp@(-1)
    272c:	4227           	clrb %sp@-
    272e:	2f0a           	movel %a2,%sp@-
    2730:	4eba d99c      	jsr %pc@(0xce)
    2734:	7200           	moveq #0,%d1
    2736:	3200           	movew %d0,%d1
    2738:	2601           	movel %d1,%d3
    273a:	4fef 000a      	lea %sp@(10),%sp
    273e:	6710           	beqs 0x2750
    2740:	2803           	movel %d3,%d4
    2742:	2f04           	movel %d4,%sp@-
    2744:	4eba 0cf8      	jsr %pc@(0x343e)
    2748:	70ff           	moveq #-1,%d0
    274a:	584f           	addqw #4,%sp
    274c:	6000 0102      	braw 0x2850
    2750:	486e ffff      	pea %fp@(-1)
    2754:	4227           	clrb %sp@-
    2756:	1f3c 00aa      	moveb #-86,%sp@-
    275a:	4eba da24      	jsr %pc@(0x180)
    275e:	7200           	moveq #0,%d1
    2760:	3200           	movew %d0,%d1
    2762:	2601           	movel %d1,%d3
    2764:	504f           	addqw #8,%sp
    2766:	6712           	beqs 0x277a
    2768:	7810           	moveq #16,%d4
    276a:	8883           	orl %d3,%d4
    276c:	2f04           	movel %d4,%sp@-
    276e:	4eba 0cce      	jsr %pc@(0x343e)
    2772:	70ff           	moveq #-1,%d0
    2774:	584f           	addqw #4,%sp
    2776:	6000 00d8      	braw 0x2850
    277a:	486e ffff      	pea %fp@(-1)
    277e:	1f3c 0001      	moveb #1,%sp@-
    2782:	4eba da72      	jsr %pc@(0x1f6)
    2786:	7200           	moveq #0,%d1
    2788:	3200           	movew %d0,%d1
    278a:	2601           	movel %d1,%d3
    278c:	7001           	moveq #1,%d0
    278e:	b680           	cmpl %d0,%d3
    2790:	5c4f           	addqw #6,%sp
    2792:	6716           	beqs 0x27aa
    2794:	4a83           	tstl %d3
    2796:	660c           	bnes 0x27a4
    2798:	7810           	moveq #16,%d4
    279a:	8883           	orl %d3,%d4
    279c:	2f04           	movel %d4,%sp@-
    279e:	4eba 0c9e      	jsr %pc@(0x343e)
    27a2:	584f           	addqw #4,%sp
    27a4:	70ff           	moveq #-1,%d0
    27a6:	6000 00a8      	braw 0x2850
    27aa:	486e ffff      	pea %fp@(-1)
    27ae:	2f0b           	movel %a3,%sp@-
    27b0:	4eba d978      	jsr %pc@(0x12a)
    27b4:	7200           	moveq #0,%d1
    27b6:	3200           	movew %d0,%d1
    27b8:	2601           	movel %d1,%d3
    27ba:	504f           	addqw #8,%sp
    27bc:	6714           	beqs 0x27d2
    27be:	7010           	moveq #16,%d0
    27c0:	8680           	orl %d0,%d3
    27c2:	2803           	movel %d3,%d4
    27c4:	2f04           	movel %d4,%sp@-
    27c6:	4eba 0c76      	jsr %pc@(0x343e)
    27ca:	70ff           	moveq #-1,%d0
    27cc:	584f           	addqw #4,%sp
    27ce:	6000 0080      	braw 0x2850
    27d2:	7000           	moveq #0,%d0
    27d4:	102b 0001      	moveb %a3@(1),%d0
    27d8:	3600           	movew %d0,%d3
    27da:	0240 0080      	andiw #128,%d0
    27de:	674e           	beqs 0x282e
    27e0:	7600           	moveq #0,%d3
    27e2:	246e 000c      	moveal %fp@(12),%a2
    27e6:	14b3 3816      	moveb %a3@(00000016,%d3:l),%a2@
    27ea:	528a           	addql #1,%a2
    27ec:	5283           	addql #1,%d3
    27ee:	700a           	moveq #10,%d0
    27f0:	b680           	cmpl %d0,%d3
    27f2:	65f2           	bcss 0x27e6
    27f4:	102b 0001      	moveb %a3@(1),%d0
    27f8:	0240 0040      	andiw #64,%d0
    27fc:	660a           	bnes 0x2808
    27fe:	2044           	moveal %d4,%a0
    2800:	7004           	moveq #4,%d0
    2802:	2080           	movel %d0,%a0@
    2804:	7000           	moveq #0,%d0
    2806:	6048           	bras 0x2850
    2808:	102b 0003      	moveb %a3@(3),%d0
    280c:	0240 0005      	andiw #5,%d0
    2810:	6712           	beqs 0x2824
    2812:	2044           	moveal %d4,%a0
    2814:	4290           	clrl %a0@
    2816:	7602           	moveq #2,%d3
    2818:	2f03           	movel %d3,%sp@-
    281a:	4eba 0c22      	jsr %pc@(0x343e)
    281e:	70ff           	moveq #-1,%d0
    2820:	584f           	addqw #4,%sp
    2822:	602c           	bras 0x2850
    2824:	2044           	moveal %d4,%a0
    2826:	7008           	moveq #8,%d0
    2828:	2080           	movel %d0,%a0@
    282a:	7000           	moveq #0,%d0
    282c:	6022           	bras 0x2850
    282e:	3003           	movew %d3,%d0
    2830:	0240 0001      	andiw #1,%d0
    2834:	670e           	beqs 0x2844
    2836:	7608           	moveq #8,%d3
    2838:	2f03           	movel %d3,%sp@-
    283a:	4eba 0c02      	jsr %pc@(0x343e)
    283e:	70ff           	moveq #-1,%d0
    2840:	584f           	addqw #4,%sp
    2842:	600c           	bras 0x2850
    2844:	7610           	moveq #16,%d3
    2846:	2f03           	movel %d3,%sp@-
    2848:	4eba 0bf4      	jsr %pc@(0x343e)
    284c:	70ff           	moveq #-1,%d0
    284e:	584f           	addqw #4,%sp
    2850:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
    2854:	4e5e           	unlk %fp
    2856:	4e75           	rts

```

# OverwriteRedundantData: 0x286c - 0x299c


```

    286c:	4e56 ffde      	linkw %fp,#-34
    2870:	48e7 1820      	moveml %d3-%d4/%a2,%sp@-
    2874:	45ee ffde      	lea %fp@(-34),%a2
    2878:	14bc 0080      	moveb #-128,%a2@
    287c:	422a 0001      	clrb %a2@(1)
    2880:	302e 0008      	movew %fp@(8),%d0
    2884:	e048           	lsrw #8,%d0
    2886:	1540 0002      	moveb %d0,%a2@(2)
    288a:	156e 0009 0003 	moveb %fp@(9),%a2@(3)
    2890:	157c 0080 0004 	moveb #-128,%a2@(4)
    2896:	156e 000a 0005 	moveb %fp@(10),%a2@(5)
    289c:	206e 000c      	moveal %fp@(12),%a0
    28a0:	1550 0006      	moveb %a0@,%a2@(6)
    28a4:	486e ffff      	pea %fp@(-1)
    28a8:	4227           	clrb %sp@-
    28aa:	2f0a           	movel %a2,%sp@-
    28ac:	4eba d820      	jsr %pc@(0xce)
    28b0:	7200           	moveq #0,%d1
    28b2:	3200           	movew %d0,%d1
    28b4:	2601           	movel %d1,%d3
    28b6:	4fef 000a      	lea %sp@(10),%sp
    28ba:	6710           	beqs 0x28cc
    28bc:	2803           	movel %d3,%d4
    28be:	2f04           	movel %d4,%sp@-
    28c0:	4eba 0b7c      	jsr %pc@(0x343e)
    28c4:	70ff           	moveq #-1,%d0
    28c6:	584f           	addqw #4,%sp
    28c8:	6000 00cc      	braw 0x2996
    28cc:	486e ffff      	pea %fp@(-1)
    28d0:	4227           	clrb %sp@-
    28d2:	1f3c 0055      	moveb #85,%sp@-
    28d6:	4eba d8a8      	jsr %pc@(0x180)
    28da:	7200           	moveq #0,%d1
    28dc:	3200           	movew %d0,%d1
    28de:	2601           	movel %d1,%d3
    28e0:	504f           	addqw #8,%sp
    28e2:	6712           	beqs 0x28f6
    28e4:	7810           	moveq #16,%d4
    28e6:	8883           	orl %d3,%d4
    28e8:	2f04           	movel %d4,%sp@-
    28ea:	4eba 0b52      	jsr %pc@(0x343e)
    28ee:	70ff           	moveq #-1,%d0
    28f0:	584f           	addqw #4,%sp
    28f2:	6000 00a2      	braw 0x2996
    28f6:	486e ffff      	pea %fp@(-1)
    28fa:	1f3c 0002      	moveb #2,%sp@-
    28fe:	4eba d8f6      	jsr %pc@(0x1f6)
    2902:	7200           	moveq #0,%d1
    2904:	3200           	movew %d0,%d1
    2906:	2601           	movel %d1,%d3
    2908:	7001           	moveq #1,%d0
    290a:	b680           	cmpl %d0,%d3
    290c:	5c4f           	addqw #6,%sp
    290e:	6714           	beqs 0x2924
    2910:	4a83           	tstl %d3
    2912:	660c           	bnes 0x2920
    2914:	7810           	moveq #16,%d4
    2916:	8883           	orl %d3,%d4
    2918:	2f04           	movel %d4,%sp@-
    291a:	4eba 0b22      	jsr %pc@(0x343e)
    291e:	584f           	addqw #4,%sp
    2920:	70ff           	moveq #-1,%d0
    2922:	6072           	bras 0x2996
    2924:	486e ffff      	pea %fp@(-1)
    2928:	486e fffe      	pea %fp@(-2)
    292c:	4eba d742      	jsr %pc@(0x70)
    2930:	7200           	moveq #0,%d1
    2932:	3200           	movew %d0,%d1
    2934:	2601           	movel %d1,%d3
    2936:	504f           	addqw #8,%sp
    2938:	6712           	beqs 0x294c
    293a:	7010           	moveq #16,%d0
    293c:	8680           	orl %d0,%d3
    293e:	2803           	movel %d3,%d4
    2940:	2f04           	movel %d4,%sp@-
    2942:	4eba 0afa      	jsr %pc@(0x343e)
    2946:	70ff           	moveq #-1,%d0
    2948:	584f           	addqw #4,%sp
    294a:	604a           	bras 0x2996
    294c:	7000           	moveq #0,%d0
    294e:	102e fffe      	moveb %fp@(-2),%d0
    2952:	3600           	movew %d0,%d3
    2954:	0240 0080      	andiw #128,%d0
    2958:	671a           	beqs 0x2974
    295a:	3003           	movew %d3,%d0
    295c:	0240 0040      	andiw #64,%d0
    2960:	6604           	bnes 0x2966
    2962:	7000           	moveq #0,%d0
    2964:	6030           	bras 0x2996
    2966:	7601           	moveq #1,%d3
    2968:	2f03           	movel %d3,%sp@-
    296a:	4eba 0ad2      	jsr %pc@(0x343e)
    296e:	70ff           	moveq #-1,%d0
    2970:	584f           	addqw #4,%sp
    2972:	6022           	bras 0x2996
    2974:	3003           	movew %d3,%d0
    2976:	0240 0001      	andiw #1,%d0
    297a:	670e           	beqs 0x298a
    297c:	7608           	moveq #8,%d3
    297e:	2f03           	movel %d3,%sp@-
    2980:	4eba 0abc      	jsr %pc@(0x343e)
    2984:	70ff           	moveq #-1,%d0
    2986:	584f           	addqw #4,%sp
    2988:	600c           	bras 0x2996
    298a:	7610           	moveq #16,%d3
    298c:	2f03           	movel %d3,%sp@-
    298e:	4eba 0aae      	jsr %pc@(0x343e)
    2992:	70ff           	moveq #-1,%d0
    2994:	584f           	addqw #4,%sp
    2996:	4cdf 0418      	moveml %sp@+,%d3-%d4/%a2
    299a:	4e5e           	unlk %fp
    299c:	4e75           	rts

```

# WriteFlashPages: 0x29b8 - 0x2c9e


```

    29b8:	4e56 ffbc      	linkw %fp,#-68
    29bc:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    29c0:	1a2e 000a      	moveb %fp@(10),%d5
    29c4:	262e 000e      	movel %fp@(14),%d3
    29c8:	246e 0016      	moveal %fp@(22),%a2
    29cc:	266e 001a      	moveal %fp@(26),%a3
    29d0:	41ee ffdc      	lea %fp@(-36),%a0
    29d4:	2e08           	movel %a0,%d7
    29d6:	41ee ffbc      	lea %fp@(-68),%a0
    29da:	2808           	movel %a0,%d4
    29dc:	1005           	moveb %d5,%d0
    29de:	d02e 000c      	addb %fp@(12),%d0
    29e2:	1d40 fffd      	moveb %d0,%fp@(-3)
    29e6:	2c03           	movel %d3,%d6
    29e8:	1685           	moveb %d5,%a3@
    29ea:	4a83           	tstl %d3
    29ec:	6706           	beqs 0x29f4
    29ee:	7001           	moveq #1,%d0
    29f0:	c083           	andl %d3,%d0
    29f2:	671e           	beqs 0x2a12
    29f4:	24bc 0000 2000 	movel #8192,%a2@
    29fa:	2f12           	movel %a2@,%sp@-
    29fc:	4eba 0a40      	jsr %pc@(0x343e)
    2a00:	5280           	addql #1,%d0
    2a02:	584f           	addqw #4,%sp
    2a04:	6606           	bnes 0x2a0c
    2a06:	0092 0000 4000 	oril #16384,%a2@
    2a0c:	70ff           	moveq #-1,%d0
    2a0e:	6000 0288      	braw 0x2c98
    2a12:	7001           	moveq #1,%d0
    2a14:	c0ae 0012      	andl %fp@(18),%d0
    2a18:	671e           	beqs 0x2a38
    2a1a:	24bc 0000 2000 	movel #8192,%a2@
    2a20:	2f12           	movel %a2@,%sp@-
    2a22:	4eba 0a1a      	jsr %pc@(0x343e)
    2a26:	5280           	addql #1,%d0
    2a28:	584f           	addqw #4,%sp
    2a2a:	6606           	bnes 0x2a32
    2a2c:	0092 0000 4000 	oril #16384,%a2@
    2a32:	70ff           	moveq #-1,%d0
    2a34:	6000 0262      	braw 0x2c98
    2a38:	2044           	moveal %d4,%a0
    2a3a:	10bc 0080      	moveb #-128,%a0@
    2a3e:	4228 0001      	clrb %a0@(1)
    2a42:	302e 0008      	movew %fp@(8),%d0
    2a46:	e048           	lsrw #8,%d0
    2a48:	1140 0002      	moveb %d0,%a0@(2)
    2a4c:	116e 0009 0003 	moveb %fp@(9),%a0@(3)
    2a52:	4228 0004      	clrb %a0@(4)
    2a56:	1145 0005      	moveb %d5,%a0@(5)
    2a5a:	7600           	moveq #0,%d3
    2a5c:	2a2e 0012      	movel %fp@(18),%d5
    2a60:	2045           	moveal %d5,%a0
    2a62:	2244           	moveal %d4,%a1
    2a64:	1390 3806      	moveb %a0@,%a1@(00000006,%d3:l)
    2a68:	5285           	addql #1,%d5
    2a6a:	5283           	addql #1,%d3
    2a6c:	700a           	moveq #10,%d0
    2a6e:	b680           	cmpl %d0,%d3
    2a70:	65ee           	bcss 0x2a60
    2a72:	486e ffff      	pea %fp@(-1)
    2a76:	4227           	clrb %sp@-
    2a78:	2f04           	movel %d4,%sp@-
    2a7a:	4eba d652      	jsr %pc@(0xce)
    2a7e:	7200           	moveq #0,%d1
    2a80:	3200           	movew %d0,%d1
    2a82:	2601           	movel %d1,%d3
    2a84:	4fef 000a      	lea %sp@(10),%sp
    2a88:	671a           	beqs 0x2aa4
    2a8a:	2483           	movel %d3,%a2@
    2a8c:	2f12           	movel %a2@,%sp@-
    2a8e:	4eba 09ae      	jsr %pc@(0x343e)
    2a92:	5280           	addql #1,%d0
    2a94:	584f           	addqw #4,%sp
    2a96:	6606           	bnes 0x2a9e
    2a98:	0092 0000 4000 	oril #16384,%a2@
    2a9e:	70ff           	moveq #-1,%d0
    2aa0:	6000 01f6      	braw 0x2c98
    2aa4:	486e ffff      	pea %fp@(-1)
    2aa8:	4227           	clrb %sp@-
    2aaa:	1f3c 0055      	moveb #85,%sp@-
    2aae:	4eba d6d0      	jsr %pc@(0x180)
    2ab2:	7200           	moveq #0,%d1
    2ab4:	3200           	movew %d0,%d1
    2ab6:	2601           	movel %d1,%d3
    2ab8:	504f           	addqw #8,%sp
    2aba:	671e           	beqs 0x2ada
    2abc:	7010           	moveq #16,%d0
    2abe:	8083           	orl %d3,%d0
    2ac0:	2480           	movel %d0,%a2@
    2ac2:	2f12           	movel %a2@,%sp@-
    2ac4:	4eba 0978      	jsr %pc@(0x343e)
    2ac8:	5280           	addql #1,%d0
    2aca:	584f           	addqw #4,%sp
    2acc:	6606           	bnes 0x2ad4
    2ace:	0092 0000 4000 	oril #16384,%a2@
    2ad4:	70ff           	moveq #-1,%d0
    2ad6:	6000 01c0      	braw 0x2c98
    2ada:	7800           	moveq #0,%d4
    2adc:	182e fffd      	moveb %fp@(-3),%d4
    2ae0:	3a04           	movew %d4,%d5
    2ae2:	486e ffff      	pea %fp@(-1)
    2ae6:	1f3c 0002      	moveb #2,%sp@-
    2aea:	4eba d70a      	jsr %pc@(0x1f6)
    2aee:	7200           	moveq #0,%d1
    2af0:	3200           	movew %d0,%d1
    2af2:	2601           	movel %d1,%d3
    2af4:	7001           	moveq #1,%d0
    2af6:	b680           	cmpl %d0,%d3
    2af8:	5c4f           	addqw #6,%sp
    2afa:	6738           	beqs 0x2b34
    2afc:	4a83           	tstl %d3
    2afe:	662e           	bnes 0x2b2e
    2b00:	7010           	moveq #16,%d0
    2b02:	8083           	orl %d3,%d0
    2b04:	2480           	movel %d0,%a2@
    2b06:	486e ffff      	pea %fp@(-1)
    2b0a:	2f07           	movel %d7,%sp@-
    2b0c:	4eba d61c      	jsr %pc@(0x12a)
    2b10:	4a40           	tstw %d0
    2b12:	504f           	addqw #8,%sp
    2b14:	6606           	bnes 0x2b1c
    2b16:	2047           	moveal %d7,%a0
    2b18:	16a8 0015      	moveb %a0@(21),%a3@
    2b1c:	2f12           	movel %a2@,%sp@-
    2b1e:	4eba 091e      	jsr %pc@(0x343e)
    2b22:	5280           	addql #1,%d0
    2b24:	584f           	addqw #4,%sp
    2b26:	6606           	bnes 0x2b2e
    2b28:	0092 0000 4000 	oril #16384,%a2@
    2b2e:	70ff           	moveq #-1,%d0
    2b30:	6000 0166      	braw 0x2c98
    2b34:	486e ffff      	pea %fp@(-1)
    2b38:	486e fffe      	pea %fp@(-2)
    2b3c:	4eba d532      	jsr %pc@(0x70)
    2b40:	7200           	moveq #0,%d1
    2b42:	3200           	movew %d0,%d1
    2b44:	2601           	movel %d1,%d3
    2b46:	504f           	addqw #8,%sp
    2b48:	671e           	beqs 0x2b68
    2b4a:	7010           	moveq #16,%d0
    2b4c:	8083           	orl %d3,%d0
    2b4e:	2480           	movel %d0,%a2@
    2b50:	2f12           	movel %a2@,%sp@-
    2b52:	4eba 08ea      	jsr %pc@(0x343e)
    2b56:	5280           	addql #1,%d0
    2b58:	584f           	addqw #4,%sp
    2b5a:	6606           	bnes 0x2b62
    2b5c:	0092 0000 4000 	oril #16384,%a2@
    2b62:	70ff           	moveq #-1,%d0
    2b64:	6000 0132      	braw 0x2c98
    2b68:	7000           	moveq #0,%d0
    2b6a:	102e fffe      	moveb %fp@(-2),%d0
    2b6e:	3600           	movew %d0,%d3
    2b70:	0240 0020      	andiw #32,%d0
    2b74:	6700 0080      	beqw 0x2bf6
    2b78:	1013           	moveb %a3@,%d0
    2b7a:	4880           	extw %d0
    2b7c:	b044           	cmpw %d4,%d0
    2b7e:	6638           	bnes 0x2bb8
    2b80:	486e ffff      	pea %fp@(-1)
    2b84:	4227           	clrb %sp@-
    2b86:	1f3c 0033      	moveb #51,%sp@-
    2b8a:	4eba d5f4      	jsr %pc@(0x180)
    2b8e:	7200           	moveq #0,%d1
    2b90:	3200           	movew %d0,%d1
    2b92:	2601           	movel %d1,%d3
    2b94:	504f           	addqw #8,%sp
    2b96:	6700 ff4a      	beqw 0x2ae2
    2b9a:	7010           	moveq #16,%d0
    2b9c:	8083           	orl %d3,%d0
    2b9e:	2480           	movel %d0,%a2@
    2ba0:	2f12           	movel %a2@,%sp@-
    2ba2:	4eba 089a      	jsr %pc@(0x343e)
    2ba6:	5280           	addql #1,%d0
    2ba8:	584f           	addqw #4,%sp
    2baa:	6606           	bnes 0x2bb2
    2bac:	0092 0000 4000 	oril #16384,%a2@
    2bb2:	70ff           	moveq #-1,%d0
    2bb4:	6000 00e2      	braw 0x2c98
    2bb8:	486e ffff      	pea %fp@(-1)
    2bbc:	2f06           	movel %d6,%sp@-
    2bbe:	4eba d734      	jsr %pc@(0x2f4)
    2bc2:	7200           	moveq #0,%d1
    2bc4:	3200           	movew %d0,%d1
    2bc6:	2601           	movel %d1,%d3
    2bc8:	504f           	addqw #8,%sp
    2bca:	671e           	beqs 0x2bea
    2bcc:	7010           	moveq #16,%d0
    2bce:	8083           	orl %d3,%d0
    2bd0:	2480           	movel %d0,%a2@
    2bd2:	2f12           	movel %a2@,%sp@-
    2bd4:	4eba 0868      	jsr %pc@(0x343e)
    2bd8:	5280           	addql #1,%d0
    2bda:	584f           	addqw #4,%sp
    2bdc:	6606           	bnes 0x2be4
    2bde:	0092 0000 4000 	oril #16384,%a2@
    2be4:	70ff           	moveq #-1,%d0
    2be6:	6000 00b0      	braw 0x2c98
    2bea:	0686 0000 0200 	addil #512,%d6
    2bf0:	5213           	addqb #1,%a3@
    2bf2:	6000 feee      	braw 0x2ae2
    2bf6:	3003           	movew %d3,%d0
    2bf8:	0240 0080      	andiw #128,%d0
    2bfc:	6738           	beqs 0x2c36
    2bfe:	3003           	movew %d3,%d0
    2c00:	0240 0040      	andiw #64,%d0
    2c04:	676c           	beqs 0x2c72
    2c06:	486e ffff      	pea %fp@(-1)
    2c0a:	2f07           	movel %d7,%sp@-
    2c0c:	4eba d51c      	jsr %pc@(0x12a)
    2c10:	4a40           	tstw %d0
    2c12:	504f           	addqw #8,%sp
    2c14:	6606           	bnes 0x2c1c
    2c16:	2047           	moveal %d7,%a0
    2c18:	16a8 0015      	moveb %a0@(21),%a3@
    2c1c:	7001           	moveq #1,%d0
    2c1e:	2480           	movel %d0,%a2@
    2c20:	2f12           	movel %a2@,%sp@-
    2c22:	4eba 081a      	jsr %pc@(0x343e)
    2c26:	5280           	addql #1,%d0
    2c28:	584f           	addqw #4,%sp
    2c2a:	6606           	bnes 0x2c32
    2c2c:	0092 0000 4000 	oril #16384,%a2@
    2c32:	70ff           	moveq #-1,%d0
    2c34:	6062           	bras 0x2c98
    2c36:	3003           	movew %d3,%d0
    2c38:	0240 0001      	andiw #1,%d0
    2c3c:	671a           	beqs 0x2c58
    2c3e:	7008           	moveq #8,%d0
    2c40:	2480           	movel %d0,%a2@
    2c42:	2f12           	movel %a2@,%sp@-
    2c44:	4eba 07f8      	jsr %pc@(0x343e)
    2c48:	5280           	addql #1,%d0
    2c4a:	584f           	addqw #4,%sp
    2c4c:	6606           	bnes 0x2c54
    2c4e:	0092 0000 4000 	oril #16384,%a2@
    2c54:	70ff           	moveq #-1,%d0
    2c56:	6040           	bras 0x2c98
    2c58:	7010           	moveq #16,%d0
    2c5a:	2480           	movel %d0,%a2@
    2c5c:	2f12           	movel %a2@,%sp@-
    2c5e:	4eba 07de      	jsr %pc@(0x343e)
    2c62:	5280           	addql #1,%d0
    2c64:	584f           	addqw #4,%sp
    2c66:	6606           	bnes 0x2c6e
    2c68:	0092 0000 4000 	oril #16384,%a2@
    2c6e:	70ff           	moveq #-1,%d0
    2c70:	6026           	bras 0x2c98
    2c72:	1013           	moveb %a3@,%d0
    2c74:	4880           	extw %d0
    2c76:	b045           	cmpw %d5,%d0
    2c78:	6604           	bnes 0x2c7e
    2c7a:	7000           	moveq #0,%d0
    2c7c:	601a           	bras 0x2c98
    2c7e:	24bc 0000 2000 	movel #8192,%a2@
    2c84:	2f12           	movel %a2@,%sp@-
    2c86:	4eba 07b6      	jsr %pc@(0x343e)
    2c8a:	5280           	addql #1,%d0
    2c8c:	584f           	addqw #4,%sp
    2c8e:	6606           	bnes 0x2c96
    2c90:	0092 0000 4000 	oril #16384,%a2@
    2c96:	70ff           	moveq #-1,%d0
    2c98:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    2c9c:	4e5e           	unlk %fp
    2c9e:	4e75           	rts

```

# ReadFlashPage: 0x2cb2 - 0x2ea6


```

    2cb2:	4e56 ffbe      	linkw %fp,#-66
    2cb6:	48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-
    2cba:	2c2e 0010      	movel %fp@(16),%d6
    2cbe:	2a2e 0014      	movel %fp@(20),%d5
    2cc2:	45ee ffde      	lea %fp@(-34),%a2
    2cc6:	47ee ffbe      	lea %fp@(-66),%a3
    2cca:	2045           	moveal %d5,%a0
    2ccc:	4290           	clrl %a0@
    2cce:	16bc 0080      	moveb #-128,%a3@
    2cd2:	422b 0001      	clrb %a3@(1)
    2cd6:	302e 0008      	movew %fp@(8),%d0
    2cda:	e048           	lsrw #8,%d0
    2cdc:	1740 0002      	moveb %d0,%a3@(2)
    2ce0:	176e 0009 0003 	moveb %fp@(9),%a3@(3)
    2ce6:	177c 0020 0004 	moveb #32,%a3@(4)
    2cec:	176e 000a 0005 	moveb %fp@(10),%a3@(5)
    2cf2:	486e ffff      	pea %fp@(-1)
    2cf6:	4227           	clrb %sp@-
    2cf8:	2f0b           	movel %a3,%sp@-
    2cfa:	4eba d3d2      	jsr %pc@(0xce)
    2cfe:	7200           	moveq #0,%d1
    2d00:	3200           	movew %d0,%d1
    2d02:	2601           	movel %d1,%d3
    2d04:	4fef 000a      	lea %sp@(10),%sp
    2d08:	6710           	beqs 0x2d1a
    2d0a:	2803           	movel %d3,%d4
    2d0c:	2f04           	movel %d4,%sp@-
    2d0e:	4eba 072e      	jsr %pc@(0x343e)
    2d12:	70ff           	moveq #-1,%d0
    2d14:	584f           	addqw #4,%sp
    2d16:	6000 0188      	braw 0x2ea0
    2d1a:	486e ffff      	pea %fp@(-1)
    2d1e:	4227           	clrb %sp@-
    2d20:	1f3c 00aa      	moveb #-86,%sp@-
    2d24:	4eba d45a      	jsr %pc@(0x180)
    2d28:	7200           	moveq #0,%d1
    2d2a:	3200           	movew %d0,%d1
    2d2c:	2601           	movel %d1,%d3
    2d2e:	504f           	addqw #8,%sp
    2d30:	6712           	beqs 0x2d44
    2d32:	7810           	moveq #16,%d4
    2d34:	8883           	orl %d3,%d4
    2d36:	2f04           	movel %d4,%sp@-
    2d38:	4eba 0704      	jsr %pc@(0x343e)
    2d3c:	70ff           	moveq #-1,%d0
    2d3e:	584f           	addqw #4,%sp
    2d40:	6000 015e      	braw 0x2ea0
    2d44:	486e ffff      	pea %fp@(-1)
    2d48:	1f3c 0001      	moveb #1,%sp@-
    2d4c:	4eba d4a8      	jsr %pc@(0x1f6)
    2d50:	7200           	moveq #0,%d1
    2d52:	3200           	movew %d0,%d1
    2d54:	2601           	movel %d1,%d3
    2d56:	7001           	moveq #1,%d0
    2d58:	b680           	cmpl %d0,%d3
    2d5a:	5c4f           	addqw #6,%sp
    2d5c:	6716           	beqs 0x2d74
    2d5e:	4a83           	tstl %d3
    2d60:	660c           	bnes 0x2d6e
    2d62:	7810           	moveq #16,%d4
    2d64:	8883           	orl %d3,%d4
    2d66:	2f04           	movel %d4,%sp@-
    2d68:	4eba 06d4      	jsr %pc@(0x343e)
    2d6c:	584f           	addqw #4,%sp
    2d6e:	70ff           	moveq #-1,%d0
    2d70:	6000 012e      	braw 0x2ea0
    2d74:	486e ffff      	pea %fp@(-1)
    2d78:	2f0a           	movel %a2,%sp@-
    2d7a:	4eba d3ae      	jsr %pc@(0x12a)
    2d7e:	7200           	moveq #0,%d1
    2d80:	3200           	movew %d0,%d1
    2d82:	2601           	movel %d1,%d3
    2d84:	504f           	addqw #8,%sp
    2d86:	6712           	beqs 0x2d9a
    2d88:	7810           	moveq #16,%d4
    2d8a:	8883           	orl %d3,%d4
    2d8c:	2f04           	movel %d4,%sp@-
    2d8e:	4eba 06ae      	jsr %pc@(0x343e)
    2d92:	70ff           	moveq #-1,%d0
    2d94:	584f           	addqw #4,%sp
    2d96:	6000 0108      	braw 0x2ea0
    2d9a:	7000           	moveq #0,%d0
    2d9c:	102a 0001      	moveb %a2@(1),%d0
    2da0:	3600           	movew %d0,%d3
    2da2:	0240 0020      	andiw #32,%d0
    2da6:	6700 00d6      	beqw 0x2e7e
    2daa:	486e ffff      	pea %fp@(-1)
    2dae:	2f2e 000c      	movel %fp@(12),%sp@-
    2db2:	4eba d4d8      	jsr %pc@(0x28c)
    2db6:	7200           	moveq #0,%d1
    2db8:	3200           	movew %d0,%d1
    2dba:	2801           	movel %d1,%d4
    2dbc:	504f           	addqw #8,%sp
    2dbe:	6712           	beqs 0x2dd2
    2dc0:	7610           	moveq #16,%d3
    2dc2:	8684           	orl %d4,%d3
    2dc4:	2f03           	movel %d3,%sp@-
    2dc6:	4eba 0676      	jsr %pc@(0x343e)
    2dca:	70ff           	moveq #-1,%d0
    2dcc:	584f           	addqw #4,%sp
    2dce:	6000 00d0      	braw 0x2ea0
    2dd2:	4a86           	tstl %d6
    2dd4:	6712           	beqs 0x2de8
    2dd6:	7600           	moveq #0,%d3
    2dd8:	2646           	moveal %d6,%a3
    2dda:	16b2 3816      	moveb %a2@(00000016,%d3:l),%a3@
    2dde:	528b           	addql #1,%a3
    2de0:	5283           	addql #1,%d3
    2de2:	700a           	moveq #10,%d0
    2de4:	b680           	cmpl %d0,%d3
    2de6:	65f2           	bcss 0x2dda
    2de8:	102a 0001      	moveb %a2@(1),%d0
    2dec:	0240 0040      	andiw #64,%d0
    2df0:	676a           	beqs 0x2e5c
    2df2:	7000           	moveq #0,%d0
    2df4:	102a 0003      	moveb %a2@(3),%d0
    2df8:	3600           	movew %d0,%d3
    2dfa:	0240 0020      	andiw #32,%d0
    2dfe:	6714           	beqs 0x2e14
    2e00:	3003           	movew %d3,%d0
    2e02:	0240 0010      	andiw #16,%d0
    2e06:	6704           	beqs 0x2e0c
    2e08:	7802           	moveq #2,%d4
    2e0a:	600e           	bras 0x2e1a
    2e0c:	2045           	moveal %d5,%a0
    2e0e:	7002           	moveq #2,%d0
    2e10:	2080           	movel %d0,%a0@
    2e12:	6006           	bras 0x2e1a
    2e14:	2045           	moveal %d5,%a0
    2e16:	7001           	moveq #1,%d0
    2e18:	2080           	movel %d0,%a0@
    2e1a:	4a86           	tstl %d6
    2e1c:	6724           	beqs 0x2e42
    2e1e:	7000           	moveq #0,%d0
    2e20:	102a 0003      	moveb %a2@(3),%d0
    2e24:	3600           	movew %d0,%d3
    2e26:	0240 000a      	andiw #10,%d0
    2e2a:	6716           	beqs 0x2e42
    2e2c:	3003           	movew %d3,%d0
    2e2e:	0240 0005      	andiw #5,%d0
    2e32:	6706           	beqs 0x2e3a
    2e34:	7002           	moveq #2,%d0
    2e36:	8880           	orl %d0,%d4
    2e38:	600e           	bras 0x2e48
    2e3a:	2045           	moveal %d5,%a0
    2e3c:	7008           	moveq #8,%d0
    2e3e:	8190           	orl %d0,%a0@
    2e40:	6006           	bras 0x2e48
    2e42:	2045           	moveal %d5,%a0
    2e44:	7004           	moveq #4,%d0
    2e46:	8190           	orl %d0,%a0@
    2e48:	7002           	moveq #2,%d0
    2e4a:	c084           	andl %d4,%d0
    2e4c:	6714           	beqs 0x2e62
    2e4e:	2604           	movel %d4,%d3
    2e50:	2f03           	movel %d3,%sp@-
    2e52:	4eba 05ea      	jsr %pc@(0x343e)
    2e56:	70ff           	moveq #-1,%d0
    2e58:	584f           	addqw #4,%sp
    2e5a:	6044           	bras 0x2ea0
    2e5c:	2045           	moveal %d5,%a0
    2e5e:	7005           	moveq #5,%d0
    2e60:	2080           	movel %d0,%a0@
    2e62:	102a 0001      	moveb %a2@(1),%d0
    2e66:	0240 0080      	andiw #128,%d0
    2e6a:	6704           	beqs 0x2e70
    2e6c:	7000           	moveq #0,%d0
    2e6e:	6030           	bras 0x2ea0
    2e70:	7610           	moveq #16,%d3
    2e72:	2f03           	movel %d3,%sp@-
    2e74:	4eba 05c8      	jsr %pc@(0x343e)
    2e78:	70ff           	moveq #-1,%d0
    2e7a:	584f           	addqw #4,%sp
    2e7c:	6022           	bras 0x2ea0
    2e7e:	3003           	movew %d3,%d0
    2e80:	0240 0001      	andiw #1,%d0
    2e84:	670e           	beqs 0x2e94
    2e86:	7608           	moveq #8,%d3
    2e88:	2f03           	movel %d3,%sp@-
    2e8a:	4eba 05b2      	jsr %pc@(0x343e)
    2e8e:	70ff           	moveq #-1,%d0
    2e90:	584f           	addqw #4,%sp
    2e92:	600c           	bras 0x2ea0
    2e94:	7610           	moveq #16,%d3
    2e96:	2f03           	movel %d3,%sp@-
    2e98:	4eba 05a4      	jsr %pc@(0x343e)
    2e9c:	70ff           	moveq #-1,%d0
    2e9e:	584f           	addqw #4,%sp
    2ea0:	4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3
    2ea4:	4e5e           	unlk %fp
    2ea6:	4e75           	rts

```

# ReadFlashPages: 0x2eb8 - 0x329c


```

    2eb8:	4e56 ffb0      	linkw %fp,#-80
    2ebc:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    2ec0:	162e 000a      	moveb %fp@(10),%d3
    2ec4:	2c2e 0016      	movel %fp@(22),%d6
    2ec8:	282e 001a      	movel %fp@(26),%d4
    2ecc:	2a2e 001e      	movel %fp@(30),%d5
    2ed0:	45ee ffd0      	lea %fp@(-48),%a2
    2ed4:	41ee ffb0      	lea %fp@(-80),%a0
    2ed8:	2e08           	movel %a0,%d7
    2eda:	1003           	moveb %d3,%d0
    2edc:	d02e 000c      	addb %fp@(12),%d0
    2ee0:	5300           	subqb #1,%d0
    2ee2:	1d40 fff3      	moveb %d0,%fp@(-13)
    2ee6:	2d6e 000e fff4 	movel %fp@(14),%fp@(-12)
    2eec:	2045           	moveal %d5,%a0
    2eee:	1083           	moveb %d3,%a0@
    2ef0:	2d6e 0012 fff8 	movel %fp@(18),%fp@(-8)
    2ef6:	7000           	moveq #0,%d0
    2ef8:	1003           	moveb %d3,%d0
    2efa:	c0fc 000a      	muluw #10,%d0
    2efe:	d1ae fff8      	addl %d0,%fp@(-8)
    2f02:	2047           	moveal %d7,%a0
    2f04:	10bc 0080      	moveb #-128,%a0@
    2f08:	4228 0001      	clrb %a0@(1)
    2f0c:	302e 0008      	movew %fp@(8),%d0
    2f10:	e048           	lsrw #8,%d0
    2f12:	1140 0002      	moveb %d0,%a0@(2)
    2f16:	116e 0009 0003 	moveb %fp@(9),%a0@(3)
    2f1c:	4228 0004      	clrb %a0@(4)
    2f20:	1143 0005      	moveb %d3,%a0@(5)
    2f24:	486e ffff      	pea %fp@(-1)
    2f28:	4227           	clrb %sp@-
    2f2a:	2f07           	movel %d7,%sp@-
    2f2c:	4eba d1a0      	jsr %pc@(0xce)
    2f30:	7200           	moveq #0,%d1
    2f32:	3200           	movew %d0,%d1
    2f34:	2601           	movel %d1,%d3
    2f36:	4fef 000a      	lea %sp@(10),%sp
    2f3a:	671e           	beqs 0x2f5a
    2f3c:	2044           	moveal %d4,%a0
    2f3e:	2083           	movel %d3,%a0@
    2f40:	2f10           	movel %a0@,%sp@-
    2f42:	4eba 04fa      	jsr %pc@(0x343e)
    2f46:	5280           	addql #1,%d0
    2f48:	584f           	addqw #4,%sp
    2f4a:	6608           	bnes 0x2f54
    2f4c:	2044           	moveal %d4,%a0
    2f4e:	0090 0000 4000 	oril #16384,%a0@
    2f54:	70ff           	moveq #-1,%d0
    2f56:	6000 033e      	braw 0x3296
    2f5a:	486e ffff      	pea %fp@(-1)
    2f5e:	4227           	clrb %sp@-
    2f60:	1f3c 00aa      	moveb #-86,%sp@-
    2f64:	4eba d21a      	jsr %pc@(0x180)
    2f68:	7200           	moveq #0,%d1
    2f6a:	3200           	movew %d0,%d1
    2f6c:	2601           	movel %d1,%d3
    2f6e:	504f           	addqw #8,%sp
    2f70:	6722           	beqs 0x2f94
    2f72:	7010           	moveq #16,%d0
    2f74:	8083           	orl %d3,%d0
    2f76:	2044           	moveal %d4,%a0
    2f78:	2080           	movel %d0,%a0@
    2f7a:	2f10           	movel %a0@,%sp@-
    2f7c:	4eba 04c0      	jsr %pc@(0x343e)
    2f80:	5280           	addql #1,%d0
    2f82:	584f           	addqw #4,%sp
    2f84:	6608           	bnes 0x2f8e
    2f86:	2044           	moveal %d4,%a0
    2f88:	0090 0000 4000 	oril #16384,%a0@
    2f8e:	70ff           	moveq #-1,%d0
    2f90:	6000 0304      	braw 0x3296
    2f94:	2046           	moveal %d6,%a0
    2f96:	7005           	moveq #5,%d0
    2f98:	2080           	movel %d0,%a0@
    2f9a:	7000           	moveq #0,%d0
    2f9c:	102e fff3      	moveb %fp@(-13),%d0
    2fa0:	3d40 fff0      	movew %d0,%fp@(-16)
    2fa4:	3d40 fffc      	movew %d0,%fp@(-4)
    2fa8:	486e ffff      	pea %fp@(-1)
    2fac:	1f3c 0001      	moveb #1,%sp@-
    2fb0:	4eba d244      	jsr %pc@(0x1f6)
    2fb4:	7200           	moveq #0,%d1
    2fb6:	3200           	movew %d0,%d1
    2fb8:	2601           	movel %d1,%d3
    2fba:	7001           	moveq #1,%d0
    2fbc:	b680           	cmpl %d0,%d3
    2fbe:	5c4f           	addqw #6,%sp
    2fc0:	6732           	beqs 0x2ff4
    2fc2:	4a83           	tstl %d3
    2fc4:	6628           	bnes 0x2fee
    2fc6:	486e ffff      	pea %fp@(-1)
    2fca:	2f0a           	movel %a2,%sp@-
    2fcc:	4eba d15c      	jsr %pc@(0x12a)
    2fd0:	7010           	moveq #16,%d0
    2fd2:	8083           	orl %d3,%d0
    2fd4:	2044           	moveal %d4,%a0
    2fd6:	2080           	movel %d0,%a0@
    2fd8:	2f10           	movel %a0@,%sp@-
    2fda:	4eba 0462      	jsr %pc@(0x343e)
    2fde:	5280           	addql #1,%d0
    2fe0:	4fef 000c      	lea %sp@(12),%sp
    2fe4:	6608           	bnes 0x2fee
    2fe6:	2044           	moveal %d4,%a0
    2fe8:	0090 0000 4000 	oril #16384,%a0@
    2fee:	70ff           	moveq #-1,%d0
    2ff0:	6000 02a4      	braw 0x3296
    2ff4:	486e ffff      	pea %fp@(-1)
    2ff8:	2f0a           	movel %a2,%sp@-
    2ffa:	4eba d12e      	jsr %pc@(0x12a)
    2ffe:	7200           	moveq #0,%d1
    3000:	3200           	movew %d0,%d1
    3002:	2601           	movel %d1,%d3
    3004:	504f           	addqw #8,%sp
    3006:	6722           	beqs 0x302a
    3008:	7010           	moveq #16,%d0
    300a:	8083           	orl %d3,%d0
    300c:	2044           	moveal %d4,%a0
    300e:	2080           	movel %d0,%a0@
    3010:	2f10           	movel %a0@,%sp@-
    3012:	4eba 042a      	jsr %pc@(0x343e)
    3016:	5280           	addql #1,%d0
    3018:	584f           	addqw #4,%sp
    301a:	6608           	bnes 0x3024
    301c:	2044           	moveal %d4,%a0
    301e:	0090 0000 4000 	oril #16384,%a0@
    3024:	70ff           	moveq #-1,%d0
    3026:	6000 026e      	braw 0x3296
    302a:	7000           	moveq #0,%d0
    302c:	102a 0001      	moveb %a2@(1),%d0
    3030:	3600           	movew %d0,%d3
    3032:	0240 0020      	andiw #32,%d0
    3036:	6700 01e4      	beqw 0x321c
    303a:	2045           	moveal %d5,%a0
    303c:	1010           	moveb %a0@,%d0
    303e:	4880           	extw %d0
    3040:	b06e fffc      	cmpw %fp@(-4),%d0
    3044:	6642           	bnes 0x3088
    3046:	3003           	movew %d3,%d0
    3048:	0240 0080      	andiw #128,%d0
    304c:	663a           	bnes 0x3088
    304e:	486e ffff      	pea %fp@(-1)
    3052:	4227           	clrb %sp@-
    3054:	1f3c 0033      	moveb #51,%sp@-
    3058:	4eba d126      	jsr %pc@(0x180)
    305c:	7200           	moveq #0,%d1
    305e:	3200           	movew %d0,%d1
    3060:	2601           	movel %d1,%d3
    3062:	504f           	addqw #8,%sp
    3064:	6722           	beqs 0x3088
    3066:	7010           	moveq #16,%d0
    3068:	8083           	orl %d3,%d0
    306a:	2044           	moveal %d4,%a0
    306c:	2080           	movel %d0,%a0@
    306e:	2f10           	movel %a0@,%sp@-
    3070:	4eba 03cc      	jsr %pc@(0x343e)
    3074:	5280           	addql #1,%d0
    3076:	584f           	addqw #4,%sp
    3078:	6608           	bnes 0x3082
    307a:	2044           	moveal %d4,%a0
    307c:	0090 0000 4000 	oril #16384,%a0@
    3082:	70ff           	moveq #-1,%d0
    3084:	6000 0210      	braw 0x3296
    3088:	4aae 0012      	tstl %fp@(18)
    308c:	671a           	beqs 0x30a8
    308e:	7600           	moveq #0,%d3
    3090:	266e fff8      	moveal %fp@(-8),%a3
    3094:	16b2 3816      	moveb %a2@(00000016,%d3:l),%a3@
    3098:	528b           	addql #1,%a3
    309a:	5283           	addql #1,%d3
    309c:	700a           	moveq #10,%d0
    309e:	b680           	cmpl %d0,%d3
    30a0:	65f2           	bcss 0x3094
    30a2:	700a           	moveq #10,%d0
    30a4:	d1ae fff8      	addl %d0,%fp@(-8)
    30a8:	486e ffff      	pea %fp@(-1)
    30ac:	2f2e fff4      	movel %fp@(-12),%sp@-
    30b0:	4eba d1da      	jsr %pc@(0x28c)
    30b4:	7200           	moveq #0,%d1
    30b6:	3200           	movew %d0,%d1
    30b8:	2601           	movel %d1,%d3
    30ba:	504f           	addqw #8,%sp
    30bc:	6722           	beqs 0x30e0
    30be:	7010           	moveq #16,%d0
    30c0:	8083           	orl %d3,%d0
    30c2:	2044           	moveal %d4,%a0
    30c4:	2080           	movel %d0,%a0@
    30c6:	2f10           	movel %a0@,%sp@-
    30c8:	4eba 0374      	jsr %pc@(0x343e)
    30cc:	5280           	addql #1,%d0
    30ce:	584f           	addqw #4,%sp
    30d0:	6608           	bnes 0x30da
    30d2:	2044           	moveal %d4,%a0
    30d4:	0090 0000 4000 	oril #16384,%a0@
    30da:	70ff           	moveq #-1,%d0
    30dc:	6000 01b8      	braw 0x3296
    30e0:	7000           	moveq #0,%d0
    30e2:	102a 0001      	moveb %a2@(1),%d0
    30e6:	3600           	movew %d0,%d3
    30e8:	0240 0040      	andiw #64,%d0
    30ec:	6700 010a      	beqw 0x31f8
    30f0:	7600           	moveq #0,%d3
    30f2:	102a 0003      	moveb %a2@(3),%d0
    30f6:	0240 0020      	andiw #32,%d0
    30fa:	671a           	beqs 0x3116
    30fc:	2046           	moveal %d6,%a0
    30fe:	70fe           	moveq #-2,%d0
    3100:	c190           	andl %d0,%a0@
    3102:	102a 0003      	moveb %a2@(3),%d0
    3106:	0240 0010      	andiw #16,%d0
    310a:	6704           	beqs 0x3110
    310c:	7602           	moveq #2,%d3
    310e:	6006           	bras 0x3116
    3110:	2046           	moveal %d6,%a0
    3112:	7002           	moveq #2,%d0
    3114:	8190           	orl %d0,%a0@
    3116:	4aae 0012      	tstl %fp@(18)
    311a:	6724           	beqs 0x3140
    311c:	102a 0003      	moveb %a2@(3),%d0
    3120:	0240 000a      	andiw #10,%d0
    3124:	671a           	beqs 0x3140
    3126:	2046           	moveal %d6,%a0
    3128:	70fb           	moveq #-5,%d0
    312a:	c190           	andl %d0,%a0@
    312c:	102a 0003      	moveb %a2@(3),%d0
    3130:	0240 0005      	andiw #5,%d0
    3134:	6704           	beqs 0x313a
    3136:	7602           	moveq #2,%d3
    3138:	6006           	bras 0x3140
    313a:	2046           	moveal %d6,%a0
    313c:	7008           	moveq #8,%d0
    313e:	8190           	orl %d0,%a0@
    3140:	7002           	moveq #2,%d0
    3142:	b680           	cmpl %d0,%d3
    3144:	661e           	bnes 0x3164
    3146:	2044           	moveal %d4,%a0
    3148:	2083           	movel %d3,%a0@
    314a:	2f10           	movel %a0@,%sp@-
    314c:	4eba 02f0      	jsr %pc@(0x343e)
    3150:	5280           	addql #1,%d0
    3152:	584f           	addqw #4,%sp
    3154:	6608           	bnes 0x315e
    3156:	2044           	moveal %d4,%a0
    3158:	0090 0000 4000 	oril #16384,%a0@
    315e:	70ff           	moveq #-1,%d0
    3160:	6000 0134      	braw 0x3296
    3164:	102a 0001      	moveb %a2@(1),%d0
    3168:	0240 0080      	andiw #128,%d0
    316c:	6700 0092      	beqw 0x3200
    3170:	2045           	moveal %d5,%a0
    3172:	1010           	moveb %a0@,%d0
    3174:	4880           	extw %d0
    3176:	b06e fffc      	cmpw %fp@(-4),%d0
    317a:	6700 00ec      	beqw 0x3268
    317e:	1010           	moveb %a0@,%d0
    3180:	5200           	addqb #1,%d0
    3182:	2047           	moveal %d7,%a0
    3184:	1140 0005      	moveb %d0,%a0@(5)
    3188:	486e ffff      	pea %fp@(-1)
    318c:	4227           	clrb %sp@-
    318e:	2f07           	movel %d7,%sp@-
    3190:	4eba cf3c      	jsr %pc@(0xce)
    3194:	7200           	moveq #0,%d1
    3196:	3200           	movew %d0,%d1
    3198:	2601           	movel %d1,%d3
    319a:	4fef 000a      	lea %sp@(10),%sp
    319e:	671e           	beqs 0x31be
    31a0:	2044           	moveal %d4,%a0
    31a2:	2083           	movel %d3,%a0@
    31a4:	2f10           	movel %a0@,%sp@-
    31a6:	4eba 0296      	jsr %pc@(0x343e)
    31aa:	5280           	addql #1,%d0
    31ac:	584f           	addqw #4,%sp
    31ae:	6608           	bnes 0x31b8
    31b0:	2044           	moveal %d4,%a0
    31b2:	0090 0000 4000 	oril #16384,%a0@
    31b8:	70ff           	moveq #-1,%d0
    31ba:	6000 00da      	braw 0x3296
    31be:	486e ffff      	pea %fp@(-1)
    31c2:	4227           	clrb %sp@-
    31c4:	1f3c 00aa      	moveb #-86,%sp@-
    31c8:	4eba cfb6      	jsr %pc@(0x180)
    31cc:	7200           	moveq #0,%d1
    31ce:	3200           	movew %d0,%d1
    31d0:	2601           	movel %d1,%d3
    31d2:	504f           	addqw #8,%sp
    31d4:	672a           	beqs 0x3200
    31d6:	7010           	moveq #16,%d0
    31d8:	8083           	orl %d3,%d0
    31da:	2044           	moveal %d4,%a0
    31dc:	2080           	movel %d0,%a0@
    31de:	2f10           	movel %a0@,%sp@-
    31e0:	4eba 025c      	jsr %pc@(0x343e)
    31e4:	5280           	addql #1,%d0
    31e6:	584f           	addqw #4,%sp
    31e8:	6608           	bnes 0x31f2
    31ea:	2044           	moveal %d4,%a0
    31ec:	0090 0000 4000 	oril #16384,%a0@
    31f2:	70ff           	moveq #-1,%d0
    31f4:	6000 00a0      	braw 0x3296
    31f8:	3003           	movew %d3,%d0
    31fa:	0240 0080      	andiw #128,%d0
    31fe:	6668           	bnes 0x3268
    3200:	2045           	moveal %d5,%a0
    3202:	1010           	moveb %a0@,%d0
    3204:	4880           	extw %d0
    3206:	b06e fffc      	cmpw %fp@(-4),%d0
    320a:	6c00 fd9c      	bgew 0x2fa8
    320e:	06ae 0000 0200 	addil #512,%fp@(-12)
    3214:	fff4
    3216:	5210           	addqb #1,%a0@
    3218:	6000 fd8e      	braw 0x2fa8
    321c:	3003           	movew %d3,%d0
    321e:	0240 0080      	andiw #128,%d0
    3222:	6644           	bnes 0x3268
    3224:	3003           	movew %d3,%d0
    3226:	0240 0001      	andiw #1,%d0
    322a:	671e           	beqs 0x324a
    322c:	2044           	moveal %d4,%a0
    322e:	7008           	moveq #8,%d0
    3230:	2080           	movel %d0,%a0@
    3232:	2f10           	movel %a0@,%sp@-
    3234:	4eba 0208      	jsr %pc@(0x343e)
    3238:	5280           	addql #1,%d0
    323a:	584f           	addqw #4,%sp
    323c:	6608           	bnes 0x3246
    323e:	2044           	moveal %d4,%a0
    3240:	0090 0000 4000 	oril #16384,%a0@
    3246:	70ff           	moveq #-1,%d0
    3248:	604c           	bras 0x3296
    324a:	2044           	moveal %d4,%a0
    324c:	7010           	moveq #16,%d0
    324e:	2080           	movel %d0,%a0@
    3250:	2f10           	movel %a0@,%sp@-
    3252:	4eba 01ea      	jsr %pc@(0x343e)
    3256:	5280           	addql #1,%d0
    3258:	584f           	addqw #4,%sp
    325a:	6608           	bnes 0x3264
    325c:	2044           	moveal %d4,%a0
    325e:	0090 0000 4000 	oril #16384,%a0@
    3264:	70ff           	moveq #-1,%d0
    3266:	602e           	bras 0x3296
    3268:	2045           	moveal %d5,%a0
    326a:	1010           	moveb %a0@,%d0
    326c:	4880           	extw %d0
    326e:	b06e fff0      	cmpw %fp@(-16),%d0
    3272:	6604           	bnes 0x3278
    3274:	7000           	moveq #0,%d0
    3276:	601e           	bras 0x3296
    3278:	2044           	moveal %d4,%a0
    327a:	20bc 0000 2000 	movel #8192,%a0@
    3280:	2f10           	movel %a0@,%sp@-
    3282:	4eba 01ba      	jsr %pc@(0x343e)
    3286:	5280           	addql #1,%d0
    3288:	584f           	addqw #4,%sp
    328a:	6608           	bnes 0x3294
    328c:	2044           	moveal %d4,%a0
    328e:	0090 0000 4000 	oril #16384,%a0@
    3294:	70ff           	moveq #-1,%d0
    3296:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    329a:	4e5e           	unlk %fp
    329c:	4e75           	rts

```

# EraseAllPages: 0x32b0 - 0x342c


```

    32b0:	4e56 ffde      	linkw %fp,#-34
    32b4:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    32b8:	246e 000a      	moveal %fp@(10),%a2
    32bc:	47ee ffde      	lea %fp@(-34),%a3
    32c0:	16bc 0080      	moveb #-128,%a3@
    32c4:	422b 0001      	clrb %a3@(1)
    32c8:	302e 0008      	movew %fp@(8),%d0
    32cc:	e048           	lsrw #8,%d0
    32ce:	1740 0002      	moveb %d0,%a3@(2)
    32d2:	176e 0009 0003 	moveb %fp@(9),%a3@(3)
    32d8:	177c 0020 0004 	moveb #32,%a3@(4)
    32de:	422b 0005      	clrb %a3@(5)
    32e2:	486e ffff      	pea %fp@(-1)
    32e6:	4227           	clrb %sp@-
    32e8:	2f0b           	movel %a3,%sp@-
    32ea:	4eba cde2      	jsr %pc@(0xce)
    32ee:	7200           	moveq #0,%d1
    32f0:	3200           	movew %d0,%d1
    32f2:	2601           	movel %d1,%d3
    32f4:	4fef 000a      	lea %sp@(10),%sp
    32f8:	671a           	beqs 0x3314
    32fa:	2483           	movel %d3,%a2@
    32fc:	2f12           	movel %a2@,%sp@-
    32fe:	4eba 013e      	jsr %pc@(0x343e)
    3302:	5280           	addql #1,%d0
    3304:	584f           	addqw #4,%sp
    3306:	6606           	bnes 0x330e
    3308:	0092 0000 4000 	oril #16384,%a2@
    330e:	70ff           	moveq #-1,%d0
    3310:	6000 0114      	braw 0x3426
    3314:	486e ffff      	pea %fp@(-1)
    3318:	4227           	clrb %sp@-
    331a:	1f3c 0099      	moveb #-103,%sp@-
    331e:	4eba ce60      	jsr %pc@(0x180)
    3322:	7200           	moveq #0,%d1
    3324:	3200           	movew %d0,%d1
    3326:	2601           	movel %d1,%d3
    3328:	504f           	addqw #8,%sp
    332a:	671e           	beqs 0x334a
    332c:	7010           	moveq #16,%d0
    332e:	8083           	orl %d3,%d0
    3330:	2480           	movel %d0,%a2@
    3332:	2f12           	movel %a2@,%sp@-
    3334:	4eba 0108      	jsr %pc@(0x343e)
    3338:	5280           	addql #1,%d0
    333a:	584f           	addqw #4,%sp
    333c:	6606           	bnes 0x3344
    333e:	0092 0000 4000 	oril #16384,%a2@
    3344:	70ff           	moveq #-1,%d0
    3346:	6000 00de      	braw 0x3426
    334a:	486e ffff      	pea %fp@(-1)
    334e:	1f3c 0003      	moveb #3,%sp@-
    3352:	4eba cea2      	jsr %pc@(0x1f6)
    3356:	7200           	moveq #0,%d1
    3358:	3200           	movew %d0,%d1
    335a:	2601           	movel %d1,%d3
    335c:	7001           	moveq #1,%d0
    335e:	b680           	cmpl %d0,%d3
    3360:	5c4f           	addqw #6,%sp
    3362:	6722           	beqs 0x3386
    3364:	4a83           	tstl %d3
    3366:	6618           	bnes 0x3380
    3368:	7010           	moveq #16,%d0
    336a:	8083           	orl %d3,%d0
    336c:	2480           	movel %d0,%a2@
    336e:	2f12           	movel %a2@,%sp@-
    3370:	4eba 00cc      	jsr %pc@(0x343e)
    3374:	5280           	addql #1,%d0
    3376:	584f           	addqw #4,%sp
    3378:	6606           	bnes 0x3380
    337a:	0092 0000 4000 	oril #16384,%a2@
    3380:	70ff           	moveq #-1,%d0
    3382:	6000 00a2      	braw 0x3426
    3386:	486e ffff      	pea %fp@(-1)
    338a:	486e fffe      	pea %fp@(-2)
    338e:	4eba cce0      	jsr %pc@(0x70)
    3392:	7200           	moveq #0,%d1
    3394:	3200           	movew %d0,%d1
    3396:	2601           	movel %d1,%d3
    3398:	504f           	addqw #8,%sp
    339a:	671c           	beqs 0x33b8
    339c:	7010           	moveq #16,%d0
    339e:	8083           	orl %d3,%d0
    33a0:	2480           	movel %d0,%a2@
    33a2:	2f12           	movel %a2@,%sp@-
    33a4:	4eba 0098      	jsr %pc@(0x343e)
    33a8:	5280           	addql #1,%d0
    33aa:	584f           	addqw #4,%sp
    33ac:	6606           	bnes 0x33b4
    33ae:	0092 0000 4000 	oril #16384,%a2@
    33b4:	70ff           	moveq #-1,%d0
    33b6:	606e           	bras 0x3426
    33b8:	7000           	moveq #0,%d0
    33ba:	102e fffe      	moveb %fp@(-2),%d0
    33be:	3600           	movew %d0,%d3
    33c0:	0240 0080      	andiw #128,%d0
    33c4:	6726           	beqs 0x33ec
    33c6:	3003           	movew %d3,%d0
    33c8:	0240 0040      	andiw #64,%d0
    33cc:	6604           	bnes 0x33d2
    33ce:	7000           	moveq #0,%d0
    33d0:	6054           	bras 0x3426
    33d2:	7004           	moveq #4,%d0
    33d4:	2480           	movel %d0,%a2@
    33d6:	2f12           	movel %a2@,%sp@-
    33d8:	4eba 0064      	jsr %pc@(0x343e)
    33dc:	5280           	addql #1,%d0
    33de:	584f           	addqw #4,%sp
    33e0:	6606           	bnes 0x33e8
    33e2:	0092 0000 4000 	oril #16384,%a2@
    33e8:	70ff           	moveq #-1,%d0
    33ea:	603a           	bras 0x3426
    33ec:	3003           	movew %d3,%d0
    33ee:	0240 0001      	andiw #1,%d0
    33f2:	671a           	beqs 0x340e
    33f4:	7008           	moveq #8,%d0
    33f6:	2480           	movel %d0,%a2@
    33f8:	2f12           	movel %a2@,%sp@-
    33fa:	4eba 0042      	jsr %pc@(0x343e)
    33fe:	5280           	addql #1,%d0
    3400:	584f           	addqw #4,%sp
    3402:	6606           	bnes 0x340a
    3404:	0092 0000 4000 	oril #16384,%a2@
    340a:	70ff           	moveq #-1,%d0
    340c:	6018           	bras 0x3426
    340e:	7010           	moveq #16,%d0
    3410:	2480           	movel %d0,%a2@
    3412:	2f12           	movel %a2@,%sp@-
    3414:	4eba 0028      	jsr %pc@(0x343e)
    3418:	5280           	addql #1,%d0
    341a:	584f           	addqw #4,%sp
    341c:	6606           	bnes 0x3424
    341e:	0092 0000 4000 	oril #16384,%a2@
    3424:	70ff           	moveq #-1,%d0
    3426:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
    342a:	4e5e           	unlk %fp
    342c:	4e75           	rts

```

# Restore: 0x343e - 0x359c


```

    343e:	4e56 ffde      	linkw %fp,#-34
    3442:	2f0a           	movel %a2,%sp@-
    3444:	2f03           	movel %d3,%sp@-
    3446:	262e 0008      	movel %fp@(8),%d3
    344a:	45ee ffde      	lea %fp@(-34),%a2
    344e:	2003           	movel %d3,%d0
    3450:	0280 0000 2008 	andil #8200,%d0
    3456:	670a           	beqs 0x3462
    3458:	486e fffe      	pea %fp@(-2)
    345c:	4eba eefe      	jsr %pc@(0x235c)
    3460:	584f           	addqw #4,%sp
    3462:	2003           	movel %d3,%d0
    3464:	0280 0000 2018 	andil #8216,%d0
    346a:	671a           	beqs 0x3486
    346c:	486e ffff      	pea %fp@(-1)
    3470:	4227           	clrb %sp@-
    3472:	1f3c 003c      	moveb #60,%sp@-
    3476:	4eba cd08      	jsr %pc@(0x180)
    347a:	4a40           	tstw %d0
    347c:	504f           	addqw #8,%sp
    347e:	6706           	beqs 0x3486
    3480:	70ff           	moveq #-1,%d0
    3482:	6000 0112      	braw 0x3596
    3486:	7010           	moveq #16,%d0
    3488:	c083           	andl %d3,%d0
    348a:	672c           	beqs 0x34b8
    348c:	486e ffff      	pea %fp@(-1)
    3490:	4227           	clrb %sp@-
    3492:	1f3c 00cc      	moveb #-52,%sp@-
    3496:	4eba cce8      	jsr %pc@(0x180)
    349a:	4a40           	tstw %d0
    349c:	504f           	addqw #8,%sp
    349e:	6612           	bnes 0x34b2
    34a0:	486e ffff      	pea %fp@(-1)
    34a4:	1f3c 0004      	moveb #4,%sp@-
    34a8:	4eba cd4c      	jsr %pc@(0x1f6)
    34ac:	5340           	subqw #1,%d0
    34ae:	5c4f           	addqw #6,%sp
    34b0:	6706           	beqs 0x34b8
    34b2:	70ff           	moveq #-1,%d0
    34b4:	6000 00e0      	braw 0x3596
    34b8:	486e ffff      	pea %fp@(-1)
    34bc:	2f0a           	movel %a2,%sp@-
    34be:	4eba cc6a      	jsr %pc@(0x12a)
    34c2:	4a40           	tstw %d0
    34c4:	504f           	addqw #8,%sp
    34c6:	6706           	beqs 0x34ce
    34c8:	70ff           	moveq #-1,%d0
    34ca:	6000 00ca      	braw 0x3596
    34ce:	102a 0002      	moveb %a2@(2),%d0
    34d2:	0240 0080      	andiw #128,%d0
    34d6:	671a           	beqs 0x34f2
    34d8:	486e ffff      	pea %fp@(-1)
    34dc:	4227           	clrb %sp@-
    34de:	1f3c 003c      	moveb #60,%sp@-
    34e2:	4eba cc9c      	jsr %pc@(0x180)
    34e6:	4a40           	tstw %d0
    34e8:	504f           	addqw #8,%sp
    34ea:	6706           	beqs 0x34f2
    34ec:	70ff           	moveq #-1,%d0
    34ee:	6000 00a6      	braw 0x3596
    34f2:	102a 0002      	moveb %a2@(2),%d0
    34f6:	0240 0040      	andiw #64,%d0
    34fa:	672a           	beqs 0x3526
    34fc:	486e ffff      	pea %fp@(-1)
    3500:	4227           	clrb %sp@-
    3502:	1f3c 00cc      	moveb #-52,%sp@-
    3506:	4eba cc78      	jsr %pc@(0x180)
    350a:	4a40           	tstw %d0
    350c:	504f           	addqw #8,%sp
    350e:	6612           	bnes 0x3522
    3510:	486e ffff      	pea %fp@(-1)
    3514:	1f3c 0004      	moveb #4,%sp@-
    3518:	4eba ccdc      	jsr %pc@(0x1f6)
    351c:	5340           	subqw #1,%d0
    351e:	5c4f           	addqw #6,%sp
    3520:	6704           	beqs 0x3526
    3522:	70ff           	moveq #-1,%d0
    3524:	6070           	bras 0x3596
    3526:	7000           	moveq #0,%d0
    3528:	102a 0002      	moveb %a2@(2),%d0
    352c:	3600           	movew %d0,%d3
    352e:	0240 0010      	andiw #16,%d0
    3532:	6608           	bnes 0x353c
    3534:	3003           	movew %d3,%d0
    3536:	0240 0020      	andiw #32,%d0
    353a:	662a           	bnes 0x3566
    353c:	486e ffff      	pea %fp@(-1)
    3540:	4227           	clrb %sp@-
    3542:	1f3c 00c3      	moveb #-61,%sp@-
    3546:	4eba cc38      	jsr %pc@(0x180)
    354a:	4a40           	tstw %d0
    354c:	504f           	addqw #8,%sp
    354e:	6612           	bnes 0x3562
    3550:	486e ffff      	pea %fp@(-1)
    3554:	1f3c 0004      	moveb #4,%sp@-
    3558:	4eba cc9c      	jsr %pc@(0x1f6)
    355c:	5340           	subqw #1,%d0
    355e:	5c4f           	addqw #6,%sp
    3560:	6704           	beqs 0x3566
    3562:	70ff           	moveq #-1,%d0
    3564:	6030           	bras 0x3596
    3566:	486e ffff      	pea %fp@(-1)
    356a:	2f0a           	movel %a2,%sp@-
    356c:	4eba cbbc      	jsr %pc@(0x12a)
    3570:	4a40           	tstw %d0
    3572:	504f           	addqw #8,%sp
    3574:	6704           	beqs 0x357a
    3576:	70ff           	moveq #-1,%d0
    3578:	601c           	bras 0x3596
    357a:	7000           	moveq #0,%d0
    357c:	102a 0002      	moveb %a2@(2),%d0
    3580:	3600           	movew %d0,%d3
    3582:	0240 00d0      	andiw #208,%d0
    3586:	660c           	bnes 0x3594
    3588:	3003           	movew %d3,%d0
    358a:	0240 0020      	andiw #32,%d0
    358e:	6704           	beqs 0x3594
    3590:	7000           	moveq #0,%d0
    3592:	6002           	bras 0x3596
    3594:	70ff           	moveq #-1,%d0
    3596:	261f           	movel %sp@+,%d3
    3598:	245f           	moveal %sp@+,%a2
    359a:	4e5e           	unlk %fp
    359c:	4e75           	rts

```

# PrvShouldWeInstall: 0x35a8 - 0x35b0


```

    35a8:	4e56 0000      	linkw %fp,#0
    35ac:	7000           	moveq #0,%d0
    35ae:	4e5e           	unlk %fp
    35b0:	4e75           	rts

```

# MemoryStickTimerProc: 0x35c8 - 0x35d4


```

    35c8:	4e56 0000      	linkw %fp,#0
    35cc:	4227           	clrb %sp@-
    35ce:	4eba ee96      	jsr %pc@(0x2466)
    35d2:	4e5e           	unlk %fp
    35d4:	4e75           	rts

```

# #MemoryStickInsertedNotificationProc: 0x35ee - 0x3616


```

    35ee:	4e56 0000      	linkw %fp,#0
    35f2:	2f0b           	movel %a3,%sp@-
    35f4:	222e 0008      	movel %fp@(8),%d1
    35f8:	2041           	moveal %d1,%a0
    35fa:	2668 000c      	moveal %a0@(12),%a3
    35fe:	224b           	moveal %a3,%a1
    3600:	3028 000a      	movew %a0@(10),%d0
    3604:	b06b 4374      	cmpw %a3@(17268),%d0
    3608:	6606           	bnes 0x3610
    360a:	0229 00fd 437a 	andib #-3,%a1@(17274)
    3610:	7000           	moveq #0,%d0
    3612:	265f           	moveal %sp@+,%a3
    3614:	4e5e           	unlk %fp
    3616:	4e75           	rts

```

# "MemoryStickRemovedNotificationProc: 0x3640 - 0x3668


```

    3640:	4e56 0000      	linkw %fp,#0
    3644:	2f0b           	movel %a3,%sp@-
    3646:	222e 0008      	movel %fp@(8),%d1
    364a:	2041           	moveal %d1,%a0
    364c:	2668 000c      	moveal %a0@(12),%a3
    3650:	224b           	moveal %a3,%a1
    3652:	3028 000a      	movew %a0@(10),%d0
    3656:	b06b 4374      	cmpw %a3@(17268),%d0
    365a:	6606           	bnes 0x3662
    365c:	0229 00fb 437a 	andib #-5,%a1@(17274)
    3662:	7000           	moveq #0,%d0
    3664:	265f           	moveal %sp@+,%a3
    3666:	4e5e           	unlk %fp
    3668:	4e75           	rts

```

# SlotOpen: 0x3690 - 0x3998


```

    3690:	4e56 ffd0      	linkw %fp,#-48
    3694:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    3698:	362e 0008      	movew %fp@(8),%d3
    369c:	2d7c 536c 4d73 	movel #1399606643,%fp@(-4)
    36a2:	fffc
    36a4:	486e fff2      	pea %fp@(-14)
    36a8:	3f3c 0002      	movew #2,%sp@-
    36ac:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    36b2:	4e4f           	trap #15
    36b4:	a27b           	0121173
    36b6:	4a40           	tstw %d0
    36b8:	4fef 000a      	lea %sp@(10),%sp
    36bc:	6720           	beqs 0x36de
    36be:	41fa 02e6      	lea %pc@(0x39a6),%a0
    36c2:	4850           	pea %a0@
    36c4:	3f3c 0107      	movew #263,%sp@-
    36c8:	41fa 02ec      	lea %pc@(0x39b6),%a0
    36cc:	4850           	pea %a0@
    36ce:	4e4f           	trap #15
    36d0:	a084           	0120204
    36d2:	303c 2901      	movew #10497,%d0
    36d6:	4fef 000a      	lea %sp@(10),%sp
    36da:	6000 02b6      	braw 0x3992
    36de:	7001           	moveq #1,%d0
    36e0:	b0ae fff2      	cmpl %fp@(-14),%d0
    36e4:	6720           	beqs 0x3706
    36e6:	41fa 02dc      	lea %pc@(0x39c4),%a0
    36ea:	4850           	pea %a0@
    36ec:	3f3c 010b      	movew #267,%sp@-
    36f0:	41fa 02e2      	lea %pc@(0x39d4),%a0
    36f4:	4850           	pea %a0@
    36f6:	4e4f           	trap #15
    36f8:	a084           	0120204
    36fa:	303c 2901      	movew #10497,%d0
    36fe:	4fef 000a      	lea %sp@(10),%sp
    3702:	6000 028e      	braw 0x3992
    3706:	3f03           	movew %d3,%sp@-
    3708:	4e4f           	trap #15
    370a:	a0b9           	0120271
    370c:	2468 0004      	moveal %a0@(4),%a2
    3710:	200a           	movel %a2,%d0
    3712:	544f           	addqw #2,%sp
    3714:	6600 0278      	bnew 0x398e
    3718:	4878 4388      	pea 0x4388
    371c:	4e4f           	trap #15
    371e:	a013           	0120023
    3720:	2448           	moveal %a0,%a2
    3722:	200a           	movel %a2,%d0
    3724:	584f           	addqw #4,%sp
    3726:	6608           	bnes 0x3730
    3728:	303c 0102      	movew #258,%d0
    372c:	6000 0264      	braw 0x3992
    3730:	4267           	clrw %sp@-
    3732:	2f0a           	movel %a2,%sp@-
    3734:	4e4f           	trap #15
    3736:	a01b           	0120033
    3738:	4227           	clrb %sp@-
    373a:	4878 4388      	pea 0x4388
    373e:	2f0a           	movel %a2,%sp@-
    3740:	4e4f           	trap #15
    3742:	a027           	0120047
    3744:	3f03           	movew %d3,%sp@-
    3746:	4e4f           	trap #15
    3748:	a0b9           	0120271
    374a:	214a 0004      	movel %a2,%a0@(4)
    374e:	486a 0014      	pea %a2@(20)
    3752:	2f3c 0000 80c0 	movel #32960,%sp@-
    3758:	3f3c 0001      	movew #1,%sp@-
    375c:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    3762:	4e4f           	trap #15
    3764:	a35a           	mov3ql #1,%a2@+
    3766:	4aaa 0014      	tstl %a2@(20)
    376a:	4fef 0020      	lea %sp@(32),%sp
    376e:	661a           	bnes 0x378a
    3770:	3f03           	movew %d3,%sp@-
    3772:	4e4f           	trap #15
    3774:	a0b9           	0120271
    3776:	42a8 0004      	clrl %a0@(4)
    377a:	2f0a           	movel %a2,%sp@-
    377c:	4e4f           	trap #15
    377e:	a012           	0120022
    3780:	303c 0102      	movew #258,%d0
    3784:	5c4f           	addqw #6,%sp
    3786:	6000 020a      	braw 0x3992
    378a:	2078 0364      	moveal 0x364,%a0
    378e:	7007           	moveq #7,%d0
    3790:	c0e8 0002      	muluw %a0@(2),%d0
    3794:	5a40           	addqw #5,%d0
    3796:	7200           	moveq #0,%d1
    3798:	3200           	movew %d0,%d1
    379a:	82fc 000a      	divuw #10,%d1
    379e:	3541 4386      	movew %d1,%a2@(17286)
    37a2:	0238 00df f431 	andib #-33,0xfffffffffffff431
    37a8:	4878 0014      	pea 0x14
    37ac:	4e4f           	trap #15
    37ae:	a249 0038      	macw %a1l,%a1l
    37b2:	0020 f431      	orib #49,%a0@-
    37b6:	486e fff1      	pea %fp@(-15)
    37ba:	4eba eb38      	jsr %pc@(0x22f4)
    37be:	42a7           	clrl %sp@-
    37c0:	42a7           	clrl %sp@-
    37c2:	487a fe04      	pea %pc@(0x35c8)
    37c6:	486e fffc      	pea %fp@(-4)
    37ca:	486a 0010      	pea %a2@(16)
    37ce:	4e4f           	trap #15
    37d0:	a099 3800      	macl %d0,%d3,%a1@+,%d0
    37d4:	4fef 001c      	lea %sp@(28),%sp
    37d8:	6728           	beqs 0x3802
    37da:	3f3c 0001      	movew #1,%sp@-
    37de:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    37e4:	4e4f           	trap #15
    37e6:	a35b           	mov3ql #1,%a3@+
    37e8:	3f03           	movew %d3,%sp@-
    37ea:	4e4f           	trap #15
    37ec:	a0b9           	0120271
    37ee:	42a8 0004      	clrl %a0@(4)
    37f2:	2f0a           	movel %a2,%sp@-
    37f4:	4e4f           	trap #15
    37f6:	a012           	0120022
    37f8:	3004           	movew %d4,%d0
    37fa:	4fef 000c      	lea %sp@(12),%sp
    37fe:	6000 0192      	braw 0x3992
    3802:	4ab8 0304      	tstl 0x304
    3806:	665e           	bnes 0x3866
    3808:	4878 0078      	pea 0x78
    380c:	4e4f           	trap #15
    380e:	a013           	0120023
    3810:	21c8 0304      	movel %a0,0x304
    3814:	584f           	addqw #4,%sp
    3816:	6632           	bnes 0x384a
    3818:	2f2a 0010      	movel %a2@(16),%sp@-
    381c:	4e4f           	trap #15
    381e:	a0bf           	0120277
    3820:	3f3c 0001      	movew #1,%sp@-
    3824:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    382a:	4e4f           	trap #15
    382c:	a35b           	mov3ql #1,%a3@+
    382e:	3f03           	movew %d3,%sp@-
    3830:	4e4f           	trap #15
    3832:	a0b9           	0120271
    3834:	42a8 0004      	clrl %a0@(4)
    3838:	2f0a           	movel %a2,%sp@-
    383a:	4e4f           	trap #15
    383c:	a012           	0120022
    383e:	303c 0102      	movew #258,%d0
    3842:	4fef 0010      	lea %sp@(16),%sp
    3846:	6000 014a      	braw 0x3992
    384a:	4267           	clrw %sp@-
    384c:	2f38 0304      	movel 0x304,%sp@-
    3850:	4e4f           	trap #15
    3852:	a01b           	0120033
    3854:	4227           	clrb %sp@-
    3856:	4878 0078      	pea 0x78
    385a:	2f38 0304      	movel 0x304,%sp@-
    385e:	4e4f           	trap #15
    3860:	a027           	0120047
    3862:	4fef 0010      	lea %sp@(16),%sp
    3866:	2078 0304      	moveal 0x304,%a0
    386a:	214a 0024      	movel %a2,%a0@(36)
    386e:	2078 0304      	moveal 0x304,%a0
    3872:	2668 0010      	moveal %a0@(16),%a3
    3876:	200b           	movel %a3,%d0
    3878:	6706           	beqs 0x3880
    387a:	377c 0001 0024 	movew #1,%a3@(36)
    3880:	486e fffc      	pea %fp@(-4)
    3884:	486a 437e      	pea %a2@(17278)
    3888:	4e4f           	trap #15
    388a:	a0ae 3800 504f 	macl %d0,%d3,%fp@(20559),%d0
    3890:	6738           	beqs 0x38ca
    3892:	2078 0304      	moveal 0x304,%a0
    3896:	42a8 0024      	clrl %a0@(36)
    389a:	2f2a 0010      	movel %a2@(16),%sp@-
    389e:	4e4f           	trap #15
    38a0:	a0bf           	0120277
    38a2:	3f3c 0001      	movew #1,%sp@-
    38a6:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    38ac:	4e4f           	trap #15
    38ae:	a35b           	mov3ql #1,%a3@+
    38b0:	3f03           	movew %d3,%sp@-
    38b2:	4e4f           	trap #15
    38b4:	a0b9           	0120271
    38b6:	42a8 0004      	clrl %a0@(4)
    38ba:	2f0a           	movel %a2,%sp@-
    38bc:	4e4f           	trap #15
    38be:	a012           	0120022
    38c0:	3004           	movew %d4,%d0
    38c2:	4fef 0010      	lea %sp@(16),%sp
    38c6:	6000 00ca      	braw 0x3992
    38ca:	486a 4374      	pea %a2@(17268)
    38ce:	3f03           	movew %d3,%sp@-
    38d0:	7404           	moveq #4,%d2
    38d2:	4e4f           	trap #15
    38d4:	a347           	mov3ql #1,%d7
    38d6:	486e fff6      	pea %fp@(-10)
    38da:	486e fffa      	pea %fp@(-6)
    38de:	1f3c 0001      	moveb #1,%sp@-
    38e2:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    38e8:	2f3c 6c69 6273 	movel #1818845811,%sp@-
    38ee:	486e ffd0      	pea %fp@(-48)
    38f2:	1f3c 0001      	moveb #1,%sp@-
    38f6:	4e4f           	trap #15
    38f8:	a078           	0120170
    38fa:	2f0a           	movel %a2,%sp@-
    38fc:	4227           	clrb %sp@-
    38fe:	487a fcee      	pea %pc@(0x35ee)
    3902:	2f3c 6372 6469 	movel #1668441193,%sp@-
    3908:	2f2e fff6      	movel %fp@(-10),%sp@-
    390c:	3f2e fffa      	movew %fp@(-6),%sp@-
    3910:	4e4f           	trap #15
    3912:	a353           	mov3ql #1,%a3@
    3914:	2f0a           	movel %a2,%sp@-
    3916:	4227           	clrb %sp@-
    3918:	487a fd26      	pea %pc@(0x3640)
    391c:	2f3c 6372 646f 	movel #1668441199,%sp@-
    3922:	2f2e fff6      	movel %fp@(-10),%sp@-
    3926:	3f2e fffa      	movew %fp@(-6),%sp@-
    392a:	4e4f           	trap #15
    392c:	a353           	mov3ql #1,%a3@
    392e:	41fa 10ce      	lea %pc@(0x49fe),%a0
    3932:	2548 0008      	movel %a0,%a2@(8)
    3936:	41fa 10c6      	lea %pc@(0x49fe),%a0
    393a:	2278 0304      	moveal 0x304,%a1
    393e:	2348 004c      	movel %a0,%a1@(76)
    3942:	41fa 11ae      	lea %pc@(0x4af2),%a0
    3946:	2548 000c      	movel %a0,%a2@(12)
    394a:	0238 00f7 f41d 	andib #-9,0xfffffffffffff41d
    3950:	0078 0800 f306 	oriw #2048,0xfffffffffffff306
    3956:	0238 00f7 f418 	andib #-9,0xfffffffffffff418
    395c:	0038 0008 f41a 	orib #8,0xfffffffffffff41a
    3962:	0238 00f7 f41f 	andib #-9,0xfffffffffffff41f
    3968:	0038 0008 f41c 	orib #8,0xfffffffffffff41c
    396e:	1038 f419      	moveb 0xfffffffffffff419,%d0
    3972:	0200 fff0      	andib #-16,%d0
    3976:	0000 0008      	orib #8,%d0
    397a:	11c0 f419      	moveb %d0,0xfffffffffffff419
    397e:	0038 0008 f41d 	orib #8,0xfffffffffffff41d
    3984:	0278 f7ff f306 	andiw #-2049,0xfffffffffffff306
    398a:	4fef 0046      	lea %sp@(70),%sp
    398e:	5292           	addql #1,%a2@
    3990:	7000           	moveq #0,%d0
    3992:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
    3996:	4e5e           	unlk %fp
    3998:	4e75           	rts

```

# SlotClose: 0x39e2 - 0x3acc


```

    39e2:	4e56 ffda      	linkw %fp,#-38
    39e6:	2f0a           	movel %a2,%sp@-
    39e8:	3f2e 0008      	movew %fp@(8),%sp@-
    39ec:	4e4f           	trap #15
    39ee:	a0b9           	0120271
    39f0:	2468 0004      	moveal %a0@(4),%a2
    39f4:	200a           	movel %a2,%d0
    39f6:	544f           	addqw #2,%sp
    39f8:	6704           	beqs 0x39fe
    39fa:	4a92           	tstl %a2@
    39fc:	6608           	bnes 0x3a06
    39fe:	303c 290a      	movew #10506,%d0
    3a02:	6000 00c4      	braw 0x3ac8
    3a06:	5392           	subql #1,%a2@
    3a08:	6708           	beqs 0x3a12
    3a0a:	303c 290b      	movew #10507,%d0
    3a0e:	6000 00b8      	braw 0x3ac8
    3a12:	4a92           	tstl %a2@
    3a14:	6600 00b0      	bnew 0x3ac6
    3a18:	0238 00f7 f41d 	andib #-9,0xfffffffffffff41d
    3a1e:	0078 0800 f306 	oriw #2048,0xfffffffffffff306
    3a24:	486e fffa      	pea %fp@(-6)
    3a28:	486e fffe      	pea %fp@(-2)
    3a2c:	1f3c 0001      	moveb #1,%sp@-
    3a30:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    3a36:	2f3c 6c69 6273 	movel #1818845811,%sp@-
    3a3c:	486e ffda      	pea %fp@(-38)
    3a40:	1f3c 0001      	moveb #1,%sp@-
    3a44:	4e4f           	trap #15
    3a46:	a078           	0120170
    3a48:	4227           	clrb %sp@-
    3a4a:	2f3c 6372 6469 	movel #1668441193,%sp@-
    3a50:	2f2e fffa      	movel %fp@(-6),%sp@-
    3a54:	3f2e fffe      	movew %fp@(-2),%sp@-
    3a58:	4e4f           	trap #15
    3a5a:	a354           	mov3ql #1,%a4@
    3a5c:	4227           	clrb %sp@-
    3a5e:	2f3c 6372 646f 	movel #1668441199,%sp@-
    3a64:	2f2e fffa      	movel %fp@(-6),%sp@-
    3a68:	3f2e fffe      	movew %fp@(-2),%sp@-
    3a6c:	4e4f           	trap #15
    3a6e:	a354           	mov3ql #1,%a4@
    3a70:	3f2a 4374      	movew %a2@(17268),%sp@-
    3a74:	7405           	moveq #5,%d2
    3a76:	4e4f           	trap #15
    3a78:	a347           	mov3ql #1,%d7
    3a7a:	2f2a 437e      	movel %a2@(17278),%sp@-
    3a7e:	4e4f           	trap #15
    3a80:	a0af           	0120257
    3a82:	2078 0304      	moveal 0x304,%a0
    3a86:	42a8 0024      	clrl %a0@(36)
    3a8a:	42a7           	clrl %sp@-
    3a8c:	2f2a 0010      	movel %a2@(16),%sp@-
    3a90:	4e4f           	trap #15
    3a92:	a09a 2f2a      	msacml %a2,%d2,>>,%a2@+,%d0
    3a96:	0010 4e4f      	orib #79,%a0@
    3a9a:	a0bf           	0120277
    3a9c:	4227           	clrb %sp@-
    3a9e:	4eba e9c6      	jsr %pc@(0x2466)
    3aa2:	3f3c 0001      	movew #1,%sp@-
    3aa6:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    3aac:	4e4f           	trap #15
    3aae:	a35b           	mov3ql #1,%a3@+
    3ab0:	3f2e 0008      	movew %fp@(8),%sp@-
    3ab4:	4e4f           	trap #15
    3ab6:	a0b9           	0120271
    3ab8:	42a8 0004      	clrl %a0@(4)
    3abc:	2f0a           	movel %a2,%sp@-
    3abe:	4e4f           	trap #15
    3ac0:	a012           	0120022
    3ac2:	4fef 0050      	lea %sp@(80),%sp
    3ac6:	7000           	moveq #0,%d0
    3ac8:	245f           	moveal %sp@+,%a2
    3aca:	4e5e           	unlk %fp
    3acc:	4e75           	rts

```

# SlotSleep: 0x3ada - 0x3b30


```

    3ada:	4e56 0000      	linkw %fp,#0
    3ade:	2f0a           	movel %a2,%sp@-
    3ae0:	3f2e 0008      	movew %fp@(8),%sp@-
    3ae4:	4e4f           	trap #15
    3ae6:	a0b9           	0120271
    3ae8:	2468 0004      	moveal %a0@(4),%a2
    3aec:	200a           	movel %a2,%d0
    3aee:	544f           	addqw #2,%sp
    3af0:	6606           	bnes 0x3af8
    3af2:	303c 290a      	movew #10506,%d0
    3af6:	6034           	bras 0x3b2c
    3af8:	0238 00df f431 	andib #-33,0xfffffffffffff431
    3afe:	42a7           	clrl %sp@-
    3b00:	2f2a 0010      	movel %a2@(16),%sp@-
    3b04:	4e4f           	trap #15
    3b06:	a09a           	0120232
    3b08:	0c38 0001 02ca 	cmpib #1,0x2ca
    3b0e:	504f           	addqw #8,%sp
    3b10:	6610           	bnes 0x3b22
    3b12:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3b16:	0240 0008      	andiw #8,%d0
    3b1a:	6606           	bnes 0x3b22
    3b1c:	002a 0008 437a 	orib #8,%a2@(17274)
    3b22:	4227           	clrb %sp@-
    3b24:	4eba e940      	jsr %pc@(0x2466)
    3b28:	7000           	moveq #0,%d0
    3b2a:	544f           	addqw #2,%sp
    3b2c:	245f           	moveal %sp@+,%a2
    3b2e:	4e5e           	unlk %fp
    3b30:	4e75           	rts

```

# SlotWake: 0x3b3e - 0x3bd0


```

    3b3e:	4e56 fffe      	linkw %fp,#-2
    3b42:	2f0a           	movel %a2,%sp@-
    3b44:	3f2e 0008      	movew %fp@(8),%sp@-
    3b48:	4e4f           	trap #15
    3b4a:	a0b9           	0120271
    3b4c:	2468 0004      	moveal %a0@(4),%a2
    3b50:	200a           	movel %a2,%d0
    3b52:	544f           	addqw #2,%sp
    3b54:	6606           	bnes 0x3b5c
    3b56:	303c 290a      	movew #10506,%d0
    3b5a:	6070           	bras 0x3bcc
    3b5c:	0038 0020 f431 	orib #32,0xfffffffffffff431
    3b62:	486e ffff      	pea %fp@(-1)
    3b66:	4eba e78c      	jsr %pc@(0x22f4)
    3b6a:	102a 437a      	moveb %a2@(17274),%d0
    3b6e:	0240 0008      	andiw #8,%d0
    3b72:	584f           	addqw #4,%sp
    3b74:	6722           	beqs 0x3b98
    3b76:	022a 00f7 437a 	andib #-9,%a2@(17274)
    3b7c:	0238 00f7 f41c 	andib #-9,0xfffffffffffff41c
    3b82:	1038 f419      	moveb 0xfffffffffffff419,%d0
    3b86:	0240 0008      	andiw #8,%d0
    3b8a:	660c           	bnes 0x3b98
    3b8c:	002a 0006 437a 	orib #6,%a2@(17274)
    3b92:	022a 00fe 437a 	andib #-2,%a2@(17274)
    3b98:	102a 437a      	moveb %a2@(17274),%d0
    3b9c:	0240 0004      	andiw #4,%d0
    3ba0:	670c           	beqs 0x3bae
    3ba2:	3f2a 4374      	movew %a2@(17268),%sp@-
    3ba6:	7407           	moveq #7,%d2
    3ba8:	4e4f           	trap #15
    3baa:	a347           	mov3ql #1,%d7
    3bac:	544f           	addqw #2,%sp
    3bae:	102a 437a      	moveb %a2@(17274),%d0
    3bb2:	0240 0002      	andiw #2,%d0
    3bb6:	670c           	beqs 0x3bc4
    3bb8:	3f2a 4374      	movew %a2@(17268),%sp@-
    3bbc:	7406           	moveq #6,%d2
    3bbe:	4e4f           	trap #15
    3bc0:	a347           	mov3ql #1,%d7
    3bc2:	544f           	addqw #2,%sp
    3bc4:	0278 f7ff f306 	andiw #-2049,0xfffffffffffff306
    3bca:	7000           	moveq #0,%d0
    3bcc:	245f           	moveal %sp@+,%a2
    3bce:	4e5e           	unlk %fp
    3bd0:	4e75           	rts

```

# SlotLibAPIVersion: 0x3bde - 0x3be6


```

    3bde:	4e56 0000      	linkw %fp,#0
    3be2:	7002           	moveq #2,%d0
    3be4:	4e5e           	unlk %fp
    3be6:	4e75           	rts

```

# SlotCustomControl: 0x3bfc - 0x3dd6


```

    3bfc:	4e56 0000      	linkw %fp,#0
    3c00:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    3c04:	266e 0010      	moveal %fp@(16),%a3
    3c08:	3f2e 0008      	movew %fp@(8),%sp@-
    3c0c:	4e4f           	trap #15
    3c0e:	a0b9           	0120271
    3c10:	2468 0004      	moveal %a0@(4),%a2
    3c14:	200a           	movel %a2,%d0
    3c16:	544f           	addqw #2,%sp
    3c18:	6608           	bnes 0x3c22
    3c1a:	303c 290a      	movew #10506,%d0
    3c1e:	6000 01b0      	braw 0x3dd0
    3c22:	0cae 536f 4e79 	cmpil #1399803513,%fp@(10)
    3c28:	000a
    3c2a:	6708           	beqs 0x3c34
    3c2c:	303c 2901      	movew #10497,%d0
    3c30:	6000 019e      	braw 0x3dd0
    3c34:	302e 000e      	movew %fp@(14),%d0
    3c38:	0440 2000      	subiw #8192,%d0
    3c3c:	0c40 0006      	cmpiw #6,%d0
    3c40:	6200 0188      	bhiw 0x3dca
    3c44:	d040           	addw %d0,%d0
    3c46:	303b 0006      	movew %pc@(0x3c4e,%d0:w),%d0
    3c4a:	4efb 0002      	jmp %pc@(0x3c4e,%d0:w)
    3c4e:	000e 0018      	orib #24,%fp
    3c52:	0074 00c8 00e8 	oriw #200,%a4@(ffffffffffffffe8,%d0:w)
    3c58:	011e           	btst %d0,%fp@+
    3c5a:	0152           	bchg %d0,%a2@
    3c5c:	7001           	moveq #1,%d0
    3c5e:	2680           	movel %d0,%a3@
    3c60:	7600           	moveq #0,%d3
    3c62:	6000 016a      	braw 0x3dce
    3c66:	302a 4374      	movew %a2@(17268),%d0
    3c6a:	b053           	cmpw %a3@,%d0
    3c6c:	6708           	beqs 0x3c76
    3c6e:	363c 2904      	movew #10500,%d3
    3c72:	6000 015a      	braw 0x3dce
    3c76:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3c7a:	0240 0008      	andiw #8,%d0
    3c7e:	6708           	beqs 0x3c88
    3c80:	363c 2903      	movew #10499,%d3
    3c84:	6000 0148      	braw 0x3dce
    3c88:	102a 437a      	moveb %a2@(17274),%d0
    3c8c:	0240 0004      	andiw #4,%d0
    3c90:	6708           	beqs 0x3c9a
    3c92:	363c 2903      	movew #10499,%d3
    3c96:	6000 0136      	braw 0x3dce
    3c9a:	7077           	moveq #119,%d0
    3c9c:	c0aa 4382      	andl %a2@(17282),%d0
    3ca0:	2740 0002      	movel %d0,%a3@(2)
    3ca4:	276a 430c 0006 	movel %a2@(17164),%a3@(6)
    3caa:	276a 4304 000a 	movel %a2@(17156),%a3@(10)
    3cb0:	203c 0000 4304 	movel #17156,%d0
    3cb6:	2772 0804 000e 	movel %a2@(00000004,%d0:l),%a3@(14)
    3cbc:	7600           	moveq #0,%d3
    3cbe:	6000 010e      	braw 0x3dce
    3cc2:	302a 4374      	movew %a2@(17268),%d0
    3cc6:	b053           	cmpw %a3@,%d0
    3cc8:	6708           	beqs 0x3cd2
    3cca:	363c 2904      	movew #10500,%d3
    3cce:	6000 00fe      	braw 0x3dce
    3cd2:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3cd6:	0240 0008      	andiw #8,%d0
    3cda:	6708           	beqs 0x3ce4
    3cdc:	363c 2903      	movew #10499,%d3
    3ce0:	6000 00ec      	braw 0x3dce
    3ce4:	102a 437a      	moveb %a2@(17274),%d0
    3ce8:	0240 0004      	andiw #4,%d0
    3cec:	6708           	beqs 0x3cf6
    3cee:	363c 2903      	movew #10499,%d3
    3cf2:	6000 00da      	braw 0x3dce
    3cf6:	7040           	moveq #64,%d0
    3cf8:	c0aa 4382      	andl %a2@(17282),%d0
    3cfc:	6608           	bnes 0x3d06
    3cfe:	363c 2906      	movew #10502,%d3
    3d02:	6000 00ca      	braw 0x3dce
    3d06:	204a           	moveal %a2,%a0
    3d08:	41e8 4158      	lea %a0@(16728),%a0
    3d0c:	2748 0002      	movel %a0,%a3@(2)
    3d10:	7600           	moveq #0,%d3
    3d12:	6000 00ba      	braw 0x3dce
    3d16:	2f2b 000a      	movel %a3@(10),%sp@-
    3d1a:	2f2b 0006      	movel %a3@(6),%sp@-
    3d1e:	2f2b 0002      	movel %a3@(2),%sp@-
    3d22:	3f13           	movew %a3@,%sp@-
    3d24:	3f2e 0008      	movew %fp@(8),%sp@-
    3d28:	4eba 08f6      	jsr %pc@(0x4620)
    3d2c:	3600           	movew %d0,%d3
    3d2e:	4fef 0010      	lea %sp@(16),%sp
    3d32:	6000 009a      	braw 0x3dce
    3d36:	7000           	moveq #0,%d0
    3d38:	302a 4374      	movew %a2@(17268),%d0
    3d3c:	b093           	cmpl %a3@,%d0
    3d3e:	6708           	beqs 0x3d48
    3d40:	363c 2904      	movew #10500,%d3
    3d44:	6000 0088      	braw 0x3dce
    3d48:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3d4c:	0240 0008      	andiw #8,%d0
    3d50:	6706           	beqs 0x3d58
    3d52:	363c 2903      	movew #10499,%d3
    3d56:	6076           	bras 0x3dce
    3d58:	102a 437a      	moveb %a2@(17274),%d0
    3d5c:	0240 0004      	andiw #4,%d0
    3d60:	6706           	beqs 0x3d68
    3d62:	363c 2903      	movew #10499,%d3
    3d66:	6066           	bras 0x3dce
    3d68:	7600           	moveq #0,%d3
    3d6a:	6062           	bras 0x3dce
    3d6c:	7000           	moveq #0,%d0
    3d6e:	302a 4374      	movew %a2@(17268),%d0
    3d72:	b093           	cmpl %a3@,%d0
    3d74:	6706           	beqs 0x3d7c
    3d76:	363c 2904      	movew #10500,%d3
    3d7a:	6052           	bras 0x3dce
    3d7c:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3d80:	0240 0008      	andiw #8,%d0
    3d84:	6706           	beqs 0x3d8c
    3d86:	363c 2903      	movew #10499,%d3
    3d8a:	6042           	bras 0x3dce
    3d8c:	102a 437a      	moveb %a2@(17274),%d0
    3d90:	0240 0004      	andiw #4,%d0
    3d94:	6706           	beqs 0x3d9c
    3d96:	363c 2903      	movew #10499,%d3
    3d9a:	6032           	bras 0x3dce
    3d9c:	7600           	moveq #0,%d3
    3d9e:	602e           	bras 0x3dce
    3da0:	362a 4374      	movew %a2@(17268),%d3
    3da4:	7000           	moveq #0,%d0
    3da6:	3003           	movew %d3,%d0
    3da8:	b093           	cmpl %a3@,%d0
    3daa:	6706           	beqs 0x3db2
    3dac:	303c 2904      	movew #10500,%d0
    3db0:	601e           	bras 0x3dd0
    3db2:	3f03           	movew %d3,%sp@-
    3db4:	7407           	moveq #7,%d2
    3db6:	4e4f           	trap #15
    3db8:	a347           	mov3ql #1,%d7
    3dba:	3f2a 4374      	movew %a2@(17268),%sp@-
    3dbe:	7406           	moveq #6,%d2
    3dc0:	4e4f           	trap #15
    3dc2:	a347           	mov3ql #1,%d7
    3dc4:	7600           	moveq #0,%d3
    3dc6:	584f           	addqw #4,%sp
    3dc8:	6004           	bras 0x3dce
    3dca:	363c 2901      	movew #10497,%d3
    3dce:	3003           	movew %d3,%d0
    3dd0:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
    3dd4:	4e5e           	unlk %fp
    3dd6:	4e75           	rts

```

# SlotCardPresent: 0x3dec - 0x3e3e


```

    3dec:	4e56 0000      	linkw %fp,#0
    3df0:	2f0a           	movel %a2,%sp@-
    3df2:	3f2e 0008      	movew %fp@(8),%sp@-
    3df6:	4e4f           	trap #15
    3df8:	a0b9           	0120271
    3dfa:	2468 0004      	moveal %a0@(4),%a2
    3dfe:	200a           	movel %a2,%d0
    3e00:	544f           	addqw #2,%sp
    3e02:	6606           	bnes 0x3e0a
    3e04:	303c 290a      	movew #10506,%d0
    3e08:	6030           	bras 0x3e3a
    3e0a:	302a 4374      	movew %a2@(17268),%d0
    3e0e:	b06e 000a      	cmpw %fp@(10),%d0
    3e12:	6706           	beqs 0x3e1a
    3e14:	303c 2904      	movew #10500,%d0
    3e18:	6020           	bras 0x3e3a
    3e1a:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3e1e:	0240 0008      	andiw #8,%d0
    3e22:	6706           	beqs 0x3e2a
    3e24:	303c 2903      	movew #10499,%d0
    3e28:	6010           	bras 0x3e3a
    3e2a:	082a 0002 437a 	btst #2,%a2@(17274)
    3e30:	6706           	beqs 0x3e38
    3e32:	303c 2903      	movew #10499,%d0
    3e36:	6002           	bras 0x3e3a
    3e38:	7000           	moveq #0,%d0
    3e3a:	245f           	moveal %sp@+,%a2
    3e3c:	4e5e           	unlk %fp
    3e3e:	4e75           	rts

```

# MSSlotCardInfo: 0x3e52 - 0x4058


```

    3e52:	4e56 fffe      	linkw %fp,#-2
    3e56:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    3e5a:	266e 000c      	moveal %fp@(12),%a3
    3e5e:	3f2e 0008      	movew %fp@(8),%sp@-
    3e62:	4e4f           	trap #15
    3e64:	a0b9           	0120271
    3e66:	2468 0004      	moveal %a0@(4),%a2
    3e6a:	200a           	movel %a2,%d0
    3e6c:	544f           	addqw #2,%sp
    3e6e:	6608           	bnes 0x3e78
    3e70:	303c 290a      	movew #10506,%d0
    3e74:	6000 01dc      	braw 0x4052
    3e78:	302a 4374      	movew %a2@(17268),%d0
    3e7c:	b06e 000a      	cmpw %fp@(10),%d0
    3e80:	6708           	beqs 0x3e8a
    3e82:	303c 2904      	movew #10500,%d0
    3e86:	6000 01ca      	braw 0x4052
    3e8a:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3e8e:	0240 0008      	andiw #8,%d0
    3e92:	6708           	beqs 0x3e9c
    3e94:	303c 2903      	movew #10499,%d0
    3e98:	6000 01b8      	braw 0x4052
    3e9c:	102a 437a      	moveb %a2@(17274),%d0
    3ea0:	0240 0004      	andiw #4,%d0
    3ea4:	6708           	beqs 0x3eae
    3ea6:	303c 2903      	movew #10499,%d0
    3eaa:	6000 01a6      	braw 0x4052
    3eae:	42a7           	clrl %sp@-
    3eb0:	42a7           	clrl %sp@-
    3eb2:	2f2a 437e      	movel %a2@(17278),%sp@-
    3eb6:	4e4f           	trap #15
    3eb8:	a0b0           	0120260
    3eba:	102a 437a      	moveb %a2@(17274),%d0
    3ebe:	0240 0001      	andiw #1,%d0
    3ec2:	4fef 000c      	lea %sp@(12),%sp
    3ec6:	6600 00d8      	bnew 0x3fa0
    3eca:	42a7           	clrl %sp@-
    3ecc:	2f2a 0010      	movel %a2@(16),%sp@-
    3ed0:	4e4f           	trap #15
    3ed2:	a09a           	0120232
    3ed4:	0238 00df f431 	andib #-33,0xfffffffffffff431
    3eda:	4878 0014      	pea 0x14
    3ede:	4e4f           	trap #15
    3ee0:	a249 0038      	macw %a1l,%a1l
    3ee4:	0020 f431      	orib #49,%a0@-
    3ee8:	486e ffff      	pea %fp@(-1)
    3eec:	4eba e406      	jsr %pc@(0x22f4)
    3ef0:	3838 fc02      	movew 0xfffffffffffffc02,%d4
    3ef4:	3004           	movew %d4,%d0
    3ef6:	0240 ff1f      	andiw #-225,%d0
    3efa:	0040 0061      	oriw #97,%d0
    3efe:	31c0 fc02      	movew %d0,0xfffffffffffffc02
    3f02:	1f3c 0001      	moveb #1,%sp@-
    3f06:	4eba e55e      	jsr %pc@(0x2466)
    3f0a:	1f3c 0001      	moveb #1,%sp@-
    3f0e:	4eba e5e2      	jsr %pc@(0x24f2)
    3f12:	1f3c 0001      	moveb #1,%sp@-
    3f16:	4eba e62e      	jsr %pc@(0x2546)
    3f1a:	2f0a           	movel %a2,%sp@-
    3f1c:	4eba c980      	jsr %pc@(0x89e)
    3f20:	2600           	movel %d0,%d3
    3f22:	4227           	clrb %sp@-
    3f24:	4eba e5cc      	jsr %pc@(0x24f2)
    3f28:	102a 437a      	moveb %a2@(17274),%d0
    3f2c:	0240 0010      	andiw #16,%d0
    3f30:	4fef 001c      	lea %sp@(28),%sp
    3f34:	6608           	bnes 0x3f3e
    3f36:	4227           	clrb %sp@-
    3f38:	4eba e60c      	jsr %pc@(0x2546)
    3f3c:	544f           	addqw #2,%sp
    3f3e:	4878 0064      	pea 0x64
    3f42:	2f2a 0010      	movel %a2@(16),%sp@-
    3f46:	4e4f           	trap #15
    3f48:	a09a 3004      	macw %d4l,%d3l,%a2@+,%d0
    3f4c:	0240 00e0      	andiw #224,%d0
    3f50:	3238 fc02      	movew 0xfffffffffffffc02,%d1
    3f54:	0241 ff1f      	andiw #-225,%d1
    3f58:	8240           	orw %d0,%d1
    3f5a:	0041 0001      	oriw #1,%d1
    3f5e:	31c1 fc02      	movew %d1,0xfffffffffffffc02
    3f62:	4a83           	tstl %d3
    3f64:	504f           	addqw #8,%sp
    3f66:	6616           	bnes 0x3f7e
    3f68:	7001           	moveq #1,%d0
    3f6a:	2540 0004      	movel %d0,%a2@(4)
    3f6e:	0c2a 0001 437b 	cmpib #1,%a2@(17275)
    3f74:	6624           	bnes 0x3f9a
    3f76:	7002           	moveq #2,%d0
    3f78:	81aa 0004      	orl %d0,%a2@(4)
    3f7c:	601c           	bras 0x3f9a
    3f7e:	7054           	moveq #84,%d0
    3f80:	b680           	cmpl %d0,%d3
    3f82:	6612           	bnes 0x3f96
    3f84:	2f2a 437e      	movel %a2@(17278),%sp@-
    3f88:	4e4f           	trap #15
    3f8a:	a0b1           	0120261
    3f8c:	303c 2903      	movew #10499,%d0
    3f90:	584f           	addqw #4,%sp
    3f92:	6000 00be      	braw 0x4052
    3f96:	42aa 0004      	clrl %a2@(4)
    3f9a:	002a 0001 437a 	orib #1,%a2@(17274)
    3fa0:	26aa 0004      	movel %a2@(4),%a3@
    3fa4:	7001           	moveq #1,%d0
    3fa6:	c0aa 0004      	andl %a2@(4),%d0
    3faa:	674e           	beqs 0x3ffa
    3fac:	3f3c 001f      	movew #31,%sp@-
    3fb0:	41fa 00ba      	lea %pc@(0x406c),%a0
    3fb4:	4850           	pea %a0@
    3fb6:	486b 0004      	pea %a3@(4)
    3fba:	4e4f           	trap #15
    3fbc:	a2ce           	0121316
    3fbe:	3f3c 001f      	movew #31,%sp@-
    3fc2:	41fa 00aa      	lea %pc@(0x406e),%a0
    3fc6:	4850           	pea %a0@
    3fc8:	486b 0024      	pea %a3@(36)
    3fcc:	4e4f           	trap #15
    3fce:	a2ce           	0121316
    3fd0:	3f3c 001f      	movew #31,%sp@-
    3fd4:	41fa 009a      	lea %pc@(0x4070),%a0
    3fd8:	4850           	pea %a0@
    3fda:	486b 0044      	pea %a3@(68)
    3fde:	4e4f           	trap #15
    3fe0:	a2ce           	0121316
    3fe2:	3f3c 001f      	movew #31,%sp@-
    3fe6:	41fa 0096      	lea %pc@(0x407e),%a0
    3fea:	4850           	pea %a0@
    3fec:	486b 0064      	pea %a3@(100)
    3ff0:	4e4f           	trap #15
    3ff2:	a2ce           	0121316
    3ff4:	4fef 0028      	lea %sp@(40),%sp
    3ff8:	604c           	bras 0x4046
    3ffa:	3f3c 001f      	movew #31,%sp@-
    3ffe:	41fa 0080      	lea %pc@(0x4080),%a0
    4002:	4850           	pea %a0@
    4004:	486b 0004      	pea %a3@(4)
    4008:	4e4f           	trap #15
    400a:	a2ce           	0121316
    400c:	3f3c 001f      	movew #31,%sp@-
    4010:	41fa 0070      	lea %pc@(0x4082),%a0
    4014:	4850           	pea %a0@
    4016:	486b 0024      	pea %a3@(36)
    401a:	4e4f           	trap #15
    401c:	a2ce           	0121316
    401e:	3f3c 001f      	movew #31,%sp@-
    4022:	41fa 0060      	lea %pc@(0x4084),%a0
    4026:	4850           	pea %a0@
    4028:	486b 0044      	pea %a3@(68)
    402c:	4e4f           	trap #15
    402e:	a2ce           	0121316
    4030:	3f3c 001f      	movew #31,%sp@-
    4034:	41fa 0050      	lea %pc@(0x4086),%a0
    4038:	4850           	pea %a0@
    403a:	486b 0064      	pea %a3@(100)
    403e:	4e4f           	trap #15
    4040:	a2ce           	0121316
    4042:	4fef 0028      	lea %sp@(40),%sp
    4046:	2f2a 437e      	movel %a2@(17278),%sp@-
    404a:	4e4f           	trap #15
    404c:	a0b1           	0120261
    404e:	7000           	moveq #0,%d0
    4050:	584f           	addqw #4,%sp
    4052:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
    4056:	4e5e           	unlk %fp
    4058:	4e75           	rts

```

# SlotCardMediaType: 0x4088 - 0x40f2


```

    4088:	4e56 0000      	linkw %fp,#0
    408c:	2f0a           	movel %a2,%sp@-
    408e:	3f2e 0008      	movew %fp@(8),%sp@-
    4092:	4e4f           	trap #15
    4094:	a0b9           	0120271
    4096:	2468 0004      	moveal %a0@(4),%a2
    409a:	200a           	movel %a2,%d0
    409c:	544f           	addqw #2,%sp
    409e:	6606           	bnes 0x40a6
    40a0:	303c 290a      	movew #10506,%d0
    40a4:	6048           	bras 0x40ee
    40a6:	302a 4374      	movew %a2@(17268),%d0
    40aa:	b06e 000a      	cmpw %fp@(10),%d0
    40ae:	6706           	beqs 0x40b6
    40b0:	303c 2904      	movew #10500,%d0
    40b4:	6038           	bras 0x40ee
    40b6:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    40ba:	0240 0008      	andiw #8,%d0
    40be:	6706           	beqs 0x40c6
    40c0:	303c 2903      	movew #10499,%d0
    40c4:	6028           	bras 0x40ee
    40c6:	102a 437a      	moveb %a2@(17274),%d0
    40ca:	0240 0004      	andiw #4,%d0
    40ce:	6706           	beqs 0x40d6
    40d0:	303c 2903      	movew #10499,%d0
    40d4:	6018           	bras 0x40ee
    40d6:	4aae 000c      	tstl %fp@(12)
    40da:	670e           	beqs 0x40ea
    40dc:	206e 000c      	moveal %fp@(12),%a0
    40e0:	20bc 6d73 746b 	movel #1836282987,%a0@
    40e6:	7000           	moveq #0,%d0
    40e8:	6004           	bras 0x40ee
    40ea:	303c 0502      	movew #1282,%d0
    40ee:	245f           	moveal %sp@+,%a2
    40f0:	4e5e           	unlk %fp
    40f2:	4e75           	rts

```

# SlotMediaType: 0x4108 - 0x4152


```

    4108:	4e56 0000      	linkw %fp,#0
    410c:	2f0a           	movel %a2,%sp@-
    410e:	3f2e 0008      	movew %fp@(8),%sp@-
    4112:	4e4f           	trap #15
    4114:	a0b9           	0120271
    4116:	2468 0004      	moveal %a0@(4),%a2
    411a:	200a           	movel %a2,%d0
    411c:	544f           	addqw #2,%sp
    411e:	6606           	bnes 0x4126
    4120:	303c 290a      	movew #10506,%d0
    4124:	6028           	bras 0x414e
    4126:	302a 4374      	movew %a2@(17268),%d0
    412a:	b06e 000a      	cmpw %fp@(10),%d0
    412e:	6706           	beqs 0x4136
    4130:	303c 2904      	movew #10500,%d0
    4134:	6018           	bras 0x414e
    4136:	4aae 000c      	tstl %fp@(12)
    413a:	670e           	beqs 0x414a
    413c:	206e 000c      	moveal %fp@(12),%a0
    4140:	20bc 6d73 746b 	movel #1836282987,%a0@
    4146:	7000           	moveq #0,%d0
    4148:	6004           	bras 0x414e
    414a:	303c 0502      	movew #1282,%d0
    414e:	245f           	moveal %sp@+,%a2
    4150:	4e5e           	unlk %fp
    4152:	4e75           	rts

```

# SlotCardReserve: 0x4164 - 0x41a8


```

    4164:	4e56 0000      	linkw %fp,#0
    4168:	2f0a           	movel %a2,%sp@-
    416a:	3f2e 0008      	movew %fp@(8),%sp@-
    416e:	4e4f           	trap #15
    4170:	a0b9           	0120271
    4172:	2468 0004      	moveal %a0@(4),%a2
    4176:	200a           	movel %a2,%d0
    4178:	544f           	addqw #2,%sp
    417a:	6606           	bnes 0x4182
    417c:	303c 290a      	movew #10506,%d0
    4180:	6022           	bras 0x41a4
    4182:	302a 4374      	movew %a2@(17268),%d0
    4186:	b06e 000a      	cmpw %fp@(10),%d0
    418a:	6706           	beqs 0x4192
    418c:	303c 2904      	movew #10500,%d0
    4190:	6012           	bras 0x41a4
    4192:	002a 0010 437a 	orib #16,%a2@(17274)
    4198:	1f3c 0001      	moveb #1,%sp@-
    419c:	4eba e3a8      	jsr %pc@(0x2546)
    41a0:	7000           	moveq #0,%d0
    41a2:	544f           	addqw #2,%sp
    41a4:	245f           	moveal %sp@+,%a2
    41a6:	4e5e           	unlk %fp
    41a8:	4e75           	rts

```

# SlotCardRelease: 0x41bc - 0x41fe


```

    41bc:	4e56 0000      	linkw %fp,#0
    41c0:	2f0a           	movel %a2,%sp@-
    41c2:	3f2e 0008      	movew %fp@(8),%sp@-
    41c6:	4e4f           	trap #15
    41c8:	a0b9           	0120271
    41ca:	2468 0004      	moveal %a0@(4),%a2
    41ce:	200a           	movel %a2,%d0
    41d0:	544f           	addqw #2,%sp
    41d2:	6606           	bnes 0x41da
    41d4:	303c 290a      	movew #10506,%d0
    41d8:	6020           	bras 0x41fa
    41da:	302a 4374      	movew %a2@(17268),%d0
    41de:	b06e 000a      	cmpw %fp@(10),%d0
    41e2:	6706           	beqs 0x41ea
    41e4:	303c 2904      	movew #10500,%d0
    41e8:	6010           	bras 0x41fa
    41ea:	022a 00ef 437a 	andib #-17,%a2@(17274)
    41f0:	4227           	clrb %sp@-
    41f2:	4eba e352      	jsr %pc@(0x2546)
    41f6:	7000           	moveq #0,%d0
    41f8:	544f           	addqw #2,%sp
    41fa:	245f           	moveal %sp@+,%a2
    41fc:	4e5e           	unlk %fp
    41fe:	4e75           	rts

```

# SlotCardGetSerialPort: 0x4212 - 0x4248


```

    4212:	4e56 0000      	linkw %fp,#0
    4216:	2f0a           	movel %a2,%sp@-
    4218:	3f2e 0008      	movew %fp@(8),%sp@-
    421c:	4e4f           	trap #15
    421e:	a0b9           	0120271
    4220:	2468 0004      	moveal %a0@(4),%a2
    4224:	200a           	movel %a2,%d0
    4226:	544f           	addqw #2,%sp
    4228:	6606           	bnes 0x4230
    422a:	303c 290a      	movew #10506,%d0
    422e:	6014           	bras 0x4244
    4230:	302a 4374      	movew %a2@(17268),%d0
    4234:	b06e 000a      	cmpw %fp@(10),%d0
    4238:	6706           	beqs 0x4240
    423a:	303c 2904      	movew #10500,%d0
    423e:	6004           	bras 0x4244
    4240:	303c 2901      	movew #10497,%d0
    4244:	245f           	moveal %sp@+,%a2
    4246:	4e5e           	unlk %fp
    4248:	4e75           	rts

```

# SlotCardIsFilesystemSupported: 0x4262 - 0x4282


```

    4262:	4e56 0000      	linkw %fp,#0
    4266:	0cae 7666 6174 	cmpil #1986421108,%fp@(12)
    426c:	000c
    426e:	670a           	beqs 0x427a
    4270:	0cae 6661 7473 	cmpil #1717662835,%fp@(12)
    4276:	000c
    4278:	6604           	bnes 0x427e
    427a:	7001           	moveq #1,%d0
    427c:	6002           	bras 0x4280
    427e:	7000           	moveq #0,%d0
    4280:	4e5e           	unlk %fp
    4282:	4e75           	rts

```

# SlotCardMetrics: 0x42a4 - 0x4464


```

    42a4:	4e56 ff7c      	linkw %fp,#-132
    42a8:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    42ac:	246e 000c      	moveal %fp@(12),%a2
    42b0:	3f2e 0008      	movew %fp@(8),%sp@-
    42b4:	4e4f           	trap #15
    42b6:	a0b9           	0120271
    42b8:	2668 0004      	moveal %a0@(4),%a3
    42bc:	200b           	movel %a3,%d0
    42be:	544f           	addqw #2,%sp
    42c0:	6608           	bnes 0x42ca
    42c2:	303c 290a      	movew #10506,%d0
    42c6:	6000 0196      	braw 0x445e
    42ca:	302b 4374      	movew %a3@(17268),%d0
    42ce:	b06e 000a      	cmpw %fp@(10),%d0
    42d2:	6708           	beqs 0x42dc
    42d4:	303c 2904      	movew #10500,%d0
    42d8:	6000 0184      	braw 0x445e
    42dc:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    42e0:	0240 0008      	andiw #8,%d0
    42e4:	6708           	beqs 0x42ee
    42e6:	303c 2903      	movew #10499,%d0
    42ea:	6000 0172      	braw 0x445e
    42ee:	102b 437a      	moveb %a3@(17274),%d0
    42f2:	0240 0001      	andiw #1,%d0
    42f6:	661c           	bnes 0x4314
    42f8:	486e ff7c      	pea %fp@(-132)
    42fc:	3f2e 000a      	movew %fp@(10),%sp@-
    4300:	3f2e 0008      	movew %fp@(8),%sp@-
    4304:	4eba fb4c      	jsr %pc@(0x3e52)
    4308:	3600           	movew %d0,%d3
    430a:	504f           	addqw #8,%sp
    430c:	6706           	beqs 0x4314
    430e:	3003           	movew %d3,%d0
    4310:	6000 014c      	braw 0x445e
    4314:	200a           	movel %a2,%d0
    4316:	6700 013e      	beqw 0x4456
    431a:	7001           	moveq #1,%d0
    431c:	c0ab 0004      	andl %a3@(4),%d0
    4320:	6700 012e      	beqw 0x4450
    4324:	357c 0200 0004 	movew #512,%a2@(4)
    432a:	357c 0010 0006 	movew #16,%a2@(6)
    4330:	157c 0080 000e 	moveb #-128,%a2@(14)
    4336:	302b 42fc      	movew %a3@(17148),%d0
    433a:	0440 0200      	subiw #512,%d0
    433e:	6722           	beqs 0x4362
    4340:	0440 0200      	subiw #512,%d0
    4344:	673e           	beqs 0x4384
    4346:	0440 0400      	subiw #1024,%d0
    434a:	6700 0092      	beqw 0x43de
    434e:	0440 0800      	subiw #2048,%d0
    4352:	6700 00aa      	beqw 0x43fe
    4356:	0440 1000      	subiw #4096,%d0
    435a:	6700 00c2      	beqw 0x441e
    435e:	6000 00de      	braw 0x443e
    4362:	24bc 0000 1ec0 	movel #7872,%a2@
    4368:	357c 0002 0008 	movew #2,%a2@(8)
    436e:	157c 0010 000c 	moveb #16,%a2@(12)
    4374:	157c 0001 000d 	moveb #1,%a2@(13)
    437a:	701b           	moveq #27,%d0
    437c:	2540 0010      	movel %d0,%a2@(16)
    4380:	6000 00c2      	braw 0x4444
    4384:	362b 42fa      	movew %a3@(17146),%d3
    4388:	0c43 0008      	cmpiw #8,%d3
    438c:	6622           	bnes 0x43b0
    438e:	24bc 0000 3dc0 	movel #15808,%a2@
    4394:	357c 0002 0008 	movew #2,%a2@(8)
    439a:	157c 0010 000c 	moveb #16,%a2@(12)
    43a0:	157c 0001 000d 	moveb #1,%a2@(13)
    43a6:	7019           	moveq #25,%d0
    43a8:	2540 0010      	movel %d0,%a2@(16)
    43ac:	6000 0096      	braw 0x4444
    43b0:	0c43 0010      	cmpiw #16,%d3
    43b4:	6620           	bnes 0x43d6
    43b6:	24bc 0000 7b80 	movel #31616,%a2@
    43bc:	357c 0004 0008 	movew #4,%a2@(8)
    43c2:	157c 0020 000c 	moveb #32,%a2@(12)
    43c8:	157c 0001 000d 	moveb #1,%a2@(13)
    43ce:	7019           	moveq #25,%d0
    43d0:	2540 0010      	movel %d0,%a2@(16)
    43d4:	606e           	bras 0x4444
    43d6:	303c 2906      	movew #10502,%d0
    43da:	6000 0082      	braw 0x445e
    43de:	24bc 0000 f780 	movel #63360,%a2@
    43e4:	357c 0004 0008 	movew #4,%a2@(8)
    43ea:	157c 0020 000c 	moveb #32,%a2@(12)
    43f0:	157c 0001 000d 	moveb #1,%a2@(13)
    43f6:	7013           	moveq #19,%d0
    43f8:	2540 0010      	movel %d0,%a2@(16)
    43fc:	6046           	bras 0x4444
    43fe:	24bc 0001 ef00 	movel #126720,%a2@
    4404:	357c 0008 0008 	movew #8,%a2@(8)
    440a:	157c 0020 000c 	moveb #32,%a2@(12)
    4410:	157c 0001 000d 	moveb #1,%a2@(13)
    4416:	7027           	moveq #39,%d0
    4418:	2540 0010      	movel %d0,%a2@(16)
    441c:	6026           	bras 0x4444
    441e:	24bc 0003 de00 	movel #253440,%a2@
    4424:	357c 0010 0008 	movew #16,%a2@(8)
    442a:	157c 0020 000c 	moveb #32,%a2@(12)
    4430:	157c 0006 000d 	moveb #6,%a2@(13)
    4436:	7021           	moveq #33,%d0
    4438:	2540 0010      	movel %d0,%a2@(16)
    443c:	6006           	bras 0x4444
    443e:	303c 2906      	movew #10502,%d0
    4442:	601a           	bras 0x445e
    4444:	2012           	movel %a2@,%d0
    4446:	90aa 0010      	subl %a2@(16),%d0
    444a:	2540 0014      	movel %d0,%a2@(20)
    444e:	600c           	bras 0x445c
    4450:	303c 2906      	movew #10502,%d0
    4454:	6008           	bras 0x445e
    4456:	303c 0502      	movew #1282,%d0
    445a:	6002           	bras 0x445e
    445c:	7000           	moveq #0,%d0
    445e:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
    4462:	4e5e           	unlk %fp
    4464:	4e75           	rts

```

# SlotCardLowLevelFormat: 0x4478 - 0x4604


```

    4478:	4e56 ff7c      	linkw %fp,#-132
    447c:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    4480:	7600           	moveq #0,%d3
    4482:	3f2e 0008      	movew %fp@(8),%sp@-
    4486:	4e4f           	trap #15
    4488:	a0b9           	0120271
    448a:	2468 0004      	moveal %a0@(4),%a2
    448e:	200a           	movel %a2,%d0
    4490:	544f           	addqw #2,%sp
    4492:	6608           	bnes 0x449c
    4494:	303c 290a      	movew #10506,%d0
    4498:	6000 0164      	braw 0x45fe
    449c:	302a 4374      	movew %a2@(17268),%d0
    44a0:	b06e 000a      	cmpw %fp@(10),%d0
    44a4:	6708           	beqs 0x44ae
    44a6:	303c 2904      	movew #10500,%d0
    44aa:	6000 0152      	braw 0x45fe
    44ae:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    44b2:	0240 0008      	andiw #8,%d0
    44b6:	6708           	beqs 0x44c0
    44b8:	303c 2903      	movew #10499,%d0
    44bc:	6000 0140      	braw 0x45fe
    44c0:	42a7           	clrl %sp@-
    44c2:	42a7           	clrl %sp@-
    44c4:	2f2a 437e      	movel %a2@(17278),%sp@-
    44c8:	4e4f           	trap #15
    44ca:	a0b0           	0120260
    44cc:	102a 437a      	moveb %a2@(17274),%d0
    44d0:	0240 0001      	andiw #1,%d0
    44d4:	4fef 000c      	lea %sp@(12),%sp
    44d8:	6626           	bnes 0x4500
    44da:	486e ff7c      	pea %fp@(-132)
    44de:	3f2e 000a      	movew %fp@(10),%sp@-
    44e2:	3f2e 0008      	movew %fp@(8),%sp@-
    44e6:	4eba f96a      	jsr %pc@(0x3e52)
    44ea:	3600           	movew %d0,%d3
    44ec:	504f           	addqw #8,%sp
    44ee:	6710           	beqs 0x4500
    44f0:	2f2a 437e      	movel %a2@(17278),%sp@-
    44f4:	4e4f           	trap #15
    44f6:	a0b1           	0120261
    44f8:	3003           	movew %d3,%d0
    44fa:	584f           	addqw #4,%sp
    44fc:	6000 0100      	braw 0x45fe
    4500:	7001           	moveq #1,%d0
    4502:	c0aa 0004      	andl %a2@(4),%d0
    4506:	6700 00da      	beqw 0x45e2
    450a:	4878 0200      	pea 0x200
    450e:	4e4f           	trap #15
    4510:	a013           	0120023
    4512:	2648           	moveal %a0,%a3
    4514:	200b           	movel %a3,%d0
    4516:	584f           	addqw #4,%sp
    4518:	6608           	bnes 0x4522
    451a:	363c 0102      	movew #258,%d3
    451e:	6000 00d2      	braw 0x45f2
    4522:	42a7           	clrl %sp@-
    4524:	2f2a 0010      	movel %a2@(16),%sp@-
    4528:	4e4f           	trap #15
    452a:	a09a 4267      	macmw %d7u,%d4l,<<,%a2@+,%d0
    452e:	2f0b           	movel %a3,%sp@-
    4530:	4e4f           	trap #15
    4532:	a01b           	0120033
    4534:	3a38 fc02      	movew 0xfffffffffffffc02,%d5
    4538:	3005           	movew %d5,%d0
    453a:	0240 ff1f      	andiw #-225,%d0
    453e:	0040 0061      	oriw #97,%d0
    4542:	31c0 fc02      	movew %d0,0xfffffffffffffc02
    4546:	1f3c 0001      	moveb #1,%sp@-
    454a:	4eba df1a      	jsr %pc@(0x2466)
    454e:	1f3c 0001      	moveb #1,%sp@-
    4552:	4eba df9e      	jsr %pc@(0x24f2)
    4556:	1f3c 0001      	moveb #1,%sp@-
    455a:	4eba dfea      	jsr %pc@(0x2546)
    455e:	2f0a           	movel %a2,%sp@-
    4560:	2f0b           	movel %a3,%sp@-
    4562:	4eba d778      	jsr %pc@(0x1cdc)
    4566:	2800           	movel %d0,%d4
    4568:	4227           	clrb %sp@-
    456a:	4eba df86      	jsr %pc@(0x24f2)
    456e:	102a 437a      	moveb %a2@(17274),%d0
    4572:	0240 0010      	andiw #16,%d0
    4576:	4fef 001e      	lea %sp@(30),%sp
    457a:	6608           	bnes 0x4584
    457c:	4227           	clrb %sp@-
    457e:	4eba dfc6      	jsr %pc@(0x2546)
    4582:	544f           	addqw #2,%sp
    4584:	4878 0064      	pea 0x64
    4588:	2f2a 0010      	movel %a2@(16),%sp@-
    458c:	4e4f           	trap #15
    458e:	a09a 3005      	macw %d5l,%d3l,%a2@+,%d0
    4592:	0240 00e0      	andiw #224,%d0
    4596:	3238 fc02      	movew 0xfffffffffffffc02,%d1
    459a:	0241 ff1f      	andiw #-225,%d1
    459e:	8240           	orw %d0,%d1
    45a0:	0041 0001      	oriw #1,%d1
    45a4:	31c1 fc02      	movew %d1,0xfffffffffffffc02
    45a8:	2f0b           	movel %a3,%sp@-
    45aa:	4e4f           	trap #15
    45ac:	a012           	0120022
    45ae:	4a84           	tstl %d4
    45b0:	4fef 000c      	lea %sp@(12),%sp
    45b4:	6726           	beqs 0x45dc
    45b6:	7012           	moveq #18,%d0
    45b8:	b880           	cmpl %d0,%d4
    45ba:	6606           	bnes 0x45c2
    45bc:	363c 2907      	movew #10503,%d3
    45c0:	601a           	bras 0x45dc
    45c2:	7054           	moveq #84,%d0
    45c4:	b880           	cmpl %d0,%d4
    45c6:	6606           	bnes 0x45ce
    45c8:	363c 2903      	movew #10499,%d3
    45cc:	600e           	bras 0x45dc
    45ce:	363c 2906      	movew #10502,%d3
    45d2:	70fe           	moveq #-2,%d0
    45d4:	c1aa 0004      	andl %d0,%a2@(4)
    45d8:	c1aa 4382      	andl %d0,%a2@(17282)
    45dc:	4e4f           	trap #15
    45de:	a130           	0120460
    45e0:	6010           	bras 0x45f2
    45e2:	2f2a 437e      	movel %a2@(17278),%sp@-
    45e6:	4e4f           	trap #15
    45e8:	a0b1           	0120261
    45ea:	303c 2906      	movew #10502,%d0
    45ee:	584f           	addqw #4,%sp
    45f0:	600c           	bras 0x45fe
    45f2:	2f2a 437e      	movel %a2@(17278),%sp@-
    45f6:	4e4f           	trap #15
    45f8:	a0b1           	0120261
    45fa:	3003           	movew %d3,%d0
    45fc:	584f           	addqw #4,%sp
    45fe:	4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3
    4602:	4e5e           	unlk %fp
    4604:	4e75           	rts

```

# MSSlotCardSectorRead: 0x4620 - 0x47a6


```

    4620:	4e56 ff78      	linkw %fp,#-136
    4624:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    4628:	266e 0014      	moveal %fp@(20),%a3
    462c:	7600           	moveq #0,%d3
    462e:	2d53 fffc      	movel %a3@,%fp@(-4)
    4632:	4293           	clrl %a3@
    4634:	3f2e 0008      	movew %fp@(8),%sp@-
    4638:	4e4f           	trap #15
    463a:	a0b9           	0120271
    463c:	2468 0004      	moveal %a0@(4),%a2
    4640:	200a           	movel %a2,%d0
    4642:	544f           	addqw #2,%sp
    4644:	6608           	bnes 0x464e
    4646:	303c 290a      	movew #10506,%d0
    464a:	6000 0154      	braw 0x47a0
    464e:	302a 4374      	movew %a2@(17268),%d0
    4652:	b06e 000a      	cmpw %fp@(10),%d0
    4656:	6708           	beqs 0x4660
    4658:	303c 2904      	movew #10500,%d0
    465c:	6000 0142      	braw 0x47a0
    4660:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    4664:	0240 0008      	andiw #8,%d0
    4668:	6708           	beqs 0x4672
    466a:	303c 2903      	movew #10499,%d0
    466e:	6000 0130      	braw 0x47a0
    4672:	42a7           	clrl %sp@-
    4674:	42a7           	clrl %sp@-
    4676:	2f2a 437e      	movel %a2@(17278),%sp@-
    467a:	4e4f           	trap #15
    467c:	a0b0           	0120260
    467e:	102a 437a      	moveb %a2@(17274),%d0
    4682:	0240 0001      	andiw #1,%d0
    4686:	4fef 000c      	lea %sp@(12),%sp
    468a:	6626           	bnes 0x46b2
    468c:	486e ff78      	pea %fp@(-136)
    4690:	3f2e 000a      	movew %fp@(10),%sp@-
    4694:	3f2e 0008      	movew %fp@(8),%sp@-
    4698:	4eba f7b8      	jsr %pc@(0x3e52)
    469c:	3600           	movew %d0,%d3
    469e:	504f           	addqw #8,%sp
    46a0:	6710           	beqs 0x46b2
    46a2:	2f2a 437e      	movel %a2@(17278),%sp@-
    46a6:	4e4f           	trap #15
    46a8:	a0b1           	0120261
    46aa:	3003           	movew %d3,%d0
    46ac:	584f           	addqw #4,%sp
    46ae:	6000 00f0      	braw 0x47a0
    46b2:	7001           	moveq #1,%d0
    46b4:	c0aa 0004      	andl %a2@(4),%d0
    46b8:	6700 00d6      	beqw 0x4790
    46bc:	4aae fffc      	tstl %fp@(-4)
    46c0:	6610           	bnes 0x46d2
    46c2:	2f2a 437e      	movel %a2@(17278),%sp@-
    46c6:	4e4f           	trap #15
    46c8:	a0b1           	0120261
    46ca:	7000           	moveq #0,%d0
    46cc:	584f           	addqw #4,%sp
    46ce:	6000 00d0      	braw 0x47a0
    46d2:	42a7           	clrl %sp@-
    46d4:	2f2a 0010      	movel %a2@(16),%sp@-
    46d8:	4e4f           	trap #15
    46da:	a09a           	0120232
    46dc:	3a38 fc02      	movew 0xfffffffffffffc02,%d5
    46e0:	3005           	movew %d5,%d0
    46e2:	0240 ff1f      	andiw #-225,%d0
    46e6:	0040 0061      	oriw #97,%d0
    46ea:	31c0 fc02      	movew %d0,0xfffffffffffffc02
    46ee:	1f3c 0001      	moveb #1,%sp@-
    46f2:	4eba dd72      	jsr %pc@(0x2466)
    46f6:	1f3c 0001      	moveb #1,%sp@-
    46fa:	4eba ddf6      	jsr %pc@(0x24f2)
    46fe:	1f3c 0001      	moveb #1,%sp@-
    4702:	4eba de42      	jsr %pc@(0x2546)
    4706:	2f0a           	movel %a2,%sp@-
    4708:	486e fffc      	pea %fp@(-4)
    470c:	2f2e 0010      	movel %fp@(16),%sp@-
    4710:	2f2e 000c      	movel %fp@(12),%sp@-
    4714:	4eba d12e      	jsr %pc@(0x1844)
    4718:	2800           	movel %d0,%d4
    471a:	4227           	clrb %sp@-
    471c:	4eba ddd4      	jsr %pc@(0x24f2)
    4720:	102a 437a      	moveb %a2@(17274),%d0
    4724:	0240 0010      	andiw #16,%d0
    4728:	4fef 0020      	lea %sp@(32),%sp
    472c:	6608           	bnes 0x4736
    472e:	4227           	clrb %sp@-
    4730:	4eba de14      	jsr %pc@(0x2546)
    4734:	544f           	addqw #2,%sp
    4736:	4878 0064      	pea 0x64
    473a:	2f2a 0010      	movel %a2@(16),%sp@-
    473e:	4e4f           	trap #15
    4740:	a09a 3005      	macw %d5l,%d3l,%a2@+,%d0
    4744:	0240 00e0      	andiw #224,%d0
    4748:	3238 fc02      	movew 0xfffffffffffffc02,%d1
    474c:	0241 ff1f      	andiw #-225,%d1
    4750:	8240           	orw %d0,%d1
    4752:	0041 0001      	oriw #1,%d1
    4756:	31c1 fc02      	movew %d1,0xfffffffffffffc02
    475a:	4a84           	tstl %d4
    475c:	504f           	addqw #8,%sp
    475e:	6606           	bnes 0x4766
    4760:	26ae fffc      	movel %fp@(-4),%a3@
    4764:	602e           	bras 0x4794
    4766:	7018           	moveq #24,%d0
    4768:	b880           	cmpl %d0,%d4
    476a:	6606           	bnes 0x4772
    476c:	363c 0502      	movew #1282,%d3
    4770:	6022           	bras 0x4794
    4772:	7013           	moveq #19,%d0
    4774:	b880           	cmpl %d0,%d4
    4776:	6606           	bnes 0x477e
    4778:	363c 2909      	movew #10505,%d3
    477c:	6016           	bras 0x4794
    477e:	7020           	moveq #32,%d0
    4780:	b880           	cmpl %d0,%d4
    4782:	6606           	bnes 0x478a
    4784:	363c 2908      	movew #10504,%d3
    4788:	600a           	bras 0x4794
    478a:	363c 2906      	movew #10502,%d3
    478e:	6004           	bras 0x4794
    4790:	363c 2906      	movew #10502,%d3
    4794:	2f2a 437e      	movel %a2@(17278),%sp@-
    4798:	4e4f           	trap #15
    479a:	a0b1           	0120261
    479c:	3003           	movew %d3,%d0
    479e:	584f           	addqw #4,%sp
    47a0:	4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3
    47a4:	4e5e           	unlk %fp
    47a6:	4e75           	rts

```

# SlotCardSectorWrite: 0x47c0 - 0x494a


```

    47c0:	4e56 ff78      	linkw %fp,#-136
    47c4:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    47c8:	266e 0014      	moveal %fp@(20),%a3
    47cc:	7600           	moveq #0,%d3
    47ce:	2d53 fffc      	movel %a3@,%fp@(-4)
    47d2:	4293           	clrl %a3@
    47d4:	3f2e 0008      	movew %fp@(8),%sp@-
    47d8:	4e4f           	trap #15
    47da:	a0b9           	0120271
    47dc:	2468 0004      	moveal %a0@(4),%a2
    47e0:	200a           	movel %a2,%d0
    47e2:	544f           	addqw #2,%sp
    47e4:	6608           	bnes 0x47ee
    47e6:	303c 290a      	movew #10506,%d0
    47ea:	6000 0158      	braw 0x4944
    47ee:	302a 4374      	movew %a2@(17268),%d0
    47f2:	b06e 000a      	cmpw %fp@(10),%d0
    47f6:	6708           	beqs 0x4800
    47f8:	303c 2904      	movew #10500,%d0
    47fc:	6000 0146      	braw 0x4944
    4800:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    4804:	0240 0008      	andiw #8,%d0
    4808:	6708           	beqs 0x4812
    480a:	303c 2903      	movew #10499,%d0
    480e:	6000 0134      	braw 0x4944
    4812:	42a7           	clrl %sp@-
    4814:	42a7           	clrl %sp@-
    4816:	2f2a 437e      	movel %a2@(17278),%sp@-
    481a:	4e4f           	trap #15
    481c:	a0b0           	0120260
    481e:	102a 437a      	moveb %a2@(17274),%d0
    4822:	0240 0001      	andiw #1,%d0
    4826:	4fef 000c      	lea %sp@(12),%sp
    482a:	6626           	bnes 0x4852
    482c:	486e ff78      	pea %fp@(-136)
    4830:	3f2e 000a      	movew %fp@(10),%sp@-
    4834:	3f2e 0008      	movew %fp@(8),%sp@-
    4838:	4eba f618      	jsr %pc@(0x3e52)
    483c:	3600           	movew %d0,%d3
    483e:	504f           	addqw #8,%sp
    4840:	6710           	beqs 0x4852
    4842:	2f2a 437e      	movel %a2@(17278),%sp@-
    4846:	4e4f           	trap #15
    4848:	a0b1           	0120261
    484a:	3003           	movew %d3,%d0
    484c:	584f           	addqw #4,%sp
    484e:	6000 00f4      	braw 0x4944
    4852:	7001           	moveq #1,%d0
    4854:	282a 0004      	movel %a2@(4),%d4
    4858:	c084           	andl %d4,%d0
    485a:	6700 00d8      	beqw 0x4934
    485e:	7002           	moveq #2,%d0
    4860:	c084           	andl %d4,%d0
    4862:	6708           	beqs 0x486c
    4864:	363c 2907      	movew #10503,%d3
    4868:	6000 00ce      	braw 0x4938
    486c:	4aae fffc      	tstl %fp@(-4)
    4870:	6610           	bnes 0x4882
    4872:	2f2a 437e      	movel %a2@(17278),%sp@-
    4876:	4e4f           	trap #15
    4878:	a0b1           	0120261
    487a:	7000           	moveq #0,%d0
    487c:	584f           	addqw #4,%sp
    487e:	6000 00c4      	braw 0x4944
    4882:	42a7           	clrl %sp@-
    4884:	2f2a 0010      	movel %a2@(16),%sp@-
    4888:	4e4f           	trap #15
    488a:	a09a           	0120232
    488c:	3a38 fc02      	movew 0xfffffffffffffc02,%d5
    4890:	3005           	movew %d5,%d0
    4892:	0240 ff1f      	andiw #-225,%d0
    4896:	0040 0061      	oriw #97,%d0
    489a:	31c0 fc02      	movew %d0,0xfffffffffffffc02
    489e:	1f3c 0001      	moveb #1,%sp@-
    48a2:	4eba dbc2      	jsr %pc@(0x2466)
    48a6:	1f3c 0001      	moveb #1,%sp@-
    48aa:	4eba dc46      	jsr %pc@(0x24f2)
    48ae:	1f3c 0001      	moveb #1,%sp@-
    48b2:	4eba dc92      	jsr %pc@(0x2546)
    48b6:	2f0a           	movel %a2,%sp@-
    48b8:	486e fffc      	pea %fp@(-4)
    48bc:	2f2e 0010      	movel %fp@(16),%sp@-
    48c0:	2f2e 000c      	movel %fp@(12),%sp@-
    48c4:	4eba cdf0      	jsr %pc@(0x16b6)
    48c8:	2800           	movel %d0,%d4
    48ca:	4227           	clrb %sp@-
    48cc:	4eba dc24      	jsr %pc@(0x24f2)
    48d0:	102a 437a      	moveb %a2@(17274),%d0
    48d4:	0240 0010      	andiw #16,%d0
    48d8:	4fef 0020      	lea %sp@(32),%sp
    48dc:	6608           	bnes 0x48e6
    48de:	4227           	clrb %sp@-
    48e0:	4eba dc64      	jsr %pc@(0x2546)
    48e4:	544f           	addqw #2,%sp
    48e6:	4878 0064      	pea 0x64
    48ea:	2f2a 0010      	movel %a2@(16),%sp@-
    48ee:	4e4f           	trap #15
    48f0:	a09a 3005      	macw %d5l,%d3l,%a2@+,%d0
    48f4:	0240 00e0      	andiw #224,%d0
    48f8:	3238 fc02      	movew 0xfffffffffffffc02,%d1
    48fc:	0241 ff1f      	andiw #-225,%d1
    4900:	8240           	orw %d0,%d1
    4902:	0041 0001      	oriw #1,%d1
    4906:	31c1 fc02      	movew %d1,0xfffffffffffffc02
    490a:	4a84           	tstl %d4
    490c:	504f           	addqw #8,%sp
    490e:	6606           	bnes 0x4916
    4910:	26ae fffc      	movel %fp@(-4),%a3@
    4914:	6022           	bras 0x4938
    4916:	7012           	moveq #18,%d0
    4918:	b880           	cmpl %d0,%d4
    491a:	6706           	beqs 0x4922
    491c:	7014           	moveq #20,%d0
    491e:	b880           	cmpl %d0,%d4
    4920:	660c           	bnes 0x492e
    4922:	363c 2907      	movew #10503,%d3
    4926:	7002           	moveq #2,%d0
    4928:	81aa 0004      	orl %d0,%a2@(4)
    492c:	600a           	bras 0x4938
    492e:	363c 2906      	movew #10502,%d3
    4932:	6004           	bras 0x4938
    4934:	363c 2906      	movew #10502,%d3
    4938:	2f2a 437e      	movel %a2@(17278),%sp@-
    493c:	4e4f           	trap #15
    493e:	a0b1           	0120261
    4940:	3003           	movew %d3,%d0
    4942:	584f           	addqw #4,%sp
    4944:	4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3
    4948:	4e5e           	unlk %fp
    494a:	4e75           	rts

```

# SlotPowerCheck: 0x4962 - 0x49ea


```

    4962:	4e56 fffe      	linkw %fp,#-2
    4966:	2f0a           	movel %a2,%sp@-
    4968:	3f2e 0008      	movew %fp@(8),%sp@-
    496c:	4e4f           	trap #15
    496e:	a0b9           	0120271
    4970:	2468 0004      	moveal %a0@(4),%a2
    4974:	200a           	movel %a2,%d0
    4976:	544f           	addqw #2,%sp
    4978:	6606           	bnes 0x4980
    497a:	303c 290a      	movew #10506,%d0
    497e:	6066           	bras 0x49e6
    4980:	302a 4374      	movew %a2@(17268),%d0
    4984:	b06e 000a      	cmpw %fp@(10),%d0
    4988:	6706           	beqs 0x4990
    498a:	303c 2904      	movew #10500,%d0
    498e:	6056           	bras 0x49e6
    4990:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    4994:	0240 0008      	andiw #8,%d0
    4998:	6706           	beqs 0x49a0
    499a:	303c 2903      	movew #10499,%d0
    499e:	6046           	bras 0x49e6
    49a0:	102a 437a      	moveb %a2@(17274),%d0
    49a4:	0240 0004      	andiw #4,%d0
    49a8:	6706           	beqs 0x49b0
    49aa:	303c 2903      	movew #10499,%d0
    49ae:	6036           	bras 0x49e6
    49b0:	7001           	moveq #1,%d0
    49b2:	c0aa 0004      	andl %a2@(4),%d0
    49b6:	672a           	beqs 0x49e2
    49b8:	486e ffff      	pea %fp@(-1)
    49bc:	42a7           	clrl %sp@-
    49be:	42a7           	clrl %sp@-
    49c0:	42a7           	clrl %sp@-
    49c2:	42a7           	clrl %sp@-
    49c4:	42a7           	clrl %sp@-
    49c6:	4227           	clrb %sp@-
    49c8:	4e4f           	trap #15
    49ca:	a324           	0121444
    49cc:	0c2e 0007 ffff 	cmpib #7,%fp@(-1)
    49d2:	4fef 001a      	lea %sp@(26),%sp
    49d6:	6206           	bhis 0x49de
    49d8:	303c 2902      	movew #10498,%d0
    49dc:	6002           	bras 0x49e0
    49de:	7000           	moveq #0,%d0
    49e0:	6004           	bras 0x49e6
    49e2:	303c 2906      	movew #10502,%d0
    49e6:	245f           	moveal %sp@+,%a2
    49e8:	4e5e           	unlk %fp
    49ea:	4e75           	rts

```

# MemoryStickINSInterrupt: 0x49fe - 0x4ad6


```

    49fe:	4e56 0000      	linkw %fp,#0
    4a02:	2f0a           	movel %a2,%sp@-
    4a04:	1038 f41c      	moveb 0xfffffffffffff41c,%d0        %d0 = pdpol;    // Port D polarity
    4a08:	0240 0008      	andiw #8,%d0
    4a0c:	6754           	beqs 0x4a62                         if (%d0 & 0x08 == 0) goto polarity_pos;
    4a0e:	4ab8 0304      	tstl 0x304
    4a12:	6734           	beqs 0x4a48                         if (global_304 == 0) goto lbl_4a48;
    4a14:	2078 0304      	moveal 0x304,%a0
    4a18:	2468 0024      	moveal %a0@(36),%a2
    4a1c:	200a           	movel %a2,%d0
    4a1e:	6728           	beqs 0x4a48
    4a20:	3f2a 4374      	movew %a2@(17268),%sp@-
    4a24:	7406           	moveq #6,%d2
    4a26:	4e4f           	trap #15
    4a28:	a347           	mov3ql #1,%d7
    4a2a:	2078 0304      	moveal 0x304,%a0
    4a2e:	2068 0024      	moveal %a0@(36),%a0
    4a32:	0228 00fe 437a 	andib #-2,%a0@(17274)
    4a38:	2078 0304      	moveal 0x304,%a0
    4a3c:	2068 0024      	moveal %a0@(36),%a0
    4a40:	0028 0002 437a 	orib #2,%a0@(17274)
    4a46:	544f           	addqw #2,%sp
    4a48:	0238 00f7 f41c 	andib #-9,0xfffffffffffff41c    lbl_4a48: pdpol &= 0xf7; // switch to positive polarity
    4a4e:	2078 0304      	moveal 0x304,%a0                    %a0 = global_304;
    4a52:	2468 0010      	moveal %a0@(16),%a2                 %a2 = *(global_304 + 16)
    4a56:	200a           	movel %a2,%d0                       %d0 = %a2;
    4a58:	6768           	beqs 0x4ac2
    4a5a:	7001           	moveq #1,%d0
    4a5c:	81aa 001c      	orl %d0,%a2@(28)
    4a60:	6060           	bras 0x4ac2
    4a62:	4ab8 0304      	tstl 0x304                      plolarity_pos:
    4a66:	6738           	beqs 0x4aa0
    4a68:	2078 0304      	moveal 0x304,%a0
    4a6c:	2468 0024      	moveal %a0@(36),%a2
    4a70:	200a           	movel %a2,%d0
    4a72:	672c           	beqs 0x4aa0
    4a74:	3f2a 4374      	movew %a2@(17268),%sp@-
    4a78:	7407           	moveq #7,%d2
    4a7a:	4e4f           	trap #15
    4a7c:	a347           	mov3ql #1,%d7
    4a7e:	2078 0304      	moveal 0x304,%a0
    4a82:	2068 0024      	moveal %a0@(36),%a0
    4a86:	41e8 437a      	lea %a0@(17274),%a0
    4a8a:	2448           	moveal %a0,%a2
    4a8c:	1010           	moveb %a0@,%d0
    4a8e:	0240 0002      	andiw #2,%d0
    4a92:	544f           	addqw #2,%sp
    4a94:	6706           	beqs 0x4a9c
    4a96:	0212 00fd      	andib #-3,%a2@
    4a9a:	6004           	bras 0x4aa0
    4a9c:	0012 0004      	orib #4,%a2@
    4aa0:	4227           	clrb %sp@-
    4aa2:	4eba d9c2      	jsr %pc@(0x2466)
    4aa6:	0038 0008 f41c 	orib #8,0xfffffffffffff41c
    4aac:	2078 0304      	moveal 0x304,%a0
    4ab0:	2468 0010      	moveal %a0@(16),%a2
    4ab4:	200a           	movel %a2,%d0
    4ab6:	544f           	addqw #2,%sp
    4ab8:	6708           	beqs 0x4ac2
    4aba:	02aa ffff ff00 	andil #-256,%a2@(28)
    4ac0:	001c
    4ac2:	1038 f419      	moveb 0xfffffffffffff419,%d0
    4ac6:	0200 fff0      	andib #-16,%d0
    4aca:	0000 0008      	orib #8,%d0
    4ace:	11c0 f419      	moveb %d0,0xfffffffffffff419
    4ad2:	245f           	moveal %sp@+,%a2
    4ad4:	4e5e           	unlk %fp
    4ad6:	4e75           	rts

```

# SlotHandleInterrupt: 0x4af2 - 0x4afe


```

    4af2:	4e56 0000      	linkw %fp,#0
    4af6:	31fc 0004 f30c 	movew #4,0xfffffffffffff30c
    4afc:	4e5e           	unlk %fp
    4afe:	4e75           	rts

```

# __Startup__: 0x4b16 - 0x4b38


```

    4b16:	4e56 0000      	linkw %fp,#0
    4b1a:	2f03           	movel %d3,%sp@-
    4b1c:	4eba ea8a      	jsr %pc@(0x35a8)
    4b20:	3600           	movew %d0,%d3
    4b22:	6704           	beqs 0x4b28
    4b24:	3003           	movew %d3,%d0
    4b26:	600c           	bras 0x4b34
    4b28:	4eba 001e      	jsr %pc@(0x4b48)
    4b2c:	226e 000a      	moveal %fp@(10),%a1
    4b30:	2288           	movel %a0,%a1@
    4b32:	7000           	moveq #0,%d0
    4b34:	261f           	movel %sp@+,%d3
    4b36:	4e5e           	unlk %fp
    4b38:	4e75           	rts

```

