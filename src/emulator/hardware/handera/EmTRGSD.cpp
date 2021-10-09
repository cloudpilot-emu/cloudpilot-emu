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

#include "EmTRGSD.h"

#include "EmCommon.h"

/************************************************************************
 * This class handles the SPI SD controller interface used in the
 * HandEra 330.
 ************************************************************************/

// SPI SD commands
#define SD_CMD0_GO_IDLE 0
#define SD_CMD1_GO_OPERATIONAL 1
#define SD_CMD9_SEND_CSD 9
#define SD_CMD10_SEND_CID 10
#define SD_CMD12_STOP_TRANS 12  // not supported
#define SD_CMD13_SEND_STATUS 13
#define SD_CMD16_SET_BLOCKLEN 16  // not supported
#define SD_CMD17_READ_SECTOR 17
#define SD_CMD18_READ_MULTI 18  // not supported
#define SD_CMD24_WRITE_SECTOR 24
#define SD_CMD25_WRITE_MULTI 25         // not supported
#define SD_CMD27_PROGRAM_CSD 27         // not supported
#define SD_CMD28_SET_WRITE_PROT 28      // not supported
#define SD_CMD29_CLR_WRITE_PROT 29      // not supported
#define SD_CMD30_SEND_WRITE_PROT 30     // not supported
#define SD_CMD32_ERASE_WR_BLK_START 32  // not supported
#define SD_CMD33_ERASE_WR_BLK_END 33    // not supported
#define SD_CMD38_ERASE 38               // not supported
#define SD_CMD42_LOCK_UNLOCK 42         // not supported
#define SD_CMD55_APP_CMD 55             // not supported
#define SD_CMD56_GEN_CMD 56             // not supported
#define SD_CMD58_READ_OCR 58            // not supported
#define SD_CMD59_CRC_ON_OFF 59          // not supported

#define SD_CMD_SIZE 6  // in bytes
#define SD_CSD_SIZE 16
#define SD_CID_SIZE 16

#define SD_R1_OP_STATE 0x00
#define SD_R1_IDLE_STATE 0x01
#define SD_R1_ILLEGAL_CMD 0x04

#define SD_STATUS_OK 0x00
#define SD_DATA_START 0xFE

#define SD_DATA_OK 0xE5

// ---------------------------------------------------------------------------
//		� EmTRGSD::EmTRGSD
// ---------------------------------------------------------------------------
EmTRGSD::EmTRGSD() {
    cmdIndex = 0;
    sdBusState = sdBusGetCmdState;
}

// ---------------------------------------------------------------------------
//		� EmTRGSD::~EmTRGSD
// ---------------------------------------------------------------------------
EmTRGSD::~EmTRGSD() {}

// ---------------------------------------------------------------------------
//		� EmTRGSD::Initialize
// ---------------------------------------------------------------------------
void EmTRGSD::Initialize(void) { DiskIO.Initialize(SD_DRIVE); }

// ---------------------------------------------------------------------------
//		� EmTRGSD::Dispose
// ---------------------------------------------------------------------------
void EmTRGSD::Dispose(void) { DiskIO.Dispose(); }

// ---------------------------------------------------------------------------
//		� EmTRGSD::CompleteCommand
// complete command processing now that we have the data for it too
// ---------------------------------------------------------------------------
void EmTRGSD::CompleteCommand(void) {
    cmdIndex = 0;
    dataIndex = 0;
    sdBusState = sdBusSendResponseState;
    sdBusNextState = sdBusGetCmdState;
    responseIndex = 0;

    switch (cmdBuffer[0] & 0x3f) {
        case SD_CMD24_WRITE_SECTOR:
            DiskIO.WriteSector(sectorNum, &dataBuffer[1]);

            // send data OK response
            responseBuffer[0] = SD_DATA_OK;
            responseSize = 1;
            break;

        default:
            // unimplemented, should never get here
            EmAssert(false);
            sdBusState = sdBusGetCmdState;
            break;
    }
}

