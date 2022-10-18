# MSSlotOpen: 0x008e - 0x0386

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-14)`    : ???
   * `%fp@(-46)`    : ???

```
    8e:  4e56 ffd2      	linkw %fp,#-46                          
    92:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-                
    96:  362e 0008      	movew %fp@(8),%d3                       
    9a:  2d7c 536c 4d73 	movel #1399606643,%fp@(-14)             
    a0:  fff2                                                    
    a2:  486e fff6      	pea %fp@(-10)                           
    a6:  3f3c 4e20      	movew #20000,%sp@-                      
    aa:  2f3c 5368 416c 	movel #1399341420,%sp@-                 
    b0:  4e4f           	trap #15                                
    b2:  a27b           	0121173                                 ; sysTrapFtrGet
    b4:  4a40           	tstw %d0                                
    b6:  4fef 000a      	lea %sp@(10),%sp                        
    ba:  6708           	beqs 0xc4                               ; ↓ label_0
    bc:  303c 2901      	movew #10497,%d0                        
    c0:  6000 02be      	braw 0x380                              ; ↓ label_17

  label_0:
    c4:  206e fff6      	moveal %fp@(-10),%a0                    
    c8:  0c68 0002 0004 	cmpiw #2,%a0@(4)                        
    ce:  6708           	beqs 0xd8                               ; ↓ label_1
    d0:  303c 2901      	movew #10497,%d0                        
    d4:  6000 02aa      	braw 0x380                              ; ↓ label_17

  label_1:
    d8:  2478 0304      	moveal 0x304,%a2                        
    dc:  2078 0304      	moveal 0x304,%a0                        
    e0:  4aa8 0010      	tstl %a0@(16)                           
    e4:  670a           	beqs 0xf0                               ; ↓ label_2
    e6:  206a 0010      	moveal %a2@(16),%a0                     
    ea:  317c 0001 0024 	movew #1,%a0@(36)                       

  label_2:
    f0:  3f03           	movew %d3,%sp@-                         
    f2:  4e4f           	trap #15                                
    f4:  a0b9           	0120271                                 ; sysTrapSysLibTblEntry
    f6:  2468 0004      	moveal %a0@(4),%a2                      
    fa:  200a           	movel %a2,%d0                           
    fc:  544f           	addqw #2,%sp                            
    fe:  6600 027c      	bnew 0x37c                              ; ↓ label_16
    102:  4878 0026      	pea 0x26                               
    106:  4e4f           	trap #15                               
    108:  a013           	0120023                                ; sysTrapMemPtrNew
    10a:  2448           	moveal %a0,%a2                         
    10c:  200a           	movel %a2,%d0                          
    10e:  584f           	addqw #4,%sp                           
    110:  6608           	bnes 0x11a                             ; ↓ label_3
    112:  303c 0102      	movew #258,%d0                         
    116:  6000 0268      	braw 0x380                             ; ↓ label_17

  label_3:
    11a:  4267           	clrw %sp@-                             
    11c:  2f0a           	movel %a2,%sp@-                        
    11e:  4e4f           	trap #15                               
    120:  a01b           	0120033                                ; sysTrapMemPtrSetOwner
    122:  4227           	clrb %sp@-                             
    124:  4878 0026      	pea 0x26                               
    128:  2f0a           	movel %a2,%sp@-                        
    12a:  4e4f           	trap #15                               
    12c:  a027           	0120047                                ; sysTrapMemSet
    12e:  3f03           	movew %d3,%sp@-                        
    130:  4e4f           	trap #15                               
    132:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    134:  214a 0004      	movel %a2,%a0@(4)                      
    138:  486a 001c      	pea %a2@(28)                           
    13c:  2f3c 0000 80c0 	movel #32960,%sp@-                     
    142:  3f3c 0001      	movew #1,%sp@-                         
    146:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    14c:  4e4f           	trap #15                               
    14e:  a35a           	0121532                                ; sysTrapFtrPtrNew
    150:  4aaa 001c      	tstl %a2@(28)                          
    154:  4fef 0020      	lea %sp@(32),%sp                       
    158:  661a           	bnes 0x174                             ; ↓ label_4
    15a:  3f03           	movew %d3,%sp@-                        
    15c:  4e4f           	trap #15                               
    15e:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    160:  42a8 0004      	clrl %a0@(4)                           
    164:  2f0a           	movel %a2,%sp@-                        
    166:  4e4f           	trap #15                               
    168:  a012           	0120022                                ; sysTrapMemChunkFree
    16a:  303c 0102      	movew #258,%d0                         
    16e:  5c4f           	addqw #6,%sp                           
    170:  6000 020e      	braw 0x380                             ; ↓ label_17

  label_4:
    174:  486a 0020      	pea %a2@(32)                           
    178:  4878 2a00      	pea 0x2a00                             
    17c:  3f3c 0002      	movew #2,%sp@-                         
    180:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    186:  4e4f           	trap #15                               
    188:  a35a           	0121532                                ; sysTrapFtrPtrNew
    18a:  4aaa 0020      	tstl %a2@(32)                          
    18e:  4fef 000e      	lea %sp@(14),%sp                       
    192:  662a           	bnes 0x1be                             ; ↓ label_5
    194:  3f3c 0001      	movew #1,%sp@-                         
    198:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    19e:  4e4f           	trap #15                               
    1a0:  a35b           	0121533                                ; sysTrapFtrPtrFree
    1a2:  3f03           	movew %d3,%sp@-                        
    1a4:  4e4f           	trap #15                               
    1a6:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    1a8:  42a8 0004      	clrl %a0@(4)                           
    1ac:  2f0a           	movel %a2,%sp@-                        
    1ae:  4e4f           	trap #15                               
    1b0:  a012           	0120022                                ; sysTrapMemChunkFree
    1b2:  303c 0102      	movew #258,%d0                         
    1b6:  4fef 000c      	lea %sp@(12),%sp                       
    1ba:  6000 01c4      	braw 0x380                             ; ↓ label_17

  label_5:
    1be:  2f0a           	movel %a2,%sp@-                        
    1c0:  4878 0064      	pea 0x64                               
    1c4:  487a 1248      	pea %pc@(0x140e)                       ; PrvTimerProc
    1c8:  486e fff2      	pea %fp@(-14)                          
    1cc:  486a 000e      	pea %a2@(14)                           
    1d0:  4e4f           	trap #15                               
    1d2:  a099           	0120231                                ; sysTrapSysTimerCreate
    1d4:  3800           	movew %d0,%d4                          
    1d6:  4fef 0014      	lea %sp@(20),%sp                       
    1da:  6736           	beqs 0x212                             ; ↓ label_6
    1dc:  3f3c 0001      	movew #1,%sp@-                         
    1e0:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    1e6:  4e4f           	trap #15                               
    1e8:  a35b           	0121533                                ; sysTrapFtrPtrFree
    1ea:  3f3c 0002      	movew #2,%sp@-                         
    1ee:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    1f4:  4e4f           	trap #15                               
    1f6:  a35b           	0121533                                ; sysTrapFtrPtrFree
    1f8:  3f03           	movew %d3,%sp@-                        
    1fa:  4e4f           	trap #15                               
    1fc:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    1fe:  42a8 0004      	clrl %a0@(4)                           
    202:  2f0a           	movel %a2,%sp@-                        
    204:  4e4f           	trap #15                               
    206:  a012           	0120022                                ; sysTrapMemChunkFree
    208:  3004           	movew %d4,%d0                          
    20a:  4fef 0012      	lea %sp@(18),%sp                       
    20e:  6000 0170      	braw 0x380                             ; ↓ label_17

  label_6:
    212:  4ab8 0304      	tstl 0x304                             
    216:  6714           	beqs 0x22c                             ; ↓ label_9
    218:  2078 0304      	moveal 0x304,%a0                       
    21c:  0c90 5368 4776 	cmpil #1399342966,%a0@                 
    222:  6604           	bnes 0x228                             ; ↓ label_7
    224:  7001           	moveq #1,%d0                           
    226:  6002           	bras 0x22a                             ; ↓ label_8

  label_7:
    228:  7000           	moveq #0,%d0                           

  label_8:
    22a:  6002           	bras 0x22e                             ; ↓ label_10

  label_9:
    22c:  7000           	moveq #0,%d0                           

  label_10:
    22e:  4a40           	tstw %d0                               
    230:  6640           	bnes 0x272                             ; ↓ label_11
    232:  2f2a 000e      	movel %a2@(14),%sp@-                   
    236:  4e4f           	trap #15                               
    238:  a0bf           	0120277                                ; sysTrapSysTimerDelete
    23a:  3f3c 0001      	movew #1,%sp@-                         
    23e:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    244:  4e4f           	trap #15                               
    246:  a35b           	0121533                                ; sysTrapFtrPtrFree
    248:  3f3c 0002      	movew #2,%sp@-                         
    24c:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    252:  4e4f           	trap #15                               
    254:  a35b           	0121533                                ; sysTrapFtrPtrFree
    256:  3f03           	movew %d3,%sp@-                        
    258:  4e4f           	trap #15                               
    25a:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    25c:  42a8 0004      	clrl %a0@(4)                           
    260:  2f0a           	movel %a2,%sp@-                        
    262:  4e4f           	trap #15                               
    264:  a012           	0120022                                ; sysTrapMemChunkFree
    266:  303c 0102      	movew #258,%d0                         
    26a:  4fef 0016      	lea %sp@(22),%sp                       
    26e:  6000 0110      	braw 0x380                             ; ↓ label_17

  label_11:
    272:  2078 0304      	moveal 0x304,%a0                       
    276:  214a 0024      	movel %a2,%a0@(36)                     
    27a:  486a 0024      	pea %a2@(36)                           
    27e:  41fa 0116      	lea %pc@(0x396),%a0                    ; ??? 0x396
    282:  4850           	pea %a0@                               
    284:  4e4f           	trap #15                               
    286:  a0ba           	0120272                                ; sysTrapSysLibFind
    288:  3800           	movew %d0,%d4                          
    28a:  504f           	addqw #8,%sp                           
    28c:  671a           	beqs 0x2a8                             ; ↓ label_12
    28e:  486a 0024      	pea %a2@(36)                           
    292:  2f3c 536c 4d67 	movel #1399606631,%sp@-                
    298:  2f3c 6c69 6272 	movel #1818845810,%sp@-                
    29e:  4e4f           	trap #15                               
    2a0:  a2ac           	0121254                                ; sysTrapSysLibLoad
    2a2:  3800           	movew %d0,%d4                          
    2a4:  4fef 000c      	lea %sp@(12),%sp                       

  label_12:
    2a8:  4a44           	tstw %d4                               
    2aa:  670a           	beqs 0x2b6                             ; ↓ label_13
    2ac:  02aa ffff feff 	andil #-257,%a2@(4)                    
    2b2:  0004                                                   
    2b4:  6020           	bras 0x2d6                             ; ↓ label_15

  label_13:
    2b6:  3f2a 0024      	movew %a2@(36),%sp@-                   
    2ba:  4e4f           	trap #15                               
    2bc:  a801           	0124001                                ; ??? 0xa801
    2be:  3800           	movew %d0,%d4                          
    2c0:  544f           	addqw #2,%sp                           
    2c2:  670a           	beqs 0x2ce                             ; ↓ label_14
    2c4:  02aa ffff feff 	andil #-257,%a2@(4)                    
    2ca:  0004                                                   
    2cc:  6008           	bras 0x2d6                             ; ↓ label_15

  label_14:
    2ce:  00aa 0000 0100 	oril #256,%a2@(4)                      
    2d4:  0004                                                   

  label_15:
    2d6:  4eba 1468      	jsr %pc@(0x1740)                       ; MSSlotHwrOpen
    2da:  486a 0012      	pea %a2@(18)                           
    2de:  3f03           	movew %d3,%sp@-                        
    2e0:  7404           	moveq #4,%d2                           
    2e2:  4e4f           	trap #15                               
    2e4:  a347           	0121507                                ; sysTrapExpansionDispatch
    2e6:  486e fffa      	pea %fp@(-6)                           
    2ea:  486e fffe      	pea %fp@(-2)                           
    2ee:  1f3c 0001      	moveb #1,%sp@-                         
    2f2:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    2f8:  2f3c 6c69 6273 	movel #1818845811,%sp@-                
    2fe:  486e ffd2      	pea %fp@(-46)                          
    302:  1f3c 0001      	moveb #1,%sp@-                         
    306:  4e4f           	trap #15                               
    308:  a078           	0120170                                ; sysTrapDmGetNextDatabaseByTypeCreator
    30a:  2f0a           	movel %a2,%sp@-                        
    30c:  1f3c fff6      	moveb #-10,%sp@-                       
    310:  487a 0e5a      	pea %pc@(0x116c)                       ; PrvCrdiNotificationHandler
    314:  2f3c 6372 6469 	movel #1668441193,%sp@-                
    31a:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    31e:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    322:  4e4f           	trap #15                               
    324:  a353           	0121523                                ; sysTrapSysNotifyRegister
    326:  2f0a           	movel %a2,%sp@-                        
    328:  1f3c fff6      	moveb #-10,%sp@-                       
    32c:  487a 0e86      	pea %pc@(0x11b4)                       ; PrvCrdoNotificationHandler
    330:  2f3c 6372 646f 	movel #1668441199,%sp@-                
    336:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    33a:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    33e:  4e4f           	trap #15                               
    340:  a353           	0121523                                ; sysTrapSysNotifyRegister
    342:  2f0a           	movel %a2,%sp@-                        
    344:  1f3c fff6      	moveb #-10,%sp@-                       
    348:  487a 0ec6      	pea %pc@(0x1210)                       ; PrvVolmNotificationHandler
    34c:  2f3c 766f 6c6d 	movel #1987013741,%sp@-                
    352:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    356:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    35a:  4e4f           	trap #15                               
    35c:  a353           	0121523                                ; sysTrapSysNotifyRegister
    35e:  7008           	moveq #8,%d0                           
    360:  81aa 0004      	orl %d0,%a2@(4)                        
    364:  4227           	clrb %sp@-                             
    366:  4227           	clrb %sp@-                             
    368:  4eba 1504      	jsr %pc@(0x186e)                       ; MSSlotHwrSetUpInterrupt
    36c:  1f3c 0001      	moveb #1,%sp@-                         
    370:  1f3c 0001      	moveb #1,%sp@-                         
    374:  4eba 14f8      	jsr %pc@(0x186e)                       ; MSSlotHwrSetUpInterrupt
    378:  4fef 0062      	lea %sp@(98),%sp                       

  label_16:
    37c:  5292           	addql #1,%a2@                          
    37e:  7000           	moveq #0,%d0                           

  label_17:
    380:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2               
    384:  4e5e           	unlk %fp                               
    386:  4e75           	rts                                    
```

# MSSlotClose: 0x03aa - 0x04c2

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-38)`    : ???

```
    3aa:  4e56 ffda      	linkw %fp,#-38                         
    3ae:  2f0a           	movel %a2,%sp@-                        
    3b0:  3f2e 0008      	movew %fp@(8),%sp@-                    
    3b4:  4e4f           	trap #15                               
    3b6:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    3b8:  2468 0004      	moveal %a0@(4),%a2                     
    3bc:  200a           	movel %a2,%d0                          
    3be:  544f           	addqw #2,%sp                           
    3c0:  6704           	beqs 0x3c6                             ; ↓ label_0
    3c2:  4a92           	tstl %a2@                              
    3c4:  6608           	bnes 0x3ce                             ; ↓ label_1

  label_0:
    3c6:  303c 290a      	movew #10506,%d0                       
    3ca:  6000 00f2      	braw 0x4be                             ; ↓ label_5

  label_1:
    3ce:  5392           	subql #1,%a2@                          
    3d0:  6708           	beqs 0x3da                             ; ↓ label_2
    3d2:  303c 290b      	movew #10507,%d0                       
    3d6:  6000 00e6      	braw 0x4be                             ; ↓ label_5

  label_2:
    3da:  4a92           	tstl %a2@                              
    3dc:  6600 00de      	bnew 0x4bc                             ; ↓ label_4
    3e0:  202a 0004      	movel %a2@(4),%d0                      
    3e4:  0280 0000 0100 	andil #256,%d0                         
    3ea:  670a           	beqs 0x3f6                             ; ↓ label_3
    3ec:  3f2a 0024      	movew %a2@(36),%sp@-                   
    3f0:  4e4f           	trap #15                               
    3f2:  a802           	0124002                                ; ??? 0xa802
    3f4:  544f           	addqw #2,%sp                           

  label_3:
    3f6:  4227           	clrb %sp@-                             
    3f8:  4eba 13d6      	jsr %pc@(0x17d0)                       ; MSSlotHwrSupplyPower
    3fc:  4eba 1398      	jsr %pc@(0x1796)                       ; MSSlotHwrClose
    400:  42a7           	clrl %sp@-                             
    402:  2f2a 000e      	movel %a2@(14),%sp@-                   
    406:  4e4f           	trap #15                               
    408:  a09a           	0120232                                ; sysTrapSysTimerWrite
    40a:  2f2a 000e      	movel %a2@(14),%sp@-                   
    40e:  4e4f           	trap #15                               
    410:  a0bf           	0120277                                ; sysTrapSysTimerDelete
    412:  486e fffa      	pea %fp@(-6)                           
    416:  486e fffe      	pea %fp@(-2)                           
    41a:  1f3c 0001      	moveb #1,%sp@-                         
    41e:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    424:  2f3c 6c69 6273 	movel #1818845811,%sp@-                
    42a:  486e ffda      	pea %fp@(-38)                          
    42e:  1f3c 0001      	moveb #1,%sp@-                         
    432:  4e4f           	trap #15                               
    434:  a078           	0120170                                ; sysTrapDmGetNextDatabaseByTypeCreator
    436:  1f3c fff6      	moveb #-10,%sp@-                       
    43a:  2f3c 6372 6469 	movel #1668441193,%sp@-                
    440:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    444:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    448:  4e4f           	trap #15                               
    44a:  a354           	0121524                                ; sysTrapSysNotifyUnregister
    44c:  1f3c fff6      	moveb #-10,%sp@-                       
    450:  2f3c 6372 646f 	movel #1668441199,%sp@-                
    456:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    45a:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    45e:  4e4f           	trap #15                               
    460:  a354           	0121524                                ; sysTrapSysNotifyUnregister
    462:  1f3c fff6      	moveb #-10,%sp@-                       
    466:  2f3c 766f 6c6d 	movel #1987013741,%sp@-                
    46c:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    470:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    474:  4e4f           	trap #15                               
    476:  a354           	0121524                                ; sysTrapSysNotifyUnregister
    478:  3f2a 0012      	movew %a2@(18),%sp@-                   
    47c:  7405           	moveq #5,%d2                           
    47e:  4e4f           	trap #15                               
    480:  a347           	0121507                                ; sysTrapExpansionDispatch
    482:  3f3c 0001      	movew #1,%sp@-                         
    486:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    48c:  4e4f           	trap #15                               
    48e:  a35b           	0121533                                ; sysTrapFtrPtrFree
    490:  3f3c 0002      	movew #2,%sp@-                         
    494:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    49a:  4e4f           	trap #15                               
    49c:  a35b           	0121533                                ; sysTrapFtrPtrFree
    49e:  2078 0304      	moveal 0x304,%a0                       
    4a2:  42a8 0024      	clrl %a0@(36)                          
    4a6:  3f2e 0008      	movew %fp@(8),%sp@-                    
    4aa:  4e4f           	trap #15                               
    4ac:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    4ae:  42a8 0004      	clrl %a0@(4)                           
    4b2:  2f0a           	movel %a2,%sp@-                        
    4b4:  4e4f           	trap #15                               
    4b6:  a012           	0120022                                ; sysTrapMemChunkFree
    4b8:  4fef 005e      	lea %sp@(94),%sp                       

  label_4:
    4bc:  7000           	moveq #0,%d0                           

  label_5:
    4be:  245f           	moveal %sp@+,%a2                       
    4c0:  4e5e           	unlk %fp                               
    4c2:  4e75           	rts                                    
```

# MSSlotSleep: 0x04d2 - 0x0530

Parameters:

   * `%fp@(8)`      : ???

```
    4d2:  4e56 0000      	linkw %fp,#0                           
    4d6:  2f0a           	movel %a2,%sp@-                        
    4d8:  3f2e 0008      	movew %fp@(8),%sp@-                    
    4dc:  4e4f           	trap #15                               
    4de:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    4e0:  2468 0004      	moveal %a0@(4),%a2                     
    4e4:  200a           	movel %a2,%d0                          
    4e6:  544f           	addqw #2,%sp                           
    4e8:  6606           	bnes 0x4f0                             ; ↓ label_0
    4ea:  303c 290a      	movew #10506,%d0                       
    4ee:  603c           	bras 0x52c                             ; ↓ label_2

  label_0:
    4f0:  4227           	clrb %sp@-                             
    4f2:  4eba 12dc      	jsr %pc@(0x17d0)                       ; MSSlotHwrSupplyPower
    4f6:  02aa ffff ff7f 	andil #-129,%a2@(4)                    
    4fc:  0004                                                   
    4fe:  42a7           	clrl %sp@-                             
    500:  2f2a 000e      	movel %a2@(14),%sp@-                   
    504:  4e4f           	trap #15                               
    506:  a09a           	0120232                                ; sysTrapSysTimerWrite
    508:  0c38 0001 02ca 	cmpib #1,0x2ca                         
    50e:  4fef 000a      	lea %sp@(10),%sp                       
    512:  6616           	bnes 0x52a                             ; ↓ label_1
    514:  7001           	moveq #1,%d0                           
    516:  c0aa 0004      	andl %a2@(4),%d0                       
    51a:  670e           	beqs 0x52a                             ; ↓ label_1
    51c:  7002           	moveq #2,%d0                           
    51e:  81aa 0004      	orl %d0,%a2@(4)                        
    522:  4227           	clrb %sp@-                             
    524:  4eba 13a8      	jsr %pc@(0x18ce)                       ; MSSlotHwrLightAccessLED
    528:  544f           	addqw #2,%sp                           

  label_1:
    52a:  7000           	moveq #0,%d0                           

  label_2:
    52c:  245f           	moveal %sp@+,%a2                       
    52e:  4e5e           	unlk %fp                               
    530:  4e75           	rts                                    
```

# MSSlotWake: 0x0540 - 0x05a8

Parameters:

   * `%fp@(8)`      : ???

```
    540:  4e56 0000      	linkw %fp,#0                           
    544:  2f0a           	movel %a2,%sp@-                        
    546:  3f2e 0008      	movew %fp@(8),%sp@-                    
    54a:  4e4f           	trap #15                               
    54c:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    54e:  2468 0004      	moveal %a0@(4),%a2                     
    552:  200a           	movel %a2,%d0                          
    554:  544f           	addqw #2,%sp                           
    556:  6606           	bnes 0x55e                             ; ↓ label_0
    558:  303c 290a      	movew #10506,%d0                       
    55c:  6046           	bras 0x5a4                             ; ↓ label_4

  label_0:
    55e:  7002           	moveq #2,%d0                           
    560:  c0aa 0004      	andl %a2@(4),%d0                       
    564:  6714           	beqs 0x57a                             ; ↓ label_1
    566:  70fd           	moveq #-3,%d0                          
    568:  c1aa 0004      	andl %d0,%a2@(4)                       
    56c:  7001           	moveq #1,%d0                           
    56e:  c0aa 0004      	andl %a2@(4),%d0                       
    572:  6706           	beqs 0x57a                             ; ↓ label_1
    574:  70f3           	moveq #-13,%d0                         
    576:  c1aa 0004      	andl %d0,%a2@(4)                       

  label_1:
    57a:  7008           	moveq #8,%d0                           
    57c:  c0aa 0004      	andl %a2@(4),%d0                       
    580:  660c           	bnes 0x58e                             ; ↓ label_2
    582:  3f2a 0012      	movew %a2@(18),%sp@-                   
    586:  7407           	moveq #7,%d2                           
    588:  4e4f           	trap #15                               
    58a:  a347           	0121507                                ; sysTrapExpansionDispatch
    58c:  544f           	addqw #2,%sp                           

  label_2:
    58e:  7004           	moveq #4,%d0                           
    590:  c0aa 0004      	andl %a2@(4),%d0                       
    594:  660c           	bnes 0x5a2                             ; ↓ label_3
    596:  3f2a 0012      	movew %a2@(18),%sp@-                   
    59a:  7406           	moveq #6,%d2                           
    59c:  4e4f           	trap #15                               
    59e:  a347           	0121507                                ; sysTrapExpansionDispatch
    5a0:  544f           	addqw #2,%sp                           

  label_3:
    5a2:  7000           	moveq #0,%d0                           

  label_4:
    5a4:  245f           	moveal %sp@+,%a2                       
    5a6:  4e5e           	unlk %fp                               
    5a8:  4e75           	rts                                    
```

# MSSlotGetVersion: 0x05b8 - 0x05c0

```
    5b8:  4e56 0000      	linkw %fp,#0                           
    5bc:  7001           	moveq #1,%d0                           
    5be:  4e5e           	unlk %fp                               
    5c0:  4e75           	rts                                    
```

# MSSlotExecCustomControl: 0x05d6 - 0x080c

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    5d6:  4e56 fffc      	linkw %fp,#-4                          
    5da:  48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-           
    5de:  382e 0008      	movew %fp@(8),%d4                      
    5e2:  266e 0010      	moveal %fp@(16),%a3                    
    5e6:  3f04           	movew %d4,%sp@-                        
    5e8:  4e4f           	trap #15                               
    5ea:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    5ec:  2468 0004      	moveal %a0@(4),%a2                     
    5f0:  200a           	movel %a2,%d0                          
    5f2:  544f           	addqw #2,%sp                           
    5f4:  6608           	bnes 0x5fe                             ; ↓ label_0
    5f6:  303c 290a      	movew #10506,%d0                       
    5fa:  6000 020a      	braw 0x806                             ; ↓ label_18

  label_0:
    5fe:  0cae 536f 4e79 	cmpil #1399803513,%fp@(10)             
    604:  000a                                                   
    606:  6708           	beqs 0x610                             ; ↓ label_1
    608:  303c 2901      	movew #10497,%d0                       
    60c:  6000 01f8      	braw 0x806                             ; ↓ label_18

  label_1:
    610:  302e 000e      	movew %fp@(14),%d0                     
    614:  0440 2000      	subiw #8192,%d0                        
    618:  0c40 0006      	cmpiw #6,%d0                           
    61c:  6200 01e2      	bhiw 0x800                             ; ↓ label_16
    620:  d040           	addw %d0,%d0                           
    622:  303b 0006      	movew %pc@(0x62a,%d0:w),%d0            
    626:  4efb 0002      	jmp %pc@(0x62a,%d0:w)                  
    62a:  000e 0018      	orib #24,%fp                           
    62e:  0088 00da 019a 	oril #14287258,%a0                     
    634:  01aa 01ba      	bclr %d0,%a2@(442)                     
    638:  7001           	moveq #1,%d0                           
    63a:  2680           	movel %d0,%a3@                         
    63c:  7600           	moveq #0,%d3                           
    63e:  6000 01c4      	braw 0x804                             ; ↓ label_17
    642:  302a 0012      	movew %a2@(18),%d0                     
    646:  b053           	cmpw %a3@,%d0                          
    648:  6708           	beqs 0x652                             ; ↓ label_2
    64a:  363c 2904      	movew #10500,%d3                       
    64e:  6000 01b4      	braw 0x804                             ; ↓ label_17

  label_2:
    652:  7001           	moveq #1,%d0                           
    654:  c0aa 0004      	andl %a2@(4),%d0                       
    658:  6608           	bnes 0x662                             ; ↓ label_3
    65a:  363c 2903      	movew #10499,%d3                       
    65e:  6000 01a4      	braw 0x804                             ; ↓ label_17

  label_3:
    662:  7008           	moveq #8,%d0                           
    664:  c0aa 0004      	andl %a2@(4),%d0                       
    668:  6608           	bnes 0x672                             ; ↓ label_4
    66a:  363c 2903      	movew #10499,%d3                       
    66e:  6000 0194      	braw 0x804                             ; ↓ label_17

  label_4:
    672:  202a 0004      	movel %a2@(4),%d0                      
    676:  0280 0303 0000 	andil #50528256,%d0                    
    67c:  7277           	moveq #119,%d1                         
    67e:  c2aa 0008      	andl %a2@(8),%d1                       
    682:  8280           	orl %d0,%d1                            
    684:  2741 0002      	movel %d1,%a3@(2)                      
    688:  206a 0020      	moveal %a2@(32),%a0                    
    68c:  2768 25b4 0006 	movel %a0@(9652),%a3@(6)               
    692:  206a 0020      	moveal %a2@(32),%a0                    
    696:  2768 25ac 000a 	movel %a0@(9644),%a3@(10)              
    69c:  206a 0020      	moveal %a2@(32),%a0                    
    6a0:  203c 0000 25ac 	movel #9644,%d0                        
    6a6:  2770 0804 000e 	movel %a0@(00000004,%d0:l),%a3@(14)    
    6ac:  7600           	moveq #0,%d3                           
    6ae:  6000 0154      	braw 0x804                             ; ↓ label_17
    6b2:  302a 0012      	movew %a2@(18),%d0                     
    6b6:  b053           	cmpw %a3@,%d0                          
    6b8:  6708           	beqs 0x6c2                             ; ↓ label_5
    6ba:  363c 2904      	movew #10500,%d3                       
    6be:  6000 0144      	braw 0x804                             ; ↓ label_17

  label_5:
    6c2:  7001           	moveq #1,%d0                           
    6c4:  c0aa 0004      	andl %a2@(4),%d0                       
    6c8:  6608           	bnes 0x6d2                             ; ↓ label_6
    6ca:  363c 2903      	movew #10499,%d3                       
    6ce:  6000 0134      	braw 0x804                             ; ↓ label_17

  label_6:
    6d2:  7008           	moveq #8,%d0                           
    6d4:  c0aa 0004      	andl %a2@(4),%d0                       
    6d8:  6608           	bnes 0x6e2                             ; ↓ label_7
    6da:  363c 2903      	movew #10499,%d3                       
    6de:  6000 0124      	braw 0x804                             ; ↓ label_17

  label_7:
    6e2:  7040           	moveq #64,%d0                          
    6e4:  c0aa 0008      	andl %a2@(8),%d0                       
    6e8:  6608           	bnes 0x6f2                             ; ↓ label_8
    6ea:  363c 2906      	movew #10502,%d3                       
    6ee:  6000 0114      	braw 0x804                             ; ↓ label_17

  label_8:
    6f2:  206a 0020      	moveal %a2@(32),%a0                    
    6f6:  41e8 2400      	lea %a0@(9216),%a0                     
    6fa:  2748 0002      	movel %a0,%a3@(2)                      
    6fe:  7600           	moveq #0,%d3                           
    700:  6000 0102      	braw 0x804                             ; ↓ label_17
    704:  302a 0012      	movew %a2@(18),%d0                     
    708:  b053           	cmpw %a3@,%d0                          
    70a:  6708           	beqs 0x714                             ; ↓ label_9
    70c:  363c 2904      	movew #10500,%d3                       
    710:  6000 00f2      	braw 0x804                             ; ↓ label_17

  label_9:
    714:  7001           	moveq #1,%d0                           
    716:  c0aa 0004      	andl %a2@(4),%d0                       
    71a:  6608           	bnes 0x724                             ; ↓ label_10
    71c:  363c 2903      	movew #10499,%d3                       
    720:  6000 00e2      	braw 0x804                             ; ↓ label_17

  label_10:
    724:  7008           	moveq #8,%d0                           
    726:  c0aa 0004      	andl %a2@(4),%d0                       
    72a:  6608           	bnes 0x734                             ; ↓ label_11
    72c:  363c 2903      	movew #10499,%d3                       
    730:  6000 00d2      	braw 0x804                             ; ↓ label_17

  label_11:
    734:  262b 000a      	movel %a3@(10),%d3                     
    738:  2043           	moveal %d3,%a0                         
    73a:  2d50 fffc      	movel %a0@,%fp@(-4)                    
    73e:  4290           	clrl %a0@                              
    740:  1f3c 0001      	moveb #1,%sp@-                         
    744:  4eba 1188      	jsr %pc@(0x18ce)                       ; MSSlotHwrLightAccessLED
    748:  1a00           	moveb %d0,%d5                          
    74a:  202a 0004      	movel %a2@(4),%d0                      
    74e:  0280 0000 0080 	andil #128,%d0                         
    754:  544f           	addqw #2,%sp                           
    756:  660e           	bnes 0x766                             ; ↓ label_12
    758:  2f0a           	movel %a2,%sp@-                        
    75a:  4eba 0ee0      	jsr %pc@(0x163c)                       ; PrvSupplyPower
    75e:  2f0a           	movel %a2,%sp@-                        
    760:  4eba 0e1a      	jsr %pc@(0x157c)                       ; PrvDoMgAuth
    764:  504f           	addqw #8,%sp                           

  label_12:
    766:  486e fffc      	pea %fp@(-4)                           
    76a:  2f2b 0006      	movel %a3@(6),%sp@-                    
    76e:  2f2b 0002      	movel %a3@(2),%sp@-                    
    772:  2f0a           	movel %a2,%sp@-                        
    774:  4eba 134e      	jsr %pc@(0x1ac4)                       ; PrvJobReadSector
    778:  2800           	movel %d0,%d4                          
    77a:  4fef 0010      	lea %sp@(16),%sp                       
    77e:  2004           	movel %d4,%d0                          
    780:  0c80 0000 0004 	cmpil #4,%d0                           
    786:  622a           	bhis 0x7b2                             ; ↓ label_13
    788:  d040           	addw %d0,%d0                           
    78a:  303b 0006      	movew %pc@(0x792,%d0:w),%d0            
    78e:  4efb 0002      	jmp %pc@(0x792,%d0:w)                  
    792:  000a 0020      	orib #32,%a2                           
    796:  0014 0020      	orib #32,%a4@                          
    79a:  001a 2043      	orib #67,%a2@+                         
    79e:  20ae fffc      	movel %fp@(-4),%a0@                    
    7a2:  7600           	moveq #0,%d3                           
    7a4:  6010           	bras 0x7b6                             ; ↓ label_14
    7a6:  363c 2903      	movew #10499,%d3                       
    7aa:  600a           	bras 0x7b6                             ; ↓ label_14
    7ac:  363c 0502      	movew #1282,%d3                        
    7b0:  6004           	bras 0x7b6                             ; ↓ label_14

  label_13:
    7b2:  363c 2908      	movew #10504,%d3                       

  label_14:
    7b6:  4a05           	tstb %d5                               
    7b8:  664a           	bnes 0x804                             ; ↓ label_17
    7ba:  4227           	clrb %sp@-                             
    7bc:  4eba 1110      	jsr %pc@(0x18ce)                       ; MSSlotHwrLightAccessLED
    7c0:  544f           	addqw #2,%sp                           
    7c2:  6040           	bras 0x804                             ; ↓ label_17
    7c4:  3f2b 0002      	movew %a3@(2),%sp@-                    
    7c8:  3f04           	movew %d4,%sp@-                        
    7ca:  4eba 0250      	jsr %pc@(0xa1c)                        ; MSSlotReserveCard
    7ce:  3600           	movew %d0,%d3                          
    7d0:  584f           	addqw #4,%sp                           
    7d2:  6030           	bras 0x804                             ; ↓ label_17
    7d4:  3f2b 0002      	movew %a3@(2),%sp@-                    
    7d8:  3f04           	movew %d4,%sp@-                        
    7da:  4eba 0294      	jsr %pc@(0xa70)                        ; MSSlotReleaseCard
    7de:  3600           	movew %d0,%d3                          
    7e0:  584f           	addqw #4,%sp                           
    7e2:  6020           	bras 0x804                             ; ↓ label_17
    7e4:  7000           	moveq #0,%d0                           
    7e6:  302a 0012      	movew %a2@(18),%d0                     
    7ea:  b093           	cmpl %a3@,%d0                          
    7ec:  6706           	beqs 0x7f4                             ; ↓ label_15
    7ee:  303c 2904      	movew #10500,%d0                       
    7f2:  6012           	bras 0x806                             ; ↓ label_18

  label_15:
    7f4:  4227           	clrb %sp@-                             
    7f6:  4eba 0b12      	jsr %pc@(0x130a)                       ; PrvInsRmvIntHandler
    7fa:  7600           	moveq #0,%d3                           
    7fc:  544f           	addqw #2,%sp                           
    7fe:  6004           	bras 0x804                             ; ↓ label_17

  label_16:
    800:  363c 2901      	movew #10497,%d3                       

  label_17:
    804:  3003           	movew %d3,%d0                          

  label_18:
    806:  4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3           
    80a:  4e5e           	unlk %fp                               
    80c:  4e75           	rts                                    
