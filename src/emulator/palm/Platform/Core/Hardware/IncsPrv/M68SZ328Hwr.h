/*******************************************************************
 * 							 Pilot Software
 *
 *		 Copyright(c) 1995-1999, Palm Computing Inc., All Rights Reserved
 *
 *-------------------------------------------------------------------  
 * FileName:
 *		M68SZ328Hwr.h
 *
 * Description:
 *		Hardware Equates for the Motorola 68SZ328 Dragonball-SZ chip 
 *
 * History:
 *   	07/05/99  TJC	Created; integrated from M68VZ328Hwr.h
 *
 *******************************************************************/  

#ifdef	NON_PORTABLE						// So apps don't include this...


#ifndef	__M68SZ328HWR_H__
#define	__M68SZ328HWR_H__

#if INCLUDE_SECRET_STUFF


#include <PalmTypes.h>							// Touchdown includes


//  Define location of the 68328 Dragonball chip
//	Note: this is not the same as kMemoryStart in this chip
#define	hwr68328Base		0xFFFFF000L					// Base address of 68328 


/***********************************************************************
 * Motorola 68SZ328  registers
 ***********************************************************************/
struct HwrM68SZ328Type
{
	UInt16	dmaControl;					// $00000: DMA Control Register
	UInt16	dmaInterruptStatus;			// $00002: DMA Interrupt Status Register
	UInt16	dmaInterruptMask;			// $00004: DMA Interrupt Mask Register
	UInt16	dmaBurstTimeOutStatus;		// $00006: DMA Burst Time-Out Status Register
	UInt16	dmaRequestTimeOutStatus;	// $00008: DMA Request Time-Out Status Register

												// Note, the spaces around the minus sign are
												// necessary.  Without them, a proper compiler
												// like gcc 3.0 thinks that the expression
												// is an invalid scientific notion number.
	UInt8									___filler00[0x00E - 0x00A];

	UInt16	dmaBurstTimeOutControl;		// $0000E: DMA Burst Time-Out Control Register

	UInt8									___filler01[0x040-0x010];

	UInt32	mem0SourceAddress;			// $00040: Memory Channel 0 Source Address Register
	UInt32	mem0DestAddress;			// $00044: Memory Channel 0 Destination Address Register
	UInt32	mem0Count;					// $00048: Memory Channel 0 Count Register
	UInt16	mem0Control;				// $0004C: Memory Channel 0 Control Register
	UInt16	mem0BurstLength;			// $0004E: Memory Channel 0 Burst Length Register
	UInt16	mem0BusUtilizationControl;	// $00050: Memory Channel 0 Bus Utilization Control Register
	UInt16	mem0BlockLength;			// $00052: Memory Channel 0 Block Length Register
	UInt16	mem0BlockSeparationDist;	// $00054: Memory Channel 0 Block Separation Distance Register
	UInt16	mem0DMARequestTimeOut;		// $00056: Memory Channel 0 DMA Request Time Out Register

	UInt8									___filler02[0x080-0x058];

	UInt32	mem1SourceAddress;			// $00080: Memory Channel 1 Source Address Register
	UInt32	mem1DestAddress;			// $00084: Memory Channel 1 Destination Address Register
	UInt32	mem1Count;					// $00088: Memory Channel 1 Count Register
	UInt16	mem1Control;				// $0008C: Memory Channel 1 Control Register
	UInt16	mem1BurstLength;			// $0008E: Memory Channel 1 Burst Length Register
	UInt16	mem1BusUtilizationControl;	// $00090: Memory Channel 1 Bus Utilization Control Register
	UInt16	mem1BlockLength;			// $00092: Memory Channel 1 Block Length Register
	UInt16	mem1BlockSeparationDist;	// $00094: Memory Channel 1 Block Separation Distance Register
	UInt16	mem1DMARequestTimeOut;		// $00096: Memory Channel 1 DMA Request Time Out Register

	UInt8									___filler03[0x0C0-0x098];

	UInt32	io2MemoryAddress;			// $000C0: I/O Channel 2 Memory Address Register
	UInt32	io2PeripheralAddress;		// $000C4: I/O Channel 2 Peripheral Address Register
	UInt32	io2CountRegister;			// $000C8: I/O Channel 2 Count Register
	UInt16	io2Control;					// $000CC: I/O Channel 2 Control Register
	UInt16	io2RequestSourceSelect;		// $000CE: I/O Channel 2 Request Source Select Register
	UInt16	io2BurstLength;				// $000D0: I/O Channel 2 Burst Length Register
	UInt16	io2DMARequestTimeOut;		// $000D2: I/O Channel 2 DMA Request Time-Out Register

	UInt8									___filler04[0x100-0x0D4];

	UInt32	io3MemoryAddress;			// $00100: I/O Channel 3 Memory Address Register
	UInt32	io3PeripheralAddress;		// $00104: I/O Channel 3 Peripheral Address Register
	UInt32	io3CountRegister;			// $00108: I/O Channel 3 Count Register
	UInt16	io3Control;					// $0010C: I/O Channel 3 Control Register
	UInt16	io3RequestSourceSelect;		// $0010E: I/O Channel 3 Request Source Select Register
	UInt16	io3BurstLength;				// $00110: I/O Channel 3 Burst Length Register
	UInt16	io3DMARequestTimeOut;		// $00112: I/O Channel 3 DMA Request Time-Out Register

	UInt8									___filler05[0x140-0x114];

	UInt32	io4MemoryAddress;			// $00140: I/O Channel 4 Memory Address Register
	UInt32	io4PeripheralAddress;		// $00144: I/O Channel 4 Peripheral Address Register
	UInt32	io4CountRegister;			// $00148: I/O Channel 4 Count Register
	UInt16	io4Control;					// $0014C: I/O Channel 4 Control Register
	UInt16	io4RequestSourceSelect;		// $0014E: I/O Channel 4 Request Source Select Register
	UInt16	io4BurstLength;				// $00150: I/O Channel 4 Burst Length Register
	UInt16	io4DMARequestTimeOut;		// $00152: I/O Channel 4 DMA Request Time-Out Register

	UInt8									___filler06[0x180-0x154];

	UInt32	io5MemoryAddress;			// $00180: I/O Channel 5 Memory Address Register
	UInt32	io5PeripheralAddress;		// $00184: I/O Channel 5 Peripheral Address Register
	UInt32	io5CountRegister;			// $00188: I/O Channel 5 Count Register
	UInt16	io5Control;					// $0018C: I/O Channel 5 Control Register
	UInt16	io5RequestSourceSelect;		// $0018E: I/O Channel 5 Request Source Select Register
	UInt16	io5BurstLength;				// $00190: I/O Channel 5 Burst Length Register
	UInt16	io5DMARequestTimeOut;		// $00192: I/O Channel 5 DMA Request Time-Out Register

	UInt8									___filler07[0x200-0x194];

	UInt16	adcPenSampleFifo;			// $00200: ADC Pen Sample FIFO

	UInt8									___filler08[0x210-0x202];

	UInt32	adcControl;					// $00210: ADC Control Register
	UInt16	adcPenADSampleRateControl;	// $00214: ADC Pen A/D Sample Rate Control Register

	UInt8									___filler09[0x218-0x216];

	UInt16	adcInterruptControl;		// $00218: ADC Interrupt Control Register
	UInt16	adcInterruptErrorStatus;	// $0021A: ADC Interrupt/Error Status Register

