/******************************************************************************
 *
 * Copyright (c) 1998-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: SED1375Hwr.h
 *
 * Description:
 *             	Hardware definitions for SED1375 controller.
 *
 * History:
 * 12/14/98 BRM	Adpated from SED1374 Test tool by Bob Ebert.
 *
 *****************************************************************************/

//-----------------------------------------------------------------------
// SED1375 Registers
//-----------------------------------------------------------------------

typedef struct SED1375RegsType {
	UInt8 productRevisionCode;				// 0x00
	UInt8 mode0;								// 0x01
	
	UInt8 mode1;								// 0x02
	UInt8 mode2;								// 0x03
	
	UInt8 horizontalPanelSize;				// 0x04
	UInt8 verticalPanelSizeLSB;			// 0x05
	
	UInt8 verticalPanelSizeMSB;			// 0x06
	UInt8 FPLineStartPosition;				// 0x07
	
	UInt8 horizontalNonDisplayPeriod;	// 0x08
	UInt8 FPFRAMEStartPosition;			// 0x09
		
	UInt8 verticalNonDisplayPeriod;		// 0x0A
	UInt8 MODRate;								// 0x0B
	
	UInt8 screen1StartAddressLSB;			// 0x0C
	UInt8 screen1StartAddressMSB;			// 0x0D
	
	UInt8 screen2StartAddressLSB;			// 0x0E
	UInt8 screen2StartAddressMSB;			// 0x0F

	UInt8 screen1StartAddressMSBit;		// 0x10	!!! YES, screen1MSBit in a weird place. Send complaints to Epson.
	UInt8 memoryAddressOffset;				// 0x11
	
	UInt8 screen1VerticalSizeLSB;			// 0x12
	UInt8 screen1VerticalSizeMSB;			// 0x13
	
	UInt8 unused1;								// 0x14
	UInt8 lookUpTableAddress;				// 0x15
		
	UInt8 unused2;								// 0x16
	UInt8 lookUpTableData;					// 0x17
	
	UInt8 GPIOConfigurationControl;		// 0x18
	UInt8 GPIOStatusControl;				// 0x19
	
	UInt8 scratchPad;							// 0x1A	
	UInt8 portraitMode;						// 0x1B
	
	UInt8 lineByteCountRegister;			// 0x1C, for portrait mode only
	UInt8 unused3;								// 0x01D not used

	UInt8 unused4;								// 0x1E
	UInt8 unused5;								// 0x1F
} SED1375RegsType;

typedef volatile SED1375RegsType *SED1375RegsPtr;


//-----------------------------------------------------------------------
// Register constants and definitions
//-----------------------------------------------------------------------
#define sed1375BaseAddress								0x1F000000
#define sed1375RegisterOffset 						0x1FFE0
#define sed1375RegsAddr									((UInt8 *)sed1375BaseAddress + sed1375RegisterOffset)

#define sed1375VideoMemStart							sed1375BaseAddress
#define sed1375VideoMemSize							81920		// 80K of memory for VRAM and CLUT's


// REG[0x00] Revision Code Register
// bits 7-2 are product code, bits 1-0 are revision code
#define sed1375RevisionCodeRegister					0x00
#define sed1375ProductCodeMask						0xFC
#define sed1375RevisionCodeMask						0x03
#define sed1375ProductCode								0x06
#define sed1375ProductCodeExpected					(sed1375ProductCode << 2)
#define sed1375RevisionCodeExpected					0x00

// REG[0x01] Mode Register 0
// bits 6 (Dual/Single) and 5 (Color/Mono) and 1 (mode) are ignored if bit 7 is set.
#define sed1375ModeRegister0							0x01
#define sed1375ModeTFT									0x80		// bit 7
#define sed1375ModeDual									0x40		// bit 6
#define sed1375ModeColor								0x20		// bit 5								
#define sed1375FPLineActiveHigh						0x10		// bit 4
#define sed1375FPFramePolarityActiveHigh			0x07		// bit 3
#define sed1375FPShiftMasked							0x04		// bit 2
#define sed1375DataWidth12BitTFTPanel				0x01		// bits 1-0

// REG[0x02] Mode Register 1
#define sed1375ModeRegister1							0x02
#define sed1375BPPMask									0xC0		// bits 7,6
#define sed1375BPPShift									6
#define sed1375BPP(n)									((((n) - 1) & 0x04) << sed1375BPPShift)
#define sed13758bpp										0xC0	
#define sed13754bpp										0x80
#define sed13752bpp										0x40
#define sed13751bpp										0x00
#define sed1375HighPerformance						0x20		// bit 5
#define sed1375InputClockDiv2							0x10		// bit 4
#define sed1375DisplayBlank							0x08		// bit 3
#define sed1375FrameRepeat								0x04		// bit 2
#define sed1375HardwareVideoInvertEnable			0x02		// bit 1
#define sed1375SoftwareVideoInvert					0x01		// bit 0

// REG[0x03] Mode Register 2
#define sed1375ModeRegister2							0x03
#define sed1375LCDPWROverride							0x08		// bit 3
#define sed1375HwrPowerSaveEnable					0x04		// bit 2
#define sed1375PowerSaveMode							0x00		// bits 1,0
#define sed1375PowerSaveNormalOperation			0x03

// REG[0x04] Horizontal Panel Size Register
// (Horizontal Panel Resolution pixels / 8)-1
#define sed1375HorizontalPanelSizeRegister		0x04
#define sed1375HorizontalResolutionMask			0x7F		// bits 6-0