```

# MSSlotCheckCardPresence: 0x0828 - 0x084e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    828:  4e56 fffc      	linkw %fp,#-4                          
    82c:  2f03           	movel %d3,%sp@-                        
    82e:  486e fffc      	pea %fp@(-4)                           
    832:  3f2e 000a      	movew %fp@(10),%sp@-                   
    836:  3f2e 0008      	movew %fp@(8),%sp@-                    
    83a:  4eba 0c1c      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    83e:  3600           	movew %d0,%d3                          
    840:  504f           	addqw #8,%sp                           
    842:  6704           	beqs 0x848                             ; ↓ label_0
    844:  3003           	movew %d3,%d0                          
    846:  6002           	bras 0x84a                             ; ↓ label_1

  label_0:
    848:  7000           	moveq #0,%d0                           

  label_1:
    84a:  261f           	movel %sp@+,%d3                        
    84c:  4e5e           	unlk %fp                               
    84e:  4e75           	rts                                    
```

# MSSlotGetCardInfo: 0x086a - 0x0936

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    86a:  4e56 fffc      	linkw %fp,#-4                          
    86e:  2f0a           	movel %a2,%sp@-                        
    870:  2f03           	movel %d3,%sp@-                        
    872:  246e 000c      	moveal %fp@(12),%a2                    
    876:  486e fffc      	pea %fp@(-4)                           
    87a:  3f2e 000a      	movew %fp@(10),%sp@-                   
    87e:  3f2e 0008      	movew %fp@(8),%sp@-                    
    882:  4eba 0bd4      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    886:  3600           	movew %d0,%d3                          
    888:  504f           	addqw #8,%sp                           
    88a:  6706           	beqs 0x892                             ; ↓ label_0
    88c:  3003           	movew %d3,%d0                          
    88e:  6000 00a0      	braw 0x930                             ; ↓ label_5

  label_0:
    892:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    896:  4eba 0c2e      	jsr %pc@(0x14c6)                       ; PrvAssureMounted
    89a:  4292           	clrl %a2@                              
    89c:  7010           	moveq #16,%d0                          
    89e:  206e fffc      	moveal %fp@(-4),%a0                    
    8a2:  c0a8 0004      	andl %a0@(4),%d0                       
    8a6:  584f           	addqw #4,%sp                           
    8a8:  6734           	beqs 0x8de                             ; ↓ label_3
    8aa:  7001           	moveq #1,%d0                           
    8ac:  c0a8 0008      	andl %a0@(8),%d0                       
    8b0:  6704           	beqs 0x8b6                             ; ↓ label_1
    8b2:  7001           	moveq #1,%d0                           
    8b4:  8192           	orl %d0,%a2@                           

  label_1:
    8b6:  7030           	moveq #48,%d0                          
    8b8:  206e fffc      	moveal %fp@(-4),%a0                    
    8bc:  c0a8 0008      	andl %a0@(8),%d0                       
    8c0:  6704           	beqs 0x8c6                             ; ↓ label_2
    8c2:  7002           	moveq #2,%d0                           
    8c4:  8192           	orl %d0,%a2@                           

  label_2:
    8c6:  3f3c 001f      	movew #31,%sp@-                        
    8ca:  41fa 0080      	lea %pc@(0x94c),%a0                    ; ??? 0x94c
    8ce:  4850           	pea %a0@                               
    8d0:  486a 0044      	pea %a2@(68)                           
    8d4:  4e4f           	trap #15                               
    8d6:  a2ce           	0121316                                ; sysTrapStrNCopy
    8d8:  4fef 000a      	lea %sp@(10),%sp                       
    8dc:  6016           	bras 0x8f4                             ; ↓ label_4

  label_3:
    8de:  3f3c 001f      	movew #31,%sp@-                        
    8e2:  41fa 0076      	lea %pc@(0x95a),%a0                    ; ??? 0x95a
    8e6:  4850           	pea %a0@                               
    8e8:  486a 0044      	pea %a2@(68)                           
    8ec:  4e4f           	trap #15                               
    8ee:  a2ce           	0121316                                ; sysTrapStrNCopy
    8f0:  4fef 000a      	lea %sp@(10),%sp                       

  label_4:
    8f4:  3f3c 001f      	movew #31,%sp@-                        
    8f8:  41fa 0062      	lea %pc@(0x95c),%a0                    ; ??? 0x95c
    8fc:  4850           	pea %a0@                               
    8fe:  486a 0004      	pea %a2@(4)                            
    902:  4e4f           	trap #15                               
    904:  a2ce           	0121316                                ; sysTrapStrNCopy
    906:  3f3c 001f      	movew #31,%sp@-                        
    90a:  41fa 0052      	lea %pc@(0x95e),%a0                    ; ??? 0x95e
    90e:  4850           	pea %a0@                               
    910:  486a 0024      	pea %a2@(36)                           
    914:  4e4f           	trap #15                               
    916:  a2ce           	0121316                                ; sysTrapStrNCopy
    918:  3f3c 001f      	movew #31,%sp@-                        
    91c:  41fa 0042      	lea %pc@(0x960),%a0                    ; ??? 0x960
    920:  4850           	pea %a0@                               
    922:  486a 0064      	pea %a2@(100)                          
    926:  4e4f           	trap #15                               
    928:  a2ce           	0121316                                ; sysTrapStrNCopy
    92a:  7000           	moveq #0,%d0                           
    92c:  4fef 001e      	lea %sp@(30),%sp                       

  label_5:
    930:  261f           	movel %sp@+,%d3                        
    932:  245f           	moveal %sp@+,%a2                       
    934:  4e5e           	unlk %fp                               
    936:  4e75           	rts                                    
```

# MSSlotGetCardMediaType: 0x0962 - 0x099e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    962:  4e56 fffc      	linkw %fp,#-4                          
    966:  2f03           	movel %d3,%sp@-                        
    968:  486e fffc      	pea %fp@(-4)                           
    96c:  3f2e 000a      	movew %fp@(10),%sp@-                   
    970:  3f2e 0008      	movew %fp@(8),%sp@-                    
    974:  4eba 0ae2      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    978:  3600           	movew %d0,%d3                          
    97a:  504f           	addqw #8,%sp                           
    97c:  6704           	beqs 0x982                             ; ↓ label_0
    97e:  3003           	movew %d3,%d0                          
    980:  6018           	bras 0x99a                             ; ↓ label_2

  label_0:
    982:  4aae 000c      	tstl %fp@(12)                          
    986:  670e           	beqs 0x996                             ; ↓ label_1
    988:  206e 000c      	moveal %fp@(12),%a0                    
    98c:  20bc 6d73 746b 	movel #1836282987,%a0@                 
    992:  7000           	moveq #0,%d0                           
    994:  6004           	bras 0x99a                             ; ↓ label_2

  label_1:
    996:  303c 0502      	movew #1282,%d0                        

  label_2:
    99a:  261f           	movel %sp@+,%d3                        
    99c:  4e5e           	unlk %fp                               
    99e:  4e75           	rts                                    
```

# MSSlotGetMediaType: 0x09ba - 0x0a04

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

```
    9ba:  4e56 0000      	linkw %fp,#0                           
    9be:  2f0a           	movel %a2,%sp@-                        
    9c0:  3f2e 0008      	movew %fp@(8),%sp@-                    
    9c4:  4e4f           	trap #15                               
    9c6:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    9c8:  2468 0004      	moveal %a0@(4),%a2                     
    9cc:  200a           	movel %a2,%d0                          
    9ce:  544f           	addqw #2,%sp                           
    9d0:  6606           	bnes 0x9d8                             ; ↓ label_0
    9d2:  303c 290a      	movew #10506,%d0                       
    9d6:  6028           	bras 0xa00                             ; ↓ label_3

  label_0:
    9d8:  302a 0012      	movew %a2@(18),%d0                     
    9dc:  b06e 000a      	cmpw %fp@(10),%d0                      
    9e0:  6706           	beqs 0x9e8                             ; ↓ label_1
    9e2:  303c 2904      	movew #10500,%d0                       
    9e6:  6018           	bras 0xa00                             ; ↓ label_3

  label_1:
    9e8:  4aae 000c      	tstl %fp@(12)                          
    9ec:  670e           	beqs 0x9fc                             ; ↓ label_2
    9ee:  206e 000c      	moveal %fp@(12),%a0                    
    9f2:  20bc 6d73 746b 	movel #1836282987,%a0@                 
    9f8:  7000           	moveq #0,%d0                           
    9fa:  6004           	bras 0xa00                             ; ↓ label_3

  label_2:
    9fc:  303c 0502      	movew #1282,%d0                        

  label_3:
    a00:  245f           	moveal %sp@+,%a2                       
    a02:  4e5e           	unlk %fp                               
    a04:  4e75           	rts                                    
```

# MSSlotReserveCard: 0x0a1c - 0x0a5a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    a1c:  4e56 fffc      	linkw %fp,#-4                          
    a20:  2f03           	movel %d3,%sp@-                        
    a22:  4227           	clrb %sp@-                             
    a24:  4e4f           	trap #15                               
    a26:  a0c3           	0120303                                ; sysTrapSysTaskSwitching
    a28:  486e fffc      	pea %fp@(-4)                           
    a2c:  3f2e 000a      	movew %fp@(10),%sp@-                   
    a30:  3f2e 0008      	movew %fp@(8),%sp@-                    
    a34:  4eba 0a22      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    a38:  3600           	movew %d0,%d3                          
    a3a:  4fef 000a      	lea %sp@(10),%sp                       
    a3e:  660a           	bnes 0xa4a                             ; ↓ label_0
    a40:  206e fffc      	moveal %fp@(-4),%a0                    
    a44:  7040           	moveq #64,%d0                          
    a46:  81a8 0004      	orl %d0,%a0@(4)                        

  label_0:
    a4a:  1f3c 0001      	moveb #1,%sp@-                         
    a4e:  4e4f           	trap #15                               
    a50:  a0c3           	0120303                                ; sysTrapSysTaskSwitching
    a52:  3003           	movew %d3,%d0                          
    a54:  544f           	addqw #2,%sp                           
    a56:  261f           	movel %sp@+,%d3                        
    a58:  4e5e           	unlk %fp                               
    a5a:  4e75           	rts                                    
```

# MSSlotReleaseCard: 0x0a70 - 0x0ad0

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    a70:  4e56 fffc      	linkw %fp,#-4                          
    a74:  2f03           	movel %d3,%sp@-                        
    a76:  4227           	clrb %sp@-                             
    a78:  4e4f           	trap #15                               
    a7a:  a0c3           	0120303                                ; sysTrapSysTaskSwitching
    a7c:  486e fffc      	pea %fp@(-4)                           
    a80:  3f2e 000a      	movew %fp@(10),%sp@-                   
    a84:  3f2e 0008      	movew %fp@(8),%sp@-                    
    a88:  4eba 09ce      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    a8c:  3600           	movew %d0,%d3                          
    a8e:  4fef 000a      	lea %sp@(10),%sp                       
    a92:  662c           	bnes 0xac0                             ; ↓ label_0
    a94:  206e fffc      	moveal %fp@(-4),%a0                    
    a98:  70bf           	moveq #-65,%d0                         
    a9a:  c1a8 0004      	andl %d0,%a0@(4)                       
    a9e:  42a7           	clrl %sp@-                             
    aa0:  206e fffc      	moveal %fp@(-4),%a0                    
    aa4:  2f28 000e      	movel %a0@(14),%sp@-                   
    aa8:  4e4f           	trap #15                               
    aaa:  a09a           	0120232                                ; sysTrapSysTimerWrite
    aac:  4878 0064      	pea 0x64                               
    ab0:  206e fffc      	moveal %fp@(-4),%a0                    
    ab4:  2f28 000e      	movel %a0@(14),%sp@-                   
    ab8:  4e4f           	trap #15                               
    aba:  a09a           	0120232                                ; sysTrapSysTimerWrite
    abc:  4fef 0010      	lea %sp@(16),%sp                       

  label_0:
    ac0:  1f3c 0001      	moveb #1,%sp@-                         
    ac4:  4e4f           	trap #15                               
    ac6:  a0c3           	0120303                                ; sysTrapSysTaskSwitching
    ac8:  3003           	movew %d3,%d0                          
    aca:  544f           	addqw #2,%sp                           
    acc:  261f           	movel %sp@+,%d3                        
    ace:  4e5e           	unlk %fp                               
    ad0:  4e75           	rts                                    
```

# MSSlotCheckFSSupport: 0x0ae6 - 0x0b30

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

```
    ae6:  4e56 0000      	linkw %fp,#0                           
    aea:  2f0a           	movel %a2,%sp@-                        
    aec:  3f2e 0008      	movew %fp@(8),%sp@-                    
    af0:  4e4f           	trap #15                               
    af2:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    af4:  2468 0004      	moveal %a0@(4),%a2                     
    af8:  200a           	movel %a2,%d0                          
    afa:  544f           	addqw #2,%sp                           
    afc:  6604           	bnes 0xb02                             ; ↓ label_0
    afe:  7000           	moveq #0,%d0                           
    b00:  602a           	bras 0xb2c                             ; ↓ label_4

  label_0:
    b02:  302a 0012      	movew %a2@(18),%d0                     
    b06:  b06e 000a      	cmpw %fp@(10),%d0                      
    b0a:  6704           	beqs 0xb10                             ; ↓ label_1
    b0c:  7000           	moveq #0,%d0                           
    b0e:  601c           	bras 0xb2c                             ; ↓ label_4

  label_1:
    b10:  202e 000c      	movel %fp@(12),%d0                     
    b14:  0480 6661 7473 	subil #1717662835,%d0                  
    b1a:  670a           	beqs 0xb26                             ; ↓ label_2
    b1c:  0480 1004 ed01 	subil #268758273,%d0                   
    b22:  6702           	beqs 0xb26                             ; ↓ label_2
    b24:  6004           	bras 0xb2a                             ; ↓ label_3

  label_2:
    b26:  7001           	moveq #1,%d0                           
    b28:  6002           	bras 0xb2c                             ; ↓ label_4

  label_3:
    b2a:  7000           	moveq #0,%d0                           

  label_4:
    b2c:  245f           	moveal %sp@+,%a2                       
    b2e:  4e5e           	unlk %fp                               
    b30:  4e75           	rts                                    
```

# MSSlotGetCardMetrics: 0x0b4a - 0x0cfe

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    b4a:  4e56 fffc      	linkw %fp,#-4                          
    b4e:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-               
    b52:  246e 000c      	moveal %fp@(12),%a2                    
    b56:  486e fffc      	pea %fp@(-4)                           
    b5a:  3f2e 000a      	movew %fp@(10),%sp@-                   
    b5e:  3f2e 0008      	movew %fp@(8),%sp@-                    
    b62:  4eba 08f4      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    b66:  3600           	movew %d0,%d3                          
    b68:  504f           	addqw #8,%sp                           
    b6a:  6706           	beqs 0xb72                             ; ↓ label_0
    b6c:  3003           	movew %d3,%d0                          
    b6e:  6000 0188      	braw 0xcf8                             ; ↓ label_14

  label_0:
    b72:  200a           	movel %a2,%d0                          
    b74:  6700 017c      	beqw 0xcf2                             ; ↓ label_12
    b78:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    b7c:  4eba 0948      	jsr %pc@(0x14c6)                       ; PrvAssureMounted
    b80:  7001           	moveq #1,%d0                           
    b82:  206e fffc      	moveal %fp@(-4),%a0                    
    b86:  c0a8 0008      	andl %a0@(8),%d0                       
    b8a:  584f           	addqw #4,%sp                           
    b8c:  6700 015e      	beqw 0xcec                             ; ↓ label_11
    b90:  7001           	moveq #1,%d0                           
    b92:  2480           	movel %d0,%a2@                         
    b94:  357c 0200 0008 	movew #512,%a2@(8)                     
    b9a:  422a 000b      	clrb %a2@(11)                          
    b9e:  357c 0010 000c 	movew #16,%a2@(12)                     
    ba4:  7001           	moveq #1,%d0                           
    ba6:  2540 0010      	movel %d0,%a2@(16)                     
    baa:  4878 0008      	pea 0x8                                
    bae:  4e4f           	trap #15                               
    bb0:  a013           	0120023                                ; sysTrapMemPtrNew
    bb2:  2548 0014      	movel %a0,%a2@(20)                     
    bb6:  266a 0014      	moveal %a2@(20),%a3                    
    bba:  200b           	movel %a3,%d0                          
    bbc:  584f           	addqw #4,%sp                           
    bbe:  6608           	bnes 0xbc8                             ; ↓ label_1
    bc0:  363c 0102      	movew #258,%d3                         
    bc4:  6000 0130      	braw 0xcf6                             ; ↓ label_13

  label_1:
    bc8:  4267           	clrw %sp@-                             
    bca:  2f0b           	movel %a3,%sp@-                        
    bcc:  4e4f           	trap #15                               
    bce:  a01b           	0120033                                ; sysTrapMemPtrSetOwner
    bd0:  206a 0014      	moveal %a2@(20),%a0                    
    bd4:  7001           	moveq #1,%d0                           
    bd6:  2080           	movel %d0,%a0@                         
    bd8:  206e fffc      	moveal %fp@(-4),%a0                    
    bdc:  2668 0020      	moveal %a0@(32),%a3                    
    be0:  302b 25a4      	movew %a3@(9636),%d0                   
    be4:  5c4f           	addqw #6,%sp                           
    be6:  0440 0200      	subiw #512,%d0                         
    bea:  6722           	beqs 0xc0e                             ; ↓ label_2
    bec:  0440 0200      	subiw #512,%d0                         
    bf0:  673e           	beqs 0xc30                             ; ↓ label_3
    bf2:  0440 0400      	subiw #1024,%d0                        
    bf6:  6700 008e      	beqw 0xc86                             ; ↓ label_7
    bfa:  0440 0800      	subiw #2048,%d0                        
    bfe:  6700 00a6      	beqw 0xca6                             ; ↓ label_8
    c02:  0440 1000      	subiw #4096,%d0                        
    c06:  6700 00be      	beqw 0xcc6                             ; ↓ label_9
    c0a:  6000 00da      	braw 0xce6                             ; ↓ label_10

  label_2:
    c0e:  257c 0000 1ec0 	movel #7872,%a2@(4)                    
    c14:  0004                                                   
    c16:  157c 0010 000a 	moveb #16,%a2@(10)                     
    c1c:  357c 0002 000e 	movew #2,%a2@(14)                      
    c22:  206a 0014      	moveal %a2@(20),%a0                    
    c26:  701a           	moveq #26,%d0                          
    c28:  2140 0004      	movel %d0,%a0@(4)                      
    c2c:  6000 00c8      	braw 0xcf6                             ; ↓ label_13

  label_3:
    c30:  302b 25a2      	movew %a3@(9634),%d0                   
    c34:  5140           	subqw #8,%d0                           
    c36:  6706           	beqs 0xc3e                             ; ↓ label_4
    c38:  5140           	subqw #8,%d0                           
    c3a:  6724           	beqs 0xc60                             ; ↓ label_5
    c3c:  6042           	bras 0xc80                             ; ↓ label_6

  label_4:
    c3e:  257c 0000 3dc0 	movel #15808,%a2@(4)                   
    c44:  0004                                                   
    c46:  157c 0010 000a 	moveb #16,%a2@(10)                     
    c4c:  357c 0002 000e 	movew #2,%a2@(14)                      
    c52:  206a 0014      	moveal %a2@(20),%a0                    
    c56:  7018           	moveq #24,%d0                          
    c58:  2140 0004      	movel %d0,%a0@(4)                      
    c5c:  6000 0098      	braw 0xcf6                             ; ↓ label_13

  label_5:
    c60:  257c 0000 7b80 	movel #31616,%a2@(4)                   
    c66:  0004                                                   
    c68:  157c 0020 000a 	moveb #32,%a2@(10)                     
    c6e:  357c 0004 000e 	movew #4,%a2@(14)                      
    c74:  206a 0014      	moveal %a2@(20),%a0                    
    c78:  7018           	moveq #24,%d0                          
    c7a:  2140 0004      	movel %d0,%a0@(4)                      
    c7e:  6076           	bras 0xcf6                             ; ↓ label_13

  label_6:
    c80:  363c 2906      	movew #10502,%d3                       
    c84:  6070           	bras 0xcf6                             ; ↓ label_13

  label_7:
    c86:  257c 0000 f780 	movel #63360,%a2@(4)                   
    c8c:  0004                                                   
    c8e:  157c 0020 000a 	moveb #32,%a2@(10)                     
    c94:  357c 0004 000e 	movew #4,%a2@(14)                      
    c9a:  206a 0014      	moveal %a2@(20),%a0                    
    c9e:  7012           	moveq #18,%d0                          
    ca0:  2140 0004      	movel %d0,%a0@(4)                      
    ca4:  6050           	bras 0xcf6                             ; ↓ label_13

  label_8:
    ca6:  257c 0001 ef00 	movel #126720,%a2@(4)                  
    cac:  0004                                                   
    cae:  157c 0020 000a 	moveb #32,%a2@(10)                     
    cb4:  357c 0008 000e 	movew #8,%a2@(14)                      
    cba:  206a 0014      	moveal %a2@(20),%a0                    
    cbe:  7026           	moveq #38,%d0                          
    cc0:  2140 0004      	movel %d0,%a0@(4)                      
    cc4:  6030           	bras 0xcf6                             ; ↓ label_13

  label_9:
    cc6:  257c 0003 de00 	movel #253440,%a2@(4)                  
    ccc:  0004                                                   
    cce:  157c 0020 000a 	moveb #32,%a2@(10)                     
    cd4:  357c 0010 000e 	movew #16,%a2@(14)                     
    cda:  206a 0014      	moveal %a2@(20),%a0                    
    cde:  7020           	moveq #32,%d0                          
    ce0:  2140 0004      	movel %d0,%a0@(4)                      
    ce4:  6010           	bras 0xcf6                             ; ↓ label_13

  label_10:
    ce6:  363c 2906      	movew #10502,%d3                       
    cea:  600a           	bras 0xcf6                             ; ↓ label_13

  label_11:
    cec:  363c 2906      	movew #10502,%d3                       
    cf0:  6004           	bras 0xcf6                             ; ↓ label_13

  label_12:
    cf2:  363c 0502      	movew #1282,%d3                        

  label_13:
    cf6:  3003           	movew %d3,%d0                          

  label_14:
    cf8:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3               
    cfc:  4e5e           	unlk %fp                               
    cfe:  4e75           	rts                                    
```

# MSSlotFormat: 0x0d18 - 0x0e66

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    d18:  4e56 fffc      	linkw %fp,#-4                          
    d1c:  2f04           	movel %d4,%sp@-                        
    d1e:  2f03           	movel %d3,%sp@-                        
    d20:  486e fffc      	pea %fp@(-4)                           
    d24:  3f2e 000a      	movew %fp@(10),%sp@-                   
    d28:  3f2e 0008      	movew %fp@(8),%sp@-                    
    d2c:  4eba 072a      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    d30:  3600           	movew %d0,%d3                          
    d32:  504f           	addqw #8,%sp                           
    d34:  6706           	beqs 0xd3c                             ; ↓ label_0
    d36:  3003           	movew %d3,%d0                          
    d38:  6000 0126      	braw 0xe60                             ; ↓ label_10

  label_0:
    d3c:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    d40:  4eba 0784      	jsr %pc@(0x14c6)                       ; PrvAssureMounted
    d44:  7001           	moveq #1,%d0                           
    d46:  206e fffc      	moveal %fp@(-4),%a0                    
    d4a:  2628 0008      	movel %a0@(8),%d3                      
    d4e:  c083           	andl %d3,%d0                           
    d50:  584f           	addqw #4,%sp                           
    d52:  6700 0106      	beqw 0xe5a                             ; ↓ label_8
    d56:  7010           	moveq #16,%d0                          
    d58:  c083           	andl %d3,%d0                           
    d5a:  6606           	bnes 0xd62                             ; ↓ label_1
    d5c:  7020           	moveq #32,%d0                          
    d5e:  c083           	andl %d3,%d0                           
    d60:  6708           	beqs 0xd6a                             ; ↓ label_2

  label_1:
    d62:  303c 2907      	movew #10503,%d0                       
    d66:  6000 00f8      	braw 0xe60                             ; ↓ label_10

  label_2:
    d6a:  42a7           	clrl %sp@-                             
    d6c:  206e fffc      	moveal %fp@(-4),%a0                    
    d70:  2f28 000e      	movel %a0@(14),%sp@-                   
    d74:  4e4f           	trap #15                               
    d76:  a09a           	0120232                                ; sysTrapSysTimerWrite
    d78:  1f3c 0001      	moveb #1,%sp@-                         
    d7c:  4eba 0b50      	jsr %pc@(0x18ce)                       ; MSSlotHwrLightAccessLED
    d80:  1800           	moveb %d0,%d4                          
    d82:  206e fffc      	moveal %fp@(-4),%a0                    
    d86:  2028 0004      	movel %a0@(4),%d0                      
    d8a:  0280 0000 0080 	andil #128,%d0                         
    d90:  4fef 000a      	lea %sp@(10),%sp                       
    d94:  6612           	bnes 0xda8                             ; ↓ label_3
    d96:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    d9a:  4eba 08a0      	jsr %pc@(0x163c)                       ; PrvSupplyPower
    d9e:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    da2:  4eba 07d8      	jsr %pc@(0x157c)                       ; PrvDoMgAuth
    da6:  504f           	addqw #8,%sp                           

  label_3:
    da8:  206e fffc      	moveal %fp@(-4),%a0                    
    dac:  02a8 feff ffff 	andil #-16777217,%a0@(4)               
    db2:  0004                                                   
    db4:  206e fffc      	moveal %fp@(-4),%a0                    
    db8:  02a8 fdff ffff 	andil #-33554433,%a0@(4)               
    dbe:  0004                                                   
    dc0:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    dc4:  4eba 0f64      	jsr %pc@(0x1d2a)                       ; PrvJobFormat
    dc8:  2600           	movel %d0,%d3                          
    dca:  584f           	addqw #4,%sp                           
    dcc:  2003           	movel %d3,%d0                          
    dce:  0c80 0000 0007 	cmpil #7,%d0                           
    dd4:  6256           	bhis 0xe2c                             ; ↓ label_5
    dd6:  d040           	addw %d0,%d0                           
    dd8:  303b 0006      	movew %pc@(0xde0,%d0:w),%d0            
    ddc:  4efb 0002      	jmp %pc@(0xde0,%d0:w)                  
    de0:  0010 004c      	orib #76,%a0@                          
    de4:  003a 004c 004c 	orib #76,%pc@(0xe34)                   ; ??? 0xe34
    dea:  004c 0040      	oriw #64,%a4                           
    dee:  0046 7002      	oriw #28674,%d6                        
    df2:  206e fffc      	moveal %fp@(-4),%a0                    
    df6:  c0a8 0008      	andl %a0@(8),%d0                       
    dfa:  671a           	beqs 0xe16                             ; ↓ label_4
    dfc:  2028 0004      	movel %a0@(4),%d0                      
    e00:  0280 0000 0100 	andil #256,%d0                         
    e06:  670e           	beqs 0xe16                             ; ↓ label_4
    e08:  3f3c ffff      	movew #-1,%sp@-                        
    e0c:  3f28 0024      	movew %a0@(36),%sp@-                   
    e10:  4e4f           	trap #15                               
    e12:  a80a           	0124012                                ; ??? 0xa80a
    e14:  584f           	addqw #4,%sp                           

  label_4:
    e16:  7600           	moveq #0,%d3                           
    e18:  6020           	bras 0xe3a                             ; ↓ label_6
    e1a:  363c 2903      	movew #10499,%d3                       
    e1e:  601a           	bras 0xe3a                             ; ↓ label_6
    e20:  363c 2907      	movew #10503,%d3                       
    e24:  6014           	bras 0xe3a                             ; ↓ label_6
    e26:  363c 0102      	movew #258,%d3                         
    e2a:  600e           	bras 0xe3a                             ; ↓ label_6

  label_5:
    e2c:  206e fffc      	moveal %fp@(-4),%a0                    
    e30:  70fe           	moveq #-2,%d0                          
    e32:  c1a8 0008      	andl %d0,%a0@(8)                       
    e36:  363c 2908      	movew #10504,%d3                       

  label_6:
    e3a:  4a04           	tstb %d4                               
    e3c:  6608           	bnes 0xe46                             ; ↓ label_7
    e3e:  4227           	clrb %sp@-                             
    e40:  4eba 0a8c      	jsr %pc@(0x18ce)                       ; MSSlotHwrLightAccessLED
    e44:  544f           	addqw #2,%sp                           

  label_7:
    e46:  4878 0064      	pea 0x64                               
    e4a:  206e fffc      	moveal %fp@(-4),%a0                    
    e4e:  2f28 000e      	movel %a0@(14),%sp@-                   
    e52:  4e4f           	trap #15                               
    e54:  a09a           	0120232                                ; sysTrapSysTimerWrite
    e56:  504f           	addqw #8,%sp                           
    e58:  6004           	bras 0xe5e                             ; ↓ label_9

  label_8:
    e5a:  363c 2906      	movew #10502,%d3                       

  label_9:
    e5e:  3003           	movew %d3,%d0                          

  label_10:
    e60:  261f           	movel %sp@+,%d3                        
    e62:  281f           	movel %sp@+,%d4                        
    e64:  4e5e           	unlk %fp                               
    e66:  4e75           	rts                                    
```

# MSSlotReadSector: 0x0e78 - 0x0f8e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???

