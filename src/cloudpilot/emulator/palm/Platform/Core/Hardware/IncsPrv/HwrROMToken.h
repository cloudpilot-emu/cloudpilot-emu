/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: HwrROMToken.h
 *
 * Description:
 *		Hardware ROM Token Equates.
 *
 * History:
 *		 5/31/99	SCL	Created by Steve Lemke, extracted from <Hardware.h>
 *
 *****************************************************************************/

#ifdef	NON_PORTABLE	   // So app's don't mistakenly include this

#ifndef __HWR_ROM_TOKEN_H__
#define __HWR_ROM_TOKEN_H__



//--------------------------------------------------------------------
// Layout of the A to D convertor calibration ROM token.
// This data is calculated for each unit during production, and burnt
// into the ROM.  It consists of 2 precise voltages which are 
// applied to the A to D convertor, as well as their respective outputs
// from the A/D convertor.  Because of manufacturing tolerances in the 
// convertor, its important that the conversion of the output (0-255) 
// into centivolts is tailored to each device during manufacturing.
//--------------------------------------------------------------------
typedef struct {
	UInt16	lowCentivolts;
	UInt16	lowAToDOutput;
	UInt16	highCentivolts;
	UInt16	highAToDOutput;
} SysAToDCalibrationTokenType;



#define	hwrROMTokenSnum		'snum'	// MUST be the first token, should be 12 bytes long
													//   Snum == Memory Card Flash ID (serial number)
#define	hwrROMTokenIrda		'irda'	// present ONLY on memory card w/IrDA (Gordon)
#define	hwrROMTokenFlex		'flex'	// present ONLY on memory card w/Flex pager (Cessna)
#define	hwrROMTokenBrst		'brst'	// present ONLY when Sharp PSRAM is used (triggers PrvHwrBurstRefresh)
#define	hwrROMTokenBoot1		'cd01'	// boot time code plug-in #1 (See RomBoot.c and HwrROMToken.c)
#define	hwrROMTokenBoot2		'cd02'	// boot time code plug-in #2 (See RomBoot.c)
#define	hwrROMTokenBoot3		'cd03'	// boot time code plug-in #3 (See RomBoot.c)
#define	hwrROMTokenBoot4		'cd04'	// boot time code plug-in #4 (See RomBoot.c)
#define	hwrROMTokenBoot5		'cd05'	// boot time code plug-in #5 (See HwrDisplayBootTD1.c)
#define	hwrROMTokenBoot6		'cd06'	// boot time code plug-in #6 (See KeyMgrTD1.c)
#define	hwrROMTokenBoot7		'cd07'	// boot time code plug-in #7 (See ROMHardwareEZ.c)
#define	hwrROMTokenBoot8		'cd08'	// boot time code plug-in #8 (See ROMHardwareEZ.c)
#define	hwrROMTokenAToD		'adcc'	// Data for the A to D Convertor calibration
// 6/17/98 jhl
#define	hwrROMTokenMobitex	'mbtx'	// Mobitex calibration and serial number
#define	hwrROMTokenAlkCal		'calk'	// alkaline calibration value
#define	hwrROMTokenNicadCal	'cncd'	// nicad calibration value


// 3/2/99 SCL
// The following defines are for the first ("manufacturer") digit of the 'snum' token.
#define hwrManu
typedef enum {
	hwrSnumManu3Com1		= '1',
	hwrSnumManuTRG			= '4',
	hwrSnumManu3Com5		= '5',
	hwrSnumManu3Com6		= '6',
	hwrSnumManuMotorola	= '7',
	hwrSnumManuSymbol		= '8',
	hwrSnumManuQualcomm	= '9',
	hwrSnumManu3ComE		= 'E'
	} HwrSnumManufacturerEnum;


// 10/06/98	jfs
// The following structure describes the format of the data field of the hwrROMTokenHardwareID ROM token.
// Note the featureIDs field is a variable length structure in which the OEM can place any number
// of IDs representing features of THEIR particular hardware. Only the oemID and productID fields
// are mandatory, the values of which are assigned to the OEM partner by Palm Computing.
typedef struct {
	UInt32	oemID;
	UInt32	productID;
	UInt32	featureIDs[1];		// OEM-specific variable length data goes here (0 or more DWords)
} HwrIDRec, *HwrIDRecP;

// The following ROM token activates the software-only hardware ID and features detection code.
// See HwrIdentifyFeatures for details.
#define	hwrROMTokenHardwareID		'hwid'

// GHwrMiscFlags ROM tokens (each has a corresponding hwrMiscFlagXXX bit definition)
#define	hwrROMTokenHasBacklight		'hbkl'
#define	hwrROMTokenHasMbdIrDA		'hirm'
#define	hwrROMTokenHasCardIrDA		'hirc'
#define	hwrROMTokenHasBurrBrown		'hbbd'
#define	hwrROMTokenHasJerryHW		'hjrm'
#define	hwrROMTokenNoRTCBug			'hrtc'
#define	hwrROMTokenHas3vRef			'h3vr'
#define	hwrROMTokenHasAntennaSw		'hant'

#define	hwrROMTokenHasCradleDetect	'hcrd'
#define	hwrROMTokenHasSWContrast	'hswc'
#define	hwrROMTokenInvertLCDForBL	'hibl'
#define	hwrROMTokenHasMiscFlagExt	'hmfe'

// GHwrMiscFlagsExt ROM tokens (each has a corresponding hwrMiscFlagExtXXX bit definition)
#define	hwrROMTokenExtHasLiIon		'hbli'
#define	hwrROMTokenExtHasRailIO		'hrio'

// NOTE: The following (flash) tokens should never be used; they're ignored by HwrIdentifyFeatures
//#define	hwrROMTokenExtHasFlash		'hflr'
//#define	hwrROMTokenExtHasFParms		'hfpr'

#define	hwrROMTokenExt115KIrOK		'h115'



/**************************************************************************
 * Prototypes of functions used only when running on the real hardware
 ***************************************************************************/
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

Err		HwrGetROMToken (UInt16 cardNo, UInt32 token, UInt8 **dataP, UInt16 * sizeP )
							HAL_CALL(sysTrapHwrGetROMToken);

#ifdef __cplusplus 
}
#endif
#endif

#endif 	//__HWR_ROM_TOKEN_H__

#endif 	// NON_PORTABLE
