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
     3cc:	4eba 2ac6      	jsr %pc@(0x2e94)
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
     42a:	4eba 2a68      	jsr %pc@(0x2e94)
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
     51c:	4eba 2770      	jsr %pc@(0x2c8e)
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
     5f0:	4eba 20d6      	jsr %pc@(0x26c8)
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
     64e:	4eba 2c3c      	jsr %pc@(0x328c)
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
     692:	4eba 2bf8      	jsr %pc@(0x328c)
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
     6e0:	4eba 1fe6      	jsr %pc@(0x26c8)
     6e4:	4a80           	tstl %d0
     6e6:	4fef 000c      	lea %sp@(12),%sp
     6ea:	6720           	beqs 0x70c
     6ec:	486e fff2      	pea %fp@(-14)
     6f0:	3f04           	movew %d4,%sp@-
     6f2:	4eba 2b98      	jsr %pc@(0x328c)
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
     71c:	4eba 2b6e      	jsr %pc@(0x328c)
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
     73e:	4eba 2b4c      	jsr %pc@(0x328c)
     742:	2053           	moveal %a3@,%a0
     744:	3083           	movew %d3,%a0@
     746:	5493           	addql #2,%a3@
     748:	5c4f           	addqw #6,%sp
     74a:	6028           	bras 0x774
     74c:	486e fff2      	pea %fp@(-14)
     750:	3f04           	movew %d4,%sp@-
     752:	4eba 2b38      	jsr %pc@(0x328c)
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
     adc:	4eba 41d0      	jsr %pc@(0x4cae)
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
     bdc:	4eba 26ae      	jsr %pc@(0x328c)
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
     c0c:	4eba 1c3a      	jsr %pc@(0x2848)
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
     da8:	4eba 1bea      	jsr %pc@(0x2994)
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
     dd8:	4eba 1a6e      	jsr %pc@(0x2848)
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
     efe:	4eba 3dae      	jsr %pc@(0x4cae)
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
     f28:	4eba 1f6a      	jsr %pc@(0x2e94)
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
     fd8:	4eba 186e      	jsr %pc@(0x2848)
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
    116e:	4eba 211c      	jsr %pc@(0x328c)
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
    119c:	4eba 16aa      	jsr %pc@(0x2848)
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
    124c:	4eba 15fa      	jsr %pc@(0x2848)
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
    12c8:	4eba 16ca      	jsr %pc@(0x2994)
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
    12f6:	4eba 1550      	jsr %pc@(0x2848)
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
    13d2:	4eba 15c0      	jsr %pc@(0x2994)
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
    1400:	4eba 1446      	jsr %pc@(0x2848)
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
    150e:	4eba 1d7c      	jsr %pc@(0x328c)
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
    1544:	4eba 1302      	jsr %pc@(0x2848)
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
    16ec:	4eba 35ec      	jsr %pc@(0x4cda)
    16f0:	3800           	movew %d0,%d4
    16f2:	202e 0008      	movel %fp@(8),%d0
    16f6:	2206           	movel %d6,%d1
    16f8:	4eba 3638      	jsr %pc@(0x4d32)
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
    1786:	4eba 3552      	jsr %pc@(0x4cda)
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
    1878:	4eba 3460      	jsr %pc@(0x4cda)
    187c:	3800           	movew %d0,%d4
    187e:	202e 0008      	movel %fp@(8),%d0
    1882:	2206           	movel %d6,%d1
    1884:	4eba 34ac      	jsr %pc@(0x4d32)
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
    1926:	4eba 0da0      	jsr %pc@(0x26c8)
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
    1984:	4eba 150e      	jsr %pc@(0x2e94)
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
    19cc:	4eba 14c6      	jsr %pc@(0x2e94)
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
    1a34:	4eba 32a4      	jsr %pc@(0x4cda)
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
    1aba:	4eba 13d8      	jsr %pc@(0x2e94)
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
    1b06:	4eba 138c      	jsr %pc@(0x2e94)
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
    1bba:	4eba 12d8      	jsr %pc@(0x2e94)
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
    1c00:	4eba 1292      	jsr %pc@(0x2e94)
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
    1ca2:	4eba 0ba4      	jsr %pc@(0x2848)
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
    1d2e:	4eba 2f7e      	jsr %pc@(0x4cae)
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
    1d56:	4eba 0f36      	jsr %pc@(0x2c8e)
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
    1e16:	4eba 1474      	jsr %pc@(0x328c)
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
    1e42:	4eba 0a04      	jsr %pc@(0x2848)
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
    1e66:	4eba 070e      	jsr %pc@(0x2576)
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
    1e90:	4eba 09b6      	jsr %pc@(0x2848)
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
    206c:	4eba 02ec      	jsr %pc@(0x235a)
    2070:	7001           	moveq #1,%d0
    2072:	584f           	addqw #4,%sp
    2074:	6056           	bras 0x20cc
    2076:	1013           	moveb %a3@,%d0
    2078:	0240 0001      	andiw #1,%d0
    207c:	6736           	beqs 0x20b4
    207e:	486e ffff      	pea %fp@(-1)
    2082:	4eba 02d6      	jsr %pc@(0x235a)
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
    20aa:	4eba 02ae      	jsr %pc@(0x235a)
    20ae:	7001           	moveq #1,%d0
    20b0:	584f           	addqw #4,%sp
    20b2:	6018           	bras 0x20cc
    20b4:	1013           	moveb %a3@,%d0
    20b6:	0240 0080      	andiw #128,%d0
    20ba:	660e           	bnes 0x20ca
    20bc:	486e ffff      	pea %fp@(-1)
    20c0:	4eba 0298      	jsr %pc@(0x235a)
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
    2224:	4eba 0134      	jsr %pc@(0x235a)
    2228:	7001           	moveq #1,%d0
    222a:	584f           	addqw #4,%sp
    222c:	6018           	bras 0x2246
    222e:	1013           	moveb %a3@,%d0
    2230:	0240 0080      	andiw #128,%d0
    2234:	660e           	bnes 0x2244
    2236:	486e ffff      	pea %fp@(-1)
    223a:	4eba 011e      	jsr %pc@(0x235a)
    223e:	7001           	moveq #1,%d0
    2240:	584f           	addqw #4,%sp
    2242:	6002           	bras 0x2246
    2244:	7000           	moveq #0,%d0
    2246:	4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3
    224a:	4e5e           	unlk %fp
    224c:	4e75           	rts

```

# wait_int: 0x225e - 0x22e4


```

    225e:	4e56 0000      	linkw %fp,#0
    2262:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    2266:	266e 0008      	moveal %fp@(8),%a3
    226a:	2c2e 000c      	movel %fp@(12),%d6
    226e:	2078 0304      	moveal 0x304,%a0
    2272:	2468 0024      	moveal %a0@(36),%a2
    2276:	3e38 f110      	movew 0xfffffffffffff110,%d7
    227a:	e24f           	lsrw #1,%d7
    227c:	0247 0007      	andiw #7,%d7
    2280:	223c 0002 0000 	movel #131072,%d1
    2286:	efa9           	lsll %d7,%d1
    2288:	7e00           	moveq #0,%d7
    228a:	3e38 f100      	movew 0xfffffffffffff100,%d7
    228e:	740d           	moveq #13,%d2
    2290:	e5af           	lsll %d2,%d7
    2292:	de81           	addl %d1,%d7
    2294:	4a86           	tstl %d6
    2296:	660c           	bnes 0x22a4
    2298:	2007           	movel %d7,%d0
    229a:	5c80           	addql #6,%d0
    229c:	2040           	moveal %d0,%a0
    229e:	1690           	moveb %a0@,%a3@
    22a0:	7000           	moveq #0,%d0
    22a2:	603a           	bras 0x22de
    22a4:	7800           	moveq #0,%d4
    22a6:	7600           	moveq #0,%d3
    22a8:	1a2a 4388      	moveb %a2@(17288),%d5
    22ac:	670a           	beqs 0x22b8
    22ae:	1685           	moveb %d5,%a3@
    22b0:	422a 4388      	clrb %a2@(17288)
    22b4:	7000           	moveq #0,%d0
    22b6:	6026           	bras 0x22de
    22b8:	4878 0064      	pea 0x64
    22bc:	4e4f           	trap #15
    22be:	a249           	0121111
    22c0:	0643 0064      	addiw #100,%d3
    22c4:	5244           	addqw #1,%d4
    22c6:	3043           	moveaw %d3,%a0
    22c8:	b1c6           	cmpal %d6,%a0
    22ca:	584f           	addqw #4,%sp
    22cc:	65da           	bcss 0x22a8
    22ce:	2007           	movel %d7,%d0
    22d0:	5c80           	addql #6,%d0
    22d2:	2040           	moveal %d0,%a0
    22d4:	1010           	moveb %a0@,%d0
    22d6:	0000 0001      	orib #1,%d0
    22da:	1680           	moveb %d0,%a3@
    22dc:	7000           	moveq #0,%d0
    22de:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    22e2:	4e5e           	unlk %fp
    22e4:	4e75           	rts

```

# init_mshif: 0x22f2 - 0x234a


```

    22f2:	4e56 0000      	linkw %fp,#0
    22f6:	2f03           	movel %d3,%sp@-
    22f8:	3638 f110      	movew 0xfffffffffffff110,%d3
    22fc:	e24b           	lsrw #1,%d3
    22fe:	0243 0007      	andiw #7,%d3
    2302:	223c 0002 0000 	movel #131072,%d1
    2308:	e7a9           	lsll %d3,%d1
    230a:	7600           	moveq #0,%d3
    230c:	3638 f100      	movew 0xfffffffffffff100,%d3
    2310:	740d           	moveq #13,%d2
    2312:	e5ab           	lsll %d2,%d3
    2314:	d681           	addl %d1,%d3
    2316:	7009           	moveq #9,%d0
    2318:	d083           	addl %d3,%d0
    231a:	2040           	moveal %d0,%a0
    231c:	4210           	clrb %a0@
    231e:	2003           	movel %d3,%d0
    2320:	5080           	addql #8,%d0
    2322:	2040           	moveal %d0,%a0
    2324:	0210 00f1      	andib #-15,%a0@
    2328:	7009           	moveq #9,%d0
    232a:	d083           	addl %d3,%d0
    232c:	2040           	moveal %d0,%a0
    232e:	10bc 000e      	moveb #14,%a0@
    2332:	2f2e 0008      	movel %fp@(8),%sp@-
    2336:	4eba 0022      	jsr %pc@(0x235a)
    233a:	2003           	movel %d3,%d0
    233c:	5680           	addql #3,%d0
    233e:	2040           	moveal %d0,%a0
    2340:	10bc 0004      	moveb #4,%a0@
    2344:	584f           	addqw #4,%sp
    2346:	261f           	movel %sp@+,%d3
    2348:	4e5e           	unlk %fp
    234a:	4e75           	rts

```

# reset_mshif: 0x235a - 0x23fc


```

    235a:	4e56 0000      	linkw %fp,#0
    235e:	2f0a           	movel %a2,%sp@-
    2360:	2f03           	movel %d3,%sp@-
    2362:	246e 0008      	moveal %fp@(8),%a2
    2366:	3638 f110      	movew 0xfffffffffffff110,%d3
    236a:	e24b           	lsrw #1,%d3
    236c:	0243 0007      	andiw #7,%d3
    2370:	223c 0002 0000 	movel #131072,%d1
    2376:	e7a9           	lsll %d3,%d1
    2378:	7600           	moveq #0,%d3
    237a:	3638 f100      	movew 0xfffffffffffff100,%d3
    237e:	740d           	moveq #13,%d2
    2380:	e5ab           	lsll %d2,%d3
    2382:	d681           	addl %d1,%d3
    2384:	42a7           	clrl %sp@-
    2386:	4eba 0084      	jsr %pc@(0x240c)
    238a:	2003           	movel %d3,%d0
    238c:	5e80           	addql #7,%d0
    238e:	2040           	moveal %d0,%a0
    2390:	0010 0080      	orib #-128,%a0@
    2394:	2003           	movel %d3,%d0
    2396:	5680           	addql #3,%d0
    2398:	2040           	moveal %d0,%a0
    239a:	0010 0080      	orib #-128,%a0@
    239e:	2003           	movel %d3,%d0
    23a0:	5680           	addql #3,%d0
    23a2:	2040           	moveal %d0,%a0
    23a4:	0210 007f      	andib #127,%a0@
    23a8:	4878 0514      	pea 0x514
    23ac:	2f0a           	movel %a2,%sp@-
    23ae:	4eba feae      	jsr %pc@(0x225e)
    23b2:	4a00           	tstb %d0
    23b4:	4fef 000c      	lea %sp@(12),%sp
    23b8:	6704           	beqs 0x23be
    23ba:	7001           	moveq #1,%d0
    23bc:	6038           	bras 0x23f6
    23be:	2003           	movel %d3,%d0
    23c0:	5480           	addql #2,%d0
    23c2:	2040           	moveal %d0,%a0
    23c4:	1010           	moveb %a0@,%d0
    23c6:	0240 0040      	andiw #64,%d0
    23ca:	6704           	beqs 0x23d0
    23cc:	0012 0020      	orib #32,%a2@
    23d0:	7000           	moveq #0,%d0
    23d2:	1012           	moveb %a2@,%d0
    23d4:	3600           	movew %d0,%d3
    23d6:	0240 0001      	andiw #1,%d0
    23da:	6704           	beqs 0x23e0
    23dc:	7001           	moveq #1,%d0
    23de:	6016           	bras 0x23f6
    23e0:	3003           	movew %d3,%d0
    23e2:	0240 0080      	andiw #128,%d0
    23e6:	6708           	beqs 0x23f0
    23e8:	3003           	movew %d3,%d0
    23ea:	0240 0020      	andiw #32,%d0
    23ee:	6704           	beqs 0x23f4
    23f0:	7001           	moveq #1,%d0
    23f2:	6002           	bras 0x23f6
    23f4:	7000           	moveq #0,%d0
    23f6:	261f           	movel %sp@+,%d3
    23f8:	245f           	moveal %sp@+,%a2
    23fa:	4e5e           	unlk %fp
    23fc:	4e75           	rts

```

# power_save_mode: 0x240c - 0x2450


```

    240c:	4e56 0000      	linkw %fp,#0
    2410:	2f03           	movel %d3,%sp@-
    2412:	3638 f110      	movew 0xfffffffffffff110,%d3
    2416:	e24b           	lsrw #1,%d3
    2418:	0243 0007      	andiw #7,%d3
    241c:	223c 0002 0000 	movel #131072,%d1
    2422:	e7a9           	lsll %d3,%d1
    2424:	7600           	moveq #0,%d3
    2426:	3638 f100      	movew 0xfffffffffffff100,%d3
    242a:	740d           	moveq #13,%d2
    242c:	e5ab           	lsll %d2,%d3
    242e:	d681           	addl %d1,%d3
    2430:	4aae 0008      	tstl %fp@(8)
    2434:	670c           	beqs 0x2442
    2436:	2003           	movel %d3,%d0
    2438:	5680           	addql #3,%d0
    243a:	2040           	moveal %d0,%a0
    243c:	0010 0040      	orib #64,%a0@
    2440:	600a           	bras 0x244c
    2442:	2003           	movel %d3,%d0
    2444:	5680           	addql #3,%d0
    2446:	2040           	moveal %d0,%a0
    2448:	0210 00bf      	andib #-65,%a0@
    244c:	261f           	movel %sp@+,%d3
    244e:	4e5e           	unlk %fp
    2450:	4e75           	rts

```

# card_power: 0x2464 - 0x24be


```

    2464:	4e56 0000      	linkw %fp,#0
    2468:	2f03           	movel %d3,%sp@-
    246a:	3638 f110      	movew 0xfffffffffffff110,%d3
    246e:	e24b           	lsrw #1,%d3
    2470:	0243 0007      	andiw #7,%d3
    2474:	223c 0002 0000 	movel #131072,%d1
    247a:	e7a9           	lsll %d3,%d1
    247c:	7600           	moveq #0,%d3
    247e:	3638 f100      	movew 0xfffffffffffff100,%d3
    2482:	740d           	moveq #13,%d2
    2484:	e5ab           	lsll %d2,%d3
    2486:	d681           	addl %d1,%d3
    2488:	4a2e 0008      	tstb %fp@(8)
    248c:	6612           	bnes 0x24a0
    248e:	2003           	movel %d3,%d0
    2490:	5680           	addql #3,%d0
    2492:	2040           	moveal %d0,%a0
    2494:	0210 00df      	andib #-33,%a0@
    2498:	0238 00df f409 	andib #-33,0xfffffffffffff409
    249e:	601a           	bras 0x24ba
    24a0:	0038 0020 f409 	orib #32,0xfffffffffffff409
    24a6:	2003           	movel %d3,%d0
    24a8:	5680           	addql #3,%d0
    24aa:	2040           	moveal %d0,%a0
    24ac:	0010 0020      	orib #32,%a0@
    24b0:	4878 0bb8      	pea 0xbb8
    24b4:	4e4f           	trap #15
    24b6:	a249           	0121111
    24b8:	584f           	addqw #4,%sp
    24ba:	261f           	movel %sp@+,%d3
    24bc:	4e5e           	unlk %fp
    24be:	4e75           	rts

```

# access_led: 0x24ce - 0x2512


```

    24ce:	4e56 0000      	linkw %fp,#0
    24d2:	2f03           	movel %d3,%sp@-
    24d4:	3638 f110      	movew 0xfffffffffffff110,%d3
    24d8:	e24b           	lsrw #1,%d3
    24da:	0243 0007      	andiw #7,%d3
    24de:	223c 0002 0000 	movel #131072,%d1
    24e4:	e7a9           	lsll %d3,%d1
    24e6:	7600           	moveq #0,%d3
    24e8:	3638 f100      	movew 0xfffffffffffff100,%d3
    24ec:	740d           	moveq #13,%d2
    24ee:	e5ab           	lsll %d2,%d3
    24f0:	d681           	addl %d1,%d3
    24f2:	4a2e 0008      	tstb %fp@(8)
    24f6:	660c           	bnes 0x2504
    24f8:	2003           	movel %d3,%d0
    24fa:	5080           	addql #8,%d0
    24fc:	2040           	moveal %d0,%a0
    24fe:	0210 00fb      	andib #-5,%a0@
    2502:	600a           	bras 0x250e
    2504:	2003           	movel %d3,%d0
    2506:	5080           	addql #8,%d0
    2508:	2040           	moveal %d0,%a0
    250a:	0010 0004      	orib #4,%a0@
    250e:	261f           	movel %sp@+,%d3
    2510:	4e5e           	unlk %fp
    2512:	4e75           	rts

```

# warning_led: 0x2522 - 0x2566


```

    2522:	4e56 0000      	linkw %fp,#0
    2526:	2f03           	movel %d3,%sp@-
    2528:	3638 f110      	movew 0xfffffffffffff110,%d3
    252c:	e24b           	lsrw #1,%d3
    252e:	0243 0007      	andiw #7,%d3
    2532:	223c 0002 0000 	movel #131072,%d1
    2538:	e7a9           	lsll %d3,%d1
    253a:	7600           	moveq #0,%d3
    253c:	3638 f100      	movew 0xfffffffffffff100,%d3
    2540:	740d           	moveq #13,%d2
    2542:	e5ab           	lsll %d2,%d3
    2544:	d681           	addl %d1,%d3
    2546:	4a2e 0008      	tstb %fp@(8)
    254a:	660c           	bnes 0x2558
    254c:	2003           	movel %d3,%d0
    254e:	5080           	addql #8,%d0
    2550:	2040           	moveal %d0,%a0
    2552:	0210 00f7      	andib #-9,%a0@
    2556:	600a           	bras 0x2562
    2558:	2003           	movel %d3,%d0
    255a:	5080           	addql #8,%d0
    255c:	2040           	moveal %d0,%a0
    255e:	0010 0008      	orib #8,%a0@
    2562:	261f           	movel %sp@+,%d3
    2564:	4e5e           	unlk %fp
    2566:	4e75           	rts

```

# WriteRedundantData: 0x2576 - 0x26b0


```

    2576:	4e56 ffde      	linkw %fp,#-34
    257a:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    257e:	45ee ffde      	lea %fp@(-34),%a2
    2582:	14bc 0080      	moveb #-128,%a2@
    2586:	422a 0001      	clrb %a2@(1)
    258a:	302e 0008      	movew %fp@(8),%d0
    258e:	e048           	lsrw #8,%d0
    2590:	1540 0002      	moveb %d0,%a2@(2)
    2594:	156e 0009 0003 	moveb %fp@(9),%a2@(3)
    259a:	157c 0040 0004 	moveb #64,%a2@(4)
    25a0:	156e 000a 0005 	moveb %fp@(10),%a2@(5)
    25a6:	7600           	moveq #0,%d3
    25a8:	266e 000c      	moveal %fp@(12),%a3
    25ac:	1593 3806      	moveb %a3@,%a2@(00000006,%d3:l)
    25b0:	528b           	addql #1,%a3
    25b2:	5283           	addql #1,%d3
    25b4:	700a           	moveq #10,%d0
    25b6:	b680           	cmpl %d0,%d3
    25b8:	65f2           	bcss 0x25ac
    25ba:	486e ffff      	pea %fp@(-1)
    25be:	4227           	clrb %sp@-
    25c0:	2f0a           	movel %a2,%sp@-
    25c2:	4eba db0a      	jsr %pc@(0xce)
    25c6:	7200           	moveq #0,%d1
    25c8:	3200           	movew %d0,%d1
    25ca:	2601           	movel %d1,%d3
    25cc:	4fef 000a      	lea %sp@(10),%sp
    25d0:	6710           	beqs 0x25e2
    25d2:	2803           	movel %d3,%d4
    25d4:	2f04           	movel %d4,%sp@-
    25d6:	4eba 0e42      	jsr %pc@(0x341a)
    25da:	70ff           	moveq #-1,%d0
    25dc:	584f           	addqw #4,%sp
    25de:	6000 00ca      	braw 0x26aa
    25e2:	486e ffff      	pea %fp@(-1)
    25e6:	4227           	clrb %sp@-
    25e8:	1f3c 0055      	moveb #85,%sp@-
    25ec:	4eba db92      	jsr %pc@(0x180)
    25f0:	7200           	moveq #0,%d1
    25f2:	3200           	movew %d0,%d1
    25f4:	2601           	movel %d1,%d3
    25f6:	504f           	addqw #8,%sp
    25f8:	6712           	beqs 0x260c
    25fa:	7810           	moveq #16,%d4
    25fc:	8883           	orl %d3,%d4
    25fe:	2f04           	movel %d4,%sp@-
    2600:	4eba 0e18      	jsr %pc@(0x341a)
    2604:	70ff           	moveq #-1,%d0
    2606:	584f           	addqw #4,%sp
    2608:	6000 00a0      	braw 0x26aa
    260c:	486e ffff      	pea %fp@(-1)
    2610:	1f3c 0002      	moveb #2,%sp@-
    2614:	4eba dbe0      	jsr %pc@(0x1f6)
    2618:	7200           	moveq #0,%d1
    261a:	3200           	movew %d0,%d1
    261c:	2601           	movel %d1,%d3
    261e:	7001           	moveq #1,%d0
    2620:	b680           	cmpl %d0,%d3
    2622:	5c4f           	addqw #6,%sp
    2624:	6714           	beqs 0x263a
    2626:	4a83           	tstl %d3
    2628:	660c           	bnes 0x2636
    262a:	7810           	moveq #16,%d4
    262c:	8883           	orl %d3,%d4
    262e:	2f04           	movel %d4,%sp@-
    2630:	4eba 0de8      	jsr %pc@(0x341a)
    2634:	584f           	addqw #4,%sp
    2636:	70ff           	moveq #-1,%d0
    2638:	6070           	bras 0x26aa
    263a:	486e ffff      	pea %fp@(-1)
    263e:	486e fffe      	pea %fp@(-2)
    2642:	4eba da2c      	jsr %pc@(0x70)
    2646:	7200           	moveq #0,%d1
    2648:	3200           	movew %d0,%d1
    264a:	2601           	movel %d1,%d3
    264c:	504f           	addqw #8,%sp
    264e:	6710           	beqs 0x2660
    2650:	7810           	moveq #16,%d4
    2652:	8883           	orl %d3,%d4
    2654:	2f04           	movel %d4,%sp@-
    2656:	4eba 0dc2      	jsr %pc@(0x341a)
    265a:	70ff           	moveq #-1,%d0
    265c:	584f           	addqw #4,%sp
    265e:	604a           	bras 0x26aa
    2660:	7000           	moveq #0,%d0
    2662:	102e fffe      	moveb %fp@(-2),%d0
    2666:	3600           	movew %d0,%d3
    2668:	0240 0080      	andiw #128,%d0
    266c:	671a           	beqs 0x2688
    266e:	3003           	movew %d3,%d0
    2670:	0240 0040      	andiw #64,%d0
    2674:	6604           	bnes 0x267a
    2676:	7000           	moveq #0,%d0
    2678:	6030           	bras 0x26aa
    267a:	7601           	moveq #1,%d3
    267c:	2f03           	movel %d3,%sp@-
    267e:	4eba 0d9a      	jsr %pc@(0x341a)
    2682:	70ff           	moveq #-1,%d0
    2684:	584f           	addqw #4,%sp
    2686:	6022           	bras 0x26aa
    2688:	3003           	movew %d3,%d0
    268a:	0240 0001      	andiw #1,%d0
    268e:	670e           	beqs 0x269e
    2690:	7608           	moveq #8,%d3
    2692:	2f03           	movel %d3,%sp@-
    2694:	4eba 0d84      	jsr %pc@(0x341a)
    2698:	70ff           	moveq #-1,%d0
    269a:	584f           	addqw #4,%sp
    269c:	600c           	bras 0x26aa
    269e:	7610           	moveq #16,%d3
    26a0:	2f03           	movel %d3,%sp@-
    26a2:	4eba 0d76      	jsr %pc@(0x341a)
    26a6:	70ff           	moveq #-1,%d0
    26a8:	584f           	addqw #4,%sp
    26aa:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
    26ae:	4e5e           	unlk %fp
    26b0:	4e75           	rts

