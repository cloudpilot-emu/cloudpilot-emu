# IspSpiCard0

```
  108310:       4e56 fffc       linkw %fp,#-4
  108314:       48e7 1038       moveml %d3/%a2-%a4,%sp@-
  108318:       2678 02a8       moveal 0x2a8,%a3
  10831c:       4ab8 02a8       tstl 0x2a8
  108320:       660a            bnes 0x10832c
  108322:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
  108328:       6000 0136       braw 0x108460
  10832c:       526b 000c       addqw #1,%a3@(12)
  108330:       4a2b 0010       tstb %a3@(16)
  108334:       670e            beqs 0x108344
  108336:       422b 0010       clrb %a3@(16)
  10833a:       0078 0010 0250  oriw #16,0x250
  108340:       4e4f            trap #15
  108342:       a130            0120460
  108344:       244b            moveal %a3,%a2
  108346:       45ea 004e       lea %a2@(78),%a2
  10834a:       704e            moveq #78,%d0
  10834c:       4a73 0820       tstw %a3@(00000020,%d0:l)
  108350:       660a            bnes 0x10835c
  108352:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
  108358:       6000 0106       braw 0x108460
  10835c:       426e fffc       clrw %fp@(-4)
  108360:       486e fffe       pea %fp@(-2)
  108364:       2f0a            movel %a2,%sp@-
  108366:       2f0b            movel %a3,%sp@-
  108368:       4eba fbd0       jsr %pc@(0x107f3a)                      if (HwrSpiSdioInterrupts(&local2, %a2, %a3)) {
  10836c:       4a40            tstw %d0
  10836e:       4fef 000c       lea %sp@(12),%sp
  108372:       6722            beqs 0x108396
  108374:       302e fffe       movew %fp@(-2),%d0
  108378:       322a 003c       movew %a2@(60),%d1
  10837c:       b340            eorw %d1,%d0
  10837e:       816e fffc       orw %d0,%fp@(-4)
  108382:       486e fffe       pea %fp@(-2)
  108386:       2f0a            movel %a2,%sp@-
  108388:       2f0b            movel %a3,%sp@-
  10838a:       4eba fbae       jsr %pc@(0x107f3a)                          HwrSpiSdioInterrupts(&local2, %a2, %a3);
  10838e:       4a40            tstw %d0
  108390:       4fef 000c       lea %sp@(12),%sp
  108394:       66de            bnes 0x108374                           }
  108396:       302e fffe       movew %fp@(-2),%d0                      %d0 = local2;
  10839a:       322a 003c       movew %a2@(60),%d1                      %d1 = %a2+60
  10839e:       b340            eorw %d1,%d0                            %d0 ^= %d1;
  1083a0:       816e fffc       orw %d0,%fp@(-4)                        local4 |= %d0; -> local4 = local2 ^ %a2+60;
  1083a4:       356e fffe 003c  movew %fp@(-2),%a2@(60)                 %a2+60 = local2;
  1083aa:       082e 0004 fffc  btst #4,%fp@(-4)
  1083b0:       6772            beqs 0x108424
  1083b2:       082a 0003 003e  btst #3,%a2@(62)
  1083b8:       6710            beqs 0x1083ca
  1083ba:       082a 0004 003c  btst #4,%a2@(60)
  1083c0:       6608            bnes 0x1083ca
  1083c2:       42a7            clrl %sp@-
  1083c4:       4eba 00b0       jsr %pc@(0x108476)
  1083c8:       584f            addqw #4,%sp
  1083ca:       4aab 0002       tstl %a3@(2)
  1083ce:       670c            beqs 0x1083dc
  1083d0:       7064            moveq #100,%d0
  1083d2:       d0b8 0254       addl 0x254,%d0
  1083d6:       2540 0018       movel %d0,%a2@(24)
  1083da:       6048            bras 0x108424
  1083dc:       7605            moveq #5,%d3
  1083de:       2f3c 0003 0d40  movel #200000,%sp@-
  1083e4:       4e4f            trap #15
  1083e6:       a249            0121111
  1083e8:       486e fffe       pea %fp@(-2)
  1083ec:       4eba e73c       jsr %pc@(0x106b2a)
  1083f0:       244b            moveal %a3,%a2
  1083f2:       45ea 004e       lea %a2@(78),%a2
  1083f6:       302e fffe       movew %fp@(-2),%d0
  1083fa:       322a 003c       movew %a2@(60),%d1
  1083fe:       b340            eorw %d1,%d0
  108400:       3d40 fffc       movew %d0,%fp@(-4)
  108404:       356e fffe 003c  movew %fp@(-2),%a2@(60)
  10840a:       5343            subqw #1,%d3
  10840c:       082e 0004 fffc  btst #4,%fp@(-4)
  108412:       504f            addqw #8,%sp
  108414:       6702            beqs 0x108418
  108416:       7605            moveq #5,%d3
  108418:       4a43            tstw %d3
  10841a:       66c2            bnes 0x1083de
  10841c:       42a7            clrl %sp@-
  10841e:       4eba 0056       jsr %pc@(0x108476)
  108422:       584f            addqw #4,%sp
  108424:       082e 0006 fffc  btst #6,%fp@(-4)
  10842a:       6730            beqs 0x10845c
  10842c:       244b            moveal %a3,%a2
  10842e:       45ea 004e       lea %a2@(78),%a2
  108432:       7600            moveq #0,%d3
  108434:       4a6a 0020       tstw %a2@(32)
  108438:       6716            beqs 0x108450
  10843a:       082a 0007 003e  btst #7,%a2@(62)
  108440:       670e            beqs 0x108450
  108442:       286a 000c       moveal %a2@(12),%a4
  108446:       200c            movel %a4,%d0
  108448:       6706            beqs 0x108450
  10844a:       3f03            movew %d3,%sp@-
  10844c:       4e94            jsr %a4@
```

