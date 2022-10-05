# MSSlotOpen: 0x0084 - 0x03b6

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-14)`    : ???
   * `%fp@(-46)`    : ???

```
    84:  4e56 ffd2      	linkw %fp,#-46                          
    88:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-                
    8c:  362e 0008      	movew %fp@(8),%d3                       
    90:  2d7c 536c 4d73 	movel #1399606643,%fp@(-14)             
    96:  fff2                                                    
    98:  486e fff6      	pea %fp@(-10)                           
    9c:  3f3c 4e20      	movew #20000,%sp@-                      
    a0:  2f3c 5368 416c 	movel #1399341420,%sp@-                 
    a6:  4e4f           	trap #15                                
    a8:  a27b           	0121173                                 ; sysTrapFtrGet
    aa:  4a40           	tstw %d0                                
    ac:  4fef 000a      	lea %sp@(10),%sp                        
    b0:  6708           	beqs 0xba                               ; ↓ label_0
    b2:  303c 2901      	movew #10497,%d0                        
    b6:  6000 02f8      	braw 0x3b0                              ; ↓ label_18

  label_0:
    ba:  206e fff6      	moveal %fp@(-10),%a0                    
    be:  0c68 0005 0004 	cmpiw #5,%a0@(4)                        
    c4:  6708           	beqs 0xce                               ; ↓ label_1
    c6:  303c 2901      	movew #10497,%d0                        
    ca:  6000 02e4      	braw 0x3b0                              ; ↓ label_18

  label_1:
    ce:  2478 0304      	moveal 0x304,%a2                        
    d2:  2078 0304      	moveal 0x304,%a0                        
    d6:  4aa8 0010      	tstl %a0@(16)                           
    da:  670a           	beqs 0xe6                               ; ↓ label_2
    dc:  206a 0010      	moveal %a2@(16),%a0                     
    e0:  317c 0001 0024 	movew #1,%a0@(36)                       

  label_2:
    e6:  3f03           	movew %d3,%sp@-                         
    e8:  4e4f           	trap #15                                
    ea:  a0b9           	0120271                                 ; sysTrapSysLibTblEntry
    ec:  2468 0004      	moveal %a0@(4),%a2                      
    f0:  200a           	movel %a2,%d0                           
    f2:  544f           	addqw #2,%sp                            
    f4:  6600 0280      	bnew 0x376                              ; ↓ label_16
    f8:  4878 004a      	pea 0x4a                                
    fc:  4e4f           	trap #15                                
    fe:  a013           	0120023                                 ; sysTrapMemPtrNew
    100:  2448           	moveal %a0,%a2                         
    102:  200a           	movel %a2,%d0                          
    104:  584f           	addqw #4,%sp                           
    106:  6608           	bnes 0x110                             ; ↓ label_3
    108:  303c 0102      	movew #258,%d0                         
    10c:  6000 02a2      	braw 0x3b0                             ; ↓ label_18

  label_3:
    110:  4267           	clrw %sp@-                             
    112:  2f0a           	movel %a2,%sp@-                        
    114:  4e4f           	trap #15                               
    116:  a01b           	0120033                                ; sysTrapMemPtrSetOwner
    118:  4227           	clrb %sp@-                             
    11a:  4878 004a      	pea 0x4a                               
    11e:  2f0a           	movel %a2,%sp@-                        
    120:  4e4f           	trap #15                               
    122:  a027           	0120047                                ; sysTrapMemSet
    124:  3f03           	movew %d3,%sp@-                        
    126:  4e4f           	trap #15                               
    128:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    12a:  214a 0004      	movel %a2,%a0@(4)                      
    12e:  486a 001c      	pea %a2@(28)                           
    132:  2f3c 0000 80c0 	movel #32960,%sp@-                     
    138:  3f3c 0001      	movew #1,%sp@-                         
    13c:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    142:  4e4f           	trap #15                               
    144:  a35a           	0121532                                ; sysTrapFtrPtrNew
    146:  4aaa 001c      	tstl %a2@(28)                          
    14a:  4fef 0020      	lea %sp@(32),%sp                       
    14e:  661a           	bnes 0x16a                             ; ↓ label_4
    150:  3f03           	movew %d3,%sp@-                        
    152:  4e4f           	trap #15                               
    154:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    156:  42a8 0004      	clrl %a0@(4)                           
    15a:  2f0a           	movel %a2,%sp@-                        
    15c:  4e4f           	trap #15                               
    15e:  a012           	0120022                                ; sysTrapMemChunkFree
    160:  303c 0102      	movew #258,%d0                         
    164:  5c4f           	addqw #6,%sp                           
    166:  6000 0248      	braw 0x3b0                             ; ↓ label_18

  label_4:
    16a:  486a 0020      	pea %a2@(32)                           
    16e:  4878 2a00      	pea 0x2a00                             
    172:  3f3c 0002      	movew #2,%sp@-                         
    176:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    17c:  4e4f           	trap #15                               
    17e:  a35a           	0121532                                ; sysTrapFtrPtrNew
    180:  4aaa 0020      	tstl %a2@(32)                          
    184:  4fef 000e      	lea %sp@(14),%sp                       
    188:  662a           	bnes 0x1b4                             ; ↓ label_5
    18a:  3f3c 0001      	movew #1,%sp@-                         
    18e:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    194:  4e4f           	trap #15                               
    196:  a35b           	0121533                                ; sysTrapFtrPtrFree
    198:  3f03           	movew %d3,%sp@-                        
    19a:  4e4f           	trap #15                               
    19c:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    19e:  42a8 0004      	clrl %a0@(4)                           
    1a2:  2f0a           	movel %a2,%sp@-                        
    1a4:  4e4f           	trap #15                               
    1a6:  a012           	0120022                                ; sysTrapMemChunkFree
    1a8:  303c 0102      	movew #258,%d0                         
    1ac:  4fef 000c      	lea %sp@(12),%sp                       
    1b0:  6000 01fe      	braw 0x3b0                             ; ↓ label_18

  label_5:
    1b4:  2f0a           	movel %a2,%sp@-                        
    1b6:  4878 0064      	pea 0x64                               
    1ba:  487a 1bc0      	pea %pc@(0x1d7c)                       ; PrvTimerProc
    1be:  486e fff2      	pea %fp@(-14)                          
    1c2:  486a 000e      	pea %a2@(14)                           
    1c6:  4e4f           	trap #15                               
    1c8:  a099           	0120231                                ; sysTrapSysTimerCreate
    1ca:  3800           	movew %d0,%d4                          
    1cc:  4fef 0014      	lea %sp@(20),%sp                       
    1d0:  6736           	beqs 0x208                             ; ↓ label_6
    1d2:  3f3c 0001      	movew #1,%sp@-                         
    1d6:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    1dc:  4e4f           	trap #15                               
    1de:  a35b           	0121533                                ; sysTrapFtrPtrFree
    1e0:  3f3c 0002      	movew #2,%sp@-                         
    1e4:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    1ea:  4e4f           	trap #15                               
    1ec:  a35b           	0121533                                ; sysTrapFtrPtrFree
    1ee:  3f03           	movew %d3,%sp@-                        
    1f0:  4e4f           	trap #15                               
    1f2:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    1f4:  42a8 0004      	clrl %a0@(4)                           
    1f8:  2f0a           	movel %a2,%sp@-                        
    1fa:  4e4f           	trap #15                               
    1fc:  a012           	0120022                                ; sysTrapMemChunkFree
    1fe:  3004           	movew %d4,%d0                          
    200:  4fef 0012      	lea %sp@(18),%sp                       
    204:  6000 01aa      	braw 0x3b0                             ; ↓ label_18

  label_6:
    208:  4ab8 0304      	tstl 0x304                             
    20c:  6714           	beqs 0x222                             ; ↓ label_9
    20e:  2078 0304      	moveal 0x304,%a0                       
    212:  0c90 5368 4776 	cmpil #1399342966,%a0@                 
    218:  6604           	bnes 0x21e                             ; ↓ label_7
    21a:  7001           	moveq #1,%d0                           
    21c:  6002           	bras 0x220                             ; ↓ label_8

  label_7:
    21e:  7000           	moveq #0,%d0                           

  label_8:
    220:  6002           	bras 0x224                             ; ↓ label_10

  label_9:
    222:  7000           	moveq #0,%d0                           

  label_10:
    224:  4a40           	tstw %d0                               
    226:  6640           	bnes 0x268                             ; ↓ label_11
    228:  2f2a 000e      	movel %a2@(14),%sp@-                   
    22c:  4e4f           	trap #15                               
    22e:  a0bf           	0120277                                ; sysTrapSysTimerDelete
    230:  3f3c 0001      	movew #1,%sp@-                         
    234:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    23a:  4e4f           	trap #15                               
    23c:  a35b           	0121533                                ; sysTrapFtrPtrFree
    23e:  3f3c 0002      	movew #2,%sp@-                         
    242:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    248:  4e4f           	trap #15                               
    24a:  a35b           	0121533                                ; sysTrapFtrPtrFree
    24c:  3f03           	movew %d3,%sp@-                        
    24e:  4e4f           	trap #15                               
    250:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    252:  42a8 0004      	clrl %a0@(4)                           
    256:  2f0a           	movel %a2,%sp@-                        
    258:  4e4f           	trap #15                               
    25a:  a012           	0120022                                ; sysTrapMemChunkFree
    25c:  303c 0102      	movew #258,%d0                         
    260:  4fef 0016      	lea %sp@(22),%sp                       
    264:  6000 014a      	braw 0x3b0                             ; ↓ label_18

  label_11:
    268:  2078 0304      	moveal 0x304,%a0                       
    26c:  214a 0024      	movel %a2,%a0@(36)                     
    270:  486a 0024      	pea %a2@(36)                           
    274:  41fa 0150      	lea %pc@(0x3c6),%a0                    ; ??? 0x3c6
    278:  4850           	pea %a0@                               
    27a:  4e4f           	trap #15                               
    27c:  a0ba           	0120272                                ; sysTrapSysLibFind
    27e:  3800           	movew %d0,%d4                          
    280:  504f           	addqw #8,%sp                           
    282:  671a           	beqs 0x29e                             ; ↓ label_12
    284:  486a 0024      	pea %a2@(36)                           
    288:  2f3c 536c 4d67 	movel #1399606631,%sp@-                
    28e:  2f3c 6c69 6272 	movel #1818845810,%sp@-                
    294:  4e4f           	trap #15                               
    296:  a2ac           	0121254                                ; sysTrapSysLibLoad
    298:  3800           	movew %d0,%d4                          
    29a:  4fef 000c      	lea %sp@(12),%sp                       

  label_12:
    29e:  4a44           	tstw %d4                               
    2a0:  670a           	beqs 0x2ac                             ; ↓ label_13
    2a2:  02aa ffff feff 	andil #-257,%a2@(4)                    
    2a8:  0004                                                   
    2aa:  6020           	bras 0x2cc                             ; ↓ label_15

  label_13:
    2ac:  3f2a 0024      	movew %a2@(36),%sp@-                   
    2b0:  4e4f           	trap #15                               
    2b2:  a801           	0124001                                ; ??? 0xa801
    2b4:  3800           	movew %d0,%d4                          
    2b6:  544f           	addqw #2,%sp                           
    2b8:  670a           	beqs 0x2c4                             ; ↓ label_14
    2ba:  02aa ffff feff 	andil #-257,%a2@(4)                    
    2c0:  0004                                                   
    2c2:  6008           	bras 0x2cc                             ; ↓ label_15

  label_14:
    2c4:  00aa 0000 0100 	oril #256,%a2@(4)                      
    2ca:  0004                                                   

  label_15:
    2cc:  4eba 1de4      	jsr %pc@(0x20b2)                       ; MSSlotHwrOpen
    2d0:  486a 0012      	pea %a2@(18)                           
    2d4:  3f03           	movew %d3,%sp@-                        
    2d6:  7404           	moveq #4,%d2                           
    2d8:  4e4f           	trap #15                               
    2da:  a347           	0121507                                ; sysTrapExpansionDispatch
    2dc:  486e fffa      	pea %fp@(-6)                           
    2e0:  486e fffe      	pea %fp@(-2)                           
    2e4:  1f3c 0001      	moveb #1,%sp@-                         
    2e8:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    2ee:  2f3c 6c69 6273 	movel #1818845811,%sp@-                
    2f4:  486e ffd2      	pea %fp@(-46)                          
    2f8:  1f3c 0001      	moveb #1,%sp@-                         
    2fc:  4e4f           	trap #15                               
    2fe:  a078           	0120170                                ; sysTrapDmGetNextDatabaseByTypeCreator
    300:  2f0a           	movel %a2,%sp@-                        
    302:  1f3c fff6      	moveb #-10,%sp@-                       
    306:  487a 179a      	pea %pc@(0x1aa2)                       ; PrvCrdiNotificationHandler
    30a:  2f3c 6372 6469 	movel #1668441193,%sp@-                
    310:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    314:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    318:  4e4f           	trap #15                               
    31a:  a353           	0121523                                ; sysTrapSysNotifyRegister
    31c:  2f0a           	movel %a2,%sp@-                        
    31e:  1f3c fff6      	moveb #-10,%sp@-                       
    322:  487a 17c6      	pea %pc@(0x1aea)                       ; PrvCrdoNotificationHandler
    326:  2f3c 6372 646f 	movel #1668441199,%sp@-                
    32c:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    330:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    334:  4e4f           	trap #15                               
    336:  a353           	0121523                                ; sysTrapSysNotifyRegister
    338:  2f0a           	movel %a2,%sp@-                        
    33a:  1f3c fff6      	moveb #-10,%sp@-                       
    33e:  487a 182a      	pea %pc@(0x1b6a)                       ; PrvVolmNotificationHandler
    342:  2f3c 766f 6c6d 	movel #1987013741,%sp@-                
    348:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    34c:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    350:  4e4f           	trap #15                               
    352:  a353           	0121523                                ; sysTrapSysNotifyRegister
    354:  700c           	moveq #12,%d0                          
    356:  81aa 0004      	orl %d0,%a2@(4)                        
    35a:  4227           	clrb %sp@-                             
    35c:  4227           	clrb %sp@-                             
    35e:  4eba 2014      	jsr %pc@(0x2374)                       ; MSSlotHwrSetUpInterrupt
    362:  1f3c 0001      	moveb #1,%sp@-                         
    366:  1f3c 0001      	moveb #1,%sp@-                         
    36a:  4eba 2008      	jsr %pc@(0x2374)                       ; MSSlotHwrSetUpInterrupt
    36e:  42aa 0046      	clrl %a2@(70)                          
    372:  4fef 0062      	lea %sp@(98),%sp                       

  label_16:
    376:  4aaa 0046      	tstl %a2@(70)                          
    37a:  6630           	bnes 0x3ac                             ; ↓ label_17
    37c:  4879 1100 8000 	pea 0x11008000                         
    382:  2f3c 5368 4473 	movel #1399342195,%sp@-                
    388:  2f3c 4473 7331 	movel #1148416817,%sp@-                
    38e:  2078 0304      	moveal 0x304,%a0                       
    392:  2068 0018      	moveal %a0@(24),%a0                    
    396:  2068 0092      	moveal %a0@(146),%a0                   
    39a:  4e90           	jsr %a0@                               
    39c:  4a40           	tstw %d0                               
    39e:  4fef 000c      	lea %sp@(12),%sp                       
    3a2:  6608           	bnes 0x3ac                             ; ↓ label_17
    3a4:  257c 4473 7331 	movel #1148416817,%a2@(70)             
    3aa:  0046                                                   

  label_17:
    3ac:  5292           	addql #1,%a2@                          
    3ae:  7000           	moveq #0,%d0                           

  label_18:
    3b0:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2               
    3b4:  4e5e           	unlk %fp                               
    3b6:  4e75           	rts                                    
```

# MSSlotClose: 0x03da - 0x04f2

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-38)`    : ???

```
    3da:  4e56 ffda      	linkw %fp,#-38                         
    3de:  2f0a           	movel %a2,%sp@-                        
    3e0:  3f2e 0008      	movew %fp@(8),%sp@-                    
    3e4:  4e4f           	trap #15                               
    3e6:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    3e8:  2468 0004      	moveal %a0@(4),%a2                     
    3ec:  200a           	movel %a2,%d0                          
    3ee:  544f           	addqw #2,%sp                           
    3f0:  6704           	beqs 0x3f6                             ; ↓ label_0
    3f2:  4a92           	tstl %a2@                              
    3f4:  6608           	bnes 0x3fe                             ; ↓ label_1

  label_0:
    3f6:  303c 290a      	movew #10506,%d0                       
    3fa:  6000 00f2      	braw 0x4ee                             ; ↓ label_5

  label_1:
    3fe:  5392           	subql #1,%a2@                          
    400:  6708           	beqs 0x40a                             ; ↓ label_2
    402:  303c 290b      	movew #10507,%d0                       
    406:  6000 00e6      	braw 0x4ee                             ; ↓ label_5

  label_2:
    40a:  4a92           	tstl %a2@                              
    40c:  6600 00de      	bnew 0x4ec                             ; ↓ label_4
    410:  202a 0004      	movel %a2@(4),%d0                      
    414:  0280 0000 0100 	andil #256,%d0                         
    41a:  670a           	beqs 0x426                             ; ↓ label_3
    41c:  3f2a 0024      	movew %a2@(36),%sp@-                   
    420:  4e4f           	trap #15                               
    422:  a802           	0124002                                ; ??? 0xa802
    424:  544f           	addqw #2,%sp                           

  label_3:
    426:  4227           	clrb %sp@-                             
    428:  4eba 1d4c      	jsr %pc@(0x2176)                       ; MSSlotHwrSupplyPower
    42c:  4eba 1cfc      	jsr %pc@(0x212a)                       ; MSSlotHwrClose
    430:  42a7           	clrl %sp@-                             
    432:  2f2a 000e      	movel %a2@(14),%sp@-                   
    436:  4e4f           	trap #15                               
    438:  a09a           	0120232                                ; sysTrapSysTimerWrite
    43a:  2f2a 000e      	movel %a2@(14),%sp@-                   
    43e:  4e4f           	trap #15                               
    440:  a0bf           	0120277                                ; sysTrapSysTimerDelete
    442:  486e fffa      	pea %fp@(-6)                           
    446:  486e fffe      	pea %fp@(-2)                           
    44a:  1f3c 0001      	moveb #1,%sp@-                         
    44e:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    454:  2f3c 6c69 6273 	movel #1818845811,%sp@-                
    45a:  486e ffda      	pea %fp@(-38)                          
    45e:  1f3c 0001      	moveb #1,%sp@-                         
    462:  4e4f           	trap #15                               
    464:  a078           	0120170                                ; sysTrapDmGetNextDatabaseByTypeCreator
    466:  1f3c fff6      	moveb #-10,%sp@-                       
    46a:  2f3c 6372 6469 	movel #1668441193,%sp@-                
    470:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    474:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    478:  4e4f           	trap #15                               
    47a:  a354           	0121524                                ; sysTrapSysNotifyUnregister
    47c:  1f3c fff6      	moveb #-10,%sp@-                       
    480:  2f3c 6372 646f 	movel #1668441199,%sp@-                
    486:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    48a:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    48e:  4e4f           	trap #15                               
    490:  a354           	0121524                                ; sysTrapSysNotifyUnregister
    492:  1f3c fff6      	moveb #-10,%sp@-                       
    496:  2f3c 766f 6c6d 	movel #1987013741,%sp@-                
    49c:  2f2e fffa      	movel %fp@(-6),%sp@-                   
    4a0:  3f2e fffe      	movew %fp@(-2),%sp@-                   
    4a4:  4e4f           	trap #15                               
    4a6:  a354           	0121524                                ; sysTrapSysNotifyUnregister
    4a8:  3f2a 0012      	movew %a2@(18),%sp@-                   
    4ac:  7405           	moveq #5,%d2                           
    4ae:  4e4f           	trap #15                               
    4b0:  a347           	0121507                                ; sysTrapExpansionDispatch
    4b2:  3f3c 0001      	movew #1,%sp@-                         
    4b6:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    4bc:  4e4f           	trap #15                               
    4be:  a35b           	0121533                                ; sysTrapFtrPtrFree
    4c0:  3f3c 0002      	movew #2,%sp@-                         
    4c4:  2f3c 536c 4d73 	movel #1399606643,%sp@-                
    4ca:  4e4f           	trap #15                               
    4cc:  a35b           	0121533                                ; sysTrapFtrPtrFree
    4ce:  2078 0304      	moveal 0x304,%a0                       
    4d2:  42a8 0024      	clrl %a0@(36)                          
    4d6:  3f2e 0008      	movew %fp@(8),%sp@-                    
    4da:  4e4f           	trap #15                               
    4dc:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    4de:  42a8 0004      	clrl %a0@(4)                           
    4e2:  2f0a           	movel %a2,%sp@-                        
    4e4:  4e4f           	trap #15                               
    4e6:  a012           	0120022                                ; sysTrapMemChunkFree
    4e8:  4fef 005e      	lea %sp@(94),%sp                       

  label_4:
    4ec:  7000           	moveq #0,%d0                           

  label_5:
    4ee:  245f           	moveal %sp@+,%a2                       
    4f0:  4e5e           	unlk %fp                               
    4f2:  4e75           	rts                                    
```

# MSSlotSleep: 0x0502 - 0x0578

Parameters:

   * `%fp@(8)`      : ???

```
    502:  4e56 0000      	linkw %fp,#0                           
    506:  2f0a           	movel %a2,%sp@-                        
    508:  3f2e 0008      	movew %fp@(8),%sp@-                    
    50c:  4e4f           	trap #15                               
    50e:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    510:  2468 0004      	moveal %a0@(4),%a2                     
    514:  200a           	movel %a2,%d0                          
    516:  544f           	addqw #2,%sp                           
    518:  6606           	bnes 0x520                             ; ↓ label_0
    51a:  303c 290a      	movew #10506,%d0                       
    51e:  6054           	bras 0x574                             ; ↓ label_2

  label_0:
    520:  2f0a           	movel %a2,%sp@-                        
    522:  4eba 1d92      	jsr %pc@(0x22b6)                       ; DisableIntFromIOStick
    526:  022a 00fe 002f 	andib #-2,%a2@(47)                     
    52c:  2f0a           	movel %a2,%sp@-                        
    52e:  4eba 5022      	jsr %pc@(0x5552)                       ; InitializeIOParam
    532:  2f0a           	movel %a2,%sp@-                        
    534:  4eba 505a      	jsr %pc@(0x5590)                       ; ReturnToMS
    538:  4227           	clrb %sp@-                             
    53a:  4eba 1c3a      	jsr %pc@(0x2176)                       ; MSSlotHwrSupplyPower
    53e:  02aa ffff ff7f 	andil #-129,%a2@(4)                    
    544:  0004                                                   
    546:  42a7           	clrl %sp@-                             
    548:  2f2a 000e      	movel %a2@(14),%sp@-                   
    54c:  4e4f           	trap #15                               
    54e:  a09a           	0120232                                ; sysTrapSysTimerWrite
    550:  0c38 0001 02ca 	cmpib #1,0x2ca                         
    556:  4fef 0016      	lea %sp@(22),%sp                       
    55a:  6616           	bnes 0x572                             ; ↓ label_1
    55c:  7001           	moveq #1,%d0                           
    55e:  c0aa 0004      	andl %a2@(4),%d0                       
    562:  670e           	beqs 0x572                             ; ↓ label_1
    564:  7002           	moveq #2,%d0                           
    566:  81aa 0004      	orl %d0,%a2@(4)                        
    56a:  4227           	clrb %sp@-                             
    56c:  4eba 1e66      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    570:  544f           	addqw #2,%sp                           

  label_1:
    572:  7000           	moveq #0,%d0                           

  label_2:
    574:  245f           	moveal %sp@+,%a2                       
    576:  4e5e           	unlk %fp                               
    578:  4e75           	rts                                    
```

# MSSlotWake: 0x0588 - 0x05f0

Parameters:

   * `%fp@(8)`      : ???

```
    588:  4e56 0000      	linkw %fp,#0                           
    58c:  2f0a           	movel %a2,%sp@-                        
    58e:  3f2e 0008      	movew %fp@(8),%sp@-                    
    592:  4e4f           	trap #15                               
    594:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    596:  2468 0004      	moveal %a0@(4),%a2                     
    59a:  200a           	movel %a2,%d0                          
    59c:  544f           	addqw #2,%sp                           
    59e:  6606           	bnes 0x5a6                             ; ↓ label_0
    5a0:  303c 290a      	movew #10506,%d0                       
    5a4:  6046           	bras 0x5ec                             ; ↓ label_4

  label_0:
    5a6:  7002           	moveq #2,%d0                           
    5a8:  c0aa 0004      	andl %a2@(4),%d0                       
    5ac:  6714           	beqs 0x5c2                             ; ↓ label_1
    5ae:  70fd           	moveq #-3,%d0                          
    5b0:  c1aa 0004      	andl %d0,%a2@(4)                       
    5b4:  7001           	moveq #1,%d0                           
    5b6:  c0aa 0004      	andl %a2@(4),%d0                       
    5ba:  6706           	beqs 0x5c2                             ; ↓ label_1
    5bc:  70f3           	moveq #-13,%d0                         
    5be:  c1aa 0004      	andl %d0,%a2@(4)                       

  label_1:
    5c2:  7008           	moveq #8,%d0                           
    5c4:  c0aa 0004      	andl %a2@(4),%d0                       
    5c8:  660c           	bnes 0x5d6                             ; ↓ label_2
    5ca:  3f2a 0012      	movew %a2@(18),%sp@-                   
    5ce:  7407           	moveq #7,%d2                           
    5d0:  4e4f           	trap #15                               
    5d2:  a347           	0121507                                ; sysTrapExpansionDispatch
    5d4:  544f           	addqw #2,%sp                           

  label_2:
    5d6:  7004           	moveq #4,%d0                           
    5d8:  c0aa 0004      	andl %a2@(4),%d0                       
    5dc:  660c           	bnes 0x5ea                             ; ↓ label_3
    5de:  3f2a 0012      	movew %a2@(18),%sp@-                   
    5e2:  7406           	moveq #6,%d2                           
    5e4:  4e4f           	trap #15                               
    5e6:  a347           	0121507                                ; sysTrapExpansionDispatch
    5e8:  544f           	addqw #2,%sp                           

  label_3:
    5ea:  7000           	moveq #0,%d0                           

  label_4:
    5ec:  245f           	moveal %sp@+,%a2                       
    5ee:  4e5e           	unlk %fp                               
    5f0:  4e75           	rts                                    
```

# MSSlotGetVersion: 0x0600 - 0x0608

```
    600:  4e56 0000      	linkw %fp,#0                           
    604:  7002           	moveq #2,%d0                           
    606:  4e5e           	unlk %fp                               
    608:  4e75           	rts                                    
```

# MSSlotExecCustomControl: 0x061e - 0x1020

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(818)`    : ???

Locals:

   * `%fp@(-4)`     : ???

```
    61e:  4e56 fffc      	linkw %fp,#-4                          
    622:  48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-           
    626:  382e 0008      	movew %fp@(8),%d4                      
    62a:  362e 000e      	movew %fp@(14),%d3                     
    62e:  266e 0010      	moveal %fp@(16),%a3                    
    632:  3f04           	movew %d4,%sp@-                        
    634:  4e4f           	trap #15                               
    636:  a0b9           	0120271                                ; sysTrapSysLibTblEntry
    638:  2468 0004      	moveal %a0@(4),%a2                     
    63c:  200a           	movel %a2,%d0                          
    63e:  544f           	addqw #2,%sp                           
    640:  6608           	bnes 0x64a                             ; ↓ label_0
    642:  303c 290a      	movew #10506,%d0                       
    646:  6000 09d2      	braw 0x101a                            ; ↓ label_90

  label_0:
    64a:  0cae 536f 4e79 	cmpil #1399803513,%fp@(10)             
    650:  000a                                                   
    652:  6708           	beqs 0x65c                             ; ↓ label_1
    654:  303c 2901      	movew #10497,%d0                       
    658:  6000 09c0      	braw 0x101a                            ; ↓ label_90

  label_1:
    65c:  3003           	movew %d3,%d0                          
    65e:  0240 0010      	andiw #16,%d0                          
    662:  6700 03bc      	beqw 0xa20                             ; ↓ label_30
    666:  3003           	movew %d3,%d0                          
    668:  0440 2110      	subiw #8464,%d0                        
    66c:  0c40 0008      	cmpiw #8,%d0                           
    670:  6200 03a6      	bhiw 0xa18                             ; ↓ label_29
    674:  d040           	addw %d0,%d0                           
    676:  303b 0006      	movew %pc@(0x67e,%d0:w),%d0            
    67a:  4efb 0002      	jmp %pc@(0x67e,%d0:w)                  
    67e:  00de           	0336                                   
    680:  0012 0076      	orib #118,%a2@                         
    684:  0146           	bchg %d0,%d6                           
    686:  01aa 020e      	bclr %d0,%a2@(526)                     
    68a:  026e 02ca 0332 	andiw #714,%fp@(818)                   
    690:  302a 0012      	movew %a2@(18),%d0                     
    694:  b053           	cmpw %a3@,%d0                          
    696:  6708           	beqs 0x6a0                             ; ↓ label_2
    698:  363c 8001      	movew #-32767,%d3                      
    69c:  6000 097a      	braw 0x1018                            ; ↓ label_89

  label_2:
    6a0:  7001           	moveq #1,%d0                           
    6a2:  c0aa 0004      	andl %a2@(4),%d0                       
    6a6:  6708           	beqs 0x6b0                             ; ↓ label_3
    6a8:  7008           	moveq #8,%d0                           
    6aa:  c0aa 0004      	andl %a2@(4),%d0                       
    6ae:  6608           	bnes 0x6b8                             ; ↓ label_4

  label_3:
    6b0:  363c 8002      	movew #-32766,%d3                      
    6b4:  6000 0962      	braw 0x1018                            ; ↓ label_89

  label_4:
    6b8:  1f3c 0001      	moveb #1,%sp@-                         
    6bc:  4eba 1d16      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    6c0:  1800           	moveb %d0,%d4                          
    6c2:  2f2b 000a      	movel %a3@(10),%sp@-                   
    6c6:  1f2b 0008      	moveb %a3@(8),%sp@-                    
    6ca:  2f2b 0004      	movel %a3@(4),%sp@-                    
    6ce:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    6d2:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    6d6:  2f0a           	movel %a2,%sp@-                        
    6d8:  4eba 48a4      	jsr %pc@(0x4f7e)                       ; IOJobTPCGetInt
    6dc:  3600           	movew %d0,%d3                          
    6de:  4a04           	tstb %d4                               
    6e0:  4fef 0014      	lea %sp@(20),%sp                       
    6e4:  6600 0932      	bnew 0x1018                            ; ↓ label_89
    6e8:  4227           	clrb %sp@-                             
    6ea:  4eba 1ce8      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    6ee:  544f           	addqw #2,%sp                           
    6f0:  6000 0926      	braw 0x1018                            ; ↓ label_89
    6f4:  302a 0012      	movew %a2@(18),%d0                     
    6f8:  b053           	cmpw %a3@,%d0                          
    6fa:  6708           	beqs 0x704                             ; ↓ label_5
    6fc:  363c 8001      	movew #-32767,%d3                      
    700:  6000 0916      	braw 0x1018                            ; ↓ label_89

  label_5:
    704:  7001           	moveq #1,%d0                           
    706:  c0aa 0004      	andl %a2@(4),%d0                       
    70a:  6708           	beqs 0x714                             ; ↓ label_6
    70c:  7008           	moveq #8,%d0                           
    70e:  c0aa 0004      	andl %a2@(4),%d0                       
    712:  6608           	bnes 0x71c                             ; ↓ label_7

  label_6:
    714:  363c 8002      	movew #-32766,%d3                      
    718:  6000 08fe      	braw 0x1018                            ; ↓ label_89

  label_7:
    71c:  1f3c 0001      	moveb #1,%sp@-                         
    720:  4eba 1cb2      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    724:  1800           	moveb %d0,%d4                          
    726:  2f2b 000a      	movel %a3@(10),%sp@-                   
    72a:  1f2b 0009      	moveb %a3@(9),%sp@-                    
    72e:  1f2b 0008      	moveb %a3@(8),%sp@-                    
    732:  2f2b 0004      	movel %a3@(4),%sp@-                    
    736:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    73a:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    73e:  2f0a           	movel %a2,%sp@-                        
    740:  4eba 4656      	jsr %pc@(0x4d98)                       ; IOJobSetReg
    744:  3600           	movew %d0,%d3                          
    746:  4a04           	tstb %d4                               
    748:  4fef 0016      	lea %sp@(22),%sp                       
    74c:  6600 08ca      	bnew 0x1018                            ; ↓ label_89
    750:  4227           	clrb %sp@-                             
    752:  4eba 1c80      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    756:  544f           	addqw #2,%sp                           
    758:  6000 08be      	braw 0x1018                            ; ↓ label_89
    75c:  302a 0012      	movew %a2@(18),%d0                     
    760:  b053           	cmpw %a3@,%d0                          
    762:  6708           	beqs 0x76c                             ; ↓ label_8
    764:  363c 8001      	movew #-32767,%d3                      
    768:  6000 08ae      	braw 0x1018                            ; ↓ label_89

  label_8:
    76c:  7001           	moveq #1,%d0                           
    76e:  c0aa 0004      	andl %a2@(4),%d0                       
    772:  6708           	beqs 0x77c                             ; ↓ label_9
    774:  7008           	moveq #8,%d0                           
    776:  c0aa 0004      	andl %a2@(4),%d0                       
    77a:  6608           	bnes 0x784                             ; ↓ label_10

  label_9:
    77c:  363c 8002      	movew #-32766,%d3                      
    780:  6000 0896      	braw 0x1018                            ; ↓ label_89

  label_10:
    784:  1f3c 0001      	moveb #1,%sp@-                         
    788:  4eba 1c4a      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    78c:  1800           	moveb %d0,%d4                          
    78e:  2f2b 000a      	movel %a3@(10),%sp@-                   
    792:  1f2b 0009      	moveb %a3@(9),%sp@-                    
    796:  1f2b 0008      	moveb %a3@(8),%sp@-                    
    79a:  2f2b 0004      	movel %a3@(4),%sp@-                    
    79e:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    7a2:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    7a6:  2f0a           	movel %a2,%sp@-                        
    7a8:  4eba 46b4      	jsr %pc@(0x4e5e)                       ; IOJobGetReg
    7ac:  3600           	movew %d0,%d3                          
    7ae:  4a04           	tstb %d4                               
    7b0:  4fef 0016      	lea %sp@(22),%sp                       
    7b4:  6600 0862      	bnew 0x1018                            ; ↓ label_89
    7b8:  4227           	clrb %sp@-                             
    7ba:  4eba 1c18      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    7be:  544f           	addqw #2,%sp                           
    7c0:  6000 0856      	braw 0x1018                            ; ↓ label_89
    7c4:  302a 0012      	movew %a2@(18),%d0                     
    7c8:  b053           	cmpw %a3@,%d0                          
    7ca:  6708           	beqs 0x7d4                             ; ↓ label_11
    7cc:  363c 8001      	movew #-32767,%d3                      
    7d0:  6000 0846      	braw 0x1018                            ; ↓ label_89

  label_11:
    7d4:  7001           	moveq #1,%d0                           
    7d6:  c0aa 0004      	andl %a2@(4),%d0                       
    7da:  6708           	beqs 0x7e4                             ; ↓ label_12
    7dc:  7008           	moveq #8,%d0                           
    7de:  c0aa 0004      	andl %a2@(4),%d0                       
    7e2:  6608           	bnes 0x7ec                             ; ↓ label_13

  label_12:
    7e4:  363c 8002      	movew #-32766,%d3                      
    7e8:  6000 082e      	braw 0x1018                            ; ↓ label_89

  label_13:
    7ec:  1f3c 0001      	moveb #1,%sp@-                         
    7f0:  4eba 1be2      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    7f4:  1800           	moveb %d0,%d4                          
    7f6:  2f2b 000a      	movel %a3@(10),%sp@-                   
    7fa:  3f2b 0008      	movew %a3@(8),%sp@-                    
    7fe:  2f2b 0004      	movel %a3@(4),%sp@-                    
    802:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    806:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    80a:  2f0a           	movel %a2,%sp@-                        
    80c:  4eba 49e4      	jsr %pc@(0x51f2)                       ; IOJobTPCReadData
    810:  3600           	movew %d0,%d3                          
    812:  4a04           	tstb %d4                               
    814:  4fef 0014      	lea %sp@(20),%sp                       
    818:  6600 07fe      	bnew 0x1018                            ; ↓ label_89
    81c:  4227           	clrb %sp@-                             
    81e:  4eba 1bb4      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    822:  544f           	addqw #2,%sp                           
    824:  6000 07f2      	braw 0x1018                            ; ↓ label_89
    828:  302a 0012      	movew %a2@(18),%d0                     
    82c:  b053           	cmpw %a3@,%d0                          
    82e:  6708           	beqs 0x838                             ; ↓ label_14
    830:  363c 8001      	movew #-32767,%d3                      
    834:  6000 07e2      	braw 0x1018                            ; ↓ label_89

  label_14:
    838:  7001           	moveq #1,%d0                           
    83a:  c0aa 0004      	andl %a2@(4),%d0                       
    83e:  6708           	beqs 0x848                             ; ↓ label_15
    840:  7008           	moveq #8,%d0                           
    842:  c0aa 0004      	andl %a2@(4),%d0                       
    846:  6608           	bnes 0x850                             ; ↓ label_16

  label_15:
    848:  363c 8002      	movew #-32766,%d3                      
    84c:  6000 07ca      	braw 0x1018                            ; ↓ label_89

  label_16:
    850:  1f3c 0001      	moveb #1,%sp@-                         
    854:  4eba 1b7e      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    858:  1800           	moveb %d0,%d4                          
    85a:  2f2b 000a      	movel %a3@(10),%sp@-                   
    85e:  3f2b 0008      	movew %a3@(8),%sp@-                    
    862:  2f2b 0004      	movel %a3@(4),%sp@-                    
    866:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    86a:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    86e:  2f0a           	movel %a2,%sp@-                        
    870:  4eba 48d6      	jsr %pc@(0x5148)                       ; IOJobTPCWriteData
    874:  3600           	movew %d0,%d3                          
    876:  4a04           	tstb %d4                               
    878:  4fef 0014      	lea %sp@(20),%sp                       
    87c:  6600 079a      	bnew 0x1018                            ; ↓ label_89
    880:  4227           	clrb %sp@-                             
    882:  4eba 1b50      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    886:  544f           	addqw #2,%sp                           
    888:  6000 078e      	braw 0x1018                            ; ↓ label_89
    88c:  302a 0012      	movew %a2@(18),%d0                     
    890:  b053           	cmpw %a3@,%d0                          
    892:  6708           	beqs 0x89c                             ; ↓ label_17
    894:  363c 8001      	movew #-32767,%d3                      
    898:  6000 077e      	braw 0x1018                            ; ↓ label_89

  label_17:
    89c:  7001           	moveq #1,%d0                           
    89e:  c0aa 0004      	andl %a2@(4),%d0                       
    8a2:  6708           	beqs 0x8ac                             ; ↓ label_18
    8a4:  7008           	moveq #8,%d0                           
    8a6:  c0aa 0004      	andl %a2@(4),%d0                       
    8aa:  6608           	bnes 0x8b4                             ; ↓ label_19

  label_18:
    8ac:  363c 8002      	movew #-32766,%d3                      
    8b0:  6000 0766      	braw 0x1018                            ; ↓ label_89

  label_19:
    8b4:  1f3c 0001      	moveb #1,%sp@-                         
    8b8:  4eba 1b1a      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    8bc:  1800           	moveb %d0,%d4                          
    8be:  2f2b 0006      	movel %a3@(6),%sp@-                    
    8c2:  1f2b 0004      	moveb %a3@(4),%sp@-                    
    8c6:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    8ca:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    8ce:  2f0a           	movel %a2,%sp@-                        
    8d0:  4eba 49ca      	jsr %pc@(0x529c)                       ; IOJobTPCSetCmd
    8d4:  3600           	movew %d0,%d3                          
    8d6:  4a04           	tstb %d4                               
    8d8:  4fef 0010      	lea %sp@(16),%sp                       
    8dc:  6600 073a      	bnew 0x1018                            ; ↓ label_89
    8e0:  4227           	clrb %sp@-                             
    8e2:  4eba 1af0      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    8e6:  544f           	addqw #2,%sp                           
    8e8:  6000 072e      	braw 0x1018                            ; ↓ label_89
    8ec:  302a 0012      	movew %a2@(18),%d0                     
    8f0:  b053           	cmpw %a3@,%d0                          
    8f2:  6708           	beqs 0x8fc                             ; ↓ label_20
    8f4:  363c 8001      	movew #-32767,%d3                      
    8f8:  6000 071e      	braw 0x1018                            ; ↓ label_89

  label_20:
    8fc:  7001           	moveq #1,%d0                           
    8fe:  c0aa 0004      	andl %a2@(4),%d0                       
    902:  6708           	beqs 0x90c                             ; ↓ label_21
    904:  7008           	moveq #8,%d0                           
    906:  c0aa 0004      	andl %a2@(4),%d0                       
    90a:  6608           	bnes 0x914                             ; ↓ label_22

  label_21:
    90c:  363c 8002      	movew #-32766,%d3                      
    910:  6000 0706      	braw 0x1018                            ; ↓ label_89

  label_22:
    914:  1f3c 0001      	moveb #1,%sp@-                         
    918:  4eba 1aba      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    91c:  1800           	moveb %d0,%d4                          
    91e:  2f2b 0004      	movel %a3@(4),%sp@-                    
    922:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    926:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    92a:  2f0a           	movel %a2,%sp@-                        
    92c:  4eba 4a04      	jsr %pc@(0x5332)                       ; IOJobWaitStickInt
    930:  3600           	movew %d0,%d3                          
    932:  4a04           	tstb %d4                               
    934:  4fef 000e      	lea %sp@(14),%sp                       
    938:  6600 06de      	bnew 0x1018                            ; ↓ label_89
    93c:  4227           	clrb %sp@-                             
    93e:  4eba 1a94      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    942:  544f           	addqw #2,%sp                           
    944:  6000 06d2      	braw 0x1018                            ; ↓ label_89
    948:  302a 0012      	movew %a2@(18),%d0                     
    94c:  b053           	cmpw %a3@,%d0                          
    94e:  6708           	beqs 0x958                             ; ↓ label_23
    950:  363c 8001      	movew #-32767,%d3                      
    954:  6000 06c2      	braw 0x1018                            ; ↓ label_89

  label_23:
    958:  7001           	moveq #1,%d0                           
    95a:  c0aa 0004      	andl %a2@(4),%d0                       
    95e:  6708           	beqs 0x968                             ; ↓ label_24
    960:  7008           	moveq #8,%d0                           
    962:  c0aa 0004      	andl %a2@(4),%d0                       
    966:  6608           	bnes 0x970                             ; ↓ label_25

  label_24:
    968:  363c 8002      	movew #-32766,%d3                      
    96c:  6000 06aa      	braw 0x1018                            ; ↓ label_89

  label_25:
    970:  1f3c 0001      	moveb #1,%sp@-                         
    974:  4eba 1a5e      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    978:  1800           	moveb %d0,%d4                          
    97a:  2f2b 000c      	movel %a3@(12),%sp@-                   
    97e:  3f2b 000a      	movew %a3@(10),%sp@-                   
    982:  2f2b 0006      	movel %a3@(6),%sp@-                    
    986:  1f2b 0004      	moveb %a3@(4),%sp@-                    
    98a:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    98e:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    992:  2f0a           	movel %a2,%sp@-                        
    994:  4eba 4a2e      	jsr %pc@(0x53c4)                       ; IOJobTPCWriteUniqTpc
    998:  3600           	movew %d0,%d3                          
    99a:  4a04           	tstb %d4                               
    99c:  4fef 0016      	lea %sp@(22),%sp                       
    9a0:  6600 0676      	bnew 0x1018                            ; ↓ label_89
    9a4:  4227           	clrb %sp@-                             
    9a6:  4eba 1a2c      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    9aa:  544f           	addqw #2,%sp                           
    9ac:  6000 066a      	braw 0x1018                            ; ↓ label_89
    9b0:  302a 0012      	movew %a2@(18),%d0                     
    9b4:  b053           	cmpw %a3@,%d0                          
    9b6:  6708           	beqs 0x9c0                             ; ↓ label_26
    9b8:  363c 8001      	movew #-32767,%d3                      
    9bc:  6000 065a      	braw 0x1018                            ; ↓ label_89

  label_26:
    9c0:  7001           	moveq #1,%d0                           
    9c2:  c0aa 0004      	andl %a2@(4),%d0                       
    9c6:  6708           	beqs 0x9d0                             ; ↓ label_27
    9c8:  7008           	moveq #8,%d0                           
    9ca:  c0aa 0004      	andl %a2@(4),%d0                       
    9ce:  6608           	bnes 0x9d8                             ; ↓ label_28

  label_27:
    9d0:  363c 8002      	movew #-32766,%d3                      
    9d4:  6000 0642      	braw 0x1018                            ; ↓ label_89

  label_28:
    9d8:  1f3c 0001      	moveb #1,%sp@-                         
    9dc:  4eba 19f6      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    9e0:  1800           	moveb %d0,%d4                          
    9e2:  2f2b 000c      	movel %a3@(12),%sp@-                   
    9e6:  3f2b 000a      	movew %a3@(10),%sp@-                   
    9ea:  2f2b 0006      	movel %a3@(6),%sp@-                    
    9ee:  1f2b 0004      	moveb %a3@(4),%sp@-                    
    9f2:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    9f6:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    9fa:  2f0a           	movel %a2,%sp@-                        
    9fc:  4eba 4a8e      	jsr %pc@(0x548c)                       ; IOJobTPCReadUniqTpc
    a00:  3600           	movew %d0,%d3                          
    a02:  4a04           	tstb %d4                               
    a04:  4fef 0016      	lea %sp@(22),%sp                       
    a08:  6600 060e      	bnew 0x1018                            ; ↓ label_89
    a0c:  4227           	clrb %sp@-                             
    a0e:  4eba 19c4      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    a12:  544f           	addqw #2,%sp                           
    a14:  6000 0602      	braw 0x1018                            ; ↓ label_89

  label_29:
    a18:  363c 2901      	movew #10497,%d3                       
    a1c:  6000 05fa      	braw 0x1018                            ; ↓ label_89

  label_30:
    a20:  3003           	movew %d3,%d0                          
    a22:  0440 2000      	subiw #8192,%d0                        
    a26:  6700 040a      	beqw 0xe32                             ; ↓ label_66
    a2a:  5340           	subqw #1,%d0                           
    a2c:  6700 040e      	beqw 0xe3c                             ; ↓ label_67
    a30:  5340           	subqw #1,%d0                           
    a32:  6700 0478      	beqw 0xeac                             ; ↓ label_71
    a36:  5340           	subqw #1,%d0                           
    a38:  6700 04c4      	beqw 0xefe                             ; ↓ label_76
    a3c:  5340           	subqw #1,%d0                           
    a3e:  6700 0598      	beqw 0xfd8                             ; ↓ label_84
    a42:  5340           	subqw #1,%d0                           
    a44:  6700 05a2      	beqw 0xfe8                             ; ↓ label_85
    a48:  5340           	subqw #1,%d0                           
    a4a:  6700 05ac      	beqw 0xff8                             ; ↓ label_86
    a4e:  0440 00fa      	subiw #250,%d0                         
    a52:  673e           	beqs 0xa92                             ; ↓ label_31
    a54:  5340           	subqw #1,%d0                           
    a56:  6700 0128      	beqw 0xb80                             ; ↓ label_40
    a5a:  5340           	subqw #1,%d0                           
    a5c:  6700 020e      	beqw 0xc6c                             ; ↓ label_48
    a60:  5340           	subqw #1,%d0                           
    a62:  6738           	beqs 0xa9c                             ; ↓ label_32
    a64:  5340           	subqw #1,%d0                           
    a66:  6700 00a6      	beqw 0xb0e                             ; ↓ label_36
    a6a:  5340           	subqw #1,%d0                           
    a6c:  6700 02a2      	beqw 0xd10                             ; ↓ label_54
    a70:  5340           	subqw #1,%d0                           
    a72:  6700 023c      	beqw 0xcb0                             ; ↓ label_50
    a76:  5340           	subqw #1,%d0                           
    a78:  6700 02da      	beqw 0xd54                             ; ↓ label_56
    a7c:  5340           	subqw #1,%d0                           
    a7e:  6700 031c      	beqw 0xd9c                             ; ↓ label_60
    a82:  5340           	subqw #1,%d0                           
    a84:  6700 0342      	beqw 0xdc8                             ; ↓ label_62
    a88:  5340           	subqw #1,%d0                           
    a8a:  6700 016a      	beqw 0xbf6                             ; ↓ label_44
    a8e:  6000 0584      	braw 0x1014                            ; ↓ label_88

  label_31:
    a92:  7001           	moveq #1,%d0                           
    a94:  2680           	movel %d0,%a3@                         
    a96:  7600           	moveq #0,%d3                           
    a98:  6000 057e      	braw 0x1018                            ; ↓ label_89

  label_32:
    a9c:  302a 0012      	movew %a2@(18),%d0                     
    aa0:  b053           	cmpw %a3@,%d0                          
    aa2:  6708           	beqs 0xaac                             ; ↓ label_33
    aa4:  363c 8001      	movew #-32767,%d3                      
    aa8:  6000 056e      	braw 0x1018                            ; ↓ label_89

  label_33:
    aac:  7001           	moveq #1,%d0                           
    aae:  c0aa 0004      	andl %a2@(4),%d0                       
    ab2:  6708           	beqs 0xabc                             ; ↓ label_34
    ab4:  7008           	moveq #8,%d0                           
    ab6:  c0aa 0004      	andl %a2@(4),%d0                       
    aba:  6608           	bnes 0xac4                             ; ↓ label_35

  label_34:
    abc:  363c 8002      	movew #-32766,%d3                      
    ac0:  6000 0556      	braw 0x1018                            ; ↓ label_89

  label_35:
    ac4:  1f3c 0001      	moveb #1,%sp@-                         
    ac8:  4eba 190a      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    acc:  1800           	moveb %d0,%d4                          
    ace:  42a7           	clrl %sp@-                             
    ad0:  2f2a 000e      	movel %a2@(14),%sp@-                   
    ad4:  4e4f           	trap #15                               
    ad6:  a09a           	0120232                                ; sysTrapSysTimerWrite
    ad8:  2f2b 0004      	movel %a3@(4),%sp@-                    
    adc:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    ae0:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    ae4:  2f0a           	movel %a2,%sp@-                        
    ae6:  4eba 392e      	jsr %pc@(0x4416)                       ; IOJobFunctionCheck
    aea:  3600           	movew %d0,%d3                          
    aec:  4878 0064      	pea 0x64                               
    af0:  2f2a 000e      	movel %a2@(14),%sp@-                   
    af4:  4e4f           	trap #15                               
    af6:  a09a           	0120232                                ; sysTrapSysTimerWrite
    af8:  4a04           	tstb %d4                               
    afa:  4fef 001e      	lea %sp@(30),%sp                       
    afe:  6600 0518      	bnew 0x1018                            ; ↓ label_89
    b02:  4227           	clrb %sp@-                             
    b04:  4eba 18ce      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    b08:  544f           	addqw #2,%sp                           
    b0a:  6000 050c      	braw 0x1018                            ; ↓ label_89

  label_36:
    b0e:  302a 0012      	movew %a2@(18),%d0                     
    b12:  b053           	cmpw %a3@,%d0                          
    b14:  6708           	beqs 0xb1e                             ; ↓ label_37
    b16:  363c 8001      	movew #-32767,%d3                      
    b1a:  6000 04fc      	braw 0x1018                            ; ↓ label_89

  label_37:
    b1e:  7001           	moveq #1,%d0                           
    b20:  c0aa 0004      	andl %a2@(4),%d0                       
    b24:  6708           	beqs 0xb2e                             ; ↓ label_38
    b26:  7008           	moveq #8,%d0                           
    b28:  c0aa 0004      	andl %a2@(4),%d0                       
    b2c:  6608           	bnes 0xb36                             ; ↓ label_39

  label_38:
    b2e:  363c 8002      	movew #-32766,%d3                      
    b32:  6000 04e4      	braw 0x1018                            ; ↓ label_89

  label_39:
    b36:  1f3c 0001      	moveb #1,%sp@-                         
    b3a:  4eba 1898      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    b3e:  1800           	moveb %d0,%d4                          
    b40:  42a7           	clrl %sp@-                             
    b42:  2f2a 000e      	movel %a2@(14),%sp@-                   
    b46:  4e4f           	trap #15                               
    b48:  a09a           	0120232                                ; sysTrapSysTimerWrite
    b4a:  2f2b 0004      	movel %a3@(4),%sp@-                    
    b4e:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    b52:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    b56:  2f0a           	movel %a2,%sp@-                        
    b58:  4eba 3994      	jsr %pc@(0x44ee)                       ; IOJobGetAttribute
    b5c:  3600           	movew %d0,%d3                          
    b5e:  4878 0064      	pea 0x64                               
    b62:  2f2a 000e      	movel %a2@(14),%sp@-                   
    b66:  4e4f           	trap #15                               
    b68:  a09a           	0120232                                ; sysTrapSysTimerWrite
    b6a:  4a04           	tstb %d4                               
    b6c:  4fef 001e      	lea %sp@(30),%sp                       
    b70:  6600 04a6      	bnew 0x1018                            ; ↓ label_89
    b74:  4227           	clrb %sp@-                             
    b76:  4eba 185c      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    b7a:  544f           	addqw #2,%sp                           
    b7c:  6000 049a      	braw 0x1018                            ; ↓ label_89

  label_40:
    b80:  302a 0012      	movew %a2@(18),%d0                     
    b84:  b053           	cmpw %a3@,%d0                          
    b86:  6708           	beqs 0xb90                             ; ↓ label_41
    b88:  363c 8001      	movew #-32767,%d3                      
    b8c:  6000 048a      	braw 0x1018                            ; ↓ label_89

  label_41:
    b90:  7001           	moveq #1,%d0                           
    b92:  c0aa 0004      	andl %a2@(4),%d0                       
    b96:  6708           	beqs 0xba0                             ; ↓ label_42
    b98:  7008           	moveq #8,%d0                           
    b9a:  c0aa 0004      	andl %a2@(4),%d0                       
    b9e:  6608           	bnes 0xba8                             ; ↓ label_43

  label_42:
    ba0:  363c 8002      	movew #-32766,%d3                      
    ba4:  6000 0472      	braw 0x1018                            ; ↓ label_89

  label_43:
    ba8:  1f3c 0001      	moveb #1,%sp@-                         
    bac:  4eba 1826      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    bb0:  1800           	moveb %d0,%d4                          
    bb2:  42a7           	clrl %sp@-                             
    bb4:  2f2a 000e      	movel %a2@(14),%sp@-                   
    bb8:  4e4f           	trap #15                               
    bba:  a09a           	0120232                                ; sysTrapSysTimerWrite
    bbc:  2f2b 0008      	movel %a3@(8),%sp@-                    
    bc0:  2f2b 0004      	movel %a3@(4),%sp@-                    
    bc4:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    bc8:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    bcc:  2f0a           	movel %a2,%sp@-                        
    bce:  4eba 3bfc      	jsr %pc@(0x47cc)                       ; IOJobFunctionEnable
    bd2:  3600           	movew %d0,%d3                          
    bd4:  4878 0064      	pea 0x64                               
    bd8:  2f2a 000e      	movel %a2@(14),%sp@-                   
    bdc:  4e4f           	trap #15                               
    bde:  a09a           	0120232                                ; sysTrapSysTimerWrite
    be0:  4a04           	tstb %d4                               
    be2:  4fef 0022      	lea %sp@(34),%sp                       
    be6:  6600 0430      	bnew 0x1018                            ; ↓ label_89
    bea:  4227           	clrb %sp@-                             
    bec:  4eba 17e6      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    bf0:  544f           	addqw #2,%sp                           
    bf2:  6000 0424      	braw 0x1018                            ; ↓ label_89

  label_44:
    bf6:  302a 0012      	movew %a2@(18),%d0                     
    bfa:  b053           	cmpw %a3@,%d0                          
    bfc:  6708           	beqs 0xc06                             ; ↓ label_45
    bfe:  363c 8001      	movew #-32767,%d3                      
    c02:  6000 0414      	braw 0x1018                            ; ↓ label_89

  label_45:
    c06:  7001           	moveq #1,%d0                           
    c08:  c0aa 0004      	andl %a2@(4),%d0                       
    c0c:  6708           	beqs 0xc16                             ; ↓ label_46
    c0e:  7008           	moveq #8,%d0                           
    c10:  c0aa 0004      	andl %a2@(4),%d0                       
    c14:  6608           	bnes 0xc1e                             ; ↓ label_47

  label_46:
    c16:  363c 8002      	movew #-32766,%d3                      
    c1a:  6000 03fc      	braw 0x1018                            ; ↓ label_89

  label_47:
    c1e:  1f3c 0001      	moveb #1,%sp@-                         
    c22:  4eba 17b0      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    c26:  1800           	moveb %d0,%d4                          
    c28:  42a7           	clrl %sp@-                             
    c2a:  2f2a 000e      	movel %a2@(14),%sp@-                   
    c2e:  4e4f           	trap #15                               
    c30:  a09a           	0120232                                ; sysTrapSysTimerWrite
    c32:  2f2b 0008      	movel %a3@(8),%sp@-                    
    c36:  2f2b 0004      	movel %a3@(4),%sp@-                    
    c3a:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    c3e:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    c42:  2f0a           	movel %a2,%sp@-                        
    c44:  4eba 3d14      	jsr %pc@(0x495a)                       ; !IOJobFunctionEnableNoCurrentC
    c48:  3600           	movew %d0,%d3                          
    c4a:  4878 0064      	pea 0x64                               
    c4e:  2f2a 000e      	movel %a2@(14),%sp@-                   
    c52:  4e4f           	trap #15                               
    c54:  a09a           	0120232                                ; sysTrapSysTimerWrite
    c56:  4a04           	tstb %d4                               
    c58:  4fef 0022      	lea %sp@(34),%sp                       
    c5c:  6600 03ba      	bnew 0x1018                            ; ↓ label_89
    c60:  4227           	clrb %sp@-                             
    c62:  4eba 1770      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    c66:  544f           	addqw #2,%sp                           
    c68:  6000 03ae      	braw 0x1018                            ; ↓ label_89

  label_48:
    c6c:  302a 0012      	movew %a2@(18),%d0                     
    c70:  b053           	cmpw %a3@,%d0                          
    c72:  6708           	beqs 0xc7c                             ; ↓ label_49
    c74:  363c 8001      	movew #-32767,%d3                      
    c78:  6000 039e      	braw 0x1018                            ; ↓ label_89

  label_49:
    c7c:  1f3c 0001      	moveb #1,%sp@-                         
    c80:  4eba 1752      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    c84:  1800           	moveb %d0,%d4                          
    c86:  2f2b 0004      	movel %a3@(4),%sp@-                    
    c8a:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    c8e:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    c92:  2f0a           	movel %a2,%sp@-                        
    c94:  4eba 3e32      	jsr %pc@(0x4ac8)                       ; IOJobFunctionDisable
    c98:  3600           	movew %d0,%d3                          
    c9a:  4a04           	tstb %d4                               
    c9c:  4fef 000e      	lea %sp@(14),%sp                       
    ca0:  6600 0376      	bnew 0x1018                            ; ↓ label_89
    ca4:  4227           	clrb %sp@-                             
    ca6:  4eba 172c      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    caa:  544f           	addqw #2,%sp                           
    cac:  6000 036a      	braw 0x1018                            ; ↓ label_89

  label_50:
    cb0:  302a 0012      	movew %a2@(18),%d0                     
    cb4:  b053           	cmpw %a3@,%d0                          
    cb6:  6708           	beqs 0xcc0                             ; ↓ label_51
    cb8:  363c 8001      	movew #-32767,%d3                      
    cbc:  6000 035a      	braw 0x1018                            ; ↓ label_89

  label_51:
    cc0:  7001           	moveq #1,%d0                           
    cc2:  c0aa 0004      	andl %a2@(4),%d0                       
    cc6:  6708           	beqs 0xcd0                             ; ↓ label_52
    cc8:  7008           	moveq #8,%d0                           
    cca:  c0aa 0004      	andl %a2@(4),%d0                       
    cce:  6608           	bnes 0xcd8                             ; ↓ label_53

  label_52:
    cd0:  363c 8002      	movew #-32766,%d3                      
    cd4:  6000 0342      	braw 0x1018                            ; ↓ label_89

  label_53:
    cd8:  1f3c 0001      	moveb #1,%sp@-                         
    cdc:  4eba 16f6      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    ce0:  1800           	moveb %d0,%d4                          
    ce2:  2f2b 0006      	movel %a3@(6),%sp@-                    
    ce6:  1f2b 0004      	moveb %a3@(4),%sp@-                    
    cea:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    cee:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    cf2:  2f0a           	movel %a2,%sp@-                        
    cf4:  4eba 3fca      	jsr %pc@(0x4cc0)                       ; IOJobPowerSave
    cf8:  3600           	movew %d0,%d3                          
    cfa:  4a04           	tstb %d4                               
    cfc:  4fef 0010      	lea %sp@(16),%sp                       
    d00:  6600 0316      	bnew 0x1018                            ; ↓ label_89
    d04:  4227           	clrb %sp@-                             
    d06:  4eba 16cc      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    d0a:  544f           	addqw #2,%sp                           
    d0c:  6000 030a      	braw 0x1018                            ; ↓ label_89

  label_54:
    d10:  302a 0012      	movew %a2@(18),%d0                     
    d14:  b053           	cmpw %a3@,%d0                          
    d16:  6708           	beqs 0xd20                             ; ↓ label_55
    d18:  363c 8001      	movew #-32767,%d3                      
    d1c:  6000 02fa      	braw 0x1018                            ; ↓ label_89

  label_55:
    d20:  1f3c 0001      	moveb #1,%sp@-                         
    d24:  4eba 16ae      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    d28:  1800           	moveb %d0,%d4                          
    d2a:  1f2b 0004      	moveb %a3@(4),%sp@-                    
    d2e:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    d32:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    d36:  2f0a           	movel %a2,%sp@-                        
    d38:  4eba 3e90      	jsr %pc@(0x4bca)                       ; IOJobIntControl
    d3c:  3600           	movew %d0,%d3                          
    d3e:  4a04           	tstb %d4                               
    d40:  4fef 000c      	lea %sp@(12),%sp                       
    d44:  6600 02d2      	bnew 0x1018                            ; ↓ label_89
    d48:  4227           	clrb %sp@-                             
    d4a:  4eba 1688      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    d4e:  544f           	addqw #2,%sp                           
    d50:  6000 02c6      	braw 0x1018                            ; ↓ label_89

  label_56:
    d54:  302a 0012      	movew %a2@(18),%d0                     
    d58:  b053           	cmpw %a3@,%d0                          
    d5a:  6708           	beqs 0xd64                             ; ↓ label_57
    d5c:  363c 8001      	movew #-32767,%d3                      
    d60:  6000 02b6      	braw 0x1018                            ; ↓ label_89

  label_57:
    d64:  7001           	moveq #1,%d0                           
    d66:  c0aa 0004      	andl %a2@(4),%d0                       
    d6a:  6708           	beqs 0xd74                             ; ↓ label_58
    d6c:  7008           	moveq #8,%d0                           
    d6e:  c0aa 0004      	andl %a2@(4),%d0                       
    d72:  6608           	bnes 0xd7c                             ; ↓ label_59

  label_58:
    d74:  363c 8002      	movew #-32766,%d3                      
    d78:  6000 029e      	braw 0x1018                            ; ↓ label_89

  label_59:
    d7c:  2f2b 0008      	movel %a3@(8),%sp@-                    
    d80:  2f2b 0004      	movel %a3@(4),%sp@-                    
    d84:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    d88:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    d8c:  2f0a           	movel %a2,%sp@-                        
    d8e:  4eba 3ede      	jsr %pc@(0x4c6e)                       ; IOJobSetDefaultToe
    d92:  3600           	movew %d0,%d3                          
    d94:  4fef 0010      	lea %sp@(16),%sp                       
    d98:  6000 027e      	braw 0x1018                            ; ↓ label_89

  label_60:
    d9c:  302a 0012      	movew %a2@(18),%d0                     
    da0:  b053           	cmpw %a3@,%d0                          
    da2:  6708           	beqs 0xdac                             ; ↓ label_61
    da4:  363c 8001      	movew #-32767,%d3                      
    da8:  6000 026e      	braw 0x1018                            ; ↓ label_89

  label_61:
    dac:  1f2b 0003      	moveb %a3@(3),%sp@-                    
    db0:  1f2b 0002      	moveb %a3@(2),%sp@-                    
    db4:  2f0a           	movel %a2,%sp@-                        
    db6:  4eba 39ca      	jsr %pc@(0x4782)                       ; IOJobCheckFunctionPower
    dba:  206b 0004      	moveal %a3@(4),%a0                     
    dbe:  1080           	moveb %d0,%a0@                         
    dc0:  7600           	moveq #0,%d3                           
    dc2:  504f           	addqw #8,%sp                           
    dc4:  6000 0252      	braw 0x1018                            ; ↓ label_89

  label_62:
    dc8:  302a 0012      	movew %a2@(18),%d0                     
    dcc:  b053           	cmpw %a3@,%d0                          
    dce:  6708           	beqs 0xdd8                             ; ↓ label_63
    dd0:  363c 8001      	movew #-32767,%d3                      
    dd4:  6000 0242      	braw 0x1018                            ; ↓ label_89

  label_63:
    dd8:  7001           	moveq #1,%d0                           
    dda:  c0aa 0004      	andl %a2@(4),%d0                       
    dde:  6708           	beqs 0xde8                             ; ↓ label_64
    de0:  7008           	moveq #8,%d0                           
    de2:  c0aa 0004      	andl %a2@(4),%d0                       
    de6:  6608           	bnes 0xdf0                             ; ↓ label_65

  label_64:
    de8:  363c 8002      	movew #-32766,%d3                      
    dec:  6000 022a      	braw 0x1018                            ; ↓ label_89

  label_65:
    df0:  1f3c 0001      	moveb #1,%sp@-                         
    df4:  4eba 15de      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    df8:  1800           	moveb %d0,%d4                          
    dfa:  42a7           	clrl %sp@-                             
    dfc:  2f2a 000e      	movel %a2@(14),%sp@-                   
    e00:  4e4f           	trap #15                               
    e02:  a09a           	0120232                                ; sysTrapSysTimerWrite
    e04:  2f2b 0002      	movel %a3@(2),%sp@-                    
    e08:  2f0a           	movel %a2,%sp@-                        
    e0a:  4eba 355c      	jsr %pc@(0x4368)                       ; IOJobCheckStickType
    e0e:  3600           	movew %d0,%d3                          
    e10:  4878 0064      	pea 0x64                               
    e14:  2f2a 000e      	movel %a2@(14),%sp@-                   
    e18:  4e4f           	trap #15                               
    e1a:  a09a           	0120232                                ; sysTrapSysTimerWrite
    e1c:  4a04           	tstb %d4                               
    e1e:  4fef 001a      	lea %sp@(26),%sp                       
    e22:  6600 01f4      	bnew 0x1018                            ; ↓ label_89
    e26:  4227           	clrb %sp@-                             
    e28:  4eba 15aa      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    e2c:  544f           	addqw #2,%sp                           
    e2e:  6000 01e8      	braw 0x1018                            ; ↓ label_89

  label_66:
    e32:  7001           	moveq #1,%d0                           
    e34:  2680           	movel %d0,%a3@                         
    e36:  7600           	moveq #0,%d3                           
    e38:  6000 01de      	braw 0x1018                            ; ↓ label_89

  label_67:
    e3c:  302a 0012      	movew %a2@(18),%d0                     
    e40:  b053           	cmpw %a3@,%d0                          
    e42:  6708           	beqs 0xe4c                             ; ↓ label_68
    e44:  363c 2904      	movew #10500,%d3                       
    e48:  6000 01ce      	braw 0x1018                            ; ↓ label_89

  label_68:
    e4c:  7001           	moveq #1,%d0                           
    e4e:  c0aa 0004      	andl %a2@(4),%d0                       
    e52:  6608           	bnes 0xe5c                             ; ↓ label_69
    e54:  363c 2903      	movew #10499,%d3                       
    e58:  6000 01be      	braw 0x1018                            ; ↓ label_89

  label_69:
    e5c:  7008           	moveq #8,%d0                           
    e5e:  c0aa 0004      	andl %a2@(4),%d0                       
    e62:  6608           	bnes 0xe6c                             ; ↓ label_70
    e64:  363c 2903      	movew #10499,%d3                       
    e68:  6000 01ae      	braw 0x1018                            ; ↓ label_89

  label_70:
    e6c:  202a 0004      	movel %a2@(4),%d0                      
    e70:  0280 0303 0000 	andil #50528256,%d0                    
    e76:  7277           	moveq #119,%d1                         
    e78:  c2aa 0008      	andl %a2@(8),%d1                       
    e7c:  8280           	orl %d0,%d1                            
    e7e:  2741 0002      	movel %d1,%a3@(2)                      
    e82:  206a 0020      	moveal %a2@(32),%a0                    
    e86:  2768 25b4 0006 	movel %a0@(9652),%a3@(6)               
    e8c:  206a 0020      	moveal %a2@(32),%a0                    
    e90:  2768 25ac 000a 	movel %a0@(9644),%a3@(10)              
    e96:  206a 0020      	moveal %a2@(32),%a0                    
    e9a:  203c 0000 25ac 	movel #9644,%d0                        
    ea0:  2770 0804 000e 	movel %a0@(00000004,%d0:l),%a3@(14)    
    ea6:  7600           	moveq #0,%d3                           
    ea8:  6000 016e      	braw 0x1018                            ; ↓ label_89

  label_71:
    eac:  302a 0012      	movew %a2@(18),%d0                     
    eb0:  b053           	cmpw %a3@,%d0                          
    eb2:  6708           	beqs 0xebc                             ; ↓ label_72
    eb4:  363c 2904      	movew #10500,%d3                       
    eb8:  6000 015e      	braw 0x1018                            ; ↓ label_89

  label_72:
    ebc:  7001           	moveq #1,%d0                           
    ebe:  c0aa 0004      	andl %a2@(4),%d0                       
    ec2:  6608           	bnes 0xecc                             ; ↓ label_73
    ec4:  363c 2903      	movew #10499,%d3                       
    ec8:  6000 014e      	braw 0x1018                            ; ↓ label_89

  label_73:
    ecc:  7008           	moveq #8,%d0                           
    ece:  c0aa 0004      	andl %a2@(4),%d0                       
    ed2:  6608           	bnes 0xedc                             ; ↓ label_74
    ed4:  363c 2903      	movew #10499,%d3                       
    ed8:  6000 013e      	braw 0x1018                            ; ↓ label_89

  label_74:
    edc:  7040           	moveq #64,%d0                          
    ede:  c0aa 0008      	andl %a2@(8),%d0                       
    ee2:  6608           	bnes 0xeec                             ; ↓ label_75
    ee4:  363c 2906      	movew #10502,%d3                       
    ee8:  6000 012e      	braw 0x1018                            ; ↓ label_89

  label_75:
    eec:  206a 0020      	moveal %a2@(32),%a0                    
    ef0:  41e8 2400      	lea %a0@(9216),%a0                     
    ef4:  2748 0002      	movel %a0,%a3@(2)                      
    ef8:  7600           	moveq #0,%d3                           
    efa:  6000 011c      	braw 0x1018                            ; ↓ label_89

  label_76:
    efe:  302a 0012      	movew %a2@(18),%d0                     
    f02:  b053           	cmpw %a3@,%d0                          
    f04:  6708           	beqs 0xf0e                             ; ↓ label_77
    f06:  363c 2904      	movew #10500,%d3                       
    f0a:  6000 010c      	braw 0x1018                            ; ↓ label_89

  label_77:
    f0e:  7001           	moveq #1,%d0                           
    f10:  c0aa 0004      	andl %a2@(4),%d0                       
    f14:  6608           	bnes 0xf1e                             ; ↓ label_78
    f16:  363c 2903      	movew #10499,%d3                       
    f1a:  6000 00fc      	braw 0x1018                            ; ↓ label_89

  label_78:
    f1e:  7008           	moveq #8,%d0                           
    f20:  c0aa 0004      	andl %a2@(4),%d0                       
    f24:  6608           	bnes 0xf2e                             ; ↓ label_79
    f26:  363c 2903      	movew #10499,%d3                       
    f2a:  6000 00ec      	braw 0x1018                            ; ↓ label_89

  label_79:
    f2e:  262b 000a      	movel %a3@(10),%d3                     
    f32:  2043           	moveal %d3,%a0                         
    f34:  2d50 fffc      	movel %a0@,%fp@(-4)                    
    f38:  4290           	clrl %a0@                              
    f3a:  1f3c 0001      	moveb #1,%sp@-                         
    f3e:  4eba 1494      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    f42:  1a00           	moveb %d0,%d5                          
    f44:  202a 0004      	movel %a2@(4),%d0                      
    f48:  0280 0000 0080 	andil #128,%d0                         
    f4e:  544f           	addqw #2,%sp                           
    f50:  660e           	bnes 0xf60                             ; ↓ label_80
    f52:  2f0a           	movel %a2,%sp@-                        
    f54:  4eba 1052      	jsr %pc@(0x1fa8)                       ; PrvSupplyPower
    f58:  2f0a           	movel %a2,%sp@-                        
    f5a:  4eba 0f8c      	jsr %pc@(0x1ee8)                       ; PrvDoMgAuth
    f5e:  504f           	addqw #8,%sp                           

  label_80:
    f60:  2f0a           	movel %a2,%sp@-                        
    f62:  4eba 462c      	jsr %pc@(0x5590)                       ; ReturnToMS
    f66:  3800           	movew %d0,%d4                          
    f68:  584f           	addqw #4,%sp                           
    f6a:  6708           	beqs 0xf74                             ; ↓ label_81
    f6c:  363c 2906      	movew #10502,%d3                       
    f70:  6000 00a6      	braw 0x1018                            ; ↓ label_89

  label_81:
    f74:  486e fffc      	pea %fp@(-4)                           
    f78:  2f2b 0006      	movel %a3@(6),%sp@-                    
    f7c:  2f2b 0002      	movel %a3@(2),%sp@-                    
    f80:  2f0a           	movel %a2,%sp@-                        
    f82:  4eba 1646      	jsr %pc@(0x25ca)                       ; PrvJobReadSector
    f86:  2800           	movel %d0,%d4                          
    f88:  2f0a           	movel %a2,%sp@-                        
    f8a:  4eba 4668      	jsr %pc@(0x55f4)                       ; ReturnToIO
    f8e:  4fef 0014      	lea %sp@(20),%sp                       
    f92:  2004           	movel %d4,%d0                          
    f94:  0c80 0000 0004 	cmpil #4,%d0                           
    f9a:  622a           	bhis 0xfc6                             ; ↓ label_82
    f9c:  d040           	addw %d0,%d0                           
    f9e:  303b 0006      	movew %pc@(0xfa6,%d0:w),%d0            
    fa2:  4efb 0002      	jmp %pc@(0xfa6,%d0:w)                  
    fa6:  000a 0020      	orib #32,%a2                           
    faa:  0014 0020      	orib #32,%a4@                          
    fae:  001a 2043      	orib #67,%a2@+                         
    fb2:  20ae fffc      	movel %fp@(-4),%a0@                    
    fb6:  7600           	moveq #0,%d3                           
    fb8:  6010           	bras 0xfca                             ; ↓ label_83
    fba:  363c 2903      	movew #10499,%d3                       
    fbe:  600a           	bras 0xfca                             ; ↓ label_83
    fc0:  363c 0502      	movew #1282,%d3                        
    fc4:  6004           	bras 0xfca                             ; ↓ label_83

  label_82:
    fc6:  363c 2908      	movew #10504,%d3                       

  label_83:
    fca:  4a05           	tstb %d5                               
    fcc:  664a           	bnes 0x1018                            ; ↓ label_89
    fce:  4227           	clrb %sp@-                             
    fd0:  4eba 1402      	jsr %pc@(0x23d4)                       ; MSSlotHwrLightAccessLED
    fd4:  544f           	addqw #2,%sp                           
    fd6:  6040           	bras 0x1018                            ; ↓ label_89

  label_84:
    fd8:  3f2b 0002      	movew %a3@(2),%sp@-                    
    fdc:  3f04           	movew %d4,%sp@-                        
    fde:  4eba 026e      	jsr %pc@(0x124e)                       ; MSSlotReserveCard
    fe2:  3600           	movew %d0,%d3                          
    fe4:  584f           	addqw #4,%sp                           
    fe6:  6030           	bras 0x1018                            ; ↓ label_89

  label_85:
    fe8:  3f2b 0002      	movew %a3@(2),%sp@-                    
    fec:  3f04           	movew %d4,%sp@-                        
    fee:  4eba 02b8      	jsr %pc@(0x12a8)                       ; MSSlotReleaseCard
    ff2:  3600           	movew %d0,%d3                          
    ff4:  584f           	addqw #4,%sp                           
    ff6:  6020           	bras 0x1018                            ; ↓ label_89

  label_86:
    ff8:  7000           	moveq #0,%d0                           
    ffa:  302a 0012      	movew %a2@(18),%d0                     
    ffe:  b093           	cmpl %a3@,%d0                          
    1000:  6706           	beqs 0x1008                           ; ↓ label_87
    1002:  303c 2904      	movew #10500,%d0                      
    1006:  6012           	bras 0x101a                           ; ↓ label_90

  label_87:
    1008:  4227           	clrb %sp@-                            
    100a:  4eba 0c58      	jsr %pc@(0x1c64)                      ; PrvInsRmvIntHandler
    100e:  7600           	moveq #0,%d3                          
    1010:  544f           	addqw #2,%sp                          
    1012:  6004           	bras 0x1018                           ; ↓ label_89

  label_88:
    1014:  363c 2901      	movew #10497,%d3                      

  label_89:
    1018:  3003           	movew %d3,%d0                         

  label_90:
    101a:  4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3          
    101e:  4e5e           	unlk %fp                              
    1020:  4e75           	rts                                   
```

# MSSlotCheckCardPresence: 0x103c - 0x1062

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    103c:  4e56 fffc      	linkw %fp,#-4                         
    1040:  2f03           	movel %d3,%sp@-                       
    1042:  486e fffc      	pea %fp@(-4)                          
    1046:  3f2e 000a      	movew %fp@(10),%sp@-                  
    104a:  3f2e 0008      	movew %fp@(8),%sp@-                   
    104e:  4eba 0d74      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    1052:  3600           	movew %d0,%d3                         
    1054:  504f           	addqw #8,%sp                          
    1056:  6704           	beqs 0x105c                           ; ↓ label_0
    1058:  3003           	movew %d3,%d0                         
    105a:  6002           	bras 0x105e                           ; ↓ label_1

  label_0:
    105c:  7000           	moveq #0,%d0                          

  label_1:
    105e:  261f           	movel %sp@+,%d3                       
    1060:  4e5e           	unlk %fp                              
    1062:  4e75           	rts                                   
```

# MSSlotGetCardInfo: 0x107e - 0x1168

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    107e:  4e56 fffc      	linkw %fp,#-4                         
    1082:  2f0a           	movel %a2,%sp@-                       
    1084:  2f03           	movel %d3,%sp@-                       
    1086:  246e 000c      	moveal %fp@(12),%a2                   
    108a:  486e fffc      	pea %fp@(-4)                          
    108e:  3f2e 000a      	movew %fp@(10),%sp@-                  
    1092:  3f2e 0008      	movew %fp@(8),%sp@-                   
    1096:  4eba 0d2c      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    109a:  3600           	movew %d0,%d3                         
    109c:  504f           	addqw #8,%sp                          
    109e:  6706           	beqs 0x10a6                           ; ↓ label_0
    10a0:  3003           	movew %d3,%d0                         
    10a2:  6000 00be      	braw 0x1162                           ; ↓ label_6

  label_0:
    10a6:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    10aa:  4eba 44e4      	jsr %pc@(0x5590)                      ; ReturnToMS
    10ae:  3600           	movew %d0,%d3                         
    10b0:  584f           	addqw #4,%sp                          
    10b2:  6708           	beqs 0x10bc                           ; ↓ label_1
    10b4:  303c 2906      	movew #10502,%d0                      
    10b8:  6000 00a8      	braw 0x1162                           ; ↓ label_6

  label_1:
    10bc:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    10c0:  4eba 0d70      	jsr %pc@(0x1e32)                      ; PrvAssureMounted
    10c4:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    10c8:  4eba 452a      	jsr %pc@(0x55f4)                      ; ReturnToIO
    10cc:  4292           	clrl %a2@                             
    10ce:  7010           	moveq #16,%d0                         
    10d0:  206e fffc      	moveal %fp@(-4),%a0                   
    10d4:  c0a8 0004      	andl %a0@(4),%d0                      
    10d8:  504f           	addqw #8,%sp                          
    10da:  6734           	beqs 0x1110                           ; ↓ label_4
    10dc:  7001           	moveq #1,%d0                          
    10de:  c0a8 0008      	andl %a0@(8),%d0                      
    10e2:  6704           	beqs 0x10e8                           ; ↓ label_2
    10e4:  7001           	moveq #1,%d0                          
    10e6:  8192           	orl %d0,%a2@                          

  label_2:
    10e8:  7030           	moveq #48,%d0                         
    10ea:  206e fffc      	moveal %fp@(-4),%a0                   
    10ee:  c0a8 0008      	andl %a0@(8),%d0                      
    10f2:  6704           	beqs 0x10f8                           ; ↓ label_3
    10f4:  7002           	moveq #2,%d0                          
    10f6:  8192           	orl %d0,%a2@                          

  label_3:
    10f8:  3f3c 001f      	movew #31,%sp@-                       
    10fc:  41fa 0080      	lea %pc@(0x117e),%a0                  ; ??? 0x117e
    1100:  4850           	pea %a0@                              
    1102:  486a 0044      	pea %a2@(68)                          
    1106:  4e4f           	trap #15                              
    1108:  a2ce           	0121316                               ; sysTrapStrNCopy
    110a:  4fef 000a      	lea %sp@(10),%sp                      
    110e:  6016           	bras 0x1126                           ; ↓ label_5

  label_4:
    1110:  3f3c 001f      	movew #31,%sp@-                       
    1114:  41fa 0076      	lea %pc@(0x118c),%a0                  ; ??? 0x118c
    1118:  4850           	pea %a0@                              
    111a:  486a 0044      	pea %a2@(68)                          
    111e:  4e4f           	trap #15                              
    1120:  a2ce           	0121316                               ; sysTrapStrNCopy
    1122:  4fef 000a      	lea %sp@(10),%sp                      

  label_5:
    1126:  3f3c 001f      	movew #31,%sp@-                       
    112a:  41fa 0062      	lea %pc@(0x118e),%a0                  ; ??? 0x118e
    112e:  4850           	pea %a0@                              
    1130:  486a 0004      	pea %a2@(4)                           
    1134:  4e4f           	trap #15                              
    1136:  a2ce           	0121316                               ; sysTrapStrNCopy
    1138:  3f3c 001f      	movew #31,%sp@-                       
    113c:  41fa 0052      	lea %pc@(0x1190),%a0                  ; ??? 0x1190
    1140:  4850           	pea %a0@                              
    1142:  486a 0024      	pea %a2@(36)                          
    1146:  4e4f           	trap #15                              
    1148:  a2ce           	0121316                               ; sysTrapStrNCopy
    114a:  3f3c 001f      	movew #31,%sp@-                       
    114e:  41fa 0042      	lea %pc@(0x1192),%a0                  ; ??? 0x1192
    1152:  4850           	pea %a0@                              
    1154:  486a 0064      	pea %a2@(100)                         
    1158:  4e4f           	trap #15                              
    115a:  a2ce           	0121316                               ; sysTrapStrNCopy
    115c:  7000           	moveq #0,%d0                          
    115e:  4fef 001e      	lea %sp@(30),%sp                      

  label_6:
    1162:  261f           	movel %sp@+,%d3                       
    1164:  245f           	moveal %sp@+,%a2                      
    1166:  4e5e           	unlk %fp                              
    1168:  4e75           	rts                                   
```

# MSSlotGetCardMediaType: 0x1194 - 0x11d0

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    1194:  4e56 fffc      	linkw %fp,#-4                         
    1198:  2f03           	movel %d3,%sp@-                       
    119a:  486e fffc      	pea %fp@(-4)                          
    119e:  3f2e 000a      	movew %fp@(10),%sp@-                  
    11a2:  3f2e 0008      	movew %fp@(8),%sp@-                   
    11a6:  4eba 0c1c      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    11aa:  3600           	movew %d0,%d3                         
    11ac:  504f           	addqw #8,%sp                          
    11ae:  6704           	beqs 0x11b4                           ; ↓ label_0
    11b0:  3003           	movew %d3,%d0                         
    11b2:  6018           	bras 0x11cc                           ; ↓ label_2

  label_0:
    11b4:  4aae 000c      	tstl %fp@(12)                         
    11b8:  670e           	beqs 0x11c8                           ; ↓ label_1
    11ba:  206e 000c      	moveal %fp@(12),%a0                   
    11be:  20bc 6d73 746b 	movel #1836282987,%a0@                
    11c4:  7000           	moveq #0,%d0                          
    11c6:  6004           	bras 0x11cc                           ; ↓ label_2

  label_1:
    11c8:  303c 0502      	movew #1282,%d0                       

  label_2:
    11cc:  261f           	movel %sp@+,%d3                       
    11ce:  4e5e           	unlk %fp                              
    11d0:  4e75           	rts                                   
```

# MSSlotGetMediaType: 0x11ec - 0x1236

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

```
    11ec:  4e56 0000      	linkw %fp,#0                          
    11f0:  2f0a           	movel %a2,%sp@-                       
    11f2:  3f2e 0008      	movew %fp@(8),%sp@-                   
    11f6:  4e4f           	trap #15                              
    11f8:  a0b9           	0120271                               ; sysTrapSysLibTblEntry
    11fa:  2468 0004      	moveal %a0@(4),%a2                    
    11fe:  200a           	movel %a2,%d0                         
    1200:  544f           	addqw #2,%sp                          
    1202:  6606           	bnes 0x120a                           ; ↓ label_0
    1204:  303c 290a      	movew #10506,%d0                      
    1208:  6028           	bras 0x1232                           ; ↓ label_3

  label_0:
    120a:  302a 0012      	movew %a2@(18),%d0                    
    120e:  b06e 000a      	cmpw %fp@(10),%d0                     
    1212:  6706           	beqs 0x121a                           ; ↓ label_1
    1214:  303c 2904      	movew #10500,%d0                      
    1218:  6018           	bras 0x1232                           ; ↓ label_3

  label_1:
    121a:  4aae 000c      	tstl %fp@(12)                         
    121e:  670e           	beqs 0x122e                           ; ↓ label_2
    1220:  206e 000c      	moveal %fp@(12),%a0                   
    1224:  20bc 6d73 746b 	movel #1836282987,%a0@                
    122a:  7000           	moveq #0,%d0                          
    122c:  6004           	bras 0x1232                           ; ↓ label_3

  label_2:
    122e:  303c 0502      	movew #1282,%d0                       

  label_3:
    1232:  245f           	moveal %sp@+,%a2                      
    1234:  4e5e           	unlk %fp                              
    1236:  4e75           	rts                                   
```

# MSSlotReserveCard: 0x124e - 0x1292

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    124e:  4e56 fffc      	linkw %fp,#-4                         
    1252:  2f03           	movel %d3,%sp@-                       
    1254:  4227           	clrb %sp@-                            
    1256:  4e4f           	trap #15                              
    1258:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    125a:  486e fffc      	pea %fp@(-4)                          
    125e:  3f2e 000a      	movew %fp@(10),%sp@-                  
    1262:  3f2e 0008      	movew %fp@(8),%sp@-                   
    1266:  4eba 0b5c      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    126a:  3600           	movew %d0,%d3                         
    126c:  4fef 000a      	lea %sp@(10),%sp                      
    1270:  6706           	beqs 0x1278                           ; ↓ label_0
    1272:  0c43 2903      	cmpiw #10499,%d3                      
    1276:  660a           	bnes 0x1282                           ; ↓ label_1

  label_0:
    1278:  206e fffc      	moveal %fp@(-4),%a0                   
    127c:  7040           	moveq #64,%d0                         
    127e:  81a8 0004      	orl %d0,%a0@(4)                       

  label_1:
    1282:  1f3c 0001      	moveb #1,%sp@-                        
    1286:  4e4f           	trap #15                              
    1288:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    128a:  3003           	movew %d3,%d0                         
    128c:  544f           	addqw #2,%sp                          
    128e:  261f           	movel %sp@+,%d3                       
    1290:  4e5e           	unlk %fp                              
    1292:  4e75           	rts                                   
```

# MSSlotReleaseCard: 0x12a8 - 0x130e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    12a8:  4e56 fffc      	linkw %fp,#-4                         
    12ac:  2f03           	movel %d3,%sp@-                       
    12ae:  4227           	clrb %sp@-                            
    12b0:  4e4f           	trap #15                              
    12b2:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    12b4:  486e fffc      	pea %fp@(-4)                          
    12b8:  3f2e 000a      	movew %fp@(10),%sp@-                  
    12bc:  3f2e 0008      	movew %fp@(8),%sp@-                   
    12c0:  4eba 0b02      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    12c4:  3600           	movew %d0,%d3                         
    12c6:  4fef 000a      	lea %sp@(10),%sp                      
    12ca:  6706           	beqs 0x12d2                           ; ↓ label_0
    12cc:  0c43 2903      	cmpiw #10499,%d3                      
    12d0:  662c           	bnes 0x12fe                           ; ↓ label_1

  label_0:
    12d2:  206e fffc      	moveal %fp@(-4),%a0                   
    12d6:  70bf           	moveq #-65,%d0                        
    12d8:  c1a8 0004      	andl %d0,%a0@(4)                      
    12dc:  42a7           	clrl %sp@-                            
    12de:  206e fffc      	moveal %fp@(-4),%a0                   
    12e2:  2f28 000e      	movel %a0@(14),%sp@-                  
    12e6:  4e4f           	trap #15                              
    12e8:  a09a           	0120232                               ; sysTrapSysTimerWrite
    12ea:  4878 0064      	pea 0x64                              
    12ee:  206e fffc      	moveal %fp@(-4),%a0                   
    12f2:  2f28 000e      	movel %a0@(14),%sp@-                  
    12f6:  4e4f           	trap #15                              
    12f8:  a09a           	0120232                               ; sysTrapSysTimerWrite
    12fa:  4fef 0010      	lea %sp@(16),%sp                      

  label_1:
    12fe:  1f3c 0001      	moveb #1,%sp@-                        
    1302:  4e4f           	trap #15                              
    1304:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    1306:  3003           	movew %d3,%d0                         
    1308:  544f           	addqw #2,%sp                          
    130a:  261f           	movel %sp@+,%d3                       
    130c:  4e5e           	unlk %fp                              
    130e:  4e75           	rts                                   
```

# MSSlotGetSerialPort: 0x1324 - 0x135a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    1324:  4e56 0000      	linkw %fp,#0                          
    1328:  2f0a           	movel %a2,%sp@-                       
    132a:  3f2e 0008      	movew %fp@(8),%sp@-                   
    132e:  4e4f           	trap #15                              
    1330:  a0b9           	0120271                               ; sysTrapSysLibTblEntry
    1332:  2468 0004      	moveal %a0@(4),%a2                    
    1336:  200a           	movel %a2,%d0                         
    1338:  544f           	addqw #2,%sp                          
    133a:  6606           	bnes 0x1342                           ; ↓ label_0
    133c:  303c 290a      	movew #10506,%d0                      
    1340:  6014           	bras 0x1356                           ; ↓ label_2

  label_0:
    1342:  302a 0012      	movew %a2@(18),%d0                    
    1346:  b06e 000a      	cmpw %fp@(10),%d0                     
    134a:  6706           	beqs 0x1352                           ; ↓ label_1
    134c:  303c 2904      	movew #10500,%d0                      
    1350:  6004           	bras 0x1356                           ; ↓ label_2

  label_1:
    1352:  303c 2901      	movew #10497,%d0                      

  label_2:
    1356:  245f           	moveal %sp@+,%a2                      
    1358:  4e5e           	unlk %fp                              
    135a:  4e75           	rts                                   
```

# MSSlotCheckFSSupport: 0x1372 - 0x13bc

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

```
    1372:  4e56 0000      	linkw %fp,#0                          
    1376:  2f0a           	movel %a2,%sp@-                       
    1378:  3f2e 0008      	movew %fp@(8),%sp@-                   
    137c:  4e4f           	trap #15                              
    137e:  a0b9           	0120271                               ; sysTrapSysLibTblEntry
    1380:  2468 0004      	moveal %a0@(4),%a2                    
    1384:  200a           	movel %a2,%d0                         
    1386:  544f           	addqw #2,%sp                          
    1388:  6604           	bnes 0x138e                           ; ↓ label_0
    138a:  7000           	moveq #0,%d0                          
    138c:  602a           	bras 0x13b8                           ; ↓ label_4

  label_0:
    138e:  302a 0012      	movew %a2@(18),%d0                    
    1392:  b06e 000a      	cmpw %fp@(10),%d0                     
    1396:  6704           	beqs 0x139c                           ; ↓ label_1
    1398:  7000           	moveq #0,%d0                          
    139a:  601c           	bras 0x13b8                           ; ↓ label_4

  label_1:
    139c:  202e 000c      	movel %fp@(12),%d0                    
    13a0:  0480 6661 7473 	subil #1717662835,%d0                 
    13a6:  670a           	beqs 0x13b2                           ; ↓ label_2
    13a8:  0480 1004 ed01 	subil #268758273,%d0                  
    13ae:  6702           	beqs 0x13b2                           ; ↓ label_2
    13b0:  6004           	bras 0x13b6                           ; ↓ label_3

  label_2:
    13b2:  7001           	moveq #1,%d0                          
    13b4:  6002           	bras 0x13b8                           ; ↓ label_4

  label_3:
    13b6:  7000           	moveq #0,%d0                          

  label_4:
    13b8:  245f           	moveal %sp@+,%a2                      
    13ba:  4e5e           	unlk %fp                              
    13bc:  4e75           	rts                                   
```

# MSSlotGetCardMetrics: 0x13d6 - 0x1580

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    13d6:  4e56 fffc      	linkw %fp,#-4                         
    13da:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    13de:  246e 000c      	moveal %fp@(12),%a2                   
    13e2:  486e fffc      	pea %fp@(-4)                          
    13e6:  3f2e 000a      	movew %fp@(10),%sp@-                  
    13ea:  3f2e 0008      	movew %fp@(8),%sp@-                   
    13ee:  4eba 09d4      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    13f2:  3600           	movew %d0,%d3                         
    13f4:  504f           	addqw #8,%sp                          
    13f6:  6706           	beqs 0x13fe                           ; ↓ label_0
    13f8:  3003           	movew %d3,%d0                         
    13fa:  6000 017e      	braw 0x157a                           ; ↓ label_14

  label_0:
    13fe:  200a           	movel %a2,%d0                         
    1400:  6700 0172      	beqw 0x1574                           ; ↓ label_12
    1404:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1408:  4eba 4186      	jsr %pc@(0x5590)                      ; ReturnToMS
    140c:  3600           	movew %d0,%d3                         
    140e:  584f           	addqw #4,%sp                          
    1410:  6708           	beqs 0x141a                           ; ↓ label_1
    1412:  303c 2906      	movew #10502,%d0                      
    1416:  6000 0162      	braw 0x157a                           ; ↓ label_14

  label_1:
    141a:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    141e:  4eba 0a12      	jsr %pc@(0x1e32)                      ; PrvAssureMounted
    1422:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1426:  4eba 41cc      	jsr %pc@(0x55f4)                      ; ReturnToIO
    142a:  7001           	moveq #1,%d0                          
    142c:  206e fffc      	moveal %fp@(-4),%a0                   
    1430:  c0a8 0008      	andl %a0@(8),%d0                      
    1434:  504f           	addqw #8,%sp                          
    1436:  6700 0136      	beqw 0x156e                           ; ↓ label_11
    143a:  357c 0200 0004 	movew #512,%a2@(4)                    
    1440:  357c 0010 0006 	movew #16,%a2@(6)                     
    1446:  157c 0080 000e 	moveb #-128,%a2@(14)                  
    144c:  206e fffc      	moveal %fp@(-4),%a0                   
    1450:  2668 0020      	moveal %a0@(32),%a3                   
    1454:  302b 25a4      	movew %a3@(9636),%d0                  
    1458:  0440 0200      	subiw #512,%d0                        
    145c:  6722           	beqs 0x1480                           ; ↓ label_2
    145e:  0440 0200      	subiw #512,%d0                        
    1462:  673e           	beqs 0x14a2                           ; ↓ label_3
    1464:  0440 0400      	subiw #1024,%d0                       
    1468:  6700 0092      	beqw 0x14fc                           ; ↓ label_6
    146c:  0440 0800      	subiw #2048,%d0                       
    1470:  6700 00aa      	beqw 0x151c                           ; ↓ label_7
    1474:  0440 1000      	subiw #4096,%d0                       
    1478:  6700 00c2      	beqw 0x153c                           ; ↓ label_8
    147c:  6000 00de      	braw 0x155c                           ; ↓ label_9

  label_2:
    1480:  24bc 0000 1ec0 	movel #7872,%a2@                      
    1486:  357c 0002 0008 	movew #2,%a2@(8)                      
    148c:  157c 0010 000c 	moveb #16,%a2@(12)                    
    1492:  157c 0001 000d 	moveb #1,%a2@(13)                     
    1498:  701b           	moveq #27,%d0                         
    149a:  2540 0010      	movel %d0,%a2@(16)                    
    149e:  6000 00c2      	braw 0x1562                           ; ↓ label_10

  label_3:
    14a2:  382b 25a2      	movew %a3@(9634),%d4                  
    14a6:  0c44 0008      	cmpiw #8,%d4                          
    14aa:  6622           	bnes 0x14ce                           ; ↓ label_4
    14ac:  24bc 0000 3dc0 	movel #15808,%a2@                     
    14b2:  357c 0002 0008 	movew #2,%a2@(8)                      
    14b8:  157c 0010 000c 	moveb #16,%a2@(12)                    
    14be:  157c 0001 000d 	moveb #1,%a2@(13)                     
    14c4:  7019           	moveq #25,%d0                         
    14c6:  2540 0010      	movel %d0,%a2@(16)                    
    14ca:  6000 0096      	braw 0x1562                           ; ↓ label_10

  label_4:
    14ce:  0c44 0010      	cmpiw #16,%d4                         
    14d2:  6620           	bnes 0x14f4                           ; ↓ label_5
    14d4:  24bc 0000 7b80 	movel #31616,%a2@                     
    14da:  357c 0004 0008 	movew #4,%a2@(8)                      
    14e0:  157c 0020 000c 	moveb #32,%a2@(12)                    
    14e6:  157c 0001 000d 	moveb #1,%a2@(13)                     
    14ec:  7019           	moveq #25,%d0                         
    14ee:  2540 0010      	movel %d0,%a2@(16)                    
    14f2:  606e           	bras 0x1562                           ; ↓ label_10

  label_5:
    14f4:  303c 2906      	movew #10502,%d0                      
    14f8:  6000 0080      	braw 0x157a                           ; ↓ label_14

  label_6:
    14fc:  24bc 0000 f780 	movel #63360,%a2@                     
    1502:  357c 0004 0008 	movew #4,%a2@(8)                      
    1508:  157c 0020 000c 	moveb #32,%a2@(12)                    
    150e:  157c 0001 000d 	moveb #1,%a2@(13)                     
    1514:  7013           	moveq #19,%d0                         
    1516:  2540 0010      	movel %d0,%a2@(16)                    
    151a:  6046           	bras 0x1562                           ; ↓ label_10

  label_7:
    151c:  24bc 0001 ef00 	movel #126720,%a2@                    
    1522:  357c 0008 0008 	movew #8,%a2@(8)                      
    1528:  157c 0020 000c 	moveb #32,%a2@(12)                    
    152e:  157c 0001 000d 	moveb #1,%a2@(13)                     
    1534:  7027           	moveq #39,%d0                         
    1536:  2540 0010      	movel %d0,%a2@(16)                    
    153a:  6026           	bras 0x1562                           ; ↓ label_10

  label_8:
    153c:  24bc 0003 de00 	movel #253440,%a2@                    
    1542:  357c 0010 0008 	movew #16,%a2@(8)                     
    1548:  157c 0020 000c 	moveb #32,%a2@(12)                    
    154e:  157c 0006 000d 	moveb #6,%a2@(13)                     
    1554:  7021           	moveq #33,%d0                         
    1556:  2540 0010      	movel %d0,%a2@(16)                    
    155a:  6006           	bras 0x1562                           ; ↓ label_10

  label_9:
    155c:  303c 2906      	movew #10502,%d0                      
    1560:  6018           	bras 0x157a                           ; ↓ label_14

  label_10:
    1562:  2012           	movel %a2@,%d0                        
    1564:  90aa 0010      	subl %a2@(16),%d0                     
    1568:  2540 0014      	movel %d0,%a2@(20)                    
    156c:  600a           	bras 0x1578                           ; ↓ label_13

  label_11:
    156e:  363c 2906      	movew #10502,%d3                      
    1572:  6004           	bras 0x1578                           ; ↓ label_13

  label_12:
    1574:  363c 0502      	movew #1282,%d3                       

  label_13:
    1578:  3003           	movew %d3,%d0                         

  label_14:
    157a:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    157e:  4e5e           	unlk %fp                              
    1580:  4e75           	rts                                   
```

# MSSlotFormat: 0x159a - 0x1724

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    159a:  4e56 fffc      	linkw %fp,#-4                         
    159e:  48e7 1c00      	moveml %d3-%d5,%sp@-                  
    15a2:  486e fffc      	pea %fp@(-4)                          
    15a6:  3f2e 000a      	movew %fp@(10),%sp@-                  
    15aa:  3f2e 0008      	movew %fp@(8),%sp@-                   
    15ae:  4eba 0814      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    15b2:  3600           	movew %d0,%d3                         
    15b4:  504f           	addqw #8,%sp                          
    15b6:  6706           	beqs 0x15be                           ; ↓ label_0
    15b8:  3003           	movew %d3,%d0                         
    15ba:  6000 0162      	braw 0x171e                           ; ↓ label_11

  label_0:
    15be:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    15c2:  4eba 3fcc      	jsr %pc@(0x5590)                      ; ReturnToMS
    15c6:  3600           	movew %d0,%d3                         
    15c8:  584f           	addqw #4,%sp                          
    15ca:  6708           	beqs 0x15d4                           ; ↓ label_1
    15cc:  303c 2906      	movew #10502,%d0                      
    15d0:  6000 014c      	braw 0x171e                           ; ↓ label_11

  label_1:
    15d4:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    15d8:  4eba 0858      	jsr %pc@(0x1e32)                      ; PrvAssureMounted
    15dc:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    15e0:  4eba 4012      	jsr %pc@(0x55f4)                      ; ReturnToIO
    15e4:  7001           	moveq #1,%d0                          
    15e6:  206e fffc      	moveal %fp@(-4),%a0                   
    15ea:  2628 0008      	movel %a0@(8),%d3                     
    15ee:  c083           	andl %d3,%d0                          
    15f0:  504f           	addqw #8,%sp                          
    15f2:  6700 0124      	beqw 0x1718                           ; ↓ label_9
    15f6:  7010           	moveq #16,%d0                         
    15f8:  c083           	andl %d3,%d0                          
    15fa:  6606           	bnes 0x1602                           ; ↓ label_2
    15fc:  7020           	moveq #32,%d0                         
    15fe:  c083           	andl %d3,%d0                          
    1600:  6708           	beqs 0x160a                           ; ↓ label_3

  label_2:
    1602:  303c 2907      	movew #10503,%d0                      
    1606:  6000 0116      	braw 0x171e                           ; ↓ label_11

  label_3:
    160a:  42a7           	clrl %sp@-                            
    160c:  206e fffc      	moveal %fp@(-4),%a0                   
    1610:  2f28 000e      	movel %a0@(14),%sp@-                  
    1614:  4e4f           	trap #15                              
    1616:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1618:  1f3c 0001      	moveb #1,%sp@-                        
    161c:  4eba 0db6      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    1620:  1800           	moveb %d0,%d4                         
    1622:  206e fffc      	moveal %fp@(-4),%a0                   
    1626:  2028 0004      	movel %a0@(4),%d0                     
    162a:  0280 0000 0080 	andil #128,%d0                        
    1630:  4fef 000a      	lea %sp@(10),%sp                      
    1634:  6612           	bnes 0x1648                           ; ↓ label_4
    1636:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    163a:  4eba 096c      	jsr %pc@(0x1fa8)                      ; PrvSupplyPower
    163e:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1642:  4eba 08a4      	jsr %pc@(0x1ee8)                      ; PrvDoMgAuth
    1646:  504f           	addqw #8,%sp                          

  label_4:
    1648:  206e fffc      	moveal %fp@(-4),%a0                   
    164c:  02a8 feff ffff 	andil #-16777217,%a0@(4)              
    1652:  0004                                                  
    1654:  206e fffc      	moveal %fp@(-4),%a0                   
    1658:  02a8 fdff ffff 	andil #-33554433,%a0@(4)              
    165e:  0004                                                  
    1660:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1664:  4eba 3f2a      	jsr %pc@(0x5590)                      ; ReturnToMS
    1668:  3600           	movew %d0,%d3                         
    166a:  584f           	addqw #4,%sp                          
    166c:  6708           	beqs 0x1676                           ; ↓ label_5
    166e:  303c 2906      	movew #10502,%d0                      
    1672:  6000 00aa      	braw 0x171e                           ; ↓ label_11

  label_5:
    1676:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    167a:  4eba 11b4      	jsr %pc@(0x2830)                      ; PrvJobFormat
    167e:  2a00           	movel %d0,%d5                         
    1680:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1684:  4eba 3f6e      	jsr %pc@(0x55f4)                      ; ReturnToIO
    1688:  504f           	addqw #8,%sp                          
    168a:  2005           	movel %d5,%d0                         
    168c:  0c80 0000 0007 	cmpil #7,%d0                          
    1692:  6256           	bhis 0x16ea                           ; ↓ label_6
    1694:  d040           	addw %d0,%d0                          
    1696:  303b 0006      	movew %pc@(0x169e,%d0:w),%d0          
    169a:  4efb 0002      	jmp %pc@(0x169e,%d0:w)                
    169e:  0010 004c      	orib #76,%a0@                         
    16a2:  003a 004c 004c 	orib #76,%pc@(0x16f2)                 ; ??? 0x16f2
    16a8:  004c 0040      	oriw #64,%a4                          
    16ac:  0046 7002      	oriw #28674,%d6                       
    16b0:  206e fffc      	moveal %fp@(-4),%a0                   
    16b4:  c0a8 0008      	andl %a0@(8),%d0                      
    16b8:  673e           	beqs 0x16f8                           ; ↓ label_7
    16ba:  2028 0004      	movel %a0@(4),%d0                     
    16be:  0280 0000 0100 	andil #256,%d0                        
    16c4:  6732           	beqs 0x16f8                           ; ↓ label_7
    16c6:  3f3c ffff      	movew #-1,%sp@-                       
    16ca:  3f28 0024      	movew %a0@(36),%sp@-                  
    16ce:  4e4f           	trap #15                              
    16d0:  a80a           	0124012                               ; ??? 0xa80a
    16d2:  3600           	movew %d0,%d3                         
    16d4:  584f           	addqw #4,%sp                          
    16d6:  6020           	bras 0x16f8                           ; ↓ label_7
    16d8:  363c 2903      	movew #10499,%d3                      
    16dc:  601a           	bras 0x16f8                           ; ↓ label_7
    16de:  363c 2907      	movew #10503,%d3                      
    16e2:  6014           	bras 0x16f8                           ; ↓ label_7
    16e4:  363c 0102      	movew #258,%d3                        
    16e8:  600e           	bras 0x16f8                           ; ↓ label_7

  label_6:
    16ea:  206e fffc      	moveal %fp@(-4),%a0                   
    16ee:  70fe           	moveq #-2,%d0                         
    16f0:  c1a8 0008      	andl %d0,%a0@(8)                      
    16f4:  363c 2908      	movew #10504,%d3                      

  label_7:
    16f8:  4a04           	tstb %d4                              
    16fa:  6608           	bnes 0x1704                           ; ↓ label_8
    16fc:  4227           	clrb %sp@-                            
    16fe:  4eba 0cd4      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    1702:  544f           	addqw #2,%sp                          

  label_8:
    1704:  4878 0064      	pea 0x64                              
    1708:  206e fffc      	moveal %fp@(-4),%a0                   
    170c:  2f28 000e      	movel %a0@(14),%sp@-                  
    1710:  4e4f           	trap #15                              
    1712:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1714:  504f           	addqw #8,%sp                          
    1716:  6004           	bras 0x171c                           ; ↓ label_10

  label_9:
    1718:  363c 2906      	movew #10502,%d3                      

  label_10:
    171c:  3003           	movew %d3,%d0                         

  label_11:
    171e:  4cdf 0038      	moveml %sp@+,%d3-%d5                  
    1722:  4e5e           	unlk %fp                              
    1724:  4e75           	rts                                   
```

# MSSlotReadSector: 0x1736 - 0x1888

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
    1736:  4e56 fff8      	linkw %fp,#-8                         
    173a:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    173e:  246e 0014      	moveal %fp@(20),%a2                   
    1742:  2d52 fff8      	movel %a2@,%fp@(-8)                   
    1746:  4292           	clrl %a2@                             
    1748:  486e fffc      	pea %fp@(-4)                          
    174c:  3f2e 000a      	movew %fp@(10),%sp@-                  
    1750:  3f2e 0008      	movew %fp@(8),%sp@-                   
    1754:  4eba 066e      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    1758:  3600           	movew %d0,%d3                         
    175a:  504f           	addqw #8,%sp                          
    175c:  6706           	beqs 0x1764                           ; ↓ label_0
    175e:  3003           	movew %d3,%d0                         
    1760:  6000 0120      	braw 0x1882                           ; ↓ label_10

  label_0:
    1764:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1768:  4eba 3e26      	jsr %pc@(0x5590)                      ; ReturnToMS
    176c:  3600           	movew %d0,%d3                         
    176e:  584f           	addqw #4,%sp                          
    1770:  6708           	beqs 0x177a                           ; ↓ label_1
    1772:  303c 2906      	movew #10502,%d0                      
    1776:  6000 010a      	braw 0x1882                           ; ↓ label_10

  label_1:
    177a:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    177e:  4eba 06b2      	jsr %pc@(0x1e32)                      ; PrvAssureMounted
    1782:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1786:  4eba 3e6c      	jsr %pc@(0x55f4)                      ; ReturnToIO
    178a:  7010           	moveq #16,%d0                         
    178c:  206e fffc      	moveal %fp@(-4),%a0                   
    1790:  c0a8 0004      	andl %a0@(4),%d0                      
    1794:  504f           	addqw #8,%sp                          
    1796:  6700 00e4      	beqw 0x187c                           ; ↓ label_8
    179a:  7001           	moveq #1,%d0                          
    179c:  c0a8 0008      	andl %a0@(8),%d0                      
    17a0:  6700 00da      	beqw 0x187c                           ; ↓ label_8
    17a4:  4aae fff8      	tstl %fp@(-8)                         
    17a8:  6606           	bnes 0x17b0                           ; ↓ label_2
    17aa:  7000           	moveq #0,%d0                          
    17ac:  6000 00d4      	braw 0x1882                           ; ↓ label_10

  label_2:
    17b0:  42a7           	clrl %sp@-                            
    17b2:  206e fffc      	moveal %fp@(-4),%a0                   
    17b6:  2f28 000e      	movel %a0@(14),%sp@-                  
    17ba:  4e4f           	trap #15                              
    17bc:  a09a           	0120232                               ; sysTrapSysTimerWrite
    17be:  1f3c 0001      	moveb #1,%sp@-                        
    17c2:  4eba 0c10      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    17c6:  1800           	moveb %d0,%d4                         
    17c8:  206e fffc      	moveal %fp@(-4),%a0                   
    17cc:  2028 0004      	movel %a0@(4),%d0                     
    17d0:  0280 0000 0080 	andil #128,%d0                        
    17d6:  4fef 000a      	lea %sp@(10),%sp                      
    17da:  6612           	bnes 0x17ee                           ; ↓ label_3
    17dc:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    17e0:  4eba 07c6      	jsr %pc@(0x1fa8)                      ; PrvSupplyPower
    17e4:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    17e8:  4eba 06fe      	jsr %pc@(0x1ee8)                      ; PrvDoMgAuth
    17ec:  504f           	addqw #8,%sp                          

  label_3:
    17ee:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    17f2:  4eba 3d9c      	jsr %pc@(0x5590)                      ; ReturnToMS
    17f6:  3600           	movew %d0,%d3                         
    17f8:  584f           	addqw #4,%sp                          
    17fa:  6708           	beqs 0x1804                           ; ↓ label_4
    17fc:  303c 2906      	movew #10502,%d0                      
    1800:  6000 0080      	braw 0x1882                           ; ↓ label_10

  label_4:
    1804:  486e fff8      	pea %fp@(-8)                          
    1808:  2f2e 0010      	movel %fp@(16),%sp@-                  
    180c:  2f2e 000c      	movel %fp@(12),%sp@-                  
    1810:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1814:  4eba 0db4      	jsr %pc@(0x25ca)                      ; PrvJobReadSector
    1818:  2600           	movel %d0,%d3                         
    181a:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    181e:  4eba 3dd4      	jsr %pc@(0x55f4)                      ; ReturnToIO
    1822:  4fef 0014      	lea %sp@(20),%sp                      
    1826:  2003           	movel %d3,%d0                         
    1828:  0c80 0000 0004 	cmpil #4,%d0                          
    182e:  6228           	bhis 0x1858                           ; ↓ label_5
    1830:  d040           	addw %d0,%d0                          
    1832:  303b 0006      	movew %pc@(0x183a,%d0:w),%d0          
    1836:  4efb 0002      	jmp %pc@(0x183a,%d0:w)                
    183a:  000a 001e      	orib #30,%a2                          
    183e:  0012 001e      	orib #30,%a2@                         
    1842:  0018 24ae      	orib #-82,%a0@+                       
    1846:  fff8           	0177770                               
    1848:  7600           	moveq #0,%d3                          
    184a:  6010           	bras 0x185c                           ; ↓ label_6
    184c:  363c 2903      	movew #10499,%d3                      
    1850:  600a           	bras 0x185c                           ; ↓ label_6
    1852:  363c 0502      	movew #1282,%d3                       
    1856:  6004           	bras 0x185c                           ; ↓ label_6

  label_5:
    1858:  363c 2908      	movew #10504,%d3                      

  label_6:
    185c:  4a04           	tstb %d4                              
    185e:  6608           	bnes 0x1868                           ; ↓ label_7
    1860:  4227           	clrb %sp@-                            
    1862:  4eba 0b70      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    1866:  544f           	addqw #2,%sp                          

  label_7:
    1868:  4878 0064      	pea 0x64                              
    186c:  206e fffc      	moveal %fp@(-4),%a0                   
    1870:  2f28 000e      	movel %a0@(14),%sp@-                  
    1874:  4e4f           	trap #15                              
    1876:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1878:  504f           	addqw #8,%sp                          
    187a:  6004           	bras 0x1880                           ; ↓ label_9

  label_8:
    187c:  363c 2906      	movew #10502,%d3                      

  label_9:
    1880:  3003           	movew %d3,%d0                         

  label_10:
    1882:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    1886:  4e5e           	unlk %fp                              
    1888:  4e75           	rts                                   
```

# MSSlotWriteSector: 0x189e - 0x1a10

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
    189e:  4e56 fff8      	linkw %fp,#-8                         
    18a2:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    18a6:  246e 0014      	moveal %fp@(20),%a2                   
    18aa:  2d52 fff8      	movel %a2@,%fp@(-8)                   
    18ae:  4292           	clrl %a2@                             
    18b0:  486e fffc      	pea %fp@(-4)                          
    18b4:  3f2e 000a      	movew %fp@(10),%sp@-                  
    18b8:  3f2e 0008      	movew %fp@(8),%sp@-                   
    18bc:  4eba 0506      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    18c0:  3600           	movew %d0,%d3                         
    18c2:  504f           	addqw #8,%sp                          
    18c4:  6706           	beqs 0x18cc                           ; ↓ label_0
    18c6:  3003           	movew %d3,%d0                         
    18c8:  6000 0140      	braw 0x1a0a                           ; ↓ label_12

  label_0:
    18cc:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    18d0:  4eba 3cbe      	jsr %pc@(0x5590)                      ; ReturnToMS
    18d4:  3600           	movew %d0,%d3                         
    18d6:  584f           	addqw #4,%sp                          
    18d8:  6708           	beqs 0x18e2                           ; ↓ label_1
    18da:  303c 2906      	movew #10502,%d0                      
    18de:  6000 012a      	braw 0x1a0a                           ; ↓ label_12

  label_1:
    18e2:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    18e6:  4eba 054a      	jsr %pc@(0x1e32)                      ; PrvAssureMounted
    18ea:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    18ee:  4eba 3d04      	jsr %pc@(0x55f4)                      ; ReturnToIO
    18f2:  7010           	moveq #16,%d0                         
    18f4:  206e fffc      	moveal %fp@(-4),%a0                   
    18f8:  c0a8 0004      	andl %a0@(4),%d0                      
    18fc:  504f           	addqw #8,%sp                          
    18fe:  6700 0104      	beqw 0x1a04                           ; ↓ label_10
    1902:  7001           	moveq #1,%d0                          
    1904:  2628 0008      	movel %a0@(8),%d3                     
    1908:  c083           	andl %d3,%d0                          
    190a:  6700 00f8      	beqw 0x1a04                           ; ↓ label_10
    190e:  7010           	moveq #16,%d0                         
    1910:  c083           	andl %d3,%d0                          
    1912:  6606           	bnes 0x191a                           ; ↓ label_2
    1914:  7020           	moveq #32,%d0                         
    1916:  c083           	andl %d3,%d0                          
    1918:  6708           	beqs 0x1922                           ; ↓ label_3

  label_2:
    191a:  303c 2907      	movew #10503,%d0                      
    191e:  6000 00ea      	braw 0x1a0a                           ; ↓ label_12

  label_3:
    1922:  4aae fff8      	tstl %fp@(-8)                         
    1926:  6606           	bnes 0x192e                           ; ↓ label_4
    1928:  7000           	moveq #0,%d0                          
    192a:  6000 00de      	braw 0x1a0a                           ; ↓ label_12

  label_4:
    192e:  42a7           	clrl %sp@-                            
    1930:  206e fffc      	moveal %fp@(-4),%a0                   
    1934:  2f28 000e      	movel %a0@(14),%sp@-                  
    1938:  4e4f           	trap #15                              
    193a:  a09a           	0120232                               ; sysTrapSysTimerWrite
    193c:  1f3c 0001      	moveb #1,%sp@-                        
    1940:  4eba 0a92      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    1944:  1800           	moveb %d0,%d4                         
    1946:  206e fffc      	moveal %fp@(-4),%a0                   
    194a:  2028 0004      	movel %a0@(4),%d0                     
    194e:  0280 0000 0080 	andil #128,%d0                        
    1954:  4fef 000a      	lea %sp@(10),%sp                      
    1958:  6612           	bnes 0x196c                           ; ↓ label_5
    195a:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    195e:  4eba 0648      	jsr %pc@(0x1fa8)                      ; PrvSupplyPower
    1962:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1966:  4eba 0580      	jsr %pc@(0x1ee8)                      ; PrvDoMgAuth
    196a:  504f           	addqw #8,%sp                          

  label_5:
    196c:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1970:  4eba 3c1e      	jsr %pc@(0x5590)                      ; ReturnToMS
    1974:  3600           	movew %d0,%d3                         
    1976:  584f           	addqw #4,%sp                          
    1978:  6708           	beqs 0x1982                           ; ↓ label_6
    197a:  303c 2906      	movew #10502,%d0                      
    197e:  6000 008a      	braw 0x1a0a                           ; ↓ label_12

  label_6:
    1982:  486e fff8      	pea %fp@(-8)                          
    1986:  2f2e 0010      	movel %fp@(16),%sp@-                  
    198a:  2f2e 000c      	movel %fp@(12),%sp@-                  
    198e:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    1992:  4eba 0d7a      	jsr %pc@(0x270e)                      ; PrvJobWriteSector
    1996:  2600           	movel %d0,%d3                         
    1998:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    199c:  4eba 3c56      	jsr %pc@(0x55f4)                      ; ReturnToIO
    19a0:  4fef 0014      	lea %sp@(20),%sp                      
    19a4:  2003           	movel %d3,%d0                         
    19a6:  0c80 0000 0006 	cmpil #6,%d0                          
    19ac:  6232           	bhis 0x19e0                           ; ↓ label_7
    19ae:  d040           	addw %d0,%d0                          
    19b0:  303b 0006      	movew %pc@(0x19b8,%d0:w),%d0          
    19b4:  4efb 0002      	jmp %pc@(0x19b8,%d0:w)                
    19b8:  000e 0028      	orib #40,%fp                          
    19bc:  0016 0028      	orib #40,%fp@                         
    19c0:  001c 0022      	orib #34,%a4@+                        
    19c4:  0022 24ae      	orib #-82,%a2@-                       
    19c8:  fff8           	0177770                               
    19ca:  7600           	moveq #0,%d3                          
    19cc:  6016           	bras 0x19e4                           ; ↓ label_8
    19ce:  363c 2903      	movew #10499,%d3                      
    19d2:  6010           	bras 0x19e4                           ; ↓ label_8
    19d4:  363c 0502      	movew #1282,%d3                       
    19d8:  600a           	bras 0x19e4                           ; ↓ label_8
    19da:  363c 2907      	movew #10503,%d3                      
    19de:  6004           	bras 0x19e4                           ; ↓ label_8

  label_7:
    19e0:  363c 2908      	movew #10504,%d3                      

  label_8:
    19e4:  4a04           	tstb %d4                              
    19e6:  6608           	bnes 0x19f0                           ; ↓ label_9
    19e8:  4227           	clrb %sp@-                            
    19ea:  4eba 09e8      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    19ee:  544f           	addqw #2,%sp                          

  label_9:
    19f0:  4878 0064      	pea 0x64                              
    19f4:  206e fffc      	moveal %fp@(-4),%a0                   
    19f8:  2f28 000e      	movel %a0@(14),%sp@-                  
    19fc:  4e4f           	trap #15                              
    19fe:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1a00:  504f           	addqw #8,%sp                          
    1a02:  6004           	bras 0x1a08                           ; ↓ label_11

  label_10:
    1a04:  363c 2906      	movew #10502,%d3                      

  label_11:
    1a08:  3003           	movew %d3,%d0                         

  label_12:
    1a0a:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    1a0e:  4e5e           	unlk %fp                              
    1a10:  4e75           	rts                                   
```

# MSSlotCheckPower: 0x1a26 - 0x1a6c

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???

```
    1a26:  4e56 fffc      	linkw %fp,#-4                         
    1a2a:  2f03           	movel %d3,%sp@-                       
    1a2c:  486e fffc      	pea %fp@(-4)                          
    1a30:  3f2e 000a      	movew %fp@(10),%sp@-                  
    1a34:  3f2e 0008      	movew %fp@(8),%sp@-                   
    1a38:  4eba 038a      	jsr %pc@(0x1dc4)                      ; PrvDoPreCheck
    1a3c:  3600           	movew %d0,%d3                         
    1a3e:  504f           	addqw #8,%sp                          
    1a40:  6704           	beqs 0x1a46                           ; ↓ label_0
    1a42:  3003           	movew %d3,%d0                         
    1a44:  6022           	bras 0x1a68                           ; ↓ label_4

  label_0:
    1a46:  7001           	moveq #1,%d0                          
    1a48:  206e fffc      	moveal %fp@(-4),%a0                   
    1a4c:  c0a8 0008      	andl %a0@(8),%d0                      
    1a50:  6712           	beqs 0x1a64                           ; ↓ label_3
    1a52:  0c38 0059 0139 	cmpib #89,0x139                       
    1a58:  6206           	bhis 0x1a60                           ; ↓ label_1
    1a5a:  303c 2902      	movew #10498,%d0                      
    1a5e:  6002           	bras 0x1a62                           ; ↓ label_2

  label_1:
    1a60:  7000           	moveq #0,%d0                          

  label_2:
    1a62:  6004           	bras 0x1a68                           ; ↓ label_4

  label_3:
    1a64:  303c 2906      	movew #10502,%d0                      

  label_4:
    1a68:  261f           	movel %sp@+,%d3                       
    1a6a:  4e5e           	unlk %fp                              
    1a6c:  4e75           	rts                                   
```

# PrvShouldWeInstall: 0x1a82 - 0x1a8a

```
    1a82:  4e56 0000      	linkw %fp,#0                          
    1a86:  7000           	moveq #0,%d0                          
    1a88:  4e5e           	unlk %fp                              
    1a8a:  4e75           	rts                                   
```

# PrvCrdiNotificationHandler: 0x1aa2 - 0x1aca

Parameters:

   * `%fp@(8)`      : ???

```
    1aa2:  4e56 0000      	linkw %fp,#0                          
    1aa6:  2f0b           	movel %a3,%sp@-                       
    1aa8:  222e 0008      	movel %fp@(8),%d1                     
    1aac:  2041           	moveal %d1,%a0                        
    1aae:  2668 000c      	moveal %a0@(12),%a3                   
    1ab2:  224b           	moveal %a3,%a1                        
    1ab4:  3028 000a      	movew %a0@(10),%d0                    
    1ab8:  b06b 0012      	cmpw %a3@(18),%d0                     
    1abc:  6606           	bnes 0x1ac4                           ; ↓ label_0
    1abe:  7004           	moveq #4,%d0                          
    1ac0:  81a9 0004      	orl %d0,%a1@(4)                       

  label_0:
    1ac4:  7000           	moveq #0,%d0                          
    1ac6:  265f           	moveal %sp@+,%a3                      
    1ac8:  4e5e           	unlk %fp                              
    1aca:  4e75           	rts                                   
```

# PrvCrdoNotificationHandler: 0x1aea - 0x1b4a

Parameters:

   * `%fp@(8)`      : ???

```
    1aea:  4e56 0000      	linkw %fp,#0                          
    1aee:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    1af2:  262e 0008      	movel %fp@(8),%d3                     
    1af6:  2043           	moveal %d3,%a0                        
    1af8:  2668 000c      	moveal %a0@(12),%a3                   
    1afc:  244b           	moveal %a3,%a2                        
    1afe:  3028 000a      	movew %a0@(10),%d0                    
    1b02:  b06b 0012      	cmpw %a3@(18),%d0                     
    1b06:  663a           	bnes 0x1b42                           ; ↓ label_0
    1b08:  0279 ff7f 1100 	andiw #-129,0x11000406                
    1b0e:  0406                                                  
    1b10:  0279 fffd 1100 	andiw #-3,0x11000222                  
    1b16:  0222                                                  
    1b18:  022a 00fe 002f 	andib #-2,%a2@(47)                    
    1b1e:  2f0a           	movel %a2,%sp@-                       
    1b20:  4eba 3a30      	jsr %pc@(0x5552)                      ; InitializeIOParam
    1b24:  2f0a           	movel %a2,%sp@-                       
    1b26:  4eba 3a68      	jsr %pc@(0x5590)                      ; ReturnToMS
    1b2a:  7008           	moveq #8,%d0                          
    1b2c:  81aa 0004      	orl %d0,%a2@(4)                       
    1b30:  4227           	clrb %sp@-                            
    1b32:  4eba 0642      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    1b36:  02aa ffff ff7f 	andil #-129,%a2@(4)                   
    1b3c:  0004                                                  
    1b3e:  4fef 000a      	lea %sp@(10),%sp                      

  label_0:
    1b42:  7000           	moveq #0,%d0                          
    1b44:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    1b48:  4e5e           	unlk %fp                              
    1b4a:  4e75           	rts                                   
```

# PrvVolmNotificationHandler: 0x1b6a - 0x1c44

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-10)`    : ???
   * `%fp@(-28)`    : ???

```
    1b6a:  4e56 ffe4      	linkw %fp,#-28                        
    1b6e:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    1b72:  266e 0008      	moveal %fp@(8),%a3                    
    1b76:  246b 000c      	moveal %a3@(12),%a2                   
    1b7a:  486e ffe4      	pea %fp@(-28)                         
    1b7e:  206b 0008      	moveal %a3@(8),%a0                    
    1b82:  3f10           	movew %a0@,%sp@-                      
    1b84:  741c           	moveq #28,%d2                         
    1b86:  4e4f           	trap #15                              
    1b88:  a348           	0121510                               ; sysTrapFileSystemDispatch
    1b8a:  4a40           	tstw %d0                              
    1b8c:  5c4f           	addqw #6,%sp                          
    1b8e:  6706           	beqs 0x1b96                           ; ↓ label_0
    1b90:  7000           	moveq #0,%d0                          
    1b92:  6000 00aa      	braw 0x1c3e                           ; ↓ label_7

  label_0:
    1b96:  302e fff6      	movew %fp@(-10),%d0                   
    1b9a:  b06a 0012      	cmpw %a2@(18),%d0                     
    1b9e:  6600 009c      	bnew 0x1c3c                           ; ↓ label_6
    1ba2:  42a7           	clrl %sp@-                            
    1ba4:  2f2a 000e      	movel %a2@(14),%sp@-                  
    1ba8:  4e4f           	trap #15                              
    1baa:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1bac:  7640           	moveq #64,%d3                         
    1bae:  c6aa 0004      	andl %a2@(4),%d3                      
    1bb2:  1f3c 0001      	moveb #1,%sp@-                        
    1bb6:  4eba 081c      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    1bba:  1800           	moveb %d0,%d4                         
    1bbc:  202a 0004      	movel %a2@(4),%d0                     
    1bc0:  0280 0000 0080 	andil #128,%d0                        
    1bc6:  4fef 000a      	lea %sp@(10),%sp                      
    1bca:  660e           	bnes 0x1bda                           ; ↓ label_1
    1bcc:  2f0a           	movel %a2,%sp@-                       
    1bce:  4eba 03d8      	jsr %pc@(0x1fa8)                      ; PrvSupplyPower
    1bd2:  2f0a           	movel %a2,%sp@-                       
    1bd4:  4eba 0312      	jsr %pc@(0x1ee8)                      ; PrvDoMgAuth
    1bd8:  504f           	addqw #8,%sp                          

  label_1:
    1bda:  202a 0004      	movel %a2@(4),%d0                     
    1bde:  0280 0001 0000 	andil #65536,%d0                      
    1be4:  673c           	beqs 0x1c22                           ; ↓ label_4
    1be6:  7040           	moveq #64,%d0                         
    1be8:  81aa 0004      	orl %d0,%a2@(4)                       
    1bec:  4267           	clrw %sp@-                            
    1bee:  486a 000c      	pea %a2@(12)                          
    1bf2:  2f2a 0020      	movel %a2@(32),%sp@-                  
    1bf6:  3f2a 0024      	movew %a2@(36),%sp@-                  
    1bfa:  4e4f           	trap #15                              
    1bfc:  a806           	0124006                               ; ??? 0xa806
    1bfe:  4a40           	tstw %d0                              
    1c00:  4fef 000c      	lea %sp@(12),%sp                      
    1c04:  670a           	beqs 0x1c10                           ; ↓ label_2
    1c06:  00aa 0200 0000 	oril #33554432,%a2@(4)                
    1c0c:  0004                                                  
    1c0e:  6008           	bras 0x1c18                           ; ↓ label_3

  label_2:
    1c10:  00aa 0100 0000 	oril #16777216,%a2@(4)                
    1c16:  0004                                                  

  label_3:
    1c18:  4a03           	tstb %d3                              
    1c1a:  6606           	bnes 0x1c22                           ; ↓ label_4
    1c1c:  70bf           	moveq #-65,%d0                        
    1c1e:  c1aa 0004      	andl %d0,%a2@(4)                      

  label_4:
    1c22:  4a04           	tstb %d4                              
    1c24:  6608           	bnes 0x1c2e                           ; ↓ label_5
    1c26:  4227           	clrb %sp@-                            
    1c28:  4eba 07aa      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    1c2c:  544f           	addqw #2,%sp                          

  label_5:
    1c2e:  4878 0064      	pea 0x64                              
    1c32:  2f2a 000e      	movel %a2@(14),%sp@-                  
    1c36:  4e4f           	trap #15                              
    1c38:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1c3a:  504f           	addqw #8,%sp                          

  label_6:
    1c3c:  7000           	moveq #0,%d0                          

  label_7:
    1c3e:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    1c42:  4e5e           	unlk %fp                              
    1c44:  4e75           	rts                                   
```

# PrvInsRmvIntHandler: 0x1c64 - 0x1d64

Parameters:

   * `%fp@(8)`      : ???

```
    1c64:  4e56 0000      	linkw %fp,#0                          
    1c68:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    1c6c:  2078 0304      	moveal 0x304,%a0                      
    1c70:  2668 0024      	moveal %a0@(36),%a3                   
    1c74:  244b           	moveal %a3,%a2                        
    1c76:  362b 0012      	movew %a3@(18),%d3                    
    1c7a:  0c2e 0001 0008 	cmpib #1,%fp@(8)                      
    1c80:  667c           	bnes 0x1cfe                           ; ↓ label_1
    1c82:  7001           	moveq #1,%d0                          
    1c84:  81aa 0004      	orl %d0,%a2@(4)                       
    1c88:  70fb           	moveq #-5,%d0                         
    1c8a:  c1aa 0004      	andl %d0,%a2@(4)                      
    1c8e:  70ef           	moveq #-17,%d0                        
    1c90:  c1aa 0004      	andl %d0,%a2@(4)                      
    1c94:  70df           	moveq #-33,%d0                        
    1c96:  c1aa 0004      	andl %d0,%a2@(4)                      
    1c9a:  02aa fffe ffff 	andil #-65537,%a2@(4)                 
    1ca0:  0004                                                  
    1ca2:  02aa fffd ffff 	andil #-131073,%a2@(4)                
    1ca8:  0004                                                  
    1caa:  02aa feff ffff 	andil #-16777217,%a2@(4)              
    1cb0:  0004                                                  
    1cb2:  02aa fdff ffff 	andil #-33554433,%a2@(4)              
    1cb8:  0004                                                  
    1cba:  2478 0304      	moveal 0x304,%a2                      
    1cbe:  2078 0304      	moveal 0x304,%a0                      
    1cc2:  4aa8 0010      	tstl %a0@(16)                         
    1cc6:  6714           	beqs 0x1cdc                           ; ↓ label_0
    1cc8:  206a 0010      	moveal %a2@(16),%a0                   
    1ccc:  7001           	moveq #1,%d0                          
    1cce:  81a8 001c      	orl %d0,%a0@(28)                      
    1cd2:  206a 0010      	moveal %a2@(16),%a0                   
    1cd6:  317c 0001 0024 	movew #1,%a0@(36)                     

  label_0:
    1cdc:  3f03           	movew %d3,%sp@-                       
    1cde:  7406           	moveq #6,%d2                          
    1ce0:  4e4f           	trap #15                              
    1ce2:  a347           	0121507                               ; sysTrapExpansionDispatch
    1ce4:  4227           	clrb %sp@-                            
    1ce6:  1f3c 0001      	moveb #1,%sp@-                        
    1cea:  4eba 0688      	jsr %pc@(0x2374)                      ; MSSlotHwrSetUpInterrupt
    1cee:  1f3c 0001      	moveb #1,%sp@-                        
    1cf2:  4227           	clrb %sp@-                            
    1cf4:  4eba 067e      	jsr %pc@(0x2374)                      ; MSSlotHwrSetUpInterrupt
    1cf8:  4fef 000a      	lea %sp@(10),%sp                      
    1cfc:  6060           	bras 0x1d5e                           ; ↓ label_5

  label_1:
    1cfe:  70fe           	moveq #-2,%d0                         
    1d00:  c1aa 0004      	andl %d0,%a2@(4)                      
    1d04:  7004           	moveq #4,%d0                          
    1d06:  c0aa 0004      	andl %a2@(4),%d0                      
    1d0a:  6608           	bnes 0x1d14                           ; ↓ label_2
    1d0c:  7004           	moveq #4,%d0                          
    1d0e:  81aa 0004      	orl %d0,%a2@(4)                       
    1d12:  6006           	bras 0x1d1a                           ; ↓ label_3

  label_2:
    1d14:  70f7           	moveq #-9,%d0                         
    1d16:  c1aa 0004      	andl %d0,%a2@(4)                      

  label_3:
    1d1a:  2478 0304      	moveal 0x304,%a2                      
    1d1e:  2078 0304      	moveal 0x304,%a0                      
    1d22:  4aa8 0010      	tstl %a0@(16)                         
    1d26:  6716           	beqs 0x1d3e                           ; ↓ label_4
    1d28:  206a 0010      	moveal %a2@(16),%a0                   
    1d2c:  02a8 ffff ff00 	andil #-256,%a0@(28)                  
    1d32:  001c                                                  
    1d34:  206a 0010      	moveal %a2@(16),%a0                   
    1d38:  317c 0001 0024 	movew #1,%a0@(36)                     

  label_4:
    1d3e:  3f03           	movew %d3,%sp@-                       
    1d40:  7407           	moveq #7,%d2                          
    1d42:  4e4f           	trap #15                              
    1d44:  a347           	0121507                               ; sysTrapExpansionDispatch
    1d46:  4227           	clrb %sp@-                            
    1d48:  4227           	clrb %sp@-                            
    1d4a:  4eba 0628      	jsr %pc@(0x2374)                      ; MSSlotHwrSetUpInterrupt
    1d4e:  1f3c 0001      	moveb #1,%sp@-                        
    1d52:  1f3c 0001      	moveb #1,%sp@-                        
    1d56:  4eba 061c      	jsr %pc@(0x2374)                      ; MSSlotHwrSetUpInterrupt
    1d5a:  4fef 000a      	lea %sp@(10),%sp                      

  label_5:
    1d5e:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    1d62:  4e5e           	unlk %fp                              
    1d64:  4e75           	rts                                   
```

# PrvTimerProc: 0x1d7c - 0x1db2

Parameters:

   * `%fp@(12)`     : ???

```
    1d7c:  4e56 0000      	linkw %fp,#0                          
    1d80:  2f0a           	movel %a2,%sp@-                       
    1d82:  2f03           	movel %d3,%sp@-                       
    1d84:  262e 000c      	movel %fp@(12),%d3                    
    1d88:  6722           	beqs 0x1dac                           ; ↓ label_0
    1d8a:  2443           	moveal %d3,%a2                        
    1d8c:  2043           	moveal %d3,%a0                        
    1d8e:  4a28 002b      	tstb %a0@(43)                         
    1d92:  6618           	bnes 0x1dac                           ; ↓ label_0
    1d94:  7040           	moveq #64,%d0                         
    1d96:  c0aa 0004      	andl %a2@(4),%d0                      
    1d9a:  6610           	bnes 0x1dac                           ; ↓ label_0
    1d9c:  4227           	clrb %sp@-                            
    1d9e:  4eba 03d6      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    1da2:  02aa ffff ff7f 	andil #-129,%a2@(4)                   
    1da8:  0004                                                  
    1daa:  544f           	addqw #2,%sp                          

  label_0:
    1dac:  261f           	movel %sp@+,%d3                       
    1dae:  245f           	moveal %sp@+,%a2                      
    1db0:  4e5e           	unlk %fp                              
    1db2:  4e75           	rts                                   
```

# PrvDoPreCheck: 0x1dc4 - 0x1e20

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

```
    1dc4:  4e56 0000      	linkw %fp,#0                          
    1dc8:  2f0b           	movel %a3,%sp@-                       
    1dca:  2f0a           	movel %a2,%sp@-                       
    1dcc:  266e 000c      	moveal %fp@(12),%a3                   
    1dd0:  3f2e 0008      	movew %fp@(8),%sp@-                   
    1dd4:  4e4f           	trap #15                              
    1dd6:  a0b9           	0120271                               ; sysTrapSysLibTblEntry
    1dd8:  26a8 0004      	movel %a0@(4),%a3@                    
    1ddc:  2453           	moveal %a3@,%a2                       
    1dde:  200a           	movel %a2,%d0                         
    1de0:  544f           	addqw #2,%sp                          
    1de2:  6606           	bnes 0x1dea                           ; ↓ label_0
    1de4:  303c 290a      	movew #10506,%d0                      
    1de8:  6030           	bras 0x1e1a                           ; ↓ label_4

  label_0:
    1dea:  302a 0012      	movew %a2@(18),%d0                    
    1dee:  b06e 000a      	cmpw %fp@(10),%d0                     
    1df2:  6706           	beqs 0x1dfa                           ; ↓ label_1
    1df4:  303c 2904      	movew #10500,%d0                      
    1df8:  6020           	bras 0x1e1a                           ; ↓ label_4

  label_1:
    1dfa:  7001           	moveq #1,%d0                          
    1dfc:  204a           	moveal %a2,%a0                        
    1dfe:  5888           	addql #4,%a0                          
    1e00:  2648           	moveal %a0,%a3                        
    1e02:  c090           	andl %a0@,%d0                         
    1e04:  6606           	bnes 0x1e0c                           ; ↓ label_2
    1e06:  303c 2903      	movew #10499,%d0                      
    1e0a:  600e           	bras 0x1e1a                           ; ↓ label_4

  label_2:
    1e0c:  7008           	moveq #8,%d0                          
    1e0e:  c093           	andl %a3@,%d0                         
    1e10:  6704           	beqs 0x1e16                           ; ↓ label_3
    1e12:  7000           	moveq #0,%d0                          
    1e14:  6004           	bras 0x1e1a                           ; ↓ label_4

  label_3:
    1e16:  303c 2903      	movew #10499,%d0                      

  label_4:
    1e1a:  245f           	moveal %sp@+,%a2                      
    1e1c:  265f           	moveal %sp@+,%a3                      
    1e1e:  4e5e           	unlk %fp                              
    1e20:  4e75           	rts                                   
```

# PrvAssureMounted: 0x1e32 - 0x1ed2

Parameters:

   * `%fp@(8)`      : ???

```
    1e32:  4e56 0000      	linkw %fp,#0                          
    1e36:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    1e3a:  246e 0008      	moveal %fp@(8),%a2                    
    1e3e:  7020           	moveq #32,%d0                         
    1e40:  c0aa 0004      	andl %a2@(4),%d0                      
    1e44:  6608           	bnes 0x1e4e                           ; ↓ label_0
    1e46:  7010           	moveq #16,%d0                         
    1e48:  c0aa 0004      	andl %a2@(4),%d0                      
    1e4c:  6702           	beqs 0x1e50                           ; ↓ label_1

  label_0:
    1e4e:  607c           	bras 0x1ecc                           ; ↓ label_7

  label_1:
    1e50:  42a7           	clrl %sp@-                            
    1e52:  2f2a 000e      	movel %a2@(14),%sp@-                  
    1e56:  4e4f           	trap #15                              
    1e58:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1e5a:  1f3c 0001      	moveb #1,%sp@-                        
    1e5e:  4eba 0574      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    1e62:  1a00           	moveb %d0,%d5                         
    1e64:  2f0a           	movel %a2,%sp@-                       
    1e66:  4eba 0140      	jsr %pc@(0x1fa8)                      ; PrvSupplyPower
    1e6a:  7600           	moveq #0,%d3                          
    1e6c:  4fef 000e      	lea %sp@(14),%sp                      

  label_2:
    1e70:  2f0a           	movel %a2,%sp@-                       
    1e72:  4eba 0602      	jsr %pc@(0x2476)                      ; PrvJobMount
    1e76:  2800           	movel %d0,%d4                         
    1e78:  584f           	addqw #4,%sp                          
    1e7a:  6610           	bnes 0x1e8c                           ; ↓ label_3
    1e7c:  7010           	moveq #16,%d0                         
    1e7e:  81aa 0004      	orl %d0,%a2@(4)                       
    1e82:  2f0a           	movel %a2,%sp@-                       
    1e84:  4eba 0062      	jsr %pc@(0x1ee8)                      ; PrvDoMgAuth
    1e88:  584f           	addqw #4,%sp                          
    1e8a:  601a           	bras 0x1ea6                           ; ↓ label_4

  label_3:
    1e8c:  4878 0003      	pea 0x3                               
    1e90:  4e4f           	trap #15                              
    1e92:  a0a0           	0120240                               ; sysTrapSysTaskDelay
    1e94:  7001           	moveq #1,%d0                          
    1e96:  c0aa 0004      	andl %a2@(4),%d0                      
    1e9a:  584f           	addqw #4,%sp                          
    1e9c:  6708           	beqs 0x1ea6                           ; ↓ label_4
    1e9e:  5243           	addqw #1,%d3                          
    1ea0:  0c43 0002      	cmpiw #2,%d3                          
    1ea4:  65ca           	bcss 0x1e70                           ; ↑ label_2

  label_4:
    1ea6:  0c43 0002      	cmpiw #2,%d3                          
    1eaa:  6506           	bcss 0x1eb2                           ; ↓ label_5
    1eac:  7020           	moveq #32,%d0                         
    1eae:  81aa 0004      	orl %d0,%a2@(4)                       

  label_5:
    1eb2:  4a05           	tstb %d5                              
    1eb4:  6608           	bnes 0x1ebe                           ; ↓ label_6
    1eb6:  4227           	clrb %sp@-                            
    1eb8:  4eba 051a      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    1ebc:  544f           	addqw #2,%sp                          

  label_6:
    1ebe:  4878 0064      	pea 0x64                              
    1ec2:  2f2a 000e      	movel %a2@(14),%sp@-                  
    1ec6:  4e4f           	trap #15                              
    1ec8:  a09a           	0120232                               ; sysTrapSysTimerWrite
    1eca:  504f           	addqw #8,%sp                          

  label_7:
    1ecc:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    1ed0:  4e5e           	unlk %fp                              
    1ed2:  4e75           	rts                                   
```

# PrvDoMgAuth: 0x1ee8 - 0x1f98

Parameters:

   * `%fp@(8)`      : ???

```
    1ee8:  4e56 0000      	linkw %fp,#0                          
    1eec:  2f0a           	movel %a2,%sp@-                       
    1eee:  2f03           	movel %d3,%sp@-                       
    1ef0:  246e 0008      	moveal %fp@(8),%a2                    
    1ef4:  4227           	clrb %sp@-                            
    1ef6:  4e4f           	trap #15                              
    1ef8:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    1efa:  7002           	moveq #2,%d0                          
    1efc:  c0aa 0008      	andl %a2@(8),%d0                      
    1f00:  544f           	addqw #2,%sp                          
    1f02:  6700 0084      	beqw 0x1f88                           ; ↓ label_5
    1f06:  202a 0004      	movel %a2@(4),%d0                     
    1f0a:  0280 0000 0100 	andil #256,%d0                        
    1f10:  6776           	beqs 0x1f88                           ; ↓ label_5
    1f12:  02aa fffe ffff 	andil #-65537,%a2@(4)                 
    1f18:  0004                                                  
    1f1a:  02aa fffd ffff 	andil #-131073,%a2@(4)                
    1f20:  0004                                                  
    1f22:  7600           	moveq #0,%d3                          

  label_0:
    1f24:  7001           	moveq #1,%d0                          
    1f26:  c0aa 0004      	andl %a2@(4),%d0                      
    1f2a:  660c           	bnes 0x1f38                           ; ↓ label_1
    1f2c:  4878 0014      	pea 0x14                              
    1f30:  4e4f           	trap #15                              
    1f32:  a249           	0121111                               ; sysTrapHwrDelay
    1f34:  584f           	addqw #4,%sp                          
    1f36:  6030           	bras 0x1f68                           ; ↓ label_2

  label_1:
    1f38:  3f2a 0024      	movew %a2@(36),%sp@-                  
    1f3c:  4e4f           	trap #15                              
    1f3e:  a805           	0124005                               ; ??? 0xa805
    1f40:  4a40           	tstw %d0                              
    1f42:  544f           	addqw #2,%sp                          
    1f44:  672a           	beqs 0x1f70                           ; ↓ label_3
    1f46:  4227           	clrb %sp@-                            
    1f48:  4eba 022c      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    1f4c:  4878 0014      	pea 0x14                              
    1f50:  4e4f           	trap #15                              
    1f52:  a249           	0121111                               ; sysTrapHwrDelay
    1f54:  1f3c 0001      	moveb #1,%sp@-                        
    1f58:  4eba 021c      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    1f5c:  4878 07d0      	pea 0x7d0                             
    1f60:  4e4f           	trap #15                              
    1f62:  a249           	0121111                               ; sysTrapHwrDelay
    1f64:  4fef 000c      	lea %sp@(12),%sp                      

  label_2:
    1f68:  5203           	addqb #1,%d3                          
    1f6a:  0c03 0003      	cmpib #3,%d3                          
    1f6e:  65b4           	bcss 0x1f24                           ; ↑ label_0

  label_3:
    1f70:  0c03 0003      	cmpib #3,%d3                          
    1f74:  640a           	bccs 0x1f80                           ; ↓ label_4
    1f76:  00aa 0001 0000 	oril #65536,%a2@(4)                   
    1f7c:  0004                                                  
    1f7e:  6008           	bras 0x1f88                           ; ↓ label_5

  label_4:
    1f80:  00aa 0002 0000 	oril #131072,%a2@(4)                  
    1f86:  0004                                                  

  label_5:
    1f88:  1f3c 0001      	moveb #1,%sp@-                        
    1f8c:  4e4f           	trap #15                              
    1f8e:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    1f90:  544f           	addqw #2,%sp                          
    1f92:  261f           	movel %sp@+,%d3                       
    1f94:  245f           	moveal %sp@+,%a2                      
    1f96:  4e5e           	unlk %fp                              
    1f98:  4e75           	rts                                   
```

# PrvSupplyPower: 0x1fa8 - 0x1fca

Parameters:

   * `%fp@(8)`      : ???

```
    1fa8:  4e56 0000      	linkw %fp,#0                          
    1fac:  1f3c 0001      	moveb #1,%sp@-                        
    1fb0:  4eba 01c4      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    1fb4:  206e 0008      	moveal %fp@(8),%a0                    
    1fb8:  00a8 0000 0080 	oril #128,%a0@(4)                     
    1fbe:  0004                                                  
    1fc0:  4878 07d0      	pea 0x7d0                             
    1fc4:  4e4f           	trap #15                              
    1fc6:  a249           	0121111                               ; sysTrapHwrDelay
    1fc8:  4e5e           	unlk %fp                              
    1fca:  4e75           	rts                                   
```

# __Startup__: 0x1fde - 0x2000

Parameters:

   * `%fp@(10)`     : ???

```
    1fde:  4e56 0000      	linkw %fp,#0                          
    1fe2:  2f03           	movel %d3,%sp@-                       
    1fe4:  4eba fa9c      	jsr %pc@(0x1a82)                      ; PrvShouldWeInstall
    1fe8:  3600           	movew %d0,%d3                         
    1fea:  6704           	beqs 0x1ff0                           ; ↓ label_0
    1fec:  3003           	movew %d3,%d0                         
    1fee:  600c           	bras 0x1ffc                           ; ↓ label_1

  label_0:
    1ff0:  4eba 001e      	jsr %pc@(0x2010)                      ; t
    1ff4:  226e 000a      	moveal %fp@(10),%a1                   
    1ff8:  2288           	movel %a0,%a1@                        
    1ffa:  7000           	moveq #0,%d0                          

  label_1:
    1ffc:  261f           	movel %sp@+,%d3                       
    1ffe:  4e5e           	unlk %fp                              
    2000:  4e75           	rts                                   
```

# t: 0x2010 - 0x2014

```
    2010:  41fa 0004      	lea %pc@(0x2016),%a0                  ; ??? 0x2016
    2014:  4e75           	rts                                   
```

# MSSlotHwrOpen: 0x20b2 - 0x2118

```
    20b2:  4e56 0000      	linkw %fp,#0                          
    20b6:  41fa 00fc      	lea %pc@(0x21b4),%a0                  ; MSSlotHwrInsIntHandler
    20ba:  2278 0304      	moveal 0x304,%a1                      
    20be:  2269 0018      	moveal %a1@(24),%a1                   
    20c2:  2348 0014      	movel %a0,%a1@(20)                    
    20c6:  41fa 012e      	lea %pc@(0x21f6),%a0                  ; MSSlotHwrRmvIntHandler
    20ca:  2278 0304      	moveal 0x304,%a1                      
    20ce:  2269 0018      	moveal %a1@(24),%a1                   
    20d2:  2348 0018      	movel %a0,%a1@(24)                    
    20d6:  41fa 0218      	lea %pc@(0x22f0),%a0                  ; SlotHandleXInt
    20da:  2278 0304      	moveal 0x304,%a1                      
    20de:  2269 0018      	moveal %a1@(24),%a1                   
    20e2:  2348 0024      	movel %a0,%a1@(36)                    
    20e6:  0038 0080 f443 	orib #-128,0xfffffffffffff443         
    20ec:  0038 0080 f440 	orib #-128,0xfffffffffffff440         
    20f2:  0238 007f f442 	andib #127,0xfffffffffffff442         
    20f8:  0038 0010 f413 	orib #16,0xfffffffffffff413           
    20fe:  0038 0010 f410 	orib #16,0xfffffffffffff410           
    2104:  0238 00ef f412 	andib #-17,0xfffffffffffff412         
    210a:  4227           	clrb %sp@-                            
    210c:  4eba 0068      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    2110:  4227           	clrb %sp@-                            
    2112:  4eba 02c0      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    2116:  4e5e           	unlk %fp                              
    2118:  4e75           	rts                                   
```

# MSSlotHwrClose: 0x212a - 0x2162

```
    212a:  4e56 0000      	linkw %fp,#0                          
    212e:  0279 fff3 1100 	andiw #-13,0x11000222                 
    2134:  0222                                                  
    2136:  2078 0304      	moveal 0x304,%a0                      
    213a:  2068 0018      	moveal %a0@(24),%a0                   
    213e:  42a8 0014      	clrl %a0@(20)                         
    2142:  2078 0304      	moveal 0x304,%a0                      
    2146:  2068 0018      	moveal %a0@(24),%a0                   
    214a:  42a8 0018      	clrl %a0@(24)                         
    214e:  2078 0304      	moveal 0x304,%a0                      
    2152:  2068 0018      	moveal %a0@(24),%a0                   
    2156:  42a8 0024      	clrl %a0@(36)                         
    215a:  4227           	clrb %sp@-                            
    215c:  4eba 0018      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    2160:  4e5e           	unlk %fp                              
    2162:  4e75           	rts                                   
```

# MSSlotHwrSupplyPower: 0x2176 - 0x219a

Parameters:

   * `%fp@(8)`      : ???

```
    2176:  4e56 0000      	linkw %fp,#0                          
    217a:  4a2e 0008      	tstb %fp@(8)                          
    217e:  6712           	beqs 0x2192                           ; ↓ label_0
    2180:  0038 0080 f441 	orib #-128,0xfffffffffffff441         
    2186:  4878 0bb8      	pea 0xbb8                             
    218a:  4e4f           	trap #15                              
    218c:  a249           	0121111                               ; sysTrapHwrDelay
    218e:  584f           	addqw #4,%sp                          
    2190:  6006           	bras 0x2198                           ; ↓ label_1

  label_0:
    2192:  0238 007f f441 	andib #127,0xfffffffffffff441         

  label_1:
    2198:  4e5e           	unlk %fp                              
    219a:  4e75           	rts                                   
```

# MSSlotHwrInsIntHandler: 0x21b4 - 0x21da

```
    21b4:  4e56 0000      	linkw %fp,#0                          
    21b8:  4878 0bb8      	pea 0xbb8                             
    21bc:  4e4f           	trap #15                              
    21be:  a249           	0121111                               ; sysTrapHwrDelay
    21c0:  3039 1100 0220 	movew 0x11000220,%d0                  
    21c6:  0240 0004      	andiw #4,%d0                          
    21ca:  584f           	addqw #4,%sp                          
    21cc:  670a           	beqs 0x21d8                           ; ↓ label_0
    21ce:  1f3c 0001      	moveb #1,%sp@-                        
    21d2:  4eba fa90      	jsr %pc@(0x1c64)                      ; PrvInsRmvIntHandler
    21d6:  544f           	addqw #2,%sp                          

  label_0:
    21d8:  4e5e           	unlk %fp                              
    21da:  4e75           	rts                                   
```

# MSSlotHwrRmvIntHandler: 0x21f6 - 0x2220

```
    21f6:  4e56 0000      	linkw %fp,#0                          
    21fa:  4878 0bb8      	pea 0xbb8                             
    21fe:  4e4f           	trap #15                              
    2200:  a249           	0121111                               ; sysTrapHwrDelay
    2202:  3039 1100 0220 	movew 0x11000220,%d0                  
    2208:  0240 0008      	andiw #8,%d0                          
    220c:  584f           	addqw #4,%sp                          
    220e:  6708           	beqs 0x2218                           ; ↓ label_0
    2210:  4227           	clrb %sp@-                            
    2212:  4eba fa50      	jsr %pc@(0x1c64)                      ; PrvInsRmvIntHandler
    2216:  544f           	addqw #2,%sp                          

  label_0:
    2218:  4227           	clrb %sp@-                            
    221a:  4eba 01b8      	jsr %pc@(0x23d4)                      ; MSSlotHwrLightAccessLED
    221e:  4e5e           	unlk %fp                              
    2220:  4e75           	rts                                   
```

# EnableIntFromIOStick: 0x223c - 0x229c

Parameters:

   * `%fp@(8)`      : ???

```
    223c:  4e56 0000      	linkw %fp,#0                          
    2240:  2f0b           	movel %a3,%sp@-                       
    2242:  2f0a           	movel %a2,%sp@-                       
    2244:  246e 0008      	moveal %fp@(8),%a2                    
    2248:  102a 002f      	moveb %a2@(47),%d0                    
    224c:  0240 0001      	andiw #1,%d0                          
    2250:  6744           	beqs 0x2296                           ; ↓ label_3
    2252:  7001           	moveq #1,%d0                          
    2254:  b0aa 0042      	cmpl %a2@(66),%d0                     
    2258:  6d38           	blts 0x2292                           ; ↓ label_2
    225a:  601c           	bras 0x2278                           ; ↓ label_1

  label_0:
    225c:  3039 1100 0406 	movew 0x11000406,%d0                  
    2262:  0240 0400      	andiw #1024,%d0                       
    2266:  672e           	beqs 0x2296                           ; ↓ label_3
    2268:  266a 0032      	moveal %a2@(50),%a3                   
    226c:  200b           	movel %a3,%d0                         
    226e:  6708           	beqs 0x2278                           ; ↓ label_1
    2270:  2f2a 0036      	movel %a2@(54),%sp@-                  
    2274:  4e93           	jsr %a3@                              
    2276:  584f           	addqw #4,%sp                          

  label_1:
    2278:  3039 1100 0406 	movew 0x11000406,%d0                  
    227e:  0240 4000      	andiw #16384,%d0                      
    2282:  66d8           	bnes 0x225c                           ; ↑ label_0
    2284:  0079 0002 1100 	oriw #2,0x11000222                    
    228a:  0222                                                  
    228c:  42aa 0042      	clrl %a2@(66)                         
    2290:  6004           	bras 0x2296                           ; ↓ label_3

  label_2:
    2292:  53aa 0042      	subql #1,%a2@(66)                     

  label_3:
    2296:  245f           	moveal %sp@+,%a2                      
    2298:  265f           	moveal %sp@+,%a3                      
    229a:  4e5e           	unlk %fp                              
    229c:  4e75           	rts                                   
```

# DisableIntFromIOStick: 0x22b6 - 0x22d6

Parameters:

   * `%fp@(8)`      : ???

```
    22b6:  4e56 0000      	linkw %fp,#0                          
    22ba:  206e 0008      	moveal %fp@(8),%a0                    
    22be:  1028 002f      	moveb %a0@(47),%d0                    
    22c2:  0240 0001      	andiw #1,%d0                          
    22c6:  670c           	beqs 0x22d4                           ; ↓ label_0
    22c8:  0279 fffd 1100 	andiw #-3,0x11000222                  
    22ce:  0222                                                  
    22d0:  52a8 0042      	addql #1,%a0@(66)                     

  label_0:
    22d4:  4e5e           	unlk %fp                              
    22d6:  4e75           	rts                                   
```

# SlotHandleXInt: 0x22f0 - 0x2360

```
    22f0:  4e56 0000      	linkw %fp,#0                          
    22f4:  2f0b           	movel %a3,%sp@-                       
    22f6:  2f0a           	movel %a2,%sp@-                       
    22f8:  2078 0304      	moveal 0x304,%a0                      
    22fc:  2468 0024      	moveal %a0@(36),%a2                   
    2300:  0279 fffd 1100 	andiw #-3,0x11000222                  
    2306:  0222                                                  
    2308:  52aa 0042      	addql #1,%a2@(66)                     
    230c:  102a 002f      	moveb %a2@(47),%d0                    
    2310:  0240 0001      	andiw #1,%d0                          
    2314:  672a           	beqs 0x2340                           ; ↓ label_2
    2316:  601c           	bras 0x2334                           ; ↓ label_1

  label_0:
    2318:  3039 1100 0406 	movew 0x11000406,%d0                  
    231e:  0240 0400      	andiw #1024,%d0                       
    2322:  6736           	beqs 0x235a                           ; ↓ label_4
    2324:  266a 0032      	moveal %a2@(50),%a3                   
    2328:  200b           	movel %a3,%d0                         
    232a:  6708           	beqs 0x2334                           ; ↓ label_1
    232c:  2f2a 0036      	movel %a2@(54),%sp@-                  
    2330:  4e93           	jsr %a3@                              
    2332:  584f           	addqw #4,%sp                          

  label_1:
    2334:  3039 1100 0406 	movew 0x11000406,%d0                  
    233a:  0240 4000      	andiw #16384,%d0                      
    233e:  66d8           	bnes 0x2318                           ; ↑ label_0

  label_2:
    2340:  7001           	moveq #1,%d0                          
    2342:  b0aa 0042      	cmpl %a2@(66),%d0                     
    2346:  6d0e           	blts 0x2356                           ; ↓ label_3
    2348:  0079 0002 1100 	oriw #2,0x11000222                    
    234e:  0222                                                  
    2350:  42aa 0042      	clrl %a2@(66)                         
    2354:  6004           	bras 0x235a                           ; ↓ label_4

  label_3:
    2356:  53aa 0042      	subql #1,%a2@(66)                     

  label_4:
    235a:  245f           	moveal %sp@+,%a2                      
    235c:  265f           	moveal %sp@+,%a3                      
    235e:  4e5e           	unlk %fp                              
    2360:  4e75           	rts                                   
```

# MSSlotHwrSetUpInterrupt: 0x2374 - 0x23b8

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    2374:  4e56 0000      	linkw %fp,#0                          
    2378:  0c2e 0001 0008 	cmpib #1,%fp@(8)                      
    237e:  661c           	bnes 0x239c                           ; ↓ label_1
    2380:  0c2e 0001 000a 	cmpib #1,%fp@(10)                     
    2386:  660a           	bnes 0x2392                           ; ↓ label_0
    2388:  0079 0004 1100 	oriw #4,0x11000222                    
    238e:  0222                                                  
    2390:  6024           	bras 0x23b6                           ; ↓ label_3

  label_0:
    2392:  0279 fffb 1100 	andiw #-5,0x11000222                  
    2398:  0222                                                  
    239a:  601a           	bras 0x23b6                           ; ↓ label_3

  label_1:
    239c:  0c2e 0001 000a 	cmpib #1,%fp@(10)                     
    23a2:  660a           	bnes 0x23ae                           ; ↓ label_2
    23a4:  0079 0008 1100 	oriw #8,0x11000222                    
    23aa:  0222                                                  
    23ac:  6008           	bras 0x23b6                           ; ↓ label_3

  label_2:
    23ae:  0279 fff7 1100 	andiw #-9,0x11000222                  
    23b4:  0222                                                  

  label_3:
    23b6:  4e5e           	unlk %fp                              
    23b8:  4e75           	rts                                   
```

# MSSlotHwrLightAccessLED: 0x23d4 - 0x23f6

Parameters:

   * `%fp@(8)`      : ???

```
    23d4:  4e56 0000      	linkw %fp,#0                          
    23d8:  1038 f411      	moveb 0xfffffffffffff411,%d0          
    23dc:  0200 0010      	andib #16,%d0                         
    23e0:  4a2e 0008      	tstb %fp@(8)                          
    23e4:  6708           	beqs 0x23ee                           ; ↓ label_0
    23e6:  0038 0010 f411 	orib #16,0xfffffffffffff411           
    23ec:  6006           	bras 0x23f4                           ; ↓ label_1

  label_0:
    23ee:  0238 00ef f411 	andib #-17,0xfffffffffffff411         

  label_1:
    23f4:  4e5e           	unlk %fp                              
    23f6:  4e75           	rts                                   
```

# PrvDspDoneProc: 0x2412 - 0x2462

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    2412:  4e56 0000      	linkw %fp,#0                          
    2416:  202e 0008      	movel %fp@(8),%d0                     
    241a:  4a2e 000c      	tstb %fp@(12)                         
    241e:  673a           	beqs 0x245a                           ; ↓ label_0
    2420:  2040           	moveal %d0,%a0                        
    2422:  30b9 1100 0c06 	movew 0x11000c06,%a0@                 
    2428:  3179 1100 0c14 	movew 0x11000c14,%a0@(2)              
    242e:  0002                                                  
    2430:  3179 1100 0c16 	movew 0x11000c16,%a0@(4)              
    2436:  0004                                                  
    2438:  3179 1100 0c18 	movew 0x11000c18,%a0@(6)              
    243e:  0006                                                  
    2440:  3179 1100 0c1a 	movew 0x11000c1a,%a0@(8)              
    2446:  0008                                                  
    2448:  3179 1100 0c1c 	movew 0x11000c1c,%a0@(10)             
    244e:  000a                                                  
    2450:  3179 1100 0c1e 	movew 0x11000c1e,%a0@(12)             
    2456:  000c                                                  
    2458:  6006           	bras 0x2460                           ; ↓ label_1

  label_0:
    245a:  2040           	moveal %d0,%a0                        
    245c:  30bc 03ff      	movew #1023,%a0@                      

  label_1:
    2460:  4e5e           	unlk %fp                              
    2462:  4e75           	rts                                   
```

# PrvJobMount: 0x2476 - 0x25ba

Parameters:

   * `%fp@(8)`      : ???

```
    2476:  4e56 0000      	linkw %fp,#0                          
    247a:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    247e:  266e 0008      	moveal %fp@(8),%a3                    
    2482:  42ab 0008      	clrl %a3@(8)                          
    2486:  2f0b           	movel %a3,%sp@-                       
    2488:  4eba 06ee      	jsr %pc@(0x2b78)                      ; PrvJobDetectBootBlock
    248c:  2600           	movel %d0,%d3                         
    248e:  584f           	addqw #4,%sp                          
    2490:  6706           	beqs 0x2498                           ; ↓ label_0
    2492:  2003           	movel %d3,%d0                         
    2494:  6000 011e      	braw 0x25b4                           ; ↓ label_16

  label_0:
    2498:  246b 0020      	moveal %a3@(32),%a2                   
    249c:  45ea 2400      	lea %a2@(9216),%a2                    
    24a0:  260a           	movel %a2,%d3                         
    24a2:  2043           	moveal %d3,%a0                        
    24a4:  0c68 1001 01c8 	cmpiw #4097,%a0@(456)                 
    24aa:  6620           	bnes 0x24cc                           ; ↓ label_2
    24ac:  2638 0304      	movel 0x304,%d3                       
    24b0:  2078 0304      	moveal 0x304,%a0                      
    24b4:  4aa8 0010      	tstl %a0@(16)                         
    24b8:  670c           	beqs 0x24c6                           ; ↓ label_1
    24ba:  2043           	moveal %d3,%a0                        
    24bc:  2068 0010      	moveal %a0@(16),%a0                   
    24c0:  7004           	moveq #4,%d0                          
    24c2:  81a8 001c      	orl %d0,%a0@(28)                      

  label_1:
    24c6:  7002           	moveq #2,%d0                          
    24c8:  81ab 0008      	orl %d0,%a3@(8)                       

  label_2:
    24cc:  302a 0002      	movew %a2@(2),%d0                     
    24d0:  0240 ff00      	andiw #-256,%d0                       
    24d4:  0c40 0100      	cmpiw #256,%d0                        
    24d8:  6706           	beqs 0x24e0                           ; ↓ label_3
    24da:  7001           	moveq #1,%d0                          
    24dc:  6000 00d6      	braw 0x25b4                           ; ↓ label_16

  label_3:
    24e0:  0c2a 0001 01a0 	cmpib #1,%a2@(416)                    
    24e6:  6706           	beqs 0x24ee                           ; ↓ label_4
    24e8:  7001           	moveq #1,%d0                          
    24ea:  6000 00c8      	braw 0x25b4                           ; ↓ label_16

  label_4:
    24ee:  0c2a 0002 01a1 	cmpib #2,%a2@(417)                    
    24f4:  6706           	beqs 0x24fc                           ; ↓ label_5
    24f6:  7001           	moveq #1,%d0                          
    24f8:  6000 00ba      	braw 0x25b4                           ; ↓ label_16

  label_5:
    24fc:  302a 01a2      	movew %a2@(418),%d0                   
    2500:  5140           	subqw #8,%d0                          
    2502:  6706           	beqs 0x250a                           ; ↓ label_6
    2504:  5140           	subqw #8,%d0                          
    2506:  670a           	beqs 0x2512                           ; ↓ label_7
    2508:  6010           	bras 0x251a                           ; ↓ label_8

  label_6:
    250a:  377c 0010 0014 	movew #16,%a3@(20)                    
    2510:  600e           	bras 0x2520                           ; ↓ label_9

  label_7:
    2512:  377c 0020 0014 	movew #32,%a3@(20)                    
    2518:  6006           	bras 0x2520                           ; ↓ label_9

  label_8:
    251a:  7001           	moveq #1,%d0                          
    251c:  6000 0096      	braw 0x25b4                           ; ↓ label_16

  label_9:
    2520:  302a 01a4      	movew %a2@(420),%d0                   
    2524:  0440 0200      	subiw #512,%d0                        
    2528:  671c           	beqs 0x2546                           ; ↓ label_10
    252a:  0440 0200      	subiw #512,%d0                        
    252e:  6716           	beqs 0x2546                           ; ↓ label_10
    2530:  0440 0400      	subiw #1024,%d0                       
    2534:  6710           	beqs 0x2546                           ; ↓ label_10
    2536:  0440 0800      	subiw #2048,%d0                       
    253a:  670a           	beqs 0x2546                           ; ↓ label_10
    253c:  0440 1000      	subiw #4096,%d0                       
    2540:  6704           	beqs 0x2546                           ; ↓ label_10
    2542:  7001           	moveq #1,%d0                          
    2544:  606e           	bras 0x25b4                           ; ↓ label_16

  label_10:
    2546:  302a 01a6      	movew %a2@(422),%d0                   
    254a:  0440 01f0      	subiw #496,%d0                        
    254e:  671c           	beqs 0x256c                           ; ↓ label_11
    2550:  0440 01f0      	subiw #496,%d0                        
    2554:  6716           	beqs 0x256c                           ; ↓ label_11
    2556:  0440 03e0      	subiw #992,%d0                        
    255a:  6710           	beqs 0x256c                           ; ↓ label_11
    255c:  0440 07c0      	subiw #1984,%d0                       
    2560:  670a           	beqs 0x256c                           ; ↓ label_11
    2562:  0440 0f80      	subiw #3968,%d0                       
    2566:  6704           	beqs 0x256c                           ; ↓ label_11
    2568:  7001           	moveq #1,%d0                          
    256a:  6048           	bras 0x25b4                           ; ↓ label_16

  label_11:
    256c:  0c2a 0001 01d6 	cmpib #1,%a2@(470)                    
    2572:  6704           	beqs 0x2578                           ; ↓ label_12
    2574:  7001           	moveq #1,%d0                          
    2576:  603c           	bras 0x25b4                           ; ↓ label_16

  label_12:
    2578:  0c2a 0001 0178 	cmpib #1,%a2@(376)                    
    257e:  6704           	beqs 0x2584                           ; ↓ label_13
    2580:  7001           	moveq #1,%d0                          
    2582:  6030           	bras 0x25b4                           ; ↓ label_16

  label_13:
    2584:  2f0b           	movel %a3,%sp@-                       
    2586:  4eba 083c      	jsr %pc@(0x2dc4)                      ; PrvJobMakeAddressTable
    258a:  2600           	movel %d0,%d3                         
    258c:  584f           	addqw #4,%sp                          
    258e:  6704           	beqs 0x2594                           ; ↓ label_14
    2590:  2003           	movel %d3,%d0                         
    2592:  6020           	bras 0x25b4                           ; ↓ label_16

  label_14:
    2594:  2478 0304      	moveal 0x304,%a2                      
    2598:  2078 0304      	moveal 0x304,%a0                      
    259c:  4aa8 0010      	tstl %a0@(16)                         
    25a0:  670a           	beqs 0x25ac                           ; ↓ label_15
    25a2:  206a 0010      	moveal %a2@(16),%a0                   
    25a6:  7002           	moveq #2,%d0                          
    25a8:  81a8 001c      	orl %d0,%a0@(28)                      

  label_15:
    25ac:  7001           	moveq #1,%d0                          
    25ae:  81ab 0008      	orl %d0,%a3@(8)                       
    25b2:  7000           	moveq #0,%d0                          

  label_16:
    25b4:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    25b8:  4e5e           	unlk %fp                              
    25ba:  4e75           	rts                                   
```

# PrvJobReadSector: 0x25ca - 0x26f8

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
    25ca:  4e56 ffd8      	linkw %fp,#-40                        
    25ce:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    25d2:  246e 0008      	moveal %fp@(8),%a2                    
    25d6:  41fa 1c1e      	lea %pc@(0x41f6),%a0                  ; ??? 0x41f6
    25da:  2d48 fff0      	movel %a0,%fp@(-16)                   
    25de:  266e 0010      	moveal %fp@(16),%a3                   
    25e2:  206e 0014      	moveal %fp@(20),%a0                   
    25e6:  2d50 ffe8      	movel %a0@,%fp@(-24)                  
    25ea:  2d50 ffec      	movel %a0@,%fp@(-20)                  
    25ee:  3e2a 0014      	movew %a2@(20),%d7                    
    25f2:  7000           	moveq #0,%d0                          
    25f4:  3007           	movew %d7,%d0                         
    25f6:  2c00           	movel %d0,%d6                         
    25f8:  2200           	movel %d0,%d1                         
    25fa:  202e 000c      	movel %fp@(12),%d0                    
    25fe:  4eba 1c58      	jsr %pc@(0x4258)                      ; anonymous_function_0
    2602:  3800           	movew %d0,%d4                         
    2604:  202e 000c      	movel %fp@(12),%d0                    
    2608:  2206           	movel %d6,%d1                         
    260a:  4eba 1ca4      	jsr %pc@(0x42b0)                      ; anonymous_function_1
    260e:  2d40 ffe4      	movel %d0,%fp@(-28)                   
    2612:  1a00           	moveb %d0,%d5                         
    2614:  7600           	moveq #0,%d3                          
    2616:  162e ffe7      	moveb %fp@(-25),%d3                   
    261a:  48c3           	extl %d3                              
    261c:  d6ae ffe8      	addl %fp@(-24),%d3                    
    2620:  5383           	subql #1,%d3                          
    2622:  2d43 ffe0      	movel %d3,%fp@(-32)                   
    2626:  7000           	moveq #0,%d0                          
    2628:  102e ffe3      	moveb %fp@(-29),%d0                   
    262c:  b047           	cmpw %d7,%d0                          
    262e:  6506           	bcss 0x2636                           ; ↓ label_0
    2630:  1607           	moveb %d7,%d3                         
    2632:  0603 ffff      	addib #-1,%d3                         

  label_0:
    2636:  202e 000c      	movel %fp@(12),%d0                    
    263a:  d0ae ffec      	addl %fp@(-20),%d0                    
    263e:  5380           	subql #1,%d0                          
    2640:  2d40 ffd8      	movel %d0,%fp@(-40)                   
    2644:  2206           	movel %d6,%d1                         
    2646:  4eba 1c10      	jsr %pc@(0x4258)                      ; anonymous_function_0
    264a:  2d40 ffdc      	movel %d0,%fp@(-36)                   
    264e:  3e00           	movew %d0,%d7                         
    2650:  206e fff0      	moveal %fp@(-16),%a0                  
    2654:  3028 0022      	movew %a0@(34),%d0                    
    2658:  b06e ffde      	cmpw %fp@(-34),%d0                    
    265c:  620c           	bhis 0x266a                           ; ↓ label_1
    265e:  206e 0014      	moveal %fp@(20),%a0                   
    2662:  4290           	clrl %a0@                             
    2664:  7004           	moveq #4,%d0                          
    2666:  6000 008a      	braw 0x26f2                           ; ↓ label_6

  label_1:
    266a:  7c00           	moveq #0,%d6                          
    266c:  3c04           	movew %d4,%d6                         
    266e:  dc86           	addl %d6,%d6                          
    2670:  2d6e ffd8 fff4 	movel %fp@(-40),%fp@(-12)             

  label_2:
    2676:  70ff           	moveq #-1,%d0                         
    2678:  2d40 fff8      	movel %d0,%fp@(-8)                    
    267c:  486e fff8      	pea %fp@(-8)                          
    2680:  2f0b           	movel %a3,%sp@-                       
    2682:  1f03           	moveb %d3,%sp@-                       
    2684:  1f05           	moveb %d5,%sp@-                       
    2686:  206a 001c      	moveal %a2@(28),%a0                   
    268a:  3f30 6800      	movew %a0@(00000000,%d6:l),%sp@-      
    268e:  2f0a           	movel %a2,%sp@-                       
    2690:  4eba 0c86      	jsr %pc@(0x3318)                      ; PrvJobReadPBlock
    2694:  2d40 fffc      	movel %d0,%fp@(-4)                    
    2698:  4aae fffc      	tstl %fp@(-4)                         
    269c:  4fef 0012      	lea %sp@(18),%sp                      
    26a0:  670c           	beqs 0x26ae                           ; ↓ label_3
    26a2:  206e 0014      	moveal %fp@(20),%a0                   
    26a6:  4290           	clrl %a0@                             
    26a8:  202e fffc      	movel %fp@(-4),%d0                    
    26ac:  6044           	bras 0x26f2                           ; ↓ label_6

  label_3:
    26ae:  5486           	addql #2,%d6                          
    26b0:  5244           	addqw #1,%d4                          
    26b2:  b847           	cmpw %d7,%d4                          
    26b4:  6304           	blss 0x26ba                           ; ↓ label_4
    26b6:  7000           	moveq #0,%d0                          
    26b8:  6038           	bras 0x26f2                           ; ↓ label_6

  label_4:
    26ba:  7000           	moveq #0,%d0                          
    26bc:  1005           	moveb %d5,%d0                         
    26be:  7200           	moveq #0,%d1                          
    26c0:  1203           	moveb %d3,%d1                         
    26c2:  9240           	subw %d0,%d1                          
    26c4:  5241           	addqw #1,%d1                          
    26c6:  7009           	moveq #9,%d0                          
    26c8:  e169           	lslw %d0,%d1                          
    26ca:  48c1           	extl %d1                              
    26cc:  d7c1           	addal %d1,%a3                         
    26ce:  7a00           	moveq #0,%d5                          
    26d0:  b847           	cmpw %d7,%d4                          
    26d2:  6614           	bnes 0x26e8                           ; ↓ label_5
    26d4:  7000           	moveq #0,%d0                          
    26d6:  302a 0014      	movew %a2@(20),%d0                    
    26da:  2200           	movel %d0,%d1                         
    26dc:  202e fff4      	movel %fp@(-12),%d0                   
    26e0:  4eba 1bce      	jsr %pc@(0x42b0)                      ; anonymous_function_1
    26e4:  1600           	moveb %d0,%d3                         
    26e6:  608e           	bras 0x2676                           ; ↑ label_2

  label_5:
    26e8:  162a 0015      	moveb %a2@(21),%d3                    
    26ec:  0603 ffff      	addib #-1,%d3                         
    26f0:  6084           	bras 0x2676                           ; ↑ label_2

  label_6:
    26f2:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    26f6:  4e5e           	unlk %fp                              
    26f8:  4e75           	rts                                   
```

# PrvJobWriteSector: 0x270e - 0x281a

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
    270e:  4e56 ffe0      	linkw %fp,#-32                        
    2712:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2716:  246e 0008      	moveal %fp@(8),%a2                    
    271a:  41fa 1ada      	lea %pc@(0x41f6),%a0                  ; ??? 0x41f6
    271e:  2d48 fff8      	movel %a0,%fp@(-8)                    
    2722:  266e 0010      	moveal %fp@(16),%a3                   
    2726:  206e 0014      	moveal %fp@(20),%a0                   
    272a:  2d50 fff0      	movel %a0@,%fp@(-16)                  
    272e:  2d50 fff4      	movel %a0@,%fp@(-12)                  
    2732:  3c2a 0014      	movew %a2@(20),%d6                    
    2736:  7000           	moveq #0,%d0                          
    2738:  3006           	movew %d6,%d0                         
    273a:  2e00           	movel %d0,%d7                         
    273c:  2200           	movel %d0,%d1                         
    273e:  202e 000c      	movel %fp@(12),%d0                    
    2742:  4eba 1b14      	jsr %pc@(0x4258)                      ; anonymous_function_0
    2746:  3600           	movew %d0,%d3                         
    2748:  202e 000c      	movel %fp@(12),%d0                    
    274c:  2207           	movel %d7,%d1                         
    274e:  4eba 1b60      	jsr %pc@(0x42b0)                      ; anonymous_function_1
    2752:  2d40 ffec      	movel %d0,%fp@(-20)                   
    2756:  1a00           	moveb %d0,%d5                         
    2758:  7800           	moveq #0,%d4                          
    275a:  182e ffef      	moveb %fp@(-17),%d4                   
    275e:  48c4           	extl %d4                              
    2760:  d8ae fff0      	addl %fp@(-16),%d4                    
    2764:  5384           	subql #1,%d4                          
    2766:  2d44 ffe8      	movel %d4,%fp@(-24)                   
    276a:  7000           	moveq #0,%d0                          
    276c:  102e ffeb      	moveb %fp@(-21),%d0                   
    2770:  b046           	cmpw %d6,%d0                          
    2772:  6506           	bcss 0x277a                           ; ↓ label_0
    2774:  1806           	moveb %d6,%d4                         
    2776:  0604 ffff      	addib #-1,%d4                         

  label_0:
    277a:  202e 000c      	movel %fp@(12),%d0                    
    277e:  d0ae fff4      	addl %fp@(-12),%d0                    
    2782:  5380           	subql #1,%d0                          
    2784:  2d40 ffe0      	movel %d0,%fp@(-32)                   
    2788:  2207           	movel %d7,%d1                         
    278a:  4eba 1acc      	jsr %pc@(0x4258)                      ; anonymous_function_0
    278e:  2d40 ffe4      	movel %d0,%fp@(-28)                   
    2792:  3c00           	movew %d0,%d6                         
    2794:  206e fff8      	moveal %fp@(-8),%a0                   
    2798:  3028 0022      	movew %a0@(34),%d0                    
    279c:  b06e ffe6      	cmpw %fp@(-26),%d0                    
    27a0:  620a           	bhis 0x27ac                           ; ↓ label_1
    27a2:  206e 0014      	moveal %fp@(20),%a0                   
    27a6:  4290           	clrl %a0@                             
    27a8:  7004           	moveq #4,%d0                          
    27aa:  6068           	bras 0x2814                           ; ↓ label_6

  label_1:
    27ac:  2d6e ffe0 fffc 	movel %fp@(-32),%fp@(-4)              

  label_2:
    27b2:  2f0b           	movel %a3,%sp@-                       
    27b4:  1f04           	moveb %d4,%sp@-                       
    27b6:  1f05           	moveb %d5,%sp@-                       
    27b8:  3f03           	movew %d3,%sp@-                       
    27ba:  2f0a           	movel %a2,%sp@-                       
    27bc:  4eba 10ae      	jsr %pc@(0x386c)                      ; PrvJobMoveWritePBlock
    27c0:  2e00           	movel %d0,%d7                         
    27c2:  4fef 000e      	lea %sp@(14),%sp                      
    27c6:  670a           	beqs 0x27d2                           ; ↓ label_3
    27c8:  206e 0014      	moveal %fp@(20),%a0                   
    27cc:  4290           	clrl %a0@                             
    27ce:  2007           	movel %d7,%d0                         
    27d0:  6042           	bras 0x2814                           ; ↓ label_6

  label_3:
    27d2:  5243           	addqw #1,%d3                          
    27d4:  b646           	cmpw %d6,%d3                          
    27d6:  6304           	blss 0x27dc                           ; ↓ label_4
    27d8:  7000           	moveq #0,%d0                          
    27da:  6038           	bras 0x2814                           ; ↓ label_6

  label_4:
    27dc:  7000           	moveq #0,%d0                          
    27de:  1005           	moveb %d5,%d0                         
    27e0:  7200           	moveq #0,%d1                          
    27e2:  1204           	moveb %d4,%d1                         
    27e4:  9240           	subw %d0,%d1                          
    27e6:  5241           	addqw #1,%d1                          
    27e8:  7009           	moveq #9,%d0                          
    27ea:  e169           	lslw %d0,%d1                          
    27ec:  48c1           	extl %d1                              
    27ee:  d7c1           	addal %d1,%a3                         
    27f0:  7a00           	moveq #0,%d5                          
    27f2:  b646           	cmpw %d6,%d3                          
    27f4:  6614           	bnes 0x280a                           ; ↓ label_5
    27f6:  7000           	moveq #0,%d0                          
    27f8:  302a 0014      	movew %a2@(20),%d0                    
    27fc:  2200           	movel %d0,%d1                         
    27fe:  202e fffc      	movel %fp@(-4),%d0                    
    2802:  4eba 1aac      	jsr %pc@(0x42b0)                      ; anonymous_function_1
    2806:  1800           	moveb %d0,%d4                         
    2808:  60a8           	bras 0x27b2                           ; ↑ label_2

  label_5:
    280a:  182a 0015      	moveb %a2@(21),%d4                    
    280e:  0604 ffff      	addib #-1,%d4                         
    2812:  609e           	bras 0x27b2                           ; ↑ label_2

  label_6:
    2814:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2818:  4e5e           	unlk %fp                              
    281a:  4e75           	rts                                   
```

# PrvJobFormat: 0x2830 - 0x2b66

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-18)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-22)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-26)`    : ???
   * `%fp@(-28)`    : ???
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
    2830:  4e56 ff38      	linkw %fp,#-200                       
    2834:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2838:  266e 0008      	moveal %fp@(8),%a3                    
    283c:  0c6b ffff 0016 	cmpiw #-1,%a3@(22)                    
    2842:  6606           	bnes 0x284a                           ; ↓ label_0
    2844:  7001           	moveq #1,%d0                          
    2846:  6000 0318      	braw 0x2b60                           ; ↓ label_21

  label_0:
    284a:  382b 0016      	movew %a3@(22),%d4                    
    284e:  5244           	addqw #1,%d4                          
    2850:  3a04           	movew %d4,%d5                         
    2852:  3605           	movew %d5,%d3                         
    2854:  0c45 0010      	cmpiw #16,%d5                         
    2858:  6400 0086      	bccw 0x28e0                           ; ↓ label_4

  label_1:
    285c:  3d7c 3601 ff96 	movew #13825,%fp@(-106)               
    2862:  3d43 ff98      	movew %d3,%fp@(-104)                  
    2866:  426e ff9a      	clrw %fp@(-102)                       
    286a:  426e ffa8      	clrw %fp@(-88)                        
    286e:  1d7c 0001 ffba 	moveb #1,%fp@(-70)                    
    2874:  41fa fb9c      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    2878:  2d48 ffbc      	movel %a0,%fp@(-68)                   
    287c:  41ee ffc4      	lea %fp@(-60),%a0                     
    2880:  2d48 ffc0      	movel %a0,%fp@(-64)                   
    2884:  426e ffc4      	clrw %fp@(-60)                        
    2888:  486e ff96      	pea %fp@(-106)                        
    288c:  2078 0304      	moveal 0x304,%a0                      
    2890:  2068 0018      	moveal %a0@(24),%a0                   
    2894:  2068 009a      	moveal %a0@(154),%a0                  
    2898:  4e90           	jsr %a0@                              
    289a:  302e ffc4      	movew %fp@(-60),%d0                   
    289e:  0240 03ff      	andiw #1023,%d0                       
    28a2:  584f           	addqw #4,%sp                          
    28a4:  670e           	beqs 0x28b4                           ; ↓ label_2
    28a6:  7001           	moveq #1,%d0                          
    28a8:  c0ab 0004      	andl %a3@(4),%d0                      
    28ac:  6628           	bnes 0x28d6                           ; ↓ label_3
    28ae:  7002           	moveq #2,%d0                          
    28b0:  6000 02ae      	braw 0x2b60                           ; ↓ label_21

  label_2:
    28b4:  41ee ffc4      	lea %fp@(-60),%a0                     
    28b8:  5488           	addql #2,%a0                          
    28ba:  2448           	moveal %a0,%a2                        
    28bc:  3010           	movew %a0@,%d0                        
    28be:  0240 8000      	andiw #-32768,%d0                     
    28c2:  6712           	beqs 0x28d6                           ; ↓ label_3
    28c4:  3012           	movew %a2@,%d0                        
    28c6:  0240 0004      	andiw #4,%d0                          
    28ca:  660a           	bnes 0x28d6                           ; ↓ label_3
    28cc:  3743 0018      	movew %d3,%a3@(24)                    
    28d0:  3803           	movew %d3,%d4                         
    28d2:  5244           	addqw #1,%d4                          
    28d4:  600a           	bras 0x28e0                           ; ↓ label_4

  label_3:
    28d6:  5243           	addqw #1,%d3                          
    28d8:  0c43 0010      	cmpiw #16,%d3                         
    28dc:  6500 ff7e      	bcsw 0x285c                           ; ↑ label_1

  label_4:
    28e0:  4e4f           	trap #15                              
    28e2:  a130           	0120460                               ; sysTrapEvtResetAutoOffTimer
    28e4:  3a04           	movew %d4,%d5                         
    28e6:  7e00           	moveq #0,%d7                          
    28e8:  7600           	moveq #0,%d3                          
    28ea:  283c 1100 8000 	movel #285245440,%d4                  

  label_5:
    28f0:  2044           	moveal %d4,%a0                        
    28f2:  70ff           	moveq #-1,%d0                         
    28f4:  2080           	movel %d0,%a0@                        
    28f6:  2003           	movel %d3,%d0                         
    28f8:  5280           	addql #1,%d0                          
    28fa:  e588           	lsll #2,%d0                           
    28fc:  2040           	moveal %d0,%a0                        
    28fe:  203c 1100 8000 	movel #285245440,%d0                  
    2904:  72ff           	moveq #-1,%d1                         
    2906:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    290a:  2003           	movel %d3,%d0                         
    290c:  5480           	addql #2,%d0                          
    290e:  e588           	lsll #2,%d0                           
    2910:  2040           	moveal %d0,%a0                        
    2912:  203c 1100 8000 	movel #285245440,%d0                  
    2918:  72ff           	moveq #-1,%d1                         
    291a:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    291e:  2003           	movel %d3,%d0                         
    2920:  5680           	addql #3,%d0                          
    2922:  e588           	lsll #2,%d0                           
    2924:  2040           	moveal %d0,%a0                        
    2926:  203c 1100 8000 	movel #285245440,%d0                  
    292c:  72ff           	moveq #-1,%d1                         
    292e:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    2932:  2003           	movel %d3,%d0                         
    2934:  5880           	addql #4,%d0                          
    2936:  e588           	lsll #2,%d0                           
    2938:  2040           	moveal %d0,%a0                        
    293a:  203c 1100 8000 	movel #285245440,%d0                  
    2940:  72ff           	moveq #-1,%d1                         
    2942:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    2946:  2003           	movel %d3,%d0                         
    2948:  5a80           	addql #5,%d0                          
    294a:  e588           	lsll #2,%d0                           
    294c:  2040           	moveal %d0,%a0                        
    294e:  203c 1100 8000 	movel #285245440,%d0                  
    2954:  72ff           	moveq #-1,%d1                         
    2956:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    295a:  2003           	movel %d3,%d0                         
    295c:  5c80           	addql #6,%d0                          
    295e:  e588           	lsll #2,%d0                           
    2960:  2040           	moveal %d0,%a0                        
    2962:  203c 1100 8000 	movel #285245440,%d0                  
    2968:  72ff           	moveq #-1,%d1                         
    296a:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    296e:  2003           	movel %d3,%d0                         
    2970:  5e80           	addql #7,%d0                          
    2972:  e588           	lsll #2,%d0                           
    2974:  2040           	moveal %d0,%a0                        
    2976:  203c 1100 8000 	movel #285245440,%d0                  
    297c:  72ff           	moveq #-1,%d1                         
    297e:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    2982:  7020           	moveq #32,%d0                         
    2984:  d880           	addl %d0,%d4                          
    2986:  5083           	addql #8,%d3                          
    2988:  0c83 0000 0080 	cmpil #128,%d3                        
    298e:  6500 ff60      	bcsw 0x28f0                           ; ↑ label_5
    2992:  6000 01b4      	braw 0x2b48                           ; ↓ label_20

  label_6:
    2996:  7800           	moveq #0,%d4                          
    2998:  7600           	moveq #0,%d3                          
    299a:  3d6b 001a ff38 	movew %a3@(26),%fp@(-200)             
    29a0:  3c2b 001a      	movew %a3@(26),%d6                    
    29a4:  600e           	bras 0x29b4                           ; ↓ label_8

  label_7:
    29a6:  204a           	moveal %a2,%a0                        
    29a8:  d1c3           	addal %d3,%a0                         
    29aa:  ba68 2600      	cmpw %a0@(9728),%d5                   
    29ae:  6708           	beqs 0x29b8                           ; ↓ label_9
    29b0:  5483           	addql #2,%d3                          
    29b2:  5244           	addqw #1,%d4                          

  label_8:
    29b4:  b846           	cmpw %d6,%d4                          
    29b6:  65ee           	bcss 0x29a6                           ; ↑ label_7

  label_9:
    29b8:  b86e ff38      	cmpw %fp@(-200),%d4                   
    29bc:  6500 0188      	bcsw 0x2b46                           ; ↓ label_19
    29c0:  3d7c 3601 ff68 	movew #13825,%fp@(-152)               
    29c6:  3d45 ff6a      	movew %d5,%fp@(-150)                  
    29ca:  426e ff6c      	clrw %fp@(-148)                       
    29ce:  426e ff7a      	clrw %fp@(-134)                       
    29d2:  1d7c 0001 ff8c 	moveb #1,%fp@(-116)                   
    29d8:  41fa fa38      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    29dc:  2d48 ff8e      	movel %a0,%fp@(-114)                  
    29e0:  41ee ffc4      	lea %fp@(-60),%a0                     
    29e4:  2d48 ff92      	movel %a0,%fp@(-110)                  
    29e8:  426e ffc4      	clrw %fp@(-60)                        
    29ec:  486e ff68      	pea %fp@(-152)                        
    29f0:  2078 0304      	moveal 0x304,%a0                      
    29f4:  2068 0018      	moveal %a0@(24),%a0                   
    29f8:  2068 009a      	moveal %a0@(154),%a0                  
    29fc:  4e90           	jsr %a0@                              
    29fe:  302e ffc4      	movew %fp@(-60),%d0                   
    2a02:  0240 03ff      	andiw #1023,%d0                       
    2a06:  584f           	addqw #4,%sp                          
    2a08:  6712           	beqs 0x2a1c                           ; ↓ label_12
    2a0a:  7001           	moveq #1,%d0                          
    2a0c:  c0ab 0004      	andl %a3@(4),%d0                      
    2a10:  6704           	beqs 0x2a16                           ; ↓ label_10
    2a12:  7003           	moveq #3,%d0                          
    2a14:  6002           	bras 0x2a18                           ; ↓ label_11

  label_10:
    2a16:  7002           	moveq #2,%d0                          

  label_11:
    2a18:  6000 0146      	braw 0x2b60                           ; ↓ label_21

  label_12:
    2a1c:  302e ffc6      	movew %fp@(-58),%d0                   
    2a20:  0240 8000      	andiw #-32768,%d0                     
    2a24:  6700 0120      	beqw 0x2b46                           ; ↓ label_19
    2a28:  3d7c 3201 ff3a 	movew #12801,%fp@(-198)               
    2a2e:  3d45 ff3c      	movew %d5,%fp@(-196)                  
    2a32:  426e ff4c      	clrw %fp@(-180)                       
    2a36:  1d7c 0001 ff5e 	moveb #1,%fp@(-162)                   
    2a3c:  41fa f9d4      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    2a40:  2d48 ff60      	movel %a0,%fp@(-160)                  
    2a44:  41ee ffc4      	lea %fp@(-60),%a0                     
    2a48:  2d48 ff64      	movel %a0,%fp@(-156)                  
    2a4c:  426e ffc4      	clrw %fp@(-60)                        
    2a50:  486e ff3a      	pea %fp@(-198)                        
    2a54:  2078 0304      	moveal 0x304,%a0                      
    2a58:  2068 0018      	moveal %a0@(24),%a0                   
    2a5c:  2068 009a      	moveal %a0@(154),%a0                  
    2a60:  4e90           	jsr %a0@                              
    2a62:  302e ffc4      	movew %fp@(-60),%d0                   
    2a66:  0240 03ff      	andiw #1023,%d0                       
    2a6a:  584f           	addqw #4,%sp                          
    2a6c:  6722           	beqs 0x2a90                           ; ↓ label_14
    2a6e:  7001           	moveq #1,%d0                          
    2a70:  c0ab 0004      	andl %a3@(4),%d0                      
    2a74:  6606           	bnes 0x2a7c                           ; ↓ label_13
    2a76:  7002           	moveq #2,%d0                          
    2a78:  6000 00e6      	braw 0x2b60                           ; ↓ label_21

  label_13:
    2a7c:  302e ffc4      	movew %fp@(-60),%d0                   
    2a80:  0240 03ff      	andiw #1023,%d0                       
    2a84:  5740           	subqw #3,%d0                          
    2a86:  6600 00be      	bnew 0x2b46                           ; ↓ label_19
    2a8a:  7006           	moveq #6,%d0                          
    2a8c:  6000 00d2      	braw 0x2b60                           ; ↓ label_21

  label_14:
    2a90:  3605           	movew %d5,%d3                         
    2a92:  7209           	moveq #9,%d1                          
    2a94:  e26b           	lsrw %d1,%d3                          
    2a96:  3005           	movew %d5,%d0                         
    2a98:  0240 01ff      	andiw #511,%d0                        
    2a9c:  660e           	bnes 0x2aac                           ; ↓ label_15
    2a9e:  7000           	moveq #0,%d0                          
    2aa0:  3003           	movew %d3,%d0                         
    2aa2:  d080           	addl %d0,%d0                          
    2aa4:  41fa 1750      	lea %pc@(0x41f6),%a0                  ; ??? 0x41f6
    2aa8:  3e30 0800      	movew %a0@(00000000,%d0:l),%d7        

  label_15:
    2aac:  3003           	movew %d3,%d0                         
    2aae:  5240           	addqw #1,%d0                          
    2ab0:  7200           	moveq #0,%d1                          
    2ab2:  3200           	movew %d0,%d1                         
    2ab4:  d281           	addl %d1,%d1                          
    2ab6:  41fa 173e      	lea %pc@(0x41f6),%a0                  ; ??? 0x41f6
    2aba:  be70 1800      	cmpw %a0@(00000000,%d1:l),%d7         
    2abe:  6400 0086      	bccw 0x2b46                           ; ↓ label_19
    2ac2:  3d7c 2a01 ffd2 	movew #10753,%fp@(-46)                
    2ac8:  3d45 ffd4      	movew %d5,%fp@(-44)                   
    2acc:  3d7c 4000 ffd6 	movew #16384,%fp@(-42)                
    2ad2:  426e ffd8      	clrw %fp@(-40)                        
    2ad6:  426e ffda      	clrw %fp@(-38)                        
    2ada:  426e ffdc      	clrw %fp@(-36)                        
    2ade:  3d7c 0800 ffe4 	movew #2048,%fp@(-28)                 
    2ae4:  3d7c ffff ffe6 	movew #-1,%fp@(-26)                   
    2aea:  3d7c 00ff ffe8 	movew #255,%fp@(-24)                  
    2af0:  3d47 ffea      	movew %d7,%fp@(-22)                   
    2af4:  426e ffec      	clrw %fp@(-20)                        
    2af8:  426e ffee      	clrw %fp@(-18)                        
    2afc:  1d7c 0001 fff6 	moveb #1,%fp@(-10)                    
    2b02:  41fa f90e      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    2b06:  2d48 fff8      	movel %a0,%fp@(-8)                    
    2b0a:  41ee ffc4      	lea %fp@(-60),%a0                     
    2b0e:  2d48 fffc      	movel %a0,%fp@(-4)                    
    2b12:  426e ffc4      	clrw %fp@(-60)                        
    2b16:  486e ffd2      	pea %fp@(-46)                         
    2b1a:  2078 0304      	moveal 0x304,%a0                      
    2b1e:  2068 0018      	moveal %a0@(24),%a0                   
    2b22:  2068 009a      	moveal %a0@(154),%a0                  
    2b26:  4e90           	jsr %a0@                              
    2b28:  302e ffc4      	movew %fp@(-60),%d0                   
    2b2c:  0240 03ff      	andiw #1023,%d0                       
    2b30:  584f           	addqw #4,%sp                          
    2b32:  6710           	beqs 0x2b44                           ; ↓ label_18
    2b34:  7001           	moveq #1,%d0                          
    2b36:  c0ab 0004      	andl %a3@(4),%d0                      
    2b3a:  6704           	beqs 0x2b40                           ; ↓ label_16
    2b3c:  7003           	moveq #3,%d0                          
    2b3e:  6002           	bras 0x2b42                           ; ↓ label_17

  label_16:
    2b40:  7002           	moveq #2,%d0                          

  label_17:
    2b42:  601c           	bras 0x2b60                           ; ↓ label_21

  label_18:
    2b44:  5247           	addqw #1,%d7                          

  label_19:
    2b46:  5245           	addqw #1,%d5                          

  label_20:
    2b48:  246b 0020      	moveal %a3@(32),%a2                   
    2b4c:  ba6a 25a4      	cmpw %a2@(9636),%d5                   
    2b50:  6500 fe44      	bcsw 0x2996                           ; ↑ label_6
    2b54:  4e4f           	trap #15                              
    2b56:  a130           	0120460                               ; sysTrapEvtResetAutoOffTimer
    2b58:  2f0b           	movel %a3,%sp@-                       
    2b5a:  4eba 0268      	jsr %pc@(0x2dc4)                      ; PrvJobMakeAddressTable
    2b5e:  584f           	addqw #4,%sp                          

  label_21:
    2b60:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2b64:  4e5e           	unlk %fp                              
    2b66:  4e75           	rts                                   
```

# PrvJobDetectBootBlock: 0x2b78 - 0x2daa

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-8)`     : ???
   * `%fp@(-14)`    : ???
   * `%fp@(-18)`    : ???
   * `%fp@(-22)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-42)`    : ???
   * `%fp@(-58)`    : ???
   * `%fp@(-60)`    : ???
   * `%fp@(-64)`    : ???
   * `%fp@(-68)`    : ???
   * `%fp@(-70)`    : ???
   * `%fp@(-88)`    : ???
   * `%fp@(-106)`   : ???

```
    2b78:  4e56 ff96      	linkw %fp,#-106                       
    2b7c:  48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-          
    2b80:  246e 0008      	moveal %fp@(8),%a2                    
    2b84:  7600           	moveq #0,%d3                          
    2b86:  357c ffff 0016 	movew #-1,%a2@(22)                    
    2b8c:  357c ffff 0018 	movew #-1,%a2@(24)                    
    2b92:  3d7c 1e01 ff96 	movew #7681,%fp@(-106)                
    2b98:  426e ffa8      	clrw %fp@(-88)                        
    2b9c:  1d7c 0001 ffba 	moveb #1,%fp@(-70)                    
    2ba2:  41fa f86e      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    2ba6:  2d48 ffbc      	movel %a0,%fp@(-68)                   
    2baa:  41ee fff2      	lea %fp@(-14),%a0                     
    2bae:  2d48 ffc0      	movel %a0,%fp@(-64)                   
    2bb2:  426e fff2      	clrw %fp@(-14)                        
    2bb6:  486e ff96      	pea %fp@(-106)                        
    2bba:  2078 0304      	moveal 0x304,%a0                      
    2bbe:  2068 0018      	moveal %a0@(24),%a0                   
    2bc2:  2068 009a      	moveal %a0@(154),%a0                  
    2bc6:  4e90           	jsr %a0@                              
    2bc8:  302e fff2      	movew %fp@(-14),%d0                   
    2bcc:  0240 03ff      	andiw #1023,%d0                       
    2bd0:  584f           	addqw #4,%sp                          
    2bd2:  4a40           	tstw %d0                              
    2bd4:  670c           	beqs 0x2be2                           ; ↓ label_0
    2bd6:  5340           	subqw #1,%d0                          
    2bd8:  672c           	beqs 0x2c06                           ; ↓ label_2
    2bda:  0440 03fe      	subiw #1022,%d0                       
    2bde:  6720           	beqs 0x2c00                           ; ↓ label_1
    2be0:  602a           	bras 0x2c0c                           ; ↓ label_3

  label_0:
    2be2:  382e fff8      	movew %fp@(-8),%d4                    
    2be6:  e04c           	lsrw #8,%d4                           
    2be8:  0244 00ff      	andiw #255,%d4                        
    2bec:  41ee fff2      	lea %fp@(-14),%a0                     
    2bf0:  5088           	addql #8,%a0                          
    2bf2:  2648           	moveal %a0,%a3                        
    2bf4:  3a10           	movew %a0@,%d5                        
    2bf6:  e04d           	lsrw #8,%d5                           
    2bf8:  0245 00ff      	andiw #255,%d5                        
    2bfc:  3013           	movew %a3@,%d0                        
    2bfe:  6012           	bras 0x2c12                           ; ↓ label_4

  label_1:
    2c00:  7001           	moveq #1,%d0                          
    2c02:  6000 01a0      	braw 0x2da4                           ; ↓ label_23

  label_2:
    2c06:  7001           	moveq #1,%d0                          
    2c08:  6000 019a      	braw 0x2da4                           ; ↓ label_23

  label_3:
    2c0c:  7003           	moveq #3,%d0                          
    2c0e:  6000 0194      	braw 0x2da4                           ; ↓ label_23

  label_4:
    2c12:  4a04           	tstb %d4                              
    2c14:  6608           	bnes 0x2c1e                           ; ↓ label_5
    2c16:  0c05 0080      	cmpib #-128,%d5                       
    2c1a:  6602           	bnes 0x2c1e                           ; ↓ label_5
    2c1c:  7609           	moveq #9,%d3                          

  label_5:
    2c1e:  0c43 0009      	cmpiw #9,%d3                          
    2c22:  6728           	beqs 0x2c4c                           ; ↓ label_10
    2c24:  4a04           	tstb %d4                              
    2c26:  6706           	beqs 0x2c2e                           ; ↓ label_6
    2c28:  0c04 00ff      	cmpib #-1,%d4                         
    2c2c:  6604           	bnes 0x2c32                           ; ↓ label_7

  label_6:
    2c2e:  7601           	moveq #1,%d3                          
    2c30:  601a           	bras 0x2c4c                           ; ↓ label_10

  label_7:
    2c32:  0c04 0001      	cmpib #1,%d4                          
    2c36:  670e           	beqs 0x2c46                           ; ↓ label_8
    2c38:  7000           	moveq #0,%d0                          
    2c3a:  1005           	moveb %d5,%d0                         
    2c3c:  0240 0080      	andiw #128,%d0                        
    2c40:  0c40 0080      	cmpiw #128,%d0                        
    2c44:  6704           	beqs 0x2c4a                           ; ↓ label_9

  label_8:
    2c46:  7602           	moveq #2,%d3                          
    2c48:  6002           	bras 0x2c4c                           ; ↓ label_10

  label_9:
    2c4a:  7609           	moveq #9,%d3                          

  label_10:
    2c4c:  3003           	movew %d3,%d0                         
    2c4e:  5340           	subqw #1,%d0                          
    2c50:  6706           	beqs 0x2c58                           ; ↓ label_11
    2c52:  5340           	subqw #1,%d0                          
    2c54:  6744           	beqs 0x2c9a                           ; ↓ label_13
    2c56:  6048           	bras 0x2ca0                           ; ↓ label_14

  label_11:
    2c58:  0caa 4473 7331 	cmpil #1148416817,%a2@(70)            
    2c5e:  0046                                                  
    2c60:  6744           	beqs 0x2ca6                           ; ↓ label_15
    2c62:  4879 1100 8000 	pea 0x11008000                        
    2c68:  2f3c 5368 4473 	movel #1399342195,%sp@-               
    2c6e:  2f3c 4473 7331 	movel #1148416817,%sp@-               
    2c74:  2078 0304      	moveal 0x304,%a0                      
    2c78:  2068 0018      	moveal %a0@(24),%a0                   
    2c7c:  2068 0092      	moveal %a0@(146),%a0                  
    2c80:  4e90           	jsr %a0@                              
    2c82:  4a40           	tstw %d0                              
    2c84:  4fef 000c      	lea %sp@(12),%sp                      
    2c88:  660a           	bnes 0x2c94                           ; ↓ label_12
    2c8a:  257c 4473 7331 	movel #1148416817,%a2@(70)            
    2c90:  0046                                                  
    2c92:  6012           	bras 0x2ca6                           ; ↓ label_15

  label_12:
    2c94:  7003           	moveq #3,%d0                          
    2c96:  6000 010c      	braw 0x2da4                           ; ↓ label_23

  label_13:
    2c9a:  7001           	moveq #1,%d0                          
    2c9c:  6000 0106      	braw 0x2da4                           ; ↓ label_23

  label_14:
    2ca0:  7001           	moveq #1,%d0                          
    2ca2:  6000 0100      	braw 0x2da4                           ; ↓ label_23

  label_15:
    2ca6:  3d7c 2201 ffc4 	movew #8705,%fp@(-60)                 
    2cac:  3d7c 4000 ffc6 	movew #16384,%fp@(-58)                
    2cb2:  426e ffd6      	clrw %fp@(-42)                        
    2cb6:  1d7c 0001 ffe8 	moveb #1,%fp@(-24)                    
    2cbc:  41fa f754      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    2cc0:  2d48 ffea      	movel %a0,%fp@(-22)                   
    2cc4:  41ee fff2      	lea %fp@(-14),%a0                     
    2cc8:  2d48 ffee      	movel %a0,%fp@(-18)                   
    2ccc:  426e fff2      	clrw %fp@(-14)                        
    2cd0:  486e ffc4      	pea %fp@(-60)                         
    2cd4:  2078 0304      	moveal 0x304,%a0                      
    2cd8:  2068 0018      	moveal %a0@(24),%a0                   
    2cdc:  2068 009a      	moveal %a0@(154),%a0                  
    2ce0:  4e90           	jsr %a0@                              
    2ce2:  302e fff2      	movew %fp@(-14),%d0                   
    2ce6:  0240 03ff      	andiw #1023,%d0                       
    2cea:  584f           	addqw #4,%sp                          
    2cec:  4a40           	tstw %d0                              
    2cee:  6714           	beqs 0x2d04                           ; ↓ label_18
    2cf0:  0440 03ff      	subiw #1023,%d0                       
    2cf4:  6702           	beqs 0x2cf8                           ; ↓ label_16
    2cf6:  6006           	bras 0x2cfe                           ; ↓ label_17

  label_16:
    2cf8:  7001           	moveq #1,%d0                          
    2cfa:  6000 00a8      	braw 0x2da4                           ; ↓ label_23

  label_17:
    2cfe:  7003           	moveq #3,%d0                          
    2d00:  6000 00a2      	braw 0x2da4                           ; ↓ label_23

  label_18:
    2d04:  302e fff2      	movew %fp@(-14),%d0                   
    2d08:  0240 03ff      	andiw #1023,%d0                       
    2d0c:  6706           	beqs 0x2d14                           ; ↓ label_19
    2d0e:  7003           	moveq #3,%d0                          
    2d10:  6000 0092      	braw 0x2da4                           ; ↓ label_23

  label_19:
    2d14:  4878 2a00      	pea 0x2a00                            
    2d18:  2f3c 1100 8000 	movel #285245440,%sp@-                
    2d1e:  42a7           	clrl %sp@-                            
    2d20:  2f2a 0020      	movel %a2@(32),%sp@-                  
    2d24:  4e4f           	trap #15                              
    2d26:  a076           	0120166                               ; sysTrapDmWrite
    2d28:  41ee fff2      	lea %fp@(-14),%a0                     
    2d2c:  5888           	addql #4,%a0                          
    2d2e:  2608           	movel %a0,%d3                         
    2d30:  3550 0016      	movew %a0@,%a2@(22)                   
    2d34:  426a 000c      	clrw %a2@(12)                         
    2d38:  41ee fff2      	lea %fp@(-14),%a0                     
    2d3c:  5488           	addql #2,%a0                          
    2d3e:  2648           	moveal %a0,%a3                        
    2d40:  3010           	movew %a0@,%d0                        
    2d42:  e448           	lsrw #2,%d0                           
    2d44:  3540 000c      	movew %d0,%a2@(12)                    
    2d48:  3013           	movew %a3@,%d0                        
    2d4a:  0240 0040      	andiw #64,%d0                         
    2d4e:  7209           	moveq #9,%d1                          
    2d50:  e368           	lslw %d1,%d0                          
    2d52:  816a 000c      	orw %d0,%a2@(12)                      
    2d56:  2043           	moveal %d3,%a0                        
    2d58:  0c50 ffff      	cmpiw #-1,%a0@                        
    2d5c:  4fef 0010      	lea %sp@(16),%sp                      
    2d60:  6706           	beqs 0x2d68                           ; ↓ label_20
    2d62:  006a 4000 000c 	oriw #16384,%a2@(12)                  

  label_20:
    2d68:  302a 000c      	movew %a2@(12),%d0                    
    2d6c:  0240 8000      	andiw #-32768,%d0                     
    2d70:  671c           	beqs 0x2d8e                           ; ↓ label_21
    2d72:  2678 0304      	moveal 0x304,%a3                      
    2d76:  7020           	moveq #32,%d0                         
    2d78:  81aa 0008      	orl %d0,%a2@(8)                       
    2d7c:  200b           	movel %a3,%d0                         
    2d7e:  670e           	beqs 0x2d8e                           ; ↓ label_21
    2d80:  262b 0010      	movel %a3@(16),%d3                    
    2d84:  6708           	beqs 0x2d8e                           ; ↓ label_21
    2d86:  2043           	moveal %d3,%a0                        
    2d88:  7008           	moveq #8,%d0                          
    2d8a:  81a8 001c      	orl %d0,%a0@(28)                      

  label_21:
    2d8e:  302a 000c      	movew %a2@(12),%d0                    
    2d92:  0240 4000      	andiw #16384,%d0                      
    2d96:  670a           	beqs 0x2da2                           ; ↓ label_22
    2d98:  7040           	moveq #64,%d0                         
    2d9a:  81aa 0008      	orl %d0,%a2@(8)                       
    2d9e:  7000           	moveq #0,%d0                          
    2da0:  6002           	bras 0x2da4                           ; ↓ label_23

  label_22:
    2da2:  7001           	moveq #1,%d0                          

  label_23:
    2da4:  4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3          
    2da8:  4e5e           	unlk %fp                              
    2daa:  4e75           	rts                                   
```

# PrvJobMakeAddressTable: 0x2dc4 - 0x32fc

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
    2dc4:  4e56 fe64      	linkw %fp,#-412                       
    2dc8:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2dcc:  41fa 1428      	lea %pc@(0x41f6),%a0                  ; ??? 0x41f6
    2dd0:  2d48 fe72      	movel %a0,%fp@(-398)                  
    2dd4:  206e 0008      	moveal %fp@(8),%a0                    
    2dd8:  4268 001a      	clrw %a0@(26)                         
    2ddc:  70ef           	moveq #-17,%d0                        
    2dde:  c1a8 0008      	andl %d0,%a0@(8)                      
    2de2:  4878 0400      	pea 0x400                             
    2de6:  4e4f           	trap #15                              
    2de8:  a013           	0120023                               ; sysTrapMemPtrNew
    2dea:  2d48 fec8      	movel %a0,%fp@(-312)                  
    2dee:  4aae fec8      	tstl %fp@(-312)                       
    2df2:  584f           	addqw #4,%sp                          
    2df4:  6606           	bnes 0x2dfc                           ; ↓ label_0
    2df6:  7007           	moveq #7,%d0                          
    2df8:  6000 04fc      	braw 0x32f6                           ; ↓ label_28

  label_0:
    2dfc:  4878 040c      	pea 0x40c                             
    2e00:  4e4f           	trap #15                              
    2e02:  a013           	0120023                               ; sysTrapMemPtrNew
    2e04:  2648           	moveal %a0,%a3                        
    2e06:  200b           	movel %a3,%d0                         
    2e08:  584f           	addqw #4,%sp                          
    2e0a:  6610           	bnes 0x2e1c                           ; ↓ label_1
    2e0c:  2f2e fec8      	movel %fp@(-312),%sp@-                
    2e10:  4e4f           	trap #15                              
    2e12:  a012           	0120022                               ; sysTrapMemChunkFree
    2e14:  7007           	moveq #7,%d0                          
    2e16:  584f           	addqw #4,%sp                          
    2e18:  6000 04dc      	braw 0x32f6                           ; ↓ label_28

  label_1:
    2e1c:  206e 0008      	moveal %fp@(8),%a0                    
    2e20:  2468 0020      	moveal %a0@(32),%a2                   
    2e24:  302a 25a4      	movew %a2@(9636),%d0                  
    2e28:  7209           	moveq #9,%d1                          
    2e2a:  e268           	lsrw %d1,%d0                          
    2e2c:  7200           	moveq #0,%d1                          
    2e2e:  3200           	movew %d0,%d1                         
    2e30:  2d41 fea8      	movel %d1,%fp@(-344)                  
    2e34:  0c2a 0001 2578 	cmpib #1,%a2@(9592)                   
    2e3a:  6706           	beqs 0x2e42                           ; ↓ label_2
    2e3c:  7001           	moveq #1,%d0                          
    2e3e:  6000 04b6      	braw 0x32f6                           ; ↓ label_28

  label_2:
    2e42:  4aaa 2570      	tstl %a2@(9584)                       
    2e46:  6706           	beqs 0x2e4e                           ; ↓ label_3
    2e48:  7001           	moveq #1,%d0                          
    2e4a:  6000 04aa      	braw 0x32f6                           ; ↓ label_28

  label_3:
    2e4e:  202a 2574      	movel %a2@(9588),%d0                  
    2e52:  e280           	asrl #1,%d0                           
    2e54:  206e 0008      	moveal %fp@(8),%a0                    
    2e58:  3140 001a      	movew %d0,%a0@(26)                    
    2e5c:  7a00           	moveq #0,%d5                          
    2e5e:  3a00           	movew %d0,%d5                         
    2e60:  2068 0020      	moveal %a0@(32),%a0                   
    2e64:  41e8 2600      	lea %a0@(9728),%a0                    
    2e68:  2d48 fec0      	movel %a0,%fp@(-320)                  
    2e6c:  7600           	moveq #0,%d3                          
    2e6e:  2448           	moveal %a0,%a2                        

  label_4:
    2e70:  0c52 ffff      	cmpiw #-1,%a2@                        
    2e74:  670c           	beqs 0x2e82                           ; ↓ label_5
    2e76:  548a           	addql #2,%a2                          
    2e78:  5283           	addql #1,%d3                          
    2e7a:  0c83 0000 0100 	cmpil #256,%d3                        
    2e80:  65ee           	bcss 0x2e70                           ; ↑ label_4

  label_5:
    2e82:  b685           	cmpl %d5,%d3                          
    2e84:  670a           	beqs 0x2e90                           ; ↓ label_6
    2e86:  206e 0008      	moveal %fp@(8),%a0                    
    2e8a:  7010           	moveq #16,%d0                         
    2e8c:  81a8 0008      	orl %d0,%a0@(8)                       

  label_6:
    2e90:  206e 0008      	moveal %fp@(8),%a0                    
    2e94:  2068 0020      	moveal %a0@(32),%a0                   
    2e98:  3628 25a4      	movew %a0@(9636),%d3                  
    2e9c:  3d7c 3a01 fe76 	movew #14849,%fp@(-394)               
    2ea2:  426e fe78      	clrw %fp@(-392)                       
    2ea6:  3d43 fe7a      	movew %d3,%fp@(-390)                  
    2eaa:  3d7c 4000 fe7c 	movew #16384,%fp@(-388)               
    2eb0:  426e fe88      	clrw %fp@(-376)                       
    2eb4:  1d7c 0001 fe9a 	moveb #1,%fp@(-358)                   
    2eba:  41fa f556      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    2ebe:  2d48 fe9c      	movel %a0,%fp@(-356)                  
    2ec2:  41ee fe64      	lea %fp@(-412),%a0                    
    2ec6:  2d48 fea0      	movel %a0,%fp@(-352)                  
    2eca:  426e fe64      	clrw %fp@(-412)                       
    2ece:  486e fe76      	pea %fp@(-394)                        
    2ed2:  2078 0304      	moveal 0x304,%a0                      
    2ed6:  2068 0018      	moveal %a0@(24),%a0                   
    2eda:  2068 009a      	moveal %a0@(154),%a0                  
    2ede:  4e90           	jsr %a0@                              
    2ee0:  302e fe64      	movew %fp@(-412),%d0                  
    2ee4:  0240 03ff      	andiw #1023,%d0                       
    2ee8:  584f           	addqw #4,%sp                          
    2eea:  6716           	beqs 0x2f02                           ; ↓ label_9
    2eec:  7001           	moveq #1,%d0                          
    2eee:  206e 0008      	moveal %fp@(8),%a0                    
    2ef2:  c0a8 0004      	andl %a0@(4),%d0                      
    2ef6:  6704           	beqs 0x2efc                           ; ↓ label_7
    2ef8:  7001           	moveq #1,%d0                          
    2efa:  6002           	bras 0x2efe                           ; ↓ label_8

  label_7:
    2efc:  7002           	moveq #2,%d0                          

  label_8:
    2efe:  6000 03f6      	braw 0x32f6                           ; ↓ label_28

  label_9:
    2f02:  426e feb2      	clrw %fp@(-334)                       
    2f06:  426e feac      	clrw %fp@(-340)                       
    2f0a:  2d6e fe72 feb4 	movel %fp@(-398),%fp@(-332)           
    2f10:  42ae feae      	clrl %fp@(-338)                       
    2f14:  6000 03cc      	braw 0x32e2                           ; ↓ label_27

  label_10:
    2f18:  7c00           	moveq #0,%d6                          
    2f1a:  1f3c 00ff      	moveb #-1,%sp@-                       
    2f1e:  4878 03e0      	pea 0x3e0                             
    2f22:  2f2e fec8      	movel %fp@(-312),%sp@-                
    2f26:  4e4f           	trap #15                              
    2f28:  a027           	0120047                               ; sysTrapMemSet
    2f2a:  204b           	moveal %a3,%a0                        
    2f2c:  5088           	addql #8,%a0                          
    2f2e:  5888           	addql #4,%a0                          
    2f30:  2688           	movel %a0,%a3@                        
    2f32:  426e fed4      	clrw %fp@(-300)                       
    2f36:  3d6e feac fed2 	movew %fp@(-340),%fp@(-302)           
    2f3c:  2d6e feb4 feda 	movel %fp@(-332),%fp@(-294)           
    2f42:  302e feb2      	movew %fp@(-334),%d0                  
    2f46:  5240           	addqw #1,%d0                          
    2f48:  7200           	moveq #0,%d1                          
    2f4a:  3200           	movew %d0,%d1                         
    2f4c:  d281           	addl %d1,%d1                          
    2f4e:  206e fe72      	moveal %fp@(-398),%a0                 
    2f52:  d1c1           	addal %d1,%a0                         
    2f54:  2d48 fec4      	movel %a0,%fp@(-316)                  
    2f58:  4fef 000a      	lea %sp@(10),%sp                      

  label_11:
    2f5c:  362e fed2      	movew %fp@(-302),%d3                  
    2f60:  7800           	moveq #0,%d4                          
    2f62:  4a85           	tstl %d5                              
    2f64:  6714           	beqs 0x2f7a                           ; ↓ label_14
    2f66:  246e fec0      	moveal %fp@(-320),%a2                 

  label_12:
    2f6a:  b652           	cmpw %a2@,%d3                         
    2f6c:  6604           	bnes 0x2f72                           ; ↓ label_13
    2f6e:  5286           	addql #1,%d6                          
    2f70:  6008           	bras 0x2f7a                           ; ↓ label_14

  label_13:
    2f72:  548a           	addql #2,%a2                          
    2f74:  5284           	addql #1,%d4                          
    2f76:  b885           	cmpl %d5,%d4                          
    2f78:  65f0           	bcss 0x2f6a                           ; ↑ label_12

  label_14:
    2f7a:  b885           	cmpl %d5,%d4                          
    2f7c:  6500 027a      	bcsw 0x31f8                           ; ↓ label_24
    2f80:  3003           	movew %d3,%d0                         
    2f82:  e548           	lslw #2,%d0                           
    2f84:  7200           	moveq #0,%d1                          
    2f86:  3200           	movew %d0,%d1                         
    2f88:  0681 1100 8000 	addil #285245440,%d1                  
    2f8e:  2e01           	movel %d1,%d7                         
    2f90:  2041           	moveal %d1,%a0                        
    2f92:  43ee fff2      	lea %fp@(-14),%a1                     
    2f96:  5489           	addql #2,%a1                          
    2f98:  2449           	moveal %a1,%a2                        
    2f9a:  3290           	movew %a0@,%a1@                       
    2f9c:  2047           	moveal %d7,%a0                        
    2f9e:  43ee fff2      	lea %fp@(-14),%a1                     
    2fa2:  5889           	addql #4,%a1                          
    2fa4:  2809           	movel %a1,%d4                         
    2fa6:  32a8 0002      	movew %a0@(2),%a1@                    
    2faa:  3012           	movew %a2@,%d0                        
    2fac:  0240 8000      	andiw #-32768,%d0                     
    2fb0:  6606           	bnes 0x2fb8                           ; ↓ label_15
    2fb2:  5286           	addql #1,%d6                          
    2fb4:  6000 0242      	braw 0x31f8                           ; ↓ label_24

  label_15:
    2fb8:  3012           	movew %a2@,%d0                        
    2fba:  0240 0004      	andiw #4,%d0                          
    2fbe:  6700 0238      	beqw 0x31f8                           ; ↓ label_24
    2fc2:  3012           	movew %a2@,%d0                        
    2fc4:  0240 0008      	andiw #8,%d0                          
    2fc8:  6642           	bnes 0x300c                           ; ↓ label_16
    2fca:  3d7c 3201 ff96 	movew #12801,%fp@(-106)               
    2fd0:  3d43 ff98      	movew %d3,%fp@(-104)                  
    2fd4:  426e ffa8      	clrw %fp@(-88)                        
    2fd8:  1d7c 0001 ffba 	moveb #1,%fp@(-70)                    
    2fde:  41fa f432      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    2fe2:  2d48 ffbc      	movel %a0,%fp@(-68)                   
    2fe6:  41ee fff2      	lea %fp@(-14),%a0                     
    2fea:  2d48 ffc0      	movel %a0,%fp@(-64)                   
    2fee:  426e fff2      	clrw %fp@(-14)                        
    2ff2:  486e ff96      	pea %fp@(-106)                        
    2ff6:  2078 0304      	moveal 0x304,%a0                      
    2ffa:  2068 0018      	moveal %a0@(24),%a0                   
    2ffe:  2068 009a      	moveal %a0@(154),%a0                  
    3002:  4e90           	jsr %a0@                              
    3004:  5286           	addql #1,%d6                          
    3006:  584f           	addqw #4,%sp                          
    3008:  6000 01ee      	braw 0x31f8                           ; ↓ label_24

  label_16:
    300c:  2044           	moveal %d4,%a0                        
    300e:  0c50 fffe      	cmpiw #-2,%a0@                        
    3012:  660a           	bnes 0x301e                           ; ↓ label_17
    3014:  2053           	moveal %a3@,%a0                       
    3016:  3083           	movew %d3,%a0@                        
    3018:  5493           	addql #2,%a3@                         
    301a:  6000 01dc      	braw 0x31f8                           ; ↓ label_24

  label_17:
    301e:  2044           	moveal %d4,%a0                        
    3020:  3e10           	movew %a0@,%d7                        
    3022:  206e feda      	moveal %fp@(-294),%a0                 
    3026:  3d50 fecc      	movew %a0@,%fp@(-308)                 
    302a:  be50           	cmpw %a0@,%d7                         
    302c:  6508           	bcss 0x3036                           ; ↓ label_18
    302e:  206e fec4      	moveal %fp@(-316),%a0                 
    3032:  be50           	cmpw %a0@,%d7                         
    3034:  6546           	bcss 0x307c                           ; ↓ label_19

  label_18:
    3036:  3d7c 3201 ff68 	movew #12801,%fp@(-152)               
    303c:  3d43 ff6a      	movew %d3,%fp@(-150)                  
    3040:  426e ff7a      	clrw %fp@(-134)                       
    3044:  1d7c 0001 ff8c 	moveb #1,%fp@(-116)                   
    304a:  41fa f3c6      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    304e:  2d48 ff8e      	movel %a0,%fp@(-114)                  
    3052:  41ee fff2      	lea %fp@(-14),%a0                     
    3056:  2d48 ff92      	movel %a0,%fp@(-110)                  
    305a:  426e fff2      	clrw %fp@(-14)                        
    305e:  486e ff68      	pea %fp@(-152)                        
    3062:  2078 0304      	moveal 0x304,%a0                      
    3066:  2068 0018      	moveal %a0@(24),%a0                   
    306a:  2068 009a      	moveal %a0@(154),%a0                  
    306e:  4e90           	jsr %a0@                              
    3070:  2053           	moveal %a3@,%a0                       
    3072:  3083           	movew %d3,%a0@                        
    3074:  5493           	addql #2,%a3@                         
    3076:  584f           	addqw #4,%sp                          
    3078:  6000 017e      	braw 0x31f8                           ; ↓ label_24

  label_19:
    307c:  7000           	moveq #0,%d0                          
    307e:  302e fecc      	movew %fp@(-308),%d0                  
    3082:  d080           	addl %d0,%d0                          
    3084:  7200           	moveq #0,%d1                          
    3086:  3207           	movew %d7,%d1                         
    3088:  d281           	addl %d1,%d1                          
    308a:  206e fec8      	moveal %fp@(-312),%a0                 
    308e:  d1c1           	addal %d1,%a0                         
    3090:  2d48 fed6      	movel %a0,%fp@(-298)                  
    3094:  91c0           	subal %d0,%a0                         
    3096:  2d48 fece      	movel %a0,%fp@(-306)                  
    309a:  3810           	movew %a0@,%d4                        
    309c:  0c44 ffff      	cmpiw #-1,%d4                         
    30a0:  6700 0150      	beqw 0x31f2                           ; ↓ label_23
    30a4:  3012           	movew %a2@,%d0                        
    30a6:  0240 1000      	andiw #4096,%d0                       
    30aa:  6700 00f0      	beqw 0x319c                           ; ↓ label_22
    30ae:  3d7c 3601 ffc4 	movew #13825,%fp@(-60)                
    30b4:  3d44 ffc6      	movew %d4,%fp@(-58)                   
    30b8:  426e ffc8      	clrw %fp@(-56)                        
    30bc:  426e ffd6      	clrw %fp@(-42)                        
    30c0:  1d7c 0001 ffe8 	moveb #1,%fp@(-24)                    
    30c6:  41fa f34a      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    30ca:  2d48 ffea      	movel %a0,%fp@(-22)                   
    30ce:  41ee fff2      	lea %fp@(-14),%a0                     
    30d2:  2d48 ffee      	movel %a0,%fp@(-18)                   
    30d6:  426e fff2      	clrw %fp@(-14)                        
    30da:  486e ffc4      	pea %fp@(-60)                         
    30de:  2078 0304      	moveal 0x304,%a0                      
    30e2:  2068 0018      	moveal %a0@(24),%a0                   
    30e6:  2068 009a      	moveal %a0@(154),%a0                  
    30ea:  4e90           	jsr %a0@                              
    30ec:  302e fff2      	movew %fp@(-14),%d0                   
    30f0:  0240 03ff      	andiw #1023,%d0                       
    30f4:  584f           	addqw #4,%sp                          
    30f6:  6608           	bnes 0x3100                           ; ↓ label_20
    30f8:  3012           	movew %a2@,%d0                        
    30fa:  0240 1000      	andiw #4096,%d0                       
    30fe:  6758           	beqs 0x3158                           ; ↓ label_21

  label_20:
    3100:  3d7c 3201 ff3a 	movew #12801,%fp@(-198)               
    3106:  3d44 ff3c      	movew %d4,%fp@(-196)                  
    310a:  426e ff4c      	clrw %fp@(-180)                       
    310e:  1d7c 0001 ff5e 	moveb #1,%fp@(-162)                   
    3114:  41fa f2fc      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    3118:  2d48 ff60      	movel %a0,%fp@(-160)                  
    311c:  41ee fff2      	lea %fp@(-14),%a0                     
    3120:  2d48 ff64      	movel %a0,%fp@(-156)                  
    3124:  426e fff2      	clrw %fp@(-14)                        
    3128:  486e ff3a      	pea %fp@(-198)                        
    312c:  2078 0304      	moveal 0x304,%a0                      
    3130:  2068 0018      	moveal %a0@(24),%a0                   
    3134:  2068 009a      	moveal %a0@(154),%a0                  
    3138:  4e90           	jsr %a0@                              
    313a:  2053           	moveal %a3@,%a0                       
    313c:  3084           	movew %d4,%a0@                        
    313e:  5493           	addql #2,%a3@                         
    3140:  206e feda      	moveal %fp@(-294),%a0                 
    3144:  7000           	moveq #0,%d0                          
    3146:  3010           	movew %a0@,%d0                        
    3148:  d080           	addl %d0,%d0                          
    314a:  206e fed6      	moveal %fp@(-298),%a0                 
    314e:  91c0           	subal %d0,%a0                         
    3150:  3083           	movew %d3,%a0@                        
    3152:  584f           	addqw #4,%sp                          
    3154:  6000 00a2      	braw 0x31f8                           ; ↓ label_24

  label_21:
    3158:  3d7c 3201 ff0c 	movew #12801,%fp@(-244)               
    315e:  3d43 ff0e      	movew %d3,%fp@(-242)                  
    3162:  426e ff1e      	clrw %fp@(-226)                       
    3166:  1d7c 0001 ff30 	moveb #1,%fp@(-208)                   
    316c:  41fa f2a4      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    3170:  2d48 ff32      	movel %a0,%fp@(-206)                  
    3174:  41ee fff2      	lea %fp@(-14),%a0                     
    3178:  2d48 ff36      	movel %a0,%fp@(-202)                  
    317c:  426e fff2      	clrw %fp@(-14)                        
    3180:  486e ff0c      	pea %fp@(-244)                        
    3184:  2078 0304      	moveal 0x304,%a0                      
    3188:  2068 0018      	moveal %a0@(24),%a0                   
    318c:  2068 009a      	moveal %a0@(154),%a0                  
    3190:  4e90           	jsr %a0@                              
    3192:  2053           	moveal %a3@,%a0                       
    3194:  3083           	movew %d3,%a0@                        
    3196:  5493           	addql #2,%a3@                         
    3198:  584f           	addqw #4,%sp                          
    319a:  605c           	bras 0x31f8                           ; ↓ label_24

  label_22:
    319c:  3d7c 3201 fede 	movew #12801,%fp@(-290)               
    31a2:  3d44 fee0      	movew %d4,%fp@(-288)                  
    31a6:  426e fef0      	clrw %fp@(-272)                       
    31aa:  1d7c 0001 ff02 	moveb #1,%fp@(-254)                   
    31b0:  41fa f260      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    31b4:  2d48 ff04      	movel %a0,%fp@(-252)                  
    31b8:  41ee fff2      	lea %fp@(-14),%a0                     
    31bc:  2d48 ff08      	movel %a0,%fp@(-248)                  
    31c0:  426e fff2      	clrw %fp@(-14)                        
    31c4:  486e fede      	pea %fp@(-290)                        
    31c8:  2078 0304      	moveal 0x304,%a0                      
    31cc:  2068 0018      	moveal %a0@(24),%a0                   
    31d0:  2068 009a      	moveal %a0@(154),%a0                  
    31d4:  4e90           	jsr %a0@                              
    31d6:  2053           	moveal %a3@,%a0                       
    31d8:  3084           	movew %d4,%a0@                        
    31da:  5493           	addql #2,%a3@                         
    31dc:  206e feda      	moveal %fp@(-294),%a0                 
    31e0:  7000           	moveq #0,%d0                          
    31e2:  3010           	movew %a0@,%d0                        
    31e4:  d080           	addl %d0,%d0                          
    31e6:  206e fed6      	moveal %fp@(-298),%a0                 
    31ea:  91c0           	subal %d0,%a0                         
    31ec:  3083           	movew %d3,%a0@                        
    31ee:  584f           	addqw #4,%sp                          
    31f0:  6006           	bras 0x31f8                           ; ↓ label_24

  label_23:
    31f2:  206e fece      	moveal %fp@(-306),%a0                 
    31f6:  3083           	movew %d3,%a0@                        

  label_24:
    31f8:  526e fed2      	addqw #1,%fp@(-302)                   
    31fc:  526e fed4      	addqw #1,%fp@(-300)                   
    3200:  0c6e 0200 fed4 	cmpiw #512,%fp@(-300)                 
    3206:  6500 fd54      	bcsw 0x2f5c                           ; ↑ label_11
    320a:  204b           	moveal %a3,%a0                        
    320c:  5088           	addql #8,%a0                          
    320e:  5888           	addql #4,%a0                          
    3210:  2253           	moveal %a3@,%a1                       
    3212:  93c8           	subal %a0,%a1                         
    3214:  2009           	movel %a1,%d0                         
    3216:  e088           	lsrl #8,%d0                           
    3218:  e088           	lsrl #8,%d0                           
    321a:  e088           	lsrl #8,%d0                           
    321c:  ee88           	lsrl #7,%d0                           
    321e:  d089           	addl %a1,%d0                          
    3220:  e280           	asrl #1,%d0                           
    3222:  2740 0004      	movel %d0,%a3@(4)                     
    3226:  202e fea8      	movel %fp@(-344),%d0                  
    322a:  5380           	subql #1,%d0                          
    322c:  b0ae fea4      	cmpl %fp@(-348),%d0                   
    3230:  6612           	bnes 0x3244                           ; ↓ label_25
    3232:  53ab 0004      	subql #1,%a3@(4)                      
    3236:  5286           	addql #1,%d6                          
    3238:  202b 0004      	movel %a3@(4),%d0                     
    323c:  d080           	addl %d0,%d0                          
    323e:  37bc ffff 080c 	movew #-1,%a3@(0000000c,%d0:l)        

  label_25:
    3244:  7010           	moveq #16,%d0                         
    3246:  bc80           	cmpl %d0,%d6                          
    3248:  6d0a           	blts 0x3254                           ; ↓ label_26
    324a:  206e 0008      	moveal %fp@(8),%a0                    
    324e:  7010           	moveq #16,%d0                         
    3250:  81a8 0008      	orl %d0,%a0@(8)                       

  label_26:
    3254:  2746 0008      	movel %d6,%a3@(8)                     
    3258:  206e 0008      	moveal %fp@(8),%a0                    
    325c:  2d68 001c febc 	movel %a0@(28),%fp@(-324)             
    3262:  4878 03e0      	pea 0x3e0                             
    3266:  2f2e fec8      	movel %fp@(-312),%sp@-                
    326a:  206e feb4      	moveal %fp@(-332),%a0                 
    326e:  7000           	moveq #0,%d0                          
    3270:  3010           	movew %a0@,%d0                        
    3272:  d080           	addl %d0,%d0                          
    3274:  206e febc      	moveal %fp@(-324),%a0                 
    3278:  d1c0           	addal %d0,%a0                         
    327a:  2008           	movel %a0,%d0                         
    327c:  90ae febc      	subl %fp@(-324),%d0                   
    3280:  2f00           	movel %d0,%sp@-                       
    3282:  2f2e febc      	movel %fp@(-324),%sp@-                
    3286:  4e4f           	trap #15                              
    3288:  a076           	0120166                               ; sysTrapDmWrite
    328a:  206e 0008      	moveal %fp@(8),%a0                    
    328e:  2068 001c      	moveal %a0@(28),%a0                   
    3292:  d1ee feae      	addal %fp@(-338),%a0                  
    3296:  41e8 400c      	lea %a0@(16396),%a0                   
    329a:  2688           	movel %a0,%a3@                        
    329c:  206e 0008      	moveal %fp@(8),%a0                    
    32a0:  2d68 001c feb8 	movel %a0@(28),%fp@(-328)             
    32a6:  4878 040c      	pea 0x40c                             
    32aa:  2f0b           	movel %a3,%sp@-                       
    32ac:  206e feb8      	moveal %fp@(-328),%a0                 
    32b0:  d1ee feae      	addal %fp@(-338),%a0                  
    32b4:  41e8 4000      	lea %a0@(16384),%a0                   
    32b8:  2008           	movel %a0,%d0                         
    32ba:  90ae feb8      	subl %fp@(-328),%d0                   
    32be:  2f00           	movel %d0,%sp@-                       
    32c0:  2f2e feb8      	movel %fp@(-328),%sp@-                
    32c4:  4e4f           	trap #15                              
    32c6:  a076           	0120166                               ; sysTrapDmWrite
    32c8:  066e 0200 feac 	addiw #512,%fp@(-340)                 
    32ce:  54ae feb4      	addql #2,%fp@(-332)                   
    32d2:  06ae 0000 040c 	addil #1036,%fp@(-338)                
    32d8:  feae                                                  
    32da:  526e feb2      	addqw #1,%fp@(-334)                   
    32de:  4fef 0020      	lea %sp@(32),%sp                      

  label_27:
    32e2:  7000           	moveq #0,%d0                          
    32e4:  302e feb2      	movew %fp@(-334),%d0                  
    32e8:  2d40 fea4      	movel %d0,%fp@(-348)                  
    32ec:  b0ae fea8      	cmpl %fp@(-344),%d0                   
    32f0:  6d00 fc26      	bltw 0x2f18                           ; ↑ label_10
    32f4:  7000           	moveq #0,%d0                          

  label_28:
    32f6:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    32fa:  4e5e           	unlk %fp                              
    32fc:  4e75           	rts                                   
```

# PrvJobReadPBlock: 0x3318 - 0x3856

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
    3318:  4e56 ff76      	linkw %fp,#-138                       
    331c:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    3320:  1e2e 000e      	moveb %fp@(14),%d7                    
    3324:  1c2e 0010      	moveb %fp@(16),%d6                    
    3328:  266e 0012      	moveal %fp@(18),%a3                   
    332c:  0c6e ffff 000c 	cmpiw #-1,%fp@(12)                    
    3332:  6626           	bnes 0x335a                           ; ↓ label_0
    3334:  4227           	clrb %sp@-                            
    3336:  7000           	moveq #0,%d0                          
    3338:  1007           	moveb %d7,%d0                         
    333a:  7200           	moveq #0,%d1                          
    333c:  1206           	moveb %d6,%d1                         
    333e:  9240           	subw %d0,%d1                          
    3340:  5241           	addqw #1,%d1                          
    3342:  7009           	moveq #9,%d0                          
    3344:  e169           	lslw %d0,%d1                          
    3346:  48c1           	extl %d1                              
    3348:  2f01           	movel %d1,%sp@-                       
    334a:  2f0b           	movel %a3,%sp@-                       
    334c:  4e4f           	trap #15                              
    334e:  a027           	0120047                               ; sysTrapMemSet
    3350:  7000           	moveq #0,%d0                          
    3352:  4fef 000a      	lea %sp@(10),%sp                      
    3356:  6000 04f8      	braw 0x3850                           ; ↓ label_21

  label_0:
    335a:  3d7c 3601 ff7a 	movew #13825,%fp@(-134)               
    3360:  3d6e 000c ff7c 	movew %fp@(12),%fp@(-132)             
    3366:  7000           	moveq #0,%d0                          
    3368:  1007           	moveb %d7,%d0                         
    336a:  3a00           	movew %d0,%d5                         
    336c:  3d40 ff7e      	movew %d0,%fp@(-130)                  
    3370:  426e ff8c      	clrw %fp@(-116)                       
    3374:  1d7c 0001 ff9e 	moveb #1,%fp@(-98)                    
    337a:  41fa f096      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    337e:  2d48 ffa0      	movel %a0,%fp@(-96)                   
    3382:  41ee ffa8      	lea %fp@(-88),%a0                     
    3386:  2d48 ffa4      	movel %a0,%fp@(-92)                   
    338a:  426e ffa8      	clrw %fp@(-88)                        
    338e:  486e ff7a      	pea %fp@(-134)                        
    3392:  2078 0304      	moveal 0x304,%a0                      
    3396:  2068 0018      	moveal %a0@(24),%a0                   
    339a:  2068 009a      	moveal %a0@(154),%a0                  
    339e:  4e90           	jsr %a0@                              
    33a0:  302e ffa8      	movew %fp@(-88),%d0                   
    33a4:  0240 03ff      	andiw #1023,%d0                       
    33a8:  584f           	addqw #4,%sp                          
    33aa:  6716           	beqs 0x33c2                           ; ↓ label_3
    33ac:  7001           	moveq #1,%d0                          
    33ae:  206e 0008      	moveal %fp@(8),%a0                    
    33b2:  c0a8 0004      	andl %a0@(4),%d0                      
    33b6:  6704           	beqs 0x33bc                           ; ↓ label_1
    33b8:  7003           	moveq #3,%d0                          
    33ba:  6002           	bras 0x33be                           ; ↓ label_2

  label_1:
    33bc:  7002           	moveq #2,%d0                          

  label_2:
    33be:  6000 0490      	braw 0x3850                           ; ↓ label_21

  label_3:
    33c2:  41ee ffa8      	lea %fp@(-88),%a0                     
    33c6:  5488           	addql #2,%a0                          
    33c8:  2448           	moveal %a0,%a2                        
    33ca:  3010           	movew %a0@,%d0                        
    33cc:  0240 0030      	andiw #48,%d0                         
    33d0:  0c40 0030      	cmpiw #48,%d0                         
    33d4:  6726           	beqs 0x33fc                           ; ↓ label_4
    33d6:  4227           	clrb %sp@-                            
    33d8:  7000           	moveq #0,%d0                          
    33da:  1007           	moveb %d7,%d0                         
    33dc:  7200           	moveq #0,%d1                          
    33de:  1206           	moveb %d6,%d1                         
    33e0:  9240           	subw %d0,%d1                          
    33e2:  5241           	addqw #1,%d1                          
    33e4:  7009           	moveq #9,%d0                          
    33e6:  e169           	lslw %d0,%d1                          
    33e8:  48c1           	extl %d1                              
    33ea:  2f01           	movel %d1,%sp@-                       
    33ec:  2f0b           	movel %a3,%sp@-                       
    33ee:  4e4f           	trap #15                              
    33f0:  a027           	0120047                               ; sysTrapMemSet
    33f2:  7000           	moveq #0,%d0                          
    33f4:  4fef 000a      	lea %sp@(10),%sp                      
    33f8:  6000 0456      	braw 0x3850                           ; ↓ label_21

  label_4:
    33fc:  7600           	moveq #0,%d3                          
    33fe:  383c 4000      	movew #16384,%d4                      
    3402:  b7fc 1100 8000 	cmpal #285245440,%a3                  
    3408:  651a           	bcss 0x3424                           ; ↓ label_5
    340a:  b7fc 1101 0000 	cmpal #285278208,%a3                  
    3410:  6412           	bccs 0x3424                           ; ↓ label_5
    3412:  204b           	moveal %a3,%a0                        
    3414:  d1fc eeff 8000 	addal #-285245440,%a0                 
    341a:  2808           	movel %a0,%d4                         
    341c:  e24c           	lsrw #1,%d4                           
    341e:  0644 4000      	addiw #16384,%d4                      
    3422:  7601           	moveq #1,%d3                          

  label_5:
    3424:  3d7c 2601 ffb6 	movew #9729,%fp@(-74)                 
    342a:  3d6e 000c ffb8 	movew %fp@(12),%fp@(-72)              
    3430:  3d44 ffba      	movew %d4,%fp@(-70)                   
    3434:  3d45 ffbc      	movew %d5,%fp@(-68)                   
    3438:  7000           	moveq #0,%d0                          
    343a:  1006           	moveb %d6,%d0                         
    343c:  3d40 ffbe      	movew %d0,%fp@(-66)                   
    3440:  426e ffc8      	clrw %fp@(-56)                        
    3444:  1d7c 0001 ffda 	moveb #1,%fp@(-38)                    
    344a:  41fa efc6      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    344e:  2d48 ffdc      	movel %a0,%fp@(-36)                   
    3452:  41ee ffa8      	lea %fp@(-88),%a0                     
    3456:  2d48 ffe0      	movel %a0,%fp@(-32)                   
    345a:  426e ffa8      	clrw %fp@(-88)                        
    345e:  486e ffb6      	pea %fp@(-74)                         
    3462:  2078 0304      	moveal 0x304,%a0                      
    3466:  2068 0018      	moveal %a0@(24),%a0                   
    346a:  2068 009a      	moveal %a0@(154),%a0                  
    346e:  4e90           	jsr %a0@                              
    3470:  302e ffa8      	movew %fp@(-88),%d0                   
    3474:  0240 03ff      	andiw #1023,%d0                       
    3478:  584f           	addqw #4,%sp                          
    347a:  6716           	beqs 0x3492                           ; ↓ label_8
    347c:  7001           	moveq #1,%d0                          
    347e:  206e 0008      	moveal %fp@(8),%a0                    
    3482:  c0a8 0004      	andl %a0@(4),%d0                      
    3486:  6704           	beqs 0x348c                           ; ↓ label_6
    3488:  7003           	moveq #3,%d0                          
    348a:  6002           	bras 0x348e                           ; ↓ label_7

  label_6:
    348c:  7002           	moveq #2,%d0                          

  label_7:
    348e:  6000 03c0      	braw 0x3850                           ; ↓ label_21

  label_8:
    3492:  3012           	movew %a2@,%d0                        
    3494:  4240           	clrw %d0                              
    3496:  806e ffac      	orw %fp@(-84),%d0                     
    349a:  7800           	moveq #0,%d4                          
    349c:  3800           	movew %d0,%d4                         
    349e:  4a84           	tstl %d4                              
    34a0:  6714           	beqs 0x34b6                           ; ↓ label_10
    34a2:  206e 0016      	moveal %fp@(22),%a0                   
    34a6:  4a90           	tstl %a0@                             
    34a8:  6706           	beqs 0x34b0                           ; ↓ label_9
    34aa:  7001           	moveq #1,%d0                          
    34ac:  6000 03a2      	braw 0x3850                           ; ↓ label_21

  label_9:
    34b0:  206e 0016      	moveal %fp@(22),%a0                   
    34b4:  2084           	movel %d4,%a0@                        

  label_10:
    34b6:  4a03           	tstb %d3                              
    34b8:  6600 036e      	bnew 0x3828                           ; ↓ label_18
    34bc:  7000           	moveq #0,%d0                          
    34be:  1007           	moveb %d7,%d0                         
    34c0:  7a00           	moveq #0,%d5                          
    34c2:  1a06           	moveb %d6,%d5                         
    34c4:  9a40           	subw %d0,%d5                          
    34c6:  5245           	addqw #1,%d5                          
    34c8:  7009           	moveq #9,%d0                          
    34ca:  e16d           	lslw %d0,%d5                          
    34cc:  48c5           	extl %d5                              
    34ce:  e48d           	lsrl #2,%d5                           
    34d0:  7001           	moveq #1,%d0                          
    34d2:  220b           	movel %a3,%d1                         
    34d4:  c280           	andl %d0,%d1                          
    34d6:  6700 0284      	beqw 0x375c                           ; ↓ label_14
    34da:  7600           	moveq #0,%d3                          
    34dc:  4a85           	tstl %d5                              
    34de:  6700 0348      	beqw 0x3828                           ; ↓ label_18
    34e2:  2005           	movel %d5,%d0                         
    34e4:  5180           	subql #8,%d0                          
    34e6:  2d40 ff76      	movel %d0,%fp@(-138)                  
    34ea:  7008           	moveq #8,%d0                          
    34ec:  ba80           	cmpl %d0,%d5                          
    34ee:  6300 033c      	blsw 0x382c                           ; ↓ label_19
    34f2:  7e00           	moveq #0,%d7                          
    34f4:  2c3c 1100 8000 	movel #285245440,%d6                  

  label_11:
    34fa:  2046           	moveal %d6,%a0                        
    34fc:  2810           	movel %a0@,%d4                        
    34fe:  2004           	movel %d4,%d0                         
    3500:  7218           	moveq #24,%d1                         
    3502:  e2a8           	lsrl %d1,%d0                          
    3504:  0280 0000 00ff 	andil #255,%d0                        
    350a:  204b           	moveal %a3,%a0                        
    350c:  d1c7           	addal %d7,%a0                         
    350e:  2448           	moveal %a0,%a2                        
    3510:  1080           	moveb %d0,%a0@                        
    3512:  2004           	movel %d4,%d0                         
    3514:  e088           	lsrl #8,%d0                           
    3516:  0280 0000 ffff 	andil #65535,%d0                      
    351c:  3540 0001      	movew %d0,%a2@(1)                     
    3520:  2004           	movel %d4,%d0                         
    3522:  0280 0000 00ff 	andil #255,%d0                        
    3528:  1540 0003      	moveb %d0,%a2@(3)                     
    352c:  2003           	movel %d3,%d0                         
    352e:  5280           	addql #1,%d0                          
    3530:  e588           	lsll #2,%d0                           
    3532:  2440           	moveal %d0,%a2                        
    3534:  2d40 fffc      	movel %d0,%fp@(-4)                    
    3538:  203c 1100 8000 	movel #285245440,%d0                  
    353e:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    3542:  2004           	movel %d4,%d0                         
    3544:  e2a8           	lsrl %d1,%d0                          
    3546:  0280 0000 00ff 	andil #255,%d0                        
    354c:  204b           	moveal %a3,%a0                        
    354e:  d1ee fffc      	addal %fp@(-4),%a0                    
    3552:  2448           	moveal %a0,%a2                        
    3554:  1080           	moveb %d0,%a0@                        
    3556:  2004           	movel %d4,%d0                         
    3558:  e088           	lsrl #8,%d0                           
    355a:  0280 0000 ffff 	andil #65535,%d0                      
    3560:  3540 0001      	movew %d0,%a2@(1)                     
    3564:  2004           	movel %d4,%d0                         
    3566:  0280 0000 00ff 	andil #255,%d0                        
    356c:  1540 0003      	moveb %d0,%a2@(3)                     
    3570:  2003           	movel %d3,%d0                         
    3572:  5480           	addql #2,%d0                          
    3574:  e588           	lsll #2,%d0                           
    3576:  2440           	moveal %d0,%a2                        
    3578:  2d40 ffe4      	movel %d0,%fp@(-28)                   
    357c:  203c 1100 8000 	movel #285245440,%d0                  
    3582:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    3586:  2004           	movel %d4,%d0                         
    3588:  e2a8           	lsrl %d1,%d0                          
    358a:  0280 0000 00ff 	andil #255,%d0                        
    3590:  204b           	moveal %a3,%a0                        
    3592:  d1ee ffe4      	addal %fp@(-28),%a0                   
    3596:  2448           	moveal %a0,%a2                        
    3598:  1080           	moveb %d0,%a0@                        
    359a:  2004           	movel %d4,%d0                         
    359c:  e088           	lsrl #8,%d0                           
    359e:  0280 0000 ffff 	andil #65535,%d0                      
    35a4:  3540 0001      	movew %d0,%a2@(1)                     
    35a8:  2004           	movel %d4,%d0                         
    35aa:  0280 0000 00ff 	andil #255,%d0                        
    35b0:  1540 0003      	moveb %d0,%a2@(3)                     
    35b4:  2003           	movel %d3,%d0                         
    35b6:  5680           	addql #3,%d0                          
    35b8:  e588           	lsll #2,%d0                           
    35ba:  2440           	moveal %d0,%a2                        
    35bc:  2d40 ffe8      	movel %d0,%fp@(-24)                   
    35c0:  203c 1100 8000 	movel #285245440,%d0                  
    35c6:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    35ca:  2004           	movel %d4,%d0                         
    35cc:  e2a8           	lsrl %d1,%d0                          
    35ce:  0280 0000 00ff 	andil #255,%d0                        
    35d4:  204b           	moveal %a3,%a0                        
    35d6:  d1ee ffe8      	addal %fp@(-24),%a0                   
    35da:  2448           	moveal %a0,%a2                        
    35dc:  1080           	moveb %d0,%a0@                        
    35de:  2004           	movel %d4,%d0                         
    35e0:  e088           	lsrl #8,%d0                           
    35e2:  0280 0000 ffff 	andil #65535,%d0                      
    35e8:  3540 0001      	movew %d0,%a2@(1)                     
    35ec:  2004           	movel %d4,%d0                         
    35ee:  0280 0000 00ff 	andil #255,%d0                        
    35f4:  1540 0003      	moveb %d0,%a2@(3)                     
    35f8:  2003           	movel %d3,%d0                         
    35fa:  5880           	addql #4,%d0                          
    35fc:  e588           	lsll #2,%d0                           
    35fe:  2440           	moveal %d0,%a2                        
    3600:  2d40 ffec      	movel %d0,%fp@(-20)                   
    3604:  203c 1100 8000 	movel #285245440,%d0                  
    360a:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    360e:  2004           	movel %d4,%d0                         
    3610:  e2a8           	lsrl %d1,%d0                          
    3612:  0280 0000 00ff 	andil #255,%d0                        
    3618:  204b           	moveal %a3,%a0                        
    361a:  d1ee ffec      	addal %fp@(-20),%a0                   
    361e:  2448           	moveal %a0,%a2                        
    3620:  1080           	moveb %d0,%a0@                        
    3622:  2004           	movel %d4,%d0                         
    3624:  e088           	lsrl #8,%d0                           
    3626:  0280 0000 ffff 	andil #65535,%d0                      
    362c:  3540 0001      	movew %d0,%a2@(1)                     
    3630:  2004           	movel %d4,%d0                         
    3632:  0280 0000 00ff 	andil #255,%d0                        
    3638:  1540 0003      	moveb %d0,%a2@(3)                     
    363c:  2003           	movel %d3,%d0                         
    363e:  5a80           	addql #5,%d0                          
    3640:  e588           	lsll #2,%d0                           
    3642:  2440           	moveal %d0,%a2                        
    3644:  2d40 fff0      	movel %d0,%fp@(-16)                   
    3648:  203c 1100 8000 	movel #285245440,%d0                  
    364e:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    3652:  2004           	movel %d4,%d0                         
    3654:  e2a8           	lsrl %d1,%d0                          
    3656:  0280 0000 00ff 	andil #255,%d0                        
    365c:  204b           	moveal %a3,%a0                        
    365e:  d1ee fff0      	addal %fp@(-16),%a0                   
    3662:  2448           	moveal %a0,%a2                        
    3664:  1080           	moveb %d0,%a0@                        
    3666:  2004           	movel %d4,%d0                         
    3668:  e088           	lsrl #8,%d0                           
    366a:  0280 0000 ffff 	andil #65535,%d0                      
    3670:  3540 0001      	movew %d0,%a2@(1)                     
    3674:  2004           	movel %d4,%d0                         
    3676:  0280 0000 00ff 	andil #255,%d0                        
    367c:  1540 0003      	moveb %d0,%a2@(3)                     
    3680:  2003           	movel %d3,%d0                         
    3682:  5c80           	addql #6,%d0                          
    3684:  e588           	lsll #2,%d0                           
    3686:  2440           	moveal %d0,%a2                        
    3688:  2d40 fff4      	movel %d0,%fp@(-12)                   
    368c:  203c 1100 8000 	movel #285245440,%d0                  
    3692:  2832 0800      	movel %a2@(00000000,%d0:l),%d4        
    3696:  2004           	movel %d4,%d0                         
    3698:  e2a8           	lsrl %d1,%d0                          
    369a:  0280 0000 00ff 	andil #255,%d0                        
    36a0:  204b           	moveal %a3,%a0                        
    36a2:  d1ee fff4      	addal %fp@(-12),%a0                   
    36a6:  2448           	moveal %a0,%a2                        
    36a8:  1080           	moveb %d0,%a0@                        
    36aa:  2004           	movel %d4,%d0                         
    36ac:  e088           	lsrl #8,%d0                           
    36ae:  0280 0000 ffff 	andil #65535,%d0                      
    36b4:  3540 0001      	movew %d0,%a2@(1)                     
    36b8:  2004           	movel %d4,%d0                         
    36ba:  0280 0000 00ff 	andil #255,%d0                        
    36c0:  1540 0003      	moveb %d0,%a2@(3)                     
    36c4:  2003           	movel %d3,%d0                         
    36c6:  5e80           	addql #7,%d0                          
    36c8:  e588           	lsll #2,%d0                           
    36ca:  2d40 fff8      	movel %d0,%fp@(-8)                    
    36ce:  2040           	moveal %d0,%a0                        
    36d0:  203c 1100 8000 	movel #285245440,%d0                  
    36d6:  2830 0800      	movel %a0@(00000000,%d0:l),%d4        
    36da:  2004           	movel %d4,%d0                         
    36dc:  e2a8           	lsrl %d1,%d0                          
    36de:  0280 0000 00ff 	andil #255,%d0                        
    36e4:  204b           	moveal %a3,%a0                        
    36e6:  d1ee fff8      	addal %fp@(-8),%a0                    
    36ea:  2448           	moveal %a0,%a2                        
    36ec:  1080           	moveb %d0,%a0@                        
    36ee:  2004           	movel %d4,%d0                         
    36f0:  e088           	lsrl #8,%d0                           
    36f2:  0280 0000 ffff 	andil #65535,%d0                      
    36f8:  3540 0001      	movew %d0,%a2@(1)                     
    36fc:  2004           	movel %d4,%d0                         
    36fe:  0280 0000 00ff 	andil #255,%d0                        
    3704:  1540 0003      	moveb %d0,%a2@(3)                     
    3708:  7020           	moveq #32,%d0                         
    370a:  de80           	addl %d0,%d7                          
    370c:  dc80           	addl %d0,%d6                          
    370e:  5083           	addql #8,%d3                          
    3710:  b6ae ff76      	cmpl %fp@(-138),%d3                   
    3714:  6500 fde4      	bcsw 0x34fa                           ; ↑ label_11
    3718:  6000 0112      	braw 0x382c                           ; ↓ label_19

  label_12:
    371c:  2046           	moveal %d6,%a0                        
    371e:  2810           	movel %a0@,%d4                        
    3720:  2004           	movel %d4,%d0                         
    3722:  7218           	moveq #24,%d1                         
    3724:  e2a8           	lsrl %d1,%d0                          
    3726:  0280 0000 00ff 	andil #255,%d0                        
    372c:  204b           	moveal %a3,%a0                        
    372e:  d1c7           	addal %d7,%a0                         
    3730:  2448           	moveal %a0,%a2                        
    3732:  1080           	moveb %d0,%a0@                        
    3734:  2004           	movel %d4,%d0                         
    3736:  e088           	lsrl #8,%d0                           
    3738:  0280 0000 ffff 	andil #65535,%d0                      
    373e:  3540 0001      	movew %d0,%a2@(1)                     
    3742:  2004           	movel %d4,%d0                         
    3744:  0280 0000 00ff 	andil #255,%d0                        
    374a:  1540 0003      	moveb %d0,%a2@(3)                     
    374e:  5887           	addql #4,%d7                          
    3750:  5886           	addql #4,%d6                          
    3752:  5283           	addql #1,%d3                          

  label_13:
    3754:  b685           	cmpl %d5,%d3                          
    3756:  65c4           	bcss 0x371c                           ; ↑ label_12
    3758:  6000 00ce      	braw 0x3828                           ; ↓ label_18

  label_14:
    375c:  7600           	moveq #0,%d3                          
    375e:  4a85           	tstl %d5                              
    3760:  6700 00c6      	beqw 0x3828                           ; ↓ label_18
    3764:  2c05           	movel %d5,%d6                         
    3766:  5186           	subql #8,%d6                          
    3768:  7008           	moveq #8,%d0                          
    376a:  ba80           	cmpl %d0,%d5                          
    376c:  6300 00d0      	blsw 0x383e                           ; ↓ label_20
    3770:  283c 1100 8000 	movel #285245440,%d4                  
    3776:  244b           	moveal %a3,%a2                        

  label_15:
    3778:  2044           	moveal %d4,%a0                        
    377a:  2490           	movel %a0@,%a2@                       
    377c:  2003           	movel %d3,%d0                         
    377e:  5280           	addql #1,%d0                          
    3780:  e588           	lsll #2,%d0                           
    3782:  2040           	moveal %d0,%a0                        
    3784:  203c 1100 8000 	movel #285245440,%d0                  
    378a:  2570 0800 0004 	movel %a0@(00000000,%d0:l),%a2@(4)    
    3790:  2003           	movel %d3,%d0                         
    3792:  5480           	addql #2,%d0                          
    3794:  e588           	lsll #2,%d0                           
    3796:  2040           	moveal %d0,%a0                        
    3798:  203c 1100 8000 	movel #285245440,%d0                  
    379e:  2570 0800 0008 	movel %a0@(00000000,%d0:l),%a2@(8)    
    37a4:  2003           	movel %d3,%d0                         
    37a6:  5680           	addql #3,%d0                          
    37a8:  e588           	lsll #2,%d0                           
    37aa:  2040           	moveal %d0,%a0                        
    37ac:  203c 1100 8000 	movel #285245440,%d0                  
    37b2:  2570 0800 000c 	movel %a0@(00000000,%d0:l),%a2@(12)   
    37b8:  2003           	movel %d3,%d0                         
    37ba:  5880           	addql #4,%d0                          
    37bc:  e588           	lsll #2,%d0                           
    37be:  2040           	moveal %d0,%a0                        
    37c0:  203c 1100 8000 	movel #285245440,%d0                  
    37c6:  2570 0800 0010 	movel %a0@(00000000,%d0:l),%a2@(16)   
    37cc:  2003           	movel %d3,%d0                         
    37ce:  5a80           	addql #5,%d0                          
    37d0:  e588           	lsll #2,%d0                           
    37d2:  2040           	moveal %d0,%a0                        
    37d4:  203c 1100 8000 	movel #285245440,%d0                  
    37da:  2570 0800 0014 	movel %a0@(00000000,%d0:l),%a2@(20)   
    37e0:  2003           	movel %d3,%d0                         
    37e2:  5c80           	addql #6,%d0                          
    37e4:  e588           	lsll #2,%d0                           
    37e6:  2040           	moveal %d0,%a0                        
    37e8:  203c 1100 8000 	movel #285245440,%d0                  
    37ee:  2570 0800 0018 	movel %a0@(00000000,%d0:l),%a2@(24)   
    37f4:  2003           	movel %d3,%d0                         
    37f6:  5e80           	addql #7,%d0                          
    37f8:  e588           	lsll #2,%d0                           
    37fa:  2040           	moveal %d0,%a0                        
    37fc:  203c 1100 8000 	movel #285245440,%d0                  
    3802:  2570 0800 001c 	movel %a0@(00000000,%d0:l),%a2@(28)   
    3808:  7020           	moveq #32,%d0                         
    380a:  d880           	addl %d0,%d4                          
    380c:  45ea 0020      	lea %a2@(32),%a2                      
    3810:  5083           	addql #8,%d3                          
    3812:  b686           	cmpl %d6,%d3                          
    3814:  6500 ff62      	bcsw 0x3778                           ; ↑ label_15
    3818:  6024           	bras 0x383e                           ; ↓ label_20

  label_16:
    381a:  2044           	moveal %d4,%a0                        
    381c:  2490           	movel %a0@,%a2@                       
    381e:  5884           	addql #4,%d4                          
    3820:  588a           	addql #4,%a2                          
    3822:  5283           	addql #1,%d3                          

  label_17:
    3824:  b685           	cmpl %d5,%d3                          
    3826:  65f2           	bcss 0x381a                           ; ↑ label_16

  label_18:
    3828:  7000           	moveq #0,%d0                          
    382a:  6024           	bras 0x3850                           ; ↓ label_21

  label_19:
    382c:  2e03           	movel %d3,%d7                         
    382e:  e58f           	lsll #2,%d7                           
    3830:  2807           	movel %d7,%d4                         
    3832:  2c04           	movel %d4,%d6                         
    3834:  0686 1100 8000 	addil #285245440,%d6                  
    383a:  6000 ff18      	braw 0x3754                           ; ↑ label_13

  label_20:
    383e:  2803           	movel %d3,%d4                         
    3840:  e58c           	lsll #2,%d4                           
    3842:  2c04           	movel %d4,%d6                         
    3844:  0684 1100 8000 	addil #285245440,%d4                  
    384a:  244b           	moveal %a3,%a2                        
    384c:  d5c6           	addal %d6,%a2                         
    384e:  60d4           	bras 0x3824                           ; ↑ label_17

  label_21:
    3850:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    3854:  4e5e           	unlk %fp                              
    3856:  4e75           	rts                                   
```

# PrvJobMoveWritePBlock: 0x386c - 0x3a60

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
    386c:  4e56 fff2      	linkw %fp,#-14                        
    3870:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    3874:  246e 0008      	moveal %fp@(8),%a2                    
    3878:  3c2e 000c      	movew %fp@(12),%d6                    
    387c:  41fa 0978      	lea %pc@(0x41f6),%a0                  ; ??? 0x41f6
    3880:  2808           	movel %a0,%d4                         
    3882:  7000           	moveq #0,%d0                          
    3884:  102e 000e      	moveb %fp@(14),%d0                    
    3888:  3d40 fff6      	movew %d0,%fp@(-10)                   
    388c:  7200           	moveq #0,%d1                          
    388e:  122e 0010      	moveb %fp@(16),%d1                    
    3892:  9240           	subw %d0,%d1                          
    3894:  5241           	addqw #1,%d1                          
    3896:  3d41 fff8      	movew %d1,%fp@(-8)                    
    389a:  42ae fffa      	clrl %fp@(-6)                         
    389e:  7601           	moveq #1,%d3                          
    38a0:  2644           	moveal %d4,%a3                        
    38a2:  548b           	addql #2,%a3                          

  label_0:
    38a4:  bc53           	cmpw %a3@,%d6                         
    38a6:  6406           	bccs 0x38ae                           ; ↓ label_1
    38a8:  5383           	subql #1,%d3                          
    38aa:  2a03           	movel %d3,%d5                         
    38ac:  600a           	bras 0x38b8                           ; ↓ label_2

  label_1:
    38ae:  548b           	addql #2,%a3                          
    38b0:  5283           	addql #1,%d3                          
    38b2:  7011           	moveq #17,%d0                         
    38b4:  b680           	cmpl %d0,%d3                          
    38b6:  6dec           	blts 0x38a4                           ; ↑ label_0

  label_2:
    38b8:  2005           	movel %d5,%d0                         
    38ba:  223c 0000 040c 	movel #1036,%d1                       
    38c0:  4eba 096a      	jsr %pc@(0x422c)                      ; __lmul__
    38c4:  262a 001c      	movel %a2@(28),%d3                    
    38c8:  2043           	moveal %d3,%a0                        
    38ca:  d1c0           	addal %d0,%a0                         
    38cc:  4aa8 4004      	tstl %a0@(16388)                      
    38d0:  6606           	bnes 0x38d8                           ; ↓ label_3
    38d2:  7005           	moveq #5,%d0                          
    38d4:  6000 0184      	braw 0x3a5a                           ; ↓ label_15

  label_3:
    38d8:  3e3c 4000      	movew #16384,%d7                      
    38dc:  382a 0014      	movew %a2@(20),%d4                    
    38e0:  b86e fff8      	cmpw %fp@(-8),%d4                     
    38e4:  663e           	bnes 0x3924                           ; ↓ label_5
    38e6:  0cae 1100 8000 	cmpil #285245440,%fp@(18)             
    38ec:  0012                                                  
    38ee:  6520           	bcss 0x3910                           ; ↓ label_4
    38f0:  0cae 1101 0000 	cmpil #285278208,%fp@(18)             
    38f6:  0012                                                  
    38f8:  6416           	bccs 0x3910                           ; ↓ label_4
    38fa:  206e 0012      	moveal %fp@(18),%a0                   
    38fe:  d1fc eeff 8000 	addal #-285245440,%a0                 
    3904:  2e08           	movel %a0,%d7                         
    3906:  e24f           	lsrw #1,%d7                           
    3908:  0647 4000      	addiw #16384,%d7                      
    390c:  6000 00f6      	braw 0x3a04                           ; ↓ label_11

  label_4:
    3910:  3f2e fff8      	movew %fp@(-8),%sp@-                  
    3914:  4267           	clrw %sp@-                            
    3916:  2f2e 0012      	movel %fp@(18),%sp@-                  
    391a:  4eba 0604      	jsr %pc@(0x3f20)                      ; PrvCopyPages2Buffer
    391e:  504f           	addqw #8,%sp                          
    3920:  6000 00e2      	braw 0x3a04                           ; ↓ label_11

  label_5:
    3924:  7000           	moveq #0,%d0                          
    3926:  3006           	movew %d6,%d0                         
    3928:  d080           	addl %d0,%d0                          
    392a:  2043           	moveal %d3,%a0                        
    392c:  d1c0           	addal %d0,%a0                         
    392e:  2648           	moveal %a0,%a3                        
    3930:  0c50 ffff      	cmpiw #-1,%a0@                        
    3934:  6600 0094      	bnew 0x39ca                           ; ↓ label_9
    3938:  7000           	moveq #0,%d0                          
    393a:  102e 000e      	moveb %fp@(14),%d0                    
    393e:  7209           	moveq #9,%d1                          
    3940:  e368           	lslw %d1,%d0                          
    3942:  48c0           	extl %d0                              
    3944:  e488           	lsrl #2,%d0                           
    3946:  2600           	movel %d0,%d3                         
    3948:  2640           	moveal %d0,%a3                        
    394a:  7800           	moveq #0,%d4                          
    394c:  4a83           	tstl %d3                              
    394e:  6714           	beqs 0x3964                           ; ↓ label_7
    3950:  263c 1100 8000 	movel #285245440,%d3                  

  label_6:
    3956:  2043           	moveal %d3,%a0                        
    3958:  70ff           	moveq #-1,%d0                         
    395a:  2080           	movel %d0,%a0@                        
    395c:  5883           	addql #4,%d3                          
    395e:  5284           	addql #1,%d4                          
    3960:  b88b           	cmpl %a3,%d4                          
    3962:  65f2           	bcss 0x3956                           ; ↑ label_6

  label_7:
    3964:  3f2e fff8      	movew %fp@(-8),%sp@-                  
    3968:  3f2e fff6      	movew %fp@(-10),%sp@-                 
    396c:  2f2e 0012      	movel %fp@(18),%sp@-                  
    3970:  4eba 05ae      	jsr %pc@(0x3f20)                      ; PrvCopyPages2Buffer
    3974:  7000           	moveq #0,%d0                          
    3976:  102e 0010      	moveb %fp@(16),%d0                    
    397a:  3600           	movew %d0,%d3                         
    397c:  322a 0014      	movew %a2@(20),%d1                    
    3980:  9240           	subw %d0,%d1                          
    3982:  0641 ffff      	addiw #-1,%d1                         
    3986:  7009           	moveq #9,%d0                          
    3988:  e169           	lslw %d0,%d1                          
    398a:  7000           	moveq #0,%d0                          
    398c:  3001           	movew %d1,%d0                         
    398e:  e488           	lsrl #2,%d0                           
    3990:  2640           	moveal %d0,%a3                        
    3992:  3003           	movew %d3,%d0                         
    3994:  5240           	addqw #1,%d0                          
    3996:  7209           	moveq #9,%d1                          
    3998:  e368           	lslw %d1,%d0                          
    399a:  48c0           	extl %d0                              
    399c:  e488           	lsrl #2,%d0                           
    399e:  2d40 fff2      	movel %d0,%fp@(-14)                   
    39a2:  7800           	moveq #0,%d4                          
    39a4:  b6fc 0000      	cmpaw #0,%a3                          
    39a8:  504f           	addqw #8,%sp                          
    39aa:  6758           	beqs 0x3a04                           ; ↓ label_11
    39ac:  262e fff2      	movel %fp@(-14),%d3                   
    39b0:  e58b           	lsll #2,%d3                           

  label_8:
    39b2:  2043           	moveal %d3,%a0                        
    39b4:  203c 1100 8000 	movel #285245440,%d0                  
    39ba:  72ff           	moveq #-1,%d1                         
    39bc:  2181 0800      	movel %d1,%a0@(00000000,%d0:l)        
    39c0:  5883           	addql #4,%d3                          
    39c2:  5284           	addql #1,%d4                          
    39c4:  b88b           	cmpl %a3,%d4                          
    39c6:  65ea           	bcss 0x39b2                           ; ↑ label_8
    39c8:  603a           	bras 0x3a04                           ; ↓ label_11

  label_9:
    39ca:  486e fffa      	pea %fp@(-6)                          
    39ce:  4879 1100 8000 	pea 0x11008000                        
    39d4:  3004           	movew %d4,%d0                         
    39d6:  0640 ffff      	addiw #-1,%d0                         
    39da:  1f00           	moveb %d0,%sp@-                       
    39dc:  4227           	clrb %sp@-                            
    39de:  3f13           	movew %a3@,%sp@-                      
    39e0:  2f0a           	movel %a2,%sp@-                       
    39e2:  4eba f934      	jsr %pc@(0x3318)                      ; PrvJobReadPBlock
    39e6:  2600           	movel %d0,%d3                         
    39e8:  4fef 0012      	lea %sp@(18),%sp                      
    39ec:  6704           	beqs 0x39f2                           ; ↓ label_10
    39ee:  2003           	movel %d3,%d0                         
    39f0:  6068           	bras 0x3a5a                           ; ↓ label_15

  label_10:
    39f2:  3f2e fff8      	movew %fp@(-8),%sp@-                  
    39f6:  3f2e fff6      	movew %fp@(-10),%sp@-                 
    39fa:  2f2e 0012      	movel %fp@(18),%sp@-                  
    39fe:  4eba 0520      	jsr %pc@(0x3f20)                      ; PrvCopyPages2Buffer
    3a02:  504f           	addqw #8,%sp                          

  label_11:
    3a04:  7800           	moveq #0,%d4                          
    3a06:  3806           	movew %d6,%d4                         
    3a08:  d884           	addl %d4,%d4                          

  label_12:
    3a0a:  486e fffe      	pea %fp@(-2)                          
    3a0e:  2f05           	movel %d5,%sp@-                       
    3a10:  2f0a           	movel %a2,%sp@-                       
    3a12:  4eba 0066      	jsr %pc@(0x3a7a)                      ; PrvJobFindFreePBlock
    3a16:  2600           	movel %d0,%d3                         
    3a18:  4fef 000c      	lea %sp@(12),%sp                      
    3a1c:  670a           	beqs 0x3a28                           ; ↓ label_13
    3a1e:  2003           	movel %d3,%d0                         
    3a20:  5780           	subql #3,%d0                          
    3a22:  67e6           	beqs 0x3a0a                           ; ↑ label_12
    3a24:  2003           	movel %d3,%d0                         
    3a26:  6032           	bras 0x3a5a                           ; ↓ label_15

  label_13:
    3a28:  2f2e fffa      	movel %fp@(-6),%sp@-                  
    3a2c:  3f07           	movew %d7,%sp@-                       
    3a2e:  3f06           	movew %d6,%sp@-                       
    3a30:  2f05           	movel %d5,%sp@-                       
    3a32:  2044           	moveal %d4,%a0                        
    3a34:  202a 001c      	movel %a2@(28),%d0                    
    3a38:  3f30 0800      	movew %a0@(00000000,%d0:l),%sp@-      
    3a3c:  3f2e fffe      	movew %fp@(-2),%sp@-                  
    3a40:  2f0a           	movel %a2,%sp@-                       
    3a42:  4eba 01f2      	jsr %pc@(0x3c36)                      ; PrvJobWritePBlock
    3a46:  2600           	movel %d0,%d3                         
    3a48:  4fef 0014      	lea %sp@(20),%sp                      
    3a4c:  670a           	beqs 0x3a58                           ; ↓ label_14
    3a4e:  2003           	movel %d3,%d0                         
    3a50:  5780           	subql #3,%d0                          
    3a52:  67b6           	beqs 0x3a0a                           ; ↑ label_12
    3a54:  2003           	movel %d3,%d0                         
    3a56:  6002           	bras 0x3a5a                           ; ↓ label_15

  label_14:
    3a58:  7000           	moveq #0,%d0                          

  label_15:
    3a5a:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    3a5e:  4e5e           	unlk %fp                              
    3a60:  4e75           	rts                                   
```

# PrvJobFindFreePBlock: 0x3a7a - 0x3c1c

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
    3a7a:  4e56 ffb4      	linkw %fp,#-76                        
    3a7e:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    3a82:  246e 0008      	moveal %fp@(8),%a2                    
    3a86:  7010           	moveq #16,%d0                         
    3a88:  c0aa 0008      	andl %a2@(8),%d0                      
    3a8c:  6706           	beqs 0x3a94                           ; ↓ label_0
    3a8e:  7005           	moveq #5,%d0                          
    3a90:  6000 0184      	braw 0x3c16                           ; ↓ label_8

  label_0:
    3a94:  202e 000c      	movel %fp@(12),%d0                    
    3a98:  223c 0000 040c 	movel #1036,%d1                       
    3a9e:  4eba 078c      	jsr %pc@(0x422c)                      ; __lmul__
    3aa2:  2c00           	movel %d0,%d6                         
    3aa4:  206a 001c      	moveal %a2@(28),%a0                   
    3aa8:  d1c0           	addal %d0,%a0                         
    3aaa:  2808           	movel %a0,%d4                         
    3aac:  2628 4004      	movel %a0@(16388),%d3                 
    3ab0:  2044           	moveal %d4,%a0                        
    3ab2:  2d68 4000 fffc 	movel %a0@(16384),%fp@(-4)            
    3ab8:  2806           	movel %d6,%d4                         
    3aba:  603a           	bras 0x3af6                           ; ↓ label_3

  label_1:
    3abc:  54ae fffc      	addql #2,%fp@(-4)                     
    3ac0:  266a 001c      	moveal %a2@(28),%a3                   
    3ac4:  2004           	movel %d4,%d0                         
    3ac6:  d08b           	addl %a3,%d0                          
    3ac8:  2040           	moveal %d0,%a0                        
    3aca:  2028 4004      	movel %a0@(16388),%d0                 
    3ace:  d080           	addl %d0,%d0                          
    3ad0:  204b           	moveal %a3,%a0                        
    3ad2:  d1c4           	addal %d4,%a0                         
    3ad4:  2a08           	movel %a0,%d5                         
    3ad6:  d1c0           	addal %d0,%a0                         
    3ad8:  41e8 400c      	lea %a0@(16396),%a0                   
    3adc:  b1ee fffc      	cmpal %fp@(-4),%a0                    
    3ae0:  660a           	bnes 0x3aec                           ; ↓ label_2
    3ae2:  2045           	moveal %d5,%a0                        
    3ae4:  41e8 400c      	lea %a0@(16396),%a0                   
    3ae8:  2d48 fffc      	movel %a0,%fp@(-4)                    

  label_2:
    3aec:  5383           	subql #1,%d3                          
    3aee:  6606           	bnes 0x3af6                           ; ↓ label_3
    3af0:  7005           	moveq #5,%d0                          
    3af2:  6000 0122      	braw 0x3c16                           ; ↓ label_8

  label_3:
    3af6:  206e fffc      	moveal %fp@(-4),%a0                   
    3afa:  0c50 ffff      	cmpiw #-1,%a0@                        
    3afe:  67bc           	beqs 0x3abc                           ; ↑ label_1
    3b00:  2d6a 001c ffbc 	movel %a2@(28),%fp@(-68)              
    3b06:  4878 0004      	pea 0x4                               
    3b0a:  486e fffc      	pea %fp@(-4)                          
    3b0e:  206e ffbc      	moveal %fp@(-68),%a0                  
    3b12:  d1c6           	addal %d6,%a0                         
    3b14:  41e8 4000      	lea %a0@(16384),%a0                   
    3b18:  2008           	movel %a0,%d0                         
    3b1a:  90ae ffbc      	subl %fp@(-68),%d0                    
    3b1e:  2f00           	movel %d0,%sp@-                       
    3b20:  2f2e ffbc      	movel %fp@(-68),%sp@-                 
    3b24:  4e4f           	trap #15                              
    3b26:  a076           	0120166                               ; sysTrapDmWrite
    3b28:  206a 001c      	moveal %a2@(28),%a0                   
    3b2c:  d1c6           	addal %d6,%a0                         
    3b2e:  2068 4000      	moveal %a0@(16384),%a0                
    3b32:  3610           	movew %a0@,%d3                        
    3b34:  3d7c 3201 ffce 	movew #12801,%fp@(-50)                
    3b3a:  3d43 ffd0      	movew %d3,%fp@(-48)                   
    3b3e:  426e ffe0      	clrw %fp@(-32)                        
    3b42:  1d7c 0001 fff2 	moveb #1,%fp@(-14)                    
    3b48:  41fa e8c8      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    3b4c:  2d48 fff4      	movel %a0,%fp@(-12)                   
    3b50:  41ee ffc0      	lea %fp@(-64),%a0                     
    3b54:  2d48 fff8      	movel %a0,%fp@(-8)                    
    3b58:  426e ffc0      	clrw %fp@(-64)                        
    3b5c:  486e ffce      	pea %fp@(-50)                         
    3b60:  2078 0304      	moveal 0x304,%a0                      
    3b64:  2068 0018      	moveal %a0@(24),%a0                   
    3b68:  2068 009a      	moveal %a0@(154),%a0                  
    3b6c:  4e90           	jsr %a0@                              
    3b6e:  302e ffc0      	movew %fp@(-64),%d0                   
    3b72:  0240 03ff      	andiw #1023,%d0                       
    3b76:  4fef 0014      	lea %sp@(20),%sp                      
    3b7a:  6700 0092      	beqw 0x3c0e                           ; ↓ label_7
    3b7e:  7001           	moveq #1,%d0                          
    3b80:  c0aa 0004      	andl %a2@(4),%d0                      
    3b84:  6606           	bnes 0x3b8c                           ; ↓ label_4
    3b86:  7002           	moveq #2,%d0                          
    3b88:  6000 008c      	braw 0x3c16                           ; ↓ label_8

  label_4:
    3b8c:  302e ffc0      	movew %fp@(-64),%d0                   
    3b90:  0240 03ff      	andiw #1023,%d0                       
    3b94:  5740           	subqw #3,%d0                          
    3b96:  6604           	bnes 0x3b9c                           ; ↓ label_5
    3b98:  7006           	moveq #6,%d0                          
    3b9a:  607a           	bras 0x3c16                           ; ↓ label_8

  label_5:
    3b9c:  2e2a 001c      	movel %a2@(28),%d7                    
    3ba0:  1f3c 00ff      	moveb #-1,%sp@-                       
    3ba4:  4878 0002      	pea 0x2                               
    3ba8:  2047           	moveal %d7,%a0                        
    3baa:  d1c6           	addal %d6,%a0                         
    3bac:  2028 4000      	movel %a0@(16384),%d0                 
    3bb0:  9087           	subl %d7,%d0                          
    3bb2:  2f00           	movel %d0,%sp@-                       
    3bb4:  2f07           	movel %d7,%sp@-                       
    3bb6:  4e4f           	trap #15                              
    3bb8:  a07e           	0120176                               ; sysTrapDmSet
    3bba:  206a 001c      	moveal %a2@(28),%a0                   
    3bbe:  d1c6           	addal %d6,%a0                         
    3bc0:  2028 4008      	movel %a0@(16392),%d0                 
    3bc4:  5280           	addql #1,%d0                          
    3bc6:  2d40 ffb4      	movel %d0,%fp@(-76)                   
    3bca:  2d6a 001c ffb8 	movel %a2@(28),%fp@(-72)              
    3bd0:  4878 0004      	pea 0x4                               
    3bd4:  486e ffb4      	pea %fp@(-76)                         
    3bd8:  206e ffb8      	moveal %fp@(-72),%a0                  
    3bdc:  d1c6           	addal %d6,%a0                         
    3bde:  41e8 4008      	lea %a0@(16392),%a0                   
    3be2:  2008           	movel %a0,%d0                         
    3be4:  90ae ffb8      	subl %fp@(-72),%d0                    
    3be8:  2f00           	movel %d0,%sp@-                       
    3bea:  2f2e ffb8      	movel %fp@(-72),%sp@-                 
    3bee:  4e4f           	trap #15                              
    3bf0:  a076           	0120166                               ; sysTrapDmWrite
    3bf2:  206a 001c      	moveal %a2@(28),%a0                   
    3bf6:  d1c6           	addal %d6,%a0                         
    3bf8:  7010           	moveq #16,%d0                         
    3bfa:  b0a8 4008      	cmpl %a0@(16392),%d0                  
    3bfe:  4fef 001e      	lea %sp@(30),%sp                      
    3c02:  6206           	bhis 0x3c0a                           ; ↓ label_6
    3c04:  7010           	moveq #16,%d0                         
    3c06:  81aa 0008      	orl %d0,%a2@(8)                       

  label_6:
    3c0a:  7003           	moveq #3,%d0                          
    3c0c:  6008           	bras 0x3c16                           ; ↓ label_8

  label_7:
    3c0e:  206e 0010      	moveal %fp@(16),%a0                   
    3c12:  3083           	movew %d3,%a0@                        
    3c14:  7000           	moveq #0,%d0                          

  label_8:
    3c16:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    3c1a:  4e5e           	unlk %fp                              
    3c1c:  4e75           	rts                                   
```

# PrvJobWritePBlock: 0x3c36 - 0x3f0a

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
   * `%fp@(-18)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-22)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-26)`    : ???
   * `%fp@(-28)`    : ???
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
    3c36:  4e56 ffb2      	linkw %fp,#-78                        
    3c3a:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    3c3e:  246e 0008      	moveal %fp@(8),%a2                    
    3c42:  0c6e ffff 000e 	cmpiw #-1,%fp@(14)                    
    3c48:  6604           	bnes 0x3c4e                           ; ↓ label_0
    3c4a:  7000           	moveq #0,%d0                          
    3c4c:  6002           	bras 0x3c50                           ; ↓ label_1

  label_0:
    3c4e:  7003           	moveq #3,%d0                          

  label_1:
    3c50:  3d40 ffb2      	movew %d0,%fp@(-78)                   
    3c54:  3d7c 2a01 ffd2 	movew #10753,%fp@(-46)                
    3c5a:  3d6e 000c ffd4 	movew %fp@(12),%fp@(-44)              
    3c60:  3d6e 0016 ffd6 	movew %fp@(22),%fp@(-42)              
    3c66:  426e ffd8      	clrw %fp@(-40)                        
    3c6a:  302a 0014      	movew %a2@(20),%d0                    
    3c6e:  0640 ffff      	addiw #-1,%d0                         
    3c72:  3d40 ffda      	movew %d0,%fp@(-38)                   
    3c76:  3d6e ffb2 ffdc 	movew %fp@(-78),%fp@(-36)             
    3c7c:  3d7c 0800 ffe4 	movew #2048,%fp@(-28)                 
    3c82:  3d6e 000e ffe6 	movew %fp@(14),%fp@(-26)              
    3c88:  3d7c 00ff ffe8 	movew #255,%fp@(-24)                  
    3c8e:  3d6e 0014 ffea 	movew %fp@(20),%fp@(-22)              
    3c94:  202e 0018      	movel %fp@(24),%d0                    
    3c98:  4240           	clrw %d0                              
    3c9a:  4840           	swap %d0                              
    3c9c:  3d40 ffec      	movew %d0,%fp@(-20)                   
    3ca0:  202e 0018      	movel %fp@(24),%d0                    
    3ca4:  0280 0000 ffff 	andil #65535,%d0                      
    3caa:  3d40 ffee      	movew %d0,%fp@(-18)                   
    3cae:  1d7c 0001 fff6 	moveb #1,%fp@(-10)                    
    3cb4:  41fa e75c      	lea %pc@(0x2412),%a0                  ; PrvDspDoneProc
    3cb8:  2d48 fff8      	movel %a0,%fp@(-8)                    
    3cbc:  41ee ffbc      	lea %fp@(-68),%a0                     
    3cc0:  2d48 fffc      	movel %a0,%fp@(-4)                    
    3cc4:  426e ffbc      	clrw %fp@(-68)                        
    3cc8:  486e ffd2      	pea %fp@(-46)                         
    3ccc:  2078 0304      	moveal 0x304,%a0                      
    3cd0:  2068 0018      	moveal %a0@(24),%a0                   
    3cd4:  2068 009a      	moveal %a0@(154),%a0                  
    3cd8:  4e90           	jsr %a0@                              
    3cda:  302e ffbc      	movew %fp@(-68),%d0                   
    3cde:  0240 03ff      	andiw #1023,%d0                       
    3ce2:  584f           	addqw #4,%sp                          
    3ce4:  6700 0108      	beqw 0x3dee                           ; ↓ label_6
    3ce8:  7001           	moveq #1,%d0                          
    3cea:  c0aa 0004      	andl %a2@(4),%d0                      
    3cee:  6606           	bnes 0x3cf6                           ; ↓ label_2
    3cf0:  7002           	moveq #2,%d0                          
    3cf2:  6000 0210      	braw 0x3f04                           ; ↓ label_10

  label_2:
    3cf6:  302e ffbc      	movew %fp@(-68),%d0                   
    3cfa:  0240 03ff      	andiw #1023,%d0                       
    3cfe:  7200           	moveq #0,%d1                          
    3d00:  3200           	movew %d0,%d1                         
    3d02:  2d41 ffce      	movel %d1,%fp@(-50)                   
    3d06:  7003           	moveq #3,%d0                          
    3d08:  b081           	cmpl %d1,%d0                          
    3d0a:  6606           	bnes 0x3d12                           ; ↓ label_3
    3d0c:  7006           	moveq #6,%d0                          
    3d0e:  6000 01f4      	braw 0x3f04                           ; ↓ label_10

  label_3:
    3d12:  7004           	moveq #4,%d0                          
    3d14:  b0ae ffce      	cmpl %fp@(-50),%d0                    
    3d18:  667c           	bnes 0x3d96                           ; ↓ label_5
    3d1a:  282a 001c      	movel %a2@(28),%d4                    
    3d1e:  1f3c 00ff      	moveb #-1,%sp@-                       
    3d22:  4878 0002      	pea 0x2                               
    3d26:  202e 0010      	movel %fp@(16),%d0                    
    3d2a:  223c 0000 040c 	movel #1036,%d1                       
    3d30:  4eba 04fa      	jsr %pc@(0x422c)                      ; __lmul__
    3d34:  2600           	movel %d0,%d3                         
    3d36:  2044           	moveal %d4,%a0                        
    3d38:  d1c0           	addal %d0,%a0                         
    3d3a:  2028 4000      	movel %a0@(16384),%d0                 
    3d3e:  9084           	subl %d4,%d0                          
    3d40:  2f00           	movel %d0,%sp@-                       
    3d42:  2f04           	movel %d4,%sp@-                       
    3d44:  4e4f           	trap #15                              
    3d46:  a07e           	0120176                               ; sysTrapDmSet
    3d48:  206a 001c      	moveal %a2@(28),%a0                   
    3d4c:  d1c3           	addal %d3,%a0                         
    3d4e:  2028 4008      	movel %a0@(16392),%d0                 
    3d52:  5280           	addql #1,%d0                          
    3d54:  2d40 ffce      	movel %d0,%fp@(-50)                   
    3d58:  2c2a 001c      	movel %a2@(28),%d6                    
    3d5c:  4878 0004      	pea 0x4                               
    3d60:  486e ffce      	pea %fp@(-50)                         
    3d64:  2046           	moveal %d6,%a0                        
    3d66:  d1c3           	addal %d3,%a0                         
    3d68:  41e8 4008      	lea %a0@(16392),%a0                   
    3d6c:  2008           	movel %a0,%d0                         
    3d6e:  9086           	subl %d6,%d0                          
    3d70:  2f00           	movel %d0,%sp@-                       
    3d72:  2f06           	movel %d6,%sp@-                       
    3d74:  4e4f           	trap #15                              
    3d76:  a076           	0120166                               ; sysTrapDmWrite
    3d78:  206a 001c      	moveal %a2@(28),%a0                   
    3d7c:  d1c3           	addal %d3,%a0                         
    3d7e:  7010           	moveq #16,%d0                         
    3d80:  b0a8 4008      	cmpl %a0@(16392),%d0                  
    3d84:  4fef 001e      	lea %sp@(30),%sp                      
    3d88:  6206           	bhis 0x3d90                           ; ↓ label_4
    3d8a:  7010           	moveq #16,%d0                         
    3d8c:  81aa 0008      	orl %d0,%a2@(8)                       

  label_4:
    3d90:  7003           	moveq #3,%d0                          
    3d92:  6000 0170      	braw 0x3f04                           ; ↓ label_10

  label_5:
    3d96:  202e 0010      	movel %fp@(16),%d0                    
    3d9a:  223c 0000 040c 	movel #1036,%d1                       
    3da0:  4eba 048a      	jsr %pc@(0x422c)                      ; __lmul__
    3da4:  2800           	movel %d0,%d4                         
    3da6:  206a 001c      	moveal %a2@(28),%a0                   
    3daa:  d1c0           	addal %d0,%a0                         
    3dac:  2028 4008      	movel %a0@(16392),%d0                 
    3db0:  5280           	addql #1,%d0                          
    3db2:  2d40 ffce      	movel %d0,%fp@(-50)                   
    3db6:  2e2a 001c      	movel %a2@(28),%d7                    
    3dba:  4878 0004      	pea 0x4                               
    3dbe:  486e ffce      	pea %fp@(-50)                         
    3dc2:  2047           	moveal %d7,%a0                        
    3dc4:  d1c4           	addal %d4,%a0                         
    3dc6:  41e8 4008      	lea %a0@(16392),%a0                   
    3dca:  2008           	movel %a0,%d0                         
    3dcc:  9087           	subl %d7,%d0                          
    3dce:  2f00           	movel %d0,%sp@-                       
    3dd0:  2f07           	movel %d7,%sp@-                       
    3dd2:  4e4f           	trap #15                              
    3dd4:  a076           	0120166                               ; sysTrapDmWrite
    3dd6:  206a 001c      	moveal %a2@(28),%a0                   
    3dda:  d1c4           	addal %d4,%a0                         
    3ddc:  7010           	moveq #16,%d0                         
    3dde:  b0a8 4008      	cmpl %a0@(16392),%d0                  
    3de2:  4fef 0010      	lea %sp@(16),%sp                      
    3de6:  6206           	bhis 0x3dee                           ; ↓ label_6
    3de8:  7010           	moveq #16,%d0                         
    3dea:  81aa 0008      	orl %d0,%a2@(8)                       

  label_6:
    3dee:  0c6e ffff 000e 	cmpiw #-1,%fp@(14)                    
    3df4:  6632           	bnes 0x3e28                           ; ↓ label_7
    3df6:  262a 001c      	movel %a2@(28),%d3                    
    3dfa:  1f3c 00ff      	moveb #-1,%sp@-                       
    3dfe:  4878 0002      	pea 0x2                               
    3e02:  202e 0010      	movel %fp@(16),%d0                    
    3e06:  223c 0000 040c 	movel #1036,%d1                       
    3e0c:  4eba 041e      	jsr %pc@(0x422c)                      ; __lmul__
    3e10:  2043           	moveal %d3,%a0                        
    3e12:  d1c0           	addal %d0,%a0                         
    3e14:  2028 4000      	movel %a0@(16384),%d0                 
    3e18:  9083           	subl %d3,%d0                          
    3e1a:  2f00           	movel %d0,%sp@-                       
    3e1c:  2f03           	movel %d3,%sp@-                       
    3e1e:  4e4f           	trap #15                              
    3e20:  a07e           	0120176                               ; sysTrapDmSet
    3e22:  4fef 000e      	lea %sp@(14),%sp                      
    3e26:  6038           	bras 0x3e60                           ; ↓ label_8

  label_7:
    3e28:  266a 001c      	moveal %a2@(28),%a3                   
    3e2c:  4878 0002      	pea 0x2                               
    3e30:  7000           	moveq #0,%d0                          
    3e32:  302e 0014      	movew %fp@(20),%d0                    
    3e36:  d080           	addl %d0,%d0                          
    3e38:  4873 0800      	pea %a3@(00000000,%d0:l)              
    3e3c:  202e 0010      	movel %fp@(16),%d0                    
    3e40:  223c 0000 040c 	movel #1036,%d1                       
    3e46:  4eba 03e4      	jsr %pc@(0x422c)                      ; __lmul__
    3e4a:  204b           	moveal %a3,%a0                        
    3e4c:  d1c0           	addal %d0,%a0                         
    3e4e:  2028 4000      	movel %a0@(16384),%d0                 
    3e52:  908b           	subl %a3,%d0                          
    3e54:  2f00           	movel %d0,%sp@-                       
    3e56:  2f0b           	movel %a3,%sp@-                       
    3e58:  4e4f           	trap #15                              
    3e5a:  a076           	0120166                               ; sysTrapDmWrite
    3e5c:  4fef 0010      	lea %sp@(16),%sp                      

  label_8:
    3e60:  2d6a 001c ffb8 	movel %a2@(28),%fp@(-72)              
    3e66:  4878 0002      	pea 0x2                               
    3e6a:  486e 000c      	pea %fp@(12)                          
    3e6e:  7000           	moveq #0,%d0                          
    3e70:  302e 0014      	movew %fp@(20),%d0                    
    3e74:  d080           	addl %d0,%d0                          
    3e76:  206e ffb8      	moveal %fp@(-72),%a0                  
    3e7a:  d1c0           	addal %d0,%a0                         
    3e7c:  2008           	movel %a0,%d0                         
    3e7e:  90ae ffb8      	subl %fp@(-72),%d0                    
    3e82:  2f00           	movel %d0,%sp@-                       
    3e84:  2f2e ffb8      	movel %fp@(-72),%sp@-                 
    3e88:  4e4f           	trap #15                              
    3e8a:  a076           	0120166                               ; sysTrapDmWrite
    3e8c:  202e 0010      	movel %fp@(16),%d0                    
    3e90:  223c 0000 040c 	movel #1036,%d1                       
    3e96:  4eba 0394      	jsr %pc@(0x422c)                      ; __lmul__
    3e9a:  2600           	movel %d0,%d3                         
    3e9c:  206a 001c      	moveal %a2@(28),%a0                   
    3ea0:  d1c0           	addal %d0,%a0                         
    3ea2:  2068 4000      	moveal %a0@(16384),%a0                
    3ea6:  5488           	addql #2,%a0                          
    3ea8:  2d48 ffca      	movel %a0,%fp@(-54)                   
    3eac:  206a 001c      	moveal %a2@(28),%a0                   
    3eb0:  d1c3           	addal %d3,%a0                         
    3eb2:  2a08           	movel %a0,%d5                         
    3eb4:  2045           	moveal %d5,%a0                        
    3eb6:  2028 4004      	movel %a0@(16388),%d0                 
    3eba:  d080           	addl %d0,%d0                          
    3ebc:  d1c0           	addal %d0,%a0                         
    3ebe:  41e8 400c      	lea %a0@(16396),%a0                   
    3ec2:  b1ee ffca      	cmpal %fp@(-54),%a0                   
    3ec6:  4fef 0010      	lea %sp@(16),%sp                      
    3eca:  660a           	bnes 0x3ed6                           ; ↓ label_9
    3ecc:  2045           	moveal %d5,%a0                        
    3ece:  41e8 400c      	lea %a0@(16396),%a0                   
    3ed2:  2d48 ffca      	movel %a0,%fp@(-54)                   

  label_9:
    3ed6:  2d6a 001c ffb4 	movel %a2@(28),%fp@(-76)              
    3edc:  4878 0004      	pea 0x4                               
    3ee0:  486e ffca      	pea %fp@(-54)                         
    3ee4:  206e ffb4      	moveal %fp@(-76),%a0                  
    3ee8:  d1c3           	addal %d3,%a0                         
    3eea:  41e8 4000      	lea %a0@(16384),%a0                   
    3eee:  2008           	movel %a0,%d0                         
    3ef0:  90ae ffb4      	subl %fp@(-76),%d0                    
    3ef4:  2f00           	movel %d0,%sp@-                       
    3ef6:  2f2e ffb4      	movel %fp@(-76),%sp@-                 
    3efa:  4e4f           	trap #15                              
    3efc:  a076           	0120166                               ; sysTrapDmWrite
    3efe:  7000           	moveq #0,%d0                          
    3f00:  4fef 0010      	lea %sp@(16),%sp                      

  label_10:
    3f04:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    3f08:  4e5e           	unlk %fp                              
    3f0a:  4e75           	rts                                   
```

# PrvCopyPages2Buffer: 0x3f20 - 0x41de

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
    3f20:  4e56 ffe0      	linkw %fp,#-32                        
    3f24:  48e7 0f10      	moveml %d4-%d7/%a3,%sp@-              
    3f28:  282e 0008      	movel %fp@(8),%d4                     
    3f2c:  302e 000e      	movew %fp@(14),%d0                    
    3f30:  7a09           	moveq #9,%d5                          
    3f32:  eb68           	lslw %d5,%d0                          
    3f34:  7a00           	moveq #0,%d5                          
    3f36:  3a00           	movew %d0,%d5                         
    3f38:  e48d           	lsrl #2,%d5                           
    3f3a:  302e 000c      	movew %fp@(12),%d0                    
    3f3e:  7209           	moveq #9,%d1                          
    3f40:  e368           	lslw %d1,%d0                          
    3f42:  7200           	moveq #0,%d1                          
    3f44:  3200           	movew %d0,%d1                         
    3f46:  0681 1100 8000 	addil #285245440,%d1                  
    3f4c:  2d41 ffe4      	movel %d1,%fp@(-28)                   
    3f50:  7001           	moveq #1,%d0                          
    3f52:  c084           	andl %d4,%d0                          
    3f54:  6700 01a0      	beqw 0x40f6                           ; ↓ label_3
    3f58:  7400           	moveq #0,%d2                          
    3f5a:  4a85           	tstl %d5                              
    3f5c:  6700 0248      	beqw 0x41a6                           ; ↓ label_11
    3f60:  2005           	movel %d5,%d0                         
    3f62:  5180           	subql #8,%d0                          
    3f64:  2d40 ffe0      	movel %d0,%fp@(-32)                   
    3f68:  7008           	moveq #8,%d0                          
    3f6a:  ba80           	cmpl %d0,%d5                          
    3f6c:  6300 023a      	blsw 0x41a8                           ; ↓ label_12
    3f70:  7e00           	moveq #0,%d7                          
    3f72:  2241           	moveal %d1,%a1                        

  label_0:
    3f74:  7c00           	moveq #0,%d6                          
    3f76:  2044           	moveal %d4,%a0                        
    3f78:  d1c7           	addal %d7,%a0                         
    3f7a:  2648           	moveal %a0,%a3                        
    3f7c:  3028 0001      	movew %a0@(1),%d0                     
    3f80:  e148           	lslw #8,%d0                           
    3f82:  7200           	moveq #0,%d1                          
    3f84:  3200           	movew %d0,%d1                         
    3f86:  8c81           	orl %d1,%d6                           
    3f88:  7000           	moveq #0,%d0                          
    3f8a:  102b 0003      	moveb %a3@(3),%d0                     
    3f8e:  8c80           	orl %d0,%d6                           
    3f90:  2286           	movel %d6,%a1@                        
    3f92:  2002           	movel %d2,%d0                         
    3f94:  5280           	addql #1,%d0                          
    3f96:  e588           	lsll #2,%d0                           
    3f98:  2d40 fffc      	movel %d0,%fp@(-4)                    
    3f9c:  7c00           	moveq #0,%d6                          
    3f9e:  2044           	moveal %d4,%a0                        
    3fa0:  d1c0           	addal %d0,%a0                         
    3fa2:  2648           	moveal %a0,%a3                        
    3fa4:  3028 0001      	movew %a0@(1),%d0                     
    3fa8:  e148           	lslw #8,%d0                           
    3faa:  7200           	moveq #0,%d1                          
    3fac:  3200           	movew %d0,%d1                         
    3fae:  8c81           	orl %d1,%d6                           
    3fb0:  7000           	moveq #0,%d0                          
    3fb2:  102b 0003      	moveb %a3@(3),%d0                     
    3fb6:  8c80           	orl %d0,%d6                           
    3fb8:  2346 0004      	movel %d6,%a1@(4)                     
    3fbc:  2002           	movel %d2,%d0                         
    3fbe:  5480           	addql #2,%d0                          
    3fc0:  e588           	lsll #2,%d0                           
    3fc2:  2d40 ffe8      	movel %d0,%fp@(-24)                   
    3fc6:  7c00           	moveq #0,%d6                          
    3fc8:  2044           	moveal %d4,%a0                        
    3fca:  d1c0           	addal %d0,%a0                         
    3fcc:  2648           	moveal %a0,%a3                        
    3fce:  3028 0001      	movew %a0@(1),%d0                     
    3fd2:  e148           	lslw #8,%d0                           
    3fd4:  7200           	moveq #0,%d1                          
    3fd6:  3200           	movew %d0,%d1                         
    3fd8:  8c81           	orl %d1,%d6                           
    3fda:  7000           	moveq #0,%d0                          
    3fdc:  102b 0003      	moveb %a3@(3),%d0                     
    3fe0:  8c80           	orl %d0,%d6                           
    3fe2:  2346 0008      	movel %d6,%a1@(8)                     
    3fe6:  2002           	movel %d2,%d0                         
    3fe8:  5680           	addql #3,%d0                          
    3fea:  e588           	lsll #2,%d0                           
    3fec:  2d40 ffec      	movel %d0,%fp@(-20)                   
    3ff0:  7c00           	moveq #0,%d6                          
    3ff2:  2044           	moveal %d4,%a0                        
    3ff4:  d1c0           	addal %d0,%a0                         
    3ff6:  2648           	moveal %a0,%a3                        
    3ff8:  3028 0001      	movew %a0@(1),%d0                     
    3ffc:  e148           	lslw #8,%d0                           
    3ffe:  7200           	moveq #0,%d1                          
    4000:  3200           	movew %d0,%d1                         
    4002:  8c81           	orl %d1,%d6                           
    4004:  7000           	moveq #0,%d0                          
    4006:  102b 0003      	moveb %a3@(3),%d0                     
    400a:  8c80           	orl %d0,%d6                           
    400c:  2346 000c      	movel %d6,%a1@(12)                    
    4010:  2002           	movel %d2,%d0                         
    4012:  5880           	addql #4,%d0                          
    4014:  e588           	lsll #2,%d0                           
    4016:  2d40 fff0      	movel %d0,%fp@(-16)                   
    401a:  7c00           	moveq #0,%d6                          
    401c:  2044           	moveal %d4,%a0                        
    401e:  d1c0           	addal %d0,%a0                         
    4020:  2648           	moveal %a0,%a3                        
    4022:  3028 0001      	movew %a0@(1),%d0                     
    4026:  e148           	lslw #8,%d0                           
    4028:  7200           	moveq #0,%d1                          
    402a:  3200           	movew %d0,%d1                         
    402c:  8c81           	orl %d1,%d6                           
    402e:  7000           	moveq #0,%d0                          
    4030:  102b 0003      	moveb %a3@(3),%d0                     
    4034:  8c80           	orl %d0,%d6                           
    4036:  2346 0010      	movel %d6,%a1@(16)                    
    403a:  2002           	movel %d2,%d0                         
    403c:  5a80           	addql #5,%d0                          
    403e:  e588           	lsll #2,%d0                           
    4040:  2d40 fff4      	movel %d0,%fp@(-12)                   
    4044:  7c00           	moveq #0,%d6                          
    4046:  2044           	moveal %d4,%a0                        
    4048:  d1c0           	addal %d0,%a0                         
    404a:  2648           	moveal %a0,%a3                        
    404c:  3028 0001      	movew %a0@(1),%d0                     
    4050:  e148           	lslw #8,%d0                           
    4052:  7200           	moveq #0,%d1                          
    4054:  3200           	movew %d0,%d1                         
    4056:  8c81           	orl %d1,%d6                           
    4058:  7000           	moveq #0,%d0                          
    405a:  102b 0003      	moveb %a3@(3),%d0                     
    405e:  8c80           	orl %d0,%d6                           
    4060:  2346 0014      	movel %d6,%a1@(20)                    
    4064:  2002           	movel %d2,%d0                         
    4066:  5c80           	addql #6,%d0                          
    4068:  e588           	lsll #2,%d0                           
    406a:  2d40 fff8      	movel %d0,%fp@(-8)                    
    406e:  7c00           	moveq #0,%d6                          
    4070:  2044           	moveal %d4,%a0                        
    4072:  d1c0           	addal %d0,%a0                         
    4074:  2648           	moveal %a0,%a3                        
    4076:  3028 0001      	movew %a0@(1),%d0                     
    407a:  e148           	lslw #8,%d0                           
    407c:  7200           	moveq #0,%d1                          
    407e:  3200           	movew %d0,%d1                         
    4080:  8c81           	orl %d1,%d6                           
    4082:  7000           	moveq #0,%d0                          
    4084:  102b 0003      	moveb %a3@(3),%d0                     
    4088:  8c80           	orl %d0,%d6                           
    408a:  2346 0018      	movel %d6,%a1@(24)                    
    408e:  7c00           	moveq #0,%d6                          
    4090:  2002           	movel %d2,%d0                         
    4092:  5e80           	addql #7,%d0                          
    4094:  e588           	lsll #2,%d0                           
    4096:  2044           	moveal %d4,%a0                        
    4098:  d1c0           	addal %d0,%a0                         
    409a:  2648           	moveal %a0,%a3                        
    409c:  3028 0001      	movew %a0@(1),%d0                     
    40a0:  e148           	lslw #8,%d0                           
    40a2:  7200           	moveq #0,%d1                          
    40a4:  3200           	movew %d0,%d1                         
    40a6:  8c81           	orl %d1,%d6                           
    40a8:  7000           	moveq #0,%d0                          
    40aa:  102b 0003      	moveb %a3@(3),%d0                     
    40ae:  8c80           	orl %d0,%d6                           
    40b0:  2346 001c      	movel %d6,%a1@(28)                    
    40b4:  7020           	moveq #32,%d0                         
    40b6:  de80           	addl %d0,%d7                          
    40b8:  43e9 0020      	lea %a1@(32),%a1                      
    40bc:  5082           	addql #8,%d2                          
    40be:  b4ae ffe0      	cmpl %fp@(-32),%d2                    
    40c2:  6500 feb0      	bcsw 0x3f74                           ; ↑ label_0
    40c6:  6000 00e0      	braw 0x41a8                           ; ↓ label_12

  label_1:
    40ca:  7c00           	moveq #0,%d6                          
    40cc:  2044           	moveal %d4,%a0                        
    40ce:  d1c7           	addal %d7,%a0                         
    40d0:  2648           	moveal %a0,%a3                        
    40d2:  3028 0001      	movew %a0@(1),%d0                     
    40d6:  e148           	lslw #8,%d0                           
    40d8:  7200           	moveq #0,%d1                          
    40da:  3200           	movew %d0,%d1                         
    40dc:  8c81           	orl %d1,%d6                           
    40de:  7000           	moveq #0,%d0                          
    40e0:  102b 0003      	moveb %a3@(3),%d0                     
    40e4:  8c80           	orl %d0,%d6                           
    40e6:  2286           	movel %d6,%a1@                        
    40e8:  5887           	addql #4,%d7                          
    40ea:  5889           	addql #4,%a1                          
    40ec:  5282           	addql #1,%d2                          

  label_2:
    40ee:  b485           	cmpl %d5,%d2                          
    40f0:  65d8           	bcss 0x40ca                           ; ↑ label_1
    40f2:  6000 00b2      	braw 0x41a6                           ; ↓ label_11

  label_3:
    40f6:  4a84           	tstl %d4                              
    40f8:  664e           	bnes 0x4148                           ; ↓ label_7
    40fa:  7400           	moveq #0,%d2                          
    40fc:  4a85           	tstl %d5                              
    40fe:  6700 00a6      	beqw 0x41a6                           ; ↓ label_11
    4102:  2805           	movel %d5,%d4                         
    4104:  5184           	subql #8,%d4                          
    4106:  7008           	moveq #8,%d0                          
    4108:  ba80           	cmpl %d0,%d5                          
    410a:  6300 00ac      	blsw 0x41b8                           ; ↓ label_13
    410e:  226e ffe4      	moveal %fp@(-28),%a1                  

  label_4:
    4112:  4291           	clrl %a1@                             
    4114:  42a9 0004      	clrl %a1@(4)                          
    4118:  42a9 0008      	clrl %a1@(8)                          
    411c:  42a9 000c      	clrl %a1@(12)                         
    4120:  42a9 0010      	clrl %a1@(16)                         
    4124:  42a9 0014      	clrl %a1@(20)                         
    4128:  42a9 0018      	clrl %a1@(24)                         
    412c:  42a9 001c      	clrl %a1@(28)                         
    4130:  43e9 0020      	lea %a1@(32),%a1                      
    4134:  5082           	addql #8,%d2                          
    4136:  b484           	cmpl %d4,%d2                          
    4138:  65d8           	bcss 0x4112                           ; ↑ label_4
    413a:  607c           	bras 0x41b8                           ; ↓ label_13

  label_5:
    413c:  4291           	clrl %a1@                             
    413e:  5889           	addql #4,%a1                          
    4140:  5282           	addql #1,%d2                          

  label_6:
    4142:  b485           	cmpl %d5,%d2                          
    4144:  65f6           	bcss 0x413c                           ; ↑ label_5
    4146:  605e           	bras 0x41a6                           ; ↓ label_11

  label_7:
    4148:  7400           	moveq #0,%d2                          
    414a:  4a85           	tstl %d5                              
    414c:  6758           	beqs 0x41a6                           ; ↓ label_11
    414e:  2c05           	movel %d5,%d6                         
    4150:  5186           	subql #8,%d6                          
    4152:  7008           	moveq #8,%d0                          
    4154:  ba80           	cmpl %d0,%d5                          
    4156:  636e           	blss 0x41c6                           ; ↓ label_14
    4158:  2244           	moveal %d4,%a1                        
    415a:  266e ffe4      	moveal %fp@(-28),%a3                  

  label_8:
    415e:  2691           	movel %a1@,%a3@                       
    4160:  2769 0004 0004 	movel %a1@(4),%a3@(4)                 
    4166:  2769 0008 0008 	movel %a1@(8),%a3@(8)                 
    416c:  2769 000c 000c 	movel %a1@(12),%a3@(12)               
    4172:  2769 0010 0010 	movel %a1@(16),%a3@(16)               
    4178:  2769 0014 0014 	movel %a1@(20),%a3@(20)               
    417e:  2769 0018 0018 	movel %a1@(24),%a3@(24)               
    4184:  2769 001c 001c 	movel %a1@(28),%a3@(28)               
    418a:  43e9 0020      	lea %a1@(32),%a1                      
    418e:  47eb 0020      	lea %a3@(32),%a3                      
    4192:  5082           	addql #8,%d2                          
    4194:  b486           	cmpl %d6,%d2                          
    4196:  65c6           	bcss 0x415e                           ; ↑ label_8
    4198:  602c           	bras 0x41c6                           ; ↓ label_14

  label_9:
    419a:  2691           	movel %a1@,%a3@                       
    419c:  5889           	addql #4,%a1                          
    419e:  588b           	addql #4,%a3                          
    41a0:  5282           	addql #1,%d2                          

  label_10:
    41a2:  b485           	cmpl %d5,%d2                          
    41a4:  65f4           	bcss 0x419a                           ; ↑ label_9

  label_11:
    41a6:  6030           	bras 0x41d8                           ; ↓ label_15

  label_12:
    41a8:  2e02           	movel %d2,%d7                         
    41aa:  e58f           	lsll #2,%d7                           
    41ac:  2c07           	movel %d7,%d6                         
    41ae:  226e ffe4      	moveal %fp@(-28),%a1                  
    41b2:  d3c6           	addal %d6,%a1                         
    41b4:  6000 ff38      	braw 0x40ee                           ; ↑ label_2

  label_13:
    41b8:  2002           	movel %d2,%d0                         
    41ba:  e588           	lsll #2,%d0                           
    41bc:  226e ffe4      	moveal %fp@(-28),%a1                  
    41c0:  d3c0           	addal %d0,%a1                         
    41c2:  6000 ff7e      	braw 0x4142                           ; ↑ label_6

  label_14:
    41c6:  2002           	movel %d2,%d0                         
    41c8:  e588           	lsll #2,%d0                           
    41ca:  2c00           	movel %d0,%d6                         
    41cc:  2244           	moveal %d4,%a1                        
    41ce:  d3c0           	addal %d0,%a1                         
    41d0:  266e ffe4      	moveal %fp@(-28),%a3                  
    41d4:  d7c6           	addal %d6,%a3                         
    41d6:  60ca           	bras 0x41a2                           ; ↑ label_10

  label_15:
    41d8:  4cdf 08f0      	moveml %sp@+,%d4-%d7/%a3              
    41dc:  4e5e           	unlk %fp                              
    41de:  4e75           	rts                                   
```

# __lmul__: 0x422c - 0x424a

```
    422c:  48e7 3000      	moveml %d2-%d3,%sp@-                  
    4230:  2400           	movel %d0,%d2                         
    4232:  4842           	swap %d2                              
    4234:  c4c1           	muluw %d1,%d2                         
    4236:  2601           	movel %d1,%d3                         
    4238:  4843           	swap %d3                              
    423a:  c6c0           	muluw %d0,%d3                         
    423c:  d443           	addw %d3,%d2                          
    423e:  4842           	swap %d2                              
    4240:  4242           	clrw %d2                              
    4242:  c0c1           	muluw %d1,%d0                         
    4244:  d082           	addl %d2,%d0                          
    4246:  4cdf 000c      	moveml %sp@+,%d2-%d3                  
    424a:  4e75           	rts                                   
```

# anonymous_function_0: 0x4258 - 0x427e

```
    4258:  48e7 3000      	moveml %d2-%d3,%sp@-                  
    425c:  2401           	movel %d1,%d2                         
    425e:  4242           	clrw %d2                              
    4260:  4842           	swap %d2                              
    4262:  661c           	bnes 0x4280                           
    4264:  3600           	movew %d0,%d3                         
    4266:  4240           	clrw %d0                              
    4268:  4840           	swap %d0                              
    426a:  6706           	beqs 0x4272                           ; ↓ label_0
    426c:  80c1           	divuw %d1,%d0                         
    426e:  3400           	movew %d0,%d2                         
    4270:  4842           	swap %d2                              

  label_0:
    4272:  3003           	movew %d3,%d0                         
    4274:  80c1           	divuw %d1,%d0                         
    4276:  3400           	movew %d0,%d2                         
    4278:  2002           	movel %d2,%d0                         
    427a:  4cdf 000c      	moveml %sp@+,%d2-%d3                  
    427e:  4e75           	rts                                   
```

# anonymous_function_1: 0x42b0 - 0x42d6

```
    42b0:  48e7 3000      	moveml %d2-%d3,%sp@-                  
    42b4:  2401           	movel %d1,%d2                         
    42b6:  4242           	clrw %d2                              
    42b8:  4842           	swap %d2                              
    42ba:  661c           	bnes 0x42d8                           
    42bc:  3600           	movew %d0,%d3                         
    42be:  4240           	clrw %d0                              
    42c0:  4840           	swap %d0                              
    42c2:  6706           	beqs 0x42ca                           ; ↓ label_0
    42c4:  80c1           	divuw %d1,%d0                         
    42c6:  3400           	movew %d0,%d2                         
    42c8:  4842           	swap %d2                              

  label_0:
    42ca:  3003           	movew %d3,%d0                         
    42cc:  80c1           	divuw %d1,%d0                         
    42ce:  4240           	clrw %d0                              
    42d0:  4840           	swap %d0                              
    42d2:  4cdf 000c      	moveml %sp@+,%d2-%d3                  
    42d6:  4e75           	rts                                   
```

# PrvDspDoneProc: 0x4304 - 0x4354

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    4304:  4e56 0000      	linkw %fp,#0                          
    4308:  202e 0008      	movel %fp@(8),%d0                     
    430c:  4a2e 000c      	tstb %fp@(12)                         
    4310:  673a           	beqs 0x434c                           ; ↓ label_0
    4312:  2040           	moveal %d0,%a0                        
    4314:  30b9 1100 0c06 	movew 0x11000c06,%a0@                 
    431a:  3179 1100 0c14 	movew 0x11000c14,%a0@(2)              
    4320:  0002                                                  
    4322:  3179 1100 0c16 	movew 0x11000c16,%a0@(4)              
    4328:  0004                                                  
    432a:  3179 1100 0c18 	movew 0x11000c18,%a0@(6)              
    4330:  0006                                                  
    4332:  3179 1100 0c1a 	movew 0x11000c1a,%a0@(8)              
    4338:  0008                                                  
    433a:  3179 1100 0c1c 	movew 0x11000c1c,%a0@(10)             
    4340:  000a                                                  
    4342:  3179 1100 0c1e 	movew 0x11000c1e,%a0@(12)             
    4348:  000c                                                  
    434a:  6006           	bras 0x4352                           ; ↓ label_1

  label_0:
    434c:  2040           	moveal %d0,%a0                        
    434e:  30bc 03ff      	movew #1023,%a0@                      

  label_1:
    4352:  4e5e           	unlk %fp                              
    4354:  4e75           	rts                                   
```

# IOJobCheckStickType: 0x4368 - 0x43fe

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-1)`     : ???

```
    4368:  4e56 fffe      	linkw %fp,#-2                         
    436c:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    4370:  266e 0008      	moveal %fp@(8),%a3                    
    4374:  246e 000c      	moveal %fp@(12),%a2                   
    4378:  0c2b 0001 002a 	cmpib #1,%a3@(42)                     
    437e:  6608           	bnes 0x4388                           ; ↓ label_0
    4380:  14bc 0001      	moveb #1,%a2@                         
    4384:  7000           	moveq #0,%d0                          
    4386:  6070           	bras 0x43f8                           ; ↓ label_7

  label_0:
    4388:  2f0b           	movel %a3,%sp@-                       
    438a:  4eba 1374      	jsr %pc@(0x5700)                      ; PrvIOSupplyPower
    438e:  2f3c 0003 0d40 	movel #200000,%sp@-                   
    4394:  4e4f           	trap #15                              
    4396:  a249           	0121111                               ; sysTrapHwrDelay
    4398:  7601           	moveq #1,%d3                          
    439a:  504f           	addqw #8,%sp                          

  label_1:
    439c:  486e ffff      	pea %fp@(-1)                          
    43a0:  1f03           	moveb %d3,%sp@-                       
    43a2:  4eba 12cc      	jsr %pc@(0x5670)                      ; PrvIODSPControlMS
    43a6:  1800           	moveb %d0,%d4                         
    43a8:  5c4f           	addqw #6,%sp                          
    43aa:  670c           	beqs 0x43b8                           ; ↓ label_2
    43ac:  0c04 0003      	cmpib #3,%d4                          
    43b0:  6724           	beqs 0x43d6                           ; ↓ label_5
    43b2:  303c 8100      	movew #-32512,%d0                     
    43b6:  6040           	bras 0x43f8                           ; ↓ label_7

  label_2:
    43b8:  14bc 0001      	moveb #1,%a2@                         
    43bc:  486e ffff      	pea %fp@(-1)                          
    43c0:  4227           	clrb %sp@-                            
    43c2:  4eba 12ac      	jsr %pc@(0x5670)                      ; PrvIODSPControlMS
    43c6:  1600           	moveb %d0,%d3                         
    43c8:  5c4f           	addqw #6,%sp                          
    43ca:  6706           	beqs 0x43d2                           ; ↓ label_3
    43cc:  303c 8100      	movew #-32512,%d0                     
    43d0:  6002           	bras 0x43d4                           ; ↓ label_4

  label_3:
    43d2:  7000           	moveq #0,%d0                          

  label_4:
    43d4:  6022           	bras 0x43f8                           ; ↓ label_7

  label_5:
    43d6:  5203           	addqb #1,%d3                          
    43d8:  0c03 00ff      	cmpib #-1,%d3                         
    43dc:  65be           	bcss 0x439c                           ; ↑ label_1
    43de:  486e ffff      	pea %fp@(-1)                          
    43e2:  4227           	clrb %sp@-                            
    43e4:  4eba 128a      	jsr %pc@(0x5670)                      ; PrvIODSPControlMS
    43e8:  1600           	moveb %d0,%d3                         
    43ea:  5c4f           	addqw #6,%sp                          
    43ec:  6706           	beqs 0x43f4                           ; ↓ label_6
    43ee:  303c 8100      	movew #-32512,%d0                     
    43f2:  6004           	bras 0x43f8                           ; ↓ label_7

  label_6:
    43f4:  4212           	clrb %a2@                             
    43f6:  7000           	moveq #0,%d0                          

  label_7:
    43f8:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    43fc:  4e5e           	unlk %fp                              
    43fe:  4e75           	rts                                   
```

# IOJobFunctionCheck: 0x4416 - 0x44d6

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???

```
    4416:  4e56 0000      	linkw %fp,#0                          
    441a:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    441e:  246e 0008      	moveal %fp@(8),%a2                    
    4422:  162e 000e      	moveb %fp@(14),%d3                    
    4426:  266e 0010      	moveal %fp@(16),%a3                   
    442a:  4a2e 000c      	tstb %fp@(12)                         
    442e:  6708           	beqs 0x4438                           ; ↓ label_0
    4430:  303c 4001      	movew #16385,%d0                      
    4434:  6000 009a      	braw 0x44d0                           ; ↓ label_9

  label_0:
    4438:  4a03           	tstb %d3                              
    443a:  6706           	beqs 0x4442                           ; ↓ label_1
    443c:  0c03 00ff      	cmpib #-1,%d3                         
    4440:  6608           	bnes 0x444a                           ; ↓ label_2

  label_1:
    4442:  303c 4002      	movew #16386,%d0                      
    4446:  6000 0088      	braw 0x44d0                           ; ↓ label_9

  label_2:
    444a:  4a2a 002b      	tstb %a2@(43)                         
    444e:  671c           	beqs 0x446c                           ; ↓ label_4
    4450:  1f03           	moveb %d3,%sp@-                       
    4452:  2f0a           	movel %a2,%sp@-                       
    4454:  4eba 1304      	jsr %pc@(0x575a)                      ; IsFunctionEnable
    4458:  4a00           	tstb %d0                              
    445a:  5c4f           	addqw #6,%sp                          
    445c:  6708           	beqs 0x4466                           ; ↓ label_3
    445e:  16aa 002e      	moveb %a2@(46),%a3@                   
    4462:  7000           	moveq #0,%d0                          
    4464:  606a           	bras 0x44d0                           ; ↓ label_9

  label_3:
    4466:  303c 8004      	movew #-32764,%d0                     
    446a:  6064           	bras 0x44d0                           ; ↓ label_9

  label_4:
    446c:  202a 0004      	movel %a2@(4),%d0                     
    4470:  0280 0000 0080 	andil #128,%d0                        
    4476:  6612           	bnes 0x448a                           ; ↓ label_5
    4478:  2f0a           	movel %a2,%sp@-                       
    447a:  4eba 1284      	jsr %pc@(0x5700)                      ; PrvIOSupplyPower
    447e:  2f3c 0003 0d40 	movel #200000,%sp@-                   
    4484:  4e4f           	trap #15                              
    4486:  a249           	0121111                               ; sysTrapHwrDelay
    4488:  504f           	addqw #8,%sp                          

  label_5:
    448a:  4a2a 002a      	tstb %a2@(42)                         
    448e:  6616           	bnes 0x44a6                           ; ↓ label_7
    4490:  1f03           	moveb %d3,%sp@-                       
    4492:  4eba 1178      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    4496:  3800           	movew %d0,%d4                         
    4498:  544f           	addqw #2,%sp                          
    449a:  6704           	beqs 0x44a0                           ; ↓ label_6
    449c:  3004           	movew %d4,%d0                         
    449e:  6030           	bras 0x44d0                           ; ↓ label_9

  label_6:
    44a0:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_7:
    44a6:  486a 0030      	pea %a2@(48)                          
    44aa:  2f0b           	movel %a3,%sp@-                       
    44ac:  1f03           	moveb %d3,%sp@-                       
    44ae:  2f0a           	movel %a2,%sp@-                       
    44b0:  4eba 12de      	jsr %pc@(0x5790)                      ; PrvCheckFunction
    44b4:  3800           	movew %d0,%d4                         
    44b6:  4fef 000e      	lea %sp@(14),%sp                      
    44ba:  6704           	beqs 0x44c0                           ; ↓ label_8
    44bc:  3004           	movew %d4,%d0                         
    44be:  6010           	bras 0x44d0                           ; ↓ label_9

  label_8:
    44c0:  1553 002e      	moveb %a3@,%a2@(46)                   
    44c4:  2f0a           	movel %a2,%sp@-                       
    44c6:  4eba 10c8      	jsr %pc@(0x5590)                      ; ReturnToMS
    44ca:  3600           	movew %d0,%d3                         
    44cc:  3003           	movew %d3,%d0                         
    44ce:  584f           	addqw #4,%sp                          

  label_9:
    44d0:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    44d4:  4e5e           	unlk %fp                              
    44d6:  4e75           	rts                                   
```

# IOJobGetAttribute: 0x44ee - 0x476c

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???
   * `%fp@(-5)`     : ???
   * `%fp@(-14)`    : ???

```
    44ee:  4e56 fff2      	linkw %fp,#-14                        
    44f2:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    44f6:  266e 0008      	moveal %fp@(8),%a3                    
    44fa:  162e 000e      	moveb %fp@(14),%d3                    
    44fe:  282e 0010      	movel %fp@(16),%d4                    
    4502:  7a00           	moveq #0,%d5                          
    4504:  4a2e 000c      	tstb %fp@(12)                         
    4508:  6708           	beqs 0x4512                           ; ↓ label_0
    450a:  303c 4001      	movew #16385,%d0                      
    450e:  6000 0256      	braw 0x4766                           ; ↓ label_25

  label_0:
    4512:  4a03           	tstb %d3                              
    4514:  6706           	beqs 0x451c                           ; ↓ label_1
    4516:  0c03 00ff      	cmpib #-1,%d3                         
    451a:  6608           	bnes 0x4524                           ; ↓ label_2

  label_1:
    451c:  303c 4002      	movew #16386,%d0                      
    4520:  6000 0244      	braw 0x4766                           ; ↓ label_25

  label_2:
    4524:  1f03           	moveb %d3,%sp@-                       
    4526:  2f0b           	movel %a3,%sp@-                       
    4528:  4eba 1230      	jsr %pc@(0x575a)                      ; IsFunctionEnable
    452c:  4a00           	tstb %d0                              
    452e:  5c4f           	addqw #6,%sp                          
    4530:  6708           	beqs 0x453a                           ; ↓ label_3
    4532:  303c 8008      	movew #-32760,%d0                     
    4536:  6000 022e      	braw 0x4766                           ; ↓ label_25

  label_3:
    453a:  202b 0004      	movel %a3@(4),%d0                     
    453e:  0280 0000 0080 	andil #128,%d0                        
    4544:  6612           	bnes 0x4558                           ; ↓ label_4
    4546:  2f0b           	movel %a3,%sp@-                       
    4548:  4eba 11b6      	jsr %pc@(0x5700)                      ; PrvIOSupplyPower
    454c:  2f3c 0003 0d40 	movel #200000,%sp@-                   
    4552:  4e4f           	trap #15                              
    4554:  a249           	0121111                               ; sysTrapHwrDelay
    4556:  504f           	addqw #8,%sp                          

  label_4:
    4558:  4a2b 002a      	tstb %a3@(42)                         
    455c:  6632           	bnes 0x4590                           ; ↓ label_7
    455e:  1f03           	moveb %d3,%sp@-                       
    4560:  4eba 10aa      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    4564:  3c00           	movew %d0,%d6                         
    4566:  544f           	addqw #2,%sp                          
    4568:  6706           	beqs 0x4570                           ; ↓ label_5
    456a:  3006           	movew %d6,%d0                         
    456c:  6000 01f8      	braw 0x4766                           ; ↓ label_25

  label_5:
    4570:  486b 0030      	pea %a3@(48)                          
    4574:  1f03           	moveb %d3,%sp@-                       
    4576:  2f0b           	movel %a3,%sp@-                       
    4578:  4eba 12be      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    457c:  3c00           	movew %d0,%d6                         
    457e:  4fef 000a      	lea %sp@(10),%sp                      
    4582:  6706           	beqs 0x458a                           ; ↓ label_6
    4584:  3006           	movew %d6,%d0                         
    4586:  6000 01de      	braw 0x4766                           ; ↓ label_25

  label_6:
    458a:  177c 0001 002a 	moveb #1,%a3@(42)                     

  label_7:
    4590:  4878 1388      	pea 0x1388                            
    4594:  4e4f           	trap #15                              
    4596:  a249           	0121111                               ; sysTrapHwrDelay
    4598:  486b 0030      	pea %a3@(48)                          
    459c:  2f2b 003a      	movel %a3@(58),%sp@-                  
    45a0:  1f3c 000f      	moveb #15,%sp@-                       
    45a4:  1f3c 0010      	moveb #16,%sp@-                       
    45a8:  1f3c 000b      	moveb #11,%sp@-                       
    45ac:  4227           	clrb %sp@-                            
    45ae:  4eba 141a      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    45b2:  3600           	movew %d0,%d3                         
    45b4:  4fef 0014      	lea %sp@(20),%sp                      
    45b8:  6706           	beqs 0x45c0                           ; ↓ label_8
    45ba:  3003           	movew %d3,%d0                         
    45bc:  6000 01a8      	braw 0x4766                           ; ↓ label_25

  label_8:
    45c0:  486b 0030      	pea %a3@(48)                          
    45c4:  42a7           	clrl %sp@-                            
    45c6:  1f3c 000b      	moveb #11,%sp@-                       
    45ca:  486e fff2      	pea %fp@(-14)                         
    45ce:  4eba 1490      	jsr %pc@(0x5a60)                      ; PrvTpcReadReg
    45d2:  3600           	movew %d0,%d3                         
    45d4:  4fef 000e      	lea %sp@(14),%sp                      
    45d8:  6706           	beqs 0x45e0                           ; ↓ label_9
    45da:  3003           	movew %d3,%d0                         
    45dc:  6000 0188      	braw 0x4766                           ; ↓ label_25

  label_9:
    45e0:  7000           	moveq #0,%d0                          
    45e2:  102e fffc      	moveb %fp@(-4),%d0                    
    45e6:  7e00           	moveq #0,%d7                          
    45e8:  1e2e fffb      	moveb %fp@(-5),%d7                    
    45ec:  e14f           	lslw #8,%d7                           
    45ee:  8e40           	orw %d0,%d7                           
    45f0:  3607           	movew %d7,%d3                         
    45f2:  6608           	bnes 0x45fc                           ; ↓ label_10
    45f4:  303c 8005      	movew #-32763,%d0                     
    45f8:  6000 016c      	braw 0x4766                           ; ↓ label_25

  label_10:
    45fc:  7000           	moveq #0,%d0                          
    45fe:  3007           	movew %d7,%d0                         
    4600:  2f00           	movel %d0,%sp@-                       
    4602:  4e4f           	trap #15                              
    4604:  a013           	0120023                               ; sysTrapMemPtrNew
    4606:  2448           	moveal %a0,%a2                        
    4608:  200a           	movel %a2,%d0                         
    460a:  584f           	addqw #4,%sp                          
    460c:  6608           	bnes 0x4616                           ; ↓ label_11
    460e:  303c 8102      	movew #-32510,%d0                     
    4612:  6000 0152      	braw 0x4766                           ; ↓ label_25

  label_11:
    4616:  486b 0030      	pea %a3@(48)                          
    461a:  42a7           	clrl %sp@-                            
    461c:  3f07           	movew %d7,%sp@-                       
    461e:  2f0a           	movel %a2,%sp@-                       
    4620:  4eba 159c      	jsr %pc@(0x5bbe)                      ; PrvTpcReadData
    4624:  3600           	movew %d0,%d3                         
    4626:  4fef 000e      	lea %sp@(14),%sp                      
    462a:  6758           	beqs 0x4684                           ; ↓ label_15
    462c:  3003           	movew %d3,%d0                         
    462e:  0240 1000      	andiw #4096,%d0                       
    4632:  670e           	beqs 0x4642                           ; ↓ label_12
    4634:  2f0a           	movel %a2,%sp@-                       
    4636:  4e4f           	trap #15                              
    4638:  a012           	0120022                               ; sysTrapMemChunkFree
    463a:  3003           	movew %d3,%d0                         
    463c:  584f           	addqw #4,%sp                          
    463e:  6000 0126      	braw 0x4766                           ; ↓ label_25

  label_12:
    4642:  7601           	moveq #1,%d3                          
    4644:  0c47 0001      	cmpiw #1,%d7                          
    4648:  633a           	blss 0x4684                           ; ↓ label_15

  label_13:
    464a:  486b 0030      	pea %a3@(48)                          
    464e:  4878 0064      	pea 0x64                              
    4652:  3f3c 0001      	movew #1,%sp@-                        
    4656:  486e fffe      	pea %fp@(-2)                          
    465a:  4eba 1562      	jsr %pc@(0x5bbe)                      ; PrvTpcReadData
    465e:  3c00           	movew %d0,%d6                         
    4660:  4fef 000e      	lea %sp@(14),%sp                      
    4664:  670e           	beqs 0x4674                           ; ↓ label_14
    4666:  2f0a           	movel %a2,%sp@-                       
    4668:  4e4f           	trap #15                              
    466a:  a012           	0120022                               ; sysTrapMemChunkFree
    466c:  3006           	movew %d6,%d0                         
    466e:  584f           	addqw #4,%sp                          
    4670:  6000 00f4      	braw 0x4766                           ; ↓ label_25

  label_14:
    4674:  7000           	moveq #0,%d0                          
    4676:  3003           	movew %d3,%d0                         
    4678:  15ae fffe 0800 	moveb %fp@(-2),%a2@(00000000,%d0:l)   
    467e:  5243           	addqw #1,%d3                          
    4680:  b647           	cmpw %d7,%d3                          
    4682:  65c6           	bcss 0x464a                           ; ↑ label_13

  label_15:
    4684:  2f0b           	movel %a3,%sp@-                       
    4686:  4eba 10a8      	jsr %pc@(0x5730)                      ; PrvIOCutPower
    468a:  264a           	moveal %a2,%a3                        
    468c:  584f           	addqw #4,%sp                          

  label_16:
    468e:  1612           	moveb %a2@,%d3                        
    4690:  0c03 0010      	cmpib #16,%d3                         
    4694:  6638           	bnes 0x46ce                           ; ↓ label_18
    4696:  7600           	moveq #0,%d3                          
    4698:  162a 0001      	moveb %a2@(1),%d3                     
    469c:  3c03           	movew %d3,%d6                         
    469e:  0c46 0020      	cmpiw #32,%d6                         
    46a2:  6310           	blss 0x46b4                           ; ↓ label_17
    46a4:  2f0b           	movel %a3,%sp@-                       
    46a6:  4e4f           	trap #15                              
    46a8:  a012           	0120022                               ; sysTrapMemChunkFree
    46aa:  303c 8006      	movew #-32762,%d0                     
    46ae:  584f           	addqw #4,%sp                          
    46b0:  6000 00b4      	braw 0x4766                           ; ↓ label_25

  label_17:
    46b4:  548a           	addql #2,%a2                          
    46b6:  3f03           	movew %d3,%sp@-                       
    46b8:  2f0a           	movel %a2,%sp@-                       
    46ba:  2f04           	movel %d4,%sp@-                       
    46bc:  4e4f           	trap #15                              
    46be:  a2ce           	0121316                               ; sysTrapStrNCopy
    46c0:  7000           	moveq #0,%d0                          
    46c2:  3003           	movew %d3,%d0                         
    46c4:  d5c0           	addal %d0,%a2                         
    46c6:  4fef 000a      	lea %sp@(10),%sp                      
    46ca:  6000 008a      	braw 0x4756                           ; ↓ label_24

  label_18:
    46ce:  0c03 0011      	cmpib #17,%d3                         
    46d2:  6638           	bnes 0x470c                           ; ↓ label_20
    46d4:  7600           	moveq #0,%d3                          
    46d6:  162a 0001      	moveb %a2@(1),%d3                     
    46da:  3c03           	movew %d3,%d6                         
    46dc:  0c46 0020      	cmpiw #32,%d6                         
    46e0:  630e           	blss 0x46f0                           ; ↓ label_19
    46e2:  2f0b           	movel %a3,%sp@-                       
    46e4:  4e4f           	trap #15                              
    46e6:  a012           	0120022                               ; sysTrapMemChunkFree
    46e8:  303c 8006      	movew #-32762,%d0                     
    46ec:  584f           	addqw #4,%sp                          
    46ee:  6076           	bras 0x4766                           ; ↓ label_25

  label_19:
    46f0:  548a           	addql #2,%a2                          
    46f2:  3f03           	movew %d3,%sp@-                       
    46f4:  2f0a           	movel %a2,%sp@-                       
    46f6:  2044           	moveal %d4,%a0                        
    46f8:  4868 0021      	pea %a0@(33)                          
    46fc:  4e4f           	trap #15                              
    46fe:  a2ce           	0121316                               ; sysTrapStrNCopy
    4700:  7000           	moveq #0,%d0                          
    4702:  3003           	movew %d3,%d0                         
    4704:  d5c0           	addal %d0,%a2                         
    4706:  4fef 000a      	lea %sp@(10),%sp                      
    470a:  604a           	bras 0x4756                           ; ↓ label_24

  label_20:
    470c:  0c03 0012      	cmpib #18,%d3                         
    4710:  6638           	bnes 0x474a                           ; ↓ label_22
    4712:  7600           	moveq #0,%d3                          
    4714:  162a 0001      	moveb %a2@(1),%d3                     
    4718:  3c03           	movew %d3,%d6                         
    471a:  0c46 0010      	cmpiw #16,%d6                         
    471e:  630e           	blss 0x472e                           ; ↓ label_21
    4720:  2f0b           	movel %a3,%sp@-                       
    4722:  4e4f           	trap #15                              
    4724:  a012           	0120022                               ; sysTrapMemChunkFree
    4726:  303c 8006      	movew #-32762,%d0                     
    472a:  584f           	addqw #4,%sp                          
    472c:  6038           	bras 0x4766                           ; ↓ label_25

  label_21:
    472e:  548a           	addql #2,%a2                          
    4730:  3f03           	movew %d3,%sp@-                       
    4732:  2f0a           	movel %a2,%sp@-                       
    4734:  2044           	moveal %d4,%a0                        
    4736:  4868 0042      	pea %a0@(66)                          
    473a:  4e4f           	trap #15                              
    473c:  a2ce           	0121316                               ; sysTrapStrNCopy
    473e:  7000           	moveq #0,%d0                          
    4740:  3003           	movew %d3,%d0                         
    4742:  d5c0           	addal %d0,%a2                         
    4744:  4fef 000a      	lea %sp@(10),%sp                      
    4748:  600c           	bras 0x4756                           ; ↓ label_24

  label_22:
    474a:  0c03 00ff      	cmpib #-1,%d3                         
    474e:  6604           	bnes 0x4754                           ; ↓ label_23
    4750:  7a01           	moveq #1,%d5                          
    4752:  6002           	bras 0x4756                           ; ↓ label_24

  label_23:
    4754:  528a           	addql #1,%a2                          

  label_24:
    4756:  4a05           	tstb %d5                              
    4758:  6700 ff34      	beqw 0x468e                           ; ↑ label_16
    475c:  2f0b           	movel %a3,%sp@-                       
    475e:  4e4f           	trap #15                              
    4760:  a012           	0120022                               ; sysTrapMemChunkFree
    4762:  7000           	moveq #0,%d0                          
    4764:  584f           	addqw #4,%sp                          

  label_25:
    4766:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    476a:  4e5e           	unlk %fp                              
    476c:  4e75           	rts                                   
```

# IOJobCheckFunctionPower: 0x4782 - 0x47b0

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???

```
    4782:  4e56 0000      	linkw %fp,#0                          
    4786:  206e 0008      	moveal %fp@(8),%a0                    
    478a:  4a28 002b      	tstb %a0@(43)                         
    478e:  671c           	beqs 0x47ac                           ; ↓ label_1
    4790:  1028 002d      	moveb %a0@(45),%d0                    
    4794:  b02e 000e      	cmpb %fp@(14),%d0                     
    4798:  660e           	bnes 0x47a8                           ; ↓ label_0
    479a:  1028 002c      	moveb %a0@(44),%d0                    
    479e:  b02e 000c      	cmpb %fp@(12),%d0                     
    47a2:  6604           	bnes 0x47a8                           ; ↓ label_0
    47a4:  7001           	moveq #1,%d0                          
    47a6:  6006           	bras 0x47ae                           ; ↓ label_2

  label_0:
    47a8:  7000           	moveq #0,%d0                          
    47aa:  6002           	bras 0x47ae                           ; ↓ label_2

  label_1:
    47ac:  7000           	moveq #0,%d0                          

  label_2:
    47ae:  4e5e           	unlk %fp                              
    47b0:  4e75           	rts                                   
```

# IOJobFunctionEnable: 0x47cc - 0x4942

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

Locals:

   * `%fp@(-1)`     : ???

```
    47cc:  4e56 fffe      	linkw %fp,#-2                         
    47d0:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    47d4:  246e 0008      	moveal %fp@(8),%a2                    
    47d8:  162e 000e      	moveb %fp@(14),%d3                    
    47dc:  4a2e 000c      	tstb %fp@(12)                         
    47e0:  6708           	beqs 0x47ea                           ; ↓ label_0
    47e2:  303c 4001      	movew #16385,%d0                      
    47e6:  6000 0154      	braw 0x493c                           ; ↓ label_15

  label_0:
    47ea:  4a03           	tstb %d3                              
    47ec:  6706           	beqs 0x47f4                           ; ↓ label_1
    47ee:  0c03 00ff      	cmpib #-1,%d3                         
    47f2:  6608           	bnes 0x47fc                           ; ↓ label_2

  label_1:
    47f4:  303c 4002      	movew #16386,%d0                      
    47f8:  6000 0142      	braw 0x493c                           ; ↓ label_15

  label_2:
    47fc:  1f03           	moveb %d3,%sp@-                       
    47fe:  2f0a           	movel %a2,%sp@-                       
    4800:  4eba 0f58      	jsr %pc@(0x575a)                      ; IsFunctionEnable
    4804:  4a00           	tstb %d0                              
    4806:  5c4f           	addqw #6,%sp                          
    4808:  6706           	beqs 0x4810                           ; ↓ label_3
    480a:  7000           	moveq #0,%d0                          
    480c:  6000 012e      	braw 0x493c                           ; ↓ label_15

  label_3:
    4810:  202a 0004      	movel %a2@(4),%d0                     
    4814:  0280 0000 0080 	andil #128,%d0                        
    481a:  6612           	bnes 0x482e                           ; ↓ label_4
    481c:  2f0a           	movel %a2,%sp@-                       
    481e:  4eba 0ee0      	jsr %pc@(0x5700)                      ; PrvIOSupplyPower
    4822:  2f3c 0003 0d40 	movel #200000,%sp@-                   
    4828:  4e4f           	trap #15                              
    482a:  a249           	0121111                               ; sysTrapHwrDelay
    482c:  504f           	addqw #8,%sp                          

  label_4:
    482e:  4a2a 002a      	tstb %a2@(42)                         
    4832:  6632           	bnes 0x4866                           ; ↓ label_7
    4834:  1f03           	moveb %d3,%sp@-                       
    4836:  4eba 0dd4      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    483a:  3800           	movew %d0,%d4                         
    483c:  544f           	addqw #2,%sp                          
    483e:  6706           	beqs 0x4846                           ; ↓ label_5
    4840:  3004           	movew %d4,%d0                         
    4842:  6000 00f8      	braw 0x493c                           ; ↓ label_15

  label_5:
    4846:  486a 0030      	pea %a2@(48)                          
    484a:  1f03           	moveb %d3,%sp@-                       
    484c:  2f0a           	movel %a2,%sp@-                       
    484e:  4eba 0fe8      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    4852:  3800           	movew %d0,%d4                         
    4854:  4fef 000a      	lea %sp@(10),%sp                      
    4858:  6706           	beqs 0x4860                           ; ↓ label_6
    485a:  3004           	movew %d4,%d0                         
    485c:  6000 00de      	braw 0x493c                           ; ↓ label_15

  label_6:
    4860:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_7:
    4866:  486a 0030      	pea %a2@(48)                          
    486a:  42a7           	clrl %sp@-                            
    486c:  1f3c 0001      	moveb #1,%sp@-                        
    4870:  1f3c 0008      	moveb #8,%sp@-                        
    4874:  1f3c 0001      	moveb #1,%sp@-                        
    4878:  1f3c 0008      	moveb #8,%sp@-                        
    487c:  4eba 114c      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    4880:  3800           	movew %d0,%d4                         
    4882:  4fef 0010      	lea %sp@(16),%sp                      
    4886:  6706           	beqs 0x488e                           ; ↓ label_8
    4888:  3004           	movew %d4,%d0                         
    488a:  6000 00b0      	braw 0x493c                           ; ↓ label_15

  label_8:
    488e:  486a 0030      	pea %a2@(48)                          
    4892:  42a7           	clrl %sp@-                            
    4894:  486e ffff      	pea %fp@(-1)                          
    4898:  4eba 1234      	jsr %pc@(0x5ace)                      ; PrvTpcRead_1_Reg
    489c:  3800           	movew %d0,%d4                         
    489e:  4fef 000c      	lea %sp@(12),%sp                      
    48a2:  6706           	beqs 0x48aa                           ; ↓ label_9
    48a4:  3004           	movew %d4,%d0                         
    48a6:  6000 0094      	braw 0x493c                           ; ↓ label_15

  label_9:
    48aa:  0cb9 b6bc 650c 	cmpil #-1229167348,0x10000052         
    48b0:  1000 0052                                             
    48b4:  6718           	beqs 0x48ce                           ; ↓ label_10
    48b6:  0cb9 b704 a99a 	cmpil #-1224431206,0x10000052         
    48bc:  1000 0052                                             
    48c0:  670c           	beqs 0x48ce                           ; ↓ label_10
    48c2:  0cb9 b737 eb95 	cmpil #-1221071979,0x10000052         
    48c8:  1000 0052                                             
    48cc:  6604           	bnes 0x48d2                           ; ↓ label_11

  label_10:
    48ce:  7841           	moveq #65,%d4                         
    48d0:  6004           	bras 0x48d6                           ; ↓ label_12

  label_11:
    48d2:  383c 012c      	movew #300,%d4                        

  label_12:
    48d6:  7000           	moveq #0,%d0                          
    48d8:  102e ffff      	moveb %fp@(-1),%d0                    
    48dc:  c1fc 000a      	mulsw #10,%d0                         
    48e0:  b044           	cmpw %d4,%d0                          
    48e2:  6306           	blss 0x48ea                           ; ↓ label_13
    48e4:  303c 800c      	movew #-32756,%d0                     
    48e8:  6052           	bras 0x493c                           ; ↓ label_15

  label_13:
    48ea:  486a 0030      	pea %a2@(48)                          
    48ee:  42a7           	clrl %sp@-                            
    48f0:  1f3c 00c0      	moveb #-64,%sp@-                      
    48f4:  4eba 1292      	jsr %pc@(0x5b88)                      ; PrvTpcWrite_1_Reg
    48f8:  3800           	movew %d0,%d4                         
    48fa:  4fef 000a      	lea %sp@(10),%sp                      
    48fe:  6704           	beqs 0x4904                           ; ↓ label_14
    4900:  3004           	movew %d4,%d0                         
    4902:  6038           	bras 0x493c                           ; ↓ label_15

  label_14:
    4904:  156e 000c 002c 	moveb %fp@(12),%a2@(44)               
    490a:  1543 002d      	moveb %d3,%a2@(45)                    
    490e:  157c 0001 002b 	moveb #1,%a2@(43)                     
    4914:  0279 fffd 1100 	andiw #-3,0x11000222                  
    491a:  0222                                                  
    491c:  0079 0080 1100 	oriw #128,0x11000406                  
    4922:  0406                                                  
    4924:  256e 0010 0032 	movel %fp@(16),%a2@(50)               
    492a:  256e 0014 0036 	movel %fp@(20),%a2@(54)               
    4930:  022a 00fe 002f 	andib #-2,%a2@(47)                    
    4936:  42aa 0042      	clrl %a2@(66)                         
    493a:  7000           	moveq #0,%d0                          

  label_15:
    493c:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    4940:  4e5e           	unlk %fp                              
    4942:  4e75           	rts                                   
```

# !IOJobFunctionEnableNoCurrentC: 0x495a - 0x4aa0

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

```
    495a:  4e56 0000      	linkw %fp,#0                          
    495e:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    4962:  246e 0008      	moveal %fp@(8),%a2                    
    4966:  182e 000e      	moveb %fp@(14),%d4                    
    496a:  7600           	moveq #0,%d3                          
    496c:  0cb9 b6bc 650c 	cmpil #-1229167348,0x10000052         
    4972:  1000 0052                                             
    4976:  6720           	beqs 0x4998                           ; ↓ label_0
    4978:  0cb9 b704 a99a 	cmpil #-1224431206,0x10000052         
    497e:  1000 0052                                             
    4982:  6714           	beqs 0x4998                           ; ↓ label_0
    4984:  0cb9 b737 eb95 	cmpil #-1221071979,0x10000052         
    498a:  1000 0052                                             
    498e:  6708           	beqs 0x4998                           ; ↓ label_0
    4990:  303c 2901      	movew #10497,%d0                      
    4994:  6000 0104      	braw 0x4a9a                           ; ↓ label_11

  label_0:
    4998:  4a2e 000c      	tstb %fp@(12)                         
    499c:  6708           	beqs 0x49a6                           ; ↓ label_1
    499e:  303c 4001      	movew #16385,%d0                      
    49a2:  6000 00f6      	braw 0x4a9a                           ; ↓ label_11

  label_1:
    49a6:  4a04           	tstb %d4                              
    49a8:  6706           	beqs 0x49b0                           ; ↓ label_2
    49aa:  0c04 00ff      	cmpib #-1,%d4                         
    49ae:  6608           	bnes 0x49b8                           ; ↓ label_3

  label_2:
    49b0:  303c 4002      	movew #16386,%d0                      
    49b4:  6000 00e4      	braw 0x4a9a                           ; ↓ label_11

  label_3:
    49b8:  1f04           	moveb %d4,%sp@-                       
    49ba:  2f0a           	movel %a2,%sp@-                       
    49bc:  4eba 0d9c      	jsr %pc@(0x575a)                      ; IsFunctionEnable
    49c0:  4a00           	tstb %d0                              
    49c2:  5c4f           	addqw #6,%sp                          
    49c4:  6706           	beqs 0x49cc                           ; ↓ label_4
    49c6:  7000           	moveq #0,%d0                          
    49c8:  6000 00d0      	braw 0x4a9a                           ; ↓ label_11

  label_4:
    49cc:  202a 0004      	movel %a2@(4),%d0                     
    49d0:  0280 0000 0080 	andil #128,%d0                        
    49d6:  6612           	bnes 0x49ea                           ; ↓ label_5
    49d8:  2f0a           	movel %a2,%sp@-                       
    49da:  4eba 0d24      	jsr %pc@(0x5700)                      ; PrvIOSupplyPower
    49de:  2f3c 0003 0d40 	movel #200000,%sp@-                   
    49e4:  4e4f           	trap #15                              
    49e6:  a249           	0121111                               ; sysTrapHwrDelay
    49e8:  504f           	addqw #8,%sp                          

  label_5:
    49ea:  4a2a 002a      	tstb %a2@(42)                         
    49ee:  6632           	bnes 0x4a22                           ; ↓ label_8
    49f0:  1f04           	moveb %d4,%sp@-                       
    49f2:  4eba 0c18      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    49f6:  3a00           	movew %d0,%d5                         
    49f8:  544f           	addqw #2,%sp                          
    49fa:  6706           	beqs 0x4a02                           ; ↓ label_6
    49fc:  3005           	movew %d5,%d0                         
    49fe:  6000 009a      	braw 0x4a9a                           ; ↓ label_11

  label_6:
    4a02:  486a 0030      	pea %a2@(48)                          
    4a06:  1f04           	moveb %d4,%sp@-                       
    4a08:  2f0a           	movel %a2,%sp@-                       
    4a0a:  4eba 0e2c      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    4a0e:  3a00           	movew %d0,%d5                         
    4a10:  4fef 000a      	lea %sp@(10),%sp                      
    4a14:  6706           	beqs 0x4a1c                           ; ↓ label_7
    4a16:  3005           	movew %d5,%d0                         
    4a18:  6000 0080      	braw 0x4a9a                           ; ↓ label_11

  label_7:
    4a1c:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_8:
    4a22:  486a 0030      	pea %a2@(48)                          
    4a26:  42a7           	clrl %sp@-                            
    4a28:  1f3c 0001      	moveb #1,%sp@-                        
    4a2c:  1f3c 0008      	moveb #8,%sp@-                        
    4a30:  1f3c 0001      	moveb #1,%sp@-                        
    4a34:  1f3c 0008      	moveb #8,%sp@-                        
    4a38:  4eba 0f90      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    4a3c:  3a00           	movew %d0,%d5                         
    4a3e:  4fef 0010      	lea %sp@(16),%sp                      
    4a42:  6704           	beqs 0x4a48                           ; ↓ label_9
    4a44:  3005           	movew %d5,%d0                         
    4a46:  6052           	bras 0x4a9a                           ; ↓ label_11

  label_9:
    4a48:  0003 0080      	orib #-128,%d3                        
    4a4c:  0003 0040      	orib #64,%d3                          
    4a50:  486a 0030      	pea %a2@(48)                          
    4a54:  42a7           	clrl %sp@-                            
    4a56:  1f03           	moveb %d3,%sp@-                       
    4a58:  4eba 112e      	jsr %pc@(0x5b88)                      ; PrvTpcWrite_1_Reg
    4a5c:  3a00           	movew %d0,%d5                         
    4a5e:  4fef 000a      	lea %sp@(10),%sp                      
    4a62:  6704           	beqs 0x4a68                           ; ↓ label_10
    4a64:  3005           	movew %d5,%d0                         
    4a66:  6032           	bras 0x4a9a                           ; ↓ label_11

  label_10:
    4a68:  1544 002d      	moveb %d4,%a2@(45)                    
    4a6c:  157c 0001 002b 	moveb #1,%a2@(43)                     
    4a72:  0279 fffd 1100 	andiw #-3,0x11000222                  
    4a78:  0222                                                  
    4a7a:  0079 0080 1100 	oriw #128,0x11000406                  
    4a80:  0406                                                  
    4a82:  256e 0010 0032 	movel %fp@(16),%a2@(50)               
    4a88:  256e 0014 0036 	movel %fp@(20),%a2@(54)               
    4a8e:  022a 00fe 002f 	andib #-2,%a2@(47)                    
    4a94:  42aa 0042      	clrl %a2@(66)                         
    4a98:  7000           	moveq #0,%d0                          

  label_11:
    4a9a:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    4a9e:  4e5e           	unlk %fp                              
    4aa0:  4e75           	rts                                   
```

# IOJobFunctionDisable: 0x4ac8 - 0x4bb0

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???

```
    4ac8:  4e56 0000      	linkw %fp,#0                          
    4acc:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    4ad0:  246e 0008      	moveal %fp@(8),%a2                    
    4ad4:  162e 000e      	moveb %fp@(14),%d3                    
    4ad8:  4a2e 000c      	tstb %fp@(12)                         
    4adc:  6708           	beqs 0x4ae6                           ; ↓ label_0
    4ade:  303c 4001      	movew #16385,%d0                      
    4ae2:  6000 00c6      	braw 0x4baa                           ; ↓ label_9

  label_0:
    4ae6:  4a03           	tstb %d3                              
    4ae8:  6706           	beqs 0x4af0                           ; ↓ label_1
    4aea:  0c03 00ff      	cmpib #-1,%d3                         
    4aee:  6608           	bnes 0x4af8                           ; ↓ label_2

  label_1:
    4af0:  303c 4002      	movew #16386,%d0                      
    4af4:  6000 00b4      	braw 0x4baa                           ; ↓ label_9

  label_2:
    4af8:  1f03           	moveb %d3,%sp@-                       
    4afa:  2f0a           	movel %a2,%sp@-                       
    4afc:  4eba 0c5c      	jsr %pc@(0x575a)                      ; IsFunctionEnable
    4b00:  4a00           	tstb %d0                              
    4b02:  5c4f           	addqw #6,%sp                          
    4b04:  6606           	bnes 0x4b0c                           ; ↓ label_3
    4b06:  7000           	moveq #0,%d0                          
    4b08:  6000 00a0      	braw 0x4baa                           ; ↓ label_9

  label_3:
    4b0c:  4a2a 002a      	tstb %a2@(42)                         
    4b10:  6630           	bnes 0x4b42                           ; ↓ label_6
    4b12:  1f03           	moveb %d3,%sp@-                       
    4b14:  4eba 0af6      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    4b18:  3800           	movew %d0,%d4                         
    4b1a:  544f           	addqw #2,%sp                          
    4b1c:  6706           	beqs 0x4b24                           ; ↓ label_4
    4b1e:  3004           	movew %d4,%d0                         
    4b20:  6000 0088      	braw 0x4baa                           ; ↓ label_9

  label_4:
    4b24:  486a 0030      	pea %a2@(48)                          
    4b28:  1f03           	moveb %d3,%sp@-                       
    4b2a:  2f0a           	movel %a2,%sp@-                       
    4b2c:  4eba 0d0a      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    4b30:  3800           	movew %d0,%d4                         
    4b32:  4fef 000a      	lea %sp@(10),%sp                      
    4b36:  6704           	beqs 0x4b3c                           ; ↓ label_5
    4b38:  3004           	movew %d4,%d0                         
    4b3a:  606e           	bras 0x4baa                           ; ↓ label_9

  label_5:
    4b3c:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_6:
    4b42:  2f0a           	movel %a2,%sp@-                       
    4b44:  4eba d770      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    4b48:  486a 0030      	pea %a2@(48)                          
    4b4c:  2f2a 003a      	movel %a2@(58),%sp@-                  
    4b50:  1f3c 0001      	moveb #1,%sp@-                        
    4b54:  1f3c 0008      	moveb #8,%sp@-                        
    4b58:  1f3c 001f      	moveb #31,%sp@-                       
    4b5c:  4227           	clrb %sp@-                            
    4b5e:  4eba 0e6a      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    4b62:  3600           	movew %d0,%d3                         
    4b64:  4fef 0014      	lea %sp@(20),%sp                      
    4b68:  6704           	beqs 0x4b6e                           ; ↓ label_7
    4b6a:  3003           	movew %d3,%d0                         
    4b6c:  603c           	bras 0x4baa                           ; ↓ label_9

  label_7:
    4b6e:  486a 0030      	pea %a2@(48)                          
    4b72:  2f2e 0010      	movel %fp@(16),%sp@-                  
    4b76:  4227           	clrb %sp@-                            
    4b78:  4eba 100e      	jsr %pc@(0x5b88)                      ; PrvTpcWrite_1_Reg
    4b7c:  3600           	movew %d0,%d3                         
    4b7e:  4fef 000a      	lea %sp@(10),%sp                      
    4b82:  6704           	beqs 0x4b88                           ; ↓ label_8
    4b84:  3003           	movew %d3,%d0                         
    4b86:  6022           	bras 0x4baa                           ; ↓ label_9

  label_8:
    4b88:  422a 002b      	clrb %a2@(43)                         
    4b8c:  2f0a           	movel %a2,%sp@-                       
    4b8e:  4eba 0a00      	jsr %pc@(0x5590)                      ; ReturnToMS
    4b92:  422a 002a      	clrb %a2@(42)                         
    4b96:  4227           	clrb %sp@-                            
    4b98:  4878 0016      	pea 0x16                              
    4b9c:  486a 002c      	pea %a2@(44)                          
    4ba0:  4e4f           	trap #15                              
    4ba2:  a027           	0120047                               ; sysTrapMemSet
    4ba4:  7000           	moveq #0,%d0                          
    4ba6:  4fef 000e      	lea %sp@(14),%sp                      

  label_9:
    4baa:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    4bae:  4e5e           	unlk %fp                              
    4bb0:  4e75           	rts                                   
```

# IOJobIntControl: 0x4bca - 0x4c5a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???

```
    4bca:  4e56 0000      	linkw %fp,#0                          
    4bce:  2f0a           	movel %a2,%sp@-                       
    4bd0:  246e 0008      	moveal %fp@(8),%a2                    
    4bd4:  4a2e 0010      	tstb %fp@(16)                         
    4bd8:  674c           	beqs 0x4c26                           ; ↓ label_5
    4bda:  102a 002f      	moveb %a2@(47),%d0                    
    4bde:  0240 0001      	andiw #1,%d0                          
    4be2:  6704           	beqs 0x4be8                           ; ↓ label_0
    4be4:  7000           	moveq #0,%d0                          
    4be6:  606e           	bras 0x4c56                           ; ↓ label_9

  label_0:
    4be8:  7001           	moveq #1,%d0                          
    4bea:  c0aa 0004      	andl %a2@(4),%d0                      
    4bee:  6708           	beqs 0x4bf8                           ; ↓ label_1
    4bf0:  7008           	moveq #8,%d0                          
    4bf2:  c0aa 0004      	andl %a2@(4),%d0                      
    4bf6:  6606           	bnes 0x4bfe                           ; ↓ label_2

  label_1:
    4bf8:  303c 8002      	movew #-32766,%d0                     
    4bfc:  6058           	bras 0x4c56                           ; ↓ label_9

  label_2:
    4bfe:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    4c04:  660a           	bnes 0x4c10                           ; ↓ label_3
    4c06:  102a 002d      	moveb %a2@(45),%d0                    
    4c0a:  b02e 000e      	cmpb %fp@(14),%d0                     
    4c0e:  6706           	beqs 0x4c16                           ; ↓ label_4

  label_3:
    4c10:  303c 8007      	movew #-32761,%d0                     
    4c14:  6040           	bras 0x4c56                           ; ↓ label_9

  label_4:
    4c16:  002a 0001 002f 	orib #1,%a2@(47)                      
    4c1c:  2f0a           	movel %a2,%sp@-                       
    4c1e:  4eba d61c      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    4c22:  584f           	addqw #4,%sp                          
    4c24:  602e           	bras 0x4c54                           ; ↓ label_8

  label_5:
    4c26:  102a 002f      	moveb %a2@(47),%d0                    
    4c2a:  0240 0001      	andiw #1,%d0                          
    4c2e:  6724           	beqs 0x4c54                           ; ↓ label_8
    4c30:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    4c36:  660a           	bnes 0x4c42                           ; ↓ label_6
    4c38:  102a 002d      	moveb %a2@(45),%d0                    
    4c3c:  b02e 000e      	cmpb %fp@(14),%d0                     
    4c40:  6704           	beqs 0x4c46                           ; ↓ label_7

  label_6:
    4c42:  7000           	moveq #0,%d0                          
    4c44:  6010           	bras 0x4c56                           ; ↓ label_9

  label_7:
    4c46:  2f0a           	movel %a2,%sp@-                       
    4c48:  4eba d66c      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    4c4c:  022a 00fe 002f 	andib #-2,%a2@(47)                    
    4c52:  584f           	addqw #4,%sp                          

  label_8:
    4c54:  7000           	moveq #0,%d0                          

  label_9:
    4c56:  245f           	moveal %sp@+,%a2                      
    4c58:  4e5e           	unlk %fp                              
    4c5a:  4e75           	rts                                   
```

# IOJobSetDefaultToe: 0x4c6e - 0x4ca8

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

```
    4c6e:  4e56 0000      	linkw %fp,#0                          
    4c72:  206e 0008      	moveal %fp@(8),%a0                    
    4c76:  0c28 0001 002b 	cmpib #1,%a0@(43)                     
    4c7c:  6614           	bnes 0x4c92                           ; ↓ label_0
    4c7e:  1028 002d      	moveb %a0@(45),%d0                    
    4c82:  b02e 000e      	cmpb %fp@(14),%d0                     
    4c86:  660a           	bnes 0x4c92                           ; ↓ label_0
    4c88:  1028 002c      	moveb %a0@(44),%d0                    
    4c8c:  b02e 000c      	cmpb %fp@(12),%d0                     
    4c90:  6706           	beqs 0x4c98                           ; ↓ label_1

  label_0:
    4c92:  303c 8007      	movew #-32761,%d0                     
    4c96:  600e           	bras 0x4ca6                           ; ↓ label_2

  label_1:
    4c98:  216e 0014 003a 	movel %fp@(20),%a0@(58)               
    4c9e:  216e 0010 003e 	movel %fp@(16),%a0@(62)               
    4ca4:  7000           	moveq #0,%d0                          

  label_2:
    4ca6:  4e5e           	unlk %fp                              
    4ca8:  4e75           	rts                                   
```

# IOJobPowerSave: 0x4cc0 - 0x4d84

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(18)`     : ???

```
    4cc0:  4e56 0000      	linkw %fp,#0                          
    4cc4:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    4cc8:  246e 0008      	moveal %fp@(8),%a2                    
    4ccc:  182e 000e      	moveb %fp@(14),%d4                    
    4cd0:  1f04           	moveb %d4,%sp@-                       
    4cd2:  2f0a           	movel %a2,%sp@-                       
    4cd4:  4eba 0a84      	jsr %pc@(0x575a)                      ; IsFunctionEnable
    4cd8:  4a00           	tstb %d0                              
    4cda:  5c4f           	addqw #6,%sp                          
    4cdc:  6608           	bnes 0x4ce6                           ; ↓ label_0
    4cde:  303c 8007      	movew #-32761,%d0                     
    4ce2:  6000 009a      	braw 0x4d7e                           ; ↓ label_7

  label_0:
    4ce6:  4a2a 002a      	tstb %a2@(42)                         
    4cea:  6630           	bnes 0x4d1c                           ; ↓ label_3
    4cec:  1f04           	moveb %d4,%sp@-                       
    4cee:  4eba 091c      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    4cf2:  3600           	movew %d0,%d3                         
    4cf4:  544f           	addqw #2,%sp                          
    4cf6:  6706           	beqs 0x4cfe                           ; ↓ label_1
    4cf8:  3003           	movew %d3,%d0                         
    4cfa:  6000 0082      	braw 0x4d7e                           ; ↓ label_7

  label_1:
    4cfe:  486a 0030      	pea %a2@(48)                          
    4d02:  1f04           	moveb %d4,%sp@-                       
    4d04:  2f0a           	movel %a2,%sp@-                       
    4d06:  4eba 0b30      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    4d0a:  3600           	movew %d0,%d3                         
    4d0c:  4fef 000a      	lea %sp@(10),%sp                      
    4d10:  6704           	beqs 0x4d16                           ; ↓ label_2
    4d12:  3003           	movew %d3,%d0                         
    4d14:  6068           	bras 0x4d7e                           ; ↓ label_7

  label_2:
    4d16:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_3:
    4d1c:  4a2e 0010      	tstb %fp@(16)                         
    4d20:  6706           	beqs 0x4d28                           ; ↓ label_4
    4d22:  103c 00c1      	moveb #-63,%d0                        
    4d26:  6004           	bras 0x4d2c                           ; ↓ label_5

  label_4:
    4d28:  103c 00c0      	moveb #-64,%d0                        

  label_5:
    4d2c:  1800           	moveb %d0,%d4                         
    4d2e:  2f0a           	movel %a2,%sp@-                       
    4d30:  4eba d584      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    4d34:  486a 0030      	pea %a2@(48)                          
    4d38:  2f2a 003a      	movel %a2@(58),%sp@-                  
    4d3c:  1f3c 0001      	moveb #1,%sp@-                        
    4d40:  1f3c 0008      	moveb #8,%sp@-                        
    4d44:  1f3c 001f      	moveb #31,%sp@-                       
    4d48:  4227           	clrb %sp@-                            
    4d4a:  4eba 0c7e      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    4d4e:  3600           	movew %d0,%d3                         
    4d50:  4fef 0014      	lea %sp@(20),%sp                      
    4d54:  670c           	beqs 0x4d62                           ; ↓ label_6
    4d56:  2f0a           	movel %a2,%sp@-                       
    4d58:  4eba d4e2      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    4d5c:  3003           	movew %d3,%d0                         
    4d5e:  584f           	addqw #4,%sp                          
    4d60:  601c           	bras 0x4d7e                           ; ↓ label_7

  label_6:
    4d62:  486a 0030      	pea %a2@(48)                          
    4d66:  2f2e 0012      	movel %fp@(18),%sp@-                  
    4d6a:  1f04           	moveb %d4,%sp@-                       
    4d6c:  4eba 0e1a      	jsr %pc@(0x5b88)                      ; PrvTpcWrite_1_Reg
    4d70:  3600           	movew %d0,%d3                         
    4d72:  2f0a           	movel %a2,%sp@-                       
    4d74:  4eba d4c6      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    4d78:  3003           	movew %d3,%d0                         
    4d7a:  4fef 000e      	lea %sp@(14),%sp                      

  label_7:
    4d7e:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    4d82:  4e5e           	unlk %fp                              
    4d84:  4e75           	rts                                   
```

# IOJobSetReg: 0x4d98 - 0x4e4e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???
   * `%fp@(22)`     : ???
   * `%fp@(24)`     : ???

```
    4d98:  4e56 0000      	linkw %fp,#0                          
    4d9c:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    4da0:  246e 0008      	moveal %fp@(8),%a2                    
    4da4:  182e 000e      	moveb %fp@(14),%d4                    
    4da8:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    4dae:  6606           	bnes 0x4db6                           ; ↓ label_0
    4db0:  b82a 002d      	cmpb %a2@(45),%d4                     
    4db4:  6708           	beqs 0x4dbe                           ; ↓ label_1

  label_0:
    4db6:  303c 8007      	movew #-32761,%d0                     
    4dba:  6000 008c      	braw 0x4e48                           ; ↓ label_6

  label_1:
    4dbe:  4a2a 002a      	tstb %a2@(42)                         
    4dc2:  662e           	bnes 0x4df2                           ; ↓ label_4
    4dc4:  1f04           	moveb %d4,%sp@-                       
    4dc6:  4eba 0844      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    4dca:  3600           	movew %d0,%d3                         
    4dcc:  544f           	addqw #2,%sp                          
    4dce:  6704           	beqs 0x4dd4                           ; ↓ label_2
    4dd0:  3003           	movew %d3,%d0                         
    4dd2:  6074           	bras 0x4e48                           ; ↓ label_6

  label_2:
    4dd4:  486a 0030      	pea %a2@(48)                          
    4dd8:  1f04           	moveb %d4,%sp@-                       
    4dda:  2f0a           	movel %a2,%sp@-                       
    4ddc:  4eba 0a5a      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    4de0:  3600           	movew %d0,%d3                         
    4de2:  4fef 000a      	lea %sp@(10),%sp                      
    4de6:  6704           	beqs 0x4dec                           ; ↓ label_3
    4de8:  3003           	movew %d3,%d0                         
    4dea:  605c           	bras 0x4e48                           ; ↓ label_6

  label_3:
    4dec:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_4:
    4df2:  2f0a           	movel %a2,%sp@-                       
    4df4:  4eba d4c0      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    4df8:  486a 0030      	pea %a2@(48)                          
    4dfc:  2f2a 003a      	movel %a2@(58),%sp@-                  
    4e00:  1f2e 0016      	moveb %fp@(22),%sp@-                  
    4e04:  1f2e 0014      	moveb %fp@(20),%sp@-                  
    4e08:  1f3c 001f      	moveb #31,%sp@-                       
    4e0c:  4227           	clrb %sp@-                            
    4e0e:  4eba 0bba      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    4e12:  3600           	movew %d0,%d3                         
    4e14:  4fef 0014      	lea %sp@(20),%sp                      
    4e18:  670c           	beqs 0x4e26                           ; ↓ label_5
    4e1a:  2f0a           	movel %a2,%sp@-                       
    4e1c:  4eba d41e      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    4e20:  3003           	movew %d3,%d0                         
    4e22:  584f           	addqw #4,%sp                          
    4e24:  6022           	bras 0x4e48                           ; ↓ label_6

  label_5:
    4e26:  486a 0030      	pea %a2@(48)                          
    4e2a:  2f2e 0018      	movel %fp@(24),%sp@-                  
    4e2e:  1f2e 0016      	moveb %fp@(22),%sp@-                  
    4e32:  2f2e 0010      	movel %fp@(16),%sp@-                  
    4e36:  4eba 0ce0      	jsr %pc@(0x5b18)                      ; PrvTpcWriteReg
    4e3a:  3600           	movew %d0,%d3                         
    4e3c:  2f0a           	movel %a2,%sp@-                       
    4e3e:  4eba d3fc      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    4e42:  3003           	movew %d3,%d0                         
    4e44:  4fef 0012      	lea %sp@(18),%sp                      

  label_6:
    4e48:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    4e4c:  4e5e           	unlk %fp                              
    4e4e:  4e75           	rts                                   
```

# IOJobGetReg: 0x4e5e - 0x4f6e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???
   * `%fp@(22)`     : ???
   * `%fp@(24)`     : ???

Locals:

   * `%fp@(-256)`   : ???

```
    4e5e:  4e56 ff00      	linkw %fp,#-256                       
    4e62:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-          
    4e66:  246e 0008      	moveal %fp@(8),%a2                    
    4e6a:  182e 000e      	moveb %fp@(14),%d4                    
    4e6e:  266e 0010      	moveal %fp@(16),%a3                   
    4e72:  1c2e 0014      	moveb %fp@(20),%d6                    
    4e76:  1a2e 0016      	moveb %fp@(22),%d5                    
    4e7a:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    4e80:  6606           	bnes 0x4e88                           ; ↓ label_0
    4e82:  b82a 002d      	cmpb %a2@(45),%d4                     
    4e86:  6708           	beqs 0x4e90                           ; ↓ label_1

  label_0:
    4e88:  303c 8007      	movew #-32761,%d0                     
    4e8c:  6000 00da      	braw 0x4f68                           ; ↓ label_9

  label_1:
    4e90:  4a2a 002a      	tstb %a2@(42)                         
    4e94:  6632           	bnes 0x4ec8                           ; ↓ label_4
    4e96:  1f04           	moveb %d4,%sp@-                       
    4e98:  4eba 0772      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    4e9c:  3600           	movew %d0,%d3                         
    4e9e:  544f           	addqw #2,%sp                          
    4ea0:  6706           	beqs 0x4ea8                           ; ↓ label_2
    4ea2:  3003           	movew %d3,%d0                         
    4ea4:  6000 00c2      	braw 0x4f68                           ; ↓ label_9

  label_2:
    4ea8:  486a 0030      	pea %a2@(48)                          
    4eac:  1f04           	moveb %d4,%sp@-                       
    4eae:  2f0a           	movel %a2,%sp@-                       
    4eb0:  4eba 0986      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    4eb4:  3600           	movew %d0,%d3                         
    4eb6:  4fef 000a      	lea %sp@(10),%sp                      
    4eba:  6706           	beqs 0x4ec2                           ; ↓ label_3
    4ebc:  3003           	movew %d3,%d0                         
    4ebe:  6000 00a8      	braw 0x4f68                           ; ↓ label_9

  label_3:
    4ec2:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_4:
    4ec8:  1806           	moveb %d6,%d4                         
    4eca:  d805           	addb %d5,%d4                          
    4ecc:  1604           	moveb %d4,%d3                         
    4ece:  0c03 00ff      	cmpib #-1,%d3                         
    4ed2:  6308           	blss 0x4edc                           ; ↓ label_5
    4ed4:  303c 4004      	movew #16388,%d0                      
    4ed8:  6000 008e      	braw 0x4f68                           ; ↓ label_9

  label_5:
    4edc:  2f0a           	movel %a2,%sp@-                       
    4ede:  4eba d3d6      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    4ee2:  486a 0030      	pea %a2@(48)                          
    4ee6:  2f2a 003a      	movel %a2@(58),%sp@-                  
    4eea:  1f3c 000f      	moveb #15,%sp@-                       
    4eee:  1f3c 0010      	moveb #16,%sp@-                       
    4ef2:  1f04           	moveb %d4,%sp@-                       
    4ef4:  4227           	clrb %sp@-                            
    4ef6:  4eba 0ad2      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    4efa:  3600           	movew %d0,%d3                         
    4efc:  4fef 0014      	lea %sp@(20),%sp                      
    4f00:  670c           	beqs 0x4f0e                           ; ↓ label_6
    4f02:  2f0a           	movel %a2,%sp@-                       
    4f04:  4eba d336      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    4f08:  3003           	movew %d3,%d0                         
    4f0a:  584f           	addqw #4,%sp                          
    4f0c:  605a           	bras 0x4f68                           ; ↓ label_9

  label_6:
    4f0e:  486a 0030      	pea %a2@(48)                          
    4f12:  2f2e 0018      	movel %fp@(24),%sp@-                  
    4f16:  1f04           	moveb %d4,%sp@-                       
    4f18:  486e ff00      	pea %fp@(-256)                        
    4f1c:  4eba 0b42      	jsr %pc@(0x5a60)                      ; PrvTpcReadReg
    4f20:  3600           	movew %d0,%d3                         
    4f22:  2f0a           	movel %a2,%sp@-                       
    4f24:  4eba d316      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    4f28:  0c05 0001      	cmpib #1,%d5                          
    4f2c:  4fef 0012      	lea %sp@(18),%sp                      
    4f30:  660e           	bnes 0x4f40                           ; ↓ label_7
    4f32:  7000           	moveq #0,%d0                          
    4f34:  1006           	moveb %d6,%d0                         
    4f36:  41ee ff00      	lea %fp@(-256),%a0                    
    4f3a:  16b0 0000      	moveb %a0@(00000000,%d0:w),%a3@       
    4f3e:  6026           	bras 0x4f66                           ; ↓ label_8

  label_7:
    4f40:  7000           	moveq #0,%d0                          
    4f42:  1005           	moveb %d5,%d0                         
    4f44:  2f00           	movel %d0,%sp@-                       
    4f46:  7000           	moveq #0,%d0                          
    4f48:  1006           	moveb %d6,%d0                         
    4f4a:  41ee ff00      	lea %fp@(-256),%a0                    
    4f4e:  4870 0000      	pea %a0@(00000000,%d0:w)              
    4f52:  2f0b           	movel %a3,%sp@-                       
    4f54:  4e4f           	trap #15                              
    4f56:  a026           	0120046                               ; sysTrapMemMove
    4f58:  3600           	movew %d0,%d3                         
    4f5a:  4fef 000c      	lea %sp@(12),%sp                      
    4f5e:  6706           	beqs 0x4f66                           ; ↓ label_8
    4f60:  303c 800d      	movew #-32755,%d0                     
    4f64:  6002           	bras 0x4f68                           ; ↓ label_9

  label_8:
    4f66:  3003           	movew %d3,%d0                         

  label_9:
    4f68:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3          
    4f6c:  4e5e           	unlk %fp                              
    4f6e:  4e75           	rts                                   
```

# IOJobTPCGetInt: 0x4f7e - 0x5134

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???
   * `%fp@(22)`     : ???

Locals:

   * `%fp@(-1)`     : ???
   * `%fp@(-2)`     : ???

```
    4f7e:  4e56 fffe      	linkw %fp,#-2                         
    4f82:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-          
    4f86:  246e 0008      	moveal %fp@(8),%a2                    
    4f8a:  182e 000e      	moveb %fp@(14),%d4                    
    4f8e:  2c2e 0010      	movel %fp@(16),%d6                    
    4f92:  1a2e 0014      	moveb %fp@(20),%d5                    
    4f96:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    4f9c:  6606           	bnes 0x4fa4                           ; ↓ label_0
    4f9e:  b82a 002d      	cmpb %a2@(45),%d4                     
    4fa2:  6708           	beqs 0x4fac                           ; ↓ label_1

  label_0:
    4fa4:  303c 8007      	movew #-32761,%d0                     
    4fa8:  6000 0184      	braw 0x512e                           ; ↓ label_14

  label_1:
    4fac:  4a2a 002a      	tstb %a2@(42)                         
    4fb0:  6632           	bnes 0x4fe4                           ; ↓ label_4
    4fb2:  1f04           	moveb %d4,%sp@-                       
    4fb4:  4eba 0656      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    4fb8:  3600           	movew %d0,%d3                         
    4fba:  544f           	addqw #2,%sp                          
    4fbc:  6706           	beqs 0x4fc4                           ; ↓ label_2
    4fbe:  3003           	movew %d3,%d0                         
    4fc0:  6000 016c      	braw 0x512e                           ; ↓ label_14

  label_2:
    4fc4:  486a 0030      	pea %a2@(48)                          
    4fc8:  1f04           	moveb %d4,%sp@-                       
    4fca:  2f0a           	movel %a2,%sp@-                       
    4fcc:  4eba 086a      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    4fd0:  3600           	movew %d0,%d3                         
    4fd2:  4fef 000a      	lea %sp@(10),%sp                      
    4fd6:  6706           	beqs 0x4fde                           ; ↓ label_3
    4fd8:  3003           	movew %d3,%d0                         
    4fda:  6000 0152      	braw 0x512e                           ; ↓ label_14

  label_3:
    4fde:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_4:
    4fe4:  0c05 0001      	cmpib #1,%d5                          
    4fe8:  6662           	bnes 0x504c                           ; ↓ label_6
    4fea:  2f0a           	movel %a2,%sp@-                       
    4fec:  4eba d2c8      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    4ff0:  486a 0030      	pea %a2@(48)                          
    4ff4:  2f2a 003a      	movel %a2@(58),%sp@-                  
    4ff8:  1f3c 000f      	moveb #15,%sp@-                       
    4ffc:  1f3c 0010      	moveb #16,%sp@-                       
    5000:  1f3c 0002      	moveb #2,%sp@-                        
    5004:  4227           	clrb %sp@-                            
    5006:  4eba 09c2      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    500a:  3600           	movew %d0,%d3                         
    500c:  4fef 0014      	lea %sp@(20),%sp                      
    5010:  670e           	beqs 0x5020                           ; ↓ label_5
    5012:  2f0a           	movel %a2,%sp@-                       
    5014:  4eba d226      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    5018:  3003           	movew %d3,%d0                         
    501a:  584f           	addqw #4,%sp                          
    501c:  6000 0110      	braw 0x512e                           ; ↓ label_14

  label_5:
    5020:  486a 0030      	pea %a2@(48)                          
    5024:  2f2e 0016      	movel %fp@(22),%sp@-                  
    5028:  1f3c 0002      	moveb #2,%sp@-                        
    502c:  486e fffe      	pea %fp@(-2)                          
    5030:  4eba 0a2e      	jsr %pc@(0x5a60)                      ; PrvTpcReadReg
    5034:  3600           	movew %d0,%d3                         
    5036:  2f0a           	movel %a2,%sp@-                       
    5038:  4eba d202      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    503c:  2046           	moveal %d6,%a0                        
    503e:  10ae ffff      	moveb %fp@(-1),%a0@                   
    5042:  3003           	movew %d3,%d0                         
    5044:  4fef 0012      	lea %sp@(18),%sp                      
    5048:  6000 00e4      	braw 0x512e                           ; ↓ label_14

  label_6:
    504c:  1605           	moveb %d5,%d3                         
    504e:  5203           	addqb #1,%d3                          
    5050:  1803           	moveb %d3,%d4                         
    5052:  0c04 00ff      	cmpib #-1,%d4                         
    5056:  6308           	blss 0x5060                           ; ↓ label_7
    5058:  303c 4004      	movew #16388,%d0                      
    505c:  6000 00d0      	braw 0x512e                           ; ↓ label_14

  label_7:
    5060:  7000           	moveq #0,%d0                          
    5062:  1003           	moveb %d3,%d0                         
    5064:  2f00           	movel %d0,%sp@-                       
    5066:  4e4f           	trap #15                              
    5068:  a013           	0120023                               ; sysTrapMemPtrNew
    506a:  2648           	moveal %a0,%a3                        
    506c:  200b           	movel %a3,%d0                         
    506e:  584f           	addqw #4,%sp                          
    5070:  6608           	bnes 0x507a                           ; ↓ label_8
    5072:  303c 100d      	movew #4109,%d0                       
    5076:  6000 00b6      	braw 0x512e                           ; ↓ label_14

  label_8:
    507a:  2f0a           	movel %a2,%sp@-                       
    507c:  4eba d238      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    5080:  486a 0030      	pea %a2@(48)                          
    5084:  2f2a 003a      	movel %a2@(58),%sp@-                  
    5088:  1f3c 000f      	moveb #15,%sp@-                       
    508c:  1f3c 0010      	moveb #16,%sp@-                       
    5090:  1f03           	moveb %d3,%sp@-                       
    5092:  4227           	clrb %sp@-                            
    5094:  4eba 0934      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    5098:  3800           	movew %d0,%d4                         
    509a:  0c44 2000      	cmpiw #8192,%d4                       
    509e:  4fef 0014      	lea %sp@(20),%sp                      
    50a2:  660e           	bnes 0x50b2                           ; ↓ label_9
    50a4:  2f0a           	movel %a2,%sp@-                       
    50a6:  4eba d194      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    50aa:  303c 1800      	movew #6144,%d0                       
    50ae:  584f           	addqw #4,%sp                          
    50b0:  607c           	bras 0x512e                           ; ↓ label_14

  label_9:
    50b2:  0c44 0008      	cmpiw #8,%d4                          
    50b6:  6614           	bnes 0x50cc                           ; ↓ label_10
    50b8:  2f0a           	movel %a2,%sp@-                       
    50ba:  4eba d180      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    50be:  7000           	moveq #0,%d0                          
    50c0:  102a 0030      	moveb %a2@(48),%d0                    
    50c4:  0040 2800      	oriw #10240,%d0                       
    50c8:  584f           	addqw #4,%sp                          
    50ca:  6062           	bras 0x512e                           ; ↓ label_14

  label_10:
    50cc:  486a 0030      	pea %a2@(48)                          
    50d0:  2f2e 0016      	movel %fp@(22),%sp@-                  
    50d4:  1f03           	moveb %d3,%sp@-                       
    50d6:  2f0b           	movel %a3,%sp@-                       
    50d8:  4eba 0986      	jsr %pc@(0x5a60)                      ; PrvTpcReadReg
    50dc:  3800           	movew %d0,%d4                         
    50de:  2f0a           	movel %a2,%sp@-                       
    50e0:  4eba d15a      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    50e4:  0c44 2000      	cmpiw #8192,%d4                       
    50e8:  4fef 0012      	lea %sp@(18),%sp                      
    50ec:  6606           	bnes 0x50f4                           ; ↓ label_11
    50ee:  303c 1400      	movew #5120,%d0                       
    50f2:  603a           	bras 0x512e                           ; ↓ label_14

  label_11:
    50f4:  0c44 0008      	cmpiw #8,%d4                          
    50f8:  660c           	bnes 0x5106                           ; ↓ label_12
    50fa:  7000           	moveq #0,%d0                          
    50fc:  102a 0030      	moveb %a2@(48),%d0                    
    5100:  0040 2400      	oriw #9216,%d0                        
    5104:  6028           	bras 0x512e                           ; ↓ label_14

  label_12:
    5106:  7000           	moveq #0,%d0                          
    5108:  1005           	moveb %d5,%d0                         
    510a:  2f00           	movel %d0,%sp@-                       
    510c:  486b 0001      	pea %a3@(1)                           
    5110:  2f06           	movel %d6,%sp@-                       
    5112:  4e4f           	trap #15                              
    5114:  a026           	0120046                               ; sysTrapMemMove
    5116:  3600           	movew %d0,%d3                         
    5118:  4fef 000c      	lea %sp@(12),%sp                      
    511c:  6706           	beqs 0x5124                           ; ↓ label_13
    511e:  303c 100d      	movew #4109,%d0                       
    5122:  600a           	bras 0x512e                           ; ↓ label_14

  label_13:
    5124:  2f0b           	movel %a3,%sp@-                       
    5126:  4e4f           	trap #15                              
    5128:  a012           	0120022                               ; sysTrapMemChunkFree
    512a:  7000           	moveq #0,%d0                          
    512c:  584f           	addqw #4,%sp                          

  label_14:
    512e:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3          
    5132:  4e5e           	unlk %fp                              
    5134:  4e75           	rts                                   
```

# IOJobTPCWriteData: 0x5148 - 0x51dc

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???
   * `%fp@(22)`     : ???

```
    5148:  4e56 0000      	linkw %fp,#0                          
    514c:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    5150:  246e 0008      	moveal %fp@(8),%a2                    
    5154:  182e 000e      	moveb %fp@(14),%d4                    
    5158:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    515e:  6606           	bnes 0x5166                           ; ↓ label_0
    5160:  b82a 002d      	cmpb %a2@(45),%d4                     
    5164:  6706           	beqs 0x516c                           ; ↓ label_1

  label_0:
    5166:  303c 8007      	movew #-32761,%d0                     
    516a:  606a           	bras 0x51d6                           ; ↓ label_6

  label_1:
    516c:  0c6e 0200 0014 	cmpiw #512,%fp@(20)                   
    5172:  6306           	blss 0x517a                           ; ↓ label_2
    5174:  303c 4004      	movew #16388,%d0                      
    5178:  605c           	bras 0x51d6                           ; ↓ label_6

  label_2:
    517a:  4a2a 002a      	tstb %a2@(42)                         
    517e:  662e           	bnes 0x51ae                           ; ↓ label_5
    5180:  1f04           	moveb %d4,%sp@-                       
    5182:  4eba 0488      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    5186:  3600           	movew %d0,%d3                         
    5188:  544f           	addqw #2,%sp                          
    518a:  6704           	beqs 0x5190                           ; ↓ label_3
    518c:  3003           	movew %d3,%d0                         
    518e:  6046           	bras 0x51d6                           ; ↓ label_6

  label_3:
    5190:  486a 0030      	pea %a2@(48)                          
    5194:  1f04           	moveb %d4,%sp@-                       
    5196:  2f0a           	movel %a2,%sp@-                       
    5198:  4eba 069e      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    519c:  3600           	movew %d0,%d3                         
    519e:  4fef 000a      	lea %sp@(10),%sp                      
    51a2:  6704           	beqs 0x51a8                           ; ↓ label_4
    51a4:  3003           	movew %d3,%d0                         
    51a6:  602e           	bras 0x51d6                           ; ↓ label_6

  label_4:
    51a8:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_5:
    51ae:  2f0a           	movel %a2,%sp@-                       
    51b0:  4eba d104      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    51b4:  486a 0030      	pea %a2@(48)                          
    51b8:  2f2e 0016      	movel %fp@(22),%sp@-                  
    51bc:  3f2e 0014      	movew %fp@(20),%sp@-                  
    51c0:  2f2e 0010      	movel %fp@(16),%sp@-                  
    51c4:  4eba 0a58      	jsr %pc@(0x5c1e)                      ; PrvTpcWriteData
    51c8:  3600           	movew %d0,%d3                         
    51ca:  2f0a           	movel %a2,%sp@-                       
    51cc:  4eba d06e      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    51d0:  3003           	movew %d3,%d0                         
    51d2:  4fef 0016      	lea %sp@(22),%sp                      

  label_6:
    51d6:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    51da:  4e5e           	unlk %fp                              
    51dc:  4e75           	rts                                   
```

# IOJobTPCReadData: 0x51f2 - 0x5286

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???
   * `%fp@(22)`     : ???

```
    51f2:  4e56 0000      	linkw %fp,#0                          
    51f6:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    51fa:  246e 0008      	moveal %fp@(8),%a2                    
    51fe:  182e 000e      	moveb %fp@(14),%d4                    
    5202:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    5208:  6606           	bnes 0x5210                           ; ↓ label_0
    520a:  b82a 002d      	cmpb %a2@(45),%d4                     
    520e:  6706           	beqs 0x5216                           ; ↓ label_1

  label_0:
    5210:  303c 8007      	movew #-32761,%d0                     
    5214:  606a           	bras 0x5280                           ; ↓ label_6

  label_1:
    5216:  0c6e 0200 0014 	cmpiw #512,%fp@(20)                   
    521c:  6306           	blss 0x5224                           ; ↓ label_2
    521e:  303c 4004      	movew #16388,%d0                      
    5222:  605c           	bras 0x5280                           ; ↓ label_6

  label_2:
    5224:  4a2a 002a      	tstb %a2@(42)                         
    5228:  662e           	bnes 0x5258                           ; ↓ label_5
    522a:  1f04           	moveb %d4,%sp@-                       
    522c:  4eba 03de      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    5230:  3600           	movew %d0,%d3                         
    5232:  544f           	addqw #2,%sp                          
    5234:  6704           	beqs 0x523a                           ; ↓ label_3
    5236:  3003           	movew %d3,%d0                         
    5238:  6046           	bras 0x5280                           ; ↓ label_6

  label_3:
    523a:  486a 0030      	pea %a2@(48)                          
    523e:  1f04           	moveb %d4,%sp@-                       
    5240:  2f0a           	movel %a2,%sp@-                       
    5242:  4eba 05f4      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    5246:  3600           	movew %d0,%d3                         
    5248:  4fef 000a      	lea %sp@(10),%sp                      
    524c:  6704           	beqs 0x5252                           ; ↓ label_4
    524e:  3003           	movew %d3,%d0                         
    5250:  602e           	bras 0x5280                           ; ↓ label_6

  label_4:
    5252:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_5:
    5258:  2f0a           	movel %a2,%sp@-                       
    525a:  4eba d05a      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    525e:  486a 0030      	pea %a2@(48)                          
    5262:  2f2e 0016      	movel %fp@(22),%sp@-                  
    5266:  3f2e 0014      	movew %fp@(20),%sp@-                  
    526a:  2f2e 0010      	movel %fp@(16),%sp@-                  
    526e:  4eba 094e      	jsr %pc@(0x5bbe)                      ; PrvTpcReadData
    5272:  3600           	movew %d0,%d3                         
    5274:  2f0a           	movel %a2,%sp@-                       
    5276:  4eba cfc4      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    527a:  3003           	movew %d3,%d0                         
    527c:  4fef 0016      	lea %sp@(22),%sp                      

  label_6:
    5280:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    5284:  4e5e           	unlk %fp                              
    5286:  4e75           	rts                                   
```

# IOJobTPCSetCmd: 0x529c - 0x531e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(18)`     : ???

```
    529c:  4e56 0000      	linkw %fp,#0                          
    52a0:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    52a4:  246e 0008      	moveal %fp@(8),%a2                    
    52a8:  182e 000e      	moveb %fp@(14),%d4                    
    52ac:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    52b2:  6606           	bnes 0x52ba                           ; ↓ label_0
    52b4:  b82a 002d      	cmpb %a2@(45),%d4                     
    52b8:  6706           	beqs 0x52c0                           ; ↓ label_1

  label_0:
    52ba:  303c 8007      	movew #-32761,%d0                     
    52be:  6058           	bras 0x5318                           ; ↓ label_5

  label_1:
    52c0:  4a2a 002a      	tstb %a2@(42)                         
    52c4:  662e           	bnes 0x52f4                           ; ↓ label_4
    52c6:  1f04           	moveb %d4,%sp@-                       
    52c8:  4eba 0342      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    52cc:  3600           	movew %d0,%d3                         
    52ce:  544f           	addqw #2,%sp                          
    52d0:  6704           	beqs 0x52d6                           ; ↓ label_2
    52d2:  3003           	movew %d3,%d0                         
    52d4:  6042           	bras 0x5318                           ; ↓ label_5

  label_2:
    52d6:  486a 0030      	pea %a2@(48)                          
    52da:  1f04           	moveb %d4,%sp@-                       
    52dc:  2f0a           	movel %a2,%sp@-                       
    52de:  4eba 0558      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    52e2:  3600           	movew %d0,%d3                         
    52e4:  4fef 000a      	lea %sp@(10),%sp                      
    52e8:  6704           	beqs 0x52ee                           ; ↓ label_3
    52ea:  3003           	movew %d3,%d0                         
    52ec:  602a           	bras 0x5318                           ; ↓ label_5

  label_3:
    52ee:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_4:
    52f4:  2f0a           	movel %a2,%sp@-                       
    52f6:  4eba cfbe      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    52fa:  486a 0030      	pea %a2@(48)                          
    52fe:  2f2e 0012      	movel %fp@(18),%sp@-                  
    5302:  1f2e 0010      	moveb %fp@(16),%sp@-                  
    5306:  4eba 063c      	jsr %pc@(0x5944)                      ; PrvTpcSetCmd
    530a:  3600           	movew %d0,%d3                         
    530c:  2f0a           	movel %a2,%sp@-                       
    530e:  4eba cf2c      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    5312:  3003           	movew %d3,%d0                         
    5314:  4fef 0012      	lea %sp@(18),%sp                      

  label_5:
    5318:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    531c:  4e5e           	unlk %fp                              
    531e:  4e75           	rts                                   
```

# IOJobWaitStickInt: 0x5332 - 0x53ae

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???

```
    5332:  4e56 0000      	linkw %fp,#0                          
    5336:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    533a:  246e 0008      	moveal %fp@(8),%a2                    
    533e:  182e 000e      	moveb %fp@(14),%d4                    
    5342:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    5348:  6606           	bnes 0x5350                           ; ↓ label_0
    534a:  b82a 002d      	cmpb %a2@(45),%d4                     
    534e:  6706           	beqs 0x5356                           ; ↓ label_1

  label_0:
    5350:  303c 8007      	movew #-32761,%d0                     
    5354:  6052           	bras 0x53a8                           ; ↓ label_6

  label_1:
    5356:  4a2a 002a      	tstb %a2@(42)                         
    535a:  662e           	bnes 0x538a                           ; ↓ label_4
    535c:  1f04           	moveb %d4,%sp@-                       
    535e:  4eba 02ac      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    5362:  3600           	movew %d0,%d3                         
    5364:  544f           	addqw #2,%sp                          
    5366:  6704           	beqs 0x536c                           ; ↓ label_2
    5368:  3003           	movew %d3,%d0                         
    536a:  603c           	bras 0x53a8                           ; ↓ label_6

  label_2:
    536c:  486a 0030      	pea %a2@(48)                          
    5370:  1f04           	moveb %d4,%sp@-                       
    5372:  2f0a           	movel %a2,%sp@-                       
    5374:  4eba 04c2      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    5378:  3600           	movew %d0,%d3                         
    537a:  4fef 000a      	lea %sp@(10),%sp                      
    537e:  6704           	beqs 0x5384                           ; ↓ label_3
    5380:  3003           	movew %d3,%d0                         
    5382:  6024           	bras 0x53a8                           ; ↓ label_6

  label_3:
    5384:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_4:
    538a:  486a 0030      	pea %a2@(48)                          
    538e:  2f2e 0010      	movel %fp@(16),%sp@-                  
    5392:  4eba 0564      	jsr %pc@(0x58f8)                      ; PrvWaitMsInt
    5396:  3600           	movew %d0,%d3                         
    5398:  0c43 0001      	cmpiw #1,%d3                          
    539c:  504f           	addqw #8,%sp                          
    539e:  6706           	beqs 0x53a6                           ; ↓ label_5
    53a0:  303c 800e      	movew #-32754,%d0                     
    53a4:  6002           	bras 0x53a8                           ; ↓ label_6

  label_5:
    53a6:  7000           	moveq #0,%d0                          

  label_6:
    53a8:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    53ac:  4e5e           	unlk %fp                              
    53ae:  4e75           	rts                                   
```

# IOJobTPCWriteUniqTpc: 0x53c4 - 0x5472

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(18)`     : ???
   * `%fp@(22)`     : ???
   * `%fp@(24)`     : ???

```
    53c4:  4e56 0000      	linkw %fp,#0                          
    53c8:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    53cc:  246e 0008      	moveal %fp@(8),%a2                    
    53d0:  1a2e 000e      	moveb %fp@(14),%d5                    
    53d4:  182e 0010      	moveb %fp@(16),%d4                    
    53d8:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    53de:  6606           	bnes 0x53e6                           ; ↓ label_0
    53e0:  ba2a 002d      	cmpb %a2@(45),%d5                     
    53e4:  6708           	beqs 0x53ee                           ; ↓ label_1

  label_0:
    53e6:  303c 8007      	movew #-32761,%d0                     
    53ea:  6000 0080      	braw 0x546c                           ; ↓ label_8

  label_1:
    53ee:  0c04 00f0      	cmpib #-16,%d4                        
    53f2:  6706           	beqs 0x53fa                           ; ↓ label_2
    53f4:  0c04 000f      	cmpib #15,%d4                         
    53f8:  6606           	bnes 0x5400                           ; ↓ label_3

  label_2:
    53fa:  303c 4003      	movew #16387,%d0                      
    53fe:  606c           	bras 0x546c                           ; ↓ label_8

  label_3:
    5400:  0c6e 0200 0016 	cmpiw #512,%fp@(22)                   
    5406:  6306           	blss 0x540e                           ; ↓ label_4
    5408:  303c 4004      	movew #16388,%d0                      
    540c:  605e           	bras 0x546c                           ; ↓ label_8

  label_4:
    540e:  4a2a 002a      	tstb %a2@(42)                         
    5412:  662e           	bnes 0x5442                           ; ↓ label_7
    5414:  1f05           	moveb %d5,%sp@-                       
    5416:  4eba 01f4      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    541a:  3600           	movew %d0,%d3                         
    541c:  544f           	addqw #2,%sp                          
    541e:  6704           	beqs 0x5424                           ; ↓ label_5
    5420:  3003           	movew %d3,%d0                         
    5422:  6048           	bras 0x546c                           ; ↓ label_8

  label_5:
    5424:  486a 0030      	pea %a2@(48)                          
    5428:  1f05           	moveb %d5,%sp@-                       
    542a:  2f0a           	movel %a2,%sp@-                       
    542c:  4eba 040a      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    5430:  3600           	movew %d0,%d3                         
    5432:  4fef 000a      	lea %sp@(10),%sp                      
    5436:  6704           	beqs 0x543c                           ; ↓ label_6
    5438:  3003           	movew %d3,%d0                         
    543a:  6030           	bras 0x546c                           ; ↓ label_8

  label_6:
    543c:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_7:
    5442:  2f0a           	movel %a2,%sp@-                       
    5444:  4eba ce70      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    5448:  486a 0030      	pea %a2@(48)                          
    544c:  2f2e 0018      	movel %fp@(24),%sp@-                  
    5450:  3f2e 0016      	movew %fp@(22),%sp@-                  
    5454:  2f2e 0012      	movel %fp@(18),%sp@-                  
    5458:  1f04           	moveb %d4,%sp@-                       
    545a:  4eba 0896      	jsr %pc@(0x5cf2)                      ; PrvTpcUniqTPCWrite
    545e:  3600           	movew %d0,%d3                         
    5460:  2f0a           	movel %a2,%sp@-                       
    5462:  4eba cdd8      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    5466:  3003           	movew %d3,%d0                         
    5468:  4fef 0018      	lea %sp@(24),%sp                      

  label_8:
    546c:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    5470:  4e5e           	unlk %fp                              
    5472:  4e75           	rts                                   
```

# IOJobTPCReadUniqTpc: 0x548c - 0x553a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(18)`     : ???
   * `%fp@(22)`     : ???
   * `%fp@(24)`     : ???

```
    548c:  4e56 0000      	linkw %fp,#0                          
    5490:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    5494:  246e 0008      	moveal %fp@(8),%a2                    
    5498:  1a2e 000e      	moveb %fp@(14),%d5                    
    549c:  182e 0010      	moveb %fp@(16),%d4                    
    54a0:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    54a6:  6606           	bnes 0x54ae                           ; ↓ label_0
    54a8:  ba2a 002d      	cmpb %a2@(45),%d5                     
    54ac:  6708           	beqs 0x54b6                           ; ↓ label_1

  label_0:
    54ae:  303c 8007      	movew #-32761,%d0                     
    54b2:  6000 0080      	braw 0x5534                           ; ↓ label_8

  label_1:
    54b6:  0c04 00f0      	cmpib #-16,%d4                        
    54ba:  6706           	beqs 0x54c2                           ; ↓ label_2
    54bc:  0c04 000f      	cmpib #15,%d4                         
    54c0:  6606           	bnes 0x54c8                           ; ↓ label_3

  label_2:
    54c2:  303c 4003      	movew #16387,%d0                      
    54c6:  606c           	bras 0x5534                           ; ↓ label_8

  label_3:
    54c8:  0c6e 0200 0016 	cmpiw #512,%fp@(22)                   
    54ce:  6306           	blss 0x54d6                           ; ↓ label_4
    54d0:  303c 4004      	movew #16388,%d0                      
    54d4:  605e           	bras 0x5534                           ; ↓ label_8

  label_4:
    54d6:  4a2a 002a      	tstb %a2@(42)                         
    54da:  662e           	bnes 0x550a                           ; ↓ label_7
    54dc:  1f05           	moveb %d5,%sp@-                       
    54de:  4eba 012c      	jsr %pc@(0x560c)                      ; PrvDirectAccessMSIF
    54e2:  3600           	movew %d0,%d3                         
    54e4:  544f           	addqw #2,%sp                          
    54e6:  6704           	beqs 0x54ec                           ; ↓ label_5
    54e8:  3003           	movew %d3,%d0                         
    54ea:  6048           	bras 0x5534                           ; ↓ label_8

  label_5:
    54ec:  486a 0030      	pea %a2@(48)                          
    54f0:  1f05           	moveb %d5,%sp@-                       
    54f2:  2f0a           	movel %a2,%sp@-                       
    54f4:  4eba 0342      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    54f8:  3600           	movew %d0,%d3                         
    54fa:  4fef 000a      	lea %sp@(10),%sp                      
    54fe:  6704           	beqs 0x5504                           ; ↓ label_6
    5500:  3003           	movew %d3,%d0                         
    5502:  6030           	bras 0x5534                           ; ↓ label_8

  label_6:
    5504:  157c 0001 002a 	moveb #1,%a2@(42)                     

  label_7:
    550a:  2f0a           	movel %a2,%sp@-                       
    550c:  4eba cda8      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    5510:  486a 0030      	pea %a2@(48)                          
    5514:  2f2e 0018      	movel %fp@(24),%sp@-                  
    5518:  3f2e 0016      	movew %fp@(22),%sp@-                  
    551c:  2f2e 0012      	movel %fp@(18),%sp@-                  
    5520:  1f04           	moveb %d4,%sp@-                       
    5522:  4eba 075a      	jsr %pc@(0x5c7e)                      ; PrvTpcUniqTPCRead
    5526:  3600           	movew %d0,%d3                         
    5528:  2f0a           	movel %a2,%sp@-                       
    552a:  4eba cd10      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    552e:  3003           	movew %d3,%d0                         
    5530:  4fef 0018      	lea %sp@(24),%sp                      

  label_8:
    5534:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    5538:  4e5e           	unlk %fp                              
    553a:  4e75           	rts                                   
```

# InitializeIOParam: 0x5552 - 0x557a

Parameters:

   * `%fp@(8)`      : ???

```
    5552:  4e56 0000      	linkw %fp,#0                          
    5556:  2f0a           	movel %a2,%sp@-                       
    5558:  246e 0008      	moveal %fp@(8),%a2                    
    555c:  422a 002b      	clrb %a2@(43)                         
    5560:  42aa 0042      	clrl %a2@(66)                         
    5564:  4227           	clrb %sp@-                            
    5566:  4878 0016      	pea 0x16                              
    556a:  486a 002c      	pea %a2@(44)                          
    556e:  4e4f           	trap #15                              
    5570:  a027           	0120047                               ; sysTrapMemSet
    5572:  4fef 000a      	lea %sp@(10),%sp                      
    5576:  245f           	moveal %sp@+,%a2                      
    5578:  4e5e           	unlk %fp                              
    557a:  4e75           	rts                                   
```

# ReturnToMS: 0x5590 - 0x55e4

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-1)`     : ???

```
    5590:  4e56 fffe      	linkw %fp,#-2                         
    5594:  2f0a           	movel %a2,%sp@-                       
    5596:  2f03           	movel %d3,%sp@-                       
    5598:  246e 0008      	moveal %fp@(8),%a2                    
    559c:  0c2a 0001 002b 	cmpib #1,%a2@(43)                     
    55a2:  6606           	bnes 0x55aa                           ; ↓ label_0
    55a4:  303c 2906      	movew #10502,%d0                      
    55a8:  6034           	bras 0x55de                           ; ↓ label_3

  label_0:
    55aa:  0c2a 0001 002a 	cmpib #1,%a2@(42)                     
    55b0:  662a           	bnes 0x55dc                           ; ↓ label_2
    55b2:  0279 fffe 1100 	andiw #-2,0x11000224                  
    55b8:  0224                                                  
    55ba:  0279 fffd 1100 	andiw #-3,0x11000202                  
    55c0:  0202                                                  
    55c2:  486e ffff      	pea %fp@(-1)                          
    55c6:  4227           	clrb %sp@-                            
    55c8:  4eba 00a6      	jsr %pc@(0x5670)                      ; PrvIODSPControlMS
    55cc:  3600           	movew %d0,%d3                         
    55ce:  5c4f           	addqw #6,%sp                          
    55d0:  6706           	beqs 0x55d8                           ; ↓ label_1
    55d2:  303c 8100      	movew #-32512,%d0                     
    55d6:  6006           	bras 0x55de                           ; ↓ label_3

  label_1:
    55d8:  422a 002a      	clrb %a2@(42)                         

  label_2:
    55dc:  7000           	moveq #0,%d0                          

  label_3:
    55de:  261f           	movel %sp@+,%d3                       
    55e0:  245f           	moveal %sp@+,%a2                      
    55e2:  4e5e           	unlk %fp                              
    55e4:  4e75           	rts                                   
```

# ReturnToIO: 0x55f4 - 0x55fc

```
    55f4:  4e56 0000      	linkw %fp,#0                          
    55f8:  7000           	moveq #0,%d0                          
    55fa:  4e5e           	unlk %fp                              
    55fc:  4e75           	rts                                   
```

# PrvDirectAccessMSIF: 0x560c - 0x5658

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-1)`     : ???

```
    560c:  4e56 fffe      	linkw %fp,#-2                         
    5610:  2f03           	movel %d3,%sp@-                       
    5612:  422e ffff      	clrb %fp@(-1)                         
    5616:  0079 0002 1100 	oriw #2,0x11000202                    
    561c:  0202                                                  
    561e:  486e ffff      	pea %fp@(-1)                          
    5622:  1f2e 0008      	moveb %fp@(8),%sp@-                   
    5626:  4eba 0048      	jsr %pc@(0x5670)                      ; PrvIODSPControlMS
    562a:  3600           	movew %d0,%d3                         
    562c:  5c4f           	addqw #6,%sp                          
    562e:  671a           	beqs 0x564a                           ; ↓ label_2
    5630:  0279 fffd 1100 	andiw #-3,0x11000202                  
    5636:  0202                                                  
    5638:  0c43 0003      	cmpiw #3,%d3                          
    563c:  6606           	bnes 0x5644                           ; ↓ label_0
    563e:  303c 8004      	movew #-32764,%d0                     
    5642:  6004           	bras 0x5648                           ; ↓ label_1

  label_0:
    5644:  303c 8100      	movew #-32512,%d0                     

  label_1:
    5648:  600a           	bras 0x5654                           ; ↓ label_3

  label_2:
    564a:  0079 0001 1100 	oriw #1,0x11000224                    
    5650:  0224                                                  
    5652:  7000           	moveq #0,%d0                          

  label_3:
    5654:  261f           	movel %sp@+,%d3                       
    5656:  4e5e           	unlk %fp                              
    5658:  4e75           	rts                                   
```

# PrvIODSPControlMS: 0x5670 - 0x56ea

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-42)`    : ???
   * `%fp@(-44)`    : ???
   * `%fp@(-46)`    : ???
   * `%fp@(-60)`    : ???

```
    5670:  4e56 ffc4      	linkw %fp,#-60                        
    5674:  2f0a           	movel %a2,%sp@-                       
    5676:  2f03           	movel %d3,%sp@-                       
    5678:  426e ffc4      	clrw %fp@(-60)                        
    567c:  41ee ffc4      	lea %fp@(-60),%a0                     
    5680:  5488           	addql #2,%a0                          
    5682:  2448           	moveal %a0,%a2                        
    5684:  4250           	clrw %a0@                             
    5686:  3d7c 4201 ffd2 	movew #16897,%fp@(-46)                
    568c:  3d7c fffd ffd4 	movew #-3,%fp@(-44)                   
    5692:  7000           	moveq #0,%d0                          
    5694:  102e 0008      	moveb %fp@(8),%d0                     
    5698:  3d40 ffd6      	movew %d0,%fp@(-42)                   
    569c:  426e ffe4      	clrw %fp@(-28)                        
    56a0:  1d7c 0001 fff6 	moveb #1,%fp@(-10)                    
    56a6:  41fa ec5c      	lea %pc@(0x4304),%a0                  ; PrvDspDoneProc
    56aa:  2d48 fff8      	movel %a0,%fp@(-8)                    
    56ae:  41ee ffc4      	lea %fp@(-60),%a0                     
    56b2:  2d48 fffc      	movel %a0,%fp@(-4)                    
    56b6:  426e ffc4      	clrw %fp@(-60)                        
    56ba:  486e ffd2      	pea %fp@(-46)                         
    56be:  2078 0304      	moveal 0x304,%a0                      
    56c2:  2068 0018      	moveal %a0@(24),%a0                   
    56c6:  2068 009a      	moveal %a0@(154),%a0                  
    56ca:  4e90           	jsr %a0@                              
    56cc:  3612           	movew %a2@,%d3                        
    56ce:  3003           	movew %d3,%d0                         
    56d0:  0200 00ff      	andib #-1,%d0                         
    56d4:  206e 000a      	moveal %fp@(10),%a0                   
    56d8:  1080           	moveb %d0,%a0@                        
    56da:  302e ffc4      	movew %fp@(-60),%d0                   
    56de:  0240 03ff      	andiw #1023,%d0                       
    56e2:  584f           	addqw #4,%sp                          
    56e4:  261f           	movel %sp@+,%d3                       
    56e6:  245f           	moveal %sp@+,%a2                      
    56e8:  4e5e           	unlk %fp                              
    56ea:  4e75           	rts                                   
```

# PrvIOSupplyPower: 0x5700 - 0x571a

Parameters:

   * `%fp@(8)`      : ???

```
    5700:  4e56 0000      	linkw %fp,#0                          
    5704:  1f3c 0001      	moveb #1,%sp@-                        
    5708:  4eba ca6c      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    570c:  206e 0008      	moveal %fp@(8),%a0                    
    5710:  00a8 0000 0080 	oril #128,%a0@(4)                     
    5716:  0004                                                  
    5718:  4e5e           	unlk %fp                              
    571a:  4e75           	rts                                   
```

# PrvIOCutPower: 0x5730 - 0x5748

Parameters:

   * `%fp@(8)`      : ???

```
    5730:  4e56 0000      	linkw %fp,#0                          
    5734:  4227           	clrb %sp@-                            
    5736:  4eba ca3e      	jsr %pc@(0x2176)                      ; MSSlotHwrSupplyPower
    573a:  206e 0008      	moveal %fp@(8),%a0                    
    573e:  02a8 ffff ff7f 	andil #-129,%a0@(4)                   
    5744:  0004                                                  
    5746:  4e5e           	unlk %fp                              
    5748:  4e75           	rts                                   
```

# IsFunctionEnable: 0x575a - 0x577a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    575a:  4e56 0000      	linkw %fp,#0                          
    575e:  206e 0008      	moveal %fp@(8),%a0                    
    5762:  4a28 002b      	tstb %a0@(43)                         
    5766:  670e           	beqs 0x5776                           ; ↓ label_0
    5768:  1028 002d      	moveb %a0@(45),%d0                    
    576c:  b02e 000c      	cmpb %fp@(12),%d0                     
    5770:  6604           	bnes 0x5776                           ; ↓ label_0
    5772:  7001           	moveq #1,%d0                          
    5774:  6002           	bras 0x5778                           ; ↓ label_1

  label_0:
    5776:  7000           	moveq #0,%d0                          

  label_1:
    5778:  4e5e           	unlk %fp                              
    577a:  4e75           	rts                                   
```

# PrvCheckFunction: 0x5790 - 0x5822

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(18)`     : ???

Locals:

   * `%fp@(-1)`     : ???
   * `%fp@(-2)`     : ???

```
    5790:  4e56 fffe      	linkw %fp,#-2                         
    5794:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    5798:  246e 0008      	moveal %fp@(8),%a2                    
    579c:  266e 0012      	moveal %fp@(18),%a3                   
    57a0:  2f0b           	movel %a3,%sp@-                       
    57a2:  1f2e 000c      	moveb %fp@(12),%sp@-                  
    57a6:  2f0a           	movel %a2,%sp@-                       
    57a8:  4eba 008e      	jsr %pc@(0x5838)                      ; PrvChangeFunction
    57ac:  3600           	movew %d0,%d3                         
    57ae:  4fef 000a      	lea %sp@(10),%sp                      
    57b2:  6704           	beqs 0x57b8                           ; ↓ label_0
    57b4:  3003           	movew %d3,%d0                         
    57b6:  6064           	bras 0x581c                           ; ↓ label_3

  label_0:
    57b8:  2f0a           	movel %a2,%sp@-                       
    57ba:  4eba cafa      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    57be:  2f0b           	movel %a3,%sp@-                       
    57c0:  2f2a 003a      	movel %a2@(58),%sp@-                  
    57c4:  1f3c 000f      	moveb #15,%sp@-                       
    57c8:  1f3c 0010      	moveb #16,%sp@-                       
    57cc:  1f3c 0002      	moveb #2,%sp@-                        
    57d0:  1f3c 0006      	moveb #6,%sp@-                        
    57d4:  4eba 01f4      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    57d8:  3600           	movew %d0,%d3                         
    57da:  4fef 0014      	lea %sp@(20),%sp                      
    57de:  670c           	beqs 0x57ec                           ; ↓ label_1
    57e0:  2f0a           	movel %a2,%sp@-                       
    57e2:  4eba ca58      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    57e6:  3003           	movew %d3,%d0                         
    57e8:  584f           	addqw #4,%sp                          
    57ea:  6030           	bras 0x581c                           ; ↓ label_3

  label_1:
    57ec:  2f0b           	movel %a3,%sp@-                       
    57ee:  2f2a 003e      	movel %a2@(62),%sp@-                  
    57f2:  1f3c 0002      	moveb #2,%sp@-                        
    57f6:  486e fffe      	pea %fp@(-2)                          
    57fa:  4eba 0264      	jsr %pc@(0x5a60)                      ; PrvTpcReadReg
    57fe:  3600           	movew %d0,%d3                         
    5800:  2f0a           	movel %a2,%sp@-                       
    5802:  4eba ca38      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    5806:  4a43           	tstw %d3                              
    5808:  4fef 0012      	lea %sp@(18),%sp                      
    580c:  6704           	beqs 0x5812                           ; ↓ label_2
    580e:  3003           	movew %d3,%d0                         
    5810:  600a           	bras 0x581c                           ; ↓ label_3

  label_2:
    5812:  206e 000e      	moveal %fp@(14),%a0                   
    5816:  10ae ffff      	moveb %fp@(-1),%a0@                   
    581a:  3003           	movew %d3,%d0                         

  label_3:
    581c:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    5820:  4e5e           	unlk %fp                              
    5822:  4e75           	rts                                   
```

# PrvChangeFunction: 0x5838 - 0x58e2

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-8)`     : ???

```
    5838:  4e56 fff8      	linkw %fp,#-8                         
    583c:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    5840:  246e 0008      	moveal %fp@(8),%a2                    
    5844:  266e 000e      	moveal %fp@(14),%a3                   
    5848:  2f0a           	movel %a2,%sp@-                       
    584a:  4eba ca6a      	jsr %pc@(0x22b6)                      ; DisableIntFromIOStick
    584e:  2f0b           	movel %a3,%sp@-                       
    5850:  2f2a 003a      	movel %a2@(58),%sp@-                  
    5854:  1f3c 0001      	moveb #1,%sp@-                        
    5858:  1f3c 0006      	moveb #6,%sp@-                        
    585c:  1f3c 0007      	moveb #7,%sp@-                        
    5860:  4227           	clrb %sp@-                            
    5862:  4eba 0166      	jsr %pc@(0x59ca)                      ; PrvTpcSetRWRegAdrs
    5866:  3600           	movew %d0,%d3                         
    5868:  4fef 0014      	lea %sp@(20),%sp                      
    586c:  670c           	beqs 0x587a                           ; ↓ label_0
    586e:  2f0a           	movel %a2,%sp@-                       
    5870:  4eba c9ca      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    5874:  3003           	movew %d3,%d0                         
    5876:  584f           	addqw #4,%sp                          
    5878:  6062           	bras 0x58dc                           ; ↓ label_4

  label_0:
    587a:  2f0b           	movel %a3,%sp@-                       
    587c:  2f2a 003a      	movel %a2@(58),%sp@-                  
    5880:  1f2e 000c      	moveb %fp@(12),%sp@-                  
    5884:  4eba 0302      	jsr %pc@(0x5b88)                      ; PrvTpcWrite_1_Reg
    5888:  3600           	movew %d0,%d3                         
    588a:  4fef 000a      	lea %sp@(10),%sp                      
    588e:  670c           	beqs 0x589c                           ; ↓ label_1
    5890:  2f0a           	movel %a2,%sp@-                       
    5892:  4eba c9a8      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    5896:  3003           	movew %d3,%d0                         
    5898:  584f           	addqw #4,%sp                          
    589a:  6040           	bras 0x58dc                           ; ↓ label_4

  label_1:
    589c:  2f0b           	movel %a3,%sp@-                       
    589e:  2f2a 003e      	movel %a2@(62),%sp@-                  
    58a2:  1f3c 0007      	moveb #7,%sp@-                        
    58a6:  486e fff8      	pea %fp@(-8)                          
    58aa:  4eba 01b4      	jsr %pc@(0x5a60)                      ; PrvTpcReadReg
    58ae:  3600           	movew %d0,%d3                         
    58b0:  4fef 000e      	lea %sp@(14),%sp                      
    58b4:  670c           	beqs 0x58c2                           ; ↓ label_2
    58b6:  2f0a           	movel %a2,%sp@-                       
    58b8:  4eba c982      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    58bc:  3003           	movew %d3,%d0                         
    58be:  584f           	addqw #4,%sp                          
    58c0:  601a           	bras 0x58dc                           ; ↓ label_4

  label_2:
    58c2:  2f0a           	movel %a2,%sp@-                       
    58c4:  4eba c976      	jsr %pc@(0x223c)                      ; EnableIntFromIOStick
    58c8:  102e fffe      	moveb %fp@(-2),%d0                    
    58cc:  b02e 000c      	cmpb %fp@(12),%d0                     
    58d0:  584f           	addqw #4,%sp                          
    58d2:  6706           	beqs 0x58da                           ; ↓ label_3
    58d4:  303c 8004      	movew #-32764,%d0                     
    58d8:  6002           	bras 0x58dc                           ; ↓ label_4

  label_3:
    58da:  3003           	movew %d3,%d0                         

  label_4:
    58dc:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    58e0:  4e5e           	unlk %fp                              
    58e2:  4e75           	rts                                   
```

# PrvWaitMsInt: 0x58f8 - 0x5932

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    58f8:  4e56 0000      	linkw %fp,#0                          
    58fc:  2f0a           	movel %a2,%sp@-                       
    58fe:  246e 000c      	moveal %fp@(12),%a2                   
    5902:  1012           	moveb %a2@,%d0                        
    5904:  0240 0040      	andiw #64,%d0                         
    5908:  6614           	bnes 0x591e                           ; ↓ label_0
    590a:  2f2e 0008      	movel %fp@(8),%sp@-                   
    590e:  2f0a           	movel %a2,%sp@-                       
    5910:  4eba 0456      	jsr %pc@(0x5d68)                      ; io_wait_X_int
    5914:  4a00           	tstb %d0                              
    5916:  504f           	addqw #8,%sp                          
    5918:  6704           	beqs 0x591e                           ; ↓ label_0
    591a:  7000           	moveq #0,%d0                          
    591c:  6010           	bras 0x592e                           ; ↓ label_2

  label_0:
    591e:  1012           	moveb %a2@,%d0                        
    5920:  0240 0040      	andiw #64,%d0                         
    5924:  6706           	beqs 0x592c                           ; ↓ label_1
    5926:  4212           	clrb %a2@                             
    5928:  7001           	moveq #1,%d0                          
    592a:  6002           	bras 0x592e                           ; ↓ label_2

  label_1:
    592c:  7000           	moveq #0,%d0                          

  label_2:
    592e:  245f           	moveal %sp@+,%a2                      
    5930:  4e5e           	unlk %fp                              
    5932:  4e75           	rts                                   
```

# PrvTpcSetCmd: 0x5944 - 0x59b8

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(14)`     : ???

Locals:

   * `%fp@(-2)`     : ???

```
    5944:  4e56 fffe      	linkw %fp,#-2                         
    5948:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    594c:  162e 0008      	moveb %fp@(8),%d3                     
    5950:  246e 000e      	moveal %fp@(14),%a2                   
    5954:  0c03 00aa      	cmpib #-86,%d3                        
    5958:  670c           	beqs 0x5966                           ; ↓ label_0
    595a:  0c03 0055      	cmpib #85,%d3                         
    595e:  6706           	beqs 0x5966                           ; ↓ label_0
    5960:  0c03 0099      	cmpib #-103,%d3                       
    5964:  6606           	bnes 0x596c                           ; ↓ label_1

  label_0:
    5966:  303c 4005      	movew #16389,%d0                      
    596a:  6046           	bras 0x59b2                           ; ↓ label_4

  label_1:
    596c:  7000           	moveq #0,%d0                          
    596e:  1003           	moveb %d3,%d0                         
    5970:  e148           	lslw #8,%d0                           
    5972:  3d40 fffe      	movew %d0,%fp@(-2)                    
    5976:  2f2e 000a      	movel %fp@(10),%sp@-                  
    597a:  2f0a           	movel %a2,%sp@-                       
    597c:  486e fffe      	pea %fp@(-2)                          
    5980:  3f3c 0001      	movew #1,%sp@-                        
    5984:  1f3c 000e      	moveb #14,%sp@-                       
    5988:  4eba 04be      	jsr %pc@(0x5e48)                      ; io_write_protocol
    598c:  4a00           	tstb %d0                              
    598e:  4fef 0010      	lea %sp@(16),%sp                      
    5992:  6706           	beqs 0x599a                           ; ↓ label_2
    5994:  303c 1e00      	movew #7680,%d0                       
    5998:  6018           	bras 0x59b2                           ; ↓ label_4

  label_2:
    599a:  1812           	moveb %a2@,%d4                        
    599c:  7000           	moveq #0,%d0                          
    599e:  1004           	moveb %d4,%d0                         
    59a0:  7201           	moveq #1,%d1                          
    59a2:  c081           	andl %d1,%d0                          
    59a4:  6604           	bnes 0x59aa                           ; ↓ label_3
    59a6:  7000           	moveq #0,%d0                          
    59a8:  6008           	bras 0x59b2                           ; ↓ label_4

  label_3:
    59aa:  7000           	moveq #0,%d0                          
    59ac:  1004           	moveb %d4,%d0                         
    59ae:  0040 2e00      	oriw #11776,%d0                       

  label_4:
    59b2:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    59b6:  4e5e           	unlk %fp                              
    59b8:  4e75           	rts                                   
```

# PrvTpcSetRWRegAdrs: 0x59ca - 0x5a48

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???

```
    59ca:  4e56 fffc      	linkw %fp,#-4                         
    59ce:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    59d2:  2a2e 0010      	movel %fp@(16),%d5                    
    59d6:  246e 0014      	moveal %fp@(20),%a2                   
    59da:  7603           	moveq #3,%d3                          
    59dc:  7000           	moveq #0,%d0                          
    59de:  102e 000a      	moveb %fp@(10),%d0                    
    59e2:  7200           	moveq #0,%d1                          
    59e4:  122e 0008      	moveb %fp@(8),%d1                     
    59e8:  e149           	lslw #8,%d1                           
    59ea:  d240           	addw %d0,%d1                          
    59ec:  3d41 fffc      	movew %d1,%fp@(-4)                    
    59f0:  7000           	moveq #0,%d0                          
    59f2:  102e 000e      	moveb %fp@(14),%d0                    
    59f6:  7200           	moveq #0,%d1                          
    59f8:  122e 000c      	moveb %fp@(12),%d1                    
    59fc:  e149           	lslw #8,%d1                           
    59fe:  d240           	addw %d0,%d1                          
    5a00:  3d41 fffe      	movew %d1,%fp@(-2)                    

  label_0:
    5a04:  2f05           	movel %d5,%sp@-                       
    5a06:  2f0a           	movel %a2,%sp@-                       
    5a08:  486e fffc      	pea %fp@(-4)                          
    5a0c:  3f3c 0004      	movew #4,%sp@-                        
    5a10:  1f3c 0008      	moveb #8,%sp@-                        
    5a14:  4eba 0432      	jsr %pc@(0x5e48)                      ; io_write_protocol
    5a18:  4a00           	tstb %d0                              
    5a1a:  4fef 0010      	lea %sp@(16),%sp                      
    5a1e:  6706           	beqs 0x5a26                           ; ↓ label_1
    5a20:  303c 1800      	movew #6144,%d0                       
    5a24:  601c           	bras 0x5a42                           ; ↓ label_3

  label_1:
    5a26:  1812           	moveb %a2@,%d4                        
    5a28:  7000           	moveq #0,%d0                          
    5a2a:  1004           	moveb %d4,%d0                         
    5a2c:  7201           	moveq #1,%d1                          
    5a2e:  c081           	andl %d1,%d0                          
    5a30:  6604           	bnes 0x5a36                           ; ↓ label_2
    5a32:  7000           	moveq #0,%d0                          
    5a34:  600c           	bras 0x5a42                           ; ↓ label_3

  label_2:
    5a36:  5383           	subql #1,%d3                          
    5a38:  66ca           	bnes 0x5a04                           ; ↑ label_0
    5a3a:  7000           	moveq #0,%d0                          
    5a3c:  1004           	moveb %d4,%d0                         
    5a3e:  0040 2800      	oriw #10240,%d0                       

  label_3:
    5a42:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    5a46:  4e5e           	unlk %fp                              
    5a48:  4e75           	rts                                   
```

# PrvTpcReadReg: 0x5a60 - 0x5abc

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(18)`     : ???

```
    5a60:  4e56 0000      	linkw %fp,#0                          
    5a64:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-          
    5a68:  266e 0008      	moveal %fp@(8),%a3                    
    5a6c:  2c2e 000e      	movel %fp@(14),%d6                    
    5a70:  246e 0012      	moveal %fp@(18),%a2                   
    5a74:  7603           	moveq #3,%d3                          
    5a76:  7800           	moveq #0,%d4                          
    5a78:  182e 000c      	moveb %fp@(12),%d4                    

  label_0:
    5a7c:  2f06           	movel %d6,%sp@-                       
    5a7e:  2f0a           	movel %a2,%sp@-                       
    5a80:  2f0b           	movel %a3,%sp@-                       
    5a82:  3f04           	movew %d4,%sp@-                       
    5a84:  1f3c 0004      	moveb #4,%sp@-                        
    5a88:  4eba 04f0      	jsr %pc@(0x5f7a)                      ; io_read_protocol
    5a8c:  4a00           	tstb %d0                              
    5a8e:  4fef 0010      	lea %sp@(16),%sp                      
    5a92:  6706           	beqs 0x5a9a                           ; ↓ label_1
    5a94:  303c 1400      	movew #5120,%d0                       
    5a98:  601c           	bras 0x5ab6                           ; ↓ label_3

  label_1:
    5a9a:  1a12           	moveb %a2@,%d5                        
    5a9c:  7000           	moveq #0,%d0                          
    5a9e:  1005           	moveb %d5,%d0                         
    5aa0:  7203           	moveq #3,%d1                          
    5aa2:  c081           	andl %d1,%d0                          
    5aa4:  6604           	bnes 0x5aaa                           ; ↓ label_2
    5aa6:  7000           	moveq #0,%d0                          
    5aa8:  600c           	bras 0x5ab6                           ; ↓ label_3

  label_2:
    5aaa:  5343           	subqw #1,%d3                          
    5aac:  6ece           	bgts 0x5a7c                           ; ↑ label_0
    5aae:  7000           	moveq #0,%d0                          
    5ab0:  1005           	moveb %d5,%d0                         
    5ab2:  0040 2400      	oriw #9216,%d0                        

  label_3:
    5ab6:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3          
    5aba:  4e5e           	unlk %fp                              
    5abc:  4e75           	rts                                   
```

# PrvTpcRead_1_Reg: 0x5ace - 0x5b02

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???

Locals:

   * `%fp@(-2)`     : ???

```
    5ace:  4e56 fffe      	linkw %fp,#-2                         
    5ad2:  2f03           	movel %d3,%sp@-                       
    5ad4:  2f2e 0010      	movel %fp@(16),%sp@-                  
    5ad8:  2f2e 000c      	movel %fp@(12),%sp@-                  
    5adc:  1f3c 0001      	moveb #1,%sp@-                        
    5ae0:  486e fffe      	pea %fp@(-2)                          
    5ae4:  4eba ff7a      	jsr %pc@(0x5a60)                      ; PrvTpcReadReg
    5ae8:  3600           	movew %d0,%d3                         
    5aea:  4fef 000e      	lea %sp@(14),%sp                      
    5aee:  6704           	beqs 0x5af4                           ; ↓ label_0
    5af0:  3003           	movew %d3,%d0                         
    5af2:  600a           	bras 0x5afe                           ; ↓ label_1

  label_0:
    5af4:  206e 0008      	moveal %fp@(8),%a0                    
    5af8:  10ae fffe      	moveb %fp@(-2),%a0@                   
    5afc:  7000           	moveq #0,%d0                          

  label_1:
    5afe:  261f           	movel %sp@+,%d3                       
    5b00:  4e5e           	unlk %fp                              
    5b02:  4e75           	rts                                   
```

# PrvTpcWriteReg: 0x5b18 - 0x5b74

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(18)`     : ???

```
    5b18:  4e56 0000      	linkw %fp,#0                          
    5b1c:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-          
    5b20:  266e 0008      	moveal %fp@(8),%a3                    
    5b24:  2c2e 000e      	movel %fp@(14),%d6                    
    5b28:  246e 0012      	moveal %fp@(18),%a2                   
    5b2c:  7603           	moveq #3,%d3                          
    5b2e:  7800           	moveq #0,%d4                          
    5b30:  182e 000c      	moveb %fp@(12),%d4                    

  label_0:
    5b34:  2f06           	movel %d6,%sp@-                       
    5b36:  2f0a           	movel %a2,%sp@-                       
    5b38:  2f0b           	movel %a3,%sp@-                       
    5b3a:  3f04           	movew %d4,%sp@-                       
    5b3c:  1f3c 000b      	moveb #11,%sp@-                       
    5b40:  4eba 0306      	jsr %pc@(0x5e48)                      ; io_write_protocol
    5b44:  4a00           	tstb %d0                              
    5b46:  4fef 0010      	lea %sp@(16),%sp                      
    5b4a:  6706           	beqs 0x5b52                           ; ↓ label_1
    5b4c:  303c 1b00      	movew #6912,%d0                       
    5b50:  601c           	bras 0x5b6e                           ; ↓ label_3

  label_1:
    5b52:  1a12           	moveb %a2@,%d5                        
    5b54:  7000           	moveq #0,%d0                          
    5b56:  1005           	moveb %d5,%d0                         
    5b58:  7201           	moveq #1,%d1                          
    5b5a:  c081           	andl %d1,%d0                          
    5b5c:  6604           	bnes 0x5b62                           ; ↓ label_2
    5b5e:  7000           	moveq #0,%d0                          
    5b60:  600c           	bras 0x5b6e                           ; ↓ label_3

  label_2:
    5b62:  5343           	subqw #1,%d3                          
    5b64:  6ece           	bgts 0x5b34                           ; ↑ label_0
    5b66:  7000           	moveq #0,%d0                          
    5b68:  1005           	moveb %d5,%d0                         
    5b6a:  0040 2b00      	oriw #11008,%d0                       

  label_3:
    5b6e:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3          
    5b72:  4e5e           	unlk %fp                              
    5b74:  4e75           	rts                                   
```

# PrvTpcWrite_1_Reg: 0x5b88 - 0x5ba8

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(14)`     : ???

Locals:

   * `%fp@(-2)`     : ???

```
    5b88:  4e56 fffe      	linkw %fp,#-2                         
    5b8c:  1d6e 0008 fffe 	moveb %fp@(8),%fp@(-2)                
    5b92:  2f2e 000e      	movel %fp@(14),%sp@-                  
    5b96:  2f2e 000a      	movel %fp@(10),%sp@-                  
    5b9a:  1f3c 0001      	moveb #1,%sp@-                        
    5b9e:  486e fffe      	pea %fp@(-2)                          
    5ba2:  4eba ff74      	jsr %pc@(0x5b18)                      ; PrvTpcWriteReg
    5ba6:  4e5e           	unlk %fp                              
    5ba8:  4e75           	rts                                   
```

# PrvTpcReadData: 0x5bbe - 0x5c0a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(18)`     : ???

```
    5bbe:  4e56 0000      	linkw %fp,#0                          
    5bc2:  2f03           	movel %d3,%sp@-                       
    5bc4:  2f2e 000e      	movel %fp@(14),%sp@-                  
    5bc8:  2f2e 0012      	movel %fp@(18),%sp@-                  
    5bcc:  2f2e 0008      	movel %fp@(8),%sp@-                   
    5bd0:  3f2e 000c      	movew %fp@(12),%sp@-                  
    5bd4:  1f3c 0002      	moveb #2,%sp@-                        
    5bd8:  4eba 03a0      	jsr %pc@(0x5f7a)                      ; io_read_protocol
    5bdc:  4a00           	tstb %d0                              
    5bde:  4fef 0010      	lea %sp@(16),%sp                      
    5be2:  6706           	beqs 0x5bea                           ; ↓ label_0
    5be4:  303c 1200      	movew #4608,%d0                       
    5be8:  601c           	bras 0x5c06                           ; ↓ label_2

  label_0:
    5bea:  206e 0012      	moveal %fp@(18),%a0                   
    5bee:  1610           	moveb %a0@,%d3                        
    5bf0:  7000           	moveq #0,%d0                          
    5bf2:  1003           	moveb %d3,%d0                         
    5bf4:  7203           	moveq #3,%d1                          
    5bf6:  c081           	andl %d1,%d0                          
    5bf8:  6604           	bnes 0x5bfe                           ; ↓ label_1
    5bfa:  7000           	moveq #0,%d0                          
    5bfc:  6008           	bras 0x5c06                           ; ↓ label_2

  label_1:
    5bfe:  7000           	moveq #0,%d0                          
    5c00:  1003           	moveb %d3,%d0                         
    5c02:  0040 2200      	oriw #8704,%d0                        

  label_2:
    5c06:  261f           	movel %sp@+,%d3                       
    5c08:  4e5e           	unlk %fp                              
    5c0a:  4e75           	rts                                   
```

# PrvTpcWriteData: 0x5c1e - 0x5c6a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(18)`     : ???

```
    5c1e:  4e56 0000      	linkw %fp,#0                          
    5c22:  2f03           	movel %d3,%sp@-                       
    5c24:  2f2e 000e      	movel %fp@(14),%sp@-                  
    5c28:  2f2e 0012      	movel %fp@(18),%sp@-                  
    5c2c:  2f2e 0008      	movel %fp@(8),%sp@-                   
    5c30:  3f2e 000c      	movew %fp@(12),%sp@-                  
    5c34:  1f3c 000d      	moveb #13,%sp@-                       
    5c38:  4eba 020e      	jsr %pc@(0x5e48)                      ; io_write_protocol
    5c3c:  4a00           	tstb %d0                              
    5c3e:  4fef 0010      	lea %sp@(16),%sp                      
    5c42:  6706           	beqs 0x5c4a                           ; ↓ label_0
    5c44:  303c 1d00      	movew #7424,%d0                       
    5c48:  601c           	bras 0x5c66                           ; ↓ label_2

  label_0:
    5c4a:  206e 0012      	moveal %fp@(18),%a0                   
    5c4e:  1610           	moveb %a0@,%d3                        
    5c50:  7000           	moveq #0,%d0                          
    5c52:  1003           	moveb %d3,%d0                         
    5c54:  7201           	moveq #1,%d1                          
    5c56:  c081           	andl %d1,%d0                          
    5c58:  6604           	bnes 0x5c5e                           ; ↓ label_1
    5c5a:  7000           	moveq #0,%d0                          
    5c5c:  6008           	bras 0x5c66                           ; ↓ label_2

  label_1:
    5c5e:  7000           	moveq #0,%d0                          
    5c60:  1003           	moveb %d3,%d0                         
    5c62:  0040 2d00      	oriw #11520,%d0                       

  label_2:
    5c66:  261f           	movel %sp@+,%d3                       
    5c68:  4e5e           	unlk %fp                              
    5c6a:  4e75           	rts                                   
```

# PrvTpcUniqTPCRead: 0x5c7e - 0x5cdc

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

```
    5c7e:  4e56 0000      	linkw %fp,#0                          
    5c82:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    5c86:  182e 0008      	moveb %fp@(8),%d4                     
    5c8a:  246e 0014      	moveal %fp@(20),%a2                   
    5c8e:  2f2e 0010      	movel %fp@(16),%sp@-                  
    5c92:  2f0a           	movel %a2,%sp@-                       
    5c94:  2f2e 000a      	movel %fp@(10),%sp@-                  
    5c98:  3f2e 000e      	movew %fp@(14),%sp@-                  
    5c9c:  1f04           	moveb %d4,%sp@-                       
    5c9e:  4eba 02da      	jsr %pc@(0x5f7a)                      ; io_read_protocol
    5ca2:  4a00           	tstb %d0                              
    5ca4:  4fef 0010      	lea %sp@(16),%sp                      
    5ca8:  670a           	beqs 0x5cb4                           ; ↓ label_0
    5caa:  7000           	moveq #0,%d0                          
    5cac:  1004           	moveb %d4,%d0                         
    5cae:  0040 1000      	oriw #4096,%d0                        
    5cb2:  6022           	bras 0x5cd6                           ; ↓ label_2

  label_0:
    5cb4:  1612           	moveb %a2@,%d3                        
    5cb6:  7000           	moveq #0,%d0                          
    5cb8:  1003           	moveb %d3,%d0                         
    5cba:  7203           	moveq #3,%d1                          
    5cbc:  c081           	andl %d1,%d0                          
    5cbe:  6604           	bnes 0x5cc4                           ; ↓ label_1
    5cc0:  7000           	moveq #0,%d0                          
    5cc2:  6012           	bras 0x5cd6                           ; ↓ label_2

  label_1:
    5cc4:  7000           	moveq #0,%d0                          
    5cc6:  1003           	moveb %d3,%d0                         
    5cc8:  7200           	moveq #0,%d1                          
    5cca:  1204           	moveb %d4,%d1                         
    5ccc:  e149           	lslw #8,%d1                           
    5cce:  0041 2000      	oriw #8192,%d1                        
    5cd2:  8240           	orw %d0,%d1                           
    5cd4:  3001           	movew %d1,%d0                         

  label_2:
    5cd6:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    5cda:  4e5e           	unlk %fp                              
    5cdc:  4e75           	rts                                   
```

# PrvTpcUniqTPCWrite: 0x5cf2 - 0x5d50

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

```
    5cf2:  4e56 0000      	linkw %fp,#0                          
    5cf6:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    5cfa:  182e 0008      	moveb %fp@(8),%d4                     
    5cfe:  246e 0014      	moveal %fp@(20),%a2                   
    5d02:  2f2e 0010      	movel %fp@(16),%sp@-                  
    5d06:  2f0a           	movel %a2,%sp@-                       
    5d08:  2f2e 000a      	movel %fp@(10),%sp@-                  
    5d0c:  3f2e 000e      	movew %fp@(14),%sp@-                  
    5d10:  1f04           	moveb %d4,%sp@-                       
    5d12:  4eba 0134      	jsr %pc@(0x5e48)                      ; io_write_protocol
    5d16:  4a00           	tstb %d0                              
    5d18:  4fef 0010      	lea %sp@(16),%sp                      
    5d1c:  670a           	beqs 0x5d28                           ; ↓ label_0
    5d1e:  7000           	moveq #0,%d0                          
    5d20:  1004           	moveb %d4,%d0                         
    5d22:  0040 1000      	oriw #4096,%d0                        
    5d26:  6022           	bras 0x5d4a                           ; ↓ label_2

  label_0:
    5d28:  1612           	moveb %a2@,%d3                        
    5d2a:  7000           	moveq #0,%d0                          
    5d2c:  1003           	moveb %d3,%d0                         
    5d2e:  7201           	moveq #1,%d1                          
    5d30:  c081           	andl %d1,%d0                          
    5d32:  6604           	bnes 0x5d38                           ; ↓ label_1
    5d34:  7000           	moveq #0,%d0                          
    5d36:  6012           	bras 0x5d4a                           ; ↓ label_2

  label_1:
    5d38:  7000           	moveq #0,%d0                          
    5d3a:  1003           	moveb %d3,%d0                         
    5d3c:  7200           	moveq #0,%d1                          
    5d3e:  1204           	moveb %d4,%d1                         
    5d40:  e149           	lslw #8,%d1                           
    5d42:  0041 2000      	oriw #8192,%d1                        
    5d46:  8240           	orw %d0,%d1                           
    5d48:  3001           	movew %d1,%d0                         

  label_2:
    5d4a:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    5d4e:  4e5e           	unlk %fp                              
    5d50:  4e75           	rts                                   
```

# io_wait_X_int: 0x5d68 - 0x5dde

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    5d68:  4e56 0000      	linkw %fp,#0                          
    5d6c:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    5d70:  246e 0008      	moveal %fp@(8),%a2                    
    5d74:  2a2e 000c      	movel %fp@(12),%d5                    
    5d78:  6612           	bnes 0x5d8c                           ; ↓ label_0
    5d7a:  3039 1100 0406 	movew 0x11000406,%d0                  
    5d80:  e048           	lsrw #8,%d0                           
    5d82:  0240 00ff      	andiw #255,%d0                        
    5d86:  1480           	moveb %d0,%a2@                        
    5d88:  7000           	moveq #0,%d0                          
    5d8a:  604c           	bras 0x5dd8                           ; ↓ label_3

  label_0:
    5d8c:  7800           	moveq #0,%d4                          
    5d8e:  7600           	moveq #0,%d3                          

  label_1:
    5d90:  3039 1100 0402 	movew 0x11000402,%d0                  
    5d96:  0240 8000      	andiw #-32768,%d0                     
    5d9a:  6714           	beqs 0x5db0                           ; ↓ label_2
    5d9c:  3639 1100 0406 	movew 0x11000406,%d3                  
    5da2:  3003           	movew %d3,%d0                         
    5da4:  e048           	lsrw #8,%d0                           
    5da6:  0240 00ff      	andiw #255,%d0                        
    5daa:  1480           	moveb %d0,%a2@                        
    5dac:  7000           	moveq #0,%d0                          
    5dae:  6028           	bras 0x5dd8                           ; ↓ label_3

  label_2:
    5db0:  4878 0064      	pea 0x64                              
    5db4:  4e4f           	trap #15                              
    5db6:  a249           	0121111                               ; sysTrapHwrDelay
    5db8:  7064           	moveq #100,%d0                        
    5dba:  d680           	addl %d0,%d3                          
    5dbc:  5284           	addql #1,%d4                          
    5dbe:  b685           	cmpl %d5,%d3                          
    5dc0:  584f           	addqw #4,%sp                          
    5dc2:  65cc           	bcss 0x5d90                           ; ↑ label_1
    5dc4:  3039 1100 0406 	movew 0x11000406,%d0                  
    5dca:  e048           	lsrw #8,%d0                           
    5dcc:  0240 00ff      	andiw #255,%d0                        
    5dd0:  0040 0001      	oriw #1,%d0                           
    5dd4:  1480           	moveb %d0,%a2@                        
    5dd6:  7000           	moveq #0,%d0                          

  label_3:
    5dd8:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    5ddc:  4e5e           	unlk %fp                              
    5dde:  4e75           	rts                                   
```

# io_wait_drq: 0x5df0 - 0x5e38

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    5df0:  4e56 0000      	linkw %fp,#0                          
    5df4:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    5df8:  246e 0008      	moveal %fp@(8),%a2                    
    5dfc:  2a2e 000c      	movel %fp@(12),%d5                    
    5e00:  7800           	moveq #0,%d4                          
    5e02:  7600           	moveq #0,%d3                          

  label_0:
    5e04:  3039 1100 0402 	movew 0x11000402,%d0                  
    5e0a:  0240 4000      	andiw #16384,%d0                      
    5e0e:  6708           	beqs 0x5e18                           ; ↓ label_1
    5e10:  14bc 0020      	moveb #32,%a2@                        
    5e14:  7000           	moveq #0,%d0                          
    5e16:  601a           	bras 0x5e32                           ; ↓ label_2

  label_1:
    5e18:  4878 000a      	pea 0xa                               
    5e1c:  4e4f           	trap #15                              
    5e1e:  a249           	0121111                               ; sysTrapHwrDelay
    5e20:  700a           	moveq #10,%d0                         
    5e22:  d680           	addl %d0,%d3                          
    5e24:  5284           	addql #1,%d4                          
    5e26:  b685           	cmpl %d5,%d3                          
    5e28:  584f           	addqw #4,%sp                          
    5e2a:  65d8           	bcss 0x5e04                           ; ↑ label_0
    5e2c:  14bc 0001      	moveb #1,%a2@                         
    5e30:  7000           	moveq #0,%d0                          

  label_2:
    5e32:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    5e36:  4e5e           	unlk %fp                              
    5e38:  4e75           	rts                                   
```

# io_write_protocol: 0x5e48 - 0x5f64

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

```
    5e48:  4e56 0000      	linkw %fp,#0                          
    5e4c:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-          
    5e50:  3a2e 000a      	movew %fp@(10),%d5                    
    5e54:  266e 000c      	moveal %fp@(12),%a3                   
    5e58:  246e 0010      	moveal %fp@(16),%a2                   
    5e5c:  2c2e 0014      	movel %fp@(20),%d6                    
    5e60:  7800           	moveq #0,%d4                          
    5e62:  0079 0020 1100 	oriw #32,0x11000402                   
    5e68:  0402                                                  
    5e6a:  0279 fff8 1100 	andiw #-8,0x11000402                  
    5e70:  0402                                                  
    5e72:  4a86           	tstl %d6                              
    5e74:  6678           	bnes 0x5eee                           ; ↓ label_4
    5e76:  0079 0005 1100 	oriw #5,0x11000402                    
    5e7c:  0402                                                  
    5e7e:  7000           	moveq #0,%d0                          
    5e80:  102e 0008      	moveb %fp@(8),%d0                     
    5e84:  720c           	moveq #12,%d1                         
    5e86:  e368           	lslw %d1,%d0                          
    5e88:  d045           	addw %d5,%d0                          
    5e8a:  33c0 1100 0400 	movew %d0,0x11000400                  
    5e90:  7600           	moveq #0,%d3                          
    5e92:  600a           	bras 0x5e9e                           ; ↓ label_1

  label_0:
    5e94:  33f3 3800 1100 	movew %a3@(00000000,%d3:l),0x11000404 
    5e9a:  0404                                                  
    5e9c:  5483           	addql #2,%d3                          

  label_1:
    5e9e:  7000           	moveq #0,%d0                          
    5ea0:  3005           	movew %d5,%d0                         
    5ea2:  b680           	cmpl %d0,%d3                          
    5ea4:  6dee           	blts 0x5e94                           ; ↑ label_0
    5ea6:  4e71           	nop                                   
    5ea8:  4e71           	nop                                   
    5eaa:  4e71           	nop                                   
    5eac:  4e71           	nop                                   
    5eae:  4e71           	nop                                   
    5eb0:  4e71           	nop                                   
    5eb2:  4e71           	nop                                   
    5eb4:  4e71           	nop                                   
    5eb6:  4e71           	nop                                   
    5eb8:  4e71           	nop                                   

  label_2:
    5eba:  3039 1100 0406 	movew 0x11000406,%d0                  
    5ec0:  0240 8000      	andiw #-32768,%d0                     
    5ec4:  6714           	beqs 0x5eda                           ; ↓ label_3
    5ec6:  3039 1100 0406 	movew 0x11000406,%d0                  
    5ecc:  e048           	lsrw #8,%d0                           
    5ece:  0240 00ff      	andiw #255,%d0                        
    5ed2:  1480           	moveb %d0,%a2@                        
    5ed4:  7000           	moveq #0,%d0                          
    5ed6:  6000 0086      	braw 0x5f5e                           ; ↓ label_10

  label_3:
    5eda:  4878 0014      	pea 0x14                              
    5ede:  4e4f           	trap #15                              
    5ee0:  a249           	0121111                               ; sysTrapHwrDelay
    5ee2:  5284           	addql #1,%d4                          
    5ee4:  7032           	moveq #50,%d0                         
    5ee6:  b880           	cmpl %d0,%d4                          
    5ee8:  584f           	addqw #4,%sp                          
    5eea:  65ce           	bcss 0x5eba                           ; ↑ label_2
    5eec:  606e           	bras 0x5f5c                           ; ↓ label_9

  label_4:
    5eee:  7000           	moveq #0,%d0                          
    5ef0:  102e 0008      	moveb %fp@(8),%d0                     
    5ef4:  720c           	moveq #12,%d1                         
    5ef6:  e368           	lslw %d1,%d0                          
    5ef8:  d045           	addw %d5,%d0                          
    5efa:  33c0 1100 0400 	movew %d0,0x11000400                  
    5f00:  7600           	moveq #0,%d3                          
    5f02:  600a           	bras 0x5f0e                           ; ↓ label_6

  label_5:
    5f04:  33f3 3800 1100 	movew %a3@(00000000,%d3:l),0x11000404 
    5f0a:  0404                                                  
    5f0c:  5483           	addql #2,%d3                          

  label_6:
    5f0e:  7000           	moveq #0,%d0                          
    5f10:  3005           	movew %d5,%d0                         
    5f12:  b680           	cmpl %d0,%d3                          
    5f14:  6dee           	blts 0x5f04                           ; ↑ label_5
    5f16:  4e71           	nop                                   
    5f18:  4e71           	nop                                   
    5f1a:  4e71           	nop                                   
    5f1c:  4e71           	nop                                   
    5f1e:  4e71           	nop                                   
    5f20:  4e71           	nop                                   
    5f22:  4e71           	nop                                   
    5f24:  4e71           	nop                                   
    5f26:  4e71           	nop                                   
    5f28:  4e71           	nop                                   

  label_7:
    5f2a:  3039 1100 0406 	movew 0x11000406,%d0                  
    5f30:  0240 8000      	andiw #-32768,%d0                     
    5f34:  6712           	beqs 0x5f48                           ; ↓ label_8
    5f36:  3039 1100 0406 	movew 0x11000406,%d0                  
    5f3c:  e048           	lsrw #8,%d0                           
    5f3e:  0240 00ff      	andiw #255,%d0                        
    5f42:  1480           	moveb %d0,%a2@                        
    5f44:  7000           	moveq #0,%d0                          
    5f46:  6016           	bras 0x5f5e                           ; ↓ label_10

  label_8:
    5f48:  4878 0010      	pea 0x10                              
    5f4c:  4e4f           	trap #15                              
    5f4e:  a249           	0121111                               ; sysTrapHwrDelay
    5f50:  5284           	addql #1,%d4                          
    5f52:  2004           	movel %d4,%d0                         
    5f54:  e988           	lsll #4,%d0                           
    5f56:  b086           	cmpl %d6,%d0                          
    5f58:  584f           	addqw #4,%sp                          
    5f5a:  65ce           	bcss 0x5f2a                           ; ↑ label_7

  label_9:
    5f5c:  7001           	moveq #1,%d0                          

  label_10:
    5f5e:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3          
    5f62:  4e5e           	unlk %fp                              
    5f64:  4e75           	rts                                   
```

# io_read_protocol: 0x5f7a - 0x606a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

```
    5f7a:  4e56 0000      	linkw %fp,#0                          
    5f7e:  48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-          
    5f82:  382e 000a      	movew %fp@(10),%d4                    
    5f86:  246e 000c      	moveal %fp@(12),%a2                   
    5f8a:  266e 0010      	moveal %fp@(16),%a3                   
    5f8e:  7a00           	moveq #0,%d5                          
    5f90:  0079 0020 1100 	oriw #32,0x11000402                   
    5f96:  0402                                                  
    5f98:  0279 fff8 1100 	andiw #-8,0x11000402                  
    5f9e:  0402                                                  
    5fa0:  4aae 0014      	tstl %fp@(20)                         
    5fa4:  6658           	bnes 0x5ffe                           ; ↓ label_5
    5fa6:  0079 0005 1100 	oriw #5,0x11000402                    
    5fac:  0402                                                  
    5fae:  7000           	moveq #0,%d0                          
    5fb0:  102e 0008      	moveb %fp@(8),%d0                     
    5fb4:  720c           	moveq #12,%d1                         
    5fb6:  e368           	lslw %d1,%d0                          
    5fb8:  d044           	addw %d4,%d0                          
    5fba:  33c0 1100 0400 	movew %d0,0x11000400                  
    5fc0:  7600           	moveq #0,%d3                          
    5fc2:  6032           	bras 0x5ff6                           ; ↓ label_4

  label_0:
    5fc4:  3039 1100 0402 	movew 0x11000402,%d0                  
    5fca:  0240 4000      	andiw #16384,%d0                      
    5fce:  6718           	beqs 0x5fe8                           ; ↓ label_3
    5fd0:  7600           	moveq #0,%d3                          
    5fd2:  600a           	bras 0x5fde                           ; ↓ label_2

  label_1:
    5fd4:  35b9 1100 0404 	movew 0x11000404,%a2@(00000000,%d3:l) 
    5fda:  3800                                                  
    5fdc:  5483           	addql #2,%d3                          

  label_2:
    5fde:  7000           	moveq #0,%d0                          
    5fe0:  3004           	movew %d4,%d0                         
    5fe2:  b680           	cmpl %d0,%d3                          
    5fe4:  6dee           	blts 0x5fd4                           ; ↑ label_1
    5fe6:  604a           	bras 0x6032                           ; ↓ label_8

  label_3:
    5fe8:  4e71           	nop                                   
    5fea:  4e71           	nop                                   
    5fec:  4e71           	nop                                   
    5fee:  4e71           	nop                                   
    5ff0:  4e71           	nop                                   
    5ff2:  4e71           	nop                                   
    5ff4:  5283           	addql #1,%d3                          

  label_4:
    5ff6:  7003           	moveq #3,%d0                          
    5ff8:  b680           	cmpl %d0,%d3                          
    5ffa:  6dc8           	blts 0x5fc4                           ; ↑ label_0
    5ffc:  6034           	bras 0x6032                           ; ↓ label_8

  label_5:
    5ffe:  7000           	moveq #0,%d0                          
    6000:  102e 0008      	moveb %fp@(8),%d0                     
    6004:  720c           	moveq #12,%d1                         
    6006:  e368           	lslw %d1,%d0                          
    6008:  d044           	addw %d4,%d0                          
    600a:  33c0 1100 0400 	movew %d0,0x11000400                  
    6010:  2f2e 0014      	movel %fp@(20),%sp@-                  
    6014:  2f0b           	movel %a3,%sp@-                       
    6016:  4eba fdd8      	jsr %pc@(0x5df0)                      ; io_wait_drq
    601a:  7600           	moveq #0,%d3                          
    601c:  504f           	addqw #8,%sp                          
    601e:  600a           	bras 0x602a                           ; ↓ label_7

  label_6:
    6020:  35b9 1100 0404 	movew 0x11000404,%a2@(00000000,%d3:l) 
    6026:  3800                                                  
    6028:  5483           	addql #2,%d3                          

  label_7:
    602a:  7000           	moveq #0,%d0                          
    602c:  3004           	movew %d4,%d0                         
    602e:  b680           	cmpl %d0,%d3                          
    6030:  6dee           	blts 0x6020                           ; ↑ label_6

  label_8:
    6032:  3039 1100 0406 	movew 0x11000406,%d0                  
    6038:  0240 8000      	andiw #-32768,%d0                     
    603c:  6712           	beqs 0x6050                           ; ↓ label_9
    603e:  3039 1100 0406 	movew 0x11000406,%d0                  
    6044:  e048           	lsrw #8,%d0                           
    6046:  0240 00ff      	andiw #255,%d0                        
    604a:  1680           	moveb %d0,%a3@                        
    604c:  7000           	moveq #0,%d0                          
    604e:  6014           	bras 0x6064                           ; ↓ label_10

  label_9:
    6050:  4878 0014      	pea 0x14                              
    6054:  4e4f           	trap #15                              
    6056:  a249           	0121111                               ; sysTrapHwrDelay
    6058:  5285           	addql #1,%d5                          
    605a:  7032           	moveq #50,%d0                         
    605c:  ba80           	cmpl %d0,%d5                          
    605e:  584f           	addqw #4,%sp                          
    6060:  65d0           	bcss 0x6032                           ; ↑ label_8
    6062:  7001           	moveq #1,%d0                          

  label_10:
    6064:  4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3          
    6068:  4e5e           	unlk %fp                              
    606a:  4e75           	rts                                   
```