```

# ReadRedundantData: 0x26c8 - 0x2832


```

    26c8:	4e56 ffbe      	linkw %fp,#-66
    26cc:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    26d0:	282e 0010      	movel %fp@(16),%d4
    26d4:	47ee ffde      	lea %fp@(-34),%a3
    26d8:	45ee ffbe      	lea %fp@(-66),%a2
    26dc:	2044           	moveal %d4,%a0
    26de:	4290           	clrl %a0@
    26e0:	14bc 0080      	moveb #-128,%a2@
    26e4:	422a 0001      	clrb %a2@(1)
    26e8:	302e 0008      	movew %fp@(8),%d0
    26ec:	e048           	lsrw #8,%d0
    26ee:	1540 0002      	moveb %d0,%a2@(2)
    26f2:	156e 0009 0003 	moveb %fp@(9),%a2@(3)
    26f8:	157c 0040 0004 	moveb #64,%a2@(4)
    26fe:	156e 000a 0005 	moveb %fp@(10),%a2@(5)
    2704:	486e ffff      	pea %fp@(-1)
    2708:	4227           	clrb %sp@-
    270a:	2f0a           	movel %a2,%sp@-
    270c:	4eba d9c0      	jsr %pc@(0xce)
    2710:	7200           	moveq #0,%d1
    2712:	3200           	movew %d0,%d1
    2714:	2601           	movel %d1,%d3
    2716:	4fef 000a      	lea %sp@(10),%sp
    271a:	6710           	beqs 0x272c
    271c:	2803           	movel %d3,%d4
    271e:	2f04           	movel %d4,%sp@-
    2720:	4eba 0cf8      	jsr %pc@(0x341a)
    2724:	70ff           	moveq #-1,%d0
    2726:	584f           	addqw #4,%sp
    2728:	6000 0102      	braw 0x282c
    272c:	486e ffff      	pea %fp@(-1)
    2730:	4227           	clrb %sp@-
    2732:	1f3c 00aa      	moveb #-86,%sp@-
    2736:	4eba da48      	jsr %pc@(0x180)
    273a:	7200           	moveq #0,%d1
    273c:	3200           	movew %d0,%d1
    273e:	2601           	movel %d1,%d3
    2740:	504f           	addqw #8,%sp
    2742:	6712           	beqs 0x2756
    2744:	7810           	moveq #16,%d4
    2746:	8883           	orl %d3,%d4
    2748:	2f04           	movel %d4,%sp@-
    274a:	4eba 0cce      	jsr %pc@(0x341a)
    274e:	70ff           	moveq #-1,%d0
    2750:	584f           	addqw #4,%sp
    2752:	6000 00d8      	braw 0x282c
    2756:	486e ffff      	pea %fp@(-1)
    275a:	1f3c 0001      	moveb #1,%sp@-
    275e:	4eba da96      	jsr %pc@(0x1f6)
    2762:	7200           	moveq #0,%d1
    2764:	3200           	movew %d0,%d1
    2766:	2601           	movel %d1,%d3
    2768:	7001           	moveq #1,%d0
    276a:	b680           	cmpl %d0,%d3
    276c:	5c4f           	addqw #6,%sp
    276e:	6716           	beqs 0x2786
    2770:	4a83           	tstl %d3
    2772:	660c           	bnes 0x2780
    2774:	7810           	moveq #16,%d4
    2776:	8883           	orl %d3,%d4
    2778:	2f04           	movel %d4,%sp@-
    277a:	4eba 0c9e      	jsr %pc@(0x341a)
    277e:	584f           	addqw #4,%sp
    2780:	70ff           	moveq #-1,%d0
    2782:	6000 00a8      	braw 0x282c
    2786:	486e ffff      	pea %fp@(-1)
    278a:	2f0b           	movel %a3,%sp@-
    278c:	4eba d99c      	jsr %pc@(0x12a)
    2790:	7200           	moveq #0,%d1
    2792:	3200           	movew %d0,%d1
    2794:	2601           	movel %d1,%d3
    2796:	504f           	addqw #8,%sp
    2798:	6714           	beqs 0x27ae
    279a:	7010           	moveq #16,%d0
    279c:	8680           	orl %d0,%d3
    279e:	2803           	movel %d3,%d4
    27a0:	2f04           	movel %d4,%sp@-
    27a2:	4eba 0c76      	jsr %pc@(0x341a)
    27a6:	70ff           	moveq #-1,%d0
    27a8:	584f           	addqw #4,%sp
    27aa:	6000 0080      	braw 0x282c
    27ae:	7000           	moveq #0,%d0
    27b0:	102b 0001      	moveb %a3@(1),%d0
    27b4:	3600           	movew %d0,%d3
    27b6:	0240 0080      	andiw #128,%d0
    27ba:	674e           	beqs 0x280a
    27bc:	7600           	moveq #0,%d3
    27be:	246e 000c      	moveal %fp@(12),%a2
    27c2:	14b3 3816      	moveb %a3@(00000016,%d3:l),%a2@
    27c6:	528a           	addql #1,%a2
    27c8:	5283           	addql #1,%d3
    27ca:	700a           	moveq #10,%d0
    27cc:	b680           	cmpl %d0,%d3
    27ce:	65f2           	bcss 0x27c2
    27d0:	102b 0001      	moveb %a3@(1),%d0
    27d4:	0240 0040      	andiw #64,%d0
    27d8:	660a           	bnes 0x27e4
    27da:	2044           	moveal %d4,%a0
    27dc:	7004           	moveq #4,%d0
    27de:	2080           	movel %d0,%a0@
    27e0:	7000           	moveq #0,%d0
    27e2:	6048           	bras 0x282c
    27e4:	102b 0003      	moveb %a3@(3),%d0
    27e8:	0240 0005      	andiw #5,%d0
    27ec:	6712           	beqs 0x2800
    27ee:	2044           	moveal %d4,%a0
    27f0:	4290           	clrl %a0@
    27f2:	7602           	moveq #2,%d3
    27f4:	2f03           	movel %d3,%sp@-
    27f6:	4eba 0c22      	jsr %pc@(0x341a)
    27fa:	70ff           	moveq #-1,%d0
    27fc:	584f           	addqw #4,%sp
    27fe:	602c           	bras 0x282c
    2800:	2044           	moveal %d4,%a0
    2802:	7008           	moveq #8,%d0
    2804:	2080           	movel %d0,%a0@
    2806:	7000           	moveq #0,%d0
    2808:	6022           	bras 0x282c
    280a:	3003           	movew %d3,%d0
    280c:	0240 0001      	andiw #1,%d0
    2810:	670e           	beqs 0x2820
    2812:	7608           	moveq #8,%d3
    2814:	2f03           	movel %d3,%sp@-
    2816:	4eba 0c02      	jsr %pc@(0x341a)
    281a:	70ff           	moveq #-1,%d0
    281c:	584f           	addqw #4,%sp
    281e:	600c           	bras 0x282c
    2820:	7610           	moveq #16,%d3
    2822:	2f03           	movel %d3,%sp@-
    2824:	4eba 0bf4      	jsr %pc@(0x341a)
    2828:	70ff           	moveq #-1,%d0
    282a:	584f           	addqw #4,%sp
    282c:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
    2830:	4e5e           	unlk %fp
    2832:	4e75           	rts

```

# OverwriteRedundantData: 0x2848 - 0x2978


```

    2848:	4e56 ffde      	linkw %fp,#-34
    284c:	48e7 1820      	moveml %d3-%d4/%a2,%sp@-
    2850:	45ee ffde      	lea %fp@(-34),%a2
    2854:	14bc 0080      	moveb #-128,%a2@
    2858:	422a 0001      	clrb %a2@(1)
    285c:	302e 0008      	movew %fp@(8),%d0
    2860:	e048           	lsrw #8,%d0
    2862:	1540 0002      	moveb %d0,%a2@(2)
    2866:	156e 0009 0003 	moveb %fp@(9),%a2@(3)
    286c:	157c 0080 0004 	moveb #-128,%a2@(4)
    2872:	156e 000a 0005 	moveb %fp@(10),%a2@(5)
    2878:	206e 000c      	moveal %fp@(12),%a0
    287c:	1550 0006      	moveb %a0@,%a2@(6)
    2880:	486e ffff      	pea %fp@(-1)
    2884:	4227           	clrb %sp@-
    2886:	2f0a           	movel %a2,%sp@-
    2888:	4eba d844      	jsr %pc@(0xce)
    288c:	7200           	moveq #0,%d1
    288e:	3200           	movew %d0,%d1
    2890:	2601           	movel %d1,%d3
    2892:	4fef 000a      	lea %sp@(10),%sp
    2896:	6710           	beqs 0x28a8
    2898:	2803           	movel %d3,%d4
    289a:	2f04           	movel %d4,%sp@-
    289c:	4eba 0b7c      	jsr %pc@(0x341a)
    28a0:	70ff           	moveq #-1,%d0
    28a2:	584f           	addqw #4,%sp
    28a4:	6000 00cc      	braw 0x2972
    28a8:	486e ffff      	pea %fp@(-1)
    28ac:	4227           	clrb %sp@-
    28ae:	1f3c 0055      	moveb #85,%sp@-
    28b2:	4eba d8cc      	jsr %pc@(0x180)
    28b6:	7200           	moveq #0,%d1
    28b8:	3200           	movew %d0,%d1
    28ba:	2601           	movel %d1,%d3
    28bc:	504f           	addqw #8,%sp
    28be:	6712           	beqs 0x28d2
    28c0:	7810           	moveq #16,%d4
    28c2:	8883           	orl %d3,%d4
    28c4:	2f04           	movel %d4,%sp@-
    28c6:	4eba 0b52      	jsr %pc@(0x341a)
    28ca:	70ff           	moveq #-1,%d0
    28cc:	584f           	addqw #4,%sp
    28ce:	6000 00a2      	braw 0x2972
    28d2:	486e ffff      	pea %fp@(-1)
    28d6:	1f3c 0002      	moveb #2,%sp@-
    28da:	4eba d91a      	jsr %pc@(0x1f6)
    28de:	7200           	moveq #0,%d1
    28e0:	3200           	movew %d0,%d1
    28e2:	2601           	movel %d1,%d3
    28e4:	7001           	moveq #1,%d0
    28e6:	b680           	cmpl %d0,%d3
    28e8:	5c4f           	addqw #6,%sp
    28ea:	6714           	beqs 0x2900
    28ec:	4a83           	tstl %d3
    28ee:	660c           	bnes 0x28fc
    28f0:	7810           	moveq #16,%d4
    28f2:	8883           	orl %d3,%d4
    28f4:	2f04           	movel %d4,%sp@-
    28f6:	4eba 0b22      	jsr %pc@(0x341a)
    28fa:	584f           	addqw #4,%sp
    28fc:	70ff           	moveq #-1,%d0
    28fe:	6072           	bras 0x2972
    2900:	486e ffff      	pea %fp@(-1)
    2904:	486e fffe      	pea %fp@(-2)
    2908:	4eba d766      	jsr %pc@(0x70)
    290c:	7200           	moveq #0,%d1
    290e:	3200           	movew %d0,%d1
    2910:	2601           	movel %d1,%d3
    2912:	504f           	addqw #8,%sp
    2914:	6712           	beqs 0x2928
    2916:	7010           	moveq #16,%d0
    2918:	8680           	orl %d0,%d3
    291a:	2803           	movel %d3,%d4
    291c:	2f04           	movel %d4,%sp@-
    291e:	4eba 0afa      	jsr %pc@(0x341a)
    2922:	70ff           	moveq #-1,%d0
    2924:	584f           	addqw #4,%sp
    2926:	604a           	bras 0x2972
    2928:	7000           	moveq #0,%d0
    292a:	102e fffe      	moveb %fp@(-2),%d0
    292e:	3600           	movew %d0,%d3
    2930:	0240 0080      	andiw #128,%d0
    2934:	671a           	beqs 0x2950
    2936:	3003           	movew %d3,%d0
    2938:	0240 0040      	andiw #64,%d0
    293c:	6604           	bnes 0x2942
    293e:	7000           	moveq #0,%d0
    2940:	6030           	bras 0x2972
    2942:	7601           	moveq #1,%d3
    2944:	2f03           	movel %d3,%sp@-
    2946:	4eba 0ad2      	jsr %pc@(0x341a)
    294a:	70ff           	moveq #-1,%d0
    294c:	584f           	addqw #4,%sp
    294e:	6022           	bras 0x2972
    2950:	3003           	movew %d3,%d0
    2952:	0240 0001      	andiw #1,%d0
    2956:	670e           	beqs 0x2966
    2958:	7608           	moveq #8,%d3
    295a:	2f03           	movel %d3,%sp@-
    295c:	4eba 0abc      	jsr %pc@(0x341a)
    2960:	70ff           	moveq #-1,%d0
    2962:	584f           	addqw #4,%sp
    2964:	600c           	bras 0x2972
    2966:	7610           	moveq #16,%d3
    2968:	2f03           	movel %d3,%sp@-
    296a:	4eba 0aae      	jsr %pc@(0x341a)
    296e:	70ff           	moveq #-1,%d0
    2970:	584f           	addqw #4,%sp
    2972:	4cdf 0418      	moveml %sp@+,%d3-%d4/%a2
    2976:	4e5e           	unlk %fp
    2978:	4e75           	rts

```

# WriteFlashPages: 0x2994 - 0x2c7a


```

    2994:	4e56 ffbc      	linkw %fp,#-68
    2998:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    299c:	1a2e 000a      	moveb %fp@(10),%d5
    29a0:	262e 000e      	movel %fp@(14),%d3
    29a4:	246e 0016      	moveal %fp@(22),%a2
    29a8:	266e 001a      	moveal %fp@(26),%a3
    29ac:	41ee ffdc      	lea %fp@(-36),%a0
    29b0:	2e08           	movel %a0,%d7
    29b2:	41ee ffbc      	lea %fp@(-68),%a0
    29b6:	2808           	movel %a0,%d4
    29b8:	1005           	moveb %d5,%d0
    29ba:	d02e 000c      	addb %fp@(12),%d0
    29be:	1d40 fffd      	moveb %d0,%fp@(-3)
    29c2:	2c03           	movel %d3,%d6
    29c4:	1685           	moveb %d5,%a3@
    29c6:	4a83           	tstl %d3
    29c8:	6706           	beqs 0x29d0
    29ca:	7001           	moveq #1,%d0
    29cc:	c083           	andl %d3,%d0
    29ce:	671e           	beqs 0x29ee
    29d0:	24bc 0000 2000 	movel #8192,%a2@
    29d6:	2f12           	movel %a2@,%sp@-
    29d8:	4eba 0a40      	jsr %pc@(0x341a)
    29dc:	5280           	addql #1,%d0
    29de:	584f           	addqw #4,%sp
    29e0:	6606           	bnes 0x29e8
    29e2:	0092 0000 4000 	oril #16384,%a2@
    29e8:	70ff           	moveq #-1,%d0
    29ea:	6000 0288      	braw 0x2c74
    29ee:	7001           	moveq #1,%d0
    29f0:	c0ae 0012      	andl %fp@(18),%d0
    29f4:	671e           	beqs 0x2a14
    29f6:	24bc 0000 2000 	movel #8192,%a2@
    29fc:	2f12           	movel %a2@,%sp@-
    29fe:	4eba 0a1a      	jsr %pc@(0x341a)
    2a02:	5280           	addql #1,%d0
    2a04:	584f           	addqw #4,%sp
    2a06:	6606           	bnes 0x2a0e
    2a08:	0092 0000 4000 	oril #16384,%a2@
    2a0e:	70ff           	moveq #-1,%d0
    2a10:	6000 0262      	braw 0x2c74
    2a14:	2044           	moveal %d4,%a0
    2a16:	10bc 0080      	moveb #-128,%a0@
    2a1a:	4228 0001      	clrb %a0@(1)
    2a1e:	302e 0008      	movew %fp@(8),%d0
    2a22:	e048           	lsrw #8,%d0
    2a24:	1140 0002      	moveb %d0,%a0@(2)
    2a28:	116e 0009 0003 	moveb %fp@(9),%a0@(3)
    2a2e:	4228 0004      	clrb %a0@(4)
    2a32:	1145 0005      	moveb %d5,%a0@(5)
    2a36:	7600           	moveq #0,%d3
    2a38:	2a2e 0012      	movel %fp@(18),%d5
    2a3c:	2045           	moveal %d5,%a0
    2a3e:	2244           	moveal %d4,%a1
    2a40:	1390 3806      	moveb %a0@,%a1@(00000006,%d3:l)
    2a44:	5285           	addql #1,%d5
    2a46:	5283           	addql #1,%d3
    2a48:	700a           	moveq #10,%d0
    2a4a:	b680           	cmpl %d0,%d3
    2a4c:	65ee           	bcss 0x2a3c
    2a4e:	486e ffff      	pea %fp@(-1)
    2a52:	4227           	clrb %sp@-
    2a54:	2f04           	movel %d4,%sp@-
    2a56:	4eba d676      	jsr %pc@(0xce)
    2a5a:	7200           	moveq #0,%d1
    2a5c:	3200           	movew %d0,%d1
    2a5e:	2601           	movel %d1,%d3
    2a60:	4fef 000a      	lea %sp@(10),%sp
    2a64:	671a           	beqs 0x2a80
    2a66:	2483           	movel %d3,%a2@
    2a68:	2f12           	movel %a2@,%sp@-
    2a6a:	4eba 09ae      	jsr %pc@(0x341a)
    2a6e:	5280           	addql #1,%d0
    2a70:	584f           	addqw #4,%sp
    2a72:	6606           	bnes 0x2a7a
    2a74:	0092 0000 4000 	oril #16384,%a2@
    2a7a:	70ff           	moveq #-1,%d0
    2a7c:	6000 01f6      	braw 0x2c74
    2a80:	486e ffff      	pea %fp@(-1)
    2a84:	4227           	clrb %sp@-
    2a86:	1f3c 0055      	moveb #85,%sp@-
    2a8a:	4eba d6f4      	jsr %pc@(0x180)
    2a8e:	7200           	moveq #0,%d1
    2a90:	3200           	movew %d0,%d1
    2a92:	2601           	movel %d1,%d3
    2a94:	504f           	addqw #8,%sp
    2a96:	671e           	beqs 0x2ab6
    2a98:	7010           	moveq #16,%d0
    2a9a:	8083           	orl %d3,%d0
    2a9c:	2480           	movel %d0,%a2@
    2a9e:	2f12           	movel %a2@,%sp@-
    2aa0:	4eba 0978      	jsr %pc@(0x341a)
    2aa4:	5280           	addql #1,%d0
    2aa6:	584f           	addqw #4,%sp
    2aa8:	6606           	bnes 0x2ab0
    2aaa:	0092 0000 4000 	oril #16384,%a2@
    2ab0:	70ff           	moveq #-1,%d0
    2ab2:	6000 01c0      	braw 0x2c74
    2ab6:	7800           	moveq #0,%d4
    2ab8:	182e fffd      	moveb %fp@(-3),%d4
    2abc:	3a04           	movew %d4,%d5
    2abe:	486e ffff      	pea %fp@(-1)
    2ac2:	1f3c 0002      	moveb #2,%sp@-
    2ac6:	4eba d72e      	jsr %pc@(0x1f6)
    2aca:	7200           	moveq #0,%d1
    2acc:	3200           	movew %d0,%d1
    2ace:	2601           	movel %d1,%d3
    2ad0:	7001           	moveq #1,%d0
    2ad2:	b680           	cmpl %d0,%d3
    2ad4:	5c4f           	addqw #6,%sp
    2ad6:	6738           	beqs 0x2b10
    2ad8:	4a83           	tstl %d3
    2ada:	662e           	bnes 0x2b0a
    2adc:	7010           	moveq #16,%d0
    2ade:	8083           	orl %d3,%d0
    2ae0:	2480           	movel %d0,%a2@
    2ae2:	486e ffff      	pea %fp@(-1)
    2ae6:	2f07           	movel %d7,%sp@-
    2ae8:	4eba d640      	jsr %pc@(0x12a)
    2aec:	4a40           	tstw %d0
    2aee:	504f           	addqw #8,%sp
    2af0:	6606           	bnes 0x2af8
    2af2:	2047           	moveal %d7,%a0
    2af4:	16a8 0015      	moveb %a0@(21),%a3@
    2af8:	2f12           	movel %a2@,%sp@-
    2afa:	4eba 091e      	jsr %pc@(0x341a)
    2afe:	5280           	addql #1,%d0
    2b00:	584f           	addqw #4,%sp
    2b02:	6606           	bnes 0x2b0a
    2b04:	0092 0000 4000 	oril #16384,%a2@
    2b0a:	70ff           	moveq #-1,%d0
    2b0c:	6000 0166      	braw 0x2c74
    2b10:	486e ffff      	pea %fp@(-1)
    2b14:	486e fffe      	pea %fp@(-2)
    2b18:	4eba d556      	jsr %pc@(0x70)
    2b1c:	7200           	moveq #0,%d1
    2b1e:	3200           	movew %d0,%d1
    2b20:	2601           	movel %d1,%d3
    2b22:	504f           	addqw #8,%sp
    2b24:	671e           	beqs 0x2b44
    2b26:	7010           	moveq #16,%d0
    2b28:	8083           	orl %d3,%d0
    2b2a:	2480           	movel %d0,%a2@
    2b2c:	2f12           	movel %a2@,%sp@-
    2b2e:	4eba 08ea      	jsr %pc@(0x341a)
    2b32:	5280           	addql #1,%d0
    2b34:	584f           	addqw #4,%sp
    2b36:	6606           	bnes 0x2b3e
    2b38:	0092 0000 4000 	oril #16384,%a2@
    2b3e:	70ff           	moveq #-1,%d0
    2b40:	6000 0132      	braw 0x2c74
    2b44:	7000           	moveq #0,%d0
    2b46:	102e fffe      	moveb %fp@(-2),%d0
    2b4a:	3600           	movew %d0,%d3
    2b4c:	0240 0020      	andiw #32,%d0
    2b50:	6700 0080      	beqw 0x2bd2
    2b54:	1013           	moveb %a3@,%d0
    2b56:	4880           	extw %d0
    2b58:	b044           	cmpw %d4,%d0
    2b5a:	6638           	bnes 0x2b94
    2b5c:	486e ffff      	pea %fp@(-1)
    2b60:	4227           	clrb %sp@-
    2b62:	1f3c 0033      	moveb #51,%sp@-
    2b66:	4eba d618      	jsr %pc@(0x180)
    2b6a:	7200           	moveq #0,%d1
    2b6c:	3200           	movew %d0,%d1
    2b6e:	2601           	movel %d1,%d3
    2b70:	504f           	addqw #8,%sp
    2b72:	6700 ff4a      	beqw 0x2abe
    2b76:	7010           	moveq #16,%d0
    2b78:	8083           	orl %d3,%d0
    2b7a:	2480           	movel %d0,%a2@
    2b7c:	2f12           	movel %a2@,%sp@-
    2b7e:	4eba 089a      	jsr %pc@(0x341a)
    2b82:	5280           	addql #1,%d0
    2b84:	584f           	addqw #4,%sp
    2b86:	6606           	bnes 0x2b8e
    2b88:	0092 0000 4000 	oril #16384,%a2@
    2b8e:	70ff           	moveq #-1,%d0
    2b90:	6000 00e2      	braw 0x2c74
    2b94:	486e ffff      	pea %fp@(-1)
    2b98:	2f06           	movel %d6,%sp@-
    2b9a:	4eba d758      	jsr %pc@(0x2f4)
    2b9e:	7200           	moveq #0,%d1
    2ba0:	3200           	movew %d0,%d1
    2ba2:	2601           	movel %d1,%d3
    2ba4:	504f           	addqw #8,%sp
    2ba6:	671e           	beqs 0x2bc6
    2ba8:	7010           	moveq #16,%d0
    2baa:	8083           	orl %d3,%d0
    2bac:	2480           	movel %d0,%a2@
    2bae:	2f12           	movel %a2@,%sp@-
    2bb0:	4eba 0868      	jsr %pc@(0x341a)
    2bb4:	5280           	addql #1,%d0
    2bb6:	584f           	addqw #4,%sp
    2bb8:	6606           	bnes 0x2bc0
    2bba:	0092 0000 4000 	oril #16384,%a2@
    2bc0:	70ff           	moveq #-1,%d0
    2bc2:	6000 00b0      	braw 0x2c74
    2bc6:	0686 0000 0200 	addil #512,%d6
    2bcc:	5213           	addqb #1,%a3@
    2bce:	6000 feee      	braw 0x2abe
    2bd2:	3003           	movew %d3,%d0
    2bd4:	0240 0080      	andiw #128,%d0
    2bd8:	6738           	beqs 0x2c12
    2bda:	3003           	movew %d3,%d0
    2bdc:	0240 0040      	andiw #64,%d0
    2be0:	676c           	beqs 0x2c4e
    2be2:	486e ffff      	pea %fp@(-1)
    2be6:	2f07           	movel %d7,%sp@-
    2be8:	4eba d540      	jsr %pc@(0x12a)
    2bec:	4a40           	tstw %d0
    2bee:	504f           	addqw #8,%sp
    2bf0:	6606           	bnes 0x2bf8
    2bf2:	2047           	moveal %d7,%a0
    2bf4:	16a8 0015      	moveb %a0@(21),%a3@
    2bf8:	7001           	moveq #1,%d0
    2bfa:	2480           	movel %d0,%a2@
    2bfc:	2f12           	movel %a2@,%sp@-
    2bfe:	4eba 081a      	jsr %pc@(0x341a)
    2c02:	5280           	addql #1,%d0
    2c04:	584f           	addqw #4,%sp
    2c06:	6606           	bnes 0x2c0e
    2c08:	0092 0000 4000 	oril #16384,%a2@
    2c0e:	70ff           	moveq #-1,%d0
    2c10:	6062           	bras 0x2c74
    2c12:	3003           	movew %d3,%d0
    2c14:	0240 0001      	andiw #1,%d0
    2c18:	671a           	beqs 0x2c34
    2c1a:	7008           	moveq #8,%d0
    2c1c:	2480           	movel %d0,%a2@
    2c1e:	2f12           	movel %a2@,%sp@-
    2c20:	4eba 07f8      	jsr %pc@(0x341a)
    2c24:	5280           	addql #1,%d0
    2c26:	584f           	addqw #4,%sp
    2c28:	6606           	bnes 0x2c30
    2c2a:	0092 0000 4000 	oril #16384,%a2@
    2c30:	70ff           	moveq #-1,%d0
    2c32:	6040           	bras 0x2c74
    2c34:	7010           	moveq #16,%d0
    2c36:	2480           	movel %d0,%a2@
    2c38:	2f12           	movel %a2@,%sp@-
    2c3a:	4eba 07de      	jsr %pc@(0x341a)
    2c3e:	5280           	addql #1,%d0
    2c40:	584f           	addqw #4,%sp
    2c42:	6606           	bnes 0x2c4a
    2c44:	0092 0000 4000 	oril #16384,%a2@
    2c4a:	70ff           	moveq #-1,%d0
    2c4c:	6026           	bras 0x2c74
    2c4e:	1013           	moveb %a3@,%d0
    2c50:	4880           	extw %d0
    2c52:	b045           	cmpw %d5,%d0
    2c54:	6604           	bnes 0x2c5a
    2c56:	7000           	moveq #0,%d0
    2c58:	601a           	bras 0x2c74
    2c5a:	24bc 0000 2000 	movel #8192,%a2@
    2c60:	2f12           	movel %a2@,%sp@-
    2c62:	4eba 07b6      	jsr %pc@(0x341a)
    2c66:	5280           	addql #1,%d0
    2c68:	584f           	addqw #4,%sp
    2c6a:	6606           	bnes 0x2c72
    2c6c:	0092 0000 4000 	oril #16384,%a2@
    2c72:	70ff           	moveq #-1,%d0
    2c74:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    2c78:	4e5e           	unlk %fp
    2c7a:	4e75           	rts

