# IspSpiCard0

```
   e4a36:       4e56 fffc       linkw %fp,#-4
   e4a3a:       48e7 1038       moveml %d3/%a2-%a4,%sp@-
   e4a3e:       2478 02a8       moveal 0x2a8,%a2
   e4a42:       4ab8 02a8       tstl 0x2a8
   e4a46:       660a            bnes 0xe4a52
   e4a48:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
   e4a4e:       6000 0126       braw 0xe4b76
   e4a52:       526a 000c       addqw #1,%a2@(12)
   e4a56:       4a2a 0010       tstb %a2@(16)
   e4a5a:       670e            beqs 0xe4a6a
   e4a5c:       422a 0010       clrb %a2@(16)
   e4a60:       0078 0010 0250  oriw #16,0x250
   e4a66:       4e4f            trap #15
   e4a68:       a130            0120460
   e4a6a:       264a            moveal %a2,%a3
   e4a6c:       47eb 004e       lea %a3@(78),%a3
   e4a70:       704e            moveq #78,%d0
   e4a72:       4a72 0820       tstw %a2@(00000020,%d0:l)
   e4a76:       660a            bnes 0xe4a82
   e4a78:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
   e4a7e:       6000 00f6       braw 0xe4b76
   e4a82:       426e fffc       clrw %fp@(-4)
   e4a86:       600e            bras 0xe4a96
   e4a88:       302e fffe       movew %fp@(-2),%d0
   e4a8c:       322b 003c       movew %a3@(60),%d1
   e4a90:       b340            eorw %d1,%d0
   e4a92:       816e fffc       orw %d0,%fp@(-4)
   e4a96:       486e fffe       pea %fp@(-2)
   e4a9a:       2f0b            movel %a3,%sp@-
   e4a9c:       2f0a            movel %a2,%sp@-
   e4a9e:       4eba fb8c       jsr %pc@(0xe462c)                             HwrSpiSdioInterrupts(&local2, %a3, %a2);
   e4aa2:       4a40            tstw %d0
   e4aa4:       4fef 000c       lea %sp@(12),%sp
   e4aa8:       66de            bnes 0xe4a88
   e4aaa:       302e fffe       movew %fp@(-2),%d0                            %d0 = local2;
   e4aae:       322b 003c       movew %a3@(60),%d1                            $d1 = %a3+60;
   e4ab2:       b340            eorw %d1,%d0                                  %d0 ^= d1;
   e4ab4:       816e fffc       orw %d0,%fp@(-4)                              local4 |= %d0;
   e4ab8:       376e fffe 003c  movew %fp@(-2),%a3@(60)                       %a3+60 = local2;
   e4abe:       082e 0004 fffc  btst #4,%fp@(-4)                              if (local4 & 0x1000 == 0) goto 0xe4b38;
   e4ac4:       6772            beqs 0xe4b38
   e4ac6:       082b 0003 003e  btst #3,%a3@(62)
   e4acc:       6710            beqs 0xe4ade
   e4ace:       082b 0004 003c  btst #4,%a3@(60)                              if ((%a3+60 & 0x1000) && (%a3+62 & 0x0800))
   e4ad4:       6608            bnes 0xe4ade
   e4ad6:       42a7            clrl %sp@-
   e4ad8:       4eba 00b2       jsr %pc@(0xe4b8c)                                PrvSpiIspCardDebounced(0);
   e4adc:       584f            addqw #4,%sp
   e4ade:       4aaa 0002       tstl %a2@(2)
   e4ae2:       670c            beqs 0xe4af0
   e4ae4:       7064            moveq #100,%d0
   e4ae6:       d0b8 0254       addl 0x254,%d0
   e4aea:       2740 0018       movel %d0,%a3@(24)
   e4aee:       6048            bras 0xe4b38
   e4af0:       7605            moveq #5,%d3
   e4af2:       2f3c 0003 0d40  movel #200000,%sp@-
   e4af8:       4e4f            trap #15
   e4afa:       a249            0121111
   e4afc:       486e fffe       pea %fp@(-2)
   e4b00:       4eba e73a       jsr %pc@(0xe323c)
   e4b04:       264a            moveal %a2,%a3
   e4b06:       47eb 004e       lea %a3@(78),%a3
   e4b0a:       302e fffe       movew %fp@(-2),%d0
   e4b0e:       322b 003c       movew %a3@(60),%d1
   e4b12:       b340            eorw %d1,%d0
   e4b14:       3d40 fffc       movew %d0,%fp@(-4)
   e4b18:       376e fffe 003c  movew %fp@(-2),%a3@(60)
   e4b1e:       5343            subqw #1,%d3
   e4b20:       082e 0004 fffc  btst #4,%fp@(-4)
   e4b26:       504f            addqw #8,%sp
   e4b28:       6702            beqs 0xe4b2c
   e4b2a:       7605            moveq #5,%d3
   e4b2c:       4a43            tstw %d3
   e4b2e:       66c2            bnes 0xe4af2
   e4b30:       42a7            clrl %sp@-
   e4b32:       4eba 0058       jsr %pc@(0xe4b8c)
   e4b36:       584f            addqw #4,%sp
   e4b38:       082e 0006 fffc  btst #6,%fp@(-4)
   e4b3e:       6732            beqs 0xe4b72
   e4b40:       264a            moveal %a2,%a3
   e4b42:       47eb 004e       lea %a3@(78),%a3
   e4b46:       7600            moveq #0,%d3
   e4b48:       6022            bras 0xe4b6c
   e4b4a:       4a6b 0020       tstw %a3@(32)
   e4b4e:       6716            beqs 0xe4b66
   e4b50:       082b 0007 003e  btst #7,%a3@(62)
   e4b56:       670e            beqs 0xe4b66
   e4b58:       286b 000c       moveal %a3@(12),%a4
   e4b5c:       200c            movel %a4,%d0
   e4b5e:       6706            beqs 0xe4b66
   e4b60:       3f03            movew %d3,%sp@-
   e4b62:       4e94            jsr %a4@
```

