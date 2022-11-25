/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2000-2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmTRGCFDefs_h
#define EmTRGCFDefs_h

#include "EmCommon.h"

#define IDE_REG_0_DATA 0x0
#define IDE_REG_1_ERROR 0x1
#define IDE_REG_1_FEATURES 0x1
#define IDE_REG_2_SECTOR_CNT 0x2
#define IDE_REG_3_LBA_7_0 0x3
#define IDE_REG_4_LBA_15_8 0x4
#define IDE_REG_5_LBA_23_16 0x5
#define IDE_REG_6_DRV_HEAD 0x6
#define IDE_REG_7_STATUS 0x7
#define IDE_REG_7_COMMAND 0x7
#define IDE_REG_8_DATA_EVEN 0x8
#define IDE_REG_9_DATA_ODD 0x9
#define IDE_REG_A_UNUSED 0xA
#define IDE_REG_B_UNUSED 0xB
#define IDE_REG_C_UNUSED 0xC
#define IDE_REG_D_ERROR 0xD
#define IDE_REG_E_ALT_STATUS 0xE
#define IDE_REG_E_DEVICE_CONTROL 0xE
#define IDE_REG_F_DRIVE_ADDR 0xF

#define NUM_IDE_REGS (IDE_REG_F_DRIVE_ADDR + 1)

#define SECTOR_SIZE 512

#define IDE_CMD_03_REQUEST_SENSE 0x03
#define IDE_CMD_10_RECALIBRATE 0x10
#define IDE_CMD_11_RECALIBRATE 0x11
#define IDE_CMD_12_RECALIBRATE 0x12
#define IDE_CMD_13_RECALIBRATE 0x13
#define IDE_CMD_14_RECALIBRATE 0x14
#define IDE_CMD_15_RECALIBRATE 0x15
#define IDE_CMD_16_RECALIBRATE 0x16
#define IDE_CMD_17_RECALIBRATE 0x17
#define IDE_CMD_18_RECALIBRATE 0x18
#define IDE_CMD_19_RECALIBRATE 0x19
#define IDE_CMD_1A_RECALIBRATE 0x1A
#define IDE_CMD_1B_RECALIBRATE 0x1B
#define IDE_CMD_1C_RECALIBRATE 0x1C
#define IDE_CMD_1D_RECALIBRATE 0x1D
#define IDE_CMD_1E_RECALIBRATE 0x1E
#define IDE_CMD_1F_RECALIBRATE 0x1F
#define IDE_CMD_20_READ_SECTORS 0x20
#define IDE_CMD_21_READ_SECTORS 0x21
#define IDE_CMD_22_READ_LONG_SECTOR 0x22
#define IDE_CMD_23_READ_LONG_SECTOR 0x23
#define IDE_CMD_30_WRITE_SECTORS 0x30
#define IDE_CMD_31_WRITE_SECTORS 0x31
#define IDE_CMD_32_WRITE_LONG_SECTOR 0x32
#define IDE_CMD_33_WRITE_LONG_SECTOR 0x33
#define IDE_CMD_38_WRITE_SECTORS_WO_ERASE 0x38
#define IDE_CMD_3C_WRITE_VERIFY 0x3C
#define IDE_CMD_40_READ_VERIFY_SECTORS 0x40
#define IDE_CMD_41_READ_VERIFY_SECTORS 0x41
#define IDE_CMD_50_FORMAT_TRACK 0x50
#define IDE_CMD_70_SEEK 0x70
#define IDE_CMD_71_SEEK 0x71
#define IDE_CMD_72_SEEK 0x72
#define IDE_CMD_73_SEEK 0x73
#define IDE_CMD_74_SEEK 0x74
#define IDE_CMD_75_SEEK 0x75
#define IDE_CMD_76_SEEK 0x76
#define IDE_CMD_77_SEEK 0x77
#define IDE_CMD_78_SEEK 0x78
#define IDE_CMD_79_SEEK 0x79
#define IDE_CMD_7A_SEEK 0x7A
#define IDE_CMD_7B_SEEK 0x7B
#define IDE_CMD_7C_SEEK 0x7C
#define IDE_CMD_7D_SEEK 0x7D
#define IDE_CMD_7E_SEEK 0x7E
#define IDE_CMD_7F_SEEK 0x7F
#define IDE_CMD_87_TRANSLATE_SECTOR 0x87
#define IDE_CMD_90_EXEC_DRIVE_DIAGNOSTIC 0x90
#define IDE_CMD_91_INIT_DRIVE_PARAMS 0x91
#define IDE_CMD_94_STANDBY_IMMEDIATE 0x94
#define IDE_CMD_95_IDLE_IMMEDIATE 0x95
#define IDE_CMD_96_STANDBY 0x96
#define IDE_CMD_97_IDLE 0x97
#define IDE_CMD_98_CHECK_POWER_MODE 0x98
#define IDE_CMD_99_SET_SLEEP_MODE 0x99
#define IDE_CMD_C0_ERASE_SECTOR 0xC0
#define IDE_CMD_C4_READ_MULTIPLE 0xC4
#define IDE_CMD_C6_SET_MULTIPLE_MODE 0xC6
#define IDE_CMD_CD_WRITE_MULT_WO_ERASE 0xCD
#define IDE_CMD_E0_STANDBY_IMMEDIATE 0xE0
#define IDE_CMD_E1_IDLE_IMMEDIATE 0xE1
#define IDE_CMD_E2_STANDBY 0xE2
#define IDE_CMD_E3_IDLE 0xE3
#define IDE_CMD_E4_READ_BUFFER 0xE4
#define IDE_CMD_E5_CHECK_POWER_MODE 0xE5
#define IDE_CMD_E6_SET_SLEEP_MODE 0xE6
#define IDE_CMD_E8_WRITE_BUFFER 0xE8
#define IDE_CMD_EC_IDENTIFY_DRIVE 0xEC
#define IDE_CMD_EF_SET_FEATURES 0xEF
#define IDE_CMD_F1_SECUR_SET_PASSWORD 0xF1
#define IDE_CMD_F2_SECUR_UNLOCK 0xF2
#define IDE_CMD_F3_SECUR_ERASE_PREPARE 0xF3
#define IDE_CMD_F4_SECUR_ERASE_UNIT 0xF4
#define IDE_CMD_F5_SECUR_FREEZE_LOCK 0xF5
#define IDE_CMD_F6_SECUR_DISABLE_PASSWORD 0xF6