# HwrSpiSdioInterrupts(&LOCAL2, %A3, %A2)

```
  107f3a:       4e56 fffe       linkw %fp,#-2
  107f3e:       48e7 0038       moveml %a2-%a4,%sp@-
  107f42:       246e 0008       moveal %fp@(8),%a2
  107f46:       286e 000c       moveal %fp@(12),%a4
  107f4a:       266e 0010       moveal %fp@(16),%a3
  107f4e:       486e fffe       pea %fp@(-2)
  107f52:       4eba ebd6       jsr %pc@(0x106b2a)
  107f56:       36ae fffe       movew %fp@(-2),%a3@
  107f5a:       0813 0006       btst #6,%a3@
  107f5e:       584f            addqw #4,%sp
  107f60:       6646            bnes 0x107fa8
  107f62:       4a2a 0011       tstb %a2@(17)
  107f66:       6740            beqs 0x107fa8
  107f68:       4a6a 000a       tstw %a2@(10)
  107f6c:       663a            bnes 0x107fa8
  107f6e:       4a6c 001c       tstw %a4@(28)
  107f72:       6634            bnes 0x107fa8
  107f74:       082a 0007 001c  btst #7,%a2@(28)
  107f7a:       672c            beqs 0x107fa8
  107f7c:       0813 0004       btst #4,%a3@
  107f80:       6726            beqs 0x107fa8
  107f82:       526a 000a       addqw #1,%a2@(10)
  107f86:       4267            clrw %sp@-
  107f88:       2f0c            movel %a4,%sp@-
  107f8a:       4eba eaf4       jsr %pc@(0x106a80)
  107f8e:       536a 000a       subqw #1,%a2@(10)
  107f92:       4a6a 000a       tstw %a2@(10)
  107f96:       5c4f            addqw #6,%sp
  107f98:       6c04            bges 0x107f9e
  107f9a:       426a 000a       clrw %a2@(10)
  107f9e:       4a2a 0011       tstb %a2@(17)
  107fa2:       6704            beqs 0x107fa8
  107fa4:       7001            moveq #1,%d0
  107fa6:       6002            bras 0x107faa
  107fa8:       7000            moveq #0,%d0
  107faa:       4cdf 1c00       moveml %sp@+,%a2-%a4
  107fae:       4e5e            unlk %fp
  107fb0:       4e75            rts
```

# PrvSpiSetPol2(&LOCAL2)