```

# ReadFlashPage: 0x2c8e - 0x2e82


```

    2c8e:	4e56 ffbe      	linkw %fp,#-66
    2c92:	48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-
    2c96:	2c2e 0010      	movel %fp@(16),%d6
    2c9a:	2a2e 0014      	movel %fp@(20),%d5
    2c9e:	45ee ffde      	lea %fp@(-34),%a2
    2ca2:	47ee ffbe      	lea %fp@(-66),%a3
    2ca6:	2045           	moveal %d5,%a0
    2ca8:	4290           	clrl %a0@
    2caa:	16bc 0080      	moveb #-128,%a3@
    2cae:	422b 0001      	clrb %a3@(1)
    2cb2:	302e 0008      	movew %fp@(8),%d0
    2cb6:	e048           	lsrw #8,%d0
    2cb8:	1740 0002      	moveb %d0,%a3@(2)
    2cbc:	176e 0009 0003 	moveb %fp@(9),%a3@(3)
    2cc2:	177c 0020 0004 	moveb #32,%a3@(4)
    2cc8:	176e 000a 0005 	moveb %fp@(10),%a3@(5)
    2cce:	486e ffff      	pea %fp@(-1)
    2cd2:	4227           	clrb %sp@-
    2cd4:	2f0b           	movel %a3,%sp@-
    2cd6:	4eba d3f6      	jsr %pc@(0xce)
    2cda:	7200           	moveq #0,%d1
    2cdc:	3200           	movew %d0,%d1
    2cde:	2601           	movel %d1,%d3
    2ce0:	4fef 000a      	lea %sp@(10),%sp
    2ce4:	6710           	beqs 0x2cf6
    2ce6:	2803           	movel %d3,%d4
    2ce8:	2f04           	movel %d4,%sp@-
    2cea:	4eba 072e      	jsr %pc@(0x341a)
    2cee:	70ff           	moveq #-1,%d0
    2cf0:	584f           	addqw #4,%sp
    2cf2:	6000 0188      	braw 0x2e7c
    2cf6:	486e ffff      	pea %fp@(-1)
    2cfa:	4227           	clrb %sp@-
    2cfc:	1f3c 00aa      	moveb #-86,%sp@-
    2d00:	4eba d47e      	jsr %pc@(0x180)
    2d04:	7200           	moveq #0,%d1
    2d06:	3200           	movew %d0,%d1
    2d08:	2601           	movel %d1,%d3
    2d0a:	504f           	addqw #8,%sp
    2d0c:	6712           	beqs 0x2d20
    2d0e:	7810           	moveq #16,%d4
    2d10:	8883           	orl %d3,%d4
    2d12:	2f04           	movel %d4,%sp@-
    2d14:	4eba 0704      	jsr %pc@(0x341a)
    2d18:	70ff           	moveq #-1,%d0
    2d1a:	584f           	addqw #4,%sp
    2d1c:	6000 015e      	braw 0x2e7c
    2d20:	486e ffff      	pea %fp@(-1)
    2d24:	1f3c 0001      	moveb #1,%sp@-
    2d28:	4eba d4cc      	jsr %pc@(0x1f6)
    2d2c:	7200           	moveq #0,%d1
    2d2e:	3200           	movew %d0,%d1
    2d30:	2601           	movel %d1,%d3
    2d32:	7001           	moveq #1,%d0
    2d34:	b680           	cmpl %d0,%d3
    2d36:	5c4f           	addqw #6,%sp
    2d38:	6716           	beqs 0x2d50
    2d3a:	4a83           	tstl %d3
    2d3c:	660c           	bnes 0x2d4a
    2d3e:	7810           	moveq #16,%d4
    2d40:	8883           	orl %d3,%d4
    2d42:	2f04           	movel %d4,%sp@-
    2d44:	4eba 06d4      	jsr %pc@(0x341a)
    2d48:	584f           	addqw #4,%sp
    2d4a:	70ff           	moveq #-1,%d0
    2d4c:	6000 012e      	braw 0x2e7c
    2d50:	486e ffff      	pea %fp@(-1)
    2d54:	2f0a           	movel %a2,%sp@-
    2d56:	4eba d3d2      	jsr %pc@(0x12a)
    2d5a:	7200           	moveq #0,%d1
    2d5c:	3200           	movew %d0,%d1
    2d5e:	2601           	movel %d1,%d3
    2d60:	504f           	addqw #8,%sp
    2d62:	6712           	beqs 0x2d76
    2d64:	7810           	moveq #16,%d4
    2d66:	8883           	orl %d3,%d4
    2d68:	2f04           	movel %d4,%sp@-
    2d6a:	4eba 06ae      	jsr %pc@(0x341a)
    2d6e:	70ff           	moveq #-1,%d0
    2d70:	584f           	addqw #4,%sp
    2d72:	6000 0108      	braw 0x2e7c
    2d76:	7000           	moveq #0,%d0
    2d78:	102a 0001      	moveb %a2@(1),%d0
    2d7c:	3600           	movew %d0,%d3
    2d7e:	0240 0020      	andiw #32,%d0
    2d82:	6700 00d6      	beqw 0x2e5a
    2d86:	486e ffff      	pea %fp@(-1)
    2d8a:	2f2e 000c      	movel %fp@(12),%sp@-
    2d8e:	4eba d4fc      	jsr %pc@(0x28c)
    2d92:	7200           	moveq #0,%d1
    2d94:	3200           	movew %d0,%d1
    2d96:	2801           	movel %d1,%d4
    2d98:	504f           	addqw #8,%sp
    2d9a:	6712           	beqs 0x2dae
    2d9c:	7610           	moveq #16,%d3
    2d9e:	8684           	orl %d4,%d3
    2da0:	2f03           	movel %d3,%sp@-
    2da2:	4eba 0676      	jsr %pc@(0x341a)
    2da6:	70ff           	moveq #-1,%d0
    2da8:	584f           	addqw #4,%sp
    2daa:	6000 00d0      	braw 0x2e7c
    2dae:	4a86           	tstl %d6
    2db0:	6712           	beqs 0x2dc4
    2db2:	7600           	moveq #0,%d3
    2db4:	2646           	moveal %d6,%a3
    2db6:	16b2 3816      	moveb %a2@(00000016,%d3:l),%a3@
    2dba:	528b           	addql #1,%a3
    2dbc:	5283           	addql #1,%d3
    2dbe:	700a           	moveq #10,%d0
    2dc0:	b680           	cmpl %d0,%d3
    2dc2:	65f2           	bcss 0x2db6
    2dc4:	102a 0001      	moveb %a2@(1),%d0
    2dc8:	0240 0040      	andiw #64,%d0
    2dcc:	676a           	beqs 0x2e38
    2dce:	7000           	moveq #0,%d0
    2dd0:	102a 0003      	moveb %a2@(3),%d0
    2dd4:	3600           	movew %d0,%d3
    2dd6:	0240 0020      	andiw #32,%d0
    2dda:	6714           	beqs 0x2df0
    2ddc:	3003           	movew %d3,%d0
    2dde:	0240 0010      	andiw #16,%d0
    2de2:	6704           	beqs 0x2de8
    2de4:	7802           	moveq #2,%d4
    2de6:	600e           	bras 0x2df6
    2de8:	2045           	moveal %d5,%a0
    2dea:	7002           	moveq #2,%d0
    2dec:	2080           	movel %d0,%a0@
    2dee:	6006           	bras 0x2df6
    2df0:	2045           	moveal %d5,%a0
    2df2:	7001           	moveq #1,%d0
    2df4:	2080           	movel %d0,%a0@
    2df6:	4a86           	tstl %d6
    2df8:	6724           	beqs 0x2e1e
    2dfa:	7000           	moveq #0,%d0
    2dfc:	102a 0003      	moveb %a2@(3),%d0
    2e00:	3600           	movew %d0,%d3
    2e02:	0240 000a      	andiw #10,%d0
    2e06:	6716           	beqs 0x2e1e
    2e08:	3003           	movew %d3,%d0
    2e0a:	0240 0005      	andiw #5,%d0
    2e0e:	6706           	beqs 0x2e16
    2e10:	7002           	moveq #2,%d0
    2e12:	8880           	orl %d0,%d4
    2e14:	600e           	bras 0x2e24
    2e16:	2045           	moveal %d5,%a0
    2e18:	7008           	moveq #8,%d0
    2e1a:	8190           	orl %d0,%a0@
    2e1c:	6006           	bras 0x2e24
    2e1e:	2045           	moveal %d5,%a0
    2e20:	7004           	moveq #4,%d0
    2e22:	8190           	orl %d0,%a0@
    2e24:	7002           	moveq #2,%d0
    2e26:	c084           	andl %d4,%d0
    2e28:	6714           	beqs 0x2e3e
    2e2a:	2604           	movel %d4,%d3
    2e2c:	2f03           	movel %d3,%sp@-
    2e2e:	4eba 05ea      	jsr %pc@(0x341a)
    2e32:	70ff           	moveq #-1,%d0
    2e34:	584f           	addqw #4,%sp
    2e36:	6044           	bras 0x2e7c
    2e38:	2045           	moveal %d5,%a0
    2e3a:	7005           	moveq #5,%d0
    2e3c:	2080           	movel %d0,%a0@
    2e3e:	102a 0001      	moveb %a2@(1),%d0
    2e42:	0240 0080      	andiw #128,%d0
    2e46:	6704           	beqs 0x2e4c
    2e48:	7000           	moveq #0,%d0
    2e4a:	6030           	bras 0x2e7c
    2e4c:	7610           	moveq #16,%d3
    2e4e:	2f03           	movel %d3,%sp@-
    2e50:	4eba 05c8      	jsr %pc@(0x341a)
    2e54:	70ff           	moveq #-1,%d0
    2e56:	584f           	addqw #4,%sp
    2e58:	6022           	bras 0x2e7c
    2e5a:	3003           	movew %d3,%d0
    2e5c:	0240 0001      	andiw #1,%d0
    2e60:	670e           	beqs 0x2e70
    2e62:	7608           	moveq #8,%d3
    2e64:	2f03           	movel %d3,%sp@-
    2e66:	4eba 05b2      	jsr %pc@(0x341a)
    2e6a:	70ff           	moveq #-1,%d0
    2e6c:	584f           	addqw #4,%sp
    2e6e:	600c           	bras 0x2e7c
    2e70:	7610           	moveq #16,%d3
    2e72:	2f03           	movel %d3,%sp@-
    2e74:	4eba 05a4      	jsr %pc@(0x341a)
    2e78:	70ff           	moveq #-1,%d0
    2e7a:	584f           	addqw #4,%sp
    2e7c:	4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3
    2e80:	4e5e           	unlk %fp
    2e82:	4e75           	rts