	UInt8									___filler10[0x22C-0x21C];

	UInt16	adcClockDivide;				// $0022C: ADC Clock Divide Register

	UInt8									___filler11[0x230-0x22E];

	UInt32	adcCompareControl;			// $00230: ADC Compare Control Register

	UInt8									___filler12[0x300-0x234];

	UInt16	mmcClockControl;			// $00300: MMC Clock Control Register

	UInt8									___filler13[0x304-0x302];

	UInt16	mmcStatus;					// $00304: MMC Status Register

	UInt8									___filler14[0x308-0x306];

	UInt16	mmcClockRate;				// $00308: MMC Clock Rate Register

	UInt8									___filler15[0x310-0x30A];

	UInt16	mmcCommandDataControl;		// $00310: MMC Command and Data Control Register

	UInt8									___filler16[0x314-0x312];

	UInt16	mmcResponseTimeOut;			// $00314: MMC Response Time Out Register

	UInt8									___filler17[0x318-0x316];

	UInt16	mmcReadTimeOut;				// $00318: MMC Read Time Out Register

	UInt8									___filler18[0x31C-0x31A];

	UInt16	mmcBlockLength;				// $0031C: MMC Block Length Register

	UInt8									___filler19[0x320-0x31E];

	UInt16	mmcNumberOfBlocks;			// $00320: MMC Number of Blocks Register

	UInt8									___filler20[0x324-0x322];

	UInt16	mmcRevisionNumber;			// $00324: MMC Revision Number Register

	UInt16	mmcIOPort;					// $00326: MMC IO Port Register
	UInt16	mmcInterruptMask;			// $00328: MMC Interrupt Mask Register

	UInt8									___filler21[0x32C-0x32A];

	UInt16	mmcCommandNumber;			// $0032C: MMC Command Number Register

	UInt8									___filler22[0x330-0x32E];

	UInt16	mmcHigherArgument;			// $00330: MMC HIGHER Argument Register
	UInt16	mmcLowerArgument;			// $00332: MMC LOWER Argument Register
	UInt16	mmcResponseFifo;			// $00334: MMC Response FIFO Register;

	UInt8									___filler23[0x338-0x336];

	UInt16	mmcBufferAccess;			// $00338: MMC Buffer Access Register

	UInt8									___filler24[0x33C-0x33A];

	UInt16	mmcBufferPartFull;			// $0033C: MMC Buffer Part Full Register

	UInt8									___filler25[0x400-0x33E];

	UInt32	usbFrameNumberMatch;		// $00400: USB Frame Number and Match
	UInt32	usbSpecificationRelease;	// $00404: USB Specification/Release Number
	UInt32	usbStatus;					// $00408: USB Status
	UInt32	usbControl;					// $0040C: USB Control
	UInt32	usbConfigurationStatus;		// $00410: USB Configuration Status
	UInt32	usbEndpointBufferData;		// $00414: USB Endpoint Buffer Data
	UInt32	usbGeneralInterruptStatus;	// $00418: USB General Interrupt Status Register
	UInt32	usbInterruptMask;			// $0041C: USB Interrupt Mask

	UInt8									___filler26[0x424-0x420];

	UInt32	usbEnable;					// $00424: USB Enable
	UInt32	usbInterruptStatus;			// $00428: USB Interrupt Status Register

	UInt8									___filler27[0x430-0x42C];

	UInt32	usbEP0StatusControl;		// $00430: USB Endpoint 0 Status/Control
	UInt32	usbEP0InterruptStatus;		// $00434: USB Endpoint 0 Interrupt Status
	UInt32	usbEP0InterruptMask;		// $00438: USB Endpoint 0 Interrupt Mask
	UInt16	usbEP0FifoData;				// $0043C: USB Endpoint 0 FIFO Data

	UInt8									___filler28[0x440-0x43E];

	UInt32	usbEP0FifoStatus;			// $00440: USB Endpoint 0 FIFO Status
	UInt32	usbEP0FifoControl;			// $00444: USB Endpoint 0 FIFO Control
	UInt32	usbEP0LastReadFramePointer;	// $00448: USB Endpoint 0 FIFO Last Read Frame Pointer
	UInt32	usbEP0LastWriteFramePointer;	// $0044C: USB Endpoint 0 FIFO Last Write Frame Pointer
	UInt32	usbEP0FifoAlarm;			// $00450: USB Endpoint 0 FIFO Alarm
	UInt32	usbEP0FifoReadPointer;		// $00454: USB Endpoint 0 FIFO Read Pointer
	UInt32	usbEP0FifoWritePointer;		// $00458: USB Endpoint 0 FIFO Write Pointer

	UInt8									___filler29[0x460-0x45C];

	UInt32	usbEP1StatusControl;		// $00460: USB Endpoint 1 Status/Control
	UInt32	usbEP1InterruptStatus;		// $00464: USB Endpoint 1 Interrupt Status
	UInt32	usbEP1InterruptMask;		// $00468: USB Endpoint 1 Interrupt Mask
	UInt16	usbEP1FifoData;				// $0046C: USB Endpoint 1 FIFO Data

	UInt8									___filler30[0x470-0x46E];

	UInt32	usbEP1FifoStatus;			// $00470: USB Endpoint 1 FIFO Status
	UInt32	usbEP1FifoControl;			// $00474: USB Endpoint 1 FIFO Control
	UInt32	usbEP1LastReadFramePointer;	// $00478: USB Endpoint 1 FIFO Last Read Frame Pointer
	UInt32	usbEP1LastWriteFramePointer;	// $0047C: USB Endpoint 1 FIFO Last Write Frame Pointer
	UInt32	usbEP1FifoAlarm;			// $00480: USB Endpoint 1 FIFO Alarm
	UInt32	usbEP1FifoReadPointer;		// $00484: USB Endpoint 1 FIFO Read Pointer
	UInt32	usbEP1FifoWritePointer;		// $00488: USB Endpoint 1 FIFO Write Pointer

	UInt8									___filler31[0x490-0x48C];

	UInt32	usbEP2StatusControl;		// $00490: USB Endpoint 2 Status/Control
	UInt32	usbEP2InterruptStatus;		// $00494: USB Endpoint 2 Interrupt Status
	UInt32	usbEP2InterruptMask;		// $00498: USB Endpoint 2 Interrupt Mask
	UInt16	usbEP2FifoData;				// $0049C: USB Endpoint 2 FIFO Data

	UInt8									___filler32[0x4A0-0x49E];

	UInt32	usbEP2FifoStatus;			// $004A0: USB Endpoint 2 FIFO Status
	UInt32	usbEP2FifoControl;			// $004A4: USB Endpoint 2 FIFO Control
	UInt32	usbEP2LastReadFramePointer;	// $004A8: USB Endpoint 2 FIFO Last Read Frame Pointer
	UInt32	usbEP2LastWriteFramePointer;	// $004AC: USB Endpoint 2 FIFO Last Write Frame Pointer
	UInt32	usbEP2FifoAlarm;			// $004B0: USB Endpoint 2 FIFO Alarm
	UInt32	usbEP2FifoReadPointer;		// $004B4: USB Endpoint 2 FIFO Read Pointer
	UInt32	usbEP2FifoWritePointer;		// $004B8: USB Endpoint 2 FIFO Write Pointer

	UInt8									___filler33[0x4C0-0x4BC];

