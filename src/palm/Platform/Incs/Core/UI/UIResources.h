/******************************************************************************
 *
 * Copyright (c) 1995-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: UIResources.h
 *
 * Description:
 *	  This file defines UI resource types & ids.
 *
 * History:
 *		??/??/??	???	Created.
 *		06/29/99	CS		Added constantRscType & ResLoadConstant().
 *		07/07/99	kwk	Added fepFieldExtraBytesID, maxCategoryWidthID,
 *							extraStackSpaceID.
 *		07/09/99	kwk	Added silkscreenRscType & formRscType.
 *		07/12/99	kwk	Added sysFatalAlert.
 *		07/18/99	kwk	Added strListRscType, system string list resources.
 *		08/08/99	kwk	Added sysEditMenuJapAddWord/LookupWord.
 *		09/07/99	kwk	Added StrippedBase/GenericLaunchErrAlert
 *		09/17/99 jmp	Added a new NoteView form and menu to eliminate the goto
 *							top/bottom menu items and other extraneous UI elements
 *							that we no longer use in the built-in apps. We need to keep
 *							the old NoteView form and menu around for backwards
 *							compatibility.
 *		12/10/99	kwk	Deleted silkscreenRscType, use sysResTSilkscreen instead.
 *
 *****************************************************************************/

#ifndef __UIRESOURCES_H__
#define __UIRESOURCES_H__

#include <CoreTraps.h>

// System Default app icon (for apps missing a tAIB)
#define defaultAppIconBitmap				10000
#define defaultAppSmallIconBitmap		10001

//	System version string ID - this is hidden in
//	the SystemVersion.rsrc resource, because the 'system' resources
//	don't have ResEdit formats.
#define systemVersionID						10000


//------------------------------------------------------------
// Resource Type Constants 
//------------------------------------------------------------

#define strRsc 								'tSTR'
#define ainRsc 								'tAIN'
#define iconType								'tAIB'
#define bitmapRsc 							'Tbmp'
#define bsBitmapRsc 							'Tbsb'
#define alertRscType   						'Talt'
#define kbdRscType							'tkbd'
#define MenuRscType							'MBAR'
#define fontRscType							'NFNT'
#define verRsc									'tver'
#define appInfoStringsRsc					'tAIS'
#define fontIndexType						'fnti'
#define midiRsc								'MIDI'
#define colorTableRsc						'tclt'
#define MenuCtlRsc							'tcbr'
#define constantRscType						'tint'
#define formRscType							'tFRM'
#define strListRscType						'tSTL'
#define wrdListRscType						'wrdl'
#define defaultCategoryRscType			'taic'


//------------------------------------------------------------
// App Version Constants 
//------------------------------------------------------------

#define appVersionID							1		// our apps use tver 1 resource
#define appVersionAlternateID				1000	// CW Constructor uses tver 1000 resource
															// so we'll look for ours first, then try theirs
#define ainID									1000

#define oemVersionID						10001 // Per-DB version provided by OEMs

#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER kwk - should resource ids >= 10000 be in a private header file, so that
#endif // PUBLIC_STUFF_STRIPPED
// developers know they're not guaranteed to be around (or in the same format)?

//------------------------------------------------------------
// System Information Constants 
//------------------------------------------------------------

#define	fepFieldExtraBytesID				10000	// Extra bytes for expanded field if FEP is active.
#define	maxCategoryWidthID				10001	// Max pixel width for category trigger.
#define	extraStackSpaceID					10002	// Extra stack space for non-English locales

//------------------------------------------------------------
// System Alerts 
//------------------------------------------------------------

#define SelectACategoryAlert				10000

// This alert broke 1.0 applications and is now disabled until later.
// It is redefined below (10015).
//#define RemoveCategoryAlert				10001
//#define RemoveCategoryRecordsButton	0
//#define RemoveCategoryNameButton		1
//#define RemoveCategoryCancelButton	2

#define LowBatteryAlert						10002
#define VeryLowBatteryAlert				10003
#define UndoAlert								10004
#define UndoCancelButton					1

#define MergeCategoryAlert					10005
#define MergeCategoryYes					0
#define MergeCategoryNo						1

#define privateRecordInfoAlert			10006

#define ClipboardLimitAlert				10007