```
    e78:  4e56 fff8      	linkw %fp,#-8                          
    e7c:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-               
    e80:  246e 0014      	moveal %fp@(20),%a2                    
    e84:  2d52 fff8      	movel %a2@,%fp@(-8)                    
    e88:  4292           	clrl %a2@                              
    e8a:  486e fffc      	pea %fp@(-4)                           
    e8e:  3f2e 000a      	movew %fp@(10),%sp@-                   
    e92:  3f2e 0008      	movew %fp@(8),%sp@-                    
    e96:  4eba 05c0      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    e9a:  3600           	movew %d0,%d3                          
    e9c:  504f           	addqw #8,%sp                           
    e9e:  6706           	beqs 0xea6                             ; ↓ label_0
    ea0:  3003           	movew %d3,%d0                          
    ea2:  6000 00e4      	braw 0xf88                             ; ↓ label_8

  label_0:
    ea6:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    eaa:  4eba 061a      	jsr %pc@(0x14c6)                       ; PrvAssureMounted
    eae:  7010           	moveq #16,%d0                          
    eb0:  206e fffc      	moveal %fp@(-4),%a0                    
    eb4:  c0a8 0004      	andl %a0@(4),%d0                       
    eb8:  584f           	addqw #4,%sp                           
    eba:  6700 00c6      	beqw 0xf82                             ; ↓ label_6
    ebe:  7001           	moveq #1,%d0                           
    ec0:  c0a8 0008      	andl %a0@(8),%d0                       
    ec4:  6700 00bc      	beqw 0xf82                             ; ↓ label_6
    ec8:  4aae fff8      	tstl %fp@(-8)                          
    ecc:  6606           	bnes 0xed4                             ; ↓ label_1
    ece:  7000           	moveq #0,%d0                           
    ed0:  6000 00b6      	braw 0xf88                             ; ↓ label_8

  label_1:
    ed4:  42a7           	clrl %sp@-                             
    ed6:  206e fffc      	moveal %fp@(-4),%a0                    
    eda:  2f28 000e      	movel %a0@(14),%sp@-                   
    ede:  4e4f           	trap #15                               
    ee0:  a09a           	0120232                                ; sysTrapSysTimerWrite
    ee2:  1f3c 0001      	moveb #1,%sp@-                         
    ee6:  4eba 09e6      	jsr %pc@(0x18ce)                       ; MSSlotHwrLightAccessLED
    eea:  1800           	moveb %d0,%d4                          
    eec:  206e fffc      	moveal %fp@(-4),%a0                    
    ef0:  2028 0004      	movel %a0@(4),%d0                      
    ef4:  0280 0000 0080 	andil #128,%d0                         
    efa:  4fef 000a      	lea %sp@(10),%sp                       
    efe:  6612           	bnes 0xf12                             ; ↓ label_2
    f00:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    f04:  4eba 0736      	jsr %pc@(0x163c)                       ; PrvSupplyPower
    f08:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    f0c:  4eba 066e      	jsr %pc@(0x157c)                       ; PrvDoMgAuth
    f10:  504f           	addqw #8,%sp                           

  label_2:
    f12:  486e fff8      	pea %fp@(-8)                           
    f16:  2f2e 0010      	movel %fp@(16),%sp@-                   
    f1a:  2f2e 000c      	movel %fp@(12),%sp@-                   
    f1e:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    f22:  4eba 0ba0      	jsr %pc@(0x1ac4)                       ; PrvJobReadSector
    f26:  2600           	movel %d0,%d3                          
    f28:  4fef 0010      	lea %sp@(16),%sp                       
    f2c:  2003           	movel %d3,%d0                          
    f2e:  0c80 0000 0004 	cmpil #4,%d0                           
    f34:  6228           	bhis 0xf5e                             ; ↓ label_3
    f36:  d040           	addw %d0,%d0                           
    f38:  303b 0006      	movew %pc@(0xf40,%d0:w),%d0            
    f3c:  4efb 0002      	jmp %pc@(0xf40,%d0:w)                  
    f40:  000a 001e      	orib #30,%a2                           
    f44:  0012 001e      	orib #30,%a2@                          
    f48:  0018 24ae      	orib #-82,%a0@+                        
    f4c:  fff8           	0177770                                
    f4e:  7600           	moveq #0,%d3                           
    f50:  6010           	bras 0xf62                             ; ↓ label_4
    f52:  363c 2903      	movew #10499,%d3                       
    f56:  600a           	bras 0xf62                             ; ↓ label_4
    f58:  363c 0502      	movew #1282,%d3                        
    f5c:  6004           	bras 0xf62                             ; ↓ label_4

  label_3:
    f5e:  363c 2908      	movew #10504,%d3                       

  label_4:
    f62:  4a04           	tstb %d4                               
    f64:  6608           	bnes 0xf6e                             ; ↓ label_5
    f66:  4227           	clrb %sp@-                             
    f68:  4eba 0964      	jsr %pc@(0x18ce)                       ; MSSlotHwrLightAccessLED
    f6c:  544f           	addqw #2,%sp                           

  label_5:
    f6e:  4878 0064      	pea 0x64                               
    f72:  206e fffc      	moveal %fp@(-4),%a0                    
    f76:  2f28 000e      	movel %a0@(14),%sp@-                   
    f7a:  4e4f           	trap #15                               
    f7c:  a09a           	0120232                                ; sysTrapSysTimerWrite
    f7e:  504f           	addqw #8,%sp                           
    f80:  6004           	bras 0xf86                             ; ↓ label_7

  label_6:
    f82:  363c 2906      	movew #10502,%d3                       

  label_7:
    f86:  3003           	movew %d3,%d0                          

  label_8:
    f88:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2               
    f8c:  4e5e           	unlk %fp                               
    f8e:  4e75           	rts                                    
```

# MSSlotWriteSector: 0x0fa4 - 0x10da

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???

```
    fa4:  4e56 fff8      	linkw %fp,#-8                          
    fa8:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-               
    fac:  246e 0014      	moveal %fp@(20),%a2                    
    fb0:  2d52 fff8      	movel %a2@,%fp@(-8)                    
    fb4:  4292           	clrl %a2@                              
    fb6:  486e fffc      	pea %fp@(-4)                           
    fba:  3f2e 000a      	movew %fp@(10),%sp@-                   
    fbe:  3f2e 0008      	movew %fp@(8),%sp@-                    
    fc2:  4eba 0494      	jsr %pc@(0x1458)                       ; PrvDoPreCheck
    fc6:  3600           	movew %d0,%d3                          
    fc8:  504f           	addqw #8,%sp                           
    fca:  6706           	beqs 0xfd2                             ; ↓ label_0
    fcc:  3003           	movew %d3,%d0                          
    fce:  6000 0104      	braw 0x10d4                            ; ↓ label_10

  label_0:
    fd2:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    fd6:  4eba 04ee      	jsr %pc@(0x14c6)                       ; PrvAssureMounted
    fda:  7010           	moveq #16,%d0                          
    fdc:  206e fffc      	moveal %fp@(-4),%a0                    
    fe0:  c0a8 0004      	andl %a0@(4),%d0                       
    fe4:  584f           	addqw #4,%sp                           
    fe6:  6700 00e6      	beqw 0x10ce                            ; ↓ label_8
    fea:  7001           	moveq #1,%d0                           
    fec:  2628 0008      	movel %a0@(8),%d3                      
    ff0:  c083           	andl %d3,%d0                           
    ff2:  6700 00da      	beqw 0x10ce                            ; ↓ label_8
    ff6:  7010           	moveq #16,%d0                          
    ff8:  c083           	andl %d3,%d0                           
    ffa:  6606           	bnes 0x1002                            ; ↓ label_1
    ffc:  7020           	moveq #32,%d0                          
    ffe:  c083           	andl %d3,%d0                           
    1000:  6708           	beqs 0x100a                           ; ↓ label_2

  label_1:
    1002:  303c 2907      	movew #10503,%d0                      
    1006:  6000 00cc      	braw 0x10d4                           ; ↓ label_10

  label_2:
    100a:  4aae fff8      	tstl %fp@(-8)                         
    100e:  6606           	bnes 0x1016                           ; ↓ label_3
    1010:  7000           	moveq #0,%d0                          
    1012:  6000 00c0      	braw 0x10d4                           ; ↓ label_10

  label_3:
    1016:  42a7           	clrl %sp@-                            
    1018:  206e fffc      	moveal %fp@(-4),%a0                   
    101c:  2f28 000e      	movel %a0@(14),%sp@-                  
    1020:  4e4f           	trap #15                              
    1022:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1024:  1f3c 0001      	moveb #1,%sp@-                        
    1028:  4eba 08a4      	jsr %pc@(0x18ce)                      ; MSSlotHwrLightAccessLED
    102c:  1800           	moveb %d0,%d4                         
    102e:  206e fffc      	moveal %fp@(-4),%a0                   
    1032:  2028 0004      	movel %a0@(4),%d0                     
    1036:  0280 0000 0080 	andil #128,%d0                        
    103c:  4fef 000a      	lea %sp@(10),%sp                      
    1040:  6612           	bnes 0x1054                           ; ↓ label_4
    1042:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1046:  4eba 05f4      	jsr %pc@(0x163c)                      ; PrvSupplyPower
    104a:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    104e:  4eba 052c      	jsr %pc@(0x157c)                      ; PrvDoMgAuth
    1052:  504f           	addqw #8,%sp                          

  label_4:
    1054:  486e fff8      	pea %fp@(-8)                          
    1058:  2f2e 0010      	movel %fp@(16),%sp@-                  
    105c:  2f2e 000c      	movel %fp@(12),%sp@-                  
    1060:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1064:  4eba 0ba2      	jsr %pc@(0x1c08)                      ; PrvJobWriteSector
    1068:  2600           	movel %d0,%d3                         
    106a:  4fef 0010      	lea %sp@(16),%sp                      
    106e:  2003           	movel %d3,%d0                         
    1070:  0c80 0000 0006 	cmpil #6,%d0                          
    1076:  6232           	bhis 0x10aa                           ; ↓ label_5
    1078:  d040           	addw %d0,%d0                          
    107a:  303b 0006      	movew %pc@(0x1082,%d0:w),%d0          
    107e:  4efb 0002      	jmp %pc@(0x1082,%d0:w)                
    1082:  000e 0028      	orib #40,%fp                          
    1086:  0016 0028      	orib #40,%fp@                         
    108a:  001c 0022      	orib #34,%a4@+                        
    108e:  0022 24ae      	orib #-82,%a2@-                       
    1092:  fff8           	0177770                               
    1094:  7600           	moveq #0,%d3                          
    1096:  6016           	bras 0x10ae                           ; ↓ label_6
    1098:  363c 2903      	movew #10499,%d3                      
    109c:  6010           	bras 0x10ae                           ; ↓ label_6
    109e:  363c 0502      	movew #1282,%d3                       
    10a2:  600a           	bras 0x10ae                           ; ↓ label_6
    10a4:  363c 2907      	movew #10503,%d3                      
    10a8:  6004           	bras 0x10ae                           ; ↓ label_6

  label_5:
    10aa:  363c 2908      	movew #10504,%d3                      

  label_6:
    10ae:  4a04           	tstb %d4                              
    10b0:  6608           	bnes 0x10ba                           ; ↓ label_7
    10b2:  4227           	clrb %sp@-                            
    10b4:  4eba 0818      	jsr %pc@(0x18ce)                      ; MSSlotHwrLightAccessLED
    10b8:  544f           	addqw #2,%sp                          

  label_7:
    10ba:  4878 0064      	pea 0x64                              
    10be:  206e fffc      	moveal %fp@(-4),%a0                   
    10c2:  2f28 000e      	movel %a0@(14),%sp@-                  
    10c6:  4e4f           	trap #15                              
    10c8:  a09a           	0120232                               ; sysTrapSysTimerWrite
    10ca:  504f           	addqw #8,%sp                          
    10cc:  6004           	bras 0x10d2                           ; ↓ label_9

  label_8:
    10ce:  363c 2906      	movew #10502,%d3                      

  label_9:
    10d2:  3003           	movew %d3,%d0                         

  label_10:
    10d4:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    10d8:  4e5e           	unlk %fp                              
    10da:  4e75           	rts                                   
```

# MSSlotCheckPower: 0x10f0 - 0x1136

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    10f0:  4e56 fffc      	linkw %fp,#-4                         
    10f4:  2f03           	movel %d3,%sp@-                       
    10f6:  486e fffc      	pea %fp@(-4)                          
    10fa:  3f2e 000a      	movew %fp@(10),%sp@-                  
    10fe:  3f2e 0008      	movew %fp@(8),%sp@-                   
    1102:  4eba 0354      	jsr %pc@(0x1458)                      ; PrvDoPreCheck
    1106:  3600           	movew %d0,%d3                         
    1108:  504f           	addqw #8,%sp                          
    110a:  6704           	beqs 0x1110                           ; ↓ label_0
    110c:  3003           	movew %d3,%d0                         
    110e:  6022           	bras 0x1132                           ; ↓ label_4

  label_0:
    1110:  7001           	moveq #1,%d0                          
    1112:  206e fffc      	moveal %fp@(-4),%a0                   
    1116:  c0a8 0008      	andl %a0@(8),%d0                      
    111a:  6712           	beqs 0x112e                           ; ↓ label_3
    111c:  0c38 00ca 0139 	cmpib #-54,0x139                      
    1122:  6206           	bhis 0x112a                           ; ↓ label_1
    1124:  303c 2902      	movew #10498,%d0                      
    1128:  6002           	bras 0x112c                           ; ↓ label_2

  label_1:
    112a:  7000           	moveq #0,%d0                          

  label_2:
    112c:  6004           	bras 0x1132                           ; ↓ label_4

  label_3:
    112e:  303c 2906      	movew #10502,%d0                      

  label_4:
    1132:  261f           	movel %sp@+,%d3                       
    1134:  4e5e           	unlk %fp                              
    1136:  4e75           	rts                                   
```

# PrvShouldWeInstall: 0x114c - 0x1154

```
    114c:  4e56 0000      	linkw %fp,#0                          
    1150:  7000           	moveq #0,%d0                          
    1152:  4e5e           	unlk %fp                              
    1154:  4e75           	rts                                   
```

# PrvCrdiNotificationHandler: 0x116c - 0x1194

Parameters:

   * `%fp@(8)`      : ???

```
    116c:  4e56 0000      	linkw %fp,#0                          
    1170:  2f0b           	movel %a3,%sp@-                       
    1172:  222e 0008      	movel %fp@(8),%d1                     
    1176:  2041           	moveal %d1,%a0                        
    1178:  2668 000c      	moveal %a0@(12),%a3                   
    117c:  224b           	moveal %a3,%a1                        
    117e:  3028 000a      	movew %a0@(10),%d0                    
    1182:  b06b 0012      	cmpw %a3@(18),%d0                     
    1186:  6606           	bnes 0x118e                           ; ↓ label_0
    1188:  7004           	moveq #4,%d0                          
    118a:  81a9 0004      	orl %d0,%a1@(4)                       

  label_0:
    118e:  7000           	moveq #0,%d0                          
    1190:  265f           	moveal %sp@+,%a3                      
    1192:  4e5e           	unlk %fp                              
    1194:  4e75           	rts                                   
```

# PrvCrdoNotificationHandler: 0x11b4 - 0x11f0

Parameters:

   * `%fp@(8)`      : ???

```
    11b4:  4e56 0000      	linkw %fp,#0                          
    11b8:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    11bc:  262e 0008      	movel %fp@(8),%d3                     
    11c0:  2043           	moveal %d3,%a0                        
    11c2:  2668 000c      	moveal %a0@(12),%a3                   
    11c6:  244b           	moveal %a3,%a2                        
    11c8:  3028 000a      	movew %a0@(10),%d0                    
    11cc:  b06b 0012      	cmpw %a3@(18),%d0                     
    11d0:  6616           	bnes 0x11e8                           ; ↓ label_0
    11d2:  7008           	moveq #8,%d0                          
    11d4:  81aa 0004      	orl %d0,%a2@(4)                       
    11d8:  4227           	clrb %sp@-                            
    11da:  4eba 05f4      	jsr %pc@(0x17d0)                      ; MSSlotHwrSupplyPower
    11de:  02aa ffff ff7f 	andil #-129,%a2@(4)                   
    11e4:  0004                                                  
    11e6:  544f           	addqw #2,%sp                          

  label_0:
    11e8:  7000           	moveq #0,%d0                          
    11ea:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    11ee:  4e5e           	unlk %fp                              
    11f0:  4e75           	rts                                   
```

# PrvVolmNotificationHandler: 0x1210 - 0x12ea

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-10)`    : ???
   * `%fp@(-28)`    : ???

```
    1210:  4e56 ffe4      	linkw %fp,#-28                        
    1214:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    1218:  266e 0008      	moveal %fp@(8),%a3                    
    121c:  246b 000c      	moveal %a3@(12),%a2                   
    1220:  486e ffe4      	pea %fp@(-28)                         
    1224:  206b 0008      	moveal %a3@(8),%a0                    
    1228:  3f10           	movew %a0@,%sp@-                      
    122a:  741c           	moveq #28,%d2                         
    122c:  4e4f           	trap #15                              
    122e:  a348           	0121510                               ; sysTrapFileSystemDispatch
    1230:  4a40           	tstw %d0                              
    1232:  5c4f           	addqw #6,%sp                          
    1234:  6706           	beqs 0x123c                           ; ↓ label_0
    1236:  7000           	moveq #0,%d0                          
    1238:  6000 00aa      	braw 0x12e4                           ; ↓ label_7

  label_0:
    123c:  302e fff6      	movew %fp@(-10),%d0                   
    1240:  b06a 0012      	cmpw %a2@(18),%d0                     
    1244:  6600 009c      	bnew 0x12e2                           ; ↓ label_6
    1248:  42a7           	clrl %sp@-                            
    124a:  2f2a 000e      	movel %a2@(14),%sp@-                  
    124e:  4e4f           	trap #15                              
    1250:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1252:  7640           	moveq #64,%d3                         
    1254:  c6aa 0004      	andl %a2@(4),%d3                      
    1258:  1f3c 0001      	moveb #1,%sp@-                        
    125c:  4eba 0670      	jsr %pc@(0x18ce)                      ; MSSlotHwrLightAccessLED
    1260:  1800           	moveb %d0,%d4                         
    1262:  202a 0004      	movel %a2@(4),%d0                     
    1266:  0280 0000 0080 	andil #128,%d0                        
    126c:  4fef 000a      	lea %sp@(10),%sp                      
    1270:  660e           	bnes 0x1280                           ; ↓ label_1
    1272:  2f0a           	movel %a2,%sp@-                       
    1274:  4eba 03c6      	jsr %pc@(0x163c)                      ; PrvSupplyPower
    1278:  2f0a           	movel %a2,%sp@-                       
    127a:  4eba 0300      	jsr %pc@(0x157c)                      ; PrvDoMgAuth
    127e:  504f           	addqw #8,%sp                          

  label_1:
    1280:  202a 0004      	movel %a2@(4),%d0                     
    1284:  0280 0001 0000 	andil #65536,%d0                      
    128a:  673c           	beqs 0x12c8                           ; ↓ label_4
    128c:  7040           	moveq #64,%d0                         
    128e:  81aa 0004      	orl %d0,%a2@(4)                       
    1292:  4267           	clrw %sp@-                            
    1294:  486a 000c      	pea %a2@(12)                          
    1298:  2f2a 0020      	movel %a2@(32),%sp@-                  
    129c:  3f2a 0024      	movew %a2@(36),%sp@-                  
    12a0:  4e4f           	trap #15                              
    12a2:  a806           	0124006                               ; ??? 0xa806
    12a4:  4a40           	tstw %d0                              
    12a6:  4fef 000c      	lea %sp@(12),%sp                      
    12aa:  670a           	beqs 0x12b6                           ; ↓ label_2
    12ac:  00aa 0200 0000 	oril #33554432,%a2@(4)                
    12b2:  0004                                                  
    12b4:  6008           	bras 0x12be                           ; ↓ label_3

  label_2:
    12b6:  00aa 0100 0000 	oril #16777216,%a2@(4)                
    12bc:  0004                                                  

  label_3:
    12be:  4a03           	tstb %d3                              
    12c0:  6606           	bnes 0x12c8                           ; ↓ label_4
    12c2:  70bf           	moveq #-65,%d0                        
    12c4:  c1aa 0004      	andl %d0,%a2@(4)                      

  label_4:
    12c8:  4a04           	tstb %d4                              
    12ca:  6608           	bnes 0x12d4                           ; ↓ label_5
    12cc:  4227           	clrb %sp@-                            
    12ce:  4eba 05fe      	jsr %pc@(0x18ce)                      ; MSSlotHwrLightAccessLED
    12d2:  544f           	addqw #2,%sp                          

  label_5:
    12d4:  4878 0064      	pea 0x64                              
    12d8:  2f2a 000e      	movel %a2@(14),%sp@-                  
    12dc:  4e4f           	trap #15                              
    12de:  a09a           	0120232                               ; sysTrapSysTimerWrite
    12e0:  504f           	addqw #8,%sp                          

  label_6:
    12e2:  7000           	moveq #0,%d0                          

  label_7:
    12e4:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    12e8:  4e5e           	unlk %fp                              
    12ea:  4e75           	rts                                   
```

# PrvInsRmvIntHandler: 0x130a - 0x13f6

Parameters:

   * `%fp@(8)`      : ???

```
    130a:  4e56 0000      	linkw %fp,#0                          
    130e:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    1312:  2078 0304      	moveal 0x304,%a0                      
    1316:  2668 0024      	moveal %a0@(36),%a3                   
    131a:  244b           	moveal %a3,%a2                        
    131c:  362b 0012      	movew %a3@(18),%d3                    
    1320:  0c2e 0001 0008 	cmpib #1,%fp@(8)                      
    1326:  6672           	bnes 0x139a                           ; ↓ label_1
    1328:  7001           	moveq #1,%d0                          
    132a:  81aa 0004      	orl %d0,%a2@(4)                       
    132e:  70fb           	moveq #-5,%d0                         
    1330:  c1aa 0004      	andl %d0,%a2@(4)                      
    1334:  70ef           	moveq #-17,%d0                        
    1336:  c1aa 0004      	andl %d0,%a2@(4)                      
    133a:  70df           	moveq #-33,%d0                        
    133c:  c1aa 0004      	andl %d0,%a2@(4)                      
    1340:  02aa fffe ffff 	andil #-65537,%a2@(4)                 
    1346:  0004                                                  
    1348:  02aa fffd ffff 	andil #-131073,%a2@(4)                
    134e:  0004                                                  
    1350:  02aa feff ffff 	andil #-16777217,%a2@(4)              
    1356:  0004                                                  
    1358:  02aa fdff ffff 	andil #-33554433,%a2@(4)              
    135e:  0004                                                  
    1360:  2478 0304      	moveal 0x304,%a2                      
    1364:  2078 0304      	moveal 0x304,%a0                      
    1368:  4aa8 0010      	tstl %a0@(16)                         
    136c:  670a           	beqs 0x1378                           ; ↓ label_0
    136e:  206a 0010      	moveal %a2@(16),%a0                   
    1372:  7001           	moveq #1,%d0                          
    1374:  81a8 001c      	orl %d0,%a0@(28)                      

  label_0:
    1378:  3f03           	movew %d3,%sp@-                       
    137a:  7406           	moveq #6,%d2                          
    137c:  4e4f           	trap #15                              
    137e:  a347           	0121507                               ; sysTrapExpansionDispatch
    1380:  4227           	clrb %sp@-                            
    1382:  1f3c 0001      	moveb #1,%sp@-                        
    1386:  4eba 04e6      	jsr %pc@(0x186e)                      ; MSSlotHwrSetUpInterrupt
    138a:  1f3c 0001      	moveb #1,%sp@-                        
    138e:  4227           	clrb %sp@-                            
    1390:  4eba 04dc      	jsr %pc@(0x186e)                      ; MSSlotHwrSetUpInterrupt
    1394:  4fef 000a      	lea %sp@(10),%sp                      
    1398:  6056           	bras 0x13f0                           ; ↓ label_5

  label_1:
    139a:  70fe           	moveq #-2,%d0                         
    139c:  c1aa 0004      	andl %d0,%a2@(4)                      
    13a0:  7004           	moveq #4,%d0                          
    13a2:  c0aa 0004      	andl %a2@(4),%d0                      
    13a6:  6608           	bnes 0x13b0                           ; ↓ label_2
    13a8:  7004           	moveq #4,%d0                          
    13aa:  81aa 0004      	orl %d0,%a2@(4)                       
    13ae:  6006           	bras 0x13b6                           ; ↓ label_3

  label_2:
    13b0:  70f7           	moveq #-9,%d0                         
    13b2:  c1aa 0004      	andl %d0,%a2@(4)                      

  label_3:
    13b6:  2478 0304      	moveal 0x304,%a2                      
    13ba:  2078 0304      	moveal 0x304,%a0                      
    13be:  4aa8 0010      	tstl %a0@(16)                         
    13c2:  670c           	beqs 0x13d0                           ; ↓ label_4
    13c4:  206a 0010      	moveal %a2@(16),%a0                   
    13c8:  02a8 ffff ff00 	andil #-256,%a0@(28)                  
    13ce:  001c                                                  

  label_4:
    13d0:  3f03           	movew %d3,%sp@-                       
    13d2:  7407           	moveq #7,%d2                          
    13d4:  4e4f           	trap #15                              
    13d6:  a347           	0121507                               ; sysTrapExpansionDispatch
    13d8:  4227           	clrb %sp@-                            
    13da:  4227           	clrb %sp@-                            
    13dc:  4eba 0490      	jsr %pc@(0x186e)                      ; MSSlotHwrSetUpInterrupt
    13e0:  1f3c 0001      	moveb #1,%sp@-                        
    13e4:  1f3c 0001      	moveb #1,%sp@-                        
    13e8:  4eba 0484      	jsr %pc@(0x186e)                      ; MSSlotHwrSetUpInterrupt
    13ec:  4fef 000a      	lea %sp@(10),%sp                      

  label_5:
    13f0:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    13f4:  4e5e           	unlk %fp                              
    13f6:  4e75           	rts                                   
```

# PrvTimerProc: 0x140e - 0x1446

Parameters:

   * `%fp@(12)`     : ???

```
    140e:  4e56 0000      	linkw %fp,#0                          
    1412:  2f0a           	movel %a2,%sp@-                       
    1414:  4aae 000c      	tstl %fp@(12)                         
    1418:  6728           	beqs 0x1442                           ; ↓ label_0
    141a:  246e 000c      	moveal %fp@(12),%a2                   
    141e:  42a7           	clrl %sp@-                            
    1420:  2f2a 000e      	movel %a2@(14),%sp@-                  
    1424:  4e4f           	trap #15                              
    1426:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1428:  7040           	moveq #64,%d0                         
    142a:  c0aa 0004      	andl %a2@(4),%d0                      
    142e:  504f           	addqw #8,%sp                          
    1430:  6610           	bnes 0x1442                           ; ↓ label_0
    1432:  4227           	clrb %sp@-                            
    1434:  4eba 039a      	jsr %pc@(0x17d0)                      ; MSSlotHwrSupplyPower
    1438:  02aa ffff ff7f 	andil #-129,%a2@(4)                   
    143e:  0004                                                  
    1440:  544f           	addqw #2,%sp                          

  label_0:
    1442:  245f           	moveal %sp@+,%a2                      
    1444:  4e5e           	unlk %fp                              
    1446:  4e75           	rts                                   
```

# PrvDoPreCheck: 0x1458 - 0x14b4

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

```
    1458:  4e56 0000      	linkw %fp,#0                          
    145c:  2f0b           	movel %a3,%sp@-                       
    145e:  2f0a           	movel %a2,%sp@-                       
    1460:  266e 000c      	moveal %fp@(12),%a3                   
    1464:  3f2e 0008      	movew %fp@(8),%sp@-                   
    1468:  4e4f           	trap #15                              
    146a:  a0b9           	0120271                               ; sysTrapSysLibTblEntry
    146c:  26a8 0004      	movel %a0@(4),%a3@                    
    1470:  2453           	moveal %a3@,%a2                       
    1472:  200a           	movel %a2,%d0                         
    1474:  544f           	addqw #2,%sp                          
    1476:  6606           	bnes 0x147e                           ; ↓ label_0
    1478:  303c 290a      	movew #10506,%d0                      
    147c:  6030           	bras 0x14ae                           ; ↓ label_4

  label_0:
    147e:  302a 0012      	movew %a2@(18),%d0                    
    1482:  b06e 000a      	cmpw %fp@(10),%d0                     
    1486:  6706           	beqs 0x148e                           ; ↓ label_1
    1488:  303c 2904      	movew #10500,%d0                      
    148c:  6020           	bras 0x14ae                           ; ↓ label_4

  label_1:
    148e:  7001           	moveq #1,%d0                          
    1490:  204a           	moveal %a2,%a0                        
    1492:  5888           	addql #4,%a0                          
    1494:  2648           	moveal %a0,%a3                        
    1496:  c090           	andl %a0@,%d0                         
    1498:  6606           	bnes 0x14a0                           ; ↓ label_2
    149a:  303c 2903      	movew #10499,%d0                      
    149e:  600e           	bras 0x14ae                           ; ↓ label_4

  label_2:
    14a0:  7008           	moveq #8,%d0                          
    14a2:  c093           	andl %a3@,%d0                         
    14a4:  6704           	beqs 0x14aa                           ; ↓ label_3
    14a6:  7000           	moveq #0,%d0                          
    14a8:  6004           	bras 0x14ae                           ; ↓ label_4

  label_3:
    14aa:  303c 2903      	movew #10499,%d0                      

  label_4:
    14ae:  245f           	moveal %sp@+,%a2                      
    14b0:  265f           	moveal %sp@+,%a3                      
    14b2:  4e5e           	unlk %fp                              
    14b4:  4e75           	rts                                   
```

# PrvAssureMounted: 0x14c6 - 0x1566

Parameters:

   * `%fp@(8)`      : ???

```
    14c6:  4e56 0000      	linkw %fp,#0                          
    14ca:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    14ce:  246e 0008      	moveal %fp@(8),%a2                    
    14d2:  7020           	moveq #32,%d0                         
    14d4:  c0aa 0004      	andl %a2@(4),%d0                      
    14d8:  6608           	bnes 0x14e2                           ; ↓ label_0
    14da:  7010           	moveq #16,%d0                         
    14dc:  c0aa 0004      	andl %a2@(4),%d0                      
    14e0:  6702           	beqs 0x14e4                           ; ↓ label_1

  label_0:
    14e2:  607c           	bras 0x1560                           ; ↓ label_7

  label_1:
    14e4:  42a7           	clrl %sp@-                            
    14e6:  2f2a 000e      	movel %a2@(14),%sp@-                  
    14ea:  4e4f           	trap #15                              
    14ec:  a09a           	0120232                               ; sysTrapSysTimerWrite
    14ee:  1f3c 0001      	moveb #1,%sp@-                        
    14f2:  4eba 03da      	jsr %pc@(0x18ce)                      ; MSSlotHwrLightAccessLED
    14f6:  1a00           	moveb %d0,%d5                         
    14f8:  2f0a           	movel %a2,%sp@-                       
    14fa:  4eba 0140      	jsr %pc@(0x163c)                      ; PrvSupplyPower
    14fe:  7600           	moveq #0,%d3                          
    1500:  4fef 000e      	lea %sp@(14),%sp                      

  label_2:
    1504:  2f0a           	movel %a2,%sp@-                       
    1506:  4eba 0468      	jsr %pc@(0x1970)                      ; PrvJobMount
    150a:  2800           	movel %d0,%d4                         
    150c:  584f           	addqw #4,%sp                          
    150e:  6610           	bnes 0x1520                           ; ↓ label_3
    1510:  7010           	moveq #16,%d0                         
    1512:  81aa 0004      	orl %d0,%a2@(4)                       
    1516:  2f0a           	movel %a2,%sp@-                       
    1518:  4eba 0062      	jsr %pc@(0x157c)                      ; PrvDoMgAuth
    151c:  584f           	addqw #4,%sp                          
    151e:  601a           	bras 0x153a                           ; ↓ label_4

  label_3:
    1520:  4878 0003      	pea 0x3                               
    1524:  4e4f           	trap #15                              
    1526:  a0a0           	0120240                               ; sysTrapSysTaskDelay
    1528:  7001           	moveq #1,%d0                          
    152a:  c0aa 0004      	andl %a2@(4),%d0                      
    152e:  584f           	addqw #4,%sp                          
    1530:  6708           	beqs 0x153a                           ; ↓ label_4
    1532:  5243           	addqw #1,%d3                          
    1534:  0c43 0002      	cmpiw #2,%d3                          
    1538:  65ca           	bcss 0x1504                           ; ↑ label_2

  label_4:
    153a:  0c43 0002      	cmpiw #2,%d3                          
    153e:  6506           	bcss 0x1546                           ; ↓ label_5
    1540:  7020           	moveq #32,%d0                         
    1542:  81aa 0004      	orl %d0,%a2@(4)                       

  label_5:
    1546:  4a05           	tstb %d5                              
    1548:  6608           	bnes 0x1552                           ; ↓ label_6
    154a:  4227           	clrb %sp@-                            
    154c:  4eba 0380      	jsr %pc@(0x18ce)                      ; MSSlotHwrLightAccessLED
    1550:  544f           	addqw #2,%sp                          

  label_6:
    1552:  4878 0064      	pea 0x64                              
    1556:  2f2a 000e      	movel %a2@(14),%sp@-                  
    155a:  4e4f           	trap #15                              
    155c:  a09a           	0120232                               ; sysTrapSysTimerWrite
    155e:  504f           	addqw #8,%sp                          

  label_7:
    1560:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    1564:  4e5e           	unlk %fp                              
    1566:  4e75           	rts                                   
```

# PrvDoMgAuth: 0x157c - 0x162c

Parameters:

   * `%fp@(8)`      : ???