// ---------------------------------------------------------------------------
//		� EmTRGSD::ProcessCommand
// process the command we just received
// ---------------------------------------------------------------------------
void EmTRGSD::ProcessCommand(void) {
    // clear command buffer
    cmdIndex = 0;
    sdBusState = sdBusSendResponseState;
    sdBusNextState = sdBusGetCmdState;
    responseIndex = 0;
    dataIndex = 0;
    dataSize = 0;

    switch (cmdBuffer[0] & 0x3f) {
        case SD_CMD0_GO_IDLE:
            responseBuffer[0] = SD_R1_IDLE_STATE;
            responseSize = 1;
            break;

        case SD_CMD1_GO_OPERATIONAL:
            responseBuffer[0] = SD_R1_OP_STATE;
            responseSize = 1;
            break;

        case SD_CMD9_SEND_CSD:
            responseBuffer[0] = SD_R1_OP_STATE;
            responseSize = 1;

            sdBusNextState = sdBusSendDataState;
            dataBuffer[0] = SD_DATA_START;
            memset(&dataBuffer[1], 0, SD_CSD_SIZE);
            dataBuffer[SD_CSD_SIZE + 2] = 0x00;  // 16bit crc (unused)
            dataBuffer[SD_CSD_SIZE + 3] = 0x00;
            dataSize = SD_CSD_SIZE + 4;
            break;

        case SD_CMD10_SEND_CID:
            responseBuffer[0] = SD_R1_OP_STATE;
            responseSize = 1;

            sdBusNextState = sdBusSendDataState;
            dataBuffer[0] = SD_DATA_START;
            memcpy(&dataBuffer[1], 0, SD_CID_SIZE);
            dataBuffer[SD_CID_SIZE + 1] = 0x00;  // 16bit crc (unused)
            dataBuffer[SD_CID_SIZE + 2] = 0x00;
            dataSize = SD_CID_SIZE + 3;
            break;

        case SD_CMD13_SEND_STATUS:
            responseBuffer[0] = SD_R1_OP_STATE;
            responseBuffer[1] = SD_STATUS_OK;
            responseSize = 2;
            break;

        case SD_CMD17_READ_SECTOR:
            responseBuffer[0] = SD_R1_OP_STATE;
            responseSize = 1;

            sdBusNextState = sdBusSendDataState;
            dataBuffer[0] = SD_DATA_START;
            sectorNum =
                ((cmdBuffer[1] << 24) | (cmdBuffer[2] << 16) | (cmdBuffer[3] << 8) | cmdBuffer[4]);
            // SD sector number is really a byte address...
            sectorNum /= SECTOR_SIZE;
            DiskIO.ReadSector(sectorNum, &dataBuffer[1]);
            dataBuffer[SECTOR_SIZE + 1] = 0x00;  // 16 bit crc (unused)
            dataBuffer[SECTOR_SIZE + 2] = 0x00;
            dataSize = SECTOR_SIZE + 3;
            break;

        case SD_CMD24_WRITE_SECTOR:
            responseBuffer[0] = SD_R1_OP_STATE;
            responseSize = 1;
            sectorNum =
                ((cmdBuffer[1] << 24) | (cmdBuffer[2] << 16) | (cmdBuffer[3] << 8) | cmdBuffer[4]);
            // SD sector number is really a byte address...
            sectorNum /= SECTOR_SIZE;

            sdBusNextState = sdBusGetDataState;
            dataSize = SECTOR_SIZE + 3;
            break;

        default:  // unimplemented
            responseBuffer[0] = SD_R1_ILLEGAL_CMD;
            responseSize = 1;
            break;
    }
}

// ---------------------------------------------------------------------------
//		� EmTRGSD::ExchangeBits
// ---------------------------------------------------------------------------
void EmTRGSD::ExchangeBits(uint16 txData, uint16 *rxData, uint16 bits) {
    switch (bits) {
        case 8:
            switch (sdBusState) {
                case sdBusGetCmdState:
                    // all SPI commands start first two bits 0, 1
                    if ((cmdIndex != 0) || ((txData & 0xC0) == 0x40)) {
                        cmdBuffer[cmdIndex] = txData;
                        cmdIndex++;
                        if (cmdIndex == SD_CMD_SIZE) ProcessCommand();
                    }
                    *rxData = 0xffff;
                    break;

                case sdBusSendResponseState:
                    *rxData = responseBuffer[responseIndex++];
                    if (responseIndex == responseSize) sdBusState = sdBusNextState;
                    break;

                case sdBusGetDataState:
                    // all data blocks start with SD_DATA_START
                    if ((dataIndex != 0) || (txData == SD_DATA_START)) {
                        dataBuffer[dataIndex++] = txData;
                        if (dataIndex == dataSize) {
                            // finish processing command waiting for this data...
                            CompleteCommand();
                        }
                    }
                    *rxData = 0xffff;
                    break;

                case sdBusSendDataState:
                    *rxData = dataBuffer[dataIndex++];
                    if (dataIndex == dataSize) sdBusState = sdBusGetCmdState;
                    break;

                default:
                    break;
            }
            break;

        case 16:
            switch (sdBusState) {
                case sdBusGetCmdState:
                    // all SPI commands start first two bits 0, 1
                    if ((cmdIndex != 0) || ((txData & 0xC000) == 0x4000)) {
                        cmdBuffer[cmdIndex++] = (txData & 0xff00) >> 8;
                        cmdBuffer[cmdIndex++] = (txData & 0x00ff);
                        if (cmdIndex >= SD_CMD_SIZE) ProcessCommand();
                    }
                    *rxData = 0xffff;
                    break;

                case sdBusSendResponseState:
                    *rxData = responseBuffer[responseIndex++] << 8;
                    *rxData |= responseBuffer[responseIndex++];
                    if (responseIndex >= responseSize) sdBusState = sdBusNextState;
                    break;

                case sdBusGetDataState:
                    dataBuffer[dataIndex++] = (txData & 0xff00) >> 8;
                    dataBuffer[dataIndex++] = (txData & 0x00ff);
                    if (dataIndex >= dataSize) {
                        // finish processing command waiting for this data...
                        CompleteCommand();

                        sdBusState = sdBusGetCmdState;
                    }
                    *rxData = 0xffff;
                    break;

                case sdBusSendDataState:
                    *rxData = dataBuffer[dataIndex++] << 8;
                    *rxData |= dataBuffer[dataIndex++];
                    if (dataIndex >= dataSize) sdBusState = sdBusGetCmdState;
                    break;

                default:
                    break;
            }
            break;

        default:
            // not supported...
            EmAssert(false);
            break;
    }
}