	UInt32	usbEP3StatusControl;		// $004C0: USB Endpoint 3 Status/Control
	UInt32	usbEP3InterruptStatus;		// $004C4: USB Endpoint 3 Interrupt Status
	UInt32	usbEP3InterruptMask;		// $004C8: USB Endpoint 3 Interrupt Mask
	UInt16	usbEP3FifoData;				// $004CC: USB Endpoint 3 FIFO Data

	UInt8									___filler34[0x4D0-0x4CE];

	UInt32	usbEP3FifoStatus;			// $004D0: USB Endpoint 3 FIFO Status
	UInt32	usbEP3FifoControl;			// $004D4: USB Endpoint 3 FIFO Control
	UInt32	usbEP3LastReadFramePointer;	// $004D8: USB Endpoint 3 FIFO Last Read Frame Pointer
	UInt32	usbEP3LastWriteFramePointer;	// $004DC: USB Endpoint 3 FIFO Last Write Frame Pointer
	UInt32	usbEP3FifoAlarm;			// $004E0: USB Endpoint 3 FIFO Alarm
	UInt32	usbEP3FifoReadPointer;		// $004E4: USB Endpoint 3 FIFO Read Pointer
	UInt32	usbEP3FifoWritePointer;		// $004E8: USB Endpoint 3 FIFO Write Pointer

	UInt8									___filler35[0x4F0-0x4EC];

	UInt32	usbEP4StatusControl;		// $004F0: USB Endpoint 4 Status/Control
	UInt32	usbEP4InterruptStatus;		// $004F4: USB Endpoint 4 Interrupt Status
	UInt32	usbEP4InterruptMask;		// $004F8: USB Endpoint 4 Interrupt Mask
	UInt16	usbEP4FifoData;				// $004FC: USB Endpoint 4 FIFO Data

	UInt8									___filler36[0x500-0x4FE];

	UInt32	usbEP4FifoStatus;			// $00500: USB Endpoint 4 FIFO Status
	UInt32	usbEP4FifoControl;			// $00504: USB Endpoint 4 FIFO Control
	UInt32	usbEP4LastReadFramePointer;	// $00508: USB Endpoint 4 FIFO Last Read Frame Pointer
	UInt32	usbEP4LastWriteFramePointer;	// $0050C: USB Endpoint 4 FIFO Last Write Frame Pointer
	UInt32	usbEP4FifoAlarm;			// $00510: USB Endpoint 4 FIFO Alarm
	UInt32	usbEP4FifoReadPointer;		// $00514: USB Endpoint 4 FIFO Read Pointer
	UInt32	usbEP4FifoWritePointer;		// $00518: USB Endpoint 4 FIFO Write Pointer

	UInt8									___filler37[0x800-0x51C];

	UInt32	lcdStartAddr;				// $00800: LCD Screen Starting Address Register
	UInt16	lcdScreenSize;				// $00804: LCD Screen Size Register
	UInt16	lcdPageWidth;				// $00806: LCD Virtual Page Width Register
	UInt16	lcdCursorXPos;				// $00808: LCD Cursor X Position
	UInt16	lcdCursorYPos;				// $0080A: LCD Cursor Y Position
	UInt16	lcdCursorSize;				// $0080C: LCD Cursor Size
	UInt16	lcdBlinkControl;			// $0080E: LCD Blink Control Register
	UInt16	lcdColorCursorMapping;		// $00810: LCD Color Cursor Mapping Register
	UInt16	lcdPanelControl0;			// $00812: LCD Panel Configuration Register 0
	UInt16	lcdPanelControl1;			// $00814: LCD Panel Configuration Register 1
	UInt16	lcdHorizontalConfig0;		// $00816: LCD Horizontal Configuration Register 0
	UInt16	lcdHorizontalConfig1;		// $00818: LCD Horizontal Configuration Register 1
	UInt16	lcdVerticalConfig0;			// $0081A: LCD Vertical Configuration Register 0
	UInt16	lcdVerticalConfig1;			// $0081C: LCD Vertical Configuration Register 1
	UInt16	lcdPanningOffset;			// $0081E: LCD Panning Offset Register
	UInt16	lcdGrayPalette;				// $00820: LCD Gray Palette Mapping Register
	UInt16	lcdPWMContrastControl;		// $00822: LCD PWM Contrast Control Register
	UInt16	lcdDMAControl;				// $00824: LCD DMA Control Register
	UInt16	lcdRefreshModeControl;		// $00826: LCD Refresh Mode Control Register
	UInt16	lcdInterruptConfiguration;	// $00828: LCD Interrupt Configuration
	UInt16	lcdInterruptStatus;			// $0082A: LCD Interrupt Status

	UInt8									___filler38[0x1F000-0x0082C];

	UInt8	scr;						// $10000: System Control Register

	UInt8									___filler39[0x003-0x001];

	UInt8	pcr;						// $10003: Peripheral Control Register
	UInt8	chipID;						// $10004: Chip ID Register
	UInt8	maskID;						// $10005: Mask ID Register
	UInt16	swID;						// $10006: Software ID Register
	UInt16	ioDriveControl;				// $10008: I/O Drive Control Register

	UInt8									___filler40[0x100-0x00A];				 

	UInt16	csAGroupBase;				// $10100: Chip Select Group A Base Register
	UInt16	csBGroupBase;				// $10102: Chip Select Group B Base Register
	UInt16	csCGroupBase;				// $10104: Chip Select Group C Base Register
	UInt16	csDGroupBase;				// $10106: Chip Select Group D Base Register

	UInt8									___filler41[0x10c-0x108];

	UInt16	csControl2;					// $1010C: Chip Select Control Register 2

	UInt8									___filler42[0x110-0x10e];

	UInt16	csASelect;					// $10110: Group A Chip Select Register
	UInt16	csBSelect;					// $10112: Group B Chip Select Register
	UInt16	csCSelect;					// $10114: Group C Chip Select Register
	UInt16	csDSelect;					// $10116: Group D Chip Select Register

	UInt16	emuCS;						// $10118: EMU Chip Select Register

	UInt8									___filler43[0x150-0x11A];		

	UInt16	csControl3;					// $10150: Chip Select Control Register 3

	UInt8									___filler43a[0x180-0x152];

	UInt16	csEGroupBase;				// $10180: Chip Select Group E Base Register
	UInt16	csFGroupBase;				// $10182: Chip Select Group F Base Register
	UInt16	csGGroupBase;				// $10184: Chip Select Group G Base Register

	UInt8									___filler44[0x18A-0x186];

	UInt16	csControl1;					// $1018A: Chip Select Control Register 1

	UInt8									___filler45[0x190-0x18C];

	UInt16	csESelect;					// $10190: Group E Chip Select Register
	UInt16	csFSelect;					// $10192: Group F Chip Select Register
	UInt16	csGSelect;					// $10194: Group G Chip Select Register

	UInt8									___filler46[0x200-0x196];

	UInt16	pllControl;					// $10200: PLL Control Register
	UInt16	pllFreqSel0;				// $10202: MCU PLL Frequency Select Register 0
	UInt16	pllFreqSel1;				// $10204: MCU PLL Frequency Select Register 1

	UInt8									___filler47;

	UInt8	pwrControl;					// $10207: CPU Power Control Register

	UInt16	usbpllFreqSel0;				// $10208: USBPLL Frequency Select Register 0
	UInt16	usbpllFreqSel1;				// $1020A: USBPLL Frequency Select Register 1

	UInt16	clockSrcCtl;				// $1020C: Clock Source Control Register