```
  106b2a:       4e56 fff8       linkw %fp,#-8
  106b2e:       2f04            movel %d4,%sp@-
  106b30:       2f03            movel %d3,%sp@-
  106b32:       7809            moveq #9,%d4                            %d4 = 9;
  106b34:       7600            moveq #0,%d3                            %d3 = 0
  106b36:       426e fff8       clrw %fp@(-8)                           %local8 = 0;
  106b3a:       1038 f419       moveb 0xfffffffffffff419,%d0            %d0 = pDdata;
  106b3e:       0240 0020       andiw #32,%d0                           %d0 &= 0x0010:
  106b42:       6706            beqs 0x106b4a                           if ((%d0 & 0x0010) != 0)
  106b44:       002e 0080 fff8  orib #-128,%fp@(-8)                         local8 |= 0x8000;
  106b4a:       3039 1080 004e  movew 0x1080004e,%d0                    %d0 = clpd_4e;
  106b50:       0240 0080       andiw #128,%d0                          %d0 &= 0x0080;
  106b54:       6706            beqs 0x106b5c                           if ((%d0 & 0x0080) != 0)
  106b56:       002e 0040 fff8  orib #64,%fp@(-8)                           local8 |= 0x4000;
  106b5c:       1038 f441       moveb 0xfffffffffffff441,%d0            %d0 = pKdata;
  106b60:       0240 0001       andiw #1,%d0                            %d0 &= 0x0001;
  106b64:       6706            beqs 0x106b6c
  106b66:       002e 0020 fff8  orib #32,%fp@(-8)
  106b6c:       3d6e fff8 fffa  movew %fp@(-8),%fp@(-6)
  106b72:       082e 0007 fffa  btst #7,%fp@(-6)
  106b78:       6708            beqs 0x106b82
  106b7a:       0278 bfff f302  andiw #-16385,0xfffffffffffff302
  106b80:       6006            bras 0x106b88
  106b82:       0078 4000 f302  oriw #16384,0xfffffffffffff302
  106b88:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
  106b8e:       4878 00fa       pea 0xfa
  106b92:       4e4f            trap #15
  106b94:       a249            0121111
  106b96:       426e fffc       clrw %fp@(-4)
  106b9a:       1038 f419       moveb 0xfffffffffffff419,%d0
  106b9e:       0240 0020       andiw #32,%d0
  106ba2:       584f            addqw #4,%sp
  106ba4:       6706            beqs 0x106bac
  106ba6:       002e 0080 fffc  orib #-128,%fp@(-4)
  106bac:       3039 1080 004e  movew 0x1080004e,%d0
  106bb2:       0240 0080       andiw #128,%d0
  106bb6:       6706            beqs 0x106bbe
  106bb8:       002e 0040 fffc  orib #64,%fp@(-4)
  106bbe:       1038 f441       moveb 0xfffffffffffff441,%d0
  106bc2:       0240 0001       andiw #1,%d0
  106bc6:       6706            beqs 0x106bce
  106bc8:       002e 0020 fffc  orib #32,%fp@(-4)
  106bce:       3d6e fffc fffe  movew %fp@(-4),%fp@(-2)
  106bd4:       302e fffa       movew %fp@(-6),%d0
  106bd8:       b06e fffe       cmpw %fp@(-2),%d0
  106bdc:       6636            bnes 0x106c14
  106bde:       3038 f30c       movew 0xfffffffffffff30c,%d0
  106be2:       8078 f310       orw 0xfffffffffffff310,%d0
  106be6:       0240 0002       andiw #2,%d0
  106bea:       6628            bnes 0x106c14
  106bec:       082e 0007 fffe  btst #7,%fp@(-2)
  106bf2:       670c            beqs 0x106c00
  106bf4:       3038 f302       movew 0xfffffffffffff302,%d0
  106bf8:       0240 4000       andiw #16384,%d0
  106bfc:       6616            bnes 0x106c14
  106bfe:       600a            bras 0x106c0a
  106c00:       3038 f302       movew 0xfffffffffffff302,%d0
  106c04:       0240 4000       andiw #16384,%d0
  106c08:       670a            beqs 0x106c14
  106c0a:       5203            addqb #1,%d3
  106c0c:       0c03 0003       cmpib #3,%d3
  106c10:       6440            bccs 0x106c52
  106c12:       6024            bras 0x106c38
  106c14:       082e 0007 fffe  btst #7,%fp@(-2)
  106c1a:       6708            beqs 0x106c24
  106c1c:       0278 bfff f302  andiw #-16385,0xfffffffffffff302
  106c22:       6006            bras 0x106c2a
  106c24:       0078 4000 f302  oriw #16384,0xfffffffffffff302
  106c2a:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
  106c30:       7600            moveq #0,%d3
  106c32:       3d6e fffe fffa  movew %fp@(-2),%fp@(-6)
  106c38:       5304            subqb #1,%d4
  106c3a:       6600 ff52       bnew 0x106b8e
  106c3e:       3038 f30c       movew 0xfffffffffffff30c,%d0
  106c42:       8078 f310       orw 0xfffffffffffff310,%d0
  106c46:       0240 0002       andiw #2,%d0
  106c4a:       6706            beqs 0x106c52
  106c4c:       0078 0002 f30c  oriw #2,0xfffffffffffff30c
  106c52:       7600            moveq #0,%d3
  106c54:       082e 0007 fffe  btst #7,%fp@(-2)
  106c5a:       6704            beqs 0x106c60
  106c5c:       0603 0080       addib #-128,%d3
  106c60:       082e 0006 fffe  btst #6,%fp@(-2)
  106c66:       6704            beqs 0x106c6c
  106c68:       0603 0080       addib #-128,%d3
  106c6c:       4a03            tstb %d3
  106c6e:       6606            bnes 0x106c76
  106c70:       002e 0010 fffe  orib #16,%fp@(-2)
  106c76:       206e 0008       moveal %fp@(8),%a0
  106c7a:       30ae fffe       movew %fp@(-2),%a0@
  106c7e:       261f            movel %sp@+,%d3
  106c80:       281f            movel %sp@+,%d4
  106c82:       4e5e            unlk %fp
  106c84:       4e75            rts
```
