/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: DateTime.h
 *
 * Description:
 *		Date and Time calculations
 *
 * History:
 *    1/19/95  rsf - Created by Roger Flores
 *    7/15/99  rsf - moved some types in from Preferences.h
 *   12/23/99  jmp - eliminated bogus maxTime definition
 *
 *****************************************************************************/

#ifndef 	__DATETIME_H__
#define	__DATETIME_H__

#include <CoreTraps.h>

typedef enum 
	{
	tfColon,
	tfColonAMPM,			// 1:00 pm
	tfColon24h,				// 13:00
	tfDot,
	tfDotAMPM,				// 1.00 pm
	tfDot24h,				// 13.00
	tfHoursAMPM,			// 1 pm
	tfHours24h,				// 13
	tfComma24h				// 13,00
	} TimeFormatType;


typedef enum {
	dsNone,					// Daylight Savings Time not observed
	dsUSA,					// United States Daylight Savings Time
	dsAustralia,			// Australian Daylight Savings Time
	dsWesternEuropean,	// Western European Daylight Savings Time
	dsMiddleEuropean,		// Middle European Daylight Savings Time
	dsEasternEuropean,	// Eastern European Daylight Savings Time
	dsGreatBritain,		// Great Britain and Eire Daylight Savings Time
	dsRumania,				// Rumanian Daylight Savings Time
	dsTurkey,				// Turkish Daylight Savings Time
	dsAustraliaShifted	// Australian Daylight Savings Time with shift in 1986
	} DaylightSavingsTypes;


// pass a TimeFormatType	
#define Use24HourFormat(t) ((t) == tfColon24h || (t) == tfDot24h || (t) == tfHours24h || (t) == tfComma24h)
#define TimeSeparator(t) ((Char) ( t <= tfColon24h ? ':' : (t <= tfDot24h ? '.' : ',')))


typedef enum {
	dfMDYWithSlashes,		// 12/31/95
	dfDMYWithSlashes,		// 31/12/95
	dfDMYWithDots,			// 31.12.95
	dfDMYWithDashes,		// 31-12-95
	dfYMDWithSlashes,		// 95/12/31
	dfYMDWithDots,			// 95.12.31
	dfYMDWithDashes,		// 95-12-31

	dfMDYLongWithComma,	// Dec 31, 1995
	dfDMYLong,				// 31 Dec 1995
	dfDMYLongWithDot,		// 31. Dec 1995
	dfDMYLongNoDay,		// Dec 1995
	dfDMYLongWithComma,	//	31 Dec, 1995
	dfYMDLongWithDot,		//	1995.12.31
	dfYMDLongWithSpace,	//	1995 Dec 31

	dfMYMed,					//	Dec '95
	dfMYMedNoPost			//	Dec 95		(added for French 2.0 ROM)
	} DateFormatType;

typedef struct {
	Int16 second;
	Int16 minute;
	Int16 hour;
	Int16 day;
	Int16 month;
	Int16 year;
	Int16 weekDay;		// Days since Sunday (0 to 6)
	} DateTimeType;
	
typedef DateTimeType *DateTimePtr;


// This is the time format.  Times are treated as words so don't 
// change the order of the members in this structure.
//
typedef struct {
	UInt8        hours;
	UInt8        minutes;
} TimeType;

typedef TimeType *TimePtr;

#define noTime	-1		// The entire TimeType is -1 if there isn't a time.


// This is the date format.  Dates are treated as words so don't 
// change the order of the members in this structure.
//
typedef struct {
	UInt16 year  :7;                   // years since 1904 (MAC format)
	UInt16 month :4; 
	UInt16 day   :5;
} DateType;

typedef DateType *DatePtr;


/************************************************************
 * Date Time Constants
 *************************************************************/

// Maximum lengths of strings return by the date and time formating
// routine DateToAscii and TimeToAscii.
#define timeStringLength		9
#define dateStringLength		9
#define longDateStrLength		15
#define dowDateStringLength	19
#define dowLongDateStrLength	25


#define firstYear				1904
#define numberOfYears		128
#define lastYear				(firstYear + numberOfYears - 1)



// Constants for time calculations
// Could change these from xIny to yPerX
#define secondsInSeconds	1
#define minutesInSeconds	60
#define hoursInMinutes		60
#define hoursInSeconds		(hoursInMinutes * minutesInSeconds)
#define hoursPerDay			24
//#define daysInSeconds		((Int32)(hoursPerDay) * ((Int32)hoursInSeconds))
#define daysInSeconds		(0x15180)	// cc bug