```
    157c:  4e56 0000      	linkw %fp,#0                          
    1580:  2f0a           	movel %a2,%sp@-                       
    1582:  2f03           	movel %d3,%sp@-                       
    1584:  246e 0008      	moveal %fp@(8),%a2                    
    1588:  4227           	clrb %sp@-                            
    158a:  4e4f           	trap #15                              
    158c:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    158e:  7002           	moveq #2,%d0                          
    1590:  c0aa 0008      	andl %a2@(8),%d0                      
    1594:  544f           	addqw #2,%sp                          
    1596:  6700 0084      	beqw 0x161c                           ; ↓ label_5
    159a:  202a 0004      	movel %a2@(4),%d0                     
    159e:  0280 0000 0100 	andil #256,%d0                        
    15a4:  6776           	beqs 0x161c                           ; ↓ label_5
    15a6:  02aa fffe ffff 	andil #-65537,%a2@(4)                 
    15ac:  0004                                                  
    15ae:  02aa fffd ffff 	andil #-131073,%a2@(4)                
    15b4:  0004                                                  
    15b6:  7600           	moveq #0,%d3                          

  label_0:
    15b8:  7001           	moveq #1,%d0                          
    15ba:  c0aa 0004      	andl %a2@(4),%d0                      
    15be:  660c           	bnes 0x15cc                           ; ↓ label_1
    15c0:  4878 0014      	pea 0x14                              
    15c4:  4e4f           	trap #15                              
    15c6:  a249           	0121111                               ; sysTrapHwrDelay
    15c8:  584f           	addqw #4,%sp                          
    15ca:  6030           	bras 0x15fc                           ; ↓ label_2

  label_1:
    15cc:  3f2a 0024      	movew %a2@(36),%sp@-                  
    15d0:  4e4f           	trap #15                              
    15d2:  a805           	0124005                               ; ??? 0xa805
    15d4:  4a40           	tstw %d0                              
    15d6:  544f           	addqw #2,%sp                          
    15d8:  672a           	beqs 0x1604                           ; ↓ label_3
    15da:  4227           	clrb %sp@-                            
    15dc:  4eba 01f2      	jsr %pc@(0x17d0)                      ; MSSlotHwrSupplyPower
    15e0:  4878 0014      	pea 0x14                              
    15e4:  4e4f           	trap #15                              
    15e6:  a249           	0121111                               ; sysTrapHwrDelay
    15e8:  1f3c 0001      	moveb #1,%sp@-                        
    15ec:  4eba 01e2      	jsr %pc@(0x17d0)                      ; MSSlotHwrSupplyPower
    15f0:  4878 07d0      	pea 0x7d0                             
    15f4:  4e4f           	trap #15                              
    15f6:  a249           	0121111                               ; sysTrapHwrDelay
    15f8:  4fef 000c      	lea %sp@(12),%sp                      

  label_2:
    15fc:  5203           	addqb #1,%d3                          
    15fe:  0c03 0003      	cmpib #3,%d3                          
    1602:  65b4           	bcss 0x15b8                           ; ↑ label_0

  label_3:
    1604:  0c03 0003      	cmpib #3,%d3                          
    1608:  640a           	bccs 0x1614                           ; ↓ label_4
    160a:  00aa 0001 0000 	oril #65536,%a2@(4)                   
    1610:  0004                                                  
    1612:  6008           	bras 0x161c                           ; ↓ label_5

  label_4:
    1614:  00aa 0002 0000 	oril #131072,%a2@(4)                  
    161a:  0004                                                  

  label_5:
    161c:  1f3c 0001      	moveb #1,%sp@-                        
    1620:  4e4f           	trap #15                              
    1622:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    1624:  544f           	addqw #2,%sp                          
    1626:  261f           	movel %sp@+,%d3                       
    1628:  245f           	moveal %sp@+,%a2                      
    162a:  4e5e           	unlk %fp                              
    162c:  4e75           	rts                                   
```

# PrvSupplyPower: 0x163c - 0x165e

Parameters:

   * `%fp@(8)`      : ???

```
    163c:  4e56 0000      	linkw %fp,#0                          
    1640:  1f3c 0001      	moveb #1,%sp@-                        
    1644:  4eba 018a      	jsr %pc@(0x17d0)                      ; MSSlotHwrSupplyPower
    1648:  206e 0008      	moveal %fp@(8),%a0                    
    164c:  00a8 0000 0080 	oril #128,%a0@(4)                     
    1652:  0004                                                  
    1654:  4878 07d0      	pea 0x7d0                             
    1658:  4e4f           	trap #15                              
    165a:  a249           	0121111                               ; sysTrapHwrDelay
    165c:  4e5e           	unlk %fp                              
    165e:  4e75           	rts                                   
```

# __Startup__: 0x1672 - 0x1694

Parameters:

   * `%fp@(10)`     : ???

```
    1672:  4e56 0000      	linkw %fp,#0                          
    1676:  2f03           	movel %d3,%sp@-                       
    1678:  4eba fad2      	jsr %pc@(0x114c)                      ; PrvShouldWeInstall
    167c:  3600           	movew %d0,%d3                         
    167e:  6704           	beqs 0x1684                           ; ↓ label_0
    1680:  3003           	movew %d3,%d0                         
    1682:  600c           	bras 0x1690                           ; ↓ label_1

  label_0:
    1684:  4eba 001e      	jsr %pc@(0x16a4)                      ; n
    1688:  226e 000a      	moveal %fp@(10),%a1                   
    168c:  2288           	movel %a0,%a1@                        
    168e:  7000           	moveq #0,%d0                          

  label_1:
    1690:  261f           	movel %sp@+,%d3                       
    1692:  4e5e           	unlk %fp                              
    1694:  4e75           	rts                                   
```

# n: 0x16a4 - 0x16a8

```
    16a4:  41fa 0004      	lea %pc@(0x16aa),%a0                  ; ??? 0x16aa
    16a8:  4e75           	rts                                   
```

# MSSlotHwrOpen: 0x1740 - 0x1784

```
    1740:  4e56 0000      	linkw %fp,#0                          
    1744:  41fa 00be      	lea %pc@(0x1804),%a0                  ; MSSlotHwrInsIntHandler
    1748:  2278 0304      	moveal 0x304,%a1                      
    174c:  2269 0018      	moveal %a1@(24),%a1                   
    1750:  2348 0014      	movel %a0,%a1@(20)                    
    1754:  41fa 00f0      	lea %pc@(0x1846),%a0                  ; MSSlotHwrRmvIntHandler
    1758:  2278 0304      	moveal 0x304,%a1                      
    175c:  2269 0018      	moveal %a1@(24),%a1                   
    1760:  2348 0018      	movel %a0,%a1@(24)                    
    1764:  0038 0060 f40b 	orib #96,0xfffffffffffff40b           
    176a:  0038 0060 f408 	orib #96,0xfffffffffffff408           
    1770:  0238 009f f40a 	andib #-97,0xfffffffffffff40a         
    1776:  4227           	clrb %sp@-                            
    1778:  4eba 0056      	jsr %pc@(0x17d0)                      ; MSSlotHwrSupplyPower
    177c:  4227           	clrb %sp@-                            
    177e:  4eba 014e      	jsr %pc@(0x18ce)                      ; MSSlotHwrLightAccessLED
    1782:  4e5e           	unlk %fp                              
    1784:  4e75           	rts                                   
```

# MSSlotHwrClose: 0x1796 - 0x17bc

```
    1796:  4e56 0000      	linkw %fp,#0                          
    179a:  0279 fff3 1080 	andiw #-13,0x10800222                 
    17a0:  0222                                                  
    17a2:  2078 0304      	moveal 0x304,%a0                      
    17a6:  2068 0018      	moveal %a0@(24),%a0                   
    17aa:  42a8 0014      	clrl %a0@(20)                         
    17ae:  2078 0304      	moveal 0x304,%a0                      
    17b2:  2068 0018      	moveal %a0@(24),%a0                   
    17b6:  42a8 0018      	clrl %a0@(24)                         
    17ba:  4e5e           	unlk %fp                              
    17bc:  4e75           	rts                                   
```

# MSSlotHwrSupplyPower: 0x17d0 - 0x17ea

Parameters:

   * `%fp@(8)`      : ???

```
    17d0:  4e56 0000      	linkw %fp,#0                          
    17d4:  4a2e 0008      	tstb %fp@(8)                          
    17d8:  6708           	beqs 0x17e2                           ; ↓ label_0
    17da:  0038 0020 f409 	orib #32,0xfffffffffffff409           
    17e0:  6006           	bras 0x17e8                           ; ↓ label_1

  label_0:
    17e2:  0238 00df f409 	andib #-33,0xfffffffffffff409         

  label_1:
    17e8:  4e5e           	unlk %fp                              
    17ea:  4e75           	rts                                   
```

# MSSlotHwrInsIntHandler: 0x1804 - 0x182a

```
    1804:  4e56 0000      	linkw %fp,#0                          
    1808:  4878 07d0      	pea 0x7d0                             
    180c:  4e4f           	trap #15                              
    180e:  a249           	0121111                               ; sysTrapHwrDelay
    1810:  3039 1080 0220 	movew 0x10800220,%d0                  
    1816:  0240 0004      	andiw #4,%d0                          
    181a:  584f           	addqw #4,%sp                          
    181c:  670a           	beqs 0x1828                           ; ↓ label_0
    181e:  1f3c 0001      	moveb #1,%sp@-                        
    1822:  4eba fae6      	jsr %pc@(0x130a)                      ; PrvInsRmvIntHandler
    1826:  544f           	addqw #2,%sp                          

  label_0:
    1828:  4e5e           	unlk %fp                              
    182a:  4e75           	rts                                   
```

# MSSlotHwrRmvIntHandler: 0x1846 - 0x1852

```
    1846:  4e56 0000      	linkw %fp,#0                          
    184a:  4227           	clrb %sp@-                            
    184c:  4eba fabc      	jsr %pc@(0x130a)                      ; PrvInsRmvIntHandler
    1850:  4e5e           	unlk %fp                              
    1852:  4e75           	rts                                   
```

# MSSlotHwrSetUpInterrupt: 0x186e - 0x18b2

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    186e:  4e56 0000      	linkw %fp,#0                          
    1872:  0c2e 0001 0008 	cmpib #1,%fp@(8)                      
    1878:  661c           	bnes 0x1896                           ; ↓ label_1
    187a:  0c2e 0001 000a 	cmpib #1,%fp@(10)                     
    1880:  660a           	bnes 0x188c                           ; ↓ label_0
    1882:  0079 0004 1080 	oriw #4,0x10800222                    
    1888:  0222                                                  
    188a:  6024           	bras 0x18b0                           ; ↓ label_3

  label_0:
    188c:  0279 fffb 1080 	andiw #-5,0x10800222                  
    1892:  0222                                                  
    1894:  601a           	bras 0x18b0                           ; ↓ label_3

  label_1:
    1896:  0c2e 0001 000a 	cmpib #1,%fp@(10)                     
    189c:  660a           	bnes 0x18a8                           ; ↓ label_2
    189e:  0079 0008 1080 	oriw #8,0x10800222                    
    18a4:  0222                                                  
    18a6:  6008           	bras 0x18b0                           ; ↓ label_3

  label_2:
    18a8:  0279 fff7 1080 	andiw #-9,0x10800222                  
    18ae:  0222                                                  

  label_3:
    18b0:  4e5e           	unlk %fp                              
    18b2:  4e75           	rts                                   
```

# MSSlotHwrLightAccessLED: 0x18ce - 0x18f0

Parameters:

   * `%fp@(8)`      : ???

```
    18ce:  4e56 0000      	linkw %fp,#0                          
    18d2:  1038 f409      	moveb 0xfffffffffffff409,%d0          
    18d6:  0200 0040      	andib #64,%d0                         
    18da:  4a2e 0008      	tstb %fp@(8)                          
    18de:  6708           	beqs 0x18e8                           ; ↓ label_0
    18e0:  0038 0040 f409 	orib #64,0xfffffffffffff409           
    18e6:  6006           	bras 0x18ee                           ; ↓ label_1

  label_0:
    18e8:  0238 00bf f409 	andib #-65,0xfffffffffffff409         

  label_1:
    18ee:  4e5e           	unlk %fp                              
    18f0:  4e75           	rts                                   
```

# PrvDspDoneProc: 0x190c - 0x195c

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    190c:  4e56 0000      	linkw %fp,#0                          
    1910:  202e 0008      	movel %fp@(8),%d0                     
    1914:  4a2e 000c      	tstb %fp@(12)                         
    1918:  673a           	beqs 0x1954                           ; ↓ label_0
    191a:  2040           	moveal %d0,%a0                        
    191c:  30b9 1080 0c06 	movew 0x10800c06,%a0@                 
    1922:  3179 1080 0c14 	movew 0x10800c14,%a0@(2)              
    1928:  0002                                                  
    192a:  3179 1080 0c16 	movew 0x10800c16,%a0@(4)              
    1930:  0004                                                  
    1932:  3179 1080 0c18 	movew 0x10800c18,%a0@(6)              
    1938:  0006                                                  
    193a:  3179 1080 0c1a 	movew 0x10800c1a,%a0@(8)              
    1940:  0008                                                  
    1942:  3179 1080 0c1c 	movew 0x10800c1c,%a0@(10)             
    1948:  000a                                                  
    194a:  3179 1080 0c1e 	movew 0x10800c1e,%a0@(12)             
    1950:  000c                                                  
    1952:  6006           	bras 0x195a                           ; ↓ label_1

  label_0:
    1954:  2040           	moveal %d0,%a0                        
    1956:  30bc 03fc      	movew #1020,%a0@                      

  label_1:
    195a:  4e5e           	unlk %fp                              
    195c:  4e75           	rts                                   
```

# PrvJobMount: 0x1970 - 0x1ab4

Parameters:

   * `%fp@(8)`      : ???

```
    1970:  4e56 0000      	linkw %fp,#0                          
    1974:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    1978:  266e 0008      	moveal %fp@(8),%a3                    
    197c:  42ab 0008      	clrl %a3@(8)                          
    1980:  2f0b           	movel %a3,%sp@-                       
    1982:  4eba 06f0      	jsr %pc@(0x2074)                      ; PrvJobDetectBootBlock
    1986:  2600           	movel %d0,%d3                         
    1988:  584f           	addqw #4,%sp                          
    198a:  6706           	beqs 0x1992                           ; ↓ label_0
    198c:  2003           	movel %d3,%d0                         
    198e:  6000 011e      	braw 0x1aae                           ; ↓ label_16

  label_0:
    1992:  246b 0020      	moveal %a3@(32),%a2                   
    1996:  45ea 2400      	lea %a2@(9216),%a2                    
    199a:  260a           	movel %a2,%d3                         
    199c:  2043           	moveal %d3,%a0                        
    199e:  0c68 1001 01c8 	cmpiw #4097,%a0@(456)                 
    19a4:  6620           	bnes 0x19c6                           ; ↓ label_2
    19a6:  2638 0304      	movel 0x304,%d3                       
    19aa:  2078 0304      	moveal 0x304,%a0                      
    19ae:  4aa8 0010      	tstl %a0@(16)                         
    19b2:  670c           	beqs 0x19c0                           ; ↓ label_1
    19b4:  2043           	moveal %d3,%a0                        
    19b6:  2068 0010      	moveal %a0@(16),%a0                   
    19ba:  7004           	moveq #4,%d0                          
    19bc:  81a8 001c      	orl %d0,%a0@(28)                      

  label_1:
    19c0:  7002           	moveq #2,%d0                          
    19c2:  81ab 0008      	orl %d0,%a3@(8)                       

  label_2:
    19c6:  302a 0002      	movew %a2@(2),%d0                     
    19ca:  0240 ff00      	andiw #-256,%d0                       
    19ce:  0c40 0100      	cmpiw #256,%d0                        
    19d2:  6706           	beqs 0x19da                           ; ↓ label_3
    19d4:  7001           	moveq #1,%d0                          
    19d6:  6000 00d6      	braw 0x1aae                           ; ↓ label_16

  label_3:
    19da:  0c2a 0001 01a0 	cmpib #1,%a2@(416)                    
    19e0:  6706           	beqs 0x19e8                           ; ↓ label_4
    19e2:  7001           	moveq #1,%d0                          
    19e4:  6000 00c8      	braw 0x1aae                           ; ↓ label_16

  label_4:
    19e8:  0c2a 0002 01a1 	cmpib #2,%a2@(417)                    
    19ee:  6706           	beqs 0x19f6                           ; ↓ label_5
    19f0:  7001           	moveq #1,%d0                          
    19f2:  6000 00ba      	braw 0x1aae                           ; ↓ label_16

  label_5:
    19f6:  302a 01a2      	movew %a2@(418),%d0                   
    19fa:  5140           	subqw #8,%d0                          
    19fc:  6706           	beqs 0x1a04                           ; ↓ label_6
    19fe:  5140           	subqw #8,%d0                          
    1a00:  670a           	beqs 0x1a0c                           ; ↓ label_7
    1a02:  6010           	bras 0x1a14                           ; ↓ label_8

  label_6:
    1a04:  377c 0010 0014 	movew #16,%a3@(20)                    
    1a0a:  600e           	bras 0x1a1a                           ; ↓ label_9

  label_7:
    1a0c:  377c 0020 0014 	movew #32,%a3@(20)                    
    1a12:  6006           	bras 0x1a1a                           ; ↓ label_9

  label_8:
    1a14:  7001           	moveq #1,%d0                          
    1a16:  6000 0096      	braw 0x1aae                           ; ↓ label_16

  label_9:
    1a1a:  302a 01a4      	movew %a2@(420),%d0                   
    1a1e:  0440 0200      	subiw #512,%d0                        
    1a22:  671c           	beqs 0x1a40                           ; ↓ label_10
    1a24:  0440 0200      	subiw #512,%d0                        
    1a28:  6716           	beqs 0x1a40                           ; ↓ label_10
    1a2a:  0440 0400      	subiw #1024,%d0                       
    1a2e:  6710           	beqs 0x1a40                           ; ↓ label_10
    1a30:  0440 0800      	subiw #2048,%d0                       
    1a34:  670a           	beqs 0x1a40                           ; ↓ label_10
    1a36:  0440 1000      	subiw #4096,%d0                       
    1a3a:  6704           	beqs 0x1a40                           ; ↓ label_10
    1a3c:  7001           	moveq #1,%d0                          
    1a3e:  606e           	bras 0x1aae                           ; ↓ label_16

  label_10:
    1a40:  302a 01a6      	movew %a2@(422),%d0                   
    1a44:  0440 01f0      	subiw #496,%d0                        
    1a48:  671c           	beqs 0x1a66                           ; ↓ label_11
    1a4a:  0440 01f0      	subiw #496,%d0                        
    1a4e:  6716           	beqs 0x1a66                           ; ↓ label_11
    1a50:  0440 03e0      	subiw #992,%d0                        
    1a54:  6710           	beqs 0x1a66                           ; ↓ label_11
    1a56:  0440 07c0      	subiw #1984,%d0                       
    1a5a:  670a           	beqs 0x1a66                           ; ↓ label_11
    1a5c:  0440 0f80      	subiw #3968,%d0                       
    1a60:  6704           	beqs 0x1a66                           ; ↓ label_11
    1a62:  7001           	moveq #1,%d0                          
    1a64:  6048           	bras 0x1aae                           ; ↓ label_16

  label_11:
    1a66:  0c2a 0001 01d6 	cmpib #1,%a2@(470)                    
    1a6c:  6704           	beqs 0x1a72                           ; ↓ label_12
    1a6e:  7001           	moveq #1,%d0                          
    1a70:  603c           	bras 0x1aae                           ; ↓ label_16

  label_12:
    1a72:  0c2a 0001 0178 	cmpib #1,%a2@(376)                    
    1a78:  6704           	beqs 0x1a7e                           ; ↓ label_13
    1a7a:  7001           	moveq #1,%d0                          
    1a7c:  6030           	bras 0x1aae                           ; ↓ label_16

  label_13:
    1a7e:  2f0b           	movel %a3,%sp@-                       
    1a80:  4eba 06da      	jsr %pc@(0x215c)                      ; PrvJobMakeAddressTable
    1a84:  2600           	movel %d0,%d3                         
    1a86:  584f           	addqw #4,%sp                          
    1a88:  6704           	beqs 0x1a8e                           ; ↓ label_14
    1a8a:  2003           	movel %d3,%d0                         
    1a8c:  6020           	bras 0x1aae                           ; ↓ label_16

  label_14:
    1a8e:  2478 0304      	moveal 0x304,%a2                      
    1a92:  2078 0304      	moveal 0x304,%a0                      
    1a96:  4aa8 0010      	tstl %a0@(16)                         
    1a9a:  670a           	beqs 0x1aa6                           ; ↓ label_15
    1a9c:  206a 0010      	moveal %a2@(16),%a0                   
    1aa0:  7002           	moveq #2,%d0                          
    1aa2:  81a8 001c      	orl %d0,%a0@(28)                      

  label_15:
    1aa6:  7001           	moveq #1,%d0                          
    1aa8:  81ab 0008      	orl %d0,%a3@(8)                       
    1aac:  7000           	moveq #0,%d0                          

  label_16:
    1aae:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    1ab2:  4e5e           	unlk %fp                              
    1ab4:  4e75           	rts                                   
```

# PrvJobReadSector: 0x1ac4 - 0x1bf2

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-16)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-25)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-29)`    : ???
   * `%fp@(-32)`    : ???
   * `%fp@(-34)`    : ???
   * `%fp@(-36)`    : ???
   * `%fp@(-40)`    : ???

```
    1ac4:  4e56 ffd8      	linkw %fp,#-40                        
    1ac8:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    1acc:  246e 0008      	moveal %fp@(8),%a2                    
    1ad0:  41fa 1ac8      	lea %pc@(0x359a),%a0                  ; ??? 0x359a
    1ad4:  2d48 fff0      	movel %a0,%fp@(-16)                   
    1ad8:  266e 0010      	moveal %fp@(16),%a3                   
    1adc:  206e 0014      	moveal %fp@(20),%a0                   
    1ae0:  2d50 ffe8      	movel %a0@,%fp@(-24)                  
    1ae4:  2d50 ffec      	movel %a0@,%fp@(-20)                  
    1ae8:  3e2a 0014      	movew %a2@(20),%d7                    
    1aec:  7000           	moveq #0,%d0                          
    1aee:  3007           	movew %d7,%d0                         
    1af0:  2c00           	movel %d0,%d6                         
    1af2:  2200           	movel %d0,%d1                         
    1af4:  202e 000c      	movel %fp@(12),%d0                    
    1af8:  4eba 1b02      	jsr %pc@(0x35fc)                      ; anonymous_function_0
    1afc:  3800           	movew %d0,%d4                         
    1afe:  202e 000c      	movel %fp@(12),%d0                    
    1b02:  2206           	movel %d6,%d1                         
    1b04:  4eba 1b4e      	jsr %pc@(0x3654)                      ; anonymous_function_1
    1b08:  2d40 ffe4      	movel %d0,%fp@(-28)                   
    1b0c:  1a00           	moveb %d0,%d5                         
    1b0e:  7600           	moveq #0,%d3                          
    1b10:  162e ffe7      	moveb %fp@(-25),%d3                   
    1b14:  48c3           	extl %d3                              
    1b16:  d6ae ffe8      	addl %fp@(-24),%d3                    
    1b1a:  5383           	subql #1,%d3                          
    1b1c:  2d43 ffe0      	movel %d3,%fp@(-32)                   
    1b20:  7000           	moveq #0,%d0                          
    1b22:  102e ffe3      	moveb %fp@(-29),%d0                   
    1b26:  b047           	cmpw %d7,%d0                          
    1b28:  6506           	bcss 0x1b30                           ; ↓ label_0
    1b2a:  1607           	moveb %d7,%d3                         
    1b2c:  0603 ffff      	addib #-1,%d3                         

  label_0:
    1b30:  202e 000c      	movel %fp@(12),%d0                    
    1b34:  d0ae ffec      	addl %fp@(-20),%d0                    
    1b38:  5380           	subql #1,%d0                          
    1b3a:  2d40 ffd8      	movel %d0,%fp@(-40)                   
    1b3e:  2206           	movel %d6,%d1                         
    1b40:  4eba 1aba      	jsr %pc@(0x35fc)                      ; anonymous_function_0
    1b44:  2d40 ffdc      	movel %d0,%fp@(-36)                   
    1b48:  3e00           	movew %d0,%d7                         
    1b4a:  206e fff0      	moveal %fp@(-16),%a0                  
    1b4e:  3028 0022      	movew %a0@(34),%d0                    
    1b52:  b06e ffde      	cmpw %fp@(-34),%d0                    
    1b56:  620c           	bhis 0x1b64                           ; ↓ label_1
    1b58:  206e 0014      	moveal %fp@(20),%a0                   
    1b5c:  4290           	clrl %a0@                             
    1b5e:  7004           	moveq #4,%d0                          
    1b60:  6000 008a      	braw 0x1bec                           ; ↓ label_6

  label_1:
    1b64:  7c00           	moveq #0,%d6                          
    1b66:  3c04           	movew %d4,%d6                         
    1b68:  dc86           	addl %d6,%d6                          
    1b6a:  2d6e ffd8 fff4 	movel %fp@(-40),%fp@(-12)             

  label_2:
    1b70:  70ff           	moveq #-1,%d0                         
    1b72:  2d40 fff8      	movel %d0,%fp@(-8)                    
    1b76:  486e fff8      	pea %fp@(-8)                          
    1b7a:  2f0b           	movel %a3,%sp@-                       
    1b7c:  1f03           	moveb %d3,%sp@-                       
    1b7e:  1f05           	moveb %d5,%sp@-                       
    1b80:  206a 001c      	moveal %a2@(28),%a0                   
    1b84:  3f30 6800      	movew %a0@(00000000,%d6:l),%sp@-      
    1b88:  2f0a           	movel %a2,%sp@-                       
    1b8a:  4eba 0b24      	jsr %pc@(0x26b0)                      ; PrvJobReadPBlock
    1b8e:  2d40 fffc      	movel %d0,%fp@(-4)                    
    1b92:  4aae fffc      	tstl %fp@(-4)                         
    1b96:  4fef 0012      	lea %sp@(18),%sp                      
    1b9a:  670c           	beqs 0x1ba8                           ; ↓ label_3
    1b9c:  206e 0014      	moveal %fp@(20),%a0                   
    1ba0:  4290           	clrl %a0@                             
    1ba2:  202e fffc      	movel %fp@(-4),%d0                    
    1ba6:  6044           	bras 0x1bec                           ; ↓ label_6

  label_3:
    1ba8:  5486           	addql #2,%d6                          
    1baa:  5244           	addqw #1,%d4                          
    1bac:  b847           	cmpw %d7,%d4                          
    1bae:  6304           	blss 0x1bb4                           ; ↓ label_4
    1bb0:  7000           	moveq #0,%d0                          
    1bb2:  6038           	bras 0x1bec                           ; ↓ label_6

  label_4:
    1bb4:  7000           	moveq #0,%d0                          
    1bb6:  1005           	moveb %d5,%d0                         
    1bb8:  7200           	moveq #0,%d1                          
    1bba:  1203           	moveb %d3,%d1                         
    1bbc:  9240           	subw %d0,%d1                          
    1bbe:  5241           	addqw #1,%d1                          
    1bc0:  7009           	moveq #9,%d0                          
    1bc2:  e169           	lslw %d0,%d1                          
    1bc4:  48c1           	extl %d1                              
    1bc6:  d7c1           	addal %d1,%a3                         
    1bc8:  7a00           	moveq #0,%d5                          
    1bca:  b847           	cmpw %d7,%d4                          
    1bcc:  6614           	bnes 0x1be2                           ; ↓ label_5
    1bce:  7000           	moveq #0,%d0                          
    1bd0:  302a 0014      	movew %a2@(20),%d0                    
    1bd4:  2200           	movel %d0,%d1                         
    1bd6:  202e fff4      	movel %fp@(-12),%d0                   
    1bda:  4eba 1a78      	jsr %pc@(0x3654)                      ; anonymous_function_1
    1bde:  1600           	moveb %d0,%d3                         
    1be0:  608e           	bras 0x1b70                           ; ↑ label_2

  label_5:
    1be2:  162a 0015      	moveb %a2@(21),%d3                    
    1be6:  0603 ffff      	addib #-1,%d3                         
    1bea:  6084           	bras 0x1b70                           ; ↑ label_2

  label_6:
    1bec:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    1bf0:  4e5e           	unlk %fp                              
    1bf2:  4e75           	rts                                   
```

# PrvJobWriteSector: 0x1c08 - 0x1d14

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-16)`    : ???
   * `%fp@(-17)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-21)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-26)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-32)`    : ???

```
    1c08:  4e56 ffe0      	linkw %fp,#-32                        
    1c0c:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    1c10:  246e 0008      	moveal %fp@(8),%a2                    
    1c14:  41fa 1984      	lea %pc@(0x359a),%a0                  ; ??? 0x359a
    1c18:  2d48 fff8      	movel %a0,%fp@(-8)                    
    1c1c:  266e 0010      	moveal %fp@(16),%a3                   
    1c20:  206e 0014      	moveal %fp@(20),%a0                   
    1c24:  2d50 fff0      	movel %a0@,%fp@(-16)                  
    1c28:  2d50 fff4      	movel %a0@,%fp@(-12)                  
    1c2c:  3c2a 0014      	movew %a2@(20),%d6                    
    1c30:  7000           	moveq #0,%d0                          
    1c32:  3006           	movew %d6,%d0                         
    1c34:  2e00           	movel %d0,%d7                         
    1c36:  2200           	movel %d0,%d1                         
    1c38:  202e 000c      	movel %fp@(12),%d0                    
    1c3c:  4eba 19be      	jsr %pc@(0x35fc)                      ; anonymous_function_0
    1c40:  3600           	movew %d0,%d3                         
    1c42:  202e 000c      	movel %fp@(12),%d0                    
    1c46:  2207           	movel %d7,%d1                         
    1c48:  4eba 1a0a      	jsr %pc@(0x3654)                      ; anonymous_function_1
    1c4c:  2d40 ffec      	movel %d0,%fp@(-20)                   
    1c50:  1a00           	moveb %d0,%d5                         
    1c52:  7800           	moveq #0,%d4                          
    1c54:  182e ffef      	moveb %fp@(-17),%d4                   
    1c58:  48c4           	extl %d4                              
    1c5a:  d8ae fff0      	addl %fp@(-16),%d4                    
    1c5e:  5384           	subql #1,%d4                          
    1c60:  2d44 ffe8      	movel %d4,%fp@(-24)                   
    1c64:  7000           	moveq #0,%d0                          
    1c66:  102e ffeb      	moveb %fp@(-21),%d0                   
    1c6a:  b046           	cmpw %d6,%d0                          
    1c6c:  6506           	bcss 0x1c74                           ; ↓ label_0
    1c6e:  1806           	moveb %d6,%d4                         
    1c70:  0604 ffff      	addib #-1,%d4                         

  label_0:
    1c74:  202e 000c      	movel %fp@(12),%d0                    
    1c78:  d0ae fff4      	addl %fp@(-12),%d0                    
    1c7c:  5380           	subql #1,%d0                          
    1c7e:  2d40 ffe0      	movel %d0,%fp@(-32)                   
    1c82:  2207           	movel %d7,%d1                         
    1c84:  4eba 1976      	jsr %pc@(0x35fc)                      ; anonymous_function_0
    1c88:  2d40 ffe4      	movel %d0,%fp@(-28)                   
    1c8c:  3c00           	movew %d0,%d6                         
    1c8e:  206e fff8      	moveal %fp@(-8),%a0                   
    1c92:  3028 0022      	movew %a0@(34),%d0                    
    1c96:  b06e ffe6      	cmpw %fp@(-26),%d0                    
    1c9a:  620a           	bhis 0x1ca6                           ; ↓ label_1
    1c9c:  206e 0014      	moveal %fp@(20),%a0                   
    1ca0:  4290           	clrl %a0@                             
    1ca2:  7004           	moveq #4,%d0                          
    1ca4:  6068           	bras 0x1d0e                           ; ↓ label_6

  label_1:
    1ca6:  2d6e ffe0 fffc 	movel %fp@(-32),%fp@(-4)              

  label_2:
    1cac:  2f0b           	movel %a3,%sp@-                       
    1cae:  1f04           	moveb %d4,%sp@-                       
    1cb0:  1f05           	moveb %d5,%sp@-                       
    1cb2:  3f03           	movew %d3,%sp@-                       
    1cb4:  2f0a           	movel %a2,%sp@-                       
    1cb6:  4eba 0f4c      	jsr %pc@(0x2c04)                      ; PrvJobMoveWritePBlock
    1cba:  2e00           	movel %d0,%d7                         
    1cbc:  4fef 000e      	lea %sp@(14),%sp                      
    1cc0:  670a           	beqs 0x1ccc                           ; ↓ label_3
    1cc2:  206e 0014      	moveal %fp@(20),%a0                   
    1cc6:  4290           	clrl %a0@                             
    1cc8:  2007           	movel %d7,%d0                         
    1cca:  6042           	bras 0x1d0e                           ; ↓ label_6

  label_3:
    1ccc:  5243           	addqw #1,%d3                          
    1cce:  b646           	cmpw %d6,%d3                          
    1cd0:  6304           	blss 0x1cd6                           ; ↓ label_4
    1cd2:  7000           	moveq #0,%d0                          
    1cd4:  6038           	bras 0x1d0e                           ; ↓ label_6

  label_4:
    1cd6:  7000           	moveq #0,%d0                          
    1cd8:  1005           	moveb %d5,%d0                         
    1cda:  7200           	moveq #0,%d1                          
    1cdc:  1204           	moveb %d4,%d1                         
    1cde:  9240           	subw %d0,%d1                          
    1ce0:  5241           	addqw #1,%d1                          
    1ce2:  7009           	moveq #9,%d0                          
    1ce4:  e169           	lslw %d0,%d1                          
    1ce6:  48c1           	extl %d1                              
    1ce8:  d7c1           	addal %d1,%a3                         
    1cea:  7a00           	moveq #0,%d5                          
    1cec:  b646           	cmpw %d6,%d3                          
    1cee:  6614           	bnes 0x1d04                           ; ↓ label_5
    1cf0:  7000           	moveq #0,%d0                          
    1cf2:  302a 0014      	movew %a2@(20),%d0                    
    1cf6:  2200           	movel %d0,%d1                         
    1cf8:  202e fffc      	movel %fp@(-4),%d0                    
    1cfc:  4eba 1956      	jsr %pc@(0x3654)                      ; anonymous_function_1
    1d00:  1800           	moveb %d0,%d4                         
    1d02:  60a8           	bras 0x1cac                           ; ↑ label_2

  label_5:
    1d04:  182a 0015      	moveb %a2@(21),%d4                    
    1d08:  0604 ffff      	addib #-1,%d4                         
    1d0c:  609e           	bras 0x1cac                           ; ↑ label_2

  label_6:
    1d0e:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    1d12:  4e5e           	unlk %fp                              
    1d14:  4e75           	rts                                   
```