	UInt8									___filler48[0x300-0x20E];

	UInt8	intVector;					// $10300: Interrupt Vector Register

	UInt8									___filler49;

	UInt16	intControl;					// $10302: Interrupt Control Register
	UInt16	intMaskHi;					// $10304: Interrupt Mask Register (high word)
	UInt16	intMaskLo;					// $10306: Interrupt Mask Register (low word)

	UInt8									___filler50[0x30C-0x308];

	UInt16	intStatusHi;				// $1030C: Interrupt Status Register (high word)
 	UInt16	intStatusLo;				// $1030E: Interrupt Status Register (low word)
	UInt16	intPendingHi;				// $10310: Interrupt Pending Register (high word)
	UInt16	intPendingLo;				// $10312: Interrupt Pending Register (low word)
	UInt16	intLevelControl;			// $10314: Interrupt Level Control Register
	UInt16	intLevelControl2;			// $10316: Interrupt Level Control Register 2
	UInt16	intLevelControl3;			// $10318: Interrupt Level Control Register 3
	UInt16	intLevelControl4;			// $1031A: Interrupt Level Control Register 4
	UInt16	intLevelControl5;			// $1031C: Interrupt Level Control Register 5
	UInt16	intLevelControl6;			// $1031E: Interrupt Level Control Register 6
	UInt16	intLevelControl7;			// $10320: Interrupt Level Control Register 7

	UInt8 									___filler51[0x400-0x322];

	UInt8	portADir;					// $10400: Port A Direction Register
	UInt8	portAData;					// $10401: Port A Data Register
	UInt8	portAPullupEn;				// $10402: Port A Pullup Enable

	UInt8									___filler52[0x408-0x403];	

	UInt8	portBDir;					// $10408: Port B Direction Register
	UInt8	portBData;					// $10409: Port B Data Register
	UInt8	portBPullupEn;				// $1040A: Port B Pullup Enable
	UInt8	portBSelect;				// $1040B: Port B Select Register	

	UInt8									___filler53[0x410-0x40C];

	UInt8	portCDir;					// $10410: Port C Direction Register
	UInt8	portCData;					// $10411: Port C Data Register
	UInt8	portCPulldnEn;				// $10412: Port C Pulldown Enable
	UInt8	portCSelect;				// $10413: Port C Select Register	

	UInt8									___filler54[0x418-0x414];

	UInt8	portDDir;					// $10418: Port D Direction Register
	UInt8	portDData;					// $10419: Port D Data Register
	UInt8	portDPullupEn;				// $1041A: Port D Pull-up Enable
	UInt8	portDSelect;				// $1041B: Port D Select Register
	UInt8	portDIntMask;				// $1041C: Port D Interrupt Mask Register
	UInt8	portDIntStatus;				// $1041D: Port D Interrupt Status Register
	UInt8	portDIntEdge;				// $1041E: Port D Interrupt Edge Register
	UInt8	portDIntPolarity;			// $1041F: Port D Interrupt Polarity Register

	UInt8	portEDir;					// $10420: Port E Direction Register
	UInt8	portEData;					// $10421: Port E Data Register
	UInt8	portEPullupEn;				// $10422: Port E Pull-up Enable
	UInt8	portESelect;				// $10423: Port E Select Register
	UInt8	portEIntMask;				// $10424: Port E Interrupt Mask Register
	UInt8	portEIntStatus;				// $10425: Port E Interrupt Status Register
	UInt8	portEIntEdge;				// $10426: Port E Interrupt Edge Register
	UInt8	portEIntPolarity;			// $10427: Port E Interrupt Polarity Register

	UInt8	portFDir;					// $10428: Port F Direction Register
	UInt8	portFData;					// $10429: Port F Data Register
	UInt8	portFPullupdnEn;			// $1042A: Port F Pull-up/down Enable
	UInt8	portFSelect;				// $1042B: Port F Select Register
	UInt8	portFIntMask;				// $1042C: Port F Interrupt Mask Register
	UInt8	portFIntStatus;				// $1042D: Port F Interrupt Status Register
	UInt8	portFIntEdge;				// $1042E: Port F Interrupt Edge Register
	UInt8	portFIntPolarity;			// $1042F: Port F Interrupt Polarity Register

	UInt8	portGDir;					// $10430: Port G Direction Register
	UInt8	portGData;					// $10431: Port G Data Register
	UInt8	portGPullupEn;				// $10432: Port G Pull-up Enable
	UInt8	portGSelect;				// $10433: Port G Select Register
	UInt8	portGIntMask;				// $10434: Port G Interrupt Mask Register
	UInt8	portGIntStatus;				// $10435: Port G Interrupt Status Register
	UInt8	portGIntEdge;				// $10436: Port G Interrupt Edge Register
	UInt8	portGIntPolarity;			// $10437: Port G Interrupt Polarity Register

	UInt8	portJDir;					// $10438: Port J Direction Register
	UInt8	portJData;					// $10439: Port J Data Register
	UInt8	portJPullupEn;				// $1043A: Port J Pull-up Enable
	UInt8	portJSelect;				// $1043B: Port J Select Register
	UInt8	portJIntMask;				// $1043C: Port J Interrupt Mask Register
	UInt8	portJIntStatus;				// $1043D: Port J Interrupt Status Register
	UInt8	portJIntEdge;				// $1043E: Port J Interrupt Edge Register
	UInt8	portJIntPolarity;			// $1043F: Port J Interrupt Polarity Register

	UInt8	portKDir;					// $10440: Port K Direction Register
	UInt8	portKData;					// $10441: Port K Data Register
	UInt8	portKPullupdnEn;			// $10442: Port K Pull-up/down Enable
	UInt8	portKSelect;				// $10443: Port K Select Register
	UInt8	portKIntMask;				// $10444: Port K Interrupt Mask Register
	UInt8	portKIntStatus;				// $10445: Port K Interrupt Status Register
	UInt8	portKIntEdge;				// $10446: Port K Interrupt Edge Register
	UInt8	portKIntPolarity;			// $10447: Port K Interrupt Polarity Register

	UInt8	portMDir;					// $10448: Port M Direction Register
	UInt8	portMData;					// $10449: Port M Data Register
	UInt8	portMPullupdnEn;			// $1044A: Port M Pull-up/down Enable
	UInt8	portMSelect;				// $1044B: Port M Select Register
	UInt8	portMIntMask;				// $1044C: Port M Interrupt Mask Register
	UInt8	portMIntStatus;				// $1044D: Port M Interrupt Status Register
	UInt8	portMIntEdge;				// $1044E: Port M Interrupt Edge Register
	UInt8	portMIntPolarity;			// $1044F: Port M Interrupt Polarity Register

	UInt8	portNDir;					// $10450: Port N Direction Register
	UInt8	portNData;					// $10451: Port N Data Register
	UInt8	portNPullupdnEn;			// $10452: Port N Pull-up/down Enable
	UInt8	portNSelect;				// $10453: Port N Select Register
	UInt8	portNIntMask;				// $10454: Port N Interrupt Mask Register
	UInt8	portNIntStatus;				// $10455: Port N Interrupt Status Register
	UInt8	portNIntEdge;				// $10456: Port N Interrupt Edge Register
	UInt8	portNIntPolarity;			// $10457: Port N Interrupt Polarity Register