#define CategoryExistsAlert				10012

#define DeviceFullAlert						10013

#define categoryAllUsedAlert				10014

#define RemoveCategoryAlert				10015		// See alert 10001
#define RemoveCategoryYes					0
#define RemoveCategoryNo					1

#define DemoUnitAlert						10016

#define NoDataToBeamAlert					10017

// New for PalmOS 3.1
#define LowCradleChargedBatteryAlert		10018	// (Not present in Palm VII)
#define VeryLowCradleChargedBatteryAlert	10019	// (Not present in Palm VII)

// New for PalmOS 3.1 (Instant Karma only)
#define CategoryTooLongAlert				10020		// (Not present in Palm VII)

// New for PalmOS 3.2 - Alerts used by the ErrAlertCustom()  call.
#define ErrOKAlert							10021		// Error Alert with just an OK button
#define ErrOKCancelAlert					10022		// Error Alert with an OK & Cancel button
#define ErrCancelAlert						10023		// Error Alert with just Cancel button.  Special case for antenna down alert.
#define InfoOKAlert							10024		// Info alert with just an OK button
#define InfoOKCancelAlert					10025		// Info alert with an OK & Cancel button
#define InfoCancelAlert						10026		// Info alert with just a Cancel button
#define PrivacyWarningAlert				10027    // Privacy warning for weblib
#define ConfirmationOKAlert				10028		// Confirmation alert with just an OK button
#define ConfirmationOKCancelAlert		10029		// Confirmation alert with an OK & Cancel button
#define ConfirmationCancelAlert			10030		// Confirmation alert with just a Cancel button
#define WarningOKAlert						10031		// Warning Alert with just an OK button
#define WarningOKCancelAlert				10032		// Warning Alert with an OK & Cancel button
#define WarningCancelAlert					10033		// Warning Alert with just Cancel button.  Special case for antenna down alert.

// New for PalmOS 3.5 - Launch error alerts
#define StrippedBaseLaunchErrAlert		10034		// Launch error because of stripped base.
#define GenericLaunchErrAlert				10035		// Generic launch error.

// New for PalmOS 3.5 - Fatal Alert template
#define sysFatalAlert						10100		// Template for fatal alert

// New for PalmOS 3.5 - Alerts used by new security traps
#define secInvalidPasswordAlert								13250
#define secGotoInvalidRecordAlert							13251
#define secShowPrivatePermanentPassEntryAlert			13261
#define secShowMaskedPrivatePermanentPassEntryAlert	13265
#define secHideRecordsAlert									13268
#define secMaskRecordsAlert									13269
#define secHideMaskRecordsOK									0
#define secHideMaskRecordsCancel								1

// command-bar bitmaps
#define BarCutBitmap							10030
#define BarCopyBitmap						10031
#define BarPasteBitmap						10032
#define BarUndoBitmap						10033
#define BarBeamBitmap						10034
#define BarSecureBitmap						10035
#define BarDeleteBitmap						10036
#define BarInfoBitmap						10037

//Masking bitmaps
#define SecLockBitmap						10050
#define SecLockWidth							6
#define SecLockHeight						8

// System Menu Bar and Menus
#define sysEditMenuID						10000
#define sysEditMenuUndoCmd					10000
#define sysEditMenuCutCmd					10001
#define sysEditMenuCopyCmd					10002
#define sysEditMenuPasteCmd				10003
#define sysEditMenuSelectAllCmd			10004
#define sysEditMenuSeparator				10005
#define sysEditMenuKeyboardCmd			10006
#define sysEditMenuGraffitiCmd			10007

// Dynamically added to System Edit menu at runtime
#define sysEditMenuJapAddWord				10100
#define sysEditMenuJapLookupWord			10101

// Note View Menu Bar and Menus
#define noteMenuID							10200 // Old NoteView MenuBar
#define noteUndoCmd							sysEditMenuUndoCmd	
#define noteCutCmd							sysEditMenuCutCmd
#define noteCopyCmd							sysEditMenuCopyCmd
#define notePasteCmd							sysEditMenuPasteCmd
#define noteSelectAllCmd					sysEditMenuSelectAllCmd
#define noteSeparator						sysEditMenuSeparator
#define noteKeyboardCmd						sysEditMenuKeyboardCmd
#define noteGraffitiCmd						sysEditMenuKeyboardCmd