# PrvJobFormat: 0x1d2a - 0x2062

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-22)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-26)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-34)`    : ???
   * `%fp@(-36)`    : ???
   * `%fp@(-38)`    : ???
   * `%fp@(-40)`    : ???
   * `%fp@(-42)`    : ???
   * `%fp@(-44)`    : ???
   * `%fp@(-46)`    : ???
   * `%fp@(-58)`    : ???
   * `%fp@(-60)`    : ???
   * `%fp@(-64)`    : ???
   * `%fp@(-68)`    : ???
   * `%fp@(-70)`    : ???
   * `%fp@(-88)`    : ???
   * `%fp@(-102)`   : ???
   * `%fp@(-104)`   : ???
   * `%fp@(-106)`   : ???
   * `%fp@(-110)`   : ???
   * `%fp@(-114)`   : ???
   * `%fp@(-116)`   : ???
   * `%fp@(-134)`   : ???
   * `%fp@(-148)`   : ???
   * `%fp@(-150)`   : ???
   * `%fp@(-152)`   : ???
   * `%fp@(-156)`   : ???
   * `%fp@(-160)`   : ???
   * `%fp@(-162)`   : ???
   * `%fp@(-180)`   : ???
   * `%fp@(-196)`   : ???
   * `%fp@(-198)`   : ???
   * `%fp@(-200)`   : ???

```
    1d2a:  4e56 ff38      	linkw %fp,#-200                       
    1d2e:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    1d32:  266e 0008      	moveal %fp@(8),%a3                    
    1d36:  0c6b ffff 0016 	cmpiw #-1,%a3@(22)                    
    1d3c:  6606           	bnes 0x1d44                           ; ↓ label_0
    1d3e:  7001           	moveq #1,%d0                          
    1d40:  6000 031a      	braw 0x205c                           ; ↓ label_21

  label_0:
    1d44:  382b 0016      	movew %a3@(22),%d4                    
    1d48:  5244           	addqw #1,%d4                          
    1d4a:  3a04           	movew %d4,%d5                         
    1d4c:  3605           	movew %d5,%d3                         
    1d4e:  0c45 0010      	cmpiw #16,%d5                         
    1d52:  6400 0086      	bccw 0x1dda                           ; ↓ label_4

  label_1:
    1d56:  3d7c 1601 ff96 	movew #5633,%fp@(-106)                
    1d5c:  3d43 ff98      	movew %d3,%fp@(-104)                  
    1d60:  426e ff9a      	clrw %fp@(-102)                       
    1d64:  426e ffa8      	clrw %fp@(-88)                        
    1d68:  1d7c 0001 ffba 	moveb #1,%fp@(-70)                    
    1d6e:  41fa fb9c      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    1d72:  2d48 ffbc      	movel %a0,%fp@(-68)                   
    1d76:  41ee ffc4      	lea %fp@(-60),%a0                     
    1d7a:  2d48 ffc0      	movel %a0,%fp@(-64)                   
    1d7e:  426e ffc4      	clrw %fp@(-60)                        
    1d82:  486e ff96      	pea %fp@(-106)                        
    1d86:  2078 0304      	moveal 0x304,%a0                      
    1d8a:  2068 0018      	moveal %a0@(24),%a0                   
    1d8e:  2068 009a      	moveal %a0@(154),%a0                  
    1d92:  4e90           	jsr %a0@                              
    1d94:  302e ffc4      	movew %fp@(-60),%d0                   
    1d98:  0240 03ff      	andiw #1023,%d0                       
    1d9c:  584f           	addqw #4,%sp                          
    1d9e:  670e           	beqs 0x1dae                           ; ↓ label_2
    1da0:  7001           	moveq #1,%d0                          
    1da2:  c0ab 0004      	andl %a3@(4),%d0                      
    1da6:  6628           	bnes 0x1dd0                           ; ↓ label_3
    1da8:  7002           	moveq #2,%d0                          
    1daa:  6000 02b0      	braw 0x205c                           ; ↓ label_21

  label_2:
    1dae:  41ee ffc4      	lea %fp@(-60),%a0                     
    1db2:  5488           	addql #2,%a0                          
    1db4:  2448           	moveal %a0,%a2                        
    1db6:  3010           	movew %a0@,%d0                        
    1db8:  0240 8000      	andiw #-32768,%d0                     
    1dbc:  6712           	beqs 0x1dd0                           ; ↓ label_3
    1dbe:  3012           	movew %a2@,%d0                        
    1dc0:  0240 0004      	andiw #4,%d0                          
    1dc4:  660a           	bnes 0x1dd0                           ; ↓ label_3
    1dc6:  3743 0018      	movew %d3,%a3@(24)                    
    1dca:  3803           	movew %d3,%d4                         
    1dcc:  5244           	addqw #1,%d4                          
    1dce:  600a           	bras 0x1dda                           ; ↓ label_4

  label_3:
    1dd0:  5243           	addqw #1,%d3                          
    1dd2:  0c43 0010      	cmpiw #16,%d3                         
    1dd6:  6500 ff7e      	bcsw 0x1d56                           ; ↑ label_1

  label_4:
    1dda:  4e4f           	trap #15                              
    1ddc:  a130           	0120460                               ; sysTrapEvtResetAutoOffTimer
    1dde:  3a04           	movew %d4,%d5                         
    1de0:  7e00           	moveq #0,%d7                          
    1de2:  7600           	moveq #0,%d3                          
    1de4:  283c 1080 8000 	movel #276856832,%d4                  

  label_5:
    1dea:  2044           	moveal %d4,%a0                        
    1dec:  70ff           	moveq #-1,%d0                         
    1dee:  2080           	movel %d0,%a0@                        
    1df0:  2003           	movel %d3,%d0                         
    1df2:  5280           	addql #1,%d0                          
    1df4:  e588           	lsll #2,%d0                           
    1df6:  2040           	moveal %d0,%a0                        
    1df8:  203c 1080 8000 	movel #276856832,%d0                  
    1dfe:  72ff           	moveq #-1,%d1                         
    1e00:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    1e04:  2003           	movel %d3,%d0                         
    1e06:  5480           	addql #2,%d0                          
    1e08:  e588           	lsll #2,%d0                           
    1e0a:  2040           	moveal %d0,%a0                        
    1e0c:  203c 1080 8000 	movel #276856832,%d0                  
    1e12:  72ff           	moveq #-1,%d1                         
    1e14:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    1e18:  2003           	movel %d3,%d0                         
    1e1a:  5680           	addql #3,%d0                          
    1e1c:  e588           	lsll #2,%d0                           
    1e1e:  2040           	moveal %d0,%a0                        
    1e20:  203c 1080 8000 	movel #276856832,%d0                  
    1e26:  72ff           	moveq #-1,%d1                         
    1e28:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    1e2c:  2003           	movel %d3,%d0                         
    1e2e:  5880           	addql #4,%d0                          
    1e30:  e588           	lsll #2,%d0                           
    1e32:  2040           	moveal %d0,%a0                        
    1e34:  203c 1080 8000 	movel #276856832,%d0                  
    1e3a:  72ff           	moveq #-1,%d1                         
    1e3c:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    1e40:  2003           	movel %d3,%d0                         
    1e42:  5a80           	addql #5,%d0                          
    1e44:  e588           	lsll #2,%d0                           
    1e46:  2040           	moveal %d0,%a0                        
    1e48:  203c 1080 8000 	movel #276856832,%d0                  
    1e4e:  72ff           	moveq #-1,%d1                         
    1e50:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    1e54:  2003           	movel %d3,%d0                         
    1e56:  5c80           	addql #6,%d0                          
    1e58:  e588           	lsll #2,%d0                           
    1e5a:  2040           	moveal %d0,%a0                        
    1e5c:  203c 1080 8000 	movel #276856832,%d0                  
    1e62:  72ff           	moveq #-1,%d1                         
    1e64:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    1e68:  2003           	movel %d3,%d0                         
    1e6a:  5e80           	addql #7,%d0                          
    1e6c:  e588           	lsll #2,%d0                           
    1e6e:  2040           	moveal %d0,%a0                        
    1e70:  203c 1080 8000 	movel #276856832,%d0                  
    1e76:  72ff           	moveq #-1,%d1                         
    1e78:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    1e7c:  7020           	moveq #32,%d0                         
    1e7e:  d880           	addl %d0,%d4                          
    1e80:  5083           	addql #8,%d3                          
    1e82:  0c83 0000 0080 	cmpil #128,%d3                        
    1e88:  6500 ff60      	bcsw 0x1dea                           ; ↑ label_5
    1e8c:  6000 01b6      	braw 0x2044                           ; ↓ label_20

  label_6:
    1e90:  7800           	moveq #0,%d4                          
    1e92:  7600           	moveq #0,%d3                          
    1e94:  3d6b 001a ff38 	movew %a3@(26),%fp@(-200)             
    1e9a:  3c2b 001a      	movew %a3@(26),%d6                    
    1e9e:  600e           	bras 0x1eae                           ; ↓ label_8

  label_7:
    1ea0:  204a           	moveal %a2,%a0                        
    1ea2:  d1c3           	addal %d3,%a0                         
    1ea4:  ba68 2600      	cmpw %a0@(9728),%d5                   
    1ea8:  6708           	beqs 0x1eb2                           ; ↓ label_9
    1eaa:  5483           	addql #2,%d3                          
    1eac:  5244           	addqw #1,%d4                          

  label_8:
    1eae:  b846           	cmpw %d6,%d4                          
    1eb0:  65ee           	bcss 0x1ea0                           ; ↑ label_7

  label_9:
    1eb2:  b86e ff38      	cmpw %fp@(-200),%d4                   
    1eb6:  6500 018a      	bcsw 0x2042                           ; ↓ label_19
    1eba:  3d7c 1601 ff68 	movew #5633,%fp@(-152)                
    1ec0:  3d45 ff6a      	movew %d5,%fp@(-150)                  
    1ec4:  426e ff6c      	clrw %fp@(-148)                       
    1ec8:  426e ff7a      	clrw %fp@(-134)                       
    1ecc:  1d7c 0001 ff8c 	moveb #1,%fp@(-116)                   
    1ed2:  41fa fa38      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    1ed6:  2d48 ff8e      	movel %a0,%fp@(-114)                  
    1eda:  41ee ffc4      	lea %fp@(-60),%a0                     
    1ede:  2d48 ff92      	movel %a0,%fp@(-110)                  
    1ee2:  426e ffc4      	clrw %fp@(-60)                        
    1ee6:  486e ff68      	pea %fp@(-152)                        
    1eea:  2078 0304      	moveal 0x304,%a0                      
    1eee:  2068 0018      	moveal %a0@(24),%a0                   
    1ef2:  2068 009a      	moveal %a0@(154),%a0                  
    1ef6:  4e90           	jsr %a0@                              
    1ef8:  302e ffc4      	movew %fp@(-60),%d0                   
    1efc:  0240 03ff      	andiw #1023,%d0                       
    1f00:  584f           	addqw #4,%sp                          
    1f02:  6712           	beqs 0x1f16                           ; ↓ label_12
    1f04:  7001           	moveq #1,%d0                          
    1f06:  c0ab 0004      	andl %a3@(4),%d0                      
    1f0a:  6704           	beqs 0x1f10                           ; ↓ label_10
    1f0c:  7003           	moveq #3,%d0                          
    1f0e:  6002           	bras 0x1f12                           ; ↓ label_11

  label_10:
    1f10:  7002           	moveq #2,%d0                          

  label_11:
    1f12:  6000 0148      	braw 0x205c                           ; ↓ label_21

  label_12:
    1f16:  302e ffc6      	movew %fp@(-58),%d0                   
    1f1a:  0240 8000      	andiw #-32768,%d0                     
    1f1e:  6700 0122      	beqw 0x2042                           ; ↓ label_19
    1f22:  3d7c 1201 ff3a 	movew #4609,%fp@(-198)                
    1f28:  3d45 ff3c      	movew %d5,%fp@(-196)                  
    1f2c:  426e ff4c      	clrw %fp@(-180)                       
    1f30:  1d7c 0001 ff5e 	moveb #1,%fp@(-162)                   
    1f36:  41fa f9d4      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    1f3a:  2d48 ff60      	movel %a0,%fp@(-160)                  
    1f3e:  41ee ffc4      	lea %fp@(-60),%a0                     
    1f42:  2d48 ff64      	movel %a0,%fp@(-156)                  
    1f46:  426e ffc4      	clrw %fp@(-60)                        
    1f4a:  486e ff3a      	pea %fp@(-198)                        
    1f4e:  2078 0304      	moveal 0x304,%a0                      
    1f52:  2068 0018      	moveal %a0@(24),%a0                   
    1f56:  2068 009a      	moveal %a0@(154),%a0                  
    1f5a:  4e90           	jsr %a0@                              
    1f5c:  302e ffc4      	movew %fp@(-60),%d0                   
    1f60:  0240 03ff      	andiw #1023,%d0                       
    1f64:  584f           	addqw #4,%sp                          
    1f66:  6724           	beqs 0x1f8c                           ; ↓ label_14
    1f68:  7001           	moveq #1,%d0                          
    1f6a:  c0ab 0004      	andl %a3@(4),%d0                      
    1f6e:  6606           	bnes 0x1f76                           ; ↓ label_13
    1f70:  7002           	moveq #2,%d0                          
    1f72:  6000 00e8      	braw 0x205c                           ; ↓ label_21

  label_13:
    1f76:  302e ffc4      	movew %fp@(-60),%d0                   
    1f7a:  0240 03f0      	andiw #1008,%d0                       
    1f7e:  e848           	lsrw #4,%d0                           
    1f80:  5540           	subqw #2,%d0                          
    1f82:  6600 00be      	bnew 0x2042                           ; ↓ label_19
    1f86:  7006           	moveq #6,%d0                          
    1f88:  6000 00d2      	braw 0x205c                           ; ↓ label_21

  label_14:
    1f8c:  3605           	movew %d5,%d3                         
    1f8e:  7209           	moveq #9,%d1                          
    1f90:  e26b           	lsrw %d1,%d3                          
    1f92:  3005           	movew %d5,%d0                         
    1f94:  0240 01ff      	andiw #511,%d0                        
    1f98:  660e           	bnes 0x1fa8                           ; ↓ label_15
    1f9a:  7000           	moveq #0,%d0                          
    1f9c:  3003           	movew %d3,%d0                         
    1f9e:  d080           	addl %d0,%d0                          
    1fa0:  41fa 15f8      	lea %pc@(0x359a),%a0                  ; ??? 0x359a
    1fa4:  3e30 0800      	movew %a0@(00000000,%d0:l),%d7        

  label_15:
    1fa8:  3003           	movew %d3,%d0                         
    1faa:  5240           	addqw #1,%d0                          
    1fac:  7200           	moveq #0,%d1                          
    1fae:  3200           	movew %d0,%d1                         
    1fb0:  d281           	addl %d1,%d1                          
    1fb2:  41fa 15e6      	lea %pc@(0x359a),%a0                  ; ??? 0x359a
    1fb6:  be70 1800      	cmpw %a0@(00000000,%d1:l),%d7         
    1fba:  6400 0086      	bccw 0x2042                           ; ↓ label_19
    1fbe:  3d7c 0a01 ffd2 	movew #2561,%fp@(-46)                 
    1fc4:  3d45 ffd4      	movew %d5,%fp@(-44)                   
    1fc8:  3d7c ffff ffd6 	movew #-1,%fp@(-42)                   
    1fce:  3d47 ffd8      	movew %d7,%fp@(-40)                   
    1fd2:  3d7c 4000 ffda 	movew #16384,%fp@(-38)                
    1fd8:  426e ffdc      	clrw %fp@(-36)                        
    1fdc:  426e ffde      	clrw %fp@(-34)                        
    1fe0:  3d7c c800 ffe4 	movew #-14336,%fp@(-28)               
    1fe6:  426e ffe6      	clrw %fp@(-26)                        
    1fea:  426e ffe8      	clrw %fp@(-24)                        
    1fee:  3d7c 00ff ffea 	movew #255,%fp@(-22)                  
    1ff4:  426e ffec      	clrw %fp@(-20)                        
    1ff8:  1d7c 0001 fff6 	moveb #1,%fp@(-10)                    
    1ffe:  41fa f90c      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    2002:  2d48 fff8      	movel %a0,%fp@(-8)                    
    2006:  41ee ffc4      	lea %fp@(-60),%a0                     
    200a:  2d48 fffc      	movel %a0,%fp@(-4)                    
    200e:  426e ffc4      	clrw %fp@(-60)                        
    2012:  486e ffd2      	pea %fp@(-46)                         
    2016:  2078 0304      	moveal 0x304,%a0                      
    201a:  2068 0018      	moveal %a0@(24),%a0                   
    201e:  2068 009a      	moveal %a0@(154),%a0                  
    2022:  4e90           	jsr %a0@                              
    2024:  302e ffc4      	movew %fp@(-60),%d0                   
    2028:  0240 03ff      	andiw #1023,%d0                       
    202c:  584f           	addqw #4,%sp                          
    202e:  6710           	beqs 0x2040                           ; ↓ label_18
    2030:  7001           	moveq #1,%d0                          
    2032:  c0ab 0004      	andl %a3@(4),%d0                      
    2036:  6704           	beqs 0x203c                           ; ↓ label_16
    2038:  7003           	moveq #3,%d0                          
    203a:  6002           	bras 0x203e                           ; ↓ label_17

  label_16:
    203c:  7002           	moveq #2,%d0                          

  label_17:
    203e:  601c           	bras 0x205c                           ; ↓ label_21

  label_18:
    2040:  5247           	addqw #1,%d7                          

  label_19:
    2042:  5245           	addqw #1,%d5                          

  label_20:
    2044:  246b 0020      	moveal %a3@(32),%a2                   
    2048:  ba6a 25a4      	cmpw %a2@(9636),%d5                   
    204c:  6500 fe42      	bcsw 0x1e90                           ; ↑ label_6
    2050:  4e4f           	trap #15                              
    2052:  a130           	0120460                               ; sysTrapEvtResetAutoOffTimer
    2054:  2f0b           	movel %a3,%sp@-                       
    2056:  4eba 0104      	jsr %pc@(0x215c)                      ; PrvJobMakeAddressTable
    205a:  584f           	addqw #4,%sp                          

  label_21:
    205c:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2060:  4e5e           	unlk %fp                              
    2062:  4e75           	rts                                   
```

# PrvJobDetectBootBlock: 0x2074 - 0x2142

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-44)`    : ???
   * `%fp@(-46)`    : ???
   * `%fp@(-58)`    : ???
   * `%fp@(-60)`    : ???

```
    2074:  4e56 ffc4      	linkw %fp,#-60                        
    2078:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    207c:  246e 0008      	moveal %fp@(8),%a2                    
    2080:  357c ffff 0016 	movew #-1,%a2@(22)                    
    2086:  357c ffff 0018 	movew #-1,%a2@(24)                    
    208c:  3d7c 6a01 ffd2 	movew #27137,%fp@(-46)                
    2092:  3d7c 4000 ffd4 	movew #16384,%fp@(-44)                
    2098:  426e ffe4      	clrw %fp@(-28)                        
    209c:  1d7c 0001 fff6 	moveb #1,%fp@(-10)                    
    20a2:  41fa f868      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    20a6:  2d48 fff8      	movel %a0,%fp@(-8)                    
    20aa:  41ee ffc4      	lea %fp@(-60),%a0                     
    20ae:  2d48 fffc      	movel %a0,%fp@(-4)                    
    20b2:  426e ffc4      	clrw %fp@(-60)                        
    20b6:  486e ffd2      	pea %fp@(-46)                         
    20ba:  2078 0304      	moveal 0x304,%a0                      
    20be:  2068 0018      	moveal %a0@(24),%a0                   
    20c2:  2068 009a      	moveal %a0@(154),%a0                  
    20c6:  4e90           	jsr %a0@                              
    20c8:  302e ffc4      	movew %fp@(-60),%d0                   
    20cc:  0240 000f      	andiw #15,%d0                         
    20d0:  584f           	addqw #4,%sp                          
    20d2:  6704           	beqs 0x20d8                           ; ↓ label_0
    20d4:  7003           	moveq #3,%d0                          
    20d6:  6064           	bras 0x213c                           ; ↓ label_3

  label_0:
    20d8:  4878 2a00      	pea 0x2a00                            
    20dc:  2f3c 1080 8000 	movel #276856832,%sp@-                
    20e2:  42a7           	clrl %sp@-                            
    20e4:  2f2a 0020      	movel %a2@(32),%sp@-                  
    20e8:  4e4f           	trap #15                              
    20ea:  a076           	0120166                               ; sysTrapDmWrite
    20ec:  356e ffc6 0016 	movew %fp@(-58),%a2@(22)              
    20f2:  41ee ffc4      	lea %fp@(-60),%a0                     
    20f6:  5888           	addql #4,%a0                          
    20f8:  2648           	moveal %a0,%a3                        
    20fa:  3550 000c      	movew %a0@,%a2@(12)                   
    20fe:  3013           	movew %a3@,%d0                        
    2100:  0240 8000      	andiw #-32768,%d0                     
    2104:  4fef 0010      	lea %sp@(16),%sp                      
    2108:  671e           	beqs 0x2128                           ; ↓ label_1
    210a:  2638 0304      	movel 0x304,%d3                       
    210e:  7020           	moveq #32,%d0                         
    2110:  81aa 0008      	orl %d0,%a2@(8)                       
    2114:  4a83           	tstl %d3                              
    2116:  6710           	beqs 0x2128                           ; ↓ label_1
    2118:  2043           	moveal %d3,%a0                        
    211a:  2828 0010      	movel %a0@(16),%d4                    
    211e:  6708           	beqs 0x2128                           ; ↓ label_1
    2120:  2044           	moveal %d4,%a0                        
    2122:  7008           	moveq #8,%d0                          
    2124:  81a8 001c      	orl %d0,%a0@(28)                      

  label_1:
    2128:  3013           	movew %a3@,%d0                        
    212a:  0240 4000      	andiw #16384,%d0                      
    212e:  670a           	beqs 0x213a                           ; ↓ label_2
    2130:  7040           	moveq #64,%d0                         
    2132:  81aa 0008      	orl %d0,%a2@(8)                       
    2136:  7000           	moveq #0,%d0                          
    2138:  6002           	bras 0x213c                           ; ↓ label_3

  label_2:
    213a:  7001           	moveq #1,%d0                          

  label_3:
    213c:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    2140:  4e5e           	unlk %fp                              
    2142:  4e75           	rts                                   
```