```

# ReadFlashPages: 0x2e94 - 0x3278


```

    2e94:	4e56 ffb0      	linkw %fp,#-80
    2e98:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    2e9c:	162e 000a      	moveb %fp@(10),%d3
    2ea0:	2c2e 0016      	movel %fp@(22),%d6
    2ea4:	282e 001a      	movel %fp@(26),%d4
    2ea8:	2a2e 001e      	movel %fp@(30),%d5
    2eac:	45ee ffd0      	lea %fp@(-48),%a2
    2eb0:	41ee ffb0      	lea %fp@(-80),%a0
    2eb4:	2e08           	movel %a0,%d7
    2eb6:	1003           	moveb %d3,%d0
    2eb8:	d02e 000c      	addb %fp@(12),%d0
    2ebc:	5300           	subqb #1,%d0
    2ebe:	1d40 fff3      	moveb %d0,%fp@(-13)
    2ec2:	2d6e 000e fff4 	movel %fp@(14),%fp@(-12)
    2ec8:	2045           	moveal %d5,%a0
    2eca:	1083           	moveb %d3,%a0@
    2ecc:	2d6e 0012 fff8 	movel %fp@(18),%fp@(-8)
    2ed2:	7000           	moveq #0,%d0
    2ed4:	1003           	moveb %d3,%d0
    2ed6:	c0fc 000a      	muluw #10,%d0
    2eda:	d1ae fff8      	addl %d0,%fp@(-8)
    2ede:	2047           	moveal %d7,%a0
    2ee0:	10bc 0080      	moveb #-128,%a0@
    2ee4:	4228 0001      	clrb %a0@(1)
    2ee8:	302e 0008      	movew %fp@(8),%d0
    2eec:	e048           	lsrw #8,%d0
    2eee:	1140 0002      	moveb %d0,%a0@(2)
    2ef2:	116e 0009 0003 	moveb %fp@(9),%a0@(3)
    2ef8:	4228 0004      	clrb %a0@(4)
    2efc:	1143 0005      	moveb %d3,%a0@(5)
    2f00:	486e ffff      	pea %fp@(-1)
    2f04:	4227           	clrb %sp@-
    2f06:	2f07           	movel %d7,%sp@-
    2f08:	4eba d1c4      	jsr %pc@(0xce)
    2f0c:	7200           	moveq #0,%d1
    2f0e:	3200           	movew %d0,%d1
    2f10:	2601           	movel %d1,%d3
    2f12:	4fef 000a      	lea %sp@(10),%sp
    2f16:	671e           	beqs 0x2f36
    2f18:	2044           	moveal %d4,%a0
    2f1a:	2083           	movel %d3,%a0@
    2f1c:	2f10           	movel %a0@,%sp@-
    2f1e:	4eba 04fa      	jsr %pc@(0x341a)
    2f22:	5280           	addql #1,%d0
    2f24:	584f           	addqw #4,%sp
    2f26:	6608           	bnes 0x2f30
    2f28:	2044           	moveal %d4,%a0
    2f2a:	0090 0000 4000 	oril #16384,%a0@
    2f30:	70ff           	moveq #-1,%d0
    2f32:	6000 033e      	braw 0x3272
    2f36:	486e ffff      	pea %fp@(-1)
    2f3a:	4227           	clrb %sp@-
    2f3c:	1f3c 00aa      	moveb #-86,%sp@-
    2f40:	4eba d23e      	jsr %pc@(0x180)
    2f44:	7200           	moveq #0,%d1
    2f46:	3200           	movew %d0,%d1
    2f48:	2601           	movel %d1,%d3
    2f4a:	504f           	addqw #8,%sp
    2f4c:	6722           	beqs 0x2f70
    2f4e:	7010           	moveq #16,%d0
    2f50:	8083           	orl %d3,%d0
    2f52:	2044           	moveal %d4,%a0
    2f54:	2080           	movel %d0,%a0@
    2f56:	2f10           	movel %a0@,%sp@-
    2f58:	4eba 04c0      	jsr %pc@(0x341a)
    2f5c:	5280           	addql #1,%d0
    2f5e:	584f           	addqw #4,%sp
    2f60:	6608           	bnes 0x2f6a
    2f62:	2044           	moveal %d4,%a0
    2f64:	0090 0000 4000 	oril #16384,%a0@
    2f6a:	70ff           	moveq #-1,%d0
    2f6c:	6000 0304      	braw 0x3272
    2f70:	2046           	moveal %d6,%a0
    2f72:	7005           	moveq #5,%d0
    2f74:	2080           	movel %d0,%a0@
    2f76:	7000           	moveq #0,%d0
    2f78:	102e fff3      	moveb %fp@(-13),%d0
    2f7c:	3d40 fff0      	movew %d0,%fp@(-16)
    2f80:	3d40 fffc      	movew %d0,%fp@(-4)
    2f84:	486e ffff      	pea %fp@(-1)
    2f88:	1f3c 0001      	moveb #1,%sp@-
    2f8c:	4eba d268      	jsr %pc@(0x1f6)
    2f90:	7200           	moveq #0,%d1
    2f92:	3200           	movew %d0,%d1
    2f94:	2601           	movel %d1,%d3
    2f96:	7001           	moveq #1,%d0
    2f98:	b680           	cmpl %d0,%d3
    2f9a:	5c4f           	addqw #6,%sp
    2f9c:	6732           	beqs 0x2fd0
    2f9e:	4a83           	tstl %d3
    2fa0:	6628           	bnes 0x2fca
    2fa2:	486e ffff      	pea %fp@(-1)
    2fa6:	2f0a           	movel %a2,%sp@-
    2fa8:	4eba d180      	jsr %pc@(0x12a)
    2fac:	7010           	moveq #16,%d0
    2fae:	8083           	orl %d3,%d0
    2fb0:	2044           	moveal %d4,%a0
    2fb2:	2080           	movel %d0,%a0@
    2fb4:	2f10           	movel %a0@,%sp@-
    2fb6:	4eba 0462      	jsr %pc@(0x341a)
    2fba:	5280           	addql #1,%d0
    2fbc:	4fef 000c      	lea %sp@(12),%sp
    2fc0:	6608           	bnes 0x2fca
    2fc2:	2044           	moveal %d4,%a0
    2fc4:	0090 0000 4000 	oril #16384,%a0@
    2fca:	70ff           	moveq #-1,%d0
    2fcc:	6000 02a4      	braw 0x3272
    2fd0:	486e ffff      	pea %fp@(-1)
    2fd4:	2f0a           	movel %a2,%sp@-
    2fd6:	4eba d152      	jsr %pc@(0x12a)
    2fda:	7200           	moveq #0,%d1
    2fdc:	3200           	movew %d0,%d1
    2fde:	2601           	movel %d1,%d3
    2fe0:	504f           	addqw #8,%sp
    2fe2:	6722           	beqs 0x3006
    2fe4:	7010           	moveq #16,%d0
    2fe6:	8083           	orl %d3,%d0
    2fe8:	2044           	moveal %d4,%a0
    2fea:	2080           	movel %d0,%a0@
    2fec:	2f10           	movel %a0@,%sp@-
    2fee:	4eba 042a      	jsr %pc@(0x341a)
    2ff2:	5280           	addql #1,%d0
    2ff4:	584f           	addqw #4,%sp
    2ff6:	6608           	bnes 0x3000
    2ff8:	2044           	moveal %d4,%a0
    2ffa:	0090 0000 4000 	oril #16384,%a0@
    3000:	70ff           	moveq #-1,%d0
    3002:	6000 026e      	braw 0x3272
    3006:	7000           	moveq #0,%d0
    3008:	102a 0001      	moveb %a2@(1),%d0
    300c:	3600           	movew %d0,%d3
    300e:	0240 0020      	andiw #32,%d0
    3012:	6700 01e4      	beqw 0x31f8
    3016:	2045           	moveal %d5,%a0
    3018:	1010           	moveb %a0@,%d0
    301a:	4880           	extw %d0
    301c:	b06e fffc      	cmpw %fp@(-4),%d0
    3020:	6642           	bnes 0x3064
    3022:	3003           	movew %d3,%d0
    3024:	0240 0080      	andiw #128,%d0
    3028:	663a           	bnes 0x3064
    302a:	486e ffff      	pea %fp@(-1)
    302e:	4227           	clrb %sp@-
    3030:	1f3c 0033      	moveb #51,%sp@-
    3034:	4eba d14a      	jsr %pc@(0x180)
    3038:	7200           	moveq #0,%d1
    303a:	3200           	movew %d0,%d1
    303c:	2601           	movel %d1,%d3
    303e:	504f           	addqw #8,%sp
    3040:	6722           	beqs 0x3064
    3042:	7010           	moveq #16,%d0
    3044:	8083           	orl %d3,%d0
    3046:	2044           	moveal %d4,%a0
    3048:	2080           	movel %d0,%a0@
    304a:	2f10           	movel %a0@,%sp@-
    304c:	4eba 03cc      	jsr %pc@(0x341a)
    3050:	5280           	addql #1,%d0
    3052:	584f           	addqw #4,%sp
    3054:	6608           	bnes 0x305e
    3056:	2044           	moveal %d4,%a0
    3058:	0090 0000 4000 	oril #16384,%a0@
    305e:	70ff           	moveq #-1,%d0
    3060:	6000 0210      	braw 0x3272
    3064:	4aae 0012      	tstl %fp@(18)
    3068:	671a           	beqs 0x3084
    306a:	7600           	moveq #0,%d3
    306c:	266e fff8      	moveal %fp@(-8),%a3
    3070:	16b2 3816      	moveb %a2@(00000016,%d3:l),%a3@
    3074:	528b           	addql #1,%a3
    3076:	5283           	addql #1,%d3
    3078:	700a           	moveq #10,%d0
    307a:	b680           	cmpl %d0,%d3
    307c:	65f2           	bcss 0x3070
    307e:	700a           	moveq #10,%d0
    3080:	d1ae fff8      	addl %d0,%fp@(-8)
    3084:	486e ffff      	pea %fp@(-1)
    3088:	2f2e fff4      	movel %fp@(-12),%sp@-
    308c:	4eba d1fe      	jsr %pc@(0x28c)
    3090:	7200           	moveq #0,%d1
    3092:	3200           	movew %d0,%d1
    3094:	2601           	movel %d1,%d3
    3096:	504f           	addqw #8,%sp
    3098:	6722           	beqs 0x30bc
    309a:	7010           	moveq #16,%d0
    309c:	8083           	orl %d3,%d0
    309e:	2044           	moveal %d4,%a0
    30a0:	2080           	movel %d0,%a0@
    30a2:	2f10           	movel %a0@,%sp@-
    30a4:	4eba 0374      	jsr %pc@(0x341a)
    30a8:	5280           	addql #1,%d0
    30aa:	584f           	addqw #4,%sp
    30ac:	6608           	bnes 0x30b6
    30ae:	2044           	moveal %d4,%a0
    30b0:	0090 0000 4000 	oril #16384,%a0@
    30b6:	70ff           	moveq #-1,%d0
    30b8:	6000 01b8      	braw 0x3272
    30bc:	7000           	moveq #0,%d0
    30be:	102a 0001      	moveb %a2@(1),%d0
    30c2:	3600           	movew %d0,%d3
    30c4:	0240 0040      	andiw #64,%d0
    30c8:	6700 010a      	beqw 0x31d4
    30cc:	7600           	moveq #0,%d3
    30ce:	102a 0003      	moveb %a2@(3),%d0
    30d2:	0240 0020      	andiw #32,%d0
    30d6:	671a           	beqs 0x30f2
    30d8:	2046           	moveal %d6,%a0
    30da:	70fe           	moveq #-2,%d0
    30dc:	c190           	andl %d0,%a0@
    30de:	102a 0003      	moveb %a2@(3),%d0
    30e2:	0240 0010      	andiw #16,%d0
    30e6:	6704           	beqs 0x30ec
    30e8:	7602           	moveq #2,%d3
    30ea:	6006           	bras 0x30f2
    30ec:	2046           	moveal %d6,%a0
    30ee:	7002           	moveq #2,%d0
    30f0:	8190           	orl %d0,%a0@
    30f2:	4aae 0012      	tstl %fp@(18)
    30f6:	6724           	beqs 0x311c
    30f8:	102a 0003      	moveb %a2@(3),%d0
    30fc:	0240 000a      	andiw #10,%d0
    3100:	671a           	beqs 0x311c
    3102:	2046           	moveal %d6,%a0
    3104:	70fb           	moveq #-5,%d0
    3106:	c190           	andl %d0,%a0@
    3108:	102a 0003      	moveb %a2@(3),%d0
    310c:	0240 0005      	andiw #5,%d0
    3110:	6704           	beqs 0x3116
    3112:	7602           	moveq #2,%d3
    3114:	6006           	bras 0x311c
    3116:	2046           	moveal %d6,%a0
    3118:	7008           	moveq #8,%d0
    311a:	8190           	orl %d0,%a0@
    311c:	7002           	moveq #2,%d0
    311e:	b680           	cmpl %d0,%d3
    3120:	661e           	bnes 0x3140
    3122:	2044           	moveal %d4,%a0
    3124:	2083           	movel %d3,%a0@
    3126:	2f10           	movel %a0@,%sp@-
    3128:	4eba 02f0      	jsr %pc@(0x341a)
    312c:	5280           	addql #1,%d0
    312e:	584f           	addqw #4,%sp
    3130:	6608           	bnes 0x313a
    3132:	2044           	moveal %d4,%a0
    3134:	0090 0000 4000 	oril #16384,%a0@
    313a:	70ff           	moveq #-1,%d0
    313c:	6000 0134      	braw 0x3272
    3140:	102a 0001      	moveb %a2@(1),%d0
    3144:	0240 0080      	andiw #128,%d0
    3148:	6700 0092      	beqw 0x31dc
    314c:	2045           	moveal %d5,%a0
    314e:	1010           	moveb %a0@,%d0
    3150:	4880           	extw %d0
    3152:	b06e fffc      	cmpw %fp@(-4),%d0
    3156:	6700 00ec      	beqw 0x3244
    315a:	1010           	moveb %a0@,%d0
    315c:	5200           	addqb #1,%d0
    315e:	2047           	moveal %d7,%a0
    3160:	1140 0005      	moveb %d0,%a0@(5)
    3164:	486e ffff      	pea %fp@(-1)
    3168:	4227           	clrb %sp@-
    316a:	2f07           	movel %d7,%sp@-
    316c:	4eba cf60      	jsr %pc@(0xce)
    3170:	7200           	moveq #0,%d1
    3172:	3200           	movew %d0,%d1
    3174:	2601           	movel %d1,%d3
    3176:	4fef 000a      	lea %sp@(10),%sp
    317a:	671e           	beqs 0x319a
    317c:	2044           	moveal %d4,%a0
    317e:	2083           	movel %d3,%a0@
    3180:	2f10           	movel %a0@,%sp@-
    3182:	4eba 0296      	jsr %pc@(0x341a)
    3186:	5280           	addql #1,%d0
    3188:	584f           	addqw #4,%sp
    318a:	6608           	bnes 0x3194
    318c:	2044           	moveal %d4,%a0
    318e:	0090 0000 4000 	oril #16384,%a0@
    3194:	70ff           	moveq #-1,%d0
    3196:	6000 00da      	braw 0x3272
    319a:	486e ffff      	pea %fp@(-1)
    319e:	4227           	clrb %sp@-
    31a0:	1f3c 00aa      	moveb #-86,%sp@-
    31a4:	4eba cfda      	jsr %pc@(0x180)
    31a8:	7200           	moveq #0,%d1
    31aa:	3200           	movew %d0,%d1
    31ac:	2601           	movel %d1,%d3
    31ae:	504f           	addqw #8,%sp
    31b0:	672a           	beqs 0x31dc
    31b2:	7010           	moveq #16,%d0
    31b4:	8083           	orl %d3,%d0
    31b6:	2044           	moveal %d4,%a0
    31b8:	2080           	movel %d0,%a0@
    31ba:	2f10           	movel %a0@,%sp@-
    31bc:	4eba 025c      	jsr %pc@(0x341a)
    31c0:	5280           	addql #1,%d0
    31c2:	584f           	addqw #4,%sp
    31c4:	6608           	bnes 0x31ce
    31c6:	2044           	moveal %d4,%a0
    31c8:	0090 0000 4000 	oril #16384,%a0@
    31ce:	70ff           	moveq #-1,%d0
    31d0:	6000 00a0      	braw 0x3272
    31d4:	3003           	movew %d3,%d0
    31d6:	0240 0080      	andiw #128,%d0
    31da:	6668           	bnes 0x3244
    31dc:	2045           	moveal %d5,%a0
    31de:	1010           	moveb %a0@,%d0
    31e0:	4880           	extw %d0
    31e2:	b06e fffc      	cmpw %fp@(-4),%d0
    31e6:	6c00 fd9c      	bgew 0x2f84
    31ea:	06ae 0000 0200 	addil #512,%fp@(-12)
    31f0:	fff4 
    31f2:	5210           	addqb #1,%a0@
    31f4:	6000 fd8e      	braw 0x2f84
    31f8:	3003           	movew %d3,%d0
    31fa:	0240 0080      	andiw #128,%d0
    31fe:	6644           	bnes 0x3244
    3200:	3003           	movew %d3,%d0
    3202:	0240 0001      	andiw #1,%d0
    3206:	671e           	beqs 0x3226
    3208:	2044           	moveal %d4,%a0
    320a:	7008           	moveq #8,%d0
    320c:	2080           	movel %d0,%a0@
    320e:	2f10           	movel %a0@,%sp@-
    3210:	4eba 0208      	jsr %pc@(0x341a)
    3214:	5280           	addql #1,%d0
    3216:	584f           	addqw #4,%sp
    3218:	6608           	bnes 0x3222
    321a:	2044           	moveal %d4,%a0
    321c:	0090 0000 4000 	oril #16384,%a0@
    3222:	70ff           	moveq #-1,%d0
    3224:	604c           	bras 0x3272
    3226:	2044           	moveal %d4,%a0
    3228:	7010           	moveq #16,%d0
    322a:	2080           	movel %d0,%a0@
    322c:	2f10           	movel %a0@,%sp@-
    322e:	4eba 01ea      	jsr %pc@(0x341a)
    3232:	5280           	addql #1,%d0
    3234:	584f           	addqw #4,%sp
    3236:	6608           	bnes 0x3240
    3238:	2044           	moveal %d4,%a0
    323a:	0090 0000 4000 	oril #16384,%a0@
    3240:	70ff           	moveq #-1,%d0
    3242:	602e           	bras 0x3272
    3244:	2045           	moveal %d5,%a0
    3246:	1010           	moveb %a0@,%d0
    3248:	4880           	extw %d0
    324a:	b06e fff0      	cmpw %fp@(-16),%d0
    324e:	6604           	bnes 0x3254
    3250:	7000           	moveq #0,%d0
    3252:	601e           	bras 0x3272
    3254:	2044           	moveal %d4,%a0
    3256:	20bc 0000 2000 	movel #8192,%a0@
    325c:	2f10           	movel %a0@,%sp@-
    325e:	4eba 01ba      	jsr %pc@(0x341a)
    3262:	5280           	addql #1,%d0
    3264:	584f           	addqw #4,%sp
    3266:	6608           	bnes 0x3270
    3268:	2044           	moveal %d4,%a0
    326a:	0090 0000 4000 	oril #16384,%a0@
    3270:	70ff           	moveq #-1,%d0
    3272:	4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3
    3276:	4e5e           	unlk %fp
    3278:	4e75           	rts

```

# EraseAllPages: 0x328c - 0x3408


```

    328c:	4e56 ffde      	linkw %fp,#-34
    3290:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    3294:	246e 000a      	moveal %fp@(10),%a2
    3298:	47ee ffde      	lea %fp@(-34),%a3
    329c:	16bc 0080      	moveb #-128,%a3@
    32a0:	422b 0001      	clrb %a3@(1)
    32a4:	302e 0008      	movew %fp@(8),%d0
    32a8:	e048           	lsrw #8,%d0
    32aa:	1740 0002      	moveb %d0,%a3@(2)
    32ae:	176e 0009 0003 	moveb %fp@(9),%a3@(3)
    32b4:	177c 0020 0004 	moveb #32,%a3@(4)
    32ba:	422b 0005      	clrb %a3@(5)
    32be:	486e ffff      	pea %fp@(-1)
    32c2:	4227           	clrb %sp@-
    32c4:	2f0b           	movel %a3,%sp@-
    32c6:	4eba ce06      	jsr %pc@(0xce)
    32ca:	7200           	moveq #0,%d1
    32cc:	3200           	movew %d0,%d1
    32ce:	2601           	movel %d1,%d3
    32d0:	4fef 000a      	lea %sp@(10),%sp
    32d4:	671a           	beqs 0x32f0
    32d6:	2483           	movel %d3,%a2@
    32d8:	2f12           	movel %a2@,%sp@-
    32da:	4eba 013e      	jsr %pc@(0x341a)
    32de:	5280           	addql #1,%d0
    32e0:	584f           	addqw #4,%sp
    32e2:	6606           	bnes 0x32ea
    32e4:	0092 0000 4000 	oril #16384,%a2@
    32ea:	70ff           	moveq #-1,%d0
    32ec:	6000 0114      	braw 0x3402
    32f0:	486e ffff      	pea %fp@(-1)
    32f4:	4227           	clrb %sp@-
    32f6:	1f3c 0099      	moveb #-103,%sp@-
    32fa:	4eba ce84      	jsr %pc@(0x180)
    32fe:	7200           	moveq #0,%d1
    3300:	3200           	movew %d0,%d1
    3302:	2601           	movel %d1,%d3
    3304:	504f           	addqw #8,%sp
    3306:	671e           	beqs 0x3326
    3308:	7010           	moveq #16,%d0
    330a:	8083           	orl %d3,%d0
    330c:	2480           	movel %d0,%a2@
    330e:	2f12           	movel %a2@,%sp@-
    3310:	4eba 0108      	jsr %pc@(0x341a)
    3314:	5280           	addql #1,%d0
    3316:	584f           	addqw #4,%sp
    3318:	6606           	bnes 0x3320
    331a:	0092 0000 4000 	oril #16384,%a2@
    3320:	70ff           	moveq #-1,%d0
    3322:	6000 00de      	braw 0x3402
    3326:	486e ffff      	pea %fp@(-1)
    332a:	1f3c 0003      	moveb #3,%sp@-
    332e:	4eba cec6      	jsr %pc@(0x1f6)
    3332:	7200           	moveq #0,%d1
    3334:	3200           	movew %d0,%d1
    3336:	2601           	movel %d1,%d3
    3338:	7001           	moveq #1,%d0
    333a:	b680           	cmpl %d0,%d3
    333c:	5c4f           	addqw #6,%sp
    333e:	6722           	beqs 0x3362
    3340:	4a83           	tstl %d3
    3342:	6618           	bnes 0x335c
    3344:	7010           	moveq #16,%d0
    3346:	8083           	orl %d3,%d0
    3348:	2480           	movel %d0,%a2@
    334a:	2f12           	movel %a2@,%sp@-
    334c:	4eba 00cc      	jsr %pc@(0x341a)
    3350:	5280           	addql #1,%d0
    3352:	584f           	addqw #4,%sp
    3354:	6606           	bnes 0x335c
    3356:	0092 0000 4000 	oril #16384,%a2@
    335c:	70ff           	moveq #-1,%d0
    335e:	6000 00a2      	braw 0x3402
    3362:	486e ffff      	pea %fp@(-1)
    3366:	486e fffe      	pea %fp@(-2)
    336a:	4eba cd04      	jsr %pc@(0x70)
    336e:	7200           	moveq #0,%d1
    3370:	3200           	movew %d0,%d1
    3372:	2601           	movel %d1,%d3
    3374:	504f           	addqw #8,%sp
    3376:	671c           	beqs 0x3394
    3378:	7010           	moveq #16,%d0
    337a:	8083           	orl %d3,%d0
    337c:	2480           	movel %d0,%a2@
    337e:	2f12           	movel %a2@,%sp@-
    3380:	4eba 0098      	jsr %pc@(0x341a)
    3384:	5280           	addql #1,%d0
    3386:	584f           	addqw #4,%sp
    3388:	6606           	bnes 0x3390
    338a:	0092 0000 4000 	oril #16384,%a2@
    3390:	70ff           	moveq #-1,%d0
    3392:	606e           	bras 0x3402
    3394:	7000           	moveq #0,%d0
    3396:	102e fffe      	moveb %fp@(-2),%d0
    339a:	3600           	movew %d0,%d3
    339c:	0240 0080      	andiw #128,%d0
    33a0:	6726           	beqs 0x33c8
    33a2:	3003           	movew %d3,%d0
    33a4:	0240 0040      	andiw #64,%d0
    33a8:	6604           	bnes 0x33ae
    33aa:	7000           	moveq #0,%d0
    33ac:	6054           	bras 0x3402
    33ae:	7004           	moveq #4,%d0
    33b0:	2480           	movel %d0,%a2@
    33b2:	2f12           	movel %a2@,%sp@-
    33b4:	4eba 0064      	jsr %pc@(0x341a)
    33b8:	5280           	addql #1,%d0
    33ba:	584f           	addqw #4,%sp
    33bc:	6606           	bnes 0x33c4
    33be:	0092 0000 4000 	oril #16384,%a2@
    33c4:	70ff           	moveq #-1,%d0
    33c6:	603a           	bras 0x3402
    33c8:	3003           	movew %d3,%d0
    33ca:	0240 0001      	andiw #1,%d0
    33ce:	671a           	beqs 0x33ea
    33d0:	7008           	moveq #8,%d0
    33d2:	2480           	movel %d0,%a2@
    33d4:	2f12           	movel %a2@,%sp@-
    33d6:	4eba 0042      	jsr %pc@(0x341a)
    33da:	5280           	addql #1,%d0
    33dc:	584f           	addqw #4,%sp
    33de:	6606           	bnes 0x33e6
    33e0:	0092 0000 4000 	oril #16384,%a2@
    33e6:	70ff           	moveq #-1,%d0
    33e8:	6018           	bras 0x3402
    33ea:	7010           	moveq #16,%d0
    33ec:	2480           	movel %d0,%a2@
    33ee:	2f12           	movel %a2@,%sp@-
    33f0:	4eba 0028      	jsr %pc@(0x341a)
    33f4:	5280           	addql #1,%d0
    33f6:	584f           	addqw #4,%sp
    33f8:	6606           	bnes 0x3400
    33fa:	0092 0000 4000 	oril #16384,%a2@
    3400:	70ff           	moveq #-1,%d0
    3402:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
    3406:	4e5e           	unlk %fp
    3408:	4e75           	rts