// REG[0x05] Vertical Panel Size Register (LSB)
// REG[0x06] Vertical Panel Size Register (MSB)
// together define 10 bits of vertical lines, set to # of lines - 1
#define sed1375VerticalPanelSizeRegisterLSB		0x05
#define sed1375VerticalPanelSizeRegisterMSB		0x06
#define sed1375VerticalResolutionMSBMask			0x30		// bits 1,0

// REG[0x07] FPLINE Start Position
#define sed1375FPLINEStartPosition					0x07
#define sed1375FPLINEStartPositionMask				0x1F		// bits 4-0

// REG[0x08] Horizontal Non-Display Period
#define sed1375HorizontalNonDisplayPeriod			0x08
#define sed1375HorizontalNonDisplayMask			0x1F		// bits 4-0
#define sed1375NoHorizontalNonDisplay				0x00

// REG[0x09] FPFRAME Start Position
#define sed1375FPFRAMEStartPosition					0x09
#define sed1375FPFRAMEStartPositionMask			0x3F		// bits 5-0

// REG[0x0A] Vertical Non-Display Period
#define sed1375VerticalNonDisplayPeriod			0x0A
#define sed1375VerticalNonDisplayStatus			0x80		// bit 7
#define sed1375VerticalNonDisplayMask				0x3F		// bits 5-0
#define sed1375NoVerticalNonDisplay					0x00

// REG[0x0B] MOD Rate Register (passive LCD only)
#define sed1375MODRateRegister						0x0B
#define sed1375MODRateUnused							0x00

// REG[0x0C] Screen 1 Start Address Register (LSB)
// REG[0x0D] Screen 1 Start Address Register (MSB)
#define sed1375Screen1StartAddressRegisterLSB	0x0C
#define sed1375Screen1StartAddressRegisterMSB	0x0D

// REG[0x0F] Screen 2 Start Address Register (LSB)
// REG[0x10] Screen 2 Start Address Register (MSB)
#define sed1375Screen2StartAddressRegisterLSB	0x0F
#define sed1375Screen2StartAddressRegisterMSB	0x10

// REG[0x12] Memory Address Offset Register
#define sed1375MemoryAddressOffsetRegister		0x12

// REG[0x13] Screen 1 Vertical Size Register (LSB)
// REG[0x14] Screen 1 Vertical Size Register (MSB)
#define sed1375Screen1VerticalSizeRegisterLSB	0x13
#define sed1375Screen1VerticalSizeRegisterMSB	0x14

// REG[0x15] Look-Up Table Address Register
#define sed1375LookUpTableAddressRegister			0x15
#define sed1375LookUpAutoIncrement					0x00
#define sed1375LookUpRedTableSelect					0x10
#define sed1375LookUpGrayGreenTableSelect			0x20
#define sed1375LookUpBlueTableSelect				0x30
#define sed1375LookUpTableAddressMask				0x0F

// REG[0x16] Look-Up Table Bank Select Register
#define sed1375LookUpTableBankSelectRegister		0x16
#define sed1375LookUpTableRedBank(n)				(((n) & 0x03) << 4)
#define sed1375LookUpTableGreenBank(n)				(((n) & 0x03) << 2)
#define sed1375LookUpTableBlueBank(n)				((n) & 0x03)
#define sed1375LookUpTableBank(n)					(sed1375LookUpTableRedBank(n) | sed1375LookUpTableGreenBank(n) | sed1375LookUpTableBlueBank(n))

// REG[0x17] Look-Up Table Data Register
#define sed1375LookUpTableDataRegister				0x17
#define sed1375LookUpTableDataMask					0x0F

// REG[0x18] GPIO Configuration Control Register
#define sed1375GPIOConfigurationControlRegister	0x18
#define sed1375GPIOPinOut(n, v)						(((v) ? 1:0) << (n))

// REG[0x19] GPIO Status/Control Register
#define sed1375GPIOStatusControlRegister			0x19
#define sed1375GPIOPinStatusMask(n)					(1 << (n))
#define sed1375GPIOPinSet(n, v)						(((v) ? 1:0) << (n))

// REG[0x1A] Scratch Pad Register
#define sed1375ScratchPadRegister					0x1A

// REG[0x1B] Portrait Mode Register
#define sed1375PortraitModeRegister					0x1B
#define sed1375LandscapeMode							0x00
#define sed1375PortraitModeEnable					0x80	// bit 7
#define sed1375X2SchemeSelect							0x40	// bit 6
#define sed1375MCLKAutoDisable						0x04	// bit 2
#define sed1375PCLKSelectBit1							0x02	// bit 1
#define sed1375PCLKSelectBit0							0x01	// bit 0

// REG[0x1C] Line UInt8 Count Register (for Portrait Mode)
#define sed1375LineByteCountRegister				0x1C

// REG[0x1D] ???? Register
#define sed1375UnusualRegister						0x1D
#define sed1375RepeatFrame								0x80
#define sed1375Skip128FramesDuringPS				0x40
#define sed1375OverlayEnable							0x20
#define sed1375OverlayOperationBit1					0x10
#define sed1375OverlayOperationBit0					0x08
#define sed1375ForceHighClock							0x04
#define sed1375FrameSkip								0x02
#define sed1375FrameSkipStatus						0x01

// REG[0x1F] Test Mode Register
#define sed1375TestModeRegister						0x1F
#define sed1375TestMode(n)								((n & 0x03) << 6)
#define sed1375TestInput(n)							((n & 0x07) << 3)
#define sed1375TestOutputMask							0x07
#define sed1375TestModeNormal							0x00
#define sed1375TestModeUnusual						sed1375TestMode(1)