	UInt8	portPDir;					// $10458: Port P Direction Register
	UInt8	portPData;					// $10459: Port P Data Register
	UInt8	portPPullupdnEn;			// $1045A: Port P Pull-up/down Enable
	UInt8	portPSelect;				// $1045B: Port P Select Register
	UInt8	portPIntMask;				// $1045C: Port P Interrupt Mask Register
	UInt8	portPIntStatus;				// $1045D: Port P Interrupt Status Register
	UInt8	portPIntEdge;				// $1045E: Port P Interrupt Edge Register
	UInt8	portPIntPolarity;			// $1045F: Port P Interrupt Polarity Register

	UInt8	portRDir;					// $10460: Port R Direction Register
	UInt8	portRData;					// $10461: Port R Data Register
	UInt8	portRPullupdnEn;			// $10462: Port R Pull-up/down Enable
	UInt8	portRSelect;				// $10463: Port R Select Register
	UInt8	portRIntMask;				// $10464: Port R Interrupt Mask Register
	UInt8	portRIntStatus;				// $10465: Port R Interrupt Status Register
	UInt8	portRIntEdge;				// $10466: Port R Interrupt Edge Register
	UInt8	portRIntPolarity;			// $10467: Port R Interrupt Polarity Register

	UInt8									___filler55[0x500-0x468];

	UInt16	pwmControl;					// $10500: PWM 1 Control Register  (PWM 1 is the same as the single PWM in EZ)
	UInt8	pwmSampleHi;    			// $10502: PWM 1 Sample - high byte
	UInt8	pwmSampleLo;				// $10503: PWM 1 Sample - low byte
	UInt8	pwmPeriod;					// $10504: PWM 1 Period
	UInt8	pwmCounter;					// $10505: PWM 1 Counter

	UInt8									___filler56[0x510-0x506];

	UInt16	pwm2Control;				// $10510: PWM 2 Control Register
	UInt16	pwm2Period;					// $10512: PWM 2 Period
	UInt16	pwm2Width;					// $10514: PWM 2 Width
	UInt16	pwm2Counter;				// $10516: PWM 2 Counter

	UInt8									___filler57[0x600-0x518];

	UInt16	tmr1Control;				// $10600: Timer 1 Control Register
	UInt16	tmr1Prescaler;				// $10602: Timer 1 Prescaler Register
	UInt16	tmr1Compare;				// $10604: Timer 1 Compare Register
	UInt16	tmr1Capture;				// $10606: Timer 1 Capture Register
	UInt16	tmr1Counter;				// $10608: Timer 1 Counter Register
	UInt16	tmr1Status;					// $1060A: Timer 1 Status Register

	UInt8									___filler58[0x610-0x60C];

	UInt16	tmr2Control;				// $10610: Timer 2 Control Register
	UInt16	tmr2Prescaler;				// $10612: Timer 2 Prescaler Register
	UInt16	tmr2Compare;				// $10614: Timer 2 Compare Register
	UInt16	tmr2Capture;				// $10616: Timer 2 Capture Register
	UInt16	tmr2Counter;				// $10618: Timer 2 Counter Register
	UInt16	tmr2Status;					// $1061A: Timer 2 Status Register

	UInt8									___filler59[0x700-0x61C];

	UInt16	spiRxD;						// $10700: SPI Unit 1 Receive Data Register
	UInt16	spiTxD;						// $10702: SPI Unit 1 Transmit Data Register
	UInt16	spiCont1;					// $10704: SPI Unit 1 Control/Status Register
	UInt16	spiIntCS;					// $10706: SPI Unit 1 Interrupt control/Status Register
	UInt16	spiTest;					// $10708: SPI Unit 1 Test Register
	UInt16	spiSpc;						// $1070A: SPI Unit 1 Sample period counter register

	UInt8									___filler60[0x800-0x70C];

	UInt8	i2cAddress;					// $10800: I2C Address Register

	UInt8									___filler61[0x804-0x801];

	UInt8	i2cFreqDiv;					// $10804: I2C Frequency Divider Register

	UInt8									___filler62[0x808-0x805];

	UInt8	i2cControl;					// $10808: I2C Control Register

	UInt8									___filler63[0x80C-0x809];

	UInt8	i2cStatus;					// $1080C: I2C Status Register

	UInt8									___filler64[0x810-0x80D];

	UInt8	i2cDataIO;					// $10810: I2C Data I/O Register

	UInt8									___filler65[0x900-0x811];

	UInt16	uControl;					// $10900: Uart 1 Status/Control Register (Uart 1 is the same as the single Uart in EZ)
	UInt16	uBaud;						// $10902: Uart 1 Baud Control Register
	UInt16	uReceive;					// $10904: Uart 1 Receive Register
	UInt16	uTransmit;					// $10906: Uart 1 Transmit Register
	UInt16	uMisc;						// $10908: Uart 1 Miscellaneous Register
	UInt16	uNonIntPresc;				// $1090A: Uart 1 Non-Integer Prescaler
	UInt16	uFIFOHMark;					// $1090C: Uart 1 Half Mark FIFO Register

	UInt8									___filler66[0x910-0x90E];

	UInt16	u2Control;					// $10910: Uart 2 Status/Control Register
	UInt16	u2Baud;						// $10912: Uart 2 Baud Control Register
	UInt16	u2Receive;					// $10914: Uart 2 Receiver Register
	UInt16	u2Transmit;					// $10916: Uart 2 Transmitter Register
	UInt16	u2Misc;						// $10918: Uart 2 Miscellaneous Register
	UInt16	u2NonIntPresc;				// $1091A: Uart 2 Non-Integer Prescaler Register
	UInt16	u2FIFOHMark;				// $1091C: Uart 2 Half Mark FIFO Register

	UInt8									___filler67[0xB00-0x91E];

	UInt32	rtcHourMinSec;				// $10B00: RTC Hours, Minutes, Seconds Register
	UInt32	rtcAlarm;					// $10B04: RTC Alarm Register

	UInt8									___filler68[0xB0A-0xB08];

	UInt16  rtcWatchDog;				// $10B0A: RTC Watchdog Timer
	UInt16	rtcControl;					// $10B0C: RTC Control Register
	UInt16	rtcIntStatus;				// $10B0E: RTC Interrupt Status Register
	UInt16	rtcIntEnable;				// $10B10: RTC Interrupt Enable Register
	UInt16	stopWatch;					// $10B12: Stopwatch Minutes

	UInt8									___filler69[0xB1A-0xB14];

	UInt16	rtcDay;						// $10B1A: RTC Day
	UInt16  rtcDayAlarm;				// $10B1C: RTC Day Alarm

	UInt8									___filler70[0xC00-0xB1E];

	UInt32	sdramControlE;				// $10C00: SDRAM Control Register for Chip Select E
	UInt32  sdramControlF;				// $10C04: SDRAM Control Register for Chip Select F
	UInt32	edoControlE;				// $10C08: EDO Control Register for Chip Select E
	UInt32	edoControlF;				// $10C0C: EDO Control Register for Chip Select F	
	UInt16	secondaryControl;			// $10C10: Secondary Control Register

	UInt8									___filler71[0xD00-0xC12];

	UInt32	emuAddrCompare;				// $10D00: Emulation Address Compare Register   
	UInt32	emuAddrMask;				// $10D04: Emulation Address Mask Register
	UInt16	emuControlCompare;			// $10D08: Emulation Control Compare Register
	UInt16	emuControlMask;				// $10D0A: Emulation Control Mask Register
	UInt16	emuControl;					// $10DOC: Emulation Control Register
	UInt16	emuStatus;					// $10D0E: Emulation Status Register
};