```

# Restore: 0x341a - 0x3578


```

    341a:	4e56 ffde      	linkw %fp,#-34
    341e:	2f0a           	movel %a2,%sp@-
    3420:	2f03           	movel %d3,%sp@-
    3422:	262e 0008      	movel %fp@(8),%d3
    3426:	45ee ffde      	lea %fp@(-34),%a2
    342a:	2003           	movel %d3,%d0
    342c:	0280 0000 2008 	andil #8200,%d0
    3432:	670a           	beqs 0x343e
    3434:	486e fffe      	pea %fp@(-2)
    3438:	4eba ef20      	jsr %pc@(0x235a)
    343c:	584f           	addqw #4,%sp
    343e:	2003           	movel %d3,%d0
    3440:	0280 0000 2018 	andil #8216,%d0
    3446:	671a           	beqs 0x3462
    3448:	486e ffff      	pea %fp@(-1)
    344c:	4227           	clrb %sp@-
    344e:	1f3c 003c      	moveb #60,%sp@-
    3452:	4eba cd2c      	jsr %pc@(0x180)
    3456:	4a40           	tstw %d0
    3458:	504f           	addqw #8,%sp
    345a:	6706           	beqs 0x3462
    345c:	70ff           	moveq #-1,%d0
    345e:	6000 0112      	braw 0x3572
    3462:	7010           	moveq #16,%d0
    3464:	c083           	andl %d3,%d0
    3466:	672c           	beqs 0x3494
    3468:	486e ffff      	pea %fp@(-1)
    346c:	4227           	clrb %sp@-
    346e:	1f3c 00cc      	moveb #-52,%sp@-
    3472:	4eba cd0c      	jsr %pc@(0x180)
    3476:	4a40           	tstw %d0
    3478:	504f           	addqw #8,%sp
    347a:	6612           	bnes 0x348e
    347c:	486e ffff      	pea %fp@(-1)
    3480:	1f3c 0004      	moveb #4,%sp@-
    3484:	4eba cd70      	jsr %pc@(0x1f6)
    3488:	5340           	subqw #1,%d0
    348a:	5c4f           	addqw #6,%sp
    348c:	6706           	beqs 0x3494
    348e:	70ff           	moveq #-1,%d0
    3490:	6000 00e0      	braw 0x3572
    3494:	486e ffff      	pea %fp@(-1)
    3498:	2f0a           	movel %a2,%sp@-
    349a:	4eba cc8e      	jsr %pc@(0x12a)
    349e:	4a40           	tstw %d0
    34a0:	504f           	addqw #8,%sp
    34a2:	6706           	beqs 0x34aa
    34a4:	70ff           	moveq #-1,%d0
    34a6:	6000 00ca      	braw 0x3572
    34aa:	102a 0002      	moveb %a2@(2),%d0
    34ae:	0240 0080      	andiw #128,%d0
    34b2:	671a           	beqs 0x34ce
    34b4:	486e ffff      	pea %fp@(-1)
    34b8:	4227           	clrb %sp@-
    34ba:	1f3c 003c      	moveb #60,%sp@-
    34be:	4eba ccc0      	jsr %pc@(0x180)
    34c2:	4a40           	tstw %d0
    34c4:	504f           	addqw #8,%sp
    34c6:	6706           	beqs 0x34ce
    34c8:	70ff           	moveq #-1,%d0
    34ca:	6000 00a6      	braw 0x3572
    34ce:	102a 0002      	moveb %a2@(2),%d0
    34d2:	0240 0040      	andiw #64,%d0
    34d6:	672a           	beqs 0x3502
    34d8:	486e ffff      	pea %fp@(-1)
    34dc:	4227           	clrb %sp@-
    34de:	1f3c 00cc      	moveb #-52,%sp@-
    34e2:	4eba cc9c      	jsr %pc@(0x180)
    34e6:	4a40           	tstw %d0
    34e8:	504f           	addqw #8,%sp
    34ea:	6612           	bnes 0x34fe
    34ec:	486e ffff      	pea %fp@(-1)
    34f0:	1f3c 0004      	moveb #4,%sp@-
    34f4:	4eba cd00      	jsr %pc@(0x1f6)
    34f8:	5340           	subqw #1,%d0
    34fa:	5c4f           	addqw #6,%sp
    34fc:	6704           	beqs 0x3502
    34fe:	70ff           	moveq #-1,%d0
    3500:	6070           	bras 0x3572
    3502:	7000           	moveq #0,%d0
    3504:	102a 0002      	moveb %a2@(2),%d0
    3508:	3600           	movew %d0,%d3
    350a:	0240 0010      	andiw #16,%d0
    350e:	6608           	bnes 0x3518
    3510:	3003           	movew %d3,%d0
    3512:	0240 0020      	andiw #32,%d0
    3516:	662a           	bnes 0x3542
    3518:	486e ffff      	pea %fp@(-1)
    351c:	4227           	clrb %sp@-
    351e:	1f3c 00c3      	moveb #-61,%sp@-
    3522:	4eba cc5c      	jsr %pc@(0x180)
    3526:	4a40           	tstw %d0
    3528:	504f           	addqw #8,%sp
    352a:	6612           	bnes 0x353e
    352c:	486e ffff      	pea %fp@(-1)
    3530:	1f3c 0004      	moveb #4,%sp@-
    3534:	4eba ccc0      	jsr %pc@(0x1f6)
    3538:	5340           	subqw #1,%d0
    353a:	5c4f           	addqw #6,%sp
    353c:	6704           	beqs 0x3542
    353e:	70ff           	moveq #-1,%d0
    3540:	6030           	bras 0x3572
    3542:	486e ffff      	pea %fp@(-1)
    3546:	2f0a           	movel %a2,%sp@-
    3548:	4eba cbe0      	jsr %pc@(0x12a)
    354c:	4a40           	tstw %d0
    354e:	504f           	addqw #8,%sp
    3550:	6704           	beqs 0x3556
    3552:	70ff           	moveq #-1,%d0
    3554:	601c           	bras 0x3572
    3556:	7000           	moveq #0,%d0
    3558:	102a 0002      	moveb %a2@(2),%d0
    355c:	3600           	movew %d0,%d3
    355e:	0240 00d0      	andiw #208,%d0
    3562:	660c           	bnes 0x3570
    3564:	3003           	movew %d3,%d0
    3566:	0240 0020      	andiw #32,%d0
    356a:	6704           	beqs 0x3570
    356c:	7000           	moveq #0,%d0
    356e:	6002           	bras 0x3572
    3570:	70ff           	moveq #-1,%d0
    3572:	261f           	movel %sp@+,%d3
    3574:	245f           	moveal %sp@+,%a2
    3576:	4e5e           	unlk %fp
    3578:	4e75           	rts

```

# PrvShouldWeInstall: 0x3584 - 0x358c


```

    3584:	4e56 0000      	linkw %fp,#0
    3588:	7000           	moveq #0,%d0
    358a:	4e5e           	unlk %fp
    358c:	4e75           	rts

```

# MemoryStickTimerProc: 0x35a4 - 0x35b0


```

    35a4:	4e56 0000      	linkw %fp,#0
    35a8:	4227           	clrb %sp@-
    35aa:	4eba eeb8      	jsr %pc@(0x2464)
    35ae:	4e5e           	unlk %fp
    35b0:	4e75           	rts

```

# #MemoryStickInsertedNotificationProc: 0x35ca - 0x35f2


```

    35ca:	4e56 0000      	linkw %fp,#0
    35ce:	2f0b           	movel %a3,%sp@-
    35d0:	222e 0008      	movel %fp@(8),%d1
    35d4:	2041           	moveal %d1,%a0
    35d6:	2668 000c      	moveal %a0@(12),%a3
    35da:	224b           	moveal %a3,%a1
    35dc:	3028 000a      	movew %a0@(10),%d0
    35e0:	b06b 4374      	cmpw %a3@(17268),%d0
    35e4:	6606           	bnes 0x35ec
    35e6:	0229 00fd 437a 	andib #-3,%a1@(17274)
    35ec:	7000           	moveq #0,%d0
    35ee:	265f           	moveal %sp@+,%a3
    35f0:	4e5e           	unlk %fp
    35f2:	4e75           	rts

```

# "MemoryStickRemovedNotificationProc: 0x361c - 0x3644


```

    361c:	4e56 0000      	linkw %fp,#0
    3620:	2f0b           	movel %a3,%sp@-
    3622:	222e 0008      	movel %fp@(8),%d1
    3626:	2041           	moveal %d1,%a0
    3628:	2668 000c      	moveal %a0@(12),%a3
    362c:	224b           	moveal %a3,%a1
    362e:	3028 000a      	movew %a0@(10),%d0
    3632:	b06b 4374      	cmpw %a3@(17268),%d0
    3636:	6606           	bnes 0x363e
    3638:	0229 00fb 437a 	andib #-5,%a1@(17274)
    363e:	7000           	moveq #0,%d0
    3640:	265f           	moveal %sp@+,%a3
    3642:	4e5e           	unlk %fp
    3644:	4e75           	rts

```

# SlotOpen: 0x366c - 0x39a8


```

    366c:	4e56 ffcc      	linkw %fp,#-52
    3670:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    3674:	362e 0008      	movew %fp@(8),%d3
    3678:	2d7c 536c 4d73 	movel #1399606643,%fp@(-4)
    367e:	fffc 
    3680:	486e ffee      	pea %fp@(-18)
    3684:	3f3c 0002      	movew #2,%sp@-
    3688:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    368e:	4e4f           	trap #15
    3690:	a27b           	0121173
    3692:	4a40           	tstw %d0
    3694:	4fef 000a      	lea %sp@(10),%sp
    3698:	6720           	beqs 0x36ba
    369a:	41fa 031a      	lea %pc@(0x39b6),%a0
    369e:	4850           	pea %a0@
    36a0:	3f3c 0109      	movew #265,%sp@-
    36a4:	41fa 0320      	lea %pc@(0x39c6),%a0
    36a8:	4850           	pea %a0@
    36aa:	4e4f           	trap #15
    36ac:	a084           	0120204
    36ae:	303c 2901      	movew #10497,%d0
    36b2:	4fef 000a      	lea %sp@(10),%sp
    36b6:	6000 02ea      	braw 0x39a2
    36ba:	7001           	moveq #1,%d0
    36bc:	b0ae ffee      	cmpl %fp@(-18),%d0
    36c0:	6720           	beqs 0x36e2
    36c2:	41fa 0310      	lea %pc@(0x39d4),%a0
    36c6:	4850           	pea %a0@
    36c8:	3f3c 010d      	movew #269,%sp@-
    36cc:	41fa 0316      	lea %pc@(0x39e4),%a0
    36d0:	4850           	pea %a0@
    36d2:	4e4f           	trap #15
    36d4:	a084           	0120204
    36d6:	303c 2901      	movew #10497,%d0
    36da:	4fef 000a      	lea %sp@(10),%sp
    36de:	6000 02c2      	braw 0x39a2
    36e2:	3f03           	movew %d3,%sp@-
    36e4:	4e4f           	trap #15
    36e6:	a0b9           	0120271
    36e8:	2468 0004      	moveal %a0@(4),%a2
    36ec:	200a           	movel %a2,%d0
    36ee:	544f           	addqw #2,%sp
    36f0:	6600 02ac      	bnew 0x399e
    36f4:	4878 438a      	pea 0x438a
    36f8:	4e4f           	trap #15
    36fa:	a013           	0120023
    36fc:	2448           	moveal %a0,%a2
    36fe:	200a           	movel %a2,%d0
    3700:	584f           	addqw #4,%sp
    3702:	6608           	bnes 0x370c
    3704:	303c 0102      	movew #258,%d0
    3708:	6000 0298      	braw 0x39a2
    370c:	4267           	clrw %sp@-
    370e:	2f0a           	movel %a2,%sp@-
    3710:	4e4f           	trap #15
    3712:	a01b           	0120033
    3714:	4227           	clrb %sp@-
    3716:	4878 438a      	pea 0x438a
    371a:	2f0a           	movel %a2,%sp@-
    371c:	4e4f           	trap #15
    371e:	a027           	0120047
    3720:	3f03           	movew %d3,%sp@-
    3722:	4e4f           	trap #15
    3724:	a0b9           	0120271
    3726:	214a 0004      	movel %a2,%a0@(4)
    372a:	486a 0014      	pea %a2@(20)
    372e:	2f3c 0000 80c0 	movel #32960,%sp@-
    3734:	3f3c 0001      	movew #1,%sp@-
    3738:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    373e:	4e4f           	trap #15
    3740:	a35a           	0121532
    3742:	4aaa 0014      	tstl %a2@(20)
    3746:	4fef 0020      	lea %sp@(32),%sp
    374a:	661a           	bnes 0x3766
    374c:	3f03           	movew %d3,%sp@-
    374e:	4e4f           	trap #15
    3750:	a0b9           	0120271
    3752:	42a8 0004      	clrl %a0@(4)
    3756:	2f0a           	movel %a2,%sp@-
    3758:	4e4f           	trap #15
    375a:	a012           	0120022
    375c:	303c 0102      	movew #258,%d0
    3760:	5c4f           	addqw #6,%sp
    3762:	6000 023e      	braw 0x39a2
    3766:	486e fff2      	pea %fp@(-14)
    376a:	3f3c 4e22      	movew #20002,%sp@-
    376e:	2f3c 5368 416c 	movel #1399341420,%sp@-
    3774:	4e4f           	trap #15
    3776:	a27b           	0121173
    3778:	4aae fff2      	tstl %fp@(-14)
    377c:	4fef 000a      	lea %sp@(10),%sp
    3780:	6608           	bnes 0x378a
    3782:	303c 2900      	movew #10496,%d0
    3786:	6000 021a      	braw 0x39a2
    378a:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    3790:	487a 13e2      	pea %pc@(0x4b74)
    3794:	3f3c 0003      	movew #3,%sp@-
    3798:	1f3c 0001      	moveb #1,%sp@-
    379c:	206e fff2      	moveal %fp@(-14),%a0
    37a0:	4e90           	jsr %a0@
    37a2:	2078 0364      	moveal 0x364,%a0
    37a6:	7007           	moveq #7,%d0
    37a8:	c0e8 0002      	muluw %a0@(2),%d0
    37ac:	5a40           	addqw #5,%d0
    37ae:	7200           	moveq #0,%d1
    37b0:	3200           	movew %d0,%d1
    37b2:	82fc 000a      	divuw #10,%d1
    37b6:	3541 4386      	movew %d1,%a2@(17286)
    37ba:	0238 00df f431 	andib #-33,0xfffffffffffff431
    37c0:	4878 0014      	pea 0x14
    37c4:	4e4f           	trap #15
    37c6:	a249           	0121111
    37c8:	0038 0020 f431 	orib #32,0xfffffffffffff431
    37ce:	486e ffed      	pea %fp@(-19)
    37d2:	4eba eb1e      	jsr %pc@(0x22f2)
    37d6:	42a7           	clrl %sp@-
    37d8:	42a7           	clrl %sp@-
    37da:	487a fdc8      	pea %pc@(0x35a4)
    37de:	486e fffc      	pea %fp@(-4)
    37e2:	486a 0010      	pea %a2@(16)
    37e6:	4e4f           	trap #15
    37e8:	a099           	0120231
    37ea:	3800           	movew %d0,%d4
    37ec:	4fef 0028      	lea %sp@(40),%sp
    37f0:	6728           	beqs 0x381a
    37f2:	3f3c 0001      	movew #1,%sp@-
    37f6:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    37fc:	4e4f           	trap #15
    37fe:	a35b           	0121533
    3800:	3f03           	movew %d3,%sp@-
    3802:	4e4f           	trap #15
    3804:	a0b9           	0120271
    3806:	42a8 0004      	clrl %a0@(4)
    380a:	2f0a           	movel %a2,%sp@-
    380c:	4e4f           	trap #15
    380e:	a012           	0120022
    3810:	3004           	movew %d4,%d0
    3812:	4fef 000c      	lea %sp@(12),%sp
    3816:	6000 018a      	braw 0x39a2
    381a:	4ab8 0304      	tstl 0x304
    381e:	665e           	bnes 0x387e
    3820:	4878 0078      	pea 0x78
    3824:	4e4f           	trap #15
    3826:	a013           	0120023
    3828:	21c8 0304      	movel %a0,0x304
    382c:	584f           	addqw #4,%sp
    382e:	6632           	bnes 0x3862
    3830:	2f2a 0010      	movel %a2@(16),%sp@-
    3834:	4e4f           	trap #15
    3836:	a0bf           	0120277
    3838:	3f3c 0001      	movew #1,%sp@-
    383c:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    3842:	4e4f           	trap #15
    3844:	a35b           	0121533
    3846:	3f03           	movew %d3,%sp@-
    3848:	4e4f           	trap #15
    384a:	a0b9           	0120271
    384c:	42a8 0004      	clrl %a0@(4)
    3850:	2f0a           	movel %a2,%sp@-
    3852:	4e4f           	trap #15
    3854:	a012           	0120022
    3856:	303c 0102      	movew #258,%d0
    385a:	4fef 0010      	lea %sp@(16),%sp
    385e:	6000 0142      	braw 0x39a2
    3862:	4267           	clrw %sp@-
    3864:	2f38 0304      	movel 0x304,%sp@-
    3868:	4e4f           	trap #15
    386a:	a01b           	0120033
    386c:	4227           	clrb %sp@-
    386e:	4878 0078      	pea 0x78
    3872:	2f38 0304      	movel 0x304,%sp@-
    3876:	4e4f           	trap #15
    3878:	a027           	0120047
    387a:	4fef 0010      	lea %sp@(16),%sp
    387e:	2078 0304      	moveal 0x304,%a0
    3882:	214a 0024      	movel %a2,%a0@(36)
    3886:	2078 0304      	moveal 0x304,%a0
    388a:	2668 0010      	moveal %a0@(16),%a3
    388e:	200b           	movel %a3,%d0
    3890:	6706           	beqs 0x3898
    3892:	377c 0001 0024 	movew #1,%a3@(36)
    3898:	486e fffc      	pea %fp@(-4)
    389c:	486a 437e      	pea %a2@(17278)
    38a0:	4e4f           	trap #15
    38a2:	a0ae           	0120256
    38a4:	3800           	movew %d0,%d4
    38a6:	504f           	addqw #8,%sp
    38a8:	6738           	beqs 0x38e2
    38aa:	2078 0304      	moveal 0x304,%a0
    38ae:	42a8 0024      	clrl %a0@(36)
    38b2:	2f2a 0010      	movel %a2@(16),%sp@-
    38b6:	4e4f           	trap #15
    38b8:	a0bf           	0120277
    38ba:	3f3c 0001      	movew #1,%sp@-
    38be:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    38c4:	4e4f           	trap #15
    38c6:	a35b           	0121533
    38c8:	3f03           	movew %d3,%sp@-
    38ca:	4e4f           	trap #15
    38cc:	a0b9           	0120271
    38ce:	42a8 0004      	clrl %a0@(4)
    38d2:	2f0a           	movel %a2,%sp@-
    38d4:	4e4f           	trap #15
    38d6:	a012           	0120022
    38d8:	3004           	movew %d4,%d0
    38da:	4fef 0010      	lea %sp@(16),%sp
    38de:	6000 00c2      	braw 0x39a2
    38e2:	486a 4374      	pea %a2@(17268)
    38e6:	3f03           	movew %d3,%sp@-
    38e8:	7404           	moveq #4,%d2
    38ea:	4e4f           	trap #15
    38ec:	a347           	0121507
    38ee:	486e fff6      	pea %fp@(-10)
    38f2:	486e fffa      	pea %fp@(-6)
    38f6:	1f3c 0001      	moveb #1,%sp@-
    38fa:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    3900:	2f3c 6c69 6273 	movel #1818845811,%sp@-
    3906:	486e ffcc      	pea %fp@(-52)
    390a:	1f3c 0001      	moveb #1,%sp@-
    390e:	4e4f           	trap #15
    3910:	a078           	0120170
    3912:	2f0a           	movel %a2,%sp@-
    3914:	4227           	clrb %sp@-
    3916:	487a fcb2      	pea %pc@(0x35ca)
    391a:	2f3c 6372 6469 	movel #1668441193,%sp@-
    3920:	2f2e fff6      	movel %fp@(-10),%sp@-
    3924:	3f2e fffa      	movew %fp@(-6),%sp@-
    3928:	4e4f           	trap #15
    392a:	a353           	0121523
    392c:	2f0a           	movel %a2,%sp@-
    392e:	4227           	clrb %sp@-
    3930:	487a fcea      	pea %pc@(0x361c)
    3934:	2f3c 6372 646f 	movel #1668441199,%sp@-
    393a:	2f2e fff6      	movel %fp@(-10),%sp@-
    393e:	3f2e fffa      	movew %fp@(-6),%sp@-
    3942:	4e4f           	trap #15
    3944:	a353           	0121523
    3946:	41fa 1138      	lea %pc@(0x4a80),%a0
    394a:	2548 0008      	movel %a0,%a2@(8)
    394e:	41fa 1130      	lea %pc@(0x4a80),%a0
    3952:	2278 0304      	moveal 0x304,%a1
    3956:	2348 004c      	movel %a0,%a1@(76)
    395a:	0238 00f7 f41d 	andib #-9,0xfffffffffffff41d
    3960:	0078 0800 f306 	oriw #2048,0xfffffffffffff306
    3966:	0238 00f7 f418 	andib #-9,0xfffffffffffff418
    396c:	0038 0008 f41a 	orib #8,0xfffffffffffff41a
    3972:	0238 00f7 f41f 	andib #-9,0xfffffffffffff41f
    3978:	0038 0008 f41c 	orib #8,0xfffffffffffff41c
    397e:	1038 f419      	moveb 0xfffffffffffff419,%d0
    3982:	0200 fff0      	andib #-16,%d0
    3986:	0000 0008      	orib #8,%d0
    398a:	11c0 f419      	moveb %d0,0xfffffffffffff419
    398e:	0038 0008 f41d 	orib #8,0xfffffffffffff41d
    3994:	0278 f7ff f306 	andiw #-2049,0xfffffffffffff306
    399a:	4fef 0046      	lea %sp@(70),%sp
    399e:	5292           	addql #1,%a2@
    39a0:	7000           	moveq #0,%d0
    39a2:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
    39a6:	4e5e           	unlk %fp
    39a8:	4e75           	rts

```

# SlotClose: 0x39f2 - 0x3b50


```

    39f2:	4e56 ffd6      	linkw %fp,#-42
    39f6:	2f0a           	movel %a2,%sp@-
    39f8:	2f03           	movel %d3,%sp@-
    39fa:	3638 f110      	movew 0xfffffffffffff110,%d3
    39fe:	e24b           	lsrw #1,%d3
    3a00:	0243 0007      	andiw #7,%d3
    3a04:	223c 0002 0000 	movel #131072,%d1
    3a0a:	e7a9           	lsll %d3,%d1
    3a0c:	7600           	moveq #0,%d3
    3a0e:	3638 f100      	movew 0xfffffffffffff100,%d3
    3a12:	740d           	moveq #13,%d2
    3a14:	e5ab           	lsll %d2,%d3
    3a16:	d681           	addl %d1,%d3
    3a18:	3f2e 0008      	movew %fp@(8),%sp@-
    3a1c:	4e4f           	trap #15
    3a1e:	a0b9           	0120271
    3a20:	2468 0004      	moveal %a0@(4),%a2
    3a24:	200a           	movel %a2,%d0
    3a26:	544f           	addqw #2,%sp
    3a28:	6704           	beqs 0x3a2e
    3a2a:	4a92           	tstl %a2@
    3a2c:	6608           	bnes 0x3a36
    3a2e:	303c 290a      	movew #10506,%d0
    3a32:	6000 0116      	braw 0x3b4a
    3a36:	5392           	subql #1,%a2@
    3a38:	6708           	beqs 0x3a42
    3a3a:	303c 290b      	movew #10507,%d0
    3a3e:	6000 010a      	braw 0x3b4a
    3a42:	4a92           	tstl %a2@
    3a44:	6600 0102      	bnew 0x3b48
    3a48:	0238 00f7 f41d 	andib #-9,0xfffffffffffff41d
    3a4e:	0078 0800 f306 	oriw #2048,0xfffffffffffff306
    3a54:	486e fffa      	pea %fp@(-6)
    3a58:	486e fffe      	pea %fp@(-2)
    3a5c:	1f3c 0001      	moveb #1,%sp@-
    3a60:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    3a66:	2f3c 6c69 6273 	movel #1818845811,%sp@-
    3a6c:	486e ffd6      	pea %fp@(-42)
    3a70:	1f3c 0001      	moveb #1,%sp@-
    3a74:	4e4f           	trap #15
    3a76:	a078           	0120170
    3a78:	4227           	clrb %sp@-
    3a7a:	2f3c 6372 6469 	movel #1668441193,%sp@-
    3a80:	2f2e fffa      	movel %fp@(-6),%sp@-
    3a84:	3f2e fffe      	movew %fp@(-2),%sp@-
    3a88:	4e4f           	trap #15
    3a8a:	a354           	0121524
    3a8c:	4227           	clrb %sp@-
    3a8e:	2f3c 6372 646f 	movel #1668441199,%sp@-
    3a94:	2f2e fffa      	movel %fp@(-6),%sp@-
    3a98:	3f2e fffe      	movew %fp@(-2),%sp@-
    3a9c:	4e4f           	trap #15
    3a9e:	a354           	0121524
    3aa0:	3f2a 4374      	movew %a2@(17268),%sp@-
    3aa4:	7405           	moveq #5,%d2
    3aa6:	4e4f           	trap #15
    3aa8:	a347           	0121507
    3aaa:	2f2a 437e      	movel %a2@(17278),%sp@-
    3aae:	4e4f           	trap #15
    3ab0:	a0af           	0120257
    3ab2:	2078 0304      	moveal 0x304,%a0
    3ab6:	42a8 0024      	clrl %a0@(36)
    3aba:	42a7           	clrl %sp@-
    3abc:	2f2a 0010      	movel %a2@(16),%sp@-
    3ac0:	4e4f           	trap #15
    3ac2:	a09a           	0120232
    3ac4:	2f2a 0010      	movel %a2@(16),%sp@-
    3ac8:	4e4f           	trap #15
    3aca:	a0bf           	0120277
    3acc:	4227           	clrb %sp@-
    3ace:	4eba e994      	jsr %pc@(0x2464)
    3ad2:	3f3c 0001      	movew #1,%sp@-
    3ad6:	2f3c 536c 4d73 	movel #1399606643,%sp@-
    3adc:	4e4f           	trap #15
    3ade:	a35b           	0121533
    3ae0:	2003           	movel %d3,%d0
    3ae2:	5e80           	addql #7,%d0
    3ae4:	2040           	moveal %d0,%a0
    3ae6:	0210 007f      	andib #127,%a0@
    3aea:	2003           	movel %d3,%d0
    3aec:	5e80           	addql #7,%d0
    3aee:	2040           	moveal %d0,%a0
    3af0:	0210 00bf      	andib #-65,%a0@
    3af4:	2003           	movel %d3,%d0
    3af6:	5e80           	addql #7,%d0
    3af8:	2040           	moveal %d0,%a0
    3afa:	0210 00df      	andib #-33,%a0@
    3afe:	486e fff6      	pea %fp@(-10)
    3b02:	3f3c 4e22      	movew #20002,%sp@-
    3b06:	2f3c 5368 416c 	movel #1399341420,%sp@-
    3b0c:	4e4f           	trap #15
    3b0e:	a27b           	0121173
    3b10:	4aae fff6      	tstl %fp@(-10)
    3b14:	4fef 0054      	lea %sp@(84),%sp
    3b18:	6606           	bnes 0x3b20
    3b1a:	303c 2900      	movew #10496,%d0
    3b1e:	602a           	bras 0x3b4a
    3b20:	42a7           	clrl %sp@-
    3b22:	487a 1050      	pea %pc@(0x4b74)
    3b26:	3f3c 0003      	movew #3,%sp@-
    3b2a:	4227           	clrb %sp@-
    3b2c:	206e fff6      	moveal %fp@(-10),%a0
    3b30:	4e90           	jsr %a0@
    3b32:	3f2e 0008      	movew %fp@(8),%sp@-
    3b36:	4e4f           	trap #15
    3b38:	a0b9           	0120271
    3b3a:	42a8 0004      	clrl %a0@(4)
    3b3e:	2f0a           	movel %a2,%sp@-
    3b40:	4e4f           	trap #15
    3b42:	a012           	0120022
    3b44:	4fef 0012      	lea %sp@(18),%sp
    3b48:	7000           	moveq #0,%d0
    3b4a:	261f           	movel %sp@+,%d3
    3b4c:	245f           	moveal %sp@+,%a2
    3b4e:	4e5e           	unlk %fp
    3b50:	4e75           	rts