# PrvJobMakeAddressTable: 0x215c - 0x2694

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-14)`    : ???
   * `%fp@(-18)`    : ???
   * `%fp@(-22)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-42)`    : ???
   * `%fp@(-56)`    : ???
   * `%fp@(-58)`    : ???
   * `%fp@(-60)`    : ???
   * `%fp@(-64)`    : ???
   * `%fp@(-68)`    : ???
   * `%fp@(-70)`    : ???
   * `%fp@(-88)`    : ???
   * `%fp@(-104)`   : ???
   * `%fp@(-106)`   : ???
   * `%fp@(-110)`   : ???
   * `%fp@(-114)`   : ???
   * `%fp@(-116)`   : ???
   * `%fp@(-134)`   : ???
   * `%fp@(-150)`   : ???
   * `%fp@(-152)`   : ???
   * `%fp@(-156)`   : ???
   * `%fp@(-160)`   : ???
   * `%fp@(-162)`   : ???
   * `%fp@(-180)`   : ???
   * `%fp@(-196)`   : ???
   * `%fp@(-198)`   : ???
   * `%fp@(-202)`   : ???
   * `%fp@(-206)`   : ???
   * `%fp@(-208)`   : ???
   * `%fp@(-226)`   : ???
   * `%fp@(-242)`   : ???
   * `%fp@(-244)`   : ???
   * `%fp@(-248)`   : ???
   * `%fp@(-252)`   : ???
   * `%fp@(-254)`   : ???
   * `%fp@(-272)`   : ???
   * `%fp@(-288)`   : ???
   * `%fp@(-290)`   : ???
   * `%fp@(-294)`   : ???
   * `%fp@(-298)`   : ???
   * `%fp@(-300)`   : ???
   * `%fp@(-302)`   : ???
   * `%fp@(-306)`   : ???
   * `%fp@(-308)`   : ???
   * `%fp@(-312)`   : ???
   * `%fp@(-316)`   : ???
   * `%fp@(-320)`   : ???
   * `%fp@(-324)`   : ???
   * `%fp@(-328)`   : ???
   * `%fp@(-332)`   : ???
   * `%fp@(-334)`   : ???
   * `%fp@(-338)`   : ???
   * `%fp@(-340)`   : ???
   * `%fp@(-344)`   : ???
   * `%fp@(-348)`   : ???
   * `%fp@(-352)`   : ???
   * `%fp@(-356)`   : ???
   * `%fp@(-358)`   : ???
   * `%fp@(-376)`   : ???
   * `%fp@(-388)`   : ???
   * `%fp@(-390)`   : ???
   * `%fp@(-392)`   : ???
   * `%fp@(-394)`   : ???
   * `%fp@(-398)`   : ???
   * `%fp@(-412)`   : ???

```
    215c:  4e56 fe64      	linkw %fp,#-412                       
    2160:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2164:  41fa 1434      	lea %pc@(0x359a),%a0                  ; ??? 0x359a
    2168:  2d48 fe72      	movel %a0,%fp@(-398)                  
    216c:  206e 0008      	moveal %fp@(8),%a0                    
    2170:  4268 001a      	clrw %a0@(26)                         
    2174:  70ef           	moveq #-17,%d0                        
    2176:  c1a8 0008      	andl %d0,%a0@(8)                      
    217a:  4878 0400      	pea 0x400                             
    217e:  4e4f           	trap #15                              
    2180:  a013           	0120023                               ; sysTrapMemPtrNew
    2182:  2d48 fec8      	movel %a0,%fp@(-312)                  
    2186:  4aae fec8      	tstl %fp@(-312)                       
    218a:  584f           	addqw #4,%sp                          
    218c:  6606           	bnes 0x2194                           ; ↓ label_0
    218e:  7007           	moveq #7,%d0                          
    2190:  6000 04fc      	braw 0x268e                           ; ↓ label_28

  label_0:
    2194:  4878 040c      	pea 0x40c                             
    2198:  4e4f           	trap #15                              
    219a:  a013           	0120023                               ; sysTrapMemPtrNew
    219c:  2648           	moveal %a0,%a3                        
    219e:  200b           	movel %a3,%d0                         
    21a0:  584f           	addqw #4,%sp                          
    21a2:  6610           	bnes 0x21b4                           ; ↓ label_1
    21a4:  2f2e fec8      	movel %fp@(-312),%sp@-                
    21a8:  4e4f           	trap #15                              
    21aa:  a012           	0120022                               ; sysTrapMemChunkFree
    21ac:  7007           	moveq #7,%d0                          
    21ae:  584f           	addqw #4,%sp                          
    21b0:  6000 04dc      	braw 0x268e                           ; ↓ label_28

  label_1:
    21b4:  206e 0008      	moveal %fp@(8),%a0                    
    21b8:  2468 0020      	moveal %a0@(32),%a2                   
    21bc:  302a 25a4      	movew %a2@(9636),%d0                  
    21c0:  7209           	moveq #9,%d1                          
    21c2:  e268           	lsrw %d1,%d0                          
    21c4:  7200           	moveq #0,%d1                          
    21c6:  3200           	movew %d0,%d1                         
    21c8:  2d41 fea8      	movel %d1,%fp@(-344)                  
    21cc:  0c2a 0001 2578 	cmpib #1,%a2@(9592)                   
    21d2:  6706           	beqs 0x21da                           ; ↓ label_2
    21d4:  7001           	moveq #1,%d0                          
    21d6:  6000 04b6      	braw 0x268e                           ; ↓ label_28

  label_2:
    21da:  4aaa 2570      	tstl %a2@(9584)                       
    21de:  6706           	beqs 0x21e6                           ; ↓ label_3
    21e0:  7001           	moveq #1,%d0                          
    21e2:  6000 04aa      	braw 0x268e                           ; ↓ label_28

  label_3:
    21e6:  202a 2574      	movel %a2@(9588),%d0                  
    21ea:  e280           	asrl #1,%d0                           
    21ec:  206e 0008      	moveal %fp@(8),%a0                    
    21f0:  3140 001a      	movew %d0,%a0@(26)                    
    21f4:  7a00           	moveq #0,%d5                          
    21f6:  3a00           	movew %d0,%d5                         
    21f8:  2068 0020      	moveal %a0@(32),%a0                   
    21fc:  41e8 2600      	lea %a0@(9728),%a0                    
    2200:  2d48 fec0      	movel %a0,%fp@(-320)                  
    2204:  7600           	moveq #0,%d3                          
    2206:  2448           	moveal %a0,%a2                        

  label_4:
    2208:  0c52 ffff      	cmpiw #-1,%a2@                        
    220c:  670c           	beqs 0x221a                           ; ↓ label_5
    220e:  548a           	addql #2,%a2                          
    2210:  5283           	addql #1,%d3                          
    2212:  0c83 0000 0100 	cmpil #256,%d3                        
    2218:  65ee           	bcss 0x2208                           ; ↑ label_4

  label_5:
    221a:  b685           	cmpl %d5,%d3                          
    221c:  670a           	beqs 0x2228                           ; ↓ label_6
    221e:  206e 0008      	moveal %fp@(8),%a0                    
    2222:  7010           	moveq #16,%d0                         
    2224:  81a8 0008      	orl %d0,%a0@(8)                       

  label_6:
    2228:  206e 0008      	moveal %fp@(8),%a0                    
    222c:  2068 0020      	moveal %a0@(32),%a0                   
    2230:  3628 25a4      	movew %a0@(9636),%d3                  
    2234:  3d7c 1a01 fe76 	movew #6657,%fp@(-394)                
    223a:  426e fe78      	clrw %fp@(-392)                       
    223e:  3d43 fe7a      	movew %d3,%fp@(-390)                  
    2242:  3d7c 4000 fe7c 	movew #16384,%fp@(-388)               
    2248:  426e fe88      	clrw %fp@(-376)                       
    224c:  1d7c 0001 fe9a 	moveb #1,%fp@(-358)                   
    2252:  41fa f6b8      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    2256:  2d48 fe9c      	movel %a0,%fp@(-356)                  
    225a:  41ee fe64      	lea %fp@(-412),%a0                    
    225e:  2d48 fea0      	movel %a0,%fp@(-352)                  
    2262:  426e fe64      	clrw %fp@(-412)                       
    2266:  486e fe76      	pea %fp@(-394)                        
    226a:  2078 0304      	moveal 0x304,%a0                      
    226e:  2068 0018      	moveal %a0@(24),%a0                   
    2272:  2068 009a      	moveal %a0@(154),%a0                  
    2276:  4e90           	jsr %a0@                              
    2278:  302e fe64      	movew %fp@(-412),%d0                  
    227c:  0240 03ff      	andiw #1023,%d0                       
    2280:  584f           	addqw #4,%sp                          
    2282:  6716           	beqs 0x229a                           ; ↓ label_9
    2284:  7001           	moveq #1,%d0                          
    2286:  206e 0008      	moveal %fp@(8),%a0                    
    228a:  c0a8 0004      	andl %a0@(4),%d0                      
    228e:  6704           	beqs 0x2294                           ; ↓ label_7
    2290:  7001           	moveq #1,%d0                          
    2292:  6002           	bras 0x2296                           ; ↓ label_8

  label_7:
    2294:  7002           	moveq #2,%d0                          

  label_8:
    2296:  6000 03f6      	braw 0x268e                           ; ↓ label_28

  label_9:
    229a:  426e feb2      	clrw %fp@(-334)                       
    229e:  426e feac      	clrw %fp@(-340)                       
    22a2:  2d6e fe72 feb4 	movel %fp@(-398),%fp@(-332)           
    22a8:  42ae feae      	clrl %fp@(-338)                       
    22ac:  6000 03cc      	braw 0x267a                           ; ↓ label_27

  label_10:
    22b0:  7c00           	moveq #0,%d6                          
    22b2:  1f3c 00ff      	moveb #-1,%sp@-                       
    22b6:  4878 03e0      	pea 0x3e0                             
    22ba:  2f2e fec8      	movel %fp@(-312),%sp@-                
    22be:  4e4f           	trap #15                              
    22c0:  a027           	0120047                               ; sysTrapMemSet
    22c2:  204b           	moveal %a3,%a0                        
    22c4:  5088           	addql #8,%a0                          
    22c6:  5888           	addql #4,%a0                          
    22c8:  2688           	movel %a0,%a3@                        
    22ca:  426e fed4      	clrw %fp@(-300)                       
    22ce:  3d6e feac fed2 	movew %fp@(-340),%fp@(-302)           
    22d4:  2d6e feb4 feda 	movel %fp@(-332),%fp@(-294)           
    22da:  302e feb2      	movew %fp@(-334),%d0                  
    22de:  5240           	addqw #1,%d0                          
    22e0:  7200           	moveq #0,%d1                          
    22e2:  3200           	movew %d0,%d1                         
    22e4:  d281           	addl %d1,%d1                          
    22e6:  206e fe72      	moveal %fp@(-398),%a0                 
    22ea:  d1c1           	addal %d1,%a0                         
    22ec:  2d48 fec4      	movel %a0,%fp@(-316)                  
    22f0:  4fef 000a      	lea %sp@(10),%sp                      

  label_11:
    22f4:  362e fed2      	movew %fp@(-302),%d3                  
    22f8:  7800           	moveq #0,%d4                          
    22fa:  4a85           	tstl %d5                              
    22fc:  6314           	blss 0x2312                           ; ↓ label_14
    22fe:  246e fec0      	moveal %fp@(-320),%a2                 

  label_12:
    2302:  b652           	cmpw %a2@,%d3                         
    2304:  6604           	bnes 0x230a                           ; ↓ label_13
    2306:  5286           	addql #1,%d6                          
    2308:  6008           	bras 0x2312                           ; ↓ label_14

  label_13:
    230a:  548a           	addql #2,%a2                          
    230c:  5284           	addql #1,%d4                          
    230e:  b885           	cmpl %d5,%d4                          
    2310:  65f0           	bcss 0x2302                           ; ↑ label_12

  label_14:
    2312:  b885           	cmpl %d5,%d4                          
    2314:  6500 027a      	bcsw 0x2590                           ; ↓ label_24
    2318:  3003           	movew %d3,%d0                         
    231a:  e548           	lslw #2,%d0                           
    231c:  7200           	moveq #0,%d1                          
    231e:  3200           	movew %d0,%d1                         
    2320:  0681 1080 8000 	addil #276856832,%d1                  
    2326:  2e01           	movel %d1,%d7                         
    2328:  2041           	moveal %d1,%a0                        
    232a:  43ee fff2      	lea %fp@(-14),%a1                     
    232e:  5489           	addql #2,%a1                          
    2330:  2449           	moveal %a1,%a2                        
    2332:  3290           	movew %a0@,%a1@                       
    2334:  2047           	moveal %d7,%a0                        
    2336:  43ee fff2      	lea %fp@(-14),%a1                     
    233a:  5889           	addql #4,%a1                          
    233c:  2809           	movel %a1,%d4                         
    233e:  32a8 0002      	movew %a0@(2),%a1@                    
    2342:  3012           	movew %a2@,%d0                        
    2344:  0240 8000      	andiw #-32768,%d0                     
    2348:  6606           	bnes 0x2350                           ; ↓ label_15
    234a:  5286           	addql #1,%d6                          
    234c:  6000 0242      	braw 0x2590                           ; ↓ label_24

  label_15:
    2350:  3012           	movew %a2@,%d0                        
    2352:  0240 0004      	andiw #4,%d0                          
    2356:  6700 0238      	beqw 0x2590                           ; ↓ label_24
    235a:  3012           	movew %a2@,%d0                        
    235c:  0240 0008      	andiw #8,%d0                          
    2360:  6642           	bnes 0x23a4                           ; ↓ label_16
    2362:  3d7c 1201 ff96 	movew #4609,%fp@(-106)                
    2368:  3d43 ff98      	movew %d3,%fp@(-104)                  
    236c:  426e ffa8      	clrw %fp@(-88)                        
    2370:  1d7c 0001 ffba 	moveb #1,%fp@(-70)                    
    2376:  41fa f594      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    237a:  2d48 ffbc      	movel %a0,%fp@(-68)                   
    237e:  41ee fff2      	lea %fp@(-14),%a0                     
    2382:  2d48 ffc0      	movel %a0,%fp@(-64)                   
    2386:  426e fff2      	clrw %fp@(-14)                        
    238a:  486e ff96      	pea %fp@(-106)                        
    238e:  2078 0304      	moveal 0x304,%a0                      
    2392:  2068 0018      	moveal %a0@(24),%a0                   
    2396:  2068 009a      	moveal %a0@(154),%a0                  
    239a:  4e90           	jsr %a0@                              
    239c:  5286           	addql #1,%d6                          
    239e:  584f           	addqw #4,%sp                          
    23a0:  6000 01ee      	braw 0x2590                           ; ↓ label_24

  label_16:
    23a4:  2044           	moveal %d4,%a0                        
    23a6:  0c50 ffff      	cmpiw #-1,%a0@                        
    23aa:  660a           	bnes 0x23b6                           ; ↓ label_17
    23ac:  2053           	moveal %a3@,%a0                       
    23ae:  3083           	movew %d3,%a0@                        
    23b0:  5493           	addql #2,%a3@                         
    23b2:  6000 01dc      	braw 0x2590                           ; ↓ label_24

  label_17:
    23b6:  2044           	moveal %d4,%a0                        
    23b8:  3e10           	movew %a0@,%d7                        
    23ba:  206e feda      	moveal %fp@(-294),%a0                 
    23be:  3d50 fecc      	movew %a0@,%fp@(-308)                 
    23c2:  be50           	cmpw %a0@,%d7                         
    23c4:  6508           	bcss 0x23ce                           ; ↓ label_18
    23c6:  206e fec4      	moveal %fp@(-316),%a0                 
    23ca:  be50           	cmpw %a0@,%d7                         
    23cc:  6546           	bcss 0x2414                           ; ↓ label_19

  label_18:
    23ce:  3d7c 1201 ff68 	movew #4609,%fp@(-152)                
    23d4:  3d43 ff6a      	movew %d3,%fp@(-150)                  
    23d8:  426e ff7a      	clrw %fp@(-134)                       
    23dc:  1d7c 0001 ff8c 	moveb #1,%fp@(-116)                   
    23e2:  41fa f528      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    23e6:  2d48 ff8e      	movel %a0,%fp@(-114)                  
    23ea:  41ee fff2      	lea %fp@(-14),%a0                     
    23ee:  2d48 ff92      	movel %a0,%fp@(-110)                  
    23f2:  426e fff2      	clrw %fp@(-14)                        
    23f6:  486e ff68      	pea %fp@(-152)                        
    23fa:  2078 0304      	moveal 0x304,%a0                      
    23fe:  2068 0018      	moveal %a0@(24),%a0                   
    2402:  2068 009a      	moveal %a0@(154),%a0                  
    2406:  4e90           	jsr %a0@                              
    2408:  2053           	moveal %a3@,%a0                       
    240a:  3083           	movew %d3,%a0@                        
    240c:  5493           	addql #2,%a3@                         
    240e:  584f           	addqw #4,%sp                          
    2410:  6000 017e      	braw 0x2590                           ; ↓ label_24

  label_19:
    2414:  7000           	moveq #0,%d0                          
    2416:  302e fecc      	movew %fp@(-308),%d0                  
    241a:  d080           	addl %d0,%d0                          
    241c:  7200           	moveq #0,%d1                          
    241e:  3207           	movew %d7,%d1                         
    2420:  d281           	addl %d1,%d1                          
    2422:  206e fec8      	moveal %fp@(-312),%a0                 
    2426:  d1c1           	addal %d1,%a0                         
    2428:  2d48 fed6      	movel %a0,%fp@(-298)                  
    242c:  91c0           	subal %d0,%a0                         
    242e:  2d48 fece      	movel %a0,%fp@(-306)                  
    2432:  3810           	movew %a0@,%d4                        
    2434:  0c44 ffff      	cmpiw #-1,%d4                         
    2438:  6700 0150      	beqw 0x258a                           ; ↓ label_23
    243c:  3012           	movew %a2@,%d0                        
    243e:  0240 1000      	andiw #4096,%d0                       
    2442:  6700 00f0      	beqw 0x2534                           ; ↓ label_22
    2446:  3d7c 1601 ffc4 	movew #5633,%fp@(-60)                 
    244c:  3d44 ffc6      	movew %d4,%fp@(-58)                   
    2450:  426e ffc8      	clrw %fp@(-56)                        
    2454:  426e ffd6      	clrw %fp@(-42)                        
    2458:  1d7c 0001 ffe8 	moveb #1,%fp@(-24)                    
    245e:  41fa f4ac      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    2462:  2d48 ffea      	movel %a0,%fp@(-22)                   
    2466:  41ee fff2      	lea %fp@(-14),%a0                     
    246a:  2d48 ffee      	movel %a0,%fp@(-18)                   
    246e:  426e fff2      	clrw %fp@(-14)                        
    2472:  486e ffc4      	pea %fp@(-60)                         
    2476:  2078 0304      	moveal 0x304,%a0                      
    247a:  2068 0018      	moveal %a0@(24),%a0                   
    247e:  2068 009a      	moveal %a0@(154),%a0                  
    2482:  4e90           	jsr %a0@                              
    2484:  302e fff2      	movew %fp@(-14),%d0                   
    2488:  0240 03ff      	andiw #1023,%d0                       
    248c:  584f           	addqw #4,%sp                          
    248e:  6608           	bnes 0x2498                           ; ↓ label_20
    2490:  3012           	movew %a2@,%d0                        
    2492:  0240 1000      	andiw #4096,%d0                       
    2496:  6758           	beqs 0x24f0                           ; ↓ label_21

  label_20:
    2498:  3d7c 1201 ff3a 	movew #4609,%fp@(-198)                
    249e:  3d44 ff3c      	movew %d4,%fp@(-196)                  
    24a2:  426e ff4c      	clrw %fp@(-180)                       
    24a6:  1d7c 0001 ff5e 	moveb #1,%fp@(-162)                   
    24ac:  41fa f45e      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    24b0:  2d48 ff60      	movel %a0,%fp@(-160)                  
    24b4:  41ee fff2      	lea %fp@(-14),%a0                     
    24b8:  2d48 ff64      	movel %a0,%fp@(-156)                  
    24bc:  426e fff2      	clrw %fp@(-14)                        
    24c0:  486e ff3a      	pea %fp@(-198)                        
    24c4:  2078 0304      	moveal 0x304,%a0                      
    24c8:  2068 0018      	moveal %a0@(24),%a0                   
    24cc:  2068 009a      	moveal %a0@(154),%a0                  
    24d0:  4e90           	jsr %a0@                              
    24d2:  2053           	moveal %a3@,%a0                       
    24d4:  3084           	movew %d4,%a0@                        
    24d6:  5493           	addql #2,%a3@                         
    24d8:  206e feda      	moveal %fp@(-294),%a0                 
    24dc:  7000           	moveq #0,%d0                          
    24de:  3010           	movew %a0@,%d0                        
    24e0:  d080           	addl %d0,%d0                          
    24e2:  206e fed6      	moveal %fp@(-298),%a0                 
    24e6:  91c0           	subal %d0,%a0                         
    24e8:  3083           	movew %d3,%a0@                        
    24ea:  584f           	addqw #4,%sp                          
    24ec:  6000 00a2      	braw 0x2590                           ; ↓ label_24

  label_21:
    24f0:  3d7c 1201 ff0c 	movew #4609,%fp@(-244)                
    24f6:  3d43 ff0e      	movew %d3,%fp@(-242)                  
    24fa:  426e ff1e      	clrw %fp@(-226)                       
    24fe:  1d7c 0001 ff30 	moveb #1,%fp@(-208)                   
    2504:  41fa f406      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    2508:  2d48 ff32      	movel %a0,%fp@(-206)                  
    250c:  41ee fff2      	lea %fp@(-14),%a0                     
    2510:  2d48 ff36      	movel %a0,%fp@(-202)                  
    2514:  426e fff2      	clrw %fp@(-14)                        
    2518:  486e ff0c      	pea %fp@(-244)                        
    251c:  2078 0304      	moveal 0x304,%a0                      
    2520:  2068 0018      	moveal %a0@(24),%a0                   
    2524:  2068 009a      	moveal %a0@(154),%a0                  
    2528:  4e90           	jsr %a0@                              
    252a:  2053           	moveal %a3@,%a0                       
    252c:  3083           	movew %d3,%a0@                        
    252e:  5493           	addql #2,%a3@                         
    2530:  584f           	addqw #4,%sp                          
    2532:  605c           	bras 0x2590                           ; ↓ label_24

  label_22:
    2534:  3d7c 1201 fede 	movew #4609,%fp@(-290)                
    253a:  3d44 fee0      	movew %d4,%fp@(-288)                  
    253e:  426e fef0      	clrw %fp@(-272)                       
    2542:  1d7c 0001 ff02 	moveb #1,%fp@(-254)                   
    2548:  41fa f3c2      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    254c:  2d48 ff04      	movel %a0,%fp@(-252)                  
    2550:  41ee fff2      	lea %fp@(-14),%a0                     
    2554:  2d48 ff08      	movel %a0,%fp@(-248)                  
    2558:  426e fff2      	clrw %fp@(-14)                        
    255c:  486e fede      	pea %fp@(-290)                        
    2560:  2078 0304      	moveal 0x304,%a0                      
    2564:  2068 0018      	moveal %a0@(24),%a0                   
    2568:  2068 009a      	moveal %a0@(154),%a0                  
    256c:  4e90           	jsr %a0@                              
    256e:  2053           	moveal %a3@,%a0                       
    2570:  3084           	movew %d4,%a0@                        
    2572:  5493           	addql #2,%a3@                         
    2574:  206e feda      	moveal %fp@(-294),%a0                 
    2578:  7000           	moveq #0,%d0                          
    257a:  3010           	movew %a0@,%d0                        
    257c:  d080           	addl %d0,%d0                          
    257e:  206e fed6      	moveal %fp@(-298),%a0                 
    2582:  91c0           	subal %d0,%a0                         
    2584:  3083           	movew %d3,%a0@                        
    2586:  584f           	addqw #4,%sp                          
    2588:  6006           	bras 0x2590                           ; ↓ label_24

  label_23:
    258a:  206e fece      	moveal %fp@(-306),%a0                 
    258e:  3083           	movew %d3,%a0@                        

  label_24:
    2590:  526e fed2      	addqw #1,%fp@(-302)                   
    2594:  526e fed4      	addqw #1,%fp@(-300)                   
    2598:  0c6e 0200 fed4 	cmpiw #512,%fp@(-300)                 
    259e:  6500 fd54      	bcsw 0x22f4                           ; ↑ label_11
    25a2:  204b           	moveal %a3,%a0                        
    25a4:  5088           	addql #8,%a0                          
    25a6:  5888           	addql #4,%a0                          
    25a8:  2253           	moveal %a3@,%a1                       
    25aa:  93c8           	subal %a0,%a1                         
    25ac:  2009           	movel %a1,%d0                         
    25ae:  e088           	lsrl #8,%d0                           
    25b0:  e088           	lsrl #8,%d0                           
    25b2:  e088           	lsrl #8,%d0                           
    25b4:  ee88           	lsrl #7,%d0                           
    25b6:  d089           	addl %a1,%d0                          
    25b8:  e280           	asrl #1,%d0                           
    25ba:  2740 0004      	movel %d0,%a3@(4)                     
    25be:  202e fea8      	movel %fp@(-344),%d0                  
    25c2:  5380           	subql #1,%d0                          
    25c4:  b0ae fea4      	cmpl %fp@(-348),%d0                   
    25c8:  6612           	bnes 0x25dc                           ; ↓ label_25
    25ca:  53ab 0004      	subql #1,%a3@(4)                      
    25ce:  5286           	addql #1,%d6                          
    25d0:  202b 0004      	movel %a3@(4),%d0                     
    25d4:  d080           	addl %d0,%d0                          
    25d6:  37bc ffff 080c 	movew #-1,%a3@(0000000c,%d0:l)        

  label_25:
    25dc:  7010           	moveq #16,%d0                         
    25de:  bc80           	cmpl %d0,%d6                          
    25e0:  6d0a           	blts 0x25ec                           ; ↓ label_26
    25e2:  206e 0008      	moveal %fp@(8),%a0                    
    25e6:  7010           	moveq #16,%d0                         
    25e8:  81a8 0008      	orl %d0,%a0@(8)                       

  label_26:
    25ec:  2746 0008      	movel %d6,%a3@(8)                     
    25f0:  206e 0008      	moveal %fp@(8),%a0                    
    25f4:  2d68 001c febc 	movel %a0@(28),%fp@(-324)             
    25fa:  4878 03e0      	pea 0x3e0                             
    25fe:  2f2e fec8      	movel %fp@(-312),%sp@-                
    2602:  206e feb4      	moveal %fp@(-332),%a0                 
    2606:  7000           	moveq #0,%d0                          
    2608:  3010           	movew %a0@,%d0                        
    260a:  d080           	addl %d0,%d0                          
    260c:  206e febc      	moveal %fp@(-324),%a0                 
    2610:  d1c0           	addal %d0,%a0                         
    2612:  2008           	movel %a0,%d0                         
    2614:  90ae febc      	subl %fp@(-324),%d0                   
    2618:  2f00           	movel %d0,%sp@-                       
    261a:  2f2e febc      	movel %fp@(-324),%sp@-                
    261e:  4e4f           	trap #15                              
    2620:  a076           	0120166                               ; sysTrapDmWrite
    2622:  206e 0008      	moveal %fp@(8),%a0                    
    2626:  2068 001c      	moveal %a0@(28),%a0                   
    262a:  d1ee feae      	addal %fp@(-338),%a0                  
    262e:  41e8 400c      	lea %a0@(16396),%a0                   
    2632:  2688           	movel %a0,%a3@                        
    2634:  206e 0008      	moveal %fp@(8),%a0                    
    2638:  2d68 001c feb8 	movel %a0@(28),%fp@(-328)             
    263e:  4878 040c      	pea 0x40c                             
    2642:  2f0b           	movel %a3,%sp@-                       
    2644:  206e feb8      	moveal %fp@(-328),%a0                 
    2648:  d1ee feae      	addal %fp@(-338),%a0                  
    264c:  41e8 4000      	lea %a0@(16384),%a0                   
    2650:  2008           	movel %a0,%d0                         
    2652:  90ae feb8      	subl %fp@(-328),%d0                   
    2656:  2f00           	movel %d0,%sp@-                       
    2658:  2f2e feb8      	movel %fp@(-328),%sp@-                
    265c:  4e4f           	trap #15                              
    265e:  a076           	0120166                               ; sysTrapDmWrite
    2660:  066e 0200 feac 	addiw #512,%fp@(-340)                 
    2666:  54ae feb4      	addql #2,%fp@(-332)                   
    266a:  06ae 0000 040c 	addil #1036,%fp@(-338)                
    2670:  feae                                                  
    2672:  526e feb2      	addqw #1,%fp@(-334)                   
    2676:  4fef 0020      	lea %sp@(32),%sp                      

  label_27:
    267a:  7000           	moveq #0,%d0                          
    267c:  302e feb2      	movew %fp@(-334),%d0                  
    2680:  2d40 fea4      	movel %d0,%fp@(-348)                  
    2684:  b0ae fea8      	cmpl %fp@(-344),%d0                   
    2688:  6d00 fc26      	bltw 0x22b0                           ; ↑ label_10
    268c:  7000           	moveq #0,%d0                          

  label_28:
    268e:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2692:  4e5e           	unlk %fp                              
    2694:  4e75           	rts                                   
```

# PrvJobReadPBlock: 0x26b0 - 0x2bee

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(18)`     : ???
   * `%fp@(22)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-16)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-32)`    : ???
   * `%fp@(-36)`    : ???
   * `%fp@(-38)`    : ???
   * `%fp@(-56)`    : ???
   * `%fp@(-66)`    : ???
   * `%fp@(-68)`    : ???
   * `%fp@(-70)`    : ???
   * `%fp@(-72)`    : ???
   * `%fp@(-74)`    : ???
   * `%fp@(-84)`    : ???
   * `%fp@(-88)`    : ???
   * `%fp@(-92)`    : ???
   * `%fp@(-96)`    : ???
   * `%fp@(-98)`    : ???
   * `%fp@(-116)`   : ???
   * `%fp@(-130)`   : ???
   * `%fp@(-132)`   : ???
   * `%fp@(-134)`   : ???
   * `%fp@(-138)`   : ???

```
    26b0:  4e56 ff76      	linkw %fp,#-138                       
    26b4:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    26b8:  1e2e 000e      	moveb %fp@(14),%d7                    
    26bc:  1c2e 0010      	moveb %fp@(16),%d6                    
    26c0:  266e 0012      	moveal %fp@(18),%a3                   
    26c4:  0c6e ffff 000c 	cmpiw #-1,%fp@(12)                    
    26ca:  6626           	bnes 0x26f2                           ; ↓ label_0
    26cc:  4227           	clrb %sp@-                            
    26ce:  7000           	moveq #0,%d0                          
    26d0:  1007           	moveb %d7,%d0                         
    26d2:  7200           	moveq #0,%d1                          
    26d4:  1206           	moveb %d6,%d1                         
    26d6:  9240           	subw %d0,%d1                          
    26d8:  5241           	addqw #1,%d1                          
    26da:  7009           	moveq #9,%d0                          
    26dc:  e169           	lslw %d0,%d1                          
    26de:  48c1           	extl %d1                              
    26e0:  2f01           	movel %d1,%sp@-                       
    26e2:  2f0b           	movel %a3,%sp@-                       
    26e4:  4e4f           	trap #15                              
    26e6:  a027           	0120047                               ; sysTrapMemSet
    26e8:  7000           	moveq #0,%d0                          
    26ea:  4fef 000a      	lea %sp@(10),%sp                      
    26ee:  6000 04f8      	braw 0x2be8                           ; ↓ label_21

  label_0:
    26f2:  3d7c 1601 ff7a 	movew #5633,%fp@(-134)                
    26f8:  3d6e 000c ff7c 	movew %fp@(12),%fp@(-132)             
    26fe:  7000           	moveq #0,%d0                          
    2700:  1007           	moveb %d7,%d0                         
    2702:  3a00           	movew %d0,%d5                         
    2704:  3d40 ff7e      	movew %d0,%fp@(-130)                  
    2708:  426e ff8c      	clrw %fp@(-116)                       
    270c:  1d7c 0001 ff9e 	moveb #1,%fp@(-98)                    
    2712:  41fa f1f8      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    2716:  2d48 ffa0      	movel %a0,%fp@(-96)                   
    271a:  41ee ffa8      	lea %fp@(-88),%a0                     
    271e:  2d48 ffa4      	movel %a0,%fp@(-92)                   
    2722:  426e ffa8      	clrw %fp@(-88)                        
    2726:  486e ff7a      	pea %fp@(-134)                        
    272a:  2078 0304      	moveal 0x304,%a0                      
    272e:  2068 0018      	moveal %a0@(24),%a0                   
    2732:  2068 009a      	moveal %a0@(154),%a0                  
    2736:  4e90           	jsr %a0@                              
    2738:  302e ffa8      	movew %fp@(-88),%d0                   
    273c:  0240 03ff      	andiw #1023,%d0                       
    2740:  584f           	addqw #4,%sp                          
    2742:  6716           	beqs 0x275a                           ; ↓ label_3
    2744:  7001           	moveq #1,%d0                          
    2746:  206e 0008      	moveal %fp@(8),%a0                    
    274a:  c0a8 0004      	andl %a0@(4),%d0                      
    274e:  6704           	beqs 0x2754                           ; ↓ label_1
    2750:  7003           	moveq #3,%d0                          
    2752:  6002           	bras 0x2756                           ; ↓ label_2

  label_1:
    2754:  7002           	moveq #2,%d0                          

  label_2:
    2756:  6000 0490      	braw 0x2be8                           ; ↓ label_21

  label_3:
    275a:  41ee ffa8      	lea %fp@(-88),%a0                     
    275e:  5488           	addql #2,%a0                          
    2760:  2448           	moveal %a0,%a2                        
    2762:  3010           	movew %a0@,%d0                        
    2764:  0240 0030      	andiw #48,%d0                         
    2768:  0c40 0030      	cmpiw #48,%d0                         
    276c:  6726           	beqs 0x2794                           ; ↓ label_4
    276e:  4227           	clrb %sp@-                            
    2770:  7000           	moveq #0,%d0                          
    2772:  1007           	moveb %d7,%d0                         
    2774:  7200           	moveq #0,%d1                          
    2776:  1206           	moveb %d6,%d1                         
    2778:  9240           	subw %d0,%d1                          
    277a:  5241           	addqw #1,%d1                          
    277c:  7009           	moveq #9,%d0                          
    277e:  e169           	lslw %d0,%d1                          
    2780:  48c1           	extl %d1                              
    2782:  2f01           	movel %d1,%sp@-                       
    2784:  2f0b           	movel %a3,%sp@-                       
    2786:  4e4f           	trap #15                              
    2788:  a027           	0120047                               ; sysTrapMemSet
    278a:  7000           	moveq #0,%d0                          
    278c:  4fef 000a      	lea %sp@(10),%sp                      
    2790:  6000 0456      	braw 0x2be8                           ; ↓ label_21

  label_4:
    2794:  7600           	moveq #0,%d3                          
    2796:  383c 4000      	movew #16384,%d4                      
    279a:  b7fc 1080 8000 	cmpal #276856832,%a3                  
    27a0:  651a           	bcss 0x27bc                           ; ↓ label_5
    27a2:  b7fc 1081 0000 	cmpal #276889600,%a3                  
    27a8:  6412           	bccs 0x27bc                           ; ↓ label_5
    27aa:  204b           	moveal %a3,%a0                        
    27ac:  d1fc ef7f 8000 	addal #-276856832,%a0                 
    27b2:  2808           	movel %a0,%d4                         
    27b4:  e24c           	lsrw #1,%d4                           
    27b6:  0644 4000      	addiw #16384,%d4                      
    27ba:  7601           	moveq #1,%d3                          

  label_5:
    27bc:  3d7c 0601 ffb6 	movew #1537,%fp@(-74)                 
    27c2:  3d6e 000c ffb8 	movew %fp@(12),%fp@(-72)              
    27c8:  3d44 ffba      	movew %d4,%fp@(-70)                   
    27cc:  3d45 ffbc      	movew %d5,%fp@(-68)                   
    27d0:  7000           	moveq #0,%d0                          
    27d2:  1006           	moveb %d6,%d0                         
    27d4:  3d40 ffbe      	movew %d0,%fp@(-66)                   
    27d8:  426e ffc8      	clrw %fp@(-56)                        
    27dc:  1d7c 0001 ffda 	moveb #1,%fp@(-38)                    
    27e2:  41fa f128      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    27e6:  2d48 ffdc      	movel %a0,%fp@(-36)                   
    27ea:  41ee ffa8      	lea %fp@(-88),%a0                     
    27ee:  2d48 ffe0      	movel %a0,%fp@(-32)                   
    27f2:  426e ffa8      	clrw %fp@(-88)                        
    27f6:  486e ffb6      	pea %fp@(-74)                         
    27fa:  2078 0304      	moveal 0x304,%a0                      
    27fe:  2068 0018      	moveal %a0@(24),%a0                   
    2802:  2068 009a      	moveal %a0@(154),%a0                  
    2806:  4e90           	jsr %a0@                              
    2808:  302e ffa8      	movew %fp@(-88),%d0                   
    280c:  0240 03ff      	andiw #1023,%d0                       
    2810:  584f           	addqw #4,%sp                          
    2812:  6716           	beqs 0x282a                           ; ↓ label_8
    2814:  7001           	moveq #1,%d0                          
    2816:  206e 0008      	moveal %fp@(8),%a0                    
    281a:  c0a8 0004      	andl %a0@(4),%d0                      
    281e:  6704           	beqs 0x2824                           ; ↓ label_6
    2820:  7003           	moveq #3,%d0                          
    2822:  6002           	bras 0x2826                           ; ↓ label_7

  label_6:
    2824:  7002           	moveq #2,%d0                          

  label_7:
    2826:  6000 03c0      	braw 0x2be8                           ; ↓ label_21

  label_8:
    282a:  3012           	movew %a2@,%d0                        
    282c:  4240           	clrw %d0                              
    282e:  806e ffac      	orw %fp@(-84),%d0                     
    2832:  7800           	moveq #0,%d4                          
    2834:  3800           	movew %d0,%d4                         
    2836:  4a84           	tstl %d4                              
    2838:  6714           	beqs 0x284e                           ; ↓ label_10
    283a:  206e 0016      	moveal %fp@(22),%a0                   
    283e:  4a90           	tstl %a0@                             
    2840:  6706           	beqs 0x2848                           ; ↓ label_9
    2842:  7001           	moveq #1,%d0                          
    2844:  6000 03a2      	braw 0x2be8                           ; ↓ label_21

  label_9:
    2848:  206e 0016      	moveal %fp@(22),%a0                   
    284c:  2084           	movel %d4,%a0@                        

  label_10:
    284e:  4a03           	tstb %d3                              
    2850:  6600 036e      	bnew 0x2bc0                           ; ↓ label_18
    2854:  7000           	moveq #0,%d0                          
    2856:  1007           	moveb %d7,%d0                         
    2858:  7a00           	moveq #0,%d5                          
    285a:  1a06           	moveb %d6,%d5                         
    285c:  9a40           	subw %d0,%d5                          
    285e:  5245           	addqw #1,%d5                          
    2860:  7009           	moveq #9,%d0                          
    2862:  e16d           	lslw %d0,%d5                          
    2864:  48c5           	extl %d5                              
    2866:  e48d           	lsrl #2,%d5                           
    2868:  7001           	moveq #1,%d0                          
    286a:  220b           	movel %a3,%d1                         
    286c:  c280           	andl %d0,%d1                          
    286e:  6700 0284      	beqw 0x2af4                           ; ↓ label_14
    2872:  7600           	moveq #0,%d3                          
    2874:  4a85           	tstl %d5                              
    2876:  6300 0348      	blsw 0x2bc0                           ; ↓ label_18
    287a:  2005           	movel %d5,%d0                         
    287c:  5180           	subql #8,%d0                          
    287e:  2d40 ff76      	movel %d0,%fp@(-138)                  
    2882:  7008           	moveq #8,%d0                          
    2884:  ba80           	cmpl %d0,%d5                          
    2886:  6300 033c      	blsw 0x2bc4                           ; ↓ label_19
    288a:  7e00           	moveq #0,%d7                          
    288c:  2c3c 1080 8000 	movel #276856832,%d6                  

  label_11:
    2892:  2046           	moveal %d6,%a0                        
    2894:  2810           	movel %a0@,%d4                        
    2896:  2004           	movel %d4,%d0                         
    2898:  7218           	moveq #24,%d1                         
    289a:  e2a8           	lsrl %d1,%d0                          
    289c:  0280 0000 00ff 	andil #255,%d0                        
    28a2:  204b           	moveal %a3,%a0                        
    28a4:  d1c7           	addal %d7,%a0                         
    28a6:  2448           	moveal %a0,%a2                        
    28a8:  1080           	moveb %d0,%a0@                        
    28aa:  2004           	movel %d4,%d0                         
    28ac:  e088           	lsrl #8,%d0                           
    28ae:  0280 0000 ffff 	andil #65535,%d0                      
    28b4:  3540 0001      	movew %d0,%a2@(1)                     
    28b8:  2004           	movel %d4,%d0                         
    28ba:  0280 0000 00ff 	andil #255,%d0                        
    28c0:  1540 0003      	moveb %d0,%a2@(3)                     
    28c4:  2003           	movel %d3,%d0                         
    28c6:  5280           	addql #1,%d0                          
    28c8:  e588           	lsll #2,%d0                           
    28ca:  2440           	moveal %d0,%a2                        
    28cc:  2d40 fffc      	movel %d0,%fp@(-4)                    
    28d0:  203c 1080 8000 	movel #276856832,%d0                  
    28d6:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    28da:  2004           	movel %d4,%d0                         
    28dc:  e2a8           	lsrl %d1,%d0                          
    28de:  0280 0000 00ff 	andil #255,%d0                        
    28e4:  204b           	moveal %a3,%a0                        
    28e6:  d1ee fffc      	addal %fp@(-4),%a0                    
    28ea:  2448           	moveal %a0,%a2                        
    28ec:  1080           	moveb %d0,%a0@                        
    28ee:  2004           	movel %d4,%d0                         
    28f0:  e088           	lsrl #8,%d0                           
    28f2:  0280 0000 ffff 	andil #65535,%d0                      
    28f8:  3540 0001      	movew %d0,%a2@(1)                     
    28fc:  2004           	movel %d4,%d0                         
    28fe:  0280 0000 00ff 	andil #255,%d0                        
    2904:  1540 0003      	moveb %d0,%a2@(3)                     
    2908:  2003           	movel %d3,%d0                         
    290a:  5480           	addql #2,%d0                          
    290c:  e588           	lsll #2,%d0                           
    290e:  2440           	moveal %d0,%a2                        
    2910:  2d40 ffe4      	movel %d0,%fp@(-28)                   
    2914:  203c 1080 8000 	movel #276856832,%d0                  
    291a:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    291e:  2004           	movel %d4,%d0                         
    2920:  e2a8           	lsrl %d1,%d0                          
    2922:  0280 0000 00ff 	andil #255,%d0                        
    2928:  204b           	moveal %a3,%a0                        
    292a:  d1ee ffe4      	addal %fp@(-28),%a0                   
    292e:  2448           	moveal %a0,%a2                        
    2930:  1080           	moveb %d0,%a0@                        
    2932:  2004           	movel %d4,%d0                         
    2934:  e088           	lsrl #8,%d0                           
    2936:  0280 0000 ffff 	andil #65535,%d0                      
    293c:  3540 0001      	movew %d0,%a2@(1)                     
    2940:  2004           	movel %d4,%d0                         
    2942:  0280 0000 00ff 	andil #255,%d0                        
    2948:  1540 0003      	moveb %d0,%a2@(3)                     
    294c:  2003           	movel %d3,%d0                         
    294e:  5680           	addql #3,%d0                          
    2950:  e588           	lsll #2,%d0                           
    2952:  2440           	moveal %d0,%a2                        
    2954:  2d40 ffe8      	movel %d0,%fp@(-24)                   
    2958:  203c 1080 8000 	movel #276856832,%d0                  
    295e:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    2962:  2004           	movel %d4,%d0                         
    2964:  e2a8           	lsrl %d1,%d0                          
    2966:  0280 0000 00ff 	andil #255,%d0                        
    296c:  204b           	moveal %a3,%a0                        
    296e:  d1ee ffe8      	addal %fp@(-24),%a0                   
    2972:  2448           	moveal %a0,%a2                        
    2974:  1080           	moveb %d0,%a0@                        
    2976:  2004           	movel %d4,%d0                         
    2978:  e088           	lsrl #8,%d0                           
    297a:  0280 0000 ffff 	andil #65535,%d0                      
    2980:  3540 0001      	movew %d0,%a2@(1)                     
    2984:  2004           	movel %d4,%d0                         
    2986:  0280 0000 00ff 	andil #255,%d0                        
    298c:  1540 0003      	moveb %d0,%a2@(3)                     
    2990:  2003           	movel %d3,%d0                         
    2992:  5880           	addql #4,%d0                          
    2994:  e588           	lsll #2,%d0                           
    2996:  2440           	moveal %d0,%a2                        
    2998:  2d40 ffec      	movel %d0,%fp@(-20)                   
    299c:  203c 1080 8000 	movel #276856832,%d0                  
    29a2:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    29a6:  2004           	movel %d4,%d0                         
    29a8:  e2a8           	lsrl %d1,%d0                          
    29aa:  0280 0000 00ff 	andil #255,%d0                        
    29b0:  204b           	moveal %a3,%a0                        
    29b2:  d1ee ffec      	addal %fp@(-20),%a0                   
    29b6:  2448           	moveal %a0,%a2                        
    29b8:  1080           	moveb %d0,%a0@                        
    29ba:  2004           	movel %d4,%d0                         
    29bc:  e088           	lsrl #8,%d0                           
    29be:  0280 0000 ffff 	andil #65535,%d0                      
    29c4:  3540 0001      	movew %d0,%a2@(1)                     
    29c8:  2004           	movel %d4,%d0                         
    29ca:  0280 0000 00ff 	andil #255,%d0                        
    29d0:  1540 0003      	moveb %d0,%a2@(3)                     
    29d4:  2003           	movel %d3,%d0                         
    29d6:  5a80           	addql #5,%d0                          
    29d8:  e588           	lsll #2,%d0                           
    29da:  2440           	moveal %d0,%a2                        
    29dc:  2d40 fff0      	movel %d0,%fp@(-16)                   
    29e0:  203c 1080 8000 	movel #276856832,%d0                  
    29e6:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    29ea:  2004           	movel %d4,%d0                         
    29ec:  e2a8           	lsrl %d1,%d0                          
    29ee:  0280 0000 00ff 	andil #255,%d0                        
    29f4:  204b           	moveal %a3,%a0                        
    29f6:  d1ee fff0      	addal %fp@(-16),%a0                   
    29fa:  2448           	moveal %a0,%a2                        
    29fc:  1080           	moveb %d0,%a0@                        
    29fe:  2004           	movel %d4,%d0                         
    2a00:  e088           	lsrl #8,%d0                           
    2a02:  0280 0000 ffff 	andil #65535,%d0                      
    2a08:  3540 0001      	movew %d0,%a2@(1)                     
    2a0c:  2004           	movel %d4,%d0                         
    2a0e:  0280 0000 00ff 	andil #255,%d0                        
    2a14:  1540 0003      	moveb %d0,%a2@(3)                     
    2a18:  2003           	movel %d3,%d0                         
    2a1a:  5c80           	addql #6,%d0                          
    2a1c:  e588           	lsll #2,%d0                           
    2a1e:  2440           	moveal %d0,%a2                        
    2a20:  2d40 fff4      	movel %d0,%fp@(-12)                   
    2a24:  203c 1080 8000 	movel #276856832,%d0                  
    2a2a:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    2a2e:  2004           	movel %d4,%d0                         
    2a30:  e2a8           	lsrl %d1,%d0                          
    2a32:  0280 0000 00ff 	andil #255,%d0                        
    2a38:  204b           	moveal %a3,%a0                        
    2a3a:  d1ee fff4      	addal %fp@(-12),%a0                   
    2a3e:  2448           	moveal %a0,%a2                        
    2a40:  1080           	moveb %d0,%a0@                        
    2a42:  2004           	movel %d4,%d0                         
    2a44:  e088           	lsrl #8,%d0                           
    2a46:  0280 0000 ffff 	andil #65535,%d0                      
    2a4c:  3540 0001      	movew %d0,%a2@(1)                     
    2a50:  2004           	movel %d4,%d0                         
    2a52:  0280 0000 00ff 	andil #255,%d0                        
    2a58:  1540 0003      	moveb %d0,%a2@(3)                     
    2a5c:  2003           	movel %d3,%d0                         
    2a5e:  5e80           	addql #7,%d0                          
    2a60:  e588           	lsll #2,%d0                           
    2a62:  2d40 fff8      	movel %d0,%fp@(-8)                    
    2a66:  2040           	moveal %d0,%a0                        
    2a68:  203c 1080 8000 	movel #276856832,%d0                  
    2a6e:  2830 0800      	movel %a0@(00000000,%d0:l),%d4        
    2a72:  2004           	movel %d4,%d0                         
    2a74:  e2a8           	lsrl %d1,%d0                          
    2a76:  0280 0000 00ff 	andil #255,%d0                        
    2a7c:  204b           	moveal %a3,%a0                        
    2a7e:  d1ee fff8      	addal %fp@(-8),%a0                    
    2a82:  2448           	moveal %a0,%a2                        
    2a84:  1080           	moveb %d0,%a0@                        
    2a86:  2004           	movel %d4,%d0                         
    2a88:  e088           	lsrl #8,%d0                           
    2a8a:  0280 0000 ffff 	andil #65535,%d0                      
    2a90:  3540 0001      	movew %d0,%a2@(1)                     
    2a94:  2004           	movel %d4,%d0                         
    2a96:  0280 0000 00ff 	andil #255,%d0                        
    2a9c:  1540 0003      	moveb %d0,%a2@(3)                     
    2aa0:  7020           	moveq #32,%d0                         
    2aa2:  de80           	addl %d0,%d7                          
    2aa4:  dc80           	addl %d0,%d6                          
    2aa6:  5083           	addql #8,%d3                          
    2aa8:  b6ae ff76      	cmpl %fp@(-138),%d3                   
    2aac:  6500 fde4      	bcsw 0x2892                           ; ↑ label_11
    2ab0:  6000 0112      	braw 0x2bc4                           ; ↓ label_19

  label_12:
    2ab4:  2046           	moveal %d6,%a0                        
    2ab6:  2810           	movel %a0@,%d4                        
    2ab8:  2004           	movel %d4,%d0                         
    2aba:  7218           	moveq #24,%d1                         
    2abc:  e2a8           	lsrl %d1,%d0                          
    2abe:  0280 0000 00ff 	andil #255,%d0                        
    2ac4:  204b           	moveal %a3,%a0                        
    2ac6:  d1c7           	addal %d7,%a0                         
    2ac8:  2448           	moveal %a0,%a2                        
    2aca:  1080           	moveb %d0,%a0@                        
    2acc:  2004           	movel %d4,%d0                         
    2ace:  e088           	lsrl #8,%d0                           
    2ad0:  0280 0000 ffff 	andil #65535,%d0                      
    2ad6:  3540 0001      	movew %d0,%a2@(1)                     
    2ada:  2004           	movel %d4,%d0                         
    2adc:  0280 0000 00ff 	andil #255,%d0                        
    2ae2:  1540 0003      	moveb %d0,%a2@(3)                     
    2ae6:  5887           	addql #4,%d7                          
    2ae8:  5886           	addql #4,%d6                          
    2aea:  5283           	addql #1,%d3                          

  label_13:
    2aec:  b685           	cmpl %d5,%d3                          
    2aee:  65c4           	bcss 0x2ab4                           ; ↑ label_12
    2af0:  6000 00ce      	braw 0x2bc0                           ; ↓ label_18

  label_14:
    2af4:  7600           	moveq #0,%d3                          
    2af6:  4a85           	tstl %d5                              
    2af8:  6300 00c6      	blsw 0x2bc0                           ; ↓ label_18
    2afc:  2c05           	movel %d5,%d6                         
    2afe:  5186           	subql #8,%d6                          
    2b00:  7008           	moveq #8,%d0                          
    2b02:  ba80           	cmpl %d0,%d5                          
    2b04:  6300 00d0      	blsw 0x2bd6                           ; ↓ label_20
    2b08:  283c 1080 8000 	movel #276856832,%d4                  
    2b0e:  244b           	moveal %a3,%a2                        

  label_15:
    2b10:  2044           	moveal %d4,%a0                        
    2b12:  2490           	movel %a0@,%a2@                       
    2b14:  2003           	movel %d3,%d0                         
    2b16:  5280           	addql #1,%d0                          
    2b18:  e588           	lsll #2,%d0                           
    2b1a:  2040           	moveal %d0,%a0                        
    2b1c:  203c 1080 8000 	movel #276856832,%d0                  
    2b22:  2570 0800 0004 	movel %a0@(00000000,%d0:l),%a2@(4)    
    2b28:  2003           	movel %d3,%d0                         
    2b2a:  5480           	addql #2,%d0                          
    2b2c:  e588           	lsll #2,%d0                           
    2b2e:  2040           	moveal %d0,%a0                        
    2b30:  203c 1080 8000 	movel #276856832,%d0                  
    2b36:  2570 0800 0008 	movel %a0@(00000000,%d0:l),%a2@(8)    
    2b3c:  2003           	movel %d3,%d0                         
    2b3e:  5680           	addql #3,%d0                          
    2b40:  e588           	lsll #2,%d0                           
    2b42:  2040           	moveal %d0,%a0                        
    2b44:  203c 1080 8000 	movel #276856832,%d0                  
    2b4a:  2570 0800 000c 	movel %a0@(00000000,%d0:l),%a2@(12)   
    2b50:  2003           	movel %d3,%d0                         
    2b52:  5880           	addql #4,%d0                          
    2b54:  e588           	lsll #2,%d0                           
    2b56:  2040           	moveal %d0,%a0                        
    2b58:  203c 1080 8000 	movel #276856832,%d0                  
    2b5e:  2570 0800 0010 	movel %a0@(00000000,%d0:l),%a2@(16)   
    2b64:  2003           	movel %d3,%d0                         
    2b66:  5a80           	addql #5,%d0                          
    2b68:  e588           	lsll #2,%d0                           
    2b6a:  2040           	moveal %d0,%a0                        
    2b6c:  203c 1080 8000 	movel #276856832,%d0                  
    2b72:  2570 0800 0014 	movel %a0@(00000000,%d0:l),%a2@(20)   
    2b78:  2003           	movel %d3,%d0                         
    2b7a:  5c80           	addql #6,%d0                          
    2b7c:  e588           	lsll #2,%d0                           
    2b7e:  2040           	moveal %d0,%a0                        
    2b80:  203c 1080 8000 	movel #276856832,%d0                  
    2b86:  2570 0800 0018 	movel %a0@(00000000,%d0:l),%a2@(24)   
    2b8c:  2003           	movel %d3,%d0                         
    2b8e:  5e80           	addql #7,%d0                          
    2b90:  e588           	lsll #2,%d0                           
    2b92:  2040           	moveal %d0,%a0                        
    2b94:  203c 1080 8000 	movel #276856832,%d0                  
    2b9a:  2570 0800 001c 	movel %a0@(00000000,%d0:l),%a2@(28)   
    2ba0:  7020           	moveq #32,%d0                         
    2ba2:  d880           	addl %d0,%d4                          
    2ba4:  45ea 0020      	lea %a2@(32),%a2                      
    2ba8:  5083           	addql #8,%d3                          
    2baa:  b686           	cmpl %d6,%d3                          
    2bac:  6500 ff62      	bcsw 0x2b10                           ; ↑ label_15
    2bb0:  6024           	bras 0x2bd6                           ; ↓ label_20

  label_16:
    2bb2:  2044           	moveal %d4,%a0                        
    2bb4:  2490           	movel %a0@,%a2@                       
    2bb6:  5884           	addql #4,%d4                          
    2bb8:  588a           	addql #4,%a2                          
    2bba:  5283           	addql #1,%d3                          

  label_17:
    2bbc:  b685           	cmpl %d5,%d3                          
    2bbe:  65f2           	bcss 0x2bb2                           ; ↑ label_16

  label_18:
    2bc0:  7000           	moveq #0,%d0                          
    2bc2:  6024           	bras 0x2be8                           ; ↓ label_21

  label_19:
    2bc4:  2e03           	movel %d3,%d7                         
    2bc6:  e58f           	lsll #2,%d7                           
    2bc8:  2807           	movel %d7,%d4                         
    2bca:  2c04           	movel %d4,%d6                         
    2bcc:  0686 1080 8000 	addil #276856832,%d6                  
    2bd2:  6000 ff18      	braw 0x2aec                           ; ↑ label_13

  label_20:
    2bd6:  2803           	movel %d3,%d4                         
    2bd8:  e58c           	lsll #2,%d4                           
    2bda:  2c04           	movel %d4,%d6                         
    2bdc:  0684 1080 8000 	addil #276856832,%d4                  
    2be2:  244b           	moveal %a3,%a2                        
    2be4:  d5c6           	addal %d6,%a2                         
    2be6:  60d4           	bras 0x2bbc                           ; ↑ label_17

  label_21:
    2be8:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2bec:  4e5e           	unlk %fp                              
    2bee:  4e75           	rts                                   
```

