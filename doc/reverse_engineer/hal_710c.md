# anonymous_function_0: 0x007e - 0x0092

```
    7e:  48e7 3000      	moveml %d2-%d3,%sp@-                    
    82:  7402           	moveq #2,%d2                            
    84:  4efb 2200      	jmp %pc@(0x86,%d2:w:2)                  
    88:  600a           	bras 0x94                               
    8a:  4c01           	046001                                  
    8c:  0000 4cdf      	orib #-33,%d0                           
    90:  000c 4e75      	orib #117,%a4                           
```

# BBHBf6: 0x00b0 - 0x00c4

```
    b0:  48e7 3000      	moveml %d2-%d3,%sp@-                    
    b4:  7402           	moveq #2,%d2                            
    b6:  4efb 2200      	jmp %pc@(0xb8,%d2:w:2)                  
    ba:  600a           	bras 0xc6                               
    bc:  4c41           	046101                                  
    be:  0000 4cdf      	orib #-33,%d0                           
    c2:  000c 4e75      	orib #117,%a4                           
```

# anonymous_function_1: 0x010e - 0x012e

```
    10e:  4a80           	tstl %d0                               
    110:  6c0c           	bges 0x11e                             ; ↓ label_0
    112:  4480           	negl %d0                               
    114:  4a81           	tstl %d1                               
    116:  6c10           	bges 0x128                             ; ↓ label_2
    118:  4481           	negl %d1                               
    11a:  4efa ff94      	jmp %pc@(0xb0)                         ; BBHBf6

  label_0:
    11e:  4a81           	tstl %d1                               
    120:  6d04           	blts 0x126                             ; ↓ label_1
    122:  4efa ff8c      	jmp %pc@(0xb0)                         ; BBHBf6

  label_1:
    126:  4481           	negl %d1                               

  label_2:
    128:  4eba ff86      	jsr %pc@(0xb0)                         ; BBHBf6
    12c:  4480           	negl %d0                               
    12e:  4e75           	rts                                    
```

# BBHBf6: 0x0130 - 0x0146

```
    130:  48e7 3000      	moveml %d2-%d3,%sp@-                   
    134:  7402           	moveq #2,%d2                           
    136:  4efb 2200      	jmp %pc@(0x138,%d2:w:2)                
    13a:  600c           	bras 0x148                             
    13c:  4c41           	046101                                 
    13e:  0001 2001      	orib #1,%d1                            
    142:  4cdf 000c      	moveml %sp@+,%d2-%d3                   
    146:  4e75           	rts                                    
```

# HwrInit: 0x01a0 - 0x043a

```
    1a0:  4e56 0000      	linkw %fp,#0                           
    1a4:  2278 0122      	moveal 0x122,%a1                       
    1a8:  41fa 13da      	lea %pc@(0x1584),%a0                   ; HwrPreDebugInit
    1ac:  2348 0ecc      	movel %a0,%a1@(3788)                   
    1b0:  41fa 172a      	lea %pc@(0x18dc),%a0                   ; HwrResetNMI
    1b4:  2348 0ed0      	movel %a0,%a1@(3792)                   
    1b8:  41fa 1738      	lea %pc@(0x18f2),%a0                   ; HwrResetPWM
    1bc:  2348 0ed4      	movel %a0,%a1@(3796)                   
    1c0:  41fa 174c      	lea %pc@(0x190e),%a0                   ; HwrMemWritable
    1c4:  2348 08f4      	movel %a0,%a1@(2292)                   
    1c8:  41fa 17d6      	lea %pc@(0x19a0),%a0                   ; HwrMemReadable
    1cc:  2348 08f8      	movel %a0,%a1@(2296)                   
    1d0:  41fa 187a      	lea %pc@(0x1a4c),%a0                   ; ??? 0x1a4c
    1d4:  2348 0928      	movel %a0,%a1@(2344)                   
    1d8:  41fa 18a0      	lea %pc@(0x1a7a),%a0                   ; ??? 0x1a7a
    1dc:  2348 092c      	movel %a0,%a1@(2348)                   
    1e0:  41fa 69b4      	lea %pc@(0x6b96),%a0                   ; KeyBootKeys
    1e4:  2348 0ed8      	movel %a0,%a1@(3800)                   
    1e8:  41fa 039c      	lea %pc@(0x586),%a0                    ; HwrDelay
    1ec:  2348 0924      	movel %a0,%a1@(2340)                   
    1f0:  41fa 02a8      	lea %pc@(0x49a),%a0                    ; HwrTimerInit
    1f4:  2348 0918      	movel %a0,%a1@(2328)                   
    1f8:  41fa 0318      	lea %pc@(0x512),%a0                    ; HwrTimerSleep
    1fc:  2348 0938      	movel %a0,%a1@(2360)                   
    200:  41fa 034e      	lea %pc@(0x550),%a0                    ; HwrTimerWake
    204:  2348 093c      	movel %a0,%a1@(2364)                   
    208:  41fa 61c6      	lea %pc@(0x63d0),%a0                   ; DrvOpen
    20c:  2348 0edc      	movel %a0,%a1@(3804)                   
    210:  41fa 620a      	lea %pc@(0x641c),%a0                   ; DrvClose
    214:  2348 0ee0      	movel %a0,%a1@(3808)                   
    218:  41fa 6228      	lea %pc@(0x6442),%a0                   ; DrvControl
    21c:  2348 0ee4      	movel %a0,%a1@(3812)                   
    220:  41fa 643a      	lea %pc@(0x665c),%a0                   ; DrvStatus
    224:  2348 0ee8      	movel %a0,%a1@(3816)                   
    228:  41fa 651a      	lea %pc@(0x6744),%a0                   ; DrvWriteChar
    22c:  2348 0eec      	movel %a0,%a1@(3820)                   
    230:  41fa 64a4      	lea %pc@(0x66d6),%a0                   ; ??? 0x66d6
    234:  2348 0ef0      	movel %a0,%a1@(3824)                   
    238:  41fa 33ae      	lea %pc@(0x35e8),%a0                   ; HwrDoze
    23c:  2348 08fc      	movel %a0,%a1@(2300)                   
    240:  41fa 3402      	lea %pc@(0x3644),%a0                   ; HwrSetSystemClock
    244:  2348 0908      	movel %a0,%a1@(2312)                   
    248:  41fa 3506      	lea %pc@(0x3750),%a0                   ; HwrSetCPUDutyCycle
    24c:  2348 090c      	movel %a0,%a1@(2316)                   
    250:  41fa 3546      	lea %pc@(0x3798),%a0                   ; HwrGetRAMMapping
    254:  2348 08f0      	movel %a0,%a1@(2288)                   
    258:  41fa 363e      	lea %pc@(0x3898),%a0                   ; HwrSleep
    25c:  2348 0900      	movel %a0,%a1@(2304)                   
    260:  41fa 390e      	lea %pc@(0x3b70),%a0                   ; HwrWake
    264:  2348 0904      	movel %a0,%a1@(2308)                   
    268:  41fa 3b0e      	lea %pc@(0x3d78),%a0                   ; HwrPostDebugInit
    26c:  2348 0ef4      	movel %a0,%a1@(3828)                   
    270:  41fa 3b52      	lea %pc@(0x3dc4),%a0                   ; HwrIdentifyFeatures
    274:  2348 0ef8      	movel %a0,%a1@(3832)                   
    278:  41fa 3b84      	lea %pc@(0x3dfe),%a0                   ; HwrModelSpecificInit
    27c:  2348 0efc      	movel %a0,%a1@(3836)                   
    280:  41fa 3bc4      	lea %pc@(0x3e46),%a0                   ; HwrModelInitStage2
    284:  2348 0f00      	movel %a0,%a1@(3840)                   
    288:  41fa 18b6      	lea %pc@(0x1b40),%a0                   ; HwrDisplayInit
    28c:  2348 0910      	movel %a0,%a1@(2320)                   
    290:  41fa 191c      	lea %pc@(0x1bae),%a0                   ; HwrDisplayWake
    294:  2348 0940      	movel %a0,%a1@(2368)                   
    298:  41fa 188a      	lea %pc@(0x1b24),%a0                   ; HwrLCDContrast
    29c:  2348 0d28      	movel %a0,%a1@(3368)                   
    2a0:  41fa 186a      	lea %pc@(0x1b0c),%a0                   ; HwrCursor
    2a4:  2348 091c      	movel %a0,%a1@(2332)                   
    2a8:  41fa 1bb0      	lea %pc@(0x1e5a),%a0                   ; HwrLCDBaseAddr
    2ac:  2348 0930      	movel %a0,%a1@(2352)                   
    2b0:  41fa 1fde      	lea %pc@(0x2290),%a0                   ; HwrDisplayDrawBootScreen
    2b4:  2348 0934      	movel %a0,%a1@(2356)                   
    2b8:  41fa 1d82      	lea %pc@(0x203c),%a0                   ; HwrDisplaySleep
    2bc:  2348 0914      	movel %a0,%a1@(2324)                   
    2c0:  41fa 1bca      	lea %pc@(0x1e8c),%a0                   ; HwrLCDGetDepth
    2c4:  2348 0cfc      	movel %a0,%a1@(3324)                   
    2c8:  41fa 1bee      	lea %pc@(0x1eb8),%a0                   ; HwrBacklight
    2cc:  2348 0ba8      	movel %a0,%a1@(2984)                   
    2d0:  41fa 1c0e      	lea %pc@(0x1ee0),%a0                   ; HwrDisplayAttributes
    2d4:  2348 0dc8      	movel %a0,%a1@(3528)                   
    2d8:  41fa 2926      	lea %pc@(0x2c00),%a0                   ; HwrDisplayDoze
    2dc:  2348 0dcc      	movel %a0,%a1@(3532)                   
    2e0:  41fa 2494      	lea %pc@(0x2776),%a0                   ; HwrDisplayPalette
    2e4:  2348 0dd0      	movel %a0,%a1@(3536)                   
    2e8:  41fa 3550      	lea %pc@(0x383a),%a0                   ; HwrGetSilkscreenID
    2ec:  2348 0f28      	movel %a0,%a1@(3880)                   
    2f0:  41fa 5500      	lea %pc@(0x57f2),%a0                   ; HwrBattery
    2f4:  2348 0da0      	movel %a0,%a1@(3488)                   
    2f8:  41fa 5b7a      	lea %pc@(0x5e74),%a0                   ; HwrBatteryLevel
    2fc:  2348 0920      	movel %a0,%a1@(2336)                   
    300:  41fa 5b24      	lea %pc@(0x5e26),%a0                   ; HwrDockStatus
    304:  2348 0d34      	movel %a0,%a1@(3380)                   
    308:  41fa 5bde      	lea %pc@(0x5ee8),%a0                   ; HwrPluggedIn
    30c:  2348 0960      	movel %a0,%a1@(2400)                   
    310:  41fa 5bfe      	lea %pc@(0x5f10),%a0                   ; HwrDockSignals
    314:  2348 095c      	movel %a0,%a1@(2396)                   
    318:  41fa 7c08      	lea %pc@(0x7f22),%a0                   ; ??? 0x7f22
    31c:  2348 0d00      	movel %a0,%a1@(3328)                   
    320:  41fa 71c8      	lea %pc@(0x74ea),%a0                   ; TimInit
    324:  2348 03e0      	movel %a0,%a1@(992)                    
    328:  41fa 7598      	lea %pc@(0x78c2),%a0                   ; TimSleep
    32c:  2348 03fc      	movel %a0,%a1@(1020)                   
    330:  41fa 75ec      	lea %pc@(0x791e),%a0                   ; TimWake
    334:  2348 0400      	movel %a0,%a1@(1024)                   
    338:  41fa 72c0      	lea %pc@(0x75fa),%a0                   ; TimGetSeconds
    33c:  2348 03d4      	movel %a0,%a1@(980)                    
    340:  41fa 7322      	lea %pc@(0x7664),%a0                   ; TimSetSeconds
    344:  2348 03d8      	movel %a0,%a1@(984)                    
    348:  41fa 73e8      	lea %pc@(0x7732),%a0                   ; TimGetTicks
    34c:  2348 03dc      	movel %a0,%a1@(988)                    
    350:  41fa 724c      	lea %pc@(0x759e),%a0                   ; TimSetAlarm
    354:  2348 03e4      	movel %a0,%a1@(996)                    
    358:  41fa 726e      	lea %pc@(0x75c8),%a0                   ; TimGetAlarm
    35c:  2348 03e8      	movel %a0,%a1@(1000)                   
    360:  41fa 73e8      	lea %pc@(0x774a),%a0                   ; TimHandleInterrupt
    364:  2348 03ec      	movel %a0,%a1@(1004)                   
    368:  41fa 28e8      	lea %pc@(0x2c52),%a0                   ; PenOpen
    36c:  2348 09b8      	movel %a0,%a1@(2488)                   
    370:  41fa 2a32      	lea %pc@(0x2da4),%a0                   ; PenClose
    374:  2348 09bc      	movel %a0,%a1@(2492)                   
    378:  41fa 2a64      	lea %pc@(0x2dde),%a0                   ; PenSleep
    37c:  2348 09d4      	movel %a0,%a1@(2516)                   
    380:  41fa 2a98      	lea %pc@(0x2e1a),%a0                   ; PenWake
    384:  2348 09d8      	movel %a0,%a1@(2520)                   
    388:  41fa 2c0c      	lea %pc@(0x2f96),%a0                   ; PenGetRawPen
    38c:  2348 09c0      	movel %a0,%a1@(2496)                   
    390:  41fa 2ec0      	lea %pc@(0x3252),%a0                   ; PenResetCalibration
    394:  2348 09d0      	movel %a0,%a1@(2512)                   
    398:  41fa 2ef2      	lea %pc@(0x328c),%a0                   ; PenCalibrate
    39c:  2348 09c4      	movel %a0,%a1@(2500)                   
    3a0:  41fa 3118      	lea %pc@(0x34ba),%a0                   ; PenRawToScreen
    3a4:  2348 09c8      	movel %a0,%a1@(2504)                   
    3a8:  41fa 31aa      	lea %pc@(0x3554),%a0                   ; PenScreenToRaw
    3ac:  2348 09cc      	movel %a0,%a1@(2508)                   
    3b0:  41fa 655c      	lea %pc@(0x690e),%a0                   ; KeyInit
    3b4:  2348 0a80      	movel %a0,%a1@(2688)                   
    3b8:  41fa 69dc      	lea %pc@(0x6d96),%a0                   ; KeyHandleInterrupt
    3bc:  2348 0a84      	movel %a0,%a1@(2692)                   
    3c0:  41fa 6ef0      	lea %pc@(0x72b2),%a0                   ; KeyRates
    3c4:  2348 0a90      	movel %a0,%a1@(2704)                   
    3c8:  41fa 6f5a      	lea %pc@(0x7324),%a0                   ; KeyResetDoubleTap
    3cc:  2348 0a8c      	movel %a0,%a1@(2700)                   
    3d0:  41fa 686e      	lea %pc@(0x6c40),%a0                   ; KeyCurrentState
    3d4:  2348 0a88      	movel %a0,%a1@(2696)                   
    3d8:  41fa 6888      	lea %pc@(0x6c62),%a0                   ; KeySetMask
    3dc:  2348 0b3c      	movel %a0,%a1@(2876)                   
    3e0:  41fa 6f68      	lea %pc@(0x734a),%a0                   ; KeySleep
    3e4:  2348 0a94      	movel %a0,%a1@(2708)                   
    3e8:  41fa 7040      	lea %pc@(0x742a),%a0                   ; KeyWake
    3ec:  2348 0a98      	movel %a0,%a1@(2712)                   
    3f0:  41fa 7820      	lea %pc@(0x7c12),%a0                   ; HwrSoundOn
    3f4:  2348 0f08      	movel %a0,%a1@(3848)                   
    3f8:  41fa 79ca      	lea %pc@(0x7dc4),%a0                   ; HwrSoundOff
    3fc:  2348 0f0c      	movel %a0,%a1@(3852)                   
    400:  41fa 5b42      	lea %pc@(0x5f44),%a0                   ; HwrInterruptsInit
    404:  2348 0f04      	movel %a0,%a1@(3844)                   
    408:  41fa 79ec      	lea %pc@(0x7df6),%a0                   ; FlashInit
    40c:  2348 0d80      	movel %a0,%a1@(3456)                   
    410:  41fa 79fa      	lea %pc@(0x7e0c),%a0                   ; FlashCompress
    414:  2348 0d84      	movel %a0,%a1@(3460)                   
    418:  41fa 7a0c      	lea %pc@(0x7e26),%a0                   ; FlashErase
    41c:  2348 0d88      	movel %a0,%a1@(3464)                   
    420:  41fa 7a1c      	lea %pc@(0x7e3e),%a0                   ; FlashProgram
    424:  2348 0d8c      	movel %a0,%a1@(3468)                   
    428:  41fa 7a2e      	lea %pc@(0x7e58),%a0                   ; FlashParmsWrite
    42c:  2348 0d78      	movel %a0,%a1@(3448)                   
    430:  41fa 7a42      	lea %pc@(0x7e74),%a0                   ; FlashParmsRead
    434:  2348 0d7c      	movel %a0,%a1@(3452)                   
    438:  4e5e           	unlk %fp                               
    43a:  4e75           	rts                                    
```

# HwrInterruptsInstall: 0x0446 - 0x0480

```
    446:  4e56 0000      	linkw %fp,#0                           
    44a:  2278 0122      	moveal 0x122,%a1                       
    44e:  41fa 5b66      	lea %pc@(0x5fb6),%a0                   ; HwrIRQ1Handler
    452:  2348 0944      	movel %a0,%a1@(2372)                   
    456:  41fa 5bae      	lea %pc@(0x6006),%a0                   ; HwrIRQ2Handler
    45a:  2348 0948      	movel %a0,%a1@(2376)                   
    45e:  41fa 5bc6      	lea %pc@(0x6026),%a0                   ; HwrIRQ3Handler
    462:  2348 094c      	movel %a0,%a1@(2380)                   
    466:  41fa 5bde      	lea %pc@(0x6046),%a0                   ; ??? 0x6046
    46a:  2348 0950      	movel %a0,%a1@(2384)                   
    46e:  41fa 5c78      	lea %pc@(0x60e8),%a0                   ; HwrIRQ5Handler
    472:  2348 0954      	movel %a0,%a1@(2388)                   
    476:  41fa 5cc0      	lea %pc@(0x6138),%a0                   ; ??? 0x6138
    47a:  2348 0958      	movel %a0,%a1@(2392)                   
    47e:  4e5e           	unlk %fp                               
    480:  4e75           	rts                                    
```

# HwrTimerInit: 0x049a - 0x0500

Parameters:

   * `%fp@(8)`      : ???

```
    49a:  4e56 0000      	linkw %fp,#0                           
    49e:  48e7 1e00      	moveml %d3-%d6,%sp@-                   
    4a2:  0c6e 0001 0008 	cmpiw #1,%fp@(8)                       
    4a8:  664c           	bnes 0x4f6                             ; ↓ label_2
    4aa:  7601           	moveq #1,%d3                           
    4ac:  7032           	moveq #50,%d0                          
    4ae:  d0b8 012e      	addl 0x12e,%d0                         
    4b2:  7264           	moveq #100,%d1                         
    4b4:  4eba fbfa      	jsr %pc@(0xb0)                         ; BBHBf6
    4b8:  2c00           	movel %d0,%d6                          
    4ba:  2a00           	movel %d0,%d5                          
    4bc:  2806           	movel %d6,%d4                          
    4be:  0c86 0000 ffff 	cmpil #65535,%d6                       
    4c4:  6314           	blss 0x4da                             ; ↓ label_1

  label_0:
    4c6:  5283           	addql #1,%d3                           
    4c8:  2005           	movel %d5,%d0                          
    4ca:  2203           	movel %d3,%d1                          
    4cc:  4eba fbe2      	jsr %pc@(0xb0)                         ; BBHBf6
    4d0:  2800           	movel %d0,%d4                          
    4d2:  0c84 0000 ffff 	cmpil #65535,%d4                       
    4d8:  62ec           	bhis 0x4c6                             ; ↑ label_0

  label_1:
    4da:  2003           	movel %d3,%d0                          
    4dc:  5380           	subql #1,%d0                           
    4de:  31c0 f602      	movew %d0,0xfffffffffffff602           
    4e2:  31c4 f604      	movew %d4,0xfffffffffffff604           
    4e6:  31fc 0033 f600 	movew #51,0xfffffffffffff600           
    4ec:  0278 fffd f306 	andiw #-3,0xfffffffffffff306           
    4f2:  7000           	moveq #0,%d0                           
    4f4:  6004           	bras 0x4fa                             ; ↓ label_3

  label_2:
    4f6:  303c ffff      	movew #-1,%d0                          

  label_3:
    4fa:  4cdf 0078      	moveml %sp@+,%d3-%d6                   
    4fe:  4e5e           	unlk %fp                               
    500:  4e75           	rts                                    
```

# HwrTimerSleep: 0x0512 - 0x053e

Parameters:

   * `%fp@(8)`      : ???

```
    512:  4e56 0000      	linkw %fp,#0                           
    516:  0c6e 0001 0008 	cmpiw #1,%fp@(8)                       
    51c:  6616           	bnes 0x534                             ; ↓ label_0
    51e:  0078 0002 f306 	oriw #2,0xfffffffffffff306             
    524:  0278 fffe f600 	andiw #-2,0xfffffffffffff600           
    52a:  3038 f60a      	movew 0xfffffffffffff60a,%d0           
    52e:  4278 f60a      	clrw 0xfffffffffffff60a                
    532:  6006           	bras 0x53a                             ; ↓ label_1

  label_0:
    534:  303c ffff      	movew #-1,%d0                          
    538:  6002           	bras 0x53c                             ; ↓ label_2

  label_1:
    53a:  7000           	moveq #0,%d0                           

  label_2:
    53c:  4e5e           	unlk %fp                               
    53e:  4e75           	rts                                    
```

# HwrTimerWake: 0x0550 - 0x0574

Parameters:

   * `%fp@(8)`      : ???

```
    550:  4e56 0000      	linkw %fp,#0                           
    554:  0c6e 0001 0008 	cmpiw #1,%fp@(8)                       
    55a:  660e           	bnes 0x56a                             ; ↓ label_0
    55c:  0078 0001 f600 	oriw #1,0xfffffffffffff600             
    562:  0278 fffd f306 	andiw #-3,0xfffffffffffff306           
    568:  6006           	bras 0x570                             ; ↓ label_1

  label_0:
    56a:  303c ffff      	movew #-1,%d0                          
    56e:  6002           	bras 0x572                             ; ↓ label_2

  label_1:
    570:  7000           	moveq #0,%d0                           

  label_2:
    572:  4e5e           	unlk %fp                               
    574:  4e75           	rts                                    
```

# HwrDelay: 0x0586 - 0x05d4

Parameters:

   * `%fp@(8)`      : ???

```
    586:  4e56 0000      	linkw %fp,#0                           
    58a:  2f05           	movel %d5,%sp@-                        
    58c:  222e 0008      	movel %fp@(8),%d1                      
    590:  207c ffff f000 	moveal #-4096,%a0                      
    596:  700f           	moveq #15,%d0                          
    598:  b280           	cmpl %d0,%d1                           
    59a:  6534           	bcss 0x5d0                             ; ↓ label_1
    59c:  2001           	movel %d1,%d0                          
    59e:  e888           	lsrl #4,%d0                            
    5a0:  d081           	addl %d1,%d0                           
    5a2:  2401           	movel %d1,%d2                          
    5a4:  e08a           	lsrl #8,%d2                            
    5a6:  d480           	addl %d0,%d2                           
    5a8:  e88a           	lsrl #4,%d2                            
    5aa:  2a02           	movel %d2,%d5                          
    5ac:  3238 f202      	movew 0xfffffffffffff202,%d1           
    5b0:  0241 8000      	andiw #-32768,%d1                      
    5b4:  4a85           	tstl %d5                               
    5b6:  6718           	beqs 0x5d0                             ; ↓ label_1

  label_0:
    5b8:  3028 0202      	movew %a0@(514),%d0                    
    5bc:  0240 8000      	andiw #-32768,%d0                      
    5c0:  b240           	cmpw %d0,%d1                           
    5c2:  67f4           	beqs 0x5b8                             ; ↑ label_0
    5c4:  3228 0202      	movew %a0@(514),%d1                    
    5c8:  0241 8000      	andiw #-32768,%d1                      
    5cc:  5382           	subql #1,%d2                           
    5ce:  66e8           	bnes 0x5b8                             ; ↑ label_0

  label_1:
    5d0:  2a1f           	movel %sp@+,%d5                        
    5d2:  4e5e           	unlk %fp                               
    5d4:  4e75           	rts                                    
```

# HwrSonyModelInit: 0x05e2 - 0x05ec

```
    5e2:  4e56 0000      	linkw %fp,#0                           
    5e6:  4eba 001a      	jsr %pc@(0x602)                        ; SonyHalMain
    5ea:  4e5e           	unlk %fp                               
    5ec:  4e75           	rts                                    
```

# SonyHalMain: 0x0602 - 0x07b6

Locals:

   * `%fp@(-30)`    : ???

```
    602:  4e56 ffe2      	linkw %fp,#-30                         
    606:  2f0a           	movel %a2,%sp@-                        
    608:  486e ffe2      	pea %fp@(-30)                          
    60c:  4227           	clrb %sp@-                             
    60e:  4e4f           	trap #15                               
    610:  a03f           	0120077                                ; sysTrapMemNVParams
    612:  4a40           	tstw %d0                               
    614:  5c4f           	addqw #6,%sp                           
    616:  661e           	bnes 0x636                             ; ↓ label_0
    618:  45ee ffe2      	lea %fp@(-30),%a2                      
    61c:  508a           	addql #8,%a2                           
    61e:  528a           	addql #1,%a2                           
    620:  4a12           	tstb %a2@                              
    622:  6612           	bnes 0x636                             ; ↓ label_0
    624:  14bc 00ff      	moveb #-1,%a2@                         
    628:  486e ffe2      	pea %fp@(-30)                          
    62c:  1f3c 0001      	moveb #1,%sp@-                         
    630:  4e4f           	trap #15                               
    632:  a03f           	0120077                                ; sysTrapMemNVParams
    634:  5c4f           	addqw #6,%sp                           

  label_0:
    636:  4227           	clrb %sp@-                             
    638:  1f3c 0001      	moveb #1,%sp@-                         
    63c:  4eba 187a      	jsr %pc@(0x1eb8)                       ; HwrBacklight
    640:  1f3c 0001      	moveb #1,%sp@-                         
    644:  1f3c 0001      	moveb #1,%sp@-                         
    648:  4eba 186e      	jsr %pc@(0x1eb8)                       ; HwrBacklight
    64c:  42b8 0300      	clrl 0x300                             
    650:  4878 0078      	pea 0x78                               
    654:  4e4f           	trap #15                               
    656:  a013           	0120023                                ; sysTrapMemPtrNew
    658:  21c8 0304      	movel %a0,0x304                        
    65c:  2008           	movel %a0,%d0                          
    65e:  4fef 000c      	lea %sp@(12),%sp                       
    662:  6700 00be      	beqw 0x722                             ; ↓ label_2
    666:  4267           	clrw %sp@-                             
    668:  2f38 0304      	movel 0x304,%sp@-                      
    66c:  4e4f           	trap #15                               
    66e:  a01b           	0120033                                ; sysTrapMemPtrSetOwner
    670:  4227           	clrb %sp@-                             
    672:  4878 0078      	pea 0x78                               
    676:  2f38 0304      	movel 0x304,%sp@-                      
    67a:  4e4f           	trap #15                               
    67c:  a027           	0120047                                ; sysTrapMemSet
    67e:  2078 0304      	moveal 0x304,%a0                       
    682:  20bc 5368 4776 	movel #1399342966,%a0@                 
    688:  2078 0304      	moveal 0x304,%a0                       
    68c:  317c 7032 0004 	movew #28722,%a0@(4)                   
    692:  2078 0304      	moveal 0x304,%a0                       
    696:  317c 0001 0006 	movew #1,%a0@(6)                       
    69c:  4878 0012      	pea 0x12                               
    6a0:  4e4f           	trap #15                               
    6a2:  a013           	0120023                                ; sysTrapMemPtrNew
    6a4:  2448           	moveal %a0,%a2                         
    6a6:  200a           	movel %a2,%d0                          
    6a8:  4fef 0014      	lea %sp@(20),%sp                       
    6ac:  675c           	beqs 0x70a                             ; ↓ label_1
    6ae:  4267           	clrw %sp@-                             
    6b0:  2f0a           	movel %a2,%sp@-                        
    6b2:  4e4f           	trap #15                               
    6b4:  a01b           	0120033                                ; sysTrapMemPtrSetOwner
    6b6:  4227           	clrb %sp@-                             
    6b8:  4878 0012      	pea 0x12                               
    6bc:  2f0a           	movel %a2,%sp@-                        
    6be:  4e4f           	trap #15                               
    6c0:  a027           	0120047                                ; sysTrapMemSet
    6c2:  34bc 0001      	movew #1,%a2@                          
    6c6:  357c 0002 0002 	movew #2,%a2@(2)                       
    6cc:  357c 0002 0004 	movew #2,%a2@(4)                       
    6d2:  357c 0001 0006 	movew #1,%a2@(6)                       
    6d8:  357c 0001 0008 	movew #1,%a2@(8)                       
    6de:  357c 0001 000a 	movew #1,%a2@(10)                      
    6e4:  357c 0001 000c 	movew #1,%a2@(12)                      
    6ea:  357c 0001 000e 	movew #1,%a2@(14)                      
    6f0:  357c 0001 0010 	movew #1,%a2@(16)                      
    6f6:  2f0a           	movel %a2,%sp@-                        
    6f8:  3f3c 4e20      	movew #20000,%sp@-                     
    6fc:  2f3c 5368 416c 	movel #1399341420,%sp@-                
    702:  4e4f           	trap #15                               
    704:  a27c           	0121174                                ; sysTrapFtrSet
    706:  4fef 001a      	lea %sp@(26),%sp                       

  label_1:
    70a:  487a 4246      	pea %pc@(0x4952)                       ; MQEnableDisplay
    70e:  3f3c 4e21      	movew #20001,%sp@-                     
    712:  2f3c 5368 416c 	movel #1399341420,%sp@-                
    718:  4e4f           	trap #15                               
    71a:  a27c           	0121174                                ; sysTrapFtrSet
    71c:  4fef 000a      	lea %sp@(10),%sp                       
    720:  6004           	bras 0x726                             ; ↓ label_3

  label_2:
    722:  6000 008e      	braw 0x7b2                             ; ↓ label_4

  label_3:
    726:  4eba 009e      	jsr %pc@(0x7c6)                        ; Hwr1859SysInitialize
    72a:  3f3c 0001      	movew #1,%sp@-                         
    72e:  2f3c 7073 7973 	movel #1886615923,%sp@-                
    734:  2f3c 7369 6c6b 	movel #1936288875,%sp@-                
    73a:  4e4f           	trap #15                               
    73c:  a075           	0120165                                ; sysTrapDmOpenDatabaseByTypeCreator
    73e:  3f3c 0005      	movew #5,%sp@-                         
    742:  2f3c 7073 7973 	movel #1886615923,%sp@-                
    748:  2f3c 7374 6b73 	movel #1937009523,%sp@-                
    74e:  4e4f           	trap #15                               
    750:  a075           	0120165                                ; sysTrapDmOpenDatabaseByTypeCreator
    752:  3f3c 0005      	movew #5,%sp@-                         
    756:  2f3c 556e 6963 	movel #1433299299,%sp@-                
    75c:  2f3c 556e 6963 	movel #1433299299,%sp@-                
    762:  4e4f           	trap #15                               
    764:  a075           	0120165                                ; sysTrapDmOpenDatabaseByTypeCreator
    766:  1038 0264      	moveb 0x264,%d0                        
    76a:  0240 0001      	andiw #1,%d0                           
    76e:  4fef 001e      	lea %sp@(30),%sp                       
    772:  673e           	beqs 0x7b2                             ; ↓ label_4
    774:  4878 0001      	pea 0x1                                
    778:  1f3c 0025      	moveb #37,%sp@-                        
    77c:  4e4f           	trap #15                               
    77e:  a2d2           	0121322                                ; sysTrapPrefSetPreference
    780:  486e ffe2      	pea %fp@(-30)                          
    784:  4227           	clrb %sp@-                             
    786:  4e4f           	trap #15                               
    788:  a03f           	0120077                                ; sysTrapMemNVParams
    78a:  4a40           	tstw %d0                               
    78c:  4fef 000c      	lea %sp@(12),%sp                       
    790:  6620           	bnes 0x7b2                             ; ↓ label_4
    792:  0cae 000c d740 	cmpil #841536,%fp@(-30)                
    798:  ffe2                                                   
    79a:  6616           	bnes 0x7b2                             ; ↓ label_4
    79c:  2d7c 000c f990 	movel #850320,%fp@(-30)                
    7a2:  ffe2                                                   
    7a4:  486e ffe2      	pea %fp@(-30)                          
    7a8:  1f3c 0001      	moveb #1,%sp@-                         
    7ac:  4e4f           	trap #15                               
    7ae:  a03f           	0120077                                ; sysTrapMemNVParams
    7b0:  5c4f           	addqw #6,%sp                           

  label_4:
    7b2:  245f           	moveal %sp@+,%a2                       
    7b4:  4e5e           	unlk %fp                               
    7b6:  4e75           	rts                                    
```

# Hwr1859SysInitialize: 0x07c6 - 0x08b8

```
    7c6:  4e56 0000      	linkw %fp,#0                           
    7ca:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-               
    7ce:  7600           	moveq #0,%d3                           
    7d0:  33fc 0009 1080 	movew #9,0x10800204                    
    7d6:  0204                                                   
    7d8:  33fc 0004 1080 	movew #4,0x10800204                    
    7de:  0204                                                   
    7e0:  4878 0064      	pea 0x64                               
    7e4:  4e4f           	trap #15                               
    7e6:  a249           	0121111                                ; sysTrapHwrDelay
    7e8:  4279 1080 0204 	clrw 0x10800204                        
    7ee:  4ab8 0304      	tstl 0x304                             
    7f2:  584f           	addqw #4,%sp                           
    7f4:  6714           	beqs 0x80a                             ; ↓ label_2
    7f6:  2078 0304      	moveal 0x304,%a0                       
    7fa:  0c90 5368 4776 	cmpil #1399342966,%a0@                 
    800:  6604           	bnes 0x806                             ; ↓ label_0
    802:  7001           	moveq #1,%d0                           
    804:  6002           	bras 0x808                             ; ↓ label_1

  label_0:
    806:  7000           	moveq #0,%d0                           

  label_1:
    808:  6002           	bras 0x80c                             ; ↓ label_3

  label_2:
    80a:  7000           	moveq #0,%d0                           

  label_3:
    80c:  4a40           	tstw %d0                               
    80e:  6604           	bnes 0x814                             ; ↓ label_4
    810:  7601           	moveq #1,%d3                           
    812:  6072           	bras 0x886                             ; ↓ label_6

  label_4:
    814:  4878 009e      	pea 0x9e                               
    818:  4e4f           	trap #15                               
    81a:  a013           	0120023                                ; sysTrapMemPtrNew
    81c:  2448           	moveal %a0,%a2                         
    81e:  2008           	movel %a0,%d0                          
    820:  584f           	addqw #4,%sp                           
    822:  6760           	beqs 0x884                             ; ↓ label_5
    824:  2678 0304      	moveal 0x304,%a3                       
    828:  2078 0304      	moveal 0x304,%a0                       
    82c:  214a 0018      	movel %a2,%a0@(24)                     
    830:  4267           	clrw %sp@-                             
    832:  2f0a           	movel %a2,%sp@-                        
    834:  4e4f           	trap #15                               
    836:  a01b           	0120033                                ; sysTrapMemPtrSetOwner
    838:  4227           	clrb %sp@-                             
    83a:  4878 009e      	pea 0x9e                               
    83e:  2f0a           	movel %a2,%sp@-                        
    840:  4e4f           	trap #15                               
    842:  a027           	0120047                                ; sysTrapMemSet
    844:  24bc 3138 3539 	movel #825767225,%a2@                  
    84a:  357c 7032 0004 	movew #28722,%a2@(4)                   
    850:  357c 0001 0006 	movew #1,%a2@(6)                       
    856:  41fa 007a      	lea %pc@(0x8d2),%a0                    ; Hwr1859SysIntHandler
    85a:  2748 004c      	movel %a0,%a3@(76)                     
    85e:  256b 0054 000c 	movel %a3@(84),%a2@(12)                
    864:  41fa 0198      	lea %pc@(0x9fe),%a0                    ; Hwr1859SysSleepHandler
    868:  2748 0054      	movel %a0,%a3@(84)                     
    86c:  256b 0058 0010 	movel %a3@(88),%a2@(16)                
    872:  41fa 01ce      	lea %pc@(0xa42),%a0                    ; Hwr1859SysWakeHandler
    876:  2748 0058      	movel %a0,%a3@(88)                     
    87a:  4eba 0230      	jsr %pc@(0xaac)                        ; Hwr1859DspInitialize
    87e:  4fef 0010      	lea %sp@(16),%sp                       
    882:  6002           	bras 0x886                             ; ↓ label_6

  label_5:
    884:  7601           	moveq #1,%d3                           

  label_6:
    886:  4a03           	tstb %d3                               
    888:  670a           	beqs 0x894                             ; ↓ label_7
    88a:  33fc 0001 1080 	movew #1,0x10800200                    
    890:  0200                                                   
    892:  601e           	bras 0x8b2                             ; ↓ label_8

  label_7:
    894:  0038 0008 f41c 	orib #8,0xfffffffffffff41c             
    89a:  0038 0008 f41d 	orib #8,0xfffffffffffff41d             
    8a0:  0238 00f7 f41f 	andib #-9,0xfffffffffffff41f           
    8a6:  0278 f7ff f30e 	andiw #-2049,0xfffffffffffff30e        
    8ac:  0278 f7ff f306 	andiw #-2049,0xfffffffffffff306        

  label_8:
    8b2:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3               
    8b6:  4e5e           	unlk %fp                               
    8b8:  4e75           	rts                                    
```

# Hwr1859SysIntHandler: 0x08d2 - 0x09e4

```
    8d2:  4e56 0000      	linkw %fp,#0                           
    8d6:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-               
    8da:  4ab8 0304      	tstl 0x304                             
    8de:  670e           	beqs 0x8ee                             ; ↓ label_0
    8e0:  2078 0304      	moveal 0x304,%a0                       
    8e4:  4aa8 0018      	tstl %a0@(24)                          
    8e8:  6704           	beqs 0x8ee                             ; ↓ label_0
    8ea:  7001           	moveq #1,%d0                           
    8ec:  6002           	bras 0x8f0                             ; ↓ label_1

  label_0:
    8ee:  7000           	moveq #0,%d0                           

  label_1:
    8f0:  4a40           	tstw %d0                               
    8f2:  6700 00ea      	beqw 0x9de                             ; ↓ label_13
    8f6:  2078 0304      	moveal 0x304,%a0                       
    8fa:  2468 0018      	moveal %a0@(24),%a2                    
    8fe:  3639 1080 0220 	movew 0x10800220,%d3                   
    904:  3039 1080 0222 	movew 0x10800222,%d0                   
    90a:  0240 0001      	andiw #1,%d0                           
    90e:  670c           	beqs 0x91c                             ; ↓ label_2
    910:  3003           	movew %d3,%d0                          
    912:  0240 0001      	andiw #1,%d0                           
    916:  6704           	beqs 0x91c                             ; ↓ label_2
    918:  4eba 029a      	jsr %pc@(0xbb4)                        ; Hwr1859DspIntHandler

  label_2:
    91c:  3039 1080 0222 	movew 0x10800222,%d0                   
    922:  0240 0100      	andiw #256,%d0                         
    926:  6708           	beqs 0x930                             ; ↓ label_3
    928:  3003           	movew %d3,%d0                          
    92a:  0240 0100      	andiw #256,%d0                         
    92e:  6628           	bnes 0x958                             ; ↓ label_5

  label_3:
    930:  3039 1080 0222 	movew 0x10800222,%d0                   
    936:  0240 0040      	andiw #64,%d0                          
    93a:  6708           	beqs 0x944                             ; ↓ label_4
    93c:  3003           	movew %d3,%d0                          
    93e:  0240 0040      	andiw #64,%d0                          
    942:  6614           	bnes 0x958                             ; ↓ label_5

  label_4:
    944:  3039 1080 0222 	movew 0x10800222,%d0                   
    94a:  0240 0010      	andiw #16,%d0                          
    94e:  671c           	beqs 0x96c                             ; ↓ label_7
    950:  3003           	movew %d3,%d0                          
    952:  0240 0010      	andiw #16,%d0                          
    956:  6714           	beqs 0x96c                             ; ↓ label_7

  label_5:
    958:  266a 001c      	moveal %a2@(28),%a3                    
    95c:  200b           	movel %a3,%d0                          
    95e:  6704           	beqs 0x964                             ; ↓ label_6
    960:  4e93           	jsr %a3@                               
    962:  6008           	bras 0x96c                             ; ↓ label_7

  label_6:
    964:  0279 feaf 1080 	andiw #-337,0x10800222                 
    96a:  0222                                                   

  label_7:
    96c:  3039 1080 0222 	movew 0x10800222,%d0                   
    972:  0240 0004      	andiw #4,%d0                           
    976:  671c           	beqs 0x994                             ; ↓ label_9
    978:  3003           	movew %d3,%d0                          
    97a:  0240 0004      	andiw #4,%d0                           
    97e:  6714           	beqs 0x994                             ; ↓ label_9
    980:  266a 0014      	moveal %a2@(20),%a3                    
    984:  200b           	movel %a3,%d0                          
    986:  6704           	beqs 0x98c                             ; ↓ label_8
    988:  4e93           	jsr %a3@                               
    98a:  6008           	bras 0x994                             ; ↓ label_9

  label_8:
    98c:  0279 fffb 1080 	andiw #-5,0x10800222                   
    992:  0222                                                   

  label_9:
    994:  3039 1080 0222 	movew 0x10800222,%d0                   
    99a:  0240 0008      	andiw #8,%d0                           
    99e:  671c           	beqs 0x9bc                             ; ↓ label_11
    9a0:  3003           	movew %d3,%d0                          
    9a2:  0240 0008      	andiw #8,%d0                           
    9a6:  6714           	beqs 0x9bc                             ; ↓ label_11
    9a8:  266a 0018      	moveal %a2@(24),%a3                    
    9ac:  200b           	movel %a3,%d0                          
    9ae:  6704           	beqs 0x9b4                             ; ↓ label_10
    9b0:  4e93           	jsr %a3@                               
    9b2:  6008           	bras 0x9bc                             ; ↓ label_11

  label_10:
    9b4:  0279 fff7 1080 	andiw #-9,0x10800222                   
    9ba:  0222                                                   

  label_11:
    9bc:  3039 1080 0222 	movew 0x10800222,%d0                   
    9c2:  0240 0020      	andiw #32,%d0                          
    9c6:  6710           	beqs 0x9d8                             ; ↓ label_12
    9c8:  3003           	movew %d3,%d0                          
    9ca:  0240 0020      	andiw #32,%d0                          
    9ce:  6708           	beqs 0x9d8                             ; ↓ label_12
    9d0:  0279 ffdf 1080 	andiw #-33,0x10800222                  
    9d6:  0222                                                   

  label_12:
    9d8:  0278 f7ff f30e 	andiw #-2049,0xfffffffffffff30e        

  label_13:
    9de:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3               
    9e2:  4e5e           	unlk %fp                               
    9e4:  4e75           	rts                                    
```

# Hwr1859SysSleepHandler: 0x09fe - 0x0a26

```
    9fe:  4e56 0000      	linkw %fp,#0                           
    a02:  2f0b           	movel %a3,%sp@-                        
    a04:  2f0a           	movel %a2,%sp@-                        
    a06:  2078 0304      	moveal 0x304,%a0                       
    a0a:  2468 0018      	moveal %a0@(24),%a2                    
    a0e:  0079 0001 1080 	oriw #1,0x10800200                     
    a14:  0200                                                   
    a16:  266a 000c      	moveal %a2@(12),%a3                    
    a1a:  200b           	movel %a3,%d0                          
    a1c:  6702           	beqs 0xa20                             ; ↓ label_0
    a1e:  4e93           	jsr %a3@                               

  label_0:
    a20:  245f           	moveal %sp@+,%a2                       
    a22:  265f           	moveal %sp@+,%a3                       
    a24:  4e5e           	unlk %fp                               
    a26:  4e75           	rts                                    
```

# Hwr1859SysWakeHandler: 0x0a42 - 0x0a92

```
    a42:  4e56 0000      	linkw %fp,#0                           
    a46:  2f0b           	movel %a3,%sp@-                        
    a48:  2f0a           	movel %a2,%sp@-                        
    a4a:  2078 0304      	moveal 0x304,%a0                       
    a4e:  2468 0018      	moveal %a0@(24),%a2                    
    a52:  0079 0002 1080 	oriw #2,0x10800200                     
    a58:  0200                                                   
    a5a:  4878 0064      	pea 0x64                               
    a5e:  4e4f           	trap #15                               
    a60:  a249           	0121111                                ; sysTrapHwrDelay
    a62:  0279 fffd 1080 	andiw #-3,0x10800200                   
    a68:  0200                                                   
    a6a:  3038 f306      	movew 0xfffffffffffff306,%d0           
    a6e:  0240 0800      	andiw #2048,%d0                        
    a72:  584f           	addqw #4,%sp                           
    a74:  670c           	beqs 0xa82                             ; ↓ label_0
    a76:  0278 f7ff f30e 	andiw #-2049,0xfffffffffffff30e        
    a7c:  0278 f7ff f306 	andiw #-2049,0xfffffffffffff306        

  label_0:
    a82:  266a 0010      	moveal %a2@(16),%a3                    
    a86:  200b           	movel %a3,%d0                          
    a88:  6702           	beqs 0xa8c                             ; ↓ label_1
    a8a:  4e93           	jsr %a3@                               

  label_1:
    a8c:  245f           	moveal %sp@+,%a2                       
    a8e:  265f           	moveal %sp@+,%a3                       
    a90:  4e5e           	unlk %fp                               
    a92:  4e75           	rts                                    
```

# Hwr1859DspInitialize: 0x0aac - 0x0b9a

```
    aac:  4e56 0000      	linkw %fp,#0                           
    ab0:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-               
    ab4:  7600           	moveq #0,%d3                           
    ab6:  2078 0304      	moveal 0x304,%a0                       
    aba:  2668 0018      	moveal %a0@(24),%a3                    
    abe:  244b           	moveal %a3,%a2                         
    ac0:  426b 003e      	clrw %a3@(62)                          
    ac4:  426a 004c      	clrw %a2@(76)                          
    ac8:  700e           	moveq #14,%d0                          
    aca:  4272 084c      	clrw %a2@(0000004c,%d0:l)              
    ace:  701c           	moveq #28,%d0                          
    ad0:  4272 084c      	clrw %a2@(0000004c,%d0:l)              
    ad4:  702a           	moveq #42,%d0                          
    ad6:  4272 084c      	clrw %a2@(0000004c,%d0:l)              
    ada:  7038           	moveq #56,%d0                          
    adc:  4272 084c      	clrw %a2@(0000004c,%d0:l)              
    ae0:  41fa 0194      	lea %pc@(0xc76),%a0                    ; Hwr1859DspLoad
    ae4:  2548 0092      	movel %a0,%a2@(146)                    
    ae8:  41fa 021c      	lea %pc@(0xd06),%a0                    ; Hwr1859DspGet
    aec:  2548 0096      	movel %a0,%a2@(150)                    
    af0:  41fa 025a      	lea %pc@(0xd4c),%a0                    ; Hwr1859DspExec
    af4:  2548 009a      	movel %a0,%a2@(154)                    

  label_0:
    af8:  0c79 fc00 1080 	cmpiw #-1024,0x10800c06                
    afe:  0c06                                                   
    b00:  66f6           	bnes 0xaf8                             ; ↑ label_0
    b02:  4279 1080 0c06 	clrw 0x10800c06                        
    b08:  1f3c 0001      	moveb #1,%sp@-                         
    b0c:  4879 1080 8000 	pea 0x10808000                         
    b12:  2f3c 5368 4473 	movel #1399342195,%sp@-                
    b18:  2f3c 4473 3149 	movel #1148399945,%sp@-                
    b1e:  4eba 0472      	jsr %pc@(0xf92)                        ; PrvLoadDspProgram
    b22:  4a40           	tstw %d0                               
    b24:  4fef 000e      	lea %sp@(14),%sp                       
    b28:  6642           	bnes 0xb6c                             ; ↓ label_3
    b2a:  4279 1080 0c06 	clrw 0x10800c06                        
    b30:  33ea 0030 1080 	movew %a2@(48),0x10800c08              
    b36:  0c08                                                   
    b38:  33fc 0037 1080 	movew #55,0x10800c04                   
    b3e:  0c04                                                   

  label_1:
    b40:  4a79 1080 0c06 	tstw 0x10800c06                        
    b46:  67f8           	beqs 0xb40                             ; ↑ label_1
    b48:  3039 1080 0c06 	movew 0x10800c06,%d0                   
    b4e:  0240 03ff      	andiw #1023,%d0                        
    b52:  660e           	bnes 0xb62                             ; ↓ label_2
    b54:  4279 1080 0c06 	clrw 0x10800c06                        
    b5a:  7001           	moveq #1,%d0                           
    b5c:  81aa 002c      	orl %d0,%a2@(44)                       
    b60:  600c           	bras 0xb6e                             ; ↓ label_4

  label_2:
    b62:  4279 1080 0c06 	clrw 0x10800c06                        
    b68:  7601           	moveq #1,%d3                           
    b6a:  6002           	bras 0xb6e                             ; ↓ label_4

  label_3:
    b6c:  7601           	moveq #1,%d3                           

  label_4:
    b6e:  4a03           	tstb %d3                               
    b70:  670a           	beqs 0xb7c                             ; ↓ label_5
    b72:  33fc 0001 1080 	movew #1,0x10800200                    
    b78:  0200                                                   
    b7a:  6018           	bras 0xb94                             ; ↓ label_6

  label_5:
    b7c:  0079 0001 1080 	oriw #1,0x10800c00                     
    b82:  0c00                                                   
    b84:  0079 0001 1080 	oriw #1,0x10800222                     
    b8a:  0222                                                   
    b8c:  0079 0010 1080 	oriw #16,0x10800c00                    
    b92:  0c00                                                   

  label_6:
    b94:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3               
    b98:  4e5e           	unlk %fp                               
    b9a:  4e75           	rts                                    
```

# Hwr1859DspIntHandler: 0x0bb4 - 0x0c5c

```
    bb4:  4e56 0000      	linkw %fp,#0                           
    bb8:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-           
    bbc:  7c00           	moveq #0,%d6                           
    bbe:  2078 0304      	moveal 0x304,%a0                       
    bc2:  2468 0018      	moveal %a0@(24),%a2                    
    bc6:  2a0a           	movel %a2,%d5                          
    bc8:  3839 1080 0c06 	movew 0x10800c06,%d4                   
    bce:  0244 fc00      	andiw #-1024,%d4                       
    bd2:  720a           	moveq #10,%d1                          
    bd4:  e26c           	lsrw %d1,%d4                           
    bd6:  302a 003e      	movew %a2@(62),%d0                     
    bda:  0240 fc00      	andiw #-1024,%d0                       
    bde:  e268           	lsrw %d1,%d0                           
    be0:  b840           	cmpw %d0,%d4                           
    be2:  660a           	bnes 0xbee                             ; ↓ label_0
    be4:  2645           	moveal %d5,%a3                         
    be6:  47eb 003e      	lea %a3@(62),%a3                       
    bea:  7c01           	moveq #1,%d6                           
    bec:  6042           	bras 0xc30                             ; ↓ label_4

  label_0:
    bee:  7600           	moveq #0,%d3                           
    bf0:  0c43 0005      	cmpiw #5,%d3                           
    bf4:  6c2c           	bges 0xc22                             ; ↓ label_3
    bf6:  2445           	moveal %d5,%a2                         

  label_1:
    bf8:  302a 004c      	movew %a2@(76),%d0                     
    bfc:  0240 fc00      	andiw #-1024,%d0                       
    c00:  720a           	moveq #10,%d1                          
    c02:  e268           	lsrw %d1,%d0                           
    c04:  b840           	cmpw %d0,%d4                           
    c06:  660e           	bnes 0xc16                             ; ↓ label_2
    c08:  700e           	moveq #14,%d0                          
    c0a:  c1c3           	mulsw %d3,%d0                          
    c0c:  2645           	moveal %d5,%a3                         
    c0e:  d7c0           	addal %d0,%a3                          
    c10:  47eb 004c      	lea %a3@(76),%a3                       
    c14:  600c           	bras 0xc22                             ; ↓ label_3

  label_2:
    c16:  508a           	addql #8,%a2                           
    c18:  5c8a           	addql #6,%a2                           
    c1a:  5243           	addqw #1,%d3                           
    c1c:  0c43 0005      	cmpiw #5,%d3                           
    c20:  6dd6           	blts 0xbf8                             ; ↑ label_1

  label_3:
    c22:  0c43 0005      	cmpiw #5,%d3                           
    c26:  6d08           	blts 0xc30                             ; ↓ label_4
    c28:  4279 1080 0c06 	clrw 0x10800c06                        
    c2e:  6026           	bras 0xc56                             ; ↓ label_5

  label_4:
    c30:  1f3c 0001      	moveb #1,%sp@-                         
    c34:  2f2b 0006      	movel %a3@(6),%sp@-                    
    c38:  206b 0002      	moveal %a3@(2),%a0                     
    c3c:  4e90           	jsr %a0@                               
    c3e:  4253           	clrw %a3@                              
    c40:  4279 1080 0c06 	clrw 0x10800c06                        
    c46:  4a06           	tstb %d6                               
    c48:  5c4f           	addqw #6,%sp                           
    c4a:  670a           	beqs 0xc56                             ; ↓ label_5
    c4c:  2f2b 000a      	movel %a3@(10),%sp@-                   
    c50:  4e4f           	trap #15                               
    c52:  a2bf           	0121277                                ; sysTrapSysTaskWake
    c54:  584f           	addqw #4,%sp                           

  label_5:
    c56:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3           
    c5a:  4e5e           	unlk %fp                               
    c5c:  4e75           	rts                                    
```

# Hwr1859DspLoad: 0x0c76 - 0x0cf2

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???

```
    c76:  4e56 0000      	linkw %fp,#0                           
    c7a:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-           
    c7e:  2078 0304      	moveal 0x304,%a0                       
    c82:  2668 0018      	moveal %a0@(24),%a3                    
    c86:  244b           	moveal %a3,%a2                         
    c88:  262b 003a      	movel %a3@(58),%d3                     
    c8c:  42a7           	clrl %sp@-                             
    c8e:  42a7           	clrl %sp@-                             
    c90:  2f03           	movel %d3,%sp@-                        
    c92:  4e4f           	trap #15                               
    c94:  a097           	0120227                                ; sysTrapSysSemaphoreWait
    c96:  0279 fffe 1080 	andiw #-2,0x10800222                   
    c9c:  0222                                                   
    c9e:  4227           	clrb %sp@-                             
    ca0:  2f2e 0010      	movel %fp@(16),%sp@-                   
    ca4:  2f2e 000c      	movel %fp@(12),%sp@-                   
    ca8:  2f2e 0008      	movel %fp@(8),%sp@-                    
    cac:  4eba 02e4      	jsr %pc@(0xf92)                        ; PrvLoadDspProgram
    cb0:  3800           	movew %d0,%d4                          
    cb2:  4fef 001a      	lea %sp@(26),%sp                       
    cb6:  6614           	bnes 0xccc                             ; ↓ label_0
    cb8:  256e 0008 0032 	movel %fp@(8),%a2@(50)                 
    cbe:  256e 000c 0036 	movel %fp@(12),%a2@(54)                
    cc4:  7001           	moveq #1,%d0                           
    cc6:  81aa 002c      	orl %d0,%a2@(44)                       
    cca:  600e           	bras 0xcda                             ; ↓ label_1

  label_0:
    ccc:  42aa 0032      	clrl %a2@(50)                          
    cd0:  42aa 0036      	clrl %a2@(54)                          
    cd4:  70fe           	moveq #-2,%d0                          
    cd6:  c1aa 002c      	andl %d0,%a2@(44)                      

  label_1:
    cda:  0079 0001 1080 	oriw #1,0x10800222                     
    ce0:  0222                                                   
    ce2:  2f03           	movel %d3,%sp@-                        
    ce4:  4e4f           	trap #15                               
    ce6:  a098           	0120230                                ; sysTrapSysSemaphoreSignal
    ce8:  3004           	movew %d4,%d0                          
    cea:  584f           	addqw #4,%sp                           
    cec:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3           
    cf0:  4e5e           	unlk %fp                               
    cf2:  4e75           	rts                                    
```

# Hwr1859DspGet: 0x0d06 - 0x0d3a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    d06:  4e56 0000      	linkw %fp,#0                           
    d0a:  2f0b           	movel %a3,%sp@-                        
    d0c:  2078 0304      	moveal 0x304,%a0                       
    d10:  2668 0018      	moveal %a0@(24),%a3                    
    d14:  224b           	moveal %a3,%a1                         
    d16:  7200           	moveq #0,%d1                           
    d18:  7001           	moveq #1,%d0                           
    d1a:  c0ab 002c      	andl %a3@(44),%d0                      
    d1e:  6712           	beqs 0xd32                             ; ↓ label_0
    d20:  206e 0008      	moveal %fp@(8),%a0                     
    d24:  20a9 0032      	movel %a1@(50),%a0@                    
    d28:  206e 000c      	moveal %fp@(12),%a0                    
    d2c:  20a9 0036      	movel %a1@(54),%a0@                    
    d30:  6002           	bras 0xd34                             ; ↓ label_1

  label_0:
    d32:  72ff           	moveq #-1,%d1                          

  label_1:
    d34:  3001           	movew %d1,%d0                          
    d36:  265f           	moveal %sp@+,%a3                       
    d38:  4e5e           	unlk %fp                               
    d3a:  4e75           	rts                                    
```

# Hwr1859DspExec: 0x0d4c - 0x0f7e

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-4)`     : ???

```
    d4c:  4e56 fffc      	linkw %fp,#-4                          
    d50:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-           
    d54:  246e 0008      	moveal %fp@(8),%a2                     
    d58:  2078 0304      	moveal 0x304,%a0                       
    d5c:  2668 0018      	moveal %a0@(24),%a3                    
    d60:  280b           	movel %a3,%d4                          
    d62:  4ab8 0304      	tstl 0x304                             
    d66:  6708           	beqs 0xd70                             ; ↓ label_0
    d68:  200b           	movel %a3,%d0                          
    d6a:  6704           	beqs 0xd70                             ; ↓ label_0
    d6c:  7001           	moveq #1,%d0                           
    d6e:  6002           	bras 0xd72                             ; ↓ label_1

  label_0:
    d70:  7000           	moveq #0,%d0                           

  label_1:
    d72:  4a40           	tstw %d0                               
    d74:  670a           	beqs 0xd80                             ; ↓ label_2
    d76:  7001           	moveq #1,%d0                           
    d78:  2044           	moveal %d4,%a0                         
    d7a:  c0a8 002c      	andl %a0@(44),%d0                      
    d7e:  6606           	bnes 0xd86                             ; ↓ label_3

  label_2:
    d80:  7001           	moveq #1,%d0                           
    d82:  6000 01f4      	braw 0xf78                             ; ↓ label_21

  label_3:
    d86:  4e4f           	trap #15                               
    d88:  a0a5           	0120245                                ; sysTrapSysDisableInts
    d8a:  3a00           	movew %d0,%d5                          
    d8c:  7004           	moveq #4,%d0                           
    d8e:  2044           	moveal %d4,%a0                         
    d90:  c0a8 002c      	andl %a0@(44),%d0                      
    d94:  6622           	bnes 0xdb8                             ; ↓ label_4
    d96:  2d7c 5368 4473 	movel #1399342195,%fp@(-4)             
    d9c:  fffc                                                   
    d9e:  7004           	moveq #4,%d0                           
    da0:  81a8 002c      	orl %d0,%a0@(44)                       
    da4:  4878 0001      	pea 0x1                                
    da8:  486e fffc      	pea %fp@(-4)                           
    dac:  4868 003a      	pea %a0@(58)                           
    db0:  4e4f           	trap #15                               
    db2:  a095           	0120225                                ; sysTrapSysSemaphoreCreate
    db4:  4fef 000c      	lea %sp@(12),%sp                       

  label_4:
    db8:  3f05           	movew %d5,%sp@-                        
    dba:  4e4f           	trap #15                               
    dbc:  a0a6           	0120246                                ; sysTrapSysRestoreStatus
    dbe:  42a7           	clrl %sp@-                             
    dc0:  42a7           	clrl %sp@-                             
    dc2:  2044           	moveal %d4,%a0                         
    dc4:  2f28 003a      	movel %a0@(58),%sp@-                   
    dc8:  4e4f           	trap #15                               
    dca:  a097           	0120227                                ; sysTrapSysSemaphoreWait
    dcc:  4a2a 0024      	tstb %a2@(36)                          
    dd0:  4fef 000e      	lea %sp@(14),%sp                       
    dd4:  6644           	bnes 0xe1a                             ; ↓ label_7
    dd6:  0279 fffe 1080 	andiw #-2,0x10800222                   
    ddc:  0222                                                   
    dde:  7600           	moveq #0,%d3                           
    de0:  0c43 0005      	cmpiw #5,%d3                           
    de4:  6c14           	bges 0xdfa                             ; ↓ label_6
    de6:  2644           	moveal %d4,%a3                         

  label_5:
    de8:  4a6b 004c      	tstw %a3@(76)                          
    dec:  670c           	beqs 0xdfa                             ; ↓ label_6
    dee:  508b           	addql #8,%a3                           
    df0:  5c8b           	addql #6,%a3                           
    df2:  5243           	addqw #1,%d3                           
    df4:  0c43 0005      	cmpiw #5,%d3                           
    df8:  6dee           	blts 0xde8                             ; ↑ label_5

  label_6:
    dfa:  0079 0001 1080 	oriw #1,0x10800222                     
    e00:  0222                                                   
    e02:  0c43 0005      	cmpiw #5,%d3                           
    e06:  6d12           	blts 0xe1a                             ; ↓ label_7
    e08:  2044           	moveal %d4,%a0                         
    e0a:  2f28 003a      	movel %a0@(58),%sp@-                   
    e0e:  4e4f           	trap #15                               
    e10:  a098           	0120230                                ; sysTrapSysSemaphoreSignal
    e12:  7002           	moveq #2,%d0                           
    e14:  584f           	addqw #4,%sp                           
    e16:  6000 0160      	braw 0xf78                             ; ↓ label_21

  label_7:
    e1a:  4a6a 0012      	tstw %a2@(18)                          
    e1e:  6740           	beqs 0xe60                             ; ↓ label_9
    e20:  33ea 0014 1080 	movew %a2@(20),0x10800c08              
    e26:  0c08                                                   
    e28:  33ea 0016 1080 	movew %a2@(22),0x10800c0a              
    e2e:  0c0a                                                   
    e30:  33ea 0018 1080 	movew %a2@(24),0x10800c0c              
    e36:  0c0c                                                   
    e38:  33ea 001a 1080 	movew %a2@(26),0x10800c0e              
    e3e:  0c0e                                                   
    e40:  33ea 001c 1080 	movew %a2@(28),0x10800c10              
    e46:  0c10                                                   
    e48:  33ea 001e 1080 	movew %a2@(30),0x10800c12              
    e4e:  0c12                                                   
    e50:  33ea 0012 1080 	movew %a2@(18),0x10800c04              
    e56:  0c04                                                   

  label_8:
    e58:  4a79 1080 0c04 	tstw 0x10800c04                        
    e5e:  66f8           	bnes 0xe58                             ; ↑ label_8

  label_9:
    e60:  33ea 0002 1080 	movew %a2@(2),0x10800c08               
    e66:  0c08                                                   
    e68:  33ea 0004 1080 	movew %a2@(4),0x10800c0a               
    e6e:  0c0a                                                   
    e70:  33ea 0006 1080 	movew %a2@(6),0x10800c0c               
    e76:  0c0c                                                   
    e78:  33ea 0008 1080 	movew %a2@(8),0x10800c0e               
    e7e:  0c0e                                                   
    e80:  33ea 000a 1080 	movew %a2@(10),0x10800c10              
    e86:  0c10                                                   
    e88:  33ea 000c 1080 	movew %a2@(12),0x10800c12              
    e8e:  0c12                                                   
    e90:  0279 fffe 1080 	andiw #-2,0x10800222                   
    e96:  0222                                                   
    e98:  4aaa 0026      	tstl %a2@(38)                          
    e9c:  6732           	beqs 0xed0                             ; ↓ label_12
    e9e:  4a2a 0024      	tstb %a2@(36)                          
    ea2:  6708           	beqs 0xeac                             ; ↓ label_10
    ea4:  2044           	moveal %d4,%a0                         
    ea6:  41e8 003e      	lea %a0@(62),%a0                       
    eaa:  600c           	bras 0xeb8                             ; ↓ label_11

  label_10:
    eac:  700e           	moveq #14,%d0                          
    eae:  c1c3           	mulsw %d3,%d0                          
    eb0:  2044           	moveal %d4,%a0                         
    eb2:  d1c0           	addal %d0,%a0                          
    eb4:  41e8 004c      	lea %a0@(76),%a0                       

  label_11:
    eb8:  2648           	moveal %a0,%a3                         
    eba:  3692           	movew %a2@,%a3@                        
    ebc:  276a 0026 0002 	movel %a2@(38),%a3@(2)                 
    ec2:  276a 002a 0006 	movel %a2@(42),%a3@(6)                 
    ec8:  4e4f           	trap #15                               
    eca:  a09e           	0120236                                ; sysTrapSysTaskID
    ecc:  2740 000a      	movel %d0,%a3@(10)                     

  label_12:
    ed0:  0079 0800 1080 	oriw #2048,0x10800202                  
    ed6:  0202                                                   
    ed8:  0079 0010 1080 	oriw #16,0x10800e60                    
    ede:  0e60                                                   
    ee0:  33d2 1080 0c04 	movew %a2@,0x10800c04                  
    ee6:  0079 0001 1080 	oriw #1,0x10800222                     
    eec:  0222                                                   
    eee:  7600           	moveq #0,%d3                           
    ef0:  4aaa 0026      	tstl %a2@(38)                          
    ef4:  6706           	beqs 0xefc                             ; ↓ label_13
    ef6:  4a2a 0024      	tstb %a2@(36)                          
    efa:  6614           	bnes 0xf10                             ; ↓ label_14

  label_13:
    efc:  2078 0304      	moveal 0x304,%a0                       
    f00:  2068 0018      	moveal %a0@(24),%a0                    
    f04:  2f28 003a      	movel %a0@(58),%sp@-                   
    f08:  4e4f           	trap #15                               
    f0a:  a098           	0120230                                ; sysTrapSysSemaphoreSignal
    f0c:  584f           	addqw #4,%sp                           
    f0e:  6066           	bras 0xf76                             ; ↓ label_20

  label_14:
    f10:  3012           	movew %a2@,%d0                         
    f12:  0440 1a01      	subiw #6657,%d0                        
    f16:  6708           	beqs 0xf20                             ; ↓ label_15
    f18:  0440 5000      	subiw #20480,%d0                       
    f1c:  6702           	beqs 0xf20                             ; ↓ label_15
    f1e:  6008           	bras 0xf28                             ; ↓ label_16

  label_15:
    f20:  2a3c 0000 01f4 	movel #500,%d5                         
    f26:  6006           	bras 0xf2e                             ; ↓ label_17

  label_16:
    f28:  2a3c 0000 00c8 	movel #200,%d5                         

  label_17:
    f2e:  2f05           	movel %d5,%sp@-                        
    f30:  4e4f           	trap #15                               
    f32:  a2be           	0121276                                ; sysTrapSysTaskWait
    f34:  3c00           	movew %d0,%d6                          
    f36:  584f           	addqw #4,%sp                           
    f38:  672a           	beqs 0xf64                             ; ↓ label_19
    f3a:  0279 fffe 1080 	andiw #-2,0x10800222                   
    f40:  0222                                                   
    f42:  266a 0026      	moveal %a2@(38),%a3                    
    f46:  200b           	movel %a3,%d0                          
    f48:  670a           	beqs 0xf54                             ; ↓ label_18
    f4a:  4227           	clrb %sp@-                             
    f4c:  2f2a 002a      	movel %a2@(42),%sp@-                   
    f50:  4e93           	jsr %a3@                               
    f52:  5c4f           	addqw #6,%sp                           

  label_18:
    f54:  2044           	moveal %d4,%a0                         
    f56:  4268 003e      	clrw %a0@(62)                          
    f5a:  7603           	moveq #3,%d3                           
    f5c:  0079 0001 1080 	oriw #1,0x10800222                     
    f62:  0222                                                   

  label_19:
    f64:  2078 0304      	moveal 0x304,%a0                       
    f68:  2068 0018      	moveal %a0@(24),%a0                    
    f6c:  2f28 003a      	movel %a0@(58),%sp@-                   
    f70:  4e4f           	trap #15                               
    f72:  a098           	0120230                                ; sysTrapSysSemaphoreSignal
    f74:  584f           	addqw #4,%sp                           

  label_20:
    f76:  3003           	movew %d3,%d0                          

  label_21:
    f78:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3           
    f7c:  4e5e           	unlk %fp                               
    f7e:  4e75           	rts                                    
```

# PrvLoadDspProgram: 0x0f92 - 0x1154

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-44)`    : ???

```
    f92:  4e56 ffd4      	linkw %fp,#-44                         
    f96:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-           
    f9a:  2c2e 0010      	movel %fp@(16),%d6                     
    f9e:  182e 0014      	moveb %fp@(20),%d4                     
    fa2:  42ae fffc      	clrl %fp@(-4)                          
    fa6:  486e fffc      	pea %fp@(-4)                           
    faa:  486e fffa      	pea %fp@(-6)                           
    fae:  1f3c 0001      	moveb #1,%sp@-                         
    fb2:  2f2e 000c      	movel %fp@(12),%sp@-                   
    fb6:  2f2e 0008      	movel %fp@(8),%sp@-                    
    fba:  486e ffd4      	pea %fp@(-44)                          
    fbe:  1f3c 0001      	moveb #1,%sp@-                         
    fc2:  4e4f           	trap #15                               
    fc4:  a078           	0120170                                ; sysTrapDmGetNextDatabaseByTypeCreator
    fc6:  4aae fffc      	tstl %fp@(-4)                          
    fca:  4fef 0018      	lea %sp@(24),%sp                       
    fce:  6606           	bnes 0xfd6                             ; ↓ label_0
    fd0:  70ff           	moveq #-1,%d0                          
    fd2:  6000 017a      	braw 0x114e                            ; ↓ label_11

  label_0:
    fd6:  3f3c 0001      	movew #1,%sp@-                         
    fda:  2f2e fffc      	movel %fp@(-4),%sp@-                   
    fde:  3f2e fffa      	movew %fp@(-6),%sp@-                   
    fe2:  4e4f           	trap #15                               
    fe4:  a049           	0120111                                ; sysTrapDmOpenDatabase
    fe6:  2a08           	movel %a0,%d5                          
    fe8:  504f           	addqw #8,%sp                           
    fea:  6700 0160      	beqw 0x114c                            ; ↓ label_10
    fee:  3f3c 03e8      	movew #1000,%sp@-                      
    ff2:  2f3c 7453 4144 	movel #1951613252,%sp@-                
    ff8:  4e4f           	trap #15                               
    ffa:  a060           	0120140                                ; sysTrapDmGet1Resource
    ffc:  2648           	moveal %a0,%a3                         
    ffe:  200b           	movel %a3,%d0                          
    1000:  5c4f           	addqw #6,%sp                          
    1002:  674e           	beqs 0x1052                           ; ↓ label_3
    1004:  2f0b           	movel %a3,%sp@-                       
    1006:  4e4f           	trap #15                              
    1008:  a021           	0120041                               ; sysTrapMemHandleLock
    100a:  2448           	moveal %a0,%a2                        
    100c:  3612           	movew %a2@,%d3                        
    100e:  3d6a 0002 fff8 	movew %a2@(2),%fp@(-8)                
    1014:  3d6a 0004 fff6 	movew %a2@(4),%fp@(-10)               
    101a:  4a04           	tstb %d4                              
    101c:  584f           	addqw #4,%sp                          
    101e:  670e           	beqs 0x102e                           ; ↓ label_1
    1020:  2078 0304      	moveal 0x304,%a0                      
    1024:  2068 0018      	moveal %a0@(24),%a0                   
    1028:  316a 0006 0030 	movew %a2@(6),%a0@(48)                

  label_1:
    102e:  2f0b           	movel %a3,%sp@-                       
    1030:  4e4f           	trap #15                              
    1032:  a02d           	0120055                               ; sysTrapMemHandleSize
    1034:  7208           	moveq #8,%d1                          
    1036:  b081           	cmpl %d1,%d0                          
    1038:  584f           	addqw #4,%sp                          
    103a:  6306           	blss 0x1042                           ; ↓ label_2
    103c:  3d6a 0008 fff4 	movew %a2@(8),%fp@(-12)               

  label_2:
    1042:  2f0a           	movel %a2,%sp@-                       
    1044:  4e4f           	trap #15                              
    1046:  a035           	0120065                               ; sysTrapMemPtrUnlock
    1048:  2f0b           	movel %a3,%sp@-                       
    104a:  4e4f           	trap #15                              
    104c:  a061           	0120141                               ; sysTrapDmReleaseResource
    104e:  504f           	addqw #8,%sp                          
    1050:  6006           	bras 0x1058                           ; ↓ label_4

  label_3:
    1052:  7601           	moveq #1,%d3                          
    1054:  6000 00ea      	braw 0x1140                           ; ↓ label_9

  label_4:
    1058:  3f3c 03e8      	movew #1000,%sp@-                     
    105c:  2f3c 7450 524d 	movel #1951421005,%sp@-               
    1062:  4e4f           	trap #15                              
    1064:  a060           	0120140                               ; sysTrapDmGet1Resource
    1066:  2448           	moveal %a0,%a2                        
    1068:  200a           	movel %a2,%d0                         
    106a:  5c4f           	addqw #6,%sp                          
    106c:  6724           	beqs 0x1092                           ; ↓ label_5
    106e:  3e03           	movew %d3,%d7                         
    1070:  1f04           	moveb %d4,%sp@-                       
    1072:  2f06           	movel %d6,%sp@-                       
    1074:  3f07           	movew %d7,%sp@-                       
    1076:  4267           	clrw %sp@-                            
    1078:  2f0a           	movel %a2,%sp@-                       
    107a:  4eba 00ee      	jsr %pc@(0x116a)                      ; PrvLoad1DspProgram
    107e:  3600           	movew %d0,%d3                         
    1080:  2f0a           	movel %a2,%sp@-                       
    1082:  4e4f           	trap #15                              
    1084:  a061           	0120141                               ; sysTrapDmReleaseResource
    1086:  4a43           	tstw %d3                              
    1088:  4fef 0012      	lea %sp@(18),%sp                      
    108c:  6600 00b2      	bnew 0x1140                           ; ↓ label_9
    1090:  6006           	bras 0x1098                           ; ↓ label_6

  label_5:
    1092:  7602           	moveq #2,%d3                          
    1094:  6000 00aa      	braw 0x1140                           ; ↓ label_9

  label_6:
    1098:  3f3c 03e9      	movew #1001,%sp@-                     
    109c:  2f3c 7450 524d 	movel #1951421005,%sp@-               
    10a2:  4e4f           	trap #15                              
    10a4:  a060           	0120140                               ; sysTrapDmGet1Resource
    10a6:  2448           	moveal %a0,%a2                        
    10a8:  200a           	movel %a2,%d0                         
    10aa:  5c4f           	addqw #6,%sp                          
    10ac:  6722           	beqs 0x10d0                           ; ↓ label_7
    10ae:  3e2e fff8      	movew %fp@(-8),%d7                    
    10b2:  1f04           	moveb %d4,%sp@-                       
    10b4:  2f06           	movel %d6,%sp@-                       
    10b6:  3f07           	movew %d7,%sp@-                       
    10b8:  4267           	clrw %sp@-                            
    10ba:  2f0a           	movel %a2,%sp@-                       
    10bc:  4eba 00ac      	jsr %pc@(0x116a)                      ; PrvLoad1DspProgram
    10c0:  3600           	movew %d0,%d3                         
    10c2:  2f0a           	movel %a2,%sp@-                       
    10c4:  4e4f           	trap #15                              
    10c6:  a061           	0120141                               ; sysTrapDmReleaseResource
    10c8:  4a43           	tstw %d3                              
    10ca:  4fef 0012      	lea %sp@(18),%sp                      
    10ce:  6670           	bnes 0x1140                           ; ↓ label_9

  label_7:
    10d0:  3f3c 03e8      	movew #1000,%sp@-                     
    10d4:  2f3c 7458 524d 	movel #1951945293,%sp@-               
    10da:  4e4f           	trap #15                              
    10dc:  a060           	0120140                               ; sysTrapDmGet1Resource
    10de:  2448           	moveal %a0,%a2                        
    10e0:  3e2e fff6      	movew %fp@(-10),%d7                   
    10e4:  200a           	movel %a2,%d0                         
    10e6:  5c4f           	addqw #6,%sp                          
    10e8:  6720           	beqs 0x110a                           ; ↓ label_8
    10ea:  1f04           	moveb %d4,%sp@-                       
    10ec:  2f06           	movel %d6,%sp@-                       
    10ee:  3f07           	movew %d7,%sp@-                       
    10f0:  3f3c 0001      	movew #1,%sp@-                        
    10f4:  2f0a           	movel %a2,%sp@-                       
    10f6:  4eba 0072      	jsr %pc@(0x116a)                      ; PrvLoad1DspProgram
    10fa:  3600           	movew %d0,%d3                         
    10fc:  2f0a           	movel %a2,%sp@-                       
    10fe:  4e4f           	trap #15                              
    1100:  a061           	0120141                               ; sysTrapDmReleaseResource
    1102:  4a43           	tstw %d3                              
    1104:  4fef 0012      	lea %sp@(18),%sp                      
    1108:  6636           	bnes 0x1140                           ; ↓ label_9

  label_8:
    110a:  3f3c 03e8      	movew #1000,%sp@-                     
    110e:  2f3c 7459 524d 	movel #1952010829,%sp@-               
    1114:  4e4f           	trap #15                              
    1116:  a060           	0120140                               ; sysTrapDmGet1Resource
    1118:  2448           	moveal %a0,%a2                        
    111a:  3e2e fff4      	movew %fp@(-12),%d7                   
    111e:  200a           	movel %a2,%d0                         
    1120:  5c4f           	addqw #6,%sp                          
    1122:  671c           	beqs 0x1140                           ; ↓ label_9
    1124:  1f04           	moveb %d4,%sp@-                       
    1126:  2f06           	movel %d6,%sp@-                       
    1128:  3f07           	movew %d7,%sp@-                       
    112a:  3f3c 0001      	movew #1,%sp@-                        
    112e:  2f0a           	movel %a2,%sp@-                       
    1130:  4eba 0038      	jsr %pc@(0x116a)                      ; PrvLoad1DspProgram
    1134:  3600           	movew %d0,%d3                         
    1136:  2f0a           	movel %a2,%sp@-                       
    1138:  4e4f           	trap #15                              
    113a:  a061           	0120141                               ; sysTrapDmReleaseResource
    113c:  4fef 0012      	lea %sp@(18),%sp                      

  label_9:
    1140:  2f05           	movel %d5,%sp@-                       
    1142:  4e4f           	trap #15                              
    1144:  a04a           	0120112                               ; sysTrapDmCloseDatabase
    1146:  3003           	movew %d3,%d0                         
    1148:  584f           	addqw #4,%sp                          
    114a:  6002           	bras 0x114e                           ; ↓ label_11

  label_10:
    114c:  70fe           	moveq #-2,%d0                         

  label_11:
    114e:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    1152:  4e5e           	unlk %fp                              
    1154:  4e75           	rts                                   
```

# PrvLoad1DspProgram: 0x116a - 0x1296

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

```
    116a:  4e56 0000      	linkw %fp,#0                          
    116e:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    1172:  246e 0008      	moveal %fp@(8),%a2                    
    1176:  7c00           	moveq #0,%d6                          
    1178:  200a           	movel %a2,%d0                         
    117a:  6700 0082      	beqw 0x11fe                           ; ↓ label_4
    117e:  2f0a           	movel %a2,%sp@-                       
    1180:  4e4f           	trap #15                              
    1182:  a021           	0120041                               ; sysTrapMemHandleLock
    1184:  2e08           	movel %a0,%d7                         
    1186:  2f0a           	movel %a2,%sp@-                       
    1188:  4e4f           	trap #15                              
    118a:  a02d           	0120055                               ; sysTrapMemHandleSize
    118c:  e288           	lsrl #1,%d0                           
    118e:  3800           	movew %d0,%d4                         
    1190:  7600           	moveq #0,%d3                          
    1192:  4a44           	tstw %d4                              
    1194:  504f           	addqw #8,%sp                          
    1196:  635c           	blss 0x11f4                           ; ↓ label_3
    1198:  3a04           	movew %d4,%d5                         
    119a:  5145           	subqw #8,%d5                          
    119c:  0c44 0008      	cmpiw #8,%d4                          
    11a0:  6300 00d8      	blsw 0x127a                           ; ↓ label_11
    11a4:  2447           	moveal %d7,%a2                        
    11a6:  266e 0010      	moveal %fp@(16),%a3                   

  label_0:
    11aa:  3692           	movew %a2@,%a3@                       
    11ac:  376a 0002 0002 	movew %a2@(2),%a3@(2)                 
    11b2:  376a 0004 0004 	movew %a2@(4),%a3@(4)                 
    11b8:  376a 0006 0006 	movew %a2@(6),%a3@(6)                 
    11be:  376a 0008 0008 	movew %a2@(8),%a3@(8)                 
    11c4:  376a 000a 000a 	movew %a2@(10),%a3@(10)               
    11ca:  376a 000c 000c 	movew %a2@(12),%a3@(12)               
    11d0:  376a 000e 000e 	movew %a2@(14),%a3@(14)               
    11d6:  508a           	addql #8,%a2                          
    11d8:  508a           	addql #8,%a2                          
    11da:  508b           	addql #8,%a3                          
    11dc:  508b           	addql #8,%a3                          
    11de:  5043           	addqw #8,%d3                          
    11e0:  b645           	cmpw %d5,%d3                          
    11e2:  65c6           	bcss 0x11aa                           ; ↑ label_0
    11e4:  6000 0094      	braw 0x127a                           ; ↓ label_11

  label_1:
    11e8:  3692           	movew %a2@,%a3@                       
    11ea:  548a           	addql #2,%a2                          
    11ec:  548b           	addql #2,%a3                          
    11ee:  5243           	addqw #1,%d3                          

  label_2:
    11f0:  b644           	cmpw %d4,%d3                          
    11f2:  65f4           	bcss 0x11e8                           ; ↑ label_1

  label_3:
    11f4:  2f07           	movel %d7,%sp@-                       
    11f6:  4e4f           	trap #15                              
    11f8:  a035           	0120065                               ; sysTrapMemPtrUnlock
    11fa:  584f           	addqw #4,%sp                          
    11fc:  6002           	bras 0x1200                           ; ↓ label_5

  label_4:
    11fe:  7800           	moveq #0,%d4                          

  label_5:
    1200:  4a2e 0014      	tstb %fp@(20)                         
    1204:  6704           	beqs 0x120a                           ; ↓ label_6
    1206:  7036           	moveq #54,%d0                         
    1208:  6004           	bras 0x120e                           ; ↓ label_7

  label_6:
    120a:  303c 5c85      	movew #23685,%d0                      

  label_7:
    120e:  3600           	movew %d0,%d3                         
    1210:  33ee 000c 1080 	movew %fp@(12),0x10800c08             
    1216:  0c08                                                  
    1218:  33c4 1080 0c0a 	movew %d4,0x10800c0a                  
    121e:  202e 0010      	movel %fp@(16),%d0                    
    1222:  0680 ef80 0000 	addil #-276824064,%d0                 
    1228:  e288           	lsrl #1,%d0                           
    122a:  33c0 1080 0c0c 	movew %d0,0x10800c0c                  
    1230:  33ee 000e 1080 	movew %fp@(14),0x10800c0e             
    1236:  0c0e                                                  
    1238:  33c3 1080 0c04 	movew %d3,0x10800c04                  

  label_8:
    123e:  4a79 1080 0c06 	tstw 0x10800c06                       
    1244:  67f8           	beqs 0x123e                           ; ↑ label_8
    1246:  4a2e 0014      	tstb %fp@(20)                         
    124a:  6716           	beqs 0x1262                           ; ↓ label_9
    124c:  3039 1080 0c06 	movew 0x10800c06,%d0                  
    1252:  0240 fc00      	andiw #-1024,%d0                      
    1256:  720a           	moveq #10,%d1                         
    1258:  e268           	lsrw %d1,%d0                          
    125a:  0c40 0036      	cmpiw #54,%d0                         
    125e:  6702           	beqs 0x1262                           ; ↓ label_9
    1260:  7cf6           	moveq #-10,%d6                        

  label_9:
    1262:  3039 1080 0c06 	movew 0x10800c06,%d0                  
    1268:  0240 03ff      	andiw #1023,%d0                       
    126c:  6702           	beqs 0x1270                           ; ↓ label_10
    126e:  7cf6           	moveq #-10,%d6                        

  label_10:
    1270:  4279 1080 0c06 	clrw 0x10800c06                       
    1276:  3006           	movew %d6,%d0                         
    1278:  6016           	bras 0x1290                           ; ↓ label_12

  label_11:
    127a:  7000           	moveq #0,%d0                          
    127c:  3003           	movew %d3,%d0                         
    127e:  d080           	addl %d0,%d0                          
    1280:  2a00           	movel %d0,%d5                         
    1282:  2447           	moveal %d7,%a2                        
    1284:  d5c0           	addal %d0,%a2                         
    1286:  266e 0010      	moveal %fp@(16),%a3                   
    128a:  d7c5           	addal %d5,%a3                         
    128c:  6000 ff62      	braw 0x11f0                           ; ↑ label_2

  label_12:
    1290:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    1294:  4e5e           	unlk %fp                              
    1296:  4e75           	rts                                   
```

# HwrPreDebugInit: 0x1584 - 0x1634

Parameters:

   * `%fp@(8)`      : ???

```
    1584:  4e56 0000      	linkw %fp,#0                          
    1588:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    158c:  11fc 0018 f300 	moveb #24,0xfffffffffffff300          
    1592:  21fc 736f 6e79 	movel #1936682617,0x32a               
    1598:  032a                                                  
    159a:  21fc 7973 6d74 	movel #2037607796,0x332               
    15a0:  0332                                                  
    15a2:  21fc 7973 6d74 	movel #2037607796,0x32e               
    15a8:  032e                                                  
    15aa:  21fc 0000 033c 	movel #828,0x102                      
    15b0:  0102                                                  
    15b2:  11fc 0001 0100 	moveb #1,0x100                        
    15b8:  2478 0102      	moveal 0x102,%a2                      
    15bc:  4292           	clrl %a2@                             
    15be:  257c 2000 0000 	movel #536870912,%a2@(4)              
    15c4:  0004                                                  
    15c6:  257c 1fff ffff 	movel #536870911,%a2@(44)             
    15cc:  002c                                                  
    15ce:  256e 0008 0010 	movel %fp@(8),%a2@(16)                
    15d4:  2f3c 2000 0000 	movel #536870912,%sp@-                
    15da:  4267           	clrw %sp@-                            
    15dc:  4eba 006a      	jsr %pc@(0x1648)                      ; PrvGetRAMSize
    15e0:  2600           	movel %d0,%d3                         
    15e2:  2543 0008      	movel %d3,%a2@(8)                     
    15e6:  21c3 0258      	movel %d3,0x258                       
    15ea:  3038 f202      	movew 0xfffffffffffff202,%d0          
    15ee:  e048           	lsrw #8,%d0                           
    15f0:  0240 003f      	andiw #63,%d0                         
    15f4:  7800           	moveq #0,%d4                          
    15f6:  3800           	movew %d0,%d4                         
    15f8:  3038 f202      	movew 0xfffffffffffff202,%d0          
    15fc:  0240 00ff      	andiw #255,%d0                        
    1600:  7600           	moveq #0,%d3                          
    1602:  3600           	movew %d0,%d3                         
    1604:  2003           	movel %d3,%d0                         
    1606:  720e           	moveq #14,%d1                         
    1608:  4eba ea74      	jsr %pc@(0x7e)                        ; anonymous_function_0
    160c:  d084           	addl %d4,%d0                          
    160e:  720f           	moveq #15,%d1                         
    1610:  d081           	addl %d1,%d0                          
    1612:  d080           	addl %d0,%d0                          
    1614:  e3a8           	lsll %d1,%d0                          
    1616:  21c0 012e      	movel %d0,0x12e                       
    161a:  e288           	lsrl #1,%d0                           
    161c:  21c0 012e      	movel %d0,0x12e                       
    1620:  11fc 001f 0252 	moveb #31,0x252                       
    1626:  7001           	moveq #1,%d0                          
    1628:  21c0 0254      	movel %d0,0x254                       
    162c:  5c4f           	addqw #6,%sp                          
    162e:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    1632:  4e5e           	unlk %fp                              
    1634:  4e75           	rts                                   
```

# PrvGetRAMSize: 0x1648 - 0x183c

Locals:

   * `%fp@(-1)`     : ???
   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-7)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-11)`    : ???

```
    1648:  4e56 fff4      	linkw %fp,#-12                        
    164c:  48e7 1f00      	moveml %d3-%d7,%sp@-                  
    1650:  2d7c 1000 0000 	movel #268435456,%fp@(-6)             
    1656:  fffa                                                  
    1658:  41fa ffee      	lea %pc@(0x1648),%a0                  ; PrvGetRAMSize
    165c:  b1ee fffa      	cmpal %fp@(-6),%a0                    
    1660:  6414           	bccs 0x1676                           ; ↓ label_0
    1662:  2f3c 0080 0000 	movel #8388608,%sp@-                  
    1668:  4878 0000      	pea 0x0                               
    166c:  4eba 01e0      	jsr %pc@(0x184e)                      ; PrvGetRAMBankSize
    1670:  504f           	addqw #8,%sp                          
    1672:  6000 01c2      	braw 0x1836                           ; ↓ label_19

  label_0:
    1676:  3038 f116      	movew 0xfffffffffffff116,%d0          
    167a:  0240 fff1      	andiw #-15,%d0                        
    167e:  0040 000e      	oriw #14,%d0                          
    1682:  31c0 f116      	movew %d0,0xfffffffffffff116          
    1686:  2f3c 0080 0000 	movel #8388608,%sp@-                  
    168c:  4878 0000      	pea 0x0                               
    1690:  4eba 01bc      	jsr %pc@(0x184e)                      ; PrvGetRAMBankSize
    1694:  2600           	movel %d0,%d3                         
    1696:  0c83 0040 0000 	cmpil #4194304,%d3                    
    169c:  504f           	addqw #8,%sp                          
    169e:  6200 00f4      	bhiw 0x1794                           ; ↓ label_8
    16a2:  0c83 0040 0000 	cmpil #4194304,%d3                    
    16a8:  661c           	bnes 0x16c6                           ; ↓ label_1
    16aa:  0078 0400 f116 	oriw #1024,0xfffffffffffff116         
    16b0:  2f3c 0080 0000 	movel #8388608,%sp@-                  
    16b6:  4878 0000      	pea 0x0                               
    16ba:  4eba 0192      	jsr %pc@(0x184e)                      ; PrvGetRAMBankSize
    16be:  2600           	movel %d0,%d3                         
    16c0:  504f           	addqw #8,%sp                          
    16c2:  6000 00d0      	braw 0x1794                           ; ↓ label_8

  label_1:
    16c6:  7800           	moveq #0,%d4                          
    16c8:  2a03           	movel %d3,%d5                         
    16ca:  e28d           	lsrl #1,%d5                           
    16cc:  6706           	beqs 0x16d4                           ; ↓ label_3

  label_2:
    16ce:  5244           	addqw #1,%d4                          
    16d0:  e28d           	lsrl #1,%d5                           
    16d2:  66fa           	bnes 0x16ce                           ; ↑ label_2

  label_3:
    16d4:  7af1           	moveq #-15,%d5                        
    16d6:  da44           	addw %d4,%d5                          
    16d8:  70f1           	moveq #-15,%d0                        
    16da:  d044           	addw %d4,%d0                          
    16dc:  0c40 0001      	cmpiw #1,%d0                          
    16e0:  6c02           	bges 0x16e4                           ; ↓ label_4
    16e2:  7a00           	moveq #0,%d5                          

  label_4:
    16e4:  0c45 0007      	cmpiw #7,%d5                          
    16e8:  6f02           	bles 0x16ec                           ; ↓ label_5
    16ea:  7a07           	moveq #7,%d5                          

  label_5:
    16ec:  3005           	movew %d5,%d0                         
    16ee:  d040           	addw %d0,%d0                          
    16f0:  3d40 fff6      	movew %d0,%fp@(-10)                   
    16f4:  3238 f116      	movew 0xfffffffffffff116,%d1          
    16f8:  0241 fff1      	andiw #-15,%d1                        
    16fc:  8240           	orw %d0,%d1                           
    16fe:  31c1 f116      	movew %d1,0xfffffffffffff116          
    1702:  1d78 0000 fff9 	moveb 0x0,%fp@(-7)                    
    1708:  4238 0000      	clrb 0x0                              
    170c:  2043           	moveal %d3,%a0                        
    170e:  1810           	moveb %a0@,%d4                        
    1710:  1004           	moveb %d4,%d0                         
    1712:  4600           	notb %d0                              
    1714:  1d40 ffff      	moveb %d0,%fp@(-1)                    
    1718:  1080           	moveb %d0,%a0@                        
    171a:  1d50 fff8      	moveb %a0@,%fp@(-8)                   
    171e:  1084           	moveb %d4,%a0@                        
    1720:  102e fff8      	moveb %fp@(-8),%d0                    
    1724:  b02e ffff      	cmpb %fp@(-1),%d0                     
    1728:  661a           	bnes 0x1744                           ; ↓ label_6
    172a:  4a38 0000      	tstb 0x0                              
    172e:  6614           	bnes 0x1744                           ; ↓ label_6
    1730:  2003           	movel %d3,%d0                         
    1732:  d080           	addl %d0,%d0                          
    1734:  2f00           	movel %d0,%sp@-                       
    1736:  4878 0000      	pea 0x0                               
    173a:  4eba 0112      	jsr %pc@(0x184e)                      ; PrvGetRAMBankSize
    173e:  2600           	movel %d0,%d3                         
    1740:  504f           	addqw #8,%sp                          
    1742:  604a           	bras 0x178e                           ; ↓ label_7

  label_6:
    1744:  2803           	movel %d3,%d4                         
    1746:  e28c           	lsrl #1,%d4                           
    1748:  3005           	movew %d5,%d0                         
    174a:  5340           	subqw #1,%d0                          
    174c:  d040           	addw %d0,%d0                          
    174e:  3238 f116      	movew 0xfffffffffffff116,%d1          
    1752:  0241 fff1      	andiw #-15,%d1                        
    1756:  0041 0400      	oriw #1024,%d1                        
    175a:  8240           	orw %d0,%d1                           
    175c:  31c1 f116      	movew %d1,0xfffffffffffff116          
    1760:  2044           	moveal %d4,%a0                        
    1762:  1a10           	moveb %a0@,%d5                        
    1764:  1005           	moveb %d5,%d0                         
    1766:  4600           	notb %d0                              
    1768:  1d40 fffe      	moveb %d0,%fp@(-2)                    
    176c:  1080           	moveb %d0,%a0@                        
    176e:  1d50 fff5      	moveb %a0@,%fp@(-11)                  
    1772:  1085           	moveb %d5,%a0@                        
    1774:  102e fff5      	moveb %fp@(-11),%d0                   
    1778:  b02e fffe      	cmpb %fp@(-2),%d0                     
    177c:  6710           	beqs 0x178e                           ; ↓ label_7
    177e:  3038 f116      	movew 0xfffffffffffff116,%d0          
    1782:  0240 fbf1      	andiw #-1039,%d0                      
    1786:  806e fff6      	orw %fp@(-10),%d0                     
    178a:  31c0 f116      	movew %d0,0xfffffffffffff116          

  label_7:
    178e:  11ee fff9 0000 	moveb %fp@(-7),0x0                    

  label_8:
    1794:  0c83 0020 0000 	cmpil #2097152,%d3                    
    179a:  6408           	bccs 0x17a4                           ; ↓ label_9
    179c:  203c 0001 0000 	movel #65536,%d0                      
    17a2:  6026           	bras 0x17ca                           ; ↓ label_12

  label_9:
    17a4:  0c83 0040 0000 	cmpil #4194304,%d3                    
    17aa:  6408           	bccs 0x17b4                           ; ↓ label_10
    17ac:  203c 0002 0000 	movel #131072,%d0                     
    17b2:  6016           	bras 0x17ca                           ; ↓ label_12

  label_10:
    17b4:  0c83 0080 0000 	cmpil #8388608,%d3                    
    17ba:  6408           	bccs 0x17c4                           ; ↓ label_11
    17bc:  203c 0004 0000 	movel #262144,%d0                     
    17c2:  6006           	bras 0x17ca                           ; ↓ label_12

  label_11:
    17c4:  203c 0010 0000 	movel #1048576,%d0                    

  label_12:
    17ca:  0480 0000 8000 	subil #32768,%d0                      
    17d0:  6722           	beqs 0x17f4                           ; ↓ label_13
    17d2:  0480 0000 8000 	subil #32768,%d0                      
    17d8:  6720           	beqs 0x17fa                           ; ↓ label_14
    17da:  0480 0001 0000 	subil #65536,%d0                      
    17e0:  6720           	beqs 0x1802                           ; ↓ label_15
    17e2:  0480 0002 0000 	subil #131072,%d0                     
    17e8:  6720           	beqs 0x180a                           ; ↓ label_16
    17ea:  0480 000c 0000 	subil #786432,%d0                     
    17f0:  6720           	beqs 0x1812                           ; ↓ label_17
    17f2:  6024           	bras 0x1818                           ; ↓ label_18

  label_13:
    17f4:  7c00           	moveq #0,%d6                          
    17f6:  7e00           	moveq #0,%d7                          
    17f8:  601e           	bras 0x1818                           ; ↓ label_18

  label_14:
    17fa:  3c3c 0800      	movew #2048,%d6                       
    17fe:  7e00           	moveq #0,%d7                          
    1800:  6016           	bras 0x1818                           ; ↓ label_18

  label_15:
    1802:  3c3c 1000      	movew #4096,%d6                       
    1806:  7e00           	moveq #0,%d7                          
    1808:  600e           	bras 0x1818                           ; ↓ label_18

  label_16:
    180a:  3c3c 1800      	movew #6144,%d6                       
    180e:  7e00           	moveq #0,%d7                          
    1810:  6006           	bras 0x1818                           ; ↓ label_18

  label_17:
    1812:  7c00           	moveq #0,%d6                          
    1814:  3e3c 4010      	movew #16400,%d7                      

  label_18:
    1818:  3038 f116      	movew 0xfffffffffffff116,%d0          
    181c:  0240 e7ff      	andiw #-6145,%d0                      
    1820:  8046           	orw %d6,%d0                           
    1822:  31c0 f116      	movew %d0,0xfffffffffffff116          
    1826:  3038 f10a      	movew 0xfffffffffffff10a,%d0          
    182a:  0240 bfef      	andiw #-16401,%d0                     
    182e:  8047           	orw %d7,%d0                           
    1830:  31c0 f10a      	movew %d0,0xfffffffffffff10a          
    1834:  2003           	movel %d3,%d0                         

  label_19:
    1836:  4cdf 00f8      	moveml %sp@+,%d3-%d7                  
    183a:  4e5e           	unlk %fp                              
    183c:  4e75           	rts                                   
```

# PrvGetRAMBankSize: 0x184e - 0x18c6

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-2)`     : ???

```
    184e:  4e56 fffe      	linkw %fp,#-2                         
    1852:  48e7 0310      	moveml %d6-%d7/%a3,%sp@-              
    1856:  2c2e 0008      	movel %fp@(8),%d6                     
    185a:  2e2e 000c      	movel %fp@(12),%d7                    
    185e:  2646           	moveal %d6,%a3                        
    1860:  2046           	moveal %d6,%a0                        
    1862:  3d50 fffe      	movew %a0@,%fp@(-2)                   
    1866:  7000           	moveq #0,%d0                          

  label_0:
    1868:  2246           	moveal %d6,%a1                        
    186a:  d3c0           	addal %d0,%a1                         
    186c:  3211           	movew %a1@,%d1                        
    186e:  3401           	movew %d1,%d2                         
    1870:  4642           	notw %d2                              
    1872:  0042 0001      	oriw #1,%d2                           
    1876:  3282           	movew %d2,%a1@                        
    1878:  b451           	cmpw %a1@,%d2                         
    187a:  6640           	bnes 0x18bc                           ; ↓ label_5
    187c:  4a80           	tstl %d0                              
    187e:  660a           	bnes 0x188a                           ; ↓ label_1
    1880:  4253           	clrw %a3@                             
    1882:  203c 0001 0000 	movel #65536,%d0                      
    1888:  60de           	bras 0x1868                           ; ↑ label_0

  label_1:
    188a:  4a53           	tstw %a3@                             
    188c:  662e           	bnes 0x18bc                           ; ↓ label_5
    188e:  3281           	movew %d1,%a1@                        
    1890:  0c80 0002 0000 	cmpil #131072,%d0                     
    1896:  6408           	bccs 0x18a0                           ; ↓ label_2
    1898:  0680 0000 4000 	addil #16384,%d0                      
    189e:  6016           	bras 0x18b6                           ; ↓ label_4

  label_2:
    18a0:  0c80 0010 0000 	cmpil #1048576,%d0                    
    18a6:  6408           	bccs 0x18b0                           ; ↓ label_3
    18a8:  0680 0001 0000 	addil #65536,%d0                      
    18ae:  6006           	bras 0x18b6                           ; ↓ label_4

  label_3:
    18b0:  0680 0010 0000 	addil #1048576,%d0                    

  label_4:
    18b6:  b087           	cmpl %d7,%d0                          
    18b8:  65ae           	bcss 0x1868                           ; ↑ label_0
    18ba:  2007           	movel %d7,%d0                         

  label_5:
    18bc:  36ae fffe      	movew %fp@(-2),%a3@                   
    18c0:  4cdf 08c0      	moveml %sp@+,%d6-%d7/%a3              
    18c4:  4e5e           	unlk %fp                              
    18c6:  4e75           	rts                                   
```

# HwrResetNMI: 0x18dc - 0x18e2

```
    18dc:  4e56 0000      	linkw %fp,#0                          
    18e0:  4e5e           	unlk %fp                              
    18e2:  4e75           	rts                                   
```

# HwrResetPWM: 0x18f2 - 0x18fe

```
    18f2:  4e56 0000      	linkw %fp,#0                          
    18f6:  0278 ffef f500 	andiw #-17,0xfffffffffffff500         
    18fc:  4e5e           	unlk %fp                              
    18fe:  4e75           	rts                                   
```

# HwrMemWritable: 0x190e - 0x198c

Parameters:

   * `%fp@(8)`      : ???

```
    190e:  4e56 0000      	linkw %fp,#0                          
    1912:  2f0a           	movel %a2,%sp@-                       
    1914:  2f03           	movel %d3,%sp@-                       
    1916:  246e 0008      	moveal %fp@(8),%a2                    
    191a:  70ff           	moveq #-1,%d0                         
    191c:  b5c0           	cmpal %d0,%a2                         
    191e:  6604           	bnes 0x1924                           ; ↓ label_0
    1920:  7000           	moveq #0,%d0                          
    1922:  6062           	bras 0x1986                           ; ↓ label_8

  label_0:
    1924:  0cb8 0020 0000 	cmpil #2097152,0x258                  
    192a:  0258                                                  
    192c:  6408           	bccs 0x1936                           ; ↓ label_1
    192e:  203c 0001 0000 	movel #65536,%d0                      
    1934:  602a           	bras 0x1960                           ; ↓ label_4

  label_1:
    1936:  0cb8 0040 0000 	cmpil #4194304,0x258                  
    193c:  0258                                                  
    193e:  6408           	bccs 0x1948                           ; ↓ label_2
    1940:  203c 0002 0000 	movel #131072,%d0                     
    1946:  6018           	bras 0x1960                           ; ↓ label_4

  label_2:
    1948:  0cb8 0080 0000 	cmpil #8388608,0x258                  
    194e:  0258                                                  
    1950:  6408           	bccs 0x195a                           ; ↓ label_3
    1952:  203c 0004 0000 	movel #262144,%d0                     
    1958:  6006           	bras 0x1960                           ; ↓ label_4

  label_3:
    195a:  203c 0010 0000 	movel #1048576,%d0                    

  label_4:
    1960:  2600           	movel %d0,%d3                         
    1962:  70fe           	moveq #-2,%d0                         
    1964:  b5c0           	cmpal %d0,%a2                         
    1966:  6604           	bnes 0x196c                           ; ↓ label_5
    1968:  2003           	movel %d3,%d0                         
    196a:  601a           	bras 0x1986                           ; ↓ label_8

  label_5:
    196c:  b5c3           	cmpal %d3,%a2                         
    196e:  6406           	bccs 0x1976                           ; ↓ label_6
    1970:  2003           	movel %d3,%d0                         
    1972:  908a           	subl %a2,%d0                          
    1974:  6010           	bras 0x1986                           ; ↓ label_8

  label_6:
    1976:  b5f8 0258      	cmpal 0x258,%a2                       
    197a:  6408           	bccs 0x1984                           ; ↓ label_7
    197c:  2038 0258      	movel 0x258,%d0                       
    1980:  908a           	subl %a2,%d0                          
    1982:  6002           	bras 0x1986                           ; ↓ label_8

  label_7:
    1984:  7000           	moveq #0,%d0                          

  label_8:
    1986:  261f           	movel %sp@+,%d3                       
    1988:  245f           	moveal %sp@+,%a2                      
    198a:  4e5e           	unlk %fp                              
    198c:  4e75           	rts                                   
```

# HwrMemReadable: 0x19a0 - 0x1a38

Parameters:

   * `%fp@(8)`      : ???

```
    19a0:  4e56 0000      	linkw %fp,#0                          
    19a4:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    19a8:  246e 0008      	moveal %fp@(8),%a2                    
    19ac:  70ff           	moveq #-1,%d0                         
    19ae:  b5c0           	cmpal %d0,%a2                         
    19b0:  6608           	bnes 0x19ba                           ; ↓ label_0
    19b2:  203c 1000 0000 	movel #268435456,%d0                  
    19b8:  6078           	bras 0x1a32                           ; ↓ label_9

  label_0:
    19ba:  0cb8 0020 0000 	cmpil #2097152,0x258                  
    19c0:  0258                                                  
    19c2:  6408           	bccs 0x19cc                           ; ↓ label_1
    19c4:  203c 0001 0000 	movel #65536,%d0                      
    19ca:  602a           	bras 0x19f6                           ; ↓ label_4

  label_1:
    19cc:  0cb8 0040 0000 	cmpil #4194304,0x258                  
    19d2:  0258                                                  
    19d4:  6408           	bccs 0x19de                           ; ↓ label_2
    19d6:  203c 0002 0000 	movel #131072,%d0                     
    19dc:  6018           	bras 0x19f6                           ; ↓ label_4

  label_2:
    19de:  0cb8 0080 0000 	cmpil #8388608,0x258                  
    19e4:  0258                                                  
    19e6:  6408           	bccs 0x19f0                           ; ↓ label_3
    19e8:  203c 0004 0000 	movel #262144,%d0                     
    19ee:  6006           	bras 0x19f6                           ; ↓ label_4

  label_3:
    19f0:  203c 0010 0000 	movel #1048576,%d0                    

  label_4:
    19f6:  2600           	movel %d0,%d3                         
    19f8:  2803           	movel %d3,%d4                         
    19fa:  b5c3           	cmpal %d3,%a2                         
    19fc:  6406           	bccs 0x1a04                           ; ↓ label_5
    19fe:  2004           	movel %d4,%d0                         
    1a00:  908a           	subl %a2,%d0                          
    1a02:  602e           	bras 0x1a32                           ; ↓ label_9

  label_5:
    1a04:  b5f8 0258      	cmpal 0x258,%a2                       
    1a08:  6408           	bccs 0x1a12                           ; ↓ label_6
    1a0a:  2038 0258      	movel 0x258,%d0                       
    1a0e:  908a           	subl %a2,%d0                          
    1a10:  6020           	bras 0x1a32                           ; ↓ label_9

  label_6:
    1a12:  b5fc 1000 0000 	cmpal #268435456,%a2                  
    1a18:  6404           	bccs 0x1a1e                           ; ↓ label_7
    1a1a:  7000           	moveq #0,%d0                          
    1a1c:  6014           	bras 0x1a32                           ; ↓ label_9

  label_7:
    1a1e:  b5fc 1040 0000 	cmpal #272629760,%a2                  
    1a24:  640a           	bccs 0x1a30                           ; ↓ label_8
    1a26:  203c 1040 0000 	movel #272629760,%d0                  
    1a2c:  908a           	subl %a2,%d0                          
    1a2e:  6002           	bras 0x1a32                           ; ↓ label_9

  label_8:
    1a30:  7000           	moveq #0,%d0                          

  label_9:
    1a32:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    1a36:  4e5e           	unlk %fp                              
    1a38:  4e75           	rts                                   
```

# anonymous_function_2: 0x1a9a - 0x1ade

```
    1a9a:  207c ffff f000 	moveal #-4096,%a0                     
    1aa0:  027c 7fff      	andiw #32767,%sr                      
    1aa4:  1038 0111      	moveb 0x111,%d0                       
    1aa8:  3228 0c02      	movew %a0@(3074),%d1                  
    1aac:  0041 4010      	oriw #16400,%d1                       
    1ab0:  0041 0001      	oriw #1,%d1                           
    1ab4:  3141 0c02      	movew %d1,%a0@(3074)                  
    1ab8:  4a00           	tstb %d0                              
    1aba:  6712           	beqs 0x1ace                           ; ↓ label_1

  label_0:
    1abc:  3028 0202      	movew %a0@(514),%d0                   
    1ac0:  6afa           	bpls 0x1abc                           ; ↑ label_0
    1ac2:  0068 0008 0200 	oriw #8,%a0@(512)                     
    1ac8:  4e72 a000      	stop #-24576                          
    1acc:  6010           	bras 0x1ade                           ; ↓ label_2

  label_1:
    1ace:  3028 0202      	movew %a0@(514),%d0                   
    1ad2:  6afa           	bpls 0x1ace                           ; ↑ label_1
    1ad4:  0068 0008 0200 	oriw #8,%a0@(512)                     
    1ada:  4e72 2000      	stop #8192                            

  label_2:
    1ade:  4e75           	rts                                   
```

# prvDelay: 0x1af6 - 0x1afe

```
    1af6:  222f 0004      	movel %sp@(4),%d1                     
    1afa:  51c9 fffe      	dbf %d1,0x1afa                        
    1afe:  4e75           	rts                                   
```

# HwrCursor: 0x1b0c - 0x1b16

```
    1b0c:  4e56 0000      	linkw %fp,#0                          
    1b10:  303c 2204      	movew #8708,%d0                       
    1b14:  4e5e           	unlk %fp                              
    1b16:  4e75           	rts                                   
```

# HwrLCDContrast: 0x1b24 - 0x1b2c

```
    1b24:  4e56 0000      	linkw %fp,#0                          
    1b28:  7004           	moveq #4,%d0                          
    1b2a:  4e5e           	unlk %fp                              
    1b2c:  4e75           	rts                                   
```

# HwrDisplayInit: 0x1b40 - 0x1b9a

Parameters:

   * `%fp@(18)`     : ???

```
    1b40:  4e56 0000      	linkw %fp,#0                          
    1b44:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    1b48:  382e 0012      	movew %fp@(18),%d4                    
    1b4c:  247c 1f00 0000 	moveal #520093696,%a2                 
    1b52:  267c 1f04 0000 	moveal #520355840,%a3                 
    1b58:  7600           	moveq #0,%d3                          
    1b5a:  4eba 2fbc      	jsr %pc@(0x4b18)                      ; MQResetDisplay
    1b5e:  4eba 2b0a      	jsr %pc@(0x466a)                      ; MQGetDrvConfigPtr
    1b62:  4a44           	tstw %d4                              
    1b64:  6602           	bnes 0x1b68                           ; ↓ label_0
    1b66:  7808           	moveq #8,%d4                          

  label_0:
    1b68:  4267           	clrw %sp@-                            
    1b6a:  2f0a           	movel %a2,%sp@-                       
    1b6c:  2f0b           	movel %a3,%sp@-                       
    1b6e:  4eba 2b5a      	jsr %pc@(0x46ca)                      ; MQInit
    1b72:  3f04           	movew %d4,%sp@-                       
    1b74:  42a7           	clrl %sp@-                            
    1b76:  2f03           	movel %d3,%sp@-                       
    1b78:  4eba 2b80      	jsr %pc@(0x46fa)                      ; MQSetDisplay
    1b7c:  0278 ffef 0250 	andiw #-17,0x250                      
    1b82:  0038 0001 f429 	orib #1,0xfffffffffffff429            
    1b88:  0038 0010 f441 	orib #16,0xfffffffffffff441           
    1b8e:  7000           	moveq #0,%d0                          
    1b90:  4fef 0014      	lea %sp@(20),%sp                      
    1b94:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    1b98:  4e5e           	unlk %fp                              
    1b9a:  4e75           	rts                                   
```

# HwrDisplayWake: 0x1bae - 0x1bfc

```
    1bae:  4e56 0000      	linkw %fp,#0                          
    1bb2:  2f0a           	movel %a2,%sp@-                       
    1bb4:  4227           	clrb %sp@-                            
    1bb6:  4e4f           	trap #15                              
    1bb8:  a373           	0121563                               ; sysTrapHwrDisplayDoze
    1bba:  0038 0010 f441 	orib #16,0xfffffffffffff441           
    1bc0:  3f3c 0001      	movew #1,%sp@-                        
    1bc4:  4eba 2d8c      	jsr %pc@(0x4952)                      ; MQEnableDisplay
    1bc8:  0278 ffef 0250 	andiw #-17,0x250                      
    1bce:  0038 0001 f429 	orib #1,0xfffffffffffff429            
    1bd4:  2478 0304      	moveal 0x304,%a2                      
    1bd8:  4ab8 0304      	tstl 0x304                            
    1bdc:  584f           	addqw #4,%sp                          
    1bde:  6716           	beqs 0x1bf6                           ; ↓ label_0
    1be0:  7004           	moveq #4,%d0                          
    1be2:  c0aa 000c      	andl %a2@(12),%d0                     
    1be6:  660e           	bnes 0x1bf6                           ; ↓ label_0
    1be8:  1f3c 0001      	moveb #1,%sp@-                        
    1bec:  1f3c 0001      	moveb #1,%sp@-                        
    1bf0:  4e4f           	trap #15                              
    1bf2:  a2ea           	0121352                               ; sysTrapHwrBacklightV33
    1bf4:  584f           	addqw #4,%sp                          

  label_0:
    1bf6:  7000           	moveq #0,%d0                          
    1bf8:  245f           	moveal %sp@+,%a2                      
    1bfa:  4e5e           	unlk %fp                              
    1bfc:  4e75           	rts                                   
```

# PrvDisplayBrightness: 0x1c10 - 0x1e40

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-21)`    : ???
   * `%fp@(-30)`    : ???
   * `%fp@(-31)`    : ???

```
    1c10:  4e56 ffe0      	linkw %fp,#-32                        
    1c14:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    1c18:  266e 000a      	moveal %fp@(10),%a3                   
    1c1c:  163c 0080      	moveb #-128,%d3                       
    1c20:  247c ffff f000 	moveal #-4096,%a2                     
    1c26:  2038 02cc      	movel 0x2cc,%d0                       
    1c2a:  0280 0000 0400 	andil #1024,%d0                       
    1c30:  6608           	bnes 0x1c3a                           ; ↓ label_0
    1c32:  303c 2204      	movew #8708,%d0                       
    1c36:  6000 0202      	braw 0x1e3a                           ; ↓ label_15

  label_0:
    1c3a:  4e4f           	trap #15                              
    1c3c:  a0a5           	0120245                               ; sysTrapSysDisableInts
    1c3e:  3e00           	movew %d0,%d7                         
    1c40:  486e ffe2      	pea %fp@(-30)                         
    1c44:  4227           	clrb %sp@-                            
    1c46:  4e4f           	trap #15                              
    1c48:  a03f           	0120077                               ; sysTrapMemNVParams
    1c4a:  4a40           	tstw %d0                              
    1c4c:  5c4f           	addqw #6,%sp                          
    1c4e:  6608           	bnes 0x1c58                           ; ↓ label_1
    1c50:  182e ffeb      	moveb %fp@(-21),%d4                   
    1c54:  6702           	beqs 0x1c58                           ; ↓ label_1
    1c56:  1604           	moveb %d4,%d3                         

  label_1:
    1c58:  4a2e 0008      	tstb %fp@(8)                          
    1c5c:  6700 0190      	beqw 0x1dee                           ; ↓ label_12
    1c60:  1a13           	moveb %a3@,%d5                        
    1c62:  1c05           	moveb %d5,%d6                         
    1c64:  7000           	moveq #0,%d0                          
    1c66:  1005           	moveb %d5,%d0                         
    1c68:  3200           	movew %d0,%d1                         
    1c6a:  e441           	asrw #2,%d1                           
    1c6c:  e049           	lsrw #8,%d1                           
    1c6e:  ea49           	lsrw #5,%d1                           
    1c70:  d240           	addw %d0,%d1                          
    1c72:  e641           	asrw #3,%d1                           
    1c74:  7000           	moveq #0,%d0                          
    1c76:  1003           	moveb %d3,%d0                         
    1c78:  3800           	movew %d0,%d4                         
    1c7a:  e444           	asrw #2,%d4                           
    1c7c:  e04c           	lsrw #8,%d4                           
    1c7e:  ea4c           	lsrw #5,%d4                           
    1c80:  d840           	addw %d0,%d4                          
    1c82:  e644           	asrw #3,%d4                           
    1c84:  9801           	subb %d1,%d4                          
    1c86:  1d44 ffe1      	moveb %d4,%fp@(-31)                   
    1c8a:  4a2e ffe1      	tstb %fp@(-31)                        
    1c8e:  6700 0144      	beqw 0x1dd4                           ; ↓ label_10
    1c92:  0238 00df f411 	andib #-33,0xfffffffffffff411         
    1c98:  4a04           	tstb %d4                              
    1c9a:  6f00 0098      	blew 0x1d34                           ; ↓ label_5
    1c9e:  0238 00bf f411 	andib #-65,0xfffffffffffff411         
    1ca4:  7600           	moveq #0,%d3                          
    1ca6:  4a04           	tstb %d4                              
    1ca8:  6f00 0124      	blew 0x1dce                           ; ↓ label_9
    1cac:  1a04           	moveb %d4,%d5                         
    1cae:  5105           	subqb #8,%d5                          
    1cb0:  0c04 0008      	cmpib #8,%d4                          
    1cb4:  6f76           	bles 0x1d2c                           ; ↓ label_4

  label_2:
    1cb6:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1cbc:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1cc2:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1cc8:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1cce:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1cd4:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1cda:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1ce0:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1ce6:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1cec:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1cf2:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1cf8:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1cfe:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d04:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d0a:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d10:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d16:  5003           	addqb #8,%d3                          
    1d18:  b605           	cmpb %d5,%d3                          
    1d1a:  6d9a           	blts 0x1cb6                           ; ↑ label_2
    1d1c:  600e           	bras 0x1d2c                           ; ↓ label_4

  label_3:
    1d1e:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d24:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d2a:  5203           	addqb #1,%d3                          

  label_4:
    1d2c:  b604           	cmpb %d4,%d3                          
    1d2e:  6dee           	blts 0x1d1e                           ; ↑ label_3
    1d30:  6000 009c      	braw 0x1dce                           ; ↓ label_9

  label_5:
    1d34:  4a04           	tstb %d4                              
    1d36:  6c00 0096      	bgew 0x1dce                           ; ↓ label_9
    1d3a:  0038 0040 f411 	orib #64,0xfffffffffffff411           
    1d40:  1604           	moveb %d4,%d3                         
    1d42:  4a04           	tstb %d4                              
    1d44:  6c00 0088      	bgew 0x1dce                           ; ↓ label_9
    1d48:  7af8           	moveq #-8,%d5                         
    1d4a:  1004           	moveb %d4,%d0                         
    1d4c:  4400           	negb %d0                              
    1d4e:  0c00 0008      	cmpib #8,%d0                          
    1d52:  6f76           	bles 0x1dca                           ; ↓ label_8

  label_6:
    1d54:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d5a:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d60:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d66:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d6c:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d72:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d78:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d7e:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d84:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d8a:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d90:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1d96:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1d9c:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1da2:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1da8:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1dae:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1db4:  5003           	addqb #8,%d3                          
    1db6:  b605           	cmpb %d5,%d3                          
    1db8:  6d9a           	blts 0x1d54                           ; ↑ label_6
    1dba:  600e           	bras 0x1dca                           ; ↓ label_8

  label_7:
    1dbc:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    1dc2:  022a 007f 0411 	andib #127,%a2@(1041)                 
    1dc8:  5203           	addqb #1,%d3                          

  label_8:
    1dca:  4a03           	tstb %d3                              
    1dcc:  6dee           	blts 0x1dbc                           ; ↑ label_7

  label_9:
    1dce:  0038 0020 f411 	orib #32,0xfffffffffffff411           

  label_10:
    1dd4:  4a06           	tstb %d6                              
    1dd6:  6602           	bnes 0x1dda                           ; ↓ label_11
    1dd8:  7c01           	moveq #1,%d6                          

  label_11:
    1dda:  1d46 ffeb      	moveb %d6,%fp@(-21)                   
    1dde:  486e ffe2      	pea %fp@(-30)                         
    1de2:  1f3c 0001      	moveb #1,%sp@-                        
    1de6:  4e4f           	trap #15                              
    1de8:  a03f           	0120077                               ; sysTrapMemNVParams
    1dea:  5c4f           	addqw #6,%sp                          
    1dec:  6002           	bras 0x1df0                           ; ↓ label_13

  label_12:
    1dee:  1683           	moveb %d3,%a3@                        

  label_13:
    1df0:  3f07           	movew %d7,%sp@-                       
    1df2:  4e4f           	trap #15                              
    1df4:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    1df6:  1038 f411      	moveb 0xfffffffffffff411,%d0          
    1dfa:  0240 0010      	andiw #16,%d0                         
    1dfe:  544f           	addqw #2,%sp                          
    1e00:  6636           	bnes 0x1e38                           ; ↓ label_14
    1e02:  1f3c 0001      	moveb #1,%sp@-                        
    1e06:  1f3c 0001      	moveb #1,%sp@-                        
    1e0a:  4e4f           	trap #15                              
    1e0c:  a2ea           	0121352                               ; sysTrapHwrBacklightV33
    1e0e:  4a00           	tstb %d0                              
    1e10:  584f           	addqw #4,%sp                          
    1e12:  6624           	bnes 0x1e38                           ; ↓ label_14
    1e14:  4227           	clrb %sp@-                            
    1e16:  4227           	clrb %sp@-                            
    1e18:  4e4f           	trap #15                              
    1e1a:  a2ea           	0121352                               ; sysTrapHwrBacklightV33
    1e1c:  4a00           	tstb %d0                              
    1e1e:  584f           	addqw #4,%sp                          
    1e20:  6616           	bnes 0x1e38                           ; ↓ label_14
    1e22:  3f3c 0008      	movew #8,%sp@-                        
    1e26:  2f3c 0122 0000 	movel #19005440,%sp@-                 
    1e2c:  4e4f           	trap #15                              
    1e2e:  a12d           	0120455                               ; sysTrapEvtEnqueueKey
    1e30:  303c 2204      	movew #8708,%d0                       
    1e34:  5c4f           	addqw #6,%sp                          
    1e36:  6002           	bras 0x1e3a                           ; ↓ label_15

  label_14:
    1e38:  7000           	moveq #0,%d0                          

  label_15:
    1e3a:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    1e3e:  4e5e           	unlk %fp                              
    1e40:  4e75           	rts                                   
```

# HwrLCDBaseAddr: 0x1e5a - 0x1e78

Parameters:

   * `%fp@(8)`      : ???

```
    1e5a:  4e56 0000      	linkw %fp,#0                          
    1e5e:  2f2e 0008      	movel %fp@(8),%sp@-                   
    1e62:  206e 0008      	moveal %fp@(8),%a0                    
    1e66:  4a90           	tstl %a0@                             
    1e68:  6704           	beqs 0x1e6e                           ; ↓ label_0
    1e6a:  7001           	moveq #1,%d0                          
    1e6c:  6002           	bras 0x1e70                           ; ↓ label_1

  label_0:
    1e6e:  7000           	moveq #0,%d0                          

  label_1:
    1e70:  1f00           	moveb %d0,%sp@-                       
    1e72:  4eba 0206      	jsr %pc@(0x207a)                      ; PrvDisplayBaseAddr
    1e76:  4e5e           	unlk %fp                              
    1e78:  4e75           	rts                                   
```

# HwrLCDGetDepth: 0x1e8c - 0x1ea4

Locals:

   * `%fp@(-2)`     : ???

```
    1e8c:  4e56 fffe      	linkw %fp,#-2                         
    1e90:  486e fffe      	pea %fp@(-2)                          
    1e94:  1f3c 000e      	moveb #14,%sp@-                       
    1e98:  4227           	clrb %sp@-                            
    1e9a:  4e4f           	trap #15                              
    1e9c:  a372           	0121562                               ; sysTrapHwrDisplayAttributes
    1e9e:  302e fffe      	movew %fp@(-2),%d0                    
    1ea2:  4e5e           	unlk %fp                              
    1ea4:  4e75           	rts                                   
```

# HwrBacklight: 0x1eb8 - 0x1ece

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    1eb8:  4e56 0000      	linkw %fp,#0                          
    1ebc:  486e 000a      	pea %fp@(10)                          
    1ec0:  1f2e 0008      	moveb %fp@(8),%sp@-                   
    1ec4:  4eba 0658      	jsr %pc@(0x251e)                      ; PrvDisplayBacklight
    1ec8:  102e 000a      	moveb %fp@(10),%d0                    
    1ecc:  4e5e           	unlk %fp                              
    1ece:  4e75           	rts                                   
```

# HwrDisplayAttributes: 0x1ee0 - 0x200c

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

```
    1ee0:  4e56 0000      	linkw %fp,#0                          
    1ee4:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    1ee8:  182e 0008      	moveb %fp@(8),%d4                     
    1eec:  246e 000c      	moveal %fp@(12),%a2                   
    1ef0:  47fa 0134      	lea %pc@(0x2026),%a3                  ; ??? 0x2026
    1ef4:  7600           	moveq #0,%d3                          
    1ef6:  7000           	moveq #0,%d0                          
    1ef8:  102e 000a      	moveb %fp@(10),%d0                    
    1efc:  0c40 0015      	cmpiw #21,%d0                         
    1f00:  6200 00fe      	bhiw 0x2000                           ; ↓ label_1
    1f04:  d040           	addw %d0,%d0                          
    1f06:  303b 0006      	movew %pc@(0x1f0e,%d0:w),%d0          
    1f0a:  4efb 0002      	jmp %pc@(0x1f0e,%d0:w)                
    1f0e:  002c 0036 003e 	orib #54,%a4@(62)                     
    1f14:  0046 004e      	oriw #78,%d6                          
    1f18:  0056 005e      	oriw #94,%fp@                         
    1f1c:  0066 006e      	oriw #110,%fp@-                       
    1f20:  0076 007e 0086 	oriw #126,%fp@(ffffffffffffff86,%d0:w)
    1f26:  0092 0098 00a6 	oril #9961638,%a2@                    
    1f2c:  00b4 00c2 00c8 	oril #12714184,%a4@(ffffffffffffffe4,%d0:w)
    1f32:  00e4                                                  
    1f34:  00d6           	0326                                  
    1f36:  00d6           	0326                                  
    1f38:  00f2           	0362                                  
    1f3a:  24bc 0000 4d51 	movel #19793,%a2@                     
    1f40:  6000 00c2      	braw 0x2004                           ; ↓ label_2
    1f44:  34bc 0001      	movew #1,%a2@                         
    1f48:  6000 00ba      	braw 0x2004                           ; ↓ label_2
    1f4c:  34bc 0001      	movew #1,%a2@                         
    1f50:  6000 00b2      	braw 0x2004                           ; ↓ label_2
    1f54:  34bc 008b      	movew #139,%a2@                       
    1f58:  6000 00aa      	braw 0x2004                           ; ↓ label_2
    1f5c:  34bc 0008      	movew #8,%a2@                         
    1f60:  6000 00a2      	braw 0x2004                           ; ↓ label_2
    1f64:  34bc 0008      	movew #8,%a2@                         
    1f68:  6000 009a      	braw 0x2004                           ; ↓ label_2
    1f6c:  34bc 0010      	movew #16,%a2@                        
    1f70:  6000 0092      	braw 0x2004                           ; ↓ label_2
    1f74:  34bc 00a0      	movew #160,%a2@                       
    1f78:  6000 008a      	braw 0x2004                           ; ↓ label_2
    1f7c:  34bc 00a0      	movew #160,%a2@                       
    1f80:  6000 0082      	braw 0x2004                           ; ↓ label_2
    1f84:  24bc 1f00 0000 	movel #520093696,%a2@                 
    1f8a:  6078           	bras 0x2004                           ; ↓ label_2
    1f8c:  24bc 0004 0000 	movel #262144,%a2@                    
    1f92:  6070           	bras 0x2004                           ; ↓ label_2
    1f94:  34bc 0001      	movew #1,%a2@                         
    1f98:  606a           	bras 0x2004                           ; ↓ label_2

  label_0:
    1f9a:  204a           	moveal %a2,%a0                        
    1f9c:  528a           	addql #1,%a2                          
    1f9e:  109b           	moveb %a3@+,%a0@                      
    1fa0:  4a13           	tstb %a3@                             
    1fa2:  66f6           	bnes 0x1f9a                           ; ↑ label_0
    1fa4:  605e           	bras 0x2004                           ; ↓ label_2
    1fa6:  2f0a           	movel %a2,%sp@-                       
    1fa8:  1f04           	moveb %d4,%sp@-                       
    1faa:  4eba 00ce      	jsr %pc@(0x207a)                      ; PrvDisplayBaseAddr
    1fae:  3600           	movew %d0,%d3                         
    1fb0:  5c4f           	addqw #6,%sp                          
    1fb2:  6050           	bras 0x2004                           ; ↓ label_2
    1fb4:  2f0a           	movel %a2,%sp@-                       
    1fb6:  1f04           	moveb %d4,%sp@-                       
    1fb8:  4eba 0160      	jsr %pc@(0x211a)                      ; PrvDisplayDepth
    1fbc:  3600           	movew %d0,%d3                         
    1fbe:  5c4f           	addqw #6,%sp                          
    1fc0:  6042           	bras 0x2004                           ; ↓ label_2
    1fc2:  2f0a           	movel %a2,%sp@-                       
    1fc4:  1f04           	moveb %d4,%sp@-                       
    1fc6:  4eba 0238      	jsr %pc@(0x2200)                      ; PrvDisplaySize
    1fca:  3600           	movew %d0,%d3                         
    1fcc:  5c4f           	addqw #6,%sp                          
    1fce:  6034           	bras 0x2004                           ; ↓ label_2
    1fd0:  34bc 00a0      	movew #160,%a2@                       
    1fd4:  602e           	bras 0x2004                           ; ↓ label_2
    1fd6:  2f0a           	movel %a2,%sp@-                       
    1fd8:  1f04           	moveb %d4,%sp@-                       
    1fda:  4eba 01e6      	jsr %pc@(0x21c2)                      ; PrvDisplayRowBytes
    1fde:  3600           	movew %d0,%d3                         
    1fe0:  5c4f           	addqw #6,%sp                          
    1fe2:  6020           	bras 0x2004                           ; ↓ label_2
    1fe4:  2f0a           	movel %a2,%sp@-                       
    1fe6:  1f04           	moveb %d4,%sp@-                       
    1fe8:  4eba fc26      	jsr %pc@(0x1c10)                      ; PrvDisplayBrightness
    1fec:  3600           	movew %d0,%d3                         
    1fee:  5c4f           	addqw #6,%sp                          
    1ff0:  6012           	bras 0x2004                           ; ↓ label_2
    1ff2:  2f0a           	movel %a2,%sp@-                       
    1ff4:  1f04           	moveb %d4,%sp@-                       
    1ff6:  4eba 0526      	jsr %pc@(0x251e)                      ; PrvDisplayBacklight
    1ffa:  3600           	movew %d0,%d3                         
    1ffc:  5c4f           	addqw #6,%sp                          
    1ffe:  6004           	bras 0x2004                           ; ↓ label_2

  label_1:
    2000:  363c 2204      	movew #8708,%d3                       

  label_2:
    2004:  3003           	movew %d3,%d0                         
    2006:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    200a:  4e5e           	unlk %fp                              
    200c:  4e75           	rts                                   
```

# HwrDisplaySleep: 0x203c - 0x2066

```
    203c:  4e56 0000      	linkw %fp,#0                          
    2040:  0078 0010 0250 	oriw #16,0x250                        
    2046:  0238 00fe f429 	andib #-2,0xfffffffffffff429          
    204c:  3f3c 0080      	movew #128,%sp@-                      
    2050:  4eba 2900      	jsr %pc@(0x4952)                      ; MQEnableDisplay
    2054:  0238 00ef f441 	andib #-17,0xfffffffffffff441         
    205a:  1f3c 0001      	moveb #1,%sp@-                        
    205e:  4e4f           	trap #15                              
    2060:  a373           	0121563                               ; sysTrapHwrDisplayDoze
    2062:  7000           	moveq #0,%d0                          
    2064:  4e5e           	unlk %fp                              
    2066:  4e75           	rts                                   
```

# PrvDisplayBaseAddr: 0x207a - 0x2102

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???

```
    207a:  4e56 fff6      	linkw %fp,#-10                        
    207e:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    2082:  266e 000a      	moveal %fp@(10),%a3                   
    2086:  200b           	movel %a3,%d0                         
    2088:  6606           	bnes 0x2090                           ; ↓ label_0
    208a:  303c 2201      	movew #8705,%d0                       
    208e:  606c           	bras 0x20fc                           ; ↓ label_4

  label_0:
    2090:  486e fffc      	pea %fp@(-4)                          
    2094:  4267           	clrw %sp@-                            
    2096:  4eba 2b26      	jsr %pc@(0x4bbe)                      ; MQSetLcdStartAddr
    209a:  262e fffc      	movel %fp@(-4),%d3                    
    209e:  0683 1f00 0000 	addil #520093696,%d3                  
    20a4:  4a2e 0008      	tstb %fp@(8)                          
    20a8:  5c4f           	addqw #6,%sp                          
    20aa:  674c           	beqs 0x20f8                           ; ↓ label_3
    20ac:  486e fff6      	pea %fp@(-10)                         
    20b0:  4267           	clrw %sp@-                            
    20b2:  4eba 2b5c      	jsr %pc@(0x4c10)                      ; MQSetLcdStride
    20b6:  382e fff6      	movew %fp@(-10),%d4                   
    20ba:  c9fc 00a0      	mulsw #160,%d4                        
    20be:  48c4           	extl %d4                              
    20c0:  2453           	moveal %a3@,%a2                       
    20c2:  b5fc 1f00 0000 	cmpal #520093696,%a2                  
    20c8:  5c4f           	addqw #6,%sp                          
    20ca:  650c           	bcss 0x20d8                           ; ↓ label_1
    20cc:  200a           	movel %a2,%d0                         
    20ce:  d084           	addl %d4,%d0                          
    20d0:  0c80 1f04 0000 	cmpil #520355840,%d0                  
    20d6:  6306           	blss 0x20de                           ; ↓ label_2

  label_1:
    20d8:  303c 2202      	movew #8706,%d0                       
    20dc:  601e           	bras 0x20fc                           ; ↓ label_4

  label_2:
    20de:  200a           	movel %a2,%d0                         
    20e0:  0680 e100 0000 	addil #-520093696,%d0                 
    20e6:  2d40 fff8      	movel %d0,%fp@(-8)                    
    20ea:  486e fff8      	pea %fp@(-8)                          
    20ee:  3f3c 0001      	movew #1,%sp@-                        
    20f2:  4eba 2aca      	jsr %pc@(0x4bbe)                      ; MQSetLcdStartAddr
    20f6:  5c4f           	addqw #6,%sp                          

  label_3:
    20f8:  2683           	movel %d3,%a3@                        
    20fa:  7000           	moveq #0,%d0                          

  label_4:
    20fc:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    2100:  4e5e           	unlk %fp                              
    2102:  4e75           	rts                                   
```

# PrvDisplayDepth: 0x211a - 0x21ae

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???

```
    211a:  4e56 fff8      	linkw %fp,#-8                         
    211e:  2f0a           	movel %a2,%sp@-                       
    2120:  246e 000a      	moveal %fp@(10),%a2                   
    2124:  200a           	movel %a2,%d0                         
    2126:  6606           	bnes 0x212e                           ; ↓ label_0
    2128:  303c 2201      	movew #8705,%d0                       
    212c:  607c           	bras 0x21aa                           ; ↓ label_4

  label_0:
    212e:  486e fffe      	pea %fp@(-2)                          
    2132:  4267           	clrw %sp@-                            
    2134:  4eba 2b4a      	jsr %pc@(0x4c80)                      ; MQSetLcdBpp
    2138:  4a2e 0008      	tstb %fp@(8)                          
    213c:  5c4f           	addqw #6,%sp                          
    213e:  6606           	bnes 0x2146                           ; ↓ label_1
    2140:  34ae fffe      	movew %fp@(-2),%a2@                   
    2144:  6062           	bras 0x21a8                           ; ↓ label_3

  label_1:
    2146:  3012           	movew %a2@,%d0                        
    2148:  b06e fffe      	cmpw %fp@(-2),%d0                     
    214c:  675a           	beqs 0x21a8                           ; ↓ label_3
    214e:  486e fffc      	pea %fp@(-4)                          
    2152:  4267           	clrw %sp@-                            
    2154:  4eba 2aba      	jsr %pc@(0x4c10)                      ; MQSetLcdStride
    2158:  4a6e fffe      	tstw %fp@(-2)                         
    215c:  5c4f           	addqw #6,%sp                          
    215e:  6712           	beqs 0x2172                           ; ↓ label_2
    2160:  302e fffc      	movew %fp@(-4),%d0                    
    2164:  c0d2           	muluw %a2@,%d0                        
    2166:  7200           	moveq #0,%d1                          
    2168:  3200           	movew %d0,%d1                         
    216a:  82ee fffe      	divuw %fp@(-2),%d1                    
    216e:  3d41 fffc      	movew %d1,%fp@(-4)                    

  label_2:
    2172:  3d52 fffe      	movew %a2@,%fp@(-2)                   
    2176:  486e fff8      	pea %fp@(-8)                          
    217a:  4267           	clrw %sp@-                            
    217c:  4eba 2a40      	jsr %pc@(0x4bbe)                      ; MQSetLcdStartAddr
    2180:  2f2e fff8      	movel %fp@(-8),%sp@-                  
    2184:  4eba 2d5e      	jsr %pc@(0x4ee4)                      ; MQErase
    2188:  4eba 2fa8      	jsr %pc@(0x5132)                      ; InVBlank
    218c:  486e fffe      	pea %fp@(-2)                          
    2190:  3f3c 0001      	movew #1,%sp@-                        
    2194:  4eba 2aea      	jsr %pc@(0x4c80)                      ; MQSetLcdBpp
    2198:  486e fffc      	pea %fp@(-4)                          
    219c:  3f3c 0001      	movew #1,%sp@-                        
    21a0:  4eba 2a6e      	jsr %pc@(0x4c10)                      ; MQSetLcdStride
    21a4:  4fef 0016      	lea %sp@(22),%sp                      

  label_3:
    21a8:  7000           	moveq #0,%d0                          

  label_4:
    21aa:  245f           	moveal %sp@+,%a2                      
    21ac:  4e5e           	unlk %fp                              
    21ae:  4e75           	rts                                   
```

# PrvDisplayRowBytes: 0x21c2 - 0x21e8

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-2)`     : ???

```
    21c2:  4e56 fffe      	linkw %fp,#-2                         
    21c6:  4a2e 0008      	tstb %fp@(8)                          
    21ca:  6706           	beqs 0x21d2                           ; ↓ label_0
    21cc:  303c 2204      	movew #8708,%d0                       
    21d0:  6014           	bras 0x21e6                           ; ↓ label_1

  label_0:
    21d2:  486e fffe      	pea %fp@(-2)                          
    21d6:  4267           	clrw %sp@-                            
    21d8:  4eba 2a36      	jsr %pc@(0x4c10)                      ; MQSetLcdStride
    21dc:  206e 000a      	moveal %fp@(10),%a0                   
    21e0:  30ae fffe      	movew %fp@(-2),%a0@                   
    21e4:  7000           	moveq #0,%d0                          

  label_1:
    21e6:  4e5e           	unlk %fp                              
    21e8:  4e75           	rts                                   
```

# PrvDisplaySize: 0x2200 - 0x227c

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???

```
    2200:  4e56 fff4      	linkw %fp,#-12                        
    2204:  2f0a           	movel %a2,%sp@-                       
    2206:  246e 000a      	moveal %fp@(10),%a2                   
    220a:  200a           	movel %a2,%d0                         
    220c:  6606           	bnes 0x2214                           ; ↓ label_0
    220e:  303c 2201      	movew #8705,%d0                       
    2212:  6064           	bras 0x2278                           ; ↓ label_3

  label_0:
    2214:  4a2e 0008      	tstb %fp@(8)                          
    2218:  6606           	bnes 0x2220                           ; ↓ label_1
    221a:  34bc 00a0      	movew #160,%a2@                       
    221e:  6056           	bras 0x2276                           ; ↓ label_2

  label_1:
    2220:  486e fffc      	pea %fp@(-4)                          
    2224:  4267           	clrw %sp@-                            
    2226:  4eba 2a58      	jsr %pc@(0x4c80)                      ; MQSetLcdBpp
    222a:  486e fff4      	pea %fp@(-12)                         
    222e:  4267           	clrw %sp@-                            
    2230:  4eba 298c      	jsr %pc@(0x4bbe)                      ; MQSetLcdStartAddr
    2234:  2f2e fff4      	movel %fp@(-12),%sp@-                 
    2238:  4eba 2caa      	jsr %pc@(0x4ee4)                      ; MQErase
    223c:  3d52 fffa      	movew %a2@,%fp@(-6)                   
    2240:  3d52 fffe      	movew %a2@,%fp@(-2)                   
    2244:  302e fffc      	movew %fp@(-4),%d0                    
    2248:  c1ee fffe      	mulsw %fp@(-2),%d0                    
    224c:  e640           	asrw #3,%d0                           
    224e:  3d40 fff8      	movew %d0,%fp@(-8)                    
    2252:  4eba 2ede      	jsr %pc@(0x5132)                      ; InVBlank
    2256:  486e fff8      	pea %fp@(-8)                          
    225a:  3f3c 0001      	movew #1,%sp@-                        
    225e:  4eba 29b0      	jsr %pc@(0x4c10)                      ; MQSetLcdStride
    2262:  486e fffa      	pea %fp@(-6)                          
    2266:  486e fffe      	pea %fp@(-2)                          
    226a:  3f3c 0001      	movew #1,%sp@-                        
    226e:  4eba 2af4      	jsr %pc@(0x4d64)                      ; MQSetLcdSize
    2272:  4fef 0020      	lea %sp@(32),%sp                      

  label_2:
    2276:  7000           	moveq #0,%d0                          

  label_3:
    2278:  245f           	moveal %sp@+,%a2                      
    227a:  4e5e           	unlk %fp                              
    227c:  4e75           	rts                                   
```

# HwrDisplayDrawBootScreen: 0x2290 - 0x2500

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-14)`    : ???

```
    2290:  4e56 fff2      	linkw %fp,#-14                        
    2294:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2298:  42ae fff4      	clrl %fp@(-12)                        
    229c:  247c ffff f000 	moveal #-4096,%a2                     
    22a2:  486e fff4      	pea %fp@(-12)                         
    22a6:  4267           	clrw %sp@-                            
    22a8:  4eba 2914      	jsr %pc@(0x4bbe)                      ; MQSetLcdStartAddr
    22ac:  282e fff4      	movel %fp@(-12),%d4                   
    22b0:  0684 1f00 0000 	addil #520093696,%d4                  
    22b6:  2a2e 000c      	movel %fp@(12),%d5                    
    22ba:  1f3c 0008      	moveb #8,%sp@-                        
    22be:  4eba 069a      	jsr %pc@(0x295a)                      ; PrvUpdateCLUT
    22c2:  302e 000a      	movew %fp@(10),%d0                    
    22c6:  c0fc 00a0      	muluw #160,%d0                        
    22ca:  d06e 0008      	addw %fp@(8),%d0                      
    22ce:  7200           	moveq #0,%d1                          
    22d0:  3200           	movew %d0,%d1                         
    22d2:  d881           	addl %d1,%d4                          
    22d4:  2045           	moveal %d5,%a0                        
    22d6:  5088           	addql #8,%a0                          
    22d8:  5088           	addql #8,%a0                          
    22da:  2608           	movel %a0,%d3                         
    22dc:  2e04           	movel %d4,%d7                         
    22de:  2045           	moveal %d5,%a0                        
    22e0:  0828 0007 0006 	btst #7,%a0@(6)                       
    22e6:  504f           	addqw #8,%sp                          
    22e8:  6700 00b4      	beqw 0x239e                           ; ↓ label_3
    22ec:  2043           	moveal %d3,%a0                        
    22ee:  3d50 fff2      	movew %a0@,%fp@(-14)                  
    22f2:  5488           	addql #2,%a0                          
    22f4:  2c08           	movel %a0,%d6                         
    22f6:  7600           	moveq #0,%d3                          
    22f8:  7800           	moveq #0,%d4                          
    22fa:  47ee fff8      	lea %fp@(-8),%a3                      

  label_0:
    22fe:  4213           	clrb %a3@                             
    2300:  3003           	movew %d3,%d0                         
    2302:  5240           	addqw #1,%d0                          
    2304:  7200           	moveq #0,%d1                          
    2306:  3200           	movew %d0,%d1                         
    2308:  41ee fff8      	lea %fp@(-8),%a0                      
    230c:  4230 1800      	clrb %a0@(00000000,%d1:l)             
    2310:  3003           	movew %d3,%d0                         
    2312:  5440           	addqw #2,%d0                          
    2314:  7200           	moveq #0,%d1                          
    2316:  3200           	movew %d0,%d1                         
    2318:  4230 1800      	clrb %a0@(00000000,%d1:l)             
    231c:  3003           	movew %d3,%d0                         
    231e:  5640           	addqw #3,%d0                          
    2320:  7200           	moveq #0,%d1                          
    2322:  3200           	movew %d0,%d1                         
    2324:  4230 1800      	clrb %a0@(00000000,%d1:l)             
    2328:  3003           	movew %d3,%d0                         
    232a:  5840           	addqw #4,%d0                          
    232c:  7200           	moveq #0,%d1                          
    232e:  3200           	movew %d0,%d1                         
    2330:  4230 1800      	clrb %a0@(00000000,%d1:l)             
    2334:  3003           	movew %d3,%d0                         
    2336:  5a40           	addqw #5,%d0                          
    2338:  7200           	moveq #0,%d1                          
    233a:  3200           	movew %d0,%d1                         
    233c:  4230 1800      	clrb %a0@(00000000,%d1:l)             
    2340:  3003           	movew %d3,%d0                         
    2342:  5c40           	addqw #6,%d0                          
    2344:  7200           	moveq #0,%d1                          
    2346:  3200           	movew %d0,%d1                         
    2348:  4230 1800      	clrb %a0@(00000000,%d1:l)             
    234c:  3003           	movew %d3,%d0                         
    234e:  5e40           	addqw #7,%d0                          
    2350:  7200           	moveq #0,%d1                          
    2352:  3200           	movew %d0,%d1                         
    2354:  4230 1800      	clrb %a0@(00000000,%d1:l)             
    2358:  508b           	addql #8,%a3                          
    235a:  5043           	addqw #8,%d3                          
    235c:  b644           	cmpw %d4,%d3                          
    235e:  639e           	blss 0x22fe                           ; ↑ label_0
    2360:  6000 018a      	braw 0x24ec                           ; ↓ label_10

  label_1:
    2364:  4213           	clrb %a3@                             
    2366:  528b           	addql #1,%a3                          
    2368:  5243           	addqw #1,%d3                          

  label_2:
    236a:  0c43 0008      	cmpiw #8,%d3                          
    236e:  63f4           	blss 0x2364                           ; ↑ label_1
    2370:  2045           	moveal %d5,%a0                        
    2372:  0c28 0002 0009 	cmpib #2,%a0@(9)                      
    2378:  6576           	bcss 0x23f0                           ; ↓ label_7
    237a:  486e fff8      	pea %fp@(-8)                          
    237e:  2f3c 0004 0000 	movel #262144,%sp@-                   
    2384:  2f07           	movel %d7,%sp@-                       
    2386:  7000           	moveq #0,%d0                          
    2388:  302e fff2      	movew %fp@(-14),%d0                   
    238c:  2f00           	movel %d0,%sp@-                       
    238e:  2f06           	movel %d6,%sp@-                       
    2390:  1f28 000d      	moveb %a0@(13),%sp@-                  
    2394:  4e4f           	trap #15                              
    2396:  a37c           	0121574                               ; sysTrapScrDecompress
    2398:  4fef 0016      	lea %sp@(22),%sp                      
    239c:  6052           	bras 0x23f0                           ; ↓ label_7

  label_3:
    239e:  3c3c 3200      	movew #12800,%d6                      
    23a2:  2643           	moveal %d3,%a3                        
    23a4:  2604           	movel %d4,%d3                         
    23a6:  7800           	moveq #0,%d4                          
    23a8:  3a3c 31f8      	movew #12792,%d5                      

  label_4:
    23ac:  2043           	moveal %d3,%a0                        
    23ae:  5483           	addql #2,%d3                          
    23b0:  309b           	movew %a3@+,%a0@                      
    23b2:  2043           	moveal %d3,%a0                        
    23b4:  5483           	addql #2,%d3                          
    23b6:  309b           	movew %a3@+,%a0@                      
    23b8:  2043           	moveal %d3,%a0                        
    23ba:  5483           	addql #2,%d3                          
    23bc:  309b           	movew %a3@+,%a0@                      
    23be:  2043           	moveal %d3,%a0                        
    23c0:  5483           	addql #2,%d3                          
    23c2:  309b           	movew %a3@+,%a0@                      
    23c4:  2043           	moveal %d3,%a0                        
    23c6:  5483           	addql #2,%d3                          
    23c8:  309b           	movew %a3@+,%a0@                      
    23ca:  2043           	moveal %d3,%a0                        
    23cc:  5483           	addql #2,%d3                          
    23ce:  309b           	movew %a3@+,%a0@                      
    23d0:  2043           	moveal %d3,%a0                        
    23d2:  5483           	addql #2,%d3                          
    23d4:  309b           	movew %a3@+,%a0@                      
    23d6:  2043           	moveal %d3,%a0                        
    23d8:  5483           	addql #2,%d3                          
    23da:  309b           	movew %a3@+,%a0@                      
    23dc:  5044           	addqw #8,%d4                          
    23de:  b845           	cmpw %d5,%d4                          
    23e0:  65ca           	bcss 0x23ac                           ; ↑ label_4
    23e2:  6008           	bras 0x23ec                           ; ↓ label_6

  label_5:
    23e4:  2043           	moveal %d3,%a0                        
    23e6:  5483           	addql #2,%d3                          
    23e8:  309b           	movew %a3@+,%a0@                      
    23ea:  5244           	addqw #1,%d4                          

  label_6:
    23ec:  b846           	cmpw %d6,%d4                          
    23ee:  65f4           	bcss 0x23e4                           ; ↑ label_5

  label_7:
    23f0:  0238 00df f411 	andib #-33,0xfffffffffffff411         
    23f6:  0238 00bf f411 	andib #-65,0xfffffffffffff411         
    23fc:  7600           	moveq #0,%d3                          

  label_8:
    23fe:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2404:  022a 007f 0411 	andib #127,%a2@(1041)                 
    240a:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2410:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2416:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    241c:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2422:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2428:  022a 007f 0411 	andib #127,%a2@(1041)                 
    242e:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2434:  022a 007f 0411 	andib #127,%a2@(1041)                 
    243a:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2440:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2446:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    244c:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2452:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2458:  022a 007f 0411 	andib #127,%a2@(1041)                 
    245e:  5003           	addqb #8,%d3                          
    2460:  0c03 0020      	cmpib #32,%d3                         
    2464:  6598           	bcss 0x23fe                           ; ↑ label_8
    2466:  0038 0010 f411 	orib #16,0xfffffffffffff411           
    246c:  0238 00df f411 	andib #-33,0xfffffffffffff411         
    2472:  0038 0040 f411 	orib #64,0xfffffffffffff411           
    2478:  7600           	moveq #0,%d3                          

  label_9:
    247a:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2480:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2486:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    248c:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2492:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2498:  022a 007f 0411 	andib #127,%a2@(1041)                 
    249e:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    24a4:  022a 007f 0411 	andib #127,%a2@(1041)                 
    24aa:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    24b0:  022a 007f 0411 	andib #127,%a2@(1041)                 
    24b6:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    24bc:  022a 007f 0411 	andib #127,%a2@(1041)                 
    24c2:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    24c8:  022a 007f 0411 	andib #127,%a2@(1041)                 
    24ce:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    24d4:  022a 007f 0411 	andib #127,%a2@(1041)                 
    24da:  5003           	addqb #8,%d3                          
    24dc:  0c03 0020      	cmpib #32,%d3                         
    24e0:  6598           	bcss 0x247a                           ; ↑ label_9
    24e2:  0038 0020 f411 	orib #32,0xfffffffffffff411           
    24e8:  7000           	moveq #0,%d0                          
    24ea:  600e           	bras 0x24fa                           ; ↓ label_11

  label_10:
    24ec:  7000           	moveq #0,%d0                          
    24ee:  3003           	movew %d3,%d0                         
    24f0:  47ee fff8      	lea %fp@(-8),%a3                      
    24f4:  d7c0           	addal %d0,%a3                         
    24f6:  6000 fe72      	braw 0x236a                           ; ↑ label_2

  label_11:
    24fa:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    24fe:  4e5e           	unlk %fp                              
    2500:  4e75           	rts                                   
```

# PrvDisplayBacklight: 0x251e - 0x275e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-27)`    : ???
   * `%fp@(-36)`    : ???

```
    251e:  4e56 ffdc      	linkw %fp,#-36                        
    2522:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2526:  2c2e 000a      	movel %fp@(10),%d6                    
    252a:  247c ffff f000 	moveal #-4096,%a2                     
    2530:  1e3c 0080      	moveb #-128,%d7                       
    2534:  2678 0304      	moveal 0x304,%a3                      
    2538:  3038 028e      	movew 0x28e,%d0                       
    253c:  0240 0001      	andiw #1,%d0                          
    2540:  6606           	bnes 0x2548                           ; ↓ label_0
    2542:  7000           	moveq #0,%d0                          
    2544:  6000 0212      	braw 0x2758                           ; ↓ label_13

  label_0:
    2548:  4a86           	tstl %d6                              
    254a:  6706           	beqs 0x2552                           ; ↓ label_1
    254c:  2046           	moveal %d6,%a0                        
    254e:  1010           	moveb %a0@,%d0                        
    2550:  6002           	bras 0x2554                           ; ↓ label_2

  label_1:
    2552:  7000           	moveq #0,%d0                          

  label_2:
    2554:  1600           	moveb %d0,%d3                         
    2556:  1038 f411      	moveb 0xfffffffffffff411,%d0          
    255a:  0240 0010      	andiw #16,%d0                         
    255e:  56c0           	sne %d0                               
    2560:  4400           	negb %d0                              
    2562:  1a00           	moveb %d0,%d5                         
    2564:  4a2e 0008      	tstb %fp@(8)                          
    2568:  6700 01e4      	beqw 0x274e                           ; ↓ label_11
    256c:  1d7c 0001 fffa 	moveb #1,%fp@(-6)                     
    2572:  4a03           	tstb %d3                              
    2574:  6700 0132      	beqw 0x26a8                           ; ↓ label_8
    2578:  4a05           	tstb %d5                              
    257a:  6600 012c      	bnew 0x26a8                           ; ↓ label_8
    257e:  486e fffa      	pea %fp@(-6)                          
    2582:  3f3c 000b      	movew #11,%sp@-                       
    2586:  4e4f           	trap #15                              
    2588:  a368           	0121550                               ; sysTrapHwrBattery
    258a:  4a2e fffe      	tstb %fp@(-2)                         
    258e:  5c4f           	addqw #6,%sp                          
    2590:  6600 00e2      	bnew 0x2674                           ; ↓ label_7
    2594:  0238 00df f411 	andib #-33,0xfffffffffffff411         
    259a:  0238 00bf f411 	andib #-65,0xfffffffffffff411         
    25a0:  7600           	moveq #0,%d3                          

  label_3:
    25a2:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    25a8:  022a 007f 0411 	andib #127,%a2@(1041)                 
    25ae:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    25b4:  022a 007f 0411 	andib #127,%a2@(1041)                 
    25ba:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    25c0:  022a 007f 0411 	andib #127,%a2@(1041)                 
    25c6:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    25cc:  022a 007f 0411 	andib #127,%a2@(1041)                 
    25d2:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    25d8:  022a 007f 0411 	andib #127,%a2@(1041)                 
    25de:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    25e4:  022a 007f 0411 	andib #127,%a2@(1041)                 
    25ea:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    25f0:  022a 007f 0411 	andib #127,%a2@(1041)                 
    25f6:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    25fc:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2602:  5003           	addqb #8,%d3                          
    2604:  0c03 0020      	cmpib #32,%d3                         
    2608:  6598           	bcss 0x25a2                           ; ↑ label_3
    260a:  486e ffdc      	pea %fp@(-36)                         
    260e:  4227           	clrb %sp@-                            
    2610:  4e4f           	trap #15                              
    2612:  a03f           	0120077                               ; sysTrapMemNVParams
    2614:  4a40           	tstw %d0                              
    2616:  5c4f           	addqw #6,%sp                          
    2618:  6608           	bnes 0x2622                           ; ↓ label_4
    261a:  162e ffe5      	moveb %fp@(-27),%d3                   
    261e:  6702           	beqs 0x2622                           ; ↓ label_4
    2620:  1e03           	moveb %d3,%d7                         

  label_4:
    2622:  0038 0010 f411 	orib #16,0xfffffffffffff411           
    2628:  0238 00df f411 	andib #-33,0xfffffffffffff411         
    262e:  0038 0040 f411 	orib #64,0xfffffffffffff411           
    2634:  7600           	moveq #0,%d3                          
    2636:  7000           	moveq #0,%d0                          
    2638:  1007           	moveb %d7,%d0                         
    263a:  3800           	movew %d0,%d4                         
    263c:  e444           	asrw #2,%d4                           
    263e:  e04c           	lsrw #8,%d4                           
    2640:  ea4c           	lsrw #5,%d4                           
    2642:  d840           	addw %d0,%d4                          
    2644:  e644           	asrw #3,%d4                           
    2646:  600e           	bras 0x2656                           ; ↓ label_6

  label_5:
    2648:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    264e:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2654:  5203           	addqb #1,%d3                          

  label_6:
    2656:  7000           	moveq #0,%d0                          
    2658:  1003           	moveb %d3,%d0                         
    265a:  b044           	cmpw %d4,%d0                          
    265c:  6dea           	blts 0x2648                           ; ↑ label_5
    265e:  0038 0020 f411 	orib #32,0xfffffffffffff411           
    2664:  200b           	movel %a3,%d0                         
    2666:  6700 00e6      	beqw 0x274e                           ; ↓ label_11
    266a:  70fb           	moveq #-5,%d0                         
    266c:  c1ab 000c      	andl %d0,%a3@(12)                     
    2670:  6000 00dc      	braw 0x274e                           ; ↓ label_11

  label_7:
    2674:  4ab8 0304      	tstl 0x304                            
    2678:  6700 00d4      	beqw 0x274e                           ; ↓ label_11
    267c:  3038 0250      	movew 0x250,%d0                       
    2680:  0240 0001      	andiw #1,%d0                          
    2684:  6600 00c8      	bnew 0x274e                           ; ↓ label_11
    2688:  7008           	moveq #8,%d0                          
    268a:  c0ab 000c      	andl %a3@(12),%d0                     
    268e:  6600 00be      	bnew 0x274e                           ; ↓ label_11
    2692:  42a7           	clrl %sp@-                            
    2694:  42a7           	clrl %sp@-                            
    2696:  42a7           	clrl %sp@-                            
    2698:  3f3c 1e01      	movew #7681,%sp@-                     
    269c:  4e4f           	trap #15                              
    269e:  a365           	0121545                               ; sysTrapErrAlertCustom
    26a0:  4fef 000e      	lea %sp@(14),%sp                      
    26a4:  6000 00a8      	braw 0x274e                           ; ↓ label_11

  label_8:
    26a8:  4a03           	tstb %d3                              
    26aa:  6600 00a2      	bnew 0x274e                           ; ↓ label_11
    26ae:  4a05           	tstb %d5                              
    26b0:  6700 009c      	beqw 0x274e                           ; ↓ label_11
    26b4:  0238 00df f411 	andib #-33,0xfffffffffffff411         
    26ba:  0238 00bf f411 	andib #-65,0xfffffffffffff411         
    26c0:  7600           	moveq #0,%d3                          

  label_9:
    26c2:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    26c8:  022a 007f 0411 	andib #127,%a2@(1041)                 
    26ce:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    26d4:  022a 007f 0411 	andib #127,%a2@(1041)                 
    26da:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    26e0:  022a 007f 0411 	andib #127,%a2@(1041)                 
    26e6:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    26ec:  022a 007f 0411 	andib #127,%a2@(1041)                 
    26f2:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    26f8:  022a 007f 0411 	andib #127,%a2@(1041)                 
    26fe:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2704:  022a 007f 0411 	andib #127,%a2@(1041)                 
    270a:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    2710:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2716:  002a 0080 0411 	orib #-128,%a2@(1041)                 
    271c:  022a 007f 0411 	andib #127,%a2@(1041)                 
    2722:  5003           	addqb #8,%d3                          
    2724:  0c03 0020      	cmpib #32,%d3                         
    2728:  6598           	bcss 0x26c2                           ; ↑ label_9
    272a:  0038 0020 f411 	orib #32,0xfffffffffffff411           
    2730:  0238 00ef f411 	andib #-17,0xfffffffffffff411         
    2736:  200b           	movel %a3,%d0                         
    2738:  6706           	beqs 0x2740                           ; ↓ label_10
    273a:  7004           	moveq #4,%d0                          
    273c:  81ab 000c      	orl %d0,%a3@(12)                      

  label_10:
    2740:  486e fffa      	pea %fp@(-6)                          
    2744:  3f3c 000c      	movew #12,%sp@-                       
    2748:  4e4f           	trap #15                              
    274a:  a368           	0121550                               ; sysTrapHwrBattery
    274c:  5c4f           	addqw #6,%sp                          

  label_11:
    274e:  4a86           	tstl %d6                              
    2750:  6704           	beqs 0x2756                           ; ↓ label_12
    2752:  2046           	moveal %d6,%a0                        
    2754:  1085           	moveb %d5,%a0@                        

  label_12:
    2756:  7000           	moveq #0,%d0                          

  label_13:
    2758:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    275c:  4e5e           	unlk %fp                              
    275e:  4e75           	rts                                   
```

# HwrDisplayPalette: 0x2776 - 0x2944

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???

Locals:

   * `%fp@(-2)`     : ???

```
    2776:  4e56 fffe      	linkw %fp,#-2                         
    277a:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    277e:  3c2e 000a      	movew %fp@(10),%d6                    
    2782:  362e 000c      	movew %fp@(12),%d3                    
    2786:  2e2e 000e      	movel %fp@(14),%d7                    
    278a:  4eba 1efe      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    278e:  2648           	moveal %a0,%a3                        
    2790:  4a87           	tstl %d7                              
    2792:  6608           	bnes 0x279c                           ; ↓ label_0
    2794:  303c 2201      	movew #8705,%d0                       
    2798:  6000 01a4      	braw 0x293e                           ; ↓ label_13

  label_0:
    279c:  486e fffe      	pea %fp@(-2)                          
    27a0:  4267           	clrw %sp@-                            
    27a2:  4eba 24dc      	jsr %pc@(0x4c80)                      ; MQSetLcdBpp
    27a6:  0c6e 0008 fffe 	cmpiw #8,%fp@(-2)                     
    27ac:  5c4f           	addqw #6,%sp                          
    27ae:  6e0a           	bgts 0x27ba                           ; ↓ label_1
    27b0:  7001           	moveq #1,%d0                          
    27b2:  322e fffe      	movew %fp@(-2),%d1                    
    27b6:  e368           	lslw %d1,%d0                          
    27b8:  6002           	bras 0x27bc                           ; ↓ label_2

  label_1:
    27ba:  7000           	moveq #0,%d0                          

  label_2:
    27bc:  3800           	movew %d0,%d4                         
    27be:  b644           	cmpw %d4,%d3                          
    27c0:  6212           	bhis 0x27d4                           ; ↓ label_3
    27c2:  0c46 ffff      	cmpiw #-1,%d6                         
    27c6:  6d0c           	blts 0x27d4                           ; ↓ label_3
    27c8:  bc44           	cmpw %d4,%d6                          
    27ca:  6c08           	bges 0x27d4                           ; ↓ label_3
    27cc:  3006           	movew %d6,%d0                         
    27ce:  d043           	addw %d3,%d0                          
    27d0:  b044           	cmpw %d4,%d0                          
    27d2:  6308           	blss 0x27dc                           ; ↓ label_4

  label_3:
    27d4:  303c 2202      	movew #8706,%d0                       
    27d8:  6000 0164      	braw 0x293e                           ; ↓ label_13

  label_4:
    27dc:  4a2e 0008      	tstb %fp@(8)                          
    27e0:  6700 009e      	beqw 0x2880                           ; ↓ label_8
    27e4:  4a46           	tstw %d6                              
    27e6:  6c4c           	bges 0x2834                           ; ↓ label_6
    27e8:  4eba 2948      	jsr %pc@(0x5132)                      ; InVBlank
    27ec:  7800           	moveq #0,%d4                          
    27ee:  4a43           	tstw %d3                              
    27f0:  6300 014a      	blsw 0x293c                           ; ↓ label_12
    27f4:  2447           	moveal %d7,%a2                        

  label_5:
    27f6:  7a00           	moveq #0,%d5                          
    27f8:  1a2a 0002      	moveb %a2@(2),%d5                     
    27fc:  e18d           	lsll #8,%d5                           
    27fe:  7200           	moveq #0,%d1                          
    2800:  122a 0001      	moveb %a2@(1),%d1                     
    2804:  8285           	orl %d5,%d1                           
    2806:  7a00           	moveq #0,%d5                          
    2808:  1a2a 0003      	moveb %a2@(3),%d5                     
    280c:  4845           	swap %d5                              
    280e:  4245           	clrw %d5                              
    2810:  8a81           	orl %d1,%d5                           
    2812:  2f05           	movel %d5,%sp@-                       
    2814:  7000           	moveq #0,%d0                          
    2816:  1012           	moveb %a2@,%d0                        
    2818:  e548           	lslw #2,%d0                           
    281a:  0640 0800      	addiw #2048,%d0                       
    281e:  4873 0000      	pea %a3@(00000000,%d0:w)              
    2822:  4eba 29c0      	jsr %pc@(0x51e4)                      ; sREGW
    2826:  588a           	addql #4,%a2                          
    2828:  5244           	addqw #1,%d4                          
    282a:  b843           	cmpw %d3,%d4                          
    282c:  504f           	addqw #8,%sp                          
    282e:  65c6           	bcss 0x27f6                           ; ↑ label_5
    2830:  6000 010a      	braw 0x293c                           ; ↓ label_12

  label_6:
    2834:  4eba 28fc      	jsr %pc@(0x5132)                      ; InVBlank
    2838:  3806           	movew %d6,%d4                         
    283a:  e54c           	lslw #2,%d4                           
    283c:  0644 0800      	addiw #2048,%d4                       
    2840:  7a00           	moveq #0,%d5                          
    2842:  4a43           	tstw %d3                              
    2844:  6300 00f6      	blsw 0x293c                           ; ↓ label_12
    2848:  2447           	moveal %d7,%a2                        

  label_7:
    284a:  7c00           	moveq #0,%d6                          
    284c:  1c2a 0002      	moveb %a2@(2),%d6                     
    2850:  e18e           	lsll #8,%d6                           
    2852:  7200           	moveq #0,%d1                          
    2854:  122a 0001      	moveb %a2@(1),%d1                     
    2858:  8286           	orl %d6,%d1                           
    285a:  7c00           	moveq #0,%d6                          
    285c:  1c2a 0003      	moveb %a2@(3),%d6                     
    2860:  4846           	swap %d6                              
    2862:  4246           	clrw %d6                              
    2864:  8c81           	orl %d1,%d6                           
    2866:  2f06           	movel %d6,%sp@-                       
    2868:  4873 4000      	pea %a3@(00000000,%d4:w)              
    286c:  4eba 2976      	jsr %pc@(0x51e4)                      ; sREGW
    2870:  588a           	addql #4,%a2                          
    2872:  5245           	addqw #1,%d5                          
    2874:  5844           	addqw #4,%d4                          
    2876:  ba43           	cmpw %d3,%d5                          
    2878:  504f           	addqw #8,%sp                          
    287a:  65ce           	bcss 0x284a                           ; ↑ label_7
    287c:  6000 00be      	braw 0x293c                           ; ↓ label_12

  label_8:
    2880:  4a46           	tstw %d6                              
    2882:  6c60           	bges 0x28e4                           ; ↓ label_10
    2884:  4eba 28ac      	jsr %pc@(0x5132)                      ; InVBlank
    2888:  7a00           	moveq #0,%d5                          
    288a:  4a43           	tstw %d3                              
    288c:  6300 00ae      	blsw 0x293c                           ; ↓ label_12
    2890:  3445           	moveaw %d5,%a2                        
    2892:  200a           	movel %a2,%d0                         
    2894:  e588           	lsll #2,%d0                           
    2896:  2447           	moveal %d7,%a2                        
    2898:  d5c0           	addal %d0,%a2                         

  label_9:
    289a:  7000           	moveq #0,%d0                          
    289c:  1012           	moveb %a2@,%d0                        
    289e:  e548           	lslw #2,%d0                           
    28a0:  0640 0800      	addiw #2048,%d0                       
    28a4:  4873 0000      	pea %a3@(00000000,%d0:w)              
    28a8:  4eba 2970      	jsr %pc@(0x521a)                      ; sREGR
    28ac:  2800           	movel %d0,%d4                         
    28ae:  2004           	movel %d4,%d0                         
    28b0:  0280 0000 00ff 	andil #255,%d0                        
    28b6:  1540 0001      	moveb %d0,%a2@(1)                     
    28ba:  2004           	movel %d4,%d0                         
    28bc:  e088           	lsrl #8,%d0                           
    28be:  0280 0000 00ff 	andil #255,%d0                        
    28c4:  1540 0002      	moveb %d0,%a2@(2)                     
    28c8:  2004           	movel %d4,%d0                         
    28ca:  4240           	clrw %d0                              
    28cc:  4840           	swap %d0                              
    28ce:  0280 0000 00ff 	andil #255,%d0                        
    28d4:  1540 0003      	moveb %d0,%a2@(3)                     
    28d8:  588a           	addql #4,%a2                          
    28da:  5245           	addqw #1,%d5                          
    28dc:  ba43           	cmpw %d3,%d5                          
    28de:  584f           	addqw #4,%sp                          
    28e0:  65b8           	bcss 0x289a                           ; ↑ label_9
    28e2:  6058           	bras 0x293c                           ; ↓ label_12

  label_10:
    28e4:  4eba 284c      	jsr %pc@(0x5132)                      ; InVBlank
    28e8:  7a00           	moveq #0,%d5                          
    28ea:  4a43           	tstw %d3                              
    28ec:  634e           	blss 0x293c                           ; ↓ label_12
    28ee:  2447           	moveal %d7,%a2                        

  label_11:
    28f0:  1485           	moveb %d5,%a2@                        
    28f2:  7000           	moveq #0,%d0                          
    28f4:  1012           	moveb %a2@,%d0                        
    28f6:  d046           	addw %d6,%d0                          
    28f8:  e548           	lslw #2,%d0                           
    28fa:  0640 0800      	addiw #2048,%d0                       
    28fe:  4873 0000      	pea %a3@(00000000,%d0:w)              
    2902:  4eba 2916      	jsr %pc@(0x521a)                      ; sREGR
    2906:  2800           	movel %d0,%d4                         
    2908:  2004           	movel %d4,%d0                         
    290a:  0280 0000 00ff 	andil #255,%d0                        
    2910:  1540 0001      	moveb %d0,%a2@(1)                     
    2914:  2004           	movel %d4,%d0                         
    2916:  e088           	lsrl #8,%d0                           
    2918:  0280 0000 00ff 	andil #255,%d0                        
    291e:  1540 0002      	moveb %d0,%a2@(2)                     
    2922:  2004           	movel %d4,%d0                         
    2924:  4240           	clrw %d0                              
    2926:  4840           	swap %d0                              
    2928:  0280 0000 00ff 	andil #255,%d0                        
    292e:  1540 0003      	moveb %d0,%a2@(3)                     
    2932:  588a           	addql #4,%a2                          
    2934:  5245           	addqw #1,%d5                          
    2936:  ba43           	cmpw %d3,%d5                          
    2938:  584f           	addqw #4,%sp                          
    293a:  65b4           	bcss 0x28f0                           ; ↑ label_11

  label_12:
    293c:  7000           	moveq #0,%d0                          

  label_13:
    293e:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2942:  4e5e           	unlk %fp                              
    2944:  4e75           	rts                                   
```

# PrvUpdateCLUT: 0x295a - 0x2be2

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-16)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-24)`    : ???
   * `%fp@(-28)`    : ???
   * `%fp@(-32)`    : ???
   * `%fp@(-36)`    : ???
   * `%fp@(-38)`    : ???
   * `%fp@(-40)`    : ???
   * `%fp@(-42)`    : ???
   * `%fp@(-52)`    : ???
   * `%fp@(-56)`    : ???
   * `%fp@(-60)`    : ???

```
    295a:  4e56 ffc4      	linkw %fp,#-60                        
    295e:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2962:  4eba 1d26      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    2966:  2648           	moveal %a0,%a3                        
    2968:  41fa 028a      	lea %pc@(0x2bf4),%a0                  ; ??? 0x2bf4
    296c:  43ee ffcc      	lea %fp@(-52),%a1                     
    2970:  22d8           	movel %a0@+,%a1@+                     
    2972:  22d8           	movel %a0@+,%a1@+                     
    2974:  22d8           	movel %a0@+,%a1@+                     
    2976:  263c 0000 0800 	movel #2048,%d3                       
    297c:  4eba 27b4      	jsr %pc@(0x5132)                      ; InVBlank
    2980:  7000           	moveq #0,%d0                          
    2982:  102e 0008      	moveb %fp@(8),%d0                     
    2986:  5340           	subqw #1,%d0                          
    2988:  6712           	beqs 0x299c                           ; ↓ label_0
    298a:  5340           	subqw #1,%d0                          
    298c:  6746           	beqs 0x29d4                           ; ↓ label_2
    298e:  5540           	subqw #2,%d0                          
    2990:  677a           	beqs 0x2a0c                           ; ↓ label_4
    2992:  5940           	subqw #4,%d0                          
    2994:  6700 00aa      	beqw 0x2a40                           ; ↓ label_6
    2998:  6000 023a      	braw 0x2bd4                           ; ↓ label_22

  label_0:
    299c:  7a01           	moveq #1,%d5                          
    299e:  6d00 023a      	bltw 0x2bda                           ; ↓ label_23

  label_1:
    29a2:  7000           	moveq #0,%d0                          
    29a4:  1005           	moveb %d5,%d0                         
    29a6:  c1fc 00f0      	mulsw #240,%d0                        
    29aa:  7c00           	moveq #0,%d6                          
    29ac:  1c00           	moveb %d0,%d6                         
    29ae:  2806           	movel %d6,%d4                         
    29b0:  2004           	movel %d4,%d0                         
    29b2:  e188           	lsll #8,%d0                           
    29b4:  8084           	orl %d4,%d0                           
    29b6:  2c04           	movel %d4,%d6                         
    29b8:  4846           	swap %d6                              
    29ba:  4246           	clrw %d6                              
    29bc:  8c80           	orl %d0,%d6                           
    29be:  2f06           	movel %d6,%sp@-                       
    29c0:  4873 3800      	pea %a3@(00000000,%d3:l)              
    29c4:  4eba 281e      	jsr %pc@(0x51e4)                      ; sREGW
    29c8:  5883           	addql #4,%d3                          
    29ca:  5345           	subqw #1,%d5                          
    29cc:  504f           	addqw #8,%sp                          
    29ce:  6cd2           	bges 0x29a2                           ; ↑ label_1
    29d0:  6000 0208      	braw 0x2bda                           ; ↓ label_23

  label_2:
    29d4:  7803           	moveq #3,%d4                          
    29d6:  6d00 0202      	bltw 0x2bda                           ; ↓ label_23
    29da:  7c0f           	moveq #15,%d6                         

  label_3:
    29dc:  1006           	moveb %d6,%d0                         
    29de:  e908           	lslb #4,%d0                           
    29e0:  7e00           	moveq #0,%d7                          
    29e2:  1e00           	moveb %d0,%d7                         
    29e4:  2a07           	movel %d7,%d5                         
    29e6:  2005           	movel %d5,%d0                         
    29e8:  e188           	lsll #8,%d0                           
    29ea:  8085           	orl %d5,%d0                           
    29ec:  2e05           	movel %d5,%d7                         
    29ee:  4847           	swap %d7                              
    29f0:  4247           	clrw %d7                              
    29f2:  8e80           	orl %d0,%d7                           
    29f4:  2f07           	movel %d7,%sp@-                       
    29f6:  4873 3800      	pea %a3@(00000000,%d3:l)              
    29fa:  4eba 27e8      	jsr %pc@(0x51e4)                      ; sREGW
    29fe:  5883           	addql #4,%d3                          
    2a00:  5b46           	subqw #5,%d6                          
    2a02:  5344           	subqw #1,%d4                          
    2a04:  504f           	addqw #8,%sp                          
    2a06:  6cd4           	bges 0x29dc                           ; ↑ label_3
    2a08:  6000 01d0      	braw 0x2bda                           ; ↓ label_23

  label_4:
    2a0c:  780f           	moveq #15,%d4                         
    2a0e:  6d00 01ca      	bltw 0x2bda                           ; ↓ label_23

  label_5:
    2a12:  1004           	moveb %d4,%d0                         
    2a14:  e908           	lslb #4,%d0                           
    2a16:  7a00           	moveq #0,%d5                          
    2a18:  1a00           	moveb %d0,%d5                         
    2a1a:  2c05           	movel %d5,%d6                         
    2a1c:  2006           	movel %d6,%d0                         
    2a1e:  e188           	lsll #8,%d0                           
    2a20:  8086           	orl %d6,%d0                           
    2a22:  2a06           	movel %d6,%d5                         
    2a24:  4845           	swap %d5                              
    2a26:  4245           	clrw %d5                              
    2a28:  8a80           	orl %d0,%d5                           
    2a2a:  2f05           	movel %d5,%sp@-                       
    2a2c:  4873 3800      	pea %a3@(00000000,%d3:l)              
    2a30:  4eba 27b2      	jsr %pc@(0x51e4)                      ; sREGW
    2a34:  5883           	addql #4,%d3                          
    2a36:  5344           	subqw #1,%d4                          
    2a38:  504f           	addqw #8,%sp                          
    2a3a:  6cd6           	bges 0x2a12                           ; ↑ label_5
    2a3c:  6000 019c      	braw 0x2bda                           ; ↓ label_23

  label_6:
    2a40:  426e ffd8      	clrw %fp@(-40)                        
    2a44:  0c6e 0006 ffd8 	cmpiw #6,%fp@(-40)                    
    2a4a:  6c00 0088      	bgew 0x2ad4                           ; ↓ label_12
    2a4e:  41ee ffcc      	lea %fp@(-52),%a0                     
    2a52:  2d48 ffec      	movel %a0,%fp@(-20)                   
    2a56:  2d48 ffc8      	movel %a0,%fp@(-56)                   

  label_7:
    2a5a:  426e fffa      	clrw %fp@(-6)                         
    2a5e:  0c6e 0003 fffa 	cmpiw #3,%fp@(-6)                     
    2a64:  6c5e           	bges 0x2ac4                           ; ↓ label_11
    2a66:  2d6e ffec fffc 	movel %fp@(-20),%fp@(-4)              
    2a6c:  2d6e ffc8 fff0 	movel %fp@(-56),%fp@(-16)             

  label_8:
    2a72:  7800           	moveq #0,%d4                          
    2a74:  0c44 0006      	cmpiw #6,%d4                          
    2a78:  6c3a           	bges 0x2ab4                           ; ↓ label_10
    2a7a:  246e ffec      	moveal %fp@(-20),%a2                  
    2a7e:  206e fff0      	moveal %fp@(-16),%a0                  
    2a82:  3250           	moveaw %a0@,%a1                       
    2a84:  2c09           	movel %a1,%d6                         
    2a86:  206e fffc      	moveal %fp@(-4),%a0                   
    2a8a:  3250           	moveaw %a0@,%a1                       
    2a8c:  2e09           	movel %a1,%d7                         
    2a8e:  4847           	swap %d7                              
    2a90:  4247           	clrw %d7                              

  label_9:
    2a92:  3052           	moveaw %a2@,%a0                       
    2a94:  2a08           	movel %a0,%d5                         
    2a96:  e18d           	lsll #8,%d5                           
    2a98:  8a86           	orl %d6,%d5                           
    2a9a:  8a87           	orl %d7,%d5                           
    2a9c:  2f05           	movel %d5,%sp@-                       
    2a9e:  4873 3800      	pea %a3@(00000000,%d3:l)              
    2aa2:  4eba 2740      	jsr %pc@(0x51e4)                      ; sREGW
    2aa6:  5883           	addql #4,%d3                          
    2aa8:  548a           	addql #2,%a2                          
    2aaa:  5244           	addqw #1,%d4                          
    2aac:  0c44 0006      	cmpiw #6,%d4                          
    2ab0:  504f           	addqw #8,%sp                          
    2ab2:  6dde           	blts 0x2a92                           ; ↑ label_9

  label_10:
    2ab4:  54ae fffc      	addql #2,%fp@(-4)                     
    2ab8:  526e fffa      	addqw #1,%fp@(-6)                     
    2abc:  0c6e 0003 fffa 	cmpiw #3,%fp@(-6)                     
    2ac2:  6dae           	blts 0x2a72                           ; ↑ label_8

  label_11:
    2ac4:  54ae ffc8      	addql #2,%fp@(-56)                    
    2ac8:  526e ffd8      	addqw #1,%fp@(-40)                    
    2acc:  0c6e 0006 ffd8 	cmpiw #6,%fp@(-40)                    
    2ad2:  6d86           	blts 0x2a5a                           ; ↑ label_7

  label_12:
    2ad4:  426e ffda      	clrw %fp@(-38)                        
    2ad8:  0c6e 0006 ffda 	cmpiw #6,%fp@(-38)                    
    2ade:  6c00 0094      	bgew 0x2b74                           ; ↓ label_18
    2ae2:  41ee ffcc      	lea %fp@(-52),%a0                     
    2ae6:  2d48 ffe4      	movel %a0,%fp@(-28)                   
    2aea:  2d48 ffc4      	movel %a0,%fp@(-60)                   

  label_13:
    2aee:  3d7c 0003 fff8 	movew #3,%fp@(-8)                     
    2af4:  0c6e 0006 fff8 	cmpiw #6,%fp@(-8)                     
    2afa:  6c66           	bges 0x2b62                           ; ↓ label_17
    2afc:  41ee ffcc      	lea %fp@(-52),%a0                     
    2b00:  5c88           	addql #6,%a0                          
    2b02:  2d48 fff4      	movel %a0,%fp@(-12)                   
    2b06:  2d6e ffc4 ffe8 	movel %fp@(-60),%fp@(-24)             

  label_14:
    2b0c:  7800           	moveq #0,%d4                          
    2b0e:  0c44 0006      	cmpiw #6,%d4                          
    2b12:  6c3e           	bges 0x2b52                           ; ↓ label_16
    2b14:  246e ffe4      	moveal %fp@(-28),%a2                  
    2b18:  2c2e ffe8      	movel %fp@(-24),%d6                   
    2b1c:  2e2e fff4      	movel %fp@(-12),%d7                   

  label_15:
    2b20:  3052           	moveaw %a2@,%a0                       
    2b22:  2a08           	movel %a0,%d5                         
    2b24:  e18d           	lsll #8,%d5                           
    2b26:  2046           	moveal %d6,%a0                        
    2b28:  3250           	moveaw %a0@,%a1                       
    2b2a:  2209           	movel %a1,%d1                         
    2b2c:  8285           	orl %d5,%d1                           
    2b2e:  2047           	moveal %d7,%a0                        
    2b30:  3250           	moveaw %a0@,%a1                       
    2b32:  2a09           	movel %a1,%d5                         
    2b34:  4845           	swap %d5                              
    2b36:  4245           	clrw %d5                              
    2b38:  8a81           	orl %d1,%d5                           
    2b3a:  2f05           	movel %d5,%sp@-                       
    2b3c:  4873 3800      	pea %a3@(00000000,%d3:l)              
    2b40:  4eba 26a2      	jsr %pc@(0x51e4)                      ; sREGW
    2b44:  5883           	addql #4,%d3                          
    2b46:  548a           	addql #2,%a2                          
    2b48:  5244           	addqw #1,%d4                          
    2b4a:  0c44 0006      	cmpiw #6,%d4                          
    2b4e:  504f           	addqw #8,%sp                          
    2b50:  6dce           	blts 0x2b20                           ; ↑ label_15

  label_16:
    2b52:  54ae fff4      	addql #2,%fp@(-12)                    
    2b56:  526e fff8      	addqw #1,%fp@(-8)                     
    2b5a:  0c6e 0006 fff8 	cmpiw #6,%fp@(-8)                     
    2b60:  6daa           	blts 0x2b0c                           ; ↑ label_14

  label_17:
    2b62:  54ae ffc4      	addql #2,%fp@(-60)                    
    2b66:  526e ffda      	addqw #1,%fp@(-38)                    
    2b6a:  0c6e 0006 ffda 	cmpiw #6,%fp@(-38)                    
    2b70:  6d00 ff7c      	bltw 0x2aee                           ; ↑ label_13

  label_18:
    2b74:  780f           	moveq #15,%d4                         
    2b76:  6d2c           	blts 0x2ba4                           ; ↓ label_20

  label_19:
    2b78:  1004           	moveb %d4,%d0                         
    2b7a:  e908           	lslb #4,%d0                           
    2b7c:  d004           	addb %d4,%d0                          
    2b7e:  7a00           	moveq #0,%d5                          
    2b80:  1a00           	moveb %d0,%d5                         
    2b82:  2d45 ffe0      	movel %d5,%fp@(-32)                   
    2b86:  2005           	movel %d5,%d0                         
    2b88:  e188           	lsll #8,%d0                           
    2b8a:  8085           	orl %d5,%d0                           
    2b8c:  4845           	swap %d5                              
    2b8e:  4245           	clrw %d5                              
    2b90:  8a80           	orl %d0,%d5                           
    2b92:  2f05           	movel %d5,%sp@-                       
    2b94:  4873 3800      	pea %a3@(00000000,%d3:l)              
    2b98:  4eba 264a      	jsr %pc@(0x51e4)                      ; sREGW
    2b9c:  5883           	addql #4,%d3                          
    2b9e:  5344           	subqw #1,%d4                          
    2ba0:  504f           	addqw #8,%sp                          
    2ba2:  6cd4           	bges 0x2b78                           ; ↑ label_19

  label_20:
    2ba4:  7817           	moveq #23,%d4                         
    2ba6:  6d32           	blts 0x2bda                           ; ↓ label_23

  label_21:
    2ba8:  306e ffd6      	moveaw %fp@(-42),%a0                  
    2bac:  2d48 ffdc      	movel %a0,%fp@(-36)                   
    2bb0:  2008           	movel %a0,%d0                         
    2bb2:  e188           	lsll #8,%d0                           
    2bb4:  80ae ffdc      	orl %fp@(-36),%d0                     
    2bb8:  2a08           	movel %a0,%d5                         
    2bba:  4845           	swap %d5                              
    2bbc:  4245           	clrw %d5                              
    2bbe:  8a80           	orl %d0,%d5                           
    2bc0:  2f05           	movel %d5,%sp@-                       
    2bc2:  4873 3800      	pea %a3@(00000000,%d3:l)              
    2bc6:  4eba 261c      	jsr %pc@(0x51e4)                      ; sREGW
    2bca:  5883           	addql #4,%d3                          
    2bcc:  5344           	subqw #1,%d4                          
    2bce:  504f           	addqw #8,%sp                          
    2bd0:  6cd6           	bges 0x2ba8                           ; ↑ label_21
    2bd2:  6006           	bras 0x2bda                           ; ↓ label_23

  label_22:
    2bd4:  303c 2203      	movew #8707,%d0                       
    2bd8:  6002           	bras 0x2bdc                           ; ↓ label_24

  label_23:
    2bda:  7000           	moveq #0,%d0                          

  label_24:
    2bdc:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    2be0:  4e5e           	unlk %fp                              
    2be2:  4e75           	rts                                   
```

# HwrDisplayDoze: 0x2c00 - 0x2c3e

Parameters:

   * `%fp@(8)`      : ???

```
    2c00:  4e56 0000      	linkw %fp,#0                          
    2c04:  4a2e 0008      	tstb %fp@(8)                          
    2c08:  6710           	beqs 0x2c1a                           ; ↓ label_0
    2c0a:  4267           	clrw %sp@-                            
    2c0c:  4eba 1f5e      	jsr %pc@(0x4b6c)                      ; MQEnableLCDClock
    2c10:  0238 00ef f431 	andib #-17,0xfffffffffffff431         
    2c16:  544f           	addqw #2,%sp                          
    2c18:  6020           	bras 0x2c3a                           ; ↓ label_1

  label_0:
    2c1a:  0038 0010 f431 	orib #16,0xfffffffffffff431           
    2c20:  42a7           	clrl %sp@-                            
    2c22:  4e4f           	trap #15                              
    2c24:  a249           	0121111                               ; sysTrapHwrDelay
    2c26:  3f3c 0001      	movew #1,%sp@-                        
    2c2a:  4eba 1f40      	jsr %pc@(0x4b6c)                      ; MQEnableLCDClock
    2c2e:  4878 03e8      	pea 0x3e8                             
    2c32:  4e4f           	trap #15                              
    2c34:  a249           	0121111                               ; sysTrapHwrDelay
    2c36:  4fef 000a      	lea %sp@(10),%sp                      

  label_1:
    2c3a:  7000           	moveq #0,%d0                          
    2c3c:  4e5e           	unlk %fp                              
    2c3e:  4e75           	rts                                   
```

# PenOpen: 0x2c52 - 0x2d98

```
    2c52:  4e56 0000      	linkw %fp,#0                          
    2c56:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    2c5a:  4e4f           	trap #15                              
    2c5c:  a26f           	0121157                               ; sysTrapPenClose
    2c5e:  4878 002c      	pea 0x2c                              
    2c62:  4e4f           	trap #15                              
    2c64:  a013           	0120023                               ; sysTrapMemPtrNew
    2c66:  2448           	moveal %a0,%a2                        
    2c68:  200a           	movel %a2,%d0                         
    2c6a:  584f           	addqw #4,%sp                          
    2c6c:  6602           	bnes 0x2c70                           ; ↓ label_0
    2c6e:  4e48           	trap #8                               

  label_0:
    2c70:  4227           	clrb %sp@-                            
    2c72:  4878 002c      	pea 0x2c                              
    2c76:  2f0a           	movel %a2,%sp@-                       
    2c78:  4e4f           	trap #15                              
    2c7a:  a027           	0120047                               ; sysTrapMemSet
    2c7c:  21ca 016c      	movel %a2,0x16c                       
    2c80:  14bc 0001      	moveb #1,%a2@                         
    2c84:  357c 000c 0020 	movew #12,%a2@(32)                    
    2c8a:  357c 0003 0022 	movew #3,%a2@(34)                     
    2c90:  357c 0002 0024 	movew #2,%a2@(36)                     
    2c96:  357c 0005 0026 	movew #5,%a2@(38)                     
    2c9c:  357c 0001 002a 	movew #1,%a2@(42)                     
    2ca2:  3038 028e      	movew 0x28e,%d0                       
    2ca6:  0240 0008      	andiw #8,%d0                          
    2caa:  4fef 000a      	lea %sp@(10),%sp                      
    2cae:  6708           	beqs 0x2cb8                           ; ↓ label_1
    2cb0:  357c 0069 001e 	movew #105,%a2@(30)                   
    2cb6:  6006           	bras 0x2cbe                           ; ↓ label_2

  label_1:
    2cb8:  357c 0180 001e 	movew #384,%a2@(30)                   

  label_2:
    2cbe:  4227           	clrb %sp@-                            
    2cc0:  4e4f           	trap #15                              
    2cc2:  a2e0           	0121340                               ; sysTrapPrefOpenPreferenceDB
    2cc4:  2808           	movel %a0,%d4                         
    2cc6:  544f           	addqw #2,%sp                          
    2cc8:  6744           	beqs 0x2d0e                           ; ↓ label_4
    2cca:  3f3c 0003      	movew #3,%sp@-                        
    2cce:  2f3c 7073 7973 	movel #1886615923,%sp@-               
    2cd4:  4e4f           	trap #15                              
    2cd6:  a05f           	0120137                               ; sysTrapDmGetResource
    2cd8:  2608           	movel %a0,%d3                         
    2cda:  5c4f           	addqw #6,%sp                          
    2cdc:  6728           	beqs 0x2d06                           ; ↓ label_3
    2cde:  2f03           	movel %d3,%sp@-                       
    2ce0:  4e4f           	trap #15                              
    2ce2:  a021           	0120041                               ; sysTrapMemHandleLock
    2ce4:  2648           	moveal %a0,%a3                        
    2ce6:  355b 0006      	movew %a3@+,%a2@(6)                   
    2cea:  355b 0008      	movew %a3@+,%a2@(8)                   
    2cee:  355b 0002      	movew %a3@+,%a2@(2)                   
    2cf2:  355b 0004      	movew %a3@+,%a2@(4)                   
    2cf6:  2f03           	movel %d3,%sp@-                       
    2cf8:  4e4f           	trap #15                              
    2cfa:  a022           	0120042                               ; sysTrapMemHandleUnlock
    2cfc:  2f03           	movel %d3,%sp@-                       
    2cfe:  4e4f           	trap #15                              
    2d00:  a061           	0120141                               ; sysTrapDmReleaseResource
    2d02:  4fef 000c      	lea %sp@(12),%sp                      

  label_3:
    2d06:  2f04           	movel %d4,%sp@-                       
    2d08:  4e4f           	trap #15                              
    2d0a:  a04a           	0120112                               ; sysTrapDmCloseDatabase
    2d0c:  584f           	addqw #4,%sp                          

  label_4:
    2d0e:  7600           	moveq #0,%d3                          
    2d10:  382a 0006      	movew %a2@(6),%d4                     
    2d14:  0c44 0070      	cmpiw #112,%d4                        
    2d18:  6506           	bcss 0x2d20                           ; ↓ label_5
    2d1a:  0c44 0200      	cmpiw #512,%d4                        
    2d1e:  6302           	blss 0x2d22                           ; ↓ label_6

  label_5:
    2d20:  7601           	moveq #1,%d3                          

  label_6:
    2d22:  382a 0008      	movew %a2@(8),%d4                     
    2d26:  0c44 0070      	cmpiw #112,%d4                        
    2d2a:  6506           	bcss 0x2d32                           ; ↓ label_7
    2d2c:  0c44 0200      	cmpiw #512,%d4                        
    2d30:  6302           	blss 0x2d34                           ; ↓ label_8

  label_7:
    2d32:  7601           	moveq #1,%d3                          

  label_8:
    2d34:  0c6a 0040 0002 	cmpiw #64,%a2@(2)                     
    2d3a:  6302           	blss 0x2d3e                           ; ↓ label_9
    2d3c:  7601           	moveq #1,%d3                          

  label_9:
    2d3e:  0c6a 0040 0004 	cmpiw #64,%a2@(4)                     
    2d44:  6302           	blss 0x2d48                           ; ↓ label_10
    2d46:  7601           	moveq #1,%d3                          

  label_10:
    2d48:  4a03           	tstb %d3                              
    2d4a:  6718           	beqs 0x2d64                           ; ↓ label_11
    2d4c:  357c 00b9 0006 	movew #185,%a2@(6)                    
    2d52:  357c 00fc 0008 	movew #252,%a2@(8)                    
    2d58:  357c 0014 0002 	movew #20,%a2@(2)                     
    2d5e:  357c 0012 0004 	movew #18,%a2@(4)                     

  label_11:
    2d64:  4238 0253      	clrb 0x253                            
    2d68:  0038 0002 f428 	orib #2,0xfffffffffffff428            
    2d6e:  0238 00fd f429 	andib #-3,0xfffffffffffff429          
    2d74:  0038 0002 f42a 	orib #2,0xfffffffffffff42a            
    2d7a:  0238 00fd f42b 	andib #-3,0xfffffffffffff42b          
    2d80:  3f3c 0090      	movew #144,%sp@-                      
    2d84:  4eba 0138      	jsr %pc@(0x2ebe)                      ; /
    2d88:  0278 ffef f304 	andiw #-17,0xfffffffffffff304         
    2d8e:  7000           	moveq #0,%d0                          
    2d90:  544f           	addqw #2,%sp                          
    2d92:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    2d96:  4e5e           	unlk %fp                              
    2d98:  4e75           	rts                                   
```

# PenClose: 0x2da4 - 0x2dd0

```
    2da4:  4e56 0000      	linkw %fp,#0                          
    2da8:  2f03           	movel %d3,%sp@-                       
    2daa:  0078 0010 f304 	oriw #16,0xfffffffffffff304           
    2db0:  4ab8 016c      	tstl 0x16c                            
    2db4:  6710           	beqs 0x2dc6                           ; ↓ label_0
    2db6:  2f38 016c      	movel 0x16c,%sp@-                     
    2dba:  4e4f           	trap #15                              
    2dbc:  a012           	0120022                               ; sysTrapMemChunkFree
    2dbe:  3600           	movew %d0,%d3                         
    2dc0:  584f           	addqw #4,%sp                          
    2dc2:  6702           	beqs 0x2dc6                           ; ↓ label_0
    2dc4:  4e48           	trap #8                               

  label_0:
    2dc6:  42b8 016c      	clrl 0x16c                            
    2dca:  7000           	moveq #0,%d0                          
    2dcc:  261f           	movel %sp@+,%d3                       
    2dce:  4e5e           	unlk %fp                              
    2dd0:  4e75           	rts                                   
```

# PenSleep: 0x2dde - 0x2e0c

```
    2dde:  4e56 0000      	linkw %fp,#0                          
    2de2:  0078 0010 f304 	oriw #16,0xfffffffffffff304           
    2de8:  0038 0002 f428 	orib #2,0xfffffffffffff428            
    2dee:  0238 00fd f429 	andib #-3,0xfffffffffffff429          
    2df4:  0238 00fd f42a 	andib #-3,0xfffffffffffff42a          
    2dfa:  0038 0002 f42b 	orib #2,0xfffffffffffff42b            
    2e00:  3f3c 0091      	movew #145,%sp@-                      
    2e04:  4eba 00b8      	jsr %pc@(0x2ebe)                      ; /
    2e08:  7000           	moveq #0,%d0                          
    2e0a:  4e5e           	unlk %fp                              
    2e0c:  4e75           	rts                                   
```

# PenWake: 0x2e1a - 0x2e4c

```
    2e1a:  4e56 0000      	linkw %fp,#0                          
    2e1e:  0038 0002 f42a 	orib #2,0xfffffffffffff42a            
    2e24:  0038 0002 f428 	orib #2,0xfffffffffffff428            
    2e2a:  0238 00fd f429 	andib #-3,0xfffffffffffff429          
    2e30:  0238 00fd f42b 	andib #-3,0xfffffffffffff42b          
    2e36:  3f3c 0090      	movew #144,%sp@-                      
    2e3a:  4eba 0082      	jsr %pc@(0x2ebe)                      ; /
    2e3e:  4238 0253      	clrb 0x253                            
    2e42:  0278 ffef f304 	andiw #-17,0xfffffffffffff304         
    2e48:  7000           	moveq #0,%d0                          
    2e4a:  4e5e           	unlk %fp                              
    2e4c:  4e75           	rts                                   
```

# PrvBBGetXY: 0x2e58 - 0x2eae

Parameters:

   * `%fp@(8)`      : ???

```
    2e58:  4e56 0000      	linkw %fp,#0                          
    2e5c:  2f0b           	movel %a3,%sp@-                       
    2e5e:  2f0a           	movel %a2,%sp@-                       
    2e60:  266e 0008      	moveal %fp@(8),%a3                    
    2e64:  2478 016c      	moveal 0x16c,%a2                      
    2e68:  0238 00fd f42a 	andib #-3,0xfffffffffffff42a          
    2e6e:  0038 0002 f42b 	orib #2,0xfffffffffffff42b            
    2e74:  3f3c 0098      	movew #152,%sp@-                      
    2e78:  4eba 0044      	jsr %pc@(0x2ebe)                      ; /
    2e7c:  e848           	lsrw #4,%d0                           
    2e7e:  3740 0002      	movew %d0,%a3@(2)                     
    2e82:  3f3c 00d8      	movew #216,%sp@-                      
    2e86:  4eba 0036      	jsr %pc@(0x2ebe)                      ; /
    2e8a:  e848           	lsrw #4,%d0                           
    2e8c:  3680           	movew %d0,%a3@                        
    2e8e:  0038 0002 f42a 	orib #2,0xfffffffffffff42a            
    2e94:  0238 00fd f42b 	andib #-3,0xfffffffffffff42b          
    2e9a:  7000           	moveq #0,%d0                          
    2e9c:  302a 001e      	movew %a2@(30),%d0                    
    2ea0:  2f00           	movel %d0,%sp@-                       
    2ea2:  4e4f           	trap #15                              
    2ea4:  a249           	0121111                               ; sysTrapHwrDelay
    2ea6:  504f           	addqw #8,%sp                          
    2ea8:  245f           	moveal %sp@+,%a2                      
    2eaa:  265f           	moveal %sp@+,%a3                      
    2eac:  4e5e           	unlk %fp                              
    2eae:  4e75           	rts                                   
```

# /: 0x2ebe - 0x2ef2

```
    2ebe:  40e7           	movew %sr,%sp@-                       
    2ec0:  007c 0700      	oriw #1792,%sr                        
    2ec4:  41f8 f802      	lea 0xfffffffffffff802,%a0            
    2ec8:  43f8 f800      	lea 0xfffffffffffff800,%a1            
    2ecc:  3f10           	movew %a0@,%sp@-                      
    2ece:  4251           	clrw %a1@                             
    2ed0:  30bc 2247      	movew #8775,%a0@                      
    2ed4:  0238 ffdf f449 	andib #-33,0xfffffffffffff449         
    2eda:  3f3c 0001      	movew #1,%sp@-                        
    2ede:  6114           	bsrs 0x2ef4                           
    2ee0:  4257           	clrw %sp@                             
    2ee2:  6110           	bsrs 0x2ef4                           
    2ee4:  4fef 0002      	lea %sp@(2),%sp                       
    2ee8:  0038 ffdf f449 	orib #-33,0xfffffffffffff449          
    2eee:  309f           	movew %sp@+,%a0@                      
    2ef0:  46df           	movew %sp@+,%sr                       
    2ef2:  4e75           	rts                                   
```

# PenGetRawPen: 0x2f96 - 0x3240

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-10)`    : ???

```
    2f96:  4e56 fff6      	linkw %fp,#-10                        
    2f9a:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    2f9e:  267c ffff f000 	moveal #-4096,%a3                     
    2fa4:  2478 016c      	moveal 0x16c,%a2                      
    2fa8:  7a00           	moveq #0,%d5                          
    2faa:  7c00           	moveq #0,%d6                          
    2fac:  4ab8 016c      	tstl 0x16c                            
    2fb0:  6608           	bnes 0x2fba                           ; ↓ label_0
    2fb2:  303c 0b01      	movew #2817,%d0                       
    2fb6:  6000 0282      	braw 0x323a                           ; ↓ label_34

  label_0:
    2fba:  7604           	moveq #4,%d3                          
    2fbc:  204a           	moveal %a2,%a0                        
    2fbe:  5088           	addql #8,%a0                          
    2fc0:  5088           	addql #8,%a0                          
    2fc2:  5088           	addql #8,%a0                          
    2fc4:  5488           	addql #2,%a0                          
    2fc6:  2d48 fff6      	movel %a0,%fp@(-10)                   
    2fca:  2808           	movel %a0,%d4                         
    2fcc:  5988           	subql #4,%a0                          
    2fce:  2e08           	movel %a0,%d7                         
    2fd0:  3003           	movew %d3,%d0                         
    2fd2:  5343           	subqw #1,%d3                          
    2fd4:  4a40           	tstw %d0                              
    2fd6:  6712           	beqs 0x2fea                           ; ↓ label_2

  label_1:
    2fd8:  2047           	moveal %d7,%a0                        
    2fda:  5987           	subql #4,%d7                          
    2fdc:  2244           	moveal %d4,%a1                        
    2fde:  5984           	subql #4,%d4                          
    2fe0:  2290           	movel %a0@,%a1@                       
    2fe2:  3003           	movew %d3,%d0                         
    2fe4:  5343           	subqw #1,%d3                          
    2fe6:  4a40           	tstw %d0                              
    2fe8:  66ee           	bnes 0x2fd8                           ; ↑ label_1

  label_2:
    2fea:  302b 0310      	movew %a3@(784),%d0                   
    2fee:  0240 0010      	andiw #16,%d0                         
    2ff2:  6616           	bnes 0x300a                           ; ↓ label_4
    2ff4:  4a38 0253      	tstb 0x253                            
    2ff8:  6606           	bnes 0x3000                           ; ↓ label_3
    2ffa:  7af6           	moveq #-10,%d5                        
    2ffc:  6000 00d0      	braw 0x30ce                           ; ↓ label_13

  label_3:
    3000:  357c ffff 000a 	movew #-1,%a2@(10)                    
    3006:  6000 00c6      	braw 0x30ce                           ; ↓ label_13

  label_4:
    300a:  4a38 0253      	tstb 0x253                            
    300e:  663a           	bnes 0x304a                           ; ↓ label_5
    3010:  357c 7fff 000a 	movew #32767,%a2@(10)                 
    3016:  356a 002a 0028 	movew %a2@(42),%a2@(40)               
    301c:  357c ffff 000e 	movew #-1,%a2@(14)                    
    3022:  204a           	moveal %a2,%a0                        
    3024:  5888           	addql #4,%a0                          
    3026:  7004           	moveq #4,%d0                          
    3028:  31bc ffff 080a 	movew #-1,%a0@(0000000a,%d0:l)        
    302e:  204a           	moveal %a2,%a0                        
    3030:  5888           	addql #4,%a0                          
    3032:  7008           	moveq #8,%d0                          
    3034:  31bc ffff 080a 	movew #-1,%a0@(0000000a,%d0:l)        
    303a:  204a           	moveal %a2,%a0                        
    303c:  5888           	addql #4,%a0                          
    303e:  700c           	moveq #12,%d0                         
    3040:  31bc ffff 080a 	movew #-1,%a0@(0000000a,%d0:l)        
    3046:  6000 0086      	braw 0x30ce                           ; ↓ label_13

  label_5:
    304a:  4a6a 0028      	tstw %a2@(40)                         
    304e:  670e           	beqs 0x305e                           ; ↓ label_6
    3050:  536a 0028      	subqw #1,%a2@(40)                     
    3054:  357c 7fff 000a 	movew #32767,%a2@(10)                 
    305a:  7af5           	moveq #-11,%d5                        
    305c:  6070           	bras 0x30ce                           ; ↓ label_13

  label_6:
    305e:  486a 000a      	pea %a2@(10)                          
    3062:  4eba fdf4      	jsr %pc@(0x2e58)                      ; PrvBBGetXY
    3066:  302b 0310      	movew %a3@(784),%d0                   
    306a:  0240 0010      	andiw #16,%d0                         
    306e:  584f           	addqw #4,%sp                          
    3070:  6608           	bnes 0x307a                           ; ↓ label_7
    3072:  357c ffff 000a 	movew #-1,%a2@(10)                    
    3078:  6054           	bras 0x30ce                           ; ↓ label_13

  label_7:
    307a:  3006           	movew %d6,%d0                         
    307c:  5246           	addqw #1,%d6                          
    307e:  4a40           	tstw %d0                              
    3080:  660a           	bnes 0x308c                           ; ↓ label_8
    3082:  2d6a 000a fffc 	movel %a2@(10),%fp@(-4)               
    3088:  6000 ff60      	braw 0x2fea                           ; ↑ label_2

  label_8:
    308c:  382e fffe      	movew %fp@(-2),%d4                    
    3090:  986a 000c      	subw %a2@(12),%d4                     
    3094:  4a44           	tstw %d4                              
    3096:  6c02           	bges 0x309a                           ; ↓ label_9
    3098:  4444           	negw %d4                              

  label_9:
    309a:  362e fffc      	movew %fp@(-4),%d3                    
    309e:  966a 000a      	subw %a2@(10),%d3                     
    30a2:  4a43           	tstw %d3                              
    30a4:  6c02           	bges 0x30a8                           ; ↓ label_10
    30a6:  4443           	negw %d3                              

  label_10:
    30a8:  3e2a 0024      	movew %a2@(36),%d7                    
    30ac:  b847           	cmpw %d7,%d4                          
    30ae:  6204           	bhis 0x30b4                           ; ↓ label_11
    30b0:  b647           	cmpw %d7,%d3                          
    30b2:  631a           	blss 0x30ce                           ; ↓ label_13

  label_11:
    30b4:  bc6a 0022      	cmpw %a2@(34),%d6                     
    30b8:  650a           	bcss 0x30c4                           ; ↓ label_12
    30ba:  357c 7fff 000a 	movew #32767,%a2@(10)                 
    30c0:  7afe           	moveq #-2,%d5                         
    30c2:  600a           	bras 0x30ce                           ; ↓ label_13

  label_12:
    30c4:  2d6a 000a fffc 	movel %a2@(10),%fp@(-4)               
    30ca:  6000 ff1e      	braw 0x2fea                           ; ↑ label_2

  label_13:
    30ce:  4a45           	tstw %d5                              
    30d0:  6600 015c      	bnew 0x322e                           ; ↓ label_32
    30d4:  4a38 0253      	tstb 0x253                            
    30d8:  6618           	bnes 0x30f2                           ; ↓ label_14
    30da:  4a6a 000a      	tstw %a2@(10)                         
    30de:  6d12           	blts 0x30f2                           ; ↓ label_14
    30e0:  0078 0010 f304 	oriw #16,0xfffffffffffff304           
    30e6:  11fc 0002 0253 	moveb #2,0x253                        
    30ec:  7afd           	moveq #-3,%d5                         
    30ee:  6000 013e      	braw 0x322e                           ; ↓ label_32

  label_14:
    30f2:  264a           	moveal %a2,%a3                        
    30f4:  47eb 0012      	lea %a3@(18),%a3                      
    30f8:  4a6a 0012      	tstw %a2@(18)                         
    30fc:  6c3a           	bges 0x3138                           ; ↓ label_18
    30fe:  7600           	moveq #0,%d3                          
    3100:  0c43 0003      	cmpiw #3,%d3                          
    3104:  6c14           	bges 0x311a                           ; ↓ label_17

  label_15:
    3106:  4a53           	tstw %a3@                             
    3108:  6d06           	blts 0x3110                           ; ↓ label_16
    310a:  7afc           	moveq #-4,%d5                         
    310c:  6000 0120      	braw 0x322e                           ; ↓ label_32

  label_16:
    3110:  5243           	addqw #1,%d3                          
    3112:  598b           	subql #4,%a3                          
    3114:  0c43 0003      	cmpiw #3,%d3                          
    3118:  6dec           	blts 0x3106                           ; ↑ label_15

  label_17:
    311a:  206e 0008      	moveal %fp@(8),%a0                    
    311e:  317c ffff 0002 	movew #-1,%a0@(2)                     
    3124:  30bc ffff      	movew #-1,%a0@                        
    3128:  4238 0253      	clrb 0x253                            
    312c:  0278 ffef f304 	andiw #-17,0xfffffffffffff304         
    3132:  7a02           	moveq #2,%d5                          
    3134:  6000 00f8      	braw 0x322e                           ; ↓ label_32

  label_18:
    3138:  0c53 7fff      	cmpiw #32767,%a3@                     
    313c:  6606           	bnes 0x3144                           ; ↓ label_19
    313e:  7afb           	moveq #-5,%d5                         
    3140:  6000 00ec      	braw 0x322e                           ; ↓ label_32

  label_19:
    3144:  3c2b fffc      	movew %a3@(-4),%d6                    
    3148:  6c32           	bges 0x317c                           ; ↓ label_22
    314a:  4a6b 0004      	tstw %a3@(4)                          
    314e:  6d2c           	blts 0x317c                           ; ↓ label_22
    3150:  382b 0004      	movew %a3@(4),%d4                     
    3154:  9853           	subw %a3@,%d4                         
    3156:  362b 0006      	movew %a3@(6),%d3                     
    315a:  966b 0002      	subw %a3@(2),%d3                      
    315e:  4a44           	tstw %d4                              
    3160:  6c02           	bges 0x3164                           ; ↓ label_20
    3162:  4444           	negw %d4                              

  label_20:
    3164:  4a43           	tstw %d3                              
    3166:  6c02           	bges 0x316a                           ; ↓ label_21
    3168:  4443           	negw %d3                              

  label_21:
    316a:  3a2a 0026      	movew %a2@(38),%d5                    
    316e:  b845           	cmpw %d5,%d4                          
    3170:  640a           	bccs 0x317c                           ; ↓ label_22
    3172:  b645           	cmpw %d5,%d3                          
    3174:  6406           	bccs 0x317c                           ; ↓ label_22
    3176:  7afa           	moveq #-6,%d5                         
    3178:  6000 00b4      	braw 0x322e                           ; ↓ label_32

  label_22:
    317c:  3a2b 0004      	movew %a3@(4),%d5                     
    3180:  0c45 7fff      	cmpiw #32767,%d5                      
    3184:  6634           	bnes 0x31ba                           ; ↓ label_25
    3186:  4a46           	tstw %d6                              
    3188:  6d30           	blts 0x31ba                           ; ↓ label_25
    318a:  4a6b fff8      	tstw %a3@(-8)                         
    318e:  6d2a           	blts 0x31ba                           ; ↓ label_25
    3190:  382b fffc      	movew %a3@(-4),%d4                    
    3194:  9853           	subw %a3@,%d4                         
    3196:  362b fffe      	movew %a3@(-2),%d3                    
    319a:  966b 0002      	subw %a3@(2),%d3                      
    319e:  4a44           	tstw %d4                              
    31a0:  6c02           	bges 0x31a4                           ; ↓ label_23
    31a2:  4444           	negw %d4                              

  label_23:
    31a4:  4a43           	tstw %d3                              
    31a6:  6c02           	bges 0x31aa                           ; ↓ label_24
    31a8:  4443           	negw %d3                              

  label_24:
    31aa:  3c2a 0026      	movew %a2@(38),%d6                    
    31ae:  b846           	cmpw %d6,%d4                          
    31b0:  6408           	bccs 0x31ba                           ; ↓ label_25
    31b2:  b646           	cmpw %d6,%d3                          
    31b4:  6404           	bccs 0x31ba                           ; ↓ label_25
    31b6:  7af9           	moveq #-7,%d5                         
    31b8:  6074           	bras 0x322e                           ; ↓ label_32

  label_25:
    31ba:  362b 0008      	movew %a3@(8),%d3                     
    31be:  6d10           	blts 0x31d0                           ; ↓ label_26
    31c0:  4a45           	tstw %d5                              
    31c2:  6d0c           	blts 0x31d0                           ; ↓ label_26
    31c4:  0c43 7fff      	cmpiw #32767,%d3                      
    31c8:  6706           	beqs 0x31d0                           ; ↓ label_26
    31ca:  0c45 7fff      	cmpiw #32767,%d5                      
    31ce:  660a           	bnes 0x31da                           ; ↓ label_27

  label_26:
    31d0:  206e 0008      	moveal %fp@(8),%a0                    
    31d4:  2093           	movel %a3@,%a0@                       
    31d6:  7a01           	moveq #1,%d5                          
    31d8:  6054           	bras 0x322e                           ; ↓ label_32

  label_27:
    31da:  362b 0006      	movew %a3@(6),%d3                     
    31de:  3c2b 0002      	movew %a3@(2),%d6                     
    31e2:  9c43           	subw %d3,%d6                          
    31e4:  3a03           	movew %d3,%d5                         
    31e6:  9a6b 000a      	subw %a3@(10),%d5                     
    31ea:  3d6b 0004 fffa 	movew %a3@(4),%fp@(-6)                
    31f0:  302e fffa      	movew %fp@(-6),%d0                    
    31f4:  906b 0008      	subw %a3@(8),%d0                      
    31f8:  3613           	movew %a3@,%d3                        
    31fa:  966e fffa      	subw %fp@(-6),%d3                     
    31fe:  9640           	subw %d0,%d3                          
    3200:  4a43           	tstw %d3                              
    3202:  6c02           	bges 0x3206                           ; ↓ label_28
    3204:  4443           	negw %d3                              

  label_28:
    3206:  3806           	movew %d6,%d4                         
    3208:  9845           	subw %d5,%d4                          
    320a:  4a44           	tstw %d4                              
    320c:  6c02           	bges 0x3210                           ; ↓ label_29
    320e:  4444           	negw %d4                              

  label_29:
    3210:  3a2a 0020      	movew %a2@(32),%d5                    
    3214:  b645           	cmpw %d5,%d3                          
    3216:  6204           	bhis 0x321c                           ; ↓ label_30
    3218:  b845           	cmpw %d5,%d4                          
    321a:  630a           	blss 0x3226                           ; ↓ label_31

  label_30:
    321c:  377c 7fff 0004 	movew #32767,%a3@(4)                  
    3222:  7af8           	moveq #-8,%d5                         
    3224:  6008           	bras 0x322e                           ; ↓ label_32

  label_31:
    3226:  206e 0008      	moveal %fp@(8),%a0                    
    322a:  2093           	movel %a3@,%a0@                       
    322c:  7a00           	moveq #0,%d5                          

  label_32:
    322e:  4a45           	tstw %d5                              
    3230:  6d04           	blts 0x3236                           ; ↓ label_33
    3232:  7000           	moveq #0,%d0                          
    3234:  6004           	bras 0x323a                           ; ↓ label_34

  label_33:
    3236:  303c 0b02      	movew #2818,%d0                       

  label_34:
    323a:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    323e:  4e5e           	unlk %fp                              
    3240:  4e75           	rts                                   
```

# PenResetCalibration: 0x3252 - 0x3274

```
    3252:  4e56 0000      	linkw %fp,#0                          
    3256:  2078 016c      	moveal 0x16c,%a0                      
    325a:  317c 0100 0006 	movew #256,%a0@(6)                    
    3260:  317c 0100 0008 	movew #256,%a0@(8)                    
    3266:  4268 0002      	clrw %a0@(2)                          
    326a:  4268 0004      	clrw %a0@(4)                          
    326e:  4210           	clrb %a0@                             
    3270:  7000           	moveq #0,%d0                          
    3272:  4e5e           	unlk %fp                              
    3274:  4e75           	rts                                   
```

# PenCalibrate: 0x328c - 0x34a8

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
   * `%fp@(-18)`    : ???

```
    328c:  4e56 ffee      	linkw %fp,#-18                        
    3290:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    3294:  266e 0010      	moveal %fp@(16),%a3                   
    3298:  2478 016c      	moveal 0x16c,%a2                      
    329c:  206e 0008      	moveal %fp@(8),%a0                    
    32a0:  3250           	moveaw %a0@,%a1                       
    32a2:  2009           	movel %a1,%d0                         
    32a4:  e188           	lsll #8,%d0                           
    32a6:  7200           	moveq #0,%d1                          
    32a8:  322a 0006      	movew %a2@(6),%d1                     
    32ac:  2801           	movel %d1,%d4                         
    32ae:  4eba ce5e      	jsr %pc@(0x10e)                       ; anonymous_function_1
    32b2:  7200           	moveq #0,%d1                          
    32b4:  322a 0002      	movew %a2@(2),%d1                     
    32b8:  2a01           	movel %d1,%d5                         
    32ba:  d280           	addl %d0,%d1                          
    32bc:  3d41 fffc      	movew %d1,%fp@(-4)                    
    32c0:  3268 0002      	moveaw %a0@(2),%a1                    
    32c4:  2009           	movel %a1,%d0                         
    32c6:  e188           	lsll #8,%d0                           
    32c8:  7200           	moveq #0,%d1                          
    32ca:  322a 0008      	movew %a2@(8),%d1                     
    32ce:  2d41 fff4      	movel %d1,%fp@(-12)                   
    32d2:  4eba ce3a      	jsr %pc@(0x10e)                       ; anonymous_function_1
    32d6:  7200           	moveq #0,%d1                          
    32d8:  322a 0004      	movew %a2@(4),%d1                     
    32dc:  2d41 fff0      	movel %d1,%fp@(-16)                   
    32e0:  d280           	addl %d0,%d1                          
    32e2:  41ee fffc      	lea %fp@(-4),%a0                      
    32e6:  5488           	addql #2,%a0                          
    32e8:  2608           	movel %a0,%d3                         
    32ea:  3081           	movew %d1,%a0@                        
    32ec:  206e 000c      	moveal %fp@(12),%a0                   
    32f0:  3250           	moveaw %a0@,%a1                       
    32f2:  2009           	movel %a1,%d0                         
    32f4:  e188           	lsll #8,%d0                           
    32f6:  2204           	movel %d4,%d1                         
    32f8:  4eba ce14      	jsr %pc@(0x10e)                       ; anonymous_function_1
    32fc:  d085           	addl %d5,%d0                          
    32fe:  3d40 fff8      	movew %d0,%fp@(-8)                    
    3302:  3268 0002      	moveaw %a0@(2),%a1                    
    3306:  2009           	movel %a1,%d0                         
    3308:  e188           	lsll #8,%d0                           
    330a:  222e fff4      	movel %fp@(-12),%d1                   
    330e:  4eba cdfe      	jsr %pc@(0x10e)                       ; anonymous_function_1
    3312:  d0ae fff0      	addl %fp@(-16),%d0                    
    3316:  41ee fff8      	lea %fp@(-8),%a0                      
    331a:  5488           	addql #2,%a0                          
    331c:  2808           	movel %a0,%d4                         
    331e:  3080           	movew %d0,%a0@                        
    3320:  302e fff8      	movew %fp@(-8),%d0                    
    3324:  b06e fffc      	cmpw %fp@(-4),%d0                     
    3328:  6e08           	bgts 0x3332                           ; ↓ label_0
    332a:  303c 0b01      	movew #2817,%d0                       
    332e:  6000 0172      	braw 0x34a2                           ; ↓ label_6

  label_0:
    3332:  2043           	moveal %d3,%a0                        
    3334:  3d50 ffee      	movew %a0@,%fp@(-18)                  
    3338:  2244           	moveal %d4,%a1                        
    333a:  3011           	movew %a1@,%d0                        
    333c:  b050           	cmpw %a0@,%d0                         
    333e:  6e08           	bgts 0x3348                           ; ↓ label_1
    3340:  303c 0b01      	movew #2817,%d0                       
    3344:  6000 015c      	braw 0x34a2                           ; ↓ label_6

  label_1:
    3348:  3a2e fff8      	movew %fp@(-8),%d5                    
    334c:  9a6e fffc      	subw %fp@(-4),%d5                     
    3350:  206e 0014      	moveal %fp@(20),%a0                   
    3354:  3010           	movew %a0@,%d0                        
    3356:  9053           	subw %a3@,%d0                         
    3358:  48c0           	extl %d0                              
    335a:  e188           	lsll #8,%d0                           
    335c:  3205           	movew %d5,%d1                         
    335e:  e249           	lsrw #1,%d1                           
    3360:  7400           	moveq #0,%d2                          
    3362:  3401           	movew %d1,%d2                         
    3364:  d480           	addl %d0,%d2                          
    3366:  7000           	moveq #0,%d0                          
    3368:  3005           	movew %d5,%d0                         
    336a:  2200           	movel %d0,%d1                         
    336c:  2002           	movel %d2,%d0                         
    336e:  4eba cd9e      	jsr %pc@(0x10e)                       ; anonymous_function_1
    3372:  3540 0006      	movew %d0,%a2@(6)                     
    3376:  2043           	moveal %d3,%a0                        
    3378:  2244           	moveal %d4,%a1                        
    337a:  3a11           	movew %a1@,%d5                        
    337c:  9a50           	subw %a0@,%d5                         
    337e:  206e 0014      	moveal %fp@(20),%a0                   
    3382:  3028 0002      	movew %a0@(2),%d0                     
    3386:  906b 0002      	subw %a3@(2),%d0                      
    338a:  48c0           	extl %d0                              
    338c:  e188           	lsll #8,%d0                           
    338e:  3205           	movew %d5,%d1                         
    3390:  e249           	lsrw #1,%d1                           
    3392:  7400           	moveq #0,%d2                          
    3394:  3401           	movew %d1,%d2                         
    3396:  d480           	addl %d0,%d2                          
    3398:  7000           	moveq #0,%d0                          
    339a:  3005           	movew %d5,%d0                         
    339c:  2200           	movel %d0,%d1                         
    339e:  2002           	movel %d2,%d0                         
    33a0:  4eba cd6c      	jsr %pc@(0x10e)                       ; anonymous_function_1
    33a4:  3540 0008      	movew %d0,%a2@(8)                     
    33a8:  3c2a 0006      	movew %a2@(6),%d6                     
    33ac:  3006           	movew %d6,%d0                         
    33ae:  e248           	lsrw #1,%d0                           
    33b0:  3213           	movew %a3@,%d1                        
    33b2:  e149           	lslw #8,%d1                           
    33b4:  d240           	addw %d0,%d1                          
    33b6:  7000           	moveq #0,%d0                          
    33b8:  3001           	movew %d1,%d0                         
    33ba:  80c6           	divuw %d6,%d0                         
    33bc:  322e fffc      	movew %fp@(-4),%d1                    
    33c0:  9240           	subw %d0,%d1                          
    33c2:  3541 0002      	movew %d1,%a2@(2)                     
    33c6:  3e2a 0008      	movew %a2@(8),%d7                     
    33ca:  3007           	movew %d7,%d0                         
    33cc:  e248           	lsrw #1,%d0                           
    33ce:  322b 0002      	movew %a3@(2),%d1                     
    33d2:  e149           	lslw #8,%d1                           
    33d4:  d240           	addw %d0,%d1                          
    33d6:  7000           	moveq #0,%d0                          
    33d8:  3001           	movew %d1,%d0                         
    33da:  80c7           	divuw %d7,%d0                         
    33dc:  322e ffee      	movew %fp@(-18),%d1                   
    33e0:  9240           	subw %d0,%d1                          
    33e2:  3541 0004      	movew %d1,%a2@(4)                     
    33e6:  4227           	clrb %sp@-                            
    33e8:  4e4f           	trap #15                              
    33ea:  a2e0           	0121340                               ; sysTrapPrefOpenPreferenceDB
    33ec:  2808           	movel %a0,%d4                         
    33ee:  544f           	addqw #2,%sp                          
    33f0:  6700 00aa      	beqw 0x349c                           ; ↓ label_5
    33f4:  3f3c 0003      	movew #3,%sp@-                        
    33f8:  2f3c 7073 7973 	movel #1886615923,%sp@-               
    33fe:  4e4f           	trap #15                              
    3400:  a05f           	0120137                               ; sysTrapDmGetResource
    3402:  2608           	movel %a0,%d3                         
    3404:  5c4f           	addqw #6,%sp                          
    3406:  661c           	bnes 0x3424                           ; ↓ label_2
    3408:  4878 0008      	pea 0x8                               
    340c:  3f3c 0003      	movew #3,%sp@-                        
    3410:  2f3c 7073 7973 	movel #1886615923,%sp@-               
    3416:  2f04           	movel %d4,%sp@-                       
    3418:  4e4f           	trap #15                              
    341a:  a06c           	0120154                               ; sysTrapDmNewResource
    341c:  2648           	moveal %a0,%a3                        
    341e:  4fef 000e      	lea %sp@(14),%sp                      
    3422:  600e           	bras 0x3432                           ; ↓ label_3

  label_2:
    3424:  4878 0008      	pea 0x8                               
    3428:  2f03           	movel %d3,%sp@-                       
    342a:  4e4f           	trap #15                              
    342c:  a062           	0120142                               ; sysTrapDmResizeResource
    342e:  2648           	moveal %a0,%a3                        
    3430:  504f           	addqw #8,%sp                          

  label_3:
    3432:  200b           	movel %a3,%d0                         
    3434:  675e           	beqs 0x3494                           ; ↓ label_4
    3436:  2f0b           	movel %a3,%sp@-                       
    3438:  4e4f           	trap #15                              
    343a:  a021           	0120041                               ; sysTrapMemHandleLock
    343c:  2608           	movel %a0,%d3                         
    343e:  4878 0002      	pea 0x2                               
    3442:  486a 0006      	pea %a2@(6)                           
    3446:  42a7           	clrl %sp@-                            
    3448:  2f03           	movel %d3,%sp@-                       
    344a:  4e4f           	trap #15                              
    344c:  a076           	0120166                               ; sysTrapDmWrite
    344e:  4878 0002      	pea 0x2                               
    3452:  486a 0008      	pea %a2@(8)                           
    3456:  4878 0002      	pea 0x2                               
    345a:  2f03           	movel %d3,%sp@-                       
    345c:  4e4f           	trap #15                              
    345e:  a076           	0120166                               ; sysTrapDmWrite
    3460:  4878 0002      	pea 0x2                               
    3464:  486a 0002      	pea %a2@(2)                           
    3468:  4878 0004      	pea 0x4                               
    346c:  2f03           	movel %d3,%sp@-                       
    346e:  4e4f           	trap #15                              
    3470:  a076           	0120166                               ; sysTrapDmWrite
    3472:  4878 0002      	pea 0x2                               
    3476:  486a 0004      	pea %a2@(4)                           
    347a:  4878 0006      	pea 0x6                               
    347e:  2f03           	movel %d3,%sp@-                       
    3480:  4e4f           	trap #15                              
    3482:  a076           	0120166                               ; sysTrapDmWrite
    3484:  2f0b           	movel %a3,%sp@-                       
    3486:  4e4f           	trap #15                              
    3488:  a022           	0120042                               ; sysTrapMemHandleUnlock
    348a:  2f0b           	movel %a3,%sp@-                       
    348c:  4e4f           	trap #15                              
    348e:  a061           	0120141                               ; sysTrapDmReleaseResource
    3490:  4fef 004c      	lea %sp@(76),%sp                      

  label_4:
    3494:  2f04           	movel %d4,%sp@-                       
    3496:  4e4f           	trap #15                              
    3498:  a04a           	0120112                               ; sysTrapDmCloseDatabase
    349a:  584f           	addqw #4,%sp                          

  label_5:
    349c:  14bc 0001      	moveb #1,%a2@                         
    34a0:  7000           	moveq #0,%d0                          

  label_6:
    34a2:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    34a6:  4e5e           	unlk %fp                              
    34a8:  4e75           	rts                                   
```

# PenRawToScreen: 0x34ba - 0x3540

Parameters:

   * `%fp@(8)`      : ???

```
    34ba:  4e56 0000      	linkw %fp,#0                          
    34be:  206e 0008      	moveal %fp@(8),%a0                    
    34c2:  2278 016c      	moveal 0x16c,%a1                      
    34c6:  4a50           	tstw %a0@                             
    34c8:  6f72           	bles 0x353c                           ; ↓ label_3
    34ca:  303c 0100      	movew #256,%d0                        
    34ce:  9050           	subw %a0@,%d0                         
    34d0:  3080           	movew %d0,%a0@                        
    34d2:  303c 0100      	movew #256,%d0                        
    34d6:  9068 0002      	subw %a0@(2),%d0                      
    34da:  3140 0002      	movew %d0,%a0@(2)                     
    34de:  3029 0002      	movew %a1@(2),%d0                     
    34e2:  9150           	subw %d0,%a0@                         
    34e4:  3029 0004      	movew %a1@(4),%d0                     
    34e8:  9168 0002      	subw %d0,%a0@(2)                      
    34ec:  4a11           	tstb %a1@                             
    34ee:  6710           	beqs 0x3500                           ; ↓ label_1
    34f0:  4a50           	tstw %a0@                             
    34f2:  6c02           	bges 0x34f6                           ; ↓ label_0
    34f4:  4250           	clrw %a0@                             

  label_0:
    34f6:  4a68 0002      	tstw %a0@(2)                          
    34fa:  6c04           	bges 0x3500                           ; ↓ label_1
    34fc:  4268 0002      	clrw %a0@(2)                          

  label_1:
    3500:  3010           	movew %a0@,%d0                        
    3502:  c0e9 0006      	muluw %a1@(6),%d0                     
    3506:  0640 0080      	addiw #128,%d0                        
    350a:  e048           	lsrw #8,%d0                           
    350c:  3080           	movew %d0,%a0@                        
    350e:  3028 0002      	movew %a0@(2),%d0                     
    3512:  c0e9 0008      	muluw %a1@(8),%d0                     
    3516:  0640 0080      	addiw #128,%d0                        
    351a:  e048           	lsrw #8,%d0                           
    351c:  3140 0002      	movew %d0,%a0@(2)                     
    3520:  4a11           	tstb %a1@                             
    3522:  6718           	beqs 0x353c                           ; ↓ label_3
    3524:  0c50 00a0      	cmpiw #160,%a0@                       
    3528:  6d04           	blts 0x352e                           ; ↓ label_2
    352a:  30bc 009f      	movew #159,%a0@                       

  label_2:
    352e:  0c68 00e0 0002 	cmpiw #224,%a0@(2)                    
    3534:  6d06           	blts 0x353c                           ; ↓ label_3
    3536:  317c 00df 0002 	movew #223,%a0@(2)                    

  label_3:
    353c:  7000           	moveq #0,%d0                          
    353e:  4e5e           	unlk %fp                              
    3540:  4e75           	rts                                   
```

# PenScreenToRaw: 0x3554 - 0x35bc

Parameters:

   * `%fp@(8)`      : ???

```
    3554:  4e56 0000      	linkw %fp,#0                          
    3558:  2f0b           	movel %a3,%sp@-                       
    355a:  2f0a           	movel %a2,%sp@-                       
    355c:  246e 0008      	moveal %fp@(8),%a2                    
    3560:  2678 016c      	moveal 0x16c,%a3                      
    3564:  4a52           	tstw %a2@                             
    3566:  6f4c           	bles 0x35b4                           ; ↓ label_0
    3568:  3052           	moveaw %a2@,%a0                       
    356a:  2008           	movel %a0,%d0                         
    356c:  e188           	lsll #8,%d0                           
    356e:  7200           	moveq #0,%d1                          
    3570:  322b 0006      	movew %a3@(6),%d1                     
    3574:  4eba cb98      	jsr %pc@(0x10e)                       ; anonymous_function_1
    3578:  7200           	moveq #0,%d1                          
    357a:  322b 0002      	movew %a3@(2),%d1                     
    357e:  d280           	addl %d0,%d1                          
    3580:  3481           	movew %d1,%a2@                        
    3582:  303c 0100      	movew #256,%d0                        
    3586:  9052           	subw %a2@,%d0                         
    3588:  3480           	movew %d0,%a2@                        
    358a:  306a 0002      	moveaw %a2@(2),%a0                    
    358e:  2008           	movel %a0,%d0                         
    3590:  e188           	lsll #8,%d0                           
    3592:  7200           	moveq #0,%d1                          
    3594:  322b 0008      	movew %a3@(8),%d1                     
    3598:  4eba cb74      	jsr %pc@(0x10e)                       ; anonymous_function_1
    359c:  7200           	moveq #0,%d1                          
    359e:  322b 0004      	movew %a3@(4),%d1                     
    35a2:  d280           	addl %d0,%d1                          
    35a4:  3541 0002      	movew %d1,%a2@(2)                     
    35a8:  303c 0100      	movew #256,%d0                        
    35ac:  906a 0002      	subw %a2@(2),%d0                      
    35b0:  3540 0002      	movew %d0,%a2@(2)                     

  label_0:
    35b4:  7000           	moveq #0,%d0                          
    35b6:  245f           	moveal %sp@+,%a2                      
    35b8:  265f           	moveal %sp@+,%a3                      
    35ba:  4e5e           	unlk %fp                              
    35bc:  4e75           	rts                                   
```

# PrvDisableInts: 0x35d0 - 0x35d4

```
    35d0:  007c 0700      	oriw #1792,%sr                        
    35d4:  4e75           	rts                                   
```

# HwrDoze: 0x35e8 - 0x3638

Parameters:

   * `%fp@(8)`      : ???

```
    35e8:  4e56 0000      	linkw %fp,#0                          
    35ec:  48e7 1c00      	moveml %d3-%d5,%sp@-                  
    35f0:  4eba ffde      	jsr %pc@(0x35d0)                      ; PrvDisableInts
    35f4:  4a2e 0008      	tstb %fp@(8)                          
    35f8:  6712           	beqs 0x360c                           ; ↓ label_0
    35fa:  3838 f304      	movew 0xfffffffffffff304,%d4          
    35fe:  3a38 f306      	movew 0xfffffffffffff306,%d5          
    3602:  31fc fff7 f304 	movew #-9,0xfffffffffffff304          
    3608:  4278 f306      	clrw 0xfffffffffffff306               

  label_0:
    360c:  2638 0254      	movel 0x254,%d3                       
    3610:  4eba 06f8      	jsr %pc@(0x3d0a)                      ; PrvShutDownCPU
    3614:  2038 0254      	movel 0x254,%d0                       
    3618:  9083           	subl %d3,%d0                          
    361a:  7229           	moveq #41,%d1                         
    361c:  4eba ca60      	jsr %pc@(0x7e)                        ; anonymous_function_0
    3620:  d1b8 025c      	addl %d0,0x25c                        
    3624:  4a2e 0008      	tstb %fp@(8)                          
    3628:  6708           	beqs 0x3632                           ; ↓ label_1
    362a:  31c4 f304      	movew %d4,0xfffffffffffff304          
    362e:  31c5 f306      	movew %d5,0xfffffffffffff306          

  label_1:
    3632:  4cdf 0038      	moveml %sp@+,%d3-%d5                  
    3636:  4e5e           	unlk %fp                              
    3638:  4e75           	rts                                   
```

# HwrSetSystemClock: 0x3644 - 0x373a

Parameters:

   * `%fp@(8)`      : ???

```
    3644:  4e56 0000      	linkw %fp,#0                          
    3648:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    364c:  246e 0008      	moveal %fp@(8),%a2                    
    3650:  7601           	moveq #1,%d3                          
    3652:  2a12           	movel %a2@,%d5                        
    3654:  2805           	movel %d5,%d4                         
    3656:  4a85           	tstl %d5                              
    3658:  6700 00d4      	beqw 0x372e                           ; ↓ label_11
    365c:  0c84 018d 0000 	cmpil #26017792,%d4                   
    3662:  6414           	bccs 0x3678                           ; ↓ label_2

  label_0:
    3664:  d643           	addw %d3,%d3                          
    3666:  d884           	addl %d4,%d4                          
    3668:  0c43 0010      	cmpiw #16,%d3                         
    366c:  6302           	blss 0x3670                           ; ↓ label_1
    366e:  4e48           	trap #8                               

  label_1:
    3670:  0c84 018d 0000 	cmpil #26017792,%d4                   
    3676:  65ec           	bcss 0x3664                           ; ↑ label_0

  label_2:
    3678:  2c04           	movel %d4,%d6                         
    367a:  0686 0000 4000 	addil #16384,%d6                      
    3680:  720f           	moveq #15,%d1                         
    3682:  e2ae           	lsrl %d1,%d6                          
    3684:  e28e           	lsrl #1,%d6                           
    3686:  72f1           	moveq #-15,%d1                        
    3688:  dc81           	addl %d1,%d6                          
    368a:  2006           	movel %d6,%d0                         
    368c:  720e           	moveq #14,%d1                         
    368e:  4eba ca20      	jsr %pc@(0xb0)                        ; BBHBf6
    3692:  2e00           	movel %d0,%d7                         
    3694:  2a00           	movel %d0,%d5                         
    3696:  2007           	movel %d7,%d0                         
    3698:  720e           	moveq #14,%d1                         
    369a:  4eba c9e2      	jsr %pc@(0x7e)                        ; anonymous_function_0
    369e:  2206           	movel %d6,%d1                         
    36a0:  9280           	subl %d0,%d1                          
    36a2:  2641           	moveal %d1,%a3                        
    36a4:  2801           	movel %d1,%d4                         
    36a6:  7001           	moveq #1,%d0                          
    36a8:  b7c0           	cmpal %d0,%a3                         
    36aa:  6202           	bhis 0x36ae                           ; ↓ label_3
    36ac:  7801           	moveq #1,%d4                          

  label_3:
    36ae:  700e           	moveq #14,%d0                         
    36b0:  b880           	cmpl %d0,%d4                          
    36b2:  6302           	blss 0x36b6                           ; ↓ label_4
    36b4:  780e           	moveq #14,%d4                         

  label_4:
    36b6:  2005           	movel %d5,%d0                         
    36b8:  5380           	subql #1,%d0                          
    36ba:  b880           	cmpl %d0,%d4                          
    36bc:  6304           	blss 0x36c2                           ; ↓ label_5
    36be:  2805           	movel %d5,%d4                         
    36c0:  5384           	subql #1,%d4                          

  label_5:
    36c2:  2005           	movel %d5,%d0                         
    36c4:  720e           	moveq #14,%d1                         
    36c6:  4eba c9b6      	jsr %pc@(0x7e)                        ; anonymous_function_0
    36ca:  d084           	addl %d4,%d0                          
    36cc:  720f           	moveq #15,%d1                         
    36ce:  d081           	addl %d1,%d0                          
    36d0:  4840           	swap %d0                              
    36d2:  4240           	clrw %d0                              
    36d4:  2c00           	movel %d0,%d6                         
    36d6:  3f04           	movew %d4,%sp@-                       
    36d8:  3f05           	movew %d5,%sp@-                       
    36da:  4eba 0662      	jsr %pc@(0x3d3e)                      ; PrvSetPLL
    36de:  3838 f200      	movew 0xfffffffffffff200,%d4          
    36e2:  0244 00ff      	andiw #255,%d4                        
    36e6:  584f           	addqw #4,%sp                          
    36e8:  3003           	movew %d3,%d0                         
    36ea:  5340           	subqw #1,%d0                          
    36ec:  6712           	beqs 0x3700                           ; ↓ label_6
    36ee:  5340           	subqw #1,%d0                          
    36f0:  6724           	beqs 0x3716                           ; ↓ label_10
    36f2:  5540           	subqw #2,%d0                          
    36f4:  6710           	beqs 0x3706                           ; ↓ label_7
    36f6:  5940           	subqw #4,%d0                          
    36f8:  6712           	beqs 0x370c                           ; ↓ label_8
    36fa:  5140           	subqw #8,%d0                          
    36fc:  6714           	beqs 0x3712                           ; ↓ label_9
    36fe:  6016           	bras 0x3716                           ; ↓ label_10

  label_6:
    3700:  0044 2400      	oriw #9216,%d4                        
    3704:  6010           	bras 0x3716                           ; ↓ label_10

  label_7:
    3706:  0044 0900      	oriw #2304,%d4                        
    370a:  600a           	bras 0x3716                           ; ↓ label_10

  label_8:
    370c:  0044 1200      	oriw #4608,%d4                        
    3710:  6004           	bras 0x3716                           ; ↓ label_10

  label_9:
    3712:  0044 1b00      	oriw #6912,%d4                        

  label_10:
    3716:  31c4 f200      	movew %d4,0xfffffffffffff200          
    371a:  7000           	moveq #0,%d0                          
    371c:  3003           	movew %d3,%d0                         
    371e:  2200           	movel %d0,%d1                         
    3720:  2006           	movel %d6,%d0                         
    3722:  4eba c98c      	jsr %pc@(0xb0)                        ; BBHBf6
    3726:  2480           	movel %d0,%a2@                        
    3728:  21d2 012e      	movel %a2@,0x12e                      
    372c:  6004           	bras 0x3732                           ; ↓ label_12

  label_11:
    372e:  24b8 012e      	movel 0x12e,%a2@                      

  label_12:
    3732:  7000           	moveq #0,%d0                          
    3734:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    3738:  4e5e           	unlk %fp                              
    373a:  4e75           	rts                                   
```

# HwrSetCPUDutyCycle: 0x3750 - 0x3780

Parameters:

   * `%fp@(8)`      : ???

```
    3750:  4e56 0000      	linkw %fp,#0                          
    3754:  206e 0008      	moveal %fp@(8),%a0                    
    3758:  3210           	movew %a0@,%d1                        
    375a:  6710           	beqs 0x376c                           ; ↓ label_0
    375c:  1001           	moveb %d1,%d0                         
    375e:  0000 0080      	orib #-128,%d0                        
    3762:  11c0 f207      	moveb %d0,0xfffffffffffff207          
    3766:  11e8 0001 0252 	moveb %a0@(1),0x252                   

  label_0:
    376c:  1038 f207      	moveb 0xfffffffffffff207,%d0          
    3770:  0240 001f      	andiw #31,%d0                         
    3774:  3080           	movew %d0,%a0@                        
    3776:  6604           	bnes 0x377c                           ; ↓ label_1
    3778:  30bc 001f      	movew #31,%a0@                        

  label_1:
    377c:  7000           	moveq #0,%d0                          
    377e:  4e5e           	unlk %fp                              
    3780:  4e75           	rts                                   
```

# HwrGetRAMMapping: 0x3798 - 0x3824

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???

```
    3798:  4e56 fffa      	linkw %fp,#-6                         
    379c:  48e7 0710      	moveml %d5-%d7/%a3,%sp@-              
    37a0:  242e 0010      	movel %fp@(16),%d2                    
    37a4:  226e 0014      	moveal %fp@(20),%a1                   
    37a8:  2e2e 0008      	movel %fp@(8),%d7                     
    37ac:  206e 000c      	moveal %fp@(12),%a0                   
    37b0:  3d50 fffe      	movew %a0@,%fp@(-2)                   
    37b4:  7200           	moveq #0,%d1                          
    37b6:  4250           	clrw %a0@                             
    37b8:  2047           	moveal %d7,%a0                        
    37ba:  2c28 001c      	movel %a0@(28),%d6                    

  label_0:
    37be:  4a81           	tstl %d1                              
    37c0:  6622           	bnes 0x37e4                           ; ↓ label_2
    37c2:  4a6e fffe      	tstw %fp@(-2)                         
    37c6:  6756           	beqs 0x381e                           ; ↓ label_5
    37c8:  2042           	moveal %d2,%a0                        
    37ca:  2a10           	movel %a0@,%d5                        
    37cc:  2668 0004      	moveal %a0@(4),%a3                    
    37d0:  220b           	movel %a3,%d1                         
    37d2:  70ff           	moveq #-1,%d0                         
    37d4:  b7c0           	cmpal %d0,%a3                         
    37d6:  6606           	bnes 0x37de                           ; ↓ label_1
    37d8:  2047           	moveal %d7,%a0                        
    37da:  2228 0008      	movel %a0@(8),%d1                     

  label_1:
    37de:  5082           	addql #8,%d2                          
    37e0:  536e fffe      	subqw #1,%fp@(-2)                     

  label_2:
    37e4:  2647           	moveal %d7,%a3                        
    37e6:  2653           	moveal %a3@,%a3                       
    37e8:  d7c5           	addal %d5,%a3                         
    37ea:  2d4b fffa      	movel %a3,%fp@(-6)                    
    37ee:  bc8b           	cmpl %a3,%d6                          
    37f0:  6314           	blss 0x3806                           ; ↓ label_3
    37f2:  204b           	moveal %a3,%a0                        
    37f4:  d1c1           	addal %d1,%a0                         
    37f6:  b1c6           	cmpal %d6,%a0                         
    37f8:  630c           	blss 0x3806                           ; ↓ label_3
    37fa:  228b           	movel %a3,%a1@                        
    37fc:  2046           	moveal %d6,%a0                        
    37fe:  91cb           	subal %a3,%a0                         
    3800:  2348 0004      	movel %a0,%a1@(4)                     
    3804:  6006           	bras 0x380c                           ; ↓ label_4

  label_3:
    3806:  228b           	movel %a3,%a1@                        
    3808:  2341 0004      	movel %d1,%a1@(4)                     

  label_4:
    380c:  2669 0004      	moveal %a1@(4),%a3                    
    3810:  928b           	subl %a3,%d1                          
    3812:  da8b           	addl %a3,%d5                          
    3814:  5089           	addql #8,%a1                          
    3816:  206e 000c      	moveal %fp@(12),%a0                   
    381a:  5250           	addqw #1,%a0@                         
    381c:  60a0           	bras 0x37be                           ; ↑ label_0

  label_5:
    381e:  4cdf 08e0      	moveml %sp@+,%d5-%d7/%a3              
    3822:  4e5e           	unlk %fp                              
    3824:  4e75           	rts                                   
```

# HwrGetSilkscreenID: 0x383a - 0x3844

```
    383a:  4e56 0000      	linkw %fp,#0                          
    383e:  303c 4a38      	movew #19000,%d0                      
    3842:  4e5e           	unlk %fp                              
    3844:  4e75           	rts                                   
```

# PrvExceptionHandler: 0x385c - 0x3880

```
    385c:  4e56 0000      	linkw %fp,#0                          
    3860:  4a38 0101      	tstb 0x101                            
    3864:  6714           	beqs 0x387a                           ; ↓ label_0
    3866:  3038 f200      	movew 0xfffffffffffff200,%d0          
    386a:  0240 f8ff      	andiw #-1793,%d0                      
    386e:  0040 0400      	oriw #1024,%d0                        
    3872:  31c0 f200      	movew %d0,0xfffffffffffff200          
    3876:  4e48           	trap #8                               
    3878:  6004           	bras 0x387e                           ; ↓ label_1

  label_0:
    387a:  4e4f           	trap #15                              
    387c:  a08c           	0120214                               ; sysTrapSysReset

  label_1:
    387e:  4e5e           	unlk %fp                              
    3880:  4e75           	rts                                   
```

# HwrSleep: 0x3898 - 0x3b42

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-16)`    : ???
   * `%fp@(-20)`    : ???
   * `%fp@(-22)`    : ???
   * `%fp@(-26)`    : ???
   * `%fp@(-30)`    : ???
   * `%fp@(-34)`    : ???
   * `%fp@(-62)`    : ???
   * `%fp@(-64)`    : ???

```
    3898:  4e56 ffc0      	linkw %fp,#-64                        
    389c:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    38a0:  1a2e 0008      	moveb %fp@(8),%d5                     
    38a4:  182e 000a      	moveb %fp@(10),%d4                    
    38a8:  247c ffff f000 	moveal #-4096,%a2                     
    38ae:  2c38 0304      	movel 0x304,%d6                       
    38b2:  3038 f304      	movew 0xfffffffffffff304,%d0          
    38b6:  4640           	notw %d0                              
    38b8:  0240 0008      	andiw #8,%d0                          
    38bc:  3d40 ffc0      	movew %d0,%fp@(-64)                   
    38c0:  0078 0008 f304 	oriw #8,0xfffffffffffff304            

  label_0:
    38c6:  0078 0001 0250 	oriw #1,0x250                         
    38cc:  4e4f           	trap #15                              
    38ce:  a0a5           	0120245                               ; sysTrapSysDisableInts
    38d0:  3d40 ffea      	movew %d0,%fp@(-22)                   
    38d4:  4e4f           	trap #15                              
    38d6:  a12c           	0120454                               ; sysTrapEvtFlushKeyQueue
    38d8:  2d78 0008 ffe6 	movel 0x8,%fp@(-26)                   
    38de:  2d78 000c ffe2 	movel 0xc,%fp@(-30)                   
    38e4:  2d78 0010 ffde 	movel 0x10,%fp@(-34)                  
    38ea:  41fa ff70      	lea %pc@(0x385c),%a0                  ; PrvExceptionHandler
    38ee:  21c8 0008      	movel %a0,0x8                         
    38f2:  41fa ff68      	lea %pc@(0x385c),%a0                  ; PrvExceptionHandler
    38f6:  21c8 000c      	movel %a0,0xc                         
    38fa:  41fa ff60      	lea %pc@(0x385c),%a0                  ; PrvExceptionHandler
    38fe:  21c8 0010      	movel %a0,0x10                        
    3902:  4227           	clrb %sp@-                            
    3904:  4227           	clrb %sp@-                            
    3906:  4e4f           	trap #15                              
    3908:  a2ea           	0121352                               ; sysTrapHwrBacklightV33
    390a:  4a00           	tstb %d0                              
    390c:  584f           	addqw #4,%sp                          
    390e:  6718           	beqs 0x3928                           ; ↓ label_1
    3910:  4227           	clrb %sp@-                            
    3912:  1f3c 0001      	moveb #1,%sp@-                        
    3916:  4e4f           	trap #15                              
    3918:  a2ea           	0121352                               ; sysTrapHwrBacklightV33
    391a:  4a86           	tstl %d6                              
    391c:  584f           	addqw #4,%sp                          
    391e:  6708           	beqs 0x3928                           ; ↓ label_1
    3920:  2046           	moveal %d6,%a0                        
    3922:  70fb           	moveq #-5,%d0                         
    3924:  c1a8 000c      	andl %d0,%a0@(12)                     

  label_1:
    3928:  3038 0250      	movew 0x250,%d0                       
    392c:  0240 0010      	andiw #16,%d0                         
    3930:  660a           	bnes 0x393c                           ; ↓ label_2
    3932:  1f04           	moveb %d4,%sp@-                       
    3934:  1f05           	moveb %d5,%sp@-                       
    3936:  4e4f           	trap #15                              
    3938:  a245           	0121105                               ; sysTrapHwrDisplaySleep
    393a:  584f           	addqw #4,%sp                          

  label_2:
    393c:  1f04           	moveb %d4,%sp@-                       
    393e:  1f05           	moveb %d5,%sp@-                       
    3940:  4e4f           	trap #15                              
    3942:  a2a5           	0121245                               ; sysTrapKeySleep
    3944:  7600           	moveq #0,%d3                          
    3946:  4a78 0144      	tstw 0x144                            
    394a:  584f           	addqw #4,%sp                          
    394c:  631e           	blss 0x396c                           ; ↓ label_5

  label_3:
    394e:  3f03           	movew %d3,%sp@-                       
    3950:  4e4f           	trap #15                              
    3952:  a0b9           	0120271                               ; sysTrapSysLibTblEntry
    3954:  2648           	moveal %a0,%a3                        
    3956:  4a93           	tstl %a3@                             
    3958:  544f           	addqw #2,%sp                          
    395a:  6708           	beqs 0x3964                           ; ↓ label_4
    395c:  3f03           	movew %d3,%sp@-                       
    395e:  4e4f           	trap #15                              
    3960:  a803           	0124003                               ; ??? 0xa803
    3962:  544f           	addqw #2,%sp                          

  label_4:
    3964:  5243           	addqw #1,%d3                          
    3966:  b678 0144      	cmpw 0x144,%d3                        
    396a:  65e2           	bcss 0x394e                           ; ↑ label_3

  label_5:
    396c:  7404           	moveq #4,%d2                          
    396e:  4e4f           	trap #15                              
    3970:  a367           	0121547                               ; sysTrapSerialDispatch
    3972:  3f3c 0001      	movew #1,%sp@-                        
    3976:  4e4f           	trap #15                              
    3978:  a24e           	0121116                               ; sysTrapHwrTimerSleep
    397a:  1f04           	moveb %d4,%sp@-                       
    397c:  1f05           	moveb %d5,%sp@-                       
    397e:  4e4f           	trap #15                              
    3980:  a0ff           	0120377                               ; sysTrapTimSleep
    3982:  302a 0500      	movew %a2@(1280),%d0                  
    3986:  0240 0010      	andiw #16,%d0                         
    398a:  5c4f           	addqw #6,%sp                          
    398c:  6706           	beqs 0x3994                           ; ↓ label_6
    398e:  0078 0002 0250 	oriw #2,0x250                         

  label_6:
    3994:  026a ffaf 0500 	andiw #-81,%a2@(1280)                 
    399a:  006a 0080 0306 	oriw #128,%a2@(774)                   
    39a0:  4e4f           	trap #15                              
    39a2:  a275           	0121165                               ; sysTrapPenSleep
    39a4:  002a 0020 0420 	orib #32,%a2@(1056)                   
    39aa:  022a 00df 0421 	andib #-33,%a2@(1057)                 
    39b0:  002a 0020 0423 	orib #32,%a2@(1059)                   
    39b6:  002a 0005 0420 	orib #5,%a2@(1056)                    
    39bc:  002a 0005 0421 	orib #5,%a2@(1057)                    
    39c2:  002a 0005 0423 	orib #5,%a2@(1059)                    
    39c8:  4a04           	tstb %d4                              
    39ca:  670e           	beqs 0x39da                           ; ↓ label_7
    39cc:  102a 0431      	moveb %a2@(1073),%d0                  
    39d0:  0000 0020      	orib #32,%d0                          
    39d4:  1540 0431      	moveb %d0,%a2@(1073)                  
    39d8:  6028           	bras 0x3a02                           ; ↓ label_9

  label_7:
    39da:  2678 0304      	moveal 0x304,%a3                      
    39de:  4ab8 0304      	tstl 0x304                            
    39e2:  671e           	beqs 0x3a02                           ; ↓ label_9
    39e4:  262b 0054      	movel %a3@(84),%d3                    
    39e8:  6704           	beqs 0x39ee                           ; ↓ label_8
    39ea:  2043           	moveal %d3,%a0                        
    39ec:  4e90           	jsr %a0@                              

  label_8:
    39ee:  7020           	moveq #32,%d0                         
    39f0:  c0ab 000c      	andl %a3@(12),%d0                     
    39f4:  670c           	beqs 0x3a02                           ; ↓ label_9
    39f6:  102a 0431      	moveb %a2@(1073),%d0                  
    39fa:  0000 0020      	orib #32,%d0                          
    39fe:  1540 0431      	moveb %d0,%a2@(1073)                  

  label_9:
    3a02:  41ee ffc2      	lea %fp@(-62),%a0                     
    3a06:  2648           	moveal %a0,%a3                        
    3a08:  20b8 0064      	movel 0x64,%a0@                       
    3a0c:  41fa 0142      	lea %pc@(0x3b50),%a0                  ; ??? 0x3b50
    3a10:  21c8 0064      	movel %a0,0x64                        
    3a14:  204b           	moveal %a3,%a0                        
    3a16:  5888           	addql #4,%a0                          
    3a18:  2e08           	movel %a0,%d7                         
    3a1a:  20b8 0068      	movel 0x68,%a0@                       
    3a1e:  41fa 0130      	lea %pc@(0x3b50),%a0                  ; ??? 0x3b50
    3a22:  21c8 0068      	movel %a0,0x68                        
    3a26:  204b           	moveal %a3,%a0                        
    3a28:  5088           	addql #8,%a0                          
    3a2a:  2d48 fffc      	movel %a0,%fp@(-4)                    
    3a2e:  20b8 006c      	movel 0x6c,%a0@                       
    3a32:  41fa 011c      	lea %pc@(0x3b50),%a0                  ; ??? 0x3b50
    3a36:  21c8 006c      	movel %a0,0x6c                        
    3a3a:  204b           	moveal %a3,%a0                        
    3a3c:  5088           	addql #8,%a0                          
    3a3e:  5888           	addql #4,%a0                          
    3a40:  2d48 fff8      	movel %a0,%fp@(-8)                    
    3a44:  20b8 0070      	movel 0x70,%a0@                       
    3a48:  41fa 0106      	lea %pc@(0x3b50),%a0                  ; ??? 0x3b50
    3a4c:  21c8 0070      	movel %a0,0x70                        
    3a50:  204b           	moveal %a3,%a0                        
    3a52:  5088           	addql #8,%a0                          
    3a54:  5088           	addql #8,%a0                          
    3a56:  2d48 fff4      	movel %a0,%fp@(-12)                   
    3a5a:  20b8 0074      	movel 0x74,%a0@                       
    3a5e:  41fa 00f0      	lea %pc@(0x3b50),%a0                  ; ??? 0x3b50
    3a62:  21c8 0074      	movel %a0,0x74                        
    3a66:  204b           	moveal %a3,%a0                        
    3a68:  41e8 0014      	lea %a0@(20),%a0                      
    3a6c:  2d48 fff0      	movel %a0,%fp@(-16)                   
    3a70:  20b8 0078      	movel 0x78,%a0@                       
    3a74:  41fa 00da      	lea %pc@(0x3b50),%a0                  ; ??? 0x3b50
    3a78:  21c8 0078      	movel %a0,0x78                        
    3a7c:  204b           	moveal %a3,%a0                        
    3a7e:  41e8 0018      	lea %a0@(24),%a0                      
    3a82:  2d48 ffec      	movel %a0,%fp@(-20)                   
    3a86:  20b8 007c      	movel 0x7c,%a0@                       
    3a8a:  41fa 00c4      	lea %pc@(0x3b50),%a0                  ; ??? 0x3b50
    3a8e:  21c8 007c      	movel %a0,0x7c                        

  label_10:
    3a92:  4227           	clrb %sp@-                            
    3a94:  4eba 0388      	jsr %pc@(0x3e1e)                      ; PrvSetVoltageDividerEnabled
    3a98:  362a 0200      	movew %a2@(512),%d3                   
    3a9c:  3003           	movew %d3,%d0                         
    3a9e:  0240 f8ff      	andiw #-1793,%d0                      
    3aa2:  0040 0300      	oriw #768,%d0                         
    3aa6:  3540 0200      	movew %d0,%a2@(512)                   
    3aaa:  4eba dfee      	jsr %pc@(0x1a9a)                      ; anonymous_function_2
    3aae:  3543 0200      	movew %d3,%a2@(512)                   
    3ab2:  1f3c 0001      	moveb #1,%sp@-                        
    3ab6:  4eba 0366      	jsr %pc@(0x3e1e)                      ; PrvSetVoltageDividerEnabled
    3aba:  4a40           	tstw %d0                              
    3abc:  584f           	addqw #4,%sp                          
    3abe:  660a           	bnes 0x3aca                           ; ↓ label_11
    3ac0:  4878 00b4      	pea 0xb4                              
    3ac4:  4eba e030      	jsr %pc@(0x1af6)                      ; prvDelay
    3ac8:  584f           	addqw #4,%sp                          

  label_11:
    3aca:  4e4f           	trap #15                              
    3acc:  a241           	0121101                               ; sysTrapHwrWake
    3ace:  3600           	movew %d0,%d3                         
    3ad0:  66c0           	bnes 0x3a92                           ; ↑ label_10
    3ad2:  21d3 0064      	movel %a3@,0x64                       
    3ad6:  2047           	moveal %d7,%a0                        
    3ad8:  21d0 0068      	movel %a0@,0x68                       
    3adc:  206e fffc      	moveal %fp@(-4),%a0                   
    3ae0:  21d0 006c      	movel %a0@,0x6c                       
    3ae4:  206e fff8      	moveal %fp@(-8),%a0                   
    3ae8:  21d0 0070      	movel %a0@,0x70                       
    3aec:  206e fff4      	moveal %fp@(-12),%a0                  
    3af0:  21d0 0074      	movel %a0@,0x74                       
    3af4:  206e fff0      	moveal %fp@(-16),%a0                  
    3af8:  21d0 0078      	movel %a0@,0x78                       
    3afc:  206e ffec      	moveal %fp@(-20),%a0                  
    3b00:  21d0 007c      	movel %a0@,0x7c                       
    3b04:  21ee ffe6 0008 	movel %fp@(-26),0x8                   
    3b0a:  21ee ffe2 000c 	movel %fp@(-30),0xc                   
    3b10:  21ee ffde 0010 	movel %fp@(-34),0x10                  
    3b16:  3f2e ffea      	movew %fp@(-22),%sp@-                 
    3b1a:  4e4f           	trap #15                              
    3b1c:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    3b1e:  302a 030c      	movew %a2@(780),%d0                   
    3b22:  0240 0008      	andiw #8,%d0                          
    3b26:  544f           	addqw #2,%sp                          
    3b28:  6600 fd9c      	bnew 0x38c6                           ; ↑ label_0
    3b2c:  31fc 0008 f30c 	movew #8,0xfffffffffffff30c           
    3b32:  302e ffc0      	movew %fp@(-64),%d0                   
    3b36:  4640           	notw %d0                              
    3b38:  c178 f304      	andw %d0,0xfffffffffffff304           
    3b3c:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    3b40:  4e5e           	unlk %fp                              
    3b42:  4e75           	rts                                   
```

# HwrWake: 0x3b70 - 0x3cfe

```
    3b70:  4e56 0000      	linkw %fp,#0                          
    3b74:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    3b78:  0278 bfef fc02 	andiw #-16401,0xfffffffffffffc02      
    3b7e:  0078 0001 fc02 	oriw #1,0xfffffffffffffc02            
    3b84:  31fc 0008 f30c 	movew #8,0xfffffffffffff30c           
    3b8a:  0c38 0001 02ca 	cmpib #1,0x2ca                        
    3b90:  6634           	bnes 0x3bc6                           ; ↓ label_1
    3b92:  1638 f423      	moveb 0xfffffffffffff423,%d3          
    3b96:  0238 00db f423 	andib #-37,0xfffffffffffff423         
    3b9c:  4e4f           	trap #15                              
    3b9e:  a248           	0121110                               ; sysTrapHwrBatteryLevel
    3ba0:  1800           	moveb %d0,%d4                         
    3ba2:  b838 02cb      	cmpb 0x2cb,%d4                        
    3ba6:  641a           	bccs 0x3bc2                           ; ↓ label_0
    3ba8:  4e4f           	trap #15                              
    3baa:  a34d           	0121515                               ; sysTrapHwrDockStatus
    3bac:  0240 0004      	andiw #4,%d0                          
    3bb0:  6610           	bnes 0x3bc2                           ; ↓ label_0
    3bb2:  11c3 f423      	moveb %d3,0xfffffffffffff423          
    3bb6:  4e4f           	trap #15                              
    3bb8:  a12c           	0120454                               ; sysTrapEvtFlushKeyQueue
    3bba:  11fc 0001 02ca 	moveb #1,0x2ca                        
    3bc0:  6004           	bras 0x3bc6                           ; ↓ label_1

  label_0:
    3bc2:  4238 02ca      	clrb 0x2ca                            

  label_1:
    3bc6:  0c38 0001 02ca 	cmpib #1,0x2ca                        
    3bcc:  670a           	beqs 0x3bd8                           ; ↓ label_2
    3bce:  3038 f310      	movew 0xfffffffffffff310,%d0          
    3bd2:  0240 0008      	andiw #8,%d0                          
    3bd6:  671e           	beqs 0x3bf6                           ; ↓ label_3

  label_2:
    3bd8:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    3bdc:  0200 fff0      	andib #-16,%d0                        
    3be0:  0000 0007      	orib #7,%d0                           
    3be4:  11c0 f419      	moveb %d0,0xfffffffffffff419          
    3be8:  0078 f8bf f306 	oriw #-1857,0xfffffffffffff306        
    3bee:  303c ffff      	movew #-1,%d0                         
    3bf2:  6000 0104      	braw 0x3cf8                           ; ↓ label_13

  label_3:
    3bf6:  0278 fff3 0250 	andiw #-13,0x250                      
    3bfc:  3038 f30e      	movew 0xfffffffffffff30e,%d0          
    3c00:  0240 0740      	andiw #1856,%d0                       
    3c04:  6706           	beqs 0x3c0c                           ; ↓ label_4
    3c06:  0078 0004 0250 	oriw #4,0x250                         

  label_4:
    3c0c:  4a78 f30c      	tstw 0xfffffffffffff30c               
    3c10:  6628           	bnes 0x3c3a                           ; ↓ label_5
    3c12:  0c78 0010 f30e 	cmpiw #16,0xfffffffffffff30e          
    3c18:  6620           	bnes 0x3c3a                           ; ↓ label_5
    3c1a:  0078 0008 0250 	oriw #8,0x250                         
    3c20:  4227           	clrb %sp@-                            
    3c22:  4e4f           	trap #15                              
    3c24:  a0fb           	0120373                               ; sysTrapTimHandleInterrupt
    3c26:  3038 0250      	movew 0x250,%d0                       
    3c2a:  0240 0008      	andiw #8,%d0                          
    3c2e:  544f           	addqw #2,%sp                          
    3c30:  6708           	beqs 0x3c3a                           ; ↓ label_5
    3c32:  303c ffff      	movew #-1,%d0                         
    3c36:  6000 00c0      	braw 0x3cf8                           ; ↓ label_13

  label_5:
    3c3a:  3038 0250      	movew 0x250,%d0                       
    3c3e:  0240 0004      	andiw #4,%d0                          
    3c42:  6728           	beqs 0x3c6c                           ; ↓ label_7
    3c44:  4eba 3848      	jsr %pc@(0x748e)                      ; KeyCheckWakeUp
    3c48:  4a40           	tstw %d0                              
    3c4a:  670e           	beqs 0x3c5a                           ; ↓ label_6
    3c4c:  0278 fffb 0250 	andiw #-5,0x250                       
    3c52:  303c ffff      	movew #-1,%d0                         
    3c56:  6000 00a0      	braw 0x3cf8                           ; ↓ label_13

  label_6:
    3c5a:  1038 f421      	moveb 0xfffffffffffff421,%d0          
    3c5e:  0240 0008      	andiw #8,%d0                          
    3c62:  6608           	bnes 0x3c6c                           ; ↓ label_7
    3c64:  303c ffff      	movew #-1,%d0                         
    3c68:  6000 008e      	braw 0x3cf8                           ; ↓ label_13

  label_7:
    3c6c:  2478 0304      	moveal 0x304,%a2                      
    3c70:  4ab8 0304      	tstl 0x304                            
    3c74:  670a           	beqs 0x3c80                           ; ↓ label_8
    3c76:  266a 0058      	moveal %a2@(88),%a3                   
    3c7a:  200b           	movel %a3,%d0                         
    3c7c:  6702           	beqs 0x3c80                           ; ↓ label_8
    3c7e:  4e93           	jsr %a3@                              

  label_8:
    3c80:  1038 f431      	moveb 0xfffffffffffff431,%d0          
    3c84:  0200 ffdf      	andib #-33,%d0                        
    3c88:  11c0 f431      	moveb %d0,0xfffffffffffff431          
    3c8c:  0238 00da f423 	andib #-38,0xfffffffffffff423         
    3c92:  3038 0250      	movew 0x250,%d0                       
    3c96:  0240 0002      	andiw #2,%d0                          
    3c9a:  6706           	beqs 0x3ca2                           ; ↓ label_9
    3c9c:  0278 fffd 0250 	andiw #-3,0x250                       

  label_9:
    3ca2:  4227           	clrb %sp@-                            
    3ca4:  4e4f           	trap #15                              
    3ca6:  a373           	0121563                               ; sysTrapHwrDisplayDoze
    3ca8:  3f3c 0001      	movew #1,%sp@-                        
    3cac:  4e4f           	trap #15                              
    3cae:  a24f           	0121117                               ; sysTrapHwrTimerWake
    3cb0:  4e4f           	trap #15                              
    3cb2:  a100           	0120400                               ; sysTrapTimWake
    3cb4:  4e4f           	trap #15                              
    3cb6:  a2a6           	0121246                               ; sysTrapKeyWake
    3cb8:  4e4f           	trap #15                              
    3cba:  a276           	0121166                               ; sysTrapPenWake
    3cbc:  7405           	moveq #5,%d2                          
    3cbe:  4e4f           	trap #15                              
    3cc0:  a367           	0121547                               ; sysTrapSerialDispatch
    3cc2:  7600           	moveq #0,%d3                          
    3cc4:  4a78 0144      	tstw 0x144                            
    3cc8:  584f           	addqw #4,%sp                          
    3cca:  631e           	blss 0x3cea                           ; ↓ label_12

  label_10:
    3ccc:  3f03           	movew %d3,%sp@-                       
    3cce:  4e4f           	trap #15                              
    3cd0:  a0b9           	0120271                               ; sysTrapSysLibTblEntry
    3cd2:  2448           	moveal %a0,%a2                        
    3cd4:  4a92           	tstl %a2@                             
    3cd6:  544f           	addqw #2,%sp                          
    3cd8:  6708           	beqs 0x3ce2                           ; ↓ label_11
    3cda:  3f03           	movew %d3,%sp@-                       
    3cdc:  4e4f           	trap #15                              
    3cde:  a804           	0124004                               ; ??? 0xa804
    3ce0:  544f           	addqw #2,%sp                          

  label_11:
    3ce2:  5243           	addqw #1,%d3                          
    3ce4:  b678 0144      	cmpw 0x144,%d3                        
    3ce8:  65e2           	bcss 0x3ccc                           ; ↑ label_10

  label_12:
    3cea:  0278 fffe 0250 	andiw #-2,0x250                       
    3cf0:  31fc 0032 0136 	movew #50,0x136                       
    3cf6:  7000           	moveq #0,%d0                          

  label_13:
    3cf8:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    3cfc:  4e5e           	unlk %fp                              
    3cfe:  4e75           	rts                                   
```

# PrvShutDownCPU: 0x3d0a - 0x3d2a

```
    3d0a:  207c ffff f000 	moveal #-4096,%a0                     
    3d10:  027c 7fff      	andiw #32767,%sr                      
    3d14:  4e71           	nop                                   
    3d16:  117c 00c0 0207 	moveb #-64,%a0@(519)                  
    3d1c:  4e72 2000      	stop #8192                            
    3d20:  4a38 0111      	tstb 0x111                            
    3d24:  6704           	beqs 0x3d2a                           ; ↓ label_0
    3d26:  007c 8000      	oriw #-32768,%sr                      

  label_0:
    3d2a:  4e75           	rts                                   
```

# PrvSetPLL: 0x3d3e - 0x3d6a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    3d3e:  4e56 0000      	linkw %fp,#0                          
    3d42:  207c ffff f000 	moveal #-4096,%a0                     
    3d48:  322e 000a      	movew %fp@(10),%d1                    
    3d4c:  e149           	lslw #8,%d1                           
    3d4e:  826e 0008      	orw %fp@(8),%d1                       

  label_0:
    3d52:  3028 0202      	movew %a0@(514),%d0                   
    3d56:  6afa           	bpls 0x3d52                           ; ↑ label_0
    3d58:  3141 0202      	movew %d1,%a0@(514)                   

  label_1:
    3d5c:  3028 0202      	movew %a0@(514),%d0                   
    3d60:  6bfa           	bmis 0x3d5c                           ; ↑ label_1

  label_2:
    3d62:  3028 0202      	movew %a0@(514),%d0                   
    3d66:  6afa           	bpls 0x3d62                           ; ↑ label_2
    3d68:  4e5e           	unlk %fp                              
    3d6a:  4e75           	rts                                   
```

# HwrPostDebugInit: 0x3d78 - 0x3dae

```
    3d78:  4e56 0000      	linkw %fp,#0                          
    3d7c:  2f0a           	movel %a2,%sp@-                       
    3d7e:  247c 0000 2800 	moveal #10240,%a2                     
    3d84:  3f3c 0008      	movew #8,%sp@-                        
    3d88:  2f3c 00a0 00a0 	movel #10485920,%sp@-                 
    3d8e:  2f0a           	movel %a2,%sp@-                       
    3d90:  3f3c 000d      	movew #13,%sp@-                       
    3d94:  4e4f           	trap #15                              
    3d96:  a244           	0121104                               ; sysTrapHwrDisplayInit
    3d98:  41fa 2118      	lea %pc@(0x5eb2),%a0                  ; ??? 0x5eb2
    3d9c:  21c8 0078      	movel %a0,0x78                        
    3da0:  0278 fff7 f304 	andiw #-9,0xfffffffffffff304          
    3da6:  4fef 000c      	lea %sp@(12),%sp                      
    3daa:  245f           	moveal %sp@+,%a2                      
    3dac:  4e5e           	unlk %fp                              
    3dae:  4e75           	rts                                   
```

# HwrIdentifyFeatures: 0x3dc4 - 0x3de6

```
    3dc4:  4e56 0000      	linkw %fp,#0                          
    3dc8:  42b8 02cc      	clrl 0x2cc                            
    3dcc:  4278 028e      	clrw 0x28e                            
    3dd0:  0078 080b 028e 	oriw #2059,0x28e                      
    3dd6:  00b8 0000 0710 	oril #1808,0x2cc                      
    3ddc:  02cc                                                  
    3dde:  70f8           	moveq #-8,%d0                         
    3de0:  c1b8 02cc      	andl %d0,0x2cc                        
    3de4:  4e5e           	unlk %fp                              
    3de6:  4e75           	rts                                   
```

# HwrModelSpecificInit: 0x3dfe - 0x3e04

```
    3dfe:  4e56 0000      	linkw %fp,#0                          
    3e02:  4e5e           	unlk %fp                              
    3e04:  4e75           	rts                                   
```

# PrvSetVoltageDividerEnabled: 0x3e1e - 0x3e26

```
    3e1e:  4e56 0000      	linkw %fp,#0                          
    3e22:  7000           	moveq #0,%d0                          
    3e24:  4e5e           	unlk %fp                              
    3e26:  4e75           	rts                                   
```

# HwrModelInitStage2: 0x3e46 - 0x3e8c

Locals:

   * `%fp@(-4)`     : ???

```
    3e46:  4e56 fffc      	linkw %fp,#-4                         
    3e4a:  42ae fffc      	clrl %fp@(-4)                         
    3e4e:  486e fffc      	pea %fp@(-4)                          
    3e52:  3f3c 0002      	movew #2,%sp@-                        
    3e56:  2f3c 7073 7973 	movel #1886615923,%sp@-               
    3e5c:  4e4f           	trap #15                              
    3e5e:  a27b           	0121173                               ; sysTrapFtrGet
    3e60:  02ae ffff 0000 	andil #-65536,%fp@(-4)                
    3e66:  fffc                                                  
    3e68:  00ae 0003 0000 	oril #196608,%fp@(-4)                 
    3e6e:  fffc                                                  
    3e70:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    3e74:  3f3c 0002      	movew #2,%sp@-                        
    3e78:  2f3c 7073 7973 	movel #1886615923,%sp@-               
    3e7e:  4e4f           	trap #15                              
    3e80:  a27c           	0121174                               ; sysTrapFtrSet
    3e82:  4eba c75e      	jsr %pc@(0x5e2)                       ; HwrSonyModelInit
    3e86:  4eba 001c      	jsr %pc@(0x3ea4)                      ; PrvHrInit
    3e8a:  4e5e           	unlk %fp                              
    3e8c:  4e75           	rts                                   
```

# PrvHrInit: 0x3ea4 - 0x3f52

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-38)`    : ???

```
    3ea4:  4e56 ffda      	linkw %fp,#-38                        
    3ea8:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    3eac:  486e fffc      	pea %fp@(-4)                          
    3eb0:  486e fffa      	pea %fp@(-6)                          
    3eb4:  1f3c 0001      	moveb #1,%sp@-                        
    3eb8:  2f3c 5368 4872 	movel #1399343218,%sp@-               
    3ebe:  2f3c 736e 6872 	movel #1936615538,%sp@-               
    3ec4:  486e ffda      	pea %fp@(-38)                         
    3ec8:  1f3c 0001      	moveb #1,%sp@-                        
    3ecc:  4e4f           	trap #15                              
    3ece:  a078           	0120170                               ; sysTrapDmGetNextDatabaseByTypeCreator
    3ed0:  3600           	movew %d0,%d3                         
    3ed2:  4fef 0018      	lea %sp@(24),%sp                      
    3ed6:  6674           	bnes 0x3f4c                           ; ↓ label_1
    3ed8:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    3edc:  4e4f           	trap #15                              
    3ede:  a024           	0120044                               ; sysTrapMemLocalIDKind
    3ee0:  5300           	subqb #1,%d0                          
    3ee2:  56c0           	sne %d0                               
    3ee4:  4400           	negb %d0                              
    3ee6:  1800           	moveb %d0,%d4                         
    3ee8:  3f3c 0001      	movew #1,%sp@-                        
    3eec:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    3ef0:  3f2e fffa      	movew %fp@(-6),%sp@-                  
    3ef4:  4e4f           	trap #15                              
    3ef6:  a049           	0120111                               ; sysTrapDmOpenDatabase
    3ef8:  2448           	moveal %a0,%a2                        
    3efa:  200a           	movel %a2,%d0                         
    3efc:  4fef 000c      	lea %sp@(12),%sp                      
    3f00:  674a           	beqs 0x3f4c                           ; ↓ label_1
    3f02:  4267           	clrw %sp@-                            
    3f04:  2f3c 6869 7273 	movel #1751741043,%sp@-               
    3f0a:  4e4f           	trap #15                              
    3f0c:  a060           	0120140                               ; sysTrapDmGet1Resource
    3f0e:  2648           	moveal %a0,%a3                        
    3f10:  200b           	movel %a3,%d0                         
    3f12:  5c4f           	addqw #6,%sp                          
    3f14:  672e           	beqs 0x3f44                           ; ↓ label_0
    3f16:  2f0b           	movel %a3,%sp@-                       
    3f18:  4e4f           	trap #15                              
    3f1a:  a021           	0120041                               ; sysTrapMemHandleLock
    3f1c:  2608           	movel %a0,%d3                         
    3f1e:  2043           	moveal %d3,%a0                        
    3f20:  4e90           	jsr %a0@                              
    3f22:  4a04           	tstb %d4                              
    3f24:  584f           	addqw #4,%sp                          
    3f26:  661c           	bnes 0x3f44                           ; ↓ label_0
    3f28:  4267           	clrw %sp@-                            
    3f2a:  2f03           	movel %d3,%sp@-                       
    3f2c:  4e4f           	trap #15                              
    3f2e:  a01b           	0120033                               ; sysTrapMemPtrSetOwner
    3f30:  1f3c 0001      	moveb #1,%sp@-                        
    3f34:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    3f38:  3f2e fffa      	movew %fp@(-6),%sp@-                  
    3f3c:  4e4f           	trap #15                              
    3f3e:  a2eb           	0121353                               ; sysTrapDmDatabaseProtect
    3f40:  4fef 000e      	lea %sp@(14),%sp                      

  label_0:
    3f44:  2f0a           	movel %a2,%sp@-                       
    3f46:  4e4f           	trap #15                              
    3f48:  a04a           	0120112                               ; sysTrapDmCloseDatabase
    3f4a:  584f           	addqw #4,%sp                          

  label_1:
    3f4c:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    3f50:  4e5e           	unlk %fp                              
    3f52:  4e75           	rts                                   
```

# MQGetDrvConfigPtr: 0x466a - 0x4674

```
    466a:  4e56 0000      	linkw %fp,#0                          
    466e:  41fa f8f0      	lea %pc@(0x3f60),%a0                  ; ??? 0x3f60
    4672:  4e5e           	unlk %fp                              
    4674:  4e75           	rts                                   
```

# MQGetRegBasePtr: 0x468a - 0x4696

```
    468a:  4e56 0000      	linkw %fp,#0                          
    468e:  207c 1f04 0000 	moveal #520355840,%a0                 
    4694:  4e5e           	unlk %fp                              
    4696:  4e75           	rts                                   
```

# MQGetMemBasePtr: 0x46aa - 0x46b6

```
    46aa:  4e56 0000      	linkw %fp,#0                          
    46ae:  207c 1f00 0000 	moveal #520093696,%a0                 
    46b4:  4e5e           	unlk %fp                              
    46b6:  4e75           	rts                                   
```

# MQInit: 0x46ca - 0x46ee

```
    46ca:  4e56 0000      	linkw %fp,#0                          
    46ce:  2f0a           	movel %a2,%sp@-                       
    46d0:  4eba ff98      	jsr %pc@(0x466a)                      ; MQGetDrvConfigPtr
    46d4:  2448           	moveal %a0,%a2                        
    46d6:  42a7           	clrl %sp@-                            
    46d8:  3f2a 0032      	movew %a2@(50),%sp@-                  
    46dc:  486a 0034      	pea %a2@(52)                          
    46e0:  4eba 0948      	jsr %pc@(0x502a)                      ; WriteRegs
    46e4:  7001           	moveq #1,%d0                          
    46e6:  4fef 000a      	lea %sp@(10),%sp                      
    46ea:  245f           	moveal %sp@+,%a2                      
    46ec:  4e5e           	unlk %fp                              
    46ee:  4e75           	rts                                   
```

# MQSetDisplay: 0x46fa - 0x4940

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???

```
    46fa:  4e56 fffc      	linkw %fp,#-4                         
    46fe:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    4702:  262e 000c      	movel %fp@(12),%d3                    
    4706:  3c2e 0010      	movew %fp@(16),%d6                    
    470a:  4eba ff5e      	jsr %pc@(0x466a)                      ; MQGetDrvConfigPtr
    470e:  2448           	moveal %a0,%a2                        
    4710:  4eba ff78      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4714:  2648           	moveal %a0,%a3                        
    4716:  4eba ff92      	jsr %pc@(0x46aa)                      ; MQGetMemBasePtr
    471a:  4a46           	tstw %d6                              
    471c:  6606           	bnes 0x4724                           ; ↓ label_0
    471e:  7c00           	moveq #0,%d6                          
    4720:  1c2a 0016      	moveb %a2@(22),%d6                    

  label_0:
    4724:  4a46           	tstw %d6                              
    4726:  6602           	bnes 0x472a                           ; ↓ label_1
    4728:  7c08           	moveq #8,%d6                          

  label_1:
    472a:  4a83           	tstl %d3                              
    472c:  662c           	bnes 0x475a                           ; ↓ label_3
    472e:  382a 001c      	movew %a2@(28),%d4                    
    4732:  6706           	beqs 0x473a                           ; ↓ label_2
    4734:  7600           	moveq #0,%d3                          
    4736:  3604           	movew %d4,%d3                         
    4738:  6020           	bras 0x475a                           ; ↓ label_3

  label_2:
    473a:  7000           	moveq #0,%d0                          
    473c:  302a 0018      	movew %a2@(24),%d0                    
    4740:  3046           	moveaw %d6,%a0                        
    4742:  2208           	movel %a0,%d1                         
    4744:  4eba b938      	jsr %pc@(0x7e)                        ; anonymous_function_0
    4748:  2200           	movel %d0,%d1                         
    474a:  e481           	asrl #2,%d1                           
    474c:  e089           	lsrl #8,%d1                           
    474e:  e089           	lsrl #8,%d1                           
    4750:  e089           	lsrl #8,%d1                           
    4752:  ea89           	lsrl #5,%d1                           
    4754:  d280           	addl %d0,%d1                          
    4756:  e681           	asrl #3,%d1                           
    4758:  2601           	movel %d1,%d3                         

  label_3:
    475a:  2f2e 0008      	movel %fp@(8),%sp@-                   
    475e:  486b 01b0      	pea %a3@(432)                         
    4762:  4eba 0a80      	jsr %pc@(0x51e4)                      ; sREGW
    4766:  2f03           	movel %d3,%sp@-                       
    4768:  486b 01b8      	pea %a3@(440)                         
    476c:  4eba 0a76      	jsr %pc@(0x51e4)                      ; sREGW
    4770:  3d6a 0018 fffe 	movew %a2@(24),%fp@(-2)               
    4776:  3d6a 001a fffc 	movew %a2@(26),%fp@(-4)               
    477c:  3a2a 0012      	movew %a2@(18),%d5                    
    4780:  ba6e fffe      	cmpw %fp@(-2),%d5                     
    4784:  4fef 0010      	lea %sp@(16),%sp                      
    4788:  6322           	blss 0x47ac                           ; ↓ label_4
    478a:  3005           	movew %d5,%d0                         
    478c:  906e fffe      	subw %fp@(-2),%d0                     
    4790:  e248           	lsrw #1,%d0                           
    4792:  7200           	moveq #0,%d1                          
    4794:  3200           	movew %d0,%d1                         
    4796:  302e fffe      	movew %fp@(-2),%d0                    
    479a:  0640 ffff      	addiw #-1,%d0                         
    479e:  7400           	moveq #0,%d2                          
    47a0:  3400           	movew %d0,%d2                         
    47a2:  4842           	swap %d2                              
    47a4:  4242           	clrw %d2                              
    47a6:  8481           	orl %d1,%d2                           
    47a8:  2002           	movel %d2,%d0                         
    47aa:  6012           	bras 0x47be                           ; ↓ label_5

  label_4:
    47ac:  302a 0012      	movew %a2@(18),%d0                    
    47b0:  0640 ffff      	addiw #-1,%d0                         
    47b4:  7200           	moveq #0,%d1                          
    47b6:  3200           	movew %d0,%d1                         
    47b8:  4841           	swap %d1                              
    47ba:  4241           	clrw %d1                              
    47bc:  2001           	movel %d1,%d0                         

  label_5:
    47be:  2600           	movel %d0,%d3                         
    47c0:  3e2a 0014      	movew %a2@(20),%d7                    
    47c4:  be6e fffc      	cmpw %fp@(-4),%d7                     
    47c8:  6322           	blss 0x47ec                           ; ↓ label_6
    47ca:  3007           	movew %d7,%d0                         
    47cc:  906e fffc      	subw %fp@(-4),%d0                     
    47d0:  e248           	lsrw #1,%d0                           
    47d2:  7200           	moveq #0,%d1                          
    47d4:  3200           	movew %d0,%d1                         
    47d6:  302e fffc      	movew %fp@(-4),%d0                    
    47da:  0640 ffff      	addiw #-1,%d0                         
    47de:  7400           	moveq #0,%d2                          
    47e0:  3400           	movew %d0,%d2                         
    47e2:  4842           	swap %d2                              
    47e4:  4242           	clrw %d2                              
    47e6:  8481           	orl %d1,%d2                           
    47e8:  2002           	movel %d2,%d0                         
    47ea:  6012           	bras 0x47fe                           ; ↓ label_7

  label_6:
    47ec:  302a 0014      	movew %a2@(20),%d0                    
    47f0:  0640 ffff      	addiw #-1,%d0                         
    47f4:  7200           	moveq #0,%d1                          
    47f6:  3200           	movew %d0,%d1                         
    47f8:  4841           	swap %d1                              
    47fa:  4241           	clrw %d1                              
    47fc:  2001           	movel %d1,%d0                         

  label_7:
    47fe:  2800           	movel %d0,%d4                         
    4800:  2f03           	movel %d3,%sp@-                       
    4802:  202a 0138      	movel %a2@(312),%d0                   
    4806:  4873 0800      	pea %a3@(00000000,%d0:l)              
    480a:  4eba 09d8      	jsr %pc@(0x51e4)                      ; sREGW
    480e:  2f04           	movel %d4,%sp@-                       
    4810:  202a 0148      	movel %a2@(328),%d0                   
    4814:  4873 0800      	pea %a3@(00000000,%d0:l)              
    4818:  4eba 09ca      	jsr %pc@(0x51e4)                      ; sREGW
    481c:  42a7           	clrl %sp@-                            
    481e:  202a 0134      	movel %a2@(308),%d0                   
    4822:  5580           	subql #2,%d0                          
    4824:  3f00           	movew %d0,%sp@-                       
    4826:  486a 0158      	pea %a2@(344)                         
    482a:  4eba 07fe      	jsr %pc@(0x502a)                      ; WriteRegs
    482e:  42a7           	clrl %sp@-                            
    4830:  3f2a 01ba      	movew %a2@(442),%sp@-                 
    4834:  486a 01bc      	pea %a2@(444)                         
    4838:  4eba 07f0      	jsr %pc@(0x502a)                      ; WriteRegs
    483c:  2a3c 0000 0640 	movel #1600,%d5                       
    4842:  7800           	moveq #0,%d4                          
    4844:  3044           	moveaw %d4,%a0                        
    4846:  2008           	movel %a0,%d0                         
    4848:  e588           	lsll #2,%d0                           
    484a:  204a           	moveal %a2,%a0                        
    484c:  d1c0           	addal %d0,%a0                         
    484e:  2608           	movel %a0,%d3                         
    4850:  4fef 0024      	lea %sp@(36),%sp                      
    4854:  6016           	bras 0x486c                           ; ↓ label_9

  label_8:
    4856:  2043           	moveal %d3,%a0                        
    4858:  2f28 0658      	movel %a0@(1624),%sp@-                
    485c:  4873 5800      	pea %a3@(00000000,%d5:l)              
    4860:  4eba 0982      	jsr %pc@(0x51e4)                      ; sREGW
    4864:  5883           	addql #4,%d3                          
    4866:  5244           	addqw #1,%d4                          
    4868:  5885           	addql #4,%d5                          
    486a:  504f           	addqw #8,%sp                          

  label_9:
    486c:  b86a 0656      	cmpw %a2@(1622),%d4                   
    4870:  6de4           	blts 0x4856                           ; ↑ label_8
    4872:  2a3c 0000 06c0 	movel #1728,%d5                       
    4878:  7800           	moveq #0,%d4                          
    487a:  3044           	moveaw %d4,%a0                        
    487c:  2008           	movel %a0,%d0                         
    487e:  e588           	lsll #2,%d0                           
    4880:  204a           	moveal %a2,%a0                        
    4882:  d1c0           	addal %d0,%a0                         
    4884:  2608           	movel %a0,%d3                         
    4886:  6016           	bras 0x489e                           ; ↓ label_11

  label_10:
    4888:  2043           	moveal %d3,%a0                        
    488a:  2f28 06dc      	movel %a0@(1756),%sp@-                
    488e:  4873 5800      	pea %a3@(00000000,%d5:l)              
    4892:  4eba 0950      	jsr %pc@(0x51e4)                      ; sREGW
    4896:  5883           	addql #4,%d3                          
    4898:  5244           	addqw #1,%d4                          
    489a:  5885           	addql #4,%d5                          
    489c:  504f           	addqw #8,%sp                          

  label_11:
    489e:  b86a 06da      	cmpw %a2@(1754),%d4                   
    48a2:  6de4           	blts 0x4888                           ; ↑ label_10
    48a4:  486b 0600      	pea %a3@(1536)                        
    48a8:  4eba 0970      	jsr %pc@(0x521a)                      ; sREGR
    48ac:  2600           	movel %d0,%d3                         
    48ae:  7001           	moveq #1,%d0                          
    48b0:  8680           	orl %d0,%d3                           
    48b2:  2f03           	movel %d3,%sp@-                       
    48b4:  486b 0600      	pea %a3@(1536)                        
    48b8:  4eba 092a      	jsr %pc@(0x51e4)                      ; sREGW
    48bc:  4fef 000c      	lea %sp@(12),%sp                      
    48c0:  3006           	movew %d6,%d0                         
    48c2:  5340           	subqw #1,%d0                          
    48c4:  6712           	beqs 0x48d8                           ; ↓ label_12
    48c6:  5340           	subqw #1,%d0                          
    48c8:  6712           	beqs 0x48dc                           ; ↓ label_13
    48ca:  5540           	subqw #2,%d0                          
    48cc:  6712           	beqs 0x48e0                           ; ↓ label_14
    48ce:  5940           	subqw #4,%d0                          
    48d0:  6712           	beqs 0x48e4                           ; ↓ label_15
    48d2:  5140           	subqw #8,%d0                          
    48d4:  6712           	beqs 0x48e8                           ; ↓ label_16
    48d6:  6018           	bras 0x48f0                           ; ↓ label_17

  label_12:
    48d8:  7600           	moveq #0,%d3                          
    48da:  6016           	bras 0x48f2                           ; ↓ label_18

  label_13:
    48dc:  7610           	moveq #16,%d3                         
    48de:  6012           	bras 0x48f2                           ; ↓ label_18

  label_14:
    48e0:  7620           	moveq #32,%d3                         
    48e2:  600e           	bras 0x48f2                           ; ↓ label_18

  label_15:
    48e4:  7630           	moveq #48,%d3                         
    48e6:  600a           	bras 0x48f2                           ; ↓ label_18

  label_16:
    48e8:  263c 0000 00c0 	movel #192,%d3                        
    48ee:  6002           	bras 0x48f2                           ; ↓ label_18

  label_17:
    48f0:  7630           	moveq #48,%d3                         

  label_18:
    48f2:  486b 0180      	pea %a3@(384)                         
    48f6:  4eba 0922      	jsr %pc@(0x521a)                      ; sREGR
    48fa:  2800           	movel %d0,%d4                         
    48fc:  0284 ffff ff0f 	andil #-241,%d4                       
    4902:  8883           	orl %d3,%d4                           
    4904:  2f04           	movel %d4,%sp@-                       
    4906:  486b 0180      	pea %a3@(384)                         
    490a:  4eba 08d8      	jsr %pc@(0x51e4)                      ; sREGW
    490e:  3f3c 0001      	movew #1,%sp@-                        
    4912:  4eba 0522      	jsr %pc@(0x4e36)                      ; MQEnableGE
    4916:  42a7           	clrl %sp@-                            
    4918:  4eba 05ca      	jsr %pc@(0x4ee4)                      ; MQErase
    491c:  486e fffc      	pea %fp@(-4)                          
    4920:  486e fffe      	pea %fp@(-2)                          
    4924:  3f3c 0001      	movew #1,%sp@-                        
    4928:  4eba 043a      	jsr %pc@(0x4d64)                      ; MQSetLcdSize
    492c:  3f3c 0001      	movew #1,%sp@-                        
    4930:  4eba 0020      	jsr %pc@(0x4952)                      ; MQEnableDisplay
    4934:  7001           	moveq #1,%d0                          
    4936:  4fef 001e      	lea %sp@(30),%sp                      
    493a:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    493e:  4e5e           	unlk %fp                              
    4940:  4e75           	rts                                   
```

# MQEnableDisplay: 0x4952 - 0x4b04

Parameters:

   * `%fp@(8)`      : ???

```
    4952:  4e56 0000      	linkw %fp,#0                          
    4956:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    495a:  4eba fd0e      	jsr %pc@(0x466a)                      ; MQGetDrvConfigPtr
    495e:  2448           	moveal %a0,%a2                        
    4960:  4eba fd28      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4964:  2648           	moveal %a0,%a3                        
    4966:  486b 0008      	pea %a3@(8)                           
    496a:  4eba 08ae      	jsr %pc@(0x521a)                      ; sREGR
    496e:  0280 0000 00aa 	andil #170,%d0                        
    4974:  2600           	movel %d0,%d3                         
    4976:  302e 0008      	movew %fp@(8),%d0                     
    497a:  584f           	addqw #4,%sp                          
    497c:  4a40           	tstw %d0                              
    497e:  6700 00fa      	beqw 0x4a7a                           ; ↓ label_3
    4982:  5340           	subqw #1,%d0                          
    4984:  670e           	beqs 0x4994                           ; ↓ label_0
    4986:  5340           	subqw #1,%d0                          
    4988:  6756           	beqs 0x49e0                           ; ↓ label_2
    498a:  0440 007e      	subiw #126,%d0                        
    498e:  6736           	beqs 0x49c6                           ; ↓ label_1
    4990:  6000 016a      	braw 0x4afc                           ; ↓ label_6

  label_0:
    4994:  0c83 0000 00aa 	cmpil #170,%d3                        
    499a:  6700 0160      	beqw 0x4afc                           ; ↓ label_6
    499e:  2f3c 0000 9c40 	movel #40000,%sp@-                    
    49a4:  4eba bbe0      	jsr %pc@(0x586)                       ; HwrDelay
    49a8:  42a7           	clrl %sp@-                            
    49aa:  3f2a 02de      	movew %a2@(734),%sp@-                 
    49ae:  486a 02e0      	pea %a2@(736)                         
    49b2:  4eba 0676      	jsr %pc@(0x502a)                      ; WriteRegs
    49b6:  4878 1388      	pea 0x1388                            
    49ba:  4eba bbca      	jsr %pc@(0x586)                       ; HwrDelay
    49be:  4fef 0012      	lea %sp@(18),%sp                      
    49c2:  6000 0138      	braw 0x4afc                           ; ↓ label_6

  label_1:
    49c6:  4eba 076a      	jsr %pc@(0x5132)                      ; InVBlank
    49ca:  42a7           	clrl %sp@-                            
    49cc:  3f2a 03b2      	movew %a2@(946),%sp@-                 
    49d0:  486a 03b4      	pea %a2@(948)                         
    49d4:  4eba 0654      	jsr %pc@(0x502a)                      ; WriteRegs
    49d8:  4fef 000a      	lea %sp@(10),%sp                      
    49dc:  6000 011e      	braw 0x4afc                           ; ↓ label_6

  label_2:
    49e0:  0c83 0000 00aa 	cmpil #170,%d3                        
    49e6:  6700 0114      	beqw 0x4afc                           ; ↓ label_6
    49ea:  4eba 0746      	jsr %pc@(0x5132)                      ; InVBlank
    49ee:  42a7           	clrl %sp@-                            
    49f0:  3f2a 0496      	movew %a2@(1174),%sp@-                
    49f4:  486a 0498      	pea %a2@(1176)                        
    49f8:  4eba 0630      	jsr %pc@(0x502a)                      ; WriteRegs
    49fc:  2f3c 0000 c350 	movel #50000,%sp@-                    
    4a02:  4eba bb82      	jsr %pc@(0x586)                       ; HwrDelay
    4a06:  4e4f           	trap #15                              
    4a08:  a0a5           	0120245                               ; sysTrapSysDisableInts
    4a0a:  3600           	movew %d0,%d3                         
    4a0c:  42a7           	clrl %sp@-                            
    4a0e:  3f2a 04ba      	movew %a2@(1210),%sp@-                
    4a12:  486a 04bc      	pea %a2@(1212)                        
    4a16:  4eba 0612      	jsr %pc@(0x502a)                      ; WriteRegs
    4a1a:  3f03           	movew %d3,%sp@-                       
    4a1c:  4e4f           	trap #15                              
    4a1e:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    4a20:  2f3c 0000 c350 	movel #50000,%sp@-                    
    4a26:  4eba bb5e      	jsr %pc@(0x586)                       ; HwrDelay
    4a2a:  4e4f           	trap #15                              
    4a2c:  a0a5           	0120245                               ; sysTrapSysDisableInts
    4a2e:  3600           	movew %d0,%d3                         
    4a30:  42a7           	clrl %sp@-                            
    4a32:  3f2a 04ee      	movew %a2@(1262),%sp@-                
    4a36:  486a 04f0      	pea %a2@(1264)                        
    4a3a:  4eba 05ee      	jsr %pc@(0x502a)                      ; WriteRegs
    4a3e:  3f03           	movew %d3,%sp@-                       
    4a40:  4e4f           	trap #15                              
    4a42:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    4a44:  2f3c 0000 80e8 	movel #33000,%sp@-                    
    4a4a:  4eba bb3a      	jsr %pc@(0x586)                       ; HwrDelay
    4a4e:  4e4f           	trap #15                              
    4a50:  a0a5           	0120245                               ; sysTrapSysDisableInts
    4a52:  3600           	movew %d0,%d3                         
    4a54:  42a7           	clrl %sp@-                            
    4a56:  3f2a 0552      	movew %a2@(1362),%sp@-                
    4a5a:  486a 0554      	pea %a2@(1364)                        
    4a5e:  4eba 05ca      	jsr %pc@(0x502a)                      ; WriteRegs
    4a62:  3f03           	movew %d3,%sp@-                       
    4a64:  4e4f           	trap #15                              
    4a66:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    4a68:  2f3c 0000 c350 	movel #50000,%sp@-                    
    4a6e:  4eba bb16      	jsr %pc@(0x586)                       ; HwrDelay
    4a72:  4fef 003e      	lea %sp@(62),%sp                      
    4a76:  6000 0084      	braw 0x4afc                           ; ↓ label_6

  label_3:
    4a7a:  4e4f           	trap #15                              
    4a7c:  a0a5           	0120245                               ; sysTrapSysDisableInts
    4a7e:  3600           	movew %d0,%d3                         
    4a80:  4eba 06b0      	jsr %pc@(0x5132)                      ; InVBlank
    4a84:  42a7           	clrl %sp@-                            
    4a86:  3f2a 0576      	movew %a2@(1398),%sp@-                
    4a8a:  486a 0578      	pea %a2@(1400)                        
    4a8e:  4eba 059a      	jsr %pc@(0x502a)                      ; WriteRegs
    4a92:  3f03           	movew %d3,%sp@-                       
    4a94:  4e4f           	trap #15                              
    4a96:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    4a98:  7605           	moveq #5,%d3                          
    4a9a:  3003           	movew %d3,%d0                         
    4a9c:  5343           	subqw #1,%d3                          
    4a9e:  4a40           	tstw %d0                              
    4aa0:  4fef 000c      	lea %sp@(12),%sp                      
    4aa4:  670c           	beqs 0x4ab2                           ; ↓ label_5

  label_4:
    4aa6:  4eba 068a      	jsr %pc@(0x5132)                      ; InVBlank
    4aaa:  3003           	movew %d3,%d0                         
    4aac:  5343           	subqw #1,%d3                          
    4aae:  4a40           	tstw %d0                              
    4ab0:  66f4           	bnes 0x4aa6                           ; ↑ label_4

  label_5:
    4ab2:  4e4f           	trap #15                              
    4ab4:  a0a5           	0120245                               ; sysTrapSysDisableInts
    4ab6:  3600           	movew %d0,%d3                         
    4ab8:  4eba 0678      	jsr %pc@(0x5132)                      ; InVBlank
    4abc:  42a7           	clrl %sp@-                            
    4abe:  3f2a 059a      	movew %a2@(1434),%sp@-                
    4ac2:  486a 059c      	pea %a2@(1436)                        
    4ac6:  4eba 0562      	jsr %pc@(0x502a)                      ; WriteRegs
    4aca:  3f03           	movew %d3,%sp@-                       
    4acc:  4e4f           	trap #15                              
    4ace:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    4ad0:  42a7           	clrl %sp@-                            
    4ad2:  3f2a 05ae      	movew %a2@(1454),%sp@-                
    4ad6:  486a 05b0      	pea %a2@(1456)                        
    4ada:  4eba 054e      	jsr %pc@(0x502a)                      ; WriteRegs
    4ade:  4e4f           	trap #15                              
    4ae0:  a0a5           	0120245                               ; sysTrapSysDisableInts
    4ae2:  3600           	movew %d0,%d3                         
    4ae4:  42a7           	clrl %sp@-                            
    4ae6:  3f2a 0602      	movew %a2@(1538),%sp@-                
    4aea:  486a 0604      	pea %a2@(1540)                        
    4aee:  4eba 053a      	jsr %pc@(0x502a)                      ; WriteRegs
    4af2:  3f03           	movew %d3,%sp@-                       
    4af4:  4e4f           	trap #15                              
    4af6:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    4af8:  4fef 0022      	lea %sp@(34),%sp                      

  label_6:
    4afc:  7001           	moveq #1,%d0                          
    4afe:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    4b02:  4e5e           	unlk %fp                              
    4b04:  4e75           	rts                                   
```

# MQResetDisplay: 0x4b18 - 0x4b58

```
    4b18:  4e56 0000      	linkw %fp,#0                          
    4b1c:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    4b20:  4eba fb48      	jsr %pc@(0x466a)                      ; MQGetDrvConfigPtr
    4b24:  2448           	moveal %a0,%a2                        
    4b26:  4eba fb62      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4b2a:  2648           	moveal %a0,%a3                        
    4b2c:  486b 0388      	pea %a3@(904)                         
    4b30:  4eba 06e8      	jsr %pc@(0x521a)                      ; sREGR
    4b34:  2600           	movel %d0,%d3                         
    4b36:  7001           	moveq #1,%d0                          
    4b38:  c083           	andl %d3,%d0                          
    4b3a:  5380           	subql #1,%d0                          
    4b3c:  584f           	addqw #4,%sp                          
    4b3e:  6612           	bnes 0x4b52                           ; ↓ label_0
    4b40:  42a7           	clrl %sp@-                            
    4b42:  3f2a 03b2      	movew %a2@(946),%sp@-                 
    4b46:  486a 03b4      	pea %a2@(948)                         
    4b4a:  4eba 04de      	jsr %pc@(0x502a)                      ; WriteRegs
    4b4e:  4fef 000a      	lea %sp@(10),%sp                      

  label_0:
    4b52:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    4b56:  4e5e           	unlk %fp                              
    4b58:  4e75           	rts                                   
```

# MQEnableLCDClock: 0x4b6c - 0x4ba8

Parameters:

   * `%fp@(8)`      : ???

```
    4b6c:  4e56 0000      	linkw %fp,#0                          
    4b70:  2f0a           	movel %a2,%sp@-                       
    4b72:  2f03           	movel %d3,%sp@-                       
    4b74:  4eba fb14      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4b78:  2448           	moveal %a0,%a2                        
    4b7a:  486a 0384      	pea %a2@(900)                         
    4b7e:  4eba 069a      	jsr %pc@(0x521a)                      ; sREGR
    4b82:  0280 ffff ff00 	andil #-256,%d0                       
    4b88:  2600           	movel %d0,%d3                         
    4b8a:  4a6e 0008      	tstw %fp@(8)                          
    4b8e:  584f           	addqw #4,%sp                          
    4b90:  6704           	beqs 0x4b96                           ; ↓ label_0
    4b92:  7003           	moveq #3,%d0                          
    4b94:  8680           	orl %d0,%d3                           

  label_0:
    4b96:  2f03           	movel %d3,%sp@-                       
    4b98:  486a 0384      	pea %a2@(900)                         
    4b9c:  4eba 0646      	jsr %pc@(0x51e4)                      ; sREGW
    4ba0:  504f           	addqw #8,%sp                          
    4ba2:  261f           	movel %sp@+,%d3                       
    4ba4:  245f           	moveal %sp@+,%a2                      
    4ba6:  4e5e           	unlk %fp                              
    4ba8:  4e75           	rts                                   
```

# MQSetLcdStartAddr: 0x4bbe - 0x4bfa

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    4bbe:  4e56 0000      	linkw %fp,#0                          
    4bc2:  2f0b           	movel %a3,%sp@-                       
    4bc4:  2f0a           	movel %a2,%sp@-                       
    4bc6:  266e 000a      	moveal %fp@(10),%a3                   
    4bca:  4eba fabe      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4bce:  2448           	moveal %a0,%a2                        
    4bd0:  4a6e 0008      	tstw %fp@(8)                          
    4bd4:  6712           	beqs 0x4be8                           ; ↓ label_0
    4bd6:  4eba 055a      	jsr %pc@(0x5132)                      ; InVBlank
    4bda:  2f13           	movel %a3@,%sp@-                      
    4bdc:  486a 01b0      	pea %a2@(432)                         
    4be0:  4eba 0602      	jsr %pc@(0x51e4)                      ; sREGW
    4be4:  504f           	addqw #8,%sp                          
    4be6:  600c           	bras 0x4bf4                           ; ↓ label_1

  label_0:
    4be8:  486a 01b0      	pea %a2@(432)                         
    4bec:  4eba 062c      	jsr %pc@(0x521a)                      ; sREGR
    4bf0:  2680           	movel %d0,%a3@                        
    4bf2:  584f           	addqw #4,%sp                          

  label_1:
    4bf4:  245f           	moveal %sp@+,%a2                      
    4bf6:  265f           	moveal %sp@+,%a3                      
    4bf8:  4e5e           	unlk %fp                              
    4bfa:  4e75           	rts                                   
```

# MQSetLcdStride: 0x4c10 - 0x4c6c

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    4c10:  4e56 0000      	linkw %fp,#0                          
    4c14:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    4c18:  266e 000a      	moveal %fp@(10),%a3                   
    4c1c:  4eba fa6c      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4c20:  2448           	moveal %a0,%a2                        
    4c22:  4a6e 0008      	tstw %fp@(8)                          
    4c26:  6732           	beqs 0x4c5a                           ; ↓ label_0
    4c28:  3053           	moveaw %a3@,%a0                       
    4c2a:  2f08           	movel %a0,%sp@-                       
    4c2c:  486a 01b8      	pea %a2@(440)                         
    4c30:  4eba 05b2      	jsr %pc@(0x51e4)                      ; sREGW
    4c34:  486a 0228      	pea %a2@(552)                         
    4c38:  4eba 05e0      	jsr %pc@(0x521a)                      ; sREGR
    4c3c:  2600           	movel %d0,%d3                         
    4c3e:  0283 ffff fc00 	andil #-1024,%d3                      
    4c44:  3053           	moveaw %a3@,%a0                       
    4c46:  2008           	movel %a0,%d0                         
    4c48:  8680           	orl %d0,%d3                           
    4c4a:  2f03           	movel %d3,%sp@-                       
    4c4c:  486a 0228      	pea %a2@(552)                         
    4c50:  4eba 0592      	jsr %pc@(0x51e4)                      ; sREGW
    4c54:  4fef 0014      	lea %sp@(20),%sp                      
    4c58:  600c           	bras 0x4c66                           ; ↓ label_1

  label_0:
    4c5a:  486a 01b8      	pea %a2@(440)                         
    4c5e:  4eba 05ba      	jsr %pc@(0x521a)                      ; sREGR
    4c62:  3680           	movew %d0,%a3@                        
    4c64:  584f           	addqw #4,%sp                          

  label_1:
    4c66:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    4c6a:  4e5e           	unlk %fp                              
    4c6c:  4e75           	rts                                   
```

# MQSetLcdBpp: 0x4c80 - 0x4d54

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    4c80:  4e56 0000      	linkw %fp,#0                          
    4c84:  48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-          
    4c88:  266e 000a      	moveal %fp@(10),%a3                   
    4c8c:  4eba f9fc      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4c90:  2448           	moveal %a0,%a2                        
    4c92:  486a 0180      	pea %a2@(384)                         
    4c96:  4eba 0582      	jsr %pc@(0x521a)                      ; sREGR
    4c9a:  2600           	movel %d0,%d3                         
    4c9c:  4a6e 0008      	tstw %fp@(8)                          
    4ca0:  584f           	addqw #4,%sp                          
    4ca2:  676c           	beqs 0x4d10                           ; ↓ label_7
    4ca4:  486a 0228      	pea %a2@(552)                         
    4ca8:  4eba 0570      	jsr %pc@(0x521a)                      ; sREGR
    4cac:  2a00           	movel %d0,%d5                         
    4cae:  0285 3fff ffff 	andil #1073741823,%d5                 
    4cb4:  3013           	movew %a3@,%d0                        
    4cb6:  584f           	addqw #4,%sp                          
    4cb8:  5340           	subqw #1,%d0                          
    4cba:  6712           	beqs 0x4cce                           ; ↓ label_0
    4cbc:  5340           	subqw #1,%d0                          
    4cbe:  6712           	beqs 0x4cd2                           ; ↓ label_1
    4cc0:  5540           	subqw #2,%d0                          
    4cc2:  6712           	beqs 0x4cd6                           ; ↓ label_2
    4cc4:  5940           	subqw #4,%d0                          
    4cc6:  6712           	beqs 0x4cda                           ; ↓ label_3
    4cc8:  5140           	subqw #8,%d0                          
    4cca:  6712           	beqs 0x4cde                           ; ↓ label_4
    4ccc:  601e           	bras 0x4cec                           ; ↓ label_5

  label_0:
    4cce:  7800           	moveq #0,%d4                          
    4cd0:  601c           	bras 0x4cee                           ; ↓ label_6

  label_1:
    4cd2:  7810           	moveq #16,%d4                         
    4cd4:  6018           	bras 0x4cee                           ; ↓ label_6

  label_2:
    4cd6:  7820           	moveq #32,%d4                         
    4cd8:  6014           	bras 0x4cee                           ; ↓ label_6

  label_3:
    4cda:  7830           	moveq #48,%d4                         
    4cdc:  6010           	bras 0x4cee                           ; ↓ label_6

  label_4:
    4cde:  283c 0000 00c0 	movel #192,%d4                        
    4ce4:  0085 4000 0000 	oril #1073741824,%d5                  
    4cea:  6002           	bras 0x4cee                           ; ↓ label_6

  label_5:
    4cec:  7830           	moveq #48,%d4                         

  label_6:
    4cee:  0283 ffff ff0f 	andil #-241,%d3                       
    4cf4:  8684           	orl %d4,%d3                           
    4cf6:  2f03           	movel %d3,%sp@-                       
    4cf8:  486a 0180      	pea %a2@(384)                         
    4cfc:  4eba 04e6      	jsr %pc@(0x51e4)                      ; sREGW
    4d00:  2f05           	movel %d5,%sp@-                       
    4d02:  486a 0228      	pea %a2@(552)                         
    4d06:  4eba 04dc      	jsr %pc@(0x51e4)                      ; sREGW
    4d0a:  4fef 0010      	lea %sp@(16),%sp                      
    4d0e:  603e           	bras 0x4d4e                           ; ↓ label_15

  label_7:
    4d10:  0283 0000 00f0 	andil #240,%d3                        
    4d16:  2003           	movel %d3,%d0                         
    4d18:  671c           	beqs 0x4d36                           ; ↓ label_8
    4d1a:  5180           	subql #8,%d0                          
    4d1c:  5180           	subql #8,%d0                          
    4d1e:  671a           	beqs 0x4d3a                           ; ↓ label_9
    4d20:  5180           	subql #8,%d0                          
    4d22:  5180           	subql #8,%d0                          
    4d24:  6718           	beqs 0x4d3e                           ; ↓ label_10
    4d26:  5180           	subql #8,%d0                          
    4d28:  5180           	subql #8,%d0                          
    4d2a:  6716           	beqs 0x4d42                           ; ↓ label_11
    4d2c:  0480 0000 0090 	subil #144,%d0                        
    4d32:  6712           	beqs 0x4d46                           ; ↓ label_12
    4d34:  6014           	bras 0x4d4a                           ; ↓ label_13

  label_8:
    4d36:  7601           	moveq #1,%d3                          
    4d38:  6012           	bras 0x4d4c                           ; ↓ label_14

  label_9:
    4d3a:  7602           	moveq #2,%d3                          
    4d3c:  600e           	bras 0x4d4c                           ; ↓ label_14

  label_10:
    4d3e:  7604           	moveq #4,%d3                          
    4d40:  600a           	bras 0x4d4c                           ; ↓ label_14

  label_11:
    4d42:  7608           	moveq #8,%d3                          
    4d44:  6006           	bras 0x4d4c                           ; ↓ label_14

  label_12:
    4d46:  7610           	moveq #16,%d3                         
    4d48:  6002           	bras 0x4d4c                           ; ↓ label_14

  label_13:
    4d4a:  7608           	moveq #8,%d3                          

  label_14:
    4d4c:  3683           	movew %d3,%a3@                        

  label_15:
    4d4e:  4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3          
    4d52:  4e5e           	unlk %fp                              
    4d54:  4e75           	rts                                   
```

# MQSetLcdSize: 0x4d64 - 0x4e24

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(14)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???

```
    4d64:  4e56 fffc      	linkw %fp,#-4                         
    4d68:  48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-          
    4d6c:  4eba f8fc      	jsr %pc@(0x466a)                      ; MQGetDrvConfigPtr
    4d70:  2648           	moveal %a0,%a3                        
    4d72:  4eba f916      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4d76:  2448           	moveal %a0,%a2                        
    4d78:  486e fffc      	pea %fp@(-4)                          
    4d7c:  4267           	clrw %sp@-                            
    4d7e:  4eba ff00      	jsr %pc@(0x4c80)                      ; MQSetLcdBpp
    4d82:  486e fffe      	pea %fp@(-2)                          
    4d86:  4267           	clrw %sp@-                            
    4d88:  4eba fe86      	jsr %pc@(0x4c10)                      ; MQSetLcdStride
    4d8c:  4a6e fffc      	tstw %fp@(-4)                         
    4d90:  4fef 000c      	lea %sp@(12),%sp                      
    4d94:  6710           	beqs 0x4da6                           ; ↓ label_0
    4d96:  302e fffe      	movew %fp@(-2),%d0                    
    4d9a:  e748           	lslw #3,%d0                           
    4d9c:  48c0           	extl %d0                              
    4d9e:  81ee fffc      	divsw %fp@(-4),%d0                    
    4da2:  3d40 fffe      	movew %d0,%fp@(-2)                    

  label_0:
    4da6:  486a 01a0      	pea %a2@(416)                         
    4daa:  4eba 046e      	jsr %pc@(0x521a)                      ; sREGR
    4dae:  2600           	movel %d0,%d3                         
    4db0:  486a 01a4      	pea %a2@(420)                         
    4db4:  4eba 0464      	jsr %pc@(0x521a)                      ; sREGR
    4db8:  2800           	movel %d0,%d4                         
    4dba:  2a03           	movel %d3,%d5                         
    4dbc:  4245           	clrw %d5                              
    4dbe:  4845           	swap %d5                              
    4dc0:  5285           	addql #1,%d5                          
    4dc2:  2604           	movel %d4,%d3                         
    4dc4:  4243           	clrw %d3                              
    4dc6:  4843           	swap %d3                              
    4dc8:  5283           	addql #1,%d3                          
    4dca:  4a6e 0008      	tstw %fp@(8)                          
    4dce:  504f           	addqw #8,%sp                          
    4dd0:  6740           	beqs 0x4e12                           ; ↓ label_3
    4dd2:  486a 0180      	pea %a2@(384)                         
    4dd6:  4eba 0442      	jsr %pc@(0x521a)                      ; sREGR
    4dda:  2600           	movel %d0,%d3                         
    4ddc:  0283 ffff 3fff 	andil #-49153,%d3                     
    4de2:  302e fffe      	movew %fp@(-2),%d0                    
    4de6:  b06b 0012      	cmpw %a3@(18),%d0                     
    4dea:  584f           	addqw #4,%sp                          
    4dec:  6c06           	bges 0x4df4                           ; ↓ label_1
    4dee:  0083 0000 4000 	oril #16384,%d3                       

  label_1:
    4df4:  302e fffe      	movew %fp@(-2),%d0                    
    4df8:  b06b 0014      	cmpw %a3@(20),%d0                     
    4dfc:  6c06           	bges 0x4e04                           ; ↓ label_2
    4dfe:  0083 0000 8000 	oril #32768,%d3                       

  label_2:
    4e04:  2f03           	movel %d3,%sp@-                       
    4e06:  486a 0180      	pea %a2@(384)                         
    4e0a:  4eba 03d8      	jsr %pc@(0x51e4)                      ; sREGW
    4e0e:  504f           	addqw #8,%sp                          
    4e10:  600c           	bras 0x4e1e                           ; ↓ label_4

  label_3:
    4e12:  206e 000a      	moveal %fp@(10),%a0                   
    4e16:  3085           	movew %d5,%a0@                        
    4e18:  206e 000e      	moveal %fp@(14),%a0                   
    4e1c:  3083           	movew %d3,%a0@                        

  label_4:
    4e1e:  4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3          
    4e22:  4e5e           	unlk %fp                              
    4e24:  4e75           	rts                                   
```

# MQEnableGE: 0x4e36 - 0x4ed4

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-2)`     : ???

```
    4e36:  4e56 fffe      	linkw %fp,#-2                         
    4e3a:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    4e3e:  4eba f84a      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4e42:  2448           	moveal %a0,%a2                        
    4e44:  4a6e 0008      	tstw %fp@(8)                          
    4e48:  6768           	beqs 0x4eb2                           ; ↓ label_2
    4e4a:  486e fffe      	pea %fp@(-2)                          
    4e4e:  4267           	clrw %sp@-                            
    4e50:  4eba fe2e      	jsr %pc@(0x4c80)                      ; MQSetLcdBpp
    4e54:  486a 01b0      	pea %a2@(432)                         
    4e58:  4eba 03c0      	jsr %pc@(0x521a)                      ; sREGR
    4e5c:  2800           	movel %d0,%d4                         
    4e5e:  486a 01b8      	pea %a2@(440)                         
    4e62:  4eba 03b6      	jsr %pc@(0x521a)                      ; sREGR
    4e66:  2600           	movel %d0,%d3                         
    4e68:  302e fffe      	movew %fp@(-2),%d0                    
    4e6c:  4fef 000e      	lea %sp@(14),%sp                      
    4e70:  5140           	subqw #8,%d0                          
    4e72:  670c           	beqs 0x4e80                           ; ↓ label_1
    4e74:  5140           	subqw #8,%d0                          
    4e76:  6702           	beqs 0x4e7a                           ; ↓ label_0
    4e78:  6006           	bras 0x4e80                           ; ↓ label_1

  label_0:
    4e7a:  0083 4000 0000 	oril #1073741824,%d3                  

  label_1:
    4e80:  2f03           	movel %d3,%sp@-                       
    4e82:  486a 0228      	pea %a2@(552)                         
    4e86:  4eba 035c      	jsr %pc@(0x51e4)                      ; sREGW
    4e8a:  2f04           	movel %d4,%sp@-                       
    4e8c:  486a 022c      	pea %a2@(556)                         
    4e90:  4eba 0352      	jsr %pc@(0x51e4)                      ; sREGW
    4e94:  486a 0394      	pea %a2@(916)                         
    4e98:  4eba 0380      	jsr %pc@(0x521a)                      ; sREGR
    4e9c:  2600           	movel %d0,%d3                         
    4e9e:  7001           	moveq #1,%d0                          
    4ea0:  8680           	orl %d0,%d3                           
    4ea2:  2f03           	movel %d3,%sp@-                       
    4ea4:  486a 0394      	pea %a2@(916)                         
    4ea8:  4eba 033a      	jsr %pc@(0x51e4)                      ; sREGW
    4eac:  4fef 001c      	lea %sp@(28),%sp                      
    4eb0:  601c           	bras 0x4ece                           ; ↓ label_3

  label_2:
    4eb2:  486a 0394      	pea %a2@(916)                         
    4eb6:  4eba 0362      	jsr %pc@(0x521a)                      ; sREGR
    4eba:  2600           	movel %d0,%d3                         
    4ebc:  70fe           	moveq #-2,%d0                         
    4ebe:  c680           	andl %d0,%d3                          
    4ec0:  2f03           	movel %d3,%sp@-                       
    4ec2:  486a 0394      	pea %a2@(916)                         
    4ec6:  4eba 031c      	jsr %pc@(0x51e4)                      ; sREGW
    4eca:  4fef 000c      	lea %sp@(12),%sp                      

  label_3:
    4ece:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    4ed2:  4e5e           	unlk %fp                              
    4ed4:  4e75           	rts                                   
```

# MQErase: 0x4ee4 - 0x501e

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-4)`     : ???

```
    4ee4:  4e56 fffc      	linkw %fp,#-4                         
    4ee8:  48e7 1c30      	moveml %d3-%d5/%a2-%a3,%sp@-          
    4eec:  2a2e 0008      	movel %fp@(8),%d5                     
    4ef0:  4eba f798      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    4ef4:  2448           	moveal %a0,%a2                        
    4ef6:  4eba f7b2      	jsr %pc@(0x46aa)                      ; MQGetMemBasePtr
    4efa:  2648           	moveal %a0,%a3                        
    4efc:  707f           	moveq #127,%d0                        
    4efe:  2d40 fffc      	movel %d0,%fp@(-4)                    
    4f02:  486a 038c      	pea %a2@(908)                         
    4f06:  4eba 0312      	jsr %pc@(0x521a)                      ; sREGR
    4f0a:  c0ae fffc      	andl %fp@(-4),%d0                     
    4f0e:  7240           	moveq #64,%d1                         
    4f10:  b081           	cmpl %d1,%d0                          
    4f12:  584f           	addqw #4,%sp                          
    4f14:  6600 00a8      	bnew 0x4fbe                           ; ↓ label_2
    4f18:  486a 0408      	pea %a2@(1032)                        
    4f1c:  4eba 02fc      	jsr %pc@(0x521a)                      ; sREGR
    4f20:  0280 ffff ff00 	andil #-256,%d0                       
    4f26:  0c80 0380 0000 	cmpil #58720256,%d0                   
    4f2c:  584f           	addqw #4,%sp                          
    4f2e:  6600 008e      	bnew 0x4fbe                           ; ↓ label_2
    4f32:  486a 022c      	pea %a2@(556)                         
    4f36:  4eba 02e2      	jsr %pc@(0x521a)                      ; sREGR
    4f3a:  2800           	movel %d0,%d4                         
    4f3c:  486a 0228      	pea %a2@(552)                         
    4f40:  4eba 02d8      	jsr %pc@(0x521a)                      ; sREGR
    4f44:  2600           	movel %d0,%d3                         
    4f46:  2003           	movel %d3,%d0                         
    4f48:  0280 4000 0000 	andil #1073741824,%d0                 
    4f4e:  504f           	addqw #8,%sp                          
    4f50:  6708           	beqs 0x4f5a                           ; ↓ label_0
    4f52:  0283 3fff ffff 	andil #1073741823,%d3                 
    4f58:  e28b           	lsrl #1,%d3                           

  label_0:
    4f5a:  203c 0001 9000 	movel #102400,%d0                     
    4f60:  2203           	movel %d3,%d1                         
    4f62:  4eba b14c      	jsr %pc@(0xb0)                        ; BBHBf6
    4f66:  4840           	swap %d0                              
    4f68:  4240           	clrw %d0                              
    4f6a:  8680           	orl %d0,%d3                           
    4f6c:  ba84           	cmpl %d4,%d5                          
    4f6e:  670c           	beqs 0x4f7c                           ; ↓ label_1
    4f70:  2f05           	movel %d5,%sp@-                       
    4f72:  486a 022c      	pea %a2@(556)                         
    4f76:  4eba 026c      	jsr %pc@(0x51e4)                      ; sREGW
    4f7a:  504f           	addqw #8,%sp                          

  label_1:
    4f7c:  2f03           	movel %d3,%sp@-                       
    4f7e:  486a 0204      	pea %a2@(516)                         
    4f82:  4eba 0260      	jsr %pc@(0x51e4)                      ; sREGW
    4f86:  42a7           	clrl %sp@-                            
    4f88:  486a 0208      	pea %a2@(520)                         
    4f8c:  4eba 0256      	jsr %pc@(0x51e4)                      ; sREGW
    4f90:  42a7           	clrl %sp@-                            
    4f92:  486a 0248      	pea %a2@(584)                         
    4f96:  4eba 024c      	jsr %pc@(0x51e4)                      ; sREGW
    4f9a:  2f3c 4000 02f0 	movel #1073742576,%sp@-               
    4fa0:  486a 0200      	pea %a2@(512)                         
    4fa4:  4eba 023e      	jsr %pc@(0x51e4)                      ; sREGW
    4fa8:  ba84           	cmpl %d4,%d5                          
    4faa:  4fef 0020      	lea %sp@(32),%sp                      
    4fae:  6768           	beqs 0x5018                           ; ↓ label_7
    4fb0:  2f04           	movel %d4,%sp@-                       
    4fb2:  486a 022c      	pea %a2@(556)                         
    4fb6:  4eba 022c      	jsr %pc@(0x51e4)                      ; sREGW
    4fba:  504f           	addqw #8,%sp                          
    4fbc:  605a           	bras 0x5018                           ; ↓ label_7

  label_2:
    4fbe:  486a 0228      	pea %a2@(552)                         
    4fc2:  4eba 0256      	jsr %pc@(0x521a)                      ; sREGR
    4fc6:  2800           	movel %d0,%d4                         
    4fc8:  2004           	movel %d4,%d0                         
    4fca:  0280 4000 0000 	andil #1073741824,%d0                 
    4fd0:  584f           	addqw #4,%sp                          
    4fd2:  6708           	beqs 0x4fdc                           ; ↓ label_3
    4fd4:  0284 3fff ffff 	andil #1073741823,%d4                 
    4fda:  e28c           	lsrl #1,%d4                           

  label_3:
    4fdc:  2004           	movel %d4,%d0                         
    4fde:  7250           	moveq #80,%d1                         
    4fe0:  4eba b09c      	jsr %pc@(0x7e)                        ; anonymous_function_0
    4fe4:  2800           	movel %d0,%d4                         
    4fe6:  244b           	moveal %a3,%a2                        
    4fe8:  7600           	moveq #0,%d3                          
    4fea:  4a84           	tstl %d4                              
    4fec:  632a           	blss 0x5018                           ; ↓ label_7
    4fee:  2a04           	movel %d4,%d5                         
    4ff0:  5185           	subql #8,%d5                          
    4ff2:  7008           	moveq #8,%d0                          
    4ff4:  b880           	cmpl %d0,%d4                          
    4ff6:  631c           	blss 0x5014                           ; ↓ label_6

  label_4:
    4ff8:  429a           	clrl %a2@+                            
    4ffa:  429a           	clrl %a2@+                            
    4ffc:  429a           	clrl %a2@+                            
    4ffe:  429a           	clrl %a2@+                            
    5000:  429a           	clrl %a2@+                            
    5002:  429a           	clrl %a2@+                            
    5004:  429a           	clrl %a2@+                            
    5006:  429a           	clrl %a2@+                            
    5008:  5083           	addql #8,%d3                          
    500a:  b685           	cmpl %d5,%d3                          
    500c:  65ea           	bcss 0x4ff8                           ; ↑ label_4
    500e:  6004           	bras 0x5014                           ; ↓ label_6

  label_5:
    5010:  429a           	clrl %a2@+                            
    5012:  5283           	addql #1,%d3                          

  label_6:
    5014:  b684           	cmpl %d4,%d3                          
    5016:  65f8           	bcss 0x5010                           ; ↑ label_5

  label_7:
    5018:  4cdf 0c38      	moveml %sp@+,%d3-%d5/%a2-%a3          
    501c:  4e5e           	unlk %fp                              
    501e:  4e75           	rts                                   
```

# WriteRegs: 0x502a - 0x5124

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???

```
    502a:  4e56 0000      	linkw %fp,#0                          
    502e:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    5032:  2c2e 000e      	movel %fp@(14),%d6                    
    5036:  4eba f652      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    503a:  2a08           	movel %a0,%d5                         
    503c:  7e00           	moveq #0,%d7                          
    503e:  4a6e 000c      	tstw %fp@(12)                         
    5042:  6f00 00da      	blew 0x511e                           ; ↓ label_10
    5046:  266e 0008      	moveal %fp@(8),%a3                    

  label_0:
    504a:  362b 000e      	movew %a3@(14),%d3                    
    504e:  3003           	movew %d3,%d0                         
    5050:  0240 0001      	andiw #1,%d0                          
    5054:  6700 00ba      	beqw 0x5110                           ; ↓ label_9
    5058:  3003           	movew %d3,%d0                         
    505a:  0240 0002      	andiw #2,%d0                          
    505e:  6728           	beqs 0x5088                           ; ↓ label_3
    5060:  244b           	moveal %a3,%a2                        

  label_1:
    5062:  2006           	movel %d6,%d0                         
    5064:  d092           	addl %a2@,%d0                         
    5066:  2045           	moveal %d5,%a0                        
    5068:  4870 0800      	pea %a0@(00000000,%d0:l)              
    506c:  4eba 01ac      	jsr %pc@(0x521a)                      ; sREGR
    5070:  2600           	movel %d0,%d3                         
    5072:  282a 0008      	movel %a2@(8),%d4                     
    5076:  584f           	addqw #4,%sp                          
    5078:  6702           	beqs 0x507c                           ; ↓ label_2
    507a:  c684           	andl %d4,%d3                          

  label_2:
    507c:  282a 0004      	movel %a2@(4),%d4                     
    5080:  b684           	cmpl %d4,%d3                          
    5082:  6700 008c      	beqw 0x5110                           ; ↓ label_9
    5086:  60da           	bras 0x5062                           ; ↑ label_1

  label_3:
    5088:  3003           	movew %d3,%d0                         
    508a:  0240 0004      	andiw #4,%d0                          
    508e:  6706           	beqs 0x5096                           ; ↓ label_4
    5090:  4eba 00a0      	jsr %pc@(0x5132)                      ; InVBlank
    5094:  6038           	bras 0x50ce                           ; ↓ label_6

  label_4:
    5096:  262b 0004      	movel %a3@(4),%d3                     
    509a:  4aab 0008      	tstl %a3@(8)                          
    509e:  671c           	beqs 0x50bc                           ; ↓ label_5
    50a0:  2006           	movel %d6,%d0                         
    50a2:  d093           	addl %a3@,%d0                         
    50a4:  2045           	moveal %d5,%a0                        
    50a6:  4870 0800      	pea %a0@(00000000,%d0:l)              
    50aa:  4eba 016e      	jsr %pc@(0x521a)                      ; sREGR
    50ae:  2800           	movel %d0,%d4                         
    50b0:  2604           	movel %d4,%d3                         
    50b2:  c6ab 0008      	andl %a3@(8),%d3                      
    50b6:  86ab 0004      	orl %a3@(4),%d3                       
    50ba:  584f           	addqw #4,%sp                          

  label_5:
    50bc:  2f03           	movel %d3,%sp@-                       
    50be:  2006           	movel %d6,%d0                         
    50c0:  d093           	addl %a3@,%d0                         
    50c2:  2045           	moveal %d5,%a0                        
    50c4:  4870 0800      	pea %a0@(00000000,%d0:l)              
    50c8:  4eba 011a      	jsr %pc@(0x51e4)                      ; sREGW
    50cc:  504f           	addqw #8,%sp                          

  label_6:
    50ce:  382b 000c      	movew %a3@(12),%d4                    
    50d2:  673c           	beqs 0x5110                           ; ↓ label_9
    50d4:  302b 000e      	movew %a3@(14),%d0                    
    50d8:  0240 0008      	andiw #8,%d0                          
    50dc:  6716           	beqs 0x50f4                           ; ↓ label_8
    50de:  3604           	movew %d4,%d3                         
    50e0:  3003           	movew %d3,%d0                         
    50e2:  5343           	subqw #1,%d3                          
    50e4:  4a40           	tstw %d0                              
    50e6:  670c           	beqs 0x50f4                           ; ↓ label_8

  label_7:
    50e8:  4eba 0048      	jsr %pc@(0x5132)                      ; InVBlank
    50ec:  3003           	movew %d3,%d0                         
    50ee:  5343           	subqw #1,%d3                          
    50f0:  4a40           	tstw %d0                              
    50f2:  66f4           	bnes 0x50e8                           ; ↑ label_7

  label_8:
    50f4:  362b 000c      	movew %a3@(12),%d3                    
    50f8:  6716           	beqs 0x5110                           ; ↓ label_9
    50fa:  7000           	moveq #0,%d0                          
    50fc:  3003           	movew %d3,%d0                         
    50fe:  223c 0000 03e8 	movel #1000,%d1                       
    5104:  4eba af78      	jsr %pc@(0x7e)                        ; anonymous_function_0
    5108:  2f00           	movel %d0,%sp@-                       
    510a:  4eba b47a      	jsr %pc@(0x586)                       ; HwrDelay
    510e:  584f           	addqw #4,%sp                          

  label_9:
    5110:  508b           	addql #8,%a3                          
    5112:  508b           	addql #8,%a3                          
    5114:  5247           	addqw #1,%d7                          
    5116:  be6e 000c      	cmpw %fp@(12),%d7                     
    511a:  6d00 ff2e      	bltw 0x504a                           ; ↑ label_0

  label_10:
    511e:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    5122:  4e5e           	unlk %fp                              
    5124:  4e75           	rts                                   
```

# InVBlank: 0x5132 - 0x51d6

Locals:

   * `%fp@(-4)`     : ???

```
    5132:  4e56 fffc      	linkw %fp,#-4                         
    5136:  48e7 1f20      	moveml %d3-%d7/%a2,%sp@-              
    513a:  4eba f54e      	jsr %pc@(0x468a)                      ; MQGetRegBasePtr
    513e:  2448           	moveal %a0,%a2                        
    5140:  707f           	moveq #127,%d0                        
    5142:  2d40 fffc      	movel %d0,%fp@(-4)                    
    5146:  486a 038c      	pea %a2@(908)                         
    514a:  4eba 00ce      	jsr %pc@(0x521a)                      ; sREGR
    514e:  c0ae fffc      	andl %fp@(-4),%d0                     
    5152:  7240           	moveq #64,%d1                         
    5154:  b081           	cmpl %d1,%d0                          
    5156:  584f           	addqw #4,%sp                          
    5158:  6674           	bnes 0x51ce                           ; ↓ label_3
    515a:  486a 0408      	pea %a2@(1032)                        
    515e:  4eba 00ba      	jsr %pc@(0x521a)                      ; sREGR
    5162:  0280 ffff ff00 	andil #-256,%d0                       
    5168:  0c80 0380 0000 	cmpil #58720256,%d0                   
    516e:  584f           	addqw #4,%sp                          
    5170:  665c           	bnes 0x51ce                           ; ↓ label_3
    5172:  486a 01f4      	pea %a2@(500)                         
    5176:  4eba 00a2      	jsr %pc@(0x521a)                      ; sREGR
    517a:  2a00           	movel %d0,%d5                         
    517c:  2605           	movel %d5,%d3                         
    517e:  0283 0000 03ff 	andil #1023,%d3                       
    5184:  2e03           	movel %d3,%d7                         
    5186:  2805           	movel %d5,%d4                         
    5188:  720c           	moveq #12,%d1                         
    518a:  e2ac           	lsrl %d1,%d4                          
    518c:  0284 0000 03ff 	andil #1023,%d4                       
    5192:  2c04           	movel %d4,%d6                         
    5194:  be46           	cmpw %d6,%d7                          
    5196:  584f           	addqw #4,%sp                          
    5198:  6f18           	bles 0x51b2                           ; ↓ label_1

  label_0:
    519a:  486a 01f4      	pea %a2@(500)                         
    519e:  4eba 007a      	jsr %pc@(0x521a)                      ; sREGR
    51a2:  2a00           	movel %d0,%d5                         
    51a4:  2605           	movel %d5,%d3                         
    51a6:  0283 0000 03ff 	andil #1023,%d3                       
    51ac:  b644           	cmpw %d4,%d3                          
    51ae:  584f           	addqw #4,%sp                          
    51b0:  6ee8           	bgts 0x519a                           ; ↑ label_0

  label_1:
    51b2:  b644           	cmpw %d4,%d3                          
    51b4:  6c18           	bges 0x51ce                           ; ↓ label_3

  label_2:
    51b6:  486a 01f4      	pea %a2@(500)                         
    51ba:  4eba 005e      	jsr %pc@(0x521a)                      ; sREGR
    51be:  2600           	movel %d0,%d3                         
    51c0:  2a03           	movel %d3,%d5                         
    51c2:  0285 0000 03ff 	andil #1023,%d5                       
    51c8:  ba44           	cmpw %d4,%d5                          
    51ca:  584f           	addqw #4,%sp                          
    51cc:  6de8           	blts 0x51b6                           ; ↑ label_2

  label_3:
    51ce:  7001           	moveq #1,%d0                          
    51d0:  4cdf 04f8      	moveml %sp@+,%d3-%d7/%a2              
    51d4:  4e5e           	unlk %fp                              
    51d6:  4e75           	rts                                   
```

# sREGW: 0x51e4 - 0x5210

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    51e4:  4e56 0000      	linkw %fp,#0                          
    51e8:  2f05           	movel %d5,%sp@-                       
    51ea:  2a2e 000c      	movel %fp@(12),%d5                    
    51ee:  3005           	movew %d5,%d0                         
    51f0:  3400           	movew %d0,%d2                         
    51f2:  e15a           	rolw #8,%d2                           
    51f4:  206e 0008      	moveal %fp@(8),%a0                    
    51f8:  3082           	movew %d2,%a0@                        
    51fa:  2005           	movel %d5,%d0                         
    51fc:  7210           	moveq #16,%d1                         
    51fe:  e3b8           	roll %d1,%d0                          
    5200:  3400           	movew %d0,%d2                         
    5202:  e15a           	rolw #8,%d2                           
    5204:  206e 0008      	moveal %fp@(8),%a0                    
    5208:  3142 0002      	movew %d2,%a0@(2)                     
    520c:  2a1f           	movel %sp@+,%d5                       
    520e:  4e5e           	unlk %fp                              
    5210:  4e75           	rts                                   
```

# sREGR: 0x521a - 0x524e

Parameters:

   * `%fp@(8)`      : ???

```
    521a:  4e56 0000      	linkw %fp,#0                          
    521e:  2f05           	movel %d5,%sp@-                       
    5220:  2f04           	movel %d4,%sp@-                       
    5222:  206e 0008      	moveal %fp@(8),%a0                    
    5226:  3410           	movew %a0@,%d2                        
    5228:  3802           	movew %d2,%d4                         
    522a:  e15c           	rolw #8,%d4                           
    522c:  206e 0008      	moveal %fp@(8),%a0                    
    5230:  3428 0002      	movew %a0@(2),%d2                     
    5234:  3a02           	movew %d2,%d5                         
    5236:  e15d           	rolw #8,%d5                           
    5238:  7400           	moveq #0,%d2                          
    523a:  3405           	movew %d5,%d2                         
    523c:  7210           	moveq #16,%d1                         
    523e:  e3ba           	roll %d1,%d2                          
    5240:  7000           	moveq #0,%d0                          
    5242:  3004           	movew %d4,%d0                         
    5244:  8480           	orl %d0,%d2                           
    5246:  2002           	movel %d2,%d0                         
    5248:  281f           	movel %sp@+,%d4                       
    524a:  2a1f           	movel %sp@+,%d5                       
    524c:  4e5e           	unlk %fp                              
    524e:  4e75           	rts                                   
```

# /: 0x5258 - 0x5284

```
    5258:  40e7           	movew %sr,%sp@-                       
    525a:  007c 0700      	oriw #1792,%sr                        
    525e:  41f8 f802      	lea 0xfffffffffffff802,%a0            
    5262:  43f8 f800      	lea 0xfffffffffffff800,%a1            
    5266:  3f10           	movew %a0@,%sp@-                      
    5268:  4251           	clrw %a1@                             
    526a:  30bc 2247      	movew #8775,%a0@                      
    526e:  0238 ffdf f449 	andib #-33,0xfffffffffffff449         
    5274:  6110           	bsrs 0x5286                           
    5276:  610e           	bsrs 0x5286                           
    5278:  610c           	bsrs 0x5286                           
    527a:  0038 0020 f449 	orib #32,0xfffffffffffff449           
    5280:  309f           	movew %sp@+,%a0@                      
    5282:  46df           	movew %sp@+,%sr                       
    5284:  4e75           	rts                                   
```

# PrvAverageBattery: 0x54dc - 0x554e

```
    54dc:  4e56 0000      	linkw %fp,#0                          
    54e0:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-          
    54e4:  2478 02f6      	moveal 0x2f6,%a2                      
    54e8:  2678 02c2      	moveal 0x2c2,%a3                      
    54ec:  7a00           	moveq #0,%d5                          
    54ee:  1a38 0139      	moveb 0x139,%d5                       
    54f2:  4e4f           	trap #15                              
    54f4:  a248           	0121110                               ; sysTrapHwrBatteryLevel
    54f6:  3600           	movew %d0,%d3                         
    54f8:  382a 0042      	movew %a2@(66),%d4                    
    54fc:  6702           	beqs 0x5500                           ; ↓ label_0
    54fe:  3604           	movew %d4,%d3                         

  label_0:
    5500:  3805           	movew %d5,%d4                         
    5502:  e54c           	lslw #2,%d4                           
    5504:  d845           	addw %d5,%d4                          
    5506:  d845           	addw %d5,%d4                          
    5508:  d845           	addw %d5,%d4                          
    550a:  d843           	addw %d3,%d4                          
    550c:  5844           	addqw #4,%d4                          
    550e:  e64c           	lsrw #3,%d4                           
    5510:  302b 001e      	movew %a3@(30),%d0                    
    5514:  e448           	lsrw #2,%d0                           
    5516:  3c00           	movew %d0,%d6                         
    5518:  d045           	addw %d5,%d0                          
    551a:  b640           	cmpw %d0,%d3                          
    551c:  6302           	blss 0x5520                           ; ↓ label_1
    551e:  3803           	movew %d3,%d4                         

  label_1:
    5520:  3005           	movew %d5,%d0                         
    5522:  9046           	subw %d6,%d0                          
    5524:  b640           	cmpw %d0,%d3                          
    5526:  6402           	bccs 0x552a                           ; ↓ label_2
    5528:  3803           	movew %d3,%d4                         

  label_2:
    552a:  7000           	moveq #0,%d0                          
    552c:  1038 0139      	moveb 0x139,%d0                       
    5530:  b044           	cmpw %d4,%d0                          
    5532:  660e           	bnes 0x5542                           ; ↓ label_4
    5534:  b644           	cmpw %d4,%d3                          
    5536:  6304           	blss 0x553c                           ; ↓ label_3
    5538:  5244           	addqw #1,%d4                          
    553a:  6006           	bras 0x5542                           ; ↓ label_4

  label_3:
    553c:  b644           	cmpw %d4,%d3                          
    553e:  6402           	bccs 0x5542                           ; ↓ label_4
    5540:  5344           	subqw #1,%d4                          

  label_4:
    5542:  11c4 0139      	moveb %d4,0x139                       
    5546:  3003           	movew %d3,%d0                         
    5548:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3          
    554c:  4e5e           	unlk %fp                              
    554e:  4e75           	rts                                   
```

# PrvAveragePercent: 0x5564 - 0x55ba

Parameters:

   * `%fp@(8)`      : ???

```
    5564:  4e56 0000      	linkw %fp,#0                          
    5568:  2f05           	movel %d5,%sp@-                       
    556a:  3a2e 0008      	movew %fp@(8),%d5                     
    556e:  7400           	moveq #0,%d2                          
    5570:  1438 02fa      	moveb 0x2fa,%d2                       
    5574:  3202           	movew %d2,%d1                         
    5576:  e549           	lslw #2,%d1                           
    5578:  d242           	addw %d2,%d1                          
    557a:  d242           	addw %d2,%d1                          
    557c:  d242           	addw %d2,%d1                          
    557e:  d245           	addw %d5,%d1                          
    5580:  5841           	addqw #4,%d1                          
    5582:  e649           	lsrw #3,%d1                           
    5584:  7019           	moveq #25,%d0                         
    5586:  d042           	addw %d2,%d0                          
    5588:  ba40           	cmpw %d0,%d5                          
    558a:  6302           	blss 0x558e                           ; ↓ label_0
    558c:  3205           	movew %d5,%d1                         

  label_0:
    558e:  3002           	movew %d2,%d0                         
    5590:  0640 ffe7      	addiw #-25,%d0                        
    5594:  ba40           	cmpw %d0,%d5                          
    5596:  6402           	bccs 0x559a                           ; ↓ label_1
    5598:  3205           	movew %d5,%d1                         

  label_1:
    559a:  7000           	moveq #0,%d0                          
    559c:  1038 02fa      	moveb 0x2fa,%d0                       
    55a0:  b041           	cmpw %d1,%d0                          
    55a2:  660e           	bnes 0x55b2                           ; ↓ label_3
    55a4:  ba41           	cmpw %d1,%d5                          
    55a6:  6304           	blss 0x55ac                           ; ↓ label_2
    55a8:  5241           	addqw #1,%d1                          
    55aa:  6006           	bras 0x55b2                           ; ↓ label_3

  label_2:
    55ac:  ba41           	cmpw %d1,%d5                          
    55ae:  6402           	bccs 0x55b2                           ; ↓ label_3
    55b0:  5341           	subqw #1,%d1                          

  label_3:
    55b2:  11c1 02fa      	moveb %d1,0x2fa                       
    55b6:  2a1f           	movel %sp@+,%d5                       
    55b8:  4e5e           	unlk %fp                              
    55ba:  4e75           	rts                                   
```

# PrvGetBatteryPercent: 0x55d0 - 0x56f0

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???
   * `%fp@(-6)`     : ???

```
    55d0:  4e56 fffa      	linkw %fp,#-6                         
    55d4:  48e7 0f00      	moveml %d4-%d7,%sp@-                  
    55d8:  382e 0008      	movew %fp@(8),%d4                     
    55dc:  2078 02f6      	moveal 0x2f6,%a0                      
    55e0:  2278 02c2      	moveal 0x2c2,%a1                      
    55e4:  4a2e 000a      	tstb %fp@(10)                         
    55e8:  6620           	bnes 0x560a                           ; ↓ label_3
    55ea:  3429 003e      	movew %a1@(62),%d2                    
    55ee:  b842           	cmpw %d2,%d4                          
    55f0:  6302           	blss 0x55f4                           ; ↓ label_0
    55f2:  3802           	movew %d2,%d4                         

  label_0:
    55f4:  3028 0004      	movew %a0@(4),%d0                     
    55f8:  0640 fe0c      	addiw #-500,%d0                       
    55fc:  6708           	beqs 0x5606                           ; ↓ label_1
    55fe:  4a44           	tstw %d4                              
    5600:  6704           	beqs 0x5606                           ; ↓ label_1
    5602:  7400           	moveq #0,%d2                          
    5604:  6002           	bras 0x5608                           ; ↓ label_2

  label_1:
    5606:  7400           	moveq #0,%d2                          

  label_2:
    5608:  d842           	addw %d2,%d4                          

  label_3:
    560a:  740a           	moveq #10,%d2                         
    560c:  2049           	moveal %a1,%a0                        
    560e:  41e8 0014      	lea %a0@(20),%a0                      
    5612:  6004           	bras 0x5618                           ; ↓ label_5

  label_4:
    5614:  5588           	subql #2,%a0                          
    5616:  5302           	subqb #1,%d2                          

  label_5:
    5618:  b868 002a      	cmpw %a0@(42),%d4                     
    561c:  6404           	bccs 0x5622                           ; ↓ label_6
    561e:  4a02           	tstb %d2                              
    5620:  66f2           	bnes 0x5614                           ; ↑ label_4

  label_6:
    5622:  0c02 000a      	cmpib #10,%d2                         
    5626:  6504           	bcss 0x562c                           ; ↓ label_7
    5628:  7464           	moveq #100,%d2                        
    562a:  604e           	bras 0x567a                           ; ↓ label_9

  label_7:
    562c:  7000           	moveq #0,%d0                          
    562e:  1002           	moveb %d2,%d0                         
    5630:  d080           	addl %d0,%d0                          
    5632:  3d71 082a fffe 	movew %a1@(0000002a,%d0:l),%fp@(-2)   
    5638:  3a31 082a      	movew %a1@(0000002a,%d0:l),%d5        
    563c:  7000           	moveq #0,%d0                          
    563e:  1002           	moveb %d2,%d0                         
    5640:  3d40 fffc      	movew %d0,%fp@(-4)                    
    5644:  5240           	addqw #1,%d0                          
    5646:  48c0           	extl %d0                              
    5648:  d080           	addl %d0,%d0                          
    564a:  3e31 082a      	movew %a1@(0000002a,%d0:l),%d7        
    564e:  b86e fffe      	cmpw %fp@(-2),%d4                     
    5652:  6404           	bccs 0x5658                           ; ↓ label_8
    5654:  7400           	moveq #0,%d2                          
    5656:  6022           	bras 0x567a                           ; ↓ label_9

  label_8:
    5658:  3007           	movew %d7,%d0                         
    565a:  9045           	subw %d5,%d0                          
    565c:  3c00           	movew %d0,%d6                         
    565e:  3006           	movew %d6,%d0                         
    5660:  e248           	lsrw #1,%d0                           
    5662:  3404           	movew %d4,%d2                         
    5664:  9445           	subw %d5,%d2                          
    5666:  c4fc 000a      	muluw #10,%d2                         
    566a:  d440           	addw %d0,%d2                          
    566c:  7000           	moveq #0,%d0                          
    566e:  3002           	movew %d2,%d0                         
    5670:  80c6           	divuw %d6,%d0                         
    5672:  740a           	moveq #10,%d2                         
    5674:  c5ee fffc      	mulsw %fp@(-4),%d2                    
    5678:  d440           	addw %d0,%d2                          

  label_9:
    567a:  0c2e 0001 000e 	cmpib #1,%fp@(14)                     
    5680:  665e           	bnes 0x56e0                           ; ↓ label_14
    5682:  3802           	movew %d2,%d4                         
    5684:  7064           	moveq #100,%d0                        
    5686:  c0ee 000c      	muluw %fp@(12),%d0                    
    568a:  7200           	moveq #0,%d1                          
    568c:  3200           	movew %d0,%d1                         
    568e:  82fc 00ff      	divuw #255,%d1                        
    5692:  3d41 000c      	movew %d1,%fp@(12)                    
    5696:  7064           	moveq #100,%d0                        
    5698:  9041           	subw %d1,%d0                          
    569a:  c0fc 000a      	muluw #10,%d0                         
    569e:  7c00           	moveq #0,%d6                          
    56a0:  3c00           	movew %d0,%d6                         
    56a2:  8cfc 0064      	divuw #100,%d6                        
    56a6:  3e06           	movew %d6,%d7                         
    56a8:  3a04           	movew %d4,%d5                         
    56aa:  5b45           	subqw #5,%d5                          
    56ac:  3d45 fffa      	movew %d5,%fp@(-6)                    
    56b0:  be45           	cmpw %d5,%d7                          
    56b2:  6f0c           	bles 0x56c0                           ; ↓ label_11
    56b4:  4a45           	tstw %d5                              
    56b6:  6c04           	bges 0x56bc                           ; ↓ label_10
    56b8:  7800           	moveq #0,%d4                          
    56ba:  6006           	bras 0x56c2                           ; ↓ label_12

  label_10:
    56bc:  3805           	movew %d5,%d4                         
    56be:  6002           	bras 0x56c2                           ; ↓ label_12

  label_11:
    56c0:  3806           	movew %d6,%d4                         

  label_12:
    56c2:  3002           	movew %d2,%d0                         
    56c4:  9044           	subw %d4,%d0                          
    56c6:  c0fc 0064      	muluw #100,%d0                        
    56ca:  7200           	moveq #0,%d1                          
    56cc:  3200           	movew %d0,%d1                         
    56ce:  82fc 005a      	divuw #90,%d1                         
    56d2:  3a01           	movew %d1,%d5                         
    56d4:  0c41 0064      	cmpiw #100,%d1                        
    56d8:  6404           	bccs 0x56de                           ; ↓ label_13
    56da:  3405           	movew %d5,%d2                         
    56dc:  6002           	bras 0x56e0                           ; ↓ label_14

  label_13:
    56de:  7464           	moveq #100,%d2                        

  label_14:
    56e0:  0c42 005a      	cmpiw #90,%d2                         
    56e4:  6502           	bcss 0x56e8                           ; ↓ label_15
    56e6:  7464           	moveq #100,%d2                        

  label_15:
    56e8:  1002           	moveb %d2,%d0                         
    56ea:  4cdf 00f0      	moveml %sp@+,%d4-%d7                  
    56ee:  4e5e           	unlk %fp                              
    56f0:  4e75           	rts                                   
```

# PrvUpdateCritWarnTimer: 0x570a - 0x578a

```
    570a:  4e56 0000      	linkw %fp,#0                          
    570e:  2f04           	movel %d4,%sp@-                       
    5710:  2078 02c2      	moveal 0x2c2,%a0                      
    5714:  2278 02f6      	moveal 0x2f6,%a1                      
    5718:  4a38 02d1      	tstb 0x2d1                            
    571c:  661a           	bnes 0x5738                           ; ↓ label_0
    571e:  31f8 030e 02d2 	movew 0x30e,0x2d2                     
    5724:  11fc 0001 02d1 	moveb #1,0x2d1                        
    572a:  21e8 001a 02d4 	movel %a0@(26),0x2d4                  
    5730:  21f8 0254 02d8 	movel 0x254,0x2d8                     
    5736:  604e           	bras 0x5786                           ; ↓ label_3

  label_0:
    5738:  3828 0018      	movew %a0@(24),%d4                    
    573c:  2438 0254      	movel 0x254,%d2                       
    5740:  3029 0004      	movew %a1@(4),%d0                     
    5744:  0640 00fa      	addiw #250,%d0                        
    5748:  7200           	moveq #0,%d1                          
    574a:  3200           	movew %d0,%d1                         
    574c:  82fc 01f4      	divuw #500,%d1                        
    5750:  7000           	moveq #0,%d0                          
    5752:  3001           	movew %d1,%d0                         
    5754:  91b8 02d4      	subl %d0,0x2d4                        
    5758:  21c2 02d8      	movel %d2,0x2d8                       
    575c:  4ab8 02d4      	tstl 0x2d4                            
    5760:  6c04           	bges 0x5766                           ; ↓ label_1
    5762:  42b8 02d4      	clrl 0x2d4                            

  label_1:
    5766:  3438 030e      	movew 0x30e,%d2                       
    576a:  3038 030e      	movew 0x30e,%d0                       
    576e:  b078 02d2      	cmpw 0x2d2,%d0                        
    5772:  6404           	bccs 0x5778                           ; ↓ label_2
    5774:  4278 02d2      	clrw 0x2d2                            

  label_2:
    5778:  3038 02d2      	movew 0x2d2,%d0                       
    577c:  d044           	addw %d4,%d0                          
    577e:  b042           	cmpw %d2,%d0                          
    5780:  6404           	bccs 0x5786                           ; ↓ label_3
    5782:  42b8 02d4      	clrl 0x2d4                            

  label_3:
    5786:  281f           	movel %sp@+,%d4                       
    5788:  4e5e           	unlk %fp                              
    578a:  4e75           	rts                                   
```

# PrvLowBatteryShutdownNow: 0x57a6 - 0x57d4

```
    57a6:  4e56 0000      	linkw %fp,#0                          
    57aa:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    57ae:  0240 0080      	andiw #128,%d0                        
    57b2:  661e           	bnes 0x57d2                           ; ↓ label_0
    57b4:  31fc ffff f304 	movew #-1,0xfffffffffffff304          
    57ba:  0078 f8bf f306 	oriw #-1857,0xfffffffffffff306        
    57c0:  11fc 0001 02ca 	moveb #1,0x2ca                        
    57c6:  1f3c 0001      	moveb #1,%sp@-                        
    57ca:  4227           	clrb %sp@-                            
    57cc:  4e4f           	trap #15                              
    57ce:  a240           	0121100                               ; sysTrapHwrSleep
    57d0:  584f           	addqw #4,%sp                          

  label_0:
    57d2:  4e5e           	unlk %fp                              
    57d4:  4e75           	rts                                   
```

# HwrBattery: 0x57f2 - 0x5d5e

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(1340)`   : ???

Locals:

   * `%fp@(-4)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-12)`    : ???

```
    57f2:  4e56 fff4      	linkw %fp,#-12                        
    57f6:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    57fa:  266e 000a      	moveal %fp@(10),%a3                   
    57fe:  7c00           	moveq #0,%d6                          
    5800:  2478 02f6      	moveal 0x2f6,%a2                      
    5804:  302e 0008      	movew %fp@(8),%d0                     
    5808:  0c40 0013      	cmpiw #19,%d0                         
    580c:  6200 0544      	bhiw 0x5d52                           ; ↓ label_30
    5810:  d040           	addw %d0,%d0                          
    5812:  303b 0006      	movew %pc@(0x581a,%d0:w),%d0          
    5816:  4efb 0002      	jmp %pc@(0x581a,%d0:w)                
    581a:  014e 053c      	movepl %fp@(1340),%d0                 
    581e:  02a2 053c 053c 	andil #87819580,%a2@-                 
    5824:  053c 053c      	btst %d2,#60                          
    5828:  0538 053c      	btst %d2,0x53c                        
    582c:  053c 053c      	btst %d2,#60                          
    5830:  02d4           	01324                                 
    5832:  0304           	btst %d1,%d4                          
    5834:  053c 0538      	btst %d2,#56                          
    5838:  03a4           	bclr %d1,%a4@-                        
    583a:  0506           	btst %d2,%d6                          
    583c:  0028 0118 0140 	orib #24,%a0@(320)                    
    5842:  2478 02c2      	moveal 0x2c2,%a2                      
    5846:  4e4f           	trap #15                              
    5848:  a34d           	0121515                               ; sysTrapHwrDockStatus
    584a:  0800 0002      	btst #2,%d0                           
    584e:  6708           	beqs 0x5858                           ; ↓ label_0
    5850:  11fc 0001 02dc 	moveb #1,0x2dc                        
    5856:  6010           	bras 0x5868                           ; ↓ label_1

  label_0:
    5858:  4a38 02dc      	tstb 0x2dc                            
    585c:  670a           	beqs 0x5868                           ; ↓ label_1
    585e:  11fc 0001 02dd 	moveb #1,0x2dd                        
    5864:  4238 02dc      	clrb 0x2dc                            

  label_1:
    5868:  31f8 0310 0136 	movew 0x310,0x136                     
    586e:  4eba fc6c      	jsr %pc@(0x54dc)                      ; PrvAverageBattery
    5872:  3e00           	movew %d0,%d7                         
    5874:  382a 001e      	movew %a2@(30),%d4                    
    5878:  3a07           	movew %d7,%d5                         
    587a:  da6a 0020      	addw %a2@(32),%d5                     
    587e:  cafc 0064      	muluw #100,%d5                        
    5882:  3204           	movew %d4,%d1                         
    5884:  e249           	lsrw #1,%d1                           
    5886:  d245           	addw %d5,%d1                          
    5888:  7a00           	moveq #0,%d5                          
    588a:  3a01           	movew %d1,%d5                         
    588c:  8ac4           	divuw %d4,%d5                         
    588e:  4227           	clrb %sp@-                            
    5890:  4267           	clrw %sp@-                            
    5892:  1f38 0265      	moveb 0x265,%sp@-                     
    5896:  3f05           	movew %d5,%sp@-                       
    5898:  4eba fd36      	jsr %pc@(0x55d0)                      ; PrvGetBatteryPercent
    589c:  1800           	moveb %d0,%d4                         
    589e:  7000           	moveq #0,%d0                          
    58a0:  1004           	moveb %d4,%d0                         
    58a2:  3f00           	movew %d0,%sp@-                       
    58a4:  4eba fcbe      	jsr %pc@(0x5564)                      ; PrvAveragePercent
    58a8:  4eba fefc      	jsr %pc@(0x57a6)                      ; PrvLowBatteryShutdownNow
    58ac:  1038 0139      	moveb 0x139,%d0                       
    58b0:  b038 0138      	cmpb 0x138,%d0                        
    58b4:  4fef 000a      	lea %sp@(10),%sp                      
    58b8:  6416           	bccs 0x58d0                           ; ↓ label_2
    58ba:  11fc 0001 02ca 	moveb #1,0x2ca                        
    58c0:  1f3c 0001      	moveb #1,%sp@-                        
    58c4:  4227           	clrb %sp@-                            
    58c6:  4e4f           	trap #15                              
    58c8:  a240           	0121100                               ; sysTrapHwrSleep
    58ca:  584f           	addqw #4,%sp                          
    58cc:  6000 0488      	braw 0x5d56                           ; ↓ label_31

  label_2:
    58d0:  4a38 02dc      	tstb 0x2dc                            
    58d4:  662e           	bnes 0x5904                           ; ↓ label_5
    58d6:  7000           	moveq #0,%d0                          
    58d8:  1038 02fa      	moveb 0x2fa,%d0                       
    58dc:  b06a 0006      	cmpw %a2@(6),%d0                      
    58e0:  641a           	bccs 0x58fc                           ; ↓ label_4
    58e2:  0c38 0001 02d0 	cmpib #1,0x2d0                        
    58e8:  6604           	bnes 0x58ee                           ; ↓ label_3
    58ea:  4eba fe1e      	jsr %pc@(0x570a)                      ; PrvUpdateCritWarnTimer

  label_3:
    58ee:  1038 02fb      	moveb 0x2fb,%d0                       
    58f2:  0240 0005      	andiw #5,%d0                          
    58f6:  6612           	bnes 0x590a                           ; ↓ label_6
    58f8:  7601           	moveq #1,%d3                          
    58fa:  600e           	bras 0x590a                           ; ↓ label_6

  label_4:
    58fc:  4238 02d1      	clrb 0x2d1                            
    5900:  7600           	moveq #0,%d3                          
    5902:  6006           	bras 0x590a                           ; ↓ label_6

  label_5:
    5904:  4238 02d1      	clrb 0x2d1                            
    5908:  7600           	moveq #0,%d3                          

  label_6:
    590a:  4a78 013a      	tstw 0x13a                            
    590e:  6c00 0446      	bgew 0x5d56                           ; ↓ label_31
    5912:  4a03           	tstb %d3                              
    5914:  6700 0440      	beqw 0x5d56                           ; ↓ label_31
    5918:  31fc 1770 013a 	movew #6000,0x13a                     
    591e:  3f3c 0008      	movew #8,%sp@-                        
    5922:  2f3c 0101 0000 	movel #16842752,%sp@-                 
    5928:  4e4f           	trap #15                              
    592a:  a12d           	0120455                               ; sysTrapEvtEnqueueKey
    592c:  5c4f           	addqw #6,%sp                          
    592e:  6000 0426      	braw 0x5d56                           ; ↓ label_31
    5932:  200b           	movel %a3,%d0                         
    5934:  6700 0420      	beqw 0x5d56                           ; ↓ label_31
    5938:  31d3 0310      	movew %a3@,0x310                      
    593c:  4278 0136      	clrw 0x136                            
    5940:  4ab8 014e      	tstl 0x14e                            
    5944:  6700 0410      	beqw 0x5d56                           ; ↓ label_31
    5948:  4878 0001      	pea 0x1                               
    594c:  2f38 014e      	movel 0x14e,%sp@-                     
    5950:  4e4f           	trap #15                              
    5952:  a09a           	0120232                               ; sysTrapSysTimerWrite
    5954:  504f           	addqw #8,%sp                          
    5956:  6000 03fe      	braw 0x5d56                           ; ↓ label_31
    595a:  200b           	movel %a3,%d0                         
    595c:  6700 03f8      	beqw 0x5d56                           ; ↓ label_31
    5960:  36b8 0310      	movew 0x310,%a3@                      
    5964:  6000 03f0      	braw 0x5d56                           ; ↓ label_31
    5968:  4878 0094      	pea 0x94                              
    596c:  4e4f           	trap #15                              
    596e:  a013           	0120023                               ; sysTrapMemPtrNew
    5970:  2448           	moveal %a0,%a2                        
    5972:  4227           	clrb %sp@-                            
    5974:  4878 0094      	pea 0x94                              
    5978:  2f0a           	movel %a2,%sp@-                       
    597a:  4e4f           	trap #15                              
    597c:  a027           	0120047                               ; sysTrapMemSet
    597e:  21ca 02f6      	movel %a2,0x2f6                       
    5982:  3038 028e      	movew 0x28e,%d0                       
    5986:  0800 0006      	btst #6,%d0                           
    598a:  4fef 000e      	lea %sp@(14),%sp                      
    598e:  6710           	beqs 0x59a0                           ; ↓ label_7
    5990:  24bc 0000 0bb9 	movel #3001,%a2@                      
    5996:  257c 0000 1d59 	movel #7513,%a2@(118)                 
    599c:  0076                                                  
    599e:  600e           	bras 0x59ae                           ; ↓ label_8

  label_7:
    59a0:  24bc 0000 0ce5 	movel #3301,%a2@                      
    59a6:  257c 0000 19ca 	movel #6602,%a2@(118)                 
    59ac:  0076                                                  

  label_8:
    59ae:  486e fff6      	pea %fp@(-10)                         
    59b2:  486e fff8      	pea %fp@(-8)                          
    59b6:  2f3c 6361 6c6b 	movel #1667329131,%sp@-               
    59bc:  4267           	clrw %sp@-                            
    59be:  4e4f           	trap #15                              
    59c0:  a340           	0121500                               ; sysTrapHwrGetROMToken
    59c2:  4a40           	tstw %d0                              
    59c4:  4fef 000e      	lea %sp@(14),%sp                      
    59c8:  660e           	bnes 0x59d8                           ; ↓ label_9
    59ca:  0c6e 0004 fff6 	cmpiw #4,%fp@(-10)                    
    59d0:  6606           	bnes 0x59d8                           ; ↓ label_9
    59d2:  206e fff8      	moveal %fp@(-8),%a0                   
    59d6:  2490           	movel %a0@,%a2@                       

  label_9:
    59d8:  486e fff6      	pea %fp@(-10)                         
    59dc:  486e fff8      	pea %fp@(-8)                          
    59e0:  2f3c 636e 6364 	movel #1668178788,%sp@-               
    59e6:  4267           	clrw %sp@-                            
    59e8:  4e4f           	trap #15                              
    59ea:  a340           	0121500                               ; sysTrapHwrGetROMToken
    59ec:  4a40           	tstw %d0                              
    59ee:  4fef 000e      	lea %sp@(14),%sp                      
    59f2:  6610           	bnes 0x5a04                           ; ↓ label_10
    59f4:  0c6e 0004 fff6 	cmpiw #4,%fp@(-10)                    
    59fa:  6608           	bnes 0x5a04                           ; ↓ label_10
    59fc:  206e fff8      	moveal %fp@(-8),%a0                   
    5a00:  2550 0076      	movel %a0@,%a2@(118)                  

  label_10:
    5a04:  0c92 0000 03e8 	cmpil #1000,%a2@                      
    5a0a:  6406           	bccs 0x5a12                           ; ↓ label_11
    5a0c:  24bc 0000 0ce5 	movel #3301,%a2@                      

  label_11:
    5a12:  0caa 0000 03e8 	cmpil #1000,%a2@(118)                 
    5a18:  0076                                                  
    5a1a:  6408           	bccs 0x5a24                           ; ↓ label_12
    5a1c:  257c 0000 1b58 	movel #7000,%a2@(118)                 
    5a22:  0076                                                  

  label_12:
    5a24:  7600           	moveq #0,%d3                          
    5a26:  264a           	moveal %a2,%a3                        

  label_13:
    5a28:  426b 0008      	clrw %a3@(8)                          
    5a2c:  548b           	addql #2,%a3                          
    5a2e:  5243           	addqw #1,%d3                          
    5a30:  0c43 0010      	cmpiw #16,%d3                         
    5a34:  65f2           	bcss 0x5a28                           ; ↑ label_13
    5a36:  357c 01f4 0008 	movew #500,%a2@(8)                    
    5a3c:  357c 015e 000a 	movew #350,%a2@(10)                   
    5a42:  357c 0038 000c 	movew #56,%a2@(12)                    
    5a48:  357c 0058 000e 	movew #88,%a2@(14)                    
    5a4e:  357c 0116 0010 	movew #278,%a2@(16)                   
    5a54:  357c 0071 0012 	movew #113,%a2@(18)                   
    5a5a:  357c 00d8 0014 	movew #216,%a2@(20)                   
    5a60:  357c 002a 0016 	movew #42,%a2@(22)                    
    5a66:  426a 0028      	clrw %a2@(40)                         
    5a6a:  357c 01f4 0004 	movew #500,%a2@(4)                    
    5a70:  7010           	moveq #16,%d0                         
    5a72:  c0b8 02cc      	andl 0x2cc,%d0                        
    5a76:  670a           	beqs 0x5a82                           ; ↓ label_14
    5a78:  41fa fa46      	lea %pc@(0x54c0),%a0                  ; ??? 0x54c0
    5a7c:  21c8 02c6      	movel %a0,0x2c6                       
    5a80:  6008           	bras 0x5a8a                           ; ↓ label_15

  label_14:
    5a82:  41fa fa18      	lea %pc@(0x549c),%a0                  ; ??? 0x549c
    5a86:  21c8 02c6      	movel %a0,0x2c6                       

  label_15:
    5a8a:  1f3c 000f      	moveb #15,%sp@-                       
    5a8e:  4e4f           	trap #15                              
    5a90:  a2d1           	0121321                               ; sysTrapPrefGetPreference
    5a92:  1d40 fff4      	moveb %d0,%fp@(-12)                   
    5a96:  486e fff4      	pea %fp@(-12)                         
    5a9a:  3f3c 000f      	movew #15,%sp@-                       
    5a9e:  4e4f           	trap #15                              
    5aa0:  a368           	0121550                               ; sysTrapHwrBattery
    5aa2:  31fc 01f4 0310 	movew #500,0x310                      
    5aa8:  31fc 1770 013a 	movew #6000,0x13a                     
    5aae:  4e4f           	trap #15                              
    5ab0:  a248           	0121110                               ; sysTrapHwrBatteryLevel
    5ab2:  11c0 0139      	moveb %d0,0x139                       
    5ab6:  504f           	addqw #8,%sp                          
    5ab8:  6000 029c      	braw 0x5d56                           ; ↓ label_31
    5abc:  260b           	movel %a3,%d3                         
    5abe:  3f3c 00e4      	movew #228,%sp@-                      
    5ac2:  4eba f794      	jsr %pc@(0x5258)                      ; /
    5ac6:  3800           	movew %d0,%d4                         
    5ac8:  3004           	movew %d4,%d0                         
    5aca:  e848           	lsrw #4,%d0                           
    5acc:  2043           	moveal %d3,%a0                        
    5ace:  3140 0002      	movew %d0,%a0@(2)                     
    5ad2:  7000           	moveq #0,%d0                          
    5ad4:  3004           	movew %d4,%d0                         
    5ad6:  2212           	movel %a2@,%d1                        
    5ad8:  4eba a5a4      	jsr %pc@(0x7e)                        ; anonymous_function_0
    5adc:  0680 0000 0800 	addil #2048,%d0                       
    5ae2:  720c           	moveq #12,%d1                         
    5ae4:  e2a8           	lsrl %d1,%d0                          
    5ae6:  3080           	movew %d0,%a0@                        
    5ae8:  544f           	addqw #2,%sp                          
    5aea:  6000 026a      	braw 0x5d56                           ; ↓ label_31
    5aee:  2478 02c2      	moveal 0x2c2,%a2                      
    5af2:  3a2a 001e      	movew %a2@(30),%d5                    
    5af6:  4e4f           	trap #15                              
    5af8:  a248           	0121110                               ; sysTrapHwrBatteryLevel
    5afa:  d06a 0020      	addw %a2@(32),%d0                     
    5afe:  c0fc 0064      	muluw #100,%d0                        
    5b02:  3205           	movew %d5,%d1                         
    5b04:  e249           	lsrw #1,%d1                           
    5b06:  d240           	addw %d0,%d1                          
    5b08:  7000           	moveq #0,%d0                          
    5b0a:  3001           	movew %d1,%d0                         
    5b0c:  80c5           	divuw %d5,%d0                         
    5b0e:  3600           	movew %d0,%d3                         
    5b10:  3f03           	movew %d3,%sp@-                       
    5b12:  2f0b           	movel %a3,%sp@-                       
    5b14:  4eba 0258      	jsr %pc@(0x5d6e)                      ; PrvEvaluateNewPowerLoad
    5b18:  5c4f           	addqw #6,%sp                          
    5b1a:  6000 023a      	braw 0x5d56                           ; ↓ label_31
    5b1e:  260b           	movel %a3,%d3                         
    5b20:  1e13           	moveb %a3@,%d7                        
    5b22:  7000           	moveq #0,%d0                          
    5b24:  1007           	moveb %d7,%d0                         
    5b26:  7a01           	moveq #1,%d5                          
    5b28:  e16d           	lslw %d0,%d5                          
    5b2a:  7000           	moveq #0,%d0                          
    5b2c:  1007           	moveb %d7,%d0                         
    5b2e:  d080           	addl %d0,%d0                          
    5b30:  3832 0808      	movew %a2@(00000008,%d0:l),%d4        
    5b34:  0c07 000f      	cmpib #15,%d7                         
    5b38:  6624           	bnes 0x5b5e                           ; ↓ label_17
    5b3a:  2043           	moveal %d3,%a0                        
    5b3c:  3828 0002      	movew %a0@(2),%d4                     
    5b40:  536a 0028      	subqw #1,%a2@(40)                     
    5b44:  3e2a 0028      	movew %a2@(40),%d7                    
    5b48:  6f0c           	bles 0x5b56                           ; ↓ label_16
    5b4a:  996a 0004      	subw %d4,%a2@(4)                      
    5b4e:  4228 0004      	clrb %a0@(4)                          
    5b52:  6000 0202      	braw 0x5d56                           ; ↓ label_31

  label_16:
    5b56:  4a47           	tstw %d7                              
    5b58:  6e04           	bgts 0x5b5e                           ; ↓ label_17
    5b5a:  426a 0028      	clrw %a2@(40)                         

  label_17:
    5b5e:  3005           	movew %d5,%d0                         
    5b60:  c06a 0006      	andw %a2@(6),%d0                      
    5b64:  660c           	bnes 0x5b72                           ; ↓ label_18
    5b66:  2043           	moveal %d3,%a0                        
    5b68:  117c 0004 0004 	moveb #4,%a0@(4)                      
    5b6e:  6000 01e6      	braw 0x5d56                           ; ↓ label_31

  label_18:
    5b72:  3005           	movew %d5,%d0                         
    5b74:  4640           	notw %d0                              
    5b76:  c16a 0006      	andw %d0,%a2@(6)                      
    5b7a:  996a 0004      	subw %d4,%a2@(4)                      
    5b7e:  2043           	moveal %d3,%a0                        
    5b80:  4228 0004      	clrb %a0@(4)                          
    5b84:  0c6a 01f4 0004 	cmpiw #500,%a2@(4)                    
    5b8a:  6600 01ca      	bnew 0x5d56                           ; ↓ label_31
    5b8e:  082a 0003 002a 	btst #3,%a2@(42)                      
    5b94:  671e           	beqs 0x5bb4                           ; ↓ label_19
    5b96:  022a 00f7 002a 	andib #-9,%a2@(42)                    
    5b9c:  082a 0001 002a 	btst #1,%a2@(42)                      
    5ba2:  6700 01b2      	beqw 0x5d56                           ; ↓ label_31
    5ba6:  3f3c 00e4      	movew #228,%sp@-                      
    5baa:  4eba f6ac      	jsr %pc@(0x5258)                      ; /
    5bae:  544f           	addqw #2,%sp                          
    5bb0:  6000 01a4      	braw 0x5d56                           ; ↓ label_31

  label_19:
    5bb4:  022a 00fb 002a 	andib #-5,%a2@(42)                    
    5bba:  6000 019a      	braw 0x5d56                           ; ↓ label_31
    5bbe:  280b           	movel %a3,%d4                         
    5bc0:  2478 02c6      	moveal 0x2c6,%a2                      
    5bc4:  1a13           	moveb %a3@,%d5                        
    5bc6:  6002           	bras 0x5bca                           ; ↓ label_21

  label_20:
    5bc8:  528a           	addql #1,%a2                          

  label_21:
    5bca:  1612           	moveb %a2@,%d3                        
    5bcc:  ba03           	cmpb %d3,%d5                          
    5bce:  6706           	beqs 0x5bd6                           ; ↓ label_22
    5bd0:  0c03 00ff      	cmpib #-1,%d3                         
    5bd4:  66f2           	bnes 0x5bc8                           ; ↑ label_20

  label_22:
    5bd6:  0c03 00ff      	cmpib #-1,%d3                         
    5bda:  660e           	bnes 0x5bea                           ; ↓ label_23
    5bdc:  4ab8 02c2      	tstl 0x2c2                            
    5be0:  6608           	bnes 0x5bea                           ; ↓ label_23
    5be2:  2078 02c6      	moveal 0x2c6,%a0                      
    5be6:  2244           	moveal %d4,%a1                        
    5be8:  1290           	moveb %a0@,%a1@                       

  label_23:
    5bea:  45fa f71c      	lea %pc@(0x5308),%a2                  ; ??? 0x5308
    5bee:  2044           	moveal %d4,%a0                        
    5bf0:  7000           	moveq #0,%d0                          
    5bf2:  1010           	moveb %a0@,%d0                        
    5bf4:  ed88           	lsll #6,%d0                           
    5bf6:  d5c0           	addal %d0,%a2                         
    5bf8:  4ab8 02c2      	tstl 0x2c2                            
    5bfc:  660e           	bnes 0x5c0c                           ; ↓ label_24
    5bfe:  4878 0040      	pea 0x40                              
    5c02:  4e4f           	trap #15                              
    5c04:  a013           	0120023                               ; sysTrapMemPtrNew
    5c06:  21c8 02c2      	movel %a0,0x2c2                       
    5c0a:  584f           	addqw #4,%sp                          

  label_24:
    5c0c:  2078 02c2      	moveal 0x2c2,%a0                      
    5c10:  43d2           	lea %a2@,%a1                          
    5c12:  700f           	moveq #15,%d0                         
    5c14:  20d9           	movel %a1@+,%a0@+                     
    5c16:  51c8 fffc      	dbf %d0,0x5c14                        
    5c1a:  2478 02c2      	moveal 0x2c2,%a2                      
    5c1e:  2044           	moveal %d4,%a0                        
    5c20:  11d0 0265      	moveb %a0@,0x265                      
    5c24:  0c10 0002      	cmpib #2,%a0@                         
    5c28:  666a           	bnes 0x5c94                           ; ↓ label_25
    5c2a:  42a7           	clrl %sp@-                            
    5c2c:  486e fffc      	pea %fp@(-4)                          
    5c30:  2f3c 6164 6363 	movel #1633968995,%sp@-               
    5c36:  4267           	clrw %sp@-                            
    5c38:  4e4f           	trap #15                              
    5c3a:  a340           	0121500                               ; sysTrapHwrGetROMToken
    5c3c:  3600           	movew %d0,%d3                         
    5c3e:  4fef 000e      	lea %sp@(14),%sp                      
    5c42:  6650           	bnes 0x5c94                           ; ↓ label_25
    5c44:  266e fffc      	moveal %fp@(-4),%a3                   
    5c48:  0c53 ffff      	cmpiw #-1,%a3@                        
    5c4c:  6746           	beqs 0x5c94                           ; ↓ label_25
    5c4e:  3a13           	movew %a3@,%d5                        
    5c50:  204b           	moveal %a3,%a0                        
    5c52:  3628 0004      	movew %a0@(4),%d3                     
    5c56:  9645           	subw %d5,%d3                          
    5c58:  3803           	movew %d3,%d4                         
    5c5a:  e24c           	lsrw #1,%d4                           
    5c5c:  5488           	addql #2,%a0                          
    5c5e:  2648           	moveal %a0,%a3                        
    5c60:  226e fffc      	moveal %fp@(-4),%a1                   
    5c64:  3229 0006      	movew %a1@(6),%d1                     
    5c68:  9250           	subw %a0@,%d1                         
    5c6a:  c2fc 0064      	muluw #100,%d1                        
    5c6e:  d244           	addw %d4,%d1                          
    5c70:  7800           	moveq #0,%d4                          
    5c72:  3801           	movew %d1,%d4                         
    5c74:  88c3           	divuw %d3,%d4                         
    5c76:  3005           	movew %d5,%d0                         
    5c78:  c0c4           	muluw %d4,%d0                         
    5c7a:  0640 0032      	addiw #50,%d0                         
    5c7e:  7600           	moveq #0,%d3                          
    5c80:  3600           	movew %d0,%d3                         
    5c82:  86fc 0064      	divuw #100,%d3                        
    5c86:  3a13           	movew %a3@,%d5                        
    5c88:  9a43           	subw %d3,%d5                          
    5c8a:  4445           	negw %d5                              
    5c8c:  3545 0020      	movew %d5,%a2@(32)                    
    5c90:  3544 001e      	movew %d4,%a2@(30)                    

  label_25:
    5c94:  4238 02ca      	clrb 0x2ca                            
    5c98:  4238 02d1      	clrb 0x2d1                            
    5c9c:  31fc ffff 013a 	movew #-1,0x13a                       
    5ca2:  4a6a 0004      	tstw %a2@(4)                          
    5ca6:  6608           	bnes 0x5cb0                           ; ↓ label_26
    5ca8:  11fc 0001 02d0 	moveb #1,0x2d0                        
    5cae:  6004           	bras 0x5cb4                           ; ↓ label_27

  label_26:
    5cb0:  4238 02d0      	clrb 0x2d0                            

  label_27:
    5cb4:  3038 028e      	movew 0x28e,%d0                       
    5cb8:  0800 0006      	btst #6,%d0                           
    5cbc:  670c           	beqs 0x5cca                           ; ↓ label_28
    5cbe:  31fc 0055 0308 	movew #85,0x308                       
    5cc4:  357c 0055 001e 	movew #85,%a2@(30)                    

  label_28:
    5cca:  302a 0002      	movew %a2@(2),%d0                     
    5cce:  c0ea 001e      	muluw %a2@(30),%d0                    
    5cd2:  0640 0032      	addiw #50,%d0                         
    5cd6:  7200           	moveq #0,%d1                          
    5cd8:  3200           	movew %d0,%d1                         
    5cda:  82fc 0064      	divuw #100,%d1                        
    5cde:  926a 0020      	subw %a2@(32),%d1                     
    5ce2:  11c1 013c      	moveb %d1,0x13c                       
    5ce6:  302a 000a      	movew %a2@(10),%d0                    
    5cea:  c0ea 001e      	muluw %a2@(30),%d0                    
    5cee:  0640 0032      	addiw #50,%d0                         
    5cf2:  7200           	moveq #0,%d1                          
    5cf4:  3200           	movew %d0,%d1                         
    5cf6:  82fc 0064      	divuw #100,%d1                        
    5cfa:  926a 0020      	subw %a2@(32),%d1                     
    5cfe:  11c1 0138      	moveb %d1,0x138                       
    5d02:  302a 000c      	movew %a2@(12),%d0                    
    5d06:  c0ea 001e      	muluw %a2@(30),%d0                    
    5d0a:  0640 0032      	addiw #50,%d0                         
    5d0e:  7200           	moveq #0,%d1                          
    5d10:  3200           	movew %d0,%d1                         
    5d12:  82fc 0064      	divuw #100,%d1                        
    5d16:  926a 0020      	subw %a2@(32),%d1                     
    5d1a:  11c1 02cb      	moveb %d1,0x2cb                       
    5d1e:  6036           	bras 0x5d56                           ; ↓ label_31
    5d20:  2478 02c2      	moveal 0x2c2,%a2                      
    5d24:  4213           	clrb %a3@                             
    5d26:  7000           	moveq #0,%d0                          
    5d28:  1038 02fa      	moveb 0x2fa,%d0                       
    5d2c:  3600           	movew %d0,%d3                         
    5d2e:  b06a 0008      	cmpw %a2@(8),%d0                      
    5d32:  640c           	bccs 0x5d40                           ; ↓ label_29
    5d34:  16bc 0002      	moveb #2,%a3@                         
    5d38:  31ea 0016 013a 	movew %a2@(22),0x13a                  
    5d3e:  6016           	bras 0x5d56                           ; ↓ label_31

  label_29:
    5d40:  b66a 0006      	cmpw %a2@(6),%d3                      
    5d44:  6410           	bccs 0x5d56                           ; ↓ label_31
    5d46:  16bc 0001      	moveb #1,%a3@                         
    5d4a:  31ea 0012 013a 	movew %a2@(18),0x13a                  
    5d50:  6004           	bras 0x5d56                           ; ↓ label_31

  label_30:
    5d52:  3c3c 0502      	movew #1282,%d6                       

  label_31:
    5d56:  3006           	movew %d6,%d0                         
    5d58:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    5d5c:  4e5e           	unlk %fp                              
    5d5e:  4e75           	rts                                   
```

# PrvEvaluateNewPowerLoad: 0x5d6e - 0x5e0a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    5d6e:  4e56 0000      	linkw %fp,#0                          
    5d72:  2f0b           	movel %a3,%sp@-                       
    5d74:  2f05           	movel %d5,%sp@-                       
    5d76:  266e 0008      	moveal %fp@(8),%a3                    
    5d7a:  2278 02f6      	moveal 0x2f6,%a1                      
    5d7e:  1a13           	moveb %a3@,%d5                        
    5d80:  7000           	moveq #0,%d0                          
    5d82:  1005           	moveb %d5,%d0                         
    5d84:  7201           	moveq #1,%d1                          
    5d86:  e169           	lslw %d0,%d1                          
    5d88:  7000           	moveq #0,%d0                          
    5d8a:  1005           	moveb %d5,%d0                         
    5d8c:  d080           	addl %d0,%d0                          
    5d8e:  2078 02f6      	moveal 0x2f6,%a0                      
    5d92:  3430 0808      	movew %a0@(00000008,%d0:l),%d2        
    5d96:  177c 0001 0004 	moveb #1,%a3@(4)                      
    5d9c:  0c13 000f      	cmpib #15,%a3@                        
    5da0:  6608           	bnes 0x5daa                           ; ↓ label_0
    5da2:  342b 0002      	movew %a3@(2),%d2                     
    5da6:  5269 0028      	addqw #1,%a1@(40)                     

  label_0:
    5daa:  3001           	movew %d1,%d0                         
    5dac:  c069 0006      	andw %a1@(6),%d0                      
    5db0:  670e           	beqs 0x5dc0                           ; ↓ label_1
    5db2:  0c41 8000      	cmpiw #-32768,%d1                     
    5db6:  6708           	beqs 0x5dc0                           ; ↓ label_1
    5db8:  177c 0003 0004 	moveb #3,%a3@(4)                      
    5dbe:  6044           	bras 0x5e04                           ; ↓ label_3

  label_1:
    5dc0:  1a13           	moveb %a3@,%d5                        
    5dc2:  0c05 0002      	cmpib #2,%d5                          
    5dc6:  661c           	bnes 0x5de4                           ; ↓ label_2
    5dc8:  0c6e 0168 000c 	cmpiw #360,%fp@(12)                   
    5dce:  6334           	blss 0x5e04                           ; ↓ label_3
    5dd0:  8369 0006      	orw %d1,%a1@(6)                       
    5dd4:  d569 0004      	addw %d2,%a1@(4)                      
    5dd8:  422b 0004      	clrb %a3@(4)                          
    5ddc:  0029 0004 002a 	orib #4,%a1@(42)                      
    5de2:  6020           	bras 0x5e04                           ; ↓ label_3

  label_2:
    5de4:  0c05 0001      	cmpib #1,%d5                          
    5de8:  661a           	bnes 0x5e04                           ; ↓ label_3
    5dea:  0c6e 016e 000c 	cmpiw #366,%fp@(12)                   
    5df0:  6312           	blss 0x5e04                           ; ↓ label_3
    5df2:  8369 0006      	orw %d1,%a1@(6)                       
    5df6:  d569 0004      	addw %d2,%a1@(4)                      
    5dfa:  422b 0004      	clrb %a3@(4)                          
    5dfe:  0029 0004 002a 	orib #4,%a1@(42)                      

  label_3:
    5e04:  2a1f           	movel %sp@+,%d5                       
    5e06:  265f           	moveal %sp@+,%a3                      
    5e08:  4e5e           	unlk %fp                              
    5e0a:  4e75           	rts                                   
```

# HwrDockStatus: 0x5e26 - 0x5e62

```
    5e26:  4e56 0000      	linkw %fp,#0                          
    5e2a:  2f04           	movel %d4,%sp@-                       
    5e2c:  2f03           	movel %d3,%sp@-                       
    5e2e:  7600           	moveq #0,%d3                          
    5e30:  3f3c 00ac      	movew #172,%sp@-                      
    5e34:  4eba f422      	jsr %pc@(0x5258)                      ; /
    5e38:  e848           	lsrw #4,%d0                           
    5e3a:  3800           	movew %d0,%d4                         
    5e3c:  0c44 0026      	cmpiw #38,%d4                         
    5e40:  544f           	addqw #2,%sp                          
    5e42:  6204           	bhis 0x5e48                           ; ↓ label_0
    5e44:  0043 0001      	oriw #1,%d3                           

  label_0:
    5e48:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    5e4c:  0240 0020      	andiw #32,%d0                         
    5e50:  6608           	bnes 0x5e5a                           ; ↓ label_1
    5e52:  0043 0004      	oriw #4,%d3                           
    5e56:  0043 0008      	oriw #8,%d3                           

  label_1:
    5e5a:  3003           	movew %d3,%d0                         
    5e5c:  261f           	movel %sp@+,%d3                       
    5e5e:  281f           	movel %sp@+,%d4                       
    5e60:  4e5e           	unlk %fp                              
    5e62:  4e75           	rts                                   
```

# HwrBatteryLevel: 0x5e74 - 0x5e9e

```
    5e74:  4e56 0000      	linkw %fp,#0                          
    5e78:  2f03           	movel %d3,%sp@-                       
    5e7a:  3f3c 00ec      	movew #236,%sp@-                      
    5e7e:  4eba f3d8      	jsr %pc@(0x5258)                      ; /
    5e82:  e848           	lsrw #4,%d0                           
    5e84:  3600           	movew %d0,%d3                         
    5e86:  2078 02f6      	moveal 0x2f6,%a0                      
    5e8a:  3028 0006      	movew %a0@(6),%d0                     
    5e8e:  0040 0002      	oriw #2,%d0                           
    5e92:  544f           	addqw #2,%sp                          
    5e94:  6602           	bnes 0x5e98                           ; ↓ label_0
    5e96:  5343           	subqw #1,%d3                          

  label_0:
    5e98:  3003           	movew %d3,%d0                         
    5e9a:  261f           	movel %sp@+,%d3                       
    5e9c:  4e5e           	unlk %fp                              
    5e9e:  4e75           	rts                                   
```

# HwrPluggedIn: 0x5ee8 - 0x5efe

```
    5ee8:  4e56 0000      	linkw %fp,#0                          
    5eec:  4e4f           	trap #15                              
    5eee:  a34d           	0121515                               ; sysTrapHwrDockStatus
    5ef0:  0240 0004      	andiw #4,%d0                          
    5ef4:  6704           	beqs 0x5efa                           ; ↓ label_0
    5ef6:  7001           	moveq #1,%d0                          
    5ef8:  6002           	bras 0x5efc                           ; ↓ label_1

  label_0:
    5efa:  7000           	moveq #0,%d0                          

  label_1:
    5efc:  4e5e           	unlk %fp                              
    5efe:  4e75           	rts                                   
```

# HwrDockSignals: 0x5f10 - 0x5f30

Parameters:

   * `%fp@(8)`      : ???

```
    5f10:  4e56 0000      	linkw %fp,#0                          
    5f14:  2f0a           	movel %a2,%sp@-                       
    5f16:  246e 0008      	moveal %fp@(8),%a2                    
    5f1a:  4252           	clrw %a2@                             
    5f1c:  4e4f           	trap #15                              
    5f1e:  a34d           	0121515                               ; sysTrapHwrDockStatus
    5f20:  0240 0001      	andiw #1,%d0                          
    5f24:  6704           	beqs 0x5f2a                           ; ↓ label_0
    5f26:  0052 0002      	oriw #2,%a2@                          

  label_0:
    5f2a:  7000           	moveq #0,%d0                          
    5f2c:  245f           	moveal %sp@+,%a2                      
    5f2e:  4e5e           	unlk %fp                              
    5f30:  4e75           	rts                                   
```

# HwrInterruptsInit: 0x5f44 - 0x5fa0

Locals:

   * `%fp@(-4)`     : ???

```
    5f44:  4e56 fffc      	linkw %fp,#-4                         
    5f48:  2f03           	movel %d3,%sp@-                       
    5f4a:  2d7c 7073 7973 	movel #1886615923,%fp@(-4)            
    5f50:  fffc                                                  
    5f52:  4eba a4f2      	jsr %pc@(0x446)                       ; HwrInterruptsInstall
    5f56:  3f3c 0001      	movew #1,%sp@-                        
    5f5a:  4e4f           	trap #15                              
    5f5c:  a246           	0121106                               ; sysTrapHwrTimerInit
    5f5e:  0278 efff f302 	andiw #-4097,0xfffffffffffff302       
    5f64:  0278 fff7 f304 	andiw #-9,0xfffffffffffff304          
    5f6a:  42a7           	clrl %sp@-                            
    5f6c:  42a7           	clrl %sp@-                            
    5f6e:  487a 0338      	pea %pc@(0x62a8)                      ; PrvSystemTimerProc
    5f72:  486e fffc      	pea %fp@(-4)                          
    5f76:  4878 014e      	pea 0x14e                             
    5f7a:  4e4f           	trap #15                              
    5f7c:  a099           	0120231                               ; sysTrapSysTimerCreate
    5f7e:  3600           	movew %d0,%d3                         
    5f80:  4fef 0016      	lea %sp@(22),%sp                      
    5f84:  6702           	beqs 0x5f88                           ; ↓ label_0
    5f86:  4e48           	trap #8                               

  label_0:
    5f88:  4878 01f4      	pea 0x1f4                             
    5f8c:  2f38 014e      	movel 0x14e,%sp@-                     
    5f90:  4e4f           	trap #15                              
    5f92:  a09a           	0120232                               ; sysTrapSysTimerWrite
    5f94:  3600           	movew %d0,%d3                         
    5f96:  504f           	addqw #8,%sp                          
    5f98:  6702           	beqs 0x5f9c                           ; ↓ label_1
    5f9a:  4e48           	trap #8                               

  label_1:
    5f9c:  261f           	movel %sp@+,%d3                       
    5f9e:  4e5e           	unlk %fp                              
    5fa0:  4e75           	rts                                   
```

# HwrIRQ1Handler: 0x5fb6 - 0x5fe6

```
    5fb6:  4e56 0000      	linkw %fp,#0                          
    5fba:  2f03           	movel %d3,%sp@-                       
    5fbc:  2638 f30c      	movel 0xfffffffffffff30c,%d3          
    5fc0:  1038 0101      	moveb 0x101,%d0                       
    5fc4:  0240 0080      	andiw #128,%d0                        
    5fc8:  670e           	beqs 0x5fd8                           ; ↓ label_0
    5fca:  41fa 002e      	lea %pc@(0x5ffa),%a0                  ; ??? 0x5ffa
    5fce:  4850           	pea %a0@                              
    5fd0:  4e4f           	trap #15                              
    5fd2:  a081           	0120201                               ; sysTrapDbgMessage
    5fd4:  4e48           	trap #8                               
    5fd6:  584f           	addqw #4,%sp                          

  label_0:
    5fd8:  2f03           	movel %d3,%sp@-                       
    5fda:  4227           	clrb %sp@-                            
    5fdc:  4e4f           	trap #15                              
    5fde:  a2a1           	0121241                               ; sysTrapKeyHandleInterrupt
    5fe0:  5c4f           	addqw #6,%sp                          
    5fe2:  261f           	movel %sp@+,%d3                       
    5fe4:  4e5e           	unlk %fp                              
    5fe6:  4e75           	rts                                   
```

# HwrIRQ2Handler: 0x6006 - 0x6012

```
    6006:  4e56 0000      	linkw %fp,#0                          
    600a:  0078 0002 f30c 	oriw #2,0xfffffffffffff30c            
    6010:  4e5e           	unlk %fp                              
    6012:  4e75           	rts                                   
```

# HwrIRQ3Handler: 0x6026 - 0x6032

```
    6026:  4e56 0000      	linkw %fp,#0                          
    602a:  0078 0004 f30c 	oriw #4,0xfffffffffffff30c            
    6030:  4e5e           	unlk %fp                              
    6032:  4e75           	rts                                   
```

# HwrIRQ4Handler: 0x6026 - 0x6032

```
   8468a:       207c ffff f000  moveal #-4096,%a0
   84690:       2428 030c       movel %a0@(780),%d2
   84694:       0802 0002       btst #2,%d2
   84698:       670a            beqs 0x846a4
   8469a:       2278 0122       moveal 0x122,%a1
   8469e:       2269 0340       moveal %a1@(832),%a1
   846a2:       4e91            jsr %a1@

   846a4:       0802 0004       btst #4,%d2
   846a8:       6716            beqs 0x846c0
   846aa:       2278 0122       moveal 0x122,%a1
   846ae:       2269 03ec       moveal %a1@(1004),%a1
   846b2:       48e7 2080       moveml %d2/%a0,%sp@-
   846b6:       4267            clrw %sp@-
   846b8:       4e91            jsr %a1@
   846ba:       544f            addqw #2,%sp
   846bc:       4cdf 0104       moveml %sp@+,%d2/%a0

   846c0:       3002            movew %d2,%d0
   846c2:       0240 0740       andiw #1856,%d0
   846c6:       6718            beqs 0x846e0
   846c8:       2278 0122       moveal 0x122,%a1
   846cc:       2269 0a84       moveal %a1@(2692),%a1
   846d0:       48e7 2080       moveml %d2/%a0,%sp@-
   846d4:       2f02            movel %d2,%sp@-
   846d6:       4267            clrw %sp@-
   846d8:       4e91            jsr %a1@
   846da:       5c4f            addqw #6,%sp
   846dc:       4cdf 0104       moveml %sp@+,%d2/%a0

   846e0:       3002            movew %d2,%d0
   846e2:       0240 0800       andiw #2048,%d0
   846e6:       671e            beqs 0x84706
   846e8:       48e7 2080       moveml %d2/%a0,%sp@-
   846ec:       2438 0304       movel 0x304,%d2
   846f0:       4a82            tstl %d2
   846f2:       670e            beqs 0x84702
   846f4:       2042            moveal %d2,%a0
   846f6:       2428 004c       movel %a0@(76),%d2
   846fa:       2242            moveal %d2,%a1
   846fc:       4a82            tstl %d2
   846fe:       6702            beqs 0x84702
   84700:       4e91            jsr %a1@              <-

   84702:       4cdf 0104       moveml %sp@+,%d2/%a0

   84706:       207c ffff f000  moveal #-4096,%a0
   8470c:       1038 0252       moveb 0x252,%d0
   84710:       0000 0080       orib #-128,%d0
   84714:       1140 0207       moveb %d0,%a0@(519)
   84718:       4e75            rts
```


# HwrIRQ5Handler: 0x60e8 - 0x6124

Locals:

   * `%fp@(-4)`     : ???

```
    60e8:  4e56 fffc      	linkw %fp,#-4                         
    60ec:  2f03           	movel %d3,%sp@-                       
    60ee:  4a38 0253      	tstb 0x253                            
    60f2:  662c           	bnes 0x6120                           ; ↓ label_1
    60f4:  486e fffc      	pea %fp@(-4)                          
    60f8:  4e4f           	trap #15                              
    60fa:  a270           	0121160                               ; sysTrapPenGetRawPen
    60fc:  3600           	movew %d0,%d3                         
    60fe:  584f           	addqw #4,%sp                          
    6100:  660a           	bnes 0x610c                           ; ↓ label_0
    6102:  486e fffc      	pea %fp@(-4)                          
    6106:  4e4f           	trap #15                              
    6108:  a126           	0120446                               ; sysTrapEvtEnqueuePenPoint
    610a:  584f           	addqw #4,%sp                          

  label_0:
    610c:  4a38 0253      	tstb 0x253                            
    6110:  670e           	beqs 0x6120                           ; ↓ label_1
    6112:  4878 0001      	pea 0x1                               
    6116:  2f38 014e      	movel 0x14e,%sp@-                     
    611a:  4e4f           	trap #15                              
    611c:  a09a           	0120232                               ; sysTrapSysTimerWrite
    611e:  504f           	addqw #8,%sp                          

  label_1:
    6120:  261f           	movel %sp@+,%d3                       
    6122:  4e5e           	unlk %fp                              
    6124:  4e75           	rts                                   
```

# PrvSystemTimerProc: 0x62a8 - 0x63b8

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???

```
    62a8:  4e56 fffa      	linkw %fp,#-6                         
    62ac:  2f04           	movel %d4,%sp@-                       
    62ae:  2f03           	movel %d3,%sp@-                       
    62b0:  263c 0000 01f4 	movel #500,%d3                        
    62b6:  426e fffe      	clrw %fp@(-2)                         
    62ba:  2f03           	movel %d3,%sp@-                       
    62bc:  2f38 014e      	movel 0x14e,%sp@-                     
    62c0:  4e4f           	trap #15                              
    62c2:  a09a           	0120232                               ; sysTrapSysTimerWrite
    62c4:  3038 0250      	movew 0x250,%d0                       
    62c8:  0240 0008      	andiw #8,%d0                          
    62cc:  504f           	addqw #8,%sp                          
    62ce:  670c           	beqs 0x62dc                           ; ↓ label_0
    62d0:  1f3c 0001      	moveb #1,%sp@-                        
    62d4:  4227           	clrb %sp@-                            
    62d6:  4e4f           	trap #15                              
    62d8:  a240           	0121100                               ; sysTrapHwrSleep
    62da:  584f           	addqw #4,%sp                          

  label_0:
    62dc:  4a38 0253      	tstb 0x253                            
    62e0:  6724           	beqs 0x6306                           ; ↓ label_2
    62e2:  486e fffa      	pea %fp@(-6)                          
    62e6:  4e4f           	trap #15                              
    62e8:  a270           	0121160                               ; sysTrapPenGetRawPen
    62ea:  3800           	movew %d0,%d4                         
    62ec:  584f           	addqw #4,%sp                          
    62ee:  660a           	bnes 0x62fa                           ; ↓ label_1
    62f0:  486e fffa      	pea %fp@(-6)                          
    62f4:  4e4f           	trap #15                              
    62f6:  a126           	0120446                               ; sysTrapEvtEnqueuePenPoint
    62f8:  584f           	addqw #4,%sp                          

  label_1:
    62fa:  4a38 0253      	tstb 0x253                            
    62fe:  6706           	beqs 0x6306                           ; ↓ label_2
    6300:  7600           	moveq #0,%d3                          
    6302:  1638 0253      	moveb 0x253,%d3                       

  label_2:
    6306:  42a7           	clrl %sp@-                            
    6308:  1f3c 0001      	moveb #1,%sp@-                        
    630c:  4e4f           	trap #15                              
    630e:  a2a1           	0121241                               ; sysTrapKeyHandleInterrupt
    6310:  2800           	movel %d0,%d4                         
    6312:  5c4f           	addqw #6,%sp                          
    6314:  6706           	beqs 0x631c                           ; ↓ label_3
    6316:  b883           	cmpl %d3,%d4                          
    6318:  6402           	bccs 0x631c                           ; ↓ label_3
    631a:  2604           	movel %d4,%d3                         

  label_3:
    631c:  4a78 0136      	tstw 0x136                            
    6320:  6e4c           	bgts 0x636e                           ; ↓ label_5
    6322:  4a38 0253      	tstb 0x253                            
    6326:  6646           	bnes 0x636e                           ; ↓ label_5
    6328:  1f3c 0001      	moveb #1,%sp@-                        
    632c:  4e4f           	trap #15                              
    632e:  a0fb           	0120373                               ; sysTrapTimHandleInterrupt
    6330:  486e fffe      	pea %fp@(-2)                          
    6334:  3f3c 0011      	movew #17,%sp@-                       
    6338:  4e4f           	trap #15                              
    633a:  a368           	0121550                               ; sysTrapHwrBattery
    633c:  4a40           	tstw %d0                              
    633e:  504f           	addqw #8,%sp                          
    6340:  6610           	bnes 0x6352                           ; ↓ label_4
    6342:  4a6e fffe      	tstw %fp@(-2)                         
    6346:  6f0a           	bles 0x6352                           ; ↓ label_4
    6348:  306e fffe      	moveaw %fp@(-2),%a0                   
    634c:  b1c3           	cmpal %d3,%a0                         
    634e:  6402           	bccs 0x6352                           ; ↓ label_4
    6350:  2608           	movel %a0,%d3                         

  label_4:
    6352:  1038 0139      	moveb 0x139,%d0                       
    6356:  b038 0138      	cmpb 0x138,%d0                        
    635a:  6412           	bccs 0x636e                           ; ↓ label_5
    635c:  11fc 0001 02ca 	moveb #1,0x2ca                        
    6362:  1f3c 0001      	moveb #1,%sp@-                        
    6366:  4227           	clrb %sp@-                            
    6368:  4e4f           	trap #15                              
    636a:  a240           	0121100                               ; sysTrapHwrSleep
    636c:  584f           	addqw #4,%sp                          

  label_5:
    636e:  4a78 0156      	tstw 0x156                            
    6372:  672a           	beqs 0x639e                           ; ↓ label_6
    6374:  7000           	moveq #0,%d0                          
    6376:  3038 0156      	movew 0x156,%d0                       
    637a:  7264           	moveq #100,%d1                        
    637c:  4eba 9d00      	jsr %pc@(0x7e)                        ; anonymous_function_0
    6380:  2238 0254      	movel 0x254,%d1                       
    6384:  92b8 0152      	subl 0x152,%d1                        
    6388:  b280           	cmpl %d0,%d1                          
    638a:  6312           	blss 0x639e                           ; ↓ label_6
    638c:  3f3c 0008      	movew #8,%sp@-                        
    6390:  2f3c 0114 0000 	movel #18087936,%sp@-                 
    6396:  4e4f           	trap #15                              
    6398:  a12d           	0120455                               ; sysTrapEvtEnqueueKey
    639a:  7601           	moveq #1,%d3                          
    639c:  5c4f           	addqw #6,%sp                          

  label_6:
    639e:  0c83 0000 01f4 	cmpil #500,%d3                        
    63a4:  640c           	bccs 0x63b2                           ; ↓ label_7
    63a6:  2f03           	movel %d3,%sp@-                       
    63a8:  2f38 014e      	movel 0x14e,%sp@-                     
    63ac:  4e4f           	trap #15                              
    63ae:  a09a           	0120232                               ; sysTrapSysTimerWrite
    63b0:  504f           	addqw #8,%sp                          

  label_7:
    63b2:  261f           	movel %sp@+,%d3                       
    63b4:  281f           	movel %sp@+,%d4                       
    63b6:  4e5e           	unlk %fp                              
    63b8:  4e75           	rts                                   
```

# DrvOpen: 0x63d0 - 0x6410

Parameters:

   * `%fp@(12)`     : ???

```
    63d0:  4e56 0000      	linkw %fp,#0                          
    63d4:  0278 ffdf f908 	andiw #-33,0xfffffffffffff908         
    63da:  0038 0040 f439 	orib #64,0xfffffffffffff439           
    63e0:  2f2e 000c      	movel %fp@(12),%sp@-                  
    63e4:  4eba 037e      	jsr %pc@(0x6764)                      ; PrvSetBaud
    63e8:  4878 00d0      	pea 0xd0                              
    63ec:  4eba 03e8      	jsr %pc@(0x67d6)                      ; PrvSetFlags
    63f0:  0078 e000 f900 	oriw #-8192,0xfffffffffffff900        
    63f6:  3038 f904      	movew 0xfffffffffffff904,%d0          
    63fa:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    63fe:  0000 0010      	orib #16,%d0                          
    6402:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    6406:  0278 fffb f306 	andiw #-5,0xfffffffffffff306          
    640c:  7000           	moveq #0,%d0                          
    640e:  4e5e           	unlk %fp                              
    6410:  4e75           	rts                                   
```

# DrvClose: 0x641c - 0x6434

Parameters:

   * `%fp@(8)`      : ???

```
    641c:  4e56 0000      	linkw %fp,#0                          
    6420:  42a7           	clrl %sp@-                            
    6422:  42a7           	clrl %sp@-                            
    6424:  3f3c 1007      	movew #4103,%sp@-                     
    6428:  2f2e 0008      	movel %fp@(8),%sp@-                   
    642c:  4eba 0014      	jsr %pc@(0x6442)                      ; DrvControl
    6430:  7000           	moveq #0,%d0                          
    6432:  4e5e           	unlk %fp                              
    6434:  4e75           	rts                                   
```

# DrvControl: 0x6442 - 0x664c

Parameters:

   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(18)`     : ???

```
    6442:  4e56 0000      	linkw %fp,#0                          
    6446:  48e7 1030      	moveml %d3/%a2-%a3,%sp@-              
    644a:  266e 000e      	moveal %fp@(14),%a3                   
    644e:  247c ffff f000 	moveal #-4096,%a2                     
    6454:  7600           	moveq #0,%d3                          
    6456:  302e 000c      	movew %fp@(12),%d0                    
    645a:  0440 1000      	subiw #4096,%d0                       
    645e:  0c40 0015      	cmpiw #21,%d0                         
    6462:  6200 01dc      	bhiw 0x6640                           ; ↓ label_4
    6466:  d040           	addw %d0,%d0                          
    6468:  303b 0006      	movew %pc@(0x6470,%d0:w),%d0          
    646c:  4efb 0002      	jmp %pc@(0x6470,%d0:w)                
    6470:  002c 0052 01d4 	orib #82,%a4@(468)                    
    6476:  00bc           	0274                                  
    6478:  00c6           	0306                                  
    647a:  00f4           	0364                                  
    647c:  00fe           	0376                                  
    647e:  0108 012c      	movepw %a0@(300),%d0                  
    6482:  00d0           	0320                                  
    6484:  00da           	0332                                  
    6486:  00e4           	0344                                  
    6488:  0148 0078      	movepl %a0@(120),%d0                  
    648c:  009e 017c 0184 	oril #24904068,%fp@+                  
    6492:  018c 01a0      	movepw %d0,%a4@(416)                  
    6496:  01d4           	bset %d0,%a4@                         
    6498:  01b8 01c4      	bclr %d0,0x1c4                        
    649c:  0078 0004 f306 	oriw #4,0xfffffffffffff306            
    64a2:  0278 1fff f900 	andiw #8191,0xfffffffffffff900        
    64a8:  2f13           	movel %a3@,%sp@-                      
    64aa:  4eba 02b8      	jsr %pc@(0x6764)                      ; PrvSetBaud
    64ae:  3600           	movew %d0,%d3                         
    64b0:  0078 e000 f900 	oriw #-8192,0xfffffffffffff900        
    64b6:  0278 fffb f306 	andiw #-5,0xfffffffffffff306          
    64bc:  584f           	addqw #4,%sp                          
    64be:  6000 0184      	braw 0x6644                           ; ↓ label_5
    64c2:  0078 0004 f306 	oriw #4,0xfffffffffffff306            
    64c8:  0278 1fff f900 	andiw #8191,0xfffffffffffff900        
    64ce:  2f13           	movel %a3@,%sp@-                      
    64d0:  4eba 0304      	jsr %pc@(0x67d6)                      ; PrvSetFlags
    64d4:  3600           	movew %d0,%d3                         
    64d6:  0078 e000 f900 	oriw #-8192,0xfffffffffffff900        
    64dc:  0278 fffb f306 	andiw #-5,0xfffffffffffff306          
    64e2:  584f           	addqw #4,%sp                          
    64e4:  6000 015e      	braw 0x6644                           ; ↓ label_5
    64e8:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    64ec:  0200 ffef      	andib #-17,%d0                        
    64f0:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    64f4:  0200 ffbf      	andib #-65,%d0                        
    64f8:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    64fc:  0078 0020 f908 	oriw #32,0xfffffffffffff908           
    6502:  4eba 035a      	jsr %pc@(0x685e)                      ; PrvCalculateNIPRPulseWidthValu
    6506:  31c0 f90a      	movew %d0,0xfffffffffffff90a          
    650a:  6000 0138      	braw 0x6644                           ; ↓ label_5
    650e:  0278 ffdf f908 	andiw #-33,0xfffffffffffff908         
    6514:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    6518:  0000 0040      	orib #64,%d0                          
    651c:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    6520:  0000 0010      	orib #16,%d0                          
    6524:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    6528:  6000 011a      	braw 0x6644                           ; ↓ label_5
    652c:  0078 e000 f900 	oriw #-8192,0xfffffffffffff900        
    6532:  6000 0110      	braw 0x6644                           ; ↓ label_5
    6536:  0278 7fff f900 	andiw #32767,0xfffffffffffff900       
    653c:  6000 0106      	braw 0x6644                           ; ↓ label_5
    6540:  0078 4000 f900 	oriw #16384,0xfffffffffffff900        
    6546:  6000 00fc      	braw 0x6644                           ; ↓ label_5
    654a:  0278 bfff f900 	andiw #-16385,0xfffffffffffff900      
    6550:  6000 00f2      	braw 0x6644                           ; ↓ label_5
    6554:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    6558:  0000 0010      	orib #16,%d0                          
    655c:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    6560:  6000 00e2      	braw 0x6644                           ; ↓ label_5
    6564:  0278 fffb f306 	andiw #-5,0xfffffffffffff306          
    656a:  6000 00d8      	braw 0x6644                           ; ↓ label_5
    656e:  0078 0004 f306 	oriw #4,0xfffffffffffff306            
    6574:  6000 00ce      	braw 0x6644                           ; ↓ label_5
    6578:  0078 0004 f306 	oriw #4,0xfffffffffffff306            
    657e:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    6582:  0200 ffef      	andib #-17,%d0                        
    6586:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    658a:  0000 0040      	orib #64,%d0                          
    658e:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    6592:  0278 7fff f900 	andiw #32767,0xfffffffffffff900       
    6598:  6000 00aa      	braw 0x6644                           ; ↓ label_5
    659c:  0078 e000 f900 	oriw #-8192,0xfffffffffffff900        
    65a2:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    65a6:  0000 0010      	orib #16,%d0                          
    65aa:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    65ae:  0278 fffb f306 	andiw #-5,0xfffffffffffff306          
    65b4:  6000 008e      	braw 0x6644                           ; ↓ label_5
    65b8:  3038 f906      	movew 0xfffffffffffff906,%d0          
    65bc:  0240 2000      	andiw #8192,%d0                       
    65c0:  6606           	bnes 0x65c8                           ; ↓ label_0
    65c2:  7008           	moveq #8,%d0                          
    65c4:  2680           	movel %d0,%a3@                        
    65c6:  607c           	bras 0x6644                           ; ↓ label_5

  label_0:
    65c8:  3038 f906      	movew 0xfffffffffffff906,%d0          
    65cc:  0240 8000      	andiw #-32768,%d0                     
    65d0:  6704           	beqs 0x65d6                           ; ↓ label_1
    65d2:  4293           	clrl %a3@                             
    65d4:  606e           	bras 0x6644                           ; ↓ label_5

  label_1:
    65d6:  3038 f906      	movew 0xfffffffffffff906,%d0          
    65da:  0240 4000      	andiw #16384,%d0                      
    65de:  6706           	beqs 0x65e6                           ; ↓ label_2
    65e0:  7001           	moveq #1,%d0                          
    65e2:  2680           	movel %d0,%a3@                        
    65e4:  605e           	bras 0x6644                           ; ↓ label_5

  label_2:
    65e6:  7004           	moveq #4,%d0                          
    65e8:  2680           	movel %d0,%a3@                        
    65ea:  6058           	bras 0x6644                           ; ↓ label_5
    65ec:  0078 1000 f906 	oriw #4096,0xfffffffffffff906         
    65f2:  6050           	bras 0x6644                           ; ↓ label_5
    65f4:  0278 efff f906 	andiw #-4097,0xfffffffffffff906       
    65fa:  6048           	bras 0x6644                           ; ↓ label_5
    65fc:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    6600:  0200 ffef      	andib #-17,%d0                        
    6604:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    6608:  0078 1000 f908 	oriw #4096,0xfffffffffffff908         
    660e:  6034           	bras 0x6644                           ; ↓ label_5
    6610:  0278 efff f908 	andiw #-4097,0xfffffffffffff908       
    6616:  1038 f439      	moveb 0xfffffffffffff439,%d0          
    661a:  0000 0010      	orib #16,%d0                          
    661e:  11c0 f439      	moveb %d0,0xfffffffffffff439          
    6622:  6020           	bras 0x6644                           ; ↓ label_5

  label_3:
    6624:  302a 0904      	movew %a2@(2308),%d0                  
    6628:  302a 0904      	movew %a2@(2308),%d0                  
    662c:  0240 2000      	andiw #8192,%d0                       
    6630:  66f2           	bnes 0x6624                           ; ↑ label_3
    6632:  6010           	bras 0x6644                           ; ↓ label_5
    6634:  4293           	clrl %a3@                             
    6636:  206e 0012      	moveal %fp@(18),%a0                   
    663a:  30bc 0004      	movew #4,%a0@                         
    663e:  6004           	bras 0x6644                           ; ↓ label_5

  label_4:
    6640:  363c 0301      	movew #769,%d3                        

  label_5:
    6644:  3003           	movew %d3,%d0                         
    6646:  4cdf 0c08      	moveml %sp@+,%d3/%a2-%a3              
    664a:  4e5e           	unlk %fp                              
    664c:  4e75           	rts                                   
```

# DrvStatus: 0x665c - 0x66c8

```
    665c:  4e56 0000      	linkw %fp,#0                          
    6660:  7200           	moveq #0,%d1                          
    6662:  3438 f906      	movew 0xfffffffffffff906,%d2          
    6666:  3002           	movew %d2,%d0                         
    6668:  0240 0200      	andiw #512,%d0                        
    666c:  6604           	bnes 0x6672                           ; ↓ label_0
    666e:  0041 0001      	oriw #1,%d1                           

  label_0:
    6672:  3002           	movew %d2,%d0                         
    6674:  0240 8000      	andiw #-32768,%d0                     
    6678:  6704           	beqs 0x667e                           ; ↓ label_1
    667a:  0041 0010      	oriw #16,%d1                          

  label_1:
    667e:  3002           	movew %d2,%d0                         
    6680:  0240 2000      	andiw #8192,%d0                       
    6684:  6604           	bnes 0x668a                           ; ↓ label_2
    6686:  0041 0008      	oriw #8,%d1                           

  label_2:
    668a:  3038 f908      	movew 0xfffffffffffff908,%d0          
    668e:  0240 0040      	andiw #64,%d0                         
    6692:  6704           	beqs 0x6698                           ; ↓ label_3
    6694:  0041 0002      	oriw #2,%d1                           

  label_3:
    6698:  3038 f906      	movew 0xfffffffffffff906,%d0          
    669c:  0240 1000      	andiw #4096,%d0                       
    66a0:  6704           	beqs 0x66a6                           ; ↓ label_4
    66a2:  0041 0020      	oriw #32,%d1                          

  label_4:
    66a6:  1438 f904      	moveb 0xfffffffffffff904,%d2          
    66aa:  1038 f904      	moveb 0xfffffffffffff904,%d0          
    66ae:  0240 0020      	andiw #32,%d0                         
    66b2:  6704           	beqs 0x66b8                           ; ↓ label_5
    66b4:  0041 0040      	oriw #64,%d1                          

  label_5:
    66b8:  1002           	moveb %d2,%d0                         
    66ba:  0240 000f      	andiw #15,%d0                         
    66be:  6704           	beqs 0x66c4                           ; ↓ label_6
    66c0:  0041 0080      	oriw #128,%d1                         

  label_6:
    66c4:  3001           	movew %d1,%d0                         
    66c6:  4e5e           	unlk %fp                              
    66c8:  4e75           	rts                                   
```

# DrvWriteChar: 0x6744 - 0x6752

Parameters:

   * `%fp@(12)`     : ???

```
    6744:  4e56 0000      	linkw %fp,#0                          
    6748:  11ee 000c f907 	moveb %fp@(12),0xfffffffffffff907     
    674e:  7000           	moveq #0,%d0                          
    6750:  4e5e           	unlk %fp                              
    6752:  4e75           	rts                                   
```

# PrvSetBaud: 0x6764 - 0x67c6

Parameters:

   * `%fp@(8)`      : ???

```
    6764:  4e56 0000      	linkw %fp,#0                          
    6768:  48e7 1e00      	moveml %d3-%d6,%sp@-                  
    676c:  202e 0008      	movel %fp@(8),%d0                     
    6770:  e288           	lsrl #1,%d0                           
    6772:  d0b8 012e      	addl 0x12e,%d0                        
    6776:  222e 0008      	movel %fp@(8),%d1                     
    677a:  4eba 9934      	jsr %pc@(0xb0)                        ; BBHBf6
    677e:  5040           	addqw #8,%d0                          
    6780:  e848           	lsrw #4,%d0                           
    6782:  3c00           	movew %d0,%d6                         
    6784:  3600           	movew %d0,%d3                         
    6786:  7a01           	moveq #1,%d5                          
    6788:  7800           	moveq #0,%d4                          
    678a:  0c46 0040      	cmpiw #64,%d6                         
    678e:  631a           	blss 0x67aa                           ; ↓ label_3

  label_0:
    6790:  3003           	movew %d3,%d0                         
    6792:  0240 0001      	andiw #1,%d0                          
    6796:  6706           	beqs 0x679e                           ; ↓ label_1
    6798:  e24b           	lsrw #1,%d3                           
    679a:  5243           	addqw #1,%d3                          
    679c:  6002           	bras 0x67a0                           ; ↓ label_2

  label_1:
    679e:  e24b           	lsrw #1,%d3                           

  label_2:
    67a0:  da45           	addw %d5,%d5                          
    67a2:  5244           	addqw #1,%d4                          
    67a4:  0c43 0040      	cmpiw #64,%d3                         
    67a8:  62e6           	bhis 0x6790                           ; ↑ label_0

  label_3:
    67aa:  4278 f902      	clrw 0xfffffffffffff902               
    67ae:  3004           	movew %d4,%d0                         
    67b0:  e148           	lslw #8,%d0                           
    67b2:  8178 f902      	orw %d0,0xfffffffffffff902            
    67b6:  7041           	moveq #65,%d0                         
    67b8:  9043           	subw %d3,%d0                          
    67ba:  8178 f902      	orw %d0,0xfffffffffffff902            
    67be:  7000           	moveq #0,%d0                          
    67c0:  4cdf 0078      	moveml %sp@+,%d3-%d6                  
    67c4:  4e5e           	unlk %fp                              
    67c6:  4e75           	rts                                   
```

# PrvSetFlags: 0x67d6 - 0x684e

Parameters:

   * `%fp@(8)`      : ???

```
    67d6:  4e56 0000      	linkw %fp,#0                          
    67da:  222e 0008      	movel %fp@(8),%d1                     
    67de:  4278 f900      	clrw 0xfffffffffffff900               
    67e2:  7002           	moveq #2,%d0                          
    67e4:  c081           	andl %d1,%d0                          
    67e6:  6706           	beqs 0x67ee                           ; ↓ label_0
    67e8:  0078 0800 f900 	oriw #2048,0xfffffffffffff900         

  label_0:
    67ee:  7004           	moveq #4,%d0                          
    67f0:  c081           	andl %d1,%d0                          
    67f2:  6606           	bnes 0x67fa                           ; ↓ label_1
    67f4:  0078 0400 f900 	oriw #1024,0xfffffffffffff900         

  label_1:
    67fa:  7001           	moveq #1,%d0                          
    67fc:  c081           	andl %d1,%d0                          
    67fe:  6706           	beqs 0x6806                           ; ↓ label_2
    6800:  0078 0200 f900 	oriw #512,0xfffffffffffff900          

  label_2:
    6806:  203c 0000 00c0 	movel #192,%d0                        
    680c:  c081           	andl %d1,%d0                          
    680e:  0c80 0000 00c0 	cmpil #192,%d0                        
    6814:  6606           	bnes 0x681c                           ; ↓ label_3
    6816:  0078 0100 f900 	oriw #256,0xfffffffffffff900          

  label_3:
    681c:  0078 0008 f900 	oriw #8,0xfffffffffffff900            
    6822:  4278 f906      	clrw 0xfffffffffffff906               
    6826:  7020           	moveq #32,%d0                         
    6828:  c081           	andl %d1,%d0                          
    682a:  6606           	bnes 0x6832                           ; ↓ label_4
    682c:  0078 0800 f906 	oriw #2048,0xfffffffffffff906         

  label_4:
    6832:  0278 0300 f908 	andiw #768,0xfffffffffffff908         
    6838:  0078 0040 f908 	oriw #64,0xfffffffffffff908           
    683e:  7010           	moveq #16,%d0                         
    6840:  c081           	andl %d1,%d0                          
    6842:  6706           	beqs 0x684a                           ; ↓ label_5
    6844:  0078 0080 f908 	oriw #128,0xfffffffffffff908          

  label_5:
    684a:  7000           	moveq #0,%d0                          
    684c:  4e5e           	unlk %fp                              
    684e:  4e75           	rts                                   
```

# PrvCalculateNIPRPulseWidthValu: 0x685e - 0x68ea

Locals:

   * `%fp@(-4)`     : ???

```
    685e:  4e56 fffc      	linkw %fp,#-4                         
    6862:  48e7 1c00      	moveml %d3-%d5,%sp@-                  
    6866:  7600           	moveq #0,%d3                          
    6868:  1a3c 0080      	moveb #-128,%d5                       
    686c:  283c 0038 4000 	movel #3686400,%d4                    
    6872:  0cb8 0038 4000 	cmpil #3686400,0x12e                  
    6878:  012e                                                  
    687a:  6404           	bccs 0x6880                           ; ↓ label_0
    687c:  7000           	moveq #0,%d0                          
    687e:  6064           	bras 0x68e4                           ; ↓ label_4

  label_0:
    6880:  d884           	addl %d4,%d4                          

  label_1:
    6882:  b8b8 012e      	cmpl 0x12e,%d4                        
    6886:  640c           	bccs 0x6894                           ; ↓ label_2
    6888:  5203           	addqb #1,%d3                          
    688a:  e20d           	lsrb #1,%d5                           
    688c:  d884           	addl %d4,%d4                          
    688e:  0c03 0007      	cmpib #7,%d3                          
    6892:  63ee           	blss 0x6882                           ; ↑ label_1

  label_2:
    6894:  0c03 0007      	cmpib #7,%d3                          
    6898:  6504           	bcss 0x689e                           ; ↓ label_3
    689a:  7000           	moveq #0,%d0                          
    689c:  6046           	bras 0x68e4                           ; ↓ label_4

  label_3:
    689e:  7000           	moveq #0,%d0                          
    68a0:  1005           	moveb %d5,%d0                         
    68a2:  48c0           	extl %d0                              
    68a4:  2d40 fffc      	movel %d0,%fp@(-4)                    
    68a8:  2038 012e      	movel 0x12e,%d0                       
    68ac:  720a           	moveq #10,%d1                         
    68ae:  4eba 97ce      	jsr %pc@(0x7e)                        ; anonymous_function_0
    68b2:  223c 001c 2000 	movel #1843200,%d1                    
    68b8:  4eba 97f6      	jsr %pc@(0xb0)                        ; BBHBf6
    68bc:  5a80           	addql #5,%d0                          
    68be:  720a           	moveq #10,%d1                         
    68c0:  4eba 97ee      	jsr %pc@(0xb0)                        ; BBHBf6
    68c4:  7200           	moveq #0,%d1                          
    68c6:  1203           	moveb %d3,%d1                         
    68c8:  7402           	moveq #2,%d2                          
    68ca:  e36a           	lslw %d1,%d2                          
    68cc:  48c2           	extl %d2                              
    68ce:  9082           	subl %d2,%d0                          
    68d0:  222e fffc      	movel %fp@(-4),%d1                    
    68d4:  4eba 97a8      	jsr %pc@(0x7e)                        ; anonymous_function_0
    68d8:  7200           	moveq #0,%d1                          
    68da:  1200           	moveb %d0,%d1                         
    68dc:  7000           	moveq #0,%d0                          
    68de:  1003           	moveb %d3,%d0                         
    68e0:  e148           	lslw #8,%d0                           
    68e2:  8041           	orw %d1,%d0                           

  label_4:
    68e4:  4cdf 0038      	moveml %sp@+,%d3-%d5                  
    68e8:  4e5e           	unlk %fp                              
    68ea:  4e75           	rts                                   
```

# KeyInit: 0x690e - 0x69d8

```
    690e:  4e56 0000      	linkw %fp,#0                          
    6912:  2f0a           	movel %a2,%sp@-                       
    6914:  2f03           	movel %d3,%sp@-                       
    6916:  4ab8 0188      	tstl 0x188                            
    691a:  6708           	beqs 0x6924                           ; ↓ label_0
    691c:  363c 0502      	movew #1282,%d3                       
    6920:  6000 00ae      	braw 0x69d0                           ; ↓ label_2

  label_0:
    6924:  4878 0024      	pea 0x24                              
    6928:  4e4f           	trap #15                              
    692a:  a013           	0120023                               ; sysTrapMemPtrNew
    692c:  2448           	moveal %a0,%a2                        
    692e:  200a           	movel %a2,%d0                         
    6930:  584f           	addqw #4,%sp                          
    6932:  6608           	bnes 0x693c                           ; ↓ label_1
    6934:  363c 0102      	movew #258,%d3                        
    6938:  6000 0096      	braw 0x69d0                           ; ↓ label_2

  label_1:
    693c:  4227           	clrb %sp@-                            
    693e:  4878 0024      	pea 0x24                              
    6942:  2f0a           	movel %a2,%sp@-                       
    6944:  4e4f           	trap #15                              
    6946:  a027           	0120047                               ; sysTrapMemSet
    6948:  21ca 0188      	movel %a2,0x188                       
    694c:  157c 0032 0007 	moveb #50,%a2@(7)                     
    6952:  157c 000c 0008 	moveb #12,%a2@(8)                     
    6958:  357c 00c8 0012 	movew #200,%a2@(18)                   
    695e:  257c 0000 01ff 	movel #511,%a2@(28)                   
    6964:  001c                                                  
    6966:  157c 00ff 0005 	moveb #-1,%a2@(5)                     
    696c:  0238 00f8 f418 	andib #-8,0xfffffffffffff418          
    6972:  0038 0007 f41a 	orib #7,0xfffffffffffff41a            
    6978:  0038 0007 f41d 	orib #7,0xfffffffffffff41d            
    697e:  0038 0007 f41c 	orib #7,0xfffffffffffff41c            
    6984:  0038 0007 f41f 	orib #7,0xfffffffffffff41f            
    698a:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    698e:  0200 fff0      	andib #-16,%d0                        
    6992:  0000 0007      	orib #7,%d0                           
    6996:  11c0 f419      	moveb %d0,0xfffffffffffff419          
    699a:  0038 0007 f410 	orib #7,0xfffffffffffff410            
    69a0:  0238 00f8 f412 	andib #-8,0xfffffffffffff412          
    69a6:  0238 00f8 f411 	andib #-8,0xfffffffffffff411          
    69ac:  0038 0007 f413 	orib #7,0xfffffffffffff413            
    69b2:  0278 f8ff f306 	andiw #-1793,0xfffffffffffff306       
    69b8:  7600           	moveq #0,%d3                          
    69ba:  0078 0800 f302 	oriw #2048,0xfffffffffffff302         
    69c0:  0278 7fff f302 	andiw #32767,0xfffffffffffff302       
    69c6:  0278 fffe f304 	andiw #-2,0xfffffffffffff304          
    69cc:  4fef 000a      	lea %sp@(10),%sp                      

  label_2:
    69d0:  3003           	movew %d3,%d0                         
    69d2:  261f           	movel %sp@+,%d3                       
    69d4:  245f           	moveal %sp@+,%a2                      
    69d6:  4e5e           	unlk %fp                              
    69d8:  4e75           	rts                                   
```

# prvDelay: 0x69e4 - 0x69ec

```
    69e4:  222f 0004      	movel %sp@(4),%d1                     
    69e8:  51c9 fffe      	dbf %d1,0x69e8                        
    69ec:  4e75           	rts                                   
```

# prvKeyScan: 0x69fa - 0x6b86

Locals:

   * `%fp@(-1)`     : ???
   * `%fp@(-4)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???

```
    69fa:  4e56 fff6      	linkw %fp,#-10                        
    69fe:  48e7 1f00      	moveml %d3-%d7,%sp@-                  
    6a02:  1838 f418      	moveb 0xfffffffffffff418,%d4          
    6a06:  1a38 f41a      	moveb 0xfffffffffffff41a,%d5          
    6a0a:  1c38 f410      	moveb 0xfffffffffffff410,%d6          
    6a0e:  1e38 f411      	moveb 0xfffffffffffff411,%d7          
    6a12:  1d78 f412 ffff 	moveb 0xfffffffffffff412,%fp@(-1)     
    6a18:  7600           	moveq #0,%d3                          
    6a1a:  3038 f304      	movew 0xfffffffffffff304,%d0          
    6a1e:  0240 0008      	andiw #8,%d0                          
    6a22:  3d40 fffa      	movew %d0,%fp@(-6)                    
    6a26:  0078 0008 f304 	oriw #8,0xfffffffffffff304            
    6a2c:  0238 00f8 f410 	andib #-8,0xfffffffffffff410          
    6a32:  0238 00f8 f412 	andib #-8,0xfffffffffffff412          
    6a38:  0038 0007 f418 	orib #7,0xfffffffffffff418            
    6a3e:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    6a42:  0200 fff0      	andib #-16,%d0                        
    6a46:  0000 0007      	orib #7,%d0                           
    6a4a:  11c0 f419      	moveb %d0,0xfffffffffffff419          
    6a4e:  0238 00f8 f41a 	andib #-8,0xfffffffffffff41a          
    6a54:  4878 0064      	pea 0x64                              
    6a58:  4eba ff8a      	jsr %pc@(0x69e4)                      ; prvDelay
    6a5c:  0238 00f8 f418 	andib #-8,0xfffffffffffff418          
    6a62:  0038 0007 f41a 	orib #7,0xfffffffffffff41a            
    6a68:  0038 0001 f410 	orib #1,0xfffffffffffff410            
    6a6e:  0238 00fe f411 	andib #-2,0xfffffffffffff411          
    6a74:  4878 0064      	pea 0x64                              
    6a78:  4eba ff6a      	jsr %pc@(0x69e4)                      ; prvDelay
    6a7c:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    6a80:  0240 0007      	andiw #7,%d0                          
    6a84:  3d40 fffc      	movew %d0,%fp@(-4)                    
    6a88:  7000           	moveq #0,%d0                          
    6a8a:  302e fffc      	movew %fp@(-4),%d0                    
    6a8e:  8680           	orl %d0,%d3                           
    6a90:  0238 00f8 f410 	andib #-8,0xfffffffffffff410          
    6a96:  0238 00f8 f412 	andib #-8,0xfffffffffffff412          
    6a9c:  0038 0007 f418 	orib #7,0xfffffffffffff418            
    6aa2:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    6aa6:  0200 fff0      	andib #-16,%d0                        
    6aaa:  0000 0007      	orib #7,%d0                           
    6aae:  11c0 f419      	moveb %d0,0xfffffffffffff419          
    6ab2:  0238 00f8 f41a 	andib #-8,0xfffffffffffff41a          
    6ab8:  4878 0064      	pea 0x64                              
    6abc:  4eba ff26      	jsr %pc@(0x69e4)                      ; prvDelay
    6ac0:  0238 00f8 f418 	andib #-8,0xfffffffffffff418          
    6ac6:  0038 0007 f41a 	orib #7,0xfffffffffffff41a            
    6acc:  0038 0002 f410 	orib #2,0xfffffffffffff410            
    6ad2:  0238 00fd f411 	andib #-3,0xfffffffffffff411          
    6ad8:  4878 0064      	pea 0x64                              
    6adc:  4eba ff06      	jsr %pc@(0x69e4)                      ; prvDelay
    6ae0:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    6ae4:  0240 0007      	andiw #7,%d0                          
    6ae8:  3d40 fff6      	movew %d0,%fp@(-10)                   
    6aec:  e948           	lslw #4,%d0                           
    6aee:  7200           	moveq #0,%d1                          
    6af0:  3200           	movew %d0,%d1                         
    6af2:  8681           	orl %d1,%d3                           
    6af4:  0238 00f8 f410 	andib #-8,0xfffffffffffff410          
    6afa:  0238 00f8 f412 	andib #-8,0xfffffffffffff412          
    6b00:  0038 0007 f418 	orib #7,0xfffffffffffff418            
    6b06:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    6b0a:  0200 fff0      	andib #-16,%d0                        
    6b0e:  0000 0007      	orib #7,%d0                           
    6b12:  11c0 f419      	moveb %d0,0xfffffffffffff419          
    6b16:  0238 00f8 f41a 	andib #-8,0xfffffffffffff41a          
    6b1c:  4878 0064      	pea 0x64                              
    6b20:  4eba fec2      	jsr %pc@(0x69e4)                      ; prvDelay
    6b24:  0238 00f8 f418 	andib #-8,0xfffffffffffff418          
    6b2a:  0038 0007 f41a 	orib #7,0xfffffffffffff41a            
    6b30:  0038 0004 f410 	orib #4,0xfffffffffffff410            
    6b36:  0238 00fb f411 	andib #-5,0xfffffffffffff411          
    6b3c:  4878 0064      	pea 0x64                              
    6b40:  4eba fea2      	jsr %pc@(0x69e4)                      ; prvDelay
    6b44:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    6b48:  0240 0007      	andiw #7,%d0                          
    6b4c:  3d40 fff8      	movew %d0,%fp@(-8)                    
    6b50:  e148           	lslw #8,%d0                           
    6b52:  7200           	moveq #0,%d1                          
    6b54:  3200           	movew %d0,%d1                         
    6b56:  8681           	orl %d1,%d3                           
    6b58:  11c4 f418      	moveb %d4,0xfffffffffffff418          
    6b5c:  11c5 f41a      	moveb %d5,0xfffffffffffff41a          
    6b60:  11c6 f410      	moveb %d6,0xfffffffffffff410          
    6b64:  11c7 f411      	moveb %d7,0xfffffffffffff411          
    6b68:  11ee ffff f412 	moveb %fp@(-1),0xfffffffffffff412     
    6b6e:  4a6e fffa      	tstw %fp@(-6)                         
    6b72:  4fef 0018      	lea %sp@(24),%sp                      
    6b76:  6606           	bnes 0x6b7e                           ; ↓ label_0
    6b78:  0278 fff7 f304 	andiw #-9,0xfffffffffffff304          

  label_0:
    6b7e:  2003           	movel %d3,%d0                         
    6b80:  4cdf 00f8      	moveml %sp@+,%d3-%d7                  
    6b84:  4e5e           	unlk %fp                              
    6b86:  4e75           	rts                                   
```

# KeyBootKeys: 0x6b96 - 0x6c18

Locals:

   * `%fp@(-24)`    : ???

```
    6b96:  4e56 ffe8      	linkw %fp,#-24                        
    6b9a:  48e7 1e20      	moveml %d3-%d6/%a2,%sp@-              
    6b9e:  41fa 0088      	lea %pc@(0x6c28),%a0                  ; ??? 0x6c28
    6ba2:  43ee ffe8      	lea %fp@(-24),%a1                     
    6ba6:  22d8           	movel %a0@+,%a1@+                     
    6ba8:  22d8           	movel %a0@+,%a1@+                     
    6baa:  22d8           	movel %a0@+,%a1@+                     
    6bac:  22d8           	movel %a0@+,%a1@+                     
    6bae:  22d8           	movel %a0@+,%a1@+                     
    6bb0:  22d8           	movel %a0@+,%a1@+                     
    6bb2:  0038 0007 f410 	orib #7,0xfffffffffffff410            
    6bb8:  0238 00f8 f412 	andib #-8,0xfffffffffffff412          
    6bbe:  0038 0007 f413 	orib #7,0xfffffffffffff413            
    6bc4:  0238 00f8 f41d 	andib #-8,0xfffffffffffff41d          
    6bca:  0238 00f8 f418 	andib #-8,0xfffffffffffff418          
    6bd0:  0038 0007 f41a 	orib #7,0xfffffffffffff41a            
    6bd6:  0038 0007 f41c 	orib #7,0xfffffffffffff41c            
    6bdc:  0238 00f8 f41f 	andib #-8,0xfffffffffffff41f          
    6be2:  4eba fe16      	jsr %pc@(0x69fa)                      ; prvKeyScan
    6be6:  2c00           	movel %d0,%d6                         
    6be8:  7800           	moveq #0,%d4                          
    6bea:  7a00           	moveq #0,%d5                          
    6bec:  2606           	movel %d6,%d3                         
    6bee:  45ee ffe8      	lea %fp@(-24),%a2                     
    6bf2:  6012           	bras 0x6c06                           ; ↓ label_2

  label_0:
    6bf4:  7001           	moveq #1,%d0                          
    6bf6:  c083           	andl %d3,%d0                          
    6bf8:  6706           	beqs 0x6c00                           ; ↓ label_1
    6bfa:  7000           	moveq #0,%d0                          
    6bfc:  3012           	movew %a2@,%d0                        
    6bfe:  8a80           	orl %d0,%d5                           

  label_1:
    6c00:  e28b           	lsrl #1,%d3                           
    6c02:  548a           	addql #2,%a2                          
    6c04:  5284           	addql #1,%d4                          

  label_2:
    6c06:  4a83           	tstl %d3                              
    6c08:  6706           	beqs 0x6c10                           ; ↓ label_3
    6c0a:  7010           	moveq #16,%d0                         
    6c0c:  b880           	cmpl %d0,%d4                          
    6c0e:  65e4           	bcss 0x6bf4                           ; ↑ label_0

  label_3:
    6c10:  2005           	movel %d5,%d0                         
    6c12:  4cdf 0478      	moveml %sp@+,%d3-%d6/%a2              
    6c16:  4e5e           	unlk %fp                              
    6c18:  4e75           	rts                                   
```

# KeyCurrentState: 0x6c40 - 0x6c4e

```
    6c40:  4e56 0000      	linkw %fp,#0                          
    6c44:  2078 0188      	moveal 0x188,%a0                      
    6c48:  2028 0018      	movel %a0@(24),%d0                    
    6c4c:  4e5e           	unlk %fp                              
    6c4e:  4e75           	rts                                   
```

# KeySetMask: 0x6c62 - 0x6c86

Parameters:

   * `%fp@(8)`      : ???

```
    6c62:  4e56 0000      	linkw %fp,#0                          
    6c66:  2078 0188      	moveal 0x188,%a0                      
    6c6a:  2228 001c      	movel %a0@(28),%d1                    
    6c6e:  216e 0008 001c 	movel %fp@(8),%a0@(28)                
    6c74:  202e 0008      	movel %fp@(8),%d0                     
    6c78:  0280 0000 00ff 	andil #255,%d0                        
    6c7e:  1140 0005      	moveb %d0,%a0@(5)                     
    6c82:  2001           	movel %d1,%d0                         
    6c84:  4e5e           	unlk %fp                              
    6c86:  4e75           	rts                                   
```

# prvEnableKeyInterrupts: 0x6c96 - 0x6ccc

Parameters:

   * `%fp@(8)`      : ???

```
    6c96:  4e56 0000      	linkw %fp,#0                          
    6c9a:  4a2e 0008      	tstb %fp@(8)                          
    6c9e:  671e           	beqs 0x6cbe                           ; ↓ label_0
    6ca0:  0038 0007 f41d 	orib #7,0xfffffffffffff41d            
    6ca6:  0038 0007 f41f 	orib #7,0xfffffffffffff41f            
    6cac:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    6cb0:  0200 fff0      	andib #-16,%d0                        
    6cb4:  0000 0007      	orib #7,%d0                           
    6cb8:  11c0 f419      	moveb %d0,0xfffffffffffff419          
    6cbc:  600c           	bras 0x6cca                           ; ↓ label_1

  label_0:
    6cbe:  0038 0007 f41d 	orib #7,0xfffffffffffff41d            
    6cc4:  0038 0007 f41f 	orib #7,0xfffffffffffff41f            

  label_1:
    6cca:  4e5e           	unlk %fp                              
    6ccc:  4e75           	rts                                   
```

# prvDisableKeyInterrupts: 0x6ce8 - 0x6cfa

```
    6ce8:  4e56 0000      	linkw %fp,#0                          
    6cec:  0238 00f8 f41d 	andib #-8,0xfffffffffffff41d          
    6cf2:  0238 00f8 f41f 	andib #-8,0xfffffffffffff41f          
    6cf8:  4e5e           	unlk %fp                              
    6cfa:  4e75           	rts                                   
```

# PrvScanKeyStatus: 0x6d16 - 0x6d68

Locals:

   * `%fp@(-24)`    : ???

```
    6d16:  4e56 ffe8      	linkw %fp,#-24                        
    6d1a:  48e7 1e20      	moveml %d3-%d6/%a2,%sp@-              
    6d1e:  41fa 005e      	lea %pc@(0x6d7e),%a0                  ; ??? 0x6d7e
    6d22:  43ee ffe8      	lea %fp@(-24),%a1                     
    6d26:  22d8           	movel %a0@+,%a1@+                     
    6d28:  22d8           	movel %a0@+,%a1@+                     
    6d2a:  22d8           	movel %a0@+,%a1@+                     
    6d2c:  22d8           	movel %a0@+,%a1@+                     
    6d2e:  22d8           	movel %a0@+,%a1@+                     
    6d30:  22d8           	movel %a0@+,%a1@+                     
    6d32:  4eba fcc6      	jsr %pc@(0x69fa)                      ; prvKeyScan
    6d36:  2c00           	movel %d0,%d6                         
    6d38:  7800           	moveq #0,%d4                          
    6d3a:  7a00           	moveq #0,%d5                          
    6d3c:  3606           	movew %d6,%d3                         
    6d3e:  45ee ffe8      	lea %fp@(-24),%a2                     
    6d42:  6012           	bras 0x6d56                           ; ↓ label_2

  label_0:
    6d44:  0803 0000      	btst #0,%d3                           
    6d48:  6706           	beqs 0x6d50                           ; ↓ label_1
    6d4a:  7000           	moveq #0,%d0                          
    6d4c:  3012           	movew %a2@,%d0                        
    6d4e:  8a80           	orl %d0,%d5                           

  label_1:
    6d50:  e243           	asrw #1,%d3                           
    6d52:  548a           	addql #2,%a2                          
    6d54:  5244           	addqw #1,%d4                          

  label_2:
    6d56:  4a43           	tstw %d3                              
    6d58:  6706           	beqs 0x6d60                           ; ↓ label_3
    6d5a:  0c44 0010      	cmpiw #16,%d4                         
    6d5e:  6de4           	blts 0x6d44                           ; ↑ label_0

  label_3:
    6d60:  2005           	movel %d5,%d0                         
    6d62:  4cdf 0478      	moveml %sp@+,%d3-%d6/%a2              
    6d66:  4e5e           	unlk %fp                              
    6d68:  4e75           	rts                                   
```

# KeyHandleInterrupt: 0x6d96 - 0x729a

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-4)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-14)`    : ???

```
    6d96:  4e56 fff2      	linkw %fp,#-14                        
    6d9a:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    6d9e:  2c2e 000a      	movel %fp@(10),%d6                    
    6da2:  7a00           	moveq #0,%d5                          
    6da4:  7800           	moveq #0,%d4                          
    6da6:  2678 0304      	moveal 0x304,%a3                      
    6daa:  4a2e 0008      	tstb %fp@(8)                          
    6dae:  6600 0332      	bnew 0x70e2                           ; ↓ label_38
    6db2:  426e fffc      	clrw %fp@(-4)                         
    6db6:  2478 0188      	moveal 0x188,%a2                      
    6dba:  200b           	movel %a3,%d0                         
    6dbc:  6708           	beqs 0x6dc6                           ; ↓ label_0
    6dbe:  7002           	moveq #2,%d0                          
    6dc0:  c0ab 000c      	andl %a3@(12),%d0                     
    6dc4:  662c           	bnes 0x6df2                           ; ↓ label_1

  label_0:
    6dc6:  0078 0001 f30c 	oriw #1,0xfffffffffffff30c            
    6dcc:  2006           	movel %d6,%d0                         
    6dce:  0280 0001 0000 	andil #65536,%d0                      
    6dd4:  671c           	beqs 0x6df2                           ; ↓ label_1
    6dd6:  3638 f302      	movew 0xfffffffffffff302,%d3          
    6dda:  0278 f7ff f302 	andiw #-2049,0xfffffffffffff302       
    6de0:  4878 2710      	pea 0x2710                            
    6de4:  4e4f           	trap #15                              
    6de6:  a249           	0121111                               ; sysTrapHwrDelay
    6de8:  2c38 f30c      	movel 0xfffffffffffff30c,%d6          
    6dec:  31c3 f302      	movew %d3,0xfffffffffffff302          
    6df0:  584f           	addqw #4,%sp                          

  label_1:
    6df2:  1038 f419      	moveb 0xfffffffffffff419,%d0          
    6df6:  0200 fff0      	andib #-16,%d0                        
    6dfa:  0000 0007      	orib #7,%d0                           
    6dfe:  11c0 f419      	moveb %d0,0xfffffffffffff419          
    6e02:  4eba fee4      	jsr %pc@(0x6ce8)                      ; prvDisableKeyInterrupts
    6e06:  200b           	movel %a3,%d0                         
    6e08:  6708           	beqs 0x6e12                           ; ↓ label_2
    6e0a:  7002           	moveq #2,%d0                          
    6e0c:  c0ab 000c      	andl %a3@(12),%d0                     
    6e10:  660c           	bnes 0x6e1e                           ; ↓ label_3

  label_2:
    6e12:  0078 0001 f304 	oriw #1,0xfffffffffffff304            
    6e18:  0278 f7ff f302 	andiw #-2049,0xfffffffffffff302       

  label_3:
    6e1e:  7600           	moveq #0,%d3                          
    6e20:  2006           	movel %d6,%d0                         
    6e22:  0280 0000 0700 	andil #1792,%d0                       
    6e28:  6706           	beqs 0x6e30                           ; ↓ label_4
    6e2a:  4eba feea      	jsr %pc@(0x6d16)                      ; PrvScanKeyStatus
    6e2e:  2600           	movel %d0,%d3                         

  label_4:
    6e30:  4a83           	tstl %d3                              
    6e32:  6612           	bnes 0x6e46                           ; ↓ label_5
    6e34:  3038 0250      	movew 0x250,%d0                       
    6e38:  0240 0004      	andiw #4,%d0                          
    6e3c:  6708           	beqs 0x6e46                           ; ↓ label_5
    6e3e:  2e2a 0018      	movel %a2@(24),%d7                    
    6e42:  6702           	beqs 0x6e46                           ; ↓ label_5
    6e44:  2607           	movel %d7,%d3                         

  label_5:
    6e46:  2006           	movel %d6,%d0                         
    6e48:  0280 0001 0000 	andil #65536,%d0                      
    6e4e:  6706           	beqs 0x6e56                           ; ↓ label_6
    6e50:  0083 0000 0080 	oril #128,%d3                         

  label_6:
    6e56:  2543 0018      	movel %d3,%a2@(24)                    
    6e5a:  202a 0018      	movel %a2@(24),%d0                    
    6e5e:  0280 0000 00ff 	andil #255,%d0                        
    6e64:  1540 0004      	moveb %d0,%a2@(4)                     
    6e68:  4a92           	tstl %a2@                             
    6e6a:  6706           	beqs 0x6e72                           ; ↓ label_7
    6e6c:  0283 ffff fdfe 	andil #-514,%d3                       

  label_7:
    6e72:  7001           	moveq #1,%d0                          
    6e74:  c083           	andl %d3,%d0                          
    6e76:  6728           	beqs 0x6ea0                           ; ↓ label_9
    6e78:  3d7c 0208 fffc 	movew #520,%fp@(-4)                   
    6e7e:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     
    6e84:  3038 0250      	movew 0x250,%d0                       
    6e88:  0240 0004      	andiw #4,%d0                          
    6e8c:  660c           	bnes 0x6e9a                           ; ↓ label_8
    6e8e:  3038 0250      	movew 0x250,%d0                       
    6e92:  0240 0010      	andiw #16,%d0                         
    6e96:  6700 0132      	beqw 0x6fca                           ; ↓ label_24

  label_8:
    6e9a:  7801           	moveq #1,%d4                          
    6e9c:  6000 012c      	braw 0x6fca                           ; ↓ label_24

  label_9:
    6ea0:  7002           	moveq #2,%d0                          
    6ea2:  c083           	andl %d3,%d0                          
    6ea4:  6710           	beqs 0x6eb6                           ; ↓ label_10
    6ea6:  3d7c 000b fffc 	movew #11,%fp@(-4)                    
    6eac:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     
    6eb2:  6000 0116      	braw 0x6fca                           ; ↓ label_24

  label_10:
    6eb6:  7004           	moveq #4,%d0                          
    6eb8:  c083           	andl %d3,%d0                          
    6eba:  6710           	beqs 0x6ecc                           ; ↓ label_11
    6ebc:  3d7c 000c fffc 	movew #12,%fp@(-4)                    
    6ec2:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     
    6ec8:  6000 0100      	braw 0x6fca                           ; ↓ label_24

  label_11:
    6ecc:  7008           	moveq #8,%d0                          
    6ece:  c083           	andl %d3,%d0                          
    6ed0:  6710           	beqs 0x6ee2                           ; ↓ label_12
    6ed2:  3d7c 0204 fffc 	movew #516,%fp@(-4)                   
    6ed8:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     
    6ede:  6000 00ea      	braw 0x6fca                           ; ↓ label_24

  label_12:
    6ee2:  7010           	moveq #16,%d0                         
    6ee4:  c083           	andl %d3,%d0                          
    6ee6:  6710           	beqs 0x6ef8                           ; ↓ label_13
    6ee8:  3d7c 0205 fffc 	movew #517,%fp@(-4)                   
    6eee:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     
    6ef4:  6000 00d4      	braw 0x6fca                           ; ↓ label_24

  label_13:
    6ef8:  7020           	moveq #32,%d0                         
    6efa:  c083           	andl %d3,%d0                          
    6efc:  6710           	beqs 0x6f0e                           ; ↓ label_14
    6efe:  3d7c 0206 fffc 	movew #518,%fp@(-4)                   
    6f04:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     
    6f0a:  6000 00be      	braw 0x6fca                           ; ↓ label_24

  label_14:
    6f0e:  7040           	moveq #64,%d0                         
    6f10:  c083           	andl %d3,%d0                          
    6f12:  6710           	beqs 0x6f24                           ; ↓ label_15
    6f14:  3d7c 0207 fffc 	movew #519,%fp@(-4)                   
    6f1a:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     
    6f20:  6000 00a8      	braw 0x6fca                           ; ↓ label_24

  label_15:
    6f24:  2c03           	movel %d3,%d6                         
    6f26:  0286 0000 0400 	andil #1024,%d6                       
    6f2c:  660a           	bnes 0x6f38                           ; ↓ label_16
    6f2e:  2003           	movel %d3,%d0                         
    6f30:  0280 0000 0800 	andil #2048,%d0                       
    6f36:  673a           	beqs 0x6f72                           ; ↓ label_19

  label_16:
    6f38:  200b           	movel %a3,%d0                         
    6f3a:  6700 008e      	beqw 0x6fca                           ; ↓ label_24
    6f3e:  4a86           	tstl %d6                              
    6f40:  6708           	beqs 0x6f4a                           ; ↓ label_17
    6f42:  203c 0000 0400 	movel #1024,%d0                       
    6f48:  6006           	bras 0x6f50                           ; ↓ label_18

  label_17:
    6f4a:  203c 0000 0800 	movel #2048,%d0                       

  label_18:
    6f50:  2600           	movel %d0,%d3                         
    6f52:  2e2b 005c      	movel %a3@(92),%d7                    
    6f56:  2c07           	movel %d7,%d6                         
    6f58:  4a87           	tstl %d7                              
    6f5a:  676e           	beqs 0x6fca                           ; ↓ label_24
    6f5c:  486e fffe      	pea %fp@(-2)                          
    6f60:  486e fffc      	pea %fp@(-4)                          
    6f64:  2f03           	movel %d3,%sp@-                       
    6f66:  2046           	moveal %d6,%a0                        
    6f68:  4e90           	jsr %a0@                              
    6f6a:  1800           	moveb %d0,%d4                         
    6f6c:  4fef 000c      	lea %sp@(12),%sp                      
    6f70:  6058           	bras 0x6fca                           ; ↓ label_24

  label_19:
    6f72:  2003           	movel %d3,%d0                         
    6f74:  0280 0000 0080 	andil #128,%d0                        
    6f7a:  6738           	beqs 0x6fb4                           ; ↓ label_23
    6f7c:  200b           	movel %a3,%d0                         
    6f7e:  6708           	beqs 0x6f88                           ; ↓ label_20
    6f80:  7002           	moveq #2,%d0                          
    6f82:  c0ab 000c      	andl %a3@(12),%d0                     
    6f86:  662c           	bnes 0x6fb4                           ; ↓ label_23

  label_20:
    6f88:  42a7           	clrl %sp@-                            
    6f8a:  486e fffa      	pea %fp@(-6)                          
    6f8e:  4e4f           	trap #15                              
    6f90:  a257           	0121127                               ; sysTrapHwrDockSignals
    6f92:  302e fffa      	movew %fp@(-6),%d0                    
    6f96:  0240 0002      	andiw #2,%d0                          
    6f9a:  504f           	addqw #8,%sp                          
    6f9c:  6708           	beqs 0x6fa6                           ; ↓ label_21
    6f9e:  3d7c 020a fffc 	movew #522,%fp@(-4)                   
    6fa4:  6006           	bras 0x6fac                           ; ↓ label_22

  label_21:
    6fa6:  3d7c 0209 fffc 	movew #521,%fp@(-4)                   

  label_22:
    6fac:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     
    6fb2:  6016           	bras 0x6fca                           ; ↓ label_24

  label_23:
    6fb4:  2003           	movel %d3,%d0                         
    6fb6:  0280 0000 0200 	andil #512,%d0                        
    6fbc:  670c           	beqs 0x6fca                           ; ↓ label_24
    6fbe:  3d7c 020b fffc 	movew #523,%fp@(-4)                   
    6fc4:  3d7c 0008 fffe 	movew #8,%fp@(-2)                     

  label_24:
    6fca:  202a 0018      	movel %a2@(24),%d0                    
    6fce:  c0aa 001c      	andl %a2@(28),%d0                     
    6fd2:  660c           	bnes 0x6fe0                           ; ↓ label_25
    6fd4:  3038 0250      	movew 0x250,%d0                       
    6fd8:  0240 0004      	andiw #4,%d0                          
    6fdc:  6602           	bnes 0x6fe0                           ; ↓ label_25
    6fde:  7801           	moveq #1,%d4                          

  label_25:
    6fe0:  4a6e fffc      	tstw %fp@(-4)                         
    6fe4:  6700 00b6      	beqw 0x709c                           ; ↓ label_34
    6fe8:  3038 0250      	movew 0x250,%d0                       
    6fec:  0240 0004      	andiw #4,%d0                          
    6ff0:  660a           	bnes 0x6ffc                           ; ↓ label_26
    6ff2:  3038 0250      	movew 0x250,%d0                       
    6ff6:  0240 0010      	andiw #16,%d0                         
    6ffa:  670a           	beqs 0x7006                           ; ↓ label_27

  label_26:
    6ffc:  006e 0100 fffe 	oriw #256,%fp@(-2)                    
    7002:  42aa 000e      	clrl %a2@(14)                         

  label_27:
    7006:  4a92           	tstl %a2@                             
    7008:  6708           	beqs 0x7012                           ; ↓ label_28
    700a:  006e 0040 fffe 	oriw #64,%fp@(-2)                     
    7010:  602a           	bras 0x703c                           ; ↓ label_29

  label_28:
    7012:  262a 000e      	movel %a2@(14),%d3                    
    7016:  6724           	beqs 0x703c                           ; ↓ label_29
    7018:  7000           	moveq #0,%d0                          
    701a:  302a 0012      	movew %a2@(18),%d0                    
    701e:  2238 0254      	movel 0x254,%d1                       
    7022:  9283           	subl %d3,%d1                          
    7024:  b280           	cmpl %d0,%d1                          
    7026:  6414           	bccs 0x703c                           ; ↓ label_29
    7028:  202a 0018      	movel %a2@(24),%d0                    
    702c:  b0aa 0020      	cmpl %a2@(32),%d0                     
    7030:  660a           	bnes 0x703c                           ; ↓ label_29
    7032:  006e 0080 fffe 	oriw #128,%fp@(-2)                    
    7038:  42aa 000e      	clrl %a2@(14)                         

  label_29:
    703c:  2578 0254 000e 	movel 0x254,%a2@(14)                  
    7042:  256a 0018 0020 	movel %a2@(24),%a2@(32)               
    7048:  202a 0018      	movel %a2@(24),%d0                    
    704c:  0280 0000 00ff 	andil #255,%d0                        
    7052:  1540 0014      	moveb %d0,%a2@(20)                    
    7056:  3038 0250      	movew 0x250,%d0                       
    705a:  0240 0004      	andiw #4,%d0                          
    705e:  6714           	beqs 0x7074                           ; ↓ label_31
    7060:  0c6e 0208 fffc 	cmpiw #520,%fp@(-4)                   
    7066:  6606           	bnes 0x706e                           ; ↓ label_30
    7068:  2578 0254 000a 	movel 0x254,%a2@(10)                  

  label_30:
    706e:  0278 fffb 0250 	andiw #-5,0x250                       

  label_31:
    7074:  4a04           	tstb %d4                              
    7076:  6612           	bnes 0x708a                           ; ↓ label_32
    7078:  3f2e fffe      	movew %fp@(-2),%sp@-                  
    707c:  4267           	clrw %sp@-                            
    707e:  3f2e fffc      	movew %fp@(-4),%sp@-                  
    7082:  4e4f           	trap #15                              
    7084:  a12d           	0120455                               ; sysTrapEvtEnqueueKey
    7086:  3a00           	movew %d0,%d5                         
    7088:  5c4f           	addqw #6,%sp                          

  label_32:
    708a:  4a92           	tstl %a2@                             
    708c:  6604           	bnes 0x7092                           ; ↓ label_33
    708e:  4e4f           	trap #15                              
    7090:  a130           	0120460                               ; sysTrapEvtResetAutoOffTimer

  label_33:
    7092:  4a92           	tstl %a2@                             
    7094:  6706           	beqs 0x709c                           ; ↓ label_34
    7096:  7000           	moveq #0,%d0                          
    7098:  6000 01fa      	braw 0x7294                           ; ↓ label_53

  label_34:
    709c:  4a45           	tstw %d5                              
    709e:  6726           	beqs 0x70c6                           ; ↓ label_37
    70a0:  4227           	clrb %sp@-                            
    70a2:  4eba fbf2      	jsr %pc@(0x6c96)                      ; prvEnableKeyInterrupts
    70a6:  200b           	movel %a3,%d0                         
    70a8:  544f           	addqw #2,%sp                          
    70aa:  6708           	beqs 0x70b4                           ; ↓ label_35
    70ac:  7002           	moveq #2,%d0                          
    70ae:  c0ab 000c      	andl %a3@(12),%d0                     
    70b2:  660c           	bnes 0x70c0                           ; ↓ label_36

  label_35:
    70b4:  0278 fffe f304 	andiw #-2,0xfffffffffffff304          
    70ba:  0078 0800 f302 	oriw #2048,0xfffffffffffff302         

  label_36:
    70c0:  7000           	moveq #0,%d0                          
    70c2:  6000 01d0      	braw 0x7294                           ; ↓ label_53

  label_37:
    70c6:  24b8 0254      	movel 0x254,%a2@                      
    70ca:  422a 0006      	clrb %a2@(6)                          
    70ce:  4878 0001      	pea 0x1                               
    70d2:  2f38 014e      	movel 0x14e,%sp@-                     
    70d6:  4e4f           	trap #15                              
    70d8:  a09a           	0120232                               ; sysTrapSysTimerWrite
    70da:  7001           	moveq #1,%d0                          
    70dc:  504f           	addqw #8,%sp                          
    70de:  6000 01b4      	braw 0x7294                           ; ↓ label_53

  label_38:
    70e2:  2478 0188      	moveal 0x188,%a2                      
    70e6:  2078 0188      	moveal 0x188,%a0                      
    70ea:  4a90           	tstl %a0@                             
    70ec:  6606           	bnes 0x70f4                           ; ↓ label_39
    70ee:  7000           	moveq #0,%d0                          
    70f0:  6000 01a2      	braw 0x7294                           ; ↓ label_53

  label_39:
    70f4:  200b           	movel %a3,%d0                         
    70f6:  6716           	beqs 0x710e                           ; ↓ label_40
    70f8:  7002           	moveq #2,%d0                          
    70fa:  c0ab 000c      	andl %a3@(12),%d0                     
    70fe:  670e           	beqs 0x710e                           ; ↓ label_40
    7100:  1838 f419      	moveb 0xfffffffffffff419,%d4          
    7104:  0244 0007      	andiw #7,%d4                          
    7108:  48c4           	extl %d4                              
    710a:  e18c           	lsll #8,%d4                           
    710c:  601e           	bras 0x712c                           ; ↓ label_41

  label_40:
    710e:  3838 f310      	movew 0xfffffffffffff310,%d4          
    7112:  0244 0001      	andiw #1,%d4                          
    7116:  7200           	moveq #0,%d1                          
    7118:  3204           	movew %d4,%d1                         
    711a:  4841           	swap %d1                              
    711c:  4241           	clrw %d1                              
    711e:  1838 f419      	moveb 0xfffffffffffff419,%d4          
    7122:  0244 0007      	andiw #7,%d4                          
    7126:  48c4           	extl %d4                              
    7128:  e18c           	lsll #8,%d4                           
    712a:  8881           	orl %d1,%d4                           

  label_41:
    712c:  4a84           	tstl %d4                              
    712e:  6600 0080      	bnew 0x71b0                           ; ↓ label_46
    7132:  522a 0006      	addqb #1,%a2@(6)                      
    7136:  0c2a 0005 0006 	cmpib #5,%a2@(6)                      
    713c:  6406           	bccs 0x7144                           ; ↓ label_42
    713e:  7001           	moveq #1,%d0                          
    7140:  6000 0152      	braw 0x7294                           ; ↓ label_53

  label_42:
    7144:  4292           	clrl %a2@                             
    7146:  200b           	movel %a3,%d0                         
    7148:  6732           	beqs 0x717c                           ; ↓ label_43
    714a:  282b 005c      	movel %a3@(92),%d4                    
    714e:  2604           	movel %d4,%d3                         
    7150:  4a84           	tstl %d4                              
    7152:  6728           	beqs 0x717c                           ; ↓ label_43
    7154:  486e fff6      	pea %fp@(-10)                         
    7158:  486e fff8      	pea %fp@(-8)                          
    715c:  4878 ffff      	pea 0xffffffffffffffff                
    7160:  2043           	moveal %d3,%a0                        
    7162:  4e90           	jsr %a0@                              
    7164:  4a00           	tstb %d0                              
    7166:  4fef 000c      	lea %sp@(12),%sp                      
    716a:  6610           	bnes 0x717c                           ; ↓ label_43
    716c:  3f2e fff6      	movew %fp@(-10),%sp@-                 
    7170:  4267           	clrw %sp@-                            
    7172:  3f2e fff8      	movew %fp@(-8),%sp@-                  
    7176:  4e4f           	trap #15                              
    7178:  a12d           	0120455                               ; sysTrapEvtEnqueueKey
    717a:  5c4f           	addqw #6,%sp                          

  label_43:
    717c:  1f3c 0001      	moveb #1,%sp@-                        
    7180:  4eba fb14      	jsr %pc@(0x6c96)                      ; prvEnableKeyInterrupts
    7184:  200b           	movel %a3,%d0                         
    7186:  544f           	addqw #2,%sp                          
    7188:  6708           	beqs 0x7192                           ; ↓ label_44
    718a:  7002           	moveq #2,%d0                          
    718c:  c0ab 000c      	andl %a3@(12),%d0                     
    7190:  660c           	bnes 0x719e                           ; ↓ label_45

  label_44:
    7192:  0078 0800 f302 	oriw #2048,0xfffffffffffff302         
    7198:  0278 fffe f304 	andiw #-2,0xfffffffffffff304          

  label_45:
    719e:  42aa 0018      	clrl %a2@(24)                         
    71a2:  422a 0004      	clrb %a2@(4)                          
    71a6:  42aa 000a      	clrl %a2@(10)                         
    71aa:  7000           	moveq #0,%d0                          
    71ac:  6000 00e6      	braw 0x7294                           ; ↓ label_53

  label_46:
    71b0:  4eba fb64      	jsr %pc@(0x6d16)                      ; PrvScanKeyStatus
    71b4:  2600           	movel %d0,%d3                         
    71b6:  2543 0018      	movel %d3,%a2@(24)                    
    71ba:  202a 0018      	movel %a2@(24),%d0                    
    71be:  0280 0000 00ff 	andil #255,%d0                        
    71c4:  1540 0004      	moveb %d0,%a2@(4)                     
    71c8:  707f           	moveq #127,%d0                        
    71ca:  c083           	andl %d3,%d0                          
    71cc:  6736           	beqs 0x7204                           ; ↓ label_47
    71ce:  200b           	movel %a3,%d0                         
    71d0:  6732           	beqs 0x7204                           ; ↓ label_47
    71d2:  2c2b 005c      	movel %a3@(92),%d6                    
    71d6:  2a06           	movel %d6,%d5                         
    71d8:  4a86           	tstl %d6                              
    71da:  6728           	beqs 0x7204                           ; ↓ label_47
    71dc:  486e fff2      	pea %fp@(-14)                         
    71e0:  486e fff4      	pea %fp@(-12)                         
    71e4:  4878 ffff      	pea 0xffffffffffffffff                
    71e8:  2045           	moveal %d5,%a0                        
    71ea:  4e90           	jsr %a0@                              
    71ec:  4a00           	tstb %d0                              
    71ee:  4fef 000c      	lea %sp@(12),%sp                      
    71f2:  6610           	bnes 0x7204                           ; ↓ label_47
    71f4:  3f2e fff2      	movew %fp@(-14),%sp@-                 
    71f8:  4267           	clrw %sp@-                            
    71fa:  3f2e fff4      	movew %fp@(-12),%sp@-                 
    71fe:  4e4f           	trap #15                              
    7200:  a12d           	0120455                               ; sysTrapEvtEnqueueKey
    7202:  5c4f           	addqw #6,%sp                          

  label_47:
    7204:  2a2a 000a      	movel %a2@(10),%d5                    
    7208:  671e           	beqs 0x7228                           ; ↓ label_48
    720a:  7064           	moveq #100,%d0                        
    720c:  d085           	addl %d5,%d0                          
    720e:  b0b8 0254      	cmpl 0x254,%d0                        
    7212:  6414           	bccs 0x7228                           ; ↓ label_48
    7214:  42aa 000a      	clrl %a2@(10)                         
    7218:  3f3c 0008      	movew #8,%sp@-                        
    721c:  2f3c 0113 0000 	movel #18022400,%sp@-                 
    7222:  4e4f           	trap #15                              
    7224:  a12d           	0120455                               ; sysTrapEvtEnqueueKey
    7226:  5c4f           	addqw #6,%sp                          

  label_48:
    7228:  422a 0006      	clrb %a2@(6)                          
    722c:  0283 ffff fdfe 	andil #-514,%d3                       
    7232:  6604           	bnes 0x7238                           ; ↓ label_49
    7234:  7001           	moveq #1,%d0                          
    7236:  605c           	bras 0x7294                           ; ↓ label_53

  label_49:
    7238:  4a2a 0009      	tstb %a2@(9)                          
    723c:  660c           	bnes 0x724a                           ; ↓ label_50
    723e:  4e4f           	trap #15                              
    7240:  a131           	0120461                               ; sysTrapEvtKeyQueueEmpty
    7242:  4a00           	tstb %d0                              
    7244:  6604           	bnes 0x724a                           ; ↓ label_50
    7246:  7001           	moveq #1,%d0                          
    7248:  604a           	bras 0x7294                           ; ↓ label_53

  label_50:
    724a:  2638 0254      	movel 0x254,%d3                       
    724e:  2c12           	movel %a2@,%d6                        
    7250:  2a06           	movel %d6,%d5                         
    7252:  4a86           	tstl %d6                              
    7254:  6f16           	bles 0x726c                           ; ↓ label_51
    7256:  7000           	moveq #0,%d0                          
    7258:  102a 0007      	moveb %a2@(7),%d0                     
    725c:  48c0           	extl %d0                              
    725e:  2203           	movel %d3,%d1                         
    7260:  9285           	subl %d5,%d1                          
    7262:  2c01           	movel %d1,%d6                         
    7264:  b280           	cmpl %d0,%d1                          
    7266:  6c1a           	bges 0x7282                           ; ↓ label_52
    7268:  7001           	moveq #1,%d0                          
    726a:  6028           	bras 0x7294                           ; ↓ label_53

  label_51:
    726c:  7000           	moveq #0,%d0                          
    726e:  102a 0008      	moveb %a2@(8),%d0                     
    7272:  48c0           	extl %d0                              
    7274:  2203           	movel %d3,%d1                         
    7276:  d285           	addl %d5,%d1                          
    7278:  2c01           	movel %d1,%d6                         
    727a:  b280           	cmpl %d0,%d1                          
    727c:  6c04           	bges 0x7282                           ; ↓ label_52
    727e:  7001           	moveq #1,%d0                          
    7280:  6012           	bras 0x7294                           ; ↓ label_53

  label_52:
    7282:  2f04           	movel %d4,%sp@-                       
    7284:  4227           	clrb %sp@-                            
    7286:  4e4f           	trap #15                              
    7288:  a2a1           	0121241                               ; sysTrapKeyHandleInterrupt
    728a:  2003           	movel %d3,%d0                         
    728c:  4480           	negl %d0                              
    728e:  2480           	movel %d0,%a2@                        
    7290:  7001           	moveq #1,%d0                          
    7292:  5c4f           	addqw #6,%sp                          

  label_53:
    7294:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    7298:  4e5e           	unlk %fp                              
    729a:  4e75           	rts                                   
```

# KeyRates: 0x72b2 - 0x7316

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???
   * `%fp@(14)`     : ???
   * `%fp@(18)`     : ???
   * `%fp@(22)`     : ???

```
    72b2:  4e56 0000      	linkw %fp,#0                          
    72b6:  2f0b           	movel %a3,%sp@-                       
    72b8:  2f05           	movel %d5,%sp@-                       
    72ba:  2a2e 000a      	movel %fp@(10),%d5                    
    72be:  242e 000e      	movel %fp@(14),%d2                    
    72c2:  222e 0012      	movel %fp@(18),%d1                    
    72c6:  266e 0016      	moveal %fp@(22),%a3                   
    72ca:  2278 0188      	moveal 0x188,%a1                      
    72ce:  4a2e 0008      	tstb %fp@(8)                          
    72d2:  671c           	beqs 0x72f0                           ; ↓ label_0
    72d4:  2045           	moveal %d5,%a0                        
    72d6:  1368 0001 0007 	moveb %a0@(1),%a1@(7)                 
    72dc:  2042           	moveal %d2,%a0                        
    72de:  1368 0001 0008 	moveb %a0@(1),%a1@(8)                 
    72e4:  2041           	moveal %d1,%a0                        
    72e6:  3350 0012      	movew %a0@,%a1@(18)                   
    72ea:  1353 0009      	moveb %a3@,%a1@(9)                    
    72ee:  601e           	bras 0x730e                           ; ↓ label_1

  label_0:
    72f0:  7000           	moveq #0,%d0                          
    72f2:  1029 0007      	moveb %a1@(7),%d0                     
    72f6:  2045           	moveal %d5,%a0                        
    72f8:  3080           	movew %d0,%a0@                        
    72fa:  7000           	moveq #0,%d0                          
    72fc:  1029 0008      	moveb %a1@(8),%d0                     
    7300:  2042           	moveal %d2,%a0                        
    7302:  3080           	movew %d0,%a0@                        
    7304:  2041           	moveal %d1,%a0                        
    7306:  30a9 0012      	movew %a1@(18),%a0@                   
    730a:  16a9 0009      	moveb %a1@(9),%a3@                    

  label_1:
    730e:  7000           	moveq #0,%d0                          
    7310:  2a1f           	movel %sp@+,%d5                       
    7312:  265f           	moveal %sp@+,%a3                      
    7314:  4e5e           	unlk %fp                              
    7316:  4e75           	rts                                   
```

# KeyResetDoubleTap: 0x7324 - 0x7334

```
    7324:  4e56 0000      	linkw %fp,#0                          
    7328:  2078 0188      	moveal 0x188,%a0                      
    732c:  42a8 000e      	clrl %a0@(14)                         
    7330:  7000           	moveq #0,%d0                          
    7332:  4e5e           	unlk %fp                              
    7334:  4e75           	rts                                   
```

# KeySleep: 0x734a - 0x741c

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(10)`     : ???

```
    734a:  4e56 0000      	linkw %fp,#0                          
    734e:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-          
    7352:  247c ffff f000 	moveal #-4096,%a2                     
    7358:  2678 0188      	moveal 0x188,%a3                      
    735c:  7a00           	moveq #0,%d5                          
    735e:  3c3c 01f4      	movew #500,%d6                        
    7362:  4a2e 0008      	tstb %fp@(8)                          
    7366:  6600 00aa      	bnew 0x7412                           ; ↓ label_9
    736a:  4a2e 000a      	tstb %fp@(10)                         
    736e:  6656           	bnes 0x73c6                           ; ↓ label_6
    7370:  0038 0007 f41d 	orib #7,0xfffffffffffff41d            
    7376:  0238 00f8 f41f 	andib #-8,0xfffffffffffff41f          
    737c:  0278 f7ff f302 	andiw #-2049,0xfffffffffffff302       

  label_0:
    7382:  7800           	moveq #0,%d4                          
    7384:  7600           	moveq #0,%d3                          

  label_1:
    7386:  4878 03e8      	pea 0x3e8                             
    738a:  4e4f           	trap #15                              
    738c:  a249           	0121111                               ; sysTrapHwrDelay
    738e:  102a 0419      	moveb %a2@(1049),%d0                  
    7392:  0240 0007      	andiw #7,%d0                          
    7396:  584f           	addqw #4,%sp                          
    7398:  6702           	beqs 0x739c                           ; ↓ label_2
    739a:  7801           	moveq #1,%d4                          

  label_2:
    739c:  302a 0310      	movew %a2@(784),%d0                   
    73a0:  0240 0001      	andiw #1,%d0                          
    73a4:  6702           	beqs 0x73a8                           ; ↓ label_3
    73a6:  7801           	moveq #1,%d4                          

  label_3:
    73a8:  5243           	addqw #1,%d3                          
    73aa:  0c43 000a      	cmpiw #10,%d3                         
    73ae:  65d6           	bcss 0x7386                           ; ↑ label_1
    73b0:  4a04           	tstb %d4                              
    73b2:  6704           	beqs 0x73b8                           ; ↓ label_4
    73b4:  7a00           	moveq #0,%d5                          
    73b6:  6002           	bras 0x73ba                           ; ↓ label_5

  label_4:
    73b8:  5245           	addqw #1,%d5                          

  label_5:
    73ba:  5346           	subqw #1,%d6                          
    73bc:  0c45 0005      	cmpiw #5,%d5                          
    73c0:  6404           	bccs 0x73c6                           ; ↓ label_6
    73c2:  4a46           	tstw %d6                              
    73c4:  6ebc           	bgts 0x7382                           ; ↑ label_0

  label_6:
    73c6:  0038 0007 f411 	orib #7,0xfffffffffffff411            
    73cc:  0238 00fa f411 	andib #-6,0xfffffffffffff411          
    73d2:  3038 028e      	movew 0x28e,%d0                       
    73d6:  0240 f000      	andiw #-4096,%d0                      
    73da:  0c40 b000      	cmpiw #-20480,%d0                     
    73de:  6610           	bnes 0x73f0                           ; ↓ label_7
    73e0:  7007           	moveq #7,%d0                          
    73e2:  c0b8 02cc      	andl 0x2cc,%d0                        
    73e6:  6708           	beqs 0x73f0                           ; ↓ label_7
    73e8:  11fc 0005 f41e 	moveb #5,0xfffffffffffff41e           
    73ee:  6006           	bras 0x73f6                           ; ↓ label_8

  label_7:
    73f0:  11fc 0007 f41e 	moveb #7,0xfffffffffffff41e           

  label_8:
    73f6:  0278 ffbf f306 	andiw #-65,0xfffffffffffff306         
    73fc:  1f3c 0001      	moveb #1,%sp@-                        
    7400:  4eba f894      	jsr %pc@(0x6c96)                      ; prvEnableKeyInterrupts
    7404:  0078 0800 f302 	oriw #2048,0xfffffffffffff302         
    740a:  0278 fffe f304 	andiw #-2,0xfffffffffffff304          
    7410:  544f           	addqw #2,%sp                          

  label_9:
    7412:  4293           	clrl %a3@                             
    7414:  7000           	moveq #0,%d0                          
    7416:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3          
    741a:  4e5e           	unlk %fp                              
    741c:  4e75           	rts                                   
```

# KeyWake: 0x742a - 0x7482

```
    742a:  4e56 0000      	linkw %fp,#0                          
    742e:  2f0a           	movel %a2,%sp@-                       
    7430:  2478 0188      	moveal 0x188,%a2                      
    7434:  4292           	clrl %a2@                             
    7436:  422a 0004      	clrb %a2@(4)                          
    743a:  422a 0006      	clrb %a2@(6)                          
    743e:  42aa 000e      	clrl %a2@(14)                         
    7442:  4eba f8a4      	jsr %pc@(0x6ce8)                      ; prvDisableKeyInterrupts
    7446:  4eba f8ce      	jsr %pc@(0x6d16)                      ; PrvScanKeyStatus
    744a:  2540 0018      	movel %d0,%a2@(24)                    
    744e:  4aaa 0018      	tstl %a2@(24)                         
    7452:  6704           	beqs 0x7458                           ; ↓ label_0
    7454:  4e4f           	trap #15                              
    7456:  a130           	0120460                               ; sysTrapEvtResetAutoOffTimer

  label_0:
    7458:  0238 00f8 f411 	andib #-8,0xfffffffffffff411          
    745e:  4238 f41e      	clrb 0xfffffffffffff41e               
    7462:  0078 0040 f306 	oriw #64,0xfffffffffffff306           
    7468:  0278 f8ff f306 	andiw #-1793,0xfffffffffffff306       
    746e:  4227           	clrb %sp@-                            
    7470:  4eba f824      	jsr %pc@(0x6c96)                      ; prvEnableKeyInterrupts
    7474:  0278 fffe f304 	andiw #-2,0xfffffffffffff304          
    747a:  7000           	moveq #0,%d0                          
    747c:  544f           	addqw #2,%sp                          
    747e:  245f           	moveal %sp@+,%a2                      
    7480:  4e5e           	unlk %fp                              
    7482:  4e75           	rts                                   
```

# KeyCheckWakeUp: 0x748e - 0x74d6

```
    748e:  4e56 0000      	linkw %fp,#0                          
    7492:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    7496:  2478 0304      	moveal 0x304,%a2                      
    749a:  7679           	moveq #121,%d3                        
    749c:  4eba f84a      	jsr %pc@(0x6ce8)                      ; prvDisableKeyInterrupts
    74a0:  4eba f874      	jsr %pc@(0x6d16)                      ; PrvScanKeyStatus
    74a4:  2800           	movel %d0,%d4                         
    74a6:  200a           	movel %a2,%d0                         
    74a8:  670e           	beqs 0x74b8                           ; ↓ label_0
    74aa:  7001           	moveq #1,%d0                          
    74ac:  c0aa 000c      	andl %a2@(12),%d0                     
    74b0:  6706           	beqs 0x74b8                           ; ↓ label_0
    74b2:  0083 0000 0800 	oril #2048,%d3                        

  label_0:
    74b8:  2004           	movel %d4,%d0                         
    74ba:  c083           	andl %d3,%d0                          
    74bc:  6704           	beqs 0x74c2                           ; ↓ label_1
    74be:  7600           	moveq #0,%d3                          
    74c0:  600c           	bras 0x74ce                           ; ↓ label_2

  label_1:
    74c2:  7601           	moveq #1,%d3                          
    74c4:  1f3c 0001      	moveb #1,%sp@-                        
    74c8:  4eba f7cc      	jsr %pc@(0x6c96)                      ; prvEnableKeyInterrupts
    74cc:  544f           	addqw #2,%sp                          

  label_2:
    74ce:  3003           	movew %d3,%d0                         
    74d0:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    74d4:  4e5e           	unlk %fp                              
    74d6:  4e75           	rts                                   
```

# TimInit: 0x74ea - 0x7592

Locals:

   * `%fp@(-26)`    : ???
   * `%fp@(-30)`    : ???

```
    74ea:  4e56 ffe2      	linkw %fp,#-30                        
    74ee:  2f0a           	movel %a2,%sp@-                       
    74f0:  2f03           	movel %d3,%sp@-                       
    74f2:  7600           	moveq #0,%d3                          
    74f4:  4878 0018      	pea 0x18                              
    74f8:  4e4f           	trap #15                              
    74fa:  a013           	0120023                               ; sysTrapMemPtrNew
    74fc:  2448           	moveal %a0,%a2                        
    74fe:  200a           	movel %a2,%d0                         
    7500:  584f           	addqw #4,%sp                          
    7502:  6700 0082      	beqw 0x7586                           ; ↓ label_3
    7506:  4227           	clrb %sp@-                            
    7508:  4878 0018      	pea 0x18                              
    750c:  2f0a           	movel %a2,%sp@-                       
    750e:  4e4f           	trap #15                              
    7510:  a027           	0120047                               ; sysTrapMemSet
    7512:  21ca 0178      	movel %a2,0x178                       
    7516:  4278 030e      	clrw 0x30e                            
    751a:  3038 028e      	movew 0x28e,%d0                       
    751e:  0240 0020      	andiw #32,%d0                         
    7522:  4fef 000a      	lea %sp@(10),%sp                      
    7526:  6606           	bnes 0x752e                           ; ↓ label_0
    7528:  157c 0001 0008 	moveb #1,%a2@(8)                      

  label_0:
    752e:  486e ffe2      	pea %fp@(-30)                         
    7532:  4227           	clrb %sp@-                            
    7534:  4e4f           	trap #15                              
    7536:  a03f           	0120077                               ; sysTrapMemNVParams
    7538:  4ab8 fb00      	tstl 0xfffffffffffffb00               
    753c:  5c4f           	addqw #6,%sp                          
    753e:  6606           	bnes 0x7546                           ; ↓ label_1
    7540:  21ee ffe6 fb00 	movel %fp@(-26),0xfffffffffffffb00    

  label_1:
    7546:  24ae ffe2      	movel %fp@(-30),%a2@                  
    754a:  31fc 0008 fb10 	movew #8,0xfffffffffffffb10           
    7550:  31fc 0080 fb0c 	movew #128,0xfffffffffffffb0c         
    7556:  4a2a 0008      	tstb %a2@(8)                          
    755a:  6722           	beqs 0x757e                           ; ↓ label_2
    755c:  422a 0016      	clrb %a2@(22)                         
    7560:  2f2e ffe2      	movel %fp@(-30),%sp@-                 
    7564:  2f2e ffe6      	movel %fp@(-26),%sp@-                 
    7568:  4eba 04b0      	jsr %pc@(0x7a1a)                      ; PrvHMSToSec
    756c:  2540 000e      	movel %d0,%a2@(14)                    
    7570:  256a 000e 0012 	movel %a2@(14),%a2@(18)               
    7576:  0078 0010 fb10 	oriw #16,0xfffffffffffffb10           
    757c:  504f           	addqw #8,%sp                          

  label_2:
    757e:  0278 ffef f306 	andiw #-17,0xfffffffffffff306         
    7584:  6004           	bras 0x758a                           ; ↓ label_4

  label_3:
    7586:  363c 0a01      	movew #2561,%d3                       

  label_4:
    758a:  3003           	movew %d3,%d0                         
    758c:  261f           	movel %sp@+,%d3                       
    758e:  245f           	moveal %sp@+,%a2                      
    7590:  4e5e           	unlk %fp                              
    7592:  4e75           	rts                                   
```

# TimSetAlarm: 0x759e - 0x75b8

Parameters:

   * `%fp@(8)`      : ???

```
    759e:  4e56 0000      	linkw %fp,#0                          
    75a2:  2f03           	movel %d3,%sp@-                       
    75a4:  4227           	clrb %sp@-                            
    75a6:  2f2e 0008      	movel %fp@(8),%sp@-                   
    75aa:  4eba 04c8      	jsr %pc@(0x7a74)                      ; PrvSetAlarmLow
    75ae:  2600           	movel %d0,%d3                         
    75b0:  2003           	movel %d3,%d0                         
    75b2:  5c4f           	addqw #6,%sp                          
    75b4:  261f           	movel %sp@+,%d3                       
    75b6:  4e5e           	unlk %fp                              
    75b8:  4e75           	rts                                   
```

# TimGetAlarm: 0x75c8 - 0x75ea

```
    75c8:  4e56 0000      	linkw %fp,#0                          
    75cc:  3238 f306      	movew 0xfffffffffffff306,%d1          
    75d0:  3001           	movew %d1,%d0                         
    75d2:  0040 0010      	oriw #16,%d0                          
    75d6:  31c0 f306      	movew %d0,0xfffffffffffff306          
    75da:  2078 0178      	moveal 0x178,%a0                      
    75de:  2428 0004      	movel %a0@(4),%d2                     
    75e2:  31c1 f306      	movew %d1,0xfffffffffffff306          
    75e6:  2002           	movel %d2,%d0                         
    75e8:  4e5e           	unlk %fp                              
    75ea:  4e75           	rts                                   
```

# TimGetSeconds: 0x75fa - 0x7652

```
    75fa:  4e56 0000      	linkw %fp,#0                          
    75fe:  48e7 1c20      	moveml %d3-%d5/%a2,%sp@-              
    7602:  2478 0178      	moveal 0x178,%a2                      
    7606:  4ab8 0178      	tstl 0x178                            
    760a:  6608           	bnes 0x7614                           ; ↓ label_0
    760c:  203c 8492 8680 	movel #-2070772096,%d0                
    7612:  6038           	bras 0x764c                           ; ↓ label_3

  label_0:
    7614:  3838 f306      	movew 0xfffffffffffff306,%d4          
    7618:  3004           	movew %d4,%d0                         
    761a:  0040 0010      	oriw #16,%d0                          
    761e:  31c0 f306      	movew %d0,0xfffffffffffff306          
    7622:  4eba 035a      	jsr %pc@(0x797e)                      ; PrvGetRTCSec
    7626:  2600           	movel %d0,%d3                         
    7628:  4a2a 0008      	tstb %a2@(8)                          
    762c:  6718           	beqs 0x7646                           ; ↓ label_2
    762e:  2a2a 000e      	movel %a2@(14),%d5                    
    7632:  b685           	cmpl %d5,%d3                          
    7634:  6404           	bccs 0x763a                           ; ↓ label_1
    7636:  2605           	movel %d5,%d3                         
    7638:  600c           	bras 0x7646                           ; ↓ label_2

  label_1:
    763a:  b685           	cmpl %d5,%d3                          
    763c:  6308           	blss 0x7646                           ; ↓ label_2
    763e:  422a 0016      	clrb %a2@(22)                         
    7642:  2543 000e      	movel %d3,%a2@(14)                    

  label_2:
    7646:  31c4 f306      	movew %d4,0xfffffffffffff306          
    764a:  2003           	movel %d3,%d0                         

  label_3:
    764c:  4cdf 0438      	moveml %sp@+,%d3-%d5/%a2              
    7650:  4e5e           	unlk %fp                              
    7652:  4e75           	rts                                   
```

# TimSetSeconds: 0x7664 - 0x7720

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-4)`     : ???

```
    7664:  4e56 fffc      	linkw %fp,#-4                         
    7668:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    766c:  262e 0008      	movel %fp@(8),%d3                     
    7670:  2478 0178      	moveal 0x178,%a2                      
    7674:  2c03           	movel %d3,%d6                         
    7676:  4e4f           	trap #15                              
    7678:  a0f5           	0120365                               ; sysTrapTimGetSeconds
    767a:  2203           	movel %d3,%d1                         
    767c:  9280           	subl %d0,%d1                          
    767e:  2d41 fffc      	movel %d1,%fp@(-4)                    
    7682:  2003           	movel %d3,%d0                         
    7684:  723c           	moveq #60,%d1                         
    7686:  4eba 8aa8      	jsr %pc@(0x130)                       ; BBHBf6
    768a:  2a00           	movel %d0,%d5                         
    768c:  2800           	movel %d0,%d4                         
    768e:  4a85           	tstl %d5                              
    7690:  6604           	bnes 0x7696                           ; ↓ label_0
    7692:  7801           	moveq #1,%d4                          
    7694:  5283           	addql #1,%d3                          

  label_0:
    7696:  2003           	movel %d3,%d0                         
    7698:  9084           	subl %d4,%d0                          
    769a:  723c           	moveq #60,%d1                         
    769c:  4eba 8a12      	jsr %pc@(0xb0)                        ; BBHBf6
    76a0:  2600           	movel %d0,%d3                         
    76a2:  2003           	movel %d3,%d0                         
    76a4:  723c           	moveq #60,%d1                         
    76a6:  4eba 8a88      	jsr %pc@(0x130)                       ; BBHBf6
    76aa:  2a00           	movel %d0,%d5                         
    76ac:  2003           	movel %d3,%d0                         
    76ae:  9085           	subl %d5,%d0                          
    76b0:  723c           	moveq #60,%d1                         
    76b2:  4eba 89fc      	jsr %pc@(0xb0)                        ; BBHBf6
    76b6:  2640           	moveal %d0,%a3                        
    76b8:  200b           	movel %a3,%d0                         
    76ba:  7218           	moveq #24,%d1                         
    76bc:  4eba 8a72      	jsr %pc@(0x130)                       ; BBHBf6
    76c0:  2600           	movel %d0,%d3                         
    76c2:  3e38 f306      	movew 0xfffffffffffff306,%d7          
    76c6:  3007           	movew %d7,%d0                         
    76c8:  0040 0010      	oriw #16,%d0                          
    76cc:  31c0 f306      	movew %d0,0xfffffffffffff306          
    76d0:  200b           	movel %a3,%d0                         
    76d2:  9083           	subl %d3,%d0                          
    76d4:  2480           	movel %d0,%a2@                        
    76d6:  2005           	movel %d5,%d0                         
    76d8:  4840           	swap %d0                              
    76da:  4240           	clrw %d0                              
    76dc:  2203           	movel %d3,%d1                         
    76de:  7418           	moveq #24,%d2                         
    76e0:  e5a9           	lsll %d2,%d1                          
    76e2:  8280           	orl %d0,%d1                           
    76e4:  8284           	orl %d4,%d1                           
    76e6:  21c1 fb00      	movel %d1,0xfffffffffffffb00          
    76ea:  4a2a 0008      	tstb %a2@(8)                          
    76ee:  670c           	beqs 0x76fc                           ; ↓ label_1
    76f0:  422a 0016      	clrb %a2@(22)                         
    76f4:  2546 000e      	movel %d6,%a2@(14)                    
    76f8:  2546 0012      	movel %d6,%a2@(18)                    

  label_1:
    76fc:  31c7 f306      	movew %d7,0xfffffffffffff306          
    7700:  4e4f           	trap #15                              
    7702:  a237           	0121067                               ; sysTrapAlmAlarmCallback
    7704:  2f2e fffc      	movel %fp@(-4),%sp@-                  
    7708:  4eba 043c      	jsr %pc@(0x7b46)                      ; PrvBroadcastTimeChange
    770c:  42a7           	clrl %sp@-                            
    770e:  3f3c 0004      	movew #4,%sp@-                        
    7712:  4e4f           	trap #15                              
    7714:  a089           	0120211                               ; sysTrapSysBroadcastActionCode
    7716:  4fef 000a      	lea %sp@(10),%sp                      
    771a:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    771e:  4e5e           	unlk %fp                              
    7720:  4e75           	rts                                   
```

# TimGetTicks: 0x7732 - 0x773a

```
    7732:  4e56 0000      	linkw %fp,#0                          
    7736:  7000           	moveq #0,%d0                          
    7738:  4e5e           	unlk %fp                              
    773a:  4e75           	rts                                   
```

# TimHandleInterrupt: 0x774a - 0x78aa

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-26)`    : ???
   * `%fp@(-30)`    : ???

```
    774a:  4e56 ffe2      	linkw %fp,#-30                        
    774e:  48e7 1830      	moveml %d3-%d4/%a2-%a3,%sp@-          
    7752:  267c ffff f000 	moveal #-4096,%a3                     
    7758:  2478 0178      	moveal 0x178,%a2                      
    775c:  3638 fb0e      	movew 0xfffffffffffffb0e,%d3          
    7760:  c678 fb10      	andw 0xfffffffffffffb10,%d3           
    7764:  4a2e 0008      	tstb %fp@(8)                          
    7768:  6736           	beqs 0x77a0                           ; ↓ label_0
    776a:  4227           	clrb %sp@-                            
    776c:  4e4f           	trap #15                              
    776e:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    7770:  486e ffe2      	pea %fp@(-30)                         
    7774:  4227           	clrb %sp@-                            
    7776:  4e4f           	trap #15                              
    7778:  a03f           	0120077                               ; sysTrapMemNVParams
    777a:  2d78 fb00 ffe6 	movel 0xfffffffffffffb00,%fp@(-26)    
    7780:  2d52 ffe2      	movel %a2@,%fp@(-30)                  
    7784:  486e ffe2      	pea %fp@(-30)                         
    7788:  1f3c 0001      	moveb #1,%sp@-                        
    778c:  4e4f           	trap #15                              
    778e:  a03f           	0120077                               ; sysTrapMemNVParams
    7790:  1f3c 0001      	moveb #1,%sp@-                        
    7794:  4e4f           	trap #15                              
    7796:  a0c3           	0120303                               ; sysTrapSysTaskSwitching
    7798:  4fef 0010      	lea %sp@(16),%sp                      
    779c:  6000 0106      	braw 0x78a4                           ; ↓ label_11

  label_0:
    77a0:  3003           	movew %d3,%d0                         
    77a2:  0240 0012      	andiw #18,%d0                         
    77a6:  6748           	beqs 0x77f0                           ; ↓ label_3
    77a8:  31fc 0012 fb0e 	movew #18,0xfffffffffffffb0e          
    77ae:  4a2a 0008      	tstb %a2@(8)                          
    77b2:  673c           	beqs 0x77f0                           ; ↓ label_3
    77b4:  3003           	movew %d3,%d0                         
    77b6:  0240 0002      	andiw #2,%d0                          
    77ba:  672c           	beqs 0x77e8                           ; ↓ label_2
    77bc:  4a2a 0016      	tstb %a2@(22)                         
    77c0:  6708           	beqs 0x77ca                           ; ↓ label_1
    77c2:  703c           	moveq #60,%d0                         
    77c4:  d1aa 000e      	addl %d0,%a2@(14)                     
    77c8:  6026           	bras 0x77f0                           ; ↓ label_3

  label_1:
    77ca:  202a 000e      	movel %a2@(14),%d0                    
    77ce:  723c           	moveq #60,%d1                         
    77d0:  4eba 88de      	jsr %pc@(0xb0)                        ; BBHBf6
    77d4:  5280           	addql #1,%d0                          
    77d6:  723c           	moveq #60,%d1                         
    77d8:  4eba 88a4      	jsr %pc@(0x7e)                        ; anonymous_function_0
    77dc:  2540 000e      	movel %d0,%a2@(14)                    
    77e0:  157c 0001 0016 	moveb #1,%a2@(22)                     
    77e6:  6008           	bras 0x77f0                           ; ↓ label_3

  label_2:
    77e8:  422a 0016      	clrb %a2@(22)                         
    77ec:  52aa 000e      	addql #1,%a2@(14)                     

  label_3:
    77f0:  3003           	movew %d3,%d0                         
    77f2:  0240 0008      	andiw #8,%d0                          
    77f6:  6752           	beqs 0x784a                           ; ↓ label_7
    77f8:  31fc 0008 fb0e 	movew #8,0xfffffffffffffb0e           
    77fe:  5278 030e      	addqw #1,0x30e                        
    7802:  4a2a 0008      	tstb %a2@(8)                          
    7806:  672e           	beqs 0x7836                           ; ↓ label_6
    7808:  3838 fb10      	movew 0xfffffffffffffb10,%d4          
    780c:  0078 0010 fb10 	oriw #16,0xfffffffffffffb10           
    7812:  31fc 0010 fb0e 	movew #16,0xfffffffffffffb0e          

  label_4:
    7818:  302b 0b0e      	movew %a3@(2830),%d0                  
    781c:  0240 0010      	andiw #16,%d0                         
    7820:  67f6           	beqs 0x7818                           ; ↑ label_4
    7822:  31fc 0010 fb0e 	movew #16,0xfffffffffffffb0e          

  label_5:
    7828:  302b 0b0e      	movew %a3@(2830),%d0                  
    782c:  0240 0010      	andiw #16,%d0                         
    7830:  67f6           	beqs 0x7828                           ; ↑ label_5
    7832:  31c4 fb10      	movew %d4,0xfffffffffffffb10          

  label_6:
    7836:  7018           	moveq #24,%d0                         
    7838:  d192           	addl %d0,%a2@                         
    783a:  282a 0004      	movel %a2@(4),%d4                     
    783e:  670a           	beqs 0x784a                           ; ↓ label_7
    7840:  4227           	clrb %sp@-                            
    7842:  2f04           	movel %d4,%sp@-                       
    7844:  4eba 022e      	jsr %pc@(0x7a74)                      ; PrvSetAlarmLow
    7848:  5c4f           	addqw #6,%sp                          

  label_7:
    784a:  4a2a 0008      	tstb %a2@(8)                          
    784e:  6728           	beqs 0x7878                           ; ↓ label_9
    7850:  0243 fffb      	andiw #-5,%d3                         
    7854:  3038 fb10      	movew 0xfffffffffffffb10,%d0          
    7858:  0240 0004      	andiw #4,%d0                          
    785c:  6714           	beqs 0x7872                           ; ↓ label_8
    785e:  282a 000a      	movel %a2@(10),%d4                    
    7862:  b8aa 0012      	cmpl %a2@(18),%d4                     
    7866:  650a           	bcss 0x7872                           ; ↓ label_8
    7868:  b8aa 000e      	cmpl %a2@(14),%d4                     
    786c:  6204           	bhis 0x7872                           ; ↓ label_8
    786e:  0043 0004      	oriw #4,%d3                           

  label_8:
    7872:  256a 000e 0012 	movel %a2@(14),%a2@(18)               

  label_9:
    7878:  3003           	movew %d3,%d0                         
    787a:  0240 0004      	andiw #4,%d0                          
    787e:  6724           	beqs 0x78a4                           ; ↓ label_11
    7880:  31fc 0004 fb0e 	movew #4,0xfffffffffffffb0e           
    7886:  4aaa 0004      	tstl %a2@(4)                          
    788a:  670a           	beqs 0x7896                           ; ↓ label_10
    788c:  4e4f           	trap #15                              
    788e:  a237           	0121067                               ; sysTrapAlmAlarmCallback
    7890:  0278 fff7 0250 	andiw #-9,0x250                       

  label_10:
    7896:  1f3c 0001      	moveb #1,%sp@-                        
    789a:  2f2a 0004      	movel %a2@(4),%sp@-                   
    789e:  4eba 01d4      	jsr %pc@(0x7a74)                      ; PrvSetAlarmLow
    78a2:  5c4f           	addqw #6,%sp                          

  label_11:
    78a4:  4cdf 0c18      	moveml %sp@+,%d3-%d4/%a2-%a3          
    78a8:  4e5e           	unlk %fp                              
    78aa:  4e75           	rts                                   
```

# TimSleep: 0x78c2 - 0x7910

Parameters:

   * `%fp@(10)`     : ???

```
    78c2:  4e56 0000      	linkw %fp,#0                          
    78c6:  2f0a           	movel %a2,%sp@-                       
    78c8:  2478 0178      	moveal 0x178,%a2                      
    78cc:  2078 0178      	moveal 0x178,%a0                      
    78d0:  4a28 0008      	tstb %a0@(8)                          
    78d4:  672a           	beqs 0x7900                           ; ↓ label_1
    78d6:  4a2e 000a      	tstb %fp@(10)                         
    78da:  6612           	bnes 0x78ee                           ; ↓ label_0
    78dc:  3038 fb0e      	movew 0xfffffffffffffb0e,%d0          
    78e0:  0240 0010      	andiw #16,%d0                         
    78e4:  6708           	beqs 0x78ee                           ; ↓ label_0
    78e6:  4227           	clrb %sp@-                            
    78e8:  4e4f           	trap #15                              
    78ea:  a0fb           	0120373                               ; sysTrapTimHandleInterrupt
    78ec:  544f           	addqw #2,%sp                          

  label_0:
    78ee:  0278 ffef fb10 	andiw #-17,0xfffffffffffffb10         
    78f4:  0078 0002 fb10 	oriw #2,0xfffffffffffffb10            
    78fa:  157c 0001 0009 	moveb #1,%a2@(9)                      

  label_1:
    7900:  4a2e 000a      	tstb %fp@(10)                         
    7904:  6704           	beqs 0x790a                           ; ↓ label_2
    7906:  4278 fb10      	clrw 0xfffffffffffffb10               

  label_2:
    790a:  7000           	moveq #0,%d0                          
    790c:  245f           	moveal %sp@+,%a2                      
    790e:  4e5e           	unlk %fp                              
    7910:  4e75           	rts                                   
```

# TimWake: 0x791e - 0x7972

```
    791e:  4e56 0000      	linkw %fp,#0                          
    7922:  2f0a           	movel %a2,%sp@-                       
    7924:  2478 0178      	moveal 0x178,%a2                      
    7928:  2078 0178      	moveal 0x178,%a0                      
    792c:  4a28 0008      	tstb %a0@(8)                          
    7930:  6722           	beqs 0x7954                           ; ↓ label_1
    7932:  3038 fb0e      	movew 0xfffffffffffffb0e,%d0          
    7936:  0240 0002      	andiw #2,%d0                          
    793a:  6708           	beqs 0x7944                           ; ↓ label_0
    793c:  4227           	clrb %sp@-                            
    793e:  4e4f           	trap #15                              
    7940:  a0fb           	0120373                               ; sysTrapTimHandleInterrupt
    7942:  544f           	addqw #2,%sp                          

  label_0:
    7944:  0278 fffd fb10 	andiw #-3,0xfffffffffffffb10          
    794a:  0078 0010 fb10 	oriw #16,0xfffffffffffffb10           
    7950:  422a 0009      	clrb %a2@(9)                          

  label_1:
    7954:  0078 0008 fb10 	oriw #8,0xfffffffffffffb10            
    795a:  0278 ffef f306 	andiw #-17,0xfffffffffffff306         
    7960:  4aaa 0004      	tstl %a2@(4)                          
    7964:  6706           	beqs 0x796c                           ; ↓ label_2
    7966:  0078 0004 fb10 	oriw #4,0xfffffffffffffb10            

  label_2:
    796c:  7000           	moveq #0,%d0                          
    796e:  245f           	moveal %sp@+,%a2                      
    7970:  4e5e           	unlk %fp                              
    7972:  4e75           	rts                                   
```

# PrvGetRTCSec: 0x797e - 0x7a08

```
    797e:  4e56 0000      	linkw %fp,#0                          
    7982:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    7986:  2678 0178      	moveal 0x178,%a3                      
    798a:  247c ffff f000 	moveal #-4096,%a2                     
    7990:  7a00           	moveq #0,%d5                          
    7992:  4e4f           	trap #15                              
    7994:  a0a5           	0120245                               ; sysTrapSysDisableInts
    7996:  3e00           	movew %d0,%d7                         
    7998:  2c13           	movel %a3@,%d6                        

  label_0:
    799a:  262a 0b00      	movel %a2@(2816),%d3                  
    799e:  282a 0b00      	movel %a2@(2816),%d4                  
    79a2:  b684           	cmpl %d4,%d3                          
    79a4:  66f4           	bnes 0x799a                           ; ↑ label_0
    79a6:  4a05           	tstb %d5                              
    79a8:  6646           	bnes 0x79f0                           ; ↓ label_3
    79aa:  302a 0b0e      	movew %a2@(2830),%d0                  
    79ae:  0240 0008      	andiw #8,%d0                          
    79b2:  673c           	beqs 0x79f0                           ; ↓ label_3
    79b4:  7018           	moveq #24,%d0                         
    79b6:  dc80           	addl %d0,%d6                          
    79b8:  7a01           	moveq #1,%d5                          
    79ba:  4a2b 0008      	tstb %a3@(8)                          
    79be:  67da           	beqs 0x799a                           ; ↑ label_0
    79c0:  362a 0b10      	movew %a2@(2832),%d3                  
    79c4:  006a 0010 0b10 	oriw #16,%a2@(2832)                   
    79ca:  357c 0010 0b0e 	movew #16,%a2@(2830)                  

  label_1:
    79d0:  302a 0b0e      	movew %a2@(2830),%d0                  
    79d4:  0240 0010      	andiw #16,%d0                         
    79d8:  67f6           	beqs 0x79d0                           ; ↑ label_1
    79da:  357c 0010 0b0e 	movew #16,%a2@(2830)                  

  label_2:
    79e0:  302a 0b0e      	movew %a2@(2830),%d0                  
    79e4:  0240 0010      	andiw #16,%d0                         
    79e8:  67f6           	beqs 0x79e0                           ; ↑ label_2
    79ea:  3543 0b10      	movew %d3,%a2@(2832)                  
    79ee:  60aa           	bras 0x799a                           ; ↑ label_0

  label_3:
    79f0:  3f07           	movew %d7,%sp@-                       
    79f2:  4e4f           	trap #15                              
    79f4:  a0a6           	0120246                               ; sysTrapSysRestoreStatus
    79f6:  2f06           	movel %d6,%sp@-                       
    79f8:  2f03           	movel %d3,%sp@-                       
    79fa:  4eba 001e      	jsr %pc@(0x7a1a)                      ; PrvHMSToSec
    79fe:  4fef 000a      	lea %sp@(10),%sp                      
    7a02:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    7a06:  4e5e           	unlk %fp                              
    7a08:  4e75           	rts                                   
```

# PrvHMSToSec: 0x7a1a - 0x7a64

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    7a1a:  4e56 0000      	linkw %fp,#0                          
    7a1e:  2f04           	movel %d4,%sp@-                       
    7a20:  2f03           	movel %d3,%sp@-                       
    7a22:  282e 0008      	movel %fp@(8),%d4                     
    7a26:  2004           	movel %d4,%d0                         
    7a28:  0280 1f00 0000 	andil #520093696,%d0                  
    7a2e:  7218           	moveq #24,%d1                         
    7a30:  e2a8           	lsrl %d1,%d0                          
    7a32:  d0ae 000c      	addl %fp@(12),%d0                     
    7a36:  223c 0000 0e10 	movel #3600,%d1                       
    7a3c:  4eba 8640      	jsr %pc@(0x7e)                        ; anonymous_function_0
    7a40:  2600           	movel %d0,%d3                         
    7a42:  2004           	movel %d4,%d0                         
    7a44:  0280 003f 0000 	andil #4128768,%d0                    
    7a4a:  4240           	clrw %d0                              
    7a4c:  4840           	swap %d0                              
    7a4e:  723c           	moveq #60,%d1                         
    7a50:  4eba 862c      	jsr %pc@(0x7e)                        ; anonymous_function_0
    7a54:  d680           	addl %d0,%d3                          
    7a56:  703f           	moveq #63,%d0                         
    7a58:  c084           	andl %d4,%d0                          
    7a5a:  d680           	addl %d0,%d3                          
    7a5c:  2003           	movel %d3,%d0                         
    7a5e:  261f           	movel %sp@+,%d3                       
    7a60:  281f           	movel %sp@+,%d4                       
    7a62:  4e5e           	unlk %fp                              
    7a64:  4e75           	rts                                   
```

# PrvSetAlarmLow: 0x7a74 - 0x7b32

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-10)`    : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-14)`    : ???

```
    7a74:  4e56 fff2      	linkw %fp,#-14                        
    7a78:  48e7 1f20      	moveml %d3-%d7/%a2,%sp@-              
    7a7c:  262e 0008      	movel %fp@(8),%d3                     
    7a80:  1e2e 000c      	moveb %fp@(12),%d7                    
    7a84:  2478 0178      	moveal 0x178,%a2                      
    7a88:  3a38 f306      	movew 0xfffffffffffff306,%d5          
    7a8c:  3005           	movew %d5,%d0                         
    7a8e:  0040 0010      	oriw #16,%d0                          
    7a92:  31c0 f306      	movew %d0,0xfffffffffffff306          
    7a96:  2c2a 0004      	movel %a2@(4),%d6                     
    7a9a:  0278 fffb fb10 	andiw #-5,0xfffffffffffffb10          
    7aa0:  31fc 0004 fb0e 	movew #4,0xfffffffffffffb0e           
    7aa6:  2543 0004      	movel %d3,%a2@(4)                     
    7aaa:  4a83           	tstl %d3                              
    7aac:  6778           	beqs 0x7b26                           ; ↓ label_4
    7aae:  4e4f           	trap #15                              
    7ab0:  a0f5           	0120365                               ; sysTrapTimGetSeconds
    7ab2:  2800           	movel %d0,%d4                         
    7ab4:  2004           	movel %d4,%d0                         
    7ab6:  5480           	addql #2,%d0                          
    7ab8:  b680           	cmpl %d0,%d3                          
    7aba:  6410           	bccs 0x7acc                           ; ↓ label_0
    7abc:  4a07           	tstb %d7                              
    7abe:  660c           	bnes 0x7acc                           ; ↓ label_0
    7ac0:  4e4f           	trap #15                              
    7ac2:  a237           	0121067                               ; sysTrapAlmAlarmCallback
    7ac4:  7e01           	moveq #1,%d7                          
    7ac6:  0278 fff7 0250 	andiw #-9,0x250                       

  label_0:
    7acc:  4a07           	tstb %d7                              
    7ace:  6706           	beqs 0x7ad6                           ; ↓ label_1
    7ad0:  760a           	moveq #10,%d3                         
    7ad2:  d684           	addl %d4,%d3                          
    7ad4:  600e           	bras 0x7ae4                           ; ↓ label_2

  label_1:
    7ad6:  2003           	movel %d3,%d0                         
    7ad8:  9084           	subl %d4,%d0                          
    7ada:  0c80 0001 5180 	cmpil #86400,%d0                      
    7ae0:  6502           	bcss 0x7ae4                           ; ↓ label_2
    7ae2:  7600           	moveq #0,%d3                          

  label_2:
    7ae4:  4a83           	tstl %d3                              
    7ae6:  673e           	beqs 0x7b26                           ; ↓ label_4
    7ae8:  486e fff2      	pea %fp@(-14)                         
    7aec:  2f03           	movel %d3,%sp@-                       
    7aee:  4e4f           	trap #15                              
    7af0:  a0fc           	0120374                               ; sysTrapTimSecondsToDateTime
    7af2:  306e fff4      	moveaw %fp@(-12),%a0                  
    7af6:  2008           	movel %a0,%d0                         
    7af8:  4840           	swap %d0                              
    7afa:  4240           	clrw %d0                              
    7afc:  306e fff2      	moveaw %fp@(-14),%a0                  
    7b00:  2208           	movel %a0,%d1                         
    7b02:  8280           	orl %d0,%d1                           
    7b04:  306e fff6      	moveaw %fp@(-10),%a0                  
    7b08:  2008           	movel %a0,%d0                         
    7b0a:  7418           	moveq #24,%d2                         
    7b0c:  e5a8           	lsll %d2,%d0                          
    7b0e:  8081           	orl %d1,%d0                           
    7b10:  21c0 fb04      	movel %d0,0xfffffffffffffb04          
    7b14:  4a2a 0008      	tstb %a2@(8)                          
    7b18:  504f           	addqw #8,%sp                          
    7b1a:  6704           	beqs 0x7b20                           ; ↓ label_3
    7b1c:  2543 000a      	movel %d3,%a2@(10)                    

  label_3:
    7b20:  0078 0004 fb10 	oriw #4,0xfffffffffffffb10            

  label_4:
    7b26:  31c5 f306      	movew %d5,0xfffffffffffff306          
    7b2a:  2006           	movel %d6,%d0                         
    7b2c:  4cdf 04f8      	moveml %sp@+,%d3-%d7/%a2              
    7b30:  4e5e           	unlk %fp                              
    7b32:  4e75           	rts                                   
```

# PrvBroadcastTimeChange: 0x7b46 - 0x7b70

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-14)`    : ???
   * `%fp@(-18)`    : ???

```
    7b46:  4e56 ffee      	linkw %fp,#-18                        
    7b4a:  2d7c 7469 6d65 	movel #1953066341,%fp@(-18)           
    7b50:  ffee                                                  
    7b52:  2d7c 7073 7973 	movel #1886615923,%fp@(-14)           
    7b58:  fff2                                                  
    7b5a:  422e fffe      	clrb %fp@(-2)                         
    7b5e:  41ee 0008      	lea %fp@(8),%a0                       
    7b62:  2d48 fff6      	movel %a0,%fp@(-10)                   
    7b66:  486e ffee      	pea %fp@(-18)                         
    7b6a:  4e4f           	trap #15                              
    7b6c:  a355           	0121525                               ; sysTrapSysNotifyBroadcast
    7b6e:  4e5e           	unlk %fp                              
    7b70:  4e75           	rts                                   
```

# HwrSoundOn: 0x7c12 - 0x7db4

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-8)`     : ???
   * `%fp@(-10)`    : ???
   * `%fp@(-12)`    : ???
   * `%fp@(-16)`    : ???
   * `%fp@(-18)`    : ???

```
    7c12:  4e56 ffee      	linkw %fp,#-18                        
    7c16:  48e7 1f30      	moveml %d3-%d7/%a2-%a3,%sp@-          
    7c1a:  262e 0008      	movel %fp@(8),%d3                     
    7c1e:  2678 0174      	moveal 0x174,%a3                      
    7c22:  4a6e 000c      	tstw %fp@(12)                         
    7c26:  6704           	beqs 0x7c2c                           ; ↓ label_0
    7c28:  4a83           	tstl %d3                              
    7c2a:  660a           	bnes 0x7c36                           ; ↓ label_1

  label_0:
    7c2c:  4e4f           	trap #15                              
    7c2e:  a3c3           	0121703                               ; sysTrapHwrSoundOff
    7c30:  7000           	moveq #0,%d0                          
    7c32:  6000 017a      	braw 0x7dae                           ; ↓ label_13

  label_1:
    7c36:  45fa ffa2      	lea %pc@(0x7bda),%a2                  ; ??? 0x7bda
    7c3a:  41fa ff50      	lea %pc@(0x7b8c),%a0                  ; ??? 0x7b8c
    7c3e:  2d48 fffa      	movel %a0,%fp@(-6)                    
    7c42:  0c83 0000 7fff 	cmpil #32767,%d3                      
    7c48:  6306           	blss 0x7c50                           ; ↓ label_2
    7c4a:  263c 0000 7fff 	movel #32767,%d3                      

  label_2:
    7c50:  2038 012e      	movel 0x12e,%d0                       
    7c54:  7213           	moveq #19,%d1                         
    7c56:  e2a8           	lsrl %d1,%d0                          
    7c58:  2800           	movel %d0,%d4                         
    7c5a:  b680           	cmpl %d0,%d3                          
    7c5c:  6204           	bhis 0x7c62                           ; ↓ label_3
    7c5e:  2604           	movel %d4,%d3                         
    7c60:  5283           	addql #1,%d3                          

  label_3:
    7c62:  2038 012e      	movel 0x12e,%d0                       
    7c66:  7212           	moveq #18,%d1                         
    7c68:  e2a8           	lsrl %d1,%d0                          
    7c6a:  b680           	cmpl %d0,%d3                          
    7c6c:  6204           	bhis 0x7c72                           ; ↓ label_4
    7c6e:  7804           	moveq #4,%d4                          
    7c70:  6022           	bras 0x7c94                           ; ↓ label_7

  label_4:
    7c72:  2038 012e      	movel 0x12e,%d0                       
    7c76:  7211           	moveq #17,%d1                         
    7c78:  e2a8           	lsrl %d1,%d0                          
    7c7a:  b680           	cmpl %d0,%d3                          
    7c7c:  6204           	bhis 0x7c82                           ; ↓ label_5
    7c7e:  7803           	moveq #3,%d4                          
    7c80:  6012           	bras 0x7c94                           ; ↓ label_7

  label_5:
    7c82:  2038 012e      	movel 0x12e,%d0                       
    7c86:  4240           	clrw %d0                              
    7c88:  4840           	swap %d0                              
    7c8a:  b680           	cmpl %d0,%d3                          
    7c8c:  6204           	bhis 0x7c92                           ; ↓ label_6
    7c8e:  7802           	moveq #2,%d4                          
    7c90:  6002           	bras 0x7c94                           ; ↓ label_7

  label_6:
    7c92:  7801           	moveq #1,%d4                          

  label_7:
    7c94:  3004           	movew %d4,%d0                         
    7c96:  5040           	addqw #8,%d0                          
    7c98:  2238 012e      	movel 0x12e,%d1                       
    7c9c:  e0a9           	lsrl %d0,%d1                          
    7c9e:  2001           	movel %d1,%d0                         
    7ca0:  2203           	movel %d3,%d1                         
    7ca2:  4eba 840c      	jsr %pc@(0xb0)                        ; BBHBf6
    7ca6:  5280           	addql #1,%d0                          
    7ca8:  2a00           	movel %d0,%d5                         
    7caa:  3d40 fff8      	movew %d0,%fp@(-8)                    
    7cae:  3004           	movew %d4,%d0                         
    7cb0:  0640 ffff      	addiw #-1,%d0                         
    7cb4:  7200           	moveq #0,%d1                          
    7cb6:  3205           	movew %d5,%d1                         
    7cb8:  3d40 ffee      	movew %d0,%fp@(-18)                   
    7cbc:  2003           	movel %d3,%d0                         
    7cbe:  4eba 83be      	jsr %pc@(0x7e)                        ; anonymous_function_0
    7cc2:  2200           	movel %d0,%d1                         
    7cc4:  2038 012e      	movel 0x12e,%d0                       
    7cc8:  4eba 83e6      	jsr %pc@(0xb0)                        ; BBHBf6
    7ccc:  322e ffee      	movew %fp@(-18),%d1                   
    7cd0:  e2a8           	lsrl %d1,%d0                          
    7cd2:  5280           	addql #1,%d0                          
    7cd4:  e288           	lsrl #1,%d0                           
    7cd6:  3c00           	movew %d0,%d6                         
    7cd8:  2e03           	movel %d3,%d7                         
    7cda:  e08f           	lsrl #8,%d7                           
    7cdc:  2d47 fff4      	movel %d7,%fp@(-12)                   
    7ce0:  7000           	moveq #0,%d0                          
    7ce2:  102b 0006      	moveb %a3@(6),%d0                     
    7ce6:  c0ee 000c      	muluw %fp@(12),%d0                    
    7cea:  ec48           	lsrw #6,%d0                           
    7cec:  7200           	moveq #0,%d1                          
    7cee:  3200           	movew %d0,%d1                         
    7cf0:  206e fffa      	moveal %fp@(-6),%a0                   
    7cf4:  7000           	moveq #0,%d0                          
    7cf6:  1030 1800      	moveb %a0@(00000000,%d1:l),%d0        
    7cfa:  48c0           	extl %d0                              
    7cfc:  2d40 fff0      	movel %d0,%fp@(-16)                   
    7d00:  0c6e 0027 fff6 	cmpiw #39,%fp@(-10)                   
    7d06:  6508           	bcss 0x7d10                           ; ↓ label_8
    7d08:  7000           	moveq #0,%d0                          
    7d0a:  102a 0027      	moveb %a2@(39),%d0                    
    7d0e:  6030           	bras 0x7d40                           ; ↓ label_9

  label_8:
    7d10:  7000           	moveq #0,%d0                          
    7d12:  3007           	movew %d7,%d0                         
    7d14:  7200           	moveq #0,%d1                          
    7d16:  1232 0800      	moveb %a2@(00000000,%d0:l),%d1        
    7d1a:  3d41 fffe      	movew %d1,%fp@(-2)                    
    7d1e:  5240           	addqw #1,%d0                          
    7d20:  7200           	moveq #0,%d1                          
    7d22:  3200           	movew %d0,%d1                         
    7d24:  7000           	moveq #0,%d0                          
    7d26:  1032 1800      	moveb %a2@(00000000,%d1:l),%d0        
    7d2a:  906e fffe      	subw %fp@(-2),%d0                     
    7d2e:  2203           	movel %d3,%d1                         
    7d30:  0281 0000 00ff 	andil #255,%d1                        
    7d36:  c2c0           	muluw %d0,%d1                         
    7d38:  e049           	lsrw #8,%d1                           
    7d3a:  d26e fffe      	addw %fp@(-2),%d1                     
    7d3e:  3001           	movew %d1,%d0                         

  label_9:
    7d40:  c0c6           	muluw %d6,%d0                         
    7d42:  222e fff0      	movel %fp@(-16),%d1                   
    7d46:  4eba 8336      	jsr %pc@(0x7e)                        ; anonymous_function_0
    7d4a:  720c           	moveq #12,%d1                         
    7d4c:  e2a8           	lsrl %d1,%d0                          
    7d4e:  3a00           	movew %d0,%d5                         
    7d50:  3006           	movew %d6,%d0                         
    7d52:  e248           	lsrw #1,%d0                           
    7d54:  3600           	movew %d0,%d3                         
    7d56:  ba40           	cmpw %d0,%d5                          
    7d58:  6302           	blss 0x7d5c                           ; ↓ label_10
    7d5a:  3a03           	movew %d3,%d5                         

  label_10:
    7d5c:  0c45 0002      	cmpiw #2,%d5                          
    7d60:  6402           	bccs 0x7d64                           ; ↓ label_11
    7d62:  7a02           	moveq #2,%d5                          

  label_11:
    7d64:  3038 f500      	movew 0xfffffffffffff500,%d0          
    7d68:  0240 0010      	andiw #16,%d0                         
    7d6c:  6610           	bnes 0x7d7e                           ; ↓ label_12
    7d6e:  0038 0080 f439 	orib #-128,0xfffffffffffff439         
    7d74:  4878 0001      	pea 0x1                               
    7d78:  4e4f           	trap #15                              
    7d7a:  a0a0           	0120240                               ; sysTrapSysTaskDelay
    7d7c:  584f           	addqw #4,%sp                          

  label_12:
    7d7e:  0078 0080 f306 	oriw #128,0xfffffffffffff306          
    7d84:  3004           	movew %d4,%d0                         
    7d86:  0640 ffff      	addiw #-1,%d0                         
    7d8a:  322e fff8      	movew %fp@(-8),%d1                    
    7d8e:  0641 ffff      	addiw #-1,%d1                         
    7d92:  e149           	lslw #8,%d1                           
    7d94:  0041 0010      	oriw #16,%d1                          
    7d98:  8240           	orw %d0,%d1                           
    7d9a:  31c1 f500      	movew %d1,0xfffffffffffff500          
    7d9e:  1006           	moveb %d6,%d0                         
    7da0:  0600 fffe      	addib #-2,%d0                         
    7da4:  11c0 f504      	moveb %d0,0xfffffffffffff504          
    7da8:  11c5 f503      	moveb %d5,0xfffffffffffff503          
    7dac:  7001           	moveq #1,%d0                          

  label_13:
    7dae:  4cdf 0cf8      	moveml %sp@+,%d3-%d7/%a2-%a3          
    7db2:  4e5e           	unlk %fp                              
    7db4:  4e75           	rts                                   
```

# HwrSoundOff: 0x7dc4 - 0x7de6

```
    7dc4:  4e56 0000      	linkw %fp,#0                          
    7dc8:  0278 ffef f500 	andiw #-17,0xfffffffffffff500         
    7dce:  42b8 0260      	clrl 0x260                            
    7dd2:  7040           	moveq #64,%d0                         
    7dd4:  2078 0304      	moveal 0x304,%a0                      
    7dd8:  c0a8 000c      	andl %a0@(12),%d0                     
    7ddc:  6706           	beqs 0x7de4                           ; ↓ label_0
    7dde:  0238 007f f439 	andib #127,0xfffffffffffff439         

  label_0:
    7de4:  4e5e           	unlk %fp                              
    7de6:  4e75           	rts                                   
```

# FlashInit: 0x7df6 - 0x7dfe

```
    7df6:  4e56 0000      	linkw %fp,#0                          
    7dfa:  7000           	moveq #0,%d0                          
    7dfc:  4e5e           	unlk %fp                              
    7dfe:  4e75           	rts                                   
```

# FlashCompress: 0x7e0c - 0x7e14

```
    7e0c:  4e56 0000      	linkw %fp,#0                          
    7e10:  7000           	moveq #0,%d0                          
    7e12:  4e5e           	unlk %fp                              
    7e14:  4e75           	rts                                   
```

# FlashErase: 0x7e26 - 0x7e2e

```
    7e26:  4e56 0000      	linkw %fp,#0                          
    7e2a:  7000           	moveq #0,%d0                          
    7e2c:  4e5e           	unlk %fp                              
    7e2e:  4e75           	rts                                   
```

# FlashProgram: 0x7e3e - 0x7e46

```
    7e3e:  4e56 0000      	linkw %fp,#0                          
    7e42:  7000           	moveq #0,%d0                          
    7e44:  4e5e           	unlk %fp                              
    7e46:  4e75           	rts                                   
```

# FlashParmsWrite: 0x7e58 - 0x7e60

```
    7e58:  4e56 0000      	linkw %fp,#0                          
    7e5c:  7000           	moveq #0,%d0                          
    7e5e:  4e5e           	unlk %fp                              
    7e60:  4e75           	rts                                   
```

# FlashParmsRead: 0x7e74 - 0x7e7c

```
    7e74:  4e56 0000      	linkw %fp,#0                          
    7e78:  7000           	moveq #0,%d0                          
    7e7a:  4e5e           	unlk %fp                              
    7e7c:  4e75           	rts                                   
```

# HwrGetROMToken: 0x7f7c - 0x7fca

Parameters:

   * `%fp@(14)`     : ???
   * `%fp@(18)`     : ???

```
    7f7c:  4e56 0000      	linkw %fp,#0                          
    7f80:  2f00           	movel %d0,%sp@-                       
    7f82:  2f00           	movel %d0,%sp@-                       
    7f84:  4eba 0184      	jsr %pc@(0x810a)                      ; CheckIDExec
    7f88:  504f           	addqw #8,%sp                          
    7f8a:  4e5e           	unlk %fp                              
    7f8c:  4a80           	tstl %d0                              
    7f8e:  672e           	beqs 0x7fbe                           ; ↓ label_2
    7f90:  2040           	moveal %d0,%a0                        
    7f92:  202e 0012      	movel %fp@(18),%d0                    
    7f96:  6706           	beqs 0x7f9e                           ; ↓ label_0
    7f98:  2240           	moveal %d0,%a1                        
    7f9a:  32a8 0004      	movew %a0@(4),%a1@                    

  label_0:
    7f9e:  202e 000e      	movel %fp@(14),%d0                    
    7fa2:  6706           	beqs 0x7faa                           ; ↓ label_1
    7fa4:  2240           	moveal %d0,%a1                        
    7fa6:  5c88           	addql #6,%a0                          
    7fa8:  2288           	movel %a0,%a1@                        

  label_1:
    7faa:  4280           	clrl %d0                              
    7fac:  6016           	bras 0x7fc4                           ; ↓ label_3
    7fae:  203c ffff fffd 	movel #-3,%d0                         
    7fb4:  600e           	bras 0x7fc4                           ; ↓ label_3
    7fb6:  203c ffff fffe 	movel #-2,%d0                         
    7fbc:  6006           	bras 0x7fc4                           ; ↓ label_3

  label_2:
    7fbe:  203c ffff ffff 	movel #-1,%d0                         

  label_3:
    7fc4:  4cdf 1400      	moveml %sp@+,%a2/%a4                  
    7fc8:  4e5e           	unlk %fp                              
    7fca:  4e75           	rts                                   
```

# CheckSonyId: 0x7fde - 0x80fa

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???
   * `%fp@(20)`     : ???
   * `%fp@(24)`     : ???

Locals:

   * `%fp@(-8)`     : ???

```
    7fde:  4e56 fff8      	linkw %fp,#-8                         
    7fe2:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-          
    7fe6:  7800           	moveq #0,%d4                          
    7fe8:  246e 0008      	moveal %fp@(8),%a2                    
    7fec:  5c8a           	addql #6,%a2                          
    7fee:  42ae fff8      	clrl %fp@(-8)                         
    7ff2:  41ee fff8      	lea %fp@(-8),%a0                      
    7ff6:  5888           	addql #4,%a0                          
    7ff8:  2648           	moveal %a0,%a3                        
    7ffa:  4290           	clrl %a0@                             
    7ffc:  0c12 0048      	cmpib #72,%a2@                        
    8000:  6706           	beqs 0x8008                           ; ↓ label_0
    8002:  7000           	moveq #0,%d0                          
    8004:  6000 00ee      	braw 0x80f4                           ; ↓ label_6

  label_0:
    8008:  7a00           	moveq #0,%d5                          

  label_1:
    800a:  202e fff8      	movel %fp@(-8),%d0                    
    800e:  eb88           	lsll #5,%d0                           
    8010:  2d40 fff8      	movel %d0,%fp@(-8)                    
    8014:  1f12           	moveb %a2@,%sp@-                      
    8016:  4eba 0180      	jsr %pc@(0x8198)                      ; SearchFlashCharCode
    801a:  1600           	moveb %d0,%d3                         
    801c:  7000           	moveq #0,%d0                          
    801e:  1003           	moveb %d3,%d0                         
    8020:  d1ae fff8      	addl %d0,%fp@(-8)                     
    8024:  d803           	addb %d3,%d4                          
    8026:  528a           	addql #1,%a2                          
    8028:  5245           	addqw #1,%d5                          
    802a:  0c45 0006      	cmpiw #6,%d5                          
    802e:  544f           	addqw #2,%sp                          
    8030:  65d8           	bcss 0x800a                           ; ↑ label_1
    8032:  7a00           	moveq #0,%d5                          

  label_2:
    8034:  2013           	movel %a3@,%d0                        
    8036:  eb88           	lsll #5,%d0                           
    8038:  2680           	movel %d0,%a3@                        
    803a:  1f12           	moveb %a2@,%sp@-                      
    803c:  4eba 015a      	jsr %pc@(0x8198)                      ; SearchFlashCharCode
    8040:  1600           	moveb %d0,%d3                         
    8042:  7000           	moveq #0,%d0                          
    8044:  1003           	moveb %d3,%d0                         
    8046:  d193           	addl %d0,%a3@                         
    8048:  d803           	addb %d3,%d4                          
    804a:  528a           	addql #1,%a2                          
    804c:  5245           	addqw #1,%d5                          
    804e:  0c45 0006      	cmpiw #6,%d5                          
    8052:  544f           	addqw #2,%sp                          
    8054:  65de           	bcss 0x8034                           ; ↑ label_2
    8056:  7c00           	moveq #0,%d6                          
    8058:  1c03           	moveb %d3,%d6                         
    805a:  7000           	moveq #0,%d0                          
    805c:  1004           	moveb %d4,%d0                         
    805e:  9046           	subw %d6,%d0                          
    8060:  9046           	subw %d6,%d0                          
    8062:  6606           	bnes 0x806a                           ; ↓ label_3
    8064:  7000           	moveq #0,%d0                          
    8066:  6000 008c      	braw 0x80f4                           ; ↓ label_6

  label_3:
    806a:  1f3c 0003      	moveb #3,%sp@-                        
    806e:  486e fff8      	pea %fp@(-8)                          
    8072:  4eba 01c8      	jsr %pc@(0x823c)                      ; IdRightRotation
    8076:  1f3c 0005      	moveb #5,%sp@-                        
    807a:  1f3c 0003      	moveb #3,%sp@-                        
    807e:  486e fff8      	pea %fp@(-8)                          
    8082:  4eba 0444      	jsr %pc@(0x84c8)                      ; IdByteChange
    8086:  1f3c 0003      	moveb #3,%sp@-                        
    808a:  486e fff8      	pea %fp@(-8)                          
    808e:  4eba 01ac      	jsr %pc@(0x823c)                      ; IdRightRotation
    8092:  2f2e 000c      	movel %fp@(12),%sp@-                  
    8096:  2f2e 0010      	movel %fp@(16),%sp@-                  
    809a:  486e fff8      	pea %fp@(-8)                          
    809e:  4eba 017a      	jsr %pc@(0x821a)                      ; IdXOR
    80a2:  2f3c a5a5 5a5a 	movel #-1515890086,%sp@-              
    80a8:  2f3c aa55 55aa 	movel #-1437248086,%sp@-              
    80ae:  486e fff8      	pea %fp@(-8)                          
    80b2:  4eba 03f2      	jsr %pc@(0x84a6)                      ; IdAdd
    80b6:  1f3c 0007      	moveb #7,%sp@-                        
    80ba:  1f3c 0004      	moveb #4,%sp@-                        
    80be:  486e fff8      	pea %fp@(-8)                          
    80c2:  4eba 0404      	jsr %pc@(0x84c8)                      ; IdByteChange
    80c6:  1f3c 000f      	moveb #15,%sp@-                       
    80ca:  486e fff8      	pea %fp@(-8)                          
    80ce:  4eba 016c      	jsr %pc@(0x823c)                      ; IdRightRotation
    80d2:  202e fff8      	movel %fp@(-8),%d0                    
    80d6:  b0ae 0014      	cmpl %fp@(20),%d0                     
    80da:  4fef 003a      	lea %sp@(58),%sp                      
    80de:  6704           	beqs 0x80e4                           ; ↓ label_4
    80e0:  7000           	moveq #0,%d0                          
    80e2:  6010           	bras 0x80f4                           ; ↓ label_6

  label_4:
    80e4:  2013           	movel %a3@,%d0                        
    80e6:  b0ae 0018      	cmpl %fp@(24),%d0                     
    80ea:  6704           	beqs 0x80f0                           ; ↓ label_5
    80ec:  7000           	moveq #0,%d0                          
    80ee:  6004           	bras 0x80f4                           ; ↓ label_6

  label_5:
    80f0:  202e 0008      	movel %fp@(8),%d0                     

  label_6:
    80f4:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3          
    80f8:  4e5e           	unlk %fp                              
    80fa:  4e75           	rts                                   
```

# CheckIDExec: 0x810a - 0x8188

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???
   * `%fp@(-10)`    : ???

```
    810a:  4e56 fff6      	linkw %fp,#-10                        
    810e:  486e fffe      	pea %fp@(-2)                          
    8112:  486e fffa      	pea %fp@(-6)                          
    8116:  2f3c 7469 6d65 	movel #1953066341,%sp@-               
    811c:  4267           	clrw %sp@-                            
    811e:  4e4f           	trap #15                              
    8120:  a340           	0121500                               ; sysTrapHwrGetROMToken
    8122:  4a40           	tstw %d0                              
    8124:  4fef 000e      	lea %sp@(14),%sp                      
    8128:  6704           	beqs 0x812e                           ; ↓ label_0
    812a:  7000           	moveq #0,%d0                          
    812c:  6058           	bras 0x8186                           ; ↓ label_4

  label_0:
    812e:  0c6e 0008 fffe 	cmpiw #8,%fp@(-2)                     
    8134:  6704           	beqs 0x813a                           ; ↓ label_1
    8136:  7000           	moveq #0,%d0                          
    8138:  604c           	bras 0x8186                           ; ↓ label_4

  label_1:
    813a:  486e fffe      	pea %fp@(-2)                          
    813e:  486e fff6      	pea %fp@(-10)                         
    8142:  2f3c 736f 6e79 	movel #1936682617,%sp@-               
    8148:  4267           	clrw %sp@-                            
    814a:  4e4f           	trap #15                              
    814c:  a340           	0121500                               ; sysTrapHwrGetROMToken
    814e:  4a40           	tstw %d0                              
    8150:  4fef 000e      	lea %sp@(14),%sp                      
    8154:  6704           	beqs 0x815a                           ; ↓ label_2
    8156:  7000           	moveq #0,%d0                          
    8158:  602c           	bras 0x8186                           ; ↓ label_4

  label_2:
    815a:  0c6e 0008 fffe 	cmpiw #8,%fp@(-2)                     
    8160:  6704           	beqs 0x8166                           ; ↓ label_3
    8162:  7000           	moveq #0,%d0                          
    8164:  6020           	bras 0x8186                           ; ↓ label_4

  label_3:
    8166:  206e fff6      	moveal %fp@(-10),%a0                  
    816a:  2f28 0004      	movel %a0@(4),%sp@-                   
    816e:  2f10           	movel %a0@,%sp@-                      
    8170:  206e fffa      	moveal %fp@(-6),%a0                   
    8174:  2f28 0004      	movel %a0@(4),%sp@-                   
    8178:  2f10           	movel %a0@,%sp@-                      
    817a:  2f2e 0008      	movel %fp@(8),%sp@-                   
    817e:  4eba fe5e      	jsr %pc@(0x7fde)                      ; CheckSonyId
    8182:  4fef 0014      	lea %sp@(20),%sp                      

  label_4:
    8186:  4e5e           	unlk %fp                              
    8188:  4e75           	rts                                   
```

# SearchFlashCharCode: 0x8198 - 0x81e2

Parameters:

   * `%fp@(8)`      : ???

Locals:

   * `%fp@(-32)`    : ???

```
    8198:  4e56 ffe0      	linkw %fp,#-32                        
    819c:  48e7 1820      	moveml %d3-%d4/%a2,%sp@-              
    81a0:  41fa 0058      	lea %pc@(0x81fa),%a0                  ; ??? 0x81fa
    81a4:  43ee ffe0      	lea %fp@(-32),%a1                     
    81a8:  22d8           	movel %a0@+,%a1@+                     
    81aa:  22d8           	movel %a0@+,%a1@+                     
    81ac:  22d8           	movel %a0@+,%a1@+                     
    81ae:  22d8           	movel %a0@+,%a1@+                     
    81b0:  22d8           	movel %a0@+,%a1@+                     
    81b2:  22d8           	movel %a0@+,%a1@+                     
    81b4:  22d8           	movel %a0@+,%a1@+                     
    81b6:  22d8           	movel %a0@+,%a1@+                     
    81b8:  7600           	moveq #0,%d3                          
    81ba:  45ee ffe0      	lea %fp@(-32),%a2                     
    81be:  7800           	moveq #0,%d4                          
    81c0:  182e 0008      	moveb %fp@(8),%d4                     

  label_0:
    81c4:  1012           	moveb %a2@,%d0                        
    81c6:  4880           	extw %d0                              
    81c8:  b840           	cmpw %d0,%d4                          
    81ca:  6604           	bnes 0x81d0                           ; ↓ label_1
    81cc:  1003           	moveb %d3,%d0                         
    81ce:  600c           	bras 0x81dc                           ; ↓ label_2

  label_1:
    81d0:  528a           	addql #1,%a2                          
    81d2:  5203           	addqb #1,%d3                          
    81d4:  0c03 0020      	cmpib #32,%d3                         
    81d8:  65ea           	bcss 0x81c4                           ; ↑ label_0
    81da:  7000           	moveq #0,%d0                          

  label_2:
    81dc:  4cdf 0418      	moveml %sp@+,%d3-%d4/%a2              
    81e0:  4e5e           	unlk %fp                              
    81e2:  4e75           	rts                                   
```

# IdXOR: 0x821a - 0x8232

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???

```
    821a:  4e56 0000      	linkw %fp,#0                          
    821e:  206e 0008      	moveal %fp@(8),%a0                    
    8222:  202e 000c      	movel %fp@(12),%d0                    
    8226:  b190           	eorl %d0,%a0@                         
    8228:  202e 0010      	movel %fp@(16),%d0                    
    822c:  b1a8 0004      	eorl %d0,%a0@(4)                      
    8230:  4e5e           	unlk %fp                              
    8232:  4e75           	rts                                   
```

# IdRightRotation: 0x823c - 0x8492

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???

```
    823c:  4e56 0000      	linkw %fp,#0                          
    8240:  48e7 0700      	moveml %d5-%d7,%sp@-                  
    8244:  206e 0008      	moveal %fp@(8),%a0                    
    8248:  1c2e 000c      	moveb %fp@(12),%d6                    
    824c:  7200           	moveq #0,%d1                          
    824e:  4a06           	tstb %d6                              
    8250:  6300 023a      	blsw 0x848c                           ; ↓ label_3
    8254:  1e06           	moveb %d6,%d7                         
    8256:  5107           	subqb #8,%d7                          
    8258:  0c06 0008      	cmpib #8,%d6                          
    825c:  6300 022a      	blsw 0x8488                           ; ↓ label_2

  label_0:
    8260:  7a01           	moveq #1,%d5                          
    8262:  ca28 0003      	andb %a0@(3),%d5                      
    8266:  7401           	moveq #1,%d2                          
    8268:  c428 0007      	andb %a0@(7),%d2                      
    826c:  2010           	movel %a0@,%d0                        
    826e:  e288           	lsrl #1,%d0                           
    8270:  2080           	movel %d0,%a0@                        
    8272:  2028 0004      	movel %a0@(4),%d0                     
    8276:  e288           	lsrl #1,%d0                           
    8278:  2140 0004      	movel %d0,%a0@(4)                     
    827c:  1005           	moveb %d5,%d0                         
    827e:  ef08           	lslb #7,%d0                           
    8280:  0200 0080      	andib #-128,%d0                       
    8284:  0228 007f 0004 	andib #127,%a0@(4)                    
    828a:  8128 0004      	orb %d0,%a0@(4)                       
    828e:  1002           	moveb %d2,%d0                         
    8290:  ef08           	lslb #7,%d0                           
    8292:  0200 0080      	andib #-128,%d0                       
    8296:  0210 007f      	andib #127,%a0@                       
    829a:  8110           	orb %d0,%a0@                          
    829c:  7401           	moveq #1,%d2                          
    829e:  c428 0003      	andb %a0@(3),%d2                      
    82a2:  7a01           	moveq #1,%d5                          
    82a4:  ca28 0007      	andb %a0@(7),%d5                      
    82a8:  2010           	movel %a0@,%d0                        
    82aa:  e288           	lsrl #1,%d0                           
    82ac:  2080           	movel %d0,%a0@                        
    82ae:  2028 0004      	movel %a0@(4),%d0                     
    82b2:  e288           	lsrl #1,%d0                           
    82b4:  2140 0004      	movel %d0,%a0@(4)                     
    82b8:  1002           	moveb %d2,%d0                         
    82ba:  ef08           	lslb #7,%d0                           
    82bc:  0200 0080      	andib #-128,%d0                       
    82c0:  0228 007f 0004 	andib #127,%a0@(4)                    
    82c6:  8128 0004      	orb %d0,%a0@(4)                       
    82ca:  1005           	moveb %d5,%d0                         
    82cc:  ef08           	lslb #7,%d0                           
    82ce:  0200 0080      	andib #-128,%d0                       
    82d2:  0210 007f      	andib #127,%a0@                       
    82d6:  8110           	orb %d0,%a0@                          
    82d8:  7401           	moveq #1,%d2                          
    82da:  c428 0003      	andb %a0@(3),%d2                      
    82de:  7a01           	moveq #1,%d5                          
    82e0:  ca28 0007      	andb %a0@(7),%d5                      
    82e4:  2010           	movel %a0@,%d0                        
    82e6:  e288           	lsrl #1,%d0                           
    82e8:  2080           	movel %d0,%a0@                        
    82ea:  2028 0004      	movel %a0@(4),%d0                     
    82ee:  e288           	lsrl #1,%d0                           
    82f0:  2140 0004      	movel %d0,%a0@(4)                     
    82f4:  1002           	moveb %d2,%d0                         
    82f6:  ef08           	lslb #7,%d0                           
    82f8:  0200 0080      	andib #-128,%d0                       
    82fc:  0228 007f 0004 	andib #127,%a0@(4)                    
    8302:  8128 0004      	orb %d0,%a0@(4)                       
    8306:  1005           	moveb %d5,%d0                         
    8308:  ef08           	lslb #7,%d0                           
    830a:  0200 0080      	andib #-128,%d0                       
    830e:  0210 007f      	andib #127,%a0@                       
    8312:  8110           	orb %d0,%a0@                          
    8314:  7401           	moveq #1,%d2                          
    8316:  c428 0003      	andb %a0@(3),%d2                      
    831a:  7a01           	moveq #1,%d5                          
    831c:  ca28 0007      	andb %a0@(7),%d5                      
    8320:  2010           	movel %a0@,%d0                        
    8322:  e288           	lsrl #1,%d0                           
    8324:  2080           	movel %d0,%a0@                        
    8326:  2028 0004      	movel %a0@(4),%d0                     
    832a:  e288           	lsrl #1,%d0                           
    832c:  2140 0004      	movel %d0,%a0@(4)                     
    8330:  1002           	moveb %d2,%d0                         
    8332:  ef08           	lslb #7,%d0                           
    8334:  0200 0080      	andib #-128,%d0                       
    8338:  0228 007f 0004 	andib #127,%a0@(4)                    
    833e:  8128 0004      	orb %d0,%a0@(4)                       
    8342:  1005           	moveb %d5,%d0                         
    8344:  ef08           	lslb #7,%d0                           
    8346:  0200 0080      	andib #-128,%d0                       
    834a:  0210 007f      	andib #127,%a0@                       
    834e:  8110           	orb %d0,%a0@                          
    8350:  7401           	moveq #1,%d2                          
    8352:  c428 0003      	andb %a0@(3),%d2                      
    8356:  7a01           	moveq #1,%d5                          
    8358:  ca28 0007      	andb %a0@(7),%d5                      
    835c:  2010           	movel %a0@,%d0                        
    835e:  e288           	lsrl #1,%d0                           
    8360:  2080           	movel %d0,%a0@                        
    8362:  2028 0004      	movel %a0@(4),%d0                     
    8366:  e288           	lsrl #1,%d0                           
    8368:  2140 0004      	movel %d0,%a0@(4)                     
    836c:  1002           	moveb %d2,%d0                         
    836e:  ef08           	lslb #7,%d0                           
    8370:  0200 0080      	andib #-128,%d0                       
    8374:  0228 007f 0004 	andib #127,%a0@(4)                    
    837a:  8128 0004      	orb %d0,%a0@(4)                       
    837e:  1005           	moveb %d5,%d0                         
    8380:  ef08           	lslb #7,%d0                           
    8382:  0200 0080      	andib #-128,%d0                       
    8386:  0210 007f      	andib #127,%a0@                       
    838a:  8110           	orb %d0,%a0@                          
    838c:  7401           	moveq #1,%d2                          
    838e:  c428 0003      	andb %a0@(3),%d2                      
    8392:  7a01           	moveq #1,%d5                          
    8394:  ca28 0007      	andb %a0@(7),%d5                      
    8398:  2010           	movel %a0@,%d0                        
    839a:  e288           	lsrl #1,%d0                           
    839c:  2080           	movel %d0,%a0@                        
    839e:  2028 0004      	movel %a0@(4),%d0                     
    83a2:  e288           	lsrl #1,%d0                           
    83a4:  2140 0004      	movel %d0,%a0@(4)                     
    83a8:  1002           	moveb %d2,%d0                         
    83aa:  ef08           	lslb #7,%d0                           
    83ac:  0200 0080      	andib #-128,%d0                       
    83b0:  0228 007f 0004 	andib #127,%a0@(4)                    
    83b6:  8128 0004      	orb %d0,%a0@(4)                       
    83ba:  1005           	moveb %d5,%d0                         
    83bc:  ef08           	lslb #7,%d0                           
    83be:  0200 0080      	andib #-128,%d0                       
    83c2:  0210 007f      	andib #127,%a0@                       
    83c6:  8110           	orb %d0,%a0@                          
    83c8:  7401           	moveq #1,%d2                          
    83ca:  c428 0003      	andb %a0@(3),%d2                      
    83ce:  7a01           	moveq #1,%d5                          
    83d0:  ca28 0007      	andb %a0@(7),%d5                      
    83d4:  2010           	movel %a0@,%d0                        
    83d6:  e288           	lsrl #1,%d0                           
    83d8:  2080           	movel %d0,%a0@                        
    83da:  2028 0004      	movel %a0@(4),%d0                     
    83de:  e288           	lsrl #1,%d0                           
    83e0:  2140 0004      	movel %d0,%a0@(4)                     
    83e4:  1002           	moveb %d2,%d0                         
    83e6:  ef08           	lslb #7,%d0                           
    83e8:  0200 0080      	andib #-128,%d0                       
    83ec:  0228 007f 0004 	andib #127,%a0@(4)                    
    83f2:  8128 0004      	orb %d0,%a0@(4)                       
    83f6:  1005           	moveb %d5,%d0                         
    83f8:  ef08           	lslb #7,%d0                           
    83fa:  0200 0080      	andib #-128,%d0                       
    83fe:  0210 007f      	andib #127,%a0@                       
    8402:  8110           	orb %d0,%a0@                          
    8404:  7401           	moveq #1,%d2                          
    8406:  c428 0003      	andb %a0@(3),%d2                      
    840a:  7a01           	moveq #1,%d5                          
    840c:  ca28 0007      	andb %a0@(7),%d5                      
    8410:  2010           	movel %a0@,%d0                        
    8412:  e288           	lsrl #1,%d0                           
    8414:  2080           	movel %d0,%a0@                        
    8416:  2028 0004      	movel %a0@(4),%d0                     
    841a:  e288           	lsrl #1,%d0                           
    841c:  2140 0004      	movel %d0,%a0@(4)                     
    8420:  1002           	moveb %d2,%d0                         
    8422:  ef08           	lslb #7,%d0                           
    8424:  0200 0080      	andib #-128,%d0                       
    8428:  0228 007f 0004 	andib #127,%a0@(4)                    
    842e:  8128 0004      	orb %d0,%a0@(4)                       
    8432:  1005           	moveb %d5,%d0                         
    8434:  ef08           	lslb #7,%d0                           
    8436:  0200 0080      	andib #-128,%d0                       
    843a:  0210 007f      	andib #127,%a0@                       
    843e:  8110           	orb %d0,%a0@                          
    8440:  5001           	addqb #8,%d1                          
    8442:  b207           	cmpb %d7,%d1                          
    8444:  6500 fe1a      	bcsw 0x8260                           ; ↑ label_0
    8448:  603e           	bras 0x8488                           ; ↓ label_2

  label_1:
    844a:  7401           	moveq #1,%d2                          
    844c:  c428 0003      	andb %a0@(3),%d2                      
    8450:  7a01           	moveq #1,%d5                          
    8452:  ca28 0007      	andb %a0@(7),%d5                      
    8456:  2010           	movel %a0@,%d0                        
    8458:  e288           	lsrl #1,%d0                           
    845a:  2080           	movel %d0,%a0@                        
    845c:  2028 0004      	movel %a0@(4),%d0                     
    8460:  e288           	lsrl #1,%d0                           
    8462:  2140 0004      	movel %d0,%a0@(4)                     
    8466:  1002           	moveb %d2,%d0                         
    8468:  ef08           	lslb #7,%d0                           
    846a:  0200 0080      	andib #-128,%d0                       
    846e:  0228 007f 0004 	andib #127,%a0@(4)                    
    8474:  8128 0004      	orb %d0,%a0@(4)                       
    8478:  1005           	moveb %d5,%d0                         
    847a:  ef08           	lslb #7,%d0                           
    847c:  0200 0080      	andib #-128,%d0                       
    8480:  0210 007f      	andib #127,%a0@                       
    8484:  8110           	orb %d0,%a0@                          
    8486:  5201           	addqb #1,%d1                          

  label_2:
    8488:  b206           	cmpb %d6,%d1                          
    848a:  65be           	bcss 0x844a                           ; ↑ label_1

  label_3:
    848c:  4cdf 00e0      	moveml %sp@+,%d5-%d7                  
    8490:  4e5e           	unlk %fp                              
    8492:  4e75           	rts                                   
```

# IdAdd: 0x84a6 - 0x84be

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(16)`     : ???

```
    84a6:  4e56 0000      	linkw %fp,#0                          
    84aa:  206e 0008      	moveal %fp@(8),%a0                    
    84ae:  202e 000c      	movel %fp@(12),%d0                    
    84b2:  d190           	addl %d0,%a0@                         
    84b4:  202e 0010      	movel %fp@(16),%d0                    
    84b8:  d1a8 0004      	addl %d0,%a0@(4)                      
    84bc:  4e5e           	unlk %fp                              
    84be:  4e75           	rts                                   
```

# IdByteChange: 0x84c8 - 0x84f4

Parameters:

   * `%fp@(8)`      : ???
   * `%fp@(12)`     : ???
   * `%fp@(14)`     : ???

```
    84c8:  4e56 0000      	linkw %fp,#0                          
    84cc:  2f0b           	movel %a3,%sp@-                       
    84ce:  7000           	moveq #0,%d0                          
    84d0:  102e 000c      	moveb %fp@(12),%d0                    
    84d4:  206e 0008      	moveal %fp@(8),%a0                    
    84d8:  d0c0           	addaw %d0,%a0                         
    84da:  2248           	moveal %a0,%a1                        
    84dc:  1210           	moveb %a0@,%d1                        
    84de:  7000           	moveq #0,%d0                          
    84e0:  102e 000e      	moveb %fp@(14),%d0                    
    84e4:  206e 0008      	moveal %fp@(8),%a0                    
    84e8:  d0c0           	addaw %d0,%a0                         
    84ea:  2648           	moveal %a0,%a3                        
    84ec:  1290           	moveb %a0@,%a1@                       
    84ee:  1681           	moveb %d1,%a3@                        
    84f0:  265f           	moveal %sp@+,%a3                      
    84f2:  4e5e           	unlk %fp                              
    84f4:  4e75           	rts                                   
```