```

# SlotSleep: 0x3b5e - 0x3bb2


```

    3b5e:	4e56 0000      	linkw %fp,#0
    3b62:	2f0a           	movel %a2,%sp@-
    3b64:	3f2e 0008      	movew %fp@(8),%sp@-
    3b68:	4e4f           	trap #15
    3b6a:	a0b9           	0120271
    3b6c:	2468 0004      	moveal %a0@(4),%a2
    3b70:	200a           	movel %a2,%d0
    3b72:	544f           	addqw #2,%sp
    3b74:	6606           	bnes 0x3b7c
    3b76:	303c 290a      	movew #10506,%d0
    3b7a:	6032           	bras 0x3bae
    3b7c:	0238 00df f431 	andib #-33,0xfffffffffffff431
    3b82:	42a7           	clrl %sp@-
    3b84:	2f2a 0010      	movel %a2@(16),%sp@-
    3b88:	4e4f           	trap #15
    3b8a:	a09a           	0120232
    3b8c:	0c38 0001 02ca 	cmpib #1,0x2ca
    3b92:	504f           	addqw #8,%sp
    3b94:	6610           	bnes 0x3ba6
    3b96:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3b9a:	0240 0008      	andiw #8,%d0
    3b9e:	6606           	bnes 0x3ba6
    3ba0:	002a 0008 437a 	orib #8,%a2@(17274)
    3ba6:	0238 00df f409 	andib #-33,0xfffffffffffff409
    3bac:	7000           	moveq #0,%d0
    3bae:	245f           	moveal %sp@+,%a2
    3bb0:	4e5e           	unlk %fp
    3bb2:	4e75           	rts

```

# SlotWake: 0x3bc0 - 0x3c52


```

    3bc0:	4e56 fffe      	linkw %fp,#-2
    3bc4:	2f0a           	movel %a2,%sp@-
    3bc6:	3f2e 0008      	movew %fp@(8),%sp@-
    3bca:	4e4f           	trap #15
    3bcc:	a0b9           	0120271
    3bce:	2468 0004      	moveal %a0@(4),%a2
    3bd2:	200a           	movel %a2,%d0
    3bd4:	544f           	addqw #2,%sp
    3bd6:	6606           	bnes 0x3bde
    3bd8:	303c 290a      	movew #10506,%d0
    3bdc:	6070           	bras 0x3c4e
    3bde:	0038 0020 f431 	orib #32,0xfffffffffffff431
    3be4:	486e ffff      	pea %fp@(-1)
    3be8:	4eba e708      	jsr %pc@(0x22f2)
    3bec:	102a 437a      	moveb %a2@(17274),%d0
    3bf0:	0240 0008      	andiw #8,%d0
    3bf4:	584f           	addqw #4,%sp
    3bf6:	6722           	beqs 0x3c1a
    3bf8:	022a 00f7 437a 	andib #-9,%a2@(17274)
    3bfe:	0238 00f7 f41c 	andib #-9,0xfffffffffffff41c
    3c04:	1038 f419      	moveb 0xfffffffffffff419,%d0
    3c08:	0240 0008      	andiw #8,%d0
    3c0c:	660c           	bnes 0x3c1a
    3c0e:	002a 0006 437a 	orib #6,%a2@(17274)
    3c14:	022a 00fe 437a 	andib #-2,%a2@(17274)
    3c1a:	102a 437a      	moveb %a2@(17274),%d0
    3c1e:	0240 0004      	andiw #4,%d0
    3c22:	670c           	beqs 0x3c30
    3c24:	3f2a 4374      	movew %a2@(17268),%sp@-
    3c28:	7407           	moveq #7,%d2
    3c2a:	4e4f           	trap #15
    3c2c:	a347           	0121507
    3c2e:	544f           	addqw #2,%sp
    3c30:	102a 437a      	moveb %a2@(17274),%d0
    3c34:	0240 0002      	andiw #2,%d0
    3c38:	670c           	beqs 0x3c46
    3c3a:	3f2a 4374      	movew %a2@(17268),%sp@-
    3c3e:	7406           	moveq #6,%d2
    3c40:	4e4f           	trap #15
    3c42:	a347           	0121507
    3c44:	544f           	addqw #2,%sp
    3c46:	0278 f7ff f306 	andiw #-2049,0xfffffffffffff306
    3c4c:	7000           	moveq #0,%d0
    3c4e:	245f           	moveal %sp@+,%a2
    3c50:	4e5e           	unlk %fp
    3c52:	4e75           	rts

```

# SlotLibAPIVersion: 0x3c60 - 0x3c68


```

    3c60:	4e56 0000      	linkw %fp,#0
    3c64:	7002           	moveq #2,%d0
    3c66:	4e5e           	unlk %fp
    3c68:	4e75           	rts

```

# SlotCustomControl: 0x3c7e - 0x3e58


```

    3c7e:	4e56 0000      	linkw %fp,#0
    3c82:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    3c86:	266e 0010      	moveal %fp@(16),%a3
    3c8a:	3f2e 0008      	movew %fp@(8),%sp@-
    3c8e:	4e4f           	trap #15
    3c90:	a0b9           	0120271
    3c92:	2468 0004      	moveal %a0@(4),%a2
    3c96:	200a           	movel %a2,%d0
    3c98:	544f           	addqw #2,%sp
    3c9a:	6608           	bnes 0x3ca4
    3c9c:	303c 290a      	movew #10506,%d0
    3ca0:	6000 01b0      	braw 0x3e52
    3ca4:	0cae 536f 4e79 	cmpil #1399803513,%fp@(10)
    3caa:	000a 
    3cac:	6708           	beqs 0x3cb6
    3cae:	303c 2901      	movew #10497,%d0
    3cb2:	6000 019e      	braw 0x3e52
    3cb6:	302e 000e      	movew %fp@(14),%d0
    3cba:	0440 2000      	subiw #8192,%d0
    3cbe:	0c40 0006      	cmpiw #6,%d0
    3cc2:	6200 0188      	bhiw 0x3e4c
    3cc6:	d040           	addw %d0,%d0
    3cc8:	303b 0006      	movew %pc@(0x3cd0,%d0:w),%d0
    3ccc:	4efb 0002      	jmp %pc@(0x3cd0,%d0:w)
    3cd0:	000e 0018      	orib #24,%fp
    3cd4:	0074 00c8 00e8 	oriw #200,%a4@(ffffffffffffffe8,%d0:w)
    3cda:	011e           	btst %d0,%fp@+
    3cdc:	0152           	bchg %d0,%a2@
    3cde:	7001           	moveq #1,%d0
    3ce0:	2680           	movel %d0,%a3@
    3ce2:	7600           	moveq #0,%d3
    3ce4:	6000 016a      	braw 0x3e50
    3ce8:	302a 4374      	movew %a2@(17268),%d0
    3cec:	b053           	cmpw %a3@,%d0
    3cee:	6708           	beqs 0x3cf8
    3cf0:	363c 2904      	movew #10500,%d3
    3cf4:	6000 015a      	braw 0x3e50
    3cf8:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3cfc:	0240 0008      	andiw #8,%d0
    3d00:	6708           	beqs 0x3d0a
    3d02:	363c 2903      	movew #10499,%d3
    3d06:	6000 0148      	braw 0x3e50
    3d0a:	102a 437a      	moveb %a2@(17274),%d0
    3d0e:	0240 0004      	andiw #4,%d0
    3d12:	6708           	beqs 0x3d1c
    3d14:	363c 2903      	movew #10499,%d3
    3d18:	6000 0136      	braw 0x3e50
    3d1c:	7077           	moveq #119,%d0
    3d1e:	c0aa 4382      	andl %a2@(17282),%d0
    3d22:	2740 0002      	movel %d0,%a3@(2)
    3d26:	276a 430c 0006 	movel %a2@(17164),%a3@(6)
    3d2c:	276a 4304 000a 	movel %a2@(17156),%a3@(10)
    3d32:	203c 0000 4304 	movel #17156,%d0
    3d38:	2772 0804 000e 	movel %a2@(00000004,%d0:l),%a3@(14)
    3d3e:	7600           	moveq #0,%d3
    3d40:	6000 010e      	braw 0x3e50
    3d44:	302a 4374      	movew %a2@(17268),%d0
    3d48:	b053           	cmpw %a3@,%d0
    3d4a:	6708           	beqs 0x3d54
    3d4c:	363c 2904      	movew #10500,%d3
    3d50:	6000 00fe      	braw 0x3e50
    3d54:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3d58:	0240 0008      	andiw #8,%d0
    3d5c:	6708           	beqs 0x3d66
    3d5e:	363c 2903      	movew #10499,%d3
    3d62:	6000 00ec      	braw 0x3e50
    3d66:	102a 437a      	moveb %a2@(17274),%d0
    3d6a:	0240 0004      	andiw #4,%d0
    3d6e:	6708           	beqs 0x3d78
    3d70:	363c 2903      	movew #10499,%d3
    3d74:	6000 00da      	braw 0x3e50
    3d78:	7040           	moveq #64,%d0
    3d7a:	c0aa 4382      	andl %a2@(17282),%d0
    3d7e:	6608           	bnes 0x3d88
    3d80:	363c 2906      	movew #10502,%d3
    3d84:	6000 00ca      	braw 0x3e50
    3d88:	204a           	moveal %a2,%a0
    3d8a:	41e8 4158      	lea %a0@(16728),%a0
    3d8e:	2748 0002      	movel %a0,%a3@(2)
    3d92:	7600           	moveq #0,%d3
    3d94:	6000 00ba      	braw 0x3e50
    3d98:	2f2b 000a      	movel %a3@(10),%sp@-
    3d9c:	2f2b 0006      	movel %a3@(6),%sp@-
    3da0:	2f2b 0002      	movel %a3@(2),%sp@-
    3da4:	3f13           	movew %a3@,%sp@-
    3da6:	3f2e 0008      	movew %fp@(8),%sp@-
    3daa:	4eba 08f6      	jsr %pc@(0x46a2)
    3dae:	3600           	movew %d0,%d3
    3db0:	4fef 0010      	lea %sp@(16),%sp
    3db4:	6000 009a      	braw 0x3e50
    3db8:	7000           	moveq #0,%d0
    3dba:	302a 4374      	movew %a2@(17268),%d0
    3dbe:	b093           	cmpl %a3@,%d0
    3dc0:	6708           	beqs 0x3dca
    3dc2:	363c 2904      	movew #10500,%d3
    3dc6:	6000 0088      	braw 0x3e50
    3dca:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3dce:	0240 0008      	andiw #8,%d0
    3dd2:	6706           	beqs 0x3dda
    3dd4:	363c 2903      	movew #10499,%d3
    3dd8:	6076           	bras 0x3e50
    3dda:	102a 437a      	moveb %a2@(17274),%d0
    3dde:	0240 0004      	andiw #4,%d0
    3de2:	6706           	beqs 0x3dea
    3de4:	363c 2903      	movew #10499,%d3
    3de8:	6066           	bras 0x3e50
    3dea:	7600           	moveq #0,%d3
    3dec:	6062           	bras 0x3e50
    3dee:	7000           	moveq #0,%d0
    3df0:	302a 4374      	movew %a2@(17268),%d0
    3df4:	b093           	cmpl %a3@,%d0
    3df6:	6706           	beqs 0x3dfe
    3df8:	363c 2904      	movew #10500,%d3
    3dfc:	6052           	bras 0x3e50
    3dfe:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3e02:	0240 0008      	andiw #8,%d0
    3e06:	6706           	beqs 0x3e0e
    3e08:	363c 2903      	movew #10499,%d3
    3e0c:	6042           	bras 0x3e50
    3e0e:	102a 437a      	moveb %a2@(17274),%d0
    3e12:	0240 0004      	andiw #4,%d0
    3e16:	6706           	beqs 0x3e1e
    3e18:	363c 2903      	movew #10499,%d3
    3e1c:	6032           	bras 0x3e50
    3e1e:	7600           	moveq #0,%d3
    3e20:	602e           	bras 0x3e50
    3e22:	362a 4374      	movew %a2@(17268),%d3
    3e26:	7000           	moveq #0,%d0
    3e28:	3003           	movew %d3,%d0
    3e2a:	b093           	cmpl %a3@,%d0
    3e2c:	6706           	beqs 0x3e34
    3e2e:	303c 2904      	movew #10500,%d0
    3e32:	601e           	bras 0x3e52
    3e34:	3f03           	movew %d3,%sp@-
    3e36:	7407           	moveq #7,%d2
    3e38:	4e4f           	trap #15
    3e3a:	a347           	0121507
    3e3c:	3f2a 4374      	movew %a2@(17268),%sp@-
    3e40:	7406           	moveq #6,%d2
    3e42:	4e4f           	trap #15
    3e44:	a347           	0121507
    3e46:	7600           	moveq #0,%d3
    3e48:	584f           	addqw #4,%sp
    3e4a:	6004           	bras 0x3e50
    3e4c:	363c 2901      	movew #10497,%d3
    3e50:	3003           	movew %d3,%d0
    3e52:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
    3e56:	4e5e           	unlk %fp
    3e58:	4e75           	rts

```

# SlotCardPresent: 0x3e6e - 0x3ec0


```

    3e6e:	4e56 0000      	linkw %fp,#0
    3e72:	2f0a           	movel %a2,%sp@-
    3e74:	3f2e 0008      	movew %fp@(8),%sp@-
    3e78:	4e4f           	trap #15
    3e7a:	a0b9           	0120271
    3e7c:	2468 0004      	moveal %a0@(4),%a2
    3e80:	200a           	movel %a2,%d0
    3e82:	544f           	addqw #2,%sp
    3e84:	6606           	bnes 0x3e8c
    3e86:	303c 290a      	movew #10506,%d0
    3e8a:	6030           	bras 0x3ebc
    3e8c:	302a 4374      	movew %a2@(17268),%d0
    3e90:	b06e 000a      	cmpw %fp@(10),%d0
    3e94:	6706           	beqs 0x3e9c
    3e96:	303c 2904      	movew #10500,%d0
    3e9a:	6020           	bras 0x3ebc
    3e9c:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3ea0:	0240 0008      	andiw #8,%d0
    3ea4:	6706           	beqs 0x3eac
    3ea6:	303c 2903      	movew #10499,%d0
    3eaa:	6010           	bras 0x3ebc
    3eac:	082a 0002 437a 	btst #2,%a2@(17274)
    3eb2:	6706           	beqs 0x3eba
    3eb4:	303c 2903      	movew #10499,%d0
    3eb8:	6002           	bras 0x3ebc
    3eba:	7000           	moveq #0,%d0
    3ebc:	245f           	moveal %sp@+,%a2
    3ebe:	4e5e           	unlk %fp
    3ec0:	4e75           	rts

```

# MSSlotCardInfo: 0x3ed4 - 0x40da


```

    3ed4:	4e56 fffe      	linkw %fp,#-2
    3ed8:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    3edc:	266e 000c      	moveal %fp@(12),%a3
    3ee0:	3f2e 0008      	movew %fp@(8),%sp@-
    3ee4:	4e4f           	trap #15
    3ee6:	a0b9           	0120271
    3ee8:	2468 0004      	moveal %a0@(4),%a2
    3eec:	200a           	movel %a2,%d0
    3eee:	544f           	addqw #2,%sp
    3ef0:	6608           	bnes 0x3efa
    3ef2:	303c 290a      	movew #10506,%d0
    3ef6:	6000 01dc      	braw 0x40d4
    3efa:	302a 4374      	movew %a2@(17268),%d0
    3efe:	b06e 000a      	cmpw %fp@(10),%d0
    3f02:	6708           	beqs 0x3f0c
    3f04:	303c 2904      	movew #10500,%d0
    3f08:	6000 01ca      	braw 0x40d4
    3f0c:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    3f10:	0240 0008      	andiw #8,%d0
    3f14:	6708           	beqs 0x3f1e
    3f16:	303c 2903      	movew #10499,%d0
    3f1a:	6000 01b8      	braw 0x40d4
    3f1e:	102a 437a      	moveb %a2@(17274),%d0
    3f22:	0240 0004      	andiw #4,%d0
    3f26:	6708           	beqs 0x3f30
    3f28:	303c 2903      	movew #10499,%d0
    3f2c:	6000 01a6      	braw 0x40d4
    3f30:	42a7           	clrl %sp@-
    3f32:	42a7           	clrl %sp@-
    3f34:	2f2a 437e      	movel %a2@(17278),%sp@-
    3f38:	4e4f           	trap #15
    3f3a:	a0b0           	0120260
    3f3c:	102a 437a      	moveb %a2@(17274),%d0
    3f40:	0240 0001      	andiw #1,%d0
    3f44:	4fef 000c      	lea %sp@(12),%sp
    3f48:	6600 00d8      	bnew 0x4022
    3f4c:	42a7           	clrl %sp@-
    3f4e:	2f2a 0010      	movel %a2@(16),%sp@-
    3f52:	4e4f           	trap #15
    3f54:	a09a           	0120232
    3f56:	0238 00df f431 	andib #-33,0xfffffffffffff431
    3f5c:	4878 0014      	pea 0x14
    3f60:	4e4f           	trap #15
    3f62:	a249           	0121111
    3f64:	0038 0020 f431 	orib #32,0xfffffffffffff431
    3f6a:	486e ffff      	pea %fp@(-1)
    3f6e:	4eba e382      	jsr %pc@(0x22f2)
    3f72:	3838 fc02      	movew 0xfffffffffffffc02,%d4
    3f76:	3004           	movew %d4,%d0
    3f78:	0240 ff1f      	andiw #-225,%d0
    3f7c:	0040 0061      	oriw #97,%d0
    3f80:	31c0 fc02      	movew %d0,0xfffffffffffffc02
    3f84:	1f3c 0001      	moveb #1,%sp@-
    3f88:	4eba e4da      	jsr %pc@(0x2464)
    3f8c:	1f3c 0001      	moveb #1,%sp@-
    3f90:	4eba e53c      	jsr %pc@(0x24ce)
    3f94:	1f3c 0001      	moveb #1,%sp@-
    3f98:	4eba e588      	jsr %pc@(0x2522)
    3f9c:	2f0a           	movel %a2,%sp@-
    3f9e:	4eba c8fe      	jsr %pc@(0x89e)
    3fa2:	2600           	movel %d0,%d3
    3fa4:	4227           	clrb %sp@-
    3fa6:	4eba e526      	jsr %pc@(0x24ce)
    3faa:	102a 437a      	moveb %a2@(17274),%d0
    3fae:	0240 0010      	andiw #16,%d0
    3fb2:	4fef 001c      	lea %sp@(28),%sp
    3fb6:	6608           	bnes 0x3fc0
    3fb8:	4227           	clrb %sp@-
    3fba:	4eba e566      	jsr %pc@(0x2522)
    3fbe:	544f           	addqw #2,%sp
    3fc0:	4878 0064      	pea 0x64
    3fc4:	2f2a 0010      	movel %a2@(16),%sp@-
    3fc8:	4e4f           	trap #15
    3fca:	a09a           	0120232
    3fcc:	3004           	movew %d4,%d0
    3fce:	0240 00e0      	andiw #224,%d0
    3fd2:	3238 fc02      	movew 0xfffffffffffffc02,%d1
    3fd6:	0241 ff1f      	andiw #-225,%d1
    3fda:	8240           	orw %d0,%d1
    3fdc:	0041 0001      	oriw #1,%d1
    3fe0:	31c1 fc02      	movew %d1,0xfffffffffffffc02
    3fe4:	4a83           	tstl %d3
    3fe6:	504f           	addqw #8,%sp
    3fe8:	6616           	bnes 0x4000
    3fea:	7001           	moveq #1,%d0
    3fec:	2540 0004      	movel %d0,%a2@(4)
    3ff0:	0c2a 0001 437b 	cmpib #1,%a2@(17275)
    3ff6:	6624           	bnes 0x401c
    3ff8:	7002           	moveq #2,%d0
    3ffa:	81aa 0004      	orl %d0,%a2@(4)
    3ffe:	601c           	bras 0x401c
    4000:	7054           	moveq #84,%d0
    4002:	b680           	cmpl %d0,%d3
    4004:	6612           	bnes 0x4018
    4006:	2f2a 437e      	movel %a2@(17278),%sp@-
    400a:	4e4f           	trap #15
    400c:	a0b1           	0120261
    400e:	303c 2903      	movew #10499,%d0
    4012:	584f           	addqw #4,%sp
    4014:	6000 00be      	braw 0x40d4
    4018:	42aa 0004      	clrl %a2@(4)
    401c:	002a 0001 437a 	orib #1,%a2@(17274)
    4022:	26aa 0004      	movel %a2@(4),%a3@
    4026:	7001           	moveq #1,%d0
    4028:	c0aa 0004      	andl %a2@(4),%d0
    402c:	674e           	beqs 0x407c
    402e:	3f3c 001f      	movew #31,%sp@-
    4032:	41fa 00ba      	lea %pc@(0x40ee),%a0
    4036:	4850           	pea %a0@
    4038:	486b 0004      	pea %a3@(4)
    403c:	4e4f           	trap #15
    403e:	a2ce           	0121316
    4040:	3f3c 001f      	movew #31,%sp@-
    4044:	41fa 00aa      	lea %pc@(0x40f0),%a0
    4048:	4850           	pea %a0@
    404a:	486b 0024      	pea %a3@(36)
    404e:	4e4f           	trap #15
    4050:	a2ce           	0121316
    4052:	3f3c 001f      	movew #31,%sp@-
    4056:	41fa 009a      	lea %pc@(0x40f2),%a0
    405a:	4850           	pea %a0@
    405c:	486b 0044      	pea %a3@(68)
    4060:	4e4f           	trap #15
    4062:	a2ce           	0121316
    4064:	3f3c 001f      	movew #31,%sp@-
    4068:	41fa 0096      	lea %pc@(0x4100),%a0
    406c:	4850           	pea %a0@
    406e:	486b 0064      	pea %a3@(100)
    4072:	4e4f           	trap #15
    4074:	a2ce           	0121316
    4076:	4fef 0028      	lea %sp@(40),%sp
    407a:	604c           	bras 0x40c8
    407c:	3f3c 001f      	movew #31,%sp@-
    4080:	41fa 0080      	lea %pc@(0x4102),%a0
    4084:	4850           	pea %a0@
    4086:	486b 0004      	pea %a3@(4)
    408a:	4e4f           	trap #15
    408c:	a2ce           	0121316
    408e:	3f3c 001f      	movew #31,%sp@-
    4092:	41fa 0070      	lea %pc@(0x4104),%a0
    4096:	4850           	pea %a0@
    4098:	486b 0024      	pea %a3@(36)
    409c:	4e4f           	trap #15
    409e:	a2ce           	0121316
    40a0:	3f3c 001f      	movew #31,%sp@-
    40a4:	41fa 0060      	lea %pc@(0x4106),%a0
    40a8:	4850           	pea %a0@
    40aa:	486b 0044      	pea %a3@(68)
    40ae:	4e4f           	trap #15
    40b0:	a2ce           	0121316
    40b2:	3f3c 001f      	movew #31,%sp@-
    40b6:	41fa 0050      	lea %pc@(0x4108),%a0
    40ba:	4850           	pea %a0@
    40bc:	486b 0064      	pea %a3@(100)
    40c0:	4e4f           	trap #15
    40c2:	a2ce           	0121316
    40c4:	4fef 0028      	lea %sp@(40),%sp
    40c8:	2f2a 437e      	movel %a2@(17278),%sp@-
    40cc:	4e4f           	trap #15
    40ce:	a0b1           	0120261
    40d0:	7000           	moveq #0,%d0
    40d2:	584f           	addqw #4,%sp
    40d4:	4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3
    40d8:	4e5e           	unlk %fp
    40da:	4e75           	rts