typedef volatile struct HwrM68SZ328Type*	HwrM68SZ328Ptr;

#define HwrDBallType HwrM68SZ328Type
#define HwrDBallPtr HwrM68SZ328Ptr

/************************************************************************
 * ADC / ASP register for SZ
 ***********************************************************************/
#define hwrSZ328adcLowPowerOffEnable					0x04000000
#define hwrSZ328adcVoiceEnable							0x00000004
#define hwrSZ328adcPenEnable							0x00000002

#define hwrSZ328adcIntEnable							0x00000097


/************************************************************************
 * Chip ID register for SZ
 ***********************************************************************/
 #define hwrSZ328chipIDSZ								0x45

/************************************************************************
 * Mask ID register for SZ
 ***********************************************************************/
#define hwrSZ328maskID0H31J								0x01
#define hwrSZ328maskID0J75C								0x02
#define hwrSZ328maskID1J75C								0x03
#define hwrSZ328maskID0J83G								0x04
 
/************************************************************************
 * LCD Controller Bits
 ************************************************************************/
// lcdCursorXPos Register
#define	hwrSZ328LcdCursorXPosCtlMask				0xC000
	#define	hwrSZ328LcdCursorXPosCtlTrans			0x0000
	#define	hwrSZ328LcdCursorXPosCtlBlack			0x4000
	#define	hwrSZ328LcdCursorXPosCtlReverse			0x8000
	#define hwrSZ328LcdCursorXPosCtlWhite			0xC000
#define  hwrSZ328LcdCursorXPosValueMask				0x1FF
// lcdCursorYPos Register
#define  hwrSZ328LcdCursorYPosValueMask				0x1FF
	
// lcdCursorSize Register
#define hwrSZ328LcdCursorWidthMask					0x3F
   #define  hwrSZ328LcdCursorWidthOffset			0x8
#define hwrSZ328LcdCursorHeightMask					0x3F

// lcdBlinkControl Register
#define hwrSZ328LcdBlinkDivisorMask					0xFF

// lcdPanelControl1 Register
#define	hwrSZ328LcdPanelControlBusMask				0x3000
	#define	hwrSZ328LcdPanelControlBus1Bit			0x0000
	#define	hwrSZ328LcdPanelControlBus2Bit			0x1000
	#define	hwrSZ328LcdPanelControlBus4Bit			0x2000
#define	hwrSZ328LcdPanelControlGrayScale			0x4000
#define	hwrSZ328LcdPanelControl4BitGrayScale		0x0200

// lcdPWMContrastControl Register
#define hwrSZ328LcdContrastPWMWidthMask				0x00FF
#define hwrSZ328LcdContrastEnable					0x0100
#define hwrSZ328LcdLDMask							0x8000



/************************************************************************
 * Interrupt Controller Bits
 ************************************************************************/

// intMaskHi bits
// NOTE: Due to a bug in early rev's of the DragonBall, the clear bits for
// IRQ[1236] on those early chips were different than the indicator bits
// (the order was reversed).  So some older code uses the OR of the IRQ1 and
// IRQ6 bits for either, and the OR of the IRQ2 and IRQ3 bits for either.
#define hwrSZ328IntHiUSB							0x8000
#define hwrSZ328IntHiI2C							0x4000
#define hwrSZ328IntHiMMC							0x2000
#define hwrSZ328IntHiPortK							0x1000
#define hwrSZ328IntHiPortM							0x0800
#define hwrSZ328IntHiPortN							0x0400
#define hwrSZ328IntHiPortP							0x0200
#define hwrSZ328IntHiADC							0x0100
#define	hwrSZ328IntHiEMU							0x0080
#define	hwrSZ328IntHiSampleTimer					0x0040
#define hwrSZ328IntHiCSPI							0x0020
#define hwrSZ328IntHiPortR							0x0010
#define hwrSZ328IntHiIRQ6							0x0008
#define hwrSZ328IntHiIRQ3							0x0004
#define hwrSZ328IntHiIRQ2							0x0002
#define hwrSZ328IntHiIRQ1							0x0001

// intMaskLo bits
#define	hwrSZ328IntLoDMA1							0x8000
#define	hwrSZ328IntLoDMA2							0x4000
#define	hwrSZ328IntLoPWM2							0x2000
#define	hwrSZ328IntLoPWM2Bit						13
#define	hwrSZ328IntLoUART2							0x1000
#define	hwrSZ328IntLoUART2Bit						12
#define	hwrSZ328IntLoPortD							0x0800
#define	hwrSZ328IntLoPortE							0x0400
#define	hwrSZ328IntLoPortF							0x0200
#define	hwrSZ328IntLoPortG							0x0100
#define hwrSZ328IntLoAllKeys						0x0F00
#define	hwrSZ328IntLoInt0Bit						8
#define	hwrSZ328IntLoPWM							0x0080
#define	hwrSZ328IntLoPWMBit							7
#define	hwrSZ328IntLoPortJ							0x0040
#define	hwrSZ328IntLoTimer2							0x0020
#define	hwrSZ328IntLoTimer2Bit						5
#define	hwrSZ328IntLoRTC							0x0010
#define	hwrSZ328IntLoRTCBit							4
#define	hwrSZ328IntLoWDT							0x0008
#define	hwrSZ328IntLoUART							0x0004
#define	hwrSZ328IntLoUARTBit						2
#define	hwrSZ328IntLoTimer							0x0002
#define	hwrSZ328IntLoTimerBit						1
#define	hwrSZ328IntLoLCDC							0x0001

// intControl bits
// NOTE: These are different than that documented in the User's manual 
// for the DragonBall (see erratas).  The DragonBall is the same as the EZ.
// Note 2: I am going to assume that these have not changed, since the SZ
// manual is the same as the VZ's (even thought the manual is not followed) [TJC]
#define	hwrSZ328IntCtlEdge1							0x0800	
#define	hwrSZ328IntCtlEdge2							0x0400
#define	hwrSZ328IntCtlEdge3							0x0200
#define	hwrSZ328IntCtlEdge6							0x0100
#define	hwrSZ328IntCtlPol1							0x8000	
#define	hwrSZ328IntCtlPol2							0x4000
#define	hwrSZ328IntCtlPol3							0x2000
#define	hwrSZ328IntCtlPol6							0x1000
#define hwrSZ328IntCtlPol5							0x0080


/************************************************************************
 * SDRAM Controller Bits bits
 * Not all the bits are defined here-- some are pretty bad to muck with
 * in anything but initial setup of the SDRAM.
 ************************************************************************/
// sdramControl[E,F] bits
#define  hwrSZ328SdramControlEnable					0x80000000
#define  hwrSZ328SdramControlOperatingMode			0x70000000
			// rsvd
#define  hwrSZ328SdramControlIAM					0x00800000
			// rsvd
			// rsvd
			// rsvd
#define  hwrSZ328SdramControlCASLatency				0x00000300

// edoControl[E,F] bits
#define  hwrSZ328EdoControlEnable					0x80000000


/************************************************************************
 * Timer bits
 ************************************************************************/