# HwrSpiSdioInterrupts(&LOCAL2, %A3, %A2)

```
   e462c:       4e56 fffe       linkw %fp,#-2
   e4630:       48e7 0038       moveml %a2-%a4,%sp@-
   e4634:       246e 0008       moveal %fp@(8),%a2                         %a2 = %A2
   e4638:       286e 000c       moveal %fp@(12),%a4                        %a4 = %A3
   e463c:       266e 0010       moveal %fp@(16),%a3                        %a3 = &LOCAL2
   e4640:       486e fffe       pea %fp@(-2)
   e4644:       4eba ebf6       jsr %pc@(0xe323c)                          PrvSpiSetPol2(&local2);
   e4648:       36ae fffe       movew %fp@(-2),%a3@                        LOCAL2 = local2;
   e464c:       0813 0006       btst #6,%a3@
   e4650:       584f            addqw #4,%sp
   e4652:       6646            bnes 0xe469a
   e4654:       4a2a 0011       tstb %a2@(17)
   e4658:       6740            beqs 0xe469a
   e465a:       4a6a 000a       tstw %a2@(10)
   e465e:       663a            bnes 0xe469a
   e4660:       4a6c 001c       tstw %a4@(28)
   e4664:       6634            bnes 0xe469a
   e4666:       082a 0007 001c  btst #7,%a2@(28)
   e466c:       672c            beqs 0xe469a
   e466e:       0813 0004       btst #4,%a3@
   e4672:       6726            beqs 0xe469a
   e4674:       526a 000a       addqw #1,%a2@(10)
   e4678:       4267            clrw %sp@-
   e467a:       2f0c            movel %a4,%sp@-
   e467c:       4eba eb14       jsr %pc@(0xe3192)
   e4680:       536a 000a       subqw #1,%a2@(10)
   e4684:       4a6a 000a       tstw %a2@(10)
   e4688:       5c4f            addqw #6,%sp
   e468a:       6c04            bges 0xe4690
   e468c:       426a 000a       clrw %a2@(10)
   e4690:       4a2a 0011       tstb %a2@(17)
   e4694:       6704            beqs 0xe469a
   e4696:       7001            moveq #1,%d0
   e4698:       6002            bras 0xe469c
   e469a:       7000            moveq #0,%d0
   e469c:       4cdf 1c00       moveml %sp@+,%a2-%a4
   e46a0:       4e5e            unlk %fp
   e46a2:       4e75            rts
```

# PrvSpiSetPol2(&LOCAL2)

