# __Startup__: 0x0070 - 0x007a

```
    70:  4e56 0000      	linkw %fp,#0                            
    74:  4eba 0014      	jsr %pc@(0x8a)                          ; InstallTrapPatches
    78:  4e5e           	unlk %fp                                
    7a:  4e75           	rts                                     
```

# InstallTrapPatches: 0x008a - 0x00f4

Locals:

   * `%fp@(-2)`     : ???
   * `%fp@(-6)`     : ???

```
    8a:  4e56 fffa      	linkw %fp,#-6                           
    8e:  2f03           	movel %d3,%sp@-                         
    90:  42a7           	clrl %sp@-                              
    92:  486e fffe      	pea %fp@(-2)                            
    96:  42a7           	clrl %sp@-                              
    98:  42a7           	clrl %sp@-                              
    9a:  486e fffa      	pea %fp@(-6)                            
    9e:  42a7           	clrl %sp@-                              
    a0:  4e4f           	trap #15                                
    a2:  a063           	0120143                                 ; sysTrapDmNextOpenResDatabase
    a4:  584f           	addqw #4,%sp                            
    a6:  2f08           	movel %a0,%sp@-                         
    a8:  4e4f           	trap #15                                
    aa:  a04c           	0120114                                 ; sysTrapDmOpenDatabaseInfo
    ac:  1f3c 0001      	moveb #1,%sp@-                          
    b0:  2f2e fffa      	movel %fp@(-6),%sp@-                    
    b4:  3f2e fffe      	movew %fp@(-2),%sp@-                    
    b8:  4e4f           	trap #15                                
    ba:  a2eb           	0121353                                 ; sysTrapDmDatabaseProtect
    bc:  4267           	clrw %sp@-                              
    be:  2f3c 6578 746e 	movel #1702392942,%sp@-                 
    c4:  4e4f           	trap #15                                
    c6:  a060           	0120140                                 ; sysTrapDmGet1Resource
    c8:  5c4f           	addqw #6,%sp                            
    ca:  2f08           	movel %a0,%sp@-                         
    cc:  4e4f           	trap #15                                
    ce:  a021           	0120041                                 ; sysTrapMemHandleLock
    d0:  4e4f           	trap #15                                
    d2:  a0a5           	0120245                                 ; sysTrapSysDisableInts
    d4:  3600           	movew %d0,%d3                           
    d6:  41fa 0034      	lea %pc@(0x10c),%a0                     ; Hwr1859DspExecPatch
    da:  2278 0304      	moveal 0x304,%a1                        
    de:  2269 0018      	moveal %a1@(24),%a1                     
    e2:  2348 009a      	movel %a0,%a1@(154)                     
    e6:  3f03           	movew %d3,%sp@-                         
    e8:  4e4f           	trap #15                                
    ea:  a0a6           	0120246                                 ; sysTrapSysRestoreStatus
    ec:  4fef 0026      	lea %sp@(38),%sp                        
    f0:  261f           	movel %sp@+,%d3                         
    f2:  4e5e           	unlk %fp                                
    f4:  4e75           	rts                                     
```

# Hwr1859DspExecPatch: 0x010c - 0x0342

Parameters:

   * `%fp@(8)`      : params

Locals:

   * `%fp@(-4)`     : ???