#define	hwrSZ328TmrControlUnused					0xFC00
#define hwrSZ328TmrControlTIOOutput					0x0200
#define	hwrSZ328TmrControlFreeRun					0x0100
#define	hwrSZ328TmrControlCaptureEdgeMask			0x00C0
	#define	hwrSZ328TmrControlCaptureEdgeNone		0x0000
	#define	hwrSZ328TmrControlCaptureEdgeRising		0x0040
	#define	hwrSZ328TmrControlCaptureEdgeFalling	0x0080
	#define	hwrSZ328TmrControlCaptureEdgeBoth		0x00C0
#define	hwrSZ328TmrControlOutputModeToggle			0x0020
#define	hwrSZ328TmrControlEnInterrupt				0x0010
#define	hwrSZ328TmrControlClkSrcMask				0x000E
	#define	hwrSZ328TmrControlClkSrcStop			0x0000
	#define	hwrSZ328TmrControlClkSrcSys				0x0002
	#define	hwrSZ328TmrControlClkSrcSysBy16			0x0004
	#define	hwrSZ328TmrControlClkSrcTIN				0x0006
	#define	hwrSZ328TmrControlClkSrc32KHz			0x0008
#define	hwrSZ328TmrControlEnable					0x0001

#define	hwrSZ328TmrStatusCapture					0x0002
#define	hwrSZ328TmrStatusCaptureBit					1
#define	hwrSZ328TmrStatusCompare					0x0001
#define	hwrSZ328TmrStatusCompareBit					0


/************************************************************************
 * 68SZ328 Serial Port Register Bits
 ************************************************************************/
// uControl Register
#define	hwrSZ328UControlUARTEnable					0x8000
#define	hwrSZ328UControlRxEnable					0x4000
#define	hwrSZ328UControlTxEnable					0x2000
#define	hwrSZ328UControlRxClock1xSync				0x1000
#define	hwrSZ328UControlParityEn					0x0800
#define	hwrSZ328UControlParityOdd					0x0400
#define	hwrSZ328UControlStopBits2					0x0200
#define	hwrSZ328UControlDataBits8					0x0100
#define	hwrSZ328UControlOldDataEn					0x0080
#define	hwrSZ328UControlCTSDeltaEn					0x0040
#define	hwrSZ328UControlRxFullEn					0x0020
#define	hwrSZ328UControlRxHalfEn					0x0010
#define	hwrSZ328UControlRxRdyEn						0x0008
#define	hwrSZ328UControlTxEmptyEn					0x0004
#define	hwrSZ328UControlTxHalfEn					0x0002
#define	hwrSZ328UControlTxAvailEn					0x0001

#define	hwrSZ328UControlEnableAll					(hwrSZ328UControlUARTEnable |  	\
														 hwrSZ328UControlRxEnable |		\
														 hwrSZ328UControlTxEnable)

// uBaud Register
// UCLK output requires PE3 Select to be 0
#define	hwrSZ328UBaudUCLKDirOut						0x2000
#define	hwrSZ328UBaudBaudSrcUCLK					0x0800
#define	hwrSZ328UBaudDivider						0x0700
#define	hwrSZ328UBaudPrescaler						0x003F

#define	hwrSZ328UBaudDivideBitOffset				8


// uReceive Register
#define	hwrSZ328UReceiveFIFOFull					0x8000
#define	hwrSZ328UReceiveFIFOHalf					0x4000
#define	hwrSZ328UReceiveDataRdy						0x2000
#define	hwrSZ328UReceiveDataRdyBit					13
#define	hwrSZ328UReceiveOldData						0x1000
#define	hwrSZ328UReceiveOverrunErr					0x0800
#define	hwrSZ328UReceiveOverrunErrBit				11
#define	hwrSZ328UReceiveFrameErr					0x0400
#define	hwrSZ328UReceiveFrameErrBit					10
#define	hwrSZ328UReceiveBreakErr					0x0200
#define	hwrSZ328UReceiveBreakErrBit					9
#define	hwrSZ328UReceiveParityErr					0x0100
#define	hwrSZ328UReceiveParityErrBit				8
#define	hwrSZ328UReceiveData						0x00FF

#define	hwrSZ328UReceiveErrsMask					(hwrSZ328UReceiveOverrunErr |	\
														 hwrSZ328UReceiveFrameErr |		\
														 hwrSZ328UReceiveBreakErr |		\
														 hwrSZ328UReceiveParityErr)

// uTransmit Register
#define	hwrSZ328UTransmitFIFOEmpty					0x8000
#define	hwrSZ328UTransmitFIFOHalf					0x4000
#define	hwrSZ328UTransmitTxAvail					0x2000
#define	hwrSZ328UTransmitSendBreak					0x1000
#define	hwrSZ328UTransmitIgnoreCTS					0x0800
#define  hwrSZ328UTransmitBusy						0x0400
#define	hwrSZ328UTransmitCTSStatus					0x0200
#define	hwrSZ328UTransmitCTSDelta					0x0100
#define	hwrSZ328UTransmitData						0x00FF

// uMisc Register
#define hwrSZ328UMiscBaudTest						0x8000
#define	hwrSZ328UMiscClkSrcUCLK						0x4000
#define	hwrSZ328UMiscForceParityErr					0x2000
#define	hwrSZ328UMiscLoopback						0x1000
#define hwrSZ328UMiscBaudReset						0x0800
#define hwrSZ328UMiscIRTestEn						0x0400
#define	hwrSZ328UMiscReservedMask					0x0300
#define	hwrSZ328UMiscRTSThruFIFO					0x0080
#define	hwrSZ328UMiscRTSOut							0x0040
#define	hwrSZ328UMiscIRDAEn							0x0020
#define	hwrSZ328UMiscLoopIRDA						0x0010
#define hwrSZ328UMiscRXPolarityInv					0x0008
#define hwrSZ328UMiscTXPolarityInv					0x0004
#define hwrSZ328UMiscErrorIntStatus					0x0002
#define hwrSZ328UMiscErrorIntEnable					0x0001
#define	hwrSZ328UMiscUnused							0x0000

// uNonIntPresc Register
#define	hwrSZ328UNonIntPrescSrcIntPresc				0x0000
#define	hwrSZ328UNonIntPrescSrcNonIntPresc			0x8000

#define	hwrSZ328UNonIntPrescSelDiv128				0x0000
#define	hwrSZ328UNonIntPrescSelDiv64				0x0100
#define	hwrSZ328UNonIntPrescSelDiv32				0x0200
#define	hwrSZ328UNonIntPrescSelDiv16				0x0300
#define	hwrSZ328UNonIntPrescSelDiv8					0x0400
#define	hwrSZ328UNonIntPrescSelDiv4					0x0500
#define	hwrSZ328UNonIntPrescSelDiv2					0x0600
#define	hwrSZ328UNonIntPrescSelDiv1					0x0700

#define	hwrSZ328UNonIntPrescStepMask				0x00FF
#define	hwrSZ328UNonIntPrescSelOffset				8

// see SZ docs for explanation of the following.  This provides the correct
// pulse width for IRDA, and is just a statement in the docs. The values don't
// appear to make sense, but I just push the buttons.

// This has been deprecated in favor of a prv function in the serial driver and
// the SerialMgr (depending on whether you have Bond or not) that calculates
// the value of this register based on the clock frequency you're running on.
// There was a bug when running EZ at 20MHz that caused the IrDA pulsewidth to
// get too short.

#if 0
#define 	hwrSZ328UNonIntPrescIRDAMode			(hwrSZ328UNonIntPrescSrcIntPresc \
															| hwrSZ328UNonIntPrescSelDiv32 \
															| (hwrSZ328UNonIntPrescStepMask & 0x20))