#define IDE_STS_BUSY 0x80
#define IDE_STS_RDY 0x40
#define IDE_STS_DWF 0x20
#define IDE_STS_DSC 0x10
#define IDE_STS_DRQ 0x08
#define IDE_STS_CORR 0x04
#define IDE_STS_IDX 0x02
#define IDE_STS_ERR 0x01

#define IDE_ERR_10_SECTOR_NOT_FOUND 0x10
#define IDE_ERR_04_ABORT 0x04
#define IDE_ERR_01_GENERAL_ERROR 0x01
#define IDE_ERR_00_NONE 0x00

#define IDE_DIA_80_SLAVE_ERR 0x80
#define IDE_DIA_05_CPU_ERR 0x05
#define IDE_DIA_04_ECC_CIRCUITRY_ERR 0x04
#define IDE_DIA_03_SECTOR_BUFFER_ERR 0x03
#define IDE_DIA_02_FORMAT_ERR 0x02
#define IDE_DIA_01_NO_ERROR 0x01

typedef struct {
    uint8 Bytes[SECTOR_SIZE];
} EmSector;

typedef uint8 DiskStatus;
typedef uint32 LogicalBlockAddr;

class SessionFile;

typedef struct {
    uint8 Bytes[2];
} _Word;

typedef enum { CF_MEM_TUPLE, CF_MEM_CONFIG, CF_MEM_ATA, CF_MEM_RESET } CFMemSection;

const uint32 kMemoryStartCF = 0x18000000;

const uint32 kMemoryOffsetCFTuple = 0;
const uint32 kMemoryStartCFTuple = kMemoryStartCF + kMemoryOffsetCFTuple;
const uint32 kMemorySizeCFTuple = 0x200;

const uint32 kMemoryOffsetCFConfig = kMemoryOffsetCFTuple + kMemorySizeCFTuple;
const uint32 kMemoryStartCFConfig = kMemoryStartCF + kMemoryOffsetCFConfig;
const uint32 kMemorySizeCFConfig = 0xE00;

const uint32 kMemoryOffsetCFAta = kMemoryOffsetCFConfig + kMemorySizeCFConfig;
const uint32 kMemoryStartCFAta = kMemoryStartCF + kMemoryOffsetCFAta;
const uint32 kMemorySizeCFAta = 0x1000;

const uint32 kMemoryOffsetCFReset = kMemoryOffsetCFAta + kMemorySizeCFAta;
const uint32 kMemoryStartCFReset = kMemoryStartCF + kMemoryOffsetCFReset;
const uint32 kMemorySizeCFReset = 0x200;

const uint32 kMemorySizeCF = kMemoryOffsetCFReset + kMemorySizeCFReset;

#include "EmHandEraCFBus.h"
#include "EmRegs.h"

#endif  // EmTRGCFDefs_h