```
   e323c:       4e56 fff8       linkw %fp,#-8
   e3240:       2f04            movel %d4,%sp@-
   e3242:       2f03            movel %d3,%sp@-
   e3244:       7809            moveq #9,%d4
   e3246:       7600            moveq #0,%d3
   e3248:       426e fff8       clrw %fp@(-8)
   e324c:       1038 f419       moveb 0xfffffffffffff419,%d0
   e3250:       0240 0020       andiw #32,%d0
   e3254:       6706            beqs 0xe325c
   e3256:       002e 0080 fff8  orib #-128,%fp@(-8)
   e325c:       1038 f421       moveb 0xfffffffffffff421,%d0
   e3260:       0240 0080       andiw #128,%d0
   e3264:       6706            beqs 0xe326c
   e3266:       002e 0040 fff8  orib #64,%fp@(-8)
   e326c:       3d6e fff8 fffa  movew %fp@(-8),%fp@(-6)
   e3272:       082e 0007 fffa  btst #7,%fp@(-6)
   e3278:       6708            beqs 0xe3282
   e327a:       0278 bfff f302  andiw #-16385,0xfffffffffffff302
   e3280:       6006            bras 0xe3288
   e3282:       0078 4000 f302  oriw #16384,0xfffffffffffff302
   e3288:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
   e328e:       4878 00fa       pea 0xfa
   e3292:       4e4f            trap #15
   e3294:       a249            0121111
   e3296:       426e fffc       clrw %fp@(-4)
   e329a:       1038 f419       moveb 0xfffffffffffff419,%d0
   e329e:       0240 0020       andiw #32,%d0
   e32a2:       584f            addqw #4,%sp
   e32a4:       6706            beqs 0xe32ac
   e32a6:       002e 0080 fffc  orib #-128,%fp@(-4)
   e32ac:       1038 f421       moveb 0xfffffffffffff421,%d0
   e32b0:       0240 0080       andiw #128,%d0
   e32b4:       6706            beqs 0xe32bc
   e32b6:       002e 0040 fffc  orib #64,%fp@(-4)
   e32bc:       3d6e fffc fffe  movew %fp@(-4),%fp@(-2)
   e32c2:       302e fffa       movew %fp@(-6),%d0
   e32c6:       b06e fffe       cmpw %fp@(-2),%d0
   e32ca:       6636            bnes 0xe3302
   e32cc:       3038 f30c       movew 0xfffffffffffff30c,%d0
   e32d0:       8078 f310       orw 0xfffffffffffff310,%d0
   e32d4:       0240 0002       andiw #2,%d0
   e32d8:       6628            bnes 0xe3302
   e32da:       082e 0007 fffe  btst #7,%fp@(-2)
   e32e0:       670c            beqs 0xe32ee
   e32e2:       3038 f302       movew 0xfffffffffffff302,%d0
   e32e6:       0240 4000       andiw #16384,%d0
   e32ea:       6616            bnes 0xe3302
   e32ec:       600a            bras 0xe32f8
   e32ee:       3038 f302       movew 0xfffffffffffff302,%d0
   e32f2:       0240 4000       andiw #16384,%d0
   e32f6:       670a            beqs 0xe3302
   e32f8:       5203            addqb #1,%d3
   e32fa:       0c03 0003       cmpib #3,%d3
   e32fe:       6440            bccs 0xe3340
   e3300:       6024            bras 0xe3326
   e3302:       082e 0007 fffe  btst #7,%fp@(-2)
   e3308:       6708            beqs 0xe3312
   e330a:       0278 bfff f302  andiw #-16385,0xfffffffffffff302
   e3310:       6006            bras 0xe3318
   e3312:       0078 4000 f302  oriw #16384,0xfffffffffffff302
   e3318:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
   e331e:       7600            moveq #0,%d3
   e3320:       3d6e fffe fffa  movew %fp@(-2),%fp@(-6)
   e3326:       5304            subqb #1,%d4
   e3328:       6600 ff64       bnew 0xe328e
   e332c:       3038 f30c       movew 0xfffffffffffff30c,%d0
   e3330:       8078 f310       orw 0xfffffffffffff310,%d0
   e3334:       0240 0002       andiw #2,%d0
   e3338:       6706            beqs 0xe3340
   e333a:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
   e3340:       7600            moveq #0,%d3
   e3342:       082e 0007 fffe  btst #7,%fp@(-2)
   e3348:       6704            beqs 0xe334e
   e334a:       0603 0080       addib #-128,%d3
   e334e:       082e 0006 fffe  btst #6,%fp@(-2)
   e3354:       6704            beqs 0xe335a
   e3356:       0603 0080       addib #-128,%d3
   e335a:       4a03            tstb %d3
   e335c:       6606            bnes 0xe3364
   e335e:       002e 0010 fffe  orib #16,%fp@(-2)                                   local2 |= 0x1000;
   e3364:       206e 0008       moveal %fp@(8),%a0
   e3368:       30ae fffe       movew %fp@(-2),%a0@                                 LOCAL2 = local2;
   e336c:       261f            movel %sp@+,%d3
   e336e:       281f            movel %sp@+,%d4
   e3370:       4e5e            unlk %fp
   e3372:       4e75            rts
```