```

# SlotCardMediaType: 0x410a - 0x4174


```

    410a:	4e56 0000      	linkw %fp,#0
    410e:	2f0a           	movel %a2,%sp@-
    4110:	3f2e 0008      	movew %fp@(8),%sp@-
    4114:	4e4f           	trap #15
    4116:	a0b9           	0120271
    4118:	2468 0004      	moveal %a0@(4),%a2
    411c:	200a           	movel %a2,%d0
    411e:	544f           	addqw #2,%sp
    4120:	6606           	bnes 0x4128
    4122:	303c 290a      	movew #10506,%d0
    4126:	6048           	bras 0x4170
    4128:	302a 4374      	movew %a2@(17268),%d0
    412c:	b06e 000a      	cmpw %fp@(10),%d0
    4130:	6706           	beqs 0x4138
    4132:	303c 2904      	movew #10500,%d0
    4136:	6038           	bras 0x4170
    4138:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    413c:	0240 0008      	andiw #8,%d0
    4140:	6706           	beqs 0x4148
    4142:	303c 2903      	movew #10499,%d0
    4146:	6028           	bras 0x4170
    4148:	102a 437a      	moveb %a2@(17274),%d0
    414c:	0240 0004      	andiw #4,%d0
    4150:	6706           	beqs 0x4158
    4152:	303c 2903      	movew #10499,%d0
    4156:	6018           	bras 0x4170
    4158:	4aae 000c      	tstl %fp@(12)
    415c:	670e           	beqs 0x416c
    415e:	206e 000c      	moveal %fp@(12),%a0
    4162:	20bc 6d73 746b 	movel #1836282987,%a0@
    4168:	7000           	moveq #0,%d0
    416a:	6004           	bras 0x4170
    416c:	303c 0502      	movew #1282,%d0
    4170:	245f           	moveal %sp@+,%a2
    4172:	4e5e           	unlk %fp
    4174:	4e75           	rts

```

# SlotMediaType: 0x418a - 0x41d4


```

    418a:	4e56 0000      	linkw %fp,#0
    418e:	2f0a           	movel %a2,%sp@-
    4190:	3f2e 0008      	movew %fp@(8),%sp@-
    4194:	4e4f           	trap #15
    4196:	a0b9           	0120271
    4198:	2468 0004      	moveal %a0@(4),%a2
    419c:	200a           	movel %a2,%d0
    419e:	544f           	addqw #2,%sp
    41a0:	6606           	bnes 0x41a8
    41a2:	303c 290a      	movew #10506,%d0
    41a6:	6028           	bras 0x41d0
    41a8:	302a 4374      	movew %a2@(17268),%d0
    41ac:	b06e 000a      	cmpw %fp@(10),%d0
    41b0:	6706           	beqs 0x41b8
    41b2:	303c 2904      	movew #10500,%d0
    41b6:	6018           	bras 0x41d0
    41b8:	4aae 000c      	tstl %fp@(12)
    41bc:	670e           	beqs 0x41cc
    41be:	206e 000c      	moveal %fp@(12),%a0
    41c2:	20bc 6d73 746b 	movel #1836282987,%a0@
    41c8:	7000           	moveq #0,%d0
    41ca:	6004           	bras 0x41d0
    41cc:	303c 0502      	movew #1282,%d0
    41d0:	245f           	moveal %sp@+,%a2
    41d2:	4e5e           	unlk %fp
    41d4:	4e75           	rts

```

# SlotCardReserve: 0x41e6 - 0x422a


```

    41e6:	4e56 0000      	linkw %fp,#0
    41ea:	2f0a           	movel %a2,%sp@-
    41ec:	3f2e 0008      	movew %fp@(8),%sp@-
    41f0:	4e4f           	trap #15
    41f2:	a0b9           	0120271
    41f4:	2468 0004      	moveal %a0@(4),%a2
    41f8:	200a           	movel %a2,%d0
    41fa:	544f           	addqw #2,%sp
    41fc:	6606           	bnes 0x4204
    41fe:	303c 290a      	movew #10506,%d0
    4202:	6022           	bras 0x4226
    4204:	302a 4374      	movew %a2@(17268),%d0
    4208:	b06e 000a      	cmpw %fp@(10),%d0
    420c:	6706           	beqs 0x4214
    420e:	303c 2904      	movew #10500,%d0
    4212:	6012           	bras 0x4226
    4214:	002a 0010 437a 	orib #16,%a2@(17274)
    421a:	1f3c 0001      	moveb #1,%sp@-
    421e:	4eba e302      	jsr %pc@(0x2522)
    4222:	7000           	moveq #0,%d0
    4224:	544f           	addqw #2,%sp
    4226:	245f           	moveal %sp@+,%a2
    4228:	4e5e           	unlk %fp
    422a:	4e75           	rts

```

# SlotCardRelease: 0x423e - 0x4280


```

    423e:	4e56 0000      	linkw %fp,#0
    4242:	2f0a           	movel %a2,%sp@-
    4244:	3f2e 0008      	movew %fp@(8),%sp@-
    4248:	4e4f           	trap #15
    424a:	a0b9           	0120271
    424c:	2468 0004      	moveal %a0@(4),%a2
    4250:	200a           	movel %a2,%d0
    4252:	544f           	addqw #2,%sp
    4254:	6606           	bnes 0x425c
    4256:	303c 290a      	movew #10506,%d0
    425a:	6020           	bras 0x427c
    425c:	302a 4374      	movew %a2@(17268),%d0
    4260:	b06e 000a      	cmpw %fp@(10),%d0
    4264:	6706           	beqs 0x426c
    4266:	303c 2904      	movew #10500,%d0
    426a:	6010           	bras 0x427c
    426c:	022a 00ef 437a 	andib #-17,%a2@(17274)
    4272:	4227           	clrb %sp@-
    4274:	4eba e2ac      	jsr %pc@(0x2522)
    4278:	7000           	moveq #0,%d0
    427a:	544f           	addqw #2,%sp
    427c:	245f           	moveal %sp@+,%a2
    427e:	4e5e           	unlk %fp
    4280:	4e75           	rts

```

# SlotCardGetSerialPort: 0x4294 - 0x42ca


```

    4294:	4e56 0000      	linkw %fp,#0
    4298:	2f0a           	movel %a2,%sp@-
    429a:	3f2e 0008      	movew %fp@(8),%sp@-
    429e:	4e4f           	trap #15
    42a0:	a0b9           	0120271
    42a2:	2468 0004      	moveal %a0@(4),%a2
    42a6:	200a           	movel %a2,%d0
    42a8:	544f           	addqw #2,%sp
    42aa:	6606           	bnes 0x42b2
    42ac:	303c 290a      	movew #10506,%d0
    42b0:	6014           	bras 0x42c6
    42b2:	302a 4374      	movew %a2@(17268),%d0
    42b6:	b06e 000a      	cmpw %fp@(10),%d0
    42ba:	6706           	beqs 0x42c2
    42bc:	303c 2904      	movew #10500,%d0
    42c0:	6004           	bras 0x42c6
    42c2:	303c 2901      	movew #10497,%d0
    42c6:	245f           	moveal %sp@+,%a2
    42c8:	4e5e           	unlk %fp
    42ca:	4e75           	rts

```

# SlotCardIsFilesystemSupported: 0x42e4 - 0x4304


```

    42e4:	4e56 0000      	linkw %fp,#0
    42e8:	0cae 7666 6174 	cmpil #1986421108,%fp@(12)
    42ee:	000c 
    42f0:	670a           	beqs 0x42fc
    42f2:	0cae 6661 7473 	cmpil #1717662835,%fp@(12)
    42f8:	000c 
    42fa:	6604           	bnes 0x4300
    42fc:	7001           	moveq #1,%d0
    42fe:	6002           	bras 0x4302
    4300:	7000           	moveq #0,%d0
    4302:	4e5e           	unlk %fp
    4304:	4e75           	rts

```

# SlotCardMetrics: 0x4326 - 0x44e6


```

    4326:	4e56 ff7c      	linkw %fp,#-132
    432a:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    432e:	246e 000c      	moveal %fp@(12),%a2
    4332:	3f2e 0008      	movew %fp@(8),%sp@-
    4336:	4e4f           	trap #15
    4338:	a0b9           	0120271
    433a:	2668 0004      	moveal %a0@(4),%a3
    433e:	200b           	movel %a3,%d0
    4340:	544f           	addqw #2,%sp
    4342:	6608           	bnes 0x434c
    4344:	303c 290a      	movew #10506,%d0
    4348:	6000 0196      	braw 0x44e0
    434c:	302b 4374      	movew %a3@(17268),%d0
    4350:	b06e 000a      	cmpw %fp@(10),%d0
    4354:	6708           	beqs 0x435e
    4356:	303c 2904      	movew #10500,%d0
    435a:	6000 0184      	braw 0x44e0
    435e:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    4362:	0240 0008      	andiw #8,%d0
    4366:	6708           	beqs 0x4370
    4368:	303c 2903      	movew #10499,%d0
    436c:	6000 0172      	braw 0x44e0
    4370:	102b 437a      	moveb %a3@(17274),%d0
    4374:	0240 0001      	andiw #1,%d0
    4378:	661c           	bnes 0x4396
    437a:	486e ff7c      	pea %fp@(-132)
    437e:	3f2e 000a      	movew %fp@(10),%sp@-
    4382:	3f2e 0008      	movew %fp@(8),%sp@-
    4386:	4eba fb4c      	jsr %pc@(0x3ed4)
    438a:	3600           	movew %d0,%d3
    438c:	504f           	addqw #8,%sp
    438e:	6706           	beqs 0x4396
    4390:	3003           	movew %d3,%d0
    4392:	6000 014c      	braw 0x44e0
    4396:	200a           	movel %a2,%d0
    4398:	6700 013e      	beqw 0x44d8
    439c:	7001           	moveq #1,%d0
    439e:	c0ab 0004      	andl %a3@(4),%d0
    43a2:	6700 012e      	beqw 0x44d2
    43a6:	357c 0200 0004 	movew #512,%a2@(4)
    43ac:	357c 0010 0006 	movew #16,%a2@(6)
    43b2:	157c 0080 000e 	moveb #-128,%a2@(14)
    43b8:	302b 42fc      	movew %a3@(17148),%d0
    43bc:	0440 0200      	subiw #512,%d0
    43c0:	6722           	beqs 0x43e4
    43c2:	0440 0200      	subiw #512,%d0
    43c6:	673e           	beqs 0x4406
    43c8:	0440 0400      	subiw #1024,%d0
    43cc:	6700 0092      	beqw 0x4460
    43d0:	0440 0800      	subiw #2048,%d0
    43d4:	6700 00aa      	beqw 0x4480
    43d8:	0440 1000      	subiw #4096,%d0
    43dc:	6700 00c2      	beqw 0x44a0
    43e0:	6000 00de      	braw 0x44c0
    43e4:	24bc 0000 1ec0 	movel #7872,%a2@
    43ea:	357c 0002 0008 	movew #2,%a2@(8)
    43f0:	157c 0010 000c 	moveb #16,%a2@(12)
    43f6:	157c 0001 000d 	moveb #1,%a2@(13)
    43fc:	701b           	moveq #27,%d0
    43fe:	2540 0010      	movel %d0,%a2@(16)
    4402:	6000 00c2      	braw 0x44c6
    4406:	362b 42fa      	movew %a3@(17146),%d3
    440a:	0c43 0008      	cmpiw #8,%d3
    440e:	6622           	bnes 0x4432
    4410:	24bc 0000 3dc0 	movel #15808,%a2@
    4416:	357c 0002 0008 	movew #2,%a2@(8)
    441c:	157c 0010 000c 	moveb #16,%a2@(12)
    4422:	157c 0001 000d 	moveb #1,%a2@(13)
    4428:	7019           	moveq #25,%d0
    442a:	2540 0010      	movel %d0,%a2@(16)
    442e:	6000 0096      	braw 0x44c6
    4432:	0c43 0010      	cmpiw #16,%d3
    4436:	6620           	bnes 0x4458
    4438:	24bc 0000 7b80 	movel #31616,%a2@
    443e:	357c 0004 0008 	movew #4,%a2@(8)
    4444:	157c 0020 000c 	moveb #32,%a2@(12)
    444a:	157c 0001 000d 	moveb #1,%a2@(13)
    4450:	7019           	moveq #25,%d0
    4452:	2540 0010      	movel %d0,%a2@(16)
    4456:	606e           	bras 0x44c6
    4458:	303c 2906      	movew #10502,%d0
    445c:	6000 0082      	braw 0x44e0
    4460:	24bc 0000 f780 	movel #63360,%a2@
    4466:	357c 0004 0008 	movew #4,%a2@(8)
    446c:	157c 0020 000c 	moveb #32,%a2@(12)
    4472:	157c 0001 000d 	moveb #1,%a2@(13)
    4478:	7013           	moveq #19,%d0
    447a:	2540 0010      	movel %d0,%a2@(16)
    447e:	6046           	bras 0x44c6
    4480:	24bc 0001 ef00 	movel #126720,%a2@
    4486:	357c 0008 0008 	movew #8,%a2@(8)
    448c:	157c 0020 000c 	moveb #32,%a2@(12)
    4492:	157c 0001 000d 	moveb #1,%a2@(13)
    4498:	7027           	moveq #39,%d0
    449a:	2540 0010      	movel %d0,%a2@(16)
    449e:	6026           	bras 0x44c6
    44a0:	24bc 0003 de00 	movel #253440,%a2@
    44a6:	357c 0010 0008 	movew #16,%a2@(8)
    44ac:	157c 0020 000c 	moveb #32,%a2@(12)
    44b2:	157c 0006 000d 	moveb #6,%a2@(13)
    44b8:	7021           	moveq #33,%d0
    44ba:	2540 0010      	movel %d0,%a2@(16)
    44be:	6006           	bras 0x44c6
    44c0:	303c 2906      	movew #10502,%d0
    44c4:	601a           	bras 0x44e0
    44c6:	2012           	movel %a2@,%d0
    44c8:	90aa 0010      	subl %a2@(16),%d0
    44cc:	2540 0014      	movel %d0,%a2@(20)
    44d0:	600c           	bras 0x44de
    44d2:	303c 2906      	movew #10502,%d0
    44d6:	6008           	bras 0x44e0
    44d8:	303c 0502      	movew #1282,%d0
    44dc:	6002           	bras 0x44e0
    44de:	7000           	moveq #0,%d0
    44e0:	4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3
    44e4:	4e5e           	unlk %fp
    44e6:	4e75           	rts

```

# SlotCardLowLevelFormat: 0x44fa - 0x4686


```

    44fa:	4e56 ff7c      	linkw %fp,#-132
    44fe:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    4502:	7600           	moveq #0,%d3
    4504:	3f2e 0008      	movew %fp@(8),%sp@-
    4508:	4e4f           	trap #15
    450a:	a0b9           	0120271
    450c:	2468 0004      	moveal %a0@(4),%a2
    4510:	200a           	movel %a2,%d0
    4512:	544f           	addqw #2,%sp
    4514:	6608           	bnes 0x451e
    4516:	303c 290a      	movew #10506,%d0
    451a:	6000 0164      	braw 0x4680
    451e:	302a 4374      	movew %a2@(17268),%d0
    4522:	b06e 000a      	cmpw %fp@(10),%d0
    4526:	6708           	beqs 0x4530
    4528:	303c 2904      	movew #10500,%d0
    452c:	6000 0152      	braw 0x4680
    4530:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    4534:	0240 0008      	andiw #8,%d0
    4538:	6708           	beqs 0x4542
    453a:	303c 2903      	movew #10499,%d0
    453e:	6000 0140      	braw 0x4680
    4542:	42a7           	clrl %sp@-
    4544:	42a7           	clrl %sp@-
    4546:	2f2a 437e      	movel %a2@(17278),%sp@-
    454a:	4e4f           	trap #15
    454c:	a0b0           	0120260
    454e:	102a 437a      	moveb %a2@(17274),%d0
    4552:	0240 0001      	andiw #1,%d0
    4556:	4fef 000c      	lea %sp@(12),%sp
    455a:	6626           	bnes 0x4582
    455c:	486e ff7c      	pea %fp@(-132)
    4560:	3f2e 000a      	movew %fp@(10),%sp@-
    4564:	3f2e 0008      	movew %fp@(8),%sp@-
    4568:	4eba f96a      	jsr %pc@(0x3ed4)
    456c:	3600           	movew %d0,%d3
    456e:	504f           	addqw #8,%sp
    4570:	6710           	beqs 0x4582
    4572:	2f2a 437e      	movel %a2@(17278),%sp@-
    4576:	4e4f           	trap #15
    4578:	a0b1           	0120261
    457a:	3003           	movew %d3,%d0
    457c:	584f           	addqw #4,%sp
    457e:	6000 0100      	braw 0x4680
    4582:	7001           	moveq #1,%d0
    4584:	c0aa 0004      	andl %a2@(4),%d0
    4588:	6700 00da      	beqw 0x4664
    458c:	4878 0200      	pea 0x200
    4590:	4e4f           	trap #15
    4592:	a013           	0120023
    4594:	2648           	moveal %a0,%a3
    4596:	200b           	movel %a3,%d0
    4598:	584f           	addqw #4,%sp
    459a:	6608           	bnes 0x45a4
    459c:	363c 0102      	movew #258,%d3
    45a0:	6000 00d2      	braw 0x4674
    45a4:	42a7           	clrl %sp@-
    45a6:	2f2a 0010      	movel %a2@(16),%sp@-
    45aa:	4e4f           	trap #15
    45ac:	a09a           	0120232
    45ae:	4267           	clrw %sp@-
    45b0:	2f0b           	movel %a3,%sp@-
    45b2:	4e4f           	trap #15
    45b4:	a01b           	0120033
    45b6:	3a38 fc02      	movew 0xfffffffffffffc02,%d5
    45ba:	3005           	movew %d5,%d0
    45bc:	0240 ff1f      	andiw #-225,%d0
    45c0:	0040 0061      	oriw #97,%d0
    45c4:	31c0 fc02      	movew %d0,0xfffffffffffffc02
    45c8:	1f3c 0001      	moveb #1,%sp@-
    45cc:	4eba de96      	jsr %pc@(0x2464)
    45d0:	1f3c 0001      	moveb #1,%sp@-
    45d4:	4eba def8      	jsr %pc@(0x24ce)
    45d8:	1f3c 0001      	moveb #1,%sp@-
    45dc:	4eba df44      	jsr %pc@(0x2522)
    45e0:	2f0a           	movel %a2,%sp@-
    45e2:	2f0b           	movel %a3,%sp@-
    45e4:	4eba d6f6      	jsr %pc@(0x1cdc)
    45e8:	2800           	movel %d0,%d4
    45ea:	4227           	clrb %sp@-
    45ec:	4eba dee0      	jsr %pc@(0x24ce)
    45f0:	102a 437a      	moveb %a2@(17274),%d0
    45f4:	0240 0010      	andiw #16,%d0
    45f8:	4fef 001e      	lea %sp@(30),%sp
    45fc:	6608           	bnes 0x4606
    45fe:	4227           	clrb %sp@-
    4600:	4eba df20      	jsr %pc@(0x2522)
    4604:	544f           	addqw #2,%sp
    4606:	4878 0064      	pea 0x64
    460a:	2f2a 0010      	movel %a2@(16),%sp@-
    460e:	4e4f           	trap #15
    4610:	a09a           	0120232
    4612:	3005           	movew %d5,%d0
    4614:	0240 00e0      	andiw #224,%d0
    4618:	3238 fc02      	movew 0xfffffffffffffc02,%d1
    461c:	0241 ff1f      	andiw #-225,%d1
    4620:	8240           	orw %d0,%d1
    4622:	0041 0001      	oriw #1,%d1
    4626:	31c1 fc02      	movew %d1,0xfffffffffffffc02
    462a:	2f0b           	movel %a3,%sp@-
    462c:	4e4f           	trap #15
    462e:	a012           	0120022
    4630:	4a84           	tstl %d4
    4632:	4fef 000c      	lea %sp@(12),%sp
    4636:	6726           	beqs 0x465e
    4638:	7012           	moveq #18,%d0
    463a:	b880           	cmpl %d0,%d4
    463c:	6606           	bnes 0x4644
    463e:	363c 2907      	movew #10503,%d3
    4642:	601a           	bras 0x465e
    4644:	7054           	moveq #84,%d0
    4646:	b880           	cmpl %d0,%d4
    4648:	6606           	bnes 0x4650
    464a:	363c 2903      	movew #10499,%d3
    464e:	600e           	bras 0x465e
    4650:	363c 2906      	movew #10502,%d3
    4654:	70fe           	moveq #-2,%d0
    4656:	c1aa 0004      	andl %d0,%a2@(4)
    465a:	c1aa 4382      	andl %d0,%a2@(17282)
    465e:	4e4f           	trap #15
    4660:	a130           	0120460
    4662:	6010           	bras 0x4674
    4664:	2f2a 437e      	movel %a2@(17278),%sp@-
    4668:	4e4f           	trap #15
    466a:	a0b1           	0120261
    466c:	303c 2906      	movew #10502,%d0
    4670:	584f           	addqw #4,%sp
    4672:	600c           	bras 0x4680
    4674:	2f2a 437e      	movel %a2@(17278),%sp@-
    4678:	4e4f           	trap #15
    467a:	a0b1           	0120261
    467c:	3003           	movew %d3,%d0
    467e:	584f           	addqw #4,%sp
    4680:	4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3
    4684:	4e5e           	unlk %fp
    4686:	4e75           	rts