#define noteFontCmd							10200	// These are here for backwards
#define noteTopOfPageCmd					10201	// compatibility.  The built-in
#define noteBottomOfPageCmd				10202 // apps no longer use them.
#define notePhoneLookupCmd					10203

#define newNoteMenuID						10300 // The Edit Menu for the new NoteView.
#define newNoteFontCmd						10300	// MenuBar is the same as it is for
#define newNotePhoneLookupCmd				10301	// the old NoteView MenuBar.

// Note View (used by Datebook, To Do, Address, and Expense apps)
#define NoteView								10900	// The new NoteView is "new" as of Palm OS 3.5.
#define NewNoteView							10950 // Same as old NoteView, but points to newNoteMenuID and doesn't ref UI objects listed below.
#define NoteField								10901
#define NoteDoneButton						10902
#define NoteSmallFontButton				10903 // Not in NewNoteView, use FontCmd instead.
#define NoteLargeFontButton				10904 // Not in NewNoteView, use FontCmd instead.
#define NoteDeleteButton					10905
#define NoteUpButton							10906 // Not in NewNoteView, use scrollbars now.
#define NoteDownButton						10907 // Not in NewNoteView, use scrollbars now.
#define NoteScrollBar						10908
#define NoteFontGroup						1
#define noteViewMaxLength					4096	// not including null, tied to tFLD rsrc 10901


//	About Box - used by Datebook, Memo, Address, To Do, & others
#define aboutDialog							11000
#define aboutNameLabel						11001
#define aboutVersionLabel					11002
#define aboutErrorStr						11003


// Category New Name Dialog (used for new and renamed categories)
#define categoryNewNameDialog				11100
#define categoryNewNameField				11103
#define categoryNewNameOKButton			11104


// Categories Edit Dialog
#define CategoriesEditForm					10000
#define CategoriesEditList					10002
#define CategoriesEditOKButton			10003
#define CategoriesEditNewButton			10004
#define CategoriesEditRenameButton		10005
#define CategoriesEditDeleteButton		10006


// Graffiti Reference Dialog
#define graffitiReferenceDialog			11200
#define graffitiReferenceDoneButton		11202
#define graffitiReferenceUpButton		11203
#define graffitiReferenceDownButton		11204
#define graffitiReferenceFirstBitmap	11205


// System string resources
#define daysOfWeekStrID						10000		// OBSOLETE - use daysOfWeekStdStrListID
#define dayFullNamesStrID					10001		// OBSOLETE - use daysOfWeekLongStrListID
#define monthNamesStrID						10002		// OBSOLETE - use monthNamesStdStrListID
#define monthFullNamesStrID				10003		// OBSOLETE - use monthNamesLongStrListID
#define categoryAllStrID					10004
#define categoryEditStrID  				10005
#define menuCommandStrID					10006
#define launcherBatteryStrID				10007
#define systemNameStrID						10008
#define phoneLookupTitleStrID				10009
#define phoneLookupAddStrID				10010
#define phoneLookupFormatStrID			10011

// System string list resources
#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER kwk - put in error string defines here (range)
#endif // PUBLIC_STUFF_STRIPPED
#define daysOfWeekShortStrListID			10200
#define daysOfWeekStdStrListID			10201
#define daysOfWeekLongStrListID			10202
#define monthNamesShortStrListID			10203
#define monthNamesStdStrListID			10204
#define monthNamesLongStrListID			10205
#define prefDateFormatsStrListID			10206
#define prefDOWDateFormatsStrListID		10207


//------------------------------------------------------------
// Misc. resource routines 
//------------------------------------------------------------
#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

void *	ResLoadForm (UInt16 rscID)
						SYS_TRAP(sysTrapResLoadForm);

void *	ResLoadMenu (UInt16 rscID)
						SYS_TRAP(sysTrapResLoadMenu);


Char *	ResLoadString (UInt16 rscID);

UInt32	ResLoadConstant (UInt16 rscID)
						SYS_TRAP(sysTrapResLoadConstant);

#ifdef __cplusplus 
}
#endif
#endif

#endif // __UIRESOURCES_H__