```
  struct Params {
    uint16 ipcCmd;              // 00
    uint16 ipcArg1;             // 02
    uint16 ipcArg2;             // 04
    uint16 ipcArg3;             // 06
    uint16 ipcArg4;             // 08
    uint16 ipcArg5;             // 10
    uint16 ipcArg6;             // 12

    uint16 ipcSubCmd;           // 18
    uint16 ipcSubArg1;          // 20
    uint16 ipcSubArg2;          // 22
    uint16 ipcSubArg3;          // 24
    uint16 ipcSubArg4;          // 26
    uint16 ipcSubArg5;          // 28
    uint16 ipcSubArg6;          // 30

    ...                         // 32 - 35

    uint8 sync                // 36
    void* DspDoneHandler        // 38
    void* dspDoneHandlerContext;// 42
  };

  uint16 DspExec(Params* params) {
    %a0 = gHalGlobals;
    %a3 = %d4 = dspGlobals = HalGlobals->gDspGlobals;

    %d0 = (gHalGlobals == 0 || dspGlobals == 0) ? 0 : 1;

    if (%d0) {
      %a0 = dspGlobals;
      %d0 = dspGlobals->long44 & 0x00000001;
    } else {
      %d0 = 1;
      return;
    }

    %d5 = SysDisableInts();

    %d0 = dspGlobals->long44 & 0x00000004;
    if (%d0 == 0) {
      local32 = 0x53684473; // "ShDs"
      dspGlobals->long44 |= 0x00000004;

      SysSemaphoreCreate(&dspGlobals->execSemaphore, local32, 0x00000001);
    }

    SysRestoreStatus(%d5);
    SysSemaphoreWait(dspGlobals->execSemaphore, 0x00000000, 0x00000000);

    if (!params->sync) {
      %d3 = 0;
      dsp_0222 &= 0xfffe;
      dsp_0222 |= 0x0001;

      SysSemaphoreSignal(dspGlobals->execSemaphore);
      %d0 = 2;
    }

    if (params->ipcSubCmd) {
      dsp_0c08 = params->ipcSubArg1;
      dsp_0c0a = params->ipcSubArg2;
      dsp_0c0c = params->ipcSubArg3;
      dsp_0c0e = params->ipcSubArg4;
      dsp_0c10 = params->ipcSubArg5;
      dsp_0c12 = params->ipcSubArg6;
      dsp_0c04 = params->ipcSubCmd;

      while (dsp_0c04) {}
    }

    dsp_0c08 = params->ipcArg1;
    dsp_0c0a = params->ipcArg2;
    dsp_0c0c = params->ipcArg3;
    dsp_0c0e = params->ipcArg4;
    dsp_0c10 = params->ipcArg5;
    dsp_0c12 = params->ipcArg6;

    dsp_0222 &= 0xfffe;

    if (params->dspDoneHandler) {
      if (params->sync) {
        myDspExecState = %a0 = &dspGlobals->dspExecState;
      } else {
        myDspExecState = %a0 = &dspGlobals->dspExecState_1;
      }

      myDspExecState->expectedResult = params->ipcCommand;
      myDspExecState->handler = params->DspDoneHandler;
      myDspExecState->context = params->dspDoneHandlerContext;
      myDspExecState->taskId = SysTaskID();
    }

    dsp_0202 |= 0x0800;
    dsp_0e60 |= 0x0010;

    SysTaskWaitClr();

    dsp_0c04 = params->ipcCmd;
    dps_0222 |= 0x01;

    %d3 = 0;

    if (!params->DspDoneHandler || !params->sync) {
      SysSemaphoreSignal(dspGlobals->execSemaphore);
      return 0;
    }

    %d5 = (params->ipcCommand == 0x2201 || params->ipcCommand == 0x3a01) ? 0x0500 : 0x0200;
    
    %d6 = SysTaskWait(%d5);
    if (%d6 != 0) {
      dsp_0222 &= 0xfffe;

      if (params->DspDoneHandler) params->DspDoneHandler(params->dspDoneHandlerContext, 0);

      dpsGlobals->dspExecState->expectedResult = 0x0000;
      %d3 = 3;

      dsp_0222 |= 0x0001;
    }

    SysSemaphoreSignal(dspGlobals->execSemaphore);
    return %d3;
  }
```