```

# MSSlotCardSectorRead: 0x46a2 - 0x4828


```

    46a2:	4e56 ff78      	linkw %fp,#-136
    46a6:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    46aa:	266e 0014      	moveal %fp@(20),%a3
    46ae:	7600           	moveq #0,%d3
    46b0:	2d53 fffc      	movel %a3@,%fp@(-4)
    46b4:	4293           	clrl %a3@
    46b6:	3f2e 0008      	movew %fp@(8),%sp@-
    46ba:	4e4f           	trap #15
    46bc:	a0b9           	0120271
    46be:	2468 0004      	moveal %a0@(4),%a2
    46c2:	200a           	movel %a2,%d0
    46c4:	544f           	addqw #2,%sp
    46c6:	6608           	bnes 0x46d0
    46c8:	303c 290a      	movew #10506,%d0
    46cc:	6000 0154      	braw 0x4822
    46d0:	302a 4374      	movew %a2@(17268),%d0
    46d4:	b06e 000a      	cmpw %fp@(10),%d0
    46d8:	6708           	beqs 0x46e2
    46da:	303c 2904      	movew #10500,%d0
    46de:	6000 0142      	braw 0x4822
    46e2:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    46e6:	0240 0008      	andiw #8,%d0
    46ea:	6708           	beqs 0x46f4
    46ec:	303c 2903      	movew #10499,%d0
    46f0:	6000 0130      	braw 0x4822
    46f4:	42a7           	clrl %sp@-
    46f6:	42a7           	clrl %sp@-
    46f8:	2f2a 437e      	movel %a2@(17278),%sp@-
    46fc:	4e4f           	trap #15
    46fe:	a0b0           	0120260
    4700:	102a 437a      	moveb %a2@(17274),%d0
    4704:	0240 0001      	andiw #1,%d0
    4708:	4fef 000c      	lea %sp@(12),%sp
    470c:	6626           	bnes 0x4734
    470e:	486e ff78      	pea %fp@(-136)
    4712:	3f2e 000a      	movew %fp@(10),%sp@-
    4716:	3f2e 0008      	movew %fp@(8),%sp@-
    471a:	4eba f7b8      	jsr %pc@(0x3ed4)
    471e:	3600           	movew %d0,%d3
    4720:	504f           	addqw #8,%sp
    4722:	6710           	beqs 0x4734
    4724:	2f2a 437e      	movel %a2@(17278),%sp@-
    4728:	4e4f           	trap #15
    472a:	a0b1           	0120261
    472c:	3003           	movew %d3,%d0
    472e:	584f           	addqw #4,%sp
    4730:	6000 00f0      	braw 0x4822
    4734:	7001           	moveq #1,%d0
    4736:	c0aa 0004      	andl %a2@(4),%d0
    473a:	6700 00d6      	beqw 0x4812
    473e:	4aae fffc      	tstl %fp@(-4)
    4742:	6610           	bnes 0x4754
    4744:	2f2a 437e      	movel %a2@(17278),%sp@-
    4748:	4e4f           	trap #15
    474a:	a0b1           	0120261
    474c:	7000           	moveq #0,%d0
    474e:	584f           	addqw #4,%sp
    4750:	6000 00d0      	braw 0x4822
    4754:	42a7           	clrl %sp@-
    4756:	2f2a 0010      	movel %a2@(16),%sp@-
    475a:	4e4f           	trap #15
    475c:	a09a           	0120232
    475e:	3a38 fc02      	movew 0xfffffffffffffc02,%d5
    4762:	3005           	movew %d5,%d0
    4764:	0240 ff1f      	andiw #-225,%d0
    4768:	0040 0061      	oriw #97,%d0
    476c:	31c0 fc02      	movew %d0,0xfffffffffffffc02
    4770:	1f3c 0001      	moveb #1,%sp@-
    4774:	4eba dcee      	jsr %pc@(0x2464)
    4778:	1f3c 0001      	moveb #1,%sp@-
    477c:	4eba dd50      	jsr %pc@(0x24ce)
    4780:	1f3c 0001      	moveb #1,%sp@-
    4784:	4eba dd9c      	jsr %pc@(0x2522)
    4788:	2f0a           	movel %a2,%sp@-
    478a:	486e fffc      	pea %fp@(-4)
    478e:	2f2e 0010      	movel %fp@(16),%sp@-
    4792:	2f2e 000c      	movel %fp@(12),%sp@-
    4796:	4eba d0ac      	jsr %pc@(0x1844)
    479a:	2800           	movel %d0,%d4
    479c:	4227           	clrb %sp@-
    479e:	4eba dd2e      	jsr %pc@(0x24ce)
    47a2:	102a 437a      	moveb %a2@(17274),%d0
    47a6:	0240 0010      	andiw #16,%d0
    47aa:	4fef 0020      	lea %sp@(32),%sp
    47ae:	6608           	bnes 0x47b8
    47b0:	4227           	clrb %sp@-
    47b2:	4eba dd6e      	jsr %pc@(0x2522)
    47b6:	544f           	addqw #2,%sp
    47b8:	4878 0064      	pea 0x64
    47bc:	2f2a 0010      	movel %a2@(16),%sp@-
    47c0:	4e4f           	trap #15
    47c2:	a09a           	0120232
    47c4:	3005           	movew %d5,%d0
    47c6:	0240 00e0      	andiw #224,%d0
    47ca:	3238 fc02      	movew 0xfffffffffffffc02,%d1
    47ce:	0241 ff1f      	andiw #-225,%d1
    47d2:	8240           	orw %d0,%d1
    47d4:	0041 0001      	oriw #1,%d1
    47d8:	31c1 fc02      	movew %d1,0xfffffffffffffc02
    47dc:	4a84           	tstl %d4
    47de:	504f           	addqw #8,%sp
    47e0:	6606           	bnes 0x47e8
    47e2:	26ae fffc      	movel %fp@(-4),%a3@
    47e6:	602e           	bras 0x4816
    47e8:	7018           	moveq #24,%d0
    47ea:	b880           	cmpl %d0,%d4
    47ec:	6606           	bnes 0x47f4
    47ee:	363c 0502      	movew #1282,%d3
    47f2:	6022           	bras 0x4816
    47f4:	7013           	moveq #19,%d0
    47f6:	b880           	cmpl %d0,%d4
    47f8:	6606           	bnes 0x4800
    47fa:	363c 2909      	movew #10505,%d3
    47fe:	6016           	bras 0x4816
    4800:	7020           	moveq #32,%d0
    4802:	b880           	cmpl %d0,%d4
    4804:	6606           	bnes 0x480c
    4806:	363c 2908      	movew #10504,%d3
    480a:	600a           	bras 0x4816
    480c:	363c 2906      	movew #10502,%d3
    4810:	6004           	bras 0x4816
    4812:	363c 2906      	movew #10502,%d3
    4816:	2f2a 437e      	movel %a2@(17278),%sp@-
    481a:	4e4f           	trap #15
    481c:	a0b1           	0120261
    481e:	3003           	movew %d3,%d0
    4820:	584f           	addqw #4,%sp
    4822:	4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3
    4826:	4e5e           	unlk %fp
    4828:	4e75           	rts

```

# SlotCardSectorWrite: 0x4842 - 0x49cc


```

    4842:	4e56 ff78      	linkw %fp,#-136
    4846:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    484a:	266e 0014      	moveal %fp@(20),%a3
    484e:	7600           	moveq #0,%d3
    4850:	2d53 fffc      	movel %a3@,%fp@(-4)
    4854:	4293           	clrl %a3@
    4856:	3f2e 0008      	movew %fp@(8),%sp@-
    485a:	4e4f           	trap #15
    485c:	a0b9           	0120271
    485e:	2468 0004      	moveal %a0@(4),%a2
    4862:	200a           	movel %a2,%d0
    4864:	544f           	addqw #2,%sp
    4866:	6608           	bnes 0x4870
    4868:	303c 290a      	movew #10506,%d0
    486c:	6000 0158      	braw 0x49c6
    4870:	302a 4374      	movew %a2@(17268),%d0
    4874:	b06e 000a      	cmpw %fp@(10),%d0
    4878:	6708           	beqs 0x4882
    487a:	303c 2904      	movew #10500,%d0
    487e:	6000 0146      	braw 0x49c6
    4882:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    4886:	0240 0008      	andiw #8,%d0
    488a:	6708           	beqs 0x4894
    488c:	303c 2903      	movew #10499,%d0
    4890:	6000 0134      	braw 0x49c6
    4894:	42a7           	clrl %sp@-
    4896:	42a7           	clrl %sp@-
    4898:	2f2a 437e      	movel %a2@(17278),%sp@-
    489c:	4e4f           	trap #15
    489e:	a0b0           	0120260
    48a0:	102a 437a      	moveb %a2@(17274),%d0
    48a4:	0240 0001      	andiw #1,%d0
    48a8:	4fef 000c      	lea %sp@(12),%sp
    48ac:	6626           	bnes 0x48d4
    48ae:	486e ff78      	pea %fp@(-136)
    48b2:	3f2e 000a      	movew %fp@(10),%sp@-
    48b6:	3f2e 0008      	movew %fp@(8),%sp@-
    48ba:	4eba f618      	jsr %pc@(0x3ed4)
    48be:	3600           	movew %d0,%d3
    48c0:	504f           	addqw #8,%sp
    48c2:	6710           	beqs 0x48d4
    48c4:	2f2a 437e      	movel %a2@(17278),%sp@-
    48c8:	4e4f           	trap #15
    48ca:	a0b1           	0120261
    48cc:	3003           	movew %d3,%d0
    48ce:	584f           	addqw #4,%sp
    48d0:	6000 00f4      	braw 0x49c6
    48d4:	7001           	moveq #1,%d0
    48d6:	282a 0004      	movel %a2@(4),%d4
    48da:	c084           	andl %d4,%d0
    48dc:	6700 00d8      	beqw 0x49b6
    48e0:	7002           	moveq #2,%d0
    48e2:	c084           	andl %d4,%d0
    48e4:	6708           	beqs 0x48ee
    48e6:	363c 2907      	movew #10503,%d3
    48ea:	6000 00ce      	braw 0x49ba
    48ee:	4aae fffc      	tstl %fp@(-4)
    48f2:	6610           	bnes 0x4904
    48f4:	2f2a 437e      	movel %a2@(17278),%sp@-
    48f8:	4e4f           	trap #15
    48fa:	a0b1           	0120261
    48fc:	7000           	moveq #0,%d0
    48fe:	584f           	addqw #4,%sp
    4900:	6000 00c4      	braw 0x49c6
    4904:	42a7           	clrl %sp@-
    4906:	2f2a 0010      	movel %a2@(16),%sp@-
    490a:	4e4f           	trap #15
    490c:	a09a           	0120232
    490e:	3a38 fc02      	movew 0xfffffffffffffc02,%d5
    4912:	3005           	movew %d5,%d0
    4914:	0240 ff1f      	andiw #-225,%d0
    4918:	0040 0061      	oriw #97,%d0
    491c:	31c0 fc02      	movew %d0,0xfffffffffffffc02
    4920:	1f3c 0001      	moveb #1,%sp@-
    4924:	4eba db3e      	jsr %pc@(0x2464)
    4928:	1f3c 0001      	moveb #1,%sp@-
    492c:	4eba dba0      	jsr %pc@(0x24ce)
    4930:	1f3c 0001      	moveb #1,%sp@-
    4934:	4eba dbec      	jsr %pc@(0x2522)
    4938:	2f0a           	movel %a2,%sp@-
    493a:	486e fffc      	pea %fp@(-4)
    493e:	2f2e 0010      	movel %fp@(16),%sp@-
    4942:	2f2e 000c      	movel %fp@(12),%sp@-
    4946:	4eba cd6e      	jsr %pc@(0x16b6)
    494a:	2800           	movel %d0,%d4
    494c:	4227           	clrb %sp@-
    494e:	4eba db7e      	jsr %pc@(0x24ce)
    4952:	102a 437a      	moveb %a2@(17274),%d0
    4956:	0240 0010      	andiw #16,%d0
    495a:	4fef 0020      	lea %sp@(32),%sp
    495e:	6608           	bnes 0x4968
    4960:	4227           	clrb %sp@-
    4962:	4eba dbbe      	jsr %pc@(0x2522)
    4966:	544f           	addqw #2,%sp
    4968:	4878 0064      	pea 0x64
    496c:	2f2a 0010      	movel %a2@(16),%sp@-
    4970:	4e4f           	trap #15
    4972:	a09a           	0120232
    4974:	3005           	movew %d5,%d0
    4976:	0240 00e0      	andiw #224,%d0
    497a:	3238 fc02      	movew 0xfffffffffffffc02,%d1
    497e:	0241 ff1f      	andiw #-225,%d1
    4982:	8240           	orw %d0,%d1
    4984:	0041 0001      	oriw #1,%d1
    4988:	31c1 fc02      	movew %d1,0xfffffffffffffc02
    498c:	4a84           	tstl %d4
    498e:	504f           	addqw #8,%sp
    4990:	6606           	bnes 0x4998
    4992:	26ae fffc      	movel %fp@(-4),%a3@
    4996:	6022           	bras 0x49ba
    4998:	7012           	moveq #18,%d0
    499a:	b880           	cmpl %d0,%d4
    499c:	6706           	beqs 0x49a4
    499e:	7014           	moveq #20,%d0
    49a0:	b880           	cmpl %d0,%d4
    49a2:	660c           	bnes 0x49b0
    49a4:	363c 2907      	movew #10503,%d3
    49a8:	7002           	moveq #2,%d0
    49aa:	81aa 0004      	orl %d0,%a2@(4)
    49ae:	600a           	bras 0x49ba
    49b0:	363c 2906      	movew #10502,%d3
    49b4:	6004           	bras 0x49ba
    49b6:	363c 2906      	movew #10502,%d3
    49ba:	2f2a 437e      	movel %a2@(17278),%sp@-
    49be:	4e4f           	trap #15
    49c0:	a0b1           	0120261
    49c2:	3003           	movew %d3,%d0
    49c4:	584f           	addqw #4,%sp
    49c6:	4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3
    49ca:	4e5e           	unlk %fp
    49cc:	4e75           	rts

```

# SlotPowerCheck: 0x49e4 - 0x4a6c


```

    49e4:	4e56 fffe      	linkw %fp,#-2
    49e8:	2f0a           	movel %a2,%sp@-
    49ea:	3f2e 0008      	movew %fp@(8),%sp@-
    49ee:	4e4f           	trap #15
    49f0:	a0b9           	0120271
    49f2:	2468 0004      	moveal %a0@(4),%a2
    49f6:	200a           	movel %a2,%d0
    49f8:	544f           	addqw #2,%sp
    49fa:	6606           	bnes 0x4a02
    49fc:	303c 290a      	movew #10506,%d0
    4a00:	6066           	bras 0x4a68
    4a02:	302a 4374      	movew %a2@(17268),%d0
    4a06:	b06e 000a      	cmpw %fp@(10),%d0
    4a0a:	6706           	beqs 0x4a12
    4a0c:	303c 2904      	movew #10500,%d0
    4a10:	6056           	bras 0x4a68
    4a12:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    4a16:	0240 0008      	andiw #8,%d0
    4a1a:	6706           	beqs 0x4a22
    4a1c:	303c 2903      	movew #10499,%d0
    4a20:	6046           	bras 0x4a68
    4a22:	102a 437a      	moveb %a2@(17274),%d0
    4a26:	0240 0004      	andiw #4,%d0
    4a2a:	6706           	beqs 0x4a32
    4a2c:	303c 2903      	movew #10499,%d0
    4a30:	6036           	bras 0x4a68
    4a32:	7001           	moveq #1,%d0
    4a34:	c0aa 0004      	andl %a2@(4),%d0
    4a38:	672a           	beqs 0x4a64
    4a3a:	486e ffff      	pea %fp@(-1)
    4a3e:	42a7           	clrl %sp@-
    4a40:	42a7           	clrl %sp@-
    4a42:	42a7           	clrl %sp@-
    4a44:	42a7           	clrl %sp@-
    4a46:	42a7           	clrl %sp@-
    4a48:	4227           	clrb %sp@-
    4a4a:	4e4f           	trap #15
    4a4c:	a324           	0121444
    4a4e:	0c2e 0007 ffff 	cmpib #7,%fp@(-1)
    4a54:	4fef 001a      	lea %sp@(26),%sp
    4a58:	6206           	bhis 0x4a60
    4a5a:	303c 2902      	movew #10498,%d0
    4a5e:	6002           	bras 0x4a62
    4a60:	7000           	moveq #0,%d0
    4a62:	6004           	bras 0x4a68
    4a64:	303c 2906      	movew #10502,%d0
    4a68:	245f           	moveal %sp@+,%a2
    4a6a:	4e5e           	unlk %fp
    4a6c:	4e75           	rts

```

# MemoryStickINSInterrupt: 0x4a80 - 0x4b58


```

    4a80:	4e56 0000      	linkw %fp,#0
    4a84:	2f0a           	movel %a2,%sp@-
    4a86:	1038 f41c      	moveb 0xfffffffffffff41c,%d0
    4a8a:	0240 0008      	andiw #8,%d0
    4a8e:	6754           	beqs 0x4ae4
    4a90:	4ab8 0304      	tstl 0x304
    4a94:	6734           	beqs 0x4aca
    4a96:	2078 0304      	moveal 0x304,%a0
    4a9a:	2468 0024      	moveal %a0@(36),%a2
    4a9e:	200a           	movel %a2,%d0
    4aa0:	6728           	beqs 0x4aca
    4aa2:	3f2a 4374      	movew %a2@(17268),%sp@-
    4aa6:	7406           	moveq #6,%d2
    4aa8:	4e4f           	trap #15
    4aaa:	a347           	0121507
    4aac:	2078 0304      	moveal 0x304,%a0
    4ab0:	2068 0024      	moveal %a0@(36),%a0
    4ab4:	0228 00fe 437a 	andib #-2,%a0@(17274)
    4aba:	2078 0304      	moveal 0x304,%a0
    4abe:	2068 0024      	moveal %a0@(36),%a0
    4ac2:	0028 0002 437a 	orib #2,%a0@(17274)
    4ac8:	544f           	addqw #2,%sp
    4aca:	0238 00f7 f41c 	andib #-9,0xfffffffffffff41c
    4ad0:	2078 0304      	moveal 0x304,%a0
    4ad4:	2468 0010      	moveal %a0@(16),%a2
    4ad8:	200a           	movel %a2,%d0
    4ada:	6768           	beqs 0x4b44
    4adc:	7001           	moveq #1,%d0
    4ade:	81aa 001c      	orl %d0,%a2@(28)
    4ae2:	6060           	bras 0x4b44
    4ae4:	4ab8 0304      	tstl 0x304
    4ae8:	6738           	beqs 0x4b22
    4aea:	2078 0304      	moveal 0x304,%a0
    4aee:	2468 0024      	moveal %a0@(36),%a2
    4af2:	200a           	movel %a2,%d0
    4af4:	672c           	beqs 0x4b22
    4af6:	3f2a 4374      	movew %a2@(17268),%sp@-
    4afa:	7407           	moveq #7,%d2
    4afc:	4e4f           	trap #15
    4afe:	a347           	0121507
    4b00:	2078 0304      	moveal 0x304,%a0
    4b04:	2068 0024      	moveal %a0@(36),%a0
    4b08:	41e8 437a      	lea %a0@(17274),%a0
    4b0c:	2448           	moveal %a0,%a2
    4b0e:	1010           	moveb %a0@,%d0
    4b10:	0240 0002      	andiw #2,%d0
    4b14:	544f           	addqw #2,%sp
    4b16:	6706           	beqs 0x4b1e
    4b18:	0212 00fd      	andib #-3,%a2@
    4b1c:	6004           	bras 0x4b22
    4b1e:	0012 0004      	orib #4,%a2@
    4b22:	4227           	clrb %sp@-
    4b24:	4eba d93e      	jsr %pc@(0x2464)
    4b28:	0038 0008 f41c 	orib #8,0xfffffffffffff41c
    4b2e:	2078 0304      	moveal 0x304,%a0
    4b32:	2468 0010      	moveal %a0@(16),%a2
    4b36:	200a           	movel %a2,%d0
    4b38:	544f           	addqw #2,%sp
    4b3a:	6708           	beqs 0x4b44
    4b3c:	02aa ffff ff00 	andil #-256,%a2@(28)
    4b42:	001c 
    4b44:	1038 f419      	moveb 0xfffffffffffff419,%d0
    4b48:	0200 fff0      	andib #-16,%d0
    4b4c:	0000 0008      	orib #8,%d0
    4b50:	11c0 f419      	moveb %d0,0xfffffffffffff419
    4b54:	245f           	moveal %sp@+,%a2
    4b56:	4e5e           	unlk %fp
    4b58:	4e75           	rts

```

# SlotHandleInterrupt: 0x4b74 - 0x4bc2


```

    4b74:	4e56 0000      	linkw %fp,#0
    4b78:	2f03           	movel %d3,%sp@-
    4b7a:	2078 0304      	moveal 0x304,%a0
    4b7e:	2268 0024      	moveal %a0@(36),%a1
    4b82:	3638 f110      	movew 0xfffffffffffff110,%d3
    4b86:	e24b           	lsrw #1,%d3
    4b88:	0243 0007      	andiw #7,%d3
    4b8c:	223c 0002 0000 	movel #131072,%d1
    4b92:	e7a9           	lsll %d3,%d1
    4b94:	7600           	moveq #0,%d3
    4b96:	3638 f100      	movew 0xfffffffffffff100,%d3
    4b9a:	740d           	moveq #13,%d2
    4b9c:	e5ab           	lsll %d2,%d3
    4b9e:	d681           	addl %d1,%d3
    4ba0:	2003           	movel %d3,%d0
    4ba2:	5480           	addql #2,%d0
    4ba4:	2040           	moveal %d0,%a0
    4ba6:	1010           	moveb %a0@,%d0
    4ba8:	0240 0080      	andiw #128,%d0
    4bac:	670e           	beqs 0x4bbc
    4bae:	2003           	movel %d3,%d0
    4bb0:	5c80           	addql #6,%d0
    4bb2:	2040           	moveal %d0,%a0
    4bb4:	1350 4388      	moveb %a0@,%a1@(17288)
    4bb8:	7001           	moveq #1,%d0
    4bba:	6002           	bras 0x4bbe
    4bbc:	7000           	moveq #0,%d0
    4bbe:	261f           	movel %sp@+,%d3
    4bc0:	4e5e           	unlk %fp
    4bc2:	4e75           	rts

```

# __Startup__: 0x4bda - 0x4bfc


```

    4bda:	4e56 0000      	linkw %fp,#0
    4bde:	2f03           	movel %d3,%sp@-
    4be0:	4eba e9a2      	jsr %pc@(0x3584)
    4be4:	3600           	movew %d0,%d3
    4be6:	6704           	beqs 0x4bec
    4be8:	3003           	movew %d3,%d0
    4bea:	600c           	bras 0x4bf8
    4bec:	4eba 001e      	jsr %pc@(0x4c0c)
    4bf0:	226e 000a      	moveal %fp@(10),%a1
    4bf4:	2288           	movel %a0,%a1@
    4bf6:	7000           	moveq #0,%d0
    4bf8:	261f           	movel %sp@+,%d3
    4bfa:	4e5e           	unlk %fp
    4bfc:	4e75           	rts

```