# PrvJobMoveWritePBlock: 0x2c04 - 0x2df8

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(18)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-14)`    : ???

```
    2c04:  4e56 fff2      	linkw %fp,#-14                        
    2c08:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2c0c:  246e 0008      	moveal %fp@(8),%a2                    
    2c10:  3c2e 000c      	movew %fp@(12),%d6                    
    2c14:  41fa 0984      	lea %pc@(0x359a),%a0                  ; ??? 0x359a
    2c18:  2808           	movel %a0,%d4                         
    2c1a:  7000           	moveq #0,%d0                          
    2c1c:  102e 000e      	moveb %fp@(14),%d0                    
    2c20:  3d40 fff6      	movew %d0,%fp@(-10)                   
    2c24:  7200           	moveq #0,%d1                          
    2c26:  122e 0010      	moveb %fp@(16),%d1                    
    2c2a:  9240           	subw %d0,%d1                          
    2c2c:  5241           	addqw #1,%d1                          
    2c2e:  3d41 fff8      	movew %d1,%fp@(-8)                    
    2c32:  42ae fffa      	clrl %fp@(-6)                         
    2c36:  7601           	moveq #1,%d3                          
    2c38:  2644           	moveal %d4,%a3                        
    2c3a:  548b           	addql #2,%a3                          

  label_0:
    2c3c:  bc53           	cmpw %a3@,%d6                         
    2c3e:  6406           	bccs 0x2c46                           ; ↓ label_1
    2c40:  5383           	subql #1,%d3                          
    2c42:  2a03           	movel %d3,%d5                         
    2c44:  600a           	bras 0x2c50                           ; ↓ label_2

  label_1:
    2c46:  548b           	addql #2,%a3                          
    2c48:  5283           	addql #1,%d3                          
    2c4a:  7011           	moveq #17,%d0                         
    2c4c:  b680           	cmpl %d0,%d3                          
    2c4e:  6dec           	blts 0x2c3c                           ; ↑ label_0

  label_2:
    2c50:  2005           	movel %d5,%d0                         
    2c52:  223c 0000 040c 	movel #1036,%d1                       
    2c58:  4eba 0976      	jsr %pc@(0x35d0)                      ; __lmul__
    2c5c:  262a 001c      	movel %a2@(28),%d3                    
    2c60:  2043           	moveal %d3,%a0                        
    2c62:  d1c0           	addal %d0,%a0                         
    2c64:  4aa8 4004      	tstl %a0@(16388)                      
    2c68:  6606           	bnes 0x2c70                           ; ↓ label_3
    2c6a:  7005           	moveq #5,%d0                          
    2c6c:  6000 0184      	braw 0x2df2                           ; ↓ label_15

  label_3:
    2c70:  3e3c 4000      	movew #16384,%d7                      
    2c74:  382a 0014      	movew %a2@(20),%d4                    
    2c78:  b86e fff8      	cmpw %fp@(-8),%d4                     
    2c7c:  663e           	bnes 0x2cbc                           ; ↓ label_5
    2c7e:  0cae 1080 8000 	cmpil #276856832,%fp@(18)             
    2c84:  0012                                                  
    2c86:  6520           	bcss 0x2ca8                           ; ↓ label_4
    2c88:  0cae 1081 0000 	cmpil #276889600,%fp@(18)             
    2c8e:  0012                                                  
    2c90:  6416           	bccs 0x2ca8                           ; ↓ label_4
    2c92:  206e 0012      	moveal %fp@(18),%a0                   
    2c96:  d1fc ef7f 8000 	addal #-276856832,%a0                 
    2c9c:  2e08           	movel %a0,%d7                         
    2c9e:  e24f           	lsrw #1,%d7                           
    2ca0:  0647 4000      	addiw #16384,%d7                      
    2ca4:  6000 00f6      	braw 0x2d9c                           ; ↓ label_11

  label_4:
    2ca8:  3f2e fff8      	movew %fp@(-8),%sp@-                  
    2cac:  4267           	clrw %sp@-                            
    2cae:  2f2e 0012      	movel %fp@(18),%sp@-                  
    2cb2:  4eba 0610      	jsr %pc@(0x32c4)                      ; PrvCopyPages2Buffer
    2cb6:  504f           	addqw #8,%sp                          
    2cb8:  6000 00e2      	braw 0x2d9c                           ; ↓ label_11

  label_5:
    2cbc:  7000           	moveq #0,%d0                          
    2cbe:  3006           	movew %d6,%d0                         
    2cc0:  d080           	addl %d0,%d0                          
    2cc2:  2043           	moveal %d3,%a0                        
    2cc4:  d1c0           	addal %d0,%a0                         
    2cc6:  2648           	moveal %a0,%a3                        
    2cc8:  0c50 ffff      	cmpiw #-1,%a0@                        
    2ccc:  6600 0094      	bnew 0x2d62                           ; ↓ label_9
    2cd0:  7000           	moveq #0,%d0                          
    2cd2:  102e 000e      	moveb %fp@(14),%d0                    
    2cd6:  7209           	moveq #9,%d1                          
    2cd8:  e368           	lslw %d1,%d0                          
    2cda:  48c0           	extl %d0                              
    2cdc:  e488           	lsrl #2,%d0                           
    2cde:  2600           	movel %d0,%d3                         
    2ce0:  2640           	moveal %d0,%a3                        
    2ce2:  7800           	moveq #0,%d4                          
    2ce4:  4a83           	tstl %d3                              
    2ce6:  6314           	blss 0x2cfc                           ; ↓ label_7
    2ce8:  263c 1080 8000 	movel #276856832,%d3                  

  label_6:
    2cee:  2043           	moveal %d3,%a0                        
    2cf0:  70ff           	moveq #-1,%d0                         
    2cf2:  2080           	movel %d0,%a0@                        
    2cf4:  5883           	addql #4,%d3                          
    2cf6:  5284           	addql #1,%d4                          
    2cf8:  b88b           	cmpl %a3,%d4                          
    2cfa:  65f2           	bcss 0x2cee                           ; ↑ label_6

  label_7:
    2cfc:  3f2e fff8      	movew %fp@(-8),%sp@-                  
    2d00:  3f2e fff6      	movew %fp@(-10),%sp@-                 
    2d04:  2f2e 0012      	movel %fp@(18),%sp@-                  
    2d08:  4eba 05ba      	jsr %pc@(0x32c4)                      ; PrvCopyPages2Buffer
    2d0c:  7000           	moveq #0,%d0                          
    2d0e:  102e 0010      	moveb %fp@(16),%d0                    
    2d12:  3600           	movew %d0,%d3                         
    2d14:  322a 0014      	movew %a2@(20),%d1                    
    2d18:  9240           	subw %d0,%d1                          
    2d1a:  0641 ffff      	addiw #-1,%d1                         
    2d1e:  7009           	moveq #9,%d0                          
    2d20:  e169           	lslw %d0,%d1                          
    2d22:  7000           	moveq #0,%d0                          
    2d24:  3001           	movew %d1,%d0                         
    2d26:  e488           	lsrl #2,%d0                           
    2d28:  2640           	moveal %d0,%a3                        
    2d2a:  3003           	movew %d3,%d0                         
    2d2c:  5240           	addqw #1,%d0                          
    2d2e:  7209           	moveq #9,%d1                          
    2d30:  e368           	lslw %d1,%d0                          
    2d32:  48c0           	extl %d0                              
    2d34:  e488           	lsrl #2,%d0                           
    2d36:  2d40 fff2      	movel %d0,%fp@(-14)                   
    2d3a:  7800           	moveq #0,%d4                          
    2d3c:  b6fc 0000      	cmpaw #0,%a3                          
    2d40:  504f           	addqw #8,%sp                          
    2d42:  6358           	blss 0x2d9c                           ; ↓ label_11
    2d44:  262e fff2      	movel %fp@(-14),%d3                   
    2d48:  e58b           	lsll #2,%d3                           

  label_8:
    2d4a:  2043           	moveal %d3,%a0                        
    2d4c:  203c 1080 8000 	movel #276856832,%d0                  
    2d52:  72ff           	moveq #-1,%d1                         
    2d54:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    2d58:  5883           	addql #4,%d3                          
    2d5a:  5284           	addql #1,%d4                          
    2d5c:  b88b           	cmpl %a3,%d4                          
    2d5e:  65ea           	bcss 0x2d4a                           ; ↑ label_8
    2d60:  603a           	bras 0x2d9c                           ; ↓ label_11

  label_9:
    2d62:  486e fffa      	pea %fp@(-6)                          
    2d66:  4879 1080 8000 	pea 0x10808000                        
    2d6c:  3004           	movew %d4,%d0                         
    2d6e:  0640 ffff      	addiw #-1,%d0                         
    2d72:  1f00           	moveb %d0,%sp@-                       
    2d74:  4227           	clrb %sp@-                            
    2d76:  3f13           	movew %a3@,%sp@-                      
    2d78:  2f0a           	movel %a2,%sp@-                       
    2d7a:  4eba f934      	jsr %pc@(0x26b0)                      ; PrvJobReadPBlock
    2d7e:  2600           	movel %d0,%d3                         
    2d80:  4fef 0012      	lea %sp@(18),%sp                      
    2d84:  6704           	beqs 0x2d8a                           ; ↓ label_10
    2d86:  2003           	movel %d3,%d0                         
    2d88:  6068           	bras 0x2df2                           ; ↓ label_15

  label_10:
    2d8a:  3f2e fff8      	movew %fp@(-8),%sp@-                  
    2d8e:  3f2e fff6      	movew %fp@(-10),%sp@-                 
    2d92:  2f2e 0012      	movel %fp@(18),%sp@-                  
    2d96:  4eba 052c      	jsr %pc@(0x32c4)                      ; PrvCopyPages2Buffer
    2d9a:  504f           	addqw #8,%sp                          

  label_11:
    2d9c:  7800           	moveq #0,%d4                          
    2d9e:  3806           	movew %d6,%d4                         
    2da0:  d884           	addl %d4,%d4                          

  label_12:
    2da2:  486e fffe      	pea %fp@(-2)                          
    2da6:  2f05           	movel %d5,%sp@-                       
    2da8:  2f0a           	movel %a2,%sp@-                       
    2daa:  4eba 0066      	jsr %pc@(0x2e12)                      ; PrvJobFindFreePBlock
    2dae:  2600           	movel %d0,%d3                         
    2db0:  4fef 000c      	lea %sp@(12),%sp                      
    2db4:  670a           	beqs 0x2dc0                           ; ↓ label_13
    2db6:  2003           	movel %d3,%d0                         
    2db8:  5780           	subql #3,%d0                          
    2dba:  67e6           	beqs 0x2da2                           ; ↑ label_12
    2dbc:  2003           	movel %d3,%d0                         
    2dbe:  6032           	bras 0x2df2                           ; ↓ label_15

  label_13:
    2dc0:  2f2e fffa      	movel %fp@(-6),%sp@-                  
    2dc4:  3f07           	movew %d7,%sp@-                       
    2dc6:  3f06           	movew %d6,%sp@-                       
    2dc8:  2f05           	movel %d5,%sp@-                       
    2dca:  2044           	moveal %d4,%a0                        
    2dcc:  202a 001c      	movel %a2@(28),%d0                    
    2dd0:  3f30 0800      	movew %a0@(00000000,%d0:l),%sp@-      
    2dd4:  3f2e fffe      	movew %fp@(-2),%sp@-                  
    2dd8:  2f0a           	movel %a2,%sp@-                       
    2dda:  4eba 01f4      	jsr %pc@(0x2fd0)                      ; PrvJobWritePBlock
    2dde:  2600           	movel %d0,%d3                         
    2de0:  4fef 0014      	lea %sp@(20),%sp                      
    2de4:  670a           	beqs 0x2df0                           ; ↓ label_14
    2de6:  2003           	movel %d3,%d0                         
    2de8:  5780           	subql #3,%d0                          
    2dea:  67b6           	beqs 0x2da2                           ; ↑ label_12
    2dec:  2003           	movel %d3,%d0                         
    2dee:  6002           	bras 0x2df2                           ; ↓ label_15

  label_14:
    2df0:  7000           	moveq #0,%d0                          

  label_15:
    2df2:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2df6:  4e5e           	unlk %fp                              
    2df8:  4e75           	rts                                   
```

# PrvJobFindFreePBlock: 0x2e12 - 0x2fb6

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-14)`    : ???
   * `%fp@(-32)`    : ???
   * `%fp@(-48)`    : ???
   * `%fp@(-50)`    : ???
   * `%fp@(-64)`    : ???
   * `%fp@(-68)`    : ???
   * `%fp@(-72)`    : ???
   * `%fp@(-76)`    : ???

```
    2e12:  4e56 ffb4      	linkw %fp,#-76                        
    2e16:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2e1a:  246e 0008      	moveal %fp@(8),%a2                    
    2e1e:  7010           	moveq #16,%d0                         
    2e20:  c0aa 0008      	andl %a2@(8),%d0                      
    2e24:  6706           	beqs 0x2e2c                           ; ↓ label_0
    2e26:  7005           	moveq #5,%d0                          
    2e28:  6000 0186      	braw 0x2fb0                           ; ↓ label_8

  label_0:
    2e2c:  202e 000c      	movel %fp@(12),%d0                    
    2e30:  223c 0000 040c 	movel #1036,%d1                       
    2e36:  4eba 0798      	jsr %pc@(0x35d0)                      ; __lmul__
    2e3a:  2c00           	movel %d0,%d6                         
    2e3c:  206a 001c      	moveal %a2@(28),%a0                   
    2e40:  d1c0           	addal %d0,%a0                         
    2e42:  2808           	movel %a0,%d4                         
    2e44:  2628 4004      	movel %a0@(16388),%d3                 
    2e48:  2044           	moveal %d4,%a0                        
    2e4a:  2d68 4000 fffc 	movel %a0@(16384),%fp@(-4)            
    2e50:  2806           	movel %d6,%d4                         
    2e52:  603a           	bras 0x2e8e                           ; ↓ label_3

  label_1:
    2e54:  54ae fffc      	addql #2,%fp@(-4)                     
    2e58:  266a 001c      	moveal %a2@(28),%a3                   
    2e5c:  2004           	movel %d4,%d0                         
    2e5e:  d08b           	addl %a3,%d0                          
    2e60:  2040           	moveal %d0,%a0                        
    2e62:  2028 4004      	movel %a0@(16388),%d0                 
    2e66:  d080           	addl %d0,%d0                          
    2e68:  204b           	moveal %a3,%a0                        
    2e6a:  d1c4           	addal %d4,%a0                         
    2e6c:  2a08           	movel %a0,%d5                         
    2e6e:  d1c0           	addal %d0,%a0                         
    2e70:  41e8 400c      	lea %a0@(16396),%a0                   
    2e74:  b1ee fffc      	cmpal %fp@(-4),%a0                    
    2e78:  660a           	bnes 0x2e84                           ; ↓ label_2
    2e7a:  2045           	moveal %d5,%a0                        
    2e7c:  41e8 400c      	lea %a0@(16396),%a0                   
    2e80:  2d48 fffc      	movel %a0,%fp@(-4)                    

  label_2:
    2e84:  5383           	subql #1,%d3                          
    2e86:  6606           	bnes 0x2e8e                           ; ↓ label_3
    2e88:  7005           	moveq #5,%d0                          
    2e8a:  6000 0124      	braw 0x2fb0                           ; ↓ label_8

  label_3:
    2e8e:  206e fffc      	moveal %fp@(-4),%a0                   
    2e92:  0c50 ffff      	cmpiw #-1,%a0@                        
    2e96:  67bc           	beqs 0x2e54                           ; ↑ label_1
    2e98:  2d6a 001c ffbc 	movel %a2@(28),%fp@(-68)              
    2e9e:  4878 0004      	pea 0x4                               
    2ea2:  486e fffc      	pea %fp@(-4)                          
    2ea6:  206e ffbc      	moveal %fp@(-68),%a0                  
    2eaa:  d1c6           	addal %d6,%a0                         
    2eac:  41e8 4000      	lea %a0@(16384),%a0                   
    2eb0:  2008           	movel %a0,%d0                         
    2eb2:  90ae ffbc      	subl %fp@(-68),%d0                    
    2eb6:  2f00           	movel %d0,%sp@-                       
    2eb8:  2f2e ffbc      	movel %fp@(-68),%sp@-                 
    2ebc:  4e4f           	trap #15                              
    2ebe:  a076           	0120166                               ; sysTrapDmWrite
    2ec0:  206a 001c      	moveal %a2@(28),%a0                   
    2ec4:  d1c6           	addal %d6,%a0                         
    2ec6:  2068 4000      	moveal %a0@(16384),%a0                
    2eca:  3610           	movew %a0@,%d3                        
    2ecc:  3d7c 1201 ffce 	movew #4609,%fp@(-50)                 
    2ed2:  3d43 ffd0      	movew %d3,%fp@(-48)                   
    2ed6:  426e ffe0      	clrw %fp@(-32)                        
    2eda:  1d7c 0001 fff2 	moveb #1,%fp@(-14)                    
    2ee0:  41fa ea2a      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    2ee4:  2d48 fff4      	movel %a0,%fp@(-12)                   
    2ee8:  41ee ffc0      	lea %fp@(-64),%a0                     
    2eec:  2d48 fff8      	movel %a0,%fp@(-8)                    
    2ef0:  426e ffc0      	clrw %fp@(-64)                        
    2ef4:  486e ffce      	pea %fp@(-50)                         
    2ef8:  2078 0304      	moveal 0x304,%a0                      
    2efc:  2068 0018      	moveal %a0@(24),%a0                   
    2f00:  2068 009a      	moveal %a0@(154),%a0                  
    2f04:  4e90           	jsr %a0@                              
    2f06:  302e ffc0      	movew %fp@(-64),%d0                   
    2f0a:  0240 03ff      	andiw #1023,%d0                       
    2f0e:  4fef 0014      	lea %sp@(20),%sp                      
    2f12:  6700 0094      	beqw 0x2fa8                           ; ↓ label_7
    2f16:  7001           	moveq #1,%d0                          
    2f18:  c0aa 0004      	andl %a2@(4),%d0                      
    2f1c:  6606           	bnes 0x2f24                           ; ↓ label_4
    2f1e:  7002           	moveq #2,%d0                          
    2f20:  6000 008e      	braw 0x2fb0                           ; ↓ label_8

  label_4:
    2f24:  302e ffc0      	movew %fp@(-64),%d0                   
    2f28:  0240 03f0      	andiw #1008,%d0                       
    2f2c:  e848           	lsrw #4,%d0                           
    2f2e:  5540           	subqw #2,%d0                          
    2f30:  6604           	bnes 0x2f36                           ; ↓ label_5
    2f32:  7006           	moveq #6,%d0                          
    2f34:  607a           	bras 0x2fb0                           ; ↓ label_8

  label_5:
    2f36:  2e2a 001c      	movel %a2@(28),%d7                    
    2f3a:  1f3c 00ff      	moveb #-1,%sp@-                       
    2f3e:  4878 0002      	pea 0x2                               
    2f42:  2047           	moveal %d7,%a0                        
    2f44:  d1c6           	addal %d6,%a0                         
    2f46:  2028 4000      	movel %a0@(16384),%d0                 
    2f4a:  9087           	subl %d7,%d0                          
    2f4c:  2f00           	movel %d0,%sp@-                       
    2f4e:  2f07           	movel %d7,%sp@-                       
    2f50:  4e4f           	trap #15                              
    2f52:  a07e           	0120176                               ; sysTrapDmSet
    2f54:  206a 001c      	moveal %a2@(28),%a0                   
    2f58:  d1c6           	addal %d6,%a0                         
    2f5a:  2028 4008      	movel %a0@(16392),%d0                 
    2f5e:  5280           	addql #1,%d0                          
    2f60:  2d40 ffb4      	movel %d0,%fp@(-76)                   
    2f64:  2d6a 001c ffb8 	movel %a2@(28),%fp@(-72)              
    2f6a:  4878 0004      	pea 0x4                               
    2f6e:  486e ffb4      	pea %fp@(-76)                         
    2f72:  206e ffb8      	moveal %fp@(-72),%a0                  
    2f76:  d1c6           	addal %d6,%a0                         
    2f78:  41e8 4008      	lea %a0@(16392),%a0                   
    2f7c:  2008           	movel %a0,%d0                         
    2f7e:  90ae ffb8      	subl %fp@(-72),%d0                    
    2f82:  2f00           	movel %d0,%sp@-                       
    2f84:  2f2e ffb8      	movel %fp@(-72),%sp@-                 
    2f88:  4e4f           	trap #15                              
    2f8a:  a076           	0120166                               ; sysTrapDmWrite
    2f8c:  206a 001c      	moveal %a2@(28),%a0                   
    2f90:  d1c6           	addal %d6,%a0                         
    2f92:  7010           	moveq #16,%d0                         
    2f94:  b0a8 4008      	cmpl %a0@(16392),%d0                  
    2f98:  4fef 001e      	lea %sp@(30),%sp                      
    2f9c:  6206           	bhis 0x2fa4                           ; ↓ label_6
    2f9e:  7010           	moveq #16,%d0                         
    2fa0:  81aa 0008      	orl %d0,%a2@(8)                       

  label_6:
    2fa4:  7003           	moveq #3,%d0                          
    2fa6:  6008           	bras 0x2fb0                           ; ↓ label_8

  label_7:
    2fa8:  206e 0010      	moveal %fp@(16),%a0                   
    2fac:  3083           	movew %d3,%a0@                        
    2fae:  7000           	moveq #0,%d0                          

  label_8:
    2fb0:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2fb4:  4e5e           	unlk %fp                              
    2fb6:  4e75           	rts                                   
```

# PrvJobWritePBlock: 0x2fd0 - 0x32ae

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???
   * `%fp@(22)`     : ???
   * `%fp@(24)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-22)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-26)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-34)`    : ???
   * `%fp@(-36)`    : ???
   * `%fp@(-38)`    : ???
   * `%fp@(-40)`    : ???
   * `%fp@(-42)`    : ???
   * `%fp@(-44)`    : ???
   * `%fp@(-46)`    : ???
   * `%fp@(-50)`    : ???
   * `%fp@(-54)`    : ???
   * `%fp@(-68)`    : ???
   * `%fp@(-72)`    : ???
   * `%fp@(-76)`    : ???
   * `%fp@(-78)`    : ???

