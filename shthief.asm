
shthief.prc:     file format prc-m68k

Disassembly of section code.1:

00000000 <code.1>:
       0:	0000 0001      	orib #1,%d0
       4:	4e56 fff4      	linkw %fp,#-12
       8:	48e7 1f00      	moveml %d3-%d7,%sp@-
       c:	2e0c           	movel %a4,%d7
       e:	486e fff4      	pea %fp@(-12)
      12:	486e fff8      	pea %fp@(-8)
      16:	486e fffc      	pea %fp@(-4)
      1a:	4e4f           	trap #15
      1c:	a08f           	0120217
      1e:	defc 000c      	addaw #12,%sp
      22:	4a40           	tstw %d0
      24:	6710           	beqs 0x36
      26:	1f3c 0003      	moveb #3,%sp@-
      2a:	4e4f           	trap #15
      2c:	a234           	0121064
      2e:	2847           	moveal %d7,%a4
      30:	70ff           	moveq #-1,%d0
      32:	6000 005c      	braw 0x90
      36:	206e fffc      	moveal %fp@(-4),%a0
      3a:	3c10           	movew %a0@,%d6
      3c:	2a28 0002      	movel %a0@(2),%d5
      40:	3828 0006      	movew %a0@(6),%d4
      44:	0804 0002      	btst #2,%d4
      48:	6710           	beqs 0x5a
      4a:	284d           	moveal %a5,%a4
      4c:	99fc 0000 07b8 	subal #1976,%a4
      52:	6100 0076      	bsrw 0xca
      56:	6000 0004      	braw 0x5c
      5a:	99cc           	subal %a4,%a4
      5c:	3f04           	movew %d4,%sp@-
      5e:	2f05           	movel %d5,%sp@-
      60:	3f06           	movew %d6,%sp@-
      62:	6100 0180      	bsrw 0x1e4
      66:	3f04           	movew %d4,%sp@-
      68:	2f05           	movel %d5,%sp@-
      6a:	3f06           	movew %d6,%sp@-
      6c:	6100 020e      	bsrw 0x27c
      70:	2600           	movel %d0,%d3
      72:	3f04           	movew %d4,%sp@-
      74:	2f05           	movel %d5,%sp@-
      76:	3f06           	movew %d6,%sp@-
      78:	6100 01b8      	bsrw 0x232
      7c:	2f2e fff4      	movel %fp@(-12),%sp@-
      80:	2f2e fff8      	movel %fp@(-8),%sp@-
      84:	2f2e fffc      	movel %fp@(-4),%sp@-
      88:	4e4f           	trap #15
      8a:	a090           	0120220
      8c:	2847           	moveal %d7,%a4
      8e:	2003           	movel %d3,%d0
      90:	4cee 00f8 ffe0 	moveml %fp@(-32),%d3-%d7
      96:	4e5e           	unlk %fp
      98:	4e75           	rts
      9a:	756e           	072556
      9c:	6b6e           	bmis 0x10c
      9e:	6f77           	bles 0x117
      a0:	6e20           	bgts 0xc2
      a2:	7265           	moveq #101,%d1
      a4:	6c6f           	bges 0x115
      a6:	632e           	blss 0xd6
      a8:	7479           	moveq #121,%d2
      aa:	7065           	moveq #101,%d0
      ac:	0063 7274      	oriw #29300,%a3@-
      b0:	302e 6300      	movew %fp@(25344),%d0
      b4:	556e 6b6e      	subqw #2,%fp@(27502)
      b8:	6f77           	bles 0x131
      ba:	6e20           	bgts 0xdc
      bc:	7265           	moveq #101,%d1
      be:	6c6f           	bges 0x12f
      c0:	632e           	blss 0xf0
      c2:	7365           	071545
      c4:	6374           	blss 0x13a
      c6:	696f           	bvss 0x137
      c8:	6e00 4e56      	bgtw 0x4f20
      cc:	fff4           	0177764
      ce:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
      d2:	43ec 0000      	lea %a4@(0),%a1
      d6:	2d49 fffc      	movel %a1,%fp@(-4)
      da:	43ec 0080      	lea %a4@(128),%a1
      de:	2d49 fff8      	movel %a1,%fp@(-8)
      e2:	43fa ff20      	lea %pc@(0x4),%a1
      e6:	2d49 fff4      	movel %a1,%fp@(-12)
      ea:	41ec 0000      	lea %a4@(0),%a0
      ee:	4a50           	tstw %a0@
      f0:	6600 00e8      	bnew 0x1da
      f4:	30bc 0001      	movew #1,%a0@
      f8:	93fc 0001 0000 	subal #65536,%a1
      fe:	2d49 fff4      	movel %a1,%fp@(-12)
     102:	04ae 0000 0080 	subil #128,%fp@(-8)
     108:	fff8
     10a:	04ae 0000 0000 	subil #0,%fp@(-4)
     110:	fffc
     112:	4267           	clrw %sp@-
     114:	2f3c 726c 6f63 	movel #1919709027,%sp@-
     11a:	4e4f           	trap #15
     11c:	a060           	0120140
     11e:	2e08           	movel %a0,%d7
     120:	5c4f           	addqw #6,%sp
     122:	6700 00b6      	beqw 0x1da
     126:	2f07           	movel %d7,%sp@-
     128:	4e4f           	trap #15
     12a:	a021           	0120041
     12c:	3658           	moveaw %a0@+,%a3
     12e:	2448           	moveal %a0,%a2
     130:	4245           	clrw %d5
     132:	584f           	addqw #4,%sp
     134:	ba4b           	cmpw %a3,%d5
     136:	6400 0096      	bccw 0x1ce
     13a:	7c00           	moveq #0,%d6
     13c:	4244           	clrw %d4
     13e:	3c05           	movew %d5,%d6
     140:	2606           	movel %d6,%d3
     142:	e783           	asll #3,%d3
     144:	0c32 00be 3800 	cmpib #-66,%a2@(00000000,%d3:l)
     14a:	6718           	beqs 0x164
     14c:	43fa ff4c      	lea %pc@(0x9a),%a1
     150:	2f09           	movel %a1,%sp@-
     152:	3f3c 006b      	movew #107,%sp@-
     156:	43fa ff55      	lea %pc@(0xad),%a1
     15a:	2f09           	movel %a1,%sp@-
     15c:	4e4f           	trap #15
     15e:	a084           	0120204
     160:	defc 000a      	addaw #10,%sp
     164:	4281           	clrl %d1
     166:	3232 3802      	movew %a2@(00000002,%d3:l),%d1
     16a:	43ec 0000      	lea %a4@(0),%a1
     16e:	2009           	movel %a1,%d0
     170:	2041           	moveal %d1,%a0
     172:	d1c0           	addal %d0,%a0
     174:	1832 3801      	moveb %a2@(00000001,%d3:l),%d4
     178:	0c44 0064      	cmpiw #100,%d4
     17c:	671c           	beqs 0x19a
     17e:	6e0a           	bgts 0x18a
     180:	0c44 0062      	cmpiw #98,%d4
     184:	671e           	beqs 0x1a4
     186:	6000 0026      	braw 0x1ae
     18a:	0c44 0074      	cmpiw #116,%d4
     18e:	661e           	bnes 0x1ae
     190:	242e fff4      	movel %fp@(-12),%d2
     194:	d590           	addl %d2,%a0@
     196:	6000 002e      	braw 0x1c6
     19a:	242e fffc      	movel %fp@(-4),%d2
     19e:	d590           	addl %d2,%a0@
     1a0:	6000 0024      	braw 0x1c6
     1a4:	242e fff8      	movel %fp@(-8),%d2
     1a8:	d590           	addl %d2,%a0@
     1aa:	6000 001a      	braw 0x1c6
     1ae:	43fa ff04      	lea %pc@(0xb4),%a1
     1b2:	2f09           	movel %a1,%sp@-
     1b4:	3f3c 007e      	movew #126,%sp@-
     1b8:	43fa fef3      	lea %pc@(0xad),%a1
     1bc:	2f09           	movel %a1,%sp@-
     1be:	4e4f           	trap #15
     1c0:	a084           	0120204
     1c2:	defc 000a      	addaw #10,%sp
     1c6:	5245           	addqw #1,%d5
     1c8:	ba4b           	cmpw %a3,%d5
     1ca:	6500 ff72      	bcsw 0x13e
     1ce:	2f07           	movel %d7,%sp@-
     1d0:	4e4f           	trap #15
     1d2:	a022           	0120042
     1d4:	2f07           	movel %d7,%sp@-
     1d6:	4e4f           	trap #15
     1d8:	a061           	0120141
     1da:	4cee 0cf8 ffd8 	moveml %fp@(-40),%d3-%d7/%a2-%a3
     1e0:	4e5e           	unlk %fp
     1e2:	4e75           	rts
     1e4:	4e56 fffc      	linkw %fp,#-4
     1e8:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
     1ec:	3a2e 0008      	movew %fp@(8),%d5
     1f0:	282e 000a      	movel %fp@(10),%d4
     1f4:	362e 000e      	movew %fp@(14),%d3
     1f8:	43fa fe0a      	lea %pc@(0x4),%a1
     1fc:	93fc 0001 0000 	subal #65536,%a1
     202:	45fa 7c30      	lea %pc@(0x7e34),%a2
     206:	47fa 7c2c      	lea %pc@(0x7e34),%a3
     20a:	b7ca           	cmpal %a2,%a3
     20c:	631a           	blss 0x228
     20e:	205a           	moveal %a2@+,%a0
     210:	d1c9           	addal %a1,%a0
     212:	3f03           	movew %d3,%sp@-
     214:	2f04           	movel %d4,%sp@-
     216:	3f05           	movew %d5,%sp@-
     218:	2d49 fffc      	movel %a1,%fp@(-4)
     21c:	4e90           	jsr %a0@
     21e:	504f           	addqw #8,%sp
     220:	226e fffc      	moveal %fp@(-4),%a1
     224:	b7ca           	cmpal %a2,%a3
     226:	62e6           	bhis 0x20e
     228:	4cee 0c38 ffe8 	moveml %fp@(-24),%d3-%d5/%a2-%a3
     22e:	4e5e           	unlk %fp
     230:	4e75           	rts
     232:	4e56 0000      	linkw %fp,#0
     236:	48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-
     23a:	3c2e 0008      	movew %fp@(8),%d6
     23e:	2a2e 000a      	movel %fp@(10),%d5
     242:	382e 000e      	movew %fp@(14),%d4
     246:	47fa fdbc      	lea %pc@(0x4),%a3
     24a:	97fc 0001 0000 	subal #65536,%a3
     250:	43fa 7be2      	lea %pc@(0x7e34),%a1
     254:	2609           	movel %a1,%d3
     256:	45fa 7bdc      	lea %pc@(0x7e34),%a2
     25a:	b68a           	cmpl %a2,%d3
     25c:	6412           	bccs 0x270
     25e:	2062           	moveal %a2@-,%a0
     260:	d1cb           	addal %a3,%a0
     262:	3f04           	movew %d4,%sp@-
     264:	2f05           	movel %d5,%sp@-
     266:	3f06           	movew %d6,%sp@-
     268:	4e90           	jsr %a0@
     26a:	504f           	addqw #8,%sp
     26c:	b68a           	cmpl %a2,%d3
     26e:	65ee           	bcss 0x25e
     270:	4cee 0c78 ffe8 	moveml %fp@(-24),%d3-%d6/%a2-%a3
     276:	4e5e           	unlk %fp
     278:	4e75           	rts
     27a:	0000 4e56      	orib #86,%d0
     27e:	0000 4a6e      	orib #110,%d0
     282:	0008 6616      	orib #22,%a0
     286:	6100 001a      	bsrw 0x2a2
     28a:	4a40           	tstw %d0
     28c:	6706           	beqs 0x294
     28e:	48c0           	extl %d0
     290:	6000 000c      	braw 0x29e
     294:	6100 1798      	bsrw 0x1a2e
     298:	6100 18ce      	bsrw 0x1b68
     29c:	7000           	moveq #0,%d0
     29e:	4e5e           	unlk %fp
     2a0:	4e75           	rts
     2a2:	4e56 0000      	linkw %fp,#0
     2a6:	3f3c 03e8      	movew #1000,%sp@-
     2aa:	4e4f           	trap #15
     2ac:	a19b           	0120633
     2ae:	4e5e           	unlk %fp
     2b0:	4e75           	rts
     2b2:	4572           	042562
     2b4:	726f           	moveq #111,%d1
     2b6:	7220           	moveq #32,%d1
     2b8:	4c6f           	046157
     2ba:	6164           	bsrs 0x320
     2bc:	696e           	bvss 0x32c
     2be:	6720           	beqs 0x2e0
     2c0:	4f66           	047546
     2c2:	6673           	bnes 0x337
     2c4:	6372           	blss 0x338
     2c6:	6565           	bcss 0x32d
     2c8:	6e20           	bgts 0x2ea
     2ca:	496d           	044555
     2cc:	6167           	bsrs 0x335
     2ce:	6573           	bcss 0x343
     2d0:	2e00           	movel %d0,%d7
     2d2:	7368           	071550
     2d4:	6164           	bsrs 0x33a
     2d6:	6f77           	bles 0x34f
     2d8:	7468           	moveq #104,%d2
     2da:	6965           	bvss 0x341
     2dc:	662e           	bnes 0x30c
     2de:	6300 4572      	blsw 0x4852
     2e2:	726f           	moveq #111,%d1
     2e4:	7220           	moveq #32,%d1
     2e6:	4c6f           	046157
     2e8:	6164           	bsrs 0x34e
     2ea:	696e           	bvss 0x35a
     2ec:	6720           	beqs 0x30e
     2ee:	4f66           	047546
     2f0:	6673           	bnes 0x365
     2f2:	6372           	blss 0x366
     2f4:	6565           	bcss 0x35b
     2f6:	6e20           	bgts 0x318
     2f8:	496d           	044555
     2fa:	6167           	bsrs 0x363
     2fc:	6573           	bcss 0x371
     2fe:	2e20           	movel %a0@-,%d7
     300:	5b32 5d00      	subqb #5,%a2@(00000000,%d5:l:4)
     304:	4572           	042562
     306:	726f           	moveq #111,%d1
     308:	7220           	moveq #32,%d1
     30a:	4c6f           	046157
     30c:	6164           	bsrs 0x372
     30e:	696e           	bvss 0x37e
     310:	6720           	beqs 0x332
     312:	4865           	pea %a5@-
     314:	726f           	moveq #111,%d1
     316:	2057           	moveal %sp@,%a0
     318:	696e           	bvss 0x388
     31a:	646f           	bccs 0x38b
     31c:	772e           	073456
     31e:	0045 7272      	oriw #29298,%d5
     322:	6f72           	bles 0x396
     324:	204c           	moveal %a4,%a0
     326:	6f61           	bles 0x389
     328:	6469           	bccs 0x393
     32a:	6e67           	bgts 0x393
     32c:	2045           	moveal %d5,%a0
     32e:	6e65           	bgts 0x395
     330:	6d79           	blts 0x3ab
     332:	2057           	moveal %sp@,%a0
     334:	696e           	bvss 0x3a4
     336:	646f           	bccs 0x3a7
     338:	772e           	073456
     33a:	0045 7272      	oriw #29298,%d5
     33e:	6f72           	bles 0x3b2
     340:	204c           	moveal %a4,%a0
     342:	6f61           	bles 0x3a5
     344:	6469           	bccs 0x3af
     346:	6e67           	bgts 0x3af
     348:	2053           	moveal %a3@,%a0
     34a:	7475           	moveq #117,%d2
     34c:	6666           	bnes 0x3b4
     34e:	2057           	moveal %sp@,%a0
     350:	696e           	bvss 0x3c0
     352:	646f           	bccs 0x3c3
     354:	772e           	073456
     356:	0000 4e56      	orib #86,%d0
     35a:	fff4           	0177764
     35c:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
     360:	363c 00a0      	movew #160,%d3
     364:	3d43 fffc      	movew %d3,%fp@(-4)
     368:	4280           	clrl %d0
     36a:	302e fffc      	movew %fp@(-4),%d0
     36e:	0080 00a0 0000 	oril #10485760,%d0
     374:	2d40 fffa      	movel %d0,%fp@(-6)
     378:	426e fff8      	clrw %fp@(-8)
     37c:	4280           	clrl %d0
     37e:	302e fff8      	movew %fp@(-8),%d0
     382:	2d40 fff6      	movel %d0,%fp@(-10)
     386:	2a0e           	movel %fp,%d5
     388:	5585           	subql #2,%d5
     38a:	2f05           	movel %d5,%sp@-
     38c:	4227           	clrb %sp@-
     38e:	3f03           	movew %d3,%sp@-
     390:	3f03           	movew %d3,%sp@-
     392:	4e4f           	trap #15
     394:	a1f7           	0120767
     396:	45ec 0084      	lea %a4@(132),%a2
     39a:	2488           	movel %a0,%a2@
     39c:	defc 000a      	addaw #10,%sp
     3a0:	4a6e fffe      	tstw %fp@(-2)
     3a4:	6718           	beqs 0x3be
     3a6:	43fa ff0a      	lea %pc@(0x2b2),%a1
     3aa:	2f09           	movel %a1,%sp@-
     3ac:	3f3c 0130      	movew #304,%sp@-
     3b0:	43fa ff20      	lea %pc@(0x2d2),%a1
     3b4:	2f09           	movel %a1,%sp@-
     3b6:	4e4f           	trap #15
     3b8:	a084           	0120204
     3ba:	defc 000a      	addaw #10,%sp
     3be:	2f12           	movel %a2@,%sp@-
     3c0:	4e4f           	trap #15
     3c2:	a1fd           	0120775
     3c4:	2808           	movel %a0,%d4
     3c6:	4267           	clrw %sp@-
     3c8:	47ee fff6      	lea %fp@(-10),%a3
     3cc:	2f0b           	movel %a3,%sp@-
     3ce:	4e4f           	trap #15
     3d0:	a219           	0121031
     3d2:	2f04           	movel %d4,%sp@-
     3d4:	4e4f           	trap #15
     3d6:	a1fd           	0120775
     3d8:	2f05           	movel %d5,%sp@-
     3da:	4227           	clrb %sp@-
     3dc:	3f03           	movew %d3,%sp@-
     3de:	3f03           	movew %d3,%sp@-
     3e0:	4e4f           	trap #15
     3e2:	a1f7           	0120767
     3e4:	45ec 0080      	lea %a4@(128),%a2
     3e8:	2488           	movel %a0,%a2@
     3ea:	defc 0018      	addaw #24,%sp
     3ee:	4a6e fffe      	tstw %fp@(-2)
     3f2:	6718           	beqs 0x40c
     3f4:	43fa feea      	lea %pc@(0x2e0),%a1
     3f8:	2f09           	movel %a1,%sp@-
     3fa:	3f3c 0138      	movew #312,%sp@-
     3fe:	43fa fed2      	lea %pc@(0x2d2),%a1
     402:	2f09           	movel %a1,%sp@-
     404:	4e4f           	trap #15
     406:	a084           	0120204
     408:	defc 000a      	addaw #10,%sp
     40c:	2f12           	movel %a2@,%sp@-
     40e:	4e4f           	trap #15
     410:	a1fd           	0120775
     412:	2808           	movel %a0,%d4
     414:	4267           	clrw %sp@-
     416:	2f0b           	movel %a3,%sp@-
     418:	4e4f           	trap #15
     41a:	a219           	0121031
     41c:	2f04           	movel %d4,%sp@-
     41e:	4e4f           	trap #15
     420:	a1fd           	0120775
     422:	2f05           	movel %d5,%sp@-
     424:	4227           	clrb %sp@-
     426:	3f3c 0080      	movew #128,%sp@-
     42a:	3f3c 0140      	movew #320,%sp@-
     42e:	4e4f           	trap #15
     430:	a1f7           	0120767
     432:	45ec 0088      	lea %a4@(136),%a2
     436:	2488           	movel %a0,%a2@
     438:	defc 0018      	addaw #24,%sp
     43c:	4a6e fffe      	tstw %fp@(-2)
     440:	6718           	beqs 0x45a
     442:	43fa fec0      	lea %pc@(0x304),%a1
     446:	2f09           	movel %a1,%sp@-
     448:	3f3c 0140      	movew #320,%sp@-
     44c:	43fa fe84      	lea %pc@(0x2d2),%a1
     450:	2f09           	movel %a1,%sp@-
     452:	4e4f           	trap #15
     454:	a084           	0120204
     456:	defc 000a      	addaw #10,%sp
     45a:	2f12           	movel %a2@,%sp@-
     45c:	4e4f           	trap #15
     45e:	a1fd           	0120775
     460:	2808           	movel %a0,%d4
     462:	3f3c 03e9      	movew #1001,%sp@-
     466:	2f3c 5462 6d70 	movel #1415736688,%sp@-
     46c:	4e4f           	trap #15
     46e:	a05f           	0120137
     470:	2608           	movel %a0,%d3
     472:	2f03           	movel %d3,%sp@-
     474:	4e4f           	trap #15
     476:	a021           	0120041
     478:	4267           	clrw %sp@-
     47a:	4267           	clrw %sp@-
     47c:	2f08           	movel %a0,%sp@-
     47e:	4e4f           	trap #15
     480:	a226           	0121046
     482:	2f03           	movel %d3,%sp@-
     484:	4e4f           	trap #15
     486:	a022           	0120042
     488:	2f03           	movel %d3,%sp@-
     48a:	4e4f           	trap #15
     48c:	a061           	0120141
     48e:	2f04           	movel %d4,%sp@-
     490:	4e4f           	trap #15
     492:	a1fd           	0120775
     494:	defc 001e      	addaw #30,%sp
     498:	2e85           	movel %d5,%sp@
     49a:	4227           	clrb %sp@-
     49c:	3f3c 0080      	movew #128,%sp@-
     4a0:	3f3c 0140      	movew #320,%sp@-
     4a4:	4e4f           	trap #15
     4a6:	a1f7           	0120767
     4a8:	45ec 008c      	lea %a4@(140),%a2
     4ac:	2488           	movel %a0,%a2@
     4ae:	defc 000a      	addaw #10,%sp
     4b2:	4a6e fffe      	tstw %fp@(-2)
     4b6:	6718           	beqs 0x4d0
     4b8:	43fa fe65      	lea %pc@(0x31f),%a1
     4bc:	2f09           	movel %a1,%sp@-
     4be:	3f3c 014f      	movew #335,%sp@-
     4c2:	43fa fe0e      	lea %pc@(0x2d2),%a1
     4c6:	2f09           	movel %a1,%sp@-
     4c8:	4e4f           	trap #15
     4ca:	a084           	0120204
     4cc:	defc 000a      	addaw #10,%sp
     4d0:	2f12           	movel %a2@,%sp@-
     4d2:	4e4f           	trap #15
     4d4:	a1fd           	0120775
     4d6:	2808           	movel %a0,%d4
     4d8:	3f3c 03e9      	movew #1001,%sp@-
     4dc:	2f3c 5462 6d70 	movel #1415736688,%sp@-
     4e2:	4e4f           	trap #15
     4e4:	a05f           	0120137
     4e6:	2608           	movel %a0,%d3
     4e8:	2f03           	movel %d3,%sp@-
     4ea:	4e4f           	trap #15
     4ec:	a021           	0120041
     4ee:	4267           	clrw %sp@-
     4f0:	4267           	clrw %sp@-
     4f2:	2f08           	movel %a0,%sp@-
     4f4:	4e4f           	trap #15
     4f6:	a226           	0121046
     4f8:	2f03           	movel %d3,%sp@-
     4fa:	4e4f           	trap #15
     4fc:	a022           	0120042
     4fe:	2f03           	movel %d3,%sp@-
     500:	4e4f           	trap #15
     502:	a061           	0120141
     504:	2f04           	movel %d4,%sp@-
     506:	4e4f           	trap #15
     508:	a1fd           	0120775
     50a:	defc 001e      	addaw #30,%sp
     50e:	2e85           	movel %d5,%sp@
     510:	4227           	clrb %sp@-
     512:	3f3c 0080      	movew #128,%sp@-
     516:	3f3c 0140      	movew #320,%sp@-
     51a:	4e4f           	trap #15
     51c:	a1f7           	0120767
     51e:	45ec 0090      	lea %a4@(144),%a2
     522:	2488           	movel %a0,%a2@
     524:	defc 000a      	addaw #10,%sp
     528:	4a6e fffe      	tstw %fp@(-2)
     52c:	6718           	beqs 0x546
     52e:	43fa fe0b      	lea %pc@(0x33b),%a1
     532:	2f09           	movel %a1,%sp@-
     534:	3f3c 015e      	movew #350,%sp@-
     538:	43fa fd98      	lea %pc@(0x2d2),%a1
     53c:	2f09           	movel %a1,%sp@-
     53e:	4e4f           	trap #15
     540:	a084           	0120204
     542:	defc 000a      	addaw #10,%sp
     546:	2f12           	movel %a2@,%sp@-
     548:	4e4f           	trap #15
     54a:	a1fd           	0120775
     54c:	2808           	movel %a0,%d4
     54e:	3f3c 03ea      	movew #1002,%sp@-
     552:	2f3c 5462 6d70 	movel #1415736688,%sp@-
     558:	4e4f           	trap #15
     55a:	a05f           	0120137
     55c:	2608           	movel %a0,%d3
     55e:	2f03           	movel %d3,%sp@-
     560:	4e4f           	trap #15
     562:	a021           	0120041
     564:	4267           	clrw %sp@-
     566:	4267           	clrw %sp@-
     568:	2f08           	movel %a0,%sp@-
     56a:	4e4f           	trap #15
     56c:	a226           	0121046
     56e:	2f03           	movel %d3,%sp@-
     570:	4e4f           	trap #15
     572:	a022           	0120042
     574:	2f03           	movel %d3,%sp@-
     576:	4e4f           	trap #15
     578:	a061           	0120141
     57a:	2f04           	movel %d4,%sp@-
     57c:	4e4f           	trap #15
     57e:	a1fd           	0120775
     580:	4cee 0c38 ffe0 	moveml %fp@(-32),%d3-%d5/%a2-%a3
     586:	4e5e           	unlk %fp
     588:	4e75           	rts
     58a:	7368           	071550
     58c:	5468 6965      	addqw #2,%a0@(26981)
     590:	6625           	bnes 0x5b7
     592:	6450           	bccs 0x5e4
     594:	6172           	bsrs 0x608
     596:	7441           	moveq #65,%d2
     598:	0025 6400      	orib #0,%a5@-
     59c:	7368           	071550
     59e:	5468 6965      	addqw #2,%a0@(26981)
     5a2:	6625           	bnes 0x5c9
     5a4:	6453           	bccs 0x5f9
     5a6:	6176           	bsrs 0x61e
     5a8:	6500 7368      	bcsw 0x7912
     5ac:	5468 6965      	addqw #2,%a0@(26981)
     5b0:	6625           	bnes 0x5d7
     5b2:	6450           	bccs 0x604
     5b4:	6172           	bsrs 0x628
     5b6:	7442           	moveq #66,%d2
     5b8:	0000 4e56      	orib #86,%d0
     5bc:	ff64           	frestore (cpid=7) %a4@-
     5be:	48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-
     5c2:	4245           	clrw %d5
     5c4:	7c01           	moveq #1,%d6
     5c6:	7801           	moveq #1,%d4
     5c8:	45ee fff0      	lea %fp@(-16),%a2
     5cc:	47ee ffb4      	lea %fp@(-76),%a3
     5d0:	3f04           	movew %d4,%sp@-
     5d2:	43fa ffb6      	lea %pc@(0x58a),%a1
     5d6:	2f09           	movel %a1,%sp@-
     5d8:	2f0a           	movel %a2,%sp@-
     5da:	4e4f           	trap #15
     5dc:	a2de           	0121336
     5de:	2f0a           	movel %a2,%sp@-
     5e0:	4267           	clrw %sp@-
     5e2:	4e4f           	trap #15
     5e4:	a045           	0120105
     5e6:	defc 0010      	addaw #16,%sp
     5ea:	4a80           	tstl %d0
     5ec:	6734           	beqs 0x622
     5ee:	3f04           	movew %d4,%sp@-
     5f0:	43fa ffa7      	lea %pc@(0x599),%a1
     5f4:	2f09           	movel %a1,%sp@-
     5f6:	3605           	movew %d5,%d3
     5f8:	c6fc 0003      	muluw #3,%d3
     5fc:	d68b           	addl %a3,%d3
     5fe:	2f03           	movel %d3,%sp@-
     600:	4e4f           	trap #15
     602:	a2de           	0121336
     604:	4280           	clrl %d0
     606:	3005           	movew %d5,%d0
     608:	e580           	asll #2,%d0
     60a:	41f6 0800      	lea %fp@(00000000,%d0:l),%a0
     60e:	2143 ff64      	movel %d3,%a0@(-156)
     612:	5245           	addqw #1,%d5
     614:	defc 000a      	addaw #10,%sp
     618:	0c45 0014      	cmpiw #20,%d5
     61c:	6602           	bnes 0x620
     61e:	7a19           	moveq #25,%d5
     620:	3c04           	movew %d4,%d6
     622:	5244           	addqw #1,%d4
     624:	0c44 0018      	cmpiw #24,%d4
     628:	63a6           	blss 0x5d0
     62a:	0c45 0001      	cmpiw #1,%d5
     62e:	636a           	blss 0x69a
     630:	3f3c 041c      	movew #1052,%sp@-
     634:	4e4f           	trap #15
     636:	a16f           	0120557
     638:	2608           	movel %a0,%d3
     63a:	3f05           	movew %d5,%sp@-
     63c:	486e ff64      	pea %fp@(-156)
     640:	3f3c 041d      	movew #1053,%sp@-
     644:	2f03           	movel %d3,%sp@-
     646:	4e4f           	trap #15
     648:	a180           	0120600
     64a:	5c4f           	addqw #6,%sp
     64c:	3f00           	movew %d0,%sp@-
     64e:	2f03           	movel %d3,%sp@-
     650:	4e4f           	trap #15
     652:	a183           	0120603
     654:	544f           	addqw #2,%sp
     656:	2e88           	movel %a0,%sp@
     658:	4e4f           	trap #15
     65a:	a1b8           	0120670
     65c:	2f03           	movel %d3,%sp@-
     65e:	4e4f           	trap #15
     660:	a193           	0120623
     662:	2f03           	movel %d3,%sp@-
     664:	4e4f           	trap #15
     666:	a170           	0120560
     668:	3f3c 041d      	movew #1053,%sp@-
     66c:	2f03           	movel %d3,%sp@-
     66e:	4e4f           	trap #15
     670:	a180           	0120600
     672:	3f00           	movew %d0,%sp@-
     674:	2f03           	movel %d3,%sp@-
     676:	4e4f           	trap #15
     678:	a183           	0120603
     67a:	2f08           	movel %a0,%sp@-
     67c:	4e4f           	trap #15
     67e:	a1b3           	0120663
     680:	defc 0024      	addaw #36,%sp
     684:	4a40           	tstw %d0
     686:	6c02           	bges 0x68a
     688:	4240           	clrw %d0
     68a:	c1fc 0003      	mulsw #3,%d0
     68e:	4876 08b4      	pea %fp@(ffffffffffffffb4,%d0:l)
     692:	4e4f           	trap #15
     694:	a0ce           	0120316
     696:	3c00           	movew %d0,%d6
     698:	584f           	addqw #4,%sp
     69a:	0c45 0001      	cmpiw #1,%d5
     69e:	6620           	bnes 0x6c0
     6a0:	4a2c 000c      	tstb %a4@(12)
     6a4:	661a           	bnes 0x6c0
     6a6:	3f3c 041e      	movew #1054,%sp@-
     6aa:	4e4f           	trap #15
     6ac:	a16f           	0120557
     6ae:	2608           	movel %a0,%d3
     6b0:	2f03           	movel %d3,%sp@-
     6b2:	4e4f           	trap #15
     6b4:	a193           	0120623
     6b6:	2f03           	movel %d3,%sp@-
     6b8:	4e4f           	trap #15
     6ba:	a170           	0120560
     6bc:	defc 000a      	addaw #10,%sp
     6c0:	4a45           	tstw %d5
     6c2:	661a           	bnes 0x6de
     6c4:	3f3c 0422      	movew #1058,%sp@-
     6c8:	4e4f           	trap #15
     6ca:	a16f           	0120557
     6cc:	2608           	movel %a0,%d3
     6ce:	2f03           	movel %d3,%sp@-
     6d0:	4e4f           	trap #15
     6d2:	a193           	0120623
     6d4:	2f03           	movel %d3,%sp@-
     6d6:	4e4f           	trap #15
     6d8:	a170           	0120560
     6da:	defc 000a      	addaw #10,%sp
     6de:	1946 0020      	moveb %d6,%a4@(32)
     6e2:	3f06           	movew %d6,%sp@-
     6e4:	43fa feb6      	lea %pc@(0x59c),%a1
     6e8:	2f09           	movel %a1,%sp@-
     6ea:	486c 0025      	pea %a4@(37)
     6ee:	4e4f           	trap #15
     6f0:	a2de           	0121336
     6f2:	3f06           	movew %d6,%sp@-
     6f4:	43fa fe94      	lea %pc@(0x58a),%a1
     6f8:	2f09           	movel %a1,%sp@-
     6fa:	486c 0033      	pea %a4@(51)
     6fe:	4e4f           	trap #15
     700:	a2de           	0121336
     702:	3f06           	movew %d6,%sp@-
     704:	43fa fea4      	lea %pc@(0x5aa),%a1
     708:	2f09           	movel %a1,%sp@-
     70a:	486c 0042      	pea %a4@(66)
     70e:	4e4f           	trap #15
     710:	a2de           	0121336
     712:	4cee 0c78 ff4c 	moveml %fp@(-180),%d3-%d6/%a2-%a3
     718:	4e5e           	unlk %fp
     71a:	4e75           	rts
     71c:	5368 6164      	subqw #1,%a0@(24932)
     720:	6f77           	bles 0x799
     722:	5468 6965      	addqw #2,%a0@(26981)
     726:	6600 4164      	bnew 0x488c
     72a:	7665           	moveq #101,%d3
     72c:	6e74           	bgts 0x7a2
     72e:	7572           	072562
     730:	6520           	bcss 0x752
     732:	2564 004c      	movel %a4@-,%a2@(76)
     736:	6f61           	bles 0x799
     738:	6469           	bccs 0x7a3
     73a:	6e67           	bgts 0x7a3
     73c:	2e20           	movel %a0@-,%d7
     73e:	506c 6561      	addqw #8,%a4@(25953)
     742:	7365           	071545
     744:	2057           	moveal %sp@,%a0
     746:	6169           	bsrs 0x7b1
     748:	742e           	moveq #46,%d2
     74a:	2e2e 0054      	movel %fp@(84),%d7
     74e:	6865           	bvcs 0x7b5
     750:	2073 6854      	moveal %a3@(00000054,%d6:l),%a0
     754:	6869           	bvcs 0x7bf
     756:	6566           	bcss 0x7be
     758:	2350 6172      	movel %a0@,%a1@(24946)
     75c:	7442           	moveq #66,%d2
     75e:	2064           	moveal %a4@-,%a0
     760:	6174           	bsrs 0x7d6
     762:	6162           	bsrs 0x7c6
     764:	6173           	bsrs 0x7d9
     766:	6520           	bcss 0x788
     768:	6361           	blss 0x7cb
     76a:	6e6e           	bgts 0x7da
     76c:	6f74           	bles 0x7e2
     76e:	2062           	moveal %a2@-,%a0
     770:	6520           	bcss 0x792
     772:	666f           	bnes 0x7e3
     774:	756e           	072556
     776:	642e           	bccs 0x7a6
     778:	0025 7300      	orib #0,%a5@-
     77c:	636f           	blss 0x7ed
     77e:	6e74           	bgts 0x7f4
     780:	696e           	bvss 0x7f0
     782:	7565           	072545
     784:	732e           	071456
     786:	2e2e 0000      	movel %fp@(0),%d7
     78a:	4e56 ffec      	linkw %fp,#-20
     78e:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
     792:	426e ffee      	clrw %fp@(-18)
     796:	4243           	clrw %d3
     798:	43ec 0025      	lea %a4@(37),%a1
     79c:	2809           	movel %a1,%d4
     79e:	2f04           	movel %d4,%sp@-
     7a0:	4267           	clrw %sp@-
     7a2:	4e4f           	trap #15
     7a4:	a045           	0120105
     7a6:	5c4f           	addqw #6,%sp
     7a8:	4a80           	tstl %d0
     7aa:	6600 0218      	bnew 0x9c4
     7ae:	486c 0033      	pea %a4@(51)
     7b2:	4267           	clrw %sp@-
     7b4:	4e4f           	trap #15
     7b6:	a045           	0120105
     7b8:	5c4f           	addqw #6,%sp
     7ba:	4a80           	tstl %d0
     7bc:	6600 00ac      	bnew 0x86a
     7c0:	4227           	clrb %sp@-
     7c2:	2f3c 4461 7461 	movel #1147237473,%sp@-
     7c8:	2f3c 5a61 6e65 	movel #1516334693,%sp@-
     7ce:	2f04           	movel %d4,%sp@-
     7d0:	4267           	clrw %sp@-
     7d2:	4e4f           	trap #15
     7d4:	a041           	0120101
     7d6:	defc 0010      	addaw #16,%sp
     7da:	4a40           	tstw %d0
     7dc:	6600 00bc      	bnew 0x89a
     7e0:	3f3c 0003      	movew #3,%sp@-
     7e4:	2f04           	movel %d4,%sp@-
     7e6:	4267           	clrw %sp@-
     7e8:	4e4f           	trap #15
     7ea:	a045           	0120105
     7ec:	544f           	addqw #2,%sp
     7ee:	2e80           	movel %d0,%sp@
     7f0:	4267           	clrw %sp@-
     7f2:	4e4f           	trap #15
     7f4:	a049           	0120111
     7f6:	45ec 073c      	lea %a4@(1852),%a2
     7fa:	2488           	movel %a0,%a2@
     7fc:	4878 0128      	pea 0x128
     800:	486e ffee      	pea %fp@(-18)
     804:	2f08           	movel %a0,%sp@-
     806:	4e4f           	trap #15
     808:	a055           	0120125
     80a:	2f08           	movel %a0,%sp@-
     80c:	4e4f           	trap #15
     80e:	a021           	0120041
     810:	2608           	movel %a0,%d3
     812:	6100 1138      	bsrw 0x194c
     816:	4878 0105      	pea 0x105
     81a:	486c 0100      	pea %a4@(256)
     81e:	42a7           	clrl %sp@-
     820:	2f03           	movel %d3,%sp@-
     822:	4e4f           	trap #15
     824:	a076           	0120166
     826:	defc 0024      	addaw #36,%sp
     82a:	2e83           	movel %d3,%sp@
     82c:	4e4f           	trap #15
     82e:	a035           	0120065
     830:	1f3c 0001      	moveb #1,%sp@-
     834:	3f2e ffee      	movew %fp@(-18),%sp@-
     838:	2f12           	movel %a2@,%sp@-
     83a:	4e4f           	trap #15
     83c:	a05e           	0120136
     83e:	197c 0001 0006 	moveb #1,%a4@(6)
     844:	422c 0007      	clrb %a4@(7)
     848:	422c 0020      	clrb %a4@(32)
     84c:	197c 0001 0016 	moveb #1,%a4@(22)
     852:	4267           	clrw %sp@-
     854:	6100 084a      	bsrw 0x10a0
     858:	43fa fec2      	lea %pc@(0x71c),%a1
     85c:	2f09           	movel %a1,%sp@-
     85e:	486c 0098      	pea %a4@(152)
     862:	4e4f           	trap #15
     864:	a0c5           	0120305
     866:	6000 0262      	braw 0xaca
     86a:	3f3c 0001      	movew #1,%sp@-
     86e:	2f00           	movel %d0,%sp@-
     870:	4267           	clrw %sp@-
     872:	4e4f           	trap #15
     874:	a049           	0120111
     876:	45ec 00f4      	lea %a4@(244),%a2
     87a:	2488           	movel %a0,%a2@
     87c:	4227           	clrb %sp@-
     87e:	2f3c 4461 7461 	movel #1147237473,%sp@-
     884:	2f3c 5a61 6e65 	movel #1516334693,%sp@-
     88a:	2f04           	movel %d4,%sp@-
     88c:	4267           	clrw %sp@-
     88e:	4e4f           	trap #15
     890:	a041           	0120101
     892:	defc 0018      	addaw #24,%sp
     896:	4a40           	tstw %d0
     898:	6706           	beqs 0x8a0
     89a:	7001           	moveq #1,%d0
     89c:	6000 022e      	braw 0xacc
     8a0:	3f3c 0003      	movew #3,%sp@-
     8a4:	2f04           	movel %d4,%sp@-
     8a6:	4267           	clrw %sp@-
     8a8:	4e4f           	trap #15
     8aa:	a045           	0120105
     8ac:	544f           	addqw #2,%sp
     8ae:	2e80           	movel %d0,%sp@
     8b0:	4267           	clrw %sp@-
     8b2:	4e4f           	trap #15
     8b4:	a049           	0120111
     8b6:	2948 073c      	movel %a0,%a4@(1852)
     8ba:	47ec 0006      	lea %a4@(6),%a3
     8be:	2f12           	movel %a2@,%sp@-
     8c0:	4e4f           	trap #15
     8c2:	a04f           	0120117
     8c4:	1680           	moveb %d0,%a3@
     8c6:	6100 0714      	bsrw 0xfdc
     8ca:	4240           	clrw %d0
     8cc:	102c 0020      	moveb %a4@(32),%d0
     8d0:	3f00           	movew %d0,%sp@-
     8d2:	43fa fe54      	lea %pc@(0x728),%a1
     8d6:	2f09           	movel %a1,%sp@-
     8d8:	45ee fff0      	lea %fp@(-16),%a2
     8dc:	2f0a           	movel %a2,%sp@-
     8de:	4e4f           	trap #15
     8e0:	a2de           	0121336
     8e2:	4267           	clrw %sp@-
     8e4:	3f3c 0032      	movew #50,%sp@-
     8e8:	2f0a           	movel %a2,%sp@-
     8ea:	6100 3d30      	bsrw 0x461c
     8ee:	4267           	clrw %sp@-
     8f0:	6100 09da      	bsrw 0x12cc
     8f4:	defc 0020      	addaw #32,%sp
     8f8:	3f3c 0001      	movew #1,%sp@-
     8fc:	3f3c 0041      	movew #65,%sp@-
     900:	486c 0100      	pea %a4@(256)
     904:	6100 3d16      	bsrw 0x461c
     908:	4267           	clrw %sp@-
     90a:	3f3c 0082      	movew #130,%sp@-
     90e:	43fa fe25      	lea %pc@(0x735),%a1
     912:	2f09           	movel %a1,%sp@-
     914:	6100 3d06      	bsrw 0x461c
     918:	defc 0010      	addaw #16,%sp
     91c:	4240           	clrw %d0
     91e:	1013           	moveb %a3@,%d0
     920:	b043           	cmpw %d3,%d0
     922:	6342           	blss 0x966
     924:	4244           	clrw %d4
     926:	3f03           	movew %d3,%sp@-
     928:	6100 09a2      	bsrw 0x12cc
     92c:	3f03           	movew %d3,%sp@-
     92e:	6100 0b92      	bsrw 0x14c2
     932:	5243           	addqw #1,%d3
     934:	3003           	movew %d3,%d0
     936:	c1fc 0064      	mulsw #100,%d0
     93a:	45ec 0006      	lea %a4@(6),%a2
     93e:	1812           	moveb %a2@,%d4
     940:	0280 0000 ffff 	andil #65535,%d0
     946:	80c4           	divuw %d4,%d0
     948:	3f00           	movew %d0,%sp@-
     94a:	3f3c 005a      	movew #90,%sp@-
     94e:	3f3c 0091      	movew #145,%sp@-
     952:	3f3c 0023      	movew #35,%sp@-
     956:	6100 46e4      	bsrw 0x503c
     95a:	defc 000c      	addaw #12,%sp
     95e:	4240           	clrw %d0
     960:	1012           	moveb %a2@,%d0
     962:	b043           	cmpw %d3,%d0
     964:	62c0           	bhis 0x926
     966:	2f2c 00f4      	movel %a4@(244),%sp@-
     96a:	4e4f           	trap #15
     96c:	a04a           	0120112
     96e:	486c 0042      	pea %a4@(66)
     972:	4267           	clrw %sp@-
     974:	4e4f           	trap #15
     976:	a045           	0120105
     978:	defc 000a      	addaw #10,%sp
     97c:	4a80           	tstl %d0
     97e:	6714           	beqs 0x994
     980:	3f3c 0001      	movew #1,%sp@-
     984:	2f00           	movel %d0,%sp@-
     986:	4267           	clrw %sp@-
     988:	4e4f           	trap #15
     98a:	a049           	0120111
     98c:	2948 00c0      	movel %a0,%a4@(192)
     990:	6000 0012      	braw 0x9a4
     994:	43fa fdb7      	lea %pc@(0x74d),%a1
     998:	2f09           	movel %a1,%sp@-
     99a:	4267           	clrw %sp@-
     99c:	3f3c 029c      	movew #668,%sp@-
     9a0:	6100 3cea      	bsrw 0x468c
     9a4:	504f           	addqw #8,%sp
     9a6:	4267           	clrw %sp@-
     9a8:	6100 06f6      	bsrw 0x10a0
     9ac:	6100 01ca      	bsrw 0xb78
     9b0:	422c 0007      	clrb %a4@(7)
     9b4:	6100 02d4      	bsrw 0xc8a
     9b8:	486c 0100      	pea %a4@(256)
     9bc:	43fa fdbb      	lea %pc@(0x779),%a1
     9c0:	6000 00fe      	braw 0xac0
     9c4:	3f3c 0003      	movew #3,%sp@-
     9c8:	2f00           	movel %d0,%sp@-
     9ca:	4267           	clrw %sp@-
     9cc:	4e4f           	trap #15
     9ce:	a049           	0120111
     9d0:	2948 073c      	movel %a0,%a4@(1852)
     9d4:	504f           	addqw #8,%sp
     9d6:	4a2c 0016      	tstb %a4@(22)
     9da:	6600 00be      	bnew 0xa9a
     9de:	6100 05fc      	bsrw 0xfdc
     9e2:	4240           	clrw %d0
     9e4:	102c 0020      	moveb %a4@(32),%d0
     9e8:	3f00           	movew %d0,%sp@-
     9ea:	43fa fd3c      	lea %pc@(0x728),%a1
     9ee:	2f09           	movel %a1,%sp@-
     9f0:	45ee fff0      	lea %fp@(-16),%a2
     9f4:	2f0a           	movel %a2,%sp@-
     9f6:	4e4f           	trap #15
     9f8:	a2de           	0121336
     9fa:	4267           	clrw %sp@-
     9fc:	3f3c 0032      	movew #50,%sp@-
     a00:	2f0a           	movel %a2,%sp@-
     a02:	6100 3c18      	bsrw 0x461c
     a06:	4267           	clrw %sp@-
     a08:	6100 0696      	bsrw 0x10a0
     a0c:	3f3c 0001      	movew #1,%sp@-
     a10:	3f3c 0041      	movew #65,%sp@-
     a14:	486c 0100      	pea %a4@(256)
     a18:	6100 3c02      	bsrw 0x461c
     a1c:	4267           	clrw %sp@-
     a1e:	3f3c 0078      	movew #120,%sp@-
     a22:	43fa fd58      	lea %pc@(0x77c),%a1
     a26:	2f09           	movel %a1,%sp@-
     a28:	6100 3bf2      	bsrw 0x461c
     a2c:	7200           	moveq #0,%d1
     a2e:	defc 0024      	addaw #36,%sp
     a32:	7000           	moveq #0,%d0
     a34:	5280           	addql #1,%d0
     a36:	0c80 0001 869f 	cmpil #99999,%d0
     a3c:	63f6           	blss 0xa34
     a3e:	5281           	addql #1,%d1
     a40:	7409           	moveq #9,%d2
     a42:	b481           	cmpl %d1,%d2
     a44:	64ec           	bccs 0xa32
     a46:	6100 0130      	bsrw 0xb78
     a4a:	7200           	moveq #0,%d1
     a4c:	7000           	moveq #0,%d0
     a4e:	5280           	addql #1,%d0
     a50:	0c80 0001 869f 	cmpil #99999,%d0
     a56:	63f6           	blss 0xa4e
     a58:	5281           	addql #1,%d1
     a5a:	7409           	moveq #9,%d2
     a5c:	b481           	cmpl %d1,%d2
     a5e:	64ec           	bccs 0xa4c
     a60:	486c 0042      	pea %a4@(66)
     a64:	4267           	clrw %sp@-
     a66:	4e4f           	trap #15
     a68:	a045           	0120105
     a6a:	5c4f           	addqw #6,%sp
     a6c:	4a80           	tstl %d0
     a6e:	6714           	beqs 0xa84
     a70:	3f3c 0001      	movew #1,%sp@-
     a74:	2f00           	movel %d0,%sp@-
     a76:	4267           	clrw %sp@-
     a78:	4e4f           	trap #15
     a7a:	a049           	0120111
     a7c:	2948 00c0      	movel %a0,%a4@(192)
     a80:	6000 0016      	braw 0xa98
     a84:	42ac 00c0      	clrl %a4@(192)
     a88:	43fa fcc3      	lea %pc@(0x74d),%a1
     a8c:	2f09           	movel %a1,%sp@-
     a8e:	4267           	clrw %sp@-
     a90:	3f3c 029c      	movew #668,%sp@-
     a94:	6100 3bf6      	bsrw 0x468c
     a98:	504f           	addqw #8,%sp
     a9a:	45ec 0006      	lea %a4@(6),%a2
     a9e:	2f2c 073c      	movel %a4@(1852),%sp@-
     aa2:	4e4f           	trap #15
     aa4:	a04f           	0120117
     aa6:	1480           	moveb %d0,%a2@
     aa8:	422c 0007      	clrb %a4@(7)
     aac:	584f           	addqw #4,%sp
     aae:	4a2c 0016      	tstb %a4@(22)
     ab2:	6604           	bnes 0xab8
     ab4:	6100 01d4      	bsrw 0xc8a
     ab8:	486c 0100      	pea %a4@(256)
     abc:	43fa fcbb      	lea %pc@(0x779),%a1
     ac0:	2f09           	movel %a1,%sp@-
     ac2:	486c 0098      	pea %a4@(152)
     ac6:	4e4f           	trap #15
     ac8:	a2de           	0121336
     aca:	4240           	clrw %d0
     acc:	4cee 0c18 ffdc 	moveml %fp@(-36),%d3-%d4/%a2-%a3
     ad2:	4e5e           	unlk %fp
     ad4:	4e75           	rts
     ad6:	5761           	subqw #3,%a1@-
     ad8:	726e           	moveq #110,%d1
     ada:	696e           	bvss 0xb4a
     adc:	673a           	beqs 0xb18
     ade:	0054 6869      	oriw #26729,%a4@
     ae2:	7320           	071440
     ae4:	4164           	040544
     ae6:	7665           	moveq #101,%d3
     ae8:	6e74           	bgts 0xb5e
     aea:	7572           	072562
     aec:	6520           	bcss 0xb0e
     aee:	7761           	073541
     af0:	7320           	071440
     af2:	6465           	bccs 0xb59
     af4:	7369           	071551
     af6:	676e           	beqs 0xb66
     af8:	6564           	bcss 0xb5e
     afa:	0066 6f72      	oriw #28530,%fp@-
     afe:	2075 7365 2077 	moveal %a5@(00002077)@(00000000),%a0
     b04:	6974           	bvss 0xb7a
     b06:	6820           	bvcs 0xb28
     b08:	5368 6164      	subqw #1,%a0@(24932)
     b0c:	6f77           	bles 0xb85
     b0e:	5468 6965      	addqw #2,%a0@(26981)
     b12:	6600 5665      	bnew 0x6179
     b16:	7273           	moveq #115,%d1
     b18:	696f           	bvss 0xb89
     b1a:	6e20           	bgts 0xb3c
     b1c:	2564 2e25      	movel %a4@-,%a2@(11813)
     b20:	6425           	bccs 0xb47
     b22:	6400 6f72      	bccw 0x7a96
     b26:	2067           	moveal %sp@-,%a0
     b28:	7265           	moveq #101,%d1
     b2a:	6174           	bsrs 0xba0
     b2c:	6572           	bcss 0xba0
     b2e:	2e00           	movel %d0,%d7
     b30:	5570 6772 6164 	subqw #2,%a0@(61646520)@(0000746f)
     b36:	6520 746f
     b3a:	2074 6865      	moveal %a4@(00000065,%d6:l),%a0
     b3e:	206c 6174      	moveal %a4@(24948),%a0
     b42:	6573           	bcss 0xbb7
     b44:	7400           	moveq #0,%d2
     b46:	7665           	moveq #101,%d3
     b48:	7273           	moveq #115,%d1
     b4a:	696f           	bvss 0xbbb
     b4c:	6e20           	bgts 0xb6e
     b4e:	6f66           	bles 0xbb6
     b50:	2053           	moveal %a3@,%a0
     b52:	6861           	bvcs 0xbb5
     b54:	646f           	bccs 0xbc5
     b56:	7754           	073524
     b58:	6869           	bvcs 0xbc3
     b5a:	6566           	bcss 0xbc2
     b5c:	0066 6f72      	oriw #28530,%fp@-
     b60:	2066           	moveal %fp@-,%a0
     b62:	756c           	072554
     b64:	6c20           	bges 0xb86
     b66:	6761           	beqs 0xbc9
     b68:	6d65           	blts 0xbcf
     b6a:	706c           	moveq #108,%d0
     b6c:	6179           	bsrs 0xbe7
     b6e:	2061           	moveal %a1@-,%a0
     b70:	6374           	blss 0xbe6
     b72:	696f           	bvss 0xbe3
     b74:	6e21           	bgts 0xb97
     b76:	0000 4e56      	orib #86,%d0
     b7a:	fff0           	0177760
     b7c:	2f0a           	movel %a2,%sp@-
     b7e:	41ec 0100      	lea %a4@(256),%a0
     b82:	2008           	movel %a0,%d0
     b84:	2440           	moveal %d0,%a2
     b86:	d4fc 00ff      	addaw #255,%a2
     b8a:	122c 0004      	moveb %a4@(4),%d1
     b8e:	1012           	moveb %a2@,%d0
     b90:	b001           	cmpb %d1,%d0
     b92:	6500 00ec      	bcsw 0xc80
     b96:	6622           	bnes 0xbba
     b98:	4241           	clrw %d1
     b9a:	122a 0001      	moveb %a2@(1),%d1
     b9e:	3001           	movew %d1,%d0
     ba0:	e740           	aslw #3,%d0
     ba2:	d041           	addw %d1,%d0
     ba4:	d041           	addw %d1,%d0
     ba6:	4241           	clrw %d1
     ba8:	122a 0002      	moveb %a2@(2),%d1
     bac:	d041           	addw %d1,%d0
     bae:	4241           	clrw %d1
     bb0:	122c 0005      	moveb %a4@(5),%d1
     bb4:	b041           	cmpw %d1,%d0
     bb6:	6300 00c8      	blsw 0xc80
     bba:	6100 0420      	bsrw 0xfdc
     bbe:	4267           	clrw %sp@-
     bc0:	3f3c 0032      	movew #50,%sp@-
     bc4:	41fa ff10      	lea %pc@(0xad6),%a0
     bc8:	2f08           	movel %a0,%sp@-
     bca:	6100 3a50      	bsrw 0x461c
     bce:	4267           	clrw %sp@-
     bd0:	3f3c 0041      	movew #65,%sp@-
     bd4:	41fa ff09      	lea %pc@(0xadf),%a0
     bd8:	2f08           	movel %a0,%sp@-
     bda:	6100 3a40      	bsrw 0x461c
     bde:	4267           	clrw %sp@-
     be0:	3f3c 004c      	movew #76,%sp@-
     be4:	41fa ff15      	lea %pc@(0xafb),%a0
     be8:	2f08           	movel %a0,%sp@-
     bea:	6100 3a30      	bsrw 0x461c
     bee:	4240           	clrw %d0
     bf0:	102a 0002      	moveb %a2@(2),%d0
     bf4:	3f00           	movew %d0,%sp@-
     bf6:	4240           	clrw %d0
     bf8:	102a 0001      	moveb %a2@(1),%d0
     bfc:	3f00           	movew %d0,%sp@-
     bfe:	4240           	clrw %d0
     c00:	1012           	moveb %a2@,%d0
     c02:	3f00           	movew %d0,%sp@-
     c04:	41fa ff0e      	lea %pc@(0xb14),%a0
     c08:	2f08           	movel %a0,%sp@-
     c0a:	45ee fff0      	lea %fp@(-16),%a2
     c0e:	2f0a           	movel %a2,%sp@-
     c10:	4e4f           	trap #15
     c12:	a2de           	0121336
     c14:	defc 0026      	addaw #38,%sp
     c18:	3f3c 0001      	movew #1,%sp@-
     c1c:	3f3c 0057      	movew #87,%sp@-
     c20:	2f0a           	movel %a2,%sp@-
     c22:	6100 39f8      	bsrw 0x461c
     c26:	4267           	clrw %sp@-
     c28:	3f3c 0062      	movew #98,%sp@-
     c2c:	41fa fef6      	lea %pc@(0xb24),%a0
     c30:	2f08           	movel %a0,%sp@-
     c32:	6100 39e8      	bsrw 0x461c
     c36:	4267           	clrw %sp@-
     c38:	3f3c 0078      	movew #120,%sp@-
     c3c:	41fa fef2      	lea %pc@(0xb30),%a0
     c40:	2f08           	movel %a0,%sp@-
     c42:	6100 39d8      	bsrw 0x461c
     c46:	4267           	clrw %sp@-
     c48:	3f3c 0083      	movew #131,%sp@-
     c4c:	41fa fef8      	lea %pc@(0xb46),%a0
     c50:	2f08           	movel %a0,%sp@-
     c52:	6100 39c8      	bsrw 0x461c
     c56:	defc 0020      	addaw #32,%sp
     c5a:	4267           	clrw %sp@-
     c5c:	3f3c 008e      	movew #142,%sp@-
     c60:	41fa fefb      	lea %pc@(0xb5d),%a0
     c64:	2f08           	movel %a0,%sp@-
     c66:	6100 39b4      	bsrw 0x461c
     c6a:	7200           	moveq #0,%d1
     c6c:	7000           	moveq #0,%d0
     c6e:	5280           	addql #1,%d0
     c70:	0c80 0001 869f 	cmpil #99999,%d0
     c76:	63f6           	blss 0xc6e
     c78:	5281           	addql #1,%d1
     c7a:	7413           	moveq #19,%d2
     c7c:	b481           	cmpl %d1,%d2
     c7e:	64ec           	bccs 0xc6c
     c80:	246e ffec      	moveal %fp@(-20),%a2
     c84:	4e5e           	unlk %fp
     c86:	4e75           	rts
     c88:	0000 4e56      	orib #86,%d0
     c8c:	0000 48e7      	orib #-25,%d0
     c90:	1830 4243      	moveb %a0@(00000043,%d4:w:2),%d4
     c94:	47ec 0230      	lea %a4@(560),%a3
     c98:	200b           	movel %a3,%d0
     c9a:	2440           	moveal %d0,%a2
     c9c:	564a           	addqw #3,%a2
     c9e:	41ec 0230      	lea %a4@(560),%a0
     ca2:	3003           	movew %d3,%d0
     ca4:	c1fc 0102      	mulsw #258,%d0
     ca8:	31bc 00fe 0800 	movew #254,%a0@(00000000,%d0:l)
     cae:	11bc 00fe 0802 	moveb #-2,%a0@(00000002,%d0:l)
     cb4:	47fa ffd2      	lea %pc@(0xc88),%a3
     cb8:	2f0b           	movel %a3,%sp@-
     cba:	4872 0800      	pea %a2@(00000000,%d0:l)
     cbe:	4e4f           	trap #15
     cc0:	a0c5           	0120305
     cc2:	504f           	addqw #8,%sp
     cc4:	5243           	addqw #1,%d3
     cc6:	0c43 0004      	cmpiw #4,%d3
     cca:	6fd2           	bles 0xc9e
     ccc:	4267           	clrw %sp@-
     cce:	6100 03d0      	bsrw 0x10a0
     cd2:	4243           	clrw %d3
     cd4:	544f           	addqw #2,%sp
     cd6:	43ec 0740      	lea %a4@(1856),%a1
     cda:	3003           	movew %d3,%d0
     cdc:	c1fc 000c      	mulsw #12,%d0
     ce0:	47ec 0100      	lea %a4@(256),%a3
     ce4:	280b           	movel %a3,%d4
     ce6:	2040           	moveal %d0,%a0
     ce8:	d1c4           	addal %d4,%a0
     cea:	13a8 0084 0800 	moveb %a0@(132),%a1@(00000000,%d0:l)
     cf0:	13a8 008e 080a 	moveb %a0@(142),%a1@(0000000a,%d0:l)
     cf6:	13a8 0085 0801 	moveb %a0@(133),%a1@(00000001,%d0:l)
     cfc:	13a8 0086 0802 	moveb %a0@(134),%a1@(00000002,%d0:l)
     d02:	13a8 0087 0803 	moveb %a0@(135),%a1@(00000003,%d0:l)
     d08:	13a8 0088 0804 	moveb %a0@(136),%a1@(00000004,%d0:l)
     d0e:	13a8 0089 0805 	moveb %a0@(137),%a1@(00000005,%d0:l)
     d14:	13a8 008a 0806 	moveb %a0@(138),%a1@(00000006,%d0:l)
     d1a:	13a8 008b 0807 	moveb %a0@(139),%a1@(00000007,%d0:l)
     d20:	13a8 008c 0808 	moveb %a0@(140),%a1@(00000008,%d0:l)
     d26:	13a8 008d 0809 	moveb %a0@(141),%a1@(00000009,%d0:l)
     d2c:	13a8 008e 080a 	moveb %a0@(142),%a1@(0000000a,%d0:l)
     d32:	13a8 008f 080b 	moveb %a0@(143),%a1@(0000000b,%d0:l)
     d38:	5243           	addqw #1,%d3
     d3a:	0c43 0009      	cmpiw #9,%d3
     d3e:	6f96           	bles 0xcd6
     d40:	45ec 00d0      	lea %a4@(208),%a2
     d44:	2044           	moveal %d4,%a0
     d46:	d0fc 0108      	addaw #264,%a0
     d4a:	1550 0004      	moveb %a0@,%a2@(4)
     d4e:	34a8 fffc      	movew %a0@(-4),%a2@
     d52:	3568 fffe 0002 	movew %a0@(-2),%a2@(2)
     d58:	1568 0001 0005 	moveb %a0@(1),%a2@(5)
     d5e:	1568 0002 0006 	moveb %a0@(2),%a2@(6)
     d64:	1568 0003 0007 	moveb %a0@(3),%a2@(7)
     d6a:	1568 0004 0008 	moveb %a0@(4),%a2@(8)
     d70:	1568 0005 0009 	moveb %a0@(5),%a2@(9)
     d76:	1568 0006 000a 	moveb %a0@(6),%a2@(10)
     d7c:	3568 000e 0012 	movew %a0@(14),%a2@(18)
     d82:	1568 0008 000c 	moveb %a0@(8),%a2@(12)
     d88:	1568 0007 000b 	moveb %a0@(7),%a2@(11)
     d8e:	1568 0009 000d 	moveb %a0@(9),%a2@(13)
     d94:	1568 000a 000e 	moveb %a0@(10),%a2@(14)
     d9a:	1568 000b 000f 	moveb %a0@(11),%a2@(15)
     da0:	1568 000c 0010 	moveb %a0@(12),%a2@(16)
     da6:	1568 000d 0011 	moveb %a0@(13),%a2@(17)
     dac:	1568 0010 0014 	moveb %a0@(16),%a2@(20)
     db2:	1568 0011 0015 	moveb %a0@(17),%a2@(21)
     db8:	1568 0012 0016 	moveb %a0@(18),%a2@(22)
     dbe:	1568 0013 0017 	moveb %a0@(19),%a2@(23)
     dc4:	1568 0014 0018 	moveb %a0@(20),%a2@(24)
     dca:	1568 0015 0019 	moveb %a0@(21),%a2@(25)
     dd0:	1568 0016 001a 	moveb %a0@(22),%a2@(26)
     dd6:	1568 0017 001b 	moveb %a0@(23),%a2@(27)
     ddc:	1568 0018 001c 	moveb %a0@(24),%a2@(28)
     de2:	1568 0019 001d 	moveb %a0@(25),%a2@(29)
     de8:	1568 001a 001e 	moveb %a0@(26),%a2@(30)
     dee:	1568 001b 001f 	moveb %a0@(27),%a2@(31)
     df4:	1568 001c 0020 	moveb %a0@(28),%a2@(32)
     dfa:	422a 0021      	clrb %a2@(33)
     dfe:	1968 001d 0010 	moveb %a0@(29),%a4@(16)
     e04:	157c 0003 000a 	moveb #3,%a2@(10)
     e0a:	d4fc 0016      	addaw #22,%a2
     e0e:	4240           	clrw %d0
     e10:	1012           	moveb %a2@,%d0
     e12:	3f00           	movew %d0,%sp@-
     e14:	6100 028a      	bsrw 0x10a0
     e18:	1952 0007      	moveb %a2@,%a4@(7)
     e1c:	422c 0008      	clrb %a4@(8)
     e20:	422c 0014      	clrb %a4@(20)
     e24:	422c 0015      	clrb %a4@(21)
     e28:	422c 0013      	clrb %a4@(19)
     e2c:	422c 000f      	clrb %a4@(15)
     e30:	2f04           	movel %d4,%sp@-
     e32:	47fa f945      	lea %pc@(0x779),%a3
     e36:	2f0b           	movel %a3,%sp@-
     e38:	486c 0098      	pea %a4@(152)
     e3c:	4e4f           	trap #15
     e3e:	a2de           	0121336
     e40:	6100 538e      	bsrw 0x61d0
     e44:	4cee 0c18 fff0 	moveml %fp@(-16),%d3-%d4/%a2-%a3
     e4a:	4e5e           	unlk %fp
     e4c:	4e75           	rts
     e4e:	4e56 fff8      	linkw %fp,#-8
     e52:	2f04           	movel %d4,%sp@-
     e54:	2f03           	movel %d3,%sp@-
     e56:	3d7c 00a0 fffe 	movew #160,%fp@(-2)
     e5c:	4280           	clrl %d0
     e5e:	302e fffe      	movew %fp@(-2),%d0
     e62:	0080 00a0 0000 	oril #10485760,%d0
     e68:	2d40 fffc      	movel %d0,%fp@(-4)
     e6c:	426e fffa      	clrw %fp@(-6)
     e70:	4280           	clrl %d0
     e72:	302e fffa      	movew %fp@(-6),%d0
     e76:	2d40 fff8      	movel %d0,%fp@(-8)
     e7a:	2f2c 0084      	movel %a4@(132),%sp@-
     e7e:	4e4f           	trap #15
     e80:	a1fd           	0120775
     e82:	2608           	movel %a0,%d3
     e84:	4267           	clrw %sp@-
     e86:	486e fff8      	pea %fp@(-8)
     e8a:	4e4f           	trap #15
     e8c:	a218           	0121030
     e8e:	2f03           	movel %d3,%sp@-
     e90:	4e4f           	trap #15
     e92:	a1fd           	0120775
     e94:	6100 2638      	bsrw 0x34ce
     e98:	defc 000e      	addaw #14,%sp
     e9c:	363c 270f      	movew #9999,%d3
     ea0:	51cb fffe      	dbf %d3,0xea0
     ea4:	3f3c 0014      	movew #20,%sp@-
     ea8:	3f3c 0025      	movew #37,%sp@-
     eac:	3f3c 03eb      	movew #1003,%sp@-
     eb0:	6100 0196      	bsrw 0x1048
     eb4:	426e fffa      	clrw %fp@(-6)
     eb8:	426e fff8      	clrw %fp@(-8)
     ebc:	3d7c 00a0 fffe 	movew #160,%fp@(-2)
     ec2:	3d7c 00a0 fffc 	movew #160,%fp@(-4)
     ec8:	3f3c 0050      	movew #80,%sp@-
     ecc:	3f3c 0009      	movew #9,%sp@-
     ed0:	3f3c 03ec      	movew #1004,%sp@-
     ed4:	6100 0172      	bsrw 0x1048
     ed8:	3f3c 0078      	movew #120,%sp@-
     edc:	3f3c 0025      	movew #37,%sp@-
     ee0:	3f3c 03ee      	movew #1006,%sp@-
     ee4:	6100 0162      	bsrw 0x1048
     ee8:	4227           	clrb %sp@-
     eea:	4267           	clrw %sp@-
     eec:	4267           	clrw %sp@-
     eee:	486e fff8      	pea %fp@(-8)
     ef2:	2f2c 0080      	movel %a4@(128),%sp@-
     ef6:	2f2c 0084      	movel %a4@(132),%sp@-
     efa:	4e4f           	trap #15
     efc:	a209           	0121011
     efe:	defc 0024      	addaw #36,%sp
     f02:	3f3c 0050      	movew #80,%sp@-
     f06:	3f3c 0009      	movew #9,%sp@-
     f0a:	3f3c 03ed      	movew #1005,%sp@-
     f0e:	6100 0138      	bsrw 0x1048
     f12:	426e fffa      	clrw %fp@(-6)
     f16:	426e fff8      	clrw %fp@(-8)
     f1a:	3d7c 00a0 fffe 	movew #160,%fp@(-2)
     f20:	3d7c 00a0 fffc 	movew #160,%fp@(-4)
     f26:	7802           	moveq #2,%d4
     f28:	763c           	moveq #60,%d3
     f2a:	5c4f           	addqw #6,%sp
     f2c:	3003           	movew %d3,%d0
     f2e:	0640 0014      	addiw #20,%d0
     f32:	3d40 fffa      	movew %d0,%fp@(-6)
     f36:	3d44 fffe      	movew %d4,%fp@(-2)
     f3a:	5444           	addqw #2,%d4
     f3c:	4227           	clrb %sp@-
     f3e:	3f00           	movew %d0,%sp@-
     f40:	4267           	clrw %sp@-
     f42:	486e fff8      	pea %fp@(-8)
     f46:	4e4f           	trap #15
     f48:	a1ff           	0120777
     f4a:	2f08           	movel %a0,%sp@-
     f4c:	2f2c 0080      	movel %a4@(128),%sp@-
     f50:	4e4f           	trap #15
     f52:	a209           	0121011
     f54:	5343           	subqw #1,%d3
     f56:	5244           	addqw #1,%d4
     f58:	defc 0012      	addaw #18,%sp
     f5c:	303c 1387      	movew #4999,%d0
     f60:	51c8 fffe      	dbf %d0,0xf60
     f64:	4227           	clrb %sp@-
     f66:	3203           	movew %d3,%d1
     f68:	0641 0014      	addiw #20,%d1
     f6c:	3f01           	movew %d1,%sp@-
     f6e:	4267           	clrw %sp@-
     f70:	486e fff8      	pea %fp@(-8)
     f74:	4e4f           	trap #15
     f76:	a1ff           	0120777
     f78:	2f08           	movel %a0,%sp@-
     f7a:	2f2c 0084      	movel %a4@(132),%sp@-
     f7e:	4e4f           	trap #15
     f80:	a209           	0121011
     f82:	defc 0012      	addaw #18,%sp
     f86:	303c 1387      	movew #4999,%d0
     f8a:	51c8 fffe      	dbf %d0,0xf8a
     f8e:	5343           	subqw #1,%d3
     f90:	4a43           	tstw %d3
     f92:	6e98           	bgts 0xf2c
     f94:	426e fffa      	clrw %fp@(-6)
     f98:	426e fff8      	clrw %fp@(-8)
     f9c:	3d7c 00a0 fffe 	movew #160,%fp@(-2)
     fa2:	3d7c 00a0 fffc 	movew #160,%fp@(-4)
     fa8:	3f3c 0078      	movew #120,%sp@-
     fac:	3f3c 0025      	movew #37,%sp@-
     fb0:	3f3c 03ee      	movew #1006,%sp@-
     fb4:	6100 0092      	bsrw 0x1048
     fb8:	4227           	clrb %sp@-
     fba:	4267           	clrw %sp@-
     fbc:	4267           	clrw %sp@-
     fbe:	486e fff8      	pea %fp@(-8)
     fc2:	4e4f           	trap #15
     fc4:	a1ff           	0120777
     fc6:	2f08           	movel %a0,%sp@-
     fc8:	2f2c 0080      	movel %a4@(128),%sp@-
     fcc:	4e4f           	trap #15
     fce:	a209           	0121011
     fd0:	262e fff0      	movel %fp@(-16),%d3
     fd4:	282e fff4      	movel %fp@(-12),%d4
     fd8:	4e5e           	unlk %fp
     fda:	4e75           	rts
     fdc:	4e56 fff8      	linkw %fp,#-8
     fe0:	3d7c 00a0 fffe 	movew #160,%fp@(-2)
     fe6:	4280           	clrl %d0
     fe8:	302e fffe      	movew %fp@(-2),%d0
     fec:	0080 00a0 0000 	oril #10485760,%d0
     ff2:	2d40 fffc      	movel %d0,%fp@(-4)
     ff6:	426e fffa      	clrw %fp@(-6)
     ffa:	4280           	clrl %d0
     ffc:	302e fffa      	movew %fp@(-6),%d0
    1000:	2d40 fff8      	movel %d0,%fp@(-8)
    1004:	4267           	clrw %sp@-
    1006:	486e fff8      	pea %fp@(-8)
    100a:	4e4f           	trap #15
    100c:	a218           	0121030
    100e:	4e5e           	unlk %fp
    1010:	4e75           	rts
    1012:	4e56 fff8      	linkw %fp,#-8
    1016:	3d7c 00a0 fffe 	movew #160,%fp@(-2)
    101c:	4280           	clrl %d0
    101e:	302e fffe      	movew %fp@(-2),%d0
    1022:	0080 00a0 0000 	oril #10485760,%d0
    1028:	2d40 fffc      	movel %d0,%fp@(-4)
    102c:	426e fffa      	clrw %fp@(-6)
    1030:	4280           	clrl %d0
    1032:	302e fffa      	movew %fp@(-6),%d0
    1036:	2d40 fff8      	movel %d0,%fp@(-8)
    103a:	4267           	clrw %sp@-
    103c:	486e fff8      	pea %fp@(-8)
    1040:	4e4f           	trap #15
    1042:	a219           	0121031
    1044:	4e5e           	unlk %fp
    1046:	4e75           	rts
    1048:	4e56 0000      	linkw %fp,#0
    104c:	48e7 1e00      	moveml %d3-%d6,%sp@-
    1050:	362e 0008      	movew %fp@(8),%d3
    1054:	3a2e 000a      	movew %fp@(10),%d5
    1058:	382e 000c      	movew %fp@(12),%d4
    105c:	2f2c 0084      	movel %a4@(132),%sp@-
    1060:	4e4f           	trap #15
    1062:	a1fd           	0120775
    1064:	2c08           	movel %a0,%d6
    1066:	3f03           	movew %d3,%sp@-
    1068:	2f3c 5462 6d70 	movel #1415736688,%sp@-
    106e:	4e4f           	trap #15
    1070:	a05f           	0120137
    1072:	2608           	movel %a0,%d3
    1074:	2f03           	movel %d3,%sp@-
    1076:	4e4f           	trap #15
    1078:	a021           	0120041
    107a:	3f04           	movew %d4,%sp@-
    107c:	3f05           	movew %d5,%sp@-
    107e:	2f08           	movel %a0,%sp@-
    1080:	4e4f           	trap #15
    1082:	a226           	0121046
    1084:	2f03           	movel %d3,%sp@-
    1086:	4e4f           	trap #15
    1088:	a022           	0120042
    108a:	2f03           	movel %d3,%sp@-
    108c:	4e4f           	trap #15
    108e:	a061           	0120141
    1090:	2f06           	movel %d6,%sp@-
    1092:	4e4f           	trap #15
    1094:	a1fd           	0120775
    1096:	4cee 0078 fff0 	moveml %fp@(-16),%d3-%d6
    109c:	4e5e           	unlk %fp
    109e:	4e75           	rts
    10a0:	4e56 0000      	linkw %fp,#0
    10a4:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    10a8:	382e 0008      	movew %fp@(8),%d4
    10ac:	3f04           	movew %d4,%sp@-
    10ae:	2f2c 073c      	movel %a4@(1852),%sp@-
    10b2:	4e4f           	trap #15
    10b4:	a05c           	0120134
    10b6:	2608           	movel %a0,%d3
    10b8:	5c4f           	addqw #6,%sp
    10ba:	660a           	bnes 0x10c6
    10bc:	3f3c 03e9      	movew #1001,%sp@-
    10c0:	4e4f           	trap #15
    10c2:	a192           	0120622
    10c4:	544f           	addqw #2,%sp
    10c6:	2f03           	movel %d3,%sp@-
    10c8:	4e4f           	trap #15
    10ca:	a021           	0120041
    10cc:	2648           	moveal %a0,%a3
    10ce:	4243           	clrw %d3
    10d0:	584f           	addqw #4,%sp
    10d2:	41ec 0100      	lea %a4@(256),%a0
    10d6:	11b3 3000 3000 	moveb %a3@(00000000,%d3:w),%a0@(00000000,%d3:w)
    10dc:	5243           	addqw #1,%d3
    10de:	0c43 001d      	cmpiw #29,%d3
    10e2:	6fee           	bles 0x10d2
    10e4:	4243           	clrw %d3
    10e6:	45e8 001e      	lea %a0@(30),%a2
    10ea:	4242           	clrw %d2
    10ec:	3203           	movew %d3,%d1
    10ee:	48c1           	extl %d1
    10f0:	2001           	movel %d1,%d0
    10f2:	e780           	asll #3,%d0
    10f4:	d081           	addl %d1,%d0
    10f6:	d081           	addl %d1,%d0
    10f8:	43f2 0800      	lea %a2@(00000000,%d0:l),%a1
    10fc:	41f3 081e      	lea %a3@(0000001e,%d0:l),%a0
    1100:	13b0 2000 2000 	moveb %a0@(00000000,%d2:w),%a1@(00000000,%d2:w)
    1106:	5242           	addqw #1,%d2
    1108:	0c42 0009      	cmpiw #9,%d2
    110c:	6ff2           	bles 0x1100
    110e:	5243           	addqw #1,%d3
    1110:	0c43 0009      	cmpiw #9,%d3
    1114:	6fd4           	bles 0x10ea
    1116:	41ec 0100      	lea %a4@(256),%a0
    111a:	116b 0082 0082 	moveb %a3@(130),%a0@(130)
    1120:	116b 0083 0083 	moveb %a3@(131),%a0@(131)
    1126:	4243           	clrw %d3
    1128:	45ec 0100      	lea %a4@(256),%a2
    112c:	3003           	movew %d3,%d0
    112e:	c1fc 000c      	mulsw #12,%d0
    1132:	43f2 0800      	lea %a2@(00000000,%d0:l),%a1
    1136:	41f3 0800      	lea %a3@(00000000,%d0:l),%a0
    113a:	1368 0084 0084 	moveb %a0@(132),%a1@(132)
    1140:	1368 0085 0085 	moveb %a0@(133),%a1@(133)
    1146:	1368 0086 0086 	moveb %a0@(134),%a1@(134)
    114c:	1368 0087 0087 	moveb %a0@(135),%a1@(135)
    1152:	1368 0088 0088 	moveb %a0@(136),%a1@(136)
    1158:	1368 0089 0089 	moveb %a0@(137),%a1@(137)
    115e:	1368 008a 008a 	moveb %a0@(138),%a1@(138)
    1164:	1368 008b 008b 	moveb %a0@(139),%a1@(139)
    116a:	1368 008d 008d 	moveb %a0@(141),%a1@(141)
    1170:	1368 008c 008c 	moveb %a0@(140),%a1@(140)
    1176:	1368 008e 008e 	moveb %a0@(142),%a1@(142)
    117c:	1368 008f 008f 	moveb %a0@(143),%a1@(143)
    1182:	5243           	addqw #1,%d3
    1184:	0c43 0009      	cmpiw #9,%d3
    1188:	6f9e           	bles 0x1128
    118a:	156b 00fc 00fc 	moveb %a3@(252),%a2@(252)
    1190:	156b 00fd 00fd 	moveb %a3@(253),%a2@(253)
    1196:	156b 00fe 00fe 	moveb %a3@(254),%a2@(254)
    119c:	156b 00ff 00ff 	moveb %a3@(255),%a2@(255)
    11a2:	156b 0100 0100 	moveb %a3@(256),%a2@(256)
    11a8:	156b 0101 0101 	moveb %a3@(257),%a2@(257)
    11ae:	156b 0102 0102 	moveb %a3@(258),%a2@(258)
    11b4:	156b 0103 0103 	moveb %a3@(259),%a2@(259)
    11ba:	156b 0108 0108 	moveb %a3@(264),%a2@(264)
    11c0:	356b 0104 0104 	movew %a3@(260),%a2@(260)
    11c6:	356b 0106 0106 	movew %a3@(262),%a2@(262)
    11cc:	156b 0109 0109 	moveb %a3@(265),%a2@(265)
    11d2:	156b 010a 010a 	moveb %a3@(266),%a2@(266)
    11d8:	156b 010b 010b 	moveb %a3@(267),%a2@(267)
    11de:	156b 010c 010c 	moveb %a3@(268),%a2@(268)
    11e4:	156b 010d 010d 	moveb %a3@(269),%a2@(269)
    11ea:	156b 010e 010e 	moveb %a3@(270),%a2@(270)
    11f0:	156b 010f 010f 	moveb %a3@(271),%a2@(271)
    11f6:	156b 0110 0110 	moveb %a3@(272),%a2@(272)
    11fc:	156b 0111 0111 	moveb %a3@(273),%a2@(273)
    1202:	156b 0112 0112 	moveb %a3@(274),%a2@(274)
    1208:	156b 0113 0113 	moveb %a3@(275),%a2@(275)
    120e:	156b 0114 0114 	moveb %a3@(276),%a2@(276)
    1214:	156b 0115 0115 	moveb %a3@(277),%a2@(277)
    121a:	356b 0116 0116 	movew %a3@(278),%a2@(278)
    1220:	156b 0118 0118 	moveb %a3@(280),%a2@(280)
    1226:	156b 0119 0119 	moveb %a3@(281),%a2@(281)
    122c:	156b 011a 011a 	moveb %a3@(282),%a2@(282)
    1232:	156b 011b 011b 	moveb %a3@(283),%a2@(283)
    1238:	156b 011c 011c 	moveb %a3@(284),%a2@(284)
    123e:	156b 011d 011d 	moveb %a3@(285),%a2@(285)
    1244:	156b 011e 011e 	moveb %a3@(286),%a2@(286)
    124a:	156b 011f 011f 	moveb %a3@(287),%a2@(287)
    1250:	156b 0120 0120 	moveb %a3@(288),%a2@(288)
    1256:	156b 0121 0121 	moveb %a3@(289),%a2@(289)
    125c:	156b 0122 0122 	moveb %a3@(290),%a2@(290)
    1262:	156b 0123 0123 	moveb %a3@(291),%a2@(291)
    1268:	156b 0124 0124 	moveb %a3@(292),%a2@(292)
    126e:	156b 0125 0125 	moveb %a3@(293),%a2@(293)
    1274:	2f0b           	movel %a3,%sp@-
    1276:	4e4f           	trap #15
    1278:	a035           	0120065
    127a:	1f3c 0001      	moveb #1,%sp@-
    127e:	3f04           	movew %d4,%sp@-
    1280:	2f2c 073c      	movel %a4@(1852),%sp@-
    1284:	4e4f           	trap #15
    1286:	a05e           	0120136
    1288:	defc 000c      	addaw #12,%sp
    128c:	4aac 00c0      	tstl %a4@(192)
    1290:	6712           	beqs 0x12a4
    1292:	102a 0108      	moveb %a2@(264),%d0
    1296:	670c           	beqs 0x12a4
    1298:	4267           	clrw %sp@-
    129a:	1f40 0001      	moveb %d0,%sp@(1)
    129e:	6100 4712      	bsrw 0x59b2
    12a2:	544f           	addqw #2,%sp
    12a4:	41ec 0100      	lea %a4@(256),%a0
    12a8:	1028 00fc      	moveb %a0@(252),%d0
    12ac:	b02c 0012      	cmpb %a4@(18),%d0
    12b0:	670c           	beqs 0x12be
    12b2:	4267           	clrw %sp@-
    12b4:	1f40 0001      	moveb %d0,%sp@(1)
    12b8:	6100 05c2      	bsrw 0x187c
    12bc:	544f           	addqw #2,%sp
    12be:	6100 2096      	bsrw 0x3356
    12c2:	4cee 0c18 fff0 	moveml %fp@(-16),%d3-%d4/%a2-%a3
    12c8:	4e5e           	unlk %fp
    12ca:	4e75           	rts
    12cc:	4e56 0000      	linkw %fp,#0
    12d0:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    12d4:	382e 0008      	movew %fp@(8),%d4
    12d8:	3f04           	movew %d4,%sp@-
    12da:	2f2c 00f4      	movel %a4@(244),%sp@-
    12de:	4e4f           	trap #15
    12e0:	a05c           	0120134
    12e2:	2608           	movel %a0,%d3
    12e4:	5c4f           	addqw #6,%sp
    12e6:	660a           	bnes 0x12f2
    12e8:	3f3c 03e9      	movew #1001,%sp@-
    12ec:	4e4f           	trap #15
    12ee:	a192           	0120622
    12f0:	544f           	addqw #2,%sp
    12f2:	2f03           	movel %d3,%sp@-
    12f4:	4e4f           	trap #15
    12f6:	a021           	0120041
    12f8:	2648           	moveal %a0,%a3
    12fa:	4243           	clrw %d3
    12fc:	584f           	addqw #4,%sp
    12fe:	41ec 0100      	lea %a4@(256),%a0
    1302:	11b3 3000 3000 	moveb %a3@(00000000,%d3:w),%a0@(00000000,%d3:w)
    1308:	5243           	addqw #1,%d3
    130a:	0c43 001d      	cmpiw #29,%d3
    130e:	6fee           	bles 0x12fe
    1310:	4243           	clrw %d3
    1312:	45e8 001e      	lea %a0@(30),%a2
    1316:	4242           	clrw %d2
    1318:	3203           	movew %d3,%d1
    131a:	48c1           	extl %d1
    131c:	2001           	movel %d1,%d0
    131e:	e780           	asll #3,%d0
    1320:	d081           	addl %d1,%d0
    1322:	d081           	addl %d1,%d0
    1324:	43f2 0800      	lea %a2@(00000000,%d0:l),%a1
    1328:	41f3 081e      	lea %a3@(0000001e,%d0:l),%a0
    132c:	13b0 2000 2000 	moveb %a0@(00000000,%d2:w),%a1@(00000000,%d2:w)
    1332:	5242           	addqw #1,%d2
    1334:	0c42 0009      	cmpiw #9,%d2
    1338:	6ff2           	bles 0x132c
    133a:	5243           	addqw #1,%d3
    133c:	0c43 0009      	cmpiw #9,%d3
    1340:	6fd4           	bles 0x1316
    1342:	41ec 0100      	lea %a4@(256),%a0
    1346:	116b 0082 0082 	moveb %a3@(130),%a0@(130)
    134c:	116b 0083 0083 	moveb %a3@(131),%a0@(131)
    1352:	4243           	clrw %d3
    1354:	45ec 0100      	lea %a4@(256),%a2
    1358:	3003           	movew %d3,%d0
    135a:	c1fc 000c      	mulsw #12,%d0
    135e:	43f2 0800      	lea %a2@(00000000,%d0:l),%a1
    1362:	41f3 0800      	lea %a3@(00000000,%d0:l),%a0
    1366:	1368 0084 0084 	moveb %a0@(132),%a1@(132)
    136c:	1368 0085 0085 	moveb %a0@(133),%a1@(133)
    1372:	1368 0086 0086 	moveb %a0@(134),%a1@(134)
    1378:	1368 0087 0087 	moveb %a0@(135),%a1@(135)
    137e:	1368 0088 0088 	moveb %a0@(136),%a1@(136)
    1384:	1368 0089 0089 	moveb %a0@(137),%a1@(137)
    138a:	1368 008a 008a 	moveb %a0@(138),%a1@(138)
    1390:	1368 008b 008b 	moveb %a0@(139),%a1@(139)
    1396:	1368 008d 008d 	moveb %a0@(141),%a1@(141)
    139c:	1368 008c 008c 	moveb %a0@(140),%a1@(140)
    13a2:	1368 008e 008e 	moveb %a0@(142),%a1@(142)
    13a8:	1368 008f 008f 	moveb %a0@(143),%a1@(143)
    13ae:	5243           	addqw #1,%d3
    13b0:	0c43 0009      	cmpiw #9,%d3
    13b4:	6f9e           	bles 0x1354
    13b6:	156b 00fc 00fc 	moveb %a3@(252),%a2@(252)
    13bc:	156b 00fd 00fd 	moveb %a3@(253),%a2@(253)
    13c2:	156b 00fe 00fe 	moveb %a3@(254),%a2@(254)
    13c8:	156b 00ff 00ff 	moveb %a3@(255),%a2@(255)
    13ce:	156b 0100 0100 	moveb %a3@(256),%a2@(256)
    13d4:	156b 0101 0101 	moveb %a3@(257),%a2@(257)
    13da:	156b 0102 0102 	moveb %a3@(258),%a2@(258)
    13e0:	156b 0103 0103 	moveb %a3@(259),%a2@(259)
    13e6:	156b 0108 0108 	moveb %a3@(264),%a2@(264)
    13ec:	356b 0104 0104 	movew %a3@(260),%a2@(260)
    13f2:	356b 0106 0106 	movew %a3@(262),%a2@(262)
    13f8:	156b 0109 0109 	moveb %a3@(265),%a2@(265)
    13fe:	156b 010a 010a 	moveb %a3@(266),%a2@(266)
    1404:	156b 010b 010b 	moveb %a3@(267),%a2@(267)
    140a:	156b 010c 010c 	moveb %a3@(268),%a2@(268)
    1410:	156b 010d 010d 	moveb %a3@(269),%a2@(269)
    1416:	156b 010e 010e 	moveb %a3@(270),%a2@(270)
    141c:	156b 010f 010f 	moveb %a3@(271),%a2@(271)
    1422:	156b 0110 0110 	moveb %a3@(272),%a2@(272)
    1428:	156b 0111 0111 	moveb %a3@(273),%a2@(273)
    142e:	156b 0112 0112 	moveb %a3@(274),%a2@(274)
    1434:	156b 0113 0113 	moveb %a3@(275),%a2@(275)
    143a:	156b 0114 0114 	moveb %a3@(276),%a2@(276)
    1440:	156b 0115 0115 	moveb %a3@(277),%a2@(277)
    1446:	356b 0116 0116 	movew %a3@(278),%a2@(278)
    144c:	156b 0118 0118 	moveb %a3@(280),%a2@(280)
    1452:	156b 0119 0119 	moveb %a3@(281),%a2@(281)
    1458:	156b 011a 011a 	moveb %a3@(282),%a2@(282)
    145e:	156b 011b 011b 	moveb %a3@(283),%a2@(283)
    1464:	156b 011c 011c 	moveb %a3@(284),%a2@(284)
    146a:	156b 011d 011d 	moveb %a3@(285),%a2@(285)
    1470:	156b 011e 011e 	moveb %a3@(286),%a2@(286)
    1476:	156b 011f 011f 	moveb %a3@(287),%a2@(287)
    147c:	156b 0120 0120 	moveb %a3@(288),%a2@(288)
    1482:	156b 0121 0121 	moveb %a3@(289),%a2@(289)
    1488:	156b 0122 0122 	moveb %a3@(290),%a2@(290)
    148e:	156b 0123 0123 	moveb %a3@(291),%a2@(291)
    1494:	156b 0124 0124 	moveb %a3@(292),%a2@(292)
    149a:	156b 0125 0125 	moveb %a3@(293),%a2@(293)
    14a0:	2f0b           	movel %a3,%sp@-
    14a2:	4e4f           	trap #15
    14a4:	a035           	0120065
    14a6:	1f3c 0001      	moveb #1,%sp@-
    14aa:	3f04           	movew %d4,%sp@-
    14ac:	2f2c 00f4      	movel %a4@(244),%sp@-
    14b0:	4e4f           	trap #15
    14b2:	a05e           	0120136
    14b4:	6100 1ea0      	bsrw 0x3356
    14b8:	4cee 0c18 fff0 	moveml %fp@(-16),%d3-%d4/%a2-%a3
    14be:	4e5e           	unlk %fp
    14c0:	4e75           	rts
    14c2:	4e56 0000      	linkw %fp,#0
    14c6:	2f0a           	movel %a2,%sp@-
    14c8:	2f03           	movel %d3,%sp@-
    14ca:	4878 0128      	pea 0x128
    14ce:	486e 0008      	pea %fp@(8)
    14d2:	45ec 073c      	lea %a4@(1852),%a2
    14d6:	2f12           	movel %a2@,%sp@-
    14d8:	4e4f           	trap #15
    14da:	a055           	0120125
    14dc:	2008           	movel %a0,%d0
    14de:	defc 000c      	addaw #12,%sp
    14e2:	672c           	beqs 0x1510
    14e4:	2f00           	movel %d0,%sp@-
    14e6:	4e4f           	trap #15
    14e8:	a021           	0120041
    14ea:	2608           	movel %a0,%d3
    14ec:	4878 0128      	pea 0x128
    14f0:	486c 0100      	pea %a4@(256)
    14f4:	42a7           	clrl %sp@-
    14f6:	2f03           	movel %d3,%sp@-
    14f8:	4e4f           	trap #15
    14fa:	a076           	0120166
    14fc:	2f03           	movel %d3,%sp@-
    14fe:	4e4f           	trap #15
    1500:	a035           	0120065
    1502:	1f3c 0001      	moveb #1,%sp@-
    1506:	3f2e 0008      	movew %fp@(8),%sp@-
    150a:	2f12           	movel %a2@,%sp@-
    150c:	4e4f           	trap #15
    150e:	a05e           	0120136
    1510:	262e fff8      	movel %fp@(-8),%d3
    1514:	246e fffc      	moveal %fp@(-4),%a2
    1518:	4e5e           	unlk %fp
    151a:	4e75           	rts
    151c:	4e56 0000      	linkw %fp,#0
    1520:	48e7 1820      	moveml %d3-%d4/%a2,%sp@-
    1524:	382e 0008      	movew %fp@(8),%d4
    1528:	3f04           	movew %d4,%sp@-
    152a:	45ec 073c      	lea %a4@(1852),%a2
    152e:	2f12           	movel %a2@,%sp@-
    1530:	4e4f           	trap #15
    1532:	a05c           	0120134
    1534:	2f08           	movel %a0,%sp@-
    1536:	4e4f           	trap #15
    1538:	a021           	0120041
    153a:	2608           	movel %a0,%d3
    153c:	4878 0128      	pea 0x128
    1540:	486c 0100      	pea %a4@(256)
    1544:	42a7           	clrl %sp@-
    1546:	2f03           	movel %d3,%sp@-
    1548:	4e4f           	trap #15
    154a:	a076           	0120166
    154c:	2f03           	movel %d3,%sp@-
    154e:	4e4f           	trap #15
    1550:	a035           	0120065
    1552:	1f3c 0001      	moveb #1,%sp@-
    1556:	3f04           	movew %d4,%sp@-
    1558:	2f12           	movel %a2@,%sp@-
    155a:	4e4f           	trap #15
    155c:	a05e           	0120136
    155e:	4240           	clrw %d0
    1560:	4cee 0418 fff4 	moveml %fp@(-12),%d3-%d4/%a2
    1566:	4e5e           	unlk %fp
    1568:	4e75           	rts
    156a:	536f 6d65      	subqw #1,%sp@(28005)
    156e:	7468           	moveq #104,%d2
    1570:	696e           	bvss 0x15e0
    1572:	6720           	beqs 0x1594
    1574:	7374           	071564
    1576:	7261           	moveq #97,%d1
    1578:	6e67           	bgts 0x15e1
    157a:	6520           	bcss 0x159c
    157c:	6a75           	bpls 0x15f3
    157e:	7374           	071564
    1580:	2068 6170      	moveal %a0@(24944),%a0
    1584:	7065           	moveq #101,%d0
    1586:	6e65           	bgts 0x15ed
    1588:	642e           	bccs 0x15b8
    158a:	2e2e 0059      	movel %fp@(89),%d7
    158e:	6f75           	bles 0x1605
    1590:	206b 6e6f      	moveal %a3@(28271),%a0
    1594:	772c           	073454
    1596:	2024           	movel %a4@-,%d0
    1598:	3135 2069      	movew %a5@(00000069,%d2:w),%a0@-
    159c:	736e           	071556
    159e:	2774 206d 7563 	movel %a4@(0000006d,%d2:w),%a3@(30051)
    15a4:	6820           	bvcs 0x15c6
    15a6:	746f           	moveq #111,%d2
    15a8:	2070 6179 2066 	moveal %a0@(20666f72)@(00000000),%a0
    15ae:	6f72
    15b0:	2073 6f6d 6520 	moveal %a3@(00006520)@(00000000),%a0
    15b6:	7265           	moveq #101,%d1
    15b8:	616c           	bsrs 0x1626
    15ba:	2071 7561 6c69 	moveal %a1@(00006c69)@(00000000),%a0
    15c0:	7479           	moveq #121,%d2
    15c2:	2065           	moveal %a5@-,%a0
    15c4:	6e74           	bgts 0x163a
    15c6:	6572           	bcss 0x163a
    15c8:	7461           	moveq #97,%d2
    15ca:	696e           	bvss 0x163a
    15cc:	6d65           	blts 0x1633
    15ce:	6e74           	bgts 0x1644
    15d0:	2e00           	movel %d0,%d7
    15d2:	596f 7520      	subqw #4,%sp@(29984)
    15d6:	6b6e           	bmis 0x1646
    15d8:	6f77           	bles 0x1651
    15da:	2c20           	movel %a0@-,%d6
    15dc:	6966           	bvss 0x1644
    15de:	2079 6f75 2764 	moveal 0x6f752764,%a0
    15e4:	206a 7573      	moveal %a2@(30067),%a0
    15e8:	7420           	moveq #32,%d2
    15ea:	7265           	moveq #101,%d1
    15ec:	6769           	beqs 0x1657
    15ee:	7374           	071564
    15f0:	6572           	bcss 0x1664
    15f2:	2049           	moveal %a1,%a0
    15f4:	2063           	moveal %a3@-,%a0
    15f6:	6f75           	bles 0x166d
    15f8:	6c64           	bges 0x165e
    15fa:	2073 746f      	moveal %a3@(0000006f,%d7:w:4),%a0
    15fe:	7020           	moveq #32,%d0
    1600:	626f           	bhis 0x1671
    1602:	7468           	moveq #104,%d2
    1604:	6572           	bcss 0x1678
    1606:	696e           	bvss 0x1676
    1608:	6720           	beqs 0x162a
    160a:	796f           	074557
    160c:	7520           	072440
    160e:	6162           	bsrs 0x1672
    1610:	6f75           	bles 0x1687
    1612:	7420           	moveq #32,%d2
    1614:	6974           	bvss 0x168a
    1616:	2e00           	movel %d0,%d7
    1618:	5468 6572      	addqw #2,%a0@(25970)
    161c:	6520           	bcss 0x163e
    161e:	6973           	bvss 0x1693
    1620:	2068 6f6e      	moveal %a0@(28526),%a0
    1624:	6f75           	bles 0x169b
    1626:	7220           	moveq #32,%d1
    1628:	616d           	bsrs 0x1697
    162a:	6f6e           	bles 0x169a
    162c:	6720           	beqs 0x164e
    162e:	7468           	moveq #104,%d2
    1630:	6965           	bvss 0x1697
    1632:	7665           	moveq #101,%d3
    1634:	732c           	071454
    1636:	2079 6f75 206b 	moveal 0x6f75206b,%a0
    163c:	6e6f           	bgts 0x16ad
    163e:	772e           	073456
    1640:	2049           	moveal %a1,%a0
    1642:	7427           	moveq #39,%d2
    1644:	7320           	071440
    1646:	6361           	blss 0x16a9
    1648:	6c6c           	bges 0x16b6
    164a:	6564           	bcss 0x16b0
    164c:	2072 6567 6973 	moveal %a2@(00006973)@(74657221),%a0
    1652:	7465 7221
    1656:	0050 7373      	oriw #29555,%a0@
    165a:	7421           	moveq #33,%d2
    165c:	2059           	moveal %a1@+,%a0
    165e:	6f75           	bles 0x16d5
    1660:	2063           	moveal %a3@-,%a0
    1662:	616e           	bsrs 0x16d2
    1664:	2072 6567 6973 	moveal %a2@(00006973)@(74657220),%a0
    166a:	7465 7220
    166e:	7468           	moveq #104,%d2
    1670:	6973           	bvss 0x16e5
    1672:	206c 6974      	moveal %a4@(26996),%a0
    1676:	746c           	moveq #108,%d2
    1678:	6520           	bcss 0x169a
    167a:	6765           	beqs 0x16e1
    167c:	6d20           	blts 0x169e
    167e:	6f6e           	bles 0x16ee
    1680:	6c69           	bges 0x16eb
    1682:	6e65           	bgts 0x16e9
    1684:	2e00           	movel %d0,%d7
    1686:	4279 2074 6865 	clrw 0x20746865
    168c:	2062           	moveal %a2@-,%a0
    168e:	792e           	074456
    1690:	2e2e 2059      	movel %fp@(8281),%d7
    1694:	6f75           	bles 0x170b
    1696:	2063           	moveal %a3@-,%a0
    1698:	616e           	bsrs 0x1708
    169a:	2072 6567 6973 	moveal %a2@(00006973)@(74657220),%a0
    16a0:	7465 7220
    16a4:	7468           	moveq #104,%d2
    16a6:	6973           	bvss 0x171b
    16a8:	2079 6172 6e20 	moveal 0x61726e20,%a0
    16ae:	7769           	073551
    16b0:	7468           	moveq #104,%d2
    16b2:	204c           	moveal %a4,%a0
    16b4:	6164           	bsrs 0x171a
    16b6:	7920           	074440
    16b8:	5669 7361      	addqw #3,%a1@(29537)
    16bc:	206f 7220      	moveal %sp@(29216),%a0
    16c0:	4c6f           	046157
    16c2:	7264           	moveq #100,%d1
    16c4:	204d           	moveal %a5,%a0
    16c6:	6173           	bsrs 0x173b
    16c8:	7465           	moveq #101,%d2
    16ca:	7243           	moveq #67,%d1
    16cc:	6172           	bsrs 0x1740
    16ce:	642e           	bccs 0x16fe
    16d0:	0000 4e56      	orib #86,%d0
    16d4:	0000 48e7      	orib #-25,%d0
    16d8:	1820           	moveb %a0@-,%d4
    16da:	362e 0008      	movew %fp@(8),%d3
    16de:	4244           	clrw %d4
    16e0:	41ec 0100      	lea %a4@(256),%a0
    16e4:	43e8 0116      	lea %a0@(278),%a1
    16e8:	0c51 00fe      	cmpiw #254,%a1@
    16ec:	6608           	bnes 0x16f6
    16ee:	4251           	clrw %a1@
    16f0:	117c 0014 011a 	moveb #20,%a0@(282)
    16f6:	4240           	clrw %d0
    16f8:	102c 0007      	moveb %a4@(7),%d0
    16fc:	3f00           	movew %d0,%sp@-
    16fe:	6100 fe1c      	bsrw 0x151c
    1702:	544f           	addqw #2,%sp
    1704:	4a43           	tstw %d3
    1706:	660a           	bnes 0x1712
    1708:	4a2c 0016      	tstb %a4@(22)
    170c:	6604           	bnes 0x1712
    170e:	7801           	moveq #1,%d4
    1710:	7601           	moveq #1,%d3
    1712:	3f03           	movew %d3,%sp@-
    1714:	6100 f98a      	bsrw 0x10a0
    1718:	1943 0007      	moveb %d3,%a4@(7)
    171c:	422c 000f      	clrb %a4@(15)
    1720:	422c 0013      	clrb %a4@(19)
    1724:	422c 0015      	clrb %a4@(21)
    1728:	422c 0014      	clrb %a4@(20)
    172c:	41ec 0100      	lea %a4@(256),%a0
    1730:	43e8 0108      	lea %a0@(264),%a1
    1734:	544f           	addqw #2,%sp
    1736:	4a11           	tstb %a1@
    1738:	670c           	beqs 0x1746
    173a:	4a29 0008      	tstb %a1@(8)
    173e:	6606           	bnes 0x1746
    1740:	117c 000c 0109 	moveb #12,%a0@(265)
    1746:	4241           	clrw %d1
    1748:	41ec 0230      	lea %a4@(560),%a0
    174c:	3001           	movew %d1,%d0
    174e:	c1fc 0102      	mulsw #258,%d0
    1752:	31bc 00fe 0800 	movew #254,%a0@(00000000,%d0:l)
    1758:	5241           	addqw #1,%d1
    175a:	0c41 0004      	cmpiw #4,%d1
    175e:	6fe8           	bles 0x1748
    1760:	422c 0008      	clrb %a4@(8)
    1764:	4a44           	tstw %d4
    1766:	6722           	beqs 0x178a
    1768:	45fa fe00      	lea %pc@(0x156a),%a2
    176c:	2f0a           	movel %a2,%sp@-
    176e:	3f3c 0001      	movew #1,%sp@-
    1772:	3f3c 029c      	movew #668,%sp@-
    1776:	6100 2f14      	bsrw 0x468c
    177a:	41ec 00d0      	lea %a4@(208),%a0
    177e:	30bc 0040      	movew #64,%a0@
    1782:	317c 0079 0002 	movew #121,%a0@(2)
    1788:	504f           	addqw #8,%sp
    178a:	4a2c 000e      	tstb %a4@(14)
    178e:	6600 0082      	bnew 0x1812
    1792:	41ec 0024      	lea %a4@(36),%a0
    1796:	1010           	moveb %a0@,%d0
    1798:	1400           	moveb %d0,%d2
    179a:	5202           	addqb #1,%d2
    179c:	1082           	moveb %d2,%a0@
    179e:	5200           	addqb #1,%d0
    17a0:	0c00 0013      	cmpib #19,%d0
    17a4:	636c           	blss 0x1812
    17a6:	42a7           	clrl %sp@-
    17a8:	4e4f           	trap #15
    17aa:	a0c2           	0120302
    17ac:	7405           	moveq #5,%d2
    17ae:	48c0           	extl %d0
    17b0:	81c2           	divsw %d2,%d0
    17b2:	2200           	movel %d0,%d1
    17b4:	4841           	swap %d1
    17b6:	584f           	addqw #4,%sp
    17b8:	0c41 0004      	cmpiw #4,%d1
    17bc:	6250           	bhis 0x180e
    17be:	4280           	clrl %d0
    17c0:	3001           	movew %d1,%d0
    17c2:	41fa 000c      	lea %pc@(0x17d0),%a0
    17c6:	d080           	addl %d0,%d0
    17c8:	3030 0800      	movew %a0@(00000000,%d0:l),%d0
    17cc:	4efb 0002      	jmp %pc@(0x17d0,%d0:w)
    17d0:	000a 0012      	orib #18,%a2
    17d4:	001a 0022      	orib #34,%a2@+
    17d8:	002a 45fa fdb1 	orib #-6,%a2@(-591)
    17de:	6000 001e      	braw 0x17fe
    17e2:	45fa fdee      	lea %pc@(0x15d2),%a2
    17e6:	6000 0016      	braw 0x17fe
    17ea:	45fa fe2c      	lea %pc@(0x1618),%a2
    17ee:	6000 000e      	braw 0x17fe
    17f2:	45fa fe63      	lea %pc@(0x1657),%a2
    17f6:	6000 0006      	braw 0x17fe
    17fa:	45fa fe8a      	lea %pc@(0x1686),%a2
    17fe:	2f0a           	movel %a2,%sp@-
    1800:	3f3c 0001      	movew #1,%sp@-
    1804:	3f3c 029c      	movew #668,%sp@-
    1808:	6100 2e82      	bsrw 0x468c
    180c:	504f           	addqw #8,%sp
    180e:	422c 0024      	clrb %a4@(36)
    1812:	45ec 0100      	lea %a4@(256),%a2
    1816:	260a           	movel %a2,%d3
    1818:	2f03           	movel %d3,%sp@-
    181a:	45fa ef5d      	lea %pc@(0x779),%a2
    181e:	2f0a           	movel %a2,%sp@-
    1820:	486c 0098      	pea %a4@(152)
    1824:	4e4f           	trap #15
    1826:	a2de           	0121336
    1828:	defc 000c      	addaw #12,%sp
    182c:	4a2c 0010      	tstb %a4@(16)
    1830:	663c           	bnes 0x186e
    1832:	2243           	moveal %d3,%a1
    1834:	d2fc 00ff      	addaw #255,%a1
    1838:	4a11           	tstb %a1@
    183a:	6714           	beqs 0x1850
    183c:	41ec 00d0      	lea %a4@(208),%a0
    1840:	1169 0001 0017 	moveb %a1@(1),%a0@(23)
    1846:	1169 0002 0018 	moveb %a1@(2),%a0@(24)
    184c:	6000 0020      	braw 0x186e
    1850:	41ec 00d0      	lea %a4@(208),%a0
    1854:	3010           	movew %a0@,%d0
    1856:	6c08           	bges 0x1860
    1858:	4228 0017      	clrb %a0@(23)
    185c:	6000 0006      	braw 0x1864
    1860:	1140 0017      	moveb %d0,%a0@(23)
    1864:	41ec 00d0      	lea %a4@(208),%a0
    1868:	1168 0003 0018 	moveb %a0@(3),%a0@(24)
    186e:	6100 4960      	bsrw 0x61d0
    1872:	4cee 0418 fff4 	moveml %fp@(-12),%d3-%d4/%a2
    1878:	4e5e           	unlk %fp
    187a:	4e75           	rts
    187c:	4e56 0000      	linkw %fp,#0
    1880:	48e7 1e20      	moveml %d3-%d6/%a2,%sp@-
    1884:	3c2e 0008      	movew %fp@(8),%d6
    1888:	663a           	bnes 0x18c4
    188a:	2f2c 0090      	movel %a4@(144),%sp@-
    188e:	4e4f           	trap #15
    1890:	a1fd           	0120775
    1892:	2a08           	movel %a0,%d5
    1894:	3f3c 03ea      	movew #1002,%sp@-
    1898:	2f3c 5462 6d70 	movel #1415736688,%sp@-
    189e:	4e4f           	trap #15
    18a0:	a05f           	0120137
    18a2:	2608           	movel %a0,%d3
    18a4:	2f03           	movel %d3,%sp@-
    18a6:	4e4f           	trap #15
    18a8:	a021           	0120041
    18aa:	4267           	clrw %sp@-
    18ac:	4267           	clrw %sp@-
    18ae:	2f08           	movel %a0,%sp@-
    18b0:	4e4f           	trap #15
    18b2:	a226           	0121046
    18b4:	2f03           	movel %d3,%sp@-
    18b6:	4e4f           	trap #15
    18b8:	a022           	0120042
    18ba:	2f03           	movel %d3,%sp@-
    18bc:	4e4f           	trap #15
    18be:	a061           	0120141
    18c0:	6000 006c      	braw 0x192e
    18c4:	45ec 00c0      	lea %a4@(192),%a2
    18c8:	2012           	movel %a2@,%d0
    18ca:	676c           	beqs 0x1938
    18cc:	4267           	clrw %sp@-
    18ce:	2f00           	movel %d0,%sp@-
    18d0:	4e4f           	trap #15
    18d2:	a05c           	0120134
    18d4:	2f08           	movel %a0,%sp@-
    18d6:	4e4f           	trap #15
    18d8:	a021           	0120041
    18da:	2608           	movel %a0,%d3
    18dc:	2f03           	movel %d3,%sp@-
    18de:	4e4f           	trap #15
    18e0:	a0ce           	0120316
    18e2:	3806           	movew %d6,%d4
    18e4:	d840           	addw %d0,%d4
    18e6:	2f03           	movel %d3,%sp@-
    18e8:	4e4f           	trap #15
    18ea:	a035           	0120065
    18ec:	4227           	clrb %sp@-
    18ee:	4267           	clrw %sp@-
    18f0:	2f12           	movel %a2@,%sp@-
    18f2:	4e4f           	trap #15
    18f4:	a05e           	0120136
    18f6:	2f2c 0090      	movel %a4@(144),%sp@-
    18fa:	4e4f           	trap #15
    18fc:	a1fd           	0120775
    18fe:	2a08           	movel %a0,%d5
    1900:	3f04           	movew %d4,%sp@-
    1902:	2f12           	movel %a2@,%sp@-
    1904:	4e4f           	trap #15
    1906:	a05c           	0120134
    1908:	2608           	movel %a0,%d3
    190a:	defc 0020      	addaw #32,%sp
    190e:	2e83           	movel %d3,%sp@
    1910:	4e4f           	trap #15
    1912:	a021           	0120041
    1914:	4267           	clrw %sp@-
    1916:	4267           	clrw %sp@-
    1918:	2f08           	movel %a0,%sp@-
    191a:	4e4f           	trap #15
    191c:	a226           	0121046
    191e:	2f03           	movel %d3,%sp@-
    1920:	4e4f           	trap #15
    1922:	a022           	0120042
    1924:	4227           	clrb %sp@-
    1926:	3f04           	movew %d4,%sp@-
    1928:	2f12           	movel %a2@,%sp@-
    192a:	4e4f           	trap #15
    192c:	a05e           	0120136
    192e:	2f05           	movel %d5,%sp@-
    1930:	4e4f           	trap #15
    1932:	a1fd           	0120775
    1934:	1946 0012      	moveb %d6,%a4@(18)
    1938:	4cee 0478 ffec 	moveml %fp@(-20),%d3-%d6/%a2
    193e:	4e5e           	unlk %fp
    1940:	4e75           	rts
    1942:	4465           	negw %a5@-
    1944:	6d6f           	blts 0x19b5
    1946:	204d           	moveal %a5,%a0
    1948:	6f64           	bles 0x19ae
    194a:	6500 4e56      	bcsw 0x67a2
    194e:	0000 48e7      	orib #-25,%d0
    1952:	1030 4240      	moveb %a0@(00000040,%d4:w:2),%d0
    1956:	102c 0007      	moveb %a4@(7),%d0
    195a:	5240           	addqw #1,%d0
    195c:	3f00           	movew %d0,%sp@-
    195e:	47fa ffe2      	lea %pc@(0x1942),%a3
    1962:	2f0b           	movel %a3,%sp@-
    1964:	45ec 0100      	lea %a4@(256),%a2
    1968:	2f0a           	movel %a2,%sp@-
    196a:	4e4f           	trap #15
    196c:	a2de           	0121336
    196e:	4243           	clrw %d3
    1970:	d4fc 001e      	addaw #30,%a2
    1974:	4242           	clrw %d2
    1976:	3203           	movew %d3,%d1
    1978:	48c1           	extl %d1
    197a:	2001           	movel %d1,%d0
    197c:	e780           	asll #3,%d0
    197e:	d081           	addl %d1,%d0
    1980:	d081           	addl %d1,%d0
    1982:	41f2 0800      	lea %a2@(00000000,%d0:l),%a0
    1986:	11bc fffe 2000 	moveb #-2,%a0@(00000000,%d2:w)
    198c:	5242           	addqw #1,%d2
    198e:	0c42 0009      	cmpiw #9,%d2
    1992:	6ff2           	bles 0x1986
    1994:	5243           	addqw #1,%d3
    1996:	0c43 0009      	cmpiw #9,%d3
    199a:	6fd8           	bles 0x1974
    199c:	4243           	clrw %d3
    199e:	41ec 0100      	lea %a4@(256),%a0
    19a2:	3003           	movew %d3,%d0
    19a4:	c1fc 000a      	mulsw #10,%d0
    19a8:	11bc 0023 0827 	moveb #35,%a0@(00000027,%d0:l)
    19ae:	5243           	addqw #1,%d3
    19b0:	0c43 0009      	cmpiw #9,%d3
    19b4:	6fe8           	bles 0x199e
    19b6:	4228 0082      	clrb %a0@(130)
    19ba:	4228 0083      	clrb %a0@(131)
    19be:	4228 00fc      	clrb %a0@(252)
    19c2:	4228 00fd      	clrb %a0@(253)
    19c6:	4228 00fe      	clrb %a0@(254)
    19ca:	4228 00ff      	clrb %a0@(255)
    19ce:	4228 0100      	clrb %a0@(256)
    19d2:	4228 0101      	clrb %a0@(257)
    19d6:	4228 0102      	clrb %a0@(258)
    19da:	4228 0103      	clrb %a0@(259)
    19de:	4243           	clrw %d3
    19e0:	41ec 0740      	lea %a4@(1856),%a0
    19e4:	3003           	movew %d3,%d0
    19e6:	c1fc 000c      	mulsw #12,%d0
    19ea:	11bc 00fe 0800 	moveb #-2,%a0@(00000000,%d0:l)
    19f0:	43ec 0100      	lea %a4@(256),%a1
    19f4:	41f1 0800      	lea %a1@(00000000,%d0:l),%a0
    19f8:	117c 00fe 0084 	moveb #-2,%a0@(132)
    19fe:	5243           	addqw #1,%d3
    1a00:	0c43 0009      	cmpiw #9,%d3
    1a04:	6fda           	bles 0x19e0
    1a06:	4229 010f      	clrb %a1@(271)
    1a0a:	41ec 00d0      	lea %a4@(208),%a0
    1a0e:	117c 000a 000b 	moveb #10,%a0@(11)
    1a14:	117c 000a 000c 	moveb #10,%a0@(12)
    1a1a:	317c 0079 0002 	movew #121,%a0@(2)
    1a20:	30bc 0040      	movew #64,%a0@
    1a24:	4cee 0c08 fff4 	moveml %fp@(-12),%d3/%a2-%a3
    1a2a:	4e5e           	unlk %fp
    1a2c:	4e75           	rts
    1a2e:	4e56 ffe4      	linkw %fp,#-28
    1a32:	48e7 1820      	moveml %d3-%d4/%a2,%sp@-
    1a36:	0c6c 00fe 0230 	cmpiw #254,%a4@(560)
    1a3c:	6608           	bnes 0x1a46
    1a3e:	4878 0004      	pea 0x4
    1a42:	6000 0006      	braw 0x1a4a
    1a46:	4878 0001      	pea 0x1
    1a4a:	486e ffe8      	pea %fp@(-24)
    1a4e:	4e4f           	trap #15
    1a50:	a11d           	0120435
    1a52:	504f           	addqw #8,%sp
    1a54:	0c6e 0004 ffe8 	cmpiw #4,%fp@(-24)
    1a5a:	6628           	bnes 0x1a84
    1a5c:	322e fff0      	movew %fp@(-16),%d1
    1a60:	3001           	movew %d1,%d0
    1a62:	0640 fff5      	addiw #-11,%d0
    1a66:	0c40 0001      	cmpiw #1,%d0
    1a6a:	6328           	blss 0x1a94
    1a6c:	3001           	movew %d1,%d0
    1a6e:	0640 fdfc      	addiw #-516,%d0
    1a72:	0c40 0001      	cmpiw #1,%d0
    1a76:	631c           	blss 0x1a94
    1a78:	3001           	movew %d1,%d0
    1a7a:	0640 fdfa      	addiw #-518,%d0
    1a7e:	0c40 0001      	cmpiw #1,%d0
    1a82:	6310           	blss 0x1a94
    1a84:	486e ffe8      	pea %fp@(-24)
    1a88:	4e4f           	trap #15
    1a8a:	a0a9           	0120251
    1a8c:	584f           	addqw #4,%sp
    1a8e:	4a00           	tstb %d0
    1a90:	6600 00c2      	bnew 0x1b54
    1a94:	486e ffe6      	pea %fp@(-26)
    1a98:	486e ffe8      	pea %fp@(-24)
    1a9c:	42a7           	clrl %sp@-
    1a9e:	4e4f           	trap #15
    1aa0:	a1bf           	0120677
    1aa2:	defc 000c      	addaw #12,%sp
    1aa6:	4a00           	tstb %d0
    1aa8:	6600 00aa      	bnew 0x1b54
    1aac:	0c6e 0017 ffe8 	cmpiw #23,%fp@(-24)
    1ab2:	6628           	bnes 0x1adc
    1ab4:	362e fff0      	movew %fp@(-16),%d3
    1ab8:	3f03           	movew %d3,%sp@-
    1aba:	4e4f           	trap #15
    1abc:	a16f           	0120557
    1abe:	2808           	movel %a0,%d4
    1ac0:	2f04           	movel %d4,%sp@-
    1ac2:	4e4f           	trap #15
    1ac4:	a174           	0120564
    1ac6:	5c4f           	addqw #6,%sp
    1ac8:	0c43 03e8      	cmpiw #1000,%d3
    1acc:	660e           	bnes 0x1adc
    1ace:	43fa 026e      	lea %pc@(0x1d3e),%a1
    1ad2:	2f09           	movel %a1,%sp@-
    1ad4:	2f04           	movel %d4,%sp@-
    1ad6:	4e4f           	trap #15
    1ad8:	a19f           	0120637
    1ada:	504f           	addqw #8,%sp
    1adc:	0c6e 0004 ffe8 	cmpiw #4,%fp@(-24)
    1ae2:	6666           	bnes 0x1b4a
    1ae4:	45ec 00d0      	lea %a4@(208),%a2
    1ae8:	4a2a 000c      	tstb %a2@(12)
    1aec:	675c           	beqs 0x1b4a
    1aee:	4e4f           	trap #15
    1af0:	a2a2           	0121242
    1af2:	0800 0004      	btst #4,%d0
    1af6:	670e           	beqs 0x1b06
    1af8:	2f0a           	movel %a2,%sp@-
    1afa:	6100 0b32      	bsrw 0x262e
    1afe:	6100 46d0      	bsrw 0x61d0
    1b02:	6000 004e      	braw 0x1b52
    1b06:	0800 0005      	btst #5,%d0
    1b0a:	670e           	beqs 0x1b1a
    1b0c:	2f0a           	movel %a2,%sp@-
    1b0e:	6100 0e02      	bsrw 0x2912
    1b12:	6100 46bc      	bsrw 0x61d0
    1b16:	6000 003a      	braw 0x1b52
    1b1a:	0800 0003      	btst #3,%d0
    1b1e:	670c           	beqs 0x1b2c
    1b20:	6100 1c82      	bsrw 0x37a4
    1b24:	6100 46aa      	bsrw 0x61d0
    1b28:	6000 002a      	braw 0x1b54
    1b2c:	0800 0006      	btst #6,%d0
    1b30:	6718           	beqs 0x1b4a
    1b32:	41ec 0100      	lea %a4@(256),%a0
    1b36:	4868 0104      	pea %a0@(260)
    1b3a:	2f0a           	movel %a2,%sp@-
    1b3c:	6100 248a      	bsrw 0x3fc8
    1b40:	6100 468e      	bsrw 0x61d0
    1b44:	504f           	addqw #8,%sp
    1b46:	6000 000c      	braw 0x1b54
    1b4a:	486e ffe8      	pea %fp@(-24)
    1b4e:	4e4f           	trap #15
    1b50:	a1a0           	0120640
    1b52:	584f           	addqw #4,%sp
    1b54:	0c6e 0016 ffe8 	cmpiw #22,%fp@(-24)
    1b5a:	6600 feda      	bnew 0x1a36
    1b5e:	4cee 0418 ffd8 	moveml %fp@(-40),%d3-%d4/%a2
    1b64:	4e5e           	unlk %fp
    1b66:	4e75           	rts
    1b68:	4e56 0000      	linkw %fp,#0
    1b6c:	6100 0006      	bsrw 0x1b74
    1b70:	4e5e           	unlk %fp
    1b72:	4e75           	rts
    1b74:	4e56 0000      	linkw %fp,#0
    1b78:	2f0a           	movel %a2,%sp@-
    1b7a:	4240           	clrw %d0
    1b7c:	102c 0007      	moveb %a4@(7),%d0
    1b80:	3f00           	movew %d0,%sp@-
    1b82:	6100 f998      	bsrw 0x151c
    1b86:	4267           	clrw %sp@-
    1b88:	6100 f516      	bsrw 0x10a0
    1b8c:	4241           	clrw %d1
    1b8e:	584f           	addqw #4,%sp
    1b90:	45ec 0100      	lea %a4@(256),%a2
    1b94:	3001           	movew %d1,%d0
    1b96:	c1fc 000c      	mulsw #12,%d0
    1b9a:	43f2 0800      	lea %a2@(00000000,%d0:l),%a1
    1b9e:	41ec 0740      	lea %a4@(1856),%a0
    1ba2:	1370 0800 0084 	moveb %a0@(00000000,%d0:l),%a1@(132)
    1ba8:	1370 080a 008e 	moveb %a0@(0000000a,%d0:l),%a1@(142)
    1bae:	1370 0801 0085 	moveb %a0@(00000001,%d0:l),%a1@(133)
    1bb4:	1370 0802 0086 	moveb %a0@(00000002,%d0:l),%a1@(134)
    1bba:	1370 0803 0087 	moveb %a0@(00000003,%d0:l),%a1@(135)
    1bc0:	1370 0804 0088 	moveb %a0@(00000004,%d0:l),%a1@(136)
    1bc6:	1370 0805 0089 	moveb %a0@(00000005,%d0:l),%a1@(137)
    1bcc:	1370 0806 008a 	moveb %a0@(00000006,%d0:l),%a1@(138)
    1bd2:	1370 0807 008b 	moveb %a0@(00000007,%d0:l),%a1@(139)
    1bd8:	1370 0808 008c 	moveb %a0@(00000008,%d0:l),%a1@(140)
    1bde:	1370 0809 008d 	moveb %a0@(00000009,%d0:l),%a1@(141)
    1be4:	1370 080a 008e 	moveb %a0@(0000000a,%d0:l),%a1@(142)
    1bea:	1370 080b 008f 	moveb %a0@(0000000b,%d0:l),%a1@(143)
    1bf0:	5241           	addqw #1,%d1
    1bf2:	0c41 0009      	cmpiw #9,%d1
    1bf6:	6f98           	bles 0x1b90
    1bf8:	43ec 00d0      	lea %a4@(208),%a1
    1bfc:	1569 0004 0108 	moveb %a1@(4),%a2@(264)
    1c02:	2049           	moveal %a1,%a0
    1c04:	3558 0104      	movew %a0@+,%a2@(260)
    1c08:	3550 0106      	movew %a0@,%a2@(262)
    1c0c:	1569 0005 0109 	moveb %a1@(5),%a2@(265)
    1c12:	1569 0006 010a 	moveb %a1@(6),%a2@(266)
    1c18:	1569 0007 010b 	moveb %a1@(7),%a2@(267)
    1c1e:	1569 0008 010c 	moveb %a1@(8),%a2@(268)
    1c24:	1569 0009 010d 	moveb %a1@(9),%a2@(269)
    1c2a:	1569 000a 010e 	moveb %a1@(10),%a2@(270)
    1c30:	3569 0012 0116 	movew %a1@(18),%a2@(278)
    1c36:	1569 000c 0110 	moveb %a1@(12),%a2@(272)
    1c3c:	1569 000b 010f 	moveb %a1@(11),%a2@(271)
    1c42:	1569 000d 0111 	moveb %a1@(13),%a2@(273)
    1c48:	1569 000e 0112 	moveb %a1@(14),%a2@(274)
    1c4e:	1569 000f 0113 	moveb %a1@(15),%a2@(275)
    1c54:	1569 0010 0114 	moveb %a1@(16),%a2@(276)
    1c5a:	1569 0011 0115 	moveb %a1@(17),%a2@(277)
    1c60:	1569 0014 0118 	moveb %a1@(20),%a2@(280)
    1c66:	1569 0015 0119 	moveb %a1@(21),%a2@(281)
    1c6c:	156c 0007 011a 	moveb %a4@(7),%a2@(282)
    1c72:	1569 0017 011b 	moveb %a1@(23),%a2@(283)
    1c78:	1569 0018 011c 	moveb %a1@(24),%a2@(284)
    1c7e:	1569 0019 011d 	moveb %a1@(25),%a2@(285)
    1c84:	1569 001a 011e 	moveb %a1@(26),%a2@(286)
    1c8a:	1569 001b 011f 	moveb %a1@(27),%a2@(287)
    1c90:	1569 001c 0120 	moveb %a1@(28),%a2@(288)
    1c96:	1569 001d 0121 	moveb %a1@(29),%a2@(289)
    1c9c:	1569 001e 0122 	moveb %a1@(30),%a2@(290)
    1ca2:	1569 001f 0123 	moveb %a1@(31),%a2@(291)
    1ca8:	1569 0020 0124 	moveb %a1@(32),%a2@(292)
    1cae:	156c 0010 0125 	moveb %a4@(16),%a2@(293)
    1cb4:	4267           	clrw %sp@-
    1cb6:	6100 f864      	bsrw 0x151c
    1cba:	2f2c 073c      	movel %a4@(1852),%sp@-
    1cbe:	4e4f           	trap #15
    1cc0:	a04a           	0120112
    1cc2:	5c4f           	addqw #6,%sp
    1cc4:	202c 00c0      	movel %a4@(192),%d0
    1cc8:	6708           	beqs 0x1cd2
    1cca:	2f00           	movel %d0,%sp@-
    1ccc:	4e4f           	trap #15
    1cce:	a04a           	0120112
    1cd0:	584f           	addqw #4,%sp
    1cd2:	4a2c 0016      	tstb %a4@(22)
    1cd6:	6712           	beqs 0x1cea
    1cd8:	486c 0025      	pea %a4@(37)
    1cdc:	4267           	clrw %sp@-
    1cde:	4e4f           	trap #15
    1ce0:	a045           	0120105
    1ce2:	2f00           	movel %d0,%sp@-
    1ce4:	4267           	clrw %sp@-
    1ce6:	4e4f           	trap #15
    1ce8:	a042           	0120102
    1cea:	246e fffc      	moveal %fp@(-4),%a2
    1cee:	4e5e           	unlk %fp
    1cf0:	4e75           	rts
    1cf2:	5468 6973      	addqw #2,%a0@(26995)
    1cf6:	2069 7320      	moveal %a1@(29472),%a0
    1cfa:	6c61           	bges 0x1d5d
    1cfc:	6d65           	blts 0x1d63
    1cfe:	2120           	movel %a0@-,%a0@-
    1d00:	596f 7520      	subqw #4,%sp@(29984)
    1d04:	6e65           	bgts 0x1d6b
    1d06:	6564           	bcss 0x1d6c
    1d08:	2074 6f20 696e 	moveal %a4@(0000696e,%d6:l:8),%a0
    1d0e:	7374           	071564
    1d10:	616c           	bsrs 0x1d7e
    1d12:	6c20           	bges 0x1d34
    1d14:	616e           	bsrs 0x1d84
    1d16:	2061           	moveal %a1@-,%a0
    1d18:	6476           	bccs 0x1d90
    1d1a:	656e           	bcss 0x1d8a
    1d1c:	7475           	moveq #117,%d2
    1d1e:	7265           	moveq #101,%d1
    1d20:	2120           	movel %a0@-,%a0@-
    1d22:	2850           	moveal %a0@,%a4
    1d24:	6172           	bsrs 0x1d98
    1d26:	7473           	moveq #115,%d2
    1d28:	2041           	moveal %d1,%a0
    1d2a:	2061           	moveal %a1@-,%a0
    1d2c:	6e64           	bgts 0x1d92
    1d2e:	2042           	moveal %d2,%a0
    1d30:	2900           	movel %d0,%a4@-
    1d32:	506c 6561      	addqw #8,%a4@(25953)
    1d36:	7365           	071545
    1d38:	2057           	moveal %sp@,%a0
    1d3a:	6169           	bsrs 0x1da5
    1d3c:	7400           	moveq #0,%d2
    1d3e:	4e56 0000      	linkw %fp,#0
    1d42:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    1d46:	226e 0008      	moveal %fp@(8),%a1
    1d4a:	4241           	clrw %d1
    1d4c:	3011           	movew %a1@,%d0
    1d4e:	0c40 0018      	cmpiw #24,%d0
    1d52:	6200 0396      	bhiw 0x20ea
    1d56:	0280 0000 ffff 	andil #65535,%d0
    1d5c:	41fa 000c      	lea %pc@(0x1d6a),%a0
    1d60:	d080           	addl %d0,%d0
    1d62:	3030 0800      	movew %a0@(00000000,%d0:l),%d0
    1d66:	4efb 0002      	jmp %pc@(0x1d6a,%d0:w)
    1d6a:	01ce 01f4      	movepl %d0,%fp@(500)
    1d6e:	0380           	bclr %d1,%d0
    1d70:	0380           	bclr %d1,%d0
    1d72:	0294 0380 0380 	andil #58721152,%a4@
    1d78:	0380           	bclr %d1,%d0
    1d7a:	0380           	bclr %d1,%d0
    1d7c:	0380           	bclr %d1,%d0
    1d7e:	0380           	bclr %d1,%d0
    1d80:	0380           	bclr %d1,%d0
    1d82:	0380           	bclr %d1,%d0
    1d84:	0380           	bclr %d1,%d0
    1d86:	0380           	bclr %d1,%d0
    1d88:	0380           	bclr %d1,%d0
    1d8a:	0380           	bclr %d1,%d0
    1d8c:	0380           	bclr %d1,%d0
    1d8e:	0380           	bclr %d1,%d0
    1d90:	0380           	bclr %d1,%d0
    1d92:	0380           	bclr %d1,%d0
    1d94:	0086 0380 0380 	oril #58721152,%d6
    1d9a:	0032 4e4f a173 	orib #79,%a2@(2f084e4f)@(ffffffffa17145ec)
    1da0:	2f08 4e4f a171
    1da6:	45ec
    1da8:	000c 584f      	orib #79,%a4
    1dac:	4a12           	tstb %a2@
    1dae:	6700 0338      	beqw 0x20e8
    1db2:	6100 e5a4      	bsrw 0x358
    1db6:	6100 f096      	bsrw 0xe4e
    1dba:	6100 e7fe      	bsrw 0x5ba
    1dbe:	6100 e9ca      	bsrw 0x78a
    1dc2:	6100 4eac      	bsrw 0x6c70
    1dc6:	6100 032e      	bsrw 0x20f6
    1dca:	4212           	clrb %a2@
    1dcc:	422c 0022      	clrb %a4@(34)
    1dd0:	6100 43fe      	bsrw 0x61d0
    1dd4:	4a2c 0016      	tstb %a4@(22)
    1dd8:	6700 030e      	beqw 0x20e8
    1ddc:	47fa ff14      	lea %pc@(0x1cf2),%a3
    1de0:	2f0b           	movel %a3,%sp@-
    1de2:	4267           	clrw %sp@-
    1de4:	3f3c 029c      	movew #668,%sp@-
    1de8:	6100 28a2      	bsrw 0x468c
    1dec:	6000 02fa      	braw 0x20e8
    1df0:	3029 0008      	movew %a1@(8),%d0
    1df4:	0640 fc17      	addiw #-1001,%d0
    1df8:	0c40 0009      	cmpiw #9,%d0
    1dfc:	6200 0158      	bhiw 0x1f56
    1e00:	0280 0000 ffff 	andil #65535,%d0
    1e06:	41fa 000c      	lea %pc@(0x1e14),%a0
    1e0a:	d080           	addl %d0,%d0
    1e0c:	3030 0800      	movew %a0@(00000000,%d0:l),%d0
    1e10:	4efb 0002      	jmp %pc@(0x1e14,%d0:w)
    1e14:	0014 0142      	orib #66,%a4@
    1e18:	0142           	bchg %d0,%d2
    1e1a:	00bc           	0274
    1e1c:	0142           	bchg %d0,%d2
    1e1e:	0062 001c      	oriw #28,%a2@-
    1e22:	0040 0142      	oriw #322,%d0
    1e26:	0092 3f3c 03e9 	oril #1060897769,%a2@
    1e2c:	6000 005e      	braw 0x1e8c
    1e30:	3f3c 03fe      	movew #1022,%sp@-
    1e34:	4e4f           	trap #15
    1e36:	a16f           	0120557
    1e38:	2448           	moveal %a0,%a2
    1e3a:	2f0a           	movel %a2,%sp@-
    1e3c:	4e4f           	trap #15
    1e3e:	a193           	0120623
    1e40:	2f0a           	movel %a2,%sp@-
    1e42:	4e4f           	trap #15
    1e44:	a170           	0120560
    1e46:	197c 0001 0023 	moveb #1,%a4@(35)
    1e4c:	defc 000a      	addaw #10,%sp
    1e50:	6000 0104      	braw 0x1f56
    1e54:	3f3c 0408      	movew #1032,%sp@-
    1e58:	4e4f           	trap #15
    1e5a:	a16f           	0120557
    1e5c:	2448           	moveal %a0,%a2
    1e5e:	2f0a           	movel %a2,%sp@-
    1e60:	4e4f           	trap #15
    1e62:	a193           	0120623
    1e64:	2f0a           	movel %a2,%sp@-
    1e66:	4e4f           	trap #15
    1e68:	a170           	0120560
    1e6a:	422c 0023      	clrb %a4@(35)
    1e6e:	defc 000a      	addaw #10,%sp
    1e72:	6000 00e2      	braw 0x1f56
    1e76:	45ec 000e      	lea %a4@(14),%a2
    1e7a:	4a12           	tstb %a2@
    1e7c:	660a           	bnes 0x1e88
    1e7e:	6100 4b4e      	bsrw 0x69ce
    1e82:	4a12           	tstb %a2@
    1e84:	6700 00d0      	beqw 0x1f56
    1e88:	3f3c 040f      	movew #1039,%sp@-
    1e8c:	4e4f           	trap #15
    1e8e:	a16f           	0120557
    1e90:	2448           	moveal %a0,%a2
    1e92:	2f0a           	movel %a2,%sp@-
    1e94:	4e4f           	trap #15
    1e96:	a193           	0120623
    1e98:	2f0a           	movel %a2,%sp@-
    1e9a:	4e4f           	trap #15
    1e9c:	a170           	0120560
    1e9e:	defc 000a      	addaw #10,%sp
    1ea2:	6000 00b2      	braw 0x1f56
    1ea6:	6100 f134      	bsrw 0xfdc
    1eaa:	4267           	clrw %sp@-
    1eac:	3f3c 0041      	movew #65,%sp@-
    1eb0:	47fa fe80      	lea %pc@(0x1d32),%a3
    1eb4:	2f0b           	movel %a3,%sp@-
    1eb6:	6100 2764      	bsrw 0x461c
    1eba:	6100 fcb8      	bsrw 0x1b74
    1ebe:	6100 e6fa      	bsrw 0x5ba
    1ec2:	6100 e8c6      	bsrw 0x78a
    1ec6:	6100 022e      	bsrw 0x20f6
    1eca:	504f           	addqw #8,%sp
    1ecc:	6000 0088      	braw 0x1f56
    1ed0:	3f3c 03f4      	movew #1012,%sp@-
    1ed4:	4e4f           	trap #15
    1ed6:	a16f           	0120557
    1ed8:	2448           	moveal %a0,%a2
    1eda:	2f0a           	movel %a2,%sp@-
    1edc:	4e4f           	trap #15
    1ede:	a193           	0120623
    1ee0:	3600           	movew %d0,%d3
    1ee2:	2f0a           	movel %a2,%sp@-
    1ee4:	4e4f           	trap #15
    1ee6:	a170           	0120560
    1ee8:	defc 000a      	addaw #10,%sp
    1eec:	0c43 03f8      	cmpiw #1016,%d3
    1ef0:	6664           	bnes 0x1f56
    1ef2:	6100 f11e      	bsrw 0x1012
    1ef6:	6100 ef56      	bsrw 0xe4e
    1efa:	2f2c 073c      	movel %a4@(1852),%sp@-
    1efe:	4e4f           	trap #15
    1f00:	a04a           	0120112
    1f02:	584f           	addqw #4,%sp
    1f04:	202c 00c0      	movel %a4@(192),%d0
    1f08:	6708           	beqs 0x1f12
    1f0a:	2f00           	movel %d0,%sp@-
    1f0c:	4e4f           	trap #15
    1f0e:	a04a           	0120112
    1f10:	584f           	addqw #4,%sp
    1f12:	486c 0025      	pea %a4@(37)
    1f16:	4267           	clrw %sp@-
    1f18:	4e4f           	trap #15
    1f1a:	a045           	0120105
    1f1c:	2f00           	movel %d0,%sp@-
    1f1e:	4267           	clrw %sp@-
    1f20:	4e4f           	trap #15
    1f22:	a042           	0120102
    1f24:	6100 e864      	bsrw 0x78a
    1f28:	6100 f0e8      	bsrw 0x1012
    1f2c:	6100 01c8      	bsrw 0x20f6
    1f30:	defc 000c      	addaw #12,%sp
    1f34:	6000 0020      	braw 0x1f56
    1f38:	45ec 0100      	lea %a4@(256),%a2
    1f3c:	486a 0104      	pea %a2@(260)
    1f40:	47ec 00d0      	lea %a4@(208),%a3
    1f44:	260b           	movel %a3,%d3
    1f46:	2f03           	movel %d3,%sp@-
    1f48:	6100 3b70      	bsrw 0x5aba
    1f4c:	2f03           	movel %d3,%sp@-
    1f4e:	486a 0104      	pea %a2@(260)
    1f52:	6100 3b66      	bsrw 0x5aba
    1f56:	6100 4278      	bsrw 0x61d0
    1f5a:	6000 018c      	braw 0x20e8
    1f5e:	45ec 000d      	lea %a4@(13),%a2
    1f62:	1212           	moveb %a2@,%d1
    1f64:	6766           	beqs 0x1fcc
    1f66:	0c01 000b      	cmpib #11,%d1
    1f6a:	661c           	bnes 0x1f88
    1f6c:	0c69 0082 0006 	cmpiw #130,%a1@(6)
    1f72:	6fe2           	bles 0x1f56
    1f74:	3029 0004      	movew %a1@(4),%d0
    1f78:	0640 ffcd      	addiw #-51,%d0
    1f7c:	0c40 003a      	cmpiw #58,%d0
    1f80:	62d4           	bhis 0x1f56
    1f82:	4212           	clrb %a2@
    1f84:	6000 ffd0      	braw 0x1f56
    1f88:	0c69 007d 0006 	cmpiw #125,%a1@(6)
    1f8e:	6fc6           	bles 0x1f56
    1f90:	3029 0004      	movew %a1@(4),%d0
    1f94:	0c40 0031      	cmpiw #49,%d0
    1f98:	6fe8           	bles 0x1f82
    1f9a:	0c40 0069      	cmpiw #105,%d0
    1f9e:	6f12           	bles 0x1fb2
    1fa0:	4240           	clrw %d0
    1fa2:	1001           	moveb %d1,%d0
    1fa4:	5340           	subqw #1,%d0
    1fa6:	3f00           	movew %d0,%sp@-
    1fa8:	6100 021a      	bsrw 0x21c4
    1fac:	544f           	addqw #2,%sp
    1fae:	6000 ffa6      	braw 0x1f56
    1fb2:	5301           	subqb #1,%d1
    1fb4:	1f01           	moveb %d1,%sp@-
    1fb6:	6100 2eb2      	bsrw 0x4e6a
    1fba:	1012           	moveb %a2@,%d0
    1fbc:	544f           	addqw #2,%sp
    1fbe:	0c00 0001      	cmpib #1,%d0
    1fc2:	632a           	blss 0x1fee
    1fc4:	5300           	subqb #1,%d0
    1fc6:	1480           	moveb %d0,%a2@
    1fc8:	6000 0024      	braw 0x1fee
    1fcc:	0c69 0013 0006 	cmpiw #19,%a1@(6)
    1fd2:	6e14           	bgts 0x1fe8
    1fd4:	0c69 008c 0004 	cmpiw #140,%a1@(4)
    1fda:	6f0c           	bles 0x1fe8
    1fdc:	14bc 000b      	moveb #11,%a2@
    1fe0:	6100 2c06      	bsrw 0x4be8
    1fe4:	6000 ff70      	braw 0x1f56
    1fe8:	197c 0001 000d 	moveb #1,%a4@(13)
    1fee:	6100 2c5e      	bsrw 0x4c4e
    1ff2:	6000 ff62      	braw 0x1f56
    1ff6:	1941 000d      	moveb %d1,%a4@(13)
    1ffa:	6000 0044      	braw 0x2040
    1ffe:	4e4f           	trap #15
    2000:	a2a2           	0121242
    2002:	2600           	movel %d0,%d3
    2004:	0803 0001      	btst #1,%d3
    2008:	6758           	beqs 0x2062
    200a:	41ec 000d      	lea %a4@(13),%a0
    200e:	1010           	moveb %a0@,%d0
    2010:	6736           	beqs 0x2048
    2012:	0c00 000a      	cmpib #10,%d0
    2016:	6242           	bhis 0x205a
    2018:	1400           	moveb %d0,%d2
    201a:	5302           	subqb #1,%d2
    201c:	1082           	moveb %d2,%a0@
    201e:	0c00 0001      	cmpib #1,%d0
    2022:	661c           	bnes 0x2040
    2024:	4241           	clrw %d1
    2026:	3001           	movew %d1,%d0
    2028:	c1fc 000c      	mulsw #12,%d0
    202c:	41ec 0740      	lea %a4@(1856),%a0
    2030:	0c30 00fe 0800 	cmpib #-2,%a0@(00000000,%d0:l)
    2036:	67be           	beqs 0x1ff6
    2038:	5241           	addqw #1,%d1
    203a:	0c41 0009      	cmpiw #9,%d1
    203e:	6fe6           	bles 0x2026
    2040:	6100 2c0c      	bsrw 0x4c4e
    2044:	6000 0014      	braw 0x205a
    2048:	41ec 00d0      	lea %a4@(208),%a0
    204c:	4a28 000c      	tstb %a0@(12)
    2050:	6708           	beqs 0x205a
    2052:	2f08           	movel %a0,%sp@-
    2054:	6100 0cfe      	bsrw 0x2d54
    2058:	584f           	addqw #4,%sp
    205a:	6100 4174      	bsrw 0x61d0
    205e:	6000 0016      	braw 0x2076
    2062:	41ec 00d0      	lea %a4@(208),%a0
    2066:	4a28 0005      	tstb %a0@(5)
    206a:	660a           	bnes 0x2076
    206c:	0803 0002      	btst #2,%d3
    2070:	660a           	bnes 0x207c
    2072:	4228 0021      	clrb %a0@(33)
    2076:	0803 0002      	btst #2,%d3
    207a:	6758           	beqs 0x20d4
    207c:	43ec 000d      	lea %a4@(13),%a1
    2080:	1011           	moveb %a1@,%d0
    2082:	6738           	beqs 0x20bc
    2084:	0c00 000a      	cmpib #10,%d0
    2088:	6200 fecc      	bhiw 0x1f56
    208c:	1400           	moveb %d0,%d2
    208e:	5202           	addqb #1,%d2
    2090:	1282           	moveb %d2,%a1@
    2092:	5200           	addqb #1,%d0
    2094:	0c00 000b      	cmpib #11,%d0
    2098:	6716           	beqs 0x20b0
    209a:	0240 00ff      	andiw #255,%d0
    209e:	5340           	subqw #1,%d0
    20a0:	c1fc 000c      	mulsw #12,%d0
    20a4:	41ec 0740      	lea %a4@(1856),%a0
    20a8:	0c30 00fe 0800 	cmpib #-2,%a0@(00000000,%d0:l)
    20ae:	6604           	bnes 0x20b4
    20b0:	12bc 0001      	moveb #1,%a1@
    20b4:	6100 2b98      	bsrw 0x4c4e
    20b8:	6000 fe9c      	braw 0x1f56
    20bc:	41ec 00d0      	lea %a4@(208),%a0
    20c0:	4a28 000c      	tstb %a0@(12)
    20c4:	6700 fe90      	beqw 0x1f56
    20c8:	2f08           	movel %a0,%sp@-
    20ca:	6100 0ec4      	bsrw 0x2f90
    20ce:	584f           	addqw #4,%sp
    20d0:	6000 fe84      	braw 0x1f56
    20d4:	41ec 00d0      	lea %a4@(208),%a0
    20d8:	4a28 0005      	tstb %a0@(5)
    20dc:	660a           	bnes 0x20e8
    20de:	0803 0001      	btst #1,%d3
    20e2:	6604           	bnes 0x20e8
    20e4:	4228 0021      	clrb %a0@(33)
    20e8:	7201           	moveq #1,%d1
    20ea:	3001           	movew %d1,%d0
    20ec:	4cee 0c08 fff4 	moveml %fp@(-12),%d3/%a2-%a3
    20f2:	4e5e           	unlk %fp
    20f4:	4e75           	rts
    20f6:	4e56 0000      	linkw %fp,#0
    20fa:	48e7 1c20      	moveml %d3-%d5/%a2,%sp@-
    20fe:	45ec 00d0      	lea %a4@(208),%a2
    2102:	4a2a 0015      	tstb %a2@(21)
    2106:	6646           	bnes 0x214e
    2108:	2f2c 0088      	movel %a4@(136),%sp@-
    210c:	4e4f           	trap #15
    210e:	a1fd           	0120775
    2110:	2808           	movel %a0,%d4
    2112:	3f3c 03e9      	movew #1001,%sp@-
    2116:	2f3c 5462 6d70 	movel #1415736688,%sp@-
    211c:	4e4f           	trap #15
    211e:	a05f           	0120137
    2120:	2608           	movel %a0,%d3
    2122:	2f03           	movel %d3,%sp@-
    2124:	4e4f           	trap #15
    2126:	a021           	0120041
    2128:	4267           	clrw %sp@-
    212a:	4267           	clrw %sp@-
    212c:	2f08           	movel %a0,%sp@-
    212e:	4e4f           	trap #15
    2130:	a226           	0121046
    2132:	2f03           	movel %d3,%sp@-
    2134:	4e4f           	trap #15
    2136:	a022           	0120042
    2138:	2f03           	movel %d3,%sp@-
    213a:	4e4f           	trap #15
    213c:	a061           	0120141
    213e:	2f04           	movel %d4,%sp@-
    2140:	4e4f           	trap #15
    2142:	a1fd           	0120775
    2144:	157c 0001 0014 	moveb #1,%a2@(20)
    214a:	6000 0046      	braw 0x2192
    214e:	4240           	clrw %d0
    2150:	4242           	clrw %d2
    2152:	3200           	movew %d0,%d1
    2154:	c3fc 000c      	mulsw #12,%d1
    2158:	43ec 0740      	lea %a4@(1856),%a1
    215c:	0c31 0006 1805 	cmpib #6,%a1@(00000005,%d1:l)
    2162:	6626           	bnes 0x218a
    2164:	41ec 00d0      	lea %a4@(208),%a0
    2168:	1a31 180a      	moveb %a1@(0000000a,%d1:l),%d5
    216c:	ba28 0015      	cmpb %a0@(21),%d5
    2170:	6618           	bnes 0x218a
    2172:	4aac 00c0      	tstl %a4@(192)
    2176:	671a           	beqs 0x2192
    2178:	1031 1808      	moveb %a1@(00000008,%d1:l),%d0
    217c:	6714           	beqs 0x2192
    217e:	1400           	moveb %d0,%d2
    2180:	3f02           	movew %d2,%sp@-
    2182:	6100 38b2      	bsrw 0x5a36
    2186:	6000 000a      	braw 0x2192
    218a:	5240           	addqw #1,%d0
    218c:	0c40 0009      	cmpiw #9,%d0
    2190:	6fc0           	bles 0x2152
    2192:	4cee 0438 fff0 	moveml %fp@(-16),%d3-%d5/%a2
    2198:	4e5e           	unlk %fp
    219a:	4e75           	rts
    219c:	4920           	chkl %a0@-,%d4
    219e:	6665           	bnes 0x2205
    21a0:	656c           	bcss 0x220e
    21a2:	2062           	moveal %a2@-,%a0
    21a4:	6574           	bcss 0x221a
    21a6:	7465           	moveq #101,%d2
    21a8:	7221           	moveq #33,%d1
    21aa:	0041 7567      	oriw #30055,%d1
    21ae:	6821           	bvcs 0x21d1
    21b0:	2050           	moveal %a0@,%a0
    21b2:	6f69           	bles 0x221d
    21b4:	736f           	071557
    21b6:	6e21           	bgts 0x21d9
    21b8:	0054 7874      	oriw #30836,%a4@
    21bc:	4442           	negw %d2
    21be:	2045           	moveal %d5,%a0
    21c0:	7272           	moveq #114,%d1
    21c2:	0000 4e56      	orib #86,%d0
    21c6:	fefc           	0177374
    21c8:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    21cc:	3a2e 0008      	movew %fp@(8),%d5
    21d0:	3605           	movew %d5,%d3
    21d2:	c7fc 000c      	mulsw #12,%d3
    21d6:	45ec 0740      	lea %a4@(1856),%a2
    21da:	1032 3805      	moveb %a2@(00000005,%d3:l),%d0
    21de:	0c00 0003      	cmpib #3,%d0
    21e2:	6600 012a      	bnew 0x230e
    21e6:	4240           	clrw %d0
    21e8:	1032 3806      	moveb %a2@(00000006,%d3:l),%d0
    21ec:	0c40 0004      	cmpiw #4,%d0
    21f0:	670a           	beqs 0x21fc
    21f2:	0c40 0005      	cmpiw #5,%d0
    21f6:	6732           	beqs 0x222a
    21f8:	6000 0084      	braw 0x227e
    21fc:	41ec 00d0      	lea %a4@(208),%a0
    2200:	47e8 000c      	lea %a0@(12),%a3
    2204:	1013           	moveb %a3@,%d0
    2206:	d032 3807      	addb %a2@(00000007,%d3:l),%d0
    220a:	1680           	moveb %d0,%a3@
    220c:	1228 000b      	moveb %a0@(11),%d1
    2210:	b200           	cmpb %d0,%d1
    2212:	6402           	bccs 0x2216
    2214:	1681           	moveb %d1,%a3@
    2216:	3f3c 0009      	movew #9,%sp@-
    221a:	6100 41ec      	bsrw 0x6408
    221e:	6100 4202      	bsrw 0x6422
    2222:	43fa ff78      	lea %pc@(0x219c),%a1
    2226:	6000 0044      	braw 0x226c
    222a:	41ec 00d0      	lea %a4@(208),%a0
    222e:	d0fc 000c      	addaw #12,%a0
    2232:	1210           	moveb %a0@,%d1
    2234:	1032 3807      	moveb %a2@(00000007,%d3:l),%d0
    2238:	b001           	cmpb %d1,%d0
    223a:	6306           	blss 0x2242
    223c:	4210           	clrb %a0@
    223e:	6000 0006      	braw 0x2246
    2242:	9200           	subb %d0,%d1
    2244:	1081           	moveb %d1,%a0@
    2246:	41ec 00d0      	lea %a4@(208),%a0
    224a:	4a28 000c      	tstb %a0@(12)
    224e:	660c           	bnes 0x225c
    2250:	117c 0006 0005 	moveb #6,%a0@(5)
    2256:	117c 0005 0021 	moveb #5,%a0@(33)
    225c:	3f3c 000b      	movew #11,%sp@-
    2260:	6100 41a6      	bsrw 0x6408
    2264:	6100 41bc      	bsrw 0x6422
    2268:	43fa ff41      	lea %pc@(0x21ab),%a1
    226c:	2f09           	movel %a1,%sp@-
    226e:	3f3c 0001      	movew #1,%sp@-
    2272:	3f3c 029c      	movew #668,%sp@-
    2276:	6100 2414      	bsrw 0x468c
    227a:	defc 000a      	addaw #10,%sp
    227e:	1605           	moveb %d5,%d3
    2280:	0c03 0008      	cmpib #8,%d3
    2284:	6266           	bhis 0x22ec
    2286:	4242           	clrw %d2
    2288:	41ec 0740      	lea %a4@(1856),%a0
    228c:	1403           	moveb %d3,%d2
    228e:	3202           	movew %d2,%d1
    2290:	c3fc 000c      	mulsw #12,%d1
    2294:	3002           	movew %d2,%d0
    2296:	5240           	addqw #1,%d0
    2298:	c1fc 000c      	mulsw #12,%d0
    229c:	11b0 0800 1800 	moveb %a0@(00000000,%d0:l),%a0@(00000000,%d1:l)
    22a2:	11b0 0801 1801 	moveb %a0@(00000001,%d0:l),%a0@(00000001,%d1:l)
    22a8:	11b0 0802 1802 	moveb %a0@(00000002,%d0:l),%a0@(00000002,%d1:l)
    22ae:	11b0 0803 1803 	moveb %a0@(00000003,%d0:l),%a0@(00000003,%d1:l)
    22b4:	11b0 0804 1804 	moveb %a0@(00000004,%d0:l),%a0@(00000004,%d1:l)
    22ba:	11b0 0805 1805 	moveb %a0@(00000005,%d0:l),%a0@(00000005,%d1:l)
    22c0:	11b0 0806 1806 	moveb %a0@(00000006,%d0:l),%a0@(00000006,%d1:l)
    22c6:	11b0 0807 1807 	moveb %a0@(00000007,%d0:l),%a0@(00000007,%d1:l)
    22cc:	11b0 0808 1808 	moveb %a0@(00000008,%d0:l),%a0@(00000008,%d1:l)
    22d2:	11b0 0809 1809 	moveb %a0@(00000009,%d0:l),%a0@(00000009,%d1:l)
    22d8:	11b0 080a 180a 	moveb %a0@(0000000a,%d0:l),%a0@(0000000a,%d1:l)
    22de:	11b0 080b 180b 	moveb %a0@(0000000b,%d0:l),%a0@(0000000b,%d1:l)
    22e4:	5203           	addqb #1,%d3
    22e6:	0c03 0008      	cmpib #8,%d3
    22ea:	639c           	blss 0x2288
    22ec:	41ec 0740      	lea %a4@(1856),%a0
    22f0:	117c 00fe 006c 	moveb #-2,%a0@(108)
    22f6:	41ec 000d      	lea %a4@(13),%a0
    22fa:	1010           	moveb %a0@,%d0
    22fc:	0c00 0001      	cmpib #1,%d0
    2300:	6304           	blss 0x2306
    2302:	5300           	subqb #1,%d0
    2304:	1080           	moveb %d0,%a0@
    2306:	6100 2946      	bsrw 0x4c4e
    230a:	6000 01de      	braw 0x24ea
    230e:	0c00 0006      	cmpib #6,%d0
    2312:	6638           	bnes 0x234c
    2314:	4aac 00c0      	tstl %a4@(192)
    2318:	6700 01d0      	beqw 0x24ea
    231c:	41ec 00d0      	lea %a4@(208),%a0
    2320:	1172 3807 0014 	moveb %a2@(00000007,%d3:l),%a0@(20)
    2326:	1172 380a 0015 	moveb %a2@(0000000a,%d3:l),%a0@(21)
    232c:	3f3c 0009      	movew #9,%sp@-
    2330:	6100 40d6      	bsrw 0x6408
    2334:	6100 40ec      	bsrw 0x6422
    2338:	544f           	addqw #2,%sp
    233a:	1032 3808      	moveb %a2@(00000008,%d3:l),%d0
    233e:	670c           	beqs 0x234c
    2340:	4267           	clrw %sp@-
    2342:	1f40 0001      	moveb %d0,%sp@(1)
    2346:	6100 36ee      	bsrw 0x5a36
    234a:	544f           	addqw #2,%sp
    234c:	3805           	movew %d5,%d4
    234e:	c9fc 000c      	mulsw #12,%d4
    2352:	45ec 0740      	lea %a4@(1856),%a2
    2356:	0c32 0006 4806 	cmpib #6,%a2@(00000006,%d4:l)
    235c:	670a           	beqs 0x2368
    235e:	0c32 0005 4805 	cmpib #5,%a2@(00000005,%d4:l)
    2364:	6600 0184      	bnew 0x24ea
    2368:	47ec 00c0      	lea %a4@(192),%a3
    236c:	4a93           	tstl %a3@
    236e:	6700 017a      	beqw 0x24ea
    2372:	2f2c 0084      	movel %a4@(132),%sp@-
    2376:	4e4f           	trap #15
    2378:	a1fd           	0120775
    237a:	2d48 fefc      	movel %a0,%fp@(-260)
    237e:	584f           	addqw #4,%sp
    2380:	4a32 4809      	tstb %a2@(00000009,%d4:l)
    2384:	6766           	beqs 0x23ec
    2386:	4267           	clrw %sp@-
    2388:	2f13           	movel %a3@,%sp@-
    238a:	4e4f           	trap #15
    238c:	a05c           	0120134
    238e:	2f08           	movel %a0,%sp@-
    2390:	4e4f           	trap #15
    2392:	a021           	0120041
    2394:	2608           	movel %a0,%d3
    2396:	2f03           	movel %d3,%sp@-
    2398:	4e4f           	trap #15
    239a:	a0ce           	0120316
    239c:	1832 4809      	moveb %a2@(00000009,%d4:l),%d4
    23a0:	0244 00ff      	andiw #255,%d4
    23a4:	d840           	addw %d0,%d4
    23a6:	2f03           	movel %d3,%sp@-
    23a8:	4e4f           	trap #15
    23aa:	a035           	0120065
    23ac:	4227           	clrb %sp@-
    23ae:	4267           	clrw %sp@-
    23b0:	2f13           	movel %a3@,%sp@-
    23b2:	4e4f           	trap #15
    23b4:	a05e           	0120136
    23b6:	3f04           	movew %d4,%sp@-
    23b8:	2f13           	movel %a3@,%sp@-
    23ba:	4e4f           	trap #15
    23bc:	a05c           	0120134
    23be:	2608           	movel %a0,%d3
    23c0:	defc 001c      	addaw #28,%sp
    23c4:	2e83           	movel %d3,%sp@
    23c6:	4e4f           	trap #15
    23c8:	a021           	0120041
    23ca:	4267           	clrw %sp@-
    23cc:	4267           	clrw %sp@-
    23ce:	2f08           	movel %a0,%sp@-
    23d0:	4e4f           	trap #15
    23d2:	a226           	0121046
    23d4:	2f03           	movel %d3,%sp@-
    23d6:	4e4f           	trap #15
    23d8:	a022           	0120042
    23da:	4227           	clrb %sp@-
    23dc:	3f04           	movew %d4,%sp@-
    23de:	2f13           	movel %a3@,%sp@-
    23e0:	4e4f           	trap #15
    23e2:	a05e           	0120136
    23e4:	defc 0018      	addaw #24,%sp
    23e8:	6000 0006      	braw 0x23f0
    23ec:	6100 ec24      	bsrw 0x1012
    23f0:	3805           	movew %d5,%d4
    23f2:	c9fc 000c      	mulsw #12,%d4
    23f6:	47ec 0740      	lea %a4@(1856),%a3
    23fa:	1033 4807      	moveb %a3@(00000007,%d4:l),%d0
    23fe:	6700 00ea      	beqw 0x24ea
    2402:	4267           	clrw %sp@-
    2404:	1f40 0001      	moveb %d0,%sp@(1)
    2408:	45ec 00c0      	lea %a4@(192),%a2
    240c:	2f12           	movel %a2@,%sp@-
    240e:	4e4f           	trap #15
    2410:	a05c           	0120134
    2412:	2008           	movel %a0,%d0
    2414:	5c4f           	addqw #6,%sp
    2416:	6730           	beqs 0x2448
    2418:	2f00           	movel %d0,%sp@-
    241a:	4e4f           	trap #15
    241c:	a021           	0120041
    241e:	2608           	movel %a0,%d3
    2420:	2f03           	movel %d3,%sp@-
    2422:	486e ff00      	pea %fp@(-256)
    2426:	4e4f           	trap #15
    2428:	a0c5           	0120305
    242a:	2f03           	movel %d3,%sp@-
    242c:	4e4f           	trap #15
    242e:	a035           	0120065
    2430:	4227           	clrb %sp@-
    2432:	4240           	clrw %d0
    2434:	1033 4807      	moveb %a3@(00000007,%d4:l),%d0
    2438:	3f00           	movew %d0,%sp@-
    243a:	2f12           	movel %a2@,%sp@-
    243c:	4e4f           	trap #15
    243e:	a05e           	0120136
    2440:	defc 0018      	addaw #24,%sp
    2444:	6000 0012      	braw 0x2458
    2448:	43fa fd6f      	lea %pc@(0x21b9),%a1
    244c:	2f09           	movel %a1,%sp@-
    244e:	486e ff00      	pea %fp@(-256)
    2452:	4e4f           	trap #15
    2454:	a0c5           	0120305
    2456:	504f           	addqw #8,%sp
    2458:	45ee ff00      	lea %fp@(-256),%a2
    245c:	2f0a           	movel %a2,%sp@-
    245e:	4e4f           	trap #15
    2460:	a0c7           	0120307
    2462:	3c00           	movew %d0,%d6
    2464:	4227           	clrb %sp@-
    2466:	4e4f           	trap #15
    2468:	a164           	0120544
    246a:	4244           	clrw %d4
    246c:	4245           	clrw %d5
    246e:	5c4f           	addqw #6,%sp
    2470:	bc45           	cmpw %d5,%d6
    2472:	6f24           	bles 0x2498
    2474:	3f3c 0078      	movew #120,%sp@-
    2478:	2f0a           	movel %a2,%sp@-
    247a:	4e4f           	trap #15
    247c:	a2d9           	0121331
    247e:	3600           	movew %d0,%d3
    2480:	3f03           	movew %d3,%sp@-
    2482:	2f0a           	movel %a2,%sp@-
    2484:	4e4f           	trap #15
    2486:	a16b           	0120553
    2488:	d843           	addw %d3,%d4
    248a:	5245           	addqw #1,%d5
    248c:	45f2 3000      	lea %a2@(00000000,%d3:w),%a2
    2490:	defc 000c      	addaw #12,%sp
    2494:	bc44           	cmpw %d4,%d6
    2496:	6edc           	bgts 0x2474
    2498:	4a45           	tstw %d5
    249a:	674e           	beqs 0x24ea
    249c:	7e14           	moveq #20,%d7
    249e:	7c14           	moveq #20,%d6
    24a0:	45ee ff00      	lea %fp@(-256),%a2
    24a4:	4244           	clrw %d4
    24a6:	ba44           	cmpw %d4,%d5
    24a8:	6f34           	bles 0x24de
    24aa:	3f3c 0078      	movew #120,%sp@-
    24ae:	2f0a           	movel %a2,%sp@-
    24b0:	4e4f           	trap #15
    24b2:	a2d9           	0121331
    24b4:	3600           	movew %d0,%d3
    24b6:	3f03           	movew %d3,%sp@-
    24b8:	2f0a           	movel %a2,%sp@-
    24ba:	4e4f           	trap #15
    24bc:	a16b           	0120553
    24be:	3f06           	movew %d6,%sp@-
    24c0:	3f07           	movew %d7,%sp@-
    24c2:	3f03           	movew %d3,%sp@-
    24c4:	2f0a           	movel %a2,%sp@-
    24c6:	4e4f           	trap #15
    24c8:	a220           	0121040
    24ca:	45f2 3000      	lea %a2@(00000000,%d3:w),%a2
    24ce:	0646 000b      	addiw #11,%d6
    24d2:	5447           	addqw #2,%d7
    24d4:	defc 0016      	addaw #22,%sp
    24d8:	5244           	addqw #1,%d4
    24da:	ba44           	cmpw %d4,%d5
    24dc:	6ecc           	bgts 0x24aa
    24de:	2f2e fefc      	movel %fp@(-260),%sp@-
    24e2:	4e4f           	trap #15
    24e4:	a1fd           	0120775
    24e6:	6100 0fe6      	bsrw 0x34ce
    24ea:	4cee 0cf8 fee0 	moveml %fp@(-288),%d3-%d7/%a2-%a3
    24f0:	4e5e           	unlk %fp
    24f2:	4e75           	rts
    24f4:	4e56 0000      	linkw %fp,#0
    24f8:	2f03           	movel %d3,%sp@-
    24fa:	226e 0008      	moveal %fp@(8),%a1
    24fe:	0c29 0003 0006 	cmpib #3,%a1@(6)
    2504:	6776           	beqs 0x257c
    2506:	3229 0002      	movew %a1@(2),%d1
    250a:	3001           	movew %d1,%d0
    250c:	0640 0020      	addiw #32,%d0
    2510:	6a06           	bpls 0x2518
    2512:	3001           	movew %d1,%d0
    2514:	0640 002f      	addiw #47,%d0
    2518:	3600           	movew %d0,%d3
    251a:	e843           	asrw #4,%d3
    251c:	3211           	movew %a1@,%d1
    251e:	3001           	movew %d1,%d0
    2520:	0640 0010      	addiw #16,%d0
    2524:	6a06           	bpls 0x252c
    2526:	3001           	movew %d1,%d0
    2528:	0640 001f      	addiw #31,%d0
    252c:	3400           	movew %d0,%d2
    252e:	e842           	asrw #4,%d2
    2530:	0c42 0009      	cmpiw #9,%d2
    2534:	6f02           	bles 0x2538
    2536:	7409           	moveq #9,%d2
    2538:	0c43 0009      	cmpiw #9,%d3
    253c:	6f02           	bles 0x2540
    253e:	7609           	moveq #9,%d3
    2540:	3002           	movew %d2,%d0
    2542:	48c0           	extl %d0
    2544:	41ec 0100      	lea %a4@(256),%a0
    2548:	2200           	movel %d0,%d1
    254a:	e781           	asll #3,%d1
    254c:	d280           	addl %d0,%d1
    254e:	d280           	addl %d0,%d1
    2550:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    2554:	1030 3000      	moveb %a0@(00000000,%d3:w),%d0
    2558:	0c00 0015      	cmpib #21,%d0
    255c:	6324           	blss 0x2582
    255e:	0c00 0023      	cmpib #35,%d0
    2562:	671e           	beqs 0x2582
    2564:	0c00 0026      	cmpib #38,%d0
    2568:	6718           	beqs 0x2582
    256a:	4a29 001f      	tstb %a1@(31)
    256e:	6612           	bnes 0x2582
    2570:	137c 000d 001f 	moveb #13,%a1@(31)
    2576:	137c 0009 0005 	moveb #9,%a1@(5)
    257c:	4240           	clrw %d0
    257e:	6000 00a6      	braw 0x2626
    2582:	0c29 0001 0008 	cmpib #1,%a1@(8)
    2588:	6612           	bnes 0x259c
    258a:	3211           	movew %a1@,%d1
    258c:	3001           	movew %d1,%d0
    258e:	5040           	addqw #8,%d0
    2590:	6a1a           	bpls 0x25ac
    2592:	3001           	movew %d1,%d0
    2594:	0640 0017      	addiw #23,%d0
    2598:	6000 0012      	braw 0x25ac
    259c:	3211           	movew %a1@,%d1
    259e:	3001           	movew %d1,%d0
    25a0:	0640 0018      	addiw #24,%d0
    25a4:	6a06           	bpls 0x25ac
    25a6:	3001           	movew %d1,%d0
    25a8:	0640 0027      	addiw #39,%d0
    25ac:	3400           	movew %d0,%d2
    25ae:	e842           	asrw #4,%d2
    25b0:	0c42 0009      	cmpiw #9,%d2
    25b4:	6f02           	bles 0x25b8
    25b6:	7409           	moveq #9,%d2
    25b8:	3002           	movew %d2,%d0
    25ba:	48c0           	extl %d0
    25bc:	41ec 0100      	lea %a4@(256),%a0
    25c0:	2200           	movel %d0,%d1
    25c2:	e781           	asll #3,%d1
    25c4:	d280           	addl %d0,%d1
    25c6:	d280           	addl %d0,%d1
    25c8:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    25cc:	1030 3000      	moveb %a0@(00000000,%d3:w),%d0
    25d0:	0c00 0008      	cmpib #8,%d0
    25d4:	6620           	bnes 0x25f6
    25d6:	137c 0003 0006 	moveb #3,%a1@(6)
    25dc:	4229 0005      	clrb %a1@(5)
    25e0:	3002           	movew %d2,%d0
    25e2:	e940           	aslw #4,%d0
    25e4:	5f40           	subqw #7,%d0
    25e6:	3280           	movew %d0,%a1@
    25e8:	0669 000a 0002 	addiw #10,%a1@(2)
    25ee:	4229 0008      	clrb %a1@(8)
    25f2:	6000 0028      	braw 0x261c
    25f6:	0c00 0002      	cmpib #2,%d0
    25fa:	6680           	bnes 0x257c
    25fc:	137c 0003 0006 	moveb #3,%a1@(6)
    2602:	4229 0005      	clrb %a1@(5)
    2606:	3002           	movew %d2,%d0
    2608:	e940           	aslw #4,%d0
    260a:	0640 fff6      	addiw #-10,%d0
    260e:	3280           	movew %d0,%a1@
    2610:	0669 000a 0002 	addiw #10,%a1@(2)
    2616:	137c 0001 0008 	moveb #1,%a1@(8)
    261c:	4229 0009      	clrb %a1@(9)
    2620:	4229 001f      	clrb %a1@(31)
    2624:	7001           	moveq #1,%d0
    2626:	262e fffc      	movel %fp@(-4),%d3
    262a:	4e5e           	unlk %fp
    262c:	4e75           	rts
    262e:	4e56 0000      	linkw %fp,#0
    2632:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    2636:	246e 0008      	moveal %fp@(8),%a2
    263a:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    2640:	6628           	bnes 0x266a
    2642:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    2648:	660c           	bnes 0x2656
    264a:	0c2a 0001 0009 	cmpib #1,%a2@(9)
    2650:	670e           	beqs 0x2660
    2652:	6000 0154      	braw 0x27a8
    2656:	0c2a 0001 0009 	cmpib #1,%a2@(9)
    265c:	6700 014a      	beqw 0x27a8
    2660:	2f0a           	movel %a2,%sp@-
    2662:	6100 06f0      	bsrw 0x2d54
    2666:	6000 0200      	braw 0x2868
    266a:	4a2c 000f      	tstb %a4@(15)
    266e:	6750           	beqs 0x26c0
    2670:	41ec 00d0      	lea %a4@(208),%a0
    2674:	47ec 0100      	lea %a4@(256),%a3
    2678:	200b           	movel %a3,%d0
    267a:	2240           	moveal %d0,%a1
    267c:	d2fc 0104      	addaw #260,%a1
    2680:	3210           	movew %a0@,%d1
    2682:	3411           	movew %a1@,%d2
    2684:	3001           	movew %d1,%d0
    2686:	9042           	subw %d2,%d0
    2688:	6a02           	bpls 0x268c
    268a:	4440           	negw %d0
    268c:	0c40 003b      	cmpiw #59,%d0
    2690:	6e2e           	bgts 0x26c0
    2692:	1a28 0006      	moveb %a0@(6),%d5
    2696:	ba29 0006      	cmpb %a1@(6),%d5
    269a:	6624           	bnes 0x26c0
    269c:	3a29 0002      	movew %a1@(2),%d5
    26a0:	ba68 0002      	cmpw %a0@(2),%d5
    26a4:	661a           	bnes 0x26c0
    26a6:	3012           	movew %a2@,%d0
    26a8:	b240           	cmpw %d0,%d1
    26aa:	6d14           	blts 0x26c0
    26ac:	b440           	cmpw %d0,%d2
    26ae:	6d10           	blts 0x26c0
    26b0:	4a2a 0008      	tstb %a2@(8)
    26b4:	660a           	bnes 0x26c0
    26b6:	157c 0001 0020 	moveb #1,%a2@(32)
    26bc:	6000 0006      	braw 0x26c4
    26c0:	422a 0020      	clrb %a2@(32)
    26c4:	4a2a 0008      	tstb %a2@(8)
    26c8:	661e           	bnes 0x26e8
    26ca:	4a2a 0020      	tstb %a2@(32)
    26ce:	6618           	bnes 0x26e8
    26d0:	4a2a 001f      	tstb %a2@(31)
    26d4:	6600 0192      	bnew 0x2868
    26d8:	157c 0001 0008 	moveb #1,%a2@(8)
    26de:	422a 0005      	clrb %a2@(5)
    26e2:	5952           	subqw #4,%a2@
    26e4:	6000 0182      	braw 0x2868
    26e8:	0c52 fff0      	cmpiw #-16,%a2@
    26ec:	6f00 0154      	blew 0x2842
    26f0:	2f0a           	movel %a2,%sp@-
    26f2:	6100 fe00      	bsrw 0x24f4
    26f6:	584f           	addqw #4,%sp
    26f8:	4a00           	tstb %d0
    26fa:	6600 016c      	bnew 0x2868
    26fe:	322a 0002      	movew %a2@(2),%d1
    2702:	3001           	movew %d1,%d0
    2704:	0640 0020      	addiw #32,%d0
    2708:	6a06           	bpls 0x2710
    270a:	3001           	movew %d1,%d0
    270c:	0640 002f      	addiw #47,%d0
    2710:	3800           	movew %d0,%d4
    2712:	e844           	asrw #4,%d4
    2714:	3012           	movew %a2@,%d0
    2716:	6c04           	bges 0x271c
    2718:	0640 000f      	addiw #15,%d0
    271c:	3400           	movew %d0,%d2
    271e:	e842           	asrw #4,%d2
    2720:	76f0           	moveq #-16,%d3
    2722:	3002           	movew %d2,%d0
    2724:	48c0           	extl %d0
    2726:	41ec 0100      	lea %a4@(256),%a0
    272a:	2200           	movel %d0,%d1
    272c:	e781           	asll #3,%d1
    272e:	d280           	addl %d0,%d1
    2730:	d280           	addl %d0,%d1
    2732:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    2736:	1030 4000      	moveb %a0@(00000000,%d4:w),%d0
    273a:	0c00 000d      	cmpib #13,%d0
    273e:	660c           	bnes 0x274c
    2740:	3002           	movew %d2,%d0
    2742:	e940           	aslw #4,%d0
    2744:	3600           	movew %d0,%d3
    2746:	5e43           	addqw #7,%d3
    2748:	6000 002c      	braw 0x2776
    274c:	0c00 0011      	cmpib #17,%d0
    2750:	660e           	bnes 0x2760
    2752:	3002           	movew %d2,%d0
    2754:	e940           	aslw #4,%d0
    2756:	3600           	movew %d0,%d3
    2758:	0643 000a      	addiw #10,%d3
    275c:	6000 0018      	braw 0x2776
    2760:	0c00 001f      	cmpib #31,%d0
    2764:	6706           	beqs 0x276c
    2766:	0c00 000f      	cmpib #15,%d0
    276a:	660a           	bnes 0x2776
    276c:	3002           	movew %d2,%d0
    276e:	e940           	aslw #4,%d0
    2770:	3600           	movew %d0,%d3
    2772:	0643 0010      	addiw #16,%d3
    2776:	b652           	cmpw %a2@,%d3
    2778:	6f12           	bles 0x278c
    277a:	3483           	movew %d3,%a2@
    277c:	4a2a 001f      	tstb %a2@(31)
    2780:	6600 00e6      	bnew 0x2868
    2784:	422a 0005      	clrb %a2@(5)
    2788:	6000 00cc      	braw 0x2856
    278c:	4e4f           	trap #15
    278e:	a2a2           	0121242
    2790:	2200           	movel %d0,%d1
    2792:	0801 0002      	btst #2,%d1
    2796:	671a           	beqs 0x27b2
    2798:	4a2a 001f      	tstb %a2@(31)
    279c:	6614           	bnes 0x27b2
    279e:	47ec 00d0      	lea %a4@(208),%a3
    27a2:	200b           	movel %a3,%d0
    27a4:	b08a           	cmpl %a2,%d0
    27a6:	660a           	bnes 0x27b2
    27a8:	2f0a           	movel %a2,%sp@-
    27aa:	6100 07e4      	bsrw 0x2f90
    27ae:	6000 00b8      	braw 0x2868
    27b2:	0801 0001      	btst #1,%d1
    27b6:	670a           	beqs 0x27c2
    27b8:	47ec 00d0      	lea %a4@(208),%a3
    27bc:	200b           	movel %a3,%d0
    27be:	b08a           	cmpl %a2,%d0
    27c0:	6706           	beqs 0x27c8
    27c2:	4a2a 001f      	tstb %a2@(31)
    27c6:	6738           	beqs 0x2800
    27c8:	4a2a 001f      	tstb %a2@(31)
    27cc:	6710           	beqs 0x27de
    27ce:	157c 0001 000a 	moveb #1,%a2@(10)
    27d4:	2f0a           	movel %a2,%sp@-
    27d6:	6100 0460      	bsrw 0x2c38
    27da:	6000 008c      	braw 0x2868
    27de:	4a2a 0021      	tstb %a2@(33)
    27e2:	6620           	bnes 0x2804
    27e4:	157c 0001 001f 	moveb #1,%a2@(31)
    27ea:	157c 0001 000a 	moveb #1,%a2@(10)
    27f0:	157c 0009 0005 	moveb #9,%a2@(5)
    27f6:	576a 0002      	subqw #3,%a2@(2)
    27fa:	5952           	subqw #4,%a2@
    27fc:	6000 006a      	braw 0x2868
    2800:	422a 0021      	clrb %a2@(33)
    2804:	102a 0005      	moveb %a2@(5),%d0
    2808:	6706           	beqs 0x2810
    280a:	0c00 0003      	cmpib #3,%d0
    280e:	6608           	bnes 0x2818
    2810:	3f3c 0001      	movew #1,%sp@-
    2814:	6100 3bf2      	bsrw 0x6408
    2818:	5952           	subqw #4,%a2@
    281a:	4a2a 0020      	tstb %a2@(32)
    281e:	671a           	beqs 0x283a
    2820:	102a 0005      	moveb %a2@(5),%d0
    2824:	660a           	bnes 0x2830
    2826:	157c 0005 0005 	moveb #5,%a2@(5)
    282c:	6000 0028      	braw 0x2856
    2830:	5300           	subqb #1,%d0
    2832:	1540 0005      	moveb %d0,%a2@(5)
    2836:	6000 001e      	braw 0x2856
    283a:	522a 0005      	addqb #1,%a2@(5)
    283e:	6000 0016      	braw 0x2856
    2842:	34bc 008c      	movew #140,%a2@
    2846:	41ec 0100      	lea %a4@(256),%a0
    284a:	4240           	clrw %d0
    284c:	1028 0082      	moveb %a0@(130),%d0
    2850:	3f00           	movew %d0,%sp@-
    2852:	6100 ee7e      	bsrw 0x16d2
    2856:	4a2a 001f      	tstb %a2@(31)
    285a:	660c           	bnes 0x2868
    285c:	0c2a 0005 0005 	cmpib #5,%a2@(5)
    2862:	6304           	blss 0x2868
    2864:	422a 0005      	clrb %a2@(5)
    2868:	4cee 0c38 ffec 	moveml %fp@(-20),%d3-%d5/%a2-%a3
    286e:	4e5e           	unlk %fp
    2870:	4e75           	rts
    2872:	4e56 0000      	linkw %fp,#0
    2876:	2f04           	movel %d4,%sp@-
    2878:	2f03           	movel %d3,%sp@-
    287a:	226e 0008      	moveal %fp@(8),%a1
    287e:	3229 0002      	movew %a1@(2),%d1
    2882:	3001           	movew %d1,%d0
    2884:	0640 0020      	addiw #32,%d0
    2888:	6a06           	bpls 0x2890
    288a:	3001           	movew %d1,%d0
    288c:	0640 002f      	addiw #47,%d0
    2890:	3800           	movew %d0,%d4
    2892:	e844           	asrw #4,%d4
    2894:	3011           	movew %a1@,%d0
    2896:	6c04           	bges 0x289c
    2898:	0640 000f      	addiw #15,%d0
    289c:	3400           	movew %d0,%d2
    289e:	e842           	asrw #4,%d2
    28a0:	76f0           	moveq #-16,%d3
    28a2:	3002           	movew %d2,%d0
    28a4:	48c0           	extl %d0
    28a6:	41ec 0100      	lea %a4@(256),%a0
    28aa:	2200           	movel %d0,%d1
    28ac:	e781           	asll #3,%d1
    28ae:	d280           	addl %d0,%d1
    28b0:	d280           	addl %d0,%d1
    28b2:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    28b6:	1030 4000      	moveb %a0@(00000000,%d4:w),%d0
    28ba:	0c00 000d      	cmpib #13,%d0
    28be:	660c           	bnes 0x28cc
    28c0:	3002           	movew %d2,%d0
    28c2:	e940           	aslw #4,%d0
    28c4:	3600           	movew %d0,%d3
    28c6:	5043           	addqw #8,%d3
    28c8:	6000 002a      	braw 0x28f4
    28cc:	0c00 0011      	cmpib #17,%d0
    28d0:	660c           	bnes 0x28de
    28d2:	3002           	movew %d2,%d0
    28d4:	e940           	aslw #4,%d0
    28d6:	3600           	movew %d0,%d3
    28d8:	5e43           	addqw #7,%d3
    28da:	6000 0018      	braw 0x28f4
    28de:	0c00 001f      	cmpib #31,%d0
    28e2:	6706           	beqs 0x28ea
    28e4:	0c00 000f      	cmpib #15,%d0
    28e8:	660a           	bnes 0x28f4
    28ea:	3002           	movew %d2,%d0
    28ec:	e940           	aslw #4,%d0
    28ee:	3600           	movew %d0,%d3
    28f0:	0643 0010      	addiw #16,%d3
    28f4:	b651           	cmpw %a1@,%d3
    28f6:	6e06           	bgts 0x28fe
    28f8:	4240           	clrw %d0
    28fa:	6000 000a      	braw 0x2906
    28fe:	3283           	movew %d3,%a1@
    2900:	4229 0005      	clrb %a1@(5)
    2904:	7001           	moveq #1,%d0
    2906:	262e fff8      	movel %fp@(-8),%d3
    290a:	282e fffc      	movel %fp@(-4),%d4
    290e:	4e5e           	unlk %fp
    2910:	4e75           	rts
    2912:	4e56 0000      	linkw %fp,#0
    2916:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    291a:	246e 0008      	moveal %fp@(8),%a2
    291e:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    2924:	662a           	bnes 0x2950
    2926:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    292c:	660e           	bnes 0x293c
    292e:	0c2a 0001 0009 	cmpib #1,%a2@(9)
    2934:	6700 017a      	beqw 0x2ab0
    2938:	6000 000c      	braw 0x2946
    293c:	0c2a 0001 0009 	cmpib #1,%a2@(9)
    2942:	6600 016c      	bnew 0x2ab0
    2946:	2f0a           	movel %a2,%sp@-
    2948:	6100 040a      	bsrw 0x2d54
    294c:	6000 021e      	braw 0x2b6c
    2950:	4a2c 000f      	tstb %a4@(15)
    2954:	6752           	beqs 0x29a8
    2956:	41ec 00d0      	lea %a4@(208),%a0
    295a:	47ec 0100      	lea %a4@(256),%a3
    295e:	200b           	movel %a3,%d0
    2960:	2240           	moveal %d0,%a1
    2962:	d2fc 0104      	addaw #260,%a1
    2966:	3210           	movew %a0@,%d1
    2968:	3411           	movew %a1@,%d2
    296a:	3001           	movew %d1,%d0
    296c:	9042           	subw %d2,%d0
    296e:	6a02           	bpls 0x2972
    2970:	4440           	negw %d0
    2972:	0c40 003b      	cmpiw #59,%d0
    2976:	6e30           	bgts 0x29a8
    2978:	1a28 0006      	moveb %a0@(6),%d5
    297c:	ba29 0006      	cmpb %a1@(6),%d5
    2980:	6626           	bnes 0x29a8
    2982:	3a29 0002      	movew %a1@(2),%d5
    2986:	ba68 0002      	cmpw %a0@(2),%d5
    298a:	661c           	bnes 0x29a8
    298c:	3012           	movew %a2@,%d0
    298e:	b240           	cmpw %d0,%d1
    2990:	6e16           	bgts 0x29a8
    2992:	b440           	cmpw %d0,%d2
    2994:	6e12           	bgts 0x29a8
    2996:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    299c:	660a           	bnes 0x29a8
    299e:	157c 0001 0020 	moveb #1,%a2@(32)
    29a4:	6000 0006      	braw 0x29ac
    29a8:	422a 0020      	clrb %a2@(32)
    29ac:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    29b2:	661c           	bnes 0x29d0
    29b4:	4a2a 0020      	tstb %a2@(32)
    29b8:	6616           	bnes 0x29d0
    29ba:	4a2a 001f      	tstb %a2@(31)
    29be:	6600 01ac      	bnew 0x2b6c
    29c2:	422a 0008      	clrb %a2@(8)
    29c6:	422a 0005      	clrb %a2@(5)
    29ca:	5852           	addqw #4,%a2@
    29cc:	6000 019e      	braw 0x2b6c
    29d0:	0c52 008f      	cmpiw #143,%a2@
    29d4:	6e00 0170      	bgtw 0x2b46
    29d8:	2f0a           	movel %a2,%sp@-
    29da:	6100 fb18      	bsrw 0x24f4
    29de:	584f           	addqw #4,%sp
    29e0:	4a00           	tstb %d0
    29e2:	6600 0188      	bnew 0x2b6c
    29e6:	322a 0002      	movew %a2@(2),%d1
    29ea:	3001           	movew %d1,%d0
    29ec:	0640 0020      	addiw #32,%d0
    29f0:	6a06           	bpls 0x29f8
    29f2:	3001           	movew %d1,%d0
    29f4:	0640 002f      	addiw #47,%d0
    29f8:	3800           	movew %d0,%d4
    29fa:	e844           	asrw #4,%d4
    29fc:	3212           	movew %a2@,%d1
    29fe:	3001           	movew %d1,%d0
    2a00:	0640 0010      	addiw #16,%d0
    2a04:	6a06           	bpls 0x2a0c
    2a06:	3001           	movew %d1,%d0
    2a08:	0640 001f      	addiw #31,%d0
    2a0c:	3600           	movew %d0,%d3
    2a0e:	e843           	asrw #4,%d3
    2a10:	343c 0090      	movew #144,%d2
    2a14:	3003           	movew %d3,%d0
    2a16:	48c0           	extl %d0
    2a18:	41ec 0100      	lea %a4@(256),%a0
    2a1c:	2200           	movel %d0,%d1
    2a1e:	e781           	asll #3,%d1
    2a20:	d280           	addl %d0,%d1
    2a22:	d280           	addl %d0,%d1
    2a24:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    2a28:	1030 4000      	moveb %a0@(00000000,%d4:w),%d0
    2a2c:	0c00 000f      	cmpib #15,%d0
    2a30:	660e           	bnes 0x2a40
    2a32:	3003           	movew %d3,%d0
    2a34:	e940           	aslw #4,%d0
    2a36:	3400           	movew %d0,%d2
    2a38:	0642 ffed      	addiw #-19,%d2
    2a3c:	6000 0040      	braw 0x2a7e
    2a40:	0c00 0011      	cmpib #17,%d0
    2a44:	660e           	bnes 0x2a54
    2a46:	3003           	movew %d3,%d0
    2a48:	e940           	aslw #4,%d0
    2a4a:	3400           	movew %d0,%d2
    2a4c:	0642 ffe4      	addiw #-28,%d2
    2a50:	6000 002c      	braw 0x2a7e
    2a54:	0c00 0014      	cmpib #20,%d0
    2a58:	660e           	bnes 0x2a68
    2a5a:	3003           	movew %d3,%d0
    2a5c:	e940           	aslw #4,%d0
    2a5e:	3400           	movew %d0,%d2
    2a60:	0642 ffee      	addiw #-18,%d2
    2a64:	6000 0018      	braw 0x2a7e
    2a68:	0c00 001f      	cmpib #31,%d0
    2a6c:	6706           	beqs 0x2a74
    2a6e:	0c00 000d      	cmpib #13,%d0
    2a72:	660a           	bnes 0x2a7e
    2a74:	3003           	movew %d3,%d0
    2a76:	e940           	aslw #4,%d0
    2a78:	3400           	movew %d0,%d2
    2a7a:	0642 ffe0      	addiw #-32,%d2
    2a7e:	b452           	cmpw %a2@,%d2
    2a80:	6c12           	bges 0x2a94
    2a82:	3482           	movew %d2,%a2@
    2a84:	4a2a 001f      	tstb %a2@(31)
    2a88:	6600 00e2      	bnew 0x2b6c
    2a8c:	422a 0005      	clrb %a2@(5)
    2a90:	6000 00c8      	braw 0x2b5a
    2a94:	4e4f           	trap #15
    2a96:	a2a2           	0121242
    2a98:	2200           	movel %d0,%d1
    2a9a:	0801 0002      	btst #2,%d1
    2a9e:	671a           	beqs 0x2aba
    2aa0:	4a2a 001f      	tstb %a2@(31)
    2aa4:	6614           	bnes 0x2aba
    2aa6:	47ec 00d0      	lea %a4@(208),%a3
    2aaa:	200b           	movel %a3,%d0
    2aac:	b08a           	cmpl %a2,%d0
    2aae:	660a           	bnes 0x2aba
    2ab0:	2f0a           	movel %a2,%sp@-
    2ab2:	6100 04dc      	bsrw 0x2f90
    2ab6:	6000 00b4      	braw 0x2b6c
    2aba:	0801 0001      	btst #1,%d1
    2abe:	670a           	beqs 0x2aca
    2ac0:	47ec 00d0      	lea %a4@(208),%a3
    2ac4:	200b           	movel %a3,%d0
    2ac6:	b08a           	cmpl %a2,%d0
    2ac8:	6706           	beqs 0x2ad0
    2aca:	4a2a 001f      	tstb %a2@(31)
    2ace:	6734           	beqs 0x2b04
    2ad0:	4a2a 001f      	tstb %a2@(31)
    2ad4:	670e           	beqs 0x2ae4
    2ad6:	422a 000a      	clrb %a2@(10)
    2ada:	2f0a           	movel %a2,%sp@-
    2adc:	6100 015a      	bsrw 0x2c38
    2ae0:	6000 008a      	braw 0x2b6c
    2ae4:	4a2a 0021      	tstb %a2@(33)
    2ae8:	661e           	bnes 0x2b08
    2aea:	157c 0001 001f 	moveb #1,%a2@(31)
    2af0:	422a 000a      	clrb %a2@(10)
    2af4:	157c 0009 0005 	moveb #9,%a2@(5)
    2afa:	576a 0002      	subqw #3,%a2@(2)
    2afe:	5852           	addqw #4,%a2@
    2b00:	6000 006a      	braw 0x2b6c
    2b04:	422a 0021      	clrb %a2@(33)
    2b08:	102a 0005      	moveb %a2@(5),%d0
    2b0c:	6706           	beqs 0x2b14
    2b0e:	0c00 0003      	cmpib #3,%d0
    2b12:	6608           	bnes 0x2b1c
    2b14:	3f3c 0001      	movew #1,%sp@-
    2b18:	6100 38ee      	bsrw 0x6408
    2b1c:	5852           	addqw #4,%a2@
    2b1e:	4a2a 0020      	tstb %a2@(32)
    2b22:	671a           	beqs 0x2b3e
    2b24:	102a 0005      	moveb %a2@(5),%d0
    2b28:	660a           	bnes 0x2b34
    2b2a:	157c 0005 0005 	moveb #5,%a2@(5)
    2b30:	6000 0028      	braw 0x2b5a
    2b34:	5300           	subqb #1,%d0
    2b36:	1540 0005      	moveb %d0,%a2@(5)
    2b3a:	6000 001e      	braw 0x2b5a
    2b3e:	522a 0005      	addqb #1,%a2@(5)
    2b42:	6000 0016      	braw 0x2b5a
    2b46:	34bc fff6      	movew #-10,%a2@
    2b4a:	41ec 0100      	lea %a4@(256),%a0
    2b4e:	4240           	clrw %d0
    2b50:	1028 0083      	moveb %a0@(131),%d0
    2b54:	3f00           	movew %d0,%sp@-
    2b56:	6100 eb7a      	bsrw 0x16d2
    2b5a:	4a2a 001f      	tstb %a2@(31)
    2b5e:	660c           	bnes 0x2b6c
    2b60:	0c2a 0005 0005 	cmpib #5,%a2@(5)
    2b66:	6304           	blss 0x2b6c
    2b68:	422a 0005      	clrb %a2@(5)
    2b6c:	4cee 0c38 ffec 	moveml %fp@(-20),%d3-%d5/%a2-%a3
    2b72:	4e5e           	unlk %fp
    2b74:	4e75           	rts
    2b76:	4e56 0000      	linkw %fp,#0
    2b7a:	2f04           	movel %d4,%sp@-
    2b7c:	2f03           	movel %d3,%sp@-
    2b7e:	226e 0008      	moveal %fp@(8),%a1
    2b82:	3229 0002      	movew %a1@(2),%d1
    2b86:	3001           	movew %d1,%d0
    2b88:	0640 0020      	addiw #32,%d0
    2b8c:	6a06           	bpls 0x2b94
    2b8e:	3001           	movew %d1,%d0
    2b90:	0640 002f      	addiw #47,%d0
    2b94:	3800           	movew %d0,%d4
    2b96:	e844           	asrw #4,%d4
    2b98:	3211           	movew %a1@,%d1
    2b9a:	3001           	movew %d1,%d0
    2b9c:	0640 0010      	addiw #16,%d0
    2ba0:	6a06           	bpls 0x2ba8
    2ba2:	3001           	movew %d1,%d0
    2ba4:	0640 001f      	addiw #31,%d0
    2ba8:	3600           	movew %d0,%d3
    2baa:	e843           	asrw #4,%d3
    2bac:	343c 0090      	movew #144,%d2
    2bb0:	3003           	movew %d3,%d0
    2bb2:	48c0           	extl %d0
    2bb4:	41ec 0100      	lea %a4@(256),%a0
    2bb8:	2200           	movel %d0,%d1
    2bba:	e781           	asll #3,%d1
    2bbc:	d280           	addl %d0,%d1
    2bbe:	d280           	addl %d0,%d1
    2bc0:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    2bc4:	1030 4000      	moveb %a0@(00000000,%d4:w),%d0
    2bc8:	0c00 000f      	cmpib #15,%d0
    2bcc:	660e           	bnes 0x2bdc
    2bce:	3003           	movew %d3,%d0
    2bd0:	e940           	aslw #4,%d0
    2bd2:	3400           	movew %d0,%d2
    2bd4:	0642 ffed      	addiw #-19,%d2
    2bd8:	6000 0040      	braw 0x2c1a
    2bdc:	0c00 0011      	cmpib #17,%d0
    2be0:	660e           	bnes 0x2bf0
    2be2:	3003           	movew %d3,%d0
    2be4:	e940           	aslw #4,%d0
    2be6:	3400           	movew %d0,%d2
    2be8:	0642 ffe4      	addiw #-28,%d2
    2bec:	6000 002c      	braw 0x2c1a
    2bf0:	0c00 0014      	cmpib #20,%d0
    2bf4:	660e           	bnes 0x2c04
    2bf6:	3003           	movew %d3,%d0
    2bf8:	e940           	aslw #4,%d0
    2bfa:	3400           	movew %d0,%d2
    2bfc:	0642 ffe8      	addiw #-24,%d2
    2c00:	6000 0018      	braw 0x2c1a
    2c04:	0c00 001f      	cmpib #31,%d0
    2c08:	6706           	beqs 0x2c10
    2c0a:	0c00 000d      	cmpib #13,%d0
    2c0e:	660a           	bnes 0x2c1a
    2c10:	3003           	movew %d3,%d0
    2c12:	e940           	aslw #4,%d0
    2c14:	3400           	movew %d0,%d2
    2c16:	0642 ffe0      	addiw #-32,%d2
    2c1a:	b451           	cmpw %a1@,%d2
    2c1c:	6d06           	blts 0x2c24
    2c1e:	4240           	clrw %d0
    2c20:	6000 000a      	braw 0x2c2c
    2c24:	3282           	movew %d2,%a1@
    2c26:	4229 0005      	clrb %a1@(5)
    2c2a:	7001           	moveq #1,%d0
    2c2c:	262e fff8      	movel %fp@(-8),%d3
    2c30:	282e fffc      	movel %fp@(-4),%d4
    2c34:	4e5e           	unlk %fp
    2c36:	4e75           	rts
    2c38:	4e56 0000      	linkw %fp,#0
    2c3c:	2f0a           	movel %a2,%sp@-
    2c3e:	246e 0008      	moveal %fp@(8),%a2
    2c42:	4240           	clrw %d0
    2c44:	102a 001f      	moveb %a2@(31),%d0
    2c48:	5340           	subqw #1,%d0
    2c4a:	0c40 000c      	cmpiw #12,%d0
    2c4e:	6200 008a      	bhiw 0x2cda
    2c52:	0280 0000 ffff 	andil #65535,%d0
    2c58:	41fa 000c      	lea %pc@(0x2c66),%a0
    2c5c:	d080           	addl %d0,%d0
    2c5e:	3030 0800      	movew %a0@(00000000,%d0:l),%d0
    2c62:	4efb 0002      	jmp %pc@(0x2c66,%d0:w)
    2c66:	001a 002c      	orib #44,%a2@+
    2c6a:	0034 003c 0044 	orib #60,%a4@(00000044,%d0:w)
    2c70:	004c 004c      	oriw #76,%a4
    2c74:	0074 0074 0074 	oriw #116,%a4@(00000074,%d0:w)
    2c7a:	0074 0074 0054 	oriw #116,%a4@(00000054,%d0:w)
    2c80:	3f3c 0003      	movew #3,%sp@-
    2c84:	6100 3782      	bsrw 0x6408
    2c88:	576a 0002      	subqw #3,%a2@(2)
    2c8c:	544f           	addqw #2,%sp
    2c8e:	6000 004a      	braw 0x2cda
    2c92:	556a 0002      	subqw #2,%a2@(2)
    2c96:	6000 0042      	braw 0x2cda
    2c9a:	536a 0002      	subqw #1,%a2@(2)
    2c9e:	6000 003a      	braw 0x2cda
    2ca2:	526a 0002      	addqw #1,%a2@(2)
    2ca6:	6000 0032      	braw 0x2cda
    2caa:	546a 0002      	addqw #2,%a2@(2)
    2cae:	6000 002a      	braw 0x2cda
    2cb2:	566a 0002      	addqw #3,%a2@(2)
    2cb6:	6000 0022      	braw 0x2cda
    2cba:	5c6a 0002      	addqw #6,%a2@(2)
    2cbe:	0c2a 0001 000a 	cmpib #1,%a2@(10)
    2cc4:	6602           	bnes 0x2cc8
    2cc6:	5752           	subqw #3,%a2@
    2cc8:	4a2a 000a      	tstb %a2@(10)
    2ccc:	6602           	bnes 0x2cd0
    2cce:	5652           	addqw #3,%a2@
    2cd0:	2f0a           	movel %a2,%sp@-
    2cd2:	6100 397e      	bsrw 0x6652
    2cd6:	6000 0074      	braw 0x2d4c
    2cda:	0c2a 0001 000a 	cmpib #1,%a2@(10)
    2ce0:	660e           	bnes 0x2cf0
    2ce2:	5752           	subqw #3,%a2@
    2ce4:	2f0a           	movel %a2,%sp@-
    2ce6:	6100 fb8a      	bsrw 0x2872
    2cea:	584f           	addqw #4,%sp
    2cec:	4a00           	tstb %d0
    2cee:	6614           	bnes 0x2d04
    2cf0:	4a2a 000a      	tstb %a2@(10)
    2cf4:	6622           	bnes 0x2d18
    2cf6:	5652           	addqw #3,%a2@
    2cf8:	2f0a           	movel %a2,%sp@-
    2cfa:	6100 fe7a      	bsrw 0x2b76
    2cfe:	584f           	addqw #4,%sp
    2d00:	4a00           	tstb %d0
    2d02:	6714           	beqs 0x2d18
    2d04:	422a 001f      	clrb %a2@(31)
    2d08:	157c 0003 000a 	moveb #3,%a2@(10)
    2d0e:	2f0a           	movel %a2,%sp@-
    2d10:	6100 0496      	bsrw 0x31a8
    2d14:	6000 0036      	braw 0x2d4c
    2d18:	102a 001f      	moveb %a2@(31),%d0
    2d1c:	1200           	moveb %d0,%d1
    2d1e:	5201           	addqb #1,%d1
    2d20:	1541 001f      	moveb %d1,%a2@(31)
    2d24:	0c00 0007      	cmpib #7,%d0
    2d28:	6622           	bnes 0x2d4c
    2d2a:	422a 001f      	clrb %a2@(31)
    2d2e:	422a 0005      	clrb %a2@(5)
    2d32:	157c 0001 0021 	moveb #1,%a2@(33)
    2d38:	2f0a           	movel %a2,%sp@-
    2d3a:	6100 f7b8      	bsrw 0x24f4
    2d3e:	0c2a 000d 001f 	cmpib #13,%a2@(31)
    2d44:	6706           	beqs 0x2d4c
    2d46:	157c 0003 000a 	moveb #3,%a2@(10)
    2d4c:	246e fffc      	moveal %fp@(-4),%a2
    2d50:	4e5e           	unlk %fp
    2d52:	4e75           	rts
    2d54:	4e56 0000      	linkw %fp,#0
    2d58:	48e7 1820      	moveml %d3-%d4/%a2,%sp@-
    2d5c:	246e 0008      	moveal %fp@(8),%a2
    2d60:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    2d66:	6600 0128      	bnew 0x2e90
    2d6a:	4a2a 0009      	tstb %a2@(9)
    2d6e:	662c           	bnes 0x2d9c
    2d70:	157c 0001 0009 	moveb #1,%a2@(9)
    2d76:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    2d7c:	660e           	bnes 0x2d8c
    2d7e:	422a 0008      	clrb %a2@(8)
    2d82:	5e52           	addqw #7,%a2@
    2d84:	556a 0002      	subqw #2,%a2@(2)
    2d88:	6000 01fc      	braw 0x2f86
    2d8c:	157c 0001 0008 	moveb #1,%a2@(8)
    2d92:	5f52           	subqw #7,%a2@
    2d94:	556a 0002      	subqw #2,%a2@(2)
    2d98:	6000 01ec      	braw 0x2f86
    2d9c:	102a 0005      	moveb %a2@(5),%d0
    2da0:	1800           	moveb %d0,%d4
    2da2:	5204           	addqb #1,%d4
    2da4:	1544 0005      	moveb %d4,%a2@(5)
    2da8:	5200           	addqb #1,%d0
    2daa:	0c00 0002      	cmpib #2,%d0
    2dae:	631a           	blss 0x2dca
    2db0:	422a 0005      	clrb %a2@(5)
    2db4:	4a2a 0008      	tstb %a2@(8)
    2db8:	6606           	bnes 0x2dc0
    2dba:	5452           	addqw #2,%a2@
    2dbc:	6000 0004      	braw 0x2dc2
    2dc0:	5552           	subqw #2,%a2@
    2dc2:	556a 0002      	subqw #2,%a2@(2)
    2dc6:	6000 0014      	braw 0x2ddc
    2dca:	4a2a 0008      	tstb %a2@(8)
    2dce:	6606           	bnes 0x2dd6
    2dd0:	5652           	addqw #3,%a2@
    2dd2:	6000 0004      	braw 0x2dd8
    2dd6:	5752           	subqw #3,%a2@
    2dd8:	576a 0002      	subqw #3,%a2@(2)
    2ddc:	4a2a 0005      	tstb %a2@(5)
    2de0:	6608           	bnes 0x2dea
    2de2:	3f3c 0001      	movew #1,%sp@-
    2de6:	6100 3620      	bsrw 0x6408
    2dea:	322a 0002      	movew %a2@(2),%d1
    2dee:	3001           	movew %d1,%d0
    2df0:	0640 0020      	addiw #32,%d0
    2df4:	6a06           	bpls 0x2dfc
    2df6:	3001           	movew %d1,%d0
    2df8:	0640 002f      	addiw #47,%d0
    2dfc:	3600           	movew %d0,%d3
    2dfe:	e843           	asrw #4,%d3
    2e00:	3212           	movew %a2@,%d1
    2e02:	3001           	movew %d1,%d0
    2e04:	0640 0010      	addiw #16,%d0
    2e08:	6a06           	bpls 0x2e10
    2e0a:	3001           	movew %d1,%d0
    2e0c:	0640 001f      	addiw #31,%d0
    2e10:	3400           	movew %d0,%d2
    2e12:	e842           	asrw #4,%d2
    2e14:	3002           	movew %d2,%d0
    2e16:	48c0           	extl %d0
    2e18:	41ec 0100      	lea %a4@(256),%a0
    2e1c:	2200           	movel %d0,%d1
    2e1e:	e781           	asll #3,%d1
    2e20:	d280           	addl %d0,%d1
    2e22:	d280           	addl %d0,%d1
    2e24:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    2e28:	1230 3000      	moveb %a0@(00000000,%d3:w),%d1
    2e2c:	6736           	beqs 0x2e64
    2e2e:	1001           	moveb %d1,%d0
    2e30:	5900           	subqb #4,%d0
    2e32:	0c00 0001      	cmpib #1,%d0
    2e36:	632c           	blss 0x2e64
    2e38:	1001           	moveb %d1,%d0
    2e3a:	0600 fff5      	addib #-11,%d0
    2e3e:	0c00 0001      	cmpib #1,%d0
    2e42:	6320           	blss 0x2e64
    2e44:	0c01 000d      	cmpib #13,%d1
    2e48:	671a           	beqs 0x2e64
    2e4a:	0c01 000f      	cmpib #15,%d1
    2e4e:	6714           	beqs 0x2e64
    2e50:	0c01 0013      	cmpib #19,%d1
    2e54:	670e           	beqs 0x2e64
    2e56:	0c01 0023      	cmpib #35,%d1
    2e5a:	6708           	beqs 0x2e64
    2e5c:	0c01 0026      	cmpib #38,%d1
    2e60:	6600 0124      	bnew 0x2f86
    2e64:	422a 0005      	clrb %a2@(5)
    2e68:	422a 0006      	clrb %a2@(6)
    2e6c:	157c 0001 0021 	moveb #1,%a2@(33)
    2e72:	3003           	movew %d3,%d0
    2e74:	e940           	aslw #4,%d0
    2e76:	0640 ffe9      	addiw #-23,%d0
    2e7a:	3540 0002      	movew %d0,%a2@(2)
    2e7e:	4a2a 0008      	tstb %a2@(8)
    2e82:	6606           	bnes 0x2e8a
    2e84:	5452           	addqw #2,%a2@
    2e86:	6000 00fe      	braw 0x2f86
    2e8a:	5552           	subqw #2,%a2@
    2e8c:	6000 00f8      	braw 0x2f86
    2e90:	322a 0002      	movew %a2@(2),%d1
    2e94:	3001           	movew %d1,%d0
    2e96:	0640 0020      	addiw #32,%d0
    2e9a:	6a06           	bpls 0x2ea2
    2e9c:	3001           	movew %d1,%d0
    2e9e:	0640 002f      	addiw #47,%d0
    2ea2:	3600           	movew %d0,%d3
    2ea4:	e843           	asrw #4,%d3
    2ea6:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    2eac:	6614           	bnes 0x2ec2
    2eae:	3212           	movew %a2@,%d1
    2eb0:	3001           	movew %d1,%d0
    2eb2:	0640 000c      	addiw #12,%d0
    2eb6:	6a1a           	bpls 0x2ed2
    2eb8:	3001           	movew %d1,%d0
    2eba:	0640 001b      	addiw #27,%d0
    2ebe:	6000 0012      	braw 0x2ed2
    2ec2:	3212           	movew %a2@,%d1
    2ec4:	3001           	movew %d1,%d0
    2ec6:	0640 0014      	addiw #20,%d0
    2eca:	6a06           	bpls 0x2ed2
    2ecc:	3001           	movew %d1,%d0
    2ece:	0640 0023      	addiw #35,%d0
    2ed2:	3400           	movew %d0,%d2
    2ed4:	e842           	asrw #4,%d2
    2ed6:	3002           	movew %d2,%d0
    2ed8:	48c0           	extl %d0
    2eda:	41ec 0100      	lea %a4@(256),%a0
    2ede:	2200           	movel %d0,%d1
    2ee0:	e781           	asll #3,%d1
    2ee2:	d280           	addl %d0,%d1
    2ee4:	d280           	addl %d0,%d1
    2ee6:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    2eea:	1030 3000      	moveb %a0@(00000000,%d3:w),%d0
    2eee:	0c00 0001      	cmpib #1,%d0
    2ef2:	670c           	beqs 0x2f00
    2ef4:	0c00 0003      	cmpib #3,%d0
    2ef8:	6706           	beqs 0x2f00
    2efa:	0c00 0014      	cmpib #20,%d0
    2efe:	661c           	bnes 0x2f1c
    2f00:	157c 0003 0006 	moveb #3,%a2@(6)
    2f06:	422a 0005      	clrb %a2@(5)
    2f0a:	3002           	movew %d2,%d0
    2f0c:	e940           	aslw #4,%d0
    2f0e:	0640 fff5      	addiw #-11,%d0
    2f12:	3480           	movew %d0,%a2@
    2f14:	422a 0008      	clrb %a2@(8)
    2f18:	6000 002c      	braw 0x2f46
    2f1c:	0c00 0007      	cmpib #7,%d0
    2f20:	670c           	beqs 0x2f2e
    2f22:	0c00 0009      	cmpib #9,%d0
    2f26:	6706           	beqs 0x2f2e
    2f28:	0c00 0015      	cmpib #21,%d0
    2f2c:	6626           	bnes 0x2f54
    2f2e:	157c 0003 0006 	moveb #3,%a2@(6)
    2f34:	422a 0005      	clrb %a2@(5)
    2f38:	3002           	movew %d2,%d0
    2f3a:	e940           	aslw #4,%d0
    2f3c:	5f40           	subqw #7,%d0
    2f3e:	3480           	movew %d0,%a2@
    2f40:	157c 0001 0008 	moveb #1,%a2@(8)
    2f46:	157c 0001 0009 	moveb #1,%a2@(9)
    2f4c:	422a 001f      	clrb %a2@(31)
    2f50:	6000 0034      	braw 0x2f86
    2f54:	102a 001f      	moveb %a2@(31),%d0
    2f58:	6716           	beqs 0x2f70
    2f5a:	0c00 0001      	cmpib #1,%d0
    2f5e:	6606           	bnes 0x2f66
    2f60:	157c 0003 000a 	moveb #3,%a2@(10)
    2f66:	2f0a           	movel %a2,%sp@-
    2f68:	6100 fcce      	bsrw 0x2c38
    2f6c:	6000 0018      	braw 0x2f86
    2f70:	4a2a 0021      	tstb %a2@(33)
    2f74:	6610           	bnes 0x2f86
    2f76:	157c 0001 001f 	moveb #1,%a2@(31)
    2f7c:	157c 0009 0005 	moveb #9,%a2@(5)
    2f82:	576a 0002      	subqw #3,%a2@(2)
    2f86:	4cee 0418 fff4 	moveml %fp@(-12),%d3-%d4/%a2
    2f8c:	4e5e           	unlk %fp
    2f8e:	4e75           	rts
    2f90:	4e56 0000      	linkw %fp,#0
    2f94:	48e7 1820      	moveml %d3-%d4/%a2,%sp@-
    2f98:	246e 0008      	moveal %fp@(8),%a2
    2f9c:	4a2a 001f      	tstb %a2@(31)
    2fa0:	6600 01fc      	bnew 0x319e
    2fa4:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    2faa:	6600 0126      	bnew 0x30d2
    2fae:	0c2a 0001 0009 	cmpib #1,%a2@(9)
    2fb4:	662a           	bnes 0x2fe0
    2fb6:	422a 0009      	clrb %a2@(9)
    2fba:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    2fc0:	660e           	bnes 0x2fd0
    2fc2:	422a 0008      	clrb %a2@(8)
    2fc6:	5e52           	addqw #7,%a2@
    2fc8:	546a 0002      	addqw #2,%a2@(2)
    2fcc:	6000 01d0      	braw 0x319e
    2fd0:	157c 0001 0008 	moveb #1,%a2@(8)
    2fd6:	5f52           	subqw #7,%a2@
    2fd8:	546a 0002      	addqw #2,%a2@(2)
    2fdc:	6000 01c0      	braw 0x319e
    2fe0:	102a 0005      	moveb %a2@(5),%d0
    2fe4:	1800           	moveb %d0,%d4
    2fe6:	5204           	addqb #1,%d4
    2fe8:	1544 0005      	moveb %d4,%a2@(5)
    2fec:	5200           	addqb #1,%d0
    2fee:	0c00 0002      	cmpib #2,%d0
    2ff2:	631a           	blss 0x300e
    2ff4:	422a 0005      	clrb %a2@(5)
    2ff8:	4a2a 0008      	tstb %a2@(8)
    2ffc:	6606           	bnes 0x3004
    2ffe:	5452           	addqw #2,%a2@
    3000:	6000 0004      	braw 0x3006
    3004:	5552           	subqw #2,%a2@
    3006:	546a 0002      	addqw #2,%a2@(2)
    300a:	6000 0014      	braw 0x3020
    300e:	4a2a 0008      	tstb %a2@(8)
    3012:	6606           	bnes 0x301a
    3014:	5652           	addqw #3,%a2@
    3016:	6000 0004      	braw 0x301c
    301a:	5752           	subqw #3,%a2@
    301c:	566a 0002      	addqw #3,%a2@(2)
    3020:	4a2a 0005      	tstb %a2@(5)
    3024:	6608           	bnes 0x302e
    3026:	3f3c 0001      	movew #1,%sp@-
    302a:	6100 33dc      	bsrw 0x6408
    302e:	322a 0002      	movew %a2@(2),%d1
    3032:	3001           	movew %d1,%d0
    3034:	0640 0020      	addiw #32,%d0
    3038:	6a06           	bpls 0x3040
    303a:	3001           	movew %d1,%d0
    303c:	0640 002f      	addiw #47,%d0
    3040:	3600           	movew %d0,%d3
    3042:	e843           	asrw #4,%d3
    3044:	3212           	movew %a2@,%d1
    3046:	3001           	movew %d1,%d0
    3048:	0640 0010      	addiw #16,%d0
    304c:	6a06           	bpls 0x3054
    304e:	3001           	movew %d1,%d0
    3050:	0640 001f      	addiw #31,%d0
    3054:	3400           	movew %d0,%d2
    3056:	e842           	asrw #4,%d2
    3058:	3002           	movew %d2,%d0
    305a:	48c0           	extl %d0
    305c:	41ec 0100      	lea %a4@(256),%a0
    3060:	2200           	movel %d0,%d1
    3062:	e781           	asll #3,%d1
    3064:	d280           	addl %d0,%d1
    3066:	d280           	addl %d0,%d1
    3068:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    306c:	1230 3000      	moveb %a0@(00000000,%d3:w),%d1
    3070:	6742           	beqs 0x30b4
    3072:	1001           	moveb %d1,%d0
    3074:	5900           	subqb #4,%d0
    3076:	0c00 0001      	cmpib #1,%d0
    307a:	6338           	blss 0x30b4
    307c:	0c01 0006      	cmpib #6,%d1
    3080:	6732           	beqs 0x30b4
    3082:	1001           	moveb %d1,%d0
    3084:	0600 fff6      	addib #-10,%d0
    3088:	0c00 0001      	cmpib #1,%d0
    308c:	6326           	blss 0x30b4
    308e:	1001           	moveb %d1,%d0
    3090:	0600 fff4      	addib #-12,%d0
    3094:	0c00 0001      	cmpib #1,%d0
    3098:	631a           	blss 0x30b4
    309a:	0c01 000f      	cmpib #15,%d1
    309e:	6714           	beqs 0x30b4
    30a0:	0c01 0013      	cmpib #19,%d1
    30a4:	670e           	beqs 0x30b4
    30a6:	0c01 0023      	cmpib #35,%d1
    30aa:	6708           	beqs 0x30b4
    30ac:	0c01 0026      	cmpib #38,%d1
    30b0:	6600 00ec      	bnew 0x319e
    30b4:	422a 0005      	clrb %a2@(5)
    30b8:	422a 0006      	clrb %a2@(6)
    30bc:	157c 0001 0021 	moveb #1,%a2@(33)
    30c2:	3003           	movew %d3,%d0
    30c4:	e940           	aslw #4,%d0
    30c6:	0640 ffe9      	addiw #-23,%d0
    30ca:	3540 0002      	movew %d0,%a2@(2)
    30ce:	6000 00ce      	braw 0x319e
    30d2:	322a 0002      	movew %a2@(2),%d1
    30d6:	3001           	movew %d1,%d0
    30d8:	0640 0020      	addiw #32,%d0
    30dc:	6a06           	bpls 0x30e4
    30de:	3001           	movew %d1,%d0
    30e0:	0640 002f      	addiw #47,%d0
    30e4:	3600           	movew %d0,%d3
    30e6:	e843           	asrw #4,%d3
    30e8:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    30ee:	6612           	bnes 0x3102
    30f0:	3212           	movew %a2@,%d1
    30f2:	3001           	movew %d1,%d0
    30f4:	5040           	addqw #8,%d0
    30f6:	6a1a           	bpls 0x3112
    30f8:	3001           	movew %d1,%d0
    30fa:	0640 0017      	addiw #23,%d0
    30fe:	6000 0012      	braw 0x3112
    3102:	3212           	movew %a2@,%d1
    3104:	3001           	movew %d1,%d0
    3106:	0640 0018      	addiw #24,%d0
    310a:	6a06           	bpls 0x3112
    310c:	3001           	movew %d1,%d0
    310e:	0640 0027      	addiw #39,%d0
    3112:	3400           	movew %d0,%d2
    3114:	e842           	asrw #4,%d2
    3116:	3002           	movew %d2,%d0
    3118:	48c0           	extl %d0
    311a:	41ec 0100      	lea %a4@(256),%a0
    311e:	2200           	movel %d0,%d1
    3120:	e781           	asll #3,%d1
    3122:	d280           	addl %d0,%d1
    3124:	d280           	addl %d0,%d1
    3126:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    312a:	1030 3000      	moveb %a0@(00000000,%d3:w),%d0
    312e:	0c00 0008      	cmpib #8,%d0
    3132:	6706           	beqs 0x313a
    3134:	0c00 000a      	cmpib #10,%d0
    3138:	6620           	bnes 0x315a
    313a:	157c 0003 0006 	moveb #3,%a2@(6)
    3140:	422a 0005      	clrb %a2@(5)
    3144:	3002           	movew %d2,%d0
    3146:	e940           	aslw #4,%d0
    3148:	5f40           	subqw #7,%d0
    314a:	3480           	movew %d0,%a2@
    314c:	066a 000a 0002 	addiw #10,%a2@(2)
    3152:	422a 0008      	clrb %a2@(8)
    3156:	6000 002e      	braw 0x3186
    315a:	0c00 0002      	cmpib #2,%d0
    315e:	6706           	beqs 0x3166
    3160:	0c00 0006      	cmpib #6,%d0
    3164:	662c           	bnes 0x3192
    3166:	157c 0003 0006 	moveb #3,%a2@(6)
    316c:	422a 0005      	clrb %a2@(5)
    3170:	3002           	movew %d2,%d0
    3172:	e940           	aslw #4,%d0
    3174:	0640 fff6      	addiw #-10,%d0
    3178:	3480           	movew %d0,%a2@
    317a:	066a 000a 0002 	addiw #10,%a2@(2)
    3180:	157c 0001 0008 	moveb #1,%a2@(8)
    3186:	422a 0009      	clrb %a2@(9)
    318a:	422a 001f      	clrb %a2@(31)
    318e:	6000 000e      	braw 0x319e
    3192:	4a2a 0021      	tstb %a2@(33)
    3196:	6606           	bnes 0x319e
    3198:	157c 0006 0005 	moveb #6,%a2@(5)
    319e:	4cee 0418 fff4 	moveml %fp@(-12),%d3-%d4/%a2
    31a4:	4e5e           	unlk %fp
    31a6:	4e75           	rts
    31a8:	4e56 0000      	linkw %fp,#0
    31ac:	2f0a           	movel %a2,%sp@-
    31ae:	226e 0008      	moveal %fp@(8),%a1
    31b2:	4229 001f      	clrb %a1@(31)
    31b6:	4229 0006      	clrb %a1@(6)
    31ba:	3229 0002      	movew %a1@(2),%d1
    31be:	3001           	movew %d1,%d0
    31c0:	0640 0020      	addiw #32,%d0
    31c4:	6a06           	bpls 0x31cc
    31c6:	3001           	movew %d1,%d0
    31c8:	0640 002f      	addiw #47,%d0
    31cc:	3400           	movew %d0,%d2
    31ce:	e842           	asrw #4,%d2
    31d0:	3011           	movew %a1@,%d0
    31d2:	3200           	movew %d0,%d1
    31d4:	0641 0010      	addiw #16,%d1
    31d8:	6a06           	bpls 0x31e0
    31da:	3200           	movew %d0,%d1
    31dc:	0641 001f      	addiw #31,%d1
    31e0:	e841           	asrw #4,%d1
    31e2:	0c42 0009      	cmpiw #9,%d2
    31e6:	6e00 0086      	bgtw 0x326e
    31ea:	48c1           	extl %d1
    31ec:	41ec 0100      	lea %a4@(256),%a0
    31f0:	2001           	movel %d1,%d0
    31f2:	e780           	asll #3,%d0
    31f4:	d081           	addl %d1,%d0
    31f6:	d081           	addl %d1,%d0
    31f8:	45f0 081e      	lea %a0@(0000001e,%d0:l),%a2
    31fc:	3002           	movew %d2,%d0
    31fe:	e940           	aslw #4,%d0
    3200:	3040           	moveaw %d0,%a0
    3202:	d0fc ffe9      	addaw #-23,%a0
    3206:	1232 2000      	moveb %a2@(00000000,%d2:w),%d1
    320a:	0c01 0001      	cmpib #1,%d1
    320e:	6344           	blss 0x3254
    3210:	1001           	moveb %d1,%d0
    3212:	5900           	subqb #4,%d0
    3214:	0c00 0001      	cmpib #1,%d0
    3218:	633a           	blss 0x3254
    321a:	1001           	moveb %d1,%d0
    321c:	5d00           	subqb #6,%d0
    321e:	0c00 0001      	cmpib #1,%d0
    3222:	6330           	blss 0x3254
    3224:	1001           	moveb %d1,%d0
    3226:	0600 fff5      	addib #-11,%d0
    322a:	0c00 0001      	cmpib #1,%d0
    322e:	6324           	blss 0x3254
    3230:	0c01 000d      	cmpib #13,%d1
    3234:	671e           	beqs 0x3254
    3236:	0c01 000f      	cmpib #15,%d1
    323a:	6718           	beqs 0x3254
    323c:	0c01 0011      	cmpib #17,%d1
    3240:	6712           	beqs 0x3254
    3242:	0c01 0023      	cmpib #35,%d1
    3246:	670c           	beqs 0x3254
    3248:	0c01 0026      	cmpib #38,%d1
    324c:	6706           	beqs 0x3254
    324e:	0c01 000a      	cmpib #10,%d1
    3252:	660e           	bnes 0x3262
    3254:	3348 0002      	movew %a0,%a1@(2)
    3258:	307c 0099      	moveaw #153,%a0
    325c:	740b           	moveq #11,%d2
    325e:	6000 0008      	braw 0x3268
    3262:	d0fc 0010      	addaw #16,%a0
    3266:	5242           	addqw #1,%d2
    3268:	0c42 0009      	cmpiw #9,%d2
    326c:	6f98           	bles 0x3206
    326e:	246e fffc      	moveal %fp@(-4),%a2
    3272:	4e5e           	unlk %fp
    3274:	4e75           	rts
    3276:	4e56 fff8      	linkw %fp,#-8
    327a:	4280           	clrl %d0
    327c:	302e fffe      	movew %fp@(-2),%d0
    3280:	08c0 0015      	bset #21,%d0
    3284:	2d40 fffc      	movel %d0,%fp@(-4)
    3288:	3d7c 0020 fffe 	movew #32,%fp@(-2)
    328e:	41ec 00d0      	lea %a4@(208),%a0
    3292:	0c28 0003 0006 	cmpib #3,%a0@(6)
    3298:	6752           	beqs 0x32ec
    329a:	1228 0005      	moveb %a0@(5),%d1
    329e:	0c01 0009      	cmpib #9,%d1
    32a2:	6228           	bhis 0x32cc
    32a4:	2001           	movel %d1,%d0
    32a6:	eb40           	aslw #5,%d0
    32a8:	4840           	swap %d0
    32aa:	4240           	clrw %d0
    32ac:	0280 1fe0 0000 	andil #534773760,%d0
    32b2:	806e fffa      	orw %fp@(-6),%d0
    32b6:	2d40 fff8      	movel %d0,%fp@(-8)
    32ba:	1028 0008      	moveb %a0@(8),%d0
    32be:	eb40           	aslw #5,%d0
    32c0:	0240 1fe0      	andiw #8160,%d0
    32c4:	3d40 fffa      	movew %d0,%fp@(-6)
    32c8:	6000 006a      	braw 0x3334
    32cc:	4240           	clrw %d0
    32ce:	1028 0008      	moveb %a0@(8),%d0
    32d2:	eb40           	aslw #5,%d0
    32d4:	0640 0040      	addiw #64,%d0
    32d8:	3d40 fffa      	movew %d0,%fp@(-6)
    32dc:	4240           	clrw %d0
    32de:	1001           	moveb %d1,%d0
    32e0:	5940           	subqw #4,%d0
    32e2:	eb40           	aslw #5,%d0
    32e4:	4840           	swap %d0
    32e6:	4240           	clrw %d0
    32e8:	6000 0042      	braw 0x332c
    32ec:	4240           	clrw %d0
    32ee:	1028 0008      	moveb %a0@(8),%d0
    32f2:	eb40           	aslw #5,%d0
    32f4:	0640 0040      	addiw #64,%d0
    32f8:	3d40 fffa      	movew %d0,%fp@(-6)
    32fc:	1028 0005      	moveb %a0@(5),%d0
    3300:	0c00 0005      	cmpib #5,%d0
    3304:	621a           	bhis 0x3320
    3306:	0c28 0001 0009 	cmpib #1,%a0@(9)
    330c:	6712           	beqs 0x3320
    330e:	0240 00ff      	andiw #255,%d0
    3312:	eb40           	aslw #5,%d0
    3314:	0640 0060      	addiw #96,%d0
    3318:	4840           	swap %d0
    331a:	4240           	clrw %d0
    331c:	6000 000e      	braw 0x332c
    3320:	eb40           	aslw #5,%d0
    3322:	4840           	swap %d0
    3324:	4240           	clrw %d0
    3326:	0280 1fe0 0000 	andil #534773760,%d0
    332c:	806e fffa      	orw %fp@(-6),%d0
    3330:	2d40 fff8      	movel %d0,%fp@(-8)
    3334:	1f3c 0004      	moveb #4,%sp@-
    3338:	41ec 00d0      	lea %a4@(208),%a0
    333c:	3f28 0002      	movew %a0@(2),%sp@-
    3340:	3f10           	movew %a0@,%sp@-
    3342:	486e fff8      	pea %fp@(-8)
    3346:	2f2c 0084      	movel %a4@(132),%sp@-
    334a:	2f2c 0088      	movel %a4@(136),%sp@-
    334e:	4e4f           	trap #15
    3350:	a209           	0121011
    3352:	4e5e           	unlk %fp
    3354:	4e75           	rts
    3356:	4e56 fff4      	linkw %fp,#-12
    335a:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    335e:	4245           	clrw %d5
    3360:	347c 0000      	moveaw #0,%a2
    3364:	367c 0000      	moveaw #0,%a3
    3368:	41ec 0100      	lea %a4@(256),%a0
    336c:	d0fc 001e      	addaw #30,%a0
    3370:	2d48 fff4      	movel %a0,%fp@(-12)
    3374:	4244           	clrw %d4
    3376:	4247           	clrw %d7
    3378:	7c00           	moveq #0,%d6
    337a:	3204           	movew %d4,%d1
    337c:	48c1           	extl %d1
    337e:	2001           	movel %d1,%d0
    3380:	e780           	asll #3,%d0
    3382:	d081           	addl %d1,%d0
    3384:	d081           	addl %d1,%d0
    3386:	d0ae fff4      	addl %fp@(-12),%d0
    338a:	2240           	moveal %d0,%a1
    338c:	1031 b000      	moveb %a1@(00000000,%a3:w),%d0
    3390:	0c00 00fe      	cmpib #-2,%d0
    3394:	6700 009c      	beqw 0x3432
    3398:	0c00 0013      	cmpib #19,%d0
    339c:	6220           	bhis 0x33be
    339e:	1e00           	moveb %d0,%d7
    33a0:	3007           	movew %d7,%d0
    33a2:	e940           	aslw #4,%d0
    33a4:	3c00           	movew %d0,%d6
    33a6:	2006           	movel %d6,%d0
    33a8:	4840           	swap %d0
    33aa:	4240           	clrw %d0
    33ac:	806e fffa      	orw %fp@(-6),%d0
    33b0:	2d40 fff8      	movel %d0,%fp@(-8)
    33b4:	3d7c 0050 fffa 	movew #80,%fp@(-6)
    33ba:	6000 0044      	braw 0x3400
    33be:	0c00 0027      	cmpib #39,%d0
    33c2:	6220           	bhis 0x33e4
    33c4:	0240 00ff      	andiw #255,%d0
    33c8:	0640 ffec      	addiw #-20,%d0
    33cc:	e940           	aslw #4,%d0
    33ce:	4840           	swap %d0
    33d0:	4240           	clrw %d0
    33d2:	806e fffa      	orw %fp@(-6),%d0
    33d6:	2d40 fff8      	movel %d0,%fp@(-8)
    33da:	3d7c 0060 fffa 	movew #96,%fp@(-6)
    33e0:	6000 001e      	braw 0x3400
    33e4:	0240 00ff      	andiw #255,%d0
    33e8:	0640 ffd8      	addiw #-40,%d0
    33ec:	e940           	aslw #4,%d0
    33ee:	4840           	swap %d0
    33f0:	4240           	clrw %d0
    33f2:	806e fffa      	orw %fp@(-6),%d0
    33f6:	2d40 fff8      	movel %d0,%fp@(-8)
    33fa:	3d7c 0070 fffa 	movew #112,%fp@(-6)
    3400:	3d7c 0010 fffe 	movew #16,%fp@(-2)
    3406:	4280           	clrl %d0
    3408:	302e fffe      	movew %fp@(-2),%d0
    340c:	08c0 0014      	bset #20,%d0
    3410:	2d40 fffc      	movel %d0,%fp@(-4)
    3414:	4227           	clrb %sp@-
    3416:	3f0a           	movew %a2,%sp@-
    3418:	3f05           	movew %d5,%sp@-
    341a:	486e fff8      	pea %fp@(-8)
    341e:	2f2c 0080      	movel %a4@(128),%sp@-
    3422:	2f2c 0090      	movel %a4@(144),%sp@-
    3426:	4e4f           	trap #15
    3428:	a209           	0121011
    342a:	defc 0012      	addaw #18,%sp
    342e:	6000 0034      	braw 0x3464
    3432:	3d45 fff8      	movew %d5,%fp@(-8)
    3436:	3d4a fffa      	movew %a2,%fp@(-6)
    343a:	3d7c 0010 fffe 	movew #16,%fp@(-2)
    3440:	3d7c 0010 fffc 	movew #16,%fp@(-4)
    3446:	2f2c 0080      	movel %a4@(128),%sp@-
    344a:	4e4f           	trap #15
    344c:	a1fd           	0120775
    344e:	2608           	movel %a0,%d3
    3450:	4267           	clrw %sp@-
    3452:	486e fff8      	pea %fp@(-8)
    3456:	4e4f           	trap #15
    3458:	a219           	0121031
    345a:	2f03           	movel %d3,%sp@-
    345c:	4e4f           	trap #15
    345e:	a1fd           	0120775
    3460:	defc 000e      	addaw #14,%sp
    3464:	5244           	addqw #1,%d4
    3466:	0645 0010      	addiw #16,%d5
    346a:	0c44 0009      	cmpiw #9,%d4
    346e:	6f00 ff0a      	blew 0x337a
    3472:	524b           	addqw #1,%a3
    3474:	d4fc 0010      	addaw #16,%a2
    3478:	4245           	clrw %d5
    347a:	7409           	moveq #9,%d2
    347c:	b44b           	cmpw %a3,%d2
    347e:	6c00 fef4      	bgew 0x3374
    3482:	4cee 0cf8 ffd8 	moveml %fp@(-40),%d3-%d7/%a2-%a3
    3488:	4e5e           	unlk %fp
    348a:	4e75           	rts
    348c:	4e56 fff8      	linkw %fp,#-8
    3490:	4280           	clrl %d0
    3492:	302e fffa      	movew %fp@(-6),%d0
    3496:	2d40 fff8      	movel %d0,%fp@(-8)
    349a:	426e fffa      	clrw %fp@(-6)
    349e:	4280           	clrl %d0
    34a0:	302e fffe      	movew %fp@(-2),%d0
    34a4:	0080 00a0 0000 	oril #10485760,%d0
    34aa:	2d40 fffc      	movel %d0,%fp@(-4)
    34ae:	3d7c 00a0 fffe 	movew #160,%fp@(-2)
    34b4:	4227           	clrb %sp@-
    34b6:	4267           	clrw %sp@-
    34b8:	4267           	clrw %sp@-
    34ba:	486e fff8      	pea %fp@(-8)
    34be:	2f2c 0084      	movel %a4@(132),%sp@-
    34c2:	2f2c 0080      	movel %a4@(128),%sp@-
    34c6:	4e4f           	trap #15
    34c8:	a209           	0121011
    34ca:	4e5e           	unlk %fp
    34cc:	4e75           	rts
    34ce:	4e56 fff8      	linkw %fp,#-8
    34d2:	4280           	clrl %d0
    34d4:	302e fffa      	movew %fp@(-6),%d0
    34d8:	2d40 fff8      	movel %d0,%fp@(-8)
    34dc:	426e fffa      	clrw %fp@(-6)
    34e0:	4280           	clrl %d0
    34e2:	302e fffe      	movew %fp@(-2),%d0
    34e6:	0080 00a0 0000 	oril #10485760,%d0
    34ec:	2d40 fffc      	movel %d0,%fp@(-4)
    34f0:	3d7c 00a0 fffe 	movew #160,%fp@(-2)
    34f6:	4227           	clrb %sp@-
    34f8:	4267           	clrw %sp@-
    34fa:	4267           	clrw %sp@-
    34fc:	486e fff8      	pea %fp@(-8)
    3500:	4e4f           	trap #15
    3502:	a1ff           	0120777
    3504:	2f08           	movel %a0,%sp@-
    3506:	2f2c 0084      	movel %a4@(132),%sp@-
    350a:	4e4f           	trap #15
    350c:	a209           	0121011
    350e:	4e5e           	unlk %fp
    3510:	4e75           	rts
    3512:	4e56 fff8      	linkw %fp,#-8
    3516:	48e7 1e00      	moveml %d3-%d6,%sp@-
    351a:	41ec 0011      	lea %a4@(17),%a0
    351e:	4a10           	tstb %a0@
    3520:	6706           	beqs 0x3528
    3522:	4210           	clrb %a0@
    3524:	6000 0006      	braw 0x352c
    3528:	10bc 0001      	moveb #1,%a0@
    352c:	4244           	clrw %d4
    352e:	4243           	clrw %d3
    3530:	7a00           	moveq #0,%d5
    3532:	3204           	movew %d4,%d1
    3534:	c3fc 000c      	mulsw #12,%d1
    3538:	43ec 0100      	lea %a4@(256),%a1
    353c:	2009           	movel %a1,%d0
    353e:	2041           	moveal %d1,%a0
    3540:	d1c0           	addal %d0,%a0
    3542:	0c28 00fe 0084 	cmpib #-2,%a0@(132)
    3548:	6700 0162      	beqw 0x36ac
    354c:	1028 0088      	moveb %a0@(136),%d0
    3550:	674e           	beqs 0x35a0
    3552:	0c00 0001      	cmpib #1,%d0
    3556:	633e           	blss 0x3596
    3558:	1028 008e      	moveb %a0@(142),%d0
    355c:	1c00           	moveb %d0,%d6
    355e:	5206           	addqb #1,%d6
    3560:	1146 008e      	moveb %d6,%a0@(142)
    3564:	5200           	addqb #1,%d0
    3566:	b028 008b      	cmpb %a0@(139),%d0
    356a:	6304           	blss 0x3570
    356c:	4228 008e      	clrb %a0@(142)
    3570:	1c28 008e      	moveb %a0@(142),%d6
    3574:	bc28 008b      	cmpb %a0@(139),%d6
    3578:	6604           	bnes 0x357e
    357a:	5228 008d      	addqb #1,%a0@(141)
    357e:	1c28 008d      	moveb %a0@(141),%d6
    3582:	bc28 008c      	cmpb %a0@(140),%d6
    3586:	6304           	blss 0x358c
    3588:	4228 008d      	clrb %a0@(141)
    358c:	4242           	clrw %d2
    358e:	1428 008d      	moveb %a0@(141),%d2
    3592:	6000 000e      	braw 0x35a2
    3596:	4242           	clrw %d2
    3598:	142c 0011      	moveb %a4@(17),%d2
    359c:	6000 0004      	braw 0x35a2
    35a0:	4242           	clrw %d2
    35a2:	3204           	movew %d4,%d1
    35a4:	c3fc 000c      	mulsw #12,%d1
    35a8:	43ec 0100      	lea %a4@(256),%a1
    35ac:	2009           	movel %a1,%d0
    35ae:	2041           	moveal %d1,%a0
    35b0:	d1c0           	addal %d0,%a0
    35b2:	1628 0085      	moveb %a0@(133),%d3
    35b6:	0c43 0001      	cmpiw #1,%d3
    35ba:	6746           	beqs 0x3602
    35bc:	6e08           	bgts 0x35c6
    35be:	4a43           	tstw %d3
    35c0:	670e           	beqs 0x35d0
    35c2:	6000 00a4      	braw 0x3668
    35c6:	0c43 0002      	cmpiw #2,%d3
    35ca:	676a           	beqs 0x3636
    35cc:	6000 009a      	braw 0x3668
    35d0:	4240           	clrw %d0
    35d2:	1028 0084      	moveb %a0@(132),%d0
    35d6:	d042           	addw %d2,%d0
    35d8:	eb40           	aslw #5,%d0
    35da:	3a00           	movew %d0,%d5
    35dc:	2005           	movel %d5,%d0
    35de:	4840           	swap %d0
    35e0:	4240           	clrw %d0
    35e2:	806e fffa      	orw %fp@(-6),%d0
    35e6:	2d40 fff8      	movel %d0,%fp@(-8)
    35ea:	426e fffa      	clrw %fp@(-6)
    35ee:	3d7c 0020 fffe 	movew #32,%fp@(-2)
    35f4:	4280           	clrl %d0
    35f6:	302e fffe      	movew %fp@(-2),%d0
    35fa:	08c0 0015      	bset #21,%d0
    35fe:	6000 0064      	braw 0x3664
    3602:	4240           	clrw %d0
    3604:	1028 0084      	moveb %a0@(132),%d0
    3608:	d042           	addw %d2,%d0
    360a:	e940           	aslw #4,%d0
    360c:	4840           	swap %d0
    360e:	4240           	clrw %d0
    3610:	806e fffa      	orw %fp@(-6),%d0
    3614:	2d40 fff8      	movel %d0,%fp@(-8)
    3618:	3d7c 0020 fffa 	movew #32,%fp@(-6)
    361e:	4280           	clrl %d0
    3620:	302e fffe      	movew %fp@(-2),%d0
    3624:	08c0 0014      	bset #20,%d0
    3628:	2d40 fffc      	movel %d0,%fp@(-4)
    362c:	3d7c 0020 fffe 	movew #32,%fp@(-2)
    3632:	6000 0034      	braw 0x3668
    3636:	1028 0084      	moveb %a0@(132),%d0
    363a:	e940           	aslw #4,%d0
    363c:	4840           	swap %d0
    363e:	4240           	clrw %d0
    3640:	0280 0ff0 0000 	andil #267386880,%d0
    3646:	806e fffa      	orw %fp@(-6),%d0
    364a:	2d40 fff8      	movel %d0,%fp@(-8)
    364e:	3d7c 0040 fffa 	movew #64,%fp@(-6)
    3654:	3d7c 0010 fffe 	movew #16,%fp@(-2)
    365a:	4280           	clrl %d0
    365c:	302e fffe      	movew %fp@(-2),%d0
    3660:	08c0 0014      	bset #20,%d0
    3664:	2d40 fffc      	movel %d0,%fp@(-4)
    3668:	3004           	movew %d4,%d0
    366a:	c1fc 000c      	mulsw #12,%d0
    366e:	43ec 0100      	lea %a4@(256),%a1
    3672:	2209           	movel %a1,%d1
    3674:	2040           	moveal %d0,%a0
    3676:	d1c1           	addal %d1,%a0
    3678:	1028 008a      	moveb %a0@(138),%d0
    367c:	5100           	subqb #8,%d0
    367e:	0c00 0001      	cmpib #1,%d0
    3682:	6328           	blss 0x36ac
    3684:	1f3c 0004      	moveb #4,%sp@-
    3688:	4240           	clrw %d0
    368a:	1028 0087      	moveb %a0@(135),%d0
    368e:	3f00           	movew %d0,%sp@-
    3690:	4240           	clrw %d0
    3692:	1028 0086      	moveb %a0@(134),%d0
    3696:	3f00           	movew %d0,%sp@-
    3698:	486e fff8      	pea %fp@(-8)
    369c:	2f2c 0084      	movel %a4@(132),%sp@-
    36a0:	2f2c 0090      	movel %a4@(144),%sp@-
    36a4:	4e4f           	trap #15
    36a6:	a209           	0121011
    36a8:	defc 0012      	addaw #18,%sp
    36ac:	5244           	addqw #1,%d4
    36ae:	0c44 0009      	cmpiw #9,%d4
    36b2:	6f00 fe7e      	blew 0x3532
    36b6:	4cee 0078 ffe8 	moveml %fp@(-24),%d3-%d6
    36bc:	4e5e           	unlk %fp
    36be:	4e75           	rts
    36c0:	2564 2067      	movel %a4@-,%a2@(8295)
    36c4:	6f6c           	bles 0x3732
    36c6:	6420           	bccs 0x36e8
    36c8:	636f           	blss 0x3739
    36ca:	696e           	bvss 0x373a
    36cc:	7321           	071441
    36ce:	0052 6574      	oriw #25972,%a2@
    36d2:	7572           	072562
    36d4:	6e20           	bgts 0x36f6
    36d6:	746f           	moveq #111,%d2
    36d8:	204c           	moveal %a4,%a0
    36da:	6966           	bvss 0x3742
    36dc:	652e           	bcss 0x370c
    36de:	2e2e 0049      	movel %fp@(73),%d7
    36e2:	206c 6976      	moveal %a4@(26998),%a0
    36e6:	6521           	bcss 0x3709
    36e8:	0057 6865      	oriw #26725,%sp@
    36ec:	7721           	073441
    36ee:	0049 276d      	oriw #10093,%a1
    36f2:	2062           	moveal %a2@-,%a0
    36f4:	6163           	bsrs 0x3759
    36f6:	6b21           	bmis 0x3719
    36f8:	004c 6f63      	oriw #28515,%a4
    36fc:	6b65           	bmis 0x3763
    36fe:	6421           	bccs 0x3721
    3700:	2042           	moveal %d2,%a0
    3702:	7574           	072564
    3704:	2074 6865      	moveal %a4@(00000065,%d6:l),%a0
    3708:	2025           	movel %a5@-,%d0
    370a:	7320           	071440
    370c:	756e           	072556
    370e:	6c6f           	bges 0x377f
    3710:	636b           	blss 0x377d
    3712:	6564           	bcss 0x3778
    3714:	2069 7421      	moveal %a1@(29729),%a0
    3718:	004c 6f63      	oriw #28515,%a4
    371c:	6b65           	bmis 0x3783
    371e:	6421           	bccs 0x3741
    3720:	2042           	moveal %d2,%a0
    3722:	7574           	072564
    3724:	206f 6e65      	moveal %sp@(28261),%a0
    3728:	206f 6620      	moveal %sp@(26144),%a0
    372c:	6d79           	blts 0x37a7
    372e:	206b 6579      	moveal %a3@(25977),%a0
    3732:	7320           	071440
    3734:	776f           	073557
    3736:	726b           	moveq #107,%d1
    3738:	6564           	bcss 0x379e
    373a:	2100           	movel %d0,%a0@-
    373c:	4c6f           	046157
    373e:	636b           	blss 0x37ab
    3740:	6564           	bcss 0x37a6
    3742:	2120           	movel %a0@-,%a0@-
    3744:	416e           	040556
    3746:	6420           	bccs 0x3768
    3748:	6e6f           	bgts 0x37b9
    374a:	6e65           	bgts 0x37b1
    374c:	206f 6620      	moveal %sp@(26144),%a0
    3750:	6d79           	blts 0x37cb
    3752:	206b 6579      	moveal %a3@(25977),%a0
    3756:	7320           	071440
    3758:	776f           	073557
    375a:	726b           	moveq #107,%d1
    375c:	6564           	bcss 0x37c2
    375e:	2100           	movel %d0,%a0@-
    3760:	4c6f           	046157
    3762:	636b           	blss 0x37cf
    3764:	6564           	bcss 0x37ca
    3766:	2100           	movel %d0,%a0@-
    3768:	4c6f           	046157
    376a:	636b           	blss 0x37d7
    376c:	6564           	bcss 0x37d2
    376e:	2120           	movel %a0@-,%a0@-
    3770:	4275 7420      	clrw %a5@(00000020,%d7:w:4)
    3774:	4920           	chkl %a0@-,%d4
    3776:	7069           	moveq #105,%d0
    3778:	636b           	blss 0x37e5
    377a:	6564           	bcss 0x37e0
    377c:	2069 7421      	moveal %a1@(29729),%a0
    3780:	004c 6f63      	oriw #28515,%a4
    3784:	6b65           	bmis 0x37eb
    3786:	6421           	bccs 0x37a9
    3788:	2041           	moveal %d1,%a0
    378a:	6e64           	bgts 0x37f0
    378c:	2049           	moveal %a1,%a0
    378e:	2063           	moveal %a3@-,%a0
    3790:	616e           	bsrs 0x3800
    3792:	2774 2070 6963 	movel %a4@(00000070,%d2:w),%a3@(26979)
    3798:	6b20           	bmis 0x37ba
    379a:	7468           	moveq #104,%d2
    379c:	6973           	bvss 0x3811
    379e:	206f 6e65      	moveal %sp@(28261),%a0
    37a2:	2100           	movel %d0,%a0@-
    37a4:	4e56 ff80      	linkw %fp,#-128
    37a8:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    37ac:	426e ff86      	clrw %fp@(-122)
    37b0:	41ec 00d0      	lea %a4@(208),%a0
    37b4:	4a28 001f      	tstb %a0@(31)
    37b8:	6600 0556      	bnew 0x3d10
    37bc:	4245           	clrw %d5
    37be:	4246           	clrw %d6
    37c0:	3405           	movew %d5,%d2
    37c2:	c5fc 000c      	mulsw #12,%d2
    37c6:	47ec 0100      	lea %a4@(256),%a3
    37ca:	200b           	movel %a3,%d0
    37cc:	2042           	moveal %d2,%a0
    37ce:	d1c0           	addal %d0,%a0
    37d0:	0c28 00fe 0084 	cmpib #-2,%a0@(132)
    37d6:	6700 00a2      	beqw 0x387a
    37da:	1c28 0086      	moveb %a0@(134),%d6
    37de:	43ec 00d0      	lea %a4@(208),%a1
    37e2:	3006           	movew %d6,%d0
    37e4:	9051           	subw %a1@,%d0
    37e6:	6a02           	bpls 0x37ea
    37e8:	4440           	negw %d0
    37ea:	0c40 000f      	cmpiw #15,%d0
    37ee:	6e00 008a      	bgtw 0x387a
    37f2:	4240           	clrw %d0
    37f4:	1028 0087      	moveb %a0@(135),%d0
    37f8:	9069 0002      	subw %a1@(2),%d0
    37fc:	6a02           	bpls 0x3800
    37fe:	4440           	negw %d0
    3800:	0c40 001e      	cmpiw #30,%d0
    3804:	6e74           	bgts 0x387a
    3806:	1228 0089      	moveb %a0@(137),%d1
    380a:	1001           	moveb %d1,%d0
    380c:	5700           	subqb #3,%d0
    380e:	0c00 0001      	cmpib #1,%d0
    3812:	630a           	blss 0x381e
    3814:	1001           	moveb %d1,%d0
    3816:	5b00           	subqb #5,%d0
    3818:	0c00 0001      	cmpib #1,%d0
    381c:	625c           	bhis 0x387a
    381e:	0c28 0008 008a 	cmpib #8,%a0@(138)
    3824:	6600 0114      	bnew 0x393a
    3828:	4243           	clrw %d3
    382a:	2242           	moveal %d2,%a1
    382c:	4244           	clrw %d4
    382e:	4242           	clrw %d2
    3830:	47ec 0100      	lea %a4@(256),%a3
    3834:	200b           	movel %a3,%d0
    3836:	41f1 0800      	lea %a1@(00000000,%d0:l),%a0
    383a:	1828 0086      	moveb %a0@(134),%d4
    383e:	3203           	movew %d3,%d1
    3840:	c3fc 000c      	mulsw #12,%d1
    3844:	2041           	moveal %d1,%a0
    3846:	d1c0           	addal %d0,%a0
    3848:	1428 0086      	moveb %a0@(134),%d2
    384c:	3004           	movew %d4,%d0
    384e:	9042           	subw %d2,%d0
    3850:	3200           	movew %d0,%d1
    3852:	6c02           	bges 0x3856
    3854:	4441           	negw %d1
    3856:	0c41 000f      	cmpiw #15,%d1
    385a:	6e16           	bgts 0x3872
    385c:	4a40           	tstw %d0
    385e:	6c02           	bges 0x3862
    3860:	4440           	negw %d0
    3862:	0c40 000f      	cmpiw #15,%d0
    3866:	6e0a           	bgts 0x3872
    3868:	0c28 0003 008a 	cmpib #3,%a0@(138)
    386e:	6700 00ca      	beqw 0x393a
    3872:	5243           	addqw #1,%d3
    3874:	0c43 0009      	cmpiw #9,%d3
    3878:	6fb6           	bles 0x3830
    387a:	5245           	addqw #1,%d5
    387c:	0c45 0009      	cmpiw #9,%d5
    3880:	6f00 ff3e      	blew 0x37c0
    3884:	47ec 0100      	lea %a4@(256),%a3
    3888:	200b           	movel %a3,%d0
    388a:	2040           	moveal %d0,%a0
    388c:	d0fc 0108      	addaw #264,%a0
    3890:	4a10           	tstb %a0@
    3892:	6700 00e0      	beqw 0x3974
    3896:	4a28 0008      	tstb %a0@(8)
    389a:	6600 00d8      	bnew 0x3974
    389e:	43ec 00d0      	lea %a4@(208),%a1
    38a2:	3028 fffe      	movew %a0@(-2),%d0
    38a6:	9069 0002      	subw %a1@(2),%d0
    38aa:	6a02           	bpls 0x38ae
    38ac:	4440           	negw %d0
    38ae:	0c40 000f      	cmpiw #15,%d0
    38b2:	6e00 00c0      	bgtw 0x3974
    38b6:	3028 fffc      	movew %a0@(-4),%d0
    38ba:	9051           	subw %a1@,%d0
    38bc:	6a02           	bpls 0x38c0
    38be:	4440           	negw %d0
    38c0:	0c40 000f      	cmpiw #15,%d0
    38c4:	6e00 00ae      	bgtw 0x3974
    38c8:	4245           	clrw %d5
    38ca:	3005           	movew %d5,%d0
    38cc:	c1fc 000c      	mulsw #12,%d0
    38d0:	47ec 0100      	lea %a4@(256),%a3
    38d4:	220b           	movel %a3,%d1
    38d6:	2040           	moveal %d0,%a0
    38d8:	d1c1           	addal %d1,%a0
    38da:	0c28 00fe 0084 	cmpib #-2,%a0@(132)
    38e0:	6708           	beqs 0x38ea
    38e2:	0c28 0009 008a 	cmpib #9,%a0@(138)
    38e8:	6750           	beqs 0x393a
    38ea:	5245           	addqw #1,%d5
    38ec:	0c45 0009      	cmpiw #9,%d5
    38f0:	6fd8           	bles 0x38ca
    38f2:	2601           	movel %d1,%d3
    38f4:	0683 0000 0116 	addil #278,%d3
    38fa:	2643           	moveal %d3,%a3
    38fc:	3013           	movew %a3@,%d0
    38fe:	6774           	beqs 0x3974
    3900:	3f00           	movew %d0,%sp@-
    3902:	47fa fdbc      	lea %pc@(0x36c0),%a3
    3906:	2f0b           	movel %a3,%sp@-
    3908:	45ee ffc4      	lea %fp@(-60),%a2
    390c:	2f0a           	movel %a2,%sp@-
    390e:	4e4f           	trap #15
    3910:	a2de           	0121336
    3912:	41ec 00d0      	lea %a4@(208),%a0
    3916:	2643           	moveal %d3,%a3
    3918:	3e13           	movew %a3@,%d7
    391a:	df68 0012      	addw %d7,%a0@(18)
    391e:	4253           	clrw %a3@
    3920:	2f0a           	movel %a2,%sp@-
    3922:	3f3c 0001      	movew #1,%sp@-
    3926:	3f3c 029c      	movew #668,%sp@-
    392a:	6100 0d60      	bsrw 0x468c
    392e:	3f3c 0009      	movew #9,%sp@-
    3932:	6100 2ad4      	bsrw 0x6408
    3936:	6000 03d8      	braw 0x3d10
    393a:	3f05           	movew %d5,%sp@-
    393c:	6100 0418      	bsrw 0x3d56
    3940:	6000 03ce      	braw 0x3d10
    3944:	47fa fdf6      	lea %pc@(0x373c),%a3
    3948:	6000 0368      	braw 0x3cb2
    394c:	117c 0001 008a 	moveb #1,%a0@(138)
    3952:	47fa fe14      	lea %pc@(0x3768),%a3
    3956:	2f0b           	movel %a3,%sp@-
    3958:	3f3c 0001      	movew #1,%sp@-
    395c:	3f3c 029c      	movew #668,%sp@-
    3960:	6100 0d2a      	bsrw 0x468c
    3964:	3f3c 0002      	movew #2,%sp@-
    3968:	6100 2a9e      	bsrw 0x6408
    396c:	2644           	moveal %d4,%a3
    396e:	5413           	addqb #2,%a3@
    3970:	6000 039e      	braw 0x3d10
    3974:	4245           	clrw %d5
    3976:	4242           	clrw %d2
    3978:	7c88           	moveq #-120,%d6
    397a:	dc8e           	addl %fp,%d6
    397c:	41ec 00d0      	lea %a4@(208),%a0
    3980:	780d           	moveq #13,%d4
    3982:	d888           	addl %a0,%d4
    3984:	3205           	movew %d5,%d1
    3986:	c3fc 000c      	mulsw #12,%d1
    398a:	47ec 0100      	lea %a4@(256),%a3
    398e:	200b           	movel %a3,%d0
    3990:	2041           	moveal %d1,%a0
    3992:	d1c0           	addal %d0,%a0
    3994:	0c28 00fe 0084 	cmpib #-2,%a0@(132)
    399a:	6700 036a      	beqw 0x3d06
    399e:	1428 0086      	moveb %a0@(134),%d2
    39a2:	43ec 00d0      	lea %a4@(208),%a1
    39a6:	3002           	movew %d2,%d0
    39a8:	9051           	subw %a1@,%d0
    39aa:	6a02           	bpls 0x39ae
    39ac:	4440           	negw %d0
    39ae:	0c40 000f      	cmpiw #15,%d0
    39b2:	6e00 0352      	bgtw 0x3d06
    39b6:	4240           	clrw %d0
    39b8:	1028 0087      	moveb %a0@(135),%d0
    39bc:	9069 0002      	subw %a1@(2),%d0
    39c0:	6a02           	bpls 0x39c4
    39c2:	4440           	negw %d0
    39c4:	0c40 002f      	cmpiw #47,%d0
    39c8:	6e00 033c      	bgtw 0x3d06
    39cc:	1228 0089      	moveb %a0@(137),%d1
    39d0:	0c01 0001      	cmpib #1,%d1
    39d4:	670e           	beqs 0x39e4
    39d6:	0c01 0008      	cmpib #8,%d1
    39da:	6708           	beqs 0x39e4
    39dc:	0c01 0002      	cmpib #2,%d1
    39e0:	6600 0092      	bnew 0x3a74
    39e4:	3205           	movew %d5,%d1
    39e6:	c3fc 000c      	mulsw #12,%d1
    39ea:	47ec 0100      	lea %a4@(256),%a3
    39ee:	200b           	movel %a3,%d0
    39f0:	2441           	moveal %d1,%a2
    39f2:	d5c0           	addal %d0,%a2
    39f4:	102a 008a      	moveb %a2@(138),%d0
    39f8:	0c00 0001      	cmpib #1,%d0
    39fc:	6728           	beqs 0x3a26
    39fe:	0c00 0007      	cmpib #7,%d0
    3a02:	6638           	bnes 0x3a3c
    3a04:	42a7           	clrl %sp@-
    3a06:	4e4f           	trap #15
    3a08:	a0c2           	0120302
    3a0a:	7e64           	moveq #100,%d7
    3a0c:	48c0           	extl %d0
    3a0e:	81c7           	divsw %d7,%d0
    3a10:	2200           	movel %d0,%d1
    3a12:	4841           	swap %d1
    3a14:	41ec 00d0      	lea %a4@(208),%a0
    3a18:	4240           	clrw %d0
    3a1a:	1028 000e      	moveb %a0@(14),%d0
    3a1e:	584f           	addqw #4,%sp
    3a20:	b041           	cmpw %d1,%d0
    3a22:	6f00 02ec      	blew 0x3d10
    3a26:	157c 0003 008a 	moveb #3,%a2@(138)
    3a2c:	522a 0084      	addqb #1,%a2@(132)
    3a30:	3f3c 0004      	movew #4,%sp@-
    3a34:	6100 29d2      	bsrw 0x6408
    3a38:	6000 02d6      	braw 0x3d10
    3a3c:	0c00 0003      	cmpib #3,%d0
    3a40:	6632           	bnes 0x3a74
    3a42:	0c2a 0008 0089 	cmpib #8,%a2@(137)
    3a48:	660e           	bnes 0x3a58
    3a4a:	41ec 00d0      	lea %a4@(208),%a0
    3a4e:	4240           	clrw %d0
    3a50:	102a 008b      	moveb %a2@(139),%d0
    3a54:	3140 0002      	movew %d0,%a0@(2)
    3a58:	4a2a 008c      	tstb %a2@(140)
    3a5c:	6700 02b2      	beqw 0x3d10
    3a60:	4240           	clrw %d0
    3a62:	102a 0086      	moveb %a2@(134),%d0
    3a66:	3940 00d0      	movew %d0,%a4@(208)
    3a6a:	4241           	clrw %d1
    3a6c:	122a 008c      	moveb %a2@(140),%d1
    3a70:	6000 0084      	braw 0x3af6
    3a74:	3205           	movew %d5,%d1
    3a76:	c3fc 000c      	mulsw #12,%d1
    3a7a:	47ec 0100      	lea %a4@(256),%a3
    3a7e:	200b           	movel %a3,%d0
    3a80:	2041           	moveal %d1,%a0
    3a82:	d1c0           	addal %d0,%a0
    3a84:	1028 0089      	moveb %a0@(137),%d0
    3a88:	0c00 0007      	cmpib #7,%d0
    3a8c:	6616           	bnes 0x3aa4
    3a8e:	4a28 008c      	tstb %a0@(140)
    3a92:	6700 027c      	beqw 0x3d10
    3a96:	4240           	clrw %d0
    3a98:	1028 0086      	moveb %a0@(134),%d0
    3a9c:	3940 00d0      	movew %d0,%a4@(208)
    3aa0:	6000 004e      	braw 0x3af0
    3aa4:	0c00 000f      	cmpib #15,%d0
    3aa8:	6656           	bnes 0x3b00
    3aaa:	43ec 00d0      	lea %a4@(208),%a1
    3aae:	1e29 001c      	moveb %a1@(28),%d7
    3ab2:	be28 008b      	cmpb %a0@(139),%d7
    3ab6:	6500 0258      	bcsw 0x3d10
    3aba:	4a28 008c      	tstb %a0@(140)
    3abe:	6700 0250      	beqw 0x3d10
    3ac2:	4240           	clrw %d0
    3ac4:	1028 0086      	moveb %a0@(134),%d0
    3ac8:	3280           	movew %d0,%a1@
    3aca:	4a28 008a      	tstb %a0@(138)
    3ace:	6616           	bnes 0x3ae6
    3ad0:	117c 0001 008a 	moveb #1,%a0@(138)
    3ad6:	5a29 000b      	addqb #5,%a1@(11)
    3ada:	5a29 000c      	addqb #5,%a1@(12)
    3ade:	2644           	moveal %d4,%a3
    3ae0:	5a13           	addqb #5,%a3@
    3ae2:	5029 000e      	addqb #8,%a1@(14)
    3ae6:	1028 008d      	moveb %a0@(141),%d0
    3aea:	6704           	beqs 0x3af0
    3aec:	1340 001d      	moveb %d0,%a1@(29)
    3af0:	4241           	clrw %d1
    3af2:	1228 008c      	moveb %a0@(140),%d1
    3af6:	3f01           	movew %d1,%sp@-
    3af8:	6100 dbd8      	bsrw 0x16d2
    3afc:	6000 0212      	braw 0x3d10
    3b00:	0c00 0010      	cmpib #16,%d0
    3b04:	663a           	bnes 0x3b40
    3b06:	4a28 008b      	tstb %a0@(139)
    3b0a:	671e           	beqs 0x3b2a
    3b0c:	5328 0084      	subqb #1,%a0@(132)
    3b10:	4228 008b      	clrb %a0@(139)
    3b14:	41ec 00d0      	lea %a4@(208),%a0
    3b18:	d0fc 001c      	addaw #28,%a0
    3b1c:	1010           	moveb %a0@,%d0
    3b1e:	6700 01f0      	beqw 0x3d10
    3b22:	5300           	subqb #1,%d0
    3b24:	1080           	moveb %d0,%a0@
    3b26:	6000 01e8      	braw 0x3d10
    3b2a:	5228 0084      	addqb #1,%a0@(132)
    3b2e:	117c 0001 008b 	moveb #1,%a0@(139)
    3b34:	41ec 00d0      	lea %a4@(208),%a0
    3b38:	5228 001c      	addqb #1,%a0@(28)
    3b3c:	6000 01d2      	braw 0x3d10
    3b40:	0c00 000e      	cmpib #14,%d0
    3b44:	6600 0092      	bnew 0x3bd8
    3b48:	45ec 00d0      	lea %a4@(208),%a2
    3b4c:	4240           	clrw %d0
    3b4e:	102a 0017      	moveb %a2@(23),%d0
    3b52:	3480           	movew %d0,%a2@
    3b54:	4240           	clrw %d0
    3b56:	102a 0018      	moveb %a2@(24),%d0
    3b5a:	3540 0002      	movew %d0,%a2@(2)
    3b5e:	47ea 0019      	lea %a2@(25),%a3
    3b62:	2d4b ff82      	movel %a3,%fp@(-126)
    3b66:	4240           	clrw %d0
    3b68:	1013           	moveb %a3@,%d0
    3b6a:	3f00           	movew %d0,%sp@-
    3b6c:	47fa fb61      	lea %pc@(0x36cf),%a3
    3b70:	2f0b           	movel %a3,%sp@-
    3b72:	486c 0098      	pea %a4@(152)
    3b76:	4e4f           	trap #15
    3b78:	a2de           	0121336
    3b7a:	156a 000b 000c 	moveb %a2@(11),%a2@(12)
    3b80:	422c 0010      	clrb %a4@(16)
    3b84:	266e ff82      	moveal %fp@(-126),%a3
    3b88:	4240           	clrw %d0
    3b8a:	1013           	moveb %a3@,%d0
    3b8c:	3f00           	movew %d0,%sp@-
    3b8e:	6100 db42      	bsrw 0x16d2
    3b92:	504f           	addqw #8,%sp
    3b94:	4297           	clrl %sp@
    3b96:	4e4f           	trap #15
    3b98:	a0c2           	0120302
    3b9a:	7e03           	moveq #3,%d7
    3b9c:	48c0           	extl %d0
    3b9e:	81c7           	divsw %d7,%d0
    3ba0:	2200           	movel %d0,%d1
    3ba2:	4841           	swap %d1
    3ba4:	584f           	addqw #4,%sp
    3ba6:	0c41 0001      	cmpiw #1,%d1
    3baa:	671c           	beqs 0x3bc8
    3bac:	6e08           	bgts 0x3bb6
    3bae:	4a41           	tstw %d1
    3bb0:	670e           	beqs 0x3bc0
    3bb2:	6000 015c      	braw 0x3d10
    3bb6:	0c41 0002      	cmpiw #2,%d1
    3bba:	6714           	beqs 0x3bd0
    3bbc:	6000 0152      	braw 0x3d10
    3bc0:	47fa fb1f      	lea %pc@(0x36e1),%a3
    3bc4:	6000 00ec      	braw 0x3cb2
    3bc8:	47fa fb1f      	lea %pc@(0x36e9),%a3
    3bcc:	6000 00e4      	braw 0x3cb2
    3bd0:	47fa fb1d      	lea %pc@(0x36ef),%a3
    3bd4:	6000 00dc      	braw 0x3cb2
    3bd8:	0c00 0002      	cmpib #2,%d0
    3bdc:	6708           	beqs 0x3be6
    3bde:	0c00 0008      	cmpib #8,%d0
    3be2:	6600 0122      	bnew 0x3d06
    3be6:	0c28 0002 008a 	cmpib #2,%a0@(138)
    3bec:	6600 0118      	bnew 0x3d06
    3bf0:	4243           	clrw %d3
    3bf2:	3403           	movew %d3,%d2
    3bf4:	c5fc 000c      	mulsw #12,%d2
    3bf8:	43ec 0740      	lea %a4@(1856),%a1
    3bfc:	0c31 0002 2801 	cmpib #2,%a1@(00000001,%d2:l)
    3c02:	6600 0088      	bnew 0x3c8c
    3c06:	0c31 0008 2800 	cmpib #8,%a1@(00000000,%d2:l)
    3c0c:	667e           	bnes 0x3c8c
    3c0e:	3d7c 0001 ff86 	movew #1,%fp@(-122)
    3c14:	3205           	movew %d5,%d1
    3c16:	c3fc 000c      	mulsw #12,%d1
    3c1a:	47ec 0100      	lea %a4@(256),%a3
    3c1e:	200b           	movel %a3,%d0
    3c20:	2041           	moveal %d1,%a0
    3c22:	d1c0           	addal %d0,%a0
    3c24:	1e28 008d      	moveb %a0@(141),%d7
    3c28:	be31 280a      	cmpb %a1@(0000000a,%d2:l),%d7
    3c2c:	665e           	bnes 0x3c8c
    3c2e:	117c 0001 008a 	moveb #1,%a0@(138)
    3c34:	4240           	clrw %d0
    3c36:	1031 280a      	moveb %a1@(0000000a,%d2:l),%d0
    3c3a:	3f00           	movew %d0,%sp@-
    3c3c:	2f06           	movel %d6,%sp@-
    3c3e:	6100 25da      	bsrw 0x621a
    3c42:	5c4f           	addqw #6,%sp
    3c44:	4a00           	tstb %d0
    3c46:	6718           	beqs 0x3c60
    3c48:	2f06           	movel %d6,%sp@-
    3c4a:	47fa faad      	lea %pc@(0x36f9),%a3
    3c4e:	2f0b           	movel %a3,%sp@-
    3c50:	486e ffc4      	pea %fp@(-60)
    3c54:	4e4f           	trap #15
    3c56:	a2de           	0121336
    3c58:	defc 000c      	addaw #12,%sp
    3c5c:	6000 0012      	braw 0x3c70
    3c60:	47fa fab7      	lea %pc@(0x3719),%a3
    3c64:	2f0b           	movel %a3,%sp@-
    3c66:	486e ffc4      	pea %fp@(-60)
    3c6a:	4e4f           	trap #15
    3c6c:	a0c5           	0120305
    3c6e:	504f           	addqw #8,%sp
    3c70:	486e ffc4      	pea %fp@(-60)
    3c74:	3f3c 0001      	movew #1,%sp@-
    3c78:	3f3c 029c      	movew #668,%sp@-
    3c7c:	6100 0a0e      	bsrw 0x468c
    3c80:	3f3c 0002      	movew #2,%sp@-
    3c84:	6100 2782      	bsrw 0x6408
    3c88:	6000 0086      	braw 0x3d10
    3c8c:	5243           	addqw #1,%d3
    3c8e:	0c43 0009      	cmpiw #9,%d3
    3c92:	6f00 ff5e      	blew 0x3bf2
    3c96:	4a2c 000e      	tstb %a4@(14)
    3c9a:	6628           	bnes 0x3cc4
    3c9c:	3f3c 000d      	movew #13,%sp@-
    3ca0:	6100 2766      	bsrw 0x6408
    3ca4:	544f           	addqw #2,%sp
    3ca6:	4a6e ff86      	tstw %fp@(-122)
    3caa:	6600 fc98      	bnew 0x3944
    3cae:	47fa fab0      	lea %pc@(0x3760),%a3
    3cb2:	2f0b           	movel %a3,%sp@-
    3cb4:	3f3c 0001      	movew #1,%sp@-
    3cb8:	3f3c 029c      	movew #668,%sp@-
    3cbc:	6100 09ce      	bsrw 0x468c
    3cc0:	6000 004e      	braw 0x3d10
    3cc4:	3205           	movew %d5,%d1
    3cc6:	c3fc 000c      	mulsw #12,%d1
    3cca:	47ec 0100      	lea %a4@(256),%a3
    3cce:	200b           	movel %a3,%d0
    3cd0:	2041           	moveal %d1,%a0
    3cd2:	d1c0           	addal %d0,%a0
    3cd4:	2644           	moveal %d4,%a3
    3cd6:	1e13           	moveb %a3@,%d7
    3cd8:	be28 008b      	cmpb %a0@(139),%d7
    3cdc:	630a           	blss 0x3ce8
    3cde:	0c28 0008 0089 	cmpib #8,%a0@(137)
    3ce4:	6600 fc66      	bnew 0x394c
    3ce8:	47fa fa97      	lea %pc@(0x3781),%a3
    3cec:	2f0b           	movel %a3,%sp@-
    3cee:	3f3c 0001      	movew #1,%sp@-
    3cf2:	3f3c 029c      	movew #668,%sp@-
    3cf6:	6100 0994      	bsrw 0x468c
    3cfa:	3f3c 000d      	movew #13,%sp@-
    3cfe:	6100 2708      	bsrw 0x6408
    3d02:	6000 000c      	braw 0x3d10
    3d06:	5245           	addqw #1,%d5
    3d08:	0c45 0009      	cmpiw #9,%d5
    3d0c:	6f00 fc76      	blew 0x3984
    3d10:	4cee 0cf8 ff64 	moveml %fp@(-156),%d3-%d7/%a2-%a3
    3d16:	4e5e           	unlk %fp
    3d18:	4e75           	rts
    3d1a:	476f           	043557
    3d1c:	7420           	moveq #32,%d2
    3d1e:	6974           	bvss 0x3d94
    3d20:	2100           	movel %d0,%a0@-
    3d22:	4865           	pea %a5@-
    3d24:	7921           	074441
    3d26:	2057           	moveal %sp@,%a0
    3d28:	6861           	bvcs 0x3d8b
    3d2a:	7427           	moveq #39,%d2
    3d2c:	7320           	071440
    3d2e:	7468           	moveq #104,%d2
    3d30:	6973           	bvss 0x3da5
    3d32:	3f00           	movew %d0,%sp@-
    3d34:	4168           	040550
    3d36:	6121           	bsrs 0x3d59
    3d38:	0049 276d      	oriw #10093,%a1
    3d3c:	2063           	moveal %a3@-,%a0
    3d3e:	6172           	bsrs 0x3db2
    3d40:	7279           	moveq #121,%d1
    3d42:	696e           	bvss 0x3db2
    3d44:	6720           	beqs 0x3d66
    3d46:	746f           	moveq #111,%d2
    3d48:	6f20           	bles 0x3d6a
    3d4a:	6d75           	blts 0x3dc1
    3d4c:	6368           	blss 0x3db6
    3d4e:	2073 7475      	moveal %a3@(00000075,%d7:w:4),%a0
    3d52:	6666           	bnes 0x3dba
    3d54:	2100           	movel %d0,%a0@-
    3d56:	4e56 0000      	linkw %fp,#0
    3d5a:	48e7 1c20      	moveml %d3-%d5/%a2,%sp@-
    3d5e:	4243           	clrw %d3
    3d60:	382e 0008      	movew %fp@(8),%d4
    3d64:	c9fc 000c      	mulsw #12,%d4
    3d68:	3203           	movew %d3,%d1
    3d6a:	c3fc 000c      	mulsw #12,%d1
    3d6e:	43ec 0740      	lea %a4@(1856),%a1
    3d72:	0c31 00fe 1800 	cmpib #-2,%a1@(00000000,%d1:l)
    3d78:	6600 0108      	bnew 0x3e82
    3d7c:	41ec 0100      	lea %a4@(256),%a0
    3d80:	41f0 4800      	lea %a0@(00000000,%d4:l),%a0
    3d84:	13a8 0084 1800 	moveb %a0@(132),%a1@(00000000,%d1:l)
    3d8a:	13a8 0085 1801 	moveb %a0@(133),%a1@(00000001,%d1:l)
    3d90:	13a8 0086 1802 	moveb %a0@(134),%a1@(00000002,%d1:l)
    3d96:	13a8 0087 1803 	moveb %a0@(135),%a1@(00000003,%d1:l)
    3d9c:	13a8 0088 1804 	moveb %a0@(136),%a1@(00000004,%d1:l)
    3da2:	13a8 0089 1805 	moveb %a0@(137),%a1@(00000005,%d1:l)
    3da8:	13a8 008a 1806 	moveb %a0@(138),%a1@(00000006,%d1:l)
    3dae:	1431 1806      	moveb %a1@(00000006,%d1:l),%d2
    3db2:	1002           	moveb %d2,%d0
    3db4:	5f00           	subqb #7,%d0
    3db6:	0c00 0001      	cmpib #1,%d0
    3dba:	6306           	blss 0x3dc2
    3dbc:	0c02 0009      	cmpib #9,%d2
    3dc0:	6604           	bnes 0x3dc6
    3dc2:	4231 1806      	clrb %a1@(00000006,%d1:l)
    3dc6:	43ec 0740      	lea %a4@(1856),%a1
    3dca:	3003           	movew %d3,%d0
    3dcc:	c1fc 000c      	mulsw #12,%d0
    3dd0:	41ec 0100      	lea %a4@(256),%a0
    3dd4:	41f0 4800      	lea %a0@(00000000,%d4:l),%a0
    3dd8:	13a8 008b 0807 	moveb %a0@(139),%a1@(00000007,%d0:l)
    3dde:	13a8 008c 0808 	moveb %a0@(140),%a1@(00000008,%d0:l)
    3de4:	13a8 008d 0809 	moveb %a0@(141),%a1@(00000009,%d0:l)
    3dea:	13a8 008e 080a 	moveb %a0@(142),%a1@(0000000a,%d0:l)
    3df0:	13a8 008f 080b 	moveb %a0@(143),%a1@(0000000b,%d0:l)
    3df6:	117c 00fe 0084 	moveb #-2,%a0@(132)
    3dfc:	1031 080b      	moveb %a1@(0000000b,%d0:l),%d0
    3e00:	6662           	bnes 0x3e64
    3e02:	42a7           	clrl %sp@-
    3e04:	4e4f           	trap #15
    3e06:	a0c2           	0120302
    3e08:	7a03           	moveq #3,%d5
    3e0a:	48c0           	extl %d0
    3e0c:	81c5           	divsw %d5,%d0
    3e0e:	2200           	movel %d0,%d1
    3e10:	4841           	swap %d1
    3e12:	584f           	addqw #4,%sp
    3e14:	0c41 0001      	cmpiw #1,%d1
    3e18:	6726           	beqs 0x3e40
    3e1a:	6e08           	bgts 0x3e24
    3e1c:	4a41           	tstw %d1
    3e1e:	670e           	beqs 0x3e2e
    3e20:	6000 0054      	braw 0x3e76
    3e24:	0c41 0002      	cmpiw #2,%d1
    3e28:	6728           	beqs 0x3e52
    3e2a:	6000 004a      	braw 0x3e76
    3e2e:	45fa feea      	lea %pc@(0x3d1a),%a2
    3e32:	2f0a           	movel %a2,%sp@-
    3e34:	3f3c 0001      	movew #1,%sp@-
    3e38:	3f3c 029c      	movew #668,%sp@-
    3e3c:	6000 0032      	braw 0x3e70
    3e40:	45fa fee0      	lea %pc@(0x3d22),%a2
    3e44:	2f0a           	movel %a2,%sp@-
    3e46:	3f3c 0001      	movew #1,%sp@-
    3e4a:	3f3c 029c      	movew #668,%sp@-
    3e4e:	6000 0020      	braw 0x3e70
    3e52:	45fa fee0      	lea %pc@(0x3d34),%a2
    3e56:	2f0a           	movel %a2,%sp@-
    3e58:	3f3c 0001      	movew #1,%sp@-
    3e5c:	3f3c 029c      	movew #668,%sp@-
    3e60:	6000 000e      	braw 0x3e70
    3e64:	42a7           	clrl %sp@-
    3e66:	3f3c 0001      	movew #1,%sp@-
    3e6a:	4267           	clrw %sp@-
    3e6c:	1f40 0001      	moveb %d0,%sp@(1)
    3e70:	6100 081a      	bsrw 0x468c
    3e74:	504f           	addqw #8,%sp
    3e76:	3f3c 0009      	movew #9,%sp@-
    3e7a:	6100 258c      	bsrw 0x6408
    3e7e:	6000 001e      	braw 0x3e9e
    3e82:	5243           	addqw #1,%d3
    3e84:	0c43 0009      	cmpiw #9,%d3
    3e88:	6f00 fede      	blew 0x3d68
    3e8c:	45fa feab      	lea %pc@(0x3d39),%a2
    3e90:	2f0a           	movel %a2,%sp@-
    3e92:	3f3c 0001      	movew #1,%sp@-
    3e96:	3f3c 029c      	movew #668,%sp@-
    3e9a:	6100 07f0      	bsrw 0x468c
    3e9e:	4cee 0438 fff0 	moveml %fp@(-16),%d3-%d5/%a2
    3ea4:	4e5e           	unlk %fp
    3ea6:	4e75           	rts
    3ea8:	4e56 0000      	linkw %fp,#0
    3eac:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    3eb0:	41ec 00d0      	lea %a4@(208),%a0
    3eb4:	0c28 0003 0006 	cmpib #3,%a0@(6)
    3eba:	6700 00c8      	beqw 0x3f84
    3ebe:	43ec 0100      	lea %a4@(256),%a1
    3ec2:	45e9 0106      	lea %a1@(262),%a2
    3ec6:	3228 0002      	movew %a0@(2),%d1
    3eca:	3001           	movew %d1,%d0
    3ecc:	9052           	subw %a2@,%d0
    3ece:	6a02           	bpls 0x3ed2
    3ed0:	4440           	negw %d0
    3ed2:	0c40 000f      	cmpiw #15,%d0
    3ed6:	6e00 00de      	bgtw 0x3fb6
    3eda:	3001           	movew %d1,%d0
    3edc:	0640 0020      	addiw #32,%d0
    3ee0:	6a06           	bpls 0x3ee8
    3ee2:	3001           	movew %d1,%d0
    3ee4:	0640 002f      	addiw #47,%d0
    3ee8:	3800           	movew %d0,%d4
    3eea:	e844           	asrw #4,%d4
    3eec:	3210           	movew %a0@,%d1
    3eee:	3001           	movew %d1,%d0
    3ef0:	0640 0010      	addiw #16,%d0
    3ef4:	6a06           	bpls 0x3efc
    3ef6:	3001           	movew %d1,%d0
    3ef8:	0640 001f      	addiw #31,%d0
    3efc:	3400           	movew %d0,%d2
    3efe:	e842           	asrw #4,%d2
    3f00:	322a fffe      	movew %a2@(-2),%d1
    3f04:	3001           	movew %d1,%d0
    3f06:	0640 0010      	addiw #16,%d0
    3f0a:	6a06           	bpls 0x3f12
    3f0c:	3001           	movew %d1,%d0
    3f0e:	0640 001f      	addiw #31,%d0
    3f12:	3600           	movew %d0,%d3
    3f14:	e843           	asrw #4,%d3
    3f16:	b642           	cmpw %d2,%d3
    3f18:	673c           	beqs 0x3f56
    3f1a:	45e9 001e      	lea %a1@(30),%a2
    3f1e:	3244           	moveaw %d4,%a1
    3f20:	3002           	movew %d2,%d0
    3f22:	48c0           	extl %d0
    3f24:	2200           	movel %d0,%d1
    3f26:	e781           	asll #3,%d1
    3f28:	d280           	addl %d0,%d1
    3f2a:	d280           	addl %d0,%d1
    3f2c:	41f2 1800      	lea %a2@(00000000,%d1:l),%a0
    3f30:	1031 8800      	moveb %a1@(00000000,%a0:l),%d0
    3f34:	0c00 000d      	cmpib #13,%d0
    3f38:	677c           	beqs 0x3fb6
    3f3a:	0c00 000f      	cmpib #15,%d0
    3f3e:	6776           	beqs 0x3fb6
    3f40:	0c00 0011      	cmpib #17,%d0
    3f44:	6770           	beqs 0x3fb6
    3f46:	b642           	cmpw %d2,%d3
    3f48:	6f06           	bles 0x3f50
    3f4a:	5242           	addqw #1,%d2
    3f4c:	6000 0004      	braw 0x3f52
    3f50:	5342           	subqw #1,%d2
    3f52:	b642           	cmpw %d2,%d3
    3f54:	66ca           	bnes 0x3f20
    3f56:	3002           	movew %d2,%d0
    3f58:	48c0           	extl %d0
    3f5a:	41ec 0100      	lea %a4@(256),%a0
    3f5e:	2200           	movel %d0,%d1
    3f60:	e781           	asll #3,%d1
    3f62:	d280           	addl %d0,%d1
    3f64:	d280           	addl %d0,%d1
    3f66:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    3f6a:	1030 4000      	moveb %a0@(00000000,%d4:w),%d0
    3f6e:	0c00 000d      	cmpib #13,%d0
    3f72:	6742           	beqs 0x3fb6
    3f74:	0c00 000f      	cmpib #15,%d0
    3f78:	673c           	beqs 0x3fb6
    3f7a:	0c00 0011      	cmpib #17,%d0
    3f7e:	6736           	beqs 0x3fb6
    3f80:	6000 002e      	braw 0x3fb0
    3f84:	47ec 0100      	lea %a4@(256),%a3
    3f88:	200b           	movel %a3,%d0
    3f8a:	2240           	moveal %d0,%a1
    3f8c:	d2fc 0106      	addaw #262,%a1
    3f90:	3028 0002      	movew %a0@(2),%d0
    3f94:	9051           	subw %a1@,%d0
    3f96:	6a02           	bpls 0x3f9a
    3f98:	4440           	negw %d0
    3f9a:	0c40 0023      	cmpiw #35,%d0
    3f9e:	6e16           	bgts 0x3fb6
    3fa0:	3010           	movew %a0@,%d0
    3fa2:	9069 fffe      	subw %a1@(-2),%d0
    3fa6:	6a02           	bpls 0x3faa
    3fa8:	4440           	negw %d0
    3faa:	0c40 0031      	cmpiw #49,%d0
    3fae:	6e06           	bgts 0x3fb6
    3fb0:	7001           	moveq #1,%d0
    3fb2:	6000 0004      	braw 0x3fb8
    3fb6:	4240           	clrw %d0
    3fb8:	4cee 0c18 fff0 	moveml %fp@(-16),%d3-%d4/%a2-%a3
    3fbe:	4e5e           	unlk %fp
    3fc0:	4e75           	rts
    3fc2:	4465           	negw %a5@-
    3fc4:	6174           	bsrs 0x403a
    3fc6:	6800 4e56      	bvcw 0x8e1e
    3fca:	0000 48e7      	orib #-25,%d0
    3fce:	1830 266e      	moveb %a0@(0000006e,%d2:w:8),%d4
    3fd2:	0008 262e      	orib #46,%a0
    3fd6:	000c 102b      	orib #43,%a4
    3fda:	0005 0c00      	orib #0,%d5
    3fde:	0006 670e      	orib #14,%d6
    3fe2:	5f00           	subqb #7,%d0
    3fe4:	0c00 0001      	cmpib #1,%d0
    3fe8:	6306           	blss 0x3ff0
    3fea:	4a2b 001f      	tstb %a3@(31)
    3fee:	670c           	beqs 0x3ffc
    3ff0:	2f03           	movel %d3,%sp@-
    3ff2:	2f0b           	movel %a3,%sp@-
    3ff4:	6100 1ac4      	bsrw 0x5aba
    3ff8:	6000 0396      	braw 0x4390
    3ffc:	3f3c 0006      	movew #6,%sp@-
    4000:	6100 2406      	bsrw 0x6408
    4004:	544f           	addqw #2,%sp
    4006:	0c2b 0003 0006 	cmpib #3,%a3@(6)
    400c:	6720           	beqs 0x402e
    400e:	177c 0008 0005 	moveb #8,%a3@(5)
    4014:	177c 0001 0021 	moveb #1,%a3@(33)
    401a:	0c2b 0001 0008 	cmpib #1,%a3@(8)
    4020:	6606           	bnes 0x4028
    4022:	5b53           	subqw #5,%a3@
    4024:	6000 0024      	braw 0x404a
    4028:	5a53           	addqw #5,%a3@
    402a:	6000 001e      	braw 0x404a
    402e:	4a2b 0009      	tstb %a3@(9)
    4032:	670a           	beqs 0x403e
    4034:	177c 0006 0005 	moveb #6,%a3@(5)
    403a:	6000 0008      	braw 0x4044
    403e:	177c 0007 0005 	moveb #7,%a3@(5)
    4044:	177c 0002 0021 	moveb #2,%a3@(33)
    404a:	43ec 0100      	lea %a4@(256),%a1
    404e:	2009           	movel %a1,%d0
    4050:	2040           	moveal %d0,%a0
    4052:	d0fc 0108      	addaw #264,%a0
    4056:	4a10           	tstb %a0@
    4058:	6700 02ce      	beqw 0x4328
    405c:	4a28 0008      	tstb %a0@(8)
    4060:	6700 02c6      	beqw 0x4328
    4064:	41ec 00d0      	lea %a4@(208),%a0
    4068:	4a28 000c      	tstb %a0@(12)
    406c:	6700 02ba      	beqw 0x4328
    4070:	6100 fe36      	bsrw 0x3ea8
    4074:	4a00           	tstb %d0
    4076:	6700 02b0      	beqw 0x4328
    407a:	0c2b 0003 0006 	cmpib #3,%a3@(6)
    4080:	6608           	bnes 0x408a
    4082:	6100 fe24      	bsrw 0x3ea8
    4086:	4a00           	tstb %d0
    4088:	6706           	beqs 0x4090
    408a:	197c 0001 000f 	moveb #1,%a4@(15)
    4090:	4a2b 001e      	tstb %a3@(30)
    4094:	6772           	beqs 0x4108
    4096:	41ec 0015      	lea %a4@(21),%a0
    409a:	4a10           	tstb %a0@
    409c:	666a           	bnes 0x4108
    409e:	0c2b 0003 0006 	cmpib #3,%a3@(6)
    40a4:	6700 02ea      	beqw 0x4390
    40a8:	10bc 0001      	moveb #1,%a0@
    40ac:	3953 0018      	movew %a3@,%a4@(24)
    40b0:	382b 0002      	movew %a3@(2),%d4
    40b4:	5444           	addqw #2,%d4
    40b6:	3944 001a      	movew %d4,%a4@(26)
    40ba:	0c2b 0001 0008 	cmpib #1,%a3@(8)
    40c0:	660a           	bnes 0x40cc
    40c2:	397c fff6 001c 	movew #-10,%a4@(28)
    40c8:	6000 0008      	braw 0x40d2
    40cc:	397c 000a 001c 	movew #10,%a4@(28)
    40d2:	45ec 001e      	lea %a4@(30),%a2
    40d6:	42a7           	clrl %sp@-
    40d8:	4e4f           	trap #15
    40da:	a0c2           	0120302
    40dc:	781e           	moveq #30,%d4
    40de:	48c0           	extl %d0
    40e0:	81c4           	divsw %d4,%d0
    40e2:	2200           	movel %d0,%d1
    40e4:	4841           	swap %d1
    40e6:	3001           	movew %d1,%d0
    40e8:	c1fc 6667      	mulsw #26215,%d0
    40ec:	4240           	clrw %d0
    40ee:	4840           	swap %d0
    40f0:	e440           	asrw #2,%d0
    40f2:	780f           	moveq #15,%d4
    40f4:	e861           	asrw %d4,%d1
    40f6:	9041           	subw %d1,%d0
    40f8:	5340           	subqw #1,%d0
    40fa:	3480           	movew %d0,%a2@
    40fc:	3f3c 0007      	movew #7,%sp@-
    4100:	6100 2306      	bsrw 0x6408
    4104:	6000 028a      	braw 0x4390
    4108:	3413           	movew %a3@,%d2
    410a:	2243           	moveal %d3,%a1
    410c:	3211           	movew %a1@,%d1
    410e:	3002           	movew %d2,%d0
    4110:	9041           	subw %d1,%d0
    4112:	6a02           	bpls 0x4116
    4114:	4440           	negw %d0
    4116:	0c40 001d      	cmpiw #29,%d0
    411a:	6e00 01b4      	bgtw 0x42d0
    411e:	9242           	subw %d2,%d1
    4120:	3001           	movew %d1,%d0
    4122:	6c02           	bges 0x4126
    4124:	4440           	negw %d0
    4126:	780a           	moveq #10,%d4
    4128:	9840           	subw %d0,%d4
    412a:	3004           	movew %d4,%d0
    412c:	6c02           	bges 0x4130
    412e:	4440           	negw %d0
    4130:	e540           	aslw #2,%d0
    4132:	7864           	moveq #100,%d4
    4134:	9840           	subw %d0,%d4
    4136:	3444           	moveaw %d4,%a2
    4138:	0c2b 0003 0006 	cmpib #3,%a3@(6)
    413e:	6642           	bnes 0x4182
    4140:	3001           	movew %d1,%d0
    4142:	6c02           	bges 0x4146
    4144:	4440           	negw %d0
    4146:	0c40 0014      	cmpiw #20,%d0
    414a:	6f10           	bles 0x415c
    414c:	340a           	movew %a2,%d2
    414e:	0642 0014      	addiw #20,%d2
    4152:	3001           	movew %d1,%d0
    4154:	6c02           	bges 0x4158
    4156:	4440           	negw %d0
    4158:	3442           	moveaw %d2,%a2
    415a:	94c0           	subaw %d0,%a2
    415c:	2243           	moveal %d3,%a1
    415e:	3029 0002      	movew %a1@(2),%d0
    4162:	906b 0002      	subw %a3@(2),%d0
    4166:	3200           	movew %d0,%d1
    4168:	6c02           	bges 0x416c
    416a:	4441           	negw %d1
    416c:	0c41 0014      	cmpiw #20,%d1
    4170:	6f10           	bles 0x4182
    4172:	320a           	movew %a2,%d1
    4174:	0641 0014      	addiw #20,%d1
    4178:	4a40           	tstw %d0
    417a:	6c02           	bges 0x417e
    417c:	4440           	negw %d0
    417e:	3441           	moveaw %d1,%a2
    4180:	94c0           	subaw %d0,%a2
    4182:	4240           	clrw %d0
    4184:	102b 0011      	moveb %a3@(17),%d0
    4188:	d4c0           	addaw %d0,%a2
    418a:	2243           	moveal %d3,%a1
    418c:	4240           	clrw %d0
    418e:	1029 0010      	moveb %a1@(16),%d0
    4192:	94c0           	subaw %d0,%a2
    4194:	42a7           	clrl %sp@-
    4196:	4e4f           	trap #15
    4198:	a0c2           	0120302
    419a:	7864           	moveq #100,%d4
    419c:	48c0           	extl %d0
    419e:	81c4           	divsw %d4,%d0
    41a0:	2200           	movel %d0,%d1
    41a2:	4841           	swap %d1
    41a4:	584f           	addqw #4,%sp
    41a6:	b24a           	cmpw %a2,%d1
    41a8:	6c00 0126      	bgew 0x42d0
    41ac:	3f3c 000b      	movew #11,%sp@-
    41b0:	6100 2256      	bsrw 0x6408
    41b4:	544f           	addqw #2,%sp
    41b6:	2243           	moveal %d3,%a1
    41b8:	0c29 0003 0006 	cmpib #3,%a1@(6)
    41be:	674e           	beqs 0x420e
    41c0:	137c 000d 0005 	moveb #13,%a1@(5)
    41c6:	137c 0003 0021 	moveb #3,%a1@(33)
    41cc:	2f03           	movel %d3,%sp@-
    41ce:	6100 efd8      	bsrw 0x31a8
    41d2:	584f           	addqw #4,%sp
    41d4:	2243           	moveal %d3,%a1
    41d6:	0c29 0003 0006 	cmpib #3,%a1@(6)
    41dc:	6730           	beqs 0x420e
    41de:	0c2b 0001 0008 	cmpib #1,%a3@(8)
    41e4:	6612           	bnes 0x41f8
    41e6:	3011           	movew %a1@,%d0
    41e8:	6f04           	bles 0x41ee
    41ea:	5540           	subqw #2,%d0
    41ec:	3280           	movew %d0,%a1@
    41ee:	2f03           	movel %d3,%sp@-
    41f0:	6100 e680      	bsrw 0x2872
    41f4:	6000 0016      	braw 0x420c
    41f8:	2243           	moveal %d3,%a1
    41fa:	3011           	movew %a1@,%d0
    41fc:	0c40 007f      	cmpiw #127,%d0
    4200:	6e04           	bgts 0x4206
    4202:	5440           	addqw #2,%d0
    4204:	3280           	movew %d0,%a1@
    4206:	2f03           	movel %d3,%sp@-
    4208:	6100 e96c      	bsrw 0x2b76
    420c:	584f           	addqw #4,%sp
    420e:	42a7           	clrl %sp@-
    4210:	4e4f           	trap #15
    4212:	a0c2           	0120302
    4214:	4241           	clrw %d1
    4216:	122b 0014      	moveb %a3@(20),%d1
    421a:	48c0           	extl %d0
    421c:	81c1           	divsw %d1,%d0
    421e:	2200           	movel %d0,%d1
    4220:	4841           	swap %d1
    4222:	5241           	addqw #1,%d1
    4224:	2243           	moveal %d3,%a1
    4226:	1429 000c      	moveb %a1@(12),%d2
    422a:	4240           	clrw %d0
    422c:	1002           	moveb %d2,%d0
    422e:	584f           	addqw #4,%sp
    4230:	b240           	cmpw %d0,%d1
    4232:	6d00 0094      	bltw 0x42c8
    4236:	4229 000c      	clrb %a1@(12)
    423a:	0c29 0003 0006 	cmpib #3,%a1@(6)
    4240:	6608           	bnes 0x424a
    4242:	2f03           	movel %d3,%sp@-
    4244:	6100 ef62      	bsrw 0x31a8
    4248:	584f           	addqw #4,%sp
    424a:	2243           	moveal %d3,%a1
    424c:	137c 0006 0005 	moveb #6,%a1@(5)
    4252:	137c 0005 0021 	moveb #5,%a1@(33)
    4258:	197c 0023 000b 	moveb #35,%a4@(11)
    425e:	422c 000f      	clrb %a4@(15)
    4262:	41ec 00d0      	lea %a4@(208),%a0
    4266:	4a28 000c      	tstb %a0@(12)
    426a:	6614           	bnes 0x4280
    426c:	43fa fd54      	lea %pc@(0x3fc2),%a1
    4270:	2f09           	movel %a1,%sp@-
    4272:	486c 0098      	pea %a4@(152)
    4276:	4e4f           	trap #15
    4278:	a2de           	0121336
    427a:	504f           	addqw #8,%sp
    427c:	6000 0052      	braw 0x42d0
    4280:	3f3c 0008      	movew #8,%sp@-
    4284:	6100 2182      	bsrw 0x6408
    4288:	43ec 0100      	lea %a4@(256),%a1
    428c:	2009           	movel %a1,%d0
    428e:	2440           	moveal %d0,%a2
    4290:	d4fc 011e      	addaw #286,%a2
    4294:	544f           	addqw #2,%sp
    4296:	1012           	moveb %a2@,%d0
    4298:	6736           	beqs 0x42d0
    429a:	0c2a 0001 fffc 	cmpib #1,%a2@(-4)
    42a0:	622e           	bhis 0x42d0
    42a2:	42a7           	clrl %sp@-
    42a4:	3f3c 0002      	movew #2,%sp@-
    42a8:	4267           	clrw %sp@-
    42aa:	1f40 0001      	moveb %d0,%sp@(1)
    42ae:	6100 03dc      	bsrw 0x468c
    42b2:	4212           	clrb %a2@
    42b4:	504f           	addqw #8,%sp
    42b6:	6000 0018      	braw 0x42d0
    42ba:	117c 0001 008a 	moveb #1,%a0@(138)
    42c0:	5328 0084      	subqb #1,%a0@(132)
    42c4:	6000 00ca      	braw 0x4390
    42c8:	9401           	subb %d1,%d2
    42ca:	2243           	moveal %d3,%a1
    42cc:	1342 000c      	moveb %d2,%a1@(12)
    42d0:	43ec 0100      	lea %a4@(256),%a1
    42d4:	2009           	movel %a1,%d0
    42d6:	2040           	moveal %d0,%a0
    42d8:	d0fc 011a      	addaw #282,%a0
    42dc:	4a10           	tstb %a0@
    42de:	6748           	beqs 0x4328
    42e0:	1028 0005      	moveb %a0@(5),%d0
    42e4:	671e           	beqs 0x4304
    42e6:	45ec 0014      	lea %a4@(20),%a2
    42ea:	4a12           	tstb %a2@
    42ec:	661c           	bnes 0x430a
    42ee:	42a7           	clrl %sp@-
    42f0:	3f3c 0002      	movew #2,%sp@-
    42f4:	4267           	clrw %sp@-
    42f6:	1f40 0001      	moveb %d0,%sp@(1)
    42fa:	6100 0390      	bsrw 0x468c
    42fe:	14bc 0046      	moveb #70,%a2@
    4302:	504f           	addqw #8,%sp
    4304:	4a2c 0014      	tstb %a4@(20)
    4308:	671e           	beqs 0x4328
    430a:	41ec 0100      	lea %a4@(256),%a0
    430e:	45e8 011a      	lea %a0@(282),%a2
    4312:	0c12 0013      	cmpib #19,%a2@
    4316:	6310           	blss 0x4328
    4318:	0c2c 0013 0014 	cmpib #19,%a4@(20)
    431e:	6208           	bhis 0x4328
    4320:	4212           	clrb %a2@
    4322:	317c 00fe 0116 	movew #254,%a0@(278)
    4328:	2f0b           	movel %a3,%sp@-
    432a:	6100 e1c8      	bsrw 0x24f4
    432e:	2f03           	movel %d3,%sp@-
    4330:	6100 e1c2      	bsrw 0x24f4
    4334:	4242           	clrw %d2
    4336:	4243           	clrw %d3
    4338:	3202           	movew %d2,%d1
    433a:	c3fc 000c      	mulsw #12,%d1
    433e:	43ec 0100      	lea %a4@(256),%a1
    4342:	2009           	movel %a1,%d0
    4344:	2041           	moveal %d1,%a0
    4346:	d1c0           	addal %d0,%a0
    4348:	0c28 00fe 0084 	cmpib #-2,%a0@(132)
    434e:	6738           	beqs 0x4388
    4350:	1628 0086      	moveb %a0@(134),%d3
    4354:	3003           	movew %d3,%d0
    4356:	9053           	subw %a3@,%d0
    4358:	6a02           	bpls 0x435c
    435a:	4440           	negw %d0
    435c:	0c40 000f      	cmpiw #15,%d0
    4360:	6e26           	bgts 0x4388
    4362:	4240           	clrw %d0
    4364:	1028 0087      	moveb %a0@(135),%d0
    4368:	906b 0002      	subw %a3@(2),%d0
    436c:	6a02           	bpls 0x4370
    436e:	4440           	negw %d0
    4370:	0c40 002f      	cmpiw #47,%d0
    4374:	6e12           	bgts 0x4388
    4376:	0c28 0001 0089 	cmpib #1,%a0@(137)
    437c:	660a           	bnes 0x4388
    437e:	0c28 0003 008a 	cmpib #3,%a0@(138)
    4384:	6700 ff34      	beqw 0x42ba
    4388:	5242           	addqw #1,%d2
    438a:	0c42 0009      	cmpiw #9,%d2
    438e:	6fa8           	bles 0x4338
    4390:	4cee 0c18 fff0 	moveml %fp@(-16),%d3-%d4/%a2-%a3
    4396:	4e5e           	unlk %fp
    4398:	4e75           	rts
    439a:	4e56 ff00      	linkw %fp,#-256
    439e:	2f0a           	movel %a2,%sp@-
    43a0:	2f03           	movel %d3,%sp@-
    43a2:	246e 0008      	moveal %fp@(8),%a2
    43a6:	362e 000c      	movew %fp@(12),%d3
    43aa:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    43b0:	6748           	beqs 0x43fa
    43b2:	157c 000d 0005 	moveb #13,%a2@(5)
    43b8:	157c 0003 0021 	moveb #3,%a2@(33)
    43be:	2f0a           	movel %a2,%sp@-
    43c0:	6100 ede6      	bsrw 0x31a8
    43c4:	584f           	addqw #4,%sp
    43c6:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    43cc:	672c           	beqs 0x43fa
    43ce:	4a2a 0008      	tstb %a2@(8)
    43d2:	6612           	bnes 0x43e6
    43d4:	3012           	movew %a2@,%d0
    43d6:	6f04           	bles 0x43dc
    43d8:	5540           	subqw #2,%d0
    43da:	3480           	movew %d0,%a2@
    43dc:	2f0a           	movel %a2,%sp@-
    43de:	6100 e492      	bsrw 0x2872
    43e2:	6000 0014      	braw 0x43f8
    43e6:	3012           	movew %a2@,%d0
    43e8:	0c40 007f      	cmpiw #127,%d0
    43ec:	6e04           	bgts 0x43f2
    43ee:	5440           	addqw #2,%d0
    43f0:	3480           	movew %d0,%a2@
    43f2:	2f0a           	movel %a2,%sp@-
    43f4:	6100 e780      	bsrw 0x2b76
    43f8:	584f           	addqw #4,%sp
    43fa:	42a7           	clrl %sp@-
    43fc:	4e4f           	trap #15
    43fe:	a0c2           	0120302
    4400:	48c0           	extl %d0
    4402:	81c3           	divsw %d3,%d0
    4404:	2200           	movel %d0,%d1
    4406:	4841           	swap %d1
    4408:	5241           	addqw #1,%d1
    440a:	142a 000c      	moveb %a2@(12),%d2
    440e:	4240           	clrw %d0
    4410:	1002           	moveb %d2,%d0
    4412:	584f           	addqw #4,%sp
    4414:	b240           	cmpw %d0,%d1
    4416:	6d00 008e      	bltw 0x44a6
    441a:	422a 000c      	clrb %a2@(12)
    441e:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    4424:	6608           	bnes 0x442e
    4426:	2f0a           	movel %a2,%sp@-
    4428:	6100 ed7e      	bsrw 0x31a8
    442c:	584f           	addqw #4,%sp
    442e:	157c 0006 0005 	moveb #6,%a2@(5)
    4434:	157c 0005 0021 	moveb #5,%a2@(33)
    443a:	197c 0023 000b 	moveb #35,%a4@(11)
    4440:	422c 000f      	clrb %a4@(15)
    4444:	41ec 00d0      	lea %a4@(208),%a0
    4448:	4a28 000c      	tstb %a0@(12)
    444c:	661e           	bnes 0x446c
    444e:	43fa fb72      	lea %pc@(0x3fc2),%a1
    4452:	2f09           	movel %a1,%sp@-
    4454:	486c 0098      	pea %a4@(152)
    4458:	4e4f           	trap #15
    445a:	a2de           	0121336
    445c:	3f3c 000c      	movew #12,%sp@-
    4460:	6100 1fa6      	bsrw 0x6408
    4464:	defc 000a      	addaw #10,%sp
    4468:	6000 0042      	braw 0x44ac
    446c:	3f3c 0008      	movew #8,%sp@-
    4470:	6100 1f96      	bsrw 0x6408
    4474:	43ec 0100      	lea %a4@(256),%a1
    4478:	2009           	movel %a1,%d0
    447a:	2440           	moveal %d0,%a2
    447c:	d4fc 011e      	addaw #286,%a2
    4480:	544f           	addqw #2,%sp
    4482:	1012           	moveb %a2@,%d0
    4484:	6726           	beqs 0x44ac
    4486:	0c2a 0001 fffc 	cmpib #1,%a2@(-4)
    448c:	621e           	bhis 0x44ac
    448e:	42a7           	clrl %sp@-
    4490:	3f3c 0002      	movew #2,%sp@-
    4494:	4267           	clrw %sp@-
    4496:	1f40 0001      	moveb %d0,%sp@(1)
    449a:	6100 01f0      	bsrw 0x468c
    449e:	4212           	clrb %a2@
    44a0:	504f           	addqw #8,%sp
    44a2:	6000 0008      	braw 0x44ac
    44a6:	9401           	subb %d1,%d2
    44a8:	1542 000c      	moveb %d2,%a2@(12)
    44ac:	3f3c 000b      	movew #11,%sp@-
    44b0:	6100 1f56      	bsrw 0x6408
    44b4:	262e fef8      	movel %fp@(-264),%d3
    44b8:	246e fefc      	moveal %fp@(-260),%a2
    44bc:	4e5e           	unlk %fp
    44be:	4e75           	rts
    44c0:	4e56 fff8      	linkw %fp,#-8
    44c4:	2f03           	movel %d3,%sp@-
    44c6:	2f2c 0084      	movel %a4@(132),%sp@-
    44ca:	4e4f           	trap #15
    44cc:	a1fd           	0120775
    44ce:	2608           	movel %a0,%d3
    44d0:	584f           	addqw #4,%sp
    44d2:	4a2c 000f      	tstb %a4@(15)
    44d6:	6608           	bnes 0x44e0
    44d8:	4a2c 000b      	tstb %a4@(11)
    44dc:	6700 00bc      	beqw 0x459a
    44e0:	3f3c 03f2      	movew #1010,%sp@-
    44e4:	6100 06c8      	bsrw 0x4bae
    44e8:	41ec 00d0      	lea %a4@(208),%a0
    44ec:	4240           	clrw %d0
    44ee:	1028 000c      	moveb %a0@(12),%d0
    44f2:	c1fc 0064      	mulsw #100,%d0
    44f6:	4241           	clrw %d1
    44f8:	1228 000b      	moveb %a0@(11),%d1
    44fc:	48c0           	extl %d0
    44fe:	81c1           	divsw %d1,%d0
    4500:	3f00           	movew %d0,%sp@-
    4502:	3f3c 002b      	movew #43,%sp@-
    4506:	3f3c 0004      	movew #4,%sp@-
    450a:	3f3c 0019      	movew #25,%sp@-
    450e:	6100 0b2c      	bsrw 0x503c
    4512:	43ec 0100      	lea %a4@(256),%a1
    4516:	2009           	movel %a1,%d0
    4518:	2040           	moveal %d0,%a0
    451a:	d0fc 0108      	addaw #264,%a0
    451e:	defc 000a      	addaw #10,%sp
    4522:	4a10           	tstb %a0@
    4524:	672c           	beqs 0x4552
    4526:	4240           	clrw %d0
    4528:	1028 0008      	moveb %a0@(8),%d0
    452c:	c1fc 0064      	mulsw #100,%d0
    4530:	4241           	clrw %d1
    4532:	1228 0007      	moveb %a0@(7),%d1
    4536:	48c0           	extl %d0
    4538:	81c1           	divsw %d1,%d0
    453a:	3f00           	movew %d0,%sp@-
    453c:	3f3c 002b      	movew #43,%sp@-
    4540:	3f3c 0004      	movew #4,%sp@-
    4544:	3f3c 0059      	movew #89,%sp@-
    4548:	6100 0af2      	bsrw 0x503c
    454c:	504f           	addqw #8,%sp
    454e:	6000 003a      	braw 0x458a
    4552:	4280           	clrl %d0
    4554:	302e fffa      	movew %fp@(-6),%d0
    4558:	0080 0058 0000 	oril #5767168,%d0
    455e:	2d40 fff8      	movel %d0,%fp@(-8)
    4562:	3d7c 0001 fffa 	movew #1,%fp@(-6)
    4568:	4280           	clrl %d0
    456a:	302e fffe      	movew %fp@(-2),%d0
    456e:	0080 0046 0000 	oril #4587520,%d0
    4574:	2d40 fffc      	movel %d0,%fp@(-4)
    4578:	3d7c 0008 fffe 	movew #8,%fp@(-2)
    457e:	4267           	clrw %sp@-
    4580:	486e fff8      	pea %fp@(-8)
    4584:	4e4f           	trap #15
    4586:	a229           	0121051
    4588:	5c4f           	addqw #6,%sp
    458a:	41ec 000b      	lea %a4@(11),%a0
    458e:	1010           	moveb %a0@,%d0
    4590:	671a           	beqs 0x45ac
    4592:	5300           	subqb #1,%d0
    4594:	1080           	moveb %d0,%a0@
    4596:	6000 0014      	braw 0x45ac
    459a:	3f3c 03f1      	movew #1009,%sp@-
    459e:	6100 060e      	bsrw 0x4bae
    45a2:	486c 0098      	pea %a4@(152)
    45a6:	6100 0012      	bsrw 0x45ba
    45aa:	5c4f           	addqw #6,%sp
    45ac:	2f03           	movel %d3,%sp@-
    45ae:	4e4f           	trap #15
    45b0:	a1fd           	0120775
    45b2:	262e fff4      	movel %fp@(-12),%d3
    45b6:	4e5e           	unlk %fp
    45b8:	4e75           	rts
    45ba:	4e56 0000      	linkw %fp,#0
    45be:	48e7 1f00      	moveml %d3-%d7,%sp@-
    45c2:	282e 0008      	movel %fp@(8),%d4
    45c6:	1f3c 0001      	moveb #1,%sp@-
    45ca:	4e4f           	trap #15
    45cc:	a164           	0120544
    45ce:	1e00           	moveb %d0,%d7
    45d0:	2f04           	movel %d4,%sp@-
    45d2:	4e4f           	trap #15
    45d4:	a0c7           	0120307
    45d6:	3c00           	movew %d0,%d6
    45d8:	3f06           	movew %d6,%sp@-
    45da:	2f04           	movel %d4,%sp@-
    45dc:	4e4f           	trap #15
    45de:	a16b           	0120553
    45e0:	363c 00a0      	movew #160,%d3
    45e4:	9640           	subw %d0,%d3
    45e6:	3003           	movew %d3,%d0
    45e8:	720f           	moveq #15,%d1
    45ea:	e268           	lsrw %d1,%d0
    45ec:	d640           	addw %d0,%d3
    45ee:	e243           	asrw #1,%d3
    45f0:	2f2c 0084      	movel %a4@(132),%sp@-
    45f4:	4e4f           	trap #15
    45f6:	a1fd           	0120775
    45f8:	2a08           	movel %a0,%d5
    45fa:	4267           	clrw %sp@-
    45fc:	3f03           	movew %d3,%sp@-
    45fe:	3f06           	movew %d6,%sp@-
    4600:	2f04           	movel %d4,%sp@-
    4602:	4e4f           	trap #15
    4604:	a22a           	0121052
    4606:	2f05           	movel %d5,%sp@-
    4608:	4e4f           	trap #15
    460a:	a1fd           	0120775
    460c:	1f07           	moveb %d7,%sp@-
    460e:	4e4f           	trap #15
    4610:	a164           	0120544
    4612:	4cee 00f8 ffec 	moveml %fp@(-20),%d3-%d7
    4618:	4e5e           	unlk %fp
    461a:	4e75           	rts
    461c:	4e56 0000      	linkw %fp,#0
    4620:	48e7 1e00      	moveml %d3-%d6,%sp@-
    4624:	262e 0008      	movel %fp@(8),%d3
    4628:	3a2e 000c      	movew %fp@(12),%d5
    462c:	1f2e 000f      	moveb %fp@(15),%sp@-
    4630:	4e4f           	trap #15
    4632:	a164           	0120544
    4634:	1c00           	moveb %d0,%d6
    4636:	2f03           	movel %d3,%sp@-
    4638:	4e4f           	trap #15
    463a:	a0c7           	0120307
    463c:	3800           	movew %d0,%d4
    463e:	3f04           	movew %d4,%sp@-
    4640:	2f03           	movel %d3,%sp@-
    4642:	4e4f           	trap #15
    4644:	a16b           	0120553
    4646:	343c 00a0      	movew #160,%d2
    464a:	9440           	subw %d0,%d2
    464c:	3002           	movew %d2,%d0
    464e:	3200           	movew %d0,%d1
    4650:	740f           	moveq #15,%d2
    4652:	e469           	lsrw %d2,%d1
    4654:	d041           	addw %d1,%d0
    4656:	e240           	asrw #1,%d0
    4658:	3f05           	movew %d5,%sp@-
    465a:	3f00           	movew %d0,%sp@-
    465c:	3f04           	movew %d4,%sp@-
    465e:	2f03           	movel %d3,%sp@-
    4660:	4e4f           	trap #15
    4662:	a22a           	0121052
    4664:	1f06           	moveb %d6,%sp@-
    4666:	4e4f           	trap #15
    4668:	a164           	0120544
    466a:	4cee 0078 fff0 	moveml %fp@(-16),%d3-%d6
    4670:	4e5e           	unlk %fp
    4672:	4e75           	rts
    4674:	4e6f           	movel %usp,%sp
    4676:	2054           	moveal %a4@,%a0
    4678:	6578           	bcss 0x46f2
    467a:	7420           	moveq #32,%d2
    467c:	4461           	negw %a1@-
    467e:	7461           	moveq #97,%d2
    4680:	6261           	bhis 0x46e3
    4682:	7365           	071545
    4684:	2046           	moveal %d6,%a0
    4686:	6f75           	bles 0x46fd
    4688:	6e64           	bgts 0x46ee
    468a:	2e00           	movel %d0,%d7
    468c:	4e56 0000      	linkw %fp,#0
    4690:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    4694:	3a2e 0008      	movew %fp@(8),%d5
    4698:	362e 000a      	movew %fp@(10),%d3
    469c:	242e 000c      	movel %fp@(12),%d2
    46a0:	4241           	clrw %d1
    46a2:	43ec 0230      	lea %a4@(560),%a1
    46a6:	2009           	movel %a1,%d0
    46a8:	2640           	moveal %d0,%a3
    46aa:	564b           	addqw #3,%a3
    46ac:	3801           	movew %d1,%d4
    46ae:	c9fc 0102      	mulsw #258,%d4
    46b2:	41ec 0230      	lea %a4@(560),%a0
    46b6:	0c70 00fe 4800 	cmpiw #254,%a0@(00000000,%d4:l)
    46bc:	6600 008c      	bnew 0x474a
    46c0:	3185 4800      	movew %d5,%a0@(00000000,%d4:l)
    46c4:	1183 4802      	moveb %d3,%a0@(00000002,%d4:l)
    46c8:	0c45 029c      	cmpiw #668,%d5
    46cc:	675c           	beqs 0x472a
    46ce:	4a82           	tstl %d2
    46d0:	6658           	bnes 0x472a
    46d2:	45ec 00c0      	lea %a4@(192),%a2
    46d6:	2012           	movel %a2@,%d0
    46d8:	6742           	beqs 0x471c
    46da:	3f05           	movew %d5,%sp@-
    46dc:	2f00           	movel %d0,%sp@-
    46de:	4e4f           	trap #15
    46e0:	a05c           	0120134
    46e2:	2008           	movel %a0,%d0
    46e4:	5c4f           	addqw #6,%sp
    46e6:	6726           	beqs 0x470e
    46e8:	2f00           	movel %d0,%sp@-
    46ea:	4e4f           	trap #15
    46ec:	a021           	0120041
    46ee:	2608           	movel %a0,%d3
    46f0:	2f03           	movel %d3,%sp@-
    46f2:	4873 4800      	pea %a3@(00000000,%d4:l)
    46f6:	4e4f           	trap #15
    46f8:	a0c5           	0120305
    46fa:	2f03           	movel %d3,%sp@-
    46fc:	4e4f           	trap #15
    46fe:	a035           	0120065
    4700:	4227           	clrb %sp@-
    4702:	3f05           	movew %d5,%sp@-
    4704:	2f12           	movel %a2@,%sp@-
    4706:	4e4f           	trap #15
    4708:	a05e           	0120136
    470a:	6000 002e      	braw 0x473a
    470e:	43fa daa9      	lea %pc@(0x21b9),%a1
    4712:	2f09           	movel %a1,%sp@-
    4714:	4873 4800      	pea %a3@(00000000,%d4:l)
    4718:	6000 001c      	braw 0x4736
    471c:	43fa ff56      	lea %pc@(0x4674),%a1
    4720:	2f09           	movel %a1,%sp@-
    4722:	4873 4800      	pea %a3@(00000000,%d4:l)
    4726:	6000 000e      	braw 0x4736
    472a:	2f02           	movel %d2,%sp@-
    472c:	3001           	movew %d1,%d0
    472e:	c1fc 0102      	mulsw #258,%d0
    4732:	4873 0800      	pea %a3@(00000000,%d0:l)
    4736:	4e4f           	trap #15
    4738:	a0c5           	0120305
    473a:	41ec 0008      	lea %a4@(8),%a0
    473e:	4a10           	tstb %a0@
    4740:	6612           	bnes 0x4754
    4742:	10bc 001a      	moveb #26,%a0@
    4746:	6000 000c      	braw 0x4754
    474a:	5241           	addqw #1,%d1
    474c:	0c41 0004      	cmpiw #4,%d1
    4750:	6f00 ff5a      	blew 0x46ac
    4754:	4cee 0c38 ffec 	moveml %fp@(-20),%d3-%d5/%a2-%a3
    475a:	4e5e           	unlk %fp
    475c:	4e75           	rts
    475e:	4e56 0000      	linkw %fp,#0
    4762:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    4766:	45ec 0230      	lea %a4@(560),%a2
    476a:	0c52 00fe      	cmpiw #254,%a2@
    476e:	660c           	bnes 0x477c
    4770:	363c 07cf      	movew #1999,%d3
    4774:	51cb fffe      	dbf %d3,0x4774
    4778:	6000 00e6      	braw 0x4860
    477c:	41ec 0008      	lea %a4@(8),%a0
    4780:	1010           	moveb %a0@,%d0
    4782:	670e           	beqs 0x4792
    4784:	1a00           	moveb %d0,%d5
    4786:	5305           	subqb #1,%d5
    4788:	1085           	moveb %d5,%a0@
    478a:	0c00 0001      	cmpib #1,%d0
    478e:	6600 0090      	bnew 0x4820
    4792:	4243           	clrw %d3
    4794:	280a           	movel %a2,%d4
    4796:	0684 0000 0105 	addil #261,%d4
    479c:	47ea 0003      	lea %a2@(3),%a3
    47a0:	45ec 0230      	lea %a4@(560),%a2
    47a4:	3003           	movew %d3,%d0
    47a6:	c1fc 0102      	mulsw #258,%d0
    47aa:	5243           	addqw #1,%d3
    47ac:	3203           	movew %d3,%d1
    47ae:	c3fc 0102      	mulsw #258,%d1
    47b2:	35b2 1800 0800 	movew %a2@(00000000,%d1:l),%a2@(00000000,%d0:l)
    47b8:	15b2 1802 0802 	moveb %a2@(00000002,%d1:l),%a2@(00000002,%d0:l)
    47be:	2240           	moveal %d0,%a1
    47c0:	4871 4800      	pea %a1@(00000000,%d4:l)
    47c4:	4873 0800      	pea %a3@(00000000,%d0:l)
    47c8:	4e4f           	trap #15
    47ca:	a0c5           	0120305
    47cc:	504f           	addqw #8,%sp
    47ce:	0c43 0003      	cmpiw #3,%d3
    47d2:	6fcc           	bles 0x47a0
    47d4:	357c 00fe 0408 	movew #254,%a2@(1032)
    47da:	157c 00fe 040a 	moveb #-2,%a2@(1034)
    47e0:	0c52 00fe      	cmpiw #254,%a2@
    47e4:	677a           	beqs 0x4860
    47e6:	47ec 0008      	lea %a4@(8),%a3
    47ea:	16bc 001a      	moveb #26,%a3@
    47ee:	486a 0003      	pea %a2@(3)
    47f2:	4e4f           	trap #15
    47f4:	a0c7           	0120307
    47f6:	3400           	movew %d0,%d2
    47f8:	584f           	addqw #4,%sp
    47fa:	0c42 0013      	cmpiw #19,%d2
    47fe:	6e10           	bgts 0x4810
    4800:	7014           	moveq #20,%d0
    4802:	9042           	subw %d2,%d0
    4804:	3200           	movew %d0,%d1
    4806:	7a0f           	moveq #15,%d5
    4808:	ea69           	lsrw %d5,%d1
    480a:	d041           	addw %d1,%d0
    480c:	e240           	asrw #1,%d0
    480e:	9113           	subb %d0,%a3@
    4810:	0c42 001e      	cmpiw #30,%d2
    4814:	6f0a           	bles 0x4820
    4816:	3002           	movew %d2,%d0
    4818:	0640 ffe7      	addiw #-25,%d0
    481c:	e540           	aslw #2,%d0
    481e:	d113           	addb %d0,%a3@
    4820:	41ec 0230      	lea %a4@(560),%a0
    4824:	4240           	clrw %d0
    4826:	1028 0002      	moveb %a0@(2),%d0
    482a:	0c40 0001      	cmpiw #1,%d0
    482e:	671c           	beqs 0x484c
    4830:	6e08           	bgts 0x483a
    4832:	4a40           	tstw %d0
    4834:	670e           	beqs 0x4844
    4836:	6000 0028      	braw 0x4860
    483a:	0c40 0002      	cmpiw #2,%d0
    483e:	6714           	beqs 0x4854
    4840:	6000 001e      	braw 0x4860
    4844:	486c 00d0      	pea %a4@(208)
    4848:	6000 0012      	braw 0x485c
    484c:	486c 00d0      	pea %a4@(208)
    4850:	6000 000a      	braw 0x485c
    4854:	41ec 0100      	lea %a4@(256),%a0
    4858:	4868 0104      	pea %a0@(260)
    485c:	6100 000c      	bsrw 0x486a
    4860:	4cee 0c38 ffec 	moveml %fp@(-20),%d3-%d5/%a2-%a3
    4866:	4e5e           	unlk %fp
    4868:	4e75           	rts
    486a:	4e56 ffec      	linkw %fp,#-20
    486e:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    4872:	4247           	clrw %d7
    4874:	703e           	moveq #62,%d0
    4876:	226e 0008      	moveal %fp@(8),%a1
    487a:	9051           	subw %a1@,%d0
    487c:	6a02           	bpls 0x4880
    487e:	5640           	addqw #3,%d0
    4880:	e440           	asrw #2,%d0
    4882:	7a50           	moveq #80,%d5
    4884:	9a40           	subw %d0,%d5
    4886:	3d45 fff0      	movew %d5,%fp@(-16)
    488a:	226e 0008      	moveal %fp@(8),%a1
    488e:	3029 0002      	movew %a1@(2),%d0
    4892:	0c40 0022      	cmpiw #34,%d0
    4896:	6e0a           	bgts 0x48a2
    4898:	3c00           	movew %d0,%d6
    489a:	0646 0028      	addiw #40,%d6
    489e:	6000 0006      	braw 0x48a6
    48a2:	3c00           	movew %d0,%d6
    48a4:	5b46           	subqw #5,%d6
    48a6:	3d46 ffee      	movew %d6,%fp@(-18)
    48aa:	45ec 0230      	lea %a4@(560),%a2
    48ae:	486a 0003      	pea %a2@(3)
    48b2:	4e4f           	trap #15
    48b4:	a0c7           	0120307
    48b6:	3640           	moveaw %d0,%a3
    48b8:	564a           	addqw #3,%a2
    48ba:	4227           	clrb %sp@-
    48bc:	4e4f           	trap #15
    48be:	a164           	0120544
    48c0:	1d40 fff3      	moveb %d0,%fp@(-13)
    48c4:	2f2c 0084      	movel %a4@(132),%sp@-
    48c8:	4e4f           	trap #15
    48ca:	a1fd           	0120775
    48cc:	2d48 fff4      	movel %a0,%fp@(-12)
    48d0:	4243           	clrw %d3
    48d2:	426e ffec      	clrw %fp@(-20)
    48d6:	defc 000a      	addaw #10,%sp
    48da:	342e ffec      	movew %fp@(-20),%d2
    48de:	b44b           	cmpw %a3,%d2
    48e0:	6c2c           	bges 0x490e
    48e2:	3f3c 0050      	movew #80,%sp@-
    48e6:	2f0a           	movel %a2,%sp@-
    48e8:	4e4f           	trap #15
    48ea:	a2d9           	0121331
    48ec:	3800           	movew %d0,%d4
    48ee:	3f04           	movew %d4,%sp@-
    48f0:	2f0a           	movel %a2,%sp@-
    48f2:	4e4f           	trap #15
    48f4:	a16b           	0120553
    48f6:	d644           	addw %d4,%d3
    48f8:	defc 000c      	addaw #12,%sp
    48fc:	be40           	cmpw %d0,%d7
    48fe:	6c02           	bges 0x4902
    4900:	3e00           	movew %d0,%d7
    4902:	526e ffec      	addqw #1,%fp@(-20)
    4906:	45f2 4000      	lea %a2@(00000000,%d4:w),%a2
    490a:	b64b           	cmpw %a3,%d3
    490c:	6dd4           	blts 0x48e2
    490e:	4a6e ffec      	tstw %fp@(-20)
    4912:	6700 0290      	beqw 0x4ba4
    4916:	3007           	movew %d7,%d0
    4918:	740f           	moveq #15,%d2
    491a:	e468           	lsrw %d2,%d0
    491c:	d047           	addw %d7,%d0
    491e:	e240           	asrw #1,%d0
    4920:	9a40           	subw %d0,%d5
    4922:	3005           	movew %d5,%d0
    4924:	5945           	subqw #4,%d5
    4926:	226e 0008      	moveal %fp@(8),%a1
    492a:	0c69 0022 0002 	cmpiw #34,%a1@(2)
    4930:	6e06           	bgts 0x4938
    4932:	5746           	subqw #3,%d6
    4934:	6000 0010      	braw 0x4946
    4938:	302e ffec      	movew %fp@(-20),%d0
    493c:	c1fc 000b      	mulsw #11,%d0
    4940:	9c40           	subw %d0,%d6
    4942:	3006           	movew %d6,%d0
    4944:	5d46           	subqw #6,%d6
    4946:	3007           	movew %d7,%d0
    4948:	5040           	addqw #8,%d0
    494a:	4840           	swap %d0
    494c:	4240           	clrw %d0
    494e:	806e fffe      	orw %fp@(-2),%d0
    4952:	2d40 fffc      	movel %d0,%fp@(-4)
    4956:	302e ffec      	movew %fp@(-20),%d0
    495a:	c1fc 000b      	mulsw #11,%d0
    495e:	5c40           	addqw #6,%d0
    4960:	3d40 fffe      	movew %d0,%fp@(-2)
    4964:	2005           	movel %d5,%d0
    4966:	4840           	swap %d0
    4968:	4240           	clrw %d0
    496a:	806e fffa      	orw %fp@(-6),%d0
    496e:	2d40 fff8      	movel %d0,%fp@(-8)
    4972:	3d46 fffa      	movew %d6,%fp@(-6)
    4976:	3f3c 000c      	movew #12,%sp@-
    497a:	486e fff8      	pea %fp@(-8)
    497e:	4e4f           	trap #15
    4980:	a218           	0121030
    4982:	3e06           	movew %d6,%d7
    4984:	5c4f           	addqw #6,%sp
    4986:	41ec 0230      	lea %a4@(560),%a0
    498a:	4240           	clrw %d0
    498c:	1028 0002      	moveb %a0@(2),%d0
    4990:	0c40 0001      	cmpiw #1,%d0
    4994:	6700 00ac      	beqw 0x4a42
    4998:	6e08           	bgts 0x49a2
    499a:	4a40           	tstw %d0
    499c:	670c           	beqs 0x49aa
    499e:	6000 019a      	braw 0x4b3a
    49a2:	0c40 0002      	cmpiw #2,%d0
    49a6:	6600 0192      	bnew 0x4b3a
    49aa:	226e 0008      	moveal %fp@(8),%a1
    49ae:	3029 0002      	movew %a1@(2),%d0
    49b2:	0c40 0022      	cmpiw #34,%d0
    49b6:	6e0a           	bgts 0x49c2
    49b8:	3c00           	movew %d0,%d6
    49ba:	0646 0019      	addiw #25,%d6
    49be:	6000 0006      	braw 0x49c6
    49c2:	3c00           	movew %d0,%d6
    49c4:	5446           	addqw #2,%d6
    49c6:	302e fff0      	movew %fp@(-16),%d0
    49ca:	0640 fff3      	addiw #-13,%d0
    49ce:	226e 0008      	moveal %fp@(8),%a1
    49d2:	3251           	moveaw %a1@,%a1
    49d4:	92c0           	subaw %d0,%a1
    49d6:	3009           	movew %a1,%d0
    49d8:	d040           	addw %d0,%d0
    49da:	3200           	movew %d0,%d1
    49dc:	c3fc 5556      	mulsw #21846,%d1
    49e0:	4241           	clrw %d1
    49e2:	4841           	swap %d1
    49e4:	740f           	moveq #15,%d2
    49e6:	e460           	asrw %d2,%d0
    49e8:	9240           	subw %d0,%d1
    49ea:	3a2e fff0      	movew %fp@(-16),%d5
    49ee:	da41           	addw %d1,%d5
    49f0:	362e fff0      	movew %fp@(-16),%d3
    49f4:	5943           	subqw #4,%d3
    49f6:	302e fff0      	movew %fp@(-16),%d0
    49fa:	5a40           	addqw #5,%d0
    49fc:	b043           	cmpw %d3,%d0
    49fe:	6f00 013a      	blew 0x4b3a
    4a02:	3800           	movew %d0,%d4
    4a04:	226e 0008      	moveal %fp@(8),%a1
    4a08:	0c69 0022 0002 	cmpiw #34,%a1@(2)
    4a0e:	6e12           	bgts 0x4a22
    4a10:	3f06           	movew %d6,%sp@-
    4a12:	3f05           	movew %d5,%sp@-
    4a14:	342e ffee      	movew %fp@(-18),%d2
    4a18:	0642 000c      	addiw #12,%d2
    4a1c:	3f02           	movew %d2,%sp@-
    4a1e:	6000 0010      	braw 0x4a30
    4a22:	3f06           	movew %d6,%sp@-
    4a24:	3f05           	movew %d5,%sp@-
    4a26:	326e ffee      	moveaw %fp@(-18),%a1
    4a2a:	d2fc fff4      	addaw #-12,%a1
    4a2e:	3f09           	movew %a1,%sp@-
    4a30:	3f03           	movew %d3,%sp@-
    4a32:	4e4f           	trap #15
    4a34:	a213           	0121023
    4a36:	504f           	addqw #8,%sp
    4a38:	5243           	addqw #1,%d3
    4a3a:	b843           	cmpw %d3,%d4
    4a3c:	6ec6           	bgts 0x4a04
    4a3e:	6000 00fa      	braw 0x4b3a
    4a42:	226e 0008      	moveal %fp@(8),%a1
    4a46:	3029 0002      	movew %a1@(2),%d0
    4a4a:	0c40 0022      	cmpiw #34,%d0
    4a4e:	6e0a           	bgts 0x4a5a
    4a50:	3c00           	movew %d0,%d6
    4a52:	0646 001f      	addiw #31,%d6
    4a56:	6000 0006      	braw 0x4a5e
    4a5a:	3c00           	movew %d0,%d6
    4a5c:	5946           	subqw #4,%d6
    4a5e:	306e fff0      	moveaw %fp@(-16),%a0
    4a62:	2f08           	movel %a0,%sp@-
    4a64:	6100 271a      	bsrw 0x7180
    4a68:	584f           	addqw #4,%sp
    4a6a:	2800           	movel %d0,%d4
    4a6c:	2a01           	movel %d1,%d5
    4a6e:	302e fff0      	movew %fp@(-16),%d0
    4a72:	0640 fff3      	addiw #-13,%d0
    4a76:	226e 0008      	moveal %fp@(8),%a1
    4a7a:	3051           	moveaw %a1@,%a0
    4a7c:	90c0           	subaw %d0,%a0
    4a7e:	3048           	moveaw %a0,%a0
    4a80:	2f08           	movel %a0,%sp@-
    4a82:	6100 26fc      	bsrw 0x7180
    4a86:	584f           	addqw #4,%sp
    4a88:	243c 400c 0000 	movel #1074528256,%d2
    4a8e:	7600           	moveq #0,%d3
    4a90:	2f03           	movel %d3,%sp@-
    4a92:	2f02           	movel %d2,%sp@-
    4a94:	2f01           	movel %d1,%sp@-
    4a96:	2f00           	movel %d0,%sp@-
    4a98:	6100 2ee8      	bsrw 0x7982
    4a9c:	defc 0010      	addaw #16,%sp
    4aa0:	2f01           	movel %d1,%sp@-
    4aa2:	2f00           	movel %d0,%sp@-
    4aa4:	2f05           	movel %d5,%sp@-
    4aa6:	2f04           	movel %d4,%sp@-
    4aa8:	6100 296e      	bsrw 0x7418
    4aac:	defc 0010      	addaw #16,%sp
    4ab0:	2f01           	movel %d1,%sp@-
    4ab2:	2f00           	movel %d0,%sp@-
    4ab4:	6100 2848      	bsrw 0x72fe
    4ab8:	504f           	addqw #8,%sp
    4aba:	3d7c 0008 fffc 	movew #8,%fp@(-4)
    4ac0:	3d7c 0005 fffe 	movew #5,%fp@(-2)
    4ac6:	3d40 fff8      	movew %d0,%fp@(-8)
    4aca:	3d46 fffa      	movew %d6,%fp@(-6)
    4ace:	3f3c 0003      	movew #3,%sp@-
    4ad2:	486e fff8      	pea %fp@(-8)
    4ad6:	4e4f           	trap #15
    4ad8:	a218           	0121030
    4ada:	226e 0008      	moveal %fp@(8),%a1
    4ade:	3029 0002      	movew %a1@(2),%d0
    4ae2:	5c4f           	addqw #6,%sp
    4ae4:	0c40 0022      	cmpiw #34,%d0
    4ae8:	6e0a           	bgts 0x4af4
    4aea:	3c00           	movew %d0,%d6
    4aec:	0646 001a      	addiw #26,%d6
    4af0:	6000 0006      	braw 0x4af8
    4af4:	3c00           	movew %d0,%d6
    4af6:	5446           	addqw #2,%d6
    4af8:	302e fff0      	movew %fp@(-16),%d0
    4afc:	0640 fff3      	addiw #-13,%d0
    4b00:	226e 0008      	moveal %fp@(8),%a1
    4b04:	3251           	moveaw %a1@,%a1
    4b06:	92c0           	subaw %d0,%a1
    4b08:	3009           	movew %a1,%d0
    4b0a:	3200           	movew %d0,%d1
    4b0c:	740f           	moveq #15,%d2
    4b0e:	e469           	lsrw %d2,%d1
    4b10:	d041           	addw %d1,%d0
    4b12:	e240           	asrw #1,%d0
    4b14:	3d7c 0005 fffc 	movew #5,%fp@(-4)
    4b1a:	3d7c 0004 fffe 	movew #4,%fp@(-2)
    4b20:	d06e fff0      	addw %fp@(-16),%d0
    4b24:	3d40 fff8      	movew %d0,%fp@(-8)
    4b28:	3d46 fffa      	movew %d6,%fp@(-6)
    4b2c:	3f3c 0003      	movew #3,%sp@-
    4b30:	486e fff8      	pea %fp@(-8)
    4b34:	4e4f           	trap #15
    4b36:	a218           	0121030
    4b38:	5c4f           	addqw #6,%sp
    4b3a:	43ec 0230      	lea %a4@(560),%a1
    4b3e:	2009           	movel %a1,%d0
    4b40:	2440           	moveal %d0,%a2
    4b42:	564a           	addqw #3,%a2
    4b44:	3c07           	movew %d7,%d6
    4b46:	5646           	addqw #3,%d6
    4b48:	4243           	clrw %d3
    4b4a:	b66e ffec      	cmpw %fp@(-20),%d3
    4b4e:	6c44           	bges 0x4b94
    4b50:	3f3c 0050      	movew #80,%sp@-
    4b54:	2f0a           	movel %a2,%sp@-
    4b56:	4e4f           	trap #15
    4b58:	a2d9           	0121331
    4b5a:	3800           	movew %d0,%d4
    4b5c:	3f04           	movew %d4,%sp@-
    4b5e:	2f0a           	movel %a2,%sp@-
    4b60:	4e4f           	trap #15
    4b62:	a16b           	0120553
    4b64:	3200           	movew %d0,%d1
    4b66:	740f           	moveq #15,%d2
    4b68:	e469           	lsrw %d2,%d1
    4b6a:	d240           	addw %d0,%d1
    4b6c:	e241           	asrw #1,%d1
    4b6e:	3f06           	movew %d6,%sp@-
    4b70:	326e fff0      	moveaw %fp@(-16),%a1
    4b74:	92c1           	subaw %d1,%a1
    4b76:	3f09           	movew %a1,%sp@-
    4b78:	3f04           	movew %d4,%sp@-
    4b7a:	2f0a           	movel %a2,%sp@-
    4b7c:	4e4f           	trap #15
    4b7e:	a22a           	0121052
    4b80:	45f2 4000      	lea %a2@(00000000,%d4:w),%a2
    4b84:	0646 000b      	addiw #11,%d6
    4b88:	defc 0016      	addaw #22,%sp
    4b8c:	5243           	addqw #1,%d3
    4b8e:	b66e ffec      	cmpw %fp@(-20),%d3
    4b92:	6dbc           	blts 0x4b50
    4b94:	2f2e fff4      	movel %fp@(-12),%sp@-
    4b98:	4e4f           	trap #15
    4b9a:	a1fd           	0120775
    4b9c:	1f2e fff3      	moveb %fp@(-13),%sp@-
    4ba0:	4e4f           	trap #15
    4ba2:	a164           	0120544
    4ba4:	4cee 0cf8 ffd0 	moveml %fp@(-48),%d3-%d7/%a2-%a3
    4baa:	4e5e           	unlk %fp
    4bac:	4e75           	rts
    4bae:	4e56 0000      	linkw %fp,#0
    4bb2:	2f03           	movel %d3,%sp@-
    4bb4:	3f2e 0008      	movew %fp@(8),%sp@-
    4bb8:	2f3c 5462 6d70 	movel #1415736688,%sp@-
    4bbe:	4e4f           	trap #15
    4bc0:	a05f           	0120137
    4bc2:	2608           	movel %a0,%d3
    4bc4:	2f03           	movel %d3,%sp@-
    4bc6:	4e4f           	trap #15
    4bc8:	a021           	0120041
    4bca:	4267           	clrw %sp@-
    4bcc:	4267           	clrw %sp@-
    4bce:	2f08           	movel %a0,%sp@-
    4bd0:	4e4f           	trap #15
    4bd2:	a226           	0121046
    4bd4:	2f03           	movel %d3,%sp@-
    4bd6:	4e4f           	trap #15
    4bd8:	a022           	0120042
    4bda:	2f03           	movel %d3,%sp@-
    4bdc:	4e4f           	trap #15
    4bde:	a061           	0120141
    4be0:	262e fffc      	movel %fp@(-4),%d3
    4be4:	4e5e           	unlk %fp
    4be6:	4e75           	rts
    4be8:	4e56 0000      	linkw %fp,#0
    4bec:	2f04           	movel %d4,%sp@-
    4bee:	2f03           	movel %d3,%sp@-
    4bf0:	2f2c 0084      	movel %a4@(132),%sp@-
    4bf4:	4e4f           	trap #15
    4bf6:	a1fd           	0120775
    4bf8:	2808           	movel %a0,%d4
    4bfa:	3f3c 03ef      	movew #1007,%sp@-
    4bfe:	2f3c 5462 6d70 	movel #1415736688,%sp@-
    4c04:	4e4f           	trap #15
    4c06:	a05f           	0120137
    4c08:	2608           	movel %a0,%d3
    4c0a:	2f03           	movel %d3,%sp@-
    4c0c:	4e4f           	trap #15
    4c0e:	a021           	0120041
    4c10:	4267           	clrw %sp@-
    4c12:	4267           	clrw %sp@-
    4c14:	2f08           	movel %a0,%sp@-
    4c16:	4e4f           	trap #15
    4c18:	a226           	0121046
    4c1a:	2f03           	movel %d3,%sp@-
    4c1c:	4e4f           	trap #15
    4c1e:	a022           	0120042
    4c20:	2f03           	movel %d3,%sp@-
    4c22:	4e4f           	trap #15
    4c24:	a061           	0120141
    4c26:	2f04           	movel %d4,%sp@-
    4c28:	4e4f           	trap #15
    4c2a:	a1fd           	0120775
    4c2c:	defc 0022      	addaw #34,%sp
    4c30:	6100 e89c      	bsrw 0x34ce
    4c34:	262e fff8      	movel %fp@(-8),%d3
    4c38:	282e fffc      	movel %fp@(-4),%d4
    4c3c:	4e5e           	unlk %fp
    4c3e:	4e75           	rts
    4c40:	556e 6e61      	subqw #2,%fp@(28257)
    4c44:	6d65           	blts 0x4cab
    4c46:	6420           	bccs 0x4c68
    4c48:	4974           	044564
    4c4a:	656d           	bcss 0x4cb9
    4c4c:	0000 4e56      	orib #86,%d0
    4c50:	fef0           	0177360
    4c52:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    4c56:	2f2c 0084      	movel %a4@(132),%sp@-
    4c5a:	4e4f           	trap #15
    4c5c:	a1fd           	0120775
    4c5e:	2d48 fef2      	movel %a0,%fp@(-270)
    4c62:	3f3c 03f0      	movew #1008,%sp@-
    4c66:	2f3c 5462 6d70 	movel #1415736688,%sp@-
    4c6c:	4e4f           	trap #15
    4c6e:	a05f           	0120137
    4c70:	2608           	movel %a0,%d3
    4c72:	2f03           	movel %d3,%sp@-
    4c74:	4e4f           	trap #15
    4c76:	a021           	0120041
    4c78:	4267           	clrw %sp@-
    4c7a:	4267           	clrw %sp@-
    4c7c:	2f08           	movel %a0,%sp@-
    4c7e:	4e4f           	trap #15
    4c80:	a226           	0121046
    4c82:	2f03           	movel %d3,%sp@-
    4c84:	4e4f           	trap #15
    4c86:	a022           	0120042
    4c88:	2f03           	movel %d3,%sp@-
    4c8a:	4e4f           	trap #15
    4c8c:	a061           	0120141
    4c8e:	45ec 00d0      	lea %a4@(208),%a2
    4c92:	4241           	clrw %d1
    4c94:	122a 000c      	moveb %a2@(12),%d1
    4c98:	c3fc 0064      	mulsw #100,%d1
    4c9c:	4240           	clrw %d0
    4c9e:	102a 000b      	moveb %a2@(11),%d0
    4ca2:	48c1           	extl %d1
    4ca4:	83c0           	divsw %d0,%d1
    4ca6:	3f01           	movew %d1,%sp@-
    4ca8:	3f3c 003d      	movew #61,%sp@-
    4cac:	3f3c 0032      	movew #50,%sp@-
    4cb0:	3f3c 0007      	movew #7,%sp@-
    4cb4:	6100 0386      	bsrw 0x503c
    4cb8:	defc 0026      	addaw #38,%sp
    4cbc:	4240           	clrw %d0
    4cbe:	102a 000e      	moveb %a2@(14),%d0
    4cc2:	3f00           	movew %d0,%sp@-
    4cc4:	3f3c 003d      	movew #61,%sp@-
    4cc8:	3f3c 0044      	movew #68,%sp@-
    4ccc:	3f3c 0007      	movew #7,%sp@-
    4cd0:	6100 036a      	bsrw 0x503c
    4cd4:	4240           	clrw %d0
    4cd6:	102a 000d      	moveb %a2@(13),%d0
    4cda:	3f00           	movew %d0,%sp@-
    4cdc:	3f3c 003d      	movew #61,%sp@-
    4ce0:	3f3c 0056      	movew #86,%sp@-
    4ce4:	3f3c 0007      	movew #7,%sp@-
    4ce8:	6100 0352      	bsrw 0x503c
    4cec:	defc 0010      	addaw #16,%sp
    4cf0:	4a2c 000e      	tstb %a4@(14)
    4cf4:	6636           	bnes 0x4d2c
    4cf6:	4280           	clrl %d0
    4cf8:	302e fefe      	movew %fp@(-258),%d0
    4cfc:	0080 0044 0000 	oril #4456448,%d0
    4d02:	2d40 fefc      	movel %d0,%fp@(-260)
    4d06:	3d7c 000d fefe 	movew #13,%fp@(-258)
    4d0c:	4280           	clrl %d0
    4d0e:	302e fefa      	movew %fp@(-262),%d0
    4d12:	08c0 0011      	bset #17,%d0
    4d16:	2d40 fef8      	movel %d0,%fp@(-264)
    4d1a:	3d7c 004d fefa 	movew #77,%fp@(-262)
    4d20:	4267           	clrw %sp@-
    4d22:	486e fef8      	pea %fp@(-264)
    4d26:	4e4f           	trap #15
    4d28:	a218           	0121030
    4d2a:	5c4f           	addqw #6,%sp
    4d2c:	4227           	clrb %sp@-
    4d2e:	4e4f           	trap #15
    4d30:	a164           	0120544
    4d32:	1d40 fef7      	moveb %d0,%fp@(-265)
    4d36:	3f2a 0012      	movew %a2@(18),%sp@-
    4d3a:	43fa b85d      	lea %pc@(0x599),%a1
    4d3e:	2f09           	movel %a1,%sp@-
    4d40:	45ee ff00      	lea %fp@(-256),%a2
    4d44:	2f0a           	movel %a2,%sp@-
    4d46:	4e4f           	trap #15
    4d48:	a2de           	0121336
    4d4a:	2f0a           	movel %a2,%sp@-
    4d4c:	4e4f           	trap #15
    4d4e:	a0c7           	0120307
    4d50:	3f3c 0068      	movew #104,%sp@-
    4d54:	3f3c 001e      	movew #30,%sp@-
    4d58:	3f00           	movew %d0,%sp@-
    4d5a:	2f0a           	movel %a2,%sp@-
    4d5c:	4e4f           	trap #15
    4d5e:	a22a           	0121052
    4d60:	4245           	clrw %d5
    4d62:	defc 001a      	addaw #26,%sp
    4d66:	426e fef0      	clrw %fp@(-272)
    4d6a:	2e0a           	movel %a2,%d7
    4d6c:	7c0a           	moveq #10,%d6
    4d6e:	3805           	movew %d5,%d4
    4d70:	c9fc 000c      	mulsw #12,%d4
    4d74:	45ec 0740      	lea %a4@(1856),%a2
    4d78:	0c32 00fe 4800 	cmpib #-2,%a2@(00000000,%d4:l)
    4d7e:	6700 00aa      	beqw 0x4e2a
    4d82:	1232 480a      	moveb %a2@(0000000a,%d4:l),%d1
    4d86:	6756           	beqs 0x4dde
    4d88:	47ec 00c0      	lea %a4@(192),%a3
    4d8c:	2013           	movel %a3@,%d0
    4d8e:	674e           	beqs 0x4dde
    4d90:	1d41 fef1      	moveb %d1,%fp@(-271)
    4d94:	3f2e fef0      	movew %fp@(-272),%sp@-
    4d98:	2f00           	movel %d0,%sp@-
    4d9a:	4e4f           	trap #15
    4d9c:	a05c           	0120134
    4d9e:	2008           	movel %a0,%d0
    4da0:	5c4f           	addqw #6,%sp
    4da2:	6732           	beqs 0x4dd6
    4da4:	2f00           	movel %d0,%sp@-
    4da6:	4e4f           	trap #15
    4da8:	a021           	0120041
    4daa:	2608           	movel %a0,%d3
    4dac:	2f03           	movel %d3,%sp@-
    4dae:	486e ff00      	pea %fp@(-256)
    4db2:	4e4f           	trap #15
    4db4:	a0c5           	0120305
    4db6:	2f03           	movel %d3,%sp@-
    4db8:	4e4f           	trap #15
    4dba:	a035           	0120065
    4dbc:	1f3c 0001      	moveb #1,%sp@-
    4dc0:	4240           	clrw %d0
    4dc2:	1032 480a      	moveb %a2@(0000000a,%d4:l),%d0
    4dc6:	3f00           	movew %d0,%sp@-
    4dc8:	2f13           	movel %a3@,%sp@-
    4dca:	4e4f           	trap #15
    4dcc:	a05e           	0120136
    4dce:	defc 0018      	addaw #24,%sp
    4dd2:	6000 001a      	braw 0x4dee
    4dd6:	43fa d3e1      	lea %pc@(0x21b9),%a1
    4dda:	6000 0006      	braw 0x4de2
    4dde:	43fa fe60      	lea %pc@(0x4c40),%a1
    4de2:	2f09           	movel %a1,%sp@-
    4de4:	486e ff00      	pea %fp@(-256)
    4de8:	4e4f           	trap #15
    4dea:	a0c5           	0120305
    4dec:	504f           	addqw #8,%sp
    4dee:	2f07           	movel %d7,%sp@-
    4df0:	4e4f           	trap #15
    4df2:	a0c7           	0120307
    4df4:	3400           	movew %d0,%d2
    4df6:	3005           	movew %d5,%d0
    4df8:	5240           	addqw #1,%d0
    4dfa:	4241           	clrw %d1
    4dfc:	122c 000d      	moveb %a4@(13),%d1
    4e00:	584f           	addqw #4,%sp
    4e02:	b240           	cmpw %d0,%d1
    4e04:	6612           	bnes 0x4e18
    4e06:	3f06           	movew %d6,%sp@-
    4e08:	3f3c 005d      	movew #93,%sp@-
    4e0c:	3f02           	movew %d2,%sp@-
    4e0e:	2f07           	movel %d7,%sp@-
    4e10:	4e4f           	trap #15
    4e12:	a22a           	0121052
    4e14:	6000 0010      	braw 0x4e26
    4e18:	3f06           	movew %d6,%sp@-
    4e1a:	3f3c 005d      	movew #93,%sp@-
    4e1e:	3f02           	movew %d2,%sp@-
    4e20:	2f07           	movel %d7,%sp@-
    4e22:	4e4f           	trap #15
    4e24:	a220           	0121040
    4e26:	defc 000a      	addaw #10,%sp
    4e2a:	0646 000b      	addiw #11,%d6
    4e2e:	5245           	addqw #1,%d5
    4e30:	0c45 0009      	cmpiw #9,%d5
    4e34:	6f00 ff38      	blew 0x4d6e
    4e38:	1f2e fef7      	moveb %fp@(-265),%sp@-
    4e3c:	4e4f           	trap #15
    4e3e:	a164           	0120544
    4e40:	2f2e fef2      	movel %fp@(-270),%sp@-
    4e44:	4e4f           	trap #15
    4e46:	a1fd           	0120775
    4e48:	6100 e684      	bsrw 0x34ce
    4e4c:	4cee 0cf8 fed4 	moveml %fp@(-300),%d3-%d7/%a2-%a3
    4e52:	4e5e           	unlk %fp
    4e54:	4e75           	rts
    4e56:	546f 6f20      	addqw #2,%sp@(28448)
    4e5a:	6d65           	blts 0x4ec1
    4e5c:	7373           	071563
    4e5e:	7920           	074440
    4e60:	696e           	bvss 0x4ed0
    4e62:	2068 6572      	moveal %a0@(25970),%a0
    4e66:	652e           	bcss 0x4e96
    4e68:	0000 4e56      	orib #86,%d0
    4e6c:	0000 48e7      	orib #-25,%d0
    4e70:	1f30 1a2e      	moveb %a0@(0000002e,%d1:l:2),%sp@-
    4e74:	0008 4240      	orib #64,%a0
    4e78:	1005           	moveb %d5,%d0
    4e7a:	c1fc 000c      	mulsw #12,%d0
    4e7e:	41ec 0740      	lea %a4@(1856),%a0
    4e82:	0c30 00fe 0800 	cmpib #-2,%a0@(00000000,%d0:l)
    4e88:	6700 01a8      	beqw 0x5032
    4e8c:	0c30 0006 0805 	cmpib #6,%a0@(00000005,%d0:l)
    4e92:	665a           	bnes 0x4eee
    4e94:	45ec 00d0      	lea %a4@(208),%a2
    4e98:	7e15           	moveq #21,%d7
    4e9a:	de8a           	addl %a2,%d7
    4e9c:	2647           	moveal %d7,%a3
    4e9e:	1c13           	moveb %a3@,%d6
    4ea0:	bc30 080a      	cmpb %a0@(0000000a,%d0:l),%d6
    4ea4:	6648           	bnes 0x4eee
    4ea6:	2f2c 0088      	movel %a4@(136),%sp@-
    4eaa:	4e4f           	trap #15
    4eac:	a1fd           	0120775
    4eae:	2808           	movel %a0,%d4
    4eb0:	3f3c 03e9      	movew #1001,%sp@-
    4eb4:	2f3c 5462 6d70 	movel #1415736688,%sp@-
    4eba:	4e4f           	trap #15
    4ebc:	a05f           	0120137
    4ebe:	2608           	movel %a0,%d3
    4ec0:	2f03           	movel %d3,%sp@-
    4ec2:	4e4f           	trap #15
    4ec4:	a021           	0120041
    4ec6:	4267           	clrw %sp@-
    4ec8:	4267           	clrw %sp@-
    4eca:	2f08           	movel %a0,%sp@-
    4ecc:	4e4f           	trap #15
    4ece:	a226           	0121046
    4ed0:	2f03           	movel %d3,%sp@-
    4ed2:	4e4f           	trap #15
    4ed4:	a022           	0120042
    4ed6:	2f03           	movel %d3,%sp@-
    4ed8:	4e4f           	trap #15
    4eda:	a061           	0120141
    4edc:	2f04           	movel %d4,%sp@-
    4ede:	4e4f           	trap #15
    4ee0:	a1fd           	0120775
    4ee2:	4213           	clrb %a3@
    4ee4:	157c 0001 0014 	moveb #1,%a2@(20)
    4eea:	defc 0022      	addaw #34,%sp
    4eee:	4242           	clrw %d2
    4ef0:	4243           	clrw %d3
    4ef2:	47ec 00d0      	lea %a4@(208),%a3
    4ef6:	200b           	movel %a3,%d0
    4ef8:	2440           	moveal %d0,%a2
    4efa:	544a           	addqw #2,%a2
    4efc:	3202           	movew %d2,%d1
    4efe:	c3fc 000c      	mulsw #12,%d1
    4f02:	47ec 0100      	lea %a4@(256),%a3
    4f06:	200b           	movel %a3,%d0
    4f08:	2241           	moveal %d1,%a1
    4f0a:	d3c0           	addal %d0,%a1
    4f0c:	0c29 00fe 0084 	cmpib #-2,%a1@(132)
    4f12:	6600 0102      	bnew 0x5016
    4f16:	1605           	moveb %d5,%d3
    4f18:	3203           	movew %d3,%d1
    4f1a:	c3fc 000c      	mulsw #12,%d1
    4f1e:	41ec 0740      	lea %a4@(1856),%a0
    4f22:	1370 1800 0084 	moveb %a0@(00000000,%d1:l),%a1@(132)
    4f28:	1370 1801 0085 	moveb %a0@(00000001,%d1:l),%a1@(133)
    4f2e:	302c 00d0      	movew %a4@(208),%d0
    4f32:	1340 0086      	moveb %d0,%a1@(134)
    4f36:	0c30 0002 1801 	cmpib #2,%a0@(00000001,%d1:l)
    4f3c:	6610           	bnes 0x4f4e
    4f3e:	1c2a 0001      	moveb %a2@(1),%d6
    4f42:	0606 0010      	addib #16,%d6
    4f46:	1346 0087      	moveb %d6,%a1@(135)
    4f4a:	6000 0008      	braw 0x4f54
    4f4e:	136a 0001 0087 	moveb %a2@(1),%a1@(135)
    4f54:	43ec 0100      	lea %a4@(256),%a1
    4f58:	3002           	movew %d2,%d0
    4f5a:	c1fc 000c      	mulsw #12,%d0
    4f5e:	43f1 0800      	lea %a1@(00000000,%d0:l),%a1
    4f62:	4240           	clrw %d0
    4f64:	1005           	moveb %d5,%d0
    4f66:	c1fc 000c      	mulsw #12,%d0
    4f6a:	41ec 0740      	lea %a4@(1856),%a0
    4f6e:	1370 0804 0088 	moveb %a0@(00000004,%d0:l),%a1@(136)
    4f74:	1370 0805 0089 	moveb %a0@(00000005,%d0:l),%a1@(137)
    4f7a:	1370 0806 008a 	moveb %a0@(00000006,%d0:l),%a1@(138)
    4f80:	1370 0807 008b 	moveb %a0@(00000007,%d0:l),%a1@(139)
    4f86:	1370 0808 008c 	moveb %a0@(00000008,%d0:l),%a1@(140)
    4f8c:	1370 0809 008d 	moveb %a0@(00000009,%d0:l),%a1@(141)
    4f92:	1370 080a 008e 	moveb %a0@(0000000a,%d0:l),%a1@(142)
    4f98:	1370 080b 008f 	moveb %a0@(0000000b,%d0:l),%a1@(143)
    4f9e:	4242           	clrw %d2
    4fa0:	1405           	moveb %d5,%d2
    4fa2:	0c42 0008      	cmpiw #8,%d2
    4fa6:	6e60           	bgts 0x5008
    4fa8:	41ec 0740      	lea %a4@(1856),%a0
    4fac:	3202           	movew %d2,%d1
    4fae:	c3fc 000c      	mulsw #12,%d1
    4fb2:	5242           	addqw #1,%d2
    4fb4:	3002           	movew %d2,%d0
    4fb6:	c1fc 000c      	mulsw #12,%d0
    4fba:	11b0 0800 1800 	moveb %a0@(00000000,%d0:l),%a0@(00000000,%d1:l)
    4fc0:	11b0 0801 1801 	moveb %a0@(00000001,%d0:l),%a0@(00000001,%d1:l)
    4fc6:	11b0 0802 1802 	moveb %a0@(00000002,%d0:l),%a0@(00000002,%d1:l)
    4fcc:	11b0 0803 1803 	moveb %a0@(00000003,%d0:l),%a0@(00000003,%d1:l)
    4fd2:	11b0 0804 1804 	moveb %a0@(00000004,%d0:l),%a0@(00000004,%d1:l)
    4fd8:	11b0 0805 1805 	moveb %a0@(00000005,%d0:l),%a0@(00000005,%d1:l)
    4fde:	11b0 0806 1806 	moveb %a0@(00000006,%d0:l),%a0@(00000006,%d1:l)
    4fe4:	11b0 0807 1807 	moveb %a0@(00000007,%d0:l),%a0@(00000007,%d1:l)
    4fea:	11b0 0808 1808 	moveb %a0@(00000008,%d0:l),%a0@(00000008,%d1:l)
    4ff0:	11b0 0809 1809 	moveb %a0@(00000009,%d0:l),%a0@(00000009,%d1:l)
    4ff6:	11b0 080a 180a 	moveb %a0@(0000000a,%d0:l),%a0@(0000000a,%d1:l)
    4ffc:	11b0 080b 180b 	moveb %a0@(0000000b,%d0:l),%a0@(0000000b,%d1:l)
    5002:	0c42 0008      	cmpiw #8,%d2
    5006:	6fa0           	bles 0x4fa8
    5008:	41ec 0740      	lea %a4@(1856),%a0
    500c:	117c 00fe 006c 	moveb #-2,%a0@(108)
    5012:	6000 001e      	braw 0x5032
    5016:	5242           	addqw #1,%d2
    5018:	0c42 0009      	cmpiw #9,%d2
    501c:	6f00 fede      	blew 0x4efc
    5020:	47fa fe34      	lea %pc@(0x4e56),%a3
    5024:	2f0b           	movel %a3,%sp@-
    5026:	3f3c 0001      	movew #1,%sp@-
    502a:	3f3c 029c      	movew #668,%sp@-
    502e:	6100 f65c      	bsrw 0x468c
    5032:	4cee 0cf8 ffe4 	moveml %fp@(-28),%d3-%d7/%a2-%a3
    5038:	4e5e           	unlk %fp
    503a:	4e75           	rts
    503c:	4e56 fff0      	linkw %fp,#-16
    5040:	2f0a           	movel %a2,%sp@-
    5042:	302e 0008      	movew %fp@(8),%d0
    5046:	4840           	swap %d0
    5048:	4240           	clrw %d0
    504a:	806e fff2      	orw %fp@(-14),%d0
    504e:	2d40 fff0      	movel %d0,%fp@(-16)
    5052:	3d6e 000a fff2 	movew %fp@(10),%fp@(-14)
    5058:	322e 000c      	movew %fp@(12),%d1
    505c:	c3ee 000e      	mulsw %fp@(14),%d1
    5060:	3001           	movew %d1,%d0
    5062:	c1fc 147b      	mulsw #5243,%d0
    5066:	4240           	clrw %d0
    5068:	4840           	swap %d0
    506a:	e640           	asrw #3,%d0
    506c:	740f           	moveq #15,%d2
    506e:	e461           	asrw %d2,%d1
    5070:	9041           	subw %d1,%d0
    5072:	4840           	swap %d0
    5074:	4240           	clrw %d0
    5076:	806e fff6      	orw %fp@(-10),%d0
    507a:	2d40 fff4      	movel %d0,%fp@(-12)
    507e:	3d7c 0003 fff6 	movew #3,%fp@(-10)
    5084:	303c 55aa      	movew #21930,%d0
    5088:	3d40 fffe      	movew %d0,%fp@(-2)
    508c:	3d40 fffa      	movew %d0,%fp@(-6)
    5090:	4280           	clrl %d0
    5092:	302e fffe      	movew %fp@(-2),%d0
    5096:	0080 55aa 0000 	oril #1437204480,%d0
    509c:	2d40 fffc      	movel %d0,%fp@(-4)
    50a0:	4280           	clrl %d0
    50a2:	302e fffa      	movew %fp@(-6),%d0
    50a6:	0080 55aa 0000 	oril #1437204480,%d0
    50ac:	2d40 fff8      	movel %d0,%fp@(-8)
    50b0:	45ee fff8      	lea %fp@(-8),%a2
    50b4:	2f0a           	movel %a2,%sp@-
    50b6:	4e4f           	trap #15
    50b8:	a224           	0121044
    50ba:	4267           	clrw %sp@-
    50bc:	486e fff0      	pea %fp@(-16)
    50c0:	4e4f           	trap #15
    50c2:	a229           	0121051
    50c4:	3d7c ffff fffe 	movew #-1,%fp@(-2)
    50ca:	3d7c ffff fffa 	movew #-1,%fp@(-6)
    50d0:	3d7c ffff fffc 	movew #-1,%fp@(-4)
    50d6:	3d7c ffff fff8 	movew #-1,%fp@(-8)
    50dc:	2f0a           	movel %a2,%sp@-
    50de:	4e4f           	trap #15
    50e0:	a224           	0121044
    50e2:	246e ffec      	moveal %fp@(-20),%a2
    50e6:	4e5e           	unlk %fp
    50e8:	4e75           	rts
    50ea:	4e56 0000      	linkw %fp,#0
    50ee:	2f0a           	movel %a2,%sp@-
    50f0:	2f03           	movel %d3,%sp@-
    50f2:	102c 000a      	moveb %a4@(10),%d0
    50f6:	5500           	subqb #2,%d0
    50f8:	0c00 0001      	cmpib #1,%d0
    50fc:	6200 011c      	bhiw 0x521a
    5100:	45ec 0100      	lea %a4@(256),%a2
    5104:	200a           	movel %a2,%d0
    5106:	2040           	moveal %d0,%a0
    5108:	d0fc 010a      	addaw #266,%a0
    510c:	0c10 0003      	cmpib #3,%a0@
    5110:	6700 0108      	beqw 0x521a
    5114:	3228 fffc      	movew %a0@(-4),%d1
    5118:	3001           	movew %d1,%d0
    511a:	0640 0020      	addiw #32,%d0
    511e:	6a06           	bpls 0x5126
    5120:	3001           	movew %d1,%d0
    5122:	0640 002f      	addiw #47,%d0
    5126:	3600           	movew %d0,%d3
    5128:	e843           	asrw #4,%d3
    512a:	0c28 0001 0002 	cmpib #1,%a0@(2)
    5130:	6616           	bnes 0x5148
    5132:	3228 fffa      	movew %a0@(-6),%d1
    5136:	3001           	movew %d1,%d0
    5138:	0640 000a      	addiw #10,%d0
    513c:	6a1c           	bpls 0x515a
    513e:	3001           	movew %d1,%d0
    5140:	0640 0019      	addiw #25,%d0
    5144:	6000 0014      	braw 0x515a
    5148:	3228 fffa      	movew %a0@(-6),%d1
    514c:	3001           	movew %d1,%d0
    514e:	0640 0018      	addiw #24,%d0
    5152:	6a06           	bpls 0x515a
    5154:	3001           	movew %d1,%d0
    5156:	0640 0027      	addiw #39,%d0
    515a:	3400           	movew %d0,%d2
    515c:	e842           	asrw #4,%d2
    515e:	3202           	movew %d2,%d1
    5160:	48c1           	extl %d1
    5162:	41ec 0100      	lea %a4@(256),%a0
    5166:	43e8 001e      	lea %a0@(30),%a1
    516a:	2001           	movel %d1,%d0
    516c:	e780           	asll #3,%d0
    516e:	d081           	addl %d1,%d0
    5170:	d081           	addl %d1,%d0
    5172:	41f1 0800      	lea %a1@(00000000,%d0:l),%a0
    5176:	1030 3000      	moveb %a0@(00000000,%d3:w),%d0
    517a:	0c00 0015      	cmpib #21,%d0
    517e:	6300 009a      	blsw 0x521a
    5182:	0c00 0023      	cmpib #35,%d0
    5186:	6700 0092      	beqw 0x521a
    518a:	0c00 0026      	cmpib #38,%d0
    518e:	6700 008a      	beqw 0x521a
    5192:	0c29 0001 00ee 	cmpib #1,%a1@(238)
    5198:	660a           	bnes 0x51a4
    519a:	4a42           	tstw %d2
    519c:	670e           	beqs 0x51ac
    519e:	5342           	subqw #1,%d2
    51a0:	6000 000a      	braw 0x51ac
    51a4:	0c42 0008      	cmpiw #8,%d2
    51a8:	6e02           	bgts 0x51ac
    51aa:	5242           	addqw #1,%d2
    51ac:	3002           	movew %d2,%d0
    51ae:	48c0           	extl %d0
    51b0:	41ec 0100      	lea %a4@(256),%a0
    51b4:	2200           	movel %d0,%d1
    51b6:	e781           	asll #3,%d1
    51b8:	d280           	addl %d0,%d1
    51ba:	d280           	addl %d0,%d1
    51bc:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    51c0:	1030 3000      	moveb %a0@(00000000,%d3:w),%d0
    51c4:	0c00 0014      	cmpib #20,%d0
    51c8:	630c           	blss 0x51d6
    51ca:	0c00 0023      	cmpib #35,%d0
    51ce:	6706           	beqs 0x51d6
    51d0:	0c00 0026      	cmpib #38,%d0
    51d4:	663e           	bnes 0x5214
    51d6:	41ec 0100      	lea %a4@(256),%a0
    51da:	43e8 0112      	lea %a0@(274),%a1
    51de:	4a11           	tstb %a1@
    51e0:	6732           	beqs 0x5214
    51e2:	117c 0001 0123 	moveb #1,%a0@(291)
    51e8:	117c 0009 0109 	moveb #9,%a0@(265)
    51ee:	5769 fff4      	subqw #3,%a1@(-12)
    51f2:	0c29 0001 fffa 	cmpib #1,%a1@(-6)
    51f8:	660c           	bnes 0x5206
    51fa:	4868 0104      	pea %a0@(260)
    51fe:	6100 d42e      	bsrw 0x262e
    5202:	6000 000a      	braw 0x520e
    5206:	4868 0104      	pea %a0@(260)
    520a:	6100 d706      	bsrw 0x2912
    520e:	7001           	moveq #1,%d0
    5210:	6000 000a      	braw 0x521c
    5214:	197c 0008 000a 	moveb #8,%a4@(10)
    521a:	4240           	clrw %d0
    521c:	262e fff8      	movel %fp@(-8),%d3
    5220:	246e fffc      	moveal %fp@(-4),%a2
    5224:	4e5e           	unlk %fp
    5226:	4e75           	rts
    5228:	4e56 0000      	linkw %fp,#0
    522c:	48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-
    5230:	4243           	clrw %d3
    5232:	47ec 0100      	lea %a4@(256),%a3
    5236:	45eb 0108      	lea %a3@(264),%a2
    523a:	4a12           	tstb %a2@
    523c:	6700 0678      	beqw 0x58b6
    5240:	122a 0019      	moveb %a2@(25),%d1
    5244:	6738           	beqs 0x527e
    5246:	41ea 0011      	lea %a2@(17),%a0
    524a:	1010           	moveb %a0@,%d0
    524c:	b200           	cmpb %d0,%d1
    524e:	622a           	bhis 0x527a
    5250:	422b 0119      	clrb %a3@(281)
    5254:	42a7           	clrl %sp@-
    5256:	4e4f           	trap #15
    5258:	a0c2           	0120302
    525a:	41ea 0008      	lea %a2@(8),%a0
    525e:	7a03           	moveq #3,%d5
    5260:	48c0           	extl %d0
    5262:	81c5           	divsw %d5,%d0
    5264:	2200           	movel %d0,%d1
    5266:	4841           	swap %d1
    5268:	d210           	addb %a0@,%d1
    526a:	1081           	moveb %d1,%a0@
    526c:	102a 0007      	moveb %a2@(7),%d0
    5270:	584f           	addqw #4,%sp
    5272:	b001           	cmpb %d1,%d0
    5274:	6208           	bhis 0x527e
    5276:	6000 0004      	braw 0x527c
    527a:	5200           	addqb #1,%d0
    527c:	1080           	moveb %d0,%a0@
    527e:	45ec 0100      	lea %a4@(256),%a2
    5282:	47ea 0110      	lea %a2@(272),%a3
    5286:	4a13           	tstb %a3@
    5288:	6608           	bnes 0x5292
    528a:	422c 000f      	clrb %a4@(15)
    528e:	6000 0626      	braw 0x58b6
    5292:	41ec 0014      	lea %a4@(20),%a0
    5296:	1010           	moveb %a0@,%d0
    5298:	6730           	beqs 0x52ca
    529a:	1a00           	moveb %d0,%d5
    529c:	5305           	subqb #1,%d5
    529e:	1085           	moveb %d5,%a0@
    52a0:	0c00 0001      	cmpib #1,%d0
    52a4:	6624           	bnes 0x52ca
    52a6:	422c 000f      	clrb %a4@(15)
    52aa:	102b 0010      	moveb %a3@(16),%d0
    52ae:	6700 0606      	beqw 0x58b6
    52b2:	42a7           	clrl %sp@-
    52b4:	3f3c 0002      	movew #2,%sp@-
    52b8:	4267           	clrw %sp@-
    52ba:	1f40 0001      	moveb %d0,%sp@(1)
    52be:	6100 f3cc      	bsrw 0x468c
    52c2:	422a 0120      	clrb %a2@(288)
    52c6:	6000 05ee      	braw 0x58b6
    52ca:	45ec 0100      	lea %a4@(256),%a2
    52ce:	47ea 0123      	lea %a2@(291),%a3
    52d2:	4a13           	tstb %a3@
    52d4:	6720           	beqs 0x52f6
    52d6:	0c2b 0001 ffe9 	cmpib #1,%a3@(-23)
    52dc:	660c           	bnes 0x52ea
    52de:	486a 0104      	pea %a2@(260)
    52e2:	6100 d34a      	bsrw 0x262e
    52e6:	6000 05ce      	braw 0x58b6
    52ea:	486a 0104      	pea %a2@(260)
    52ee:	6100 d622      	bsrw 0x2912
    52f2:	6000 05c2      	braw 0x58b6
    52f6:	6100 fdf2      	bsrw 0x50ea
    52fa:	0c00 0001      	cmpib #1,%d0
    52fe:	6700 05b6      	beqw 0x58b6
    5302:	41ec 0009      	lea %a4@(9),%a0
    5306:	1010           	moveb %a0@,%d0
    5308:	6708           	beqs 0x5312
    530a:	5300           	subqb #1,%d0
    530c:	1080           	moveb %d0,%a0@
    530e:	6000 05a6      	braw 0x58b6
    5312:	6100 eb94      	bsrw 0x3ea8
    5316:	4a00           	tstb %d0
    5318:	6700 02ea      	beqw 0x5604
    531c:	41ec 00d0      	lea %a4@(208),%a0
    5320:	4a28 000c      	tstb %a0@(12)
    5324:	6700 02de      	beqw 0x5604
    5328:	4a2b ffee      	tstb %a3@(-18)
    532c:	673e           	beqs 0x536c
    532e:	3a2b ffe1      	movew %a3@(-31),%d5
    5332:	ba50           	cmpw %a0@,%d5
    5334:	6f12           	bles 0x5348
    5336:	4a2b ffe9      	tstb %a3@(-23)
    533a:	660c           	bnes 0x5348
    533c:	486a 0104      	pea %a2@(260)
    5340:	6100 d2ec      	bsrw 0x262e
    5344:	6000 0024      	braw 0x536a
    5348:	43ec 0100      	lea %a4@(256),%a1
    534c:	2009           	movel %a1,%d0
    534e:	2040           	moveal %d0,%a0
    5350:	d0fc 0104      	addaw #260,%a0
    5354:	3a10           	movew %a0@,%d5
    5356:	ba6c 00d0      	cmpw %a4@(208),%d5
    535a:	6c10           	bges 0x536c
    535c:	0c28 0001 0008 	cmpib #1,%a0@(8)
    5362:	6608           	bnes 0x536c
    5364:	2f08           	movel %a0,%sp@-
    5366:	6100 d5aa      	bsrw 0x2912
    536a:	584f           	addqw #4,%sp
    536c:	47ec 0100      	lea %a4@(256),%a3
    5370:	45eb 011b      	lea %a3@(283),%a2
    5374:	1012           	moveb %a2@,%d0
    5376:	672c           	beqs 0x53a4
    5378:	42a7           	clrl %sp@-
    537a:	3f3c 0002      	movew #2,%sp@-
    537e:	4267           	clrw %sp@-
    5380:	1f40 0001      	moveb %d0,%sp@(1)
    5384:	6100 f306      	bsrw 0x468c
    5388:	421a           	clrb %a2@+
    538a:	504f           	addqw #8,%sp
    538c:	1012           	moveb %a2@,%d0
    538e:	6730           	beqs 0x53c0
    5390:	42a7           	clrl %sp@-
    5392:	4267           	clrw %sp@-
    5394:	4267           	clrw %sp@-
    5396:	1f40 0001      	moveb %d0,%sp@(1)
    539a:	6100 f2f0      	bsrw 0x468c
    539e:	4212           	clrb %a2@
    53a0:	6000 001c      	braw 0x53be
    53a4:	102a 0001      	moveb %a2@(1),%d0
    53a8:	6716           	beqs 0x53c0
    53aa:	42a7           	clrl %sp@-
    53ac:	3f3c 0001      	movew #1,%sp@-
    53b0:	4267           	clrw %sp@-
    53b2:	1f40 0001      	moveb %d0,%sp@(1)
    53b6:	6100 f2d4      	bsrw 0x468c
    53ba:	422b 011c      	clrb %a3@(284)
    53be:	504f           	addqw #8,%sp
    53c0:	47ec 0100      	lea %a4@(256),%a3
    53c4:	45eb 011a      	lea %a3@(282),%a2
    53c8:	4a12           	tstb %a2@
    53ca:	663e           	bnes 0x540a
    53cc:	122a 0005      	moveb %a2@(5),%d1
    53d0:	6738           	beqs 0x540a
    53d2:	102a fff6      	moveb %a2@(-10),%d0
    53d6:	6732           	beqs 0x540a
    53d8:	0c00 0004      	cmpib #4,%d0
    53dc:	622c           	bhis 0x540a
    53de:	42a7           	clrl %sp@-
    53e0:	3f3c 0002      	movew #2,%sp@-
    53e4:	4267           	clrw %sp@-
    53e6:	1f41 0001      	moveb %d1,%sp@(1)
    53ea:	6100 f2a0      	bsrw 0x468c
    53ee:	422b 011f      	clrb %a3@(287)
    53f2:	504f           	addqw #8,%sp
    53f4:	4a2a 0006      	tstb %a2@(6)
    53f8:	6710           	beqs 0x540a
    53fa:	14bc 0001      	moveb #1,%a2@
    53fe:	197c 0032 0014 	moveb #50,%a4@(20)
    5404:	197c 0006 000a 	moveb #6,%a4@(10)
    540a:	41ec 0100      	lea %a4@(256),%a0
    540e:	45e8 010b      	lea %a0@(267),%a2
    5412:	0c12 0001      	cmpib #1,%a2@
    5416:	6634           	bnes 0x544c
    5418:	4a2a 0010      	tstb %a2@(16)
    541c:	662e           	bnes 0x544c
    541e:	202a 0011      	movel %a2@(17),%d0
    5422:	0240 ff00      	andiw #-256,%d0
    5426:	4a80           	tstl %d0
    5428:	6622           	bnes 0x544c
    542a:	0c2a 0032 000f 	cmpib #50,%a2@(15)
    5430:	6716           	beqs 0x5448
    5432:	117c 0009 0109 	moveb #9,%a0@(265)
    5438:	197c 0014 0009 	moveb #20,%a4@(9)
    543e:	117c 0032 011a 	moveb #50,%a0@(282)
    5444:	6000 0006      	braw 0x544c
    5448:	4228 0108      	clrb %a0@(264)
    544c:	0c6c 00fe 0230 	cmpiw #254,%a4@(560)
    5452:	667e           	bnes 0x54d2
    5454:	45ec 0100      	lea %a4@(256),%a2
    5458:	47ea 011a      	lea %a2@(282),%a3
    545c:	1213           	moveb %a3@,%d1
    545e:	6772           	beqs 0x54d2
    5460:	4a2c 000f      	tstb %a4@(15)
    5464:	666c           	bnes 0x54d2
    5466:	102b 0003      	moveb %a3@(3),%d0
    546a:	6718           	beqs 0x5484
    546c:	42a7           	clrl %sp@-
    546e:	3f3c 0002      	movew #2,%sp@-
    5472:	4267           	clrw %sp@-
    5474:	1f40 0001      	moveb %d0,%sp@(1)
    5478:	6100 f212      	bsrw 0x468c
    547c:	422a 011d      	clrb %a2@(285)
    5480:	6000 0434      	braw 0x58b6
    5484:	102b 0004      	moveb %a3@(4),%d0
    5488:	6748           	beqs 0x54d2
    548a:	4a2c 0013      	tstb %a4@(19)
    548e:	6642           	bnes 0x54d2
    5490:	0c01 0001      	cmpib #1,%d1
    5494:	633c           	blss 0x54d2
    5496:	42a7           	clrl %sp@-
    5498:	3f3c 0002      	movew #2,%sp@-
    549c:	4267           	clrw %sp@-
    549e:	1f40 0001      	moveb %d0,%sp@(1)
    54a2:	6100 f1e8      	bsrw 0x468c
    54a6:	1013           	moveb %a3@,%d0
    54a8:	0c00 0006      	cmpib #6,%d0
    54ac:	670c           	beqs 0x54ba
    54ae:	0c00 000b      	cmpib #11,%d0
    54b2:	6706           	beqs 0x54ba
    54b4:	0c00 0015      	cmpib #21,%d0
    54b8:	660a           	bnes 0x54c4
    54ba:	197c 0001 0013 	moveb #1,%a4@(19)
    54c0:	6000 03f4      	braw 0x58b6
    54c4:	422a 011e      	clrb %a2@(286)
    54c8:	197c 0032 0009 	moveb #50,%a4@(9)
    54ce:	6000 03e6      	braw 0x58b6
    54d2:	45ec 0100      	lea %a4@(256),%a2
    54d6:	47ea 0122      	lea %a2@(290),%a3
    54da:	4a13           	tstb %a3@
    54dc:	6778           	beqs 0x5556
    54de:	4a2c 0015      	tstb %a4@(21)
    54e2:	6600 0120      	bnew 0x5604
    54e6:	43ec 00d0      	lea %a4@(208),%a1
    54ea:	2809           	movel %a1,%d4
    54ec:	3011           	movew %a1@,%d0
    54ee:	906b ffe2      	subw %a3@(-30),%d0
    54f2:	6a02           	bpls 0x54f6
    54f4:	4440           	negw %d0
    54f6:	0c40 0014      	cmpiw #20,%d0
    54fa:	6f40           	bles 0x553c
    54fc:	4a2b fff8      	tstb %a3@(-8)
    5500:	6602           	bnes 0x5504
    5502:	7655           	moveq #85,%d3
    5504:	42a7           	clrl %sp@-
    5506:	4e4f           	trap #15
    5508:	a0c2           	0120302
    550a:	7a64           	moveq #100,%d5
    550c:	48c0           	extl %d0
    550e:	81c5           	divsw %d5,%d0
    5510:	2200           	movel %d0,%d1
    5512:	4841           	swap %d1
    5514:	584f           	addqw #4,%sp
    5516:	b641           	cmpw %d1,%d3
    5518:	6f00 00ea      	blew 0x5604
    551c:	197c 0001 000a 	moveb #1,%a4@(10)
    5522:	2f04           	movel %d4,%sp@-
    5524:	486a 0104      	pea %a2@(260)
    5528:	6100 ea9e      	bsrw 0x3fc8
    552c:	1a3c 002d      	moveb #45,%d5
    5530:	9a2b fff1      	subb %a3@(-15),%d5
    5534:	1945 0009      	moveb %d5,%a4@(9)
    5538:	6000 037c      	braw 0x58b6
    553c:	41ec 000a      	lea %a4@(10),%a0
    5540:	0c10 0006      	cmpib #6,%a0@
    5544:	6700 00be      	beqw 0x5604
    5548:	10bc 0008      	moveb #8,%a0@
    554c:	197c 0008 0009 	moveb #8,%a4@(9)
    5552:	6000 00b0      	braw 0x5604
    5556:	342c 00d0      	movew %a4@(208),%d2
    555a:	322b ffe2      	movew %a3@(-30),%d1
    555e:	3002           	movew %d2,%d0
    5560:	9041           	subw %d1,%d0
    5562:	6a02           	bpls 0x5566
    5564:	4440           	negw %d0
    5566:	0c40 001d      	cmpiw #29,%d0
    556a:	6e00 0098      	bgtw 0x5604
    556e:	b242           	cmpw %d2,%d1
    5570:	6c08           	bges 0x557a
    5572:	422a 010c      	clrb %a2@(268)
    5576:	6000 0008      	braw 0x5580
    557a:	157c 0001 010c 	moveb #1,%a2@(268)
    5580:	47ec 0100      	lea %a4@(256),%a3
    5584:	45eb 011a      	lea %a3@(282),%a2
    5588:	4a12           	tstb %a2@
    558a:	6602           	bnes 0x558e
    558c:	7650           	moveq #80,%d3
    558e:	4a2a 0008      	tstb %a2@(8)
    5592:	6704           	beqs 0x5598
    5594:	0643 ffce      	addiw #-50,%d3
    5598:	1a2a fff5      	moveb %a2@(-11),%d5
    559c:	ba2a fff6      	cmpb %a2@(-10),%d5
    55a0:	6604           	bnes 0x55a6
    55a2:	0643 ffe2      	addiw #-30,%d3
    55a6:	42a7           	clrl %sp@-
    55a8:	4e4f           	trap #15
    55aa:	a0c2           	0120302
    55ac:	7a64           	moveq #100,%d5
    55ae:	48c0           	extl %d0
    55b0:	81c5           	divsw %d5,%d0
    55b2:	2200           	movel %d0,%d1
    55b4:	4841           	swap %d1
    55b6:	584f           	addqw #4,%sp
    55b8:	b641           	cmpw %d1,%d3
    55ba:	6f34           	bles 0x55f0
    55bc:	197c 0001 000a 	moveb #1,%a4@(10)
    55c2:	486c 00d0      	pea %a4@(208)
    55c6:	486b 0104      	pea %a3@(260)
    55ca:	6100 e9fc      	bsrw 0x3fc8
    55ce:	102a fff9      	moveb %a2@(-7),%d0
    55d2:	0c00 000f      	cmpib #15,%d0
    55d6:	630a           	blss 0x55e2
    55d8:	197c 0002 0009 	moveb #2,%a4@(9)
    55de:	6000 02d6      	braw 0x58b6
    55e2:	1a3c 000f      	moveb #15,%d5
    55e6:	9a00           	subb %d0,%d5
    55e8:	1945 0009      	moveb %d5,%a4@(9)
    55ec:	6000 02c8      	braw 0x58b6
    55f0:	41ec 000a      	lea %a4@(10),%a0
    55f4:	0c10 0006      	cmpib #6,%a0@
    55f8:	670a           	beqs 0x5604
    55fa:	10bc 0008      	moveb #8,%a0@
    55fe:	197c 000a 0009 	moveb #10,%a4@(9)
    5604:	7646           	moveq #70,%d3
    5606:	0c2c 0002 000a 	cmpib #2,%a4@(10)
    560c:	660e           	bnes 0x561c
    560e:	41ec 0100      	lea %a4@(256),%a0
    5612:	0c68 0027 0104 	cmpiw #39,%a0@(260)
    5618:	6e02           	bgts 0x561c
    561a:	7623           	moveq #35,%d3
    561c:	0c2c 0003 000a 	cmpib #3,%a4@(10)
    5622:	6610           	bnes 0x5634
    5624:	41ec 0100      	lea %a4@(256),%a0
    5628:	0c68 0078 0104 	cmpiw #120,%a0@(260)
    562e:	6f04           	bles 0x5634
    5630:	0643 ffdd      	addiw #-35,%d3
    5634:	0c2c 0006 000a 	cmpib #6,%a4@(10)
    563a:	6610           	bnes 0x564c
    563c:	41ec 0100      	lea %a4@(256),%a0
    5640:	0c28 0004 0110 	cmpib #4,%a0@(272)
    5646:	6204           	bhis 0x564c
    5648:	0643 0014      	addiw #20,%d3
    564c:	43ec 0100      	lea %a4@(256),%a1
    5650:	2009           	movel %a1,%d0
    5652:	2040           	moveal %d0,%a0
    5654:	d0fc 0123      	addaw #291,%a0
    5658:	4a10           	tstb %a0@
    565a:	671a           	beqs 0x5676
    565c:	0c28 0001 ffe9 	cmpib #1,%a0@(-23)
    5662:	660a           	bnes 0x566e
    5664:	197c 0002 000a 	moveb #2,%a4@(10)
    566a:	6000 0008      	braw 0x5674
    566e:	197c 0003 000a 	moveb #3,%a4@(10)
    5674:	7664           	moveq #100,%d3
    5676:	42a7           	clrl %sp@-
    5678:	4e4f           	trap #15
    567a:	a0c2           	0120302
    567c:	7a64           	moveq #100,%d5
    567e:	48c0           	extl %d0
    5680:	81c5           	divsw %d5,%d0
    5682:	2200           	movel %d0,%d1
    5684:	4841           	swap %d1
    5686:	584f           	addqw #4,%sp
    5688:	b641           	cmpw %d1,%d3
    568a:	6e2c           	bgts 0x56b8
    568c:	45ec 000a      	lea %a4@(10),%a2
    5690:	42a7           	clrl %sp@-
    5692:	4e4f           	trap #15
    5694:	a0c2           	0120302
    5696:	7a06           	moveq #6,%d5
    5698:	48c0           	extl %d0
    569a:	81c5           	divsw %d5,%d0
    569c:	2200           	movel %d0,%d1
    569e:	4841           	swap %d1
    56a0:	1481           	moveb %d1,%a2@
    56a2:	584f           	addqw #4,%sp
    56a4:	0c01 0001      	cmpib #1,%d1
    56a8:	6604           	bnes 0x56ae
    56aa:	14bc 0008      	moveb #8,%a2@
    56ae:	0c12 0006      	cmpib #6,%a2@
    56b2:	6304           	blss 0x56b8
    56b4:	14bc 0008      	moveb #8,%a2@
    56b8:	45ec 000a      	lea %a4@(10),%a2
    56bc:	1012           	moveb %a2@,%d0
    56be:	5500           	subqb #2,%d0
    56c0:	0c00 0001      	cmpib #1,%d0
    56c4:	6204           	bhis 0x56ca
    56c6:	6100 fa22      	bsrw 0x50ea
    56ca:	4240           	clrw %d0
    56cc:	1012           	moveb %a2@,%d0
    56ce:	0c40 0008      	cmpiw #8,%d0
    56d2:	6200 01e2      	bhiw 0x58b6
    56d6:	0280 0000 00ff 	andil #255,%d0
    56dc:	41fa 000c      	lea %pc@(0x56ea),%a0
    56e0:	d080           	addl %d0,%d0
    56e2:	3030 0800      	movew %a0@(00000000,%d0:l),%d0
    56e6:	4efb 0002      	jmp %pc@(0x56ea,%d0:w)
    56ea:	00fa           	0372
    56ec:	01cc 0012      	movepl %d0,%a4@(18)
    56f0:	0036 005a 0104 	orib #90,%fp@(00000000)@(00000000,%d0:w)
    56f6:	01aa 01cc      	bclr %d0,%a2@(460)
    56fa:	00fa           	0372
    56fc:	43ec 0100      	lea %a4@(256),%a1
    5700:	2009           	movel %a1,%d0
    5702:	2040           	moveal %d0,%a0
    5704:	d0fc 0104      	addaw #260,%a0
    5708:	0c50 0009      	cmpiw #9,%a0@
    570c:	6f00 01a8      	blew 0x58b6
    5710:	2f08           	movel %a0,%sp@-
    5712:	6100 cf1a      	bsrw 0x262e
    5716:	197c 0002 0009 	moveb #2,%a4@(9)
    571c:	6000 0198      	braw 0x58b6
    5720:	43ec 0100      	lea %a4@(256),%a1
    5724:	2009           	movel %a1,%d0
    5726:	2040           	moveal %d0,%a0
    5728:	d0fc 0104      	addaw #260,%a0
    572c:	0c50 0076      	cmpiw #118,%a0@
    5730:	6e00 0184      	bgtw 0x58b6
    5734:	2f08           	movel %a0,%sp@-
    5736:	6100 d1da      	bsrw 0x2912
    573a:	197c 0002 0009 	moveb #2,%a4@(9)
    5740:	6000 0174      	braw 0x58b6
    5744:	41ec 0100      	lea %a4@(256),%a0
    5748:	45e8 010a      	lea %a0@(266),%a2
    574c:	0c12 0003      	cmpib #3,%a2@
    5750:	6700 008a      	beqw 0x57dc
    5754:	322a fffc      	movew %a2@(-4),%d1
    5758:	3001           	movew %d1,%d0
    575a:	0640 0020      	addiw #32,%d0
    575e:	6a06           	bpls 0x5766
    5760:	3001           	movew %d1,%d0
    5762:	0640 002f      	addiw #47,%d0
    5766:	3400           	movew %d0,%d2
    5768:	e842           	asrw #4,%d2
    576a:	0c2a 0001 0002 	cmpib #1,%a2@(2)
    5770:	6616           	bnes 0x5788
    5772:	322a fffa      	movew %a2@(-6),%d1
    5776:	3001           	movew %d1,%d0
    5778:	0640 000c      	addiw #12,%d0
    577c:	6a1c           	bpls 0x579a
    577e:	3001           	movew %d1,%d0
    5780:	0640 001b      	addiw #27,%d0
    5784:	6000 0014      	braw 0x579a
    5788:	322a fffa      	movew %a2@(-6),%d1
    578c:	3001           	movew %d1,%d0
    578e:	0640 0014      	addiw #20,%d0
    5792:	6a06           	bpls 0x579a
    5794:	3001           	movew %d1,%d0
    5796:	0640 0023      	addiw #35,%d0
    579a:	e840           	asrw #4,%d0
    579c:	48c0           	extl %d0
    579e:	41ec 0100      	lea %a4@(256),%a0
    57a2:	2200           	movel %d0,%d1
    57a4:	e781           	asll #3,%d1
    57a6:	d280           	addl %d0,%d1
    57a8:	d280           	addl %d0,%d1
    57aa:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    57ae:	1030 2000      	moveb %a0@(00000000,%d2:w),%d0
    57b2:	0c00 0001      	cmpib #1,%d0
    57b6:	6720           	beqs 0x57d8
    57b8:	0c00 0003      	cmpib #3,%d0
    57bc:	671a           	beqs 0x57d8
    57be:	0c00 0014      	cmpib #20,%d0
    57c2:	6714           	beqs 0x57d8
    57c4:	0c00 0007      	cmpib #7,%d0
    57c8:	670e           	beqs 0x57d8
    57ca:	0c00 0009      	cmpib #9,%d0
    57ce:	6708           	beqs 0x57d8
    57d0:	0c00 0015      	cmpib #21,%d0
    57d4:	6600 00e0      	bnew 0x58b6
    57d8:	41ec 0100      	lea %a4@(256),%a0
    57dc:	4868 0104      	pea %a0@(260)
    57e0:	6100 d572      	bsrw 0x2d54
    57e4:	197c 0005 0009 	moveb #5,%a4@(9)
    57ea:	6000 00ca      	braw 0x58b6
    57ee:	41ec 0100      	lea %a4@(256),%a0
    57f2:	45e8 010a      	lea %a0@(266),%a2
    57f6:	0c12 0003      	cmpib #3,%a2@
    57fa:	6700 008c      	beqw 0x5888
    57fe:	322a fffc      	movew %a2@(-4),%d1
    5802:	3001           	movew %d1,%d0
    5804:	0640 0020      	addiw #32,%d0
    5808:	6a06           	bpls 0x5810
    580a:	3001           	movew %d1,%d0
    580c:	0640 002f      	addiw #47,%d0
    5810:	3400           	movew %d0,%d2
    5812:	e842           	asrw #4,%d2
    5814:	0c2a 0001 0002 	cmpib #1,%a2@(2)
    581a:	6614           	bnes 0x5830
    581c:	322a fffa      	movew %a2@(-6),%d1
    5820:	3001           	movew %d1,%d0
    5822:	5040           	addqw #8,%d0
    5824:	6a1c           	bpls 0x5842
    5826:	3001           	movew %d1,%d0
    5828:	0640 0017      	addiw #23,%d0
    582c:	6000 0014      	braw 0x5842
    5830:	322a fffa      	movew %a2@(-6),%d1
    5834:	3001           	movew %d1,%d0
    5836:	0640 0018      	addiw #24,%d0
    583a:	6a06           	bpls 0x5842
    583c:	3001           	movew %d1,%d0
    583e:	0640 0027      	addiw #39,%d0
    5842:	e840           	asrw #4,%d0
    5844:	48c0           	extl %d0
    5846:	41ec 0100      	lea %a4@(256),%a0
    584a:	2200           	movel %d0,%d1
    584c:	e781           	asll #3,%d1
    584e:	d280           	addl %d0,%d1
    5850:	d280           	addl %d0,%d1
    5852:	41f0 181e      	lea %a0@(0000001e,%d1:l),%a0
    5856:	1030 2000      	moveb %a0@(00000000,%d2:w),%d0
    585a:	0c00 0008      	cmpib #8,%d0
    585e:	6712           	beqs 0x5872
    5860:	0c00 000a      	cmpib #10,%d0
    5864:	670c           	beqs 0x5872
    5866:	0c00 0002      	cmpib #2,%d0
    586a:	6706           	beqs 0x5872
    586c:	0c00 0006      	cmpib #6,%d0
    5870:	6644           	bnes 0x58b6
    5872:	41ec 0100      	lea %a4@(256),%a0
    5876:	4868 0104      	pea %a0@(260)
    587a:	6100 d714      	bsrw 0x2f90
    587e:	197c 0002 0009 	moveb #2,%a4@(9)
    5884:	6000 0030      	braw 0x58b6
    5888:	4868 0104      	pea %a0@(260)
    588c:	6100 d702      	bsrw 0x2f90
    5890:	6000 ff52      	braw 0x57e4
    5894:	41ec 0100      	lea %a4@(256),%a0
    5898:	0c28 0004 0110 	cmpib #4,%a0@(272)
    589e:	630a           	blss 0x58aa
    58a0:	197c 0003 0009 	moveb #3,%a4@(9)
    58a6:	6000 000e      	braw 0x58b6
    58aa:	117c 0006 0109 	moveb #6,%a0@(265)
    58b0:	197c 0019 0009 	moveb #25,%a4@(9)
    58b6:	4cee 0c38 ffec 	moveml %fp@(-20),%d3-%d5/%a2-%a3
    58bc:	4e5e           	unlk %fp
    58be:	4e75           	rts
    58c0:	4e56 fff8      	linkw %fp,#-8
    58c4:	43ec 0100      	lea %a4@(256),%a1
    58c8:	2009           	movel %a1,%d0
    58ca:	2040           	moveal %d0,%a0
    58cc:	d0fc 0108      	addaw #264,%a0
    58d0:	4a10           	tstb %a0@
    58d2:	6700 00da      	beqw 0x59ae
    58d6:	3d7c 0020 fffe 	movew #32,%fp@(-2)
    58dc:	4280           	clrl %d0
    58de:	302e fffe      	movew %fp@(-2),%d0
    58e2:	08c0 0015      	bset #21,%d0
    58e6:	2d40 fffc      	movel %d0,%fp@(-4)
    58ea:	0c28 0003 0002 	cmpib #3,%a0@(2)
    58f0:	6752           	beqs 0x5944
    58f2:	1228 0001      	moveb %a0@(1),%d1
    58f6:	0c01 0009      	cmpib #9,%d1
    58fa:	6228           	bhis 0x5924
    58fc:	2001           	movel %d1,%d0
    58fe:	eb40           	aslw #5,%d0
    5900:	4840           	swap %d0
    5902:	4240           	clrw %d0
    5904:	0280 1fe0 0000 	andil #534773760,%d0
    590a:	806e fffa      	orw %fp@(-6),%d0
    590e:	2d40 fff8      	movel %d0,%fp@(-8)
    5912:	1028 0004      	moveb %a0@(4),%d0
    5916:	eb40           	aslw #5,%d0
    5918:	0240 1fe0      	andiw #8160,%d0
    591c:	3d40 fffa      	movew %d0,%fp@(-6)
    5920:	6000 006a      	braw 0x598c
    5924:	4240           	clrw %d0
    5926:	1028 0004      	moveb %a0@(4),%d0
    592a:	eb40           	aslw #5,%d0
    592c:	0640 0040      	addiw #64,%d0
    5930:	3d40 fffa      	movew %d0,%fp@(-6)
    5934:	4240           	clrw %d0
    5936:	1001           	moveb %d1,%d0
    5938:	5940           	subqw #4,%d0
    593a:	eb40           	aslw #5,%d0
    593c:	4840           	swap %d0
    593e:	4240           	clrw %d0
    5940:	6000 0042      	braw 0x5984
    5944:	4240           	clrw %d0
    5946:	1028 0004      	moveb %a0@(4),%d0
    594a:	eb40           	aslw #5,%d0
    594c:	0640 0040      	addiw #64,%d0
    5950:	3d40 fffa      	movew %d0,%fp@(-6)
    5954:	1028 0001      	moveb %a0@(1),%d0
    5958:	0c00 0005      	cmpib #5,%d0
    595c:	621a           	bhis 0x5978
    595e:	0c28 0001 0005 	cmpib #1,%a0@(5)
    5964:	6712           	beqs 0x5978
    5966:	0240 00ff      	andiw #255,%d0
    596a:	eb40           	aslw #5,%d0
    596c:	0640 0060      	addiw #96,%d0
    5970:	4840           	swap %d0
    5972:	4240           	clrw %d0
    5974:	6000 000e      	braw 0x5984
    5978:	eb40           	aslw #5,%d0
    597a:	4840           	swap %d0
    597c:	4240           	clrw %d0
    597e:	0280 1fe0 0000 	andil #534773760,%d0
    5984:	806e fffa      	orw %fp@(-6),%d0
    5988:	2d40 fff8      	movel %d0,%fp@(-8)
    598c:	1f3c 0004      	moveb #4,%sp@-
    5990:	41ec 0100      	lea %a4@(256),%a0
    5994:	d0fc 0106      	addaw #262,%a0
    5998:	3f10           	movew %a0@,%sp@-
    599a:	3f28 fffe      	movew %a0@(-2),%sp@-
    599e:	486e fff8      	pea %fp@(-8)
    59a2:	2f2c 0084      	movel %a4@(132),%sp@-
    59a6:	2f2c 008c      	movel %a4@(140),%sp@-
    59aa:	4e4f           	trap #15
    59ac:	a209           	0121011
    59ae:	4e5e           	unlk %fp
    59b0:	4e75           	rts
    59b2:	4e56 0000      	linkw %fp,#0
    59b6:	48e7 1c20      	moveml %d3-%d5/%a2,%sp@-
    59ba:	45ec 00c0      	lea %a4@(192),%a2
    59be:	2012           	movel %a2@,%d0
    59c0:	676a           	beqs 0x5a2c
    59c2:	4267           	clrw %sp@-
    59c4:	2f00           	movel %d0,%sp@-
    59c6:	4e4f           	trap #15
    59c8:	a05c           	0120134
    59ca:	2f08           	movel %a0,%sp@-
    59cc:	4e4f           	trap #15
    59ce:	a021           	0120041
    59d0:	2608           	movel %a0,%d3
    59d2:	2f03           	movel %d3,%sp@-
    59d4:	4e4f           	trap #15
    59d6:	a0ce           	0120316
    59d8:	382e 0008      	movew %fp@(8),%d4
    59dc:	d840           	addw %d0,%d4
    59de:	2f03           	movel %d3,%sp@-
    59e0:	4e4f           	trap #15
    59e2:	a035           	0120065
    59e4:	4227           	clrb %sp@-
    59e6:	4267           	clrw %sp@-
    59e8:	2f12           	movel %a2@,%sp@-
    59ea:	4e4f           	trap #15
    59ec:	a05e           	0120136
    59ee:	2f2c 008c      	movel %a4@(140),%sp@-
    59f2:	4e4f           	trap #15
    59f4:	a1fd           	0120775
    59f6:	2a08           	movel %a0,%d5
    59f8:	3f04           	movew %d4,%sp@-
    59fa:	2f12           	movel %a2@,%sp@-
    59fc:	4e4f           	trap #15
    59fe:	a05c           	0120134
    5a00:	2608           	movel %a0,%d3
    5a02:	defc 0020      	addaw #32,%sp
    5a06:	2e83           	movel %d3,%sp@
    5a08:	4e4f           	trap #15
    5a0a:	a021           	0120041
    5a0c:	4267           	clrw %sp@-
    5a0e:	4267           	clrw %sp@-
    5a10:	2f08           	movel %a0,%sp@-
    5a12:	4e4f           	trap #15
    5a14:	a226           	0121046
    5a16:	2f03           	movel %d3,%sp@-
    5a18:	4e4f           	trap #15
    5a1a:	a022           	0120042
    5a1c:	4227           	clrb %sp@-
    5a1e:	3f04           	movew %d4,%sp@-
    5a20:	2f12           	movel %a2@,%sp@-
    5a22:	4e4f           	trap #15
    5a24:	a05e           	0120136
    5a26:	2f05           	movel %d5,%sp@-
    5a28:	4e4f           	trap #15
    5a2a:	a1fd           	0120775
    5a2c:	4cee 0438 fff0 	moveml %fp@(-16),%d3-%d5/%a2
    5a32:	4e5e           	unlk %fp
    5a34:	4e75           	rts
    5a36:	4e56 0000      	linkw %fp,#0
    5a3a:	48e7 1c20      	moveml %d3-%d5/%a2,%sp@-
    5a3e:	45ec 00c0      	lea %a4@(192),%a2
    5a42:	2012           	movel %a2@,%d0
    5a44:	676a           	beqs 0x5ab0
    5a46:	4267           	clrw %sp@-
    5a48:	2f00           	movel %d0,%sp@-
    5a4a:	4e4f           	trap #15
    5a4c:	a05c           	0120134
    5a4e:	2f08           	movel %a0,%sp@-
    5a50:	4e4f           	trap #15
    5a52:	a021           	0120041
    5a54:	2608           	movel %a0,%d3
    5a56:	2f03           	movel %d3,%sp@-
    5a58:	4e4f           	trap #15
    5a5a:	a0ce           	0120316
    5a5c:	382e 0008      	movew %fp@(8),%d4
    5a60:	d840           	addw %d0,%d4
    5a62:	2f03           	movel %d3,%sp@-
    5a64:	4e4f           	trap #15
    5a66:	a035           	0120065
    5a68:	4227           	clrb %sp@-
    5a6a:	4267           	clrw %sp@-
    5a6c:	2f12           	movel %a2@,%sp@-
    5a6e:	4e4f           	trap #15
    5a70:	a05e           	0120136
    5a72:	2f2c 0088      	movel %a4@(136),%sp@-
    5a76:	4e4f           	trap #15
    5a78:	a1fd           	0120775
    5a7a:	2a08           	movel %a0,%d5
    5a7c:	3f04           	movew %d4,%sp@-
    5a7e:	2f12           	movel %a2@,%sp@-
    5a80:	4e4f           	trap #15
    5a82:	a05c           	0120134
    5a84:	2608           	movel %a0,%d3
    5a86:	defc 0020      	addaw #32,%sp
    5a8a:	2e83           	movel %d3,%sp@
    5a8c:	4e4f           	trap #15
    5a8e:	a021           	0120041
    5a90:	4267           	clrw %sp@-
    5a92:	4267           	clrw %sp@-
    5a94:	2f08           	movel %a0,%sp@-
    5a96:	4e4f           	trap #15
    5a98:	a226           	0121046
    5a9a:	2f03           	movel %d3,%sp@-
    5a9c:	4e4f           	trap #15
    5a9e:	a022           	0120042
    5aa0:	4227           	clrb %sp@-
    5aa2:	3f04           	movew %d4,%sp@-
    5aa4:	2f12           	movel %a2@,%sp@-
    5aa6:	4e4f           	trap #15
    5aa8:	a05e           	0120136
    5aaa:	2f05           	movel %d5,%sp@-
    5aac:	4e4f           	trap #15
    5aae:	a1fd           	0120775
    5ab0:	4cee 0438 fff0 	moveml %fp@(-16),%d3-%d5/%a2
    5ab6:	4e5e           	unlk %fp
    5ab8:	4e75           	rts
    5aba:	4e56 0000      	linkw %fp,#0
    5abe:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    5ac2:	246e 0008      	moveal %fp@(8),%a2
    5ac6:	266e 000c      	moveal %fp@(12),%a3
    5aca:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    5ad0:	661e           	bnes 0x5af0
    5ad2:	102a 0005      	moveb %a2@(5),%d0
    5ad6:	5d00           	subqb #6,%d0
    5ad8:	0c00 0001      	cmpib #1,%d0
    5adc:	6212           	bhis 0x5af0
    5ade:	102a 0021      	moveb %a2@(33),%d0
    5ae2:	6700 0322      	beqw 0x5e06
    5ae6:	5300           	subqb #1,%d0
    5ae8:	1540 0021      	moveb %d0,%a2@(33)
    5aec:	6000 031c      	braw 0x5e0a
    5af0:	0c2a 0007 0005 	cmpib #7,%a2@(5)
    5af6:	6600 0192      	bnew 0x5c8a
    5afa:	102a 0021      	moveb %a2@(33),%d0
    5afe:	670a           	beqs 0x5b0a
    5b00:	5300           	subqb #1,%d0
    5b02:	1540 0021      	moveb %d0,%a2@(33)
    5b06:	6000 0182      	braw 0x5c8a
    5b0a:	422a 0005      	clrb %a2@(5)
    5b0e:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    5b14:	6600 00c2      	bnew 0x5bd8
    5b18:	2f0a           	movel %a2,%sp@-
    5b1a:	6100 cd56      	bsrw 0x2872
    5b1e:	584f           	addqw #4,%sp
    5b20:	4a00           	tstb %d0
    5b22:	6706           	beqs 0x5b2a
    5b24:	5a52           	addqw #5,%a2@
    5b26:	6000 0162      	braw 0x5c8a
    5b2a:	4a2a 001e      	tstb %a2@(30)
    5b2e:	6706           	beqs 0x5b36
    5b30:	5a52           	addqw #5,%a2@
    5b32:	6000 0004      	braw 0x5b38
    5b36:	5852           	addqw #4,%a2@
    5b38:	143c 0001      	moveb #1,%d2
    5b3c:	4a2c 000f      	tstb %a4@(15)
    5b40:	6700 0148      	beqw 0x5c8a
    5b44:	43ec 0100      	lea %a4@(256),%a1
    5b48:	2009           	movel %a1,%d0
    5b4a:	2800           	movel %d0,%d4
    5b4c:	0684 0000 0104 	addil #260,%d4
    5b52:	2604           	movel %d4,%d3
    5b54:	5c83           	addql #6,%d3
    5b56:	41ec 00d0      	lea %a4@(208),%a0
    5b5a:	3010           	movew %a0@,%d0
    5b5c:	2244           	moveal %d4,%a1
    5b5e:	9051           	subw %a1@,%d0
    5b60:	6a02           	bpls 0x5b64
    5b62:	4440           	negw %d0
    5b64:	0c40 0015      	cmpiw #21,%d0
    5b68:	6e00 0120      	bgtw 0x5c8a
    5b6c:	0c28 0003 0006 	cmpib #3,%a0@(6)
    5b72:	660a           	bnes 0x5b7e
    5b74:	2243           	moveal %d3,%a1
    5b76:	0c11 0003      	cmpib #3,%a1@
    5b7a:	6700 010e      	beqw 0x5c8a
    5b7e:	3213           	movew %a3@,%d1
    5b80:	0c41 0004      	cmpiw #4,%d1
    5b84:	6e10           	bgts 0x5b96
    5b86:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    5b8c:	6602           	bnes 0x5b90
    5b8e:	4202           	clrb %d2
    5b90:	5452           	addqw #2,%a2@
    5b92:	6000 0032      	braw 0x5bc6
    5b96:	3012           	movew %a2@,%d0
    5b98:	0c40 007b      	cmpiw #123,%d0
    5b9c:	6f12           	bles 0x5bb0
    5b9e:	0c2b 0003 0006 	cmpib #3,%a3@(6)
    5ba4:	6602           	bnes 0x5ba8
    5ba6:	4202           	clrb %d2
    5ba8:	5541           	subqw #2,%d1
    5baa:	3681           	movew %d1,%a3@
    5bac:	6000 0018      	braw 0x5bc6
    5bb0:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    5bb6:	6704           	beqs 0x5bbc
    5bb8:	5240           	addqw #1,%d0
    5bba:	3480           	movew %d0,%a2@
    5bbc:	0c2b 0003 0006 	cmpib #3,%a3@(6)
    5bc2:	6702           	beqs 0x5bc6
    5bc4:	5353           	subqw #1,%a3@
    5bc6:	4a02           	tstb %d2
    5bc8:	6700 00c0      	beqw 0x5c8a
    5bcc:	4a2c 000f      	tstb %a4@(15)
    5bd0:	6700 00b8      	beqw 0x5c8a
    5bd4:	6000 ff80      	braw 0x5b56
    5bd8:	2f0a           	movel %a2,%sp@-
    5bda:	6100 cf9a      	bsrw 0x2b76
    5bde:	584f           	addqw #4,%sp
    5be0:	4a00           	tstb %d0
    5be2:	6706           	beqs 0x5bea
    5be4:	5b52           	subqw #5,%a2@
    5be6:	6000 00a2      	braw 0x5c8a
    5bea:	4a2a 001e      	tstb %a2@(30)
    5bee:	6706           	beqs 0x5bf6
    5bf0:	5b52           	subqw #5,%a2@
    5bf2:	6000 0004      	braw 0x5bf8
    5bf6:	5952           	subqw #4,%a2@
    5bf8:	143c 0001      	moveb #1,%d2
    5bfc:	4a2c 000f      	tstb %a4@(15)
    5c00:	6700 0088      	beqw 0x5c8a
    5c04:	43ec 0100      	lea %a4@(256),%a1
    5c08:	2009           	movel %a1,%d0
    5c0a:	2600           	movel %d0,%d3
    5c0c:	0683 0000 0104 	addil #260,%d3
    5c12:	2203           	movel %d3,%d1
    5c14:	5c81           	addql #6,%d1
    5c16:	41ec 00d0      	lea %a4@(208),%a0
    5c1a:	3010           	movew %a0@,%d0
    5c1c:	2243           	moveal %d3,%a1
    5c1e:	9051           	subw %a1@,%d0
    5c20:	6a02           	bpls 0x5c24
    5c22:	4440           	negw %d0
    5c24:	0c40 0015      	cmpiw #21,%d0
    5c28:	6e60           	bgts 0x5c8a
    5c2a:	0c28 0003 0006 	cmpib #3,%a0@(6)
    5c30:	6608           	bnes 0x5c3a
    5c32:	2241           	moveal %d1,%a1
    5c34:	0c11 0003      	cmpib #3,%a1@
    5c38:	6750           	beqs 0x5c8a
    5c3a:	3012           	movew %a2@,%d0
    5c3c:	0c40 0004      	cmpiw #4,%d0
    5c40:	6e10           	bgts 0x5c52
    5c42:	0c2b 0003 0006 	cmpib #3,%a3@(6)
    5c48:	6602           	bnes 0x5c4c
    5c4a:	4202           	clrb %d2
    5c4c:	5453           	addqw #2,%a3@
    5c4e:	6000 0030      	braw 0x5c80
    5c52:	0c53 007b      	cmpiw #123,%a3@
    5c56:	6f12           	bles 0x5c6a
    5c58:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    5c5e:	6602           	bnes 0x5c62
    5c60:	4202           	clrb %d2
    5c62:	5540           	subqw #2,%d0
    5c64:	3480           	movew %d0,%a2@
    5c66:	6000 0018      	braw 0x5c80
    5c6a:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    5c70:	6704           	beqs 0x5c76
    5c72:	5340           	subqw #1,%d0
    5c74:	3480           	movew %d0,%a2@
    5c76:	0c2b 0003 0006 	cmpib #3,%a3@(6)
    5c7c:	6702           	beqs 0x5c80
    5c7e:	5253           	addqw #1,%a3@
    5c80:	4a02           	tstb %d2
    5c82:	6706           	beqs 0x5c8a
    5c84:	4a2c 000f      	tstb %a4@(15)
    5c88:	668c           	bnes 0x5c16
    5c8a:	0c2a 0008 0005 	cmpib #8,%a2@(5)
    5c90:	661c           	bnes 0x5cae
    5c92:	102a 0021      	moveb %a2@(33),%d0
    5c96:	670a           	beqs 0x5ca2
    5c98:	5300           	subqb #1,%d0
    5c9a:	1540 0021      	moveb %d0,%a2@(33)
    5c9e:	6000 000e      	braw 0x5cae
    5ca2:	157c 0007 0005 	moveb #7,%a2@(5)
    5ca8:	157c 0002 0021 	moveb #2,%a2@(33)
    5cae:	0c2a 0009 0005 	cmpib #9,%a2@(5)
    5cb4:	6610           	bnes 0x5cc6
    5cb6:	4a2a 001f      	tstb %a2@(31)
    5cba:	670a           	beqs 0x5cc6
    5cbc:	2f0a           	movel %a2,%sp@-
    5cbe:	6100 cf78      	bsrw 0x2c38
    5cc2:	6000 0146      	braw 0x5e0a
    5cc6:	102a 0005      	moveb %a2@(5),%d0
    5cca:	0c00 0006      	cmpib #6,%d0
    5cce:	6704           	beqs 0x5cd4
    5cd0:	4a00           	tstb %d0
    5cd2:	6676           	bnes 0x5d4a
    5cd4:	4a2a 000c      	tstb %a2@(12)
    5cd8:	6670           	bnes 0x5d4a
    5cda:	102a 0021      	moveb %a2@(33),%d0
    5cde:	670a           	beqs 0x5cea
    5ce0:	5300           	subqb #1,%d0
    5ce2:	1540 0021      	moveb %d0,%a2@(33)
    5ce6:	6000 0062      	braw 0x5d4a
    5cea:	157c 000c 0005 	moveb #12,%a2@(5)
    5cf0:	2f0a           	movel %a2,%sp@-
    5cf2:	6100 d4b4      	bsrw 0x31a8
    5cf6:	41ec 00d0      	lea %a4@(208),%a0
    5cfa:	584f           	addqw #4,%sp
    5cfc:	4a28 000c      	tstb %a0@(12)
    5d00:	6612           	bnes 0x5d14
    5d02:	3f3c 000c      	movew #12,%sp@-
    5d06:	6100 0700      	bsrw 0x6408
    5d0a:	6100 0b08      	bsrw 0x6814
    5d0e:	544f           	addqw #2,%sp
    5d10:	6000 0038      	braw 0x5d4a
    5d14:	3f3c 0008      	movew #8,%sp@-
    5d18:	6100 06ee      	bsrw 0x6408
    5d1c:	43ec 0100      	lea %a4@(256),%a1
    5d20:	2009           	movel %a1,%d0
    5d22:	2640           	moveal %d0,%a3
    5d24:	d6fc 011e      	addaw #286,%a3
    5d28:	544f           	addqw #2,%sp
    5d2a:	1013           	moveb %a3@,%d0
    5d2c:	671c           	beqs 0x5d4a
    5d2e:	0c2b 0001 fffc 	cmpib #1,%a3@(-4)
    5d34:	6214           	bhis 0x5d4a
    5d36:	42a7           	clrl %sp@-
    5d38:	3f3c 0002      	movew #2,%sp@-
    5d3c:	4267           	clrw %sp@-
    5d3e:	1f40 0001      	moveb %d0,%sp@(1)
    5d42:	6100 e948      	bsrw 0x468c
    5d46:	4213           	clrb %a3@
    5d48:	504f           	addqw #8,%sp
    5d4a:	0c2a 000d 0005 	cmpib #13,%a2@(5)
    5d50:	6614           	bnes 0x5d66
    5d52:	102a 0021      	moveb %a2@(33),%d0
    5d56:	670a           	beqs 0x5d62
    5d58:	5300           	subqb #1,%d0
    5d5a:	1540 0021      	moveb %d0,%a2@(33)
    5d5e:	6000 0006      	braw 0x5d66
    5d62:	422a 0005      	clrb %a2@(5)
    5d66:	102a 0005      	moveb %a2@(5),%d0
    5d6a:	6760           	beqs 0x5dcc
    5d6c:	0c00 0003      	cmpib #3,%d0
    5d70:	675a           	beqs 0x5dcc
    5d72:	0c00 0005      	cmpib #5,%d0
    5d76:	6254           	bhis 0x5dcc
    5d78:	0c2a 0003 0006 	cmpib #3,%a2@(6)
    5d7e:	661a           	bnes 0x5d9a
    5d80:	4a2a 0009      	tstb %a2@(9)
    5d84:	670a           	beqs 0x5d90
    5d86:	2f0a           	movel %a2,%sp@-
    5d88:	6100 cfca      	bsrw 0x2d54
    5d8c:	6000 003c      	braw 0x5dca
    5d90:	2f0a           	movel %a2,%sp@-
    5d92:	6100 d1fc      	bsrw 0x2f90
    5d96:	6000 0032      	braw 0x5dca
    5d9a:	4a2a 0020      	tstb %a2@(32)
    5d9e:	6712           	beqs 0x5db2
    5da0:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    5da6:	671c           	beqs 0x5dc4
    5da8:	2f0a           	movel %a2,%sp@-
    5daa:	6100 c882      	bsrw 0x262e
    5dae:	6000 001a      	braw 0x5dca
    5db2:	0c2a 0001 0008 	cmpib #1,%a2@(8)
    5db8:	660a           	bnes 0x5dc4
    5dba:	2f0a           	movel %a2,%sp@-
    5dbc:	6100 c870      	bsrw 0x262e
    5dc0:	6000 0008      	braw 0x5dca
    5dc4:	2f0a           	movel %a2,%sp@-
    5dc6:	6100 cb4a      	bsrw 0x2912
    5dca:	584f           	addqw #4,%sp
    5dcc:	4e4f           	trap #15
    5dce:	a2a2           	0121242
    5dd0:	4a2a 0005      	tstb %a2@(5)
    5dd4:	6610           	bnes 0x5de6
    5dd6:	4a2a 0021      	tstb %a2@(33)
    5dda:	670a           	beqs 0x5de6
    5ddc:	0800 0001      	btst #1,%d0
    5de0:	6604           	bnes 0x5de6
    5de2:	422a 0021      	clrb %a2@(33)
    5de6:	0c2a 0006 0005 	cmpib #6,%a2@(5)
    5dec:	661c           	bnes 0x5e0a
    5dee:	0800 0002      	btst #2,%d0
    5df2:	6616           	bnes 0x5e0a
    5df4:	4a2a 000c      	tstb %a2@(12)
    5df8:	6710           	beqs 0x5e0a
    5dfa:	41ec 00d0      	lea %a4@(208),%a0
    5dfe:	0c28 0006 0005 	cmpib #6,%a0@(5)
    5e04:	6604           	bnes 0x5e0a
    5e06:	422a 0005      	clrb %a2@(5)
    5e0a:	4cee 0c18 fff0 	moveml %fp@(-16),%d3-%d4/%a2-%a3
    5e10:	4e5e           	unlk %fp
    5e12:	4e75           	rts
    5e14:	4e56 0000      	linkw %fp,#0
    5e18:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    5e1c:	4243           	clrw %d3
    5e1e:	4244           	clrw %d4
    5e20:	3203           	movew %d3,%d1
    5e22:	c3fc 000c      	mulsw #12,%d1
    5e26:	47ec 0100      	lea %a4@(256),%a3
    5e2a:	200b           	movel %a3,%d0
    5e2c:	2241           	moveal %d1,%a1
    5e2e:	d3c0           	addal %d0,%a1
    5e30:	0c29 0009 0089 	cmpib #9,%a1@(137)
    5e36:	661e           	bnes 0x5e56
    5e38:	41ec 00d0      	lea %a4@(208),%a0
    5e3c:	1428 001c      	moveb %a0@(28),%d2
    5e40:	b429 008b      	cmpb %a1@(139),%d2
    5e44:	6510           	bcss 0x5e56
    5e46:	1029 0087      	moveb %a1@(135),%d0
    5e4a:	b029 008d      	cmpb %a1@(141),%d0
    5e4e:	6306           	blss 0x5e56
    5e50:	5500           	subqb #2,%d0
    5e52:	1340 0087      	moveb %d0,%a1@(135)
    5e56:	3203           	movew %d3,%d1
    5e58:	c3fc 000c      	mulsw #12,%d1
    5e5c:	47ec 0100      	lea %a4@(256),%a3
    5e60:	200b           	movel %a3,%d0
    5e62:	2041           	moveal %d1,%a0
    5e64:	d1c0           	addal %d0,%a0
    5e66:	0c28 0012 0089 	cmpib #18,%a0@(137)
    5e6c:	667e           	bnes 0x5eec
    5e6e:	43ec 00d0      	lea %a4@(208),%a1
    5e72:	1828 0086      	moveb %a0@(134),%d4
    5e76:	3011           	movew %a1@,%d0
    5e78:	9044           	subw %d4,%d0
    5e7a:	6a02           	bpls 0x5e7e
    5e7c:	4440           	negw %d0
    5e7e:	0c40 001f      	cmpiw #31,%d0
    5e82:	6e68           	bgts 0x5eec
    5e84:	4241           	clrw %d1
    5e86:	1228 0087      	moveb %a0@(135),%d1
    5e8a:	3269 0002      	moveaw %a1@(2),%a1
    5e8e:	92c1           	subaw %d1,%a1
    5e90:	3209           	movew %a1,%d1
    5e92:	6c02           	bges 0x5e96
    5e94:	4441           	negw %d1
    5e96:	0c41 001f      	cmpiw #31,%d1
    5e9a:	6e50           	bgts 0x5eec
    5e9c:	3f03           	movew %d3,%sp@-
    5e9e:	6100 0e98      	bsrw 0x6d38
    5ea2:	2f2c 073c      	movel %a4@(1852),%sp@-
    5ea6:	4e4f           	trap #15
    5ea8:	a04a           	0120112
    5eaa:	5c4f           	addqw #6,%sp
    5eac:	202c 00c0      	movel %a4@(192),%d0
    5eb0:	6708           	beqs 0x5eba
    5eb2:	2f00           	movel %d0,%sp@-
    5eb4:	4e4f           	trap #15
    5eb6:	a04a           	0120112
    5eb8:	584f           	addqw #4,%sp
    5eba:	486c 0025      	pea %a4@(37)
    5ebe:	4267           	clrw %sp@-
    5ec0:	4e4f           	trap #15
    5ec2:	a045           	0120105
    5ec4:	2f00           	movel %d0,%sp@-
    5ec6:	4267           	clrw %sp@-
    5ec8:	4e4f           	trap #15
    5eca:	a042           	0120102
    5ecc:	6100 1250      	bsrw 0x711e
    5ed0:	6100 b140      	bsrw 0x1012
    5ed4:	6100 af78      	bsrw 0xe4e
    5ed8:	6100 a6e0      	bsrw 0x5ba
    5edc:	6100 a8ac      	bsrw 0x78a
    5ee0:	6100 b130      	bsrw 0x1012
    5ee4:	6100 c210      	bsrw 0x20f6
    5ee8:	6000 02dc      	braw 0x61c6
    5eec:	3203           	movew %d3,%d1
    5eee:	c3fc 000c      	mulsw #12,%d1
    5ef2:	47ec 0100      	lea %a4@(256),%a3
    5ef6:	200b           	movel %a3,%d0
    5ef8:	2441           	moveal %d1,%a2
    5efa:	d5c0           	addal %d0,%a2
    5efc:	0c2a 000d 0089 	cmpib #13,%a2@(137)
    5f02:	6654           	bnes 0x5f58
    5f04:	41ec 00d0      	lea %a4@(208),%a0
    5f08:	4240           	clrw %d0
    5f0a:	102a 0086      	moveb %a2@(134),%d0
    5f0e:	3410           	movew %a0@,%d2
    5f10:	9440           	subw %d0,%d2
    5f12:	3002           	movew %d2,%d0
    5f14:	6c02           	bges 0x5f18
    5f16:	4440           	negw %d0
    5f18:	4241           	clrw %d1
    5f1a:	122a 008b      	moveb %a2@(139),%d1
    5f1e:	b240           	cmpw %d0,%d1
    5f20:	6f36           	bles 0x5f58
    5f22:	4240           	clrw %d0
    5f24:	102a 0087      	moveb %a2@(135),%d0
    5f28:	3068 0002      	moveaw %a0@(2),%a0
    5f2c:	90c0           	subaw %d0,%a0
    5f2e:	3008           	movew %a0,%d0
    5f30:	6c02           	bges 0x5f34
    5f32:	4440           	negw %d0
    5f34:	b240           	cmpw %d0,%d1
    5f36:	6f20           	bles 0x5f58
    5f38:	122a 008f      	moveb %a2@(143),%d1
    5f3c:	671a           	beqs 0x5f58
    5f3e:	42a7           	clrl %sp@-
    5f40:	4240           	clrw %d0
    5f42:	102a 008c      	moveb %a2@(140),%d0
    5f46:	3f00           	movew %d0,%sp@-
    5f48:	4267           	clrw %sp@-
    5f4a:	1f41 0001      	moveb %d1,%sp@(1)
    5f4e:	6100 e73c      	bsrw 0x468c
    5f52:	422a 008f      	clrb %a2@(143)
    5f56:	504f           	addqw #8,%sp
    5f58:	3203           	movew %d3,%d1
    5f5a:	c3fc 000c      	mulsw #12,%d1
    5f5e:	47ec 0100      	lea %a4@(256),%a3
    5f62:	200b           	movel %a3,%d0
    5f64:	2041           	moveal %d1,%a0
    5f66:	d1c0           	addal %d0,%a0
    5f68:	0c28 000c 0089 	cmpib #12,%a0@(137)
    5f6e:	6600 009c      	bnew 0x600c
    5f72:	4a28 008a      	tstb %a0@(138)
    5f76:	6752           	beqs 0x5fca
    5f78:	43ec 00d0      	lea %a4@(208),%a1
    5f7c:	4240           	clrw %d0
    5f7e:	1028 0086      	moveb %a0@(134),%d0
    5f82:	3411           	movew %a1@,%d2
    5f84:	9440           	subw %d0,%d2
    5f86:	3002           	movew %d2,%d0
    5f88:	6c02           	bges 0x5f8c
    5f8a:	4440           	negw %d0
    5f8c:	4241           	clrw %d1
    5f8e:	1228 008b      	moveb %a0@(139),%d1
    5f92:	b240           	cmpw %d0,%d1
    5f94:	6d1a           	blts 0x5fb0
    5f96:	4240           	clrw %d0
    5f98:	1028 0087      	moveb %a0@(135),%d0
    5f9c:	0640 ffe0      	addiw #-32,%d0
    5fa0:	3269 0002      	moveaw %a1@(2),%a1
    5fa4:	92c0           	subaw %d0,%a1
    5fa6:	3009           	movew %a1,%d0
    5fa8:	6c02           	bges 0x5fac
    5faa:	4440           	negw %d0
    5fac:	b240           	cmpw %d0,%d1
    5fae:	6c5c           	bges 0x600c
    5fb0:	41ec 0100      	lea %a4@(256),%a0
    5fb4:	3003           	movew %d3,%d0
    5fb6:	c1fc 000c      	mulsw #12,%d0
    5fba:	41f0 0800      	lea %a0@(00000000,%d0:l),%a0
    5fbe:	4228 008a      	clrb %a0@(138)
    5fc2:	5328 0084      	subqb #1,%a0@(132)
    5fc6:	6000 0044      	braw 0x600c
    5fca:	43ec 00d0      	lea %a4@(208),%a1
    5fce:	4240           	clrw %d0
    5fd0:	1028 0086      	moveb %a0@(134),%d0
    5fd4:	3651           	moveaw %a1@,%a3
    5fd6:	96c0           	subaw %d0,%a3
    5fd8:	300b           	movew %a3,%d0
    5fda:	6c02           	bges 0x5fde
    5fdc:	4440           	negw %d0
    5fde:	4241           	clrw %d1
    5fe0:	1228 008b      	moveb %a0@(139),%d1
    5fe4:	b240           	cmpw %d0,%d1
    5fe6:	6d24           	blts 0x600c
    5fe8:	4240           	clrw %d0
    5fea:	1028 0087      	moveb %a0@(135),%d0
    5fee:	0640 ffe0      	addiw #-32,%d0
    5ff2:	3269 0002      	moveaw %a1@(2),%a1
    5ff6:	92c0           	subaw %d0,%a1
    5ff8:	3009           	movew %a1,%d0
    5ffa:	6c02           	bges 0x5ffe
    5ffc:	4440           	negw %d0
    5ffe:	b240           	cmpw %d0,%d1
    6000:	6d0a           	blts 0x600c
    6002:	117c 0001 008a 	moveb #1,%a0@(138)
    6008:	5228 0084      	addqb #1,%a0@(132)
    600c:	3203           	movew %d3,%d1
    600e:	c3fc 000c      	mulsw #12,%d1
    6012:	47ec 0100      	lea %a4@(256),%a3
    6016:	200b           	movel %a3,%d0
    6018:	2041           	moveal %d1,%a0
    601a:	d1c0           	addal %d0,%a0
    601c:	0c28 0011 0089 	cmpib #17,%a0@(137)
    6022:	6600 009c      	bnew 0x60c0
    6026:	0c28 0001 008a 	cmpib #1,%a0@(138)
    602c:	6200 0092      	bhiw 0x60c0
    6030:	43ec 00d0      	lea %a4@(208),%a1
    6034:	4240           	clrw %d0
    6036:	1028 0086      	moveb %a0@(134),%d0
    603a:	3211           	movew %a1@,%d1
    603c:	9240           	subw %d0,%d1
    603e:	6a02           	bpls 0x6042
    6040:	4441           	negw %d1
    6042:	4240           	clrw %d0
    6044:	1028 008b      	moveb %a0@(139),%d0
    6048:	b041           	cmpw %d1,%d0
    604a:	6f20           	bles 0x606c
    604c:	4240           	clrw %d0
    604e:	1028 0087      	moveb %a0@(135),%d0
    6052:	0640 ffe0      	addiw #-32,%d0
    6056:	3269 0002      	moveaw %a1@(2),%a1
    605a:	92c0           	subaw %d0,%a1
    605c:	3009           	movew %a1,%d0
    605e:	6c02           	bges 0x6062
    6060:	4440           	negw %d0
    6062:	4241           	clrw %d1
    6064:	1228 008d      	moveb %a0@(141),%d1
    6068:	b240           	cmpw %d0,%d1
    606a:	6e16           	bgts 0x6082
    606c:	3003           	movew %d3,%d0
    606e:	c1fc 000c      	mulsw #12,%d0
    6072:	41ec 0100      	lea %a4@(256),%a0
    6076:	41f0 0800      	lea %a0@(00000000,%d0:l),%a0
    607a:	0c28 0001 008a 	cmpib #1,%a0@(138)
    6080:	663e           	bnes 0x60c0
    6082:	3203           	movew %d3,%d1
    6084:	c3fc 000c      	mulsw #12,%d1
    6088:	47ec 0100      	lea %a4@(256),%a3
    608c:	200b           	movel %a3,%d0
    608e:	2041           	moveal %d1,%a0
    6090:	d1c0           	addal %d0,%a0
    6092:	4a28 008a      	tstb %a0@(138)
    6096:	6606           	bnes 0x609e
    6098:	117c 0001 008a 	moveb #1,%a0@(138)
    609e:	4a28 008c      	tstb %a0@(140)
    60a2:	671c           	beqs 0x60c0
    60a4:	5228 0084      	addqb #1,%a0@(132)
    60a8:	1028 008c      	moveb %a0@(140),%d0
    60ac:	1400           	moveb %d0,%d2
    60ae:	5302           	subqb #1,%d2
    60b0:	1142 008c      	moveb %d2,%a0@(140)
    60b4:	0c00 0001      	cmpib #1,%d0
    60b8:	6606           	bnes 0x60c0
    60ba:	117c 0002 008a 	moveb #2,%a0@(138)
    60c0:	3203           	movew %d3,%d1
    60c2:	c3fc 000c      	mulsw #12,%d1
    60c6:	47ec 0100      	lea %a4@(256),%a3
    60ca:	200b           	movel %a3,%d0
    60cc:	2041           	moveal %d1,%a0
    60ce:	d1c0           	addal %d0,%a0
    60d0:	0c28 000a 0089 	cmpib #10,%a0@(137)
    60d6:	6600 00e4      	bnew 0x61bc
    60da:	1228 008b      	moveb %a0@(139),%d1
    60de:	6700 00a0      	beqw 0x6180
    60e2:	1028 008a      	moveb %a0@(138),%d0
    60e6:	0c00 0001      	cmpib #1,%d0
    60ea:	626a           	bhis 0x6156
    60ec:	b228 008c      	cmpb %a0@(140),%d1
    60f0:	6454           	bccs 0x6146
    60f2:	d228 008d      	addb %a0@(141),%d1
    60f6:	1141 008b      	moveb %d1,%a0@(139)
    60fa:	1028 0087      	moveb %a0@(135),%d0
    60fe:	d028 008d      	addb %a0@(141),%d0
    6102:	1140 0087      	moveb %d0,%a0@(135)
    6106:	43ec 00d0      	lea %a4@(208),%a1
    610a:	4241           	clrw %d1
    610c:	1228 008e      	moveb %a0@(142),%d1
    6110:	3411           	movew %a1@,%d2
    6112:	9441           	subw %d1,%d2
    6114:	3202           	movew %d2,%d1
    6116:	6c02           	bges 0x611a
    6118:	4441           	negw %d1
    611a:	0c41 0007      	cmpiw #7,%d1
    611e:	6e00 009c      	bgtw 0x61bc
    6122:	0240 00ff      	andiw #255,%d0
    6126:	3669 0002      	moveaw %a1@(2),%a3
    612a:	96c0           	subaw %d0,%a3
    612c:	300b           	movew %a3,%d0
    612e:	0c40 0007      	cmpiw #7,%d0
    6132:	6e00 0088      	bgtw 0x61bc
    6136:	3f3c 000c      	movew #12,%sp@-
    613a:	2f09           	movel %a1,%sp@-
    613c:	6100 e25c      	bsrw 0x439a
    6140:	5c4f           	addqw #6,%sp
    6142:	6000 0078      	braw 0x61bc
    6146:	0c00 0001      	cmpib #1,%d0
    614a:	6670           	bnes 0x61bc
    614c:	117c 0002 008a 	moveb #2,%a0@(138)
    6152:	6000 0068      	braw 0x61bc
    6156:	b028 008f      	cmpb %a0@(143),%d0
    615a:	640a           	bccs 0x6166
    615c:	5200           	addqb #1,%d0
    615e:	1140 008a      	moveb %d0,%a0@(138)
    6162:	6000 0058      	braw 0x61bc
    6166:	5301           	subqb #1,%d1
    6168:	1141 008b      	moveb %d1,%a0@(139)
    616c:	5328 0087      	subqb #1,%a0@(135)
    6170:	4a28 008b      	tstb %a0@(139)
    6174:	6646           	bnes 0x61bc
    6176:	117c 0001 008a 	moveb #1,%a0@(138)
    617c:	6000 003e      	braw 0x61bc
    6180:	43ec 00d0      	lea %a4@(208),%a1
    6184:	4240           	clrw %d0
    6186:	1028 008e      	moveb %a0@(142),%d0
    618a:	3411           	movew %a1@,%d2
    618c:	9440           	subw %d0,%d2
    618e:	3002           	movew %d2,%d0
    6190:	6c02           	bges 0x6194
    6192:	4440           	negw %d0
    6194:	0c40 000f      	cmpiw #15,%d0
    6198:	6e22           	bgts 0x61bc
    619a:	4240           	clrw %d0
    619c:	1028 0087      	moveb %a0@(135),%d0
    61a0:	3269 0002      	moveaw %a1@(2),%a1
    61a4:	92c0           	subaw %d0,%a1
    61a6:	3009           	movew %a1,%d0
    61a8:	0c40 0045      	cmpiw #69,%d0
    61ac:	6e0e           	bgts 0x61bc
    61ae:	1168 008d 008b 	moveb %a0@(141),%a0@(139)
    61b4:	1428 008d      	moveb %a0@(141),%d2
    61b8:	d528 0087      	addb %d2,%a0@(135)
    61bc:	5243           	addqw #1,%d3
    61be:	0c43 0009      	cmpiw #9,%d3
    61c2:	6f00 fc5c      	blew 0x5e20
    61c6:	4cee 0c18 fff0 	moveml %fp@(-16),%d3-%d4/%a2-%a3
    61cc:	4e5e           	unlk %fp
    61ce:	4e75           	rts
    61d0:	4e56 ffcc      	linkw %fp,#-52
    61d4:	4e4f           	trap #15
    61d6:	a1c2           	0120702
    61d8:	b0fc 0000      	cmpaw #0,%a0
    61dc:	6638           	bnes 0x6216
    61de:	4a2c 000d      	tstb %a4@(13)
    61e2:	6632           	bnes 0x6216
    61e4:	6100 d2a6      	bsrw 0x348c
    61e8:	6100 fc2a      	bsrw 0x5e14
    61ec:	6100 d324      	bsrw 0x3512
    61f0:	6100 d084      	bsrw 0x3276
    61f4:	6100 f032      	bsrw 0x5228
    61f8:	6100 f6c6      	bsrw 0x58c0
    61fc:	6100 0072      	bsrw 0x6270
    6200:	6100 e2be      	bsrw 0x44c0
    6204:	6100 e558      	bsrw 0x475e
    6208:	6100 d2c4      	bsrw 0x34ce
    620c:	4a2c 0023      	tstb %a4@(35)
    6210:	6704           	beqs 0x6216
    6212:	6100 020e      	bsrw 0x6422
    6216:	4e5e           	unlk %fp
    6218:	4e75           	rts
    621a:	4e56 0000      	linkw %fp,#0
    621e:	48e7 1820      	moveml %d3-%d4/%a2,%sp@-
    6222:	382e 000c      	movew %fp@(12),%d4
    6226:	45ec 00c0      	lea %a4@(192),%a2
    622a:	2012           	movel %a2@,%d0
    622c:	6736           	beqs 0x6264
    622e:	3f04           	movew %d4,%sp@-
    6230:	2f00           	movel %d0,%sp@-
    6232:	4e4f           	trap #15
    6234:	a05c           	0120134
    6236:	2008           	movel %a0,%d0
    6238:	5c4f           	addqw #6,%sp
    623a:	6728           	beqs 0x6264
    623c:	2f00           	movel %d0,%sp@-
    623e:	4e4f           	trap #15
    6240:	a021           	0120041
    6242:	2608           	movel %a0,%d3
    6244:	2f03           	movel %d3,%sp@-
    6246:	2f2e 0008      	movel %fp@(8),%sp@-
    624a:	4e4f           	trap #15
    624c:	a0c5           	0120305
    624e:	2f03           	movel %d3,%sp@-
    6250:	4e4f           	trap #15
    6252:	a035           	0120065
    6254:	4227           	clrb %sp@-
    6256:	3f04           	movew %d4,%sp@-
    6258:	2f12           	movel %a2@,%sp@-
    625a:	4e4f           	trap #15
    625c:	a05e           	0120136
    625e:	7001           	moveq #1,%d0
    6260:	6000 0004      	braw 0x6266
    6264:	4240           	clrw %d0
    6266:	4cee 0418 fff4 	moveml %fp@(-12),%d3-%d4/%a2
    626c:	4e5e           	unlk %fp
    626e:	4e75           	rts
    6270:	4e56 fff8      	linkw %fp,#-8
    6274:	48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-
    6278:	43ec 0015      	lea %a4@(21),%a1
    627c:	2609           	movel %a1,%d3
    627e:	4a11           	tstb %a1@
    6280:	6700 017c      	beqw 0x63fe
    6284:	41ec 0018      	lea %a4@(24),%a0
    6288:	47ec 001c      	lea %a4@(28),%a3
    628c:	3813           	movew %a3@,%d4
    628e:	d950           	addw %d4,%a0@
    6290:	4a2c 0011      	tstb %a4@(17)
    6294:	6708           	beqs 0x629e
    6296:	382c 001e      	movew %a4@(30),%d4
    629a:	d96c 001a      	addw %d4,%a4@(26)
    629e:	3010           	movew %a0@,%d0
    62a0:	0c40 00a0      	cmpiw #160,%d0
    62a4:	6308           	blss 0x62ae
    62a6:	2243           	moveal %d3,%a1
    62a8:	4211           	clrb %a1@
    62aa:	6000 0152      	braw 0x63fe
    62ae:	0640 fff0      	addiw #-16,%d0
    62b2:	45ec 00d0      	lea %a4@(208),%a2
    62b6:	3200           	movew %d0,%d1
    62b8:	9252           	subw %a2@,%d1
    62ba:	6a02           	bpls 0x62be
    62bc:	4441           	negw %d1
    62be:	3013           	movew %a3@,%d0
    62c0:	6c02           	bges 0x62c4
    62c2:	4440           	negw %d0
    62c4:	5940           	subqw #4,%d0
    62c6:	b041           	cmpw %d1,%d0
    62c8:	6f00 00be      	blew 0x6388
    62cc:	102a 001f      	moveb %a2@(31),%d0
    62d0:	5700           	subqb #3,%d0
    62d2:	0c00 0002      	cmpib #2,%d0
    62d6:	6210           	bhis 0x62e8
    62d8:	302c 001a      	movew %a4@(26),%d0
    62dc:	906a 0002      	subw %a2@(2),%d0
    62e0:	0c40 0001      	cmpiw #1,%d0
    62e4:	6e00 00a2      	bgtw 0x6388
    62e8:	0c2a 0006 0005 	cmpib #6,%a2@(5)
    62ee:	6610           	bnes 0x6300
    62f0:	302c 001a      	movew %a4@(26),%d0
    62f4:	906a 0002      	subw %a2@(2),%d0
    62f8:	0c40 0006      	cmpiw #6,%d0
    62fc:	6f00 008a      	blew 0x6388
    6300:	42a7           	clrl %sp@-
    6302:	4e4f           	trap #15
    6304:	a0c2           	0120302
    6306:	41ec 0100      	lea %a4@(256),%a0
    630a:	4241           	clrw %d1
    630c:	1228 0118      	moveb %a0@(280),%d1
    6310:	48c0           	extl %d0
    6312:	81c1           	divsw %d1,%d0
    6314:	2200           	movel %d0,%d1
    6316:	4841           	swap %d1
    6318:	5241           	addqw #1,%d1
    631a:	2243           	moveal %d3,%a1
    631c:	4211           	clrb %a1@
    631e:	584f           	addqw #4,%sp
    6320:	4a53           	tstw %a3@
    6322:	6c06           	bges 0x632a
    6324:	5552           	subqw #2,%a2@
    6326:	6000 0004      	braw 0x632c
    632a:	5452           	addqw #2,%a2@
    632c:	41ec 00d0      	lea %a4@(208),%a0
    6330:	45e8 000c      	lea %a0@(12),%a2
    6334:	1412           	moveb %a2@,%d2
    6336:	4240           	clrw %d0
    6338:	1002           	moveb %d2,%d0
    633a:	b240           	cmpw %d0,%d1
    633c:	6d36           	blts 0x6374
    633e:	4212           	clrb %a2@
    6340:	117c 0006 0005 	moveb #6,%a0@(5)
    6346:	117c 0005 0021 	moveb #5,%a0@(33)
    634c:	197c 0023 000b 	moveb #35,%a4@(11)
    6352:	422c 000f      	clrb %a4@(15)
    6356:	43fa dc6a      	lea %pc@(0x3fc2),%a1
    635a:	2f09           	movel %a1,%sp@-
    635c:	486c 0098      	pea %a4@(152)
    6360:	4e4f           	trap #15
    6362:	a2de           	0121336
    6364:	3f3c 000c      	movew #12,%sp@-
    6368:	6100 009e      	bsrw 0x6408
    636c:	defc 000a      	addaw #10,%sp
    6370:	6000 0016      	braw 0x6388
    6374:	9401           	subb %d1,%d2
    6376:	1482           	moveb %d2,%a2@
    6378:	197c 0001 000f 	moveb #1,%a4@(15)
    637e:	3f3c 000b      	movew #11,%sp@-
    6382:	6100 0084      	bsrw 0x6408
    6386:	544f           	addqw #2,%sp
    6388:	4a6c 001c      	tstw %a4@(28)
    638c:	6c18           	bges 0x63a6
    638e:	41ec 0100      	lea %a4@(256),%a0
    6392:	1028 0122      	moveb %a0@(290),%d0
    6396:	e940           	aslw #4,%d0
    6398:	4840           	swap %d0
    639a:	4240           	clrw %d0
    639c:	0280 0ff0 0000 	andil #267386880,%d0
    63a2:	6000 001c      	braw 0x63c0
    63a6:	41ec 0100      	lea %a4@(256),%a0
    63aa:	d0fc 0122      	addaw #290,%a0
    63ae:	4240           	clrw %d0
    63b0:	1010           	moveb %a0@,%d0
    63b2:	4241           	clrw %d1
    63b4:	1228 fff0      	moveb %a0@(-16),%d1
    63b8:	d041           	addw %d1,%d0
    63ba:	e940           	aslw #4,%d0
    63bc:	4840           	swap %d0
    63be:	4240           	clrw %d0
    63c0:	806e fffa      	orw %fp@(-6),%d0
    63c4:	2d40 fff8      	movel %d0,%fp@(-8)
    63c8:	3d7c 0040 fffa 	movew #64,%fp@(-6)
    63ce:	3d7c 0010 fffe 	movew #16,%fp@(-2)
    63d4:	4280           	clrl %d0
    63d6:	302e fffe      	movew %fp@(-2),%d0
    63da:	08c0 0014      	bset #20,%d0
    63de:	2d40 fffc      	movel %d0,%fp@(-4)
    63e2:	1f3c 0004      	moveb #4,%sp@-
    63e6:	3f2c 001a      	movew %a4@(26),%sp@-
    63ea:	3f2c 0018      	movew %a4@(24),%sp@-
    63ee:	486e fff8      	pea %fp@(-8)
    63f2:	2f2c 0084      	movel %a4@(132),%sp@-
    63f6:	2f2c 0090      	movel %a4@(144),%sp@-
    63fa:	4e4f           	trap #15
    63fc:	a209           	0121011
    63fe:	4cee 0c18 ffe8 	moveml %fp@(-24),%d3-%d4/%a2-%a3
    6404:	4e5e           	unlk %fp
    6406:	4e75           	rts
    6408:	4e56 0000      	linkw %fp,#0
    640c:	322e 0008      	movew %fp@(8),%d1
    6410:	41ec 0022      	lea %a4@(34),%a0
    6414:	4240           	clrw %d0
    6416:	1010           	moveb %a0@,%d0
    6418:	b041           	cmpw %d1,%d0
    641a:	6c02           	bges 0x641e
    641c:	1081           	moveb %d1,%a0@
    641e:	4e5e           	unlk %fp
    6420:	4e75           	rts
    6422:	4e56 fff4      	linkw %fp,#-12
    6426:	48e7 1e00      	moveml %d3-%d6,%sp@-
    642a:	4244           	clrw %d4
    642c:	7a01           	moveq #1,%d5
    642e:	4246           	clrw %d6
    6430:	102c 0022      	moveb %a4@(34),%d0
    6434:	6700 01f4      	beqw 0x662a
    6438:	4a2c 0023      	tstb %a4@(35)
    643c:	6700 01ec      	beqw 0x662a
    6440:	0240 00ff      	andiw #255,%d0
    6444:	5340           	subqw #1,%d0
    6446:	0c40 000c      	cmpiw #12,%d0
    644a:	6200 0194      	bhiw 0x65e0
    644e:	0280 0000 ffff 	andil #65535,%d0
    6454:	41fa 000c      	lea %pc@(0x6462),%a0
    6458:	d080           	addl %d0,%d0
    645a:	3030 0800      	movew %a0@(00000000,%d0:l),%d0
    645e:	4efb 0002      	jmp %pc@(0x6462,%d0:w)
    6462:	006c 0084 00b8 	oriw #132,%a4@(184)
    6468:	001a 0050      	orib #80,%a2@+
    646c:	0034 00dc 0100 	orib #-36,%a4@(00000000,%d0:w)
    6472:	011e           	btst %d0,%fp@+
    6474:	0198           	bclr %d0,%a0@+
    6476:	013c 0160      	btst %d0,#96
    647a:	009e 1d7c 0001 	oril #494665729,%fp@+
    6480:	fff6           	0177766
    6482:	7205           	moveq #5,%d1
    6484:	2d41 fff8      	movel %d1,%fp@(-8)
    6488:	7801           	moveq #1,%d4
    648a:	7a19           	moveq #25,%d5
    648c:	3d7c 0014 fffc 	movew #20,%fp@(-4)
    6492:	6000 0160      	braw 0x65f4
    6496:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    649c:	725a           	moveq #90,%d1
    649e:	2d41 fff8      	movel %d1,%fp@(-8)
    64a2:	78ec           	moveq #-20,%d4
    64a4:	7a04           	moveq #4,%d5
    64a6:	3d7c 0005 fffc 	movew #5,%fp@(-4)
    64ac:	7cfa           	moveq #-6,%d6
    64ae:	6000 0144      	braw 0x65f4
    64b2:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    64b8:	2d7c 0000 012c 	movel #300,%fp@(-8)
    64be:	fff8
    64c0:	7816           	moveq #22,%d4
    64c2:	7a08           	moveq #8,%d5
    64c4:	3d7c 0008 fffc 	movew #8,%fp@(-4)
    64ca:	6000 00ea      	braw 0x65b6
    64ce:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    64d4:	7228           	moveq #40,%d1
    64d6:	2d41 fff8      	movel %d1,%fp@(-8)
    64da:	7801           	moveq #1,%d4
    64dc:	3d7c 000a fffc 	movew #10,%fp@(-4)
    64e2:	6000 0110      	braw 0x65f4
    64e6:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    64ec:	7214           	moveq #20,%d1
    64ee:	2d41 fff8      	movel %d1,%fp@(-8)
    64f2:	783c           	moveq #60,%d4
    64f4:	7a03           	moveq #3,%d5
    64f6:	3d7c 0019 fffc 	movew #25,%fp@(-4)
    64fc:	6000 00f6      	braw 0x65f4
    6500:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    6506:	7214           	moveq #20,%d1
    6508:	2d41 fff8      	movel %d1,%fp@(-8)
    650c:	7814           	moveq #20,%d4
    650e:	7a02           	moveq #2,%d5
    6510:	3d7c 001e fffc 	movew #30,%fp@(-4)
    6516:	6000 00dc      	braw 0x65f4
    651a:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    6520:	2d7c 0000 01c2 	movel #450,%fp@(-8)
    6526:	fff8
    6528:	782d           	moveq #45,%d4
    652a:	7a03           	moveq #3,%d5
    652c:	3d7c 0001 fffc 	movew #1,%fp@(-4)
    6532:	3d7c 0010 fffe 	movew #16,%fp@(-2)
    6538:	7cfd           	moveq #-3,%d6
    653a:	6000 00be      	braw 0x65fa
    653e:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    6544:	2d7c 0000 00c8 	movel #200,%fp@(-8)
    654a:	fff8
    654c:	78b0           	moveq #-80,%d4
    654e:	7a03           	moveq #3,%d5
    6550:	3d7c 000c fffc 	movew #12,%fp@(-4)
    6556:	7cfd           	moveq #-3,%d6
    6558:	3d7c 000c fffe 	movew #12,%fp@(-2)
    655e:	6000 009a      	braw 0x65fa
    6562:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    6568:	2d7c 0000 00dc 	movel #220,%fp@(-8)
    656e:	fff8
    6570:	383c 012c      	movew #300,%d4
    6574:	7a02           	moveq #2,%d5
    6576:	3d7c 00c8 fffc 	movew #200,%fp@(-4)
    657c:	6000 0076      	braw 0x65f4
    6580:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    6586:	2d7c 0000 014a 	movel #330,%fp@(-8)
    658c:	fff8
    658e:	383c 02bc      	movew #700,%d4
    6592:	7a02           	moveq #2,%d5
    6594:	3d7c 0078 fffc 	movew #120,%fp@(-4)
    659a:	6000 0058      	braw 0x65f4
    659e:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    65a4:	2d7c 0000 012c 	movel #300,%fp@(-8)
    65aa:	fff8
    65ac:	7816           	moveq #22,%d4
    65ae:	7a08           	moveq #8,%d5
    65b0:	3d7c 0004 fffc 	movew #4,%fp@(-4)
    65b6:	4246           	clrw %d6
    65b8:	3d7c 0003 fffe 	movew #3,%fp@(-2)
    65be:	6000 003a      	braw 0x65fa
    65c2:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    65c8:	2d7c 0000 01ae 	movel #430,%fp@(-8)
    65ce:	fff8
    65d0:	383c fe70      	movew #-400,%d4
    65d4:	7a02           	moveq #2,%d5
    65d6:	3d7c 01f4 fffc 	movew #500,%fp@(-4)
    65dc:	6000 0016      	braw 0x65f4
    65e0:	1d7c 0001 fff6 	moveb #1,%fp@(-10)
    65e6:	7214           	moveq #20,%d1
    65e8:	2d41 fff8      	movel %d1,%fp@(-8)
    65ec:	7801           	moveq #1,%d4
    65ee:	3d7c 0005 fffc 	movew #5,%fp@(-4)
    65f4:	3d7c 0040 fffe 	movew #64,%fp@(-2)
    65fa:	4243           	clrw %d3
    65fc:	ba43           	cmpw %d3,%d5
    65fe:	6f26           	bles 0x6626
    6600:	4227           	clrb %sp@-
    6602:	486e fff6      	pea %fp@(-10)
    6606:	42a7           	clrl %sp@-
    6608:	4e4f           	trap #15
    660a:	a233           	0121063
    660c:	206e fff8      	moveal %fp@(-8),%a0
    6610:	41f0 4000      	lea %a0@(00000000,%d4:w),%a0
    6614:	2d48 fff8      	movel %a0,%fp@(-8)
    6618:	dd6e fffe      	addw %d6,%fp@(-2)
    661c:	defc 000a      	addaw #10,%sp
    6620:	5243           	addqw #1,%d3
    6622:	ba43           	cmpw %d3,%d5
    6624:	6eda           	bgts 0x6600
    6626:	422c 0022      	clrb %a4@(34)
    662a:	4cee 0078 ffe4 	moveml %fp@(-28),%d3-%d6
    6630:	4e5e           	unlk %fp
    6632:	4e75           	rts
    6634:	4661           	notw %a1@-
    6636:	6c6c           	bges 0x66a4
    6638:	2074 6f20 4465 	moveal %a4@(00004465,%d6:l:8),%a0
    663e:	6174           	bsrs 0x66b4
    6640:	6800 5370      	bvcw 0xb9b2
    6644:	696b           	bvss 0x66b1
    6646:	6573           	bcss 0x66bb
    6648:	202d 2044      	movel %a5@(8260),%d0
    664c:	6561           	bcss 0x66af
    664e:	7468           	moveq #104,%d2
    6650:	0000 4e56      	orib #86,%d0
    6654:	0000 48e7      	orib #-25,%d0
    6658:	1820           	moveb %a0@-,%d4
    665a:	246e 0008      	moveal %fp@(8),%a2
    665e:	3812           	movew %a2@,%d4
    6660:	0644 0010      	addiw #16,%d4
    6664:	362a 0002      	movew %a2@(2),%d3
    6668:	0643 0018      	addiw #24,%d3
    666c:	0c43 00b4      	cmpiw #180,%d3
    6670:	6f3a           	bles 0x66ac
    6672:	422a 000c      	clrb %a2@(12)
    6676:	422a 001f      	clrb %a2@(31)
    667a:	41ec 00d0      	lea %a4@(208),%a0
    667e:	4a28 000c      	tstb %a0@(12)
    6682:	6628           	bnes 0x66ac
    6684:	197c 0001 000b 	moveb #1,%a4@(11)
    668a:	3f3c 000c      	movew #12,%sp@-
    668e:	6100 fd78      	bsrw 0x6408
    6692:	422c 000f      	clrb %a4@(15)
    6696:	43fa ff9c      	lea %pc@(0x6634),%a1
    669a:	2f09           	movel %a1,%sp@-
    669c:	486c 0098      	pea %a4@(152)
    66a0:	4e4f           	trap #15
    66a2:	a2de           	0121336
    66a4:	6100 016e      	bsrw 0x6814
    66a8:	defc 000a      	addaw #10,%sp
    66ac:	0c43 00a0      	cmpiw #160,%d3
    66b0:	6e00 0134      	bgtw 0x67e6
    66b4:	3004           	movew %d4,%d0
    66b6:	6c04           	bges 0x66bc
    66b8:	0640 000f      	addiw #15,%d0
    66bc:	3800           	movew %d0,%d4
    66be:	e844           	asrw #4,%d4
    66c0:	3003           	movew %d3,%d0
    66c2:	6c04           	bges 0x66c8
    66c4:	0640 000f      	addiw #15,%d0
    66c8:	3400           	movew %d0,%d2
    66ca:	e842           	asrw #4,%d2
    66cc:	0c44 0009      	cmpiw #9,%d4
    66d0:	6302           	blss 0x66d4
    66d2:	7809           	moveq #9,%d4
    66d4:	0c42 0009      	cmpiw #9,%d2
    66d8:	6302           	blss 0x66dc
    66da:	7409           	moveq #9,%d2
    66dc:	4281           	clrl %d1
    66de:	3204           	movew %d4,%d1
    66e0:	41ec 0100      	lea %a4@(256),%a0
    66e4:	2001           	movel %d1,%d0
    66e6:	e780           	asll #3,%d0
    66e8:	d081           	addl %d1,%d0
    66ea:	d081           	addl %d1,%d0
    66ec:	4281           	clrl %d1
    66ee:	3202           	movew %d2,%d1
    66f0:	41f0 081e      	lea %a0@(0000001e,%d0:l),%a0
    66f4:	1030 1800      	moveb %a0@(00000000,%d1:l),%d0
    66f8:	0c00 0015      	cmpib #21,%d0
    66fc:	6308           	blss 0x6706
    66fe:	0c00 0026      	cmpib #38,%d0
    6702:	6600 0092      	bnew 0x6796
    6706:	0240 00ff      	andiw #255,%d0
    670a:	0c40 0026      	cmpiw #38,%d0
    670e:	6200 0086      	bhiw 0x6796
    6712:	0280 0000 00ff 	andil #255,%d0
    6718:	41fa 000c      	lea %pc@(0x6726),%a0
    671c:	d080           	addl %d0,%d0
    671e:	3030 0800      	movew %a0@(00000000,%d0:l),%d0
    6722:	4efb 0002      	jmp %pc@(0x6726,%d0:w)
    6726:	004e 004e      	oriw #78,%fp
    672a:	0070 0070 004e 	oriw #112,%a0@(0000004e,%d0:w)
    6730:	004e 004e      	oriw #78,%fp
    6734:	004e 0070      	oriw #112,%fp
    6738:	0070 004e 004e 	oriw #78,%a0@(0000004e,%d0:w)
    673e:	004e 004e      	oriw #78,%fp
    6742:	0070 004e 0070 	oriw #78,%a0@(00000070,%d0:w)
    6748:	0070 0070 004e 	oriw #112,%a0@(0000004e,%d0:w)
    674e:	0070 0070 0070 	oriw #112,%a0@(00000070,%d0:w)
    6754:	0070 0070 0070 	oriw #112,%a0@(00000070,%d0:w)
    675a:	0070 0070 0070 	oriw #112,%a0@(00000070,%d0:w)
    6760:	0070 0070 0070 	oriw #112,%a0@(00000070,%d0:w)
    6766:	0070 0070 0070 	oriw #112,%a0@(00000070,%d0:w)
    676c:	004e 0070      	oriw #112,%fp
    6770:	0070 004e 157c 	oriw #78,%a0@(00060005)@(00000000)
    6776:	0006 0005
    677a:	157c 0002 0021 	moveb #2,%a2@(33)
    6780:	422a 001f      	clrb %a2@(31)
    6784:	157c 0003 000a 	moveb #3,%a2@(10)
    678a:	3002           	movew %d2,%d0
    678c:	e940           	aslw #4,%d0
    678e:	0640 ffe9      	addiw #-23,%d0
    6792:	3540 0002      	movew %d0,%a2@(2)
    6796:	4281           	clrl %d1
    6798:	3204           	movew %d4,%d1
    679a:	41ec 0100      	lea %a4@(256),%a0
    679e:	2001           	movel %d1,%d0
    67a0:	e780           	asll #3,%d0
    67a2:	d081           	addl %d1,%d0
    67a4:	d081           	addl %d1,%d0
    67a6:	4281           	clrl %d1
    67a8:	3202           	movew %d2,%d1
    67aa:	41f0 081e      	lea %a0@(0000001e,%d0:l),%a0
    67ae:	0c30 002a 1800 	cmpib #42,%a0@(00000000,%d1:l)
    67b4:	6630           	bnes 0x67e6
    67b6:	422a 000c      	clrb %a2@(12)
    67ba:	422a 001f      	clrb %a2@(31)
    67be:	41ec 00d0      	lea %a4@(208),%a0
    67c2:	4a28 000c      	tstb %a0@(12)
    67c6:	661e           	bnes 0x67e6
    67c8:	3f3c 000c      	movew #12,%sp@-
    67cc:	6100 fc3a      	bsrw 0x6408
    67d0:	422c 000f      	clrb %a4@(15)
    67d4:	43fa fe6c      	lea %pc@(0x6642),%a1
    67d8:	2f09           	movel %a1,%sp@-
    67da:	486c 0098      	pea %a4@(152)
    67de:	4e4f           	trap #15
    67e0:	a2de           	0121336
    67e2:	6100 0030      	bsrw 0x6814
    67e6:	4cee 0418 fff4 	moveml %fp@(-12),%d3-%d4/%a2
    67ec:	4e5e           	unlk %fp
    67ee:	4e75           	rts
    67f0:	416d           	040555
    67f2:	2049           	moveal %a1,%a0
    67f4:	2064           	moveal %a4@-,%a0
    67f6:	6561           	bcss 0x6859
    67f8:	643f           	bccs 0x6839
    67fa:	0049 7320      	oriw #29472,%a1
    67fe:	7468           	moveq #104,%d2
    6800:	6973           	bvss 0x6875
    6802:	2048           	moveal %a0,%a0
    6804:	656c           	bcss 0x6872
    6806:	3f2e 2e2e      	movew %fp@(11822),%sp@-
    680a:	0055 682d      	oriw #26669,%a5@
    680e:	6f68           	bles 0x6878
    6810:	2e2e 2e00      	movel %fp@(11776),%d7
    6814:	4e56 0000      	linkw %fp,#0
    6818:	2f03           	movel %d3,%sp@-
    681a:	6100 f9b4      	bsrw 0x61d0
    681e:	203c 0001 d4bf 	movel #119999,%d0
    6824:	51c8 fffe      	dbf %d0,0x6824
    6828:	4240           	clrw %d0
    682a:	5380           	subql #1,%d0
    682c:	64f6           	bccs 0x6824
    682e:	760a           	moveq #10,%d3
    6830:	3f03           	movew %d3,%sp@-
    6832:	3f3c 009f      	movew #159,%sp@-
    6836:	3f03           	movew %d3,%sp@-
    6838:	4267           	clrw %sp@-
    683a:	4e4f           	trap #15
    683c:	a213           	0121023
    683e:	504f           	addqw #8,%sp
    6840:	203c 0000 03e7 	movel #999,%d0
    6846:	51c8 fffe      	dbf %d0,0x6846
    684a:	4240           	clrw %d0
    684c:	5380           	subql #1,%d0
    684e:	64f6           	bccs 0x6846
    6850:	5243           	addqw #1,%d3
    6852:	0c43 009f      	cmpiw #159,%d3
    6856:	6fd8           	bles 0x6830
    6858:	4a2c 0010      	tstb %a4@(16)
    685c:	6620           	bnes 0x687e
    685e:	41ec 0100      	lea %a4@(256),%a0
    6862:	1028 00ff      	moveb %a0@(255),%d0
    6866:	670c           	beqs 0x6874
    6868:	41ec 00d0      	lea %a4@(208),%a0
    686c:	1140 0019      	moveb %d0,%a0@(25)
    6870:	6000 000c      	braw 0x687e
    6874:	41ec 00d0      	lea %a4@(208),%a0
    6878:	116c 0007 0019 	moveb %a4@(7),%a0@(25)
    687e:	41ec 00d0      	lea %a4@(208),%a0
    6882:	117c 0006 0005 	moveb #6,%a0@(5)
    6888:	117c 0001 000c 	moveb #1,%a0@(12)
    688e:	30bc 0040      	movew #64,%a0@
    6892:	317c 0079 0002 	movew #121,%a0@(2)
    6898:	117c 0002 0006 	moveb #2,%a0@(6)
    689e:	197c 0001 0010 	moveb #1,%a4@(16)
    68a4:	4240           	clrw %d0
    68a6:	1028 001d      	moveb %a0@(29),%d0
    68aa:	3f00           	movew %d0,%sp@-
    68ac:	6100 ae24      	bsrw 0x16d2
    68b0:	544f           	addqw #2,%sp
    68b2:	42a7           	clrl %sp@-
    68b4:	4e4f           	trap #15
    68b6:	a0c2           	0120302
    68b8:	7403           	moveq #3,%d2
    68ba:	48c0           	extl %d0
    68bc:	81c2           	divsw %d2,%d0
    68be:	2200           	movel %d0,%d1
    68c0:	4841           	swap %d1
    68c2:	584f           	addqw #4,%sp
    68c4:	0c41 0001      	cmpiw #1,%d1
    68c8:	671c           	beqs 0x68e6
    68ca:	6e08           	bgts 0x68d4
    68cc:	4a41           	tstw %d1
    68ce:	670e           	beqs 0x68de
    68d0:	6000 002e      	braw 0x6900
    68d4:	0c41 0002      	cmpiw #2,%d1
    68d8:	6714           	beqs 0x68ee
    68da:	6000 0024      	braw 0x6900
    68de:	43fa ff10      	lea %pc@(0x67f0),%a1
    68e2:	6000 000e      	braw 0x68f2
    68e6:	43fa ff13      	lea %pc@(0x67fb),%a1
    68ea:	6000 0006      	braw 0x68f2
    68ee:	43fa ff1b      	lea %pc@(0x680b),%a1
    68f2:	2f09           	movel %a1,%sp@-
    68f4:	3f3c 0001      	movew #1,%sp@-
    68f8:	3f3c 029c      	movew #668,%sp@-
    68fc:	6100 dd8e      	bsrw 0x468c
    6900:	262e fffc      	movel %fp@(-4),%d3
    6904:	4e5e           	unlk %fp
    6906:	4e75           	rts

###################################################################################################

    6908:	4e56 ffe0      	linkw %fp,#-32
    690c:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    6910:	246e 0008      	moveal %fp@(8),%a2
    6914:	266e 000c      	moveal %fp@(12),%a3
    6918:	2f0a           	movel %a2,%sp@-
    691a:	4e4f           	trap #15
    691c:	a0c7           	0120307
    691e:	584f           	addqw #4,%sp
    6920:	0c40 0009      	cmpiw #9,%d0
    6924:	622c           	bhis 0x6952
    6926:	2f0a           	movel %a2,%sp@-
    6928:	4e4f           	trap #15
    692a:	a0c7           	0120307
    692c:	3400           	movew %d0,%d2
    692e:	4241           	clrw %d1
    6930:	0c42 0009      	cmpiw #9,%d2
    6934:	6e1c           	bgts 0x6952
    6936:	1032 1000      	moveb %a2@(00000000,%d1:w),%d0
    693a:	4880           	extw %d0
    693c:	c1fc 0043      	mulsw #67,%d0
    6940:	0600 ffd4      	addib #-44,%d0
    6944:	1580 2000      	moveb %d0,%a2@(00000000,%d2:w)
    6948:	5242           	addqw #1,%d2
    694a:	5241           	addqw #1,%d1
    694c:	0c42 0009      	cmpiw #9,%d2
    6950:	6fe4           	bles 0x6936
    6952:	4242           	clrw %d2
    6954:	327c 0021      	moveaw #33,%a1
    6958:	3042           	moveaw %d2,%a0
    695a:	1030 a800      	moveb %a0@(00000000,%a2:l),%d0
    695e:	4880           	extw %d0
    6960:	d049           	addw %a1,%d0
    6962:	c1fc 0043      	mulsw #67,%d0
    6966:	1180 e8e2      	moveb %d0,%a0@(ffffffffffffffe2,%fp:l)
    696a:	1200           	moveb %d0,%d1
    696c:	6000 0024      	braw 0x6992
    6970:	1230 e8e2      	moveb %a0@(ffffffffffffffe2,%fp:l),%d1
    6974:	1001           	moveb %d1,%d0
    6976:	0600 ffd0      	addib #-48,%d0
    697a:	0c00 0009      	cmpib #9,%d0
    697e:	6328           	blss 0x69a8
    6980:	1001           	moveb %d1,%d0
    6982:	4880           	extw %d0
    6984:	c1fc 0043      	mulsw #67,%d0
    6988:	1200           	moveb %d0,%d1
    698a:	5601           	addqb #3,%d1
    698c:	1181 e8e2      	moveb %d1,%a0@(ffffffffffffffe2,%fp:l)
    6990:	1001           	moveb %d1,%d0
    6992:	0600 ffbf      	addib #-65,%d0
    6996:	0c00 0019      	cmpib #25,%d0
    699a:	630c           	blss 0x69a8
    699c:	1001           	moveb %d1,%d0
    699e:	0600 ff9f      	addib #-97,%d0
    69a2:	0c00 0019      	cmpib #25,%d0
    69a6:	62c8           	bhis 0x6970
    69a8:	1633 2000      	moveb %a3@(00000000,%d2:w),%d3
    69ac:	b636 20e2      	cmpb %fp@(ffffffffffffffe2,%d2:w),%d3
    69b0:	6706           	beqs 0x69b8
    69b2:	4240           	clrw %d0
    69b4:	6000 000c      	braw 0x69c2
    69b8:	5649           	addqw #3,%a1
    69ba:	5242           	addqw #1,%d2
    69bc:	0c42 0009      	cmpiw #9,%d2
    69c0:	6f96           	bles 0x6958
    69c2:	7001           	moveq #1,%d0
    69c4:	4cee 0c08 ffd4 	moveml %fp@(-44),%d3/%a2-%a3
    69ca:	4e5e           	unlk %fp
    69cc:	4e75           	rts

###################################################################################################
Show registration modal

    69ce:	4e56 ff90      	linkw %fp,#-112
    69d2:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    69d6:	3f3c 040e      	movew #1038,%sp@-
    69da:	4e4f           	trap #15
    69dc:	a16f           	0120557
    69de:	2608           	movel %a0,%d3
    69e0:	4878 001e      	pea 0x1e
    69e4:	4e4f           	trap #15
    69e6:	a01e           	0120036
    69e8:	2e08           	movel %a0,%d7
    69ea:	4878 001e      	pea 0x1e
    69ee:	4e4f           	trap #15
    69f0:	a01e           	0120036
    69f2:	2c08           	movel %a0,%d6
    69f4:	2f07           	movel %d7,%sp@-
    69f6:	4e4f           	trap #15
    69f8:	a021           	0120041
    69fa:	2808           	movel %a0,%d4
    69fc:	2f06           	movel %d6,%sp@-
    69fe:	4e4f           	trap #15
    6a00:	a021           	0120041
    6a02:	2a08           	movel %a0,%d5
    6a04:	2f04           	movel %d4,%sp@-
    6a06:	6100 0102      	bsrw 0x6b0a
    6a0a:	43fa a27c      	lea %pc@(0xc88),%a1
    6a0e:	2f09           	movel %a1,%sp@-
    6a10:	2f05           	movel %d5,%sp@-
    6a12:	4e4f           	trap #15
    6a14:	a0c5           	0120305
    6a16:	2f04           	movel %d4,%sp@-
    6a18:	4e4f           	trap #15
    6a1a:	a035           	0120065
    6a1c:	defc 001e      	addaw #30,%sp
    6a20:	2e85           	movel %d5,%sp@
    6a22:	4e4f           	trap #15
    6a24:	a035           	0120065
    6a26:	2f07           	movel %d7,%sp@-
    6a28:	3f3c 041a      	movew #1050,%sp@-
    6a2c:	2f03           	movel %d3,%sp@-
    6a2e:	4e4f           	trap #15
    6a30:	a180           	0120600
    6a32:	5c4f           	addqw #6,%sp
    6a34:	3f00           	movew %d0,%sp@-
    6a36:	2f03           	movel %d3,%sp@-
    6a38:	4e4f           	trap #15
    6a3a:	a183           	0120603
    6a3c:	544f           	addqw #2,%sp
    6a3e:	2e88           	movel %a0,%sp@
    6a40:	4e4f           	trap #15
    6a42:	a158           	0120530
    6a44:	2f06           	movel %d6,%sp@-
    6a46:	3f3c 041b      	movew #1051,%sp@-
    6a4a:	2f03           	movel %d3,%sp@-
    6a4c:	4e4f           	trap #15
    6a4e:	a180           	0120600
    6a50:	5c4f           	addqw #6,%sp
    6a52:	3f00           	movew %d0,%sp@-
    6a54:	2f03           	movel %d3,%sp@-
    6a56:	4e4f           	trap #15
    6a58:	a183           	0120603
    6a5a:	544f           	addqw #2,%sp
    6a5c:	2e88           	movel %a0,%sp@
    6a5e:	4e4f           	trap #15
    6a60:	a158           	0120530
    6a62:	2f03           	movel %d3,%sp@-
    6a64:	4e4f           	trap #15
    6a66:	a193           	0120623
    6a68:	42a7           	clrl %sp@-
    6a6a:	3f3c 041a      	movew #1050,%sp@-
    6a6e:	2f03           	movel %d3,%sp@-
    6a70:	4e4f           	trap #15
    6a72:	a180           	0120600
    6a74:	5c4f           	addqw #6,%sp
    6a76:	3f00           	movew %d0,%sp@-
    6a78:	2f03           	movel %d3,%sp@-
    6a7a:	4e4f           	trap #15
    6a7c:	a183           	0120603
    6a7e:	544f           	addqw #2,%sp
    6a80:	2e88           	movel %a0,%sp@
    6a82:	4e4f           	trap #15
    6a84:	a158           	0120530
    6a86:	defc 001c      	addaw #28,%sp
    6a8a:	4297           	clrl %sp@
    6a8c:	3f3c 041b      	movew #1051,%sp@-
    6a90:	2f03           	movel %d3,%sp@-
    6a92:	4e4f           	trap #15
    6a94:	a180           	0120600
    6a96:	5c4f           	addqw #6,%sp
    6a98:	3f00           	movew %d0,%sp@-
    6a9a:	2f03           	movel %d3,%sp@-
    6a9c:	4e4f           	trap #15
    6a9e:	a183           	0120603
    6aa0:	544f           	addqw #2,%sp
    6aa2:	2e88           	movel %a0,%sp@
    6aa4:	4e4f           	trap #15
    6aa6:	a158           	0120530
    6aa8:	2f04           	movel %d4,%sp@-
    6aaa:	78e2           	moveq #-30,%d4
    6aac:	d88e           	addl %fp,%d4
    6aae:	2f04           	movel %d4,%sp@-
    6ab0:	4e4f           	trap #15
    6ab2:	a0c5           	0120305
    6ab4:	2f05           	movel %d5,%sp@-
    6ab6:	47ee ffc4      	lea %fp@(-60),%a3
    6aba:	2f0b           	movel %a3,%sp@-
    6abc:	4e4f           	trap #15
    6abe:	a0c5           	0120305
    6ac0:	2f07           	movel %d7,%sp@-
    6ac2:	4e4f           	trap #15
    6ac4:	a02b           	0120053
    6ac6:	2f06           	movel %d6,%sp@-
    6ac8:	4e4f           	trap #15
    6aca:	a02b           	0120053
    6acc:	defc 001c      	addaw #28,%sp
    6ad0:	2e83           	movel %d3,%sp@
    6ad2:	4e4f           	trap #15
    6ad4:	a170           	0120560
    6ad6:	2f0b           	movel %a3,%sp@-
    6ad8:	2f04           	movel %d4,%sp@-
    6ada:	6100 fe2c      	bsrw 0x6908
    6ade:	defc 000c      	addaw #12,%sp
    6ae2:	4a00           	tstb %d0
    6ae4:	671a           	beqs 0x6b00
    6ae6:	197c 0001 000e 	moveb #1,%a4@(14)                                       -> Update registration
    6aec:	45ee ff92      	lea %fp@(-110),%a2
    6af0:	2f0a           	movel %a2,%sp@-
    6af2:	6100 0016      	bsrw 0x6b0a
    6af6:	2f0b           	movel %a3,%sp@-
    6af8:	2f04           	movel %d4,%sp@-
    6afa:	2f0a           	movel %a2,%sp@-
    6afc:	6100 0090      	bsrw 0x6b8e
    6b00:	4cee 0cf8 ff74 	moveml %fp@(-140),%d3-%d7/%a2-%a3
    6b06:	4e5e           	unlk %fp
    6b08:	4e75           	rts

###################################################################################################
Store registration?

    6b0a:	4e56 0000      	linkw %fp,#0
    6b0e:	48e7 1e00      	moveml %d3-%d6,%sp@-
    6b12:	2c2e 0008      	movel %fp@(8),%d6
    6b16:	4878 0029      	pea 0x29
    6b1a:	4e4f           	trap #15                           -> sysTrapMemHandleNew | alloc 0x29 bytes
    6b1c:	a01e           	0120036
    6b1e:	2a08           	movel %a0,%d5
    6b20:	584f           	addqw #4,%sp
    6b22:	6610           	bnes 0x6b34
    6b24:	43fa a162      	lea %pc@(0xc88),%a1
    6b28:	2f09           	movel %a1,%sp@-
    6b2a:	2f06           	movel %d6,%sp@-
    6b2c:	4e4f           	trap #15                           -> sysTrapStrCopy
    6b2e:	a0c5           	0120305
    6b30:	6000 0052      	braw 0x6b84
    6b34:	2f05           	movel %d5,%sp@-
    6b36:	4e4f           	trap #15                           -> sysTrapMemHandleLock
    6b38:	a021           	0120041
    6b3a:	2608           	movel %a0,%d3
    6b3c:	4227           	clrb %sp@-
    6b3e:	4878 0029      	pea 0x29
    6b42:	2f03           	movel %d3,%sp@-
    6b44:	4e4f           	trap #15                           -> sysTrapMemSet
    6b46:	a027           	0120047
    6b48:	42a7           	clrl %sp@-
    6b4a:	42a7           	clrl %sp@-
    6b4c:	2f03           	movel %d3,%sp@-
    6b4e:	42a7           	clrl %sp@-
    6b50:	42a7           	clrl %sp@-
    6b52:	42a7           	clrl %sp@-
    6b54:	4e4f           	trap #15                           -> sysTrapDlkGetSyncInfo | get username into handle
    6b56:	a2a9           	0121251
    6b58:	defc 0022      	addaw #34,%sp
    6b5c:	2e83           	movel %d3,%sp@
    6b5e:	4e4f           	trap #15                           -> sysTrapStrLen
    6b60:	a0c7           	0120307
    6b62:	5240           	addqw #1,%d0
    6b64:	4284           	clrl %d4
    6b66:	3800           	movew %d0,%d4
    6b68:	2f05           	movel %d5,%sp@-
    6b6a:	4e4f           	trap #15                           -> sysTrapMemHandleUnlock
    6b6c:	a022           	0120042
    6b6e:	2f04           	movel %d4,%sp@-
    6b70:	2f05           	movel %d5,%sp@-
    6b72:	4e4f           	trap #15                           -> sysTrapMemHandleResize
    6b74:	a033           	0120063
    6b76:	2f03           	movel %d3,%sp@-
    6b78:	2f06           	movel %d6,%sp@-
    6b7a:	4e4f           	trap #15                           -> sysTrapStrCopy
    6b7c:	a0c5           	0120305
    6b7e:	2f05           	movel %d5,%sp@-
    6b80:	4e4f           	trap #15                           -> sysTrapMemHandleFree
    6b82:	a02b           	0120053
    6b84:	4cee 0078 fff0 	moveml %fp@(-16),%d3-%d6
    6b8a:	4e5e           	unlk %fp
    6b8c:	4e75           	rts

###################################################################################################
Store registration ?

    6b8e:	4e56 ffb8      	linkw %fp,#-72
    6b92:	48e7 1820      	moveml %d3-%d4/%a2,%sp@-
    6b96:	262e 000c      	movel %fp@(12),%d3
    6b9a:	282e 0010      	movel %fp@(16),%d4
    6b9e:	2f2e 0008      	movel %fp@(8),%sp@-
    6ba2:	45ee ffba      	lea %fp@(-70),%a2
    6ba6:	2f0a           	movel %a2,%sp@-
    6ba8:	4e4f           	trap #15
    6baa:	a0c5           	0120305                            -> sysTrapStrCopy
    6bac:	3f3c 000a      	movew #10,%sp@-
    6bb0:	2f03           	movel %d3,%sp@-
    6bb2:	486e ffec      	pea %fp@(-20)
    6bb6:	4e4f           	trap #15                           -> sysTrapStrNCopy
    6bb8:	a2ce           	0121316
    6bba:	3f3c 000a      	movew #10,%sp@-
    6bbe:	2f04           	movel %d4,%sp@-
    6bc0:	486e fff6      	pea %fp@(-10)
    6bc4:	4e4f           	trap #15                           -> sysTrapStrNCopy
    6bc6:	a2ce           	0121316
    6bc8:	43ec 0051      	lea %a4@(81),%a1
    6bcc:	2609           	movel %a1,%d3
    6bce:	2f03           	movel %d3,%sp@-
    6bd0:	4267           	clrw %sp@-
    6bd2:	4e4f           	trap #15                           -> sysTrapDmFindDatabase
    6bd4:	a045           	0120105
    6bd6:	defc 0022      	addaw #34,%sp
    6bda:	4a80           	tstl %d0
    6bdc:	670a           	beqs 0x6be8
    6bde:	2f00           	movel %d0,%sp@-
    6be0:	4267           	clrw %sp@-
    6be2:	4e4f           	trap #15                           -> sysTrapDmDeleteDatabase
    6be4:	a042           	0120102
    6be6:	5c4f           	addqw #6,%sp
    6be8:	4227           	clrb %sp@-
    6bea:	2f3c 4461 7461 	movel #1147237473,%sp@-
    6bf0:	2f3c 5a61 6e65 	movel #1516334693,%sp@-
    6bf6:	2f03           	movel %d3,%sp@-
    6bf8:	4267           	clrw %sp@-
    6bfa:	4e4f           	trap #15                           -> sysTrapDmCreateDatabase
    6bfc:	a041           	0120101
    6bfe:	defc 0010      	addaw #16,%sp
    6c02:	4a40           	tstw %d0
    6c04:	6656           	bnes 0x6c5c
    6c06:	3f3c 0003      	movew #3,%sp@-
    6c0a:	2f03           	movel %d3,%sp@-
    6c0c:	4267           	clrw %sp@-
    6c0e:	4e4f           	trap #15                           -> sysTrapDmFindDatabase
    6c10:	a045           	0120105
    6c12:	544f           	addqw #2,%sp
    6c14:	2e80           	movel %d0,%sp@
    6c16:	4267           	clrw %sp@-
    6c18:	4e4f           	trap #15                           -> sysTrapDmOpenDatabase
    6c1a:	a049           	0120111
    6c1c:	2808           	movel %a0,%d4
    6c1e:	4878 0046      	pea 0x46
    6c22:	42a7           	clrl %sp@-
    6c24:	2f04           	movel %d4,%sp@-
    6c26:	4e4f           	trap #15                           -> sysTrapDmNewRecord
    6c28:	a055           	0120125
    6c2a:	2f08           	movel %a0,%sp@-
    6c2c:	4e4f           	trap #15                           -> sysTrapMemHandleLock
    6c2e:	a021           	0120041
    6c30:	2608           	movel %a0,%d3
    6c32:	4878 0046      	pea 0x46
    6c36:	2f0a           	movel %a2,%sp@-
    6c38:	42a7           	clrl %sp@-
    6c3a:	2f03           	movel %d3,%sp@-
    6c3c:	4e4f           	trap #15                           -> sysTrapDmWrite
    6c3e:	a076           	0120166
    6c40:	defc 0024      	addaw #36,%sp
    6c44:	2e83           	movel %d3,%sp@
    6c46:	4e4f           	trap #15                           -> sysTrapMemPtrUnlock
    6c48:	a035           	0120065
    6c4a:	1f3c 0001      	moveb #1,%sp@-
    6c4e:	4267           	clrw %sp@-
    6c50:	2f04           	movel %d4,%sp@-
    6c52:	4e4f           	trap #15                           -> sysTrapDmReleaseRecord
    6c54:	a05e           	0120136
    6c56:	2f04           	movel %d4,%sp@-
    6c58:	4e4f           	trap #15                           -> sysTrapDmCloseDatabases
    6c5a:	a04a           	0120112
    6c5c:	4cee 0418 ffac 	moveml %fp@(-84),%d3-%d4/%a2
    6c62:	4e5e           	unlk %fp
    6c64:	4e75           	rts

###################################################################################################

    6c66:	2573 2021 3d20 	movel %a3@(00000021,%d2:w),%a2@(15648)
    6c6c:	2573 0000 4e56 	movel %a3@(00000000,%d0:w),%a2@(20054)
    6c72:	ff90           	0177620
    6c74:	48e7 1030      	moveml %d3/%a2-%a3,%sp@-
    6c78:	45ec 0051      	lea %a4@(81),%a2
    6c7c:	2f0a           	movel %a2,%sp@-
    6c7e:	4267           	clrw %sp@-
    6c80:	4e4f           	trap #15
    6c82:	a045           	0120105
    6c84:	5c4f           	addqw #6,%sp
    6c86:	4a80           	tstl %d0
    6c88:	6700 00a4      	beqw 0x6d2e
    6c8c:	3f3c 0001      	movew #1,%sp@-
    6c90:	2f0a           	movel %a2,%sp@-
    6c92:	4267           	clrw %sp@-
    6c94:	4e4f           	trap #15
    6c96:	a045           	0120105
    6c98:	544f           	addqw #2,%sp
    6c9a:	2e80           	movel %d0,%sp@
    6c9c:	4267           	clrw %sp@-
    6c9e:	4e4f           	trap #15
    6ca0:	a049           	0120111
    6ca2:	2608           	movel %a0,%d3
    6ca4:	4267           	clrw %sp@-
    6ca6:	2f03           	movel %d3,%sp@-
    6ca8:	4e4f           	trap #15
    6caa:	a05c           	0120134
    6cac:	2448           	moveal %a0,%a2
    6cae:	defc 000e      	addaw #14,%sp
    6cb2:	b4fc 0000      	cmpaw #0,%a2
    6cb6:	660a           	bnes 0x6cc2
    6cb8:	3f3c 03e9      	movew #1001,%sp@-
    6cbc:	4e4f           	trap #15
    6cbe:	a192           	0120622
    6cc0:	544f           	addqw #2,%sp
    6cc2:	2f0a           	movel %a2,%sp@-
    6cc4:	4e4f           	trap #15
    6cc6:	a021           	0120041
    6cc8:	2648           	moveal %a0,%a3
    6cca:	45ee ffce      	lea %fp@(-50),%a2
    6cce:	2f0a           	movel %a2,%sp@-
    6cd0:	6100 fe38      	bsrw 0x6b0a
    6cd4:	2f0a           	movel %a2,%sp@-
    6cd6:	2f0b           	movel %a3,%sp@-
    6cd8:	4e4f           	trap #15
    6cda:	a0c8           	0120310
    6cdc:	defc 0010      	addaw #16,%sp
    6ce0:	4a40           	tstw %d0
    6ce2:	671a           	beqs 0x6cfe
    6ce4:	2f0a           	movel %a2,%sp@-
    6ce6:	2f0b           	movel %a3,%sp@-
    6ce8:	43fa ff7c      	lea %pc@(0x6c66),%a1
    6cec:	2f09           	movel %a1,%sp@-
    6cee:	486c 0098      	pea %a4@(152)
    6cf2:	4e4f           	trap #15
    6cf4:	a2de           	0121336
    6cf6:	422c 000e      	clrb %a4@(14)
    6cfa:	6000 001a      	braw 0x6d16
    6cfe:	486b 003c      	pea %a3@(60)
    6d02:	486b 0032      	pea %a3@(50)
    6d06:	6100 fc00      	bsrw 0x6908
    6d0a:	504f           	addqw #8,%sp
    6d0c:	4a00           	tstb %d0
    6d0e:	6706           	beqs 0x6d16
    6d10:	197c 0001 000e 	moveb #1,%a4@(14)
    6d16:	2f0b           	movel %a3,%sp@-
    6d18:	4e4f           	trap #15
    6d1a:	a035           	0120065
    6d1c:	1f3c 0001      	moveb #1,%sp@-
    6d20:	4267           	clrw %sp@-
    6d22:	2f03           	movel %d3,%sp@-
    6d24:	4e4f           	trap #15
    6d26:	a05e           	0120136
    6d28:	2f03           	movel %d3,%sp@-
    6d2a:	4e4f           	trap #15
    6d2c:	a04a           	0120112
    6d2e:	4cee 0c08 ff84 	moveml %fp@(-124),%d3/%a2-%a3
    6d34:	4e5e           	unlk %fp
    6d36:	4e75           	rts

###################################################################################################

    6d38:	4e56 ff00      	linkw %fp,#-256
    6d3c:	48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-
    6d40:	3e2e 0008      	movew %fp@(8),%d7
    6d44:	4267           	clrw %sp@-
    6d46:	47ec 00c0      	lea %a4@(192),%a3
    6d4a:	2f13           	movel %a3@,%sp@-
    6d4c:	4e4f           	trap #15
    6d4e:	a05c           	0120134
    6d50:	2f08           	movel %a0,%sp@-
    6d52:	4e4f           	trap #15
    6d54:	a021           	0120041
    6d56:	2608           	movel %a0,%d3
    6d58:	2f03           	movel %d3,%sp@-
    6d5a:	4e4f           	trap #15
    6d5c:	a0ce           	0120316
    6d5e:	3c00           	movew %d0,%d6
    6d60:	2f03           	movel %d3,%sp@-
    6d62:	4e4f           	trap #15
    6d64:	a035           	0120065
    6d66:	4227           	clrb %sp@-
    6d68:	4267           	clrw %sp@-
    6d6a:	2f13           	movel %a3@,%sp@-
    6d6c:	4e4f           	trap #15
    6d6e:	a05e           	0120136
    6d70:	3207           	movew %d7,%d1
    6d72:	c3fc 000c      	mulsw #12,%d1
    6d76:	43ec 0100      	lea %a4@(256),%a1
    6d7a:	2009           	movel %a1,%d0
    6d7c:	2441           	moveal %d1,%a2
    6d7e:	d5c0           	addal %d0,%a2
    6d80:	defc 001a      	addaw #26,%sp
    6d84:	4a2a 008b      	tstb %a2@(139)
    6d88:	6608           	bnes 0x6d92
    6d8a:	4a2a 008d      	tstb %a2@(141)
    6d8e:	6700 00de      	beqw 0x6e6e
    6d92:	2f2c 0084      	movel %a4@(132),%sp@-
    6d96:	4e4f           	trap #15
    6d98:	a1fd           	0120775
    6d9a:	2a08           	movel %a0,%d5
    6d9c:	584f           	addqw #4,%sp
    6d9e:	122a 008b      	moveb %a2@(139),%d1
    6da2:	673a           	beqs 0x6dde
    6da4:	0241 00ff      	andiw #255,%d1
    6da8:	3806           	movew %d6,%d4
    6daa:	d841           	addw %d1,%d4
    6dac:	3f04           	movew %d4,%sp@-
    6dae:	2f13           	movel %a3@,%sp@-
    6db0:	4e4f           	trap #15
    6db2:	a05c           	0120134
    6db4:	2608           	movel %a0,%d3
    6db6:	2f03           	movel %d3,%sp@-
    6db8:	4e4f           	trap #15
    6dba:	a021           	0120041
    6dbc:	4267           	clrw %sp@-
    6dbe:	4267           	clrw %sp@-
    6dc0:	2f08           	movel %a0,%sp@-
    6dc2:	4e4f           	trap #15
    6dc4:	a226           	0121046
    6dc6:	2f03           	movel %d3,%sp@-
    6dc8:	4e4f           	trap #15
    6dca:	a022           	0120042
    6dcc:	4227           	clrb %sp@-
    6dce:	3f04           	movew %d4,%sp@-
    6dd0:	2f13           	movel %a3@,%sp@-
    6dd2:	4e4f           	trap #15
    6dd4:	a05e           	0120136
    6dd6:	defc 001e      	addaw #30,%sp
    6dda:	6000 0006      	braw 0x6de2
    6dde:	6100 a1fc      	bsrw 0xfdc
    6de2:	3207           	movew %d7,%d1
    6de4:	c3fc 000c      	mulsw #12,%d1
    6de8:	43ec 0100      	lea %a4@(256),%a1
    6dec:	2009           	movel %a1,%d0
    6dee:	2801           	movel %d1,%d4
    6df0:	d880           	addl %d0,%d4
    6df2:	2244           	moveal %d4,%a1
    6df4:	1029 008d      	moveb %a1@(141),%d0
    6df8:	674c           	beqs 0x6e46
    6dfa:	4267           	clrw %sp@-
    6dfc:	1f40 0001      	moveb %d0,%sp@(1)
    6e00:	47ec 00c0      	lea %a4@(192),%a3
    6e04:	2f13           	movel %a3@,%sp@-
    6e06:	4e4f           	trap #15
    6e08:	a05c           	0120134
    6e0a:	2008           	movel %a0,%d0
    6e0c:	5c4f           	addqw #6,%sp
    6e0e:	6736           	beqs 0x6e46
    6e10:	2f00           	movel %d0,%sp@-
    6e12:	4e4f           	trap #15
    6e14:	a021           	0120041
    6e16:	2608           	movel %a0,%d3
    6e18:	2f03           	movel %d3,%sp@-
    6e1a:	45ee ff00      	lea %fp@(-256),%a2
    6e1e:	2f0a           	movel %a2,%sp@-
    6e20:	4e4f           	trap #15
    6e22:	a0c5           	0120305
    6e24:	2f03           	movel %d3,%sp@-
    6e26:	4e4f           	trap #15
    6e28:	a035           	0120065
    6e2a:	4227           	clrb %sp@-
    6e2c:	2244           	moveal %d4,%a1
    6e2e:	4241           	clrw %d1
    6e30:	1229 008d      	moveb %a1@(141),%d1
    6e34:	3f01           	movew %d1,%sp@-
    6e36:	2f13           	movel %a3@,%sp@-
    6e38:	4e4f           	trap #15
    6e3a:	a05e           	0120136
    6e3c:	2f0a           	movel %a2,%sp@-
    6e3e:	6100 01b8      	bsrw 0x6ff8
    6e42:	defc 001c      	addaw #28,%sp
    6e46:	2f05           	movel %d5,%sp@-
    6e48:	4e4f           	trap #15
    6e4a:	a1fd           	0120775
    6e4c:	6100 c680      	bsrw 0x34ce
    6e50:	584f           	addqw #4,%sp
    6e52:	7231           	moveq #49,%d1
    6e54:	203c 0001 869f 	movel #99999,%d0
    6e5a:	51c8 fffe      	dbf %d0,0x6e5a
    6e5e:	4240           	clrw %d0
    6e60:	5380           	subql #1,%d0
    6e62:	64f6           	bccs 0x6e5a
    6e64:	51c9 ffee      	dbf %d1,0x6e54
    6e68:	4241           	clrw %d1
    6e6a:	5381           	subql #1,%d1
    6e6c:	64e6           	bccs 0x6e54
    6e6e:	3207           	movew %d7,%d1
    6e70:	c3fc 000c      	mulsw #12,%d1
    6e74:	43ec 0100      	lea %a4@(256),%a1
    6e78:	2009           	movel %a1,%d0
    6e7a:	2441           	moveal %d1,%a2
    6e7c:	d5c0           	addal %d0,%a2
    6e7e:	4a2a 008c      	tstb %a2@(140)
    6e82:	6608           	bnes 0x6e8c
    6e84:	4a2a 008e      	tstb %a2@(142)
    6e88:	6700 00e2      	beqw 0x6f6c
    6e8c:	2f2c 0084      	movel %a4@(132),%sp@-
    6e90:	4e4f           	trap #15
    6e92:	a1fd           	0120775
    6e94:	2a08           	movel %a0,%d5
    6e96:	584f           	addqw #4,%sp
    6e98:	122a 008c      	moveb %a2@(140),%d1
    6e9c:	673e           	beqs 0x6edc
    6e9e:	0241 00ff      	andiw #255,%d1
    6ea2:	3806           	movew %d6,%d4
    6ea4:	d841           	addw %d1,%d4
    6ea6:	3f04           	movew %d4,%sp@-
    6ea8:	45ec 00c0      	lea %a4@(192),%a2
    6eac:	2f12           	movel %a2@,%sp@-
    6eae:	4e4f           	trap #15
    6eb0:	a05c           	0120134
    6eb2:	2608           	movel %a0,%d3
    6eb4:	2f03           	movel %d3,%sp@-
    6eb6:	4e4f           	trap #15
    6eb8:	a021           	0120041
    6eba:	4267           	clrw %sp@-
    6ebc:	4267           	clrw %sp@-
    6ebe:	2f08           	movel %a0,%sp@-
    6ec0:	4e4f           	trap #15
    6ec2:	a226           	0121046
    6ec4:	2f03           	movel %d3,%sp@-
    6ec6:	4e4f           	trap #15
    6ec8:	a022           	0120042
    6eca:	4227           	clrb %sp@-
    6ecc:	3f04           	movew %d4,%sp@-
    6ece:	2f12           	movel %a2@,%sp@-
    6ed0:	4e4f           	trap #15
    6ed2:	a05e           	0120136
    6ed4:	defc 001e      	addaw #30,%sp
    6ed8:	6000 0006      	braw 0x6ee0
    6edc:	6100 a0fe      	bsrw 0xfdc
    6ee0:	3207           	movew %d7,%d1
    6ee2:	c3fc 000c      	mulsw #12,%d1
    6ee6:	43ec 0100      	lea %a4@(256),%a1
    6eea:	2009           	movel %a1,%d0
    6eec:	2801           	movel %d1,%d4
    6eee:	d880           	addl %d0,%d4
    6ef0:	2244           	moveal %d4,%a1
    6ef2:	1029 008e      	moveb %a1@(142),%d0
    6ef6:	674c           	beqs 0x6f44
    6ef8:	4267           	clrw %sp@-
    6efa:	1f40 0001      	moveb %d0,%sp@(1)
    6efe:	47ec 00c0      	lea %a4@(192),%a3
    6f02:	2f13           	movel %a3@,%sp@-
    6f04:	4e4f           	trap #15
    6f06:	a05c           	0120134
    6f08:	2008           	movel %a0,%d0
    6f0a:	5c4f           	addqw #6,%sp
    6f0c:	6736           	beqs 0x6f44
    6f0e:	2f00           	movel %d0,%sp@-
    6f10:	4e4f           	trap #15
    6f12:	a021           	0120041
    6f14:	2608           	movel %a0,%d3
    6f16:	2f03           	movel %d3,%sp@-
    6f18:	45ee ff00      	lea %fp@(-256),%a2
    6f1c:	2f0a           	movel %a2,%sp@-
    6f1e:	4e4f           	trap #15
    6f20:	a0c5           	0120305
    6f22:	2f03           	movel %d3,%sp@-
    6f24:	4e4f           	trap #15
    6f26:	a035           	0120065
    6f28:	4227           	clrb %sp@-
    6f2a:	2244           	moveal %d4,%a1
    6f2c:	4241           	clrw %d1
    6f2e:	1229 008e      	moveb %a1@(142),%d1
    6f32:	3f01           	movew %d1,%sp@-
    6f34:	2f13           	movel %a3@,%sp@-
    6f36:	4e4f           	trap #15
    6f38:	a05e           	0120136
    6f3a:	2f0a           	movel %a2,%sp@-
    6f3c:	6100 00ba      	bsrw 0x6ff8
    6f40:	defc 001c      	addaw #28,%sp
    6f44:	2f05           	movel %d5,%sp@-
    6f46:	4e4f           	trap #15
    6f48:	a1fd           	0120775
    6f4a:	6100 c582      	bsrw 0x34ce
    6f4e:	584f           	addqw #4,%sp
    6f50:	7231           	moveq #49,%d1
    6f52:	203c 0001 869f 	movel #99999,%d0
    6f58:	51c8 fffe      	dbf %d0,0x6f58
    6f5c:	4240           	clrw %d0
    6f5e:	5380           	subql #1,%d0
    6f60:	64f6           	bccs 0x6f58
    6f62:	51c9 ffee      	dbf %d1,0x6f52
    6f66:	4241           	clrw %d1
    6f68:	5381           	subql #1,%d1
    6f6a:	64e6           	bccs 0x6f52
    6f6c:	3207           	movew %d7,%d1
    6f6e:	c3fc 000c      	mulsw #12,%d1
    6f72:	43ec 0100      	lea %a4@(256),%a1
    6f76:	2009           	movel %a1,%d0
    6f78:	2441           	moveal %d1,%a2
    6f7a:	d5c0           	addal %d0,%a2
    6f7c:	4a2a 008f      	tstb %a2@(143)
    6f80:	676c           	beqs 0x6fee
    6f82:	2f2c 0084      	movel %a4@(132),%sp@-
    6f86:	4e4f           	trap #15
    6f88:	a1fd           	0120775
    6f8a:	2a08           	movel %a0,%d5
    6f8c:	4241           	clrw %d1
    6f8e:	122a 008f      	moveb %a2@(143),%d1
    6f92:	3806           	movew %d6,%d4
    6f94:	d841           	addw %d1,%d4
    6f96:	3f04           	movew %d4,%sp@-
    6f98:	45ec 00c0      	lea %a4@(192),%a2
    6f9c:	2f12           	movel %a2@,%sp@-
    6f9e:	4e4f           	trap #15
    6fa0:	a05c           	0120134
    6fa2:	2608           	movel %a0,%d3
    6fa4:	2f03           	movel %d3,%sp@-
    6fa6:	4e4f           	trap #15
    6fa8:	a021           	0120041
    6faa:	4267           	clrw %sp@-
    6fac:	4267           	clrw %sp@-
    6fae:	2f08           	movel %a0,%sp@-
    6fb0:	4e4f           	trap #15
    6fb2:	a226           	0121046
    6fb4:	2f03           	movel %d3,%sp@-
    6fb6:	4e4f           	trap #15
    6fb8:	a022           	0120042
    6fba:	4227           	clrb %sp@-
    6fbc:	3f04           	movew %d4,%sp@-
    6fbe:	2f12           	movel %a2@,%sp@-
    6fc0:	4e4f           	trap #15
    6fc2:	a05e           	0120136
    6fc4:	defc 001e      	addaw #30,%sp
    6fc8:	2e85           	movel %d5,%sp@
    6fca:	4e4f           	trap #15
    6fcc:	a1fd           	0120775
    6fce:	6100 c4fe      	bsrw 0x34ce
    6fd2:	721d           	moveq #29,%d1
    6fd4:	203c 0001 869f 	movel #99999,%d0
    6fda:	51c8 fffe      	dbf %d0,0x6fda
    6fde:	4240           	clrw %d0
    6fe0:	5380           	subql #1,%d0
    6fe2:	64f6           	bccs 0x6fda
    6fe4:	51c9 ffee      	dbf %d1,0x6fd4
    6fe8:	4241           	clrw %d1
    6fea:	5381           	subql #1,%d1
    6fec:	64e6           	bccs 0x6fd4
    6fee:	4cee 0cf8 fee4 	moveml %fp@(-284),%d3-%d7/%a2-%a3
    6ff4:	4e5e           	unlk %fp
    6ff6:	4e75           	rts
    6ff8:	4e56 0000      	linkw %fp,#0
    6ffc:	48e7 1f20      	moveml %d3-%d7/%a2,%sp@-
    7000:	2e2e 0008      	movel %fp@(8),%d7
    7004:	2f07           	movel %d7,%sp@-
    7006:	4e4f           	trap #15
    7008:	a0c7           	0120307
    700a:	3c00           	movew %d0,%d6
    700c:	2447           	moveal %d7,%a2
    700e:	4227           	clrb %sp@-
    7010:	4e4f           	trap #15
    7012:	a164           	0120544
    7014:	4244           	clrw %d4
    7016:	4245           	clrw %d5
    7018:	5c4f           	addqw #6,%sp
    701a:	bc45           	cmpw %d5,%d6
    701c:	6f24           	bles 0x7042
    701e:	3f3c 0078      	movew #120,%sp@-
    7022:	2f0a           	movel %a2,%sp@-
    7024:	4e4f           	trap #15
    7026:	a2d9           	0121331
    7028:	3600           	movew %d0,%d3
    702a:	3f03           	movew %d3,%sp@-
    702c:	2f0a           	movel %a2,%sp@-
    702e:	4e4f           	trap #15
    7030:	a16b           	0120553
    7032:	d843           	addw %d3,%d4
    7034:	5245           	addqw #1,%d5
    7036:	45f2 3000      	lea %a2@(00000000,%d3:w),%a2
    703a:	defc 000c      	addaw #12,%sp
    703e:	bc44           	cmpw %d4,%d6
    7040:	6edc           	bgts 0x701e
    7042:	4a45           	tstw %d5
    7044:	674a           	beqs 0x7090
    7046:	7c0a           	moveq #10,%d6
    7048:	2447           	moveal %d7,%a2
    704a:	4244           	clrw %d4
    704c:	ba44           	cmpw %d4,%d5
    704e:	6f40           	bles 0x7090
    7050:	3f3c 0078      	movew #120,%sp@-
    7054:	2f0a           	movel %a2,%sp@-
    7056:	4e4f           	trap #15
    7058:	a2d9           	0121331
    705a:	3600           	movew %d0,%d3
    705c:	3f03           	movew %d3,%sp@-
    705e:	2f0a           	movel %a2,%sp@-
    7060:	4e4f           	trap #15
    7062:	a16b           	0120553
    7064:	3200           	movew %d0,%d1
    7066:	740f           	moveq #15,%d2
    7068:	e469           	lsrw %d2,%d1
    706a:	d240           	addw %d0,%d1
    706c:	e241           	asrw #1,%d1
    706e:	3f06           	movew %d6,%sp@-
    7070:	7450           	moveq #80,%d2
    7072:	9441           	subw %d1,%d2
    7074:	3f02           	movew %d2,%sp@-
    7076:	3f03           	movew %d3,%sp@-
    7078:	2f0a           	movel %a2,%sp@-
    707a:	4e4f           	trap #15
    707c:	a22a           	0121052
    707e:	45f2 3000      	lea %a2@(00000000,%d3:w),%a2
    7082:	0646 000b      	addiw #11,%d6
    7086:	defc 0016      	addaw #22,%sp
    708a:	5244           	addqw #1,%d4
    708c:	ba44           	cmpw %d4,%d5
    708e:	6ec0           	bgts 0x7050
    7090:	4cee 04f8 ffe8 	moveml %fp@(-24),%d3-%d7/%a2
    7096:	4e5e           	unlk %fp
    7098:	4e75           	rts
    709a:	4e56 0000      	linkw %fp,#0
    709e:	7200           	moveq #0,%d1
    70a0:	7000           	moveq #0,%d0
    70a2:	5280           	addql #1,%d0
    70a4:	0c80 0001 869f 	cmpil #99999,%d0
    70aa:	63f6           	blss 0x70a2
    70ac:	5281           	addql #1,%d1
    70ae:	740b           	moveq #11,%d2
    70b0:	b481           	cmpl %d1,%d2
    70b2:	64ec           	bccs 0x70a0
    70b4:	4e5e           	unlk %fp
    70b6:	4e75           	rts
    70b8:	5669 7369      	addqw #3,%a1@(29545)
    70bc:	7400           	moveq #0,%d2
    70be:	7777           	073567
    70c0:	772e           	073456
    70c2:	646e           	bccs 0x7132
    70c4:	6169           	bsrs 0x712f
    70c6:	2e63           	moveal %a3@-,%sp
    70c8:	6f6d           	bles 0x7137
    70ca:	2f7e           	027576
    70cc:	7a61           	moveq #97,%d5
    70ce:	6e65           	bgts 0x7135
    70d0:	2f73 6861 646f 	movel %a3@(00000061,%d6:l),%sp@(25711)
    70d6:	7774           	073564
    70d8:	6869           	bvcs 0x7143
    70da:	6566           	bcss 0x7142
    70dc:	0066 6f72      	oriw #28530,%fp@-
    70e0:	206d 6f72      	moveal %a5@(28530),%a0
    70e4:	6520           	bcss 0x7106
    70e6:	6578           	bcss 0x7160
    70e8:	6369           	blss 0x7153
    70ea:	7469           	moveq #105,%d2
    70ec:	6e67           	bgts 0x7155
    70ee:	2061           	moveal %a1@-,%a0
    70f0:	6476           	bccs 0x7168
    70f2:	656e           	bcss 0x7162
    70f4:	7475           	moveq #117,%d2
    70f6:	7265           	moveq #101,%d1
    70f8:	7321           	071441
    70fa:	0028 6329 3938 	orib #41,%a0@(14648)
    7100:	2062           	moveal %a2@-,%a0
    7102:	7920           	074440
    7104:	727a           	moveq #122,%d1
    7106:	722e           	moveq #46,%d1
    7108:	2061           	moveal %a1@-,%a0
    710a:	6c6c           	bges 0x7178
    710c:	2072 6967 6874 	moveal %a2@(00006874)@(73207265),%a0
    7112:	7320 7265
    7116:	7365           	071545
    7118:	7276           	moveq #118,%d1
    711a:	6564           	bcss 0x7180
    711c:	2e00           	movel %d0,%d7
    711e:	4e56 0000      	linkw %fp,#0
    7122:	6100 9eb8      	bsrw 0xfdc
    7126:	4267           	clrw %sp@-
    7128:	3f3c 0032      	movew #50,%sp@-
    712c:	41fa ff8a      	lea %pc@(0x70b8),%a0
    7130:	2f08           	movel %a0,%sp@-
    7132:	6100 d4e8      	bsrw 0x461c
    7136:	4267           	clrw %sp@-
    7138:	3f3c 0041      	movew #65,%sp@-
    713c:	41fa ff80      	lea %pc@(0x70be),%a0
    7140:	2f08           	movel %a0,%sp@-
    7142:	6100 d4d8      	bsrw 0x461c
    7146:	4267           	clrw %sp@-
    7148:	3f3c 0050      	movew #80,%sp@-
    714c:	41fa ff8f      	lea %pc@(0x70dd),%a0
    7150:	2f08           	movel %a0,%sp@-
    7152:	6100 d4c8      	bsrw 0x461c
    7156:	4267           	clrw %sp@-
    7158:	3f3c 008b      	movew #139,%sp@-
    715c:	41fa ff9d      	lea %pc@(0x70fb),%a0
    7160:	2f08           	movel %a0,%sp@-
    7162:	6100 d4b8      	bsrw 0x461c
    7166:	7200           	moveq #0,%d1
    7168:	7000           	moveq #0,%d0
    716a:	5280           	addql #1,%d0
    716c:	0c80 0001 869f 	cmpil #99999,%d0
    7172:	63f6           	blss 0x716a
    7174:	5281           	addql #1,%d1
    7176:	740b           	moveq #11,%d2
    7178:	b481           	cmpl %d1,%d2
    717a:	64ec           	bccs 0x7168
    717c:	4e5e           	unlk %fp
    717e:	4e75           	rts
    7180:	4e56 0000      	linkw %fp,#0
    7184:	2f04           	movel %d4,%sp@-
    7186:	2f03           	movel %d3,%sp@-
    7188:	262e 0008      	movel %fp@(8),%d3
    718c:	91c8           	subal %a0,%a0
    718e:	203c 0000 041d 	movel #1053,%d0
    7194:	4a83           	tstl %d3
    7196:	6608           	bnes 0x71a0
    7198:	7400           	moveq #0,%d2
    719a:	7200           	moveq #0,%d1
    719c:	6000 005a      	braw 0x71f8
    71a0:	4a83           	tstl %d3
    71a2:	6c1a           	bges 0x71be
    71a4:	207c 8000 0000 	moveal #-2147483648,%a0
    71aa:	4483           	negl %d3
    71ac:	6a10           	bpls 0x71be
    71ae:	223c c1e0 0000 	movel #-1042284544,%d1
    71b4:	7400           	moveq #0,%d2
    71b6:	6000 0040      	braw 0x71f8
    71ba:	e983           	asll #4,%d3
    71bc:	5980           	subql #4,%d0
    71be:	0c83 00ff ffff 	cmpil #16777215,%d3
    71c4:	6ff4           	bles 0x71ba
    71c6:	0c83 3fff ffff 	cmpil #1073741823,%d3
    71cc:	6e0c           	bgts 0x71da
    71ce:	d683           	addl %d3,%d3
    71d0:	5380           	subql #1,%d0
    71d2:	0c83 3fff ffff 	cmpil #1073741823,%d3
    71d8:	6ff4           	bles 0x71ce
    71da:	2208           	movel %a0,%d1
    71dc:	e940           	aslw #4,%d0
    71de:	4840           	swap %d0
    71e0:	4240           	clrw %d0
    71e2:	8280           	orl %d0,%d1
    71e4:	2003           	movel %d3,%d0
    71e6:	780a           	moveq #10,%d4
    71e8:	e8a0           	asrl %d4,%d0
    71ea:	0880 0014      	bclr #20,%d0
    71ee:	8280           	orl %d0,%d1
    71f0:	2403           	movel %d3,%d2
    71f2:	ed42           	aslw #6,%d2
    71f4:	4842           	swap %d2
    71f6:	4242           	clrw %d2
    71f8:	2001           	movel %d1,%d0
    71fa:	2202           	movel %d2,%d1
    71fc:	262e fff8      	movel %fp@(-8),%d3
    7200:	282e fffc      	movel %fp@(-4),%d4
    7204:	4e5e           	unlk %fp
    7206:	4e75           	rts
    7208:	4e56 0000      	linkw %fp,#0
    720c:	2f2e 0008      	movel %fp@(8),%sp@-
    7210:	6100 ff6e      	bsrw 0x7180
    7214:	2f01           	movel %d1,%sp@-
    7216:	2f00           	movel %d0,%sp@-
    7218:	6100 0062      	bsrw 0x727c
    721c:	4e5e           	unlk %fp
    721e:	4e75           	rts
    7220:	4e56 0000      	linkw %fp,#0
    7224:	2f04           	movel %d4,%sp@-
    7226:	2f03           	movel %d3,%sp@-
    7228:	222e 0008      	movel %fp@(8),%d1
    722c:	673a           	beqs 0x7268
    722e:	2401           	movel %d1,%d2
    7230:	0282 8000 0000 	andil #-2147483648,%d2
    7236:	2001           	movel %d1,%d0
    7238:	4840           	swap %d0
    723a:	ee40           	asrw #7,%d0
    723c:	48c0           	extl %d0
    723e:	0280 0000 00ff 	andil #255,%d0
    7244:	0680 0000 0380 	addil #896,%d0
    724a:	e940           	aslw #4,%d0
    724c:	4840           	swap %d0
    724e:	4240           	clrw %d0
    7250:	8480           	orl %d0,%d2
    7252:	0281 007f ffff 	andil #8388607,%d1
    7258:	2001           	movel %d1,%d0
    725a:	e688           	lsrl #3,%d0
    725c:	8480           	orl %d0,%d2
    725e:	2601           	movel %d1,%d3
    7260:	781d           	moveq #29,%d4
    7262:	e9a3           	asll %d4,%d3
    7264:	6000 0006      	braw 0x726c
    7268:	7600           	moveq #0,%d3
    726a:	7400           	moveq #0,%d2
    726c:	2002           	movel %d2,%d0
    726e:	2203           	movel %d3,%d1
    7270:	262e fff8      	movel %fp@(-8),%d3
    7274:	282e fffc      	movel %fp@(-4),%d4
    7278:	4e5e           	unlk %fp
    727a:	4e75           	rts
    727c:	4e56 0000      	linkw %fp,#0
    7280:	48e7 1e00      	moveml %d3-%d6,%sp@-
    7284:	262e 0008      	movel %fp@(8),%d3
    7288:	282e 000c      	movel %fp@(12),%d4
    728c:	4a83           	tstl %d3
    728e:	660a           	bnes 0x729a
    7290:	4a84           	tstl %d4
    7292:	6606           	bnes 0x729a
    7294:	7000           	moveq #0,%d0
    7296:	6000 005c      	braw 0x72f4
    729a:	2003           	movel %d3,%d0
    729c:	4840           	swap %d0
    729e:	e840           	asrw #4,%d0
    72a0:	48c0           	extl %d0
    72a2:	0280 0000 07ff 	andil #2047,%d0
    72a8:	2a00           	movel %d0,%d5
    72aa:	0685 ffff fc80 	addil #-896,%d5
    72b0:	2003           	movel %d3,%d0
    72b2:	0280 000f ffff 	andil #1048575,%d0
    72b8:	08c0 0014      	bset #20,%d0
    72bc:	7c0a           	moveq #10,%d6
    72be:	eda0           	asll %d6,%d0
    72c0:	2204           	movel %d4,%d1
    72c2:	4241           	clrw %d1
    72c4:	4841           	swap %d1
    72c6:	ec49           	lsrw #6,%d1
    72c8:	8081           	orl %d1,%d0
    72ca:	2400           	movel %d0,%d2
    72cc:	ec8a           	lsrl #6,%d2
    72ce:	5282           	addql #1,%d2
    72d0:	e282           	asrl #1,%d2
    72d2:	0802 0018      	btst #24,%d2
    72d6:	6704           	beqs 0x72dc
    72d8:	e282           	asrl #1,%d2
    72da:	5285           	addql #1,%d5
    72dc:	0882 0017      	bclr #23,%d2
    72e0:	2003           	movel %d3,%d0
    72e2:	0280 8000 0000 	andil #-2147483648,%d0
    72e8:	2205           	movel %d5,%d1
    72ea:	ef41           	aslw #7,%d1
    72ec:	4841           	swap %d1
    72ee:	4241           	clrw %d1
    72f0:	8081           	orl %d1,%d0
    72f2:	8082           	orl %d2,%d0
    72f4:	4cee 0078 fff0 	moveml %fp@(-16),%d3-%d6
    72fa:	4e5e           	unlk %fp
    72fc:	4e75           	rts
    72fe:	4e56 0000      	linkw %fp,#0
    7302:	48e7 1c00      	moveml %d3-%d5,%sp@-
    7306:	242e 0008      	movel %fp@(8),%d2
    730a:	262e 000c      	movel %fp@(12),%d3
    730e:	4a82           	tstl %d2
    7310:	6604           	bnes 0x7316
    7312:	4a83           	tstl %d3
    7314:	674e           	beqs 0x7364
    7316:	2002           	movel %d2,%d0
    7318:	4840           	swap %d0
    731a:	e840           	asrw #4,%d0
    731c:	48c0           	extl %d0
    731e:	0280 0000 07ff 	andil #2047,%d0
    7324:	2800           	movel %d0,%d4
    7326:	0684 ffff fbe3 	addil #-1053,%d4
    732c:	2002           	movel %d2,%d0
    732e:	0280 000f ffff 	andil #1048575,%d0
    7334:	08c0 0014      	bset #20,%d0
    7338:	7a0a           	moveq #10,%d5
    733a:	eba0           	asll %d5,%d0
    733c:	2203           	movel %d3,%d1
    733e:	4241           	clrw %d1
    7340:	4841           	swap %d1
    7342:	ec49           	lsrw #6,%d1
    7344:	8280           	orl %d0,%d1
    7346:	4a84           	tstl %d4
    7348:	6f14           	bles 0x735e
    734a:	203c 7fff ffff 	movel #2147483647,%d0
    7350:	4a82           	tstl %d2
    7352:	6c2a           	bges 0x737e
    7354:	203c 8000 0000 	movel #-2147483648,%d0
    735a:	6000 0022      	braw 0x737e
    735e:	7ae0           	moveq #-32,%d5
    7360:	ba84           	cmpl %d4,%d5
    7362:	6d06           	blts 0x736a
    7364:	7000           	moveq #0,%d0
    7366:	6000 0016      	braw 0x737e
    736a:	4a84           	tstl %d4
    736c:	6c08           	bges 0x7376
    736e:	3004           	movew %d4,%d0
    7370:	4440           	negw %d0
    7372:	48c0           	extl %d0
    7374:	e0a1           	asrl %d0,%d1
    7376:	2001           	movel %d1,%d0
    7378:	4a82           	tstl %d2
    737a:	6c02           	bges 0x737e
    737c:	4480           	negl %d0
    737e:	4cee 0038 fff4 	moveml %fp@(-12),%d3-%d5
    7384:	4e5e           	unlk %fp
    7386:	4e75           	rts
    7388:	4e56 0000      	linkw %fp,#0
    738c:	2f2e 0008      	movel %fp@(8),%sp@-
    7390:	6100 fe8e      	bsrw 0x7220
    7394:	584f           	addqw #4,%sp
    7396:	2f01           	movel %d1,%sp@-
    7398:	2f00           	movel %d0,%sp@-
    739a:	6100 ff62      	bsrw 0x72fe
    739e:	4e5e           	unlk %fp
    73a0:	4e75           	rts
    73a2:	0000 8087      	orib #-121,%d0
    73a6:	3e3c 0002      	movew #2,%d7
    73aa:	0047 0001      	oriw #1,%d7
    73ae:	3c3c 0002      	movew #2,%d6
    73b2:	4efa 0a0e      	jmp %pc@(0x7dc2)
    73b6:	203c 7ff0 0000 	movel #2146435072,%d0
    73bc:	7200           	moveq #0,%d1
    73be:	8087           	orl %d7,%d0
    73c0:	3e3c 0004      	movew #4,%d7
    73c4:	0047 0001      	oriw #1,%d7
    73c8:	3c3c 0002      	movew #2,%d6
    73cc:	4efa 09f4      	jmp %pc@(0x7dc2)
    73d0:	7000           	moveq #0,%d0
    73d2:	2200           	movel %d0,%d1
    73d4:	3e3c 0002      	movew #2,%d7
    73d8:	0047 0001      	oriw #1,%d7
    73dc:	3c3c 0002      	movew #2,%d6
    73e0:	4efa 09e0      	jmp %pc@(0x7dc2)
    73e4:	70ff           	moveq #-1,%d0
    73e6:	2200           	movel %d0,%d1
    73e8:	3e3c 0010      	movew #16,%d7
    73ec:	0047 0001      	oriw #1,%d7
    73f0:	3c3c 0002      	movew #2,%d6
    73f4:	4efa 09cc      	jmp %pc@(0x7dc2)
    73f8:	203c 7ff0 0000 	movel #2146435072,%d0
    73fe:	7200           	moveq #0,%d1
    7400:	8087           	orl %d7,%d0
    7402:	3e3c 0008      	movew #8,%d7
    7406:	0047 0001      	oriw #1,%d7
    740a:	3c3c 0002      	movew #2,%d6
    740e:	4efa 09b2      	jmp %pc@(0x7dc2)
    7412:	086f 001f 000c 	bchg #31,%sp@(12)
    7418:	4e56 0000      	linkw %fp,#0
    741c:	48e7 3f00      	moveml %d2-%d7,%sp@-
    7420:	202e 0008      	movel %fp@(8),%d0
    7424:	222e 000c      	movel %fp@(12),%d1
    7428:	242e 0010      	movel %fp@(16),%d2
    742c:	262e 0014      	movel %fp@(20),%d3
    7430:	2e00           	movel %d0,%d7
    7432:	d281           	addl %d1,%d1
    7434:	d180           	addxl %d0,%d0
    7436:	6700 02a8      	beqw 0x76e0
    743a:	2c02           	movel %d2,%d6
    743c:	d683           	addl %d3,%d3
    743e:	d582           	addxl %d2,%d2
    7440:	6700 02a6      	beqw 0x76e8
    7444:	0287 8000 0000 	andil #-2147483648,%d7
    744a:	4846           	swap %d6
    744c:	0246 8000      	andiw #-32768,%d6
    7450:	8e46           	orw %d6,%d7
    7452:	2047           	moveal %d7,%a0
    7454:	2c3c 001f ffff 	movel #2097151,%d6
    745a:	2e3c 0020 0000 	movel #2097152,%d7
    7460:	2800           	movel %d0,%d4
    7462:	c086           	andl %d6,%d0
    7464:	4686           	notl %d6
    7466:	c886           	andl %d6,%d4
    7468:	6700 0268      	beqw 0x76d2
    746c:	b886           	cmpl %d6,%d4
    746e:	6700 02e0      	beqw 0x7750
    7472:	8087           	orl %d7,%d0
    7474:	4844           	swap %d4
    7476:	ea4c           	lsrw #5,%d4
    7478:	2a02           	movel %d2,%d5
    747a:	ca86           	andl %d6,%d5
    747c:	6700 025a      	beqw 0x76d8
    7480:	ba86           	cmpl %d6,%d5
    7482:	6700 02cc      	beqw 0x7750
    7486:	4686           	notl %d6
    7488:	c486           	andl %d6,%d2
    748a:	8487           	orl %d7,%d2
    748c:	4845           	swap %d5
    748e:	ea4d           	lsrw #5,%d5
    7490:	48e7 0030      	moveml %a2-%a3,%sp@-
    7494:	2444           	moveal %d4,%a2
    7496:	2645           	moveal %d5,%a3
    7498:	7e00           	moveq #0,%d7
    749a:	2c07           	movel %d7,%d6
    749c:	2a03           	movel %d3,%d5
    749e:	2802           	movel %d2,%d4
    74a0:	2607           	movel %d7,%d3
    74a2:	2407           	movel %d7,%d2
    74a4:	c98a           	exg %d4,%a2
    74a6:	cb8b           	exg %d5,%a3
    74a8:	ba44           	cmpw %d4,%d5
    74aa:	6700 00cc      	beqw 0x7578
    74ae:	6200 0064      	bhiw 0x7514
    74b2:	3404           	movew %d4,%d2
    74b4:	9445           	subw %d5,%d2
    74b6:	c98a           	exg %d4,%a2
    74b8:	cb8b           	exg %d5,%a3
    74ba:	0c42 0037      	cmpiw #55,%d2
    74be:	6c00 01ec      	bgew 0x76ac
    74c2:	0c42 0020      	cmpiw #32,%d2
    74c6:	6c00 0022      	bgew 0x74ea
    74ca:	0c42 0010      	cmpiw #16,%d2
    74ce:	6c00 002a      	bgew 0x74fa
    74d2:	6000 000a      	braw 0x74de
    74d6:	e28c           	lsrl #1,%d4
    74d8:	e295           	roxrl #1,%d5
    74da:	e296           	roxrl #1,%d6
    74dc:	e297           	roxrl #1,%d7
    74de:	51ca fff6      	dbf %d2,0x74d6
    74e2:	7400           	moveq #0,%d2
    74e4:	2602           	movel %d2,%d3
    74e6:	6000 0094      	braw 0x757c
    74ea:	2e06           	movel %d6,%d7
    74ec:	2c05           	movel %d5,%d6
    74ee:	2a04           	movel %d4,%d5
    74f0:	7800           	moveq #0,%d4
    74f2:	0442 0020      	subiw #32,%d2
    74f6:	6000 ffd2      	braw 0x74ca
    74fa:	3e06           	movew %d6,%d7
    74fc:	4847           	swap %d7
    74fe:	3c05           	movew %d5,%d6
    7500:	4846           	swap %d6
    7502:	3a04           	movew %d4,%d5
    7504:	4845           	swap %d5
    7506:	383c 0000      	movew #0,%d4
    750a:	4844           	swap %d4
    750c:	0442 0010      	subiw #16,%d2
    7510:	6000 ffcc      	braw 0x74de
    7514:	c945           	exg %d4,%d5
    7516:	3c04           	movew %d4,%d6
    7518:	9c45           	subw %d5,%d6
    751a:	c98a           	exg %d4,%a2
    751c:	cb8b           	exg %d5,%a3
    751e:	0c46 0037      	cmpiw #55,%d6
    7522:	6c00 0162      	bgew 0x7686
    7526:	0c46 0020      	cmpiw #32,%d6
    752a:	6c00 0022      	bgew 0x754e
    752e:	0c46 0010      	cmpiw #16,%d6
    7532:	6c00 002a      	bgew 0x755e
    7536:	6000 000a      	braw 0x7542
    753a:	e288           	lsrl #1,%d0
    753c:	e291           	roxrl #1,%d1
    753e:	e292           	roxrl #1,%d2
    7540:	e293           	roxrl #1,%d3
    7542:	51ce fff6      	dbf %d6,0x753a
    7546:	7e00           	moveq #0,%d7
    7548:	2c07           	movel %d7,%d6
    754a:	6000 0030      	braw 0x757c
    754e:	2602           	movel %d2,%d3
    7550:	2401           	movel %d1,%d2
    7552:	2200           	movel %d0,%d1
    7554:	7000           	moveq #0,%d0
    7556:	0446 0020      	subiw #32,%d6
    755a:	6000 ffd2      	braw 0x752e
    755e:	3602           	movew %d2,%d3
    7560:	4843           	swap %d3
    7562:	3401           	movew %d1,%d2
    7564:	4842           	swap %d2
    7566:	3200           	movew %d0,%d1
    7568:	4841           	swap %d1
    756a:	303c 0000      	movew #0,%d0
    756e:	4840           	swap %d0
    7570:	0446 0010      	subiw #16,%d6
    7574:	6000 ffcc      	braw 0x7542
    7578:	c98a           	exg %d4,%a2
    757a:	cb8b           	exg %d5,%a3
    757c:	cf88           	exg %d7,%a0
    757e:	cd8b           	exg %d6,%a3
    7580:	2c07           	movel %d7,%d6
    7582:	3e3c 0000      	movew #0,%d7
    7586:	4846           	swap %d6
    7588:	3c3c 0000      	movew #0,%d6
    758c:	bf86           	eorl %d7,%d6
    758e:	6b00 0078      	bmiw 0x7608
    7592:	cf88           	exg %d7,%a0
    7594:	cd8b           	exg %d6,%a3
    7596:	d687           	addl %d7,%d3
    7598:	d586           	addxl %d6,%d2
    759a:	d385           	addxl %d5,%d1
    759c:	d184           	addxl %d4,%d0
    759e:	280a           	movel %a2,%d4
    75a0:	2e08           	movel %a0,%d7
    75a2:	0287 8000 0000 	andil #-2147483648,%d7
    75a8:	4cdf 0c00      	moveml %sp@+,%a2-%a3
    75ac:	0800 0036      	btst #54,%d0
    75b0:	6700 000c      	beqw 0x75be
    75b4:	e288           	lsrl #1,%d0
    75b6:	e291           	roxrl #1,%d1
    75b8:	e292           	roxrl #1,%d2
    75ba:	e293           	roxrl #1,%d3
    75bc:	5244           	addqw #1,%d4
    75be:	41fa 0028      	lea %pc@(0x75e8),%a0
    75c2:	43f9 0000 0064 	lea 0x64,%a1
    75c8:	93fc 0000 07b8 	subal #1976,%a1
    75ce:	d3cd           	addal %a5,%a1
    75d0:	3c29 0006      	movew %a1@(6),%d6
    75d4:	6700 0770      	beqw 0x7d46
    75d8:	0c46 0002      	cmpiw #2,%d6
    75dc:	6200 07cc      	bhiw 0x7daa
    75e0:	6d00 07c8      	bltw 0x7daa
    75e4:	6000 07c4      	braw 0x7daa
    75e8:	0c44 07ff      	cmpiw #2047,%d4
    75ec:	6c00 0012      	bgew 0x7600
    75f0:	0880 0034      	bclr #52,%d0
    75f4:	e94c           	lslw #4,%d4
    75f6:	4840           	swap %d0
    75f8:	8044           	orw %d4,%d0
    75fa:	4840           	swap %d0
    75fc:	6000 012e      	braw 0x772c
    7600:	3a3c 0001      	movew #1,%d5
    7604:	6000 fdb0      	braw 0x73b6
    7608:	cf88           	exg %d7,%a0
    760a:	cd8b           	exg %d6,%a3
    760c:	9687           	subl %d7,%d3
    760e:	9586           	subxl %d6,%d2
    7610:	9385           	subxl %d5,%d1
    7612:	9184           	subxl %d4,%d0
    7614:	6700 0112      	beqw 0x7728
    7618:	6a00 0012      	bplw 0x762c
    761c:	cf88           	exg %d7,%a0
    761e:	0847 001f      	bchg #31,%d7
    7622:	cf88           	exg %d7,%a0
    7624:	4483           	negl %d3
    7626:	4082           	negxl %d2
    7628:	4081           	negxl %d1
    762a:	4080           	negxl %d0
    762c:	280a           	movel %a2,%d4
    762e:	2e08           	movel %a0,%d7
    7630:	0287 8000 0000 	andil #-2147483648,%d7
    7636:	4cdf 0c00      	moveml %sp@+,%a2-%a3
    763a:	0800 0036      	btst #54,%d0
    763e:	6700 000c      	beqw 0x764c
    7642:	e288           	lsrl #1,%d0
    7644:	e291           	roxrl #1,%d1
    7646:	e292           	roxrl #1,%d2
    7648:	e293           	roxrl #1,%d3
    764a:	5244           	addqw #1,%d4
    764c:	41fa 0028      	lea %pc@(0x7676),%a0
    7650:	43f9 0000 0064 	lea 0x64,%a1
    7656:	93fc 0000 07b8 	subal #1976,%a1
    765c:	d3cd           	addal %a5,%a1
    765e:	3c29 0006      	movew %a1@(6),%d6
    7662:	6700 06e2      	beqw 0x7d46
    7666:	0c46 0002      	cmpiw #2,%d6
    766a:	6200 073e      	bhiw 0x7daa
    766e:	6d00 073a      	bltw 0x7daa
    7672:	6000 0736      	braw 0x7daa
    7676:	0880 0034      	bclr #52,%d0
    767a:	e94c           	lslw #4,%d4
    767c:	4840           	swap %d0
    767e:	8044           	orw %d4,%d0
    7680:	4840           	swap %d0
    7682:	6000 00a8      	braw 0x772c
    7686:	4cdf 0c00      	moveml %sp@+,%a2-%a3
    768a:	202e 0010      	movel %fp@(16),%d0
    768e:	222e 0014      	movel %fp@(20),%d1
    7692:	41f9 0000 0064 	lea 0x64,%a0
    7698:	91fc 0000 07b8 	subal #1976,%a0
    769e:	d1cd           	addal %a5,%a0
    76a0:	30bc 0000      	movew #0,%a0@
    76a4:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    76a8:	4e5e           	unlk %fp
    76aa:	4e75           	rts
    76ac:	4cdf 0c00      	moveml %sp@+,%a2-%a3
    76b0:	202e 0008      	movel %fp@(8),%d0
    76b4:	222e 000c      	movel %fp@(12),%d1
    76b8:	41f9 0000 0064 	lea 0x64,%a0
    76be:	91fc 0000 07b8 	subal #1976,%a0
    76c4:	d1cd           	addal %a5,%a0
    76c6:	30bc 0000      	movew #0,%a0@
    76ca:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    76ce:	4e5e           	unlk %fp
    76d0:	4e75           	rts
    76d2:	2807           	movel %d7,%d4
    76d4:	6000 fd9e      	braw 0x7474
    76d8:	2a07           	movel %d7,%d5
    76da:	4686           	notl %d6
    76dc:	6000 fdae      	braw 0x748c
    76e0:	2002           	movel %d2,%d0
    76e2:	2203           	movel %d3,%d1
    76e4:	6000 000a      	braw 0x76f0
    76e8:	202e 0008      	movel %fp@(8),%d0
    76ec:	222e 000c      	movel %fp@(12),%d1
    76f0:	3a3c 0001      	movew #1,%d5
    76f4:	2e00           	movel %d0,%d7
    76f6:	0287 8000 0000 	andil #-2147483648,%d7
    76fc:	0880 001f      	bclr #31,%d0
    7700:	0c80 7ff0 0000 	cmpil #2146435072,%d0
    7706:	6c00 0010      	bgew 0x7718
    770a:	2000           	movel %d0,%d0
    770c:	6600 001e      	bnew 0x772c
    7710:	0887 001f      	bclr #31,%d7
    7714:	6000 0016      	braw 0x772c
    7718:	0280 000f ffff 	andil #1048575,%d0
    771e:	8081           	orl %d1,%d0
    7720:	6600 fcc2      	bnew 0x73e4
    7724:	6000 fc90      	braw 0x73b6
    7728:	4cdf 0c00      	moveml %sp@+,%a2-%a3
    772c:	41f9 0000 0064 	lea 0x64,%a0
    7732:	91fc 0000 07b8 	subal #1976,%a0
    7738:	d1cd           	addal %a5,%a0
    773a:	30bc 0000      	movew #0,%a0@
    773e:	8087           	orl %d7,%d0
    7740:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7744:	4e5e           	unlk %fp
    7746:	4e75           	rts
    7748:	e288           	lsrl #1,%d0
    774a:	e291           	roxrl #1,%d1
    774c:	6000 ffde      	braw 0x772c
    7750:	3a3c 0001      	movew #1,%d5
    7754:	202e 0008      	movel %fp@(8),%d0
    7758:	222e 000c      	movel %fp@(12),%d1
    775c:	242e 0010      	movel %fp@(16),%d2
    7760:	262e 0014      	movel %fp@(20),%d3
    7764:	283c 7ff0 0000 	movel #2146435072,%d4
    776a:	2e00           	movel %d0,%d7
    776c:	2c02           	movel %d2,%d6
    776e:	0880 001f      	bclr #31,%d0
    7772:	0882 001f      	bclr #31,%d2
    7776:	b084           	cmpl %d4,%d0
    7778:	6200 fc6a      	bhiw 0x73e4
    777c:	6600 0008      	bnew 0x7786
    7780:	4a81           	tstl %d1
    7782:	6600 fc60      	bnew 0x73e4
    7786:	b484           	cmpl %d4,%d2
    7788:	6200 fc5a      	bhiw 0x73e4
    778c:	6600 0008      	bnew 0x7796
    7790:	4a83           	tstl %d3
    7792:	6600 fc50      	bnew 0x73e4
    7796:	bf86           	eorl %d7,%d6
    7798:	6b00 000c      	bmiw 0x77a6
    779c:	0287 8000 0000 	andil #-2147483648,%d7
    77a2:	6000 fc12      	braw 0x73b6
    77a6:	b082           	cmpl %d2,%d0
    77a8:	6600 0008      	bnew 0x77b2
    77ac:	b283           	cmpl %d3,%d1
    77ae:	6700 fc34      	beqw 0x73e4
    77b2:	0287 8000 0000 	andil #-2147483648,%d7
    77b8:	b084           	cmpl %d4,%d0
    77ba:	6700 fbfa      	beqw 0x73b6
    77be:	0847 001f      	bchg #31,%d7
    77c2:	6000 fbf2      	braw 0x73b6
    77c6:	4e56 0000      	linkw %fp,#0
    77ca:	48e7 3f00      	moveml %d2-%d7,%sp@-
    77ce:	202e 0008      	movel %fp@(8),%d0
    77d2:	222e 000c      	movel %fp@(12),%d1
    77d6:	242e 0010      	movel %fp@(16),%d2
    77da:	262e 0014      	movel %fp@(20),%d3
    77de:	2e00           	movel %d0,%d7
    77e0:	b587           	eorl %d2,%d7
    77e2:	0287 8000 0000 	andil #-2147483648,%d7
    77e8:	2047           	moveal %d7,%a0
    77ea:	2e3c 7ff0 0000 	movel #2146435072,%d7
    77f0:	2c07           	movel %d7,%d6
    77f2:	4686           	notl %d6
    77f4:	0880 001f      	bclr #31,%d0
    77f8:	2800           	movel %d0,%d4
    77fa:	8881           	orl %d1,%d4
    77fc:	6700 0128      	beqw 0x7926
    7800:	2800           	movel %d0,%d4
    7802:	0882 001f      	bclr #31,%d2
    7806:	2a02           	movel %d2,%d5
    7808:	8a83           	orl %d3,%d5
    780a:	6700 010e      	beqw 0x791a
    780e:	2a02           	movel %d2,%d5
    7810:	b087           	cmpl %d7,%d0
    7812:	6200 00de      	bhiw 0x78f2
    7816:	6700 00f2      	beqw 0x790a
    781a:	b487           	cmpl %d7,%d2
    781c:	6200 00d4      	bhiw 0x78f2
    7820:	6700 00d8      	beqw 0x78fa
    7824:	c887           	andl %d7,%d4
    7826:	6700 012e      	beqw 0x7956
    782a:	c086           	andl %d6,%d0
    782c:	0080 0010 0000 	oril #1048576,%d0
    7832:	4844           	swap %d4
    7834:	e84c           	lsrw #4,%d4
    7836:	ca87           	andl %d7,%d5
    7838:	6700 0132      	beqw 0x796c
    783c:	c486           	andl %d6,%d2
    783e:	0082 0010 0000 	oril #1048576,%d2
    7844:	4845           	swap %d5
    7846:	e84d           	lsrw #4,%d5
    7848:	d845           	addw %d5,%d4
    784a:	0444 03ff      	subiw #1023,%d4
    784e:	48e7 0030      	moveml %a2-%a3,%sp@-
    7852:	247c 0000 0000 	moveal #0,%a2
    7858:	2644           	moveal %d4,%a3
    785a:	ea9a           	rorl #5,%d2
    785c:	4842           	swap %d2
    785e:	ea9b           	rorl #5,%d3
    7860:	4843           	swap %d3
    7862:	3c03           	movew %d3,%d6
    7864:	0246 07ff      	andiw #2047,%d6
    7868:	8446           	orw %d6,%d2
    786a:	0243 f800      	andiw #-2048,%d3
    786e:	2c02           	movel %d2,%d6
    7870:	2e03           	movel %d3,%d7
    7872:	2800           	movel %d0,%d4
    7874:	2a01           	movel %d1,%d5
    7876:	7600           	moveq #0,%d3
    7878:	2403           	movel %d3,%d2
    787a:	2203           	movel %d3,%d1
    787c:	2003           	movel %d3,%d0
    787e:	227c 0000 0034 	moveal #52,%a1
    7884:	cf89           	exg %d7,%a1
    7886:	cf89           	exg %d7,%a1
    7888:	d683           	addl %d3,%d3
    788a:	d582           	addxl %d2,%d2
    788c:	d381           	addxl %d1,%d1
    788e:	d180           	addxl %d0,%d0
    7890:	de87           	addl %d7,%d7
    7892:	dd86           	addxl %d6,%d6
    7894:	6400 000e      	bccw 0x78a4
    7898:	cf8a           	exg %d7,%a2
    789a:	d685           	addl %d5,%d3
    789c:	d584           	addxl %d4,%d2
    789e:	d387           	addxl %d7,%d1
    78a0:	d187           	addxl %d7,%d0
    78a2:	cf8a           	exg %d7,%a2
    78a4:	cf89           	exg %d7,%a1
    78a6:	51cf ffde      	dbf %d7,0x7886
    78aa:	280b           	movel %a3,%d4
    78ac:	4cdf 0c00      	moveml %sp@+,%a2-%a3
    78b0:	4840           	swap %d0
    78b2:	4841           	swap %d1
    78b4:	3001           	movew %d1,%d0
    78b6:	4842           	swap %d2
    78b8:	3202           	movew %d2,%d1
    78ba:	4843           	swap %d3
    78bc:	3403           	movew %d3,%d2
    78be:	363c 0000      	movew #0,%d3
    78c2:	e288           	lsrl #1,%d0
    78c4:	e291           	roxrl #1,%d1
    78c6:	e292           	roxrl #1,%d2
    78c8:	e293           	roxrl #1,%d3
    78ca:	e288           	lsrl #1,%d0
    78cc:	e291           	roxrl #1,%d1
    78ce:	e292           	roxrl #1,%d2
    78d0:	e293           	roxrl #1,%d3
    78d2:	e288           	lsrl #1,%d0
    78d4:	e291           	roxrl #1,%d1
    78d6:	e292           	roxrl #1,%d2
    78d8:	e293           	roxrl #1,%d3
    78da:	2e08           	movel %a0,%d7
    78dc:	3a3c 0002      	movew #2,%d5
    78e0:	0800 0016      	btst #22,%d0
    78e4:	6700 029a      	beqw 0x7b80
    78e8:	e288           	lsrl #1,%d0
    78ea:	e291           	roxrl #1,%d1
    78ec:	5244           	addqw #1,%d4
    78ee:	6000 0290      	braw 0x7b80
    78f2:	3a3c 0002      	movew #2,%d5
    78f6:	6000 faec      	braw 0x73e4
    78fa:	3a3c 0002      	movew #2,%d5
    78fe:	2e08           	movel %a0,%d7
    7900:	4a83           	tstl %d3
    7902:	6600 fae0      	bnew 0x73e4
    7906:	6000 faae      	braw 0x73b6
    790a:	3a3c 0002      	movew #2,%d5
    790e:	2e08           	movel %a0,%d7
    7910:	4a81           	tstl %d1
    7912:	6600 fad0      	bnew 0x73e4
    7916:	6000 fa9e      	braw 0x73b6
    791a:	3a3c 0002      	movew #2,%d5
    791e:	c540           	exg %d2,%d0
    7920:	c741           	exg %d3,%d1
    7922:	6000 000e      	braw 0x7932
    7926:	242e 0010      	movel %fp@(16),%d2
    792a:	262e 0014      	movel %fp@(20),%d3
    792e:	0882 001f      	bclr #31,%d2
    7932:	0c82 7ff0 0000 	cmpil #2146435072,%d2
    7938:	6c00 faaa      	bgew 0x73e4
    793c:	41f9 0000 0064 	lea 0x64,%a0
    7942:	91fc 0000 07b8 	subal #1976,%a0
    7948:	d1cd           	addal %a5,%a0
    794a:	30bc 0000      	movew #0,%a0@
    794e:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7952:	4e5e           	unlk %fp
    7954:	4e75           	rts
    7956:	7801           	moveq #1,%d4
    7958:	c086           	andl %d6,%d0
    795a:	d281           	addl %d1,%d1
    795c:	d180           	addxl %d0,%d0
    795e:	5344           	subqw #1,%d4
    7960:	0800 0014      	btst #20,%d0
    7964:	6600 fed0      	bnew 0x7836
    7968:	6000 fff0      	braw 0x795a
    796c:	7a01           	moveq #1,%d5
    796e:	c486           	andl %d6,%d2
    7970:	d683           	addl %d3,%d3
    7972:	d582           	addxl %d2,%d2
    7974:	5345           	subqw #1,%d5
    7976:	0802 0014      	btst #20,%d2
    797a:	6600 fecc      	bnew 0x7848
    797e:	6000 fff0      	braw 0x7970
    7982:	4e56 0000      	linkw %fp,#0
    7986:	48e7 3f00      	moveml %d2-%d7,%sp@-
    798a:	202e 0008      	movel %fp@(8),%d0
    798e:	222e 000c      	movel %fp@(12),%d1
    7992:	242e 0010      	movel %fp@(16),%d2
    7996:	262e 0014      	movel %fp@(20),%d3
    799a:	2e00           	movel %d0,%d7
    799c:	b587           	eorl %d2,%d7
    799e:	0287 8000 0000 	andil #-2147483648,%d7
    79a4:	2047           	moveal %d7,%a0
    79a6:	2e3c 7ff0 0000 	movel #2146435072,%d7
    79ac:	2c07           	movel %d7,%d6
    79ae:	4686           	notl %d6
    79b0:	0880 001f      	bclr #31,%d0
    79b4:	2800           	movel %d0,%d4
    79b6:	8881           	orl %d1,%d4
    79b8:	6700 0116      	beqw 0x7ad0
    79bc:	2800           	movel %d0,%d4
    79be:	0882 001f      	bclr #31,%d2
    79c2:	2a02           	movel %d2,%d5
    79c4:	8a83           	orl %d3,%d5
    79c6:	6700 014a      	beqw 0x7b12
    79ca:	2a02           	movel %d2,%d5
    79cc:	b087           	cmpl %d7,%d0
    79ce:	6200 00f8      	bhiw 0x7ac8
    79d2:	6700 0166      	beqw 0x7b3a
    79d6:	b487           	cmpl %d7,%d2
    79d8:	6200 00ee      	bhiw 0x7ac8
    79dc:	6700 014e      	beqw 0x7b2c
    79e0:	c887           	andl %d7,%d4
    79e2:	6700 0170      	beqw 0x7b54
    79e6:	c086           	andl %d6,%d0
    79e8:	0080 0010 0000 	oril #1048576,%d0
    79ee:	4844           	swap %d4
    79f0:	e84c           	lsrw #4,%d4
    79f2:	ca87           	andl %d7,%d5
    79f4:	6700 0174      	beqw 0x7b6a
    79f8:	c486           	andl %d6,%d2
    79fa:	0082 0010 0000 	oril #1048576,%d2
    7a00:	4845           	swap %d5
    7a02:	e84d           	lsrw #4,%d5
    7a04:	9845           	subw %d5,%d4
    7a06:	0644 03fe      	addiw #1022,%d4
    7a0a:	7c00           	moveq #0,%d6
    7a0c:	2e06           	movel %d6,%d7
    7a0e:	227c 0000 0000 	moveal #0,%a1
    7a14:	7a16           	moveq #22,%d5
    7a16:	b480           	cmpl %d0,%d2
    7a18:	6200 000c      	bhiw 0x7a26
    7a1c:	6700 0014      	beqw 0x7a32
    7a20:	9283           	subl %d3,%d1
    7a22:	9182           	subxl %d2,%d0
    7a24:	0bc6           	bset %d5,%d6
    7a26:	d281           	addl %d1,%d1
    7a28:	d180           	addxl %d0,%d0
    7a2a:	51cd ffea      	dbf %d5,0x7a16
    7a2e:	6000 000c      	braw 0x7a3c
    7a32:	b681           	cmpl %d1,%d3
    7a34:	6200 fff0      	bhiw 0x7a26
    7a38:	6000 ffe6      	braw 0x7a20
    7a3c:	7a1f           	moveq #31,%d5
    7a3e:	b480           	cmpl %d0,%d2
    7a40:	6200 000c      	bhiw 0x7a4e
    7a44:	6700 0014      	beqw 0x7a5a
    7a48:	9283           	subl %d3,%d1
    7a4a:	9182           	subxl %d2,%d0
    7a4c:	0bc7           	bset %d5,%d7
    7a4e:	d281           	addl %d1,%d1
    7a50:	d180           	addxl %d0,%d0
    7a52:	51cd ffea      	dbf %d5,0x7a3e
    7a56:	6000 000c      	braw 0x7a64
    7a5a:	b681           	cmpl %d1,%d3
    7a5c:	6200 fff0      	bhiw 0x7a4e
    7a60:	6000 ffe6      	braw 0x7a48
    7a64:	7a35           	moveq #53,%d5
    7a66:	b082           	cmpl %d2,%d0
    7a68:	6200 001c      	bhiw 0x7a86
    7a6c:	6700 0012      	beqw 0x7a80
    7a70:	d281           	addl %d1,%d1
    7a72:	d180           	addxl %d0,%d0
    7a74:	51cd fff0      	dbf %d5,0x7a66
    7a78:	7400           	moveq #0,%d2
    7a7a:	2602           	movel %d2,%d3
    7a7c:	6000 0028      	braw 0x7aa6
    7a80:	b681           	cmpl %d1,%d3
    7a82:	6200 ffec      	bhiw 0x7a70
    7a86:	7400           	moveq #0,%d2
    7a88:	2602           	movel %d2,%d3
    7a8a:	0445 0035      	subiw #53,%d5
    7a8e:	0645 003f      	addiw #63,%d5
    7a92:	0c45 001f      	cmpiw #31,%d5
    7a96:	6200 000c      	bhiw 0x7aa4
    7a9a:	0bc3           	bset %d5,%d3
    7a9c:	6000 0008      	braw 0x7aa6
    7aa0:	0445 0020      	subiw #32,%d5
    7aa4:	0bc2           	bset %d5,%d2
    7aa6:	2006           	movel %d6,%d0
    7aa8:	2207           	movel %d7,%d1
    7aaa:	7600           	moveq #0,%d3
    7aac:	0800 0016      	btst #22,%d0
    7ab0:	6700 000c      	beqw 0x7abe
    7ab4:	e288           	lsrl #1,%d0
    7ab6:	e291           	roxrl #1,%d1
    7ab8:	e292           	roxrl #1,%d2
    7aba:	e293           	roxrl #1,%d3
    7abc:	5244           	addqw #1,%d4
    7abe:	2e08           	movel %a0,%d7
    7ac0:	3a3c 0003      	movew #3,%d5
    7ac4:	6000 00ba      	braw 0x7b80
    7ac8:	3a3c 0003      	movew #3,%d5
    7acc:	6000 f916      	braw 0x73e4
    7ad0:	3a3c 0003      	movew #3,%d5
    7ad4:	0882 001f      	bclr #31,%d2
    7ad8:	2802           	movel %d2,%d4
    7ada:	8883           	orl %d3,%d4
    7adc:	6700 f906      	beqw 0x73e4
    7ae0:	0c82 7ff0 0000 	cmpil #2146435072,%d2
    7ae6:	6200 f8fc      	bhiw 0x73e4
    7aea:	6d00 0008      	bltw 0x7af4
    7aee:	4a83           	tstl %d3
    7af0:	6600 f8f2      	bnew 0x73e4
    7af4:	7000           	moveq #0,%d0
    7af6:	2200           	movel %d0,%d1
    7af8:	41f9 0000 0064 	lea 0x64,%a0
    7afe:	91fc 0000 07b8 	subal #1976,%a0
    7b04:	d1cd           	addal %a5,%a0
    7b06:	30bc 0000      	movew #0,%a0@
    7b0a:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7b0e:	4e5e           	unlk %fp
    7b10:	4e75           	rts
    7b12:	3a3c 0003      	movew #3,%d5
    7b16:	2e08           	movel %a0,%d7
    7b18:	0c80 7ff0 0000 	cmpil #2146435072,%d0
    7b1e:	6200 f8c4      	bhiw 0x73e4
    7b22:	4a81           	tstl %d1
    7b24:	6600 f8be      	bnew 0x73e4
    7b28:	6000 f8ce      	braw 0x73f8
    7b2c:	3a3c 0003      	movew #3,%d5
    7b30:	4a83           	tstl %d3
    7b32:	6600 f8b0      	bnew 0x73e4
    7b36:	6000 f898      	braw 0x73d0
    7b3a:	3a3c 0003      	movew #3,%d5
    7b3e:	4a81           	tstl %d1
    7b40:	6600 f8a2      	bnew 0x73e4
    7b44:	b487           	cmpl %d7,%d2
    7b46:	6c00 f89c      	bgew 0x73e4
    7b4a:	4a83           	tstl %d3
    7b4c:	6600 f896      	bnew 0x73e4
    7b50:	6000 f864      	braw 0x73b6
    7b54:	7801           	moveq #1,%d4
    7b56:	c086           	andl %d6,%d0
    7b58:	d281           	addl %d1,%d1
    7b5a:	d180           	addxl %d0,%d0
    7b5c:	5344           	subqw #1,%d4
    7b5e:	0800 0014      	btst #20,%d0
    7b62:	6600 fe8e      	bnew 0x79f2
    7b66:	6000 fff0      	braw 0x7b58
    7b6a:	7a01           	moveq #1,%d5
    7b6c:	c486           	andl %d6,%d2
    7b6e:	d683           	addl %d3,%d3
    7b70:	d582           	addxl %d2,%d2
    7b72:	5345           	subqw #1,%d5
    7b74:	0802 0014      	btst #20,%d2
    7b78:	6600 fe8a      	bnew 0x7a04
    7b7c:	6000 fff0      	braw 0x7b6e
    7b80:	0c44 ffca      	cmpiw #-54,%d4
    7b84:	6d00 f84a      	bltw 0x73d0
    7b88:	2047           	moveal %d7,%a0
    7b8a:	7c00           	moveq #0,%d6
    7b8c:	2e06           	movel %d6,%d7
    7b8e:	0c44 0001      	cmpiw #1,%d4
    7b92:	6c00 0020      	bgew 0x7bb4
    7b96:	5244           	addqw #1,%d4
    7b98:	e288           	lsrl #1,%d0
    7b9a:	e291           	roxrl #1,%d1
    7b9c:	e292           	roxrl #1,%d2
    7b9e:	e293           	roxrl #1,%d3
    7ba0:	e296           	roxrl #1,%d6
    7ba2:	e297           	roxrl #1,%d7
    7ba4:	0c44 0001      	cmpiw #1,%d4
    7ba8:	6700 000a      	beqw 0x7bb4
    7bac:	6000 ffe8      	braw 0x7b96
    7bb0:	6000 f81e      	braw 0x73d0
    7bb4:	8486           	orl %d6,%d2
    7bb6:	8687           	orl %d7,%d3
    7bb8:	2e08           	movel %a0,%d7
    7bba:	41fa 0028      	lea %pc@(0x7be4),%a0
    7bbe:	43f9 0000 0064 	lea 0x64,%a1
    7bc4:	93fc 0000 07b8 	subal #1976,%a1
    7bca:	d3cd           	addal %a5,%a1
    7bcc:	3c29 0006      	movew %a1@(6),%d6
    7bd0:	6700 0174      	beqw 0x7d46
    7bd4:	0c46 0002      	cmpiw #2,%d6
    7bd8:	6200 01d0      	bhiw 0x7daa
    7bdc:	6d00 01cc      	bltw 0x7daa
    7be0:	6000 01c8      	braw 0x7daa
    7be4:	0c44 07ff      	cmpiw #2047,%d4
    7be8:	6c00 f7cc      	bgew 0x73b6
    7bec:	3804           	movew %d4,%d4
    7bee:	6700 f7b4      	beqw 0x73a4
    7bf2:	e94c           	lslw #4,%d4
    7bf4:	0880 0014      	bclr #20,%d0
    7bf8:	4840           	swap %d0
    7bfa:	8044           	orw %d4,%d0
    7bfc:	4840           	swap %d0
    7bfe:	8087           	orl %d7,%d0
    7c00:	41f9 0000 0064 	lea 0x64,%a0
    7c06:	91fc 0000 07b8 	subal #1976,%a0
    7c0c:	d1cd           	addal %a5,%a0
    7c0e:	30bc 0000      	movew #0,%a0@
    7c12:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7c16:	4e5e           	unlk %fp
    7c18:	4e75           	rts
    7c1a:	4e56 0000      	linkw %fp,#0
    7c1e:	48e7 3f00      	moveml %d2-%d7,%sp@-
    7c22:	3a3c 0004      	movew #4,%d5
    7c26:	202e 0008      	movel %fp@(8),%d0
    7c2a:	222e 000c      	movel %fp@(12),%d1
    7c2e:	0840 001f      	bchg #31,%d0
    7c32:	2400           	movel %d0,%d2
    7c34:	0882 001f      	bclr #31,%d2
    7c38:	2802           	movel %d2,%d4
    7c3a:	8881           	orl %d1,%d4
    7c3c:	6700 003c      	beqw 0x7c7a
    7c40:	0c82 7ff0 0000 	cmpil #2146435072,%d2
    7c46:	6d00 0018      	bltw 0x7c60
    7c4a:	6200 f798      	bhiw 0x73e4
    7c4e:	4a81           	tstl %d1
    7c50:	6600 f792      	bnew 0x73e4
    7c54:	2e00           	movel %d0,%d7
    7c56:	0287 8000 0000 	andil #-2147483648,%d7
    7c5c:	6000 f758      	braw 0x73b6
    7c60:	41f9 0000 0064 	lea 0x64,%a0
    7c66:	91fc 0000 07b8 	subal #1976,%a0
    7c6c:	d1cd           	addal %a5,%a0
    7c6e:	30bc 0000      	movew #0,%a0@
    7c72:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7c76:	4e5e           	unlk %fp
    7c78:	4e75           	rts
    7c7a:	0880 001f      	bclr #31,%d0
    7c7e:	6000 ffe0      	braw 0x7c60
    7c82:	4e56 0000      	linkw %fp,#0
    7c86:	48e7 3f00      	moveml %d2-%d7,%sp@-
    7c8a:	3a3c 0005      	movew #5,%d5
    7c8e:	202e 0008      	movel %fp@(8),%d0
    7c92:	222e 000c      	movel %fp@(12),%d1
    7c96:	242e 0010      	movel %fp@(16),%d2
    7c9a:	262e 0014      	movel %fp@(20),%d3
    7c9e:	2c00           	movel %d0,%d6
    7ca0:	0880 001f      	bclr #31,%d0
    7ca4:	2e02           	movel %d2,%d7
    7ca6:	0882 001f      	bclr #31,%d2
    7caa:	0c80 7fff 0000 	cmpil #2147418112,%d0
    7cb0:	6200 f732      	bhiw 0x73e4
    7cb4:	6700 007c      	beqw 0x7d32
    7cb8:	2800           	movel %d0,%d4
    7cba:	8881           	orl %d1,%d4
    7cbc:	6700 0064      	beqw 0x7d22
    7cc0:	0c82 7fff 0000 	cmpil #2147418112,%d2
    7cc6:	6200 f71c      	bhiw 0x73e4
    7cca:	6700 0070      	beqw 0x7d3c
    7cce:	2802           	movel %d2,%d4
    7cd0:	8883           	orl %d3,%d4
    7cd2:	6700 0056      	beqw 0x7d2a
    7cd6:	bd87           	eorl %d6,%d7
    7cd8:	6a00 000c      	bplw 0x7ce6
    7cdc:	4a86           	tstl %d6
    7cde:	6a00 002e      	bplw 0x7d0e
    7ce2:	6b00 0034      	bmiw 0x7d18
    7ce6:	4a86           	tstl %d6
    7ce8:	6a00 0006      	bplw 0x7cf0
    7cec:	c142           	exg %d0,%d2
    7cee:	c343           	exg %d1,%d3
    7cf0:	b480           	cmpl %d0,%d2
    7cf2:	6200 0024      	bhiw 0x7d18
    7cf6:	6600 0016      	bnew 0x7d0e
    7cfa:	b681           	cmpl %d1,%d3
    7cfc:	6200 001a      	bhiw 0x7d18
    7d00:	6600 000c      	bnew 0x7d0e
    7d04:	7000           	moveq #0,%d0
    7d06:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7d0a:	4e5e           	unlk %fp
    7d0c:	4e75           	rts
    7d0e:	7001           	moveq #1,%d0
    7d10:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7d14:	4e5e           	unlk %fp
    7d16:	4e75           	rts
    7d18:	70ff           	moveq #-1,%d0
    7d1a:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7d1e:	4e5e           	unlk %fp
    7d20:	4e75           	rts
    7d22:	0886 001f      	bclr #31,%d6
    7d26:	6000 ff98      	braw 0x7cc0
    7d2a:	0887 001f      	bclr #31,%d7
    7d2e:	6000 ffa6      	braw 0x7cd6
    7d32:	4a81           	tstl %d1
    7d34:	6600 f6ae      	bnew 0x73e4
    7d38:	6000 ff86      	braw 0x7cc0
    7d3c:	4a83           	tstl %d3
    7d3e:	6600 f6a4      	bnew 0x73e4
    7d42:	6000 ff92      	braw 0x7cd6
    7d46:	0800 0015      	btst #21,%d0
    7d4a:	6600 0016      	bnew 0x7d62
    7d4e:	0c44 0001      	cmpiw #1,%d4
    7d52:	6700 000e      	beqw 0x7d62
    7d56:	d683           	addl %d3,%d3
    7d58:	d582           	addxl %d2,%d2
    7d5a:	d381           	addxl %d1,%d1
    7d5c:	d180           	addxl %d0,%d0
    7d5e:	51cc ffe6      	dbf %d4,0x7d46
    7d62:	0801 0000      	btst #0,%d1
    7d66:	6700 0022      	beqw 0x7d8a
    7d6a:	8682           	orl %d2,%d3
    7d6c:	6600 0014      	bnew 0x7d82
    7d70:	2601           	movel %d1,%d3
    7d72:	0283 0000 0002 	andil #2,%d3
    7d78:	7400           	moveq #0,%d2
    7d7a:	d283           	addl %d3,%d1
    7d7c:	d182           	addxl %d2,%d0
    7d7e:	6000 000a      	braw 0x7d8a
    7d82:	7601           	moveq #1,%d3
    7d84:	7400           	moveq #0,%d2
    7d86:	d283           	addl %d3,%d1
    7d88:	d182           	addxl %d2,%d0
    7d8a:	e288           	lsrl #1,%d0
    7d8c:	e291           	roxrl #1,%d1
    7d8e:	0800 0015      	btst #21,%d0
    7d92:	6700 0008      	beqw 0x7d9c
    7d96:	e288           	lsrl #1,%d0
    7d98:	e291           	roxrl #1,%d1
    7d9a:	5244           	addqw #1,%d4
    7d9c:	0800 0014      	btst #20,%d0
    7da0:	6700 0004      	beqw 0x7da6
    7da4:	4ed0           	jmp %a0@
    7da6:	7800           	moveq #0,%d4
    7da8:	4ed0           	jmp %a0@
    7daa:	4ed0           	jmp %a0@
    7dac:	41f9 0000 0064 	lea 0x64,%a0
    7db2:	91fc 0000 07b8 	subal #1976,%a0
    7db8:	d1cd           	addal %a5,%a0
    7dba:	317c 0000 0004 	movew #0,%a0@(4)
    7dc0:	4e75           	rts
    7dc2:	41f9 0000 0064 	lea 0x64,%a0
    7dc8:	91fc 0000 07b8 	subal #1976,%a0
    7dce:	d1cd           	addal %a5,%a0
    7dd0:	3147 0000      	movew %d7,%a0@(0)
    7dd4:	8f68 0004      	orw %d7,%a0@(4)
    7dd8:	3146 0008      	movew %d6,%a0@(8)
    7ddc:	3145 000a      	movew %d5,%a0@(10)
    7de0:	0c46 0001      	cmpiw #1,%d6
    7de4:	6700 001e      	beqw 0x7e04
    7de8:	216e 0008 000c 	movel %fp@(8),%a0@(12)
    7dee:	216e 000c 0010 	movel %fp@(12),%a0@(16)
    7df4:	216e 0010 0014 	movel %fp@(16),%a0@(20)
    7dfa:	216e 0014 0018 	movel %fp@(20),%a0@(24)
    7e00:	6000 000e      	braw 0x7e10
    7e04:	216e 0008 000c 	movel %fp@(8),%a0@(12)
    7e0a:	216e 000c 0014 	movel %fp@(12),%a0@(20)
    7e10:	ce68 0002      	andw %a0@(2),%d7
    7e14:	6700 0014      	beqw 0x7e2a
    7e18:	41f9 0000 0064 	lea 0x64,%a0
    7e1e:	91fc 0000 07b8 	subal #1976,%a0
    7e24:	d1cd           	addal %a5,%a0
    7e26:	2f08           	movel %a0,%sp@-
    7e28:	4e4f           	trap #15
    7e2a:	4cdf 00fc      	moveml %sp@+,%d2-%d7
    7e2e:	4e5e           	unlk %fp
    7e30:	4e75           	rts
	...
