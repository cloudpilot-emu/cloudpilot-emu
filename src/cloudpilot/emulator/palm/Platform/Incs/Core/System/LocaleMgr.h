/******************************************************************************
 *
 * Copyright (c) 2000 Palm, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: LocaleMgr.h
 *
 * Release: 
 *
 * Description:
 *		Public header for routines that support locales (information specific
 *			to locales and regions).
 *
 * History:
 *	04/28/00	CS		Created by Chris Schneider.
 *	05/16/00	CS		LmCountryType/LmLanguageType are now back to
 *						CountryType/LanguageType.
 *	05/31/00	CS		Moved country and language codes to new Locale.h and removed
 *						kLanguageFirst, etc.
 *	06/06/00	kwk	Made _LmLocaleType's language & country fields be UInt16's,
 *						so that it's binary compatible with OmLocaleType.
 *	07/28/00	CS		Replaced lmChoiceMinutesWestOfGMT & lmChoiceDaylightSavings
 *						selectors with lmChoiceTimeZone.
 *	08/08/00	CS		Renamed LmGetLocaleSetting's <iMaxSize> parameter <iValueSize>
 *						to reflect the fact that the routine now checks to make sure
 *						that <oValue> is the correct size for all fixed-size settings.
 *	09/29/00	CS		Made iLocale parm of LmLocaleToIndex const.
 *	11/17/00	CS		Removed support for lmChoiceLanguage & lmChoiceCountry,
 *						since these guys were returning UInt8's, which probably
 *						won't cut it at some point in the future.  Callers can use
 *						lmChoiceLocale, which returns an LmLocaleType struct that
 *						places the country and language into UInt16 fields.
 *				CS		Defined lmAnyCountry & lmAnyLanguage, which can now be passed
 *						in LmLocaleToIndex's iLocale parameter (as wildcards).
 *
 *****************************************************************************/

#ifndef	__LOCALEMGR_H__
#define	__LOCALEMGR_H__

/* Supporting lmChoiceLanguageName would add over 3K to the 'locs' resource.
	DOLATER CS - either do it or punt.
*/
#define SUPPORT_LANGUAGE_NAME 0

#include <CoreTraps.h>

/***********************************************************************
 * Locale Manager errors
 **********************************************************************/

/* Locale not found in 'locs' resource.
*/
#define	lmErrUnknownLocale				(lmErrorClass | 1)

/* Locale index >= LmGetNumLocales().
*/
#define	lmErrBadLocaleIndex				(lmErrorClass | 2)

/* LmLocaleSettingChoice out of bounds.
*/
#define	lmErrBadLocaleSettingChoice	(lmErrorClass | 3)

/* Data for locale setting too big for destination.
*/
#define	lmErrSettingDataOverflow		(lmErrorClass | 4)

/***********************************************************************
 * Locale Manager settings (pass to LmGetLocaleSetting)
 **********************************************************************/

typedef UInt16 LmLocaleSettingChoice;

/* LmLocaleType
*/
#define lmChoiceLocale						((LmLocaleSettingChoice)1)

/* Char[kMaxLanguageNameLen+1] - Name of the language spoken there (localized)
*/
#if SUPPORT_LANGUAGE_NAME
	#define lmChoiceLanguageName				((LmLocaleSettingChoice)4)
#else
	#define lmChoiceLanguageName				#error lmChoiceLanguageName not supported
#endif

/* Char[kMaxCountryNameLen+1] - Name of the country (localized)
*/
#define lmChoiceCountryName				((LmLocaleSettingChoice)5)

/* DateFormatType
*/
#define lmChoiceDateFormat					((LmLocaleSettingChoice)6)

/* DateFormatType
*/
#define lmChoiceLongDateFormat			((LmLocaleSettingChoice)7)

/* TimeFormatType
*/
#define lmChoiceTimeFormat					((LmLocaleSettingChoice)8)

/* UInt16 - Weekday for calendar column 1 (sunday=0, monday=1, etc.)
*/
#define lmChoiceWeekStartDay				((LmLocaleSettingChoice)9)

/* Int16 - Default GMT offset minutes, + for east of GMT, - for west
*/
#define lmChoiceTimeZone					((LmLocaleSettingChoice)10)