```
    10c:  4e56 fffc      	linkw %fp,#-4                          
    110:  48e7 1e30      	moveml %d3-%d6/%a2-%a3,%sp@-           
    114:  246e 0008      	moveal %fp@(8),%a2                     
    118:  2078 0304      	moveal 0x304,%a0                       
    11c:  2668 0018      	moveal %a0@(24),%a3                    
    120:  280b           	movel %a3,%d4                          
    122:  4ab8 0304      	tstl 0x304                             
    126:  6708           	beqs 0x130                             ; ↓ label_0
    128:  200b           	movel %a3,%d0                          
    12a:  6704           	beqs 0x130                             ; ↓ label_0
    12c:  7001           	moveq #1,%d0                           
    12e:  6002           	bras 0x132                             ; ↓ label_1

  label_0:
    130:  7000           	moveq #0,%d0                           

  label_1:
    132:  4a40           	tstw %d0                               
    134:  670a           	beqs 0x140                             ; ↓ label_2
    136:  7001           	moveq #1,%d0                           
    138:  2044           	moveal %d4,%a0                         
    13a:  c0a8 002c      	andl %a0@(44),%d0                      
    13e:  6606           	bnes 0x146                             ; ↓ label_3

  label_2:
    140:  7001           	moveq #1,%d0                           
    142:  6000 01f8      	braw 0x33c                             ; ↓ label_21

  label_3:
    146:  4e4f           	trap #15                               
    148:  a0a5           	0120245                                ; sysTrapSysDisableInts
    14a:  3a00           	movew %d0,%d5                          
    14c:  7004           	moveq #4,%d0                           
    14e:  2044           	moveal %d4,%a0                         
    150:  c0a8 002c      	andl %a0@(44),%d0                      
    154:  6622           	bnes 0x178                             ; ↓ label_4
    156:  2d7c 5368 4473 	movel #1399342195,%fp@(-4)             
    15c:  fffc                                                   
    15e:  7004           	moveq #4,%d0                           
    160:  81a8 002c      	orl %d0,%a0@(44)                       
    164:  4878 0001      	pea 0x1                                
    168:  486e fffc      	pea %fp@(-4)                           
    16c:  4868 003a      	pea %a0@(58)                           
    170:  4e4f           	trap #15                               
    172:  a095           	0120225                                ; sysTrapSysSemaphoreCreate
    174:  4fef 000c      	lea %sp@(12),%sp                       

  label_4:
    178:  3f05           	movew %d5,%sp@-                        
    17a:  4e4f           	trap #15                               
    17c:  a0a6           	0120246                                ; sysTrapSysRestoreStatus
    17e:  42a7           	clrl %sp@-                             
    180:  42a7           	clrl %sp@-                             
    182:  2044           	moveal %d4,%a0                         
    184:  2f28 003a      	movel %a0@(58),%sp@-                   
    188:  4e4f           	trap #15                               
    18a:  a097           	0120227                                ; sysTrapSysSemaphoreWait
    18c:  4a2a 0024      	tstb %a2@(36)                          
    190:  4fef 000e      	lea %sp@(14),%sp                       
    194:  6644           	bnes 0x1da                             ; ↓ label_7
    196:  0279 fffe 1100 	andiw #-2,0x11000222                   
    19c:  0222                                                   
    19e:  7600           	moveq #0,%d3                           
    1a0:  0c43 0005      	cmpiw #5,%d3                           
    1a4:  6c14           	bges 0x1ba                             ; ↓ label_6
    1a6:  2644           	moveal %d4,%a3                         

  label_5:
    1a8:  4a6b 004c      	tstw %a3@(76)                          
    1ac:  670c           	beqs 0x1ba                             ; ↓ label_6
    1ae:  508b           	addql #8,%a3                           
    1b0:  5c8b           	addql #6,%a3                           
    1b2:  5243           	addqw #1,%d3                           
    1b4:  0c43 0005      	cmpiw #5,%d3                           
    1b8:  6dee           	blts 0x1a8                             ; ↑ label_5

  label_6:
    1ba:  0079 0001 1100 	oriw #1,0x11000222                     
    1c0:  0222                                                   
    1c2:  0c43 0005      	cmpiw #5,%d3                           
    1c6:  6d12           	blts 0x1da                             ; ↓ label_7
    1c8:  2044           	moveal %d4,%a0                         
    1ca:  2f28 003a      	movel %a0@(58),%sp@-                   
    1ce:  4e4f           	trap #15                               
    1d0:  a098           	0120230                                ; sysTrapSysSemaphoreSignal
    1d2:  7002           	moveq #2,%d0                           
    1d4:  584f           	addqw #4,%sp                           
    1d6:  6000 0164      	braw 0x33c                             ; ↓ label_21

  label_7:
    1da:  4a6a 0012      	tstw %a2@(18)                          
    1de:  6740           	beqs 0x220                             ; ↓ label_9
    1e0:  33ea 0014 1100 	movew %a2@(20),0x11000c08              
    1e6:  0c08                                                   
    1e8:  33ea 0016 1100 	movew %a2@(22),0x11000c0a              
    1ee:  0c0a                                                   
    1f0:  33ea 0018 1100 	movew %a2@(24),0x11000c0c              
    1f6:  0c0c                                                   
    1f8:  33ea 001a 1100 	movew %a2@(26),0x11000c0e              
    1fe:  0c0e                                                   
    200:  33ea 001c 1100 	movew %a2@(28),0x11000c10              
    206:  0c10                                                   
    208:  33ea 001e 1100 	movew %a2@(30),0x11000c12              
    20e:  0c12                                                   
    210:  33ea 0012 1100 	movew %a2@(18),0x11000c04              
    216:  0c04                                                   

  label_8:
    218:  4a79 1100 0c04 	tstw 0x11000c04                        
    21e:  66f8           	bnes 0x218                             ; ↑ label_8

  label_9:
    220:  33ea 0002 1100 	movew %a2@(2),0x11000c08               
    226:  0c08                                                   
    228:  33ea 0004 1100 	movew %a2@(4),0x11000c0a               
    22e:  0c0a                                                   
    230:  33ea 0006 1100 	movew %a2@(6),0x11000c0c               
    236:  0c0c                                                   
    238:  33ea 0008 1100 	movew %a2@(8),0x11000c0e               
    23e:  0c0e                                                   
    240:  33ea 000a 1100 	movew %a2@(10),0x11000c10              
    246:  0c10                                                   
    248:  33ea 000c 1100 	movew %a2@(12),0x11000c12              
    24e:  0c12                                                   
    250:  0279 fffe 1100 	andiw #-2,0x11000222                   
    256:  0222                                                   
    258:  4aaa 0026      	tstl %a2@(38)                          
    25c:  6732           	beqs 0x290                             ; ↓ label_12
    25e:  4a2a 0024      	tstb %a2@(36)                          
    262:  6708           	beqs 0x26c                             ; ↓ label_10
    264:  2044           	moveal %d4,%a0                         
    266:  41e8 003e      	lea %a0@(62),%a0                       
    26a:  600c           	bras 0x278                             ; ↓ label_11

  label_10:
    26c:  700e           	moveq #14,%d0                          
    26e:  c1c3           	mulsw %d3,%d0                          
    270:  2044           	moveal %d4,%a0                         
    272:  d1c0           	addal %d0,%a0                          
    274:  41e8 004c      	lea %a0@(76),%a0                       

  label_11:
    278:  2648           	moveal %a0,%a3                         
    27a:  3692           	movew %a2@,%a3@                        
    27c:  276a 0026 0002 	movel %a2@(38),%a3@(2)                 
    282:  276a 002a 0006 	movel %a2@(42),%a3@(6)                 
    288:  4e4f           	trap #15                               
    28a:  a09e           	0120236                                ; sysTrapSysTaskID
    28c:  2740 000a      	movel %d0,%a3@(10)                     

  label_12:
    290:  0079 0800 1100 	oriw #2048,0x11000202                  
    296:  0202                                                   
    298:  0079 0010 1100 	oriw #16,0x11000e60                    
    29e:  0e60                                                   
    2a0:  4e4f           	trap #15                               
    2a2:  a2c0           	0121300                                ; sysTrapSysTaskWaitClr
    2a4:  33d2 1100 0c04 	movew %a2@,0x11000c04                  
    2aa:  0079 0001 1100 	oriw #1,0x11000222                     
    2b0:  0222                                                   
    2b2:  7600           	moveq #0,%d3                           
    2b4:  4aaa 0026      	tstl %a2@(38)                          
    2b8:  6706           	beqs 0x2c0                             ; ↓ label_13
    2ba:  4a2a 0024      	tstb %a2@(36)                          
    2be:  6614           	bnes 0x2d4                             ; ↓ label_14

  label_13:
    2c0:  2078 0304      	moveal 0x304,%a0                       
    2c4:  2068 0018      	moveal %a0@(24),%a0                    
    2c8:  2f28 003a      	movel %a0@(58),%sp@-                   
    2cc:  4e4f           	trap #15                               
    2ce:  a098           	0120230                                ; sysTrapSysSemaphoreSignal
    2d0:  584f           	addqw #4,%sp                           
    2d2:  6066           	bras 0x33a                             ; ↓ label_20

  label_14:
    2d4:  3012           	movew %a2@,%d0                         
    2d6:  0440 2201      	subiw #8705,%d0                        
    2da:  6708           	beqs 0x2e4                             ; ↓ label_15
    2dc:  0440 1800      	subiw #6144,%d0                        
    2e0:  6702           	beqs 0x2e4                             ; ↓ label_15
    2e2:  6008           	bras 0x2ec                             ; ↓ label_16

  label_15:
    2e4:  2a3c 0000 01f4 	movel #500,%d5                         
    2ea:  6006           	bras 0x2f2                             ; ↓ label_17

  label_16:
    2ec:  2a3c 0000 00c8 	movel #200,%d5                         

  label_17:
    2f2:  2f05           	movel %d5,%sp@-                        
    2f4:  4e4f           	trap #15                               
    2f6:  a2be           	0121276                                ; sysTrapSysTaskWait
    2f8:  3c00           	movew %d0,%d6                          
    2fa:  584f           	addqw #4,%sp                           
    2fc:  672a           	beqs 0x328                             ; ↓ label_19
    2fe:  0279 fffe 1100 	andiw #-2,0x11000222                   
    304:  0222                                                   
    306:  266a 0026      	moveal %a2@(38),%a3                    
    30a:  200b           	movel %a3,%d0                          
    30c:  670a           	beqs 0x318                             ; ↓ label_18
    30e:  4227           	clrb %sp@-                             
    310:  2f2a 002a      	movel %a2@(42),%sp@-                   
    314:  4e93           	jsr %a3@                               
    316:  5c4f           	addqw #6,%sp                           

  label_18:
    318:  2044           	moveal %d4,%a0                         
    31a:  4268 003e      	clrw %a0@(62)                          
    31e:  7603           	moveq #3,%d3                           
    320:  0079 0001 1100 	oriw #1,0x11000222                     
    326:  0222                                                   

  label_19:
    328:  2078 0304      	moveal 0x304,%a0                       
    32c:  2068 0018      	moveal %a0@(24),%a0                    
    330:  2f28 003a      	movel %a0@(58),%sp@-                   
    334:  4e4f           	trap #15                               
    336:  a098           	0120230                                ; sysTrapSysSemaphoreSignal
    338:  584f           	addqw #4,%sp                           

  label_20:
    33a:  3003           	movew %d3,%d0                          

  label_21:
    33c:  4cdf 0c78      	moveml %sp@+,%d3-%d6/%a2-%a3           
    340:  4e5e           	unlk %fp                               
    342:  4e75           	rts                                    
```