```
    2fd0:  4e56 ffb2      	linkw %fp,#-78                        
    2fd4:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2fd8:  246e 0008      	moveal %fp@(8),%a2                    
    2fdc:  0c6e ffff 000e 	cmpiw #-1,%fp@(14)                    
    2fe2:  6604           	bnes 0x2fe8                           ; ↓ label_0
    2fe4:  7000           	moveq #0,%d0                          
    2fe6:  6002           	bras 0x2fea                           ; ↓ label_1

  label_0:
    2fe8:  7003           	moveq #3,%d0                          

  label_1:
    2fea:  3d40 ffb2      	movew %d0,%fp@(-78)                   
    2fee:  3d7c 0a01 ffd2 	movew #2561,%fp@(-46)                 
    2ff4:  3d6e 000c ffd4 	movew %fp@(12),%fp@(-44)              
    2ffa:  3d6e 000e ffd6 	movew %fp@(14),%fp@(-42)              
    3000:  3d6e 0014 ffd8 	movew %fp@(20),%fp@(-40)              
    3006:  3d6e 0016 ffda 	movew %fp@(22),%fp@(-38)              
    300c:  426e ffdc      	clrw %fp@(-36)                        
    3010:  302a 0014      	movew %a2@(20),%d0                    
    3014:  0640 ffff      	addiw #-1,%d0                         
    3018:  3d40 ffde      	movew %d0,%fp@(-34)                   
    301c:  3d7c c800 ffe4 	movew #-14336,%fp@(-28)               
    3022:  202e 0018      	movel %fp@(24),%d0                    
    3026:  4240           	clrw %d0                              
    3028:  4840           	swap %d0                              
    302a:  3d40 ffe6      	movew %d0,%fp@(-26)                   
    302e:  202e 0018      	movel %fp@(24),%d0                    
    3032:  0280 0000 ffff 	andil #65535,%d0                      
    3038:  3d40 ffe8      	movew %d0,%fp@(-24)                   
    303c:  3d7c 00ff ffea 	movew #255,%fp@(-22)                  
    3042:  3d6e ffb2 ffec 	movew %fp@(-78),%fp@(-20)             
    3048:  1d7c 0001 fff6 	moveb #1,%fp@(-10)                    
    304e:  41fa e8bc      	lea %pc@(0x190c),%a0                  ; PrvDspDoneProc
    3052:  2d48 fff8      	movel %a0,%fp@(-8)                    
    3056:  41ee ffbc      	lea %fp@(-68),%a0                     
    305a:  2d48 fffc      	movel %a0,%fp@(-4)                    
    305e:  426e ffbc      	clrw %fp@(-68)                        
    3062:  486e ffd2      	pea %fp@(-46)                         
    3066:  2078 0304      	moveal 0x304,%a0                      
    306a:  2068 0018      	moveal %a0@(24),%a0                   
    306e:  2068 009a      	moveal %a0@(154),%a0                  
    3072:  4e90           	jsr %a0@                              
    3074:  302e ffbc      	movew %fp@(-68),%d0                   
    3078:  0240 03ff      	andiw #1023,%d0                       
    307c:  584f           	addqw #4,%sp                          
    307e:  6700 0112      	beqw 0x3192                           ; ↓ label_7
    3082:  7001           	moveq #1,%d0                          
    3084:  c0aa 0004      	andl %a2@(4),%d0                      
    3088:  6606           	bnes 0x3090                           ; ↓ label_2
    308a:  7002           	moveq #2,%d0                          
    308c:  6000 021a      	braw 0x32a8                           ; ↓ label_11

  label_2:
    3090:  302e ffbc      	movew %fp@(-68),%d0                   
    3094:  0240 03f0      	andiw #1008,%d0                       
    3098:  e848           	lsrw #4,%d0                           
    309a:  7200           	moveq #0,%d1                          
    309c:  3200           	movew %d0,%d1                         
    309e:  2d41 ffce      	movel %d1,%fp@(-50)                   
    30a2:  7002           	moveq #2,%d0                          
    30a4:  b081           	cmpl %d1,%d0                          
    30a6:  6606           	bnes 0x30ae                           ; ↓ label_3
    30a8:  7006           	moveq #6,%d0                          
    30aa:  6000 01fc      	braw 0x32a8                           ; ↓ label_11

  label_3:
    30ae:  7009           	moveq #9,%d0                          
    30b0:  b0ae ffce      	cmpl %fp@(-50),%d0                    
    30b4:  6208           	bhis 0x30be                           ; ↓ label_4
    30b6:  700d           	moveq #13,%d0                         
    30b8:  b0ae ffce      	cmpl %fp@(-50),%d0                    
    30bc:  647c           	bccs 0x313a                           ; ↓ label_6

  label_4:
    30be:  282a 001c      	movel %a2@(28),%d4                    
    30c2:  1f3c 00ff      	moveb #-1,%sp@-                       
    30c6:  4878 0002      	pea 0x2                               
    30ca:  202e 0010      	movel %fp@(16),%d0                    
    30ce:  223c 0000 040c 	movel #1036,%d1                       
    30d4:  4eba 04fa      	jsr %pc@(0x35d0)                      ; __lmul__
    30d8:  2600           	movel %d0,%d3                         
    30da:  2044           	moveal %d4,%a0                        
    30dc:  d1c0           	addal %d0,%a0                         
    30de:  2028 4000      	movel %a0@(16384),%d0                 
    30e2:  9084           	subl %d4,%d0                          
    30e4:  2f00           	movel %d0,%sp@-                       
    30e6:  2f04           	movel %d4,%sp@-                       
    30e8:  4e4f           	trap #15                              
    30ea:  a07e           	0120176                               ; sysTrapDmSet
    30ec:  206a 001c      	moveal %a2@(28),%a0                   
    30f0:  d1c3           	addal %d3,%a0                         
    30f2:  2028 4008      	movel %a0@(16392),%d0                 
    30f6:  5280           	addql #1,%d0                          
    30f8:  2d40 ffce      	movel %d0,%fp@(-50)                   
    30fc:  2c2a 001c      	movel %a2@(28),%d6                    
    3100:  4878 0004      	pea 0x4                               
    3104:  486e ffce      	pea %fp@(-50)                         
    3108:  2046           	moveal %d6,%a0                        
    310a:  d1c3           	addal %d3,%a0                         
    310c:  41e8 4008      	lea %a0@(16392),%a0                   
    3110:  2008           	movel %a0,%d0                         
    3112:  9086           	subl %d6,%d0                          
    3114:  2f00           	movel %d0,%sp@-                       
    3116:  2f06           	movel %d6,%sp@-                       
    3118:  4e4f           	trap #15                              
    311a:  a076           	0120166                               ; sysTrapDmWrite
    311c:  206a 001c      	moveal %a2@(28),%a0                   
    3120:  d1c3           	addal %d3,%a0                         
    3122:  7010           	moveq #16,%d0                         
    3124:  b0a8 4008      	cmpl %a0@(16392),%d0                  
    3128:  4fef 001e      	lea %sp@(30),%sp                      
    312c:  6206           	bhis 0x3134                           ; ↓ label_5
    312e:  7010           	moveq #16,%d0                         
    3130:  81aa 0008      	orl %d0,%a2@(8)                       

  label_5:
    3134:  7003           	moveq #3,%d0                          
    3136:  6000 0170      	braw 0x32a8                           ; ↓ label_11

  label_6:
    313a:  202e 0010      	movel %fp@(16),%d0                    
    313e:  223c 0000 040c 	movel #1036,%d1                       
    3144:  4eba 048a      	jsr %pc@(0x35d0)                      ; __lmul__
    3148:  2800           	movel %d0,%d4                         
    314a:  206a 001c      	moveal %a2@(28),%a0                   
    314e:  d1c0           	addal %d0,%a0                         
    3150:  2028 4008      	movel %a0@(16392),%d0                 
    3154:  5280           	addql #1,%d0                          
    3156:  2d40 ffce      	movel %d0,%fp@(-50)                   
    315a:  2e2a 001c      	movel %a2@(28),%d7                    
    315e:  4878 0004      	pea 0x4                               
    3162:  486e ffce      	pea %fp@(-50)                         
    3166:  2047           	moveal %d7,%a0                        
    3168:  d1c4           	addal %d4,%a0                         
    316a:  41e8 4008      	lea %a0@(16392),%a0                   
    316e:  2008           	movel %a0,%d0                         
    3170:  9087           	subl %d7,%d0                          
    3172:  2f00           	movel %d0,%sp@-                       
    3174:  2f07           	movel %d7,%sp@-                       
    3176:  4e4f           	trap #15                              
    3178:  a076           	0120166                               ; sysTrapDmWrite
    317a:  206a 001c      	moveal %a2@(28),%a0                   
    317e:  d1c4           	addal %d4,%a0                         
    3180:  7010           	moveq #16,%d0                         
    3182:  b0a8 4008      	cmpl %a0@(16392),%d0                  
    3186:  4fef 0010      	lea %sp@(16),%sp                      
    318a:  6206           	bhis 0x3192                           ; ↓ label_7
    318c:  7010           	moveq #16,%d0                         
    318e:  81aa 0008      	orl %d0,%a2@(8)                       

  label_7:
    3192:  0c6e ffff 000e 	cmpiw #-1,%fp@(14)                    
    3198:  6632           	bnes 0x31cc                           ; ↓ label_8
    319a:  262a 001c      	movel %a2@(28),%d3                    
    319e:  1f3c 00ff      	moveb #-1,%sp@-                       
    31a2:  4878 0002      	pea 0x2                               
    31a6:  202e 0010      	movel %fp@(16),%d0                    
    31aa:  223c 0000 040c 	movel #1036,%d1                       
    31b0:  4eba 041e      	jsr %pc@(0x35d0)                      ; __lmul__
    31b4:  2043           	moveal %d3,%a0                        
    31b6:  d1c0           	addal %d0,%a0                         
    31b8:  2028 4000      	movel %a0@(16384),%d0                 
    31bc:  9083           	subl %d3,%d0                          
    31be:  2f00           	movel %d0,%sp@-                       
    31c0:  2f03           	movel %d3,%sp@-                       
    31c2:  4e4f           	trap #15                              
    31c4:  a07e           	0120176                               ; sysTrapDmSet
    31c6:  4fef 000e      	lea %sp@(14),%sp                      
    31ca:  6038           	bras 0x3204                           ; ↓ label_9

  label_8:
    31cc:  266a 001c      	moveal %a2@(28),%a3                   
    31d0:  4878 0002      	pea 0x2                               
    31d4:  7000           	moveq #0,%d0                          
    31d6:  302e 0014      	movew %fp@(20),%d0                    
    31da:  d080           	addl %d0,%d0                          
    31dc:  4873 0800      	pea %a3@(00000000,%d0:l)              
    31e0:  202e 0010      	movel %fp@(16),%d0                    
    31e4:  223c 0000 040c 	movel #1036,%d1                       
    31ea:  4eba 03e4      	jsr %pc@(0x35d0)                      ; __lmul__
    31ee:  204b           	moveal %a3,%a0                        
    31f0:  d1c0           	addal %d0,%a0                         
    31f2:  2028 4000      	movel %a0@(16384),%d0                 
    31f6:  908b           	subl %a3,%d0                          
    31f8:  2f00           	movel %d0,%sp@-                       
    31fa:  2f0b           	movel %a3,%sp@-                       
    31fc:  4e4f           	trap #15                              
    31fe:  a076           	0120166                               ; sysTrapDmWrite
    3200:  4fef 0010      	lea %sp@(16),%sp                      

  label_9:
    3204:  2d6a 001c ffb8 	movel %a2@(28),%fp@(-72)              
    320a:  4878 0002      	pea 0x2                               
    320e:  486e 000c      	pea %fp@(12)                          
    3212:  7000           	moveq #0,%d0                          
    3214:  302e 0014      	movew %fp@(20),%d0                    
    3218:  d080           	addl %d0,%d0                          
    321a:  206e ffb8      	moveal %fp@(-72),%a0                  
    321e:  d1c0           	addal %d0,%a0                         
    3220:  2008           	movel %a0,%d0                         
    3222:  90ae ffb8      	subl %fp@(-72),%d0                    
    3226:  2f00           	movel %d0,%sp@-                       
    3228:  2f2e ffb8      	movel %fp@(-72),%sp@-                 
    322c:  4e4f           	trap #15                              
    322e:  a076           	0120166                               ; sysTrapDmWrite
    3230:  202e 0010      	movel %fp@(16),%d0                    
    3234:  223c 0000 040c 	movel #1036,%d1                       
    323a:  4eba 0394      	jsr %pc@(0x35d0)                      ; __lmul__
    323e:  2600           	movel %d0,%d3                         
    3240:  206a 001c      	moveal %a2@(28),%a0                   
    3244:  d1c0           	addal %d0,%a0                         
    3246:  2068 4000      	moveal %a0@(16384),%a0                
    324a:  5488           	addql #2,%a0                          
    324c:  2d48 ffca      	movel %a0,%fp@(-54)                   
    3250:  206a 001c      	moveal %a2@(28),%a0                   
    3254:  d1c3           	addal %d3,%a0                         
    3256:  2a08           	movel %a0,%d5                         
    3258:  2045           	moveal %d5,%a0                        
    325a:  2028 4004      	movel %a0@(16388),%d0                 
    325e:  d080           	addl %d0,%d0                          
    3260:  d1c0           	addal %d0,%a0                         
    3262:  41e8 400c      	lea %a0@(16396),%a0                   
    3266:  b1ee ffca      	cmpal %fp@(-54),%a0                   
    326a:  4fef 0010      	lea %sp@(16),%sp                      
    326e:  660a           	bnes 0x327a                           ; ↓ label_10
    3270:  2045           	moveal %d5,%a0                        
    3272:  41e8 400c      	lea %a0@(16396),%a0                   
    3276:  2d48 ffca      	movel %a0,%fp@(-54)                   

  label_10:
    327a:  2d6a 001c ffb4 	movel %a2@(28),%fp@(-76)              
    3280:  4878 0004      	pea 0x4                               
    3284:  486e ffca      	pea %fp@(-54)                         
    3288:  206e ffb4      	moveal %fp@(-76),%a0                  
    328c:  d1c3           	addal %d3,%a0                         
    328e:  41e8 4000      	lea %a0@(16384),%a0                   
    3292:  2008           	movel %a0,%d0                         
    3294:  90ae ffb4      	subl %fp@(-76),%d0                    
    3298:  2f00           	movel %d0,%sp@-                       
    329a:  2f2e ffb4      	movel %fp@(-76),%sp@-                 
    329e:  4e4f           	trap #15                              
    32a0:  a076           	0120166                               ; sysTrapDmWrite
    32a2:  7000           	moveq #0,%d0                          
    32a4:  4fef 0010      	lea %sp@(16),%sp                      

  label_11:
    32a8:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    32ac:  4e5e           	unlk %fp                              
    32ae:  4e75           	rts                                   
```

# PrvCopyPages2Buffer: 0x32c4 - 0x3582

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-16)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-32)`    : ???

```
    32c4:  4e56 ffe0      	linkw %fp,#-32                        
    32c8:  48e7 0f10      	moveml %d4-%d7/%a3,%sp@-              
    32cc:  282e 0008      	movel %fp@(8),%d4                     
    32d0:  302e 000e      	movew %fp@(14),%d0                    
    32d4:  7a09           	moveq #9,%d5                          
    32d6:  eb68           	lslw %d5,%d0                          
    32d8:  7a00           	moveq #0,%d5                          
    32da:  3a00           	movew %d0,%d5                         
    32dc:  e48d           	lsrl #2,%d5                           
    32de:  302e 000c      	movew %fp@(12),%d0                    
    32e2:  7209           	moveq #9,%d1                          
    32e4:  e368           	lslw %d1,%d0                          
    32e6:  7200           	moveq #0,%d1                          
    32e8:  3200           	movew %d0,%d1                         
    32ea:  0681 1080 8000 	addil #276856832,%d1                  
    32f0:  2d41 ffe4      	movel %d1,%fp@(-28)                   
    32f4:  7001           	moveq #1,%d0                          
    32f6:  c084           	andl %d4,%d0                          
    32f8:  6700 01a0      	beqw 0x349a                           ; ↓ label_3
    32fc:  7400           	moveq #0,%d2                          
    32fe:  4a85           	tstl %d5                              
    3300:  6300 0248      	blsw 0x354a                           ; ↓ label_11
    3304:  2005           	movel %d5,%d0                         
    3306:  5180           	subql #8,%d0                          
    3308:  2d40 ffe0      	movel %d0,%fp@(-32)                   
    330c:  7008           	moveq #8,%d0                          
    330e:  ba80           	cmpl %d0,%d5                          
    3310:  6300 023a      	blsw 0x354c                           ; ↓ label_12
    3314:  7e00           	moveq #0,%d7                          
    3316:  2241           	moveal %d1,%a1                        

  label_0:
    3318:  7c00           	moveq #0,%d6                          
    331a:  2044           	moveal %d4,%a0                        
    331c:  d1c7           	addal %d7,%a0                         
    331e:  2648           	moveal %a0,%a3                        
    3320:  3028 0001      	movew %a0@(1),%d0                     
    3324:  e148           	lslw #8,%d0                           
    3326:  7200           	moveq #0,%d1                          
    3328:  3200           	movew %d0,%d1                         
    332a:  8c81           	orl %d1,%d6                           
    332c:  7000           	moveq #0,%d0                          
    332e:  102b 0003      	moveb %a3@(3),%d0                     
    3332:  8c80           	orl %d0,%d6                           
    3334:  2286           	movel %d6,%a1@                        
    3336:  2002           	movel %d2,%d0                         
    3338:  5280           	addql #1,%d0                          
    333a:  e588           	lsll #2,%d0                           
    333c:  2d40 fffc      	movel %d0,%fp@(-4)                    
    3340:  7c00           	moveq #0,%d6                          
    3342:  2044           	moveal %d4,%a0                        
    3344:  d1c0           	addal %d0,%a0                         
    3346:  2648           	moveal %a0,%a3                        
    3348:  3028 0001      	movew %a0@(1),%d0                     
    334c:  e148           	lslw #8,%d0                           
    334e:  7200           	moveq #0,%d1                          
    3350:  3200           	movew %d0,%d1                         
    3352:  8c81           	orl %d1,%d6                           
    3354:  7000           	moveq #0,%d0                          
    3356:  102b 0003      	moveb %a3@(3),%d0                     
    335a:  8c80           	orl %d0,%d6                           
    335c:  2346 0004      	movel %d6,%a1@(4)                     
    3360:  2002           	movel %d2,%d0                         
    3362:  5480           	addql #2,%d0                          
    3364:  e588           	lsll #2,%d0                           
    3366:  2d40 ffe8      	movel %d0,%fp@(-24)                   
    336a:  7c00           	moveq #0,%d6                          
    336c:  2044           	moveal %d4,%a0                        
    336e:  d1c0           	addal %d0,%a0                         
    3370:  2648           	moveal %a0,%a3                        
    3372:  3028 0001      	movew %a0@(1),%d0                     
    3376:  e148           	lslw #8,%d0                           
    3378:  7200           	moveq #0,%d1                          
    337a:  3200           	movew %d0,%d1                         
    337c:  8c81           	orl %d1,%d6                           
    337e:  7000           	moveq #0,%d0                          
    3380:  102b 0003      	moveb %a3@(3),%d0                     
    3384:  8c80           	orl %d0,%d6                           
    3386:  2346 0008      	movel %d6,%a1@(8)                     
    338a:  2002           	movel %d2,%d0                         
    338c:  5680           	addql #3,%d0                          
    338e:  e588           	lsll #2,%d0                           
    3390:  2d40 ffec      	movel %d0,%fp@(-20)                   
    3394:  7c00           	moveq #0,%d6                          
    3396:  2044           	moveal %d4,%a0                        
    3398:  d1c0           	addal %d0,%a0                         
    339a:  2648           	moveal %a0,%a3                        
    339c:  3028 0001      	movew %a0@(1),%d0                     
    33a0:  e148           	lslw #8,%d0                           
    33a2:  7200           	moveq #0,%d1                          
    33a4:  3200           	movew %d0,%d1                         
    33a6:  8c81           	orl %d1,%d6                           
    33a8:  7000           	moveq #0,%d0                          
    33aa:  102b 0003      	moveb %a3@(3),%d0                     
    33ae:  8c80           	orl %d0,%d6                           
    33b0:  2346 000c      	movel %d6,%a1@(12)                    
    33b4:  2002           	movel %d2,%d0                         
    33b6:  5880           	addql #4,%d0                          
    33b8:  e588           	lsll #2,%d0                           
    33ba:  2d40 fff0      	movel %d0,%fp@(-16)                   
    33be:  7c00           	moveq #0,%d6                          
    33c0:  2044           	moveal %d4,%a0                        
    33c2:  d1c0           	addal %d0,%a0                         
    33c4:  2648           	moveal %a0,%a3                        
    33c6:  3028 0001      	movew %a0@(1),%d0                     
    33ca:  e148           	lslw #8,%d0                           
    33cc:  7200           	moveq #0,%d1                          
    33ce:  3200           	movew %d0,%d1                         
    33d0:  8c81           	orl %d1,%d6                           
    33d2:  7000           	moveq #0,%d0                          
    33d4:  102b 0003      	moveb %a3@(3),%d0                     
    33d8:  8c80           	orl %d0,%d6                           
    33da:  2346 0010      	movel %d6,%a1@(16)                    
    33de:  2002           	movel %d2,%d0                         
    33e0:  5a80           	addql #5,%d0                          
    33e2:  e588           	lsll #2,%d0                           
    33e4:  2d40 fff4      	movel %d0,%fp@(-12)                   
    33e8:  7c00           	moveq #0,%d6                          
    33ea:  2044           	moveal %d4,%a0                        
    33ec:  d1c0           	addal %d0,%a0                         
    33ee:  2648           	moveal %a0,%a3                        
    33f0:  3028 0001      	movew %a0@(1),%d0                     
    33f4:  e148           	lslw #8,%d0                           
    33f6:  7200           	moveq #0,%d1                          
    33f8:  3200           	movew %d0,%d1                         
    33fa:  8c81           	orl %d1,%d6                           
    33fc:  7000           	moveq #0,%d0                          
    33fe:  102b 0003      	moveb %a3@(3),%d0                     
    3402:  8c80           	orl %d0,%d6                           
    3404:  2346 0014      	movel %d6,%a1@(20)                    
    3408:  2002           	movel %d2,%d0                         
    340a:  5c80           	addql #6,%d0                          
    340c:  e588           	lsll #2,%d0                           
    340e:  2d40 fff8      	movel %d0,%fp@(-8)                    
    3412:  7c00           	moveq #0,%d6                          
    3414:  2044           	moveal %d4,%a0                        
    3416:  d1c0           	addal %d0,%a0                         
    3418:  2648           	moveal %a0,%a3                        
    341a:  3028 0001      	movew %a0@(1),%d0                     
    341e:  e148           	lslw #8,%d0                           
    3420:  7200           	moveq #0,%d1                          
    3422:  3200           	movew %d0,%d1                         
    3424:  8c81           	orl %d1,%d6                           
    3426:  7000           	moveq #0,%d0                          
    3428:  102b 0003      	moveb %a3@(3),%d0                     
    342c:  8c80           	orl %d0,%d6                           
    342e:  2346 0018      	movel %d6,%a1@(24)                    
    3432:  7c00           	moveq #0,%d6                          
    3434:  2002           	movel %d2,%d0                         
    3436:  5e80           	addql #7,%d0                          
    3438:  e588           	lsll #2,%d0                           
    343a:  2044           	moveal %d4,%a0                        
    343c:  d1c0           	addal %d0,%a0                         
    343e:  2648           	moveal %a0,%a3                        
    3440:  3028 0001      	movew %a0@(1),%d0                     
    3444:  e148           	lslw #8,%d0                           
    3446:  7200           	moveq #0,%d1                          
    3448:  3200           	movew %d0,%d1                         
    344a:  8c81           	orl %d1,%d6                           
    344c:  7000           	moveq #0,%d0                          
    344e:  102b 0003      	moveb %a3@(3),%d0                     
    3452:  8c80           	orl %d0,%d6                           
    3454:  2346 001c      	movel %d6,%a1@(28)                    
    3458:  7020           	moveq #32,%d0                         
    345a:  de80           	addl %d0,%d7                          
    345c:  43e9 0020      	lea %a1@(32),%a1                      
    3460:  5082           	addql #8,%d2                          
    3462:  b4ae ffe0      	cmpl %fp@(-32),%d2                    
    3466:  6500 feb0      	bcsw 0x3318                           ; ↑ label_0
    346a:  6000 00e0      	braw 0x354c                           ; ↓ label_12

  label_1:
    346e:  7c00           	moveq #0,%d6                          
    3470:  2044           	moveal %d4,%a0                        
    3472:  d1c7           	addal %d7,%a0                         
    3474:  2648           	moveal %a0,%a3                        
    3476:  3028 0001      	movew %a0@(1),%d0                     
    347a:  e148           	lslw #8,%d0                           
    347c:  7200           	moveq #0,%d1                          
    347e:  3200           	movew %d0,%d1                         
    3480:  8c81           	orl %d1,%d6                           
    3482:  7000           	moveq #0,%d0                          
    3484:  102b 0003      	moveb %a3@(3),%d0                     
    3488:  8c80           	orl %d0,%d6                           
    348a:  2286           	movel %d6,%a1@                        
    348c:  5887           	addql #4,%d7                          
    348e:  5889           	addql #4,%a1                          
    3490:  5282           	addql #1,%d2                          

  label_2:
    3492:  b485           	cmpl %d5,%d2                          
    3494:  65d8           	bcss 0x346e                           ; ↑ label_1
    3496:  6000 00b2      	braw 0x354a                           ; ↓ label_11

  label_3:
    349a:  4a84           	tstl %d4                              
    349c:  664e           	bnes 0x34ec                           ; ↓ label_7
    349e:  7400           	moveq #0,%d2                          
    34a0:  4a85           	tstl %d5                              
    34a2:  6300 00a6      	blsw 0x354a                           ; ↓ label_11
    34a6:  2805           	movel %d5,%d4                         
    34a8:  5184           	subql #8,%d4                          
    34aa:  7008           	moveq #8,%d0                          
    34ac:  ba80           	cmpl %d0,%d5                          
    34ae:  6300 00ac      	blsw 0x355c                           ; ↓ label_13
    34b2:  226e ffe4      	moveal %fp@(-28),%a1                  

  label_4:
    34b6:  4291           	clrl %a1@                             
    34b8:  42a9 0004      	clrl %a1@(4)                          
    34bc:  42a9 0008      	clrl %a1@(8)                          
    34c0:  42a9 000c      	clrl %a1@(12)                         
    34c4:  42a9 0010      	clrl %a1@(16)                         
    34c8:  42a9 0014      	clrl %a1@(20)                         
    34cc:  42a9 0018      	clrl %a1@(24)                         
    34d0:  42a9 001c      	clrl %a1@(28)                         
    34d4:  43e9 0020      	lea %a1@(32),%a1                      
    34d8:  5082           	addql #8,%d2                          
    34da:  b484           	cmpl %d4,%d2                          
    34dc:  65d8           	bcss 0x34b6                           ; ↑ label_4
    34de:  607c           	bras 0x355c                           ; ↓ label_13

  label_5:
    34e0:  4291           	clrl %a1@                             
    34e2:  5889           	addql #4,%a1                          
    34e4:  5282           	addql #1,%d2                          

  label_6:
    34e6:  b485           	cmpl %d5,%d2                          
    34e8:  65f6           	bcss 0x34e0                           ; ↑ label_5
    34ea:  605e           	bras 0x354a                           ; ↓ label_11

  label_7:
    34ec:  7400           	moveq #0,%d2                          
    34ee:  4a85           	tstl %d5                              
    34f0:  6358           	blss 0x354a                           ; ↓ label_11
    34f2:  2c05           	movel %d5,%d6                         
    34f4:  5186           	subql #8,%d6                          
    34f6:  7008           	moveq #8,%d0                          
    34f8:  ba80           	cmpl %d0,%d5                          
    34fa:  636e           	blss 0x356a                           ; ↓ label_14
    34fc:  2244           	moveal %d4,%a1                        
    34fe:  266e ffe4      	moveal %fp@(-28),%a3                  

  label_8:
    3502:  2691           	movel %a1@,%a3@                       
    3504:  2769 0004 0004 	movel %a1@(4),%a3@(4)                 
    350a:  2769 0008 0008 	movel %a1@(8),%a3@(8)                 
    3510:  2769 000c 000c 	movel %a1@(12),%a3@(12)               
    3516:  2769 0010 0010 	movel %a1@(16),%a3@(16)               
    351c:  2769 0014 0014 	movel %a1@(20),%a3@(20)               
    3522:  2769 0018 0018 	movel %a1@(24),%a3@(24)               
    3528:  2769 001c 001c 	movel %a1@(28),%a3@(28)               
    352e:  43e9 0020      	lea %a1@(32),%a1                      
    3532:  47eb 0020      	lea %a3@(32),%a3                      
    3536:  5082           	addql #8,%d2                          
    3538:  b486           	cmpl %d6,%d2                          
    353a:  65c6           	bcss 0x3502                           ; ↑ label_8
    353c:  602c           	bras 0x356a                           ; ↓ label_14

  label_9:
    353e:  2691           	movel %a1@,%a3@                       
    3540:  5889           	addql #4,%a1                          
    3542:  588b           	addql #4,%a3                          
    3544:  5282           	addql #1,%d2                          

  label_10:
    3546:  b485           	cmpl %d5,%d2                          
    3548:  65f4           	bcss 0x353e                           ; ↑ label_9

  label_11:
    354a:  6030           	bras 0x357c                           ; ↓ label_15

  label_12:
    354c:  2e02           	movel %d2,%d7                         
    354e:  e58f           	lsll #2,%d7                           
    3550:  2c07           	movel %d7,%d6                         
    3552:  226e ffe4      	moveal %fp@(-28),%a1                  
    3556:  d3c6           	addal %d6,%a1                         
    3558:  6000 ff38      	braw 0x3492                           ; ↑ label_2

  label_13:
    355c:  2002           	movel %d2,%d0                         
    355e:  e588           	lsll #2,%d0                           
    3560:  226e ffe4      	moveal %fp@(-28),%a1                  
    3564:  d3c0           	addal %d0,%a1                         
    3566:  6000 ff7e      	braw 0x34e6                           ; ↑ label_6

  label_14:
    356a:  2002           	movel %d2,%d0                         
    356c:  e588           	lsll #2,%d0                           
    356e:  2c00           	movel %d0,%d6                         
    3570:  2244           	moveal %d4,%a1                        
    3572:  d3c0           	addal %d0,%a1                         
    3574:  266e ffe4      	moveal %fp@(-28),%a3                  
    3578:  d7c6           	addal %d6,%a3                         
    357a:  60ca           	bras 0x3546                           ; ↑ label_10

  label_15:
    357c:  4cdf 08f0      	moveml %sp@+,%d4-%d7/%a3              
    3580:  4e5e           	unlk %fp                              
    3582:  4e75           	rts                                   
```

# __lmul__: 0x35d0 - 0x35ee

```
    35d0:  48e7 3000      	moveml %d2-%d3,%sp@-                  
    35d4:  2400           	movel %d0,%d2                         
    35d6:  4842           	swap %d2                              
    35d8:  c4c1           	muluw %d1,%d2                         
    35da:  2601           	movel %d1,%d3                         
    35dc:  4843           	swap %d3                              
    35de:  c6c0           	muluw %d0,%d3                         
    35e0:  d443           	addw %d3,%d2                          
    35e2:  4842           	swap %d2                              
    35e4:  4242           	clrw %d2                              
    35e6:  c0c1           	muluw %d1,%d0                         
    35e8:  d082           	addl %d2,%d0                          
    35ea:  4cdf 000c      	moveml %sp@+,%d2-%d3                  
    35ee:  4e75           	rts                                   
```

# anonymous_function_0: 0x35fc - 0x3622

```
    35fc:  48e7 3000      	moveml %d2-%d3,%sp@-                  
    3600:  2401           	movel %d1,%d2                         
    3602:  4242           	clrw %d2                              
    3604:  4842           	swap %d2                              
    3606:  661c           	bnes 0x3624                           
    3608:  3600           	movew %d0,%d3                         
    360a:  4240           	clrw %d0                              
    360c:  4840           	swap %d0                              
    360e:  6706           	beqs 0x3616                           ; ↓ label_0
    3610:  80c1           	divuw %d1,%d0                         
    3612:  3400           	movew %d0,%d2                         
    3614:  4842           	swap %d2                              

  label_0:
    3616:  3003           	movew %d3,%d0                         
    3618:  80c1           	divuw %d1,%d0                         
    361a:  3400           	movew %d0,%d2                         
    361c:  2002           	movel %d2,%d0                         
    361e:  4cdf 000c      	moveml %sp@+,%d2-%d3                  
    3622:  4e75           	rts                                   
```

# anonymous_function_1: 0x3654 - 0x367a

```
    3654:  48e7 3000      	moveml %d2-%d3,%sp@-                  
    3658:  2401           	movel %d1,%d2                         
    365a:  4242           	clrw %d2                              
    365c:  4842           	swap %d2                              
    365e:  661c           	bnes 0x367c                           
    3660:  3600           	movew %d0,%d3                         
    3662:  4240           	clrw %d0                              
    3664:  4840           	swap %d0                              
    3666:  6706           	beqs 0x366e                           ; ↓ label_0
    3668:  80c1           	divuw %d1,%d0                         
    366a:  3400           	movew %d0,%d2                         
    366c:  4842           	swap %d2                              

  label_0:
    366e:  3003           	movew %d3,%d0                         
    3670:  80c1           	divuw %d1,%d0                         
    3672:  4240           	clrw %d0                              
    3674:  4840           	swap %d0                              
    3676:  4cdf 000c      	moveml %sp@+,%d2-%d3                  
    367a:  4e75           	rts                                   
```