#endif

/************************************************************************
 * PWM Bits
 ************************************************************************/
// PWM register bits of interest
#define	hwrSZ328PWMControlEnable					0x0010
#define	hwrSZ328PWMControlEnableBit					4
#define	hwrSZ328PWMControlEnableIRQ					0x0040
#define	hwrSZ328PWMIRQStatus						0x0080
// PWM divider chain output selectors
#define	hwrSZ328PWMControlDivMask					0x0003
#define	hwrSZ328PWMControlDivBy2					0x0
#define	hwrSZ328PWMControlDivBy4					0x1
#define	hwrSZ328PWMControlDivBy8					0x2
#define	hwrSZ328PWMControlDivBy16					0x3
// PWM Repeat Counts
#define	hwrSZ328PWMRepeatSample1x					0x0
#define	hwrSZ328PWMRepeatSample2x					0x4
#define	hwrSZ328PWMRepeatSample4x					0x8
#define	hwrSZ328PWMRepeatSample8x					0xC
// PWM hardware constraints
#define	hwrSZ328PWMFifoSizeInBytes					4
#define	hwrSZ328PWMPrescalerMask					0x7F00
#define	hwrSZ328PWMPrescalerOffset					8

/************************************************************************
 * PLL Bits
 ************************************************************************/
#define hwrSZ328PLLControlMCURestart				0x8000
#define hwrSZ328PLLControlUSBRestart				0x4000
#define hwrSZ328PLLControlClkEnable					0x0010
#define	hwrSZ328PLLControlMCUDisable				0x0008
#define hwrSZ328PLLControlUSBDisable				0x0004

#define	hwrSZ328PLLControlSysDMADiv2				0x0000
#define	hwrSZ328PLLControlSysDMADiv4				0x0100
#define	hwrSZ328PLLControlSysDMADiv8				0x0200
#define	hwrSZ328PLLControlSysDMADiv16				0x0300
#define	hwrSZ328PLLControlSysDMADiv1				0x0400

#define	hwrSZ328PLLControlPixDMADiv2				0x0000  
#define	hwrSZ328PLLControlPixDMADiv4				0x0800
#define	hwrSZ328PLLControlPixDMADiv8				0x1000
#define	hwrSZ328PLLControlPixDMADiv16				0x1800
#define	hwrSZ328PLLControlPixDMADiv1				0x2000


/************************************************************************
 * Real Time Clock (RTC) Bits
 ************************************************************************/

// RTC Control Register (RTCCTL / rtcControl) bits
#define hwrSZ328RTCControlRTCEnable					0x0080

// RTC Interrupt Enable Register (RTCIENR / rtcIntEnable) bits
#define hwrSZ328RTCIntEnableSample7					0x8000
#define hwrSZ328RTCIntEnableSample6					0x4000
#define hwrSZ328RTCIntEnableSample5					0x2000
#define hwrSZ328RTCIntEnableSample4					0x1000
#define hwrSZ328RTCIntEnableSample3					0x0800
#define hwrSZ328RTCIntEnableSample2					0x0400
#define hwrSZ328RTCIntEnableSample1					0x0200
#define hwrSZ328RTCIntEnableSample0					0x0100
#define hwrSZ328RTCIntEnableHour					0x0020
#define hwrSZ328RTCIntEnableSec						0x0010
#define hwrSZ328RTCIntEnable24Hr					0x0008
#define hwrSZ328RTCIntEnableAlarm					0x0004
#define hwrSZ328RTCIntEnableMinute					0x0002
#define hwrSZ328RTCIntEnableStopWatch				0x0001

// RTC Interrupt Status Register bits (rtcIntStatus)
#define hwrSZ328RTCIntStatusSample7					0x8000
#define hwrSZ328RTCIntStatusSample6					0x4000
#define hwrSZ328RTCIntStatusSample5					0x2000
#define hwrSZ328RTCIntStatusSample4					0x1000
#define hwrSZ328RTCIntStatusSample3					0x0800
#define hwrSZ328RTCIntStatusSample2					0x0400
#define hwrSZ328RTCIntStatusSample1					0x0200
#define hwrSZ328RTCIntStatusSample0					0x0100
#define hwrSZ328RTCIntStatusHour					0x0020
#define hwrSZ328RTCIntStatusSec						0x0010
#define hwrSZ328RTCIntStatus24Hr					0x0008
#define hwrSZ328RTCIntStatusAlarm					0x0004
#define hwrSZ328RTCIntStatusMinute					0x0002
#define hwrSZ328RTCIntStatusStopWatch				0x0001

// RTC Alarm register (RTCALRM / rtcAlarm) masks and offsets
#define hwrSZ328RTCAlarmSecondsMask					0x0000003fL
#define hwrSZ328RTCAlarmSecondsOffset				0
#define hwrSZ328RTCAlarmMinutesMask					0x003f0000L
#define hwrSZ328RTCAlarmMinutesOffset				16
#define hwrSZ328RTCAlarmHoursMask					0x1f000000L
#define hwrSZ328RTCAlarmHoursOffset					24

// RTC hours/minutes/seconds register (RTCHMS / rtcHourMinSec) masks and offsets
#define hwrSZ328RTCHourMinSecSecondsMask			0x0000003fL
#define hwrSZ328RTCHourMinSecSecondsOffset			0
#define hwrSZ328RTCHourMinSecMinutesMask			0x003f0000L
#define hwrSZ328RTCHourMinSecMinutesOffset			16
#define hwrSZ328RTCHourMinSecHoursMask				0x1f000000L
#define hwrSZ328RTCHourMinSecHoursOffset			24

// RTC Day register masks/offsets
#define hwrSZ328RTCDayDayMask						0x01FF
#define hwrSZ328RTCDayDayOffset						0

// RTC Watchdog Timer masks/offsets
#define hwrSZ328RTCWatchdogEnable					0x0001
#define hwrSZ328RTCWatchdogIntSel					0x0002
#define hwrSZ328RTCWatchdogIntStatus				0x0080
#define hwrSZ328RTCWatchdogCountMask				0x0300
#define hwrSZ328RTCWatchdogCountOffset				8

/************************************************************************
 * SPI Master bits
 ************************************************************************/

// spiCont1 Register
#define	hwrSZ328SPIMControlRateMask					0xE000
	#define	hwrSZ328SPIMControlRateDiv4				0x0000
	#define	hwrSZ328SPIMControlRateDiv8				0x2000
	#define	hwrSZ328SPIMControlRateDiv16			0x4000
	#define	hwrSZ328SPIMControlRateDiv32			0x6000
	#define	hwrSZ328SPIMControlRateDiv64			0x8000
	#define	hwrSZ328SPIMControlRateDiv128			0xA000
	#define	hwrSZ328SPIMControlRateDiv256			0xC000
	#define	hwrSZ328SPIMControlRateDiv512			0xE000
#define	hwrSZ328SPIMControlEnable					0x0200
#define	hwrSZ328SPIMControlExchange					0x0100
#define	hwrSZ328SPIMControlIntStatus				0x0080
#define	hwrSZ328SPIMControlIntEnable				0x0040
#define	hwrSZ328SPIMControlOppPhase					0x0020
#define	hwrSZ328SPIMControlInvPolarity				0x0010
#define	hwrSZ328SPIMControlBitsMask					0x000F


#endif	// INCLUDE_SECRET_STUFF

#endif // __M68SZ328_H__

#endif // NON_PORTABLE