#define daysInWeek			7
#define daysInYear			365
#define daysInLeapYear		366
#define daysInFourYears		(daysInLeapYear + 3 * daysInYear)

#define monthsInYear			12

#define maxDays				((UInt32) numberOfYears / 4 * daysInFourYears - 1)
#define maxSeconds			((UInt32) maxDays * daysInSeconds)

// Values returned by DayOfWeek routine.
#define sunday					0
#define monday					1
#define tuesday				2
#define wednesday				3
#define thursday				4
#define friday					5
#define saturday				6

// Months of the year
#define january				1
#define february				2
#define march					3
#define april					4
#define may						5
#define june					6
#define july					7
#define august					8
#define september				9
#define october				10
#define november				11
#define december				12


// Values returned by DoyOfMonth routine.
typedef enum {
	dom1stSun, dom1stMon, dom1stTue, dom1stWen, dom1stThu, dom1stFri, dom1stSat,
	dom2ndSun, dom2ndMon, dom2ndTue, dom2ndWen, dom2ndThu, dom2ndFri, dom2ndSat,
	dom3rdSun, dom3rdMon, dom3rdTue, dom3rdWen, dom3rdThu, dom3rdFri, dom3rdSat,
	dom4thSun, dom4thMon, dom4thTue, dom4thWen, dom4thThu, dom4thFri, dom4thSat,
	domLastSun, domLastMon, domLastTue, domLastWen, domLastThu, domLastFri, 
	domLastSat
	} DayOfWeekType;

// Values used by DateTemplateToAscii routine.
#define dateTemplateChar					chrCircumflexAccent

enum {
	dateTemplateDayNum = '0',
	dateTemplateDOWName,
	dateTemplateMonthName,
	dateTemplateMonthNum,
	dateTemplateYearNum
};

#define	dateTemplateShortModifier		's'
#define	dateTemplateRegularModifier	'r'
#define	dateTemplateLongModifier		'l'
#define	dateTemplateLeadZeroModifier	'z'

//************************************************************
//* Date and Time macros
//***********************************************************

// Convert a date in a DateType structure to an UInt16.
 #define DateToInt(date) (*(UInt16 *) &date)
 
 
// Convert a date in a DateType structure to a signed int.
 #define TimeToInt(time) (*(Int16 *) &time)
 
 

//************************************************************
//* Date Time procedures
//************************************************************
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif


void TimSecondsToDateTime(UInt32 seconds, DateTimePtr dateTimeP)
			SYS_TRAP(sysTrapTimSecondsToDateTime);

UInt32 TimDateTimeToSeconds(DateTimePtr dateTimeP)
			SYS_TRAP(sysTrapTimDateTimeToSeconds);

void TimAdjust(DateTimePtr dateTimeP, Int32 adjustment)
			SYS_TRAP(sysTrapTimAdjust);

void TimeToAscii(UInt8 hours, UInt8 minutes, TimeFormatType timeFormat, 
				Char *pString)
			SYS_TRAP(sysTrapTimeToAscii);



Int16 DaysInMonth(Int16 month, Int16 year)
			SYS_TRAP(sysTrapDaysInMonth);

Int16 DayOfWeek (Int16 month, Int16 day, Int16 year)
			SYS_TRAP(sysTrapDayOfWeek);

Int16 DayOfMonth (Int16 month, Int16 day, Int16 year)
			SYS_TRAP(sysTrapDayOfMonth);



// Date routines.
void DateSecondsToDate (UInt32 seconds, DatePtr date)
			SYS_TRAP(sysTrapDateSecondsToDate);

void DateDaysToDate (UInt32 days, DatePtr date)
			SYS_TRAP(sysTrapDateDaysToDate);

UInt32 DateToDays (DateType date)
			SYS_TRAP(sysTrapDateToDays);

void DateAdjust (DatePtr dateP, Int32 adjustment)
			SYS_TRAP(sysTrapDateAdjust);

void DateToAscii(UInt8 months, UInt8 days, UInt16 years, 
				DateFormatType dateFormat, Char *pString)
			SYS_TRAP(sysTrapDateToAscii);

void DateToDOWDMFormat(UInt8 months, UInt8 days,  UInt16 years,
				DateFormatType dateFormat, Char *pString)
			SYS_TRAP(sysTrapDateToDOWDMFormat);

UInt16 DateTemplateToAscii(const Char *templateP, UInt8 months,
				UInt8 days, UInt16 years, Char *stringP, Int16 stringLen)
			SYS_TRAP(sysTrapDateTemplateToAscii);

#ifdef __cplusplus 
}
#endif
#endif


#endif //__DATETIME_H__