/* NumberFormatType - Specifies decimal and thousands separator characters
*/
#define lmChoiceNumberFormat				((LmLocaleSettingChoice)11)

/* Char[kMaxCurrencyNameLen+1] - Name of local currency (e.g., "US Dollar")
*/
#define lmChoiceCurrencyName				((LmLocaleSettingChoice)12)

/* Char[kMaxCurrencySymbolLen+1] - Currency symbol (e.g., "$")
*/
#define lmChoiceCurrencySymbol			((LmLocaleSettingChoice)13)

/* Char[kMaxCurrencySymbolLen+1] - Unique currency symbol (e.g., "US$")
*/
#define lmChoiceUniqueCurrencySymbol	((LmLocaleSettingChoice)14)

/* UInt16 - Number of decimals for currency (e.g., 2 for $10.12)
*/
#define lmChoiceCurrencyDecimalPlaces	((LmLocaleSettingChoice)15)

/* MeasurementSystemType - Metric, English, etc.
*/
#define lmChoiceMeasurementSystem		((LmLocaleSettingChoice)16)

/***********************************************************************
 * Locale Manager constants
 **********************************************************************/
#define lmAnyCountry					65535U	//	Pass LmLocaleToIndex's iLocale
#define lmAnyLanguage				65535U	//	Pass LmLocaleToIndex's iLocale

#define kMaxCountryNameLen			19
#if SUPPORT_LANGUAGE_NAME
	#define kMaxLanguageNameLen		19
#endif
#define kMaxCurrencyNameLen		19
#define kMaxCurrencySymbolLen		5

/***********************************************************************
 * Selectors & macros used for calling Locale Manager routines
 **********************************************************************/

#ifndef DIRECT_LOCALE_CALLS
	#define DIRECT_LOCALE_CALLS 0
#endif
#if DIRECT_LOCALE_CALLS
	#define	LMDISPATCH_TRAP(routineSelector)
#else
	#define	LMDISPATCH_TRAP(routineSelector)													\
		_SYSTEM_API(_CALL_WITH_SELECTOR)(_SYSTEM_TABLE,										\
													sysTrapLmDispatch,								\
													routineSelector)
#endif

/* Selectors used for getting to the right Locale Manager routine via
the LmDispatch trap.  */

// DOLATER:jwm: remove me after fixing LocaleMgr.c:PrvSelectorError
typedef UInt16 LmRoutineSelector;
#define lmInit							0
#define lmGetNumLocales				1
#define lmLocaleToIndex				2
#define lmGetLocaleSetting			3

#define lmMaxRoutineSelector		lmGetLocaleSetting

/***********************************************************************
 * Locale Manager types
 **********************************************************************/

typedef UInt8 LanguageType;
typedef UInt8 CountryType;

typedef struct _LmLocaleType LmLocaleType;
struct _LmLocaleType
{
	UInt16	language;		// Language spoken in locale (LanguageType)
	UInt16	country;			// Specifies "dialect" of language (CountryType)
};

/***********************************************************************
 * Locale Manager routines
 **********************************************************************/

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
	extern "C" {
#endif

/* Return the number of known locales (maximum locale index + 1).
*/
UInt16
LmGetNumLocales(void)
	LMDISPATCH_TRAP(lmGetNumLocales);

/* Convert <iLocale> to <oLocaleIndex> by locating it within the set of known
locales.
*/
Err
LmLocaleToIndex(		const
							LmLocaleType*	iLocale,
							UInt16*			oLocaleIndex)
	LMDISPATCH_TRAP(lmLocaleToIndex);

/* Return in <oValue> the setting identified by <iChoice> which is appropriate for
the locale identified by <iLocaleIndex>.  Return lmErrSettingDataOverflow if the
data for <iChoice> occupies more than <iValueSize> bytes.  Display a non-fatal
error if <iValueSize> is larger than the data for a fixed-size setting.
*/
Err
LmGetLocaleSetting(	UInt16			iLocaleIndex,
							LmLocaleSettingChoice iChoice,
							void*				oValue,
							UInt16			iValueSize)
	LMDISPATCH_TRAP(lmGetLocaleSetting);

#ifdef __cplusplus
	}
#endif
#endif

#endif
