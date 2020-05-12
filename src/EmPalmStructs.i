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

// This file contains the macros that define the fields of structs in a
// way that allows us to create wrapper classes that access those fields
// in a binary-compatible, cross-platform fashion.
//
// This file is #included by EmPalmStructs.h.  The macros are stored in
// this separate .i file merely because of problems trying to edit them
// if they're stored in EmPalmStructs.h.  Visual Studio's editor seems
// to get bogged down when trying to edit a source file containing many
// large macros.  It frequently "goes to lunch" for a second or so,
// making editing, selecting, scrolling, etc., very frustrating.  Placing
// the macros in a .i file -- which the editor doesn't seem to try to
// parse -- eliminates this problem.


// Define macros that allow us to iterate over all the fields in a struct.
// The various columns are:
//
//		1:	field offset
//		2:	field type
//		3:	field name

/*
	# Format the first column
	find ¥; replace -c ° /(¶tDO_TO_FIELD ¶([0-9]Ç1,1È,)¨1[¶t]+(Å)¨2/	"¨1¶t¶t¨2"
	find ¥; replace -c ° /(¶tDO_TO_FIELD ¶([0-9]Ç2,4È,)¨1[¶t]+(Å)¨2/	"¨1¶t¨2"
	
	# Format the second column
	find ¥; replace -c ° /(Å,[¶t]+[_a-z0-9]Ç1,2È,)¨1[¶t]+(Å)¨2/		"¨1¶t¶t¶t¶t¶t¶t¶t¨2"
	find ¥; replace -c ° /(Å,[¶t]+[_a-z0-9]Ç3,6È,)¨1[¶t]+(Å)¨2/		"¨1¶t¶t¶t¶t¶t¶t¨2"
	find ¥; replace -c ° /(Å,[¶t]+[_a-z0-9]Ç7,10È,)¨1[¶t]+(Å)¨2/	"¨1¶t¶t¶t¶t¶t¨2"
	find ¥; replace -c ° /(Å,[¶t]+[_a-z0-9]Ç11,14È,)¨1[¶t]+(Å)¨2/	"¨1¶t¶t¶t¶t¨2"
	find ¥; replace -c ° /(Å,[¶t]+[_a-z0-9]Ç15,18È,)¨1[¶t]+(Å)¨2/	"¨1¶t¶t¶t¨2"
	find ¥; replace -c ° /(Å,[¶t]+[_a-z0-9]Ç19,22È,)¨1[¶t]+(Å)¨2/	"¨1¶t¶t¨2"
	find ¥; replace -c ° /(Å,[¶t]+[_a-z0-9]Ç23,99È,)¨1[¶t]+(Å)¨2/	"¨1¶t¨2"
	
	# Format the third column
	find ¥; replace -c ° /(,[¶t]+[_a-z0-9]Ç1,2È¶))¨5[¶t]+\/		"¨5¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(,[¶t]+[_a-z0-9]Ç3,6È¶))¨5[¶t]+\/		"¨5¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(,[¶t]+[_a-z0-9]Ç7,10È¶))¨5[¶t]+\/	"¨5¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(,[¶t]+[_a-z0-9]Ç11,14È¶))¨5[¶t]+\/	"¨5¶t¶t¶t¶t\"
	find ¥; replace -c ° /(,[¶t]+[_a-z0-9]Ç15,18È¶))¨5[¶t]+\/	"¨5¶t¶t¶t\"
	find ¥; replace -c ° /(,[¶t]+[_a-z0-9]Ç19,22È¶))¨5[¶t]+\/	"¨5¶t¶t\"
	find ¥; replace -c ° /(,[¶t]+[_a-z0-9]Ç23,99È¶))¨5[¶t]+\/	"¨5¶t\"
	
	# Format the #define lines
	find ¥; replace -c ° /(¶#define [Â¶t]Ç1,3È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç4,7È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç8,11È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç12,15È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç16,19È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç20,23È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç24,27È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç28,31È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç32,35È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç36,39È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç40,43È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç44,47È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç48,51È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç52,55È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç56,59È)¨1[¶t]+\/		"¨1¶t¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç60,63È)¨1[¶t]+\/		"¨1¶t¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç64,67È)¨1[¶t]+\/		"¨1¶t¶t\"
	find ¥; replace -c ° /(¶#define [Â¶t]Ç68,99È)¨1[¶t]+\/		"¨1¶t\"
*/

// ========================================
// ===== generic =====
// ========================================

#define FOR_generic_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (generic, 16, FOR_generic_FIELDS)

#define FOR_generic_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						datum)						\


// ========================================
// ===== penUp =====
// ========================================

#define FOR_penUp_STRUCT(DO_TO_STRUCT)											\
	DO_TO_STRUCT (penUp, 8, FOR_penUp_FIELDS)

#define FOR_penUp_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		PointType,					start)						\
	DO_TO_FIELD (4,		PointType,					end)						\


// ========================================
// ===== keyDown =====
// ========================================

#define FOR_keyDown_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (keyDown, 6, FOR_keyDown_FIELDS)

#define FOR_keyDown_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		WChar,						chr)						\
	DO_TO_FIELD (2,		UInt16,						keyCode)					\
	DO_TO_FIELD (4,		UInt16,						modifiers)					\


// ========================================
// ===== winEnter =====
// ========================================

#define FOR_winEnter_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (winEnter, 8, FOR_winEnter_FIELDS)

#define FOR_winEnter_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		emuptr,						enterWindow)				\
	DO_TO_FIELD (4,		emuptr,						exitWindow)					\


// ========================================
// ===== winExit =====
// ========================================

#define FOR_winExit_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (winExit, 8, FOR_winExit_FIELDS)

#define FOR_winExit_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		emuptr,						enterWindow)				\
	DO_TO_FIELD (4,		emuptr,						exitWindow)					\


// ========================================
// ===== tsmConfirm =====
// ========================================

#define FOR_tsmConfirm_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (tsmConfirm, 6, FOR_tsmConfirm_FIELDS)

#define FOR_tsmConfirm_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		emuptr,						yomiText)					\
	DO_TO_FIELD (4,		UInt16,						formID)						\


// ========================================
// ===== tsmFepButton =====
// ========================================

#define FOR_tsmFepButton_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (tsmFepButton, 2, FOR_tsmFepButton_FIELDS)

#define FOR_tsmFepButton_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						buttonID)					\


// ========================================
// ===== tsmFepMode =====
// ========================================

#define FOR_tsmFepMode_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (tsmFepMode, 2, FOR_tsmFepMode_FIELDS)

#define FOR_tsmFepMode_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						mode)						\


// ========================================
// ===== ctlEnter =====
// ========================================

#define FOR_ctlEnter_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (ctlEnter, 6, FOR_ctlEnter_FIELDS)

#define FOR_ctlEnter_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Int16,						controlID)					\
	DO_TO_FIELD (2,		emuptr,						pControl)					\


// ========================================
// ===== ctlSelect =====
// ========================================

#define FOR_ctlSelect_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (ctlSelect, 10, FOR_ctlSelect_FIELDS)

#define FOR_ctlSelect_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Int16,						controlID)					\
	DO_TO_FIELD (2,		emuptr,						pControl)					\
	DO_TO_FIELD (6,		Boolean,					on)							\
	DO_TO_FIELD (7,		UInt8,						reserved1)					\
	DO_TO_FIELD (8,		UInt16,						value)						\


// ========================================
// ===== ctlRepeat =====
// ========================================

#define FOR_ctlRepeat_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (ctlRepeat, 12, FOR_ctlRepeat_FIELDS)

#define FOR_ctlRepeat_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Int16,						controlID)					\
	DO_TO_FIELD (2,		emuptr,						pControl)					\
	DO_TO_FIELD (6,		UInt32,						time)						\
	DO_TO_FIELD (10,	UInt16,						value)						\


// ========================================
// ===== ctlExit =====
// ========================================

#define FOR_ctlExit_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (ctlExit, 6, FOR_ctlExit_FIELDS)

#define FOR_ctlExit_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		Int16,						controlID)					\
	DO_TO_FIELD (2,		emuptr,						pControl)					\


// ========================================
// ===== fldEnter =====
// ========================================

#define FOR_fldEnter_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (fldEnter, 6, FOR_fldEnter_FIELDS)

#define FOR_fldEnter_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						fieldID)					\
	DO_TO_FIELD (2,		emuptr,						pField)						\


// ========================================
// ===== fldHeightChanged =====
// ========================================

#define FOR_fldHeightChanged_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (fldHeightChanged, 10, FOR_fldHeightChanged_FIELDS)

#define FOR_fldHeightChanged_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						fieldID)					\
	DO_TO_FIELD (2,		emuptr,						pField)						\
	DO_TO_FIELD (6,		Int16,						newHeight)					\
	DO_TO_FIELD (8,		UInt16,						currentPos)					\


// ========================================
// ===== fldChanged =====
// ========================================

#define FOR_fldChanged_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (fldChanged, 6, FOR_fldChanged_FIELDS)

#define FOR_fldChanged_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						fieldID)					\
	DO_TO_FIELD (2,		emuptr,						pField)						\


// ========================================
// ===== fldExit =====
// ========================================

#define FOR_fldExit_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (fldExit, 6, FOR_fldExit_FIELDS)

#define FOR_fldExit_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						fieldID)					\
	DO_TO_FIELD (2,		emuptr,						pField)						\


// ========================================
// ===== lstEnter =====
// ========================================

#define FOR_lstEnter_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (lstEnter, 8, FOR_lstEnter_FIELDS)

#define FOR_lstEnter_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						listID)						\
	DO_TO_FIELD (2,		emuptr,						pList)						\
	DO_TO_FIELD (6,		Int16,						selection)					\


// ========================================
// ===== lstExit =====
// ========================================

#define FOR_lstExit_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (lstExit, 6, FOR_lstExit_FIELDS)

#define FOR_lstExit_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						listID)						\
	DO_TO_FIELD (2,		emuptr,						pList)						\


// ========================================
// ===== lstSelect =====
// ========================================

#define FOR_lstSelect_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (lstSelect, 8, FOR_lstSelect_FIELDS)

#define FOR_lstSelect_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						listID)						\
	DO_TO_FIELD (2,		emuptr,						pList)						\
	DO_TO_FIELD (6,		Int16,						selection)					\


// ========================================
// ===== tblEnter =====
// ========================================

#define FOR_tblEnter_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (tblEnter, 10, FOR_tblEnter_FIELDS)

#define FOR_tblEnter_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						tableID)					\
	DO_TO_FIELD (2,		emuptr,						pTable)						\
	DO_TO_FIELD (6,		Int16,						row)						\
	DO_TO_FIELD (8,		Int16,						column)						\


// ========================================
// ===== tblExit =====
// ========================================

#define FOR_tblExit_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (tblExit, 10, FOR_tblExit_FIELDS)

#define FOR_tblExit_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						tableID)					\
	DO_TO_FIELD (2,		emuptr,						pTable)						\
	DO_TO_FIELD (6,		Int16,						row)						\
	DO_TO_FIELD (8,		Int16,						column)						\


// ========================================
// ===== tblSelect =====
// ========================================

#define FOR_tblSelect_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (tblSelect, 10, FOR_tblSelect_FIELDS)

#define FOR_tblSelect_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						tableID)					\
	DO_TO_FIELD (2,		emuptr,						pTable)						\
	DO_TO_FIELD (6,		Int16,						row)						\
	DO_TO_FIELD (8,		Int16,						column)						\


// ========================================
// ===== frmLoad =====
// ========================================

#define FOR_frmLoad_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (frmLoad, 2, FOR_frmLoad_FIELDS)

#define FOR_frmLoad_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						formID)						\


// ========================================
// ===== frmOpen =====
// ========================================

#define FOR_frmOpen_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (frmOpen, 2, FOR_frmOpen_FIELDS)

#define FOR_frmOpen_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						formID)						\


// ========================================
// ===== frmGoto =====
// ========================================

#define FOR_frmGoto_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (frmGoto, 14, FOR_frmGoto_FIELDS)

#define FOR_frmGoto_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						formID)						\
	DO_TO_FIELD (2,		UInt16,						recordNum)					\
	DO_TO_FIELD (4,		UInt16,						matchPos)					\
	DO_TO_FIELD (6,		UInt16,						matchLen)					\
	DO_TO_FIELD (8,		UInt16,						matchFieldNum)				\
	DO_TO_FIELD (10,	UInt32,						matchCustom)				\


// ========================================
// ===== frmClose =====
// ========================================

#define FOR_frmClose_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (frmClose, 2, FOR_frmClose_FIELDS)

#define FOR_frmClose_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						formID)						\


// ========================================
// ===== frmUpdate =====
// ========================================

#define FOR_frmUpdate_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (frmUpdate, 4, FOR_frmUpdate_FIELDS)

#define FOR_frmUpdate_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						formID)						\
	DO_TO_FIELD (2,		UInt16,						updateCode)					\


// ========================================
// ===== frmTitleEnter =====
// ========================================

#define FOR_frmTitleEnter_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (frmTitleEnter, 2, FOR_frmTitleEnter_FIELDS)

#define FOR_frmTitleEnter_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						formID)						\


// ========================================
// ===== frmTitleSelect =====
// ========================================

#define FOR_frmTitleSelect_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (frmTitleSelect, 2, FOR_frmTitleSelect_FIELDS)

#define FOR_frmTitleSelect_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						formID)						\


// ========================================
// ===== attnIndicatorEnter =====
// ========================================

#define FOR_attnIndicatorEnter_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (attnIndicatorEnter, 2, FOR_attnIndicatorEnter_FIELDS)

#define FOR_attnIndicatorEnter_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						formID)						\


// ========================================
// ===== attnIndicatorSelect =====
// ========================================

#define FOR_attnIndicatorSelect_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (attnIndicatorSelect, 2, FOR_attnIndicatorSelect_FIELDS)

#define FOR_attnIndicatorSelect_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						formID)						\


// ========================================
// ===== daySelect =====
// ========================================

#define FOR_daySelect_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (daySelect, 8, FOR_daySelect_FIELDS)

#define FOR_daySelect_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		emuptr,						pSelector)					\
	DO_TO_FIELD (4,		Int16,						selection)					\
	DO_TO_FIELD (6,		Boolean,					useThisDate)				\
	DO_TO_FIELD (7,		UInt8,						reserved1)					\


// ========================================
// ===== menu =====
// ========================================

#define FOR_menu_STRUCT(DO_TO_STRUCT)											\
	DO_TO_STRUCT (menu, 2, FOR_menu_FIELDS)

#define FOR_menu_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						itemID)						\


// ========================================
// ===== popSelect =====
// ========================================

#define FOR_popSelect_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (popSelect, 16, FOR_popSelect_FIELDS)

#define FOR_popSelect_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						controlID)					\
	DO_TO_FIELD (2,		emuptr,						controlP)					\
	DO_TO_FIELD (6,		UInt16,						listID)						\
	DO_TO_FIELD (8,		emuptr,						listP)						\
	DO_TO_FIELD (12,	Int16,						selection)					\
	DO_TO_FIELD (14,	Int16,						priorSelection)				\


// ========================================
// ===== sclEnter =====
// ========================================

#define FOR_sclEnter_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (sclEnter, 6, FOR_sclEnter_FIELDS)

#define FOR_sclEnter_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						scrollBarID)				\
	DO_TO_FIELD (2,		emuptr,						pScrollBar)					\


// ========================================
// ===== sclExit =====
// ========================================

#define FOR_sclExit_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (sclExit, 10, FOR_sclExit_FIELDS)

#define FOR_sclExit_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		UInt16,						scrollBarID)				\
	DO_TO_FIELD (2,		emuptr,						pScrollBar)					\
	DO_TO_FIELD (6,		Int16,						value)						\
	DO_TO_FIELD (8,		Int16,						newValue)					\


// ========================================
// ===== sclRepeat =====
// ========================================

#define FOR_sclRepeat_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (sclRepeat, 14, FOR_sclRepeat_FIELDS)

#define FOR_sclRepeat_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						scrollBarID)				\
	DO_TO_FIELD (2,		emuptr,						pScrollBar)					\
	DO_TO_FIELD (6,		Int16,						value)						\
	DO_TO_FIELD (8,		Int16,						newValue)					\
	DO_TO_FIELD (10,	Int32,						time)						\


// ========================================
// ===== menuCmdBarOpen =====
// ========================================

#define FOR_menuCmdBarOpen_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (menuCmdBarOpen, 2, FOR_menuCmdBarOpen_FIELDS)

#define FOR_menuCmdBarOpen_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		Boolean,					preventFieldButtons)		\
	DO_TO_FIELD (1,		UInt8,						reserved)					\


// ========================================
// ===== menuOpen =====
// ========================================

#define FOR_menuOpen_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (menuOpen, 4, FOR_menuOpen_FIELDS)

#define FOR_menuOpen_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						menuRscID)					\
	DO_TO_FIELD (2,		Int16,						cause)						\


// ========================================
// ===== gadgetEnter =====
// ========================================

#define FOR_gadgetEnter_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (gadgetEnter, 6, FOR_gadgetEnter_FIELDS)

#define FOR_gadgetEnter_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						gadgetID)					\
	DO_TO_FIELD (2,		emuptr,						gadgetP)					\


// ========================================
// ===== gadgetMisc =====
// ========================================

#define FOR_gadgetMisc_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (gadgetMisc, 12, FOR_gadgetMisc_FIELDS)

#define FOR_gadgetMisc_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						gadgetID)					\
	DO_TO_FIELD (2,		emuptr,						gadgetP)					\
	DO_TO_FIELD (6,		UInt16,						selector)					\
	DO_TO_FIELD (8,		emuptr,						dataP)						\


// ========================================
// ===== AbsRectType =====
// ========================================

#define FOR_AbsRectType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (AbsRectType, 8, FOR_AbsRectType_FIELDS)

#define FOR_AbsRectType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Coord,						left)						\
	DO_TO_FIELD (2,		Coord,						top)						\
	DO_TO_FIELD (4,		Coord,						right)						\
	DO_TO_FIELD (6,		Coord,						bottom)						\


// ========================================
// ===== BitmapTypeV2 =====
// ========================================

#define FOR_BitmapTypeV2_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (BitmapTypeV2, 16, FOR_BitmapTypeV2_FIELDS)

#define FOR_BitmapTypeV2_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		Int16,						width)						\
	DO_TO_FIELD (2,		Int16,						height)						\
	DO_TO_FIELD (4,		UInt16,						rowBytes)					\
	DO_TO_FIELD (6,		UInt16,						flags)						\
	DO_TO_FIELD (8,		UInt8,						pixelSize)					\
	DO_TO_FIELD (9,		UInt8,						version)					\
	DO_TO_FIELD (10,	UInt16,						nextDepthOffset)			\
	DO_TO_FIELD (12,	UInt8,						transparentIndex)			\
	DO_TO_FIELD (13,	UInt8,						compressionType)			\
	DO_TO_FIELD (14,	UInt16,						reserved)					\


// ========================================
// ===== BitmapTypeV3 =====
// ========================================

#define FOR_BitmapTypeV3_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (BitmapTypeV3, 24, FOR_BitmapTypeV3_FIELDS)

#define FOR_BitmapTypeV3_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		Int16,						width)						\
	DO_TO_FIELD (2,		Int16,						height)						\
	DO_TO_FIELD (4,		UInt16,						rowBytes)					\
	DO_TO_FIELD (6,		UInt16,						flags)						\
	DO_TO_FIELD (8,		UInt8,						pixelSize)					\
	DO_TO_FIELD (9,		UInt8,						version)					\
	DO_TO_FIELD (10,	UInt8,						size)						\
	DO_TO_FIELD (11,	UInt8,						pixelFormat)				\
	DO_TO_FIELD (12,	UInt8,						unused)						\
	DO_TO_FIELD (13,	UInt8,						compressionType)			\
	DO_TO_FIELD (14,	UInt16,						density)					\
	DO_TO_FIELD (16,	UInt32,						transparentValue)			\
	DO_TO_FIELD (20,	UInt32,						nextDepthOffset)			\


// ========================================
// ===== BreakpointType =====
// ========================================

#define FOR_BreakpointType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (BreakpointType, 6, FOR_BreakpointType_FIELDS)

#define FOR_BreakpointType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						addr)						\
	DO_TO_FIELD (4,		Boolean,					enabled)					\
	DO_TO_FIELD (5,		Boolean,					installed)					\


// ========================================
// ===== CardHeaderType =====
// ========================================

#define FOR_CardHeaderType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (CardHeaderType, 256, FOR_CardHeaderType_FIELDS)

#define FOR_CardHeaderType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt32,					initStack)						\
	DO_TO_FIELD (4,		UInt32,					resetVector)					\
	DO_TO_FIELD (8,		UInt32,					signature)						\
	DO_TO_FIELD (12,	UInt16,					hdrVersion)						\
	DO_TO_FIELD (14,	UInt16,					flags)							\
	DO_TO_FIELD (16,	UInt8,					name)							\
	DO_TO_FIELD (48,	UInt8,					manuf)							\
	DO_TO_FIELD (80,	UInt16,					version)						\
	DO_TO_FIELD (82,	UInt32,					creationDate)					\
	DO_TO_FIELD (86,	UInt16,					numRAMBlocks)					\
	DO_TO_FIELD (88,	UInt32,					blockListOffset)				\
	DO_TO_FIELD (92,	UInt32,					readWriteParmsOffset)			\
	DO_TO_FIELD (96,	UInt32,					readWriteParmsSize)				\
	DO_TO_FIELD (100,	UInt32,					readOnlyParmsOffset)			\
	DO_TO_FIELD (104,	UInt32,					bigROMOffset)					\
	DO_TO_FIELD (108,	UInt32,					checksumBytes)					\
	DO_TO_FIELD (112,	UInt16,					checksumValue)					\
	DO_TO_FIELD (114,	UInt32,					readWriteWorkingOffset)			\
	DO_TO_FIELD (118,	UInt32,					readWriteWorkingSize)			\
	DO_TO_FIELD (122,	UInt32,					halCodeOffset)					\


// ========================================
// ===== CardInfoType =====
// ========================================

#define FOR_CardInfoType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (CardInfoType, 48, FOR_CardInfoType_FIELDS)

#define FOR_CardInfoType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,					baseP)							\
	DO_TO_FIELD (4,		UInt32,					size)							\
	DO_TO_FIELD (8,		UInt32,					firstRAMBlockSize)				\
	DO_TO_FIELD (12,	UInt32,					targetROMShift)					\
	DO_TO_FIELD (16,	UInt32,					cardHeaderOffset)				\
	DO_TO_FIELD (20,	UInt32,					rsvSpace)						\
	DO_TO_FIELD (24,	UInt32,					dynHeapSpace)					\
	DO_TO_FIELD (28,	emuptr,					ramStoreP)						\
	DO_TO_FIELD (32,	UInt16,					numRAMHeaps)					\
	DO_TO_FIELD (34,	emuptr,					ramHeapOffsetsP)				\
	DO_TO_FIELD (38,	UInt16,					numROMHeaps)					\
	DO_TO_FIELD (40,	emuptr,					romHeapOffsetsP)				\
	DO_TO_FIELD (44,	UInt32,					cardOffsetMask)					\


// ========================================
// ===== ClipboardItem =====
// ========================================

#define FOR_ClipboardItem_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (ClipboardItem, 6, FOR_ClipboardItem_FIELDS)

#define FOR_ClipboardItem_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						item)						\
	DO_TO_FIELD (4,		UInt16,						length)						\


// ========================================
// ===== ControlAttrType =====
// ========================================

#define FOR_ControlAttrType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (ControlAttrType, 2, FOR_ControlAttrType_FIELDS)

#define FOR_ControlAttrType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		Int16,						flags)						\


// ========================================
// ===== ControlType =====
// ========================================

#define FOR_ControlType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (ControlType, 20, FOR_ControlType_FIELDS)

#define FOR_ControlType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Int16,						id)							\
	DO_TO_FIELD (2,		RectangleType,				bounds)						\
	DO_TO_FIELD (10,	emuptr,						text)						\
	DO_TO_FIELD (14,	ControlAttrType,			attr)						\
	DO_TO_FIELD (16,	ControlStyleType,			style)						\
	DO_TO_FIELD (17,	FontID,						font)						\
	DO_TO_FIELD (18,	UInt8,						group)						\
	DO_TO_FIELD (19,	UInt8,						reserved)					\


// ========================================
// ===== DatabaseDirEntryType =====
// ========================================

#define FOR_DatabaseDirEntryType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (DatabaseDirEntryType, 4, FOR_DatabaseDirEntryType_FIELDS)

#define FOR_DatabaseDirEntryType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		LocalID,					baseID)						\


// ========================================
// ===== DatabaseDirType =====
// ========================================

#define FOR_DatabaseDirType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (DatabaseDirType, 10, FOR_DatabaseDirType_FIELDS)

#define FOR_DatabaseDirType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		LocalID,					nextDatabaseListID)			\
	DO_TO_FIELD (4,		UInt16,						numDatabases)				\
	DO_TO_FIELD (6,		DatabaseDirEntryType,		databaseID)					\


// ========================================
// ===== DatabaseHdrType =====
// ========================================

#define FOR_DatabaseHdrType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (DatabaseHdrType, 78, FOR_DatabaseHdrType_FIELDS)

#define FOR_DatabaseHdrType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		Int16,						name)						\
	DO_TO_FIELD (32,	UInt16,						attributes)					\
	DO_TO_FIELD (34,	UInt16,						version)					\
	DO_TO_FIELD (36,	UInt32,						creationDate)				\
	DO_TO_FIELD (40,	UInt32,						modificationDate)			\
	DO_TO_FIELD (44,	UInt32,						lastBackupDate)				\
	DO_TO_FIELD (48,	UInt32,						modificationNumber)			\
	DO_TO_FIELD (52,	LocalID,					appInfoID)					\
	DO_TO_FIELD (56,	LocalID,					sortInfoID)					\
	DO_TO_FIELD (60,	UInt32,						type)						\
	DO_TO_FIELD (64,	UInt32,						creator)					\
	DO_TO_FIELD (68,	UInt32,						uniqueIDSeed)				\
	DO_TO_FIELD (72,	RecordListType,				recordList)					\


// ========================================
// ===== DlkDBCreatorList =====
// ========================================

#define FOR_DlkDBCreatorList_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (DlkDBCreatorList, 6, FOR_DlkDBCreatorList_FIELDS)

#define FOR_DlkDBCreatorList_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						count)						\
	DO_TO_FIELD (2,		emuptr,						listH)						\


// ========================================
// ===== DlkServerSessionType =====
// ========================================

#define FOR_DlkServerSessionType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (DlkServerSessionType, 146, FOR_DlkServerSessionType_FIELDS)

#define FOR_DlkServerSessionType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt16,						htalLibRefNum)				\
	DO_TO_FIELD (2,		UInt32,						maxHtalXferSize)			\
	DO_TO_FIELD (6,		emuptr,						eventProcP)					\
	DO_TO_FIELD (10,	UInt32,						eventRef)					\
	DO_TO_FIELD (14,	emuptr,						canProcP)					\
	DO_TO_FIELD (18,	UInt32,						canRef)						\
	DO_TO_FIELD (22,	emuptr,						condFilterH)				\
	DO_TO_FIELD (26,	UInt8,						dlkDBID)					\
	DO_TO_FIELD (27,	UInt8,						reserved1)					\
	DO_TO_FIELD (28,	emuptr,						dbR)						\
	DO_TO_FIELD (32,	UInt16,						cardNo)						\
	DO_TO_FIELD (34,	UInt32,						dbCreator)					\
	DO_TO_FIELD (38,	Char,						dbName)						\
	DO_TO_FIELD (70,	UInt16,						dbOpenMode)					\
	DO_TO_FIELD (72,	Boolean,					created)					\
	DO_TO_FIELD (73,	Boolean,					isResDB)					\
	DO_TO_FIELD (74,	Boolean,					ramBased)					\
	DO_TO_FIELD (75,	Boolean,					readOnly)					\
	DO_TO_FIELD (76,	LocalID,					dbLocalID)					\
	DO_TO_FIELD (80,	UInt32,						initialModNum)				\
	DO_TO_FIELD (84,	UInt32,						curRecIndex)				\
	DO_TO_FIELD (88,	DlkDBCreatorList,			creatorList)				\
	DO_TO_FIELD (94,	UInt8,						syncState)					\
	DO_TO_FIELD (95,	Boolean,					complete)					\
	DO_TO_FIELD (96,	Boolean,					conduitOpened)				\
	DO_TO_FIELD (97,	Boolean,					logCleared)					\
	DO_TO_FIELD (98,	Boolean,					resetPending)				\
	DO_TO_FIELD (99,	Boolean,					gotCommand)					\
	DO_TO_FIELD (100,	UInt8,						cmdTID)						\
	DO_TO_FIELD (101,	UInt8,						reserved2)					\
	DO_TO_FIELD (102,	UInt16,						cmdLen)						\
	DO_TO_FIELD (104,	emuptr,						cmdP)						\
	DO_TO_FIELD (108,	emuptr,						cmdH)						\
	DO_TO_FIELD (112,	UInt16,						wStateFlags)				\
	DO_TO_FIELD (114,	DmSearchStateType,			dbSearchState)				\


// ========================================
// ===== DmAccessType =====
// ========================================

#define FOR_DmAccessType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (DmAccessType, 16, FOR_DmAccessType_FIELDS)

#define FOR_DmAccessType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						next)						\
	DO_TO_FIELD (4,		UInt16,						mode)						\
	DO_TO_FIELD (6,		emuptr,						openP)						\
	DO_TO_FIELD (10,	UInt32,						savedModNum)				\
	DO_TO_FIELD (14,	UInt8,						openType)					\
	DO_TO_FIELD (15,	UInt8,						reserved)					\


// ========================================
// ===== DmOpenInfoType =====
// ========================================

#define FOR_DmOpenInfoType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (DmOpenInfoType, 36, FOR_DmOpenInfoType_FIELDS)

#define FOR_DmOpenInfoType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						next)						\
	DO_TO_FIELD (4,		UInt16,						openCount)					\
	DO_TO_FIELD (6,		UInt32,						ownerTaskID)				\
	DO_TO_FIELD (10,	UInt16,						flags)						\
	DO_TO_FIELD (12,	LocalID,					hdrID)						\
	DO_TO_FIELD (16,	emuptr,						hdrH)						\
	DO_TO_FIELD (20,	emuptr,						hdrP)						\
	DO_TO_FIELD (24,	UInt16,						cardNo)						\
	DO_TO_FIELD (26,	UInt16,						numRecords)					\
	DO_TO_FIELD (28,	emuptr,						hdrMasterP)					\
	DO_TO_FIELD (32,	emuptr,						handleTableP)				\


// ========================================
// ===== DmSearchStateType =====
// ========================================

#define FOR_DmSearchStateType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (DmSearchStateType, 32, FOR_DmSearchStateType_FIELDS)

#define FOR_DmSearchStateType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt32,						info)						\


// ========================================
// ===== EventTypeData =====
// ========================================

#define FOR_EventTypeData_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (EventTypeData, 16, FOR_EventTypeData_FIELDS)

#define FOR_EventTypeData_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		generic,					generic)					\
	DO_TO_FIELD (0,		penUp,						penUp)						\
	DO_TO_FIELD (0,		keyDown,					keyDown)					\
	DO_TO_FIELD (0,		winEnter,					winEnter)					\
	DO_TO_FIELD (0,		winExit,					winExit)					\
	DO_TO_FIELD (0,		tsmConfirm,					tsmConfirm)					\
	DO_TO_FIELD (0,		tsmFepButton,				tsmFepButton)				\
	DO_TO_FIELD (0,		tsmFepMode,					tsmFepMode)					\
	DO_TO_FIELD (0,		ctlEnter,					ctlEnter)					\
	DO_TO_FIELD (0,		ctlSelect,					ctlSelect)					\
	DO_TO_FIELD (0,		ctlRepeat,					ctlRepeat)					\
	DO_TO_FIELD (0,		ctlExit,					ctlExit)					\
	DO_TO_FIELD (0,		fldEnter,					fldEnter)					\
	DO_TO_FIELD (0,		fldHeightChanged,			fldHeightChanged)			\
	DO_TO_FIELD (0,		fldChanged,					fldChanged)					\
	DO_TO_FIELD (0,		fldExit,					fldExit)					\
	DO_TO_FIELD (0,		lstEnter,					lstEnter)					\
	DO_TO_FIELD (0,		lstExit,					lstExit)					\
	DO_TO_FIELD (0,		lstSelect,					lstSelect)					\
	DO_TO_FIELD (0,		tblEnter,					tblEnter)					\
	DO_TO_FIELD (0,		tblExit,					tblExit)					\
	DO_TO_FIELD (0,		tblSelect,					tblSelect)					\
	DO_TO_FIELD (0,		frmLoad,					frmLoad)					\
	DO_TO_FIELD (0,		frmOpen,					frmOpen)					\
	DO_TO_FIELD (0,		frmGoto,					frmGoto)					\
	DO_TO_FIELD (0,		frmClose,					frmClose)					\
	DO_TO_FIELD (0,		frmUpdate,					frmUpdate)					\
	DO_TO_FIELD (0,		frmTitleEnter,				frmTitleEnter)				\
	DO_TO_FIELD (0,		frmTitleSelect,				frmTitleSelect)				\
	DO_TO_FIELD (0,		attnIndicatorEnter,			attnIndicatorEnter)			\
	DO_TO_FIELD (0,		attnIndicatorSelect,		attnIndicatorSelect)		\
	DO_TO_FIELD (0,		daySelect,					daySelect)					\
	DO_TO_FIELD (0,		menu,						menu)						\
	DO_TO_FIELD (0,		popSelect,					popSelect)					\
	DO_TO_FIELD (0,		sclEnter,					sclEnter)					\
	DO_TO_FIELD (0,		sclExit,					sclExit)					\
	DO_TO_FIELD (0,		sclRepeat,					sclRepeat)					\
	DO_TO_FIELD (0,		menuCmdBarOpen,				menuCmdBarOpen)				\
	DO_TO_FIELD (0,		menuOpen,					menuOpen)					\
	DO_TO_FIELD (0,		gadgetEnter,				gadgetEnter)				\
	DO_TO_FIELD (0,		gadgetMisc,					gadgetMisc)					\


// ========================================
// ===== EventType =====
// ========================================

#define FOR_EventType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (EventType, 24, FOR_EventType_FIELDS)

#define FOR_EventType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Int16,						eType)						\
	DO_TO_FIELD (2,		Boolean,					penDown)					\
	DO_TO_FIELD (3,		UInt8,						tapCount)					\
	DO_TO_FIELD (4,		Int16,						screenX)					\
	DO_TO_FIELD (6,		Int16,						screenY)					\
	DO_TO_FIELD (8,		EventTypeData,				data)						\


// ========================================
// ===== ExgGoToType =====
// ========================================

#define FOR_ExgGoToType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (ExgGoToType, 16, FOR_ExgGoToType_FIELDS)

#define FOR_ExgGoToType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						dbCardNo)					\
	DO_TO_FIELD (2,		LocalID,					dbID)						\
	DO_TO_FIELD (6,		UInt16,						recordNum)					\
	DO_TO_FIELD (8,		UInt32,						uniqueID)					\
	DO_TO_FIELD (12,	UInt32,						matchCustom)				\


// ========================================
// ===== ExgSocketType =====
// ========================================

#define FOR_ExgSocketType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (ExgSocketType, 60, FOR_ExgSocketType_FIELDS)

#define FOR_ExgSocketType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						libraryRef)					\
	DO_TO_FIELD (2,		UInt32,						socketRef)					\
	DO_TO_FIELD (6,		UInt32,						target)						\
	DO_TO_FIELD (10,	UInt32,						count)						\
	DO_TO_FIELD (14,	UInt32,						length)						\
	DO_TO_FIELD (18,	UInt32,						time)						\
	DO_TO_FIELD (22,	UInt32,						appData)					\
	DO_TO_FIELD (26,	UInt32,						goToCreator)				\
	DO_TO_FIELD (30,	ExgGoToType,				goToParams)					\
	DO_TO_FIELD (46,	UInt16,						flags)						\
	DO_TO_FIELD (48,	emuptr,						description)				\
	DO_TO_FIELD (52,	emuptr,						type)						\
	DO_TO_FIELD (56,	emuptr,						name)						\


// ========================================
// ===== FieldUndoType =====
// ========================================

#define FOR_FieldUndoType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FieldUndoType, 12, FOR_FieldUndoType_FIELDS)

#define FOR_FieldUndoType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UndoMode,					mode)						\
	DO_TO_FIELD (1,		UInt8,						reserved)					\
	DO_TO_FIELD (2,		UInt16,						start)						\
	DO_TO_FIELD (4,		UInt16,						end)						\
	DO_TO_FIELD (6,		UInt16,						bufferLen)					\
	DO_TO_FIELD (8,		emuptr,						buffer)						\


// ========================================
// ===== FixedGlobalsType =====
// ========================================

#undef memCardInfoP

#define FOR_FixedGlobalsType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (FixedGlobalsType, 650, FOR_FixedGlobalsType_FIELDS)

#define FOR_FixedGlobalsType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						memCardSlots)				\
	DO_TO_FIELD (1,		UInt8,						dbgWasEntered)				\
	DO_TO_FIELD (2,		emuptr,						memCardInfoP)				\
	DO_TO_FIELD (6,		UInt32,						memSemaphoreID)				\
	DO_TO_FIELD (10,	UInt16,						memDebugMode)				\
	DO_TO_FIELD (12,	emuptr,						dmOpenList)					\
	DO_TO_FIELD (16,	UInt8,						dbgInDebugger)				\
	DO_TO_FIELD (17,	UInt8,						dbgTracing)					\
	DO_TO_FIELD (18,	emuptr,						dbgGlobalsP)				\
	DO_TO_FIELD (22,	emuptr,						dbgSerGlobalsP)				\
	DO_TO_FIELD (26,	emuptr,						sysAppInfoP)				\
	DO_TO_FIELD (30,	emuptr,						sysKernelDataP)				\
	DO_TO_FIELD (34,	emuptr,						sysDispatchTableP)			\
	DO_TO_FIELD (38,	UInt32,						sysOwnerIDsInUse)			\
	DO_TO_FIELD (42,	emuptr,						sysAMXAppInfoP)				\
	DO_TO_FIELD (46,	UInt32,						sysClockFreq)				\
	DO_TO_FIELD (50,	emuptr,						sysHardKeyCreators)			\
	DO_TO_FIELD (54,	Int16,						sysBatteryCheckTimer)		\
	DO_TO_FIELD (56,	UInt8,						sysBatteryMinThreshold)		\
	DO_TO_FIELD (57,	UInt8,						hwrBatteryLevel)			\
	DO_TO_FIELD (58,	UInt16,						sysNextBatteryAlertTimer)	\
	DO_TO_FIELD (60,	UInt8,						sysBatteryWarnThreshold)	\
	DO_TO_FIELD (61,	UInt8,						sysDispatchTableRev)		\
	DO_TO_FIELD (62,	UInt16,						sysDispatchTableSize)		\
	DO_TO_FIELD (64,	emuptr,						sysLibTableP)				\
	DO_TO_FIELD (68,	UInt16,						sysLibTableEntries)			\
	DO_TO_FIELD (70,	emuptr,						sysConsoleStackChunkP)		\
	DO_TO_FIELD (74,	emuptr,						sysUIShellAppInfoP)			\
	DO_TO_FIELD (78,	UInt32,						sysTimerID)					\
	DO_TO_FIELD (82,	UInt32,						sysAutoOffEvtTicks)			\
	DO_TO_FIELD (86,	UInt16,						sysAutoOffSeconds)			\
	DO_TO_FIELD (88,	Int32,						sysRandomSeed)				\
	DO_TO_FIELD (92,	emuptr,						slkGlobalsP)				\
	DO_TO_FIELD (96,	emuptr,						serGlobalsP)				\
	DO_TO_FIELD (100,	emuptr,						scrGlobalsP)				\
	DO_TO_FIELD (104,	emuptr,						fplGlobalsP)				\
	DO_TO_FIELD (108,	emuptr,						penGlobalsP)				\
	DO_TO_FIELD (112,	emuptr,						sysEvtMgrGlobalsP)			\
	DO_TO_FIELD (116,	emuptr,						sndGlobalsP)				\
	DO_TO_FIELD (120,	emuptr,						timGlobalsP)				\
	DO_TO_FIELD (124,	emuptr,						almGlobalsP)				\
	DO_TO_FIELD (128,	emuptr,						ftrGlobalsP)				\
	DO_TO_FIELD (132,	emuptr,						grfGlobalsP)				\
	DO_TO_FIELD (136,	emuptr,						keyGlobalsP)				\
																				\
	DO_TO_FIELD (140,	UIGlobalsType,				uiGlobals)					\
	DO_TO_FIELD (140,	UIGlobalsTypeCommon,		uiGlobalsCommon)			\
	DO_TO_FIELD (140,	UIGlobalsTypeV1,			uiGlobalsV1)				\
	DO_TO_FIELD (140,	UIGlobalsTypeV2,			uiGlobalsV2)				\
	DO_TO_FIELD (140,	UIGlobalsTypeV3,			uiGlobalsV3)				\
	DO_TO_FIELD (140,	UIGlobalsTypeV31,			uiGlobalsV31)				\
	DO_TO_FIELD (140,	UIGlobalsTypeV32,			uiGlobalsV32)				\
	DO_TO_FIELD (140,	UIGlobalsTypeV35,			uiGlobalsV35)				\
																				\
	DO_TO_FIELD (316,	emuptr,						uiExtensionsP)				\
	DO_TO_FIELD (320,	emuptr,						curUIAppInfoP)				\
	DO_TO_FIELD (324,	UInt32,						nextUIAppDBID)				\
	DO_TO_FIELD (328,	UInt16,						nextUIAppCmd)				\
	DO_TO_FIELD (330,	emuptr,						nextUIAppCmdPBP)			\
	DO_TO_FIELD (334,	UInt8,						nextUIAppCardNo)			\
	DO_TO_FIELD (335,	Int8,						memDataWELevel)				\
	DO_TO_FIELD (336,	UInt16,						hwrWakeUp)					\
	DO_TO_FIELD (338,	UInt8,						hwrCPUDutyCycle)			\
	DO_TO_FIELD (339,	UInt8,						hwrPenDown)					\
	DO_TO_FIELD (340,	UInt32,						hwrCurTicks)				\
	DO_TO_FIELD (344,	UInt32,						hwrTotalRAMSize)			\
	DO_TO_FIELD (348,	UInt32,						hwrDozeSubTicks)			\
	DO_TO_FIELD (352,	UInt32,						sndOffTicks)				\
	DO_TO_FIELD (356,	UInt8,						sysResetFlags)				\
	DO_TO_FIELD (357,	UInt8,						sysBatteryKind)				\
	DO_TO_FIELD (358,	UInt32,						memMinDynHeapFree)			\
	DO_TO_FIELD (362,	UInt16,						sysPrefFlags)				\
	DO_TO_FIELD (364,	emuptr,						sysGlobalsP)				\
	DO_TO_FIELD (368,	UInt16,						sysMiscFlags)				\
	DO_TO_FIELD (370,	UInt16,						sysLibNet)					\
	DO_TO_FIELD (372,	emuptr,						netPktFreeQ)				\
	DO_TO_FIELD (376,	UInt32,						sysEvGroupID)				\
	DO_TO_FIELD (380,	emuptr,						irq3GlobalsP)				\
	DO_TO_FIELD (384,	UInt32,						sysLastBatteryWarning)		\
	DO_TO_FIELD (388,	UInt32,						sysLowMemChecksum)			\
	DO_TO_FIELD (392,	UInt16,						hwrHardwareRev)				\
	DO_TO_FIELD (394,	emuptr,						dmProtectListH)				\
	DO_TO_FIELD (398,	UInt16,						hwrMiscFlags)				\
	DO_TO_FIELD (400,	UInt32,						sysProfileRefcon)			\
	DO_TO_FIELD (404,	emuptr,						sysProfileProcP)			\
	DO_TO_FIELD (408,	UInt32,						flpSoftFPSCR)				\
	DO_TO_FIELD (412,	emuptr,						irLibGlobalsP)				\
	DO_TO_FIELD (416,	UInt16,						exgActiveLib)				\
	DO_TO_FIELD (418,	UInt16,						uiBusyCount)				\
	DO_TO_FIELD (420,	emuptr,						irq1GlobalsP)				\
	DO_TO_FIELD (424,	emuptr,						irq2GlobalsP)				\
	DO_TO_FIELD (428,	emuptr,						irq6GlobalsP)				\
	DO_TO_FIELD (432,	emuptr,						intlMgrGlobalsP)			\
	DO_TO_FIELD (436,	UInt16,						inlineStart)				\
	DO_TO_FIELD (438,	UInt16,						inlineEnd)					\
	DO_TO_FIELD (440,	Boolean,					inlineActive)				\
	DO_TO_FIELD (441,	UInt8,						inlineDefTerm)				\
	DO_TO_FIELD (442,	emuptr,						tsmFepLibStatusP)			\
	DO_TO_FIELD (446,	UInt16,						tsmFepLibRefNum)			\
	DO_TO_FIELD (448,	UInt16,						inlineHighlightLen)			\
	DO_TO_FIELD (450,	emuptr,						sysBatteryDataP)			\
	DO_TO_FIELD (454,	emuptr,						sysBatteryValidKindsP)		\
	DO_TO_FIELD (458,	Boolean,					sysInShutdown)				\
	DO_TO_FIELD (459,	UInt8,						sysMinWakeupThreshold)		\
	DO_TO_FIELD (460,	UInt32,						hwrMiscFlagsExt)			\
	DO_TO_FIELD (464,	Boolean,					sysUseCritWarnTimer)		\
	DO_TO_FIELD (465,	Boolean,					sysCritWarnTimerRunning)	\
	DO_TO_FIELD (466,	Int16,						sysCritWarnRTCDayStarted)	\
	DO_TO_FIELD (468,	Int32,						sysCritWarnTickTimer)		\
	DO_TO_FIELD (472,	UInt32,						sysCritWarnLastTick)		\
	DO_TO_FIELD (476,	Boolean,					sysCharging)				\
	DO_TO_FIELD (477,	Boolean,					sysUpdateVoltageLevel)		\
	DO_TO_FIELD (478,	emuptr,						notifyGlobalsH)				\
	DO_TO_FIELD (482,	UInt32,						expGlobalsP)				\
	DO_TO_FIELD (486,	UInt32,						vfsGlobalsP)				\
	DO_TO_FIELD (490,	UInt32,						sysReserved31DWord3)		\
	DO_TO_FIELD (494,	UInt32,						sysReserved31DWord4)		\
	DO_TO_FIELD (498,	emuptr,						ramGlobalsP)				\
	DO_TO_FIELD (502,	emuptr,						hwrBatteryGlobalsP)			\
	DO_TO_FIELD (506,	UInt8,						hwrBatteryPercent)			\
	DO_TO_FIELD (507,	UInt8,						commActivityFlags)			\
	DO_TO_FIELD (508,	emuptr,						uart328GlobalsP)			\
	DO_TO_FIELD (512,	UInt32,						sysOEMGlobal1)				\
	DO_TO_FIELD (516,	UInt32,						sysOEMGlobal2)				\
	DO_TO_FIELD (520,	UInt16,						hwrBatteryStepsPerVolt)		\
	DO_TO_FIELD (522,	emuptr,						sysPendingStackFrees)		\
	DO_TO_FIELD (526,	UInt16,						sysDayCounter)				\
	DO_TO_FIELD (528,	Int16,						sysBatteryCheckPeriod)		\
	DO_TO_FIELD (530,	UInt16,						hwrJerryPldShadow)			\
	DO_TO_FIELD (532,	emuptr,						radioGlobalsP)				\
	DO_TO_FIELD (536,	emuptr,						fatalAlertTemplateP)		\
	DO_TO_FIELD (540,	UInt16,						fatalAlertBtnWidth)			\
	DO_TO_FIELD (542,	emuptr,						silkscreenInfoP)			\
	DO_TO_FIELD (546,	UInt16,						inlineDefChecksum)			\
	DO_TO_FIELD (548,	emuptr,						inlineDefField)				\
	DO_TO_FIELD (552,	Boolean,					notifyBroadcastPending)		\
	DO_TO_FIELD (553,	UInt8,						sysReserved35Byte1)			\
	DO_TO_FIELD (554,	UInt32,						hwrOEMCompanyID)			\
	DO_TO_FIELD (558,	UInt32,						hwrOEMDeviceID)				\
	DO_TO_FIELD (562,	UInt32,						hwrOEMHALID)				\
	DO_TO_FIELD (566,	emuptr,						sysUpdateGlobalsP)			\
	DO_TO_FIELD (570,	UInt16,						smallROMChecksum)			\
	DO_TO_FIELD (572,	emuptr,						sysNotifyInterruptGlobalsP)	\
	DO_TO_FIELD (576,	emuptr,						sysAppSubFontListPtr)		\
	DO_TO_FIELD (580,	UInt16,						sysUSBConfiguration)		\
	DO_TO_FIELD (582,	emuptr,						intlDispatchTableP)			\
	DO_TO_FIELD (586,	Boolean,					sysAutoLockUponPowerOff)	\
	DO_TO_FIELD (588,	UInt32,						sysHardPowerEvtTicks)		\
	DO_TO_FIELD (592,	UInt32,						sysAutoLockTimeoutSecs)		\
	DO_TO_FIELD (596,	emuptr,						attnGlobalsP)				\
	DO_TO_FIELD (600,	emuptr,						expVfsSharedGlobalsP)		\
	DO_TO_FIELD (604,	emuptr,						btExgLibGlobalsP)			\
	DO_TO_FIELD (608,	emuptr,						testHarnessGlobalsP)		\
	DO_TO_FIELD (612,	emuptr,						pwrManagerGlobalsP)			\
	DO_TO_FIELD (616,	UInt16,						inlineClauseOffset)			\
	DO_TO_FIELD (618,	emuptr,						btLibGlobalsP)				\
	DO_TO_FIELD (622,	Boolean,					sysSetPwdTimeoutAllowed)	\
	DO_TO_FIELD (624,	UInt32,						sysSetPwdTimeoutSecs)		\
	DO_TO_FIELD (628,	UInt32,						sysSetPwdTimeoutTicks)		\
	DO_TO_FIELD (632,	UInt16,						dbgLockout)					\
	DO_TO_FIELD (634,	UInt32,						sysReserved40DWord1)		\
	DO_TO_FIELD (638,	UInt32,						sysReserved40DWord2)		\
	DO_TO_FIELD (642,	UInt32,						sysReserved40DWord3)		\
	DO_TO_FIELD (646,	UInt32,						sysReserved40DWord4)		\


// ========================================
// ===== FieldAttrType =====
// ========================================

#define FOR_FieldAttrType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FieldAttrType, 2, FOR_FieldAttrType_FIELDS)

#define FOR_FieldAttrType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						flags)						\


// ========================================
// ===== FieldType =====
// ========================================

#define FOR_FieldType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (FieldType, 40, FOR_FieldType_FIELDS)

#define FOR_FieldType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						id)							\
	DO_TO_FIELD (2,		RectangleType,				rect)						\
	DO_TO_FIELD (10,	FieldAttrType,				attr)						\
	DO_TO_FIELD (12,	emuptr,						text)						\
	DO_TO_FIELD (16,	emuptr,						textHandle)					\
	DO_TO_FIELD (20,	emuptr,						lines)						\
	DO_TO_FIELD (24,	UInt16,						textLen)					\
	DO_TO_FIELD (26,	UInt16,						textBlockSize)				\
	DO_TO_FIELD (28,	UInt16,						maxChars)					\
	DO_TO_FIELD (30,	UInt16,						selFirstPos)				\
	DO_TO_FIELD (32,	UInt16,						selLastPos)					\
	DO_TO_FIELD (34,	UInt16,						insPtXPos)					\
	DO_TO_FIELD (36,	UInt16,						insPtYPos)					\
	DO_TO_FIELD (38,	FontID,						fontID)						\
	DO_TO_FIELD (39,	UInt16,						maxVisibleLines)			\


// ========================================
// ===== FormBitmapType =====
// ========================================

#define FOR_FormBitmapType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FormBitmapType, 8, FOR_FormBitmapType_FIELDS)

#define FOR_FormBitmapType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		FormObjAttrType,			attr)						\
	DO_TO_FIELD (2,		PointType,					pos)						\
	DO_TO_FIELD (6,		UInt16,						rscID)						\


// ========================================
// ===== FormFrameType =====
// ========================================

#define FOR_FormFrameType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FormFrameType, 14, FOR_FormFrameType_FIELDS)

#define FOR_FormFrameType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						id)							\
	DO_TO_FIELD (2,		FormObjAttrType,			attr)						\
	DO_TO_FIELD (4,		RectangleType,				rect)						\
	DO_TO_FIELD (12,	UInt16,						frameType)					\


// ========================================
// ===== FormGadgetAttrType =====
// ========================================

#define FOR_FormGadgetAttrType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (FormGadgetAttrType, 2, FOR_FormGadgetAttrType_FIELDS)

#define FOR_FormGadgetAttrType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						flags)						\


// ========================================
// ===== FormGadgetType =====
// ========================================

#define FOR_FormGadgetType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FormGadgetType, 20, FOR_FormGadgetType_FIELDS)

#define FOR_FormGadgetType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						id)							\
	DO_TO_FIELD (2,		FormGadgetAttrType,			attr)						\
	DO_TO_FIELD (4,		RectangleType,				rect)						\
	DO_TO_FIELD (12,	emuptr,						data)						\
	DO_TO_FIELD (16,	emuptr,						handler)					\


// ========================================
// ===== FormLabelType =====
// ========================================

#define FOR_FormLabelType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FormLabelType, 14, FOR_FormLabelType_FIELDS)

#define FOR_FormLabelType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						id)							\
	DO_TO_FIELD (2,		PointType,					pos)						\
	DO_TO_FIELD (6,		FormObjAttrType,			attr)						\
	DO_TO_FIELD (8,		FontID,						fontID)						\
	DO_TO_FIELD (9,		UInt8,						reserved)					\
	DO_TO_FIELD (10,	emuptr,						text)						\


// ========================================
// ===== FormLineType =====
// ========================================

#define FOR_FormLineType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FormLineType, 10, FOR_FormLineType_FIELDS)

#define FOR_FormLineType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						attr)						\
	DO_TO_FIELD (2,		PointType,					point1)						\
	DO_TO_FIELD (6,		PointType,					point2)						\


// ========================================
// ===== FormObjAttrType =====
// ========================================

#define FOR_FormObjAttrType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (FormObjAttrType, 2, FOR_FormObjAttrType_FIELDS)

#define FOR_FormObjAttrType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						flags)						\


// ========================================
// ===== FormObjListType =====
// ========================================

#define FOR_FormObjListType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (FormObjListType, 6, FOR_FormObjListType_FIELDS)

#define FOR_FormObjListType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		FormObjectKind,				objectType)					\
	DO_TO_FIELD (1,		UInt8,						reserved)					\
	DO_TO_FIELD (2,		emuptr,						object)						\


// ========================================
// ===== FormPopupType =====
// ========================================

#define FOR_FormPopupType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FormPopupType, 4, FOR_FormPopupType_FIELDS)

#define FOR_FormPopupType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						controlID)					\
	DO_TO_FIELD (2,		UInt16,						listID)						\


// ========================================
// ===== FormRectangleType =====
// ========================================

#define FOR_FormRectangleType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (FormRectangleType, 10, FOR_FormRectangleType_FIELDS)

#define FOR_FormRectangleType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		FormObjAttrType,			attr)						\
	DO_TO_FIELD (2,		RectangleType,				rect)						\


// ========================================
// ===== FormTitleType =====
// ========================================

#define FOR_FormTitleType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FormTitleType, 12, FOR_FormTitleType_FIELDS)

#define FOR_FormTitleType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		RectangleType,				rect)						\
	DO_TO_FIELD (8,		emuptr,						text)						\


// ========================================
// ===== FormType =====
// ========================================

#define FOR_FormType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (FormType, 68, FOR_FormType_FIELDS)

#define FOR_FormType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		WindowType,					window)						\
	DO_TO_FIELD (40,	UInt16,						formId)						\
	DO_TO_FIELD (42,	FormAttrType,				attr)						\
	DO_TO_FIELD (46,	emuptr,						bitsBehindForm)				\
	DO_TO_FIELD (50,	emuptr,						handler)					\
	DO_TO_FIELD (54,	UInt16,						focus)						\
	DO_TO_FIELD (56,	UInt16,						defaultButton)				\
	DO_TO_FIELD (58,	UInt16,						helpRscId)					\
	DO_TO_FIELD (60,	UInt16,						menuRscId)					\
	DO_TO_FIELD (62,	UInt16,						numObjects)					\
	DO_TO_FIELD (64,	emuptr,						objects)					\


// ========================================
// ===== FrameBitsType =====
// ========================================

#define FOR_FrameBitsType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FrameBitsType, 2, FOR_FrameBitsType_FIELDS)

#define FOR_FrameBitsType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						flags)						\


// ========================================
// ===== FrmGraffitiStateType =====
// ========================================

#define FOR_FrmGraffitiStateType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (FrmGraffitiStateType, 4, FOR_FrmGraffitiStateType_FIELDS)

#define FOR_FrmGraffitiStateType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		PointType,					pos)						\


// ========================================
// ===== GraphicControlType =====
// ========================================

#define FOR_GraphicControlType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (GraphicControlType, 20, FOR_GraphicControlType_FIELDS)

#define FOR_GraphicControlType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						id)							\
	DO_TO_FIELD (2,		RectangleType,				bounds)						\
	DO_TO_FIELD (10,	DmResID,					bitmapID)					\
	DO_TO_FIELD (12,	DmResID,					selectedBitmapID)			\
	DO_TO_FIELD (14,	ControlAttrType,			attr)						\
	DO_TO_FIELD (16,	ControlStyleType,			style)						\
	DO_TO_FIELD (17,	UInt8,						unused)						\
	DO_TO_FIELD (18,	UInt8,						group)						\
	DO_TO_FIELD (19,	UInt8,						reserved)					\


// ========================================
// ===== GraphicStateTypeV1 =====
// ========================================
// Used in Palm OS 1.0 - 2.0.

#define FOR_GraphicStateTypeV1_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (GraphicStateTypeV1, 20, FOR_GraphicStateTypeV1_FIELDS)

#define FOR_GraphicStateTypeV1_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		ScrOperation,				grafMode)					\
	DO_TO_FIELD (1,		Boolean,					patternMode)				\
	DO_TO_FIELD (2,		PatternType,				pattern)					\
	DO_TO_FIELD (4,		UInt16,						customPtn)					\
	DO_TO_FIELD (12,	FontID,						fontId)						\
	DO_TO_FIELD (14,	emuptr,						font)						\
	DO_TO_FIELD (18,	UnderlineModeType,			underlineMode)				\


// ========================================
// ===== GraphicStateTypeV2 =====
// ========================================
// Used in Palm OS 3.0 - 3.3.

#define FOR_GraphicStateTypeV2_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (GraphicStateTypeV2, 28, FOR_GraphicStateTypeV2_FIELDS)

#define FOR_GraphicStateTypeV2_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		ScrOperation,				grafMode)					\
	DO_TO_FIELD (1,		Boolean,					patternMode)				\
	DO_TO_FIELD (2,		PatternType,				pattern)					\
	DO_TO_FIELD (4,		UInt16,						customPtn)					\
	DO_TO_FIELD (12,	FontID,						fontId)						\
	DO_TO_FIELD (14,	emuptr,						font)						\
	DO_TO_FIELD (18,	UnderlineModeType,			underlineMode)				\
	DO_TO_FIELD (20,	RGBColorType,				foreColor)					\
	DO_TO_FIELD (24,	RGBColorType,				backColor)					\


// ========================================
// ===== GraphicStateTypeV3 =====
// ========================================
// Used in Palm OS 3.5 and later.

#define FOR_GraphicStateTypeV3_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (GraphicStateTypeV3, 12, FOR_GraphicStateTypeV3_FIELDS)

#define FOR_GraphicStateTypeV3_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		emuptr,						drawStateP)					\
	DO_TO_FIELD (4,		emuptr,						drawStateStackP)			\
	DO_TO_FIELD (8,		Int16,						drawStateIndex)				\
	DO_TO_FIELD (10,	UInt16,						screenLockCount)			\


// ========================================
// ===== FormAttrType =====
// ========================================

#define FOR_FormAttrType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (FormAttrType, 4, FOR_FormAttrType_FIELDS)

#define FOR_FormAttrType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt32,						flags)						\


// ========================================
// ===== HostDirEntType =====
// ========================================

#define FOR_HostDirEntType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (HostDirEntType, 256, FOR_HostDirEntType_FIELDS)

#define FOR_HostDirEntType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		Int8,						d_name)						\


// ========================================
// ===== HostGremlinInfoType =====
// ========================================

#define FOR_HostGremlinInfoType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (HostGremlinInfoType, 220, FOR_HostGremlinInfoType_FIELDS)

#define FOR_HostGremlinInfoType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		Int32,						fFirstGremlin)				\
	DO_TO_FIELD (4,		Int32,						fLastGremlin)				\
	DO_TO_FIELD (8,		Int32,						fSaveFrequency)				\
	DO_TO_FIELD (12,	Int32,						fSwitchDepth)				\
	DO_TO_FIELD (16,	Int32,						fMaxDepth)					\
	DO_TO_FIELD (20,	Int8,						fAppNames)					\


// ========================================
// ===== HostStatType =====
// ========================================

#define FOR_HostStatType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (HostStatType, 56, FOR_HostStatType_FIELDS)

#define FOR_HostStatType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		Int32,						st_dev_)					\
	DO_TO_FIELD (4,		Int32,						st_ino_)					\
	DO_TO_FIELD (8,		Int32,						st_mode_)					\
	DO_TO_FIELD (12,	Int32,						st_nlink_)					\
	DO_TO_FIELD (16,	Int32,						st_uid_)					\
	DO_TO_FIELD (20,	Int32,						st_gid_)					\
	DO_TO_FIELD (24,	Int32,						st_rdev_)					\
	DO_TO_FIELD (28,	HostTimeType,				st_atime_)					\
	DO_TO_FIELD (32,	HostTimeType,				st_mtime_)					\
	DO_TO_FIELD (36,	HostTimeType,				st_ctime_)					\
	DO_TO_FIELD (40,	Int32,						st_size_)					\
	DO_TO_FIELD (44,	Int32,						st_blksize_)				\
	DO_TO_FIELD (48,	Int32,						st_blocks_)					\
	DO_TO_FIELD (52,	Int32,						st_flags_)					\


// ========================================
// ===== HostTmType =====
// ========================================

#define FOR_HostTmType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (HostTmType, 36, FOR_HostTmType_FIELDS)

#define FOR_HostTmType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Int32,						tm_sec_)					\
	DO_TO_FIELD (4,		Int32,						tm_min_)					\
	DO_TO_FIELD (8,		Int32,						tm_hour_)					\
	DO_TO_FIELD (12,	Int32,						tm_mday_)					\
	DO_TO_FIELD (16,	Int32,						tm_mon_)					\
	DO_TO_FIELD (20,	Int32,						tm_year_)					\
	DO_TO_FIELD (24,	Int32,						tm_wday_)					\
	DO_TO_FIELD (28,	Int32,						tm_yday_)					\
	DO_TO_FIELD (32,	Int32,						tm_isdst_)					\


// ========================================
// ===== HostUTimeType =====
// ========================================

#define FOR_HostUTimeType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (HostUTimeType, 12, FOR_HostUTimeType_FIELDS)

#define FOR_HostUTimeType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		HostTimeType,				crtime_)					\
	DO_TO_FIELD (4,		HostTimeType,				actime_)					\
	DO_TO_FIELD (8,		HostTimeType,				modtime_)					\


// ========================================
// ===== HwrBatCmdReadType =====
// ========================================

#define FOR_HwrBatCmdReadType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (HwrBatCmdReadType, 4, FOR_HwrBatCmdReadType_FIELDS)

#define FOR_HwrBatCmdReadType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,				mVolts)								\
	DO_TO_FIELD (2,		UInt16,				abs)								\


// ========================================
// ===== HwrJerryPLDType =====
// ========================================

#define FOR_HwrJerryPLDType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (HwrJerryPLDType, 20, FOR_HwrJerryPLDType_FIELDS)

#define FOR_HwrJerryPLDType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt8,						dspOn)						\
	DO_TO_FIELD (1,		UInt8,						unused1)					\
	DO_TO_FIELD (2,		UInt8,						chargeOn)					\
	DO_TO_FIELD (3,		UInt8,						unused2)					\
	DO_TO_FIELD (4,		UInt8,						refOn)						\
	DO_TO_FIELD (5,		UInt8,						unused3)					\
	DO_TO_FIELD (6,		UInt8,						pipaBiasEnable)				\
	DO_TO_FIELD (7,		UInt8,						unused4)					\
	DO_TO_FIELD (8,		UInt8,						dspReset)					\
	DO_TO_FIELD (9,		UInt8,						unused5)					\
	DO_TO_FIELD (10,	UInt8,						ezToDsl)					\
	DO_TO_FIELD (11,	UInt8,						unused6)					\
	DO_TO_FIELD (12,	UInt8,						rs232Shdn)					\
	DO_TO_FIELD (13,	UInt8,						unused7)					\
	DO_TO_FIELD (14,	UInt8,						spareOut)					\
	DO_TO_FIELD (15,	UInt8,						unused8)					\
	DO_TO_FIELD (16,	UInt16,						dTo3Sln)					\
	DO_TO_FIELD (17,	UInt16,						iXtrnl2)					\


// ========================================
// ===== HwrLAPType =====
// ========================================

#define FOR_HwrLAPType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (HwrLAPType, 0x64, FOR_HwrLAPType_FIELDS)

#define FOR_HwrLAPType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0x000,	UInt32,						control)					\
	DO_TO_FIELD (0x010,	UInt32,						status)						\
	DO_TO_FIELD (0x040,	UInt32,						timing0)					\
	DO_TO_FIELD (0x050,	UInt32,						timing1)					\
	DO_TO_FIELD (0x060,	UInt32,						timing2)					\


// ========================================
// ===== HwrMediaQ11xxType =====
// ========================================

#define FOR_HwrMediaQ11xxType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (HwrMediaQ11xxType, 0x2000, FOR_HwrMediaQ11xxType_FIELDS)

#define FOR_HwrMediaQ11xxType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0x000,	UInt32,						ccREG)						\
	DO_TO_FIELD (0x080,	UInt32,						mmREG)						\
	DO_TO_FIELD (0x100,	UInt32,						inREG)						\
	DO_TO_FIELD (0x180,	UInt32,						gcREG)						\
	DO_TO_FIELD (0x200,	UInt32,						geREG)						\
	DO_TO_FIELD (0x280,	UInt32,						ssREG)						\
	DO_TO_FIELD (0x300,	UInt32,						spREG)						\
	DO_TO_FIELD (0x380,	UInt32,						dcREG)						\
	DO_TO_FIELD (0x400,	UInt32,						pcREG)						\
	DO_TO_FIELD (0x500,	UInt32,						uhREG)						\
	DO_TO_FIELD (0x600,	UInt32,						fpREG)						\
	DO_TO_FIELD (0x800,	UInt32,						cpREG)						\
	DO_TO_FIELD (0xC00,	UInt32,						sfREG)						\
	DO_TO_FIELD (0x1000,UInt32,						udREG)						\


// ========================================
// ===== HwrPalmI705PLDType =====
// ========================================

#define FOR_HwrPalmI705PLDType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (HwrPalmI705PLDType, 42, FOR_HwrPalmI705PLDType_FIELDS)

#define FOR_HwrPalmI705PLDType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						todsln3)					\
	DO_TO_FIELD (2,		UInt16,						green_led)					\
	DO_TO_FIELD (4,		UInt16,						pin_notify_enable)			\
	DO_TO_FIELD (6,		UInt16,						red_led)					\
	DO_TO_FIELD (8,		UInt16,						acpr)						\
	DO_TO_FIELD (10,	UInt16,						low_batt)					\
	DO_TO_FIELD (12,	UInt16,						dto3svc_irq)				\
	DO_TO_FIELD (14,	UInt16,						dsp_irq_enable)				\
	DO_TO_FIELD (16,	UInt16,						dsp_irq_polarity)			\
	DO_TO_FIELD (18,	UInt16,						charging_in_progress)		\
	DO_TO_FIELD (20,	UInt16,						charging_in_prog2)			\
	DO_TO_FIELD (22,	UInt16,						charging_in_prog_OE)		\
	DO_TO_FIELD (24,	UInt16,						twister_irq_wake)			\
	DO_TO_FIELD (26,	UInt16,						low_batt_mask)				\
	DO_TO_FIELD (28,	UInt16,						acpr_mask)					\
	DO_TO_FIELD (30,	UInt16,						acpr_polarity)				\
	DO_TO_FIELD (32,	UInt16,						usb_int)					\
	DO_TO_FIELD (34,	UInt16,						usb_int_mask)				\
	DO_TO_FIELD (36,	UInt16,						bullet_det)					\
	DO_TO_FIELD (38,	UInt16,						bullet_det_mask)			\
	DO_TO_FIELD (40,	UInt16,						bullet_det_pol)				\


// ========================================
// ===== HwrSymbolASICType =====
// ========================================

#define FOR_HwrSymbolASICType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (HwrSymbolASICType, 256, FOR_HwrSymbolASICType_FIELDS)

#define FOR_HwrSymbolASICType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						S24IO)						\
	DO_TO_FIELD (128,	UInt8,						S24Attribute)				\
	DO_TO_FIELD (160,	UInt8,						UART8251MacroSelect)		\
	DO_TO_FIELD (165,	UInt8,						ScannerDecoderControl)		\
	DO_TO_FIELD (247,	UInt8,						Control)					\
	DO_TO_FIELD (253,	UInt8,						GPIOData)					\
	DO_TO_FIELD (255,	UInt8,						GPIODirection)				\


// ========================================
// ===== ListAttrType =====
// ========================================

#define FOR_ListAttrType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (ListAttrType, 2, FOR_ListAttrType_FIELDS)

#define FOR_ListAttrType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						flags)						\


// ========================================
// ===== ListType =====
// ========================================

#define FOR_ListType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (ListType, 32, FOR_ListType_FIELDS)

#define FOR_ListType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						id)							\
	DO_TO_FIELD (2,		RectangleType,				bounds)						\
	DO_TO_FIELD (10,	ListAttrType,				attr)						\
	DO_TO_FIELD (12,	emuptr,						itemsText)					\
	DO_TO_FIELD (16,	Int16,						numItems)					\
	DO_TO_FIELD (18,	Int16,						currentItem)				\
	DO_TO_FIELD (20,	Int16,						topItem)					\
	DO_TO_FIELD (22,	FontID,						font)						\
	DO_TO_FIELD (23,	UInt8,						reserved)					\
	DO_TO_FIELD (24,	emuptr,						popupWin)					\
	DO_TO_FIELD (28,	emuptr,						drawItemsCallback)			\


// ========================================
// ===== LowMemHdrType =====
// ========================================

#define FOR_LowMemHdrType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (LowMemHdrType, 828, FOR_LowMemHdrType_FIELDS)

#define FOR_LowMemHdrType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		M68KExcTableType,			vectors)					\
	DO_TO_FIELD (256,	FixedGlobalsType,			globals)					\


// ========================================
// ===== M68KExcTableType =====
// ========================================

#define FOR_M68KExcTableType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (M68KExcTableType, 256, FOR_M68KExcTableType_FIELDS)

#define FOR_M68KExcTableType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt32,						initStack)					\
	DO_TO_FIELD (4,		UInt32,						initPC)						\
	DO_TO_FIELD (8,		UInt32,						busErr)						\
	DO_TO_FIELD (12,	UInt32,						addressErr)					\
	DO_TO_FIELD (16,	UInt32,						illegalInstr)				\
	DO_TO_FIELD (20,	UInt32,						divideByZero)				\
	DO_TO_FIELD (24,	UInt32,						chk)						\
	DO_TO_FIELD (28,	UInt32,						trap)						\
	DO_TO_FIELD (32,	UInt32,						privilege)					\
	DO_TO_FIELD (36,	UInt32,						trace)						\
	DO_TO_FIELD (40,	UInt32,						aTrap)						\
	DO_TO_FIELD (44,	UInt32,						fTrap)						\
	DO_TO_FIELD (48,	UInt32,						reserved12)					\
	DO_TO_FIELD (52,	UInt32,						coproc)						\
	DO_TO_FIELD (56,	UInt32,						formatErr)					\
	DO_TO_FIELD (60,	UInt32,						unitializedInt)				\
	DO_TO_FIELD (96,	UInt32,						spuriousInt)				\
	DO_TO_FIELD (100,	UInt32,						autoVec1)					\
	DO_TO_FIELD (104,	UInt32,						autoVec2)					\
	DO_TO_FIELD (108,	UInt32,						autoVec3)					\
	DO_TO_FIELD (112,	UInt32,						autoVec4)					\
	DO_TO_FIELD (116,	UInt32,						autoVec5)					\
	DO_TO_FIELD (120,	UInt32,						autoVec6)					\
	DO_TO_FIELD (124,	UInt32,						autoVec7)					\


// ========================================
// ===== M68KRegsType =====
// ========================================

#define FOR_M68KRegsType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (M68KRegsType, 74, FOR_M68KRegsType_FIELDS)

#define FOR_M68KRegsType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt32,						d)							\
	DO_TO_FIELD (32,	UInt32,						a)							\
	DO_TO_FIELD (60,	UInt32,						usp)						\
	DO_TO_FIELD (64,	UInt32,						ssp)						\
	DO_TO_FIELD (68,	UInt32,						pc)							\
	DO_TO_FIELD (72,	UInt16,						sr)							\


// ========================================
// ===== NetHostInfoBufType =====
// ========================================

#define FOR_NetHostInfoBufType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (NetHostInfoBufType, 568, FOR_NetHostInfoBufType_FIELDS)

#define FOR_NetHostInfoBufType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		NetHostInfoType,			hostInfo)					\
	DO_TO_FIELD (16,	Char,						name)						\
	DO_TO_FIELD (272,	emuptr,						aliasList)					\
	DO_TO_FIELD (280,	Char,						aliases)					\
	DO_TO_FIELD (536,	emuptr,						addressList)				\
	DO_TO_FIELD (552,	NetIPAddr,					address)					\


// ========================================
// ===== NetHostInfoType =====
// ========================================

#define FOR_NetHostInfoType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (NetHostInfoType, 16, FOR_NetHostInfoType_FIELDS)

#define FOR_NetHostInfoType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						nameP)						\
	DO_TO_FIELD (4,		emuptr,						nameAliasesP)				\
	DO_TO_FIELD (8,		UInt16,						addrType)					\
	DO_TO_FIELD (10,	UInt16,						addrLen)					\
	DO_TO_FIELD (12,	emuptr,						addrListP)					\


// ========================================
// ===== NetIOParamType =====
// ========================================

#define FOR_NetIOParamType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (NetIOParamType, 18, FOR_NetIOParamType_FIELDS)

#define FOR_NetIOParamType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						addrP)						\
	DO_TO_FIELD (4,		UInt16,						addrLen)					\
	DO_TO_FIELD (6,		emuptr,						iov)						\
	DO_TO_FIELD (10,	UInt16,						iovLen)						\
	DO_TO_FIELD (12,	emuptr,						accessRights)				\
	DO_TO_FIELD (16,	UInt16,						accessRightsLen)			\


// ========================================
// ===== NetIOVecType =====
// ========================================

#define FOR_NetIOVecType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (NetIOVecType, 6, FOR_NetIOVecType_FIELDS)

#define FOR_NetIOVecType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						bufP)						\
	DO_TO_FIELD (4,		UInt16,						bufLen)						\


// ========================================
// ===== NetServInfoBufType =====
// ========================================

#define FOR_NetServInfoBufType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (NetServInfoBufType, 70, FOR_NetServInfoBufType_FIELDS)

#define FOR_NetServInfoBufType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		NetServInfoType,			servInfo)					\
	DO_TO_FIELD (14,	Char,						name)						\
	DO_TO_FIELD (30,	emuptr,						aliasList)					\
	DO_TO_FIELD (38,	Char,						aliases)					\
	DO_TO_FIELD (53,	Char,						protoName)					\
	DO_TO_FIELD (69,	UInt8,						reserved)					\


// ========================================
// ===== NetServInfoType =====
// ========================================

#define FOR_NetServInfoType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (NetServInfoType, 14, FOR_NetServInfoType_FIELDS)

#define FOR_NetServInfoType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						nameP)						\
	DO_TO_FIELD (4,		emuptr,						nameAliasesP)				\
	DO_TO_FIELD (8,		UInt16,						port)						\
	DO_TO_FIELD (10,	emuptr,						protoP)						\


// ========================================
// ===== NetSocketAddrINType =====
// ========================================

#define FOR_NetSocketAddrINType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (NetSocketAddrINType, 8, FOR_NetSocketAddrINType_FIELDS)

#define FOR_NetSocketAddrINType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		Int16,						family)						\
	DO_TO_FIELD (2,		UInt16,						port)						\
	DO_TO_FIELD (4,		NetIPAddr,					addr)						\


// ========================================
// ===== NetSocketAddrRawType =====
// ========================================

#define FOR_NetSocketAddrRawType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (NetSocketAddrRawType, 8, FOR_NetSocketAddrRawType_FIELDS)

#define FOR_NetSocketAddrRawType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		Int16,						family)						\
	DO_TO_FIELD (2,		UInt16,						ifInstance)					\
	DO_TO_FIELD (4,		UInt32,						ifCreator)					\


// ========================================
// ===== NetSocketAddrType =====
// ========================================

#define FOR_NetSocketAddrType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (NetSocketAddrType, 16, FOR_NetSocketAddrType_FIELDS)

#define FOR_NetSocketAddrType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		Int16,						family)						\
	DO_TO_FIELD (2,		UInt8,						data)						\


// ========================================
// ===== PenBtnInfoType =====
// ========================================

#define FOR_PenBtnInfoType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (PenBtnInfoType, 14, FOR_PenBtnInfoType_FIELDS)

#define FOR_PenBtnInfoType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		RectangleType,				boundsR)					\
	DO_TO_FIELD (8,		WChar,						asciiCode)					\
	DO_TO_FIELD (10,	UInt16,						keyCode)					\
	DO_TO_FIELD (12,	UInt16,						modifiers)					\


// ========================================
// ===== PointType =====
// ========================================

#define FOR_PointType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (PointType, 4, FOR_PointType_FIELDS)

#define FOR_PointType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Coord,						x)							\
	DO_TO_FIELD (2,		Coord,						y)							\


// ========================================
// ===== RecordEntryType =====
// ========================================

#define FOR_RecordEntryType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (RecordEntryType, 8, FOR_RecordEntryType_FIELDS)

#define FOR_RecordEntryType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		LocalID,					localChunkID)				\
	DO_TO_FIELD (4,		UInt8,						attributes)					\
	DO_TO_FIELD (5,		UInt8,						uniqueID)					\


// ========================================
// ===== RecordListType =====
// ========================================

#define FOR_RecordListType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (RecordListType, 6, FOR_RecordListType_FIELDS)

#define FOR_RecordListType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		LocalID,					nextRecordListID)			\
	DO_TO_FIELD (4,		UInt16,						numRecords)					\
	DO_TO_FIELD (6,		RecordEntryType,			records)					\
	DO_TO_FIELD (6,		RsrcEntryType,				resources)					\


// ========================================
// ===== RectangleType =====
// ========================================

#undef topLeft

#define FOR_RectangleType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (RectangleType, 8, FOR_RectangleType_FIELDS)

#define FOR_RectangleType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		PointType,					topLeft)					\
	DO_TO_FIELD (4,		PointType,					extent)						\


// ========================================
// ===== RGBColorType =====
// ========================================

#define FOR_RGBColorType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (RGBColorType, 6, FOR_RGBColorType_FIELDS)

#define FOR_RGBColorType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		LocalID,					index)						\
	DO_TO_FIELD (4,		UInt8,						r)							\
	DO_TO_FIELD (6,		UInt8,						g)							\
	DO_TO_FIELD (6,		UInt8,						b)							\


// ========================================
// ===== RsrcEntryType =====
// ========================================

#define FOR_RsrcEntryType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (RsrcEntryType, 10, FOR_RsrcEntryType_FIELDS)

#define FOR_RsrcEntryType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt32,						type)						\
	DO_TO_FIELD (4,		UInt16,						id)							\
	DO_TO_FIELD (6,		LocalID,					localChunkID)				\


// ========================================
// ===== ScrollBarAttrType =====
// ========================================

#define FOR_ScrollBarAttrType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (ScrollBarAttrType, 2, FOR_ScrollBarAttrType_FIELDS)

#define FOR_ScrollBarAttrType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						flags)						\


// ========================================
// ===== ScrollBarType =====
// ========================================

#define FOR_ScrollBarType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (ScrollBarType, 24, FOR_ScrollBarType_FIELDS)

#define FOR_ScrollBarType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		RectangleType,				bounds)						\
	DO_TO_FIELD (8,		UInt16,						id)							\
	DO_TO_FIELD (10,	ScrollBarAttrType,			attr)						\
	DO_TO_FIELD (12,	Int16,						value)						\
	DO_TO_FIELD (14,	Int16,						minValue)					\
	DO_TO_FIELD (16,	Int16,						maxValue)					\
	DO_TO_FIELD (18,	Int16,						pageSize)					\
	DO_TO_FIELD (20,	Int16,						penPosInCar)				\
	DO_TO_FIELD (22,	Int16,						savePos)					\


// ========================================
// ===== SED1375RegsType =====
// ========================================

#define FOR_SED1375RegsType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SED1375RegsType, 32, FOR_SED1375RegsType_FIELDS)

#define FOR_SED1375RegsType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt8,						productRevisionCode)		\
	DO_TO_FIELD (1,		UInt8,						mode0)						\
	DO_TO_FIELD (2,		UInt8,						mode1)						\
	DO_TO_FIELD (3,		UInt8,						mode2)						\
	DO_TO_FIELD (4,		UInt8,						horizontalPanelSize)		\
	DO_TO_FIELD (5,		UInt8,						verticalPanelSizeLSB)		\
	DO_TO_FIELD (6,		UInt8,						verticalPanelSizeMSB)		\
	DO_TO_FIELD (7,		UInt8,						FPLineStartPosition)		\
	DO_TO_FIELD (8,		UInt8,						horizontalNonDisplayPeriod)	\
	DO_TO_FIELD (9,		UInt8,						FPFRAMEStartPosition)		\
	DO_TO_FIELD (10,	UInt8,						verticalNonDisplayPeriod)	\
	DO_TO_FIELD (11,	UInt8,						MODRate)					\
	DO_TO_FIELD (12,	UInt8,						screen1StartAddressLSB)		\
	DO_TO_FIELD (13,	UInt8,						screen1StartAddressMSB)		\
	DO_TO_FIELD (14,	UInt8,						screen2StartAddressLSB)		\
	DO_TO_FIELD (15,	UInt8,						screen2StartAddressMSB)		\
	DO_TO_FIELD (16,	UInt8,						screen1StartAddressMSBit)	\
	DO_TO_FIELD (17,	UInt8,						memoryAddressOffset)		\
	DO_TO_FIELD (18,	UInt8,						screen1VerticalSizeLSB)		\
	DO_TO_FIELD (19,	UInt8,						screen1VerticalSizeMSB)		\
	DO_TO_FIELD (20,	UInt8,						unused1)		\
	DO_TO_FIELD (21,	UInt8,						lookUpTableAddress)			\
	DO_TO_FIELD (22,	UInt8,						unused2)					\
	DO_TO_FIELD (23,	UInt8,						lookUpTableData)			\
	DO_TO_FIELD (24,	UInt8,						GPIOConfigurationControl)	\
	DO_TO_FIELD (25,	UInt8,						GPIOStatusControl)			\
	DO_TO_FIELD (26,	UInt8,						scratchPad)					\
	DO_TO_FIELD (27,	UInt8,						portraitMode)				\
	DO_TO_FIELD (28,	UInt8,						lineByteCountRegister)		\
	DO_TO_FIELD (29,	UInt8,						unused3)					\
	DO_TO_FIELD (30,	UInt8,						unused4)					\
	DO_TO_FIELD (31,	UInt8,						unused5)					\


// ========================================
// ===== SED1376RegsType =====
// ========================================

#define FOR_SED1376RegsType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SED1376RegsType, 0xB4, FOR_SED1376RegsType_FIELDS)

#define FOR_SED1376RegsType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0x00,	UInt8,						productRevisionCode)		\
	DO_TO_FIELD (0x01,	UInt8,						displayBufferSize)			\
	DO_TO_FIELD (0x02,	UInt8,						configurationReadback)		\
	DO_TO_FIELD (0x04,	UInt8,						memoryClockConfiguration)	\
	DO_TO_FIELD (0x05,	UInt8,						pixelClockConfiguration)	\
	DO_TO_FIELD (0x08,	UInt8,						lutWriteBlue)				\
	DO_TO_FIELD (0x09,	UInt8,						lutWriteGreen)				\
	DO_TO_FIELD (0x0A,	UInt8,						lutWriteRed)				\
	DO_TO_FIELD (0x0B,	UInt8,						lutWriteAddress)			\
	DO_TO_FIELD (0x0C,	UInt8,						lutReadBlue)				\
	DO_TO_FIELD (0x0D,	UInt8,						lutReadGreen)				\
	DO_TO_FIELD (0x0E,	UInt8,						lutReadRed)					\
	DO_TO_FIELD (0x0F,	UInt8,						lutReadAddress)				\
	DO_TO_FIELD (0x10,	UInt8,						panelType)					\
	DO_TO_FIELD (0x11,	UInt8,						MODRate)					\
	DO_TO_FIELD (0x12,	UInt8,						horizontalTotal)			\
	DO_TO_FIELD (0x14,	UInt8,						horizontalPeriod)			\
	DO_TO_FIELD (0x16,	UInt8,						horizontalPeriodStart0)		\
	DO_TO_FIELD (0x17,	UInt8,						horizontalPeriodStart1)		\
	DO_TO_FIELD (0x18,	UInt8,						verticalTotal0)				\
	DO_TO_FIELD (0x19,	UInt8,						verticalTotal1)				\
	DO_TO_FIELD (0x1C,	UInt8,						verticalPeriod0)			\
	DO_TO_FIELD (0x1D,	UInt8,						verticalPeriod1)			\
	DO_TO_FIELD (0x1E,	UInt8,						verticalPeriodStart0)		\
	DO_TO_FIELD (0x1F,	UInt8,						verticalPeriodStart1)		\
	DO_TO_FIELD (0x20,	UInt8,						FPLINEPulseWidth)			\
	DO_TO_FIELD (0x22,	UInt8,						FPLINEPulseStart0)			\
	DO_TO_FIELD (0x23,	UInt8,						FPLINEPulseStart1)			\
	DO_TO_FIELD (0x24,	UInt8,						FPFRAMEPulseWidth)			\
	DO_TO_FIELD (0x26,	UInt8,						FPFRAMEPulseStart0)			\
	DO_TO_FIELD (0x27,	UInt8,						FPFRAMEPulseStart1)			\
	DO_TO_FIELD (0x28,	UInt8,						DTFD_GCPIndex)				\
	DO_TO_FIELD (0x2C,	UInt8,						DTFD_GCPData)				\
	DO_TO_FIELD (0x70,	UInt8,						displayMode)				\
	DO_TO_FIELD (0x71,	UInt8,						specialEffects)				\
	DO_TO_FIELD (0x74,	UInt8,						mainStartAddress0)			\
	DO_TO_FIELD (0x75,	UInt8,						mainStartAddress1)			\
	DO_TO_FIELD (0x76,	UInt8,						mainStartAddress2)			\
	DO_TO_FIELD (0x78,	UInt8,						mainLineAddressOffset0)		\
	DO_TO_FIELD (0x79,	UInt8,						mainLineAddressOffset1)		\
	DO_TO_FIELD (0x7C,	UInt8,						ovlyStartAddress0)			\
	DO_TO_FIELD (0x7D,	UInt8,						ovlyStartAddress1)			\
	DO_TO_FIELD (0x7E,	UInt8,						ovlyStartAddress2)			\
	DO_TO_FIELD (0x80,	UInt8,						ovlyLineAddressOffset0)		\
	DO_TO_FIELD (0x81,	UInt8,						ovlyLineAddressOffset1)		\
	DO_TO_FIELD (0x84,	UInt8,						ovlyStartXPosition0)		\
	DO_TO_FIELD (0x85,	UInt8,						ovlyStartXPosition1)		\
	DO_TO_FIELD (0x88,	UInt8,						ovlyStartYPosition0)		\
	DO_TO_FIELD (0x89,	UInt8,						ovlyStartYPosition1)		\
	DO_TO_FIELD (0x8C,	UInt8,						ovlyEndXPosition0)			\
	DO_TO_FIELD (0x8D,	UInt8,						ovlyEndXPosition1)			\
	DO_TO_FIELD (0x90,	UInt8,						ovlyEndYPosition0)			\
	DO_TO_FIELD (0x91,	UInt8,						ovlyEndYPosition1)			\
	DO_TO_FIELD (0xA0,	UInt8,						powerSaveConfiguration)		\
	DO_TO_FIELD (0xA1,	UInt8,						reserved)					\
	DO_TO_FIELD (0xA2,	UInt8,						SoftwareReset)				\
	DO_TO_FIELD (0xA4,	UInt8,						scratchPad0)				\
	DO_TO_FIELD (0xA5,	UInt8,						scratchPad1)				\
	DO_TO_FIELD (0xA8,	UInt8,						GPIOConfiguration0)			\
	DO_TO_FIELD (0xA9,	UInt8,						GPIOConfiguration1)			\
	DO_TO_FIELD (0xAC,	UInt8,						GPIOStatusControl0)			\
	DO_TO_FIELD (0xAD,	UInt8,						GPIOStatusControl1)			\
	DO_TO_FIELD (0xB0,	UInt8,						PWMClockCVPulseControl)		\
	DO_TO_FIELD (0xB1,	UInt8,						PWMClockCVPulseConfig)		\
	DO_TO_FIELD (0xB2,	UInt8,						CVPulseBurstLength)			\
	DO_TO_FIELD (0xB3,	UInt8,						PWMOutDutyCycle)			\


// ========================================
// ===== SliderControlType =====
// ========================================

#define FOR_SliderControlType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SliderControlType, 30, FOR_SliderControlType_FIELDS)

#define FOR_SliderControlType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						id)							\
	DO_TO_FIELD (2,		RectangleType,				bounds)						\
	DO_TO_FIELD (10,	DmResID,					thumbID)					\
	DO_TO_FIELD (12,	DmResID,					backgroundID)				\
	DO_TO_FIELD (14,	ControlAttrType,			attr)						\
	DO_TO_FIELD (16,	ControlStyleType,			style)						\
	DO_TO_FIELD (17,	UInt8,						reserved)					\
	DO_TO_FIELD (18,	UInt16,						minValue)					\
	DO_TO_FIELD (20,	UInt16,						maxValue)					\
	DO_TO_FIELD (22,	UInt16,						pageSize)					\
	DO_TO_FIELD (24,	UInt16,						value)						\
	DO_TO_FIELD (26,	emuptr,						activeSliderP)				\


// ========================================
// ===== SlkPktFooterType =====
// ========================================

#define FOR_SlkPktFooterType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SlkPktFooterType, 2, FOR_SlkPktFooterType_FIELDS)

#define FOR_SlkPktFooterType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						crc16)						\


// ========================================
// ===== SlkPktHeaderType =====
// ========================================

#define FOR_SlkPktHeaderType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SlkPktHeaderType, 10, FOR_SlkPktHeaderType_FIELDS)

#define FOR_SlkPktHeaderType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						signature1)					\
	DO_TO_FIELD (2,		UInt8,						signature2)					\
	DO_TO_FIELD (3,		UInt8,						dest)						\
	DO_TO_FIELD (4,		UInt8,						src)						\
	DO_TO_FIELD (5,		UInt8,						type)						\
	DO_TO_FIELD (6,		UInt16,						bodySize)					\
	DO_TO_FIELD (8,		UInt8,						transId)					\
	DO_TO_FIELD (9,		SlkPktHeaderChecksum,		checksum)					\


// ========================================
// ===== SndCommandType =====
// ========================================

#define FOR_SndCommandType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (SndCommandType, 10, FOR_SndCommandType_FIELDS)

#define FOR_SndCommandType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt8,						cmd)						\
	DO_TO_FIELD (1,		UInt8,						reserved)					\
	DO_TO_FIELD (2,		Int32,						param1)						\
	DO_TO_FIELD (6,		UInt16,						param2)						\
	DO_TO_FIELD (8,		UInt16,						param3)						\


// ========================================
// ===== StorageHeaderType =====
// ========================================

#define FOR_StorageHeaderType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (StorageHeaderType, 76, FOR_StorageHeaderType_FIELDS)

#define FOR_StorageHeaderType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt32,						signature)					\
	DO_TO_FIELD (4,		UInt16,						version)					\
	DO_TO_FIELD (6,		UInt16,						flags)						\
	DO_TO_FIELD (8,		UInt8,						name)						\
	DO_TO_FIELD (40,	UInt32,						creationDate)				\
	DO_TO_FIELD (44,	UInt32,						backupDate)					\
	DO_TO_FIELD (48,	UInt32,						heapListOffset)				\
	DO_TO_FIELD (52,	UInt32,						initCodeOffset1)			\
	DO_TO_FIELD (56,	UInt32,						initCodeOffset2)			\
	DO_TO_FIELD (60,	LocalID,					databaseDirID)				\
	DO_TO_FIELD (64,	UInt32,						rsvSpace)					\
	DO_TO_FIELD (68,	UInt32,						dynHeapSpace)				\
	DO_TO_FIELD (72,	UInt32,						firstRAMBlockSize)			\


// ========================================
// ===== SysAppInfoType =====
// ========================================

#define FOR_SysAppInfoType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (SysAppInfoType, 60, FOR_SysAppInfoType_FIELDS)

#define FOR_SysAppInfoType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		Int16,						cmd)						\
	DO_TO_FIELD (2,		emuptr,						cmdPBP)						\
	DO_TO_FIELD (6,		UInt16,						launchFlags)				\
	DO_TO_FIELD (8,		UInt32,						taskID)						\
	DO_TO_FIELD (12,	emuptr,						codeH)						\
	DO_TO_FIELD (16,	emuptr,						dbP)						\
	DO_TO_FIELD (20,	emuptr,						stackP)						\
	DO_TO_FIELD (24,	emuptr,						globalsChunkP)				\
	DO_TO_FIELD (28,	UInt16,						memOwnerID)					\
	DO_TO_FIELD (30,	emuptr,						dmAccessP)					\
	DO_TO_FIELD (34,	Err,						dmLastErr)					\
	DO_TO_FIELD (36,	emuptr,						errExceptionP)				\
	DO_TO_FIELD (40,	emuptr,						a5Ptr)						\
	DO_TO_FIELD (44,	emuptr,						stackEndP)					\
	DO_TO_FIELD (48,	emuptr,						globalEndP)					\
	DO_TO_FIELD (52,	emuptr,						rootP)						\
	DO_TO_FIELD (56,	emuptr,						extraP)						\


// ========================================
// ===== SysBatteryDataStructV1 =====
// ========================================

#define FOR_SysBatteryDataStructV1_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysBatteryDataStructV1, 50, FOR_SysBatteryDataStructV1_FIELDS)

#define FOR_SysBatteryDataStructV1_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt16,						sysBattDataStructVersion)	\
	DO_TO_FIELD (2,		UInt16,						sysBattMildWrngVoltage)		\
	DO_TO_FIELD (4,		UInt16,						sysBattSevereWrngVoltage)	\
	DO_TO_FIELD (6,		UInt16,						sysBattShutdownEnVoltage)	\
	DO_TO_FIELD (8,		UInt16,						sysBattShutdownDisVoltage)	\
	DO_TO_FIELD (10,	UInt16,						sysBattMinIRVoltage)		\
	DO_TO_FIELD (12,	UInt32,						sysBattMildWrngTimeout)		\
	DO_TO_FIELD (16,	UInt32,						sysBattSevereWrngTimeout)	\
	DO_TO_FIELD (20,	UInt16,						sysBattCritWarnOffDays)		\
	DO_TO_FIELD (22,	UInt32,						sysBattCritWarnOnTicks)		\
	DO_TO_FIELD (26,	UInt16,						sysBattStepsPerVolt)		\
	DO_TO_FIELD (28,	Int16,						sysBattVoltageStepOffset)	\
	DO_TO_FIELD (30,	UInt32,						sysBattReserved1)			\
	DO_TO_FIELD (34,	UInt32,						sysBattReserved2)			\
	DO_TO_FIELD (38,	UInt16,						sysBattVoltageCurve)		\


// ========================================
// ===== SysBatteryDataStructV2 =====
// ========================================

#define FOR_SysBatteryDataStructV2_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysBatteryDataStructV2, 54, FOR_SysBatteryDataStructV2_FIELDS)

#define FOR_SysBatteryDataStructV2_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt16,						sysBattDataStructVersion)	\
	DO_TO_FIELD (2,		UInt16,						sysBattMildWrngVoltage)		\
	DO_TO_FIELD (4,		UInt16,						sysBattSevereWrngVoltage)	\
	DO_TO_FIELD (6,		UInt16,						sysBattMildWrngPercent)		\
	DO_TO_FIELD (8,		UInt16,						sysBattSevereWrngPercent)	\
	DO_TO_FIELD (10,	UInt16,						sysBattShutdownEnVoltage)	\
	DO_TO_FIELD (12,	UInt16,						sysBattShutdownDisVoltage)	\
	DO_TO_FIELD (14,	UInt16,						sysBattMinIRVoltage)		\
	DO_TO_FIELD (16,	UInt32,						sysBattMildWrngTimeout)		\
	DO_TO_FIELD (20,	UInt32,						sysBattSevereWrngTimeout)	\
	DO_TO_FIELD (24,	UInt16,						sysBattCritWarnOffDays)		\
	DO_TO_FIELD (26,	UInt32,						sysBattCritWarnOnTicks)		\
	DO_TO_FIELD (30,	UInt16,						sysBattStepsPerVolt)		\
	DO_TO_FIELD (32,	Int16,						sysBattVoltageStepOffset)	\
	DO_TO_FIELD (34,	UInt32,						sysBattReserved1)			\
	DO_TO_FIELD (38,	UInt32,						sysBattReserved2)			\
	DO_TO_FIELD (42,	UInt16,						sysBattVoltageCurve)		\


// ========================================
// ===== SysBatteryDataStructV3 =====
// ========================================

#define FOR_SysBatteryDataStructV3_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysBatteryDataStructV3, 38, FOR_SysBatteryDataStructV3_FIELDS)

#define FOR_SysBatteryDataStructV3_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt16,						sysBattDataStructVersion)	\
	DO_TO_FIELD (2,		UInt16,						sysBattShutdownEnVoltage)	\
	DO_TO_FIELD (4,		UInt16,						sysBattShutdownDisVoltage)	\
	DO_TO_FIELD (6,		UInt16,						sysBattTableFlags)			\
	DO_TO_FIELD (8,		UInt16,						sysBattCritWarnOffDays)		\
	DO_TO_FIELD (10,	UInt32,						sysBattCritWarnOnTicks)		\
	DO_TO_FIELD (14,	UInt16,						sysBattStepsPerVolt)		\
	DO_TO_FIELD (16,	Int16,						sysBattVoltageStepOffset)	\
	DO_TO_FIELD (18,	UInt32,						sysBattReserved1)			\
	DO_TO_FIELD (22,	UInt32,						sysBattReserved2)			\
	DO_TO_FIELD (26,	UInt16,						sysBattVoltageCurve)		\


// ========================================
// ===== SysLibTblEntryType =====
// ========================================

#define FOR_SysLibTblEntryType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysLibTblEntryType, 10, FOR_SysLibTblEntryType_FIELDS)

#define FOR_SysLibTblEntryType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		emuptr,						dispatchTblP)				\
	DO_TO_FIELD (4,		emuptr,						globalsP)					\
	DO_TO_FIELD (8,		LocalID,					dbID)						\
	DO_TO_FIELD (12,	emuptr,						codeRscH)					\


// ========================================
// ===== SysKernelInfoType =====
// ========================================

#define FOR_SysKernelInfoType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysKernelInfoType, 62, FOR_SysKernelInfoType_FIELDS)

#define FOR_SysKernelInfoType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						selector)					\
	DO_TO_FIELD (1,		UInt8,						reserved)					\
	DO_TO_FIELD (2,		UInt32,						id)							\
	DO_TO_FIELD (6,		kernel_info_task,			task)						\
	DO_TO_FIELD (6,		kernel_info_semaphore,		semaphore)					\
	DO_TO_FIELD (6,		kernel_info_timer,			timer)						\


// ========================================
// ===== kernel_info_task =====
// ========================================

#define FOR_kernel_info_task_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (kernel_info_task, 56, FOR_kernel_info_task_FIELDS)

#define FOR_kernel_info_task_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt32,						id)							\
	DO_TO_FIELD (4,		UInt32,						nextID)						\
	DO_TO_FIELD (8,		UInt32,						tag)						\
	DO_TO_FIELD (12,	UInt32,						status)						\
	DO_TO_FIELD (16,	UInt32,						timer)						\
	DO_TO_FIELD (20,	UInt32,						timeSlice)					\
	DO_TO_FIELD (24,	Int16,						priority)					\
	DO_TO_FIELD (26,	UInt32,						attributes)					\
	DO_TO_FIELD (30,	Int16,						pendingCalls)				\
	DO_TO_FIELD (32,	UInt32,						senderTaskID)				\
	DO_TO_FIELD (36,	UInt32,						msgExchangeID)				\
	DO_TO_FIELD (40,	UInt32,						tcbP)						\
	DO_TO_FIELD (44,	UInt32,						stackP)						\
	DO_TO_FIELD (48,	UInt32,						stackStart)					\
	DO_TO_FIELD (52,	UInt32,						stackSize)					\


// ========================================
// ===== kernel_info_semaphore =====
// ========================================

#define FOR_kernel_info_semaphore_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (kernel_info_semaphore, 22, FOR_kernel_info_semaphore_FIELDS)

#define FOR_kernel_info_semaphore_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt32,						id)							\
	DO_TO_FIELD (4,		UInt32,						nextID)						\
	DO_TO_FIELD (8,		UInt32,						tag)						\
	DO_TO_FIELD (12,	Int16,						initValue)					\
	DO_TO_FIELD (14,	Int16,						curValue)					\
	DO_TO_FIELD (16,	Int16,						nestLevel)					\
	DO_TO_FIELD (18,	UInt32,						ownerID)					\


// ========================================
// ===== kernel_info_timer =====
// ========================================

#define FOR_kernel_info_timer_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (kernel_info_timer, 24, FOR_kernel_info_timer_FIELDS)

#define FOR_kernel_info_timer_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt32,						id)							\
	DO_TO_FIELD (4,		UInt32,						nextID)						\
	DO_TO_FIELD (8,		UInt32,						tag)						\
	DO_TO_FIELD (12,	UInt32,						ticksLeft)					\
	DO_TO_FIELD (16,	UInt32,						period)						\
	DO_TO_FIELD (20,	UInt32,						proc)						\


// ========================================
// ===== SysNVParamsType =====
// ========================================

#define FOR_SysNVParamsType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysNVParamsType, 30, FOR_SysNVParamsType_FIELDS)

#define FOR_SysNVParamsType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt32,						rtcHours)					\
	DO_TO_FIELD (4,		UInt32,						rtcHourMinSecCopy)			\
	DO_TO_FIELD (8,		UInt8,						swrLCDContrastValue)		\
	DO_TO_FIELD (9,		UInt8,						swrLCDBrightnessValue)		\
	DO_TO_FIELD (10,	emuptr,						splashScreenPtr)			\
	DO_TO_FIELD (14,	emuptr,						hardResetScreenPtr)			\
	DO_TO_FIELD (18,	UInt16,						localeLanguage)				\
	DO_TO_FIELD (20,	UInt16,						localeCountry)				\
	DO_TO_FIELD (22,	UInt32,						sysNVOEMStorage1)			\
	DO_TO_FIELD (26,	UInt32,						sysNVOEMStorage2)			\


// ========================================
// ===== SysPktBodyType =====
// ========================================

#define FOR_SysPktBodyType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (SysPktBodyType, sysPktMaxBodySize, FOR_SysPktBodyType_FIELDS)

#define FOR_SysPktBodyType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		UInt8,						data)						\


// ========================================
// ===== SysPktContinueCmdType =====
// ========================================

#define FOR_SysPktContinueCmdType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktContinueCmdType, 90, FOR_SysPktContinueCmdType_FIELDS)

#define FOR_SysPktContinueCmdType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		M68KRegsType,				regs)						\
	DO_TO_FIELD (76,	Boolean,					stepSpy)					\
	DO_TO_FIELD (77,	UInt8,						reserved)					\
	DO_TO_FIELD (78,	UInt32,						ssAddr)						\
	DO_TO_FIELD (82,	UInt32,						ssCount)					\
	DO_TO_FIELD (86,	UInt32,						ssCheckSum)					\


// ========================================
// ===== SysPktDbgBreakToggleCmdType =====
// ========================================

#define FOR_SysPktDbgBreakToggleCmdType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktDbgBreakToggleCmdType, 2, FOR_SysPktDbgBreakToggleCmdType_FIELDS)

#define FOR_SysPktDbgBreakToggleCmdType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktDbgBreakToggleRspType =====
// ========================================

#define FOR_SysPktDbgBreakToggleRspType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktDbgBreakToggleRspType, 4, FOR_SysPktDbgBreakToggleRspType_FIELDS)

#define FOR_SysPktDbgBreakToggleRspType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		Boolean,					newState)					\
	DO_TO_FIELD (3,		UInt8,						reserved)					\


// ========================================
// ===== SysPktFindCmdType =====
// ========================================

#define FOR_SysPktFindCmdType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysPktFindCmdType, sysPktMaxBodySize, FOR_SysPktFindCmdType_FIELDS)

#define FOR_SysPktFindCmdType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		UInt32,						firstAddr)					\
	DO_TO_FIELD (6,		UInt32,						lastAddr)					\
	DO_TO_FIELD (10,	UInt16,						numBytes)					\
	DO_TO_FIELD (12,	Boolean,					caseInsensitive)			\
	DO_TO_FIELD (13,	UInt8,						padding)					\
	DO_TO_FIELD (14,	UInt8,						data)						\


// ========================================
// ===== SysPktFindRspType =====
// ========================================

#define FOR_SysPktFindRspType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysPktFindRspType, 8, FOR_SysPktFindRspType_FIELDS)

#define FOR_SysPktFindRspType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		emuptr,						addr)						\
	DO_TO_FIELD (6,		UInt8,						found)						\
	DO_TO_FIELD (7,		UInt8,						padding)					\


// ========================================
// ===== SysPktGetBreakpointsCmdType =====
// ========================================

#define FOR_SysPktGetBreakpointsCmdType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktGetBreakpointsCmdType, 2, FOR_SysPktGetBreakpointsCmdType_FIELDS)

#define FOR_SysPktGetBreakpointsCmdType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktEmptyRspType =====
// ========================================

#define FOR_SysPktEmptyRspType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysPktEmptyRspType, 2, FOR_SysPktEmptyRspType_FIELDS)

#define FOR_SysPktEmptyRspType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktGetBreakpointsRspType =====
// ========================================

#define FOR_SysPktGetBreakpointsRspType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktGetBreakpointsRspType, 38, FOR_SysPktGetBreakpointsRspType_FIELDS)

#define FOR_SysPktGetBreakpointsRspType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		BreakpointType,				bp)							\


// ========================================
// ===== SysPktGetTrapBreaksCmdType =====
// ========================================

#define FOR_SysPktGetTrapBreaksCmdType_STRUCT(DO_TO_STRUCT)						\
	DO_TO_STRUCT (SysPktGetTrapBreaksCmdType, 2, FOR_SysPktGetTrapBreaksCmdType_FIELDS)

#define FOR_SysPktGetTrapBreaksCmdType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktGetTrapBreaksRspType =====
// ========================================

#define FOR_SysPktGetTrapBreaksRspType_STRUCT(DO_TO_STRUCT)						\
	DO_TO_STRUCT (SysPktGetTrapBreaksRspType, 12, FOR_SysPktGetTrapBreaksRspType_FIELDS)

#define FOR_SysPktGetTrapBreaksRspType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		UInt16,						trapBP)						\


// ========================================
// ===== SysPktGetTrapConditionsCmdType =====
// ========================================

#define FOR_SysPktGetTrapConditionsCmdType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktGetTrapConditionsCmdType, 2, FOR_SysPktGetTrapConditionsCmdType_FIELDS)

#define FOR_SysPktGetTrapConditionsCmdType_FIELDS(DO_TO_FIELD)					\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktGetTrapConditionsRspType =====
// ========================================

#define FOR_SysPktGetTrapConditionsRspType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktGetTrapConditionsRspType, 12, FOR_SysPktGetTrapConditionsRspType_FIELDS)

#define FOR_SysPktGetTrapConditionsRspType_FIELDS(DO_TO_FIELD)					\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		UInt16,						trapParam)					\


// ========================================
// ===== SysPktReadMemCmdType =====
// ========================================

#define FOR_SysPktReadMemCmdType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktReadMemCmdType, 8, FOR_SysPktReadMemCmdType_FIELDS)

#define FOR_SysPktReadMemCmdType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		emuptr,						address)					\
	DO_TO_FIELD (6,		UInt16,						numBytes)					\


// ========================================
// ===== SysPktReadMemRspType =====
// ========================================

#define FOR_SysPktReadMemRspType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktReadMemRspType, sysPktMaxBodySize, FOR_SysPktReadMemRspType_FIELDS)

#define FOR_SysPktReadMemRspType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		char,						data)						\


// ========================================
// ===== SysPktReadRegsCmdType =====
// ========================================

#define FOR_SysPktReadRegsCmdType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktReadRegsCmdType, 2, FOR_SysPktReadRegsCmdType_FIELDS)

#define FOR_SysPktReadRegsCmdType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktReadRegsRspType =====
// ========================================

#define FOR_SysPktReadRegsRspType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktReadRegsRspType, 76, FOR_SysPktReadRegsRspType_FIELDS)

#define FOR_SysPktReadRegsRspType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		M68KRegsType,				reg)						\


// ========================================
// ===== SysPktRPC2Type =====
// ========================================

#define FOR_SysPktRPC2Type_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (SysPktRPC2Type, sysPktMaxBodySize, FOR_SysPktRPC2Type_FIELDS)

#define FOR_SysPktRPC2Type_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		UInt16,						trapWord)					\
	DO_TO_FIELD (4,		UInt32,						resultD0)					\
	DO_TO_FIELD (8,		UInt32,						resultA0)					\
	DO_TO_FIELD (12,	UInt16,						resultException)			\
	DO_TO_FIELD (14,	UInt8,						DRegMask)					\
	DO_TO_FIELD (15,	UInt8,						ARegMask)					\
	DO_TO_FIELD (16,	UInt32,						Regs)						\
	DO_TO_FIELD (1,		UInt16,						numParams)					\
	DO_TO_FIELD (1,		char,						param)						\


// ========================================
// ===== SysPktRPCParamType =====
// ========================================

#define FOR_SysPktRPCParamType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysPktRPCParamType, 6, FOR_SysPktRPCParamType_FIELDS)

#define FOR_SysPktRPCParamType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						byRef)						\
	DO_TO_FIELD (1,		UInt8,						size)						\
	DO_TO_FIELD (2,		UInt8,						asByte)						\
	DO_TO_FIELD (2,		UInt16,						asShort)					\
	DO_TO_FIELD (2,		UInt32,						asLong)						\


// ========================================
// ===== SysPktRPCType =====
// ========================================

#define FOR_SysPktRPCType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (SysPktRPCType, sysPktMaxBodySize, FOR_SysPktRPCType_FIELDS)

#define FOR_SysPktRPCType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		UInt16,						trapWord)					\
	DO_TO_FIELD (4,		UInt32,						resultD0)					\
	DO_TO_FIELD (8,		UInt32,						resultA0)					\
	DO_TO_FIELD (12,	UInt16,						numParams)					\
	DO_TO_FIELD (14,	char,						param)						\


// ========================================
// ===== SysPktRtnNameCmdType =====
// ========================================

#define FOR_SysPktRtnNameCmdType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktRtnNameCmdType, 6, FOR_SysPktRtnNameCmdType_FIELDS)

#define FOR_SysPktRtnNameCmdType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		emuptr,						address)					\


// ========================================
// ===== SysPktRtnNameRspType =====
// ========================================

#define FOR_SysPktRtnNameRspType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktRtnNameRspType, (14 + sysPktMaxNameLen), FOR_SysPktRtnNameRspType_FIELDS)

#define FOR_SysPktRtnNameRspType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		emuptr,						address)					\
	DO_TO_FIELD (6,		emuptr,						startAddr)					\
	DO_TO_FIELD (10,	emuptr,						endAddr)					\
	DO_TO_FIELD (14,	char,						name)						\


// ========================================
// ===== SysPktSetBreakpointsCmdType =====
// ========================================

#define FOR_SysPktSetBreakpointsCmdType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktSetBreakpointsCmdType, 38, FOR_SysPktSetBreakpointsCmdType_FIELDS)

#define FOR_SysPktSetBreakpointsCmdType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		BreakpointType,				bp)							\


// ========================================
// ===== SysPktSetBreakpointsRspType =====
// ========================================

#define FOR_SysPktSetBreakpointsRspType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktSetBreakpointsRspType, 2, FOR_SysPktSetBreakpointsRspType_FIELDS)

#define FOR_SysPktSetBreakpointsRspType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktSetTrapBreaksCmdType =====
// ========================================

#define FOR_SysPktSetTrapBreaksCmdType_STRUCT(DO_TO_STRUCT)						\
	DO_TO_STRUCT (SysPktSetTrapBreaksCmdType, 12, FOR_SysPktSetTrapBreaksCmdType_FIELDS)

#define FOR_SysPktSetTrapBreaksCmdType_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		UInt16,						trapBP)						\


// ========================================
// ===== SysPktSetTrapConditionsCmdType =====
// ========================================

#define FOR_SysPktSetTrapConditionsCmdType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktSetTrapConditionsCmdType, 12, FOR_SysPktSetTrapConditionsCmdType_FIELDS)

#define FOR_SysPktSetTrapConditionsCmdType_FIELDS(DO_TO_FIELD)					\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		UInt16,						trapParam)					\


// ========================================
// ===== SysPktSetTrapConditionsRspType =====
// ========================================

#define FOR_SysPktSetTrapConditionsRspType_STRUCT(DO_TO_STRUCT)					\
	DO_TO_STRUCT (SysPktSetTrapConditionsRspType, 2, FOR_SysPktSetTrapConditionsRspType_FIELDS)

#define FOR_SysPktSetTrapConditionsRspType_FIELDS(DO_TO_FIELD)					\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktStateCmdType =====
// ========================================

#define FOR_SysPktStateCmdType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysPktStateCmdType, 2, FOR_SysPktStateCmdType_FIELDS)

#define FOR_SysPktStateCmdType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktStateRspType =====
// ========================================

#define FOR_SysPktStateRspType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (SysPktStateRspType, 188, FOR_SysPktStateRspType_FIELDS)

#define FOR_SysPktStateRspType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		Boolean,					resetted)					\
	DO_TO_FIELD (3,		UInt8,						reserved1)					\
	DO_TO_FIELD (4,		UInt16,						exceptionId)				\
	DO_TO_FIELD (6,		M68KRegsType,				reg)						\
	DO_TO_FIELD (80,	UInt16,						inst)						\
	DO_TO_FIELD (110,	BreakpointType,				bp)							\
	DO_TO_FIELD (146,	emuptr,						startAddr)					\
	DO_TO_FIELD (150,	emuptr,						endAddr)					\
	DO_TO_FIELD (154,	char,						name)						\
	DO_TO_FIELD (186,	UInt8,						trapTableRev)				\
	DO_TO_FIELD (187,	UInt8,						reserved2)					\


// ========================================
// ===== SysPktWriteMemCmdType =====
// ========================================

#define FOR_SysPktWriteMemCmdType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktWriteMemCmdType, sysPktMaxBodySize, FOR_SysPktWriteMemCmdType_FIELDS)

#define FOR_SysPktWriteMemCmdType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		emuptr,						address)					\
	DO_TO_FIELD (6,		UInt16,						numBytes)					\
	DO_TO_FIELD (8,		char,						data)						\


// ========================================
// ===== SysPktWriteMemRspType =====
// ========================================

#define FOR_SysPktWriteMemRspType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktWriteMemRspType, 2, FOR_SysPktWriteMemRspType_FIELDS)

#define FOR_SysPktWriteMemRspType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


// ========================================
// ===== SysPktWriteRegsCmdType =====
// ========================================

#define FOR_SysPktWriteRegsCmdType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktWriteRegsCmdType, 76, FOR_SysPktWriteRegsCmdType_FIELDS)

#define FOR_SysPktWriteRegsCmdType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\
	DO_TO_FIELD (2,		M68KRegsType,				reg)						\


// ========================================
// ===== SysPktWriteRegsRspType =====
// ========================================

#define FOR_SysPktWriteRegsRspType_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (SysPktWriteRegsRspType, 2, FOR_SysPktWriteRegsRspType_FIELDS)

#define FOR_SysPktWriteRegsRspType_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt8,						command)					\
	DO_TO_FIELD (1,		UInt8,						_filler)					\


/* Note: Several of the following structures have a numeral in their
         name. These serve to distinguish the separate versions,
         and are roughly related to the version of the ROM heap when
         the particular variant was introduced. */


// ========================================
// ===== ROMCardHeader5Type =====
// ========================================

#define FOR_ROMCardHeader5Type_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (ROMCardHeader5Type, 256, FOR_ROMCardHeader5Type_FIELDS)

#define FOR_ROMCardHeader5Type_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt32,						initStack)					\
	DO_TO_FIELD (4,		UInt32,						resetVector)				\
	DO_TO_FIELD (8,		UInt32,						signature)					\
	DO_TO_FIELD (12,	UInt16,						hdrVersion)					\
	DO_TO_FIELD (14,	UInt16,						flags)						\
	DO_TO_FIELD (16,	char,						cardName)					\
	DO_TO_FIELD (48,	char,						cardManuf)					\
	DO_TO_FIELD (80,	UInt16,						cardVersion)				\
	DO_TO_FIELD (82,	UInt32,						cardTime)					\
	DO_TO_FIELD (86,	UInt16,						unknown1)					\
	DO_TO_FIELD (88,	UInt32,						ramlist)					\
	DO_TO_FIELD (92,	UInt32,						rwoffset)					\
	DO_TO_FIELD (96,	UInt32,						rwlength)					\
	DO_TO_FIELD (100,	UInt32,						rooffset)					\
	DO_TO_FIELD (104,	UInt32,						bigrom)						\
	DO_TO_FIELD (108,	UInt32,						checksumLength)				\
	DO_TO_FIELD (112,	UInt16,						checksumValue)				\
	DO_TO_FIELD (114,	UInt32,						rwwoffset)					\
	DO_TO_FIELD (118,	UInt32,						rwwlength)					\
	DO_TO_FIELD (122,	UInt32,						halDispatch)				\
	DO_TO_FIELD (126,	UInt32,						companyID)					\
	DO_TO_FIELD (130,	UInt32,						halID)						\
	DO_TO_FIELD (134,	UInt32,						romVersionNumber)			\
	DO_TO_FIELD (138,	char,						romVersionString)			\
	DO_TO_FIELD (170,	char,						reserved)					\


// ========================================
// ===== ROMStoreHeader4Type =====
// ========================================

#define FOR_ROMStoreHeader4Type_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (ROMStoreHeader4Type, 256, FOR_ROMStoreHeader4Type_FIELDS)

#define FOR_ROMStoreHeader4Type_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt32,						signature)					\
	DO_TO_FIELD (4,		UInt16,						version)					\
	DO_TO_FIELD (6,		UInt16,						flags)						\
	DO_TO_FIELD (8,		char,						name)						\
	DO_TO_FIELD (40,	UInt32,						creationDate)				\
	DO_TO_FIELD (44,	UInt32,						backupDate)					\
	DO_TO_FIELD (48,	UInt32,						heapAddr)					\
	DO_TO_FIELD (52,	UInt32,						initAddr1)					\
	DO_TO_FIELD (56,	UInt32,						initAddr2)					\
	DO_TO_FIELD (60,	UInt32,						dbLID)						\
	DO_TO_FIELD (64,	UInt32,						reserved)					\
	DO_TO_FIELD (68,	UInt32,						heapSize)					\
	DO_TO_FIELD (72,	UInt32,						ramSize)					\


// ========================================
// ===== ROMHeapHeader3Type =====
// ========================================

#define FOR_ROMHeapHeader3Type_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (ROMHeapHeader3Type, 16, FOR_ROMHeapHeader3Type_FIELDS)

#define FOR_ROMHeapHeader3Type_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						flags)						\
	DO_TO_FIELD (2,		UInt32,						size)						\
	DO_TO_FIELD (6,		UInt32,						freeChunk)					\
	DO_TO_FIELD (10,	UInt16,						masterPointers)				\
	DO_TO_FIELD (12,	UInt32,						nextMasterTable)			\


// ========================================
// ===== ROMHeapHeader2Type =====
// ========================================

#define FOR_ROMHeapHeader2Type_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (ROMHeapHeader2Type, 12, FOR_ROMHeapHeader2Type_FIELDS)

#define FOR_ROMHeapHeader2Type_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						flags)						\
	DO_TO_FIELD (2,		UInt32,						size)						\
	DO_TO_FIELD (6,		UInt16,						masterPointers)				\
	DO_TO_FIELD (8,		UInt32,						nextMasterTable)			\


// ========================================
// ===== ROMHeapHeader1Type =====
// ========================================

#define FOR_ROMHeapHeader1Type_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (ROMHeapHeader1Type, 8, FOR_ROMHeapHeader1Type_FIELDS)

#define FOR_ROMHeapHeader1Type_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		UInt16,						flags)						\
	DO_TO_FIELD (2,		UInt16,						size)						\
	DO_TO_FIELD (4,		UInt16,						masterPointers)				\
	DO_TO_FIELD (6,		UInt16,						nextMasterTable)			\


// ========================================
// ===== ROMDBDir1Type =====
// ========================================

#define FOR_ROMDBDir1Type_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (ROMDBDir1Type, 6, FOR_ROMDBDir1Type_FIELDS)

#define FOR_ROMDBDir1Type_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt32,						nextTable)					\
	DO_TO_FIELD (4,		UInt16,						count)						\
	DO_TO_FIELD (6,		UInt32,						dbLID)						\


// ========================================
// ===== ROMDBRecordHeader1Type =====
// ========================================

#define FOR_ROMDBRecordHeader1Type_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (ROMDBRecordHeader1Type, 8, FOR_ROMDBRecordHeader1Type_FIELDS)

#define FOR_ROMDBRecordHeader1Type_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt32,						chunkLID)					\
	DO_TO_FIELD (4,		UInt32,						recordID)					\


// ========================================
// ===== ROMDBResourceHeader1Type =====
// ========================================

#define FOR_ROMDBResourceHeader1Type_STRUCT(DO_TO_STRUCT)						\
	DO_TO_STRUCT (ROMDBResourceHeader1Type, 10, FOR_ROMDBResourceHeader1Type_FIELDS)

#define FOR_ROMDBResourceHeader1Type_FIELDS(DO_TO_FIELD)						\
	DO_TO_FIELD (0,		UInt32,						type)						\
	DO_TO_FIELD (4,		UInt16,						id)							\
	DO_TO_FIELD (6,		UInt32,						chunkLID)					\


// ========================================
// ===== ROMDBHeader1Type =====
// ========================================

#define FOR_ROMDBHeader1Type_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (ROMDBHeader1Type, 78, FOR_ROMDBHeader1Type_FIELDS)

#define FOR_ROMDBHeader1Type_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		char,						name)						\
	DO_TO_FIELD (32,	UInt16,						flags)						\
	DO_TO_FIELD (34,	UInt16,						version)					\
	DO_TO_FIELD (36,	UInt32,						creationTime)				\
	DO_TO_FIELD (40,	UInt32,						modificationTime)			\
	DO_TO_FIELD (44,	UInt32,						backupTime)					\
	DO_TO_FIELD (48,	UInt32,						modificationNumber)			\
	DO_TO_FIELD (52,	UInt32,						appinfoLID)					\
	DO_TO_FIELD (56,	UInt32,						sortinfoLID)				\
	DO_TO_FIELD (60,	UInt32,						type)						\
	DO_TO_FIELD (64,	UInt32,						creator)					\
	DO_TO_FIELD (68,	UInt32,						uniqueIDSeed)				\
	DO_TO_FIELD (72,	UInt32,						extensionHeader)			\
	DO_TO_FIELD (76,	UInt16,						recordCount)				\
	DO_TO_FIELD (78,	ROMDBRecordHeader1Type,		records)					\
	DO_TO_FIELD (78,	ROMDBResourceHeader1Type,	resources)					\


// ========================================
// ===== ROMHeapChunkHdr2Type =====
// ========================================

#define FOR_ROMHeapChunkHdr2Type_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (ROMHeapChunkHdr2Type, 8, FOR_ROMHeapChunkHdr2Type_FIELDS)

#define FOR_ROMHeapChunkHdr2Type_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt32,						long1)						\
	DO_TO_FIELD (4,		UInt32,						long2)						\


// ========================================
// ===== ROMHeapChunkHdr1Type =====
// ========================================

#define FOR_ROMHeapChunkHdr1Type_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (ROMHeapChunkHdr1Type, 6, FOR_ROMHeapChunkHdr1Type_FIELDS)

#define FOR_ROMHeapChunkHdr1Type_FIELDS(DO_TO_FIELD)							\
	DO_TO_FIELD (0,		UInt16,						size)						\
	DO_TO_FIELD (2,		UInt8,						lockOwner)					\
	DO_TO_FIELD (3,		UInt8,						flags)						\
	DO_TO_FIELD (4,		UInt16,						hOffset)					\


// ========================================
// ===== ROMFtrFeatureType =====
// ========================================

#define FOR_ROMFtrFeatureType_STRUCT(DO_TO_STRUCT)	\
	DO_TO_STRUCT(ROMFtrFeatureType, 6, FOR_ROMFtrFeatureType_FIELDS)

#define FOR_ROMFtrFeatureType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						num)	\
	DO_TO_FIELD (2,		UInt32,						value)	\


// ========================================
// ===== ROMFtrCreatorType =====
// ========================================

#define FOR_ROMFtrCreatorType_STRUCT(DO_TO_STRUCT)	\
	DO_TO_STRUCT(ROMFtrCreatorType, 6, FOR_ROMFtrCreatorType_FIELDS)

#define FOR_ROMFtrCreatorType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt32,						creator)	\
	DO_TO_FIELD (4,		UInt16,						numEntries)	\
	DO_TO_FIELD (6,		ROMFtrFeatureType,			feature)	\


// ========================================
// ===== ROMFtrTableType =====
// ========================================

#define FOR_ROMFtrTableType_STRUCT(DO_TO_STRUCT)	\
	DO_TO_STRUCT(ROMFtrTableType, 2, FOR_ROMFtrTableType_FIELDS)

#define FOR_ROMFtrTableType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						numEntries)	\
	DO_TO_FIELD (2,		ROMFtrCreatorType,			creator)	\


// ========================================
// ===== TableAttrType =====
// ========================================

#define FOR_TableAttrType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT(TableAttrType, 2, FOR_TableAttrType_FIELDS)

#define FOR_TableAttrType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt32,						flags)						\


// ========================================
// ===== TableType =====
// ========================================

#define FOR_TableType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT(TableType, 74, FOR_TableType_FIELDS)

#define FOR_TableType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt16,						id)							\
	DO_TO_FIELD (2,		RectangleType,				bounds)						\
	DO_TO_FIELD (10,	TableAttrType,				attr)						\
	DO_TO_FIELD (12,	Int16,						numColumns)					\
	DO_TO_FIELD (14,	Int16,						numRows)					\
	DO_TO_FIELD (16,	Int16,						currentRow)					\
	DO_TO_FIELD (18,	Int16,						currentColumn)				\
	DO_TO_FIELD (20,	Int16,						topRow)						\
	DO_TO_FIELD (22,	emuptr,						columnAttrs)				\
	DO_TO_FIELD (26,	emuptr,						rowAttrs)					\
	DO_TO_FIELD (30,	emuptr,						items)						\
	DO_TO_FIELD (34,	FieldType,					currentField)				\


// ========================================
// ===== TimGlobalsType =====
// ========================================

#define FOR_TimGlobalsType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT(TimGlobalsType, 24, FOR_TimGlobalsType_FIELDS)

#define FOR_TimGlobalsType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt32,						rtcHours)					\
	DO_TO_FIELD (4,		UInt32,						alarmSeconds)				\
	DO_TO_FIELD (8,		Boolean,					rtcBugWorkaround)			\
	DO_TO_FIELD (9,		Boolean,					rtcBugAsleep)				\
	DO_TO_FIELD (10,	UInt32,						rtcBugAlarm)				\
	DO_TO_FIELD (14,	UInt32,						rtcBugSeconds)				\
	DO_TO_FIELD (18,	UInt32,						rtcBugLastSeconds)			\
	DO_TO_FIELD (22,	Boolean,					rtcBugFastMinuteCalc)		\
	DO_TO_FIELD (23,	UInt8,						reserved)					\


// ========================================
// ===== UIColorStateType =====
// ========================================

#define FOR_UIColorStateType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (UIColorStateType, 12, FOR_UIColorStateType_FIELDS)

#define FOR_UIColorStateType_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		emuptr,						colorTableP)				\
	DO_TO_FIELD (4,		emuptr,						colorTableStackP)			\
	DO_TO_FIELD (8,		Int16,						colorTableStackIndex)		\
	DO_TO_FIELD (10,	Boolean,					tablesDirty)				\
	DO_TO_FIELD (11,	UInt8,						reserved)					\


// ========================================
// ===== UIGlobalsType =====
// ========================================
// Universal type; contains only parts up to where
// different versions diverge.

#define FOR_UIGlobalsType_STRUCT(DO_TO_STRUCT)									\
	DO_TO_STRUCT (UIGlobalsType, 56, FOR_UIGlobalsType_FIELDS)

#define FOR_UIGlobalsType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						activeWindow)				\
	DO_TO_FIELD (4,		emuptr,						displayWindow)				\
	DO_TO_FIELD (8,		emuptr,						drawWindow)					\
	DO_TO_FIELD (12,	emuptr,						firstWindow)				\
	DO_TO_FIELD (16,	emuptr,						exitWindowID)				\
	DO_TO_FIELD (20,	emuptr,						enterWindowID)				\
	DO_TO_FIELD (24,	emuptr,						exitedWindowID)				\
	DO_TO_FIELD (28,	GraphicStateTypeV1,			gStateV1)					\
	DO_TO_FIELD (28,	GraphicStateTypeV2,			gStateV2)					\
	DO_TO_FIELD (28,	GraphicStateTypeV3,			gStateV3)					\


// ========================================
// ===== UIGlobalsTypeCommon =====
// ========================================
// Mostly Universal type.  Can be used on anything except 1.0.  Skips over
// a gap if shifting fields between the gState and uiCurrentFontID fields,
// does include uiCurrentMenuRscID and uiDecimalSeparator, but doesn't
// include allFormsClosing, uiOptions and menuCmdBarCurrent fields.

#define FOR_UIGlobalsTypeCommon_STRUCT(DO_TO_STRUCT)							\
	DO_TO_STRUCT (UIGlobalsTypeCommon, 163, FOR_UIGlobalsTypeCommon_FIELDS)

#define FOR_UIGlobalsTypeCommon_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		emuptr,						activeWindow)				\
	DO_TO_FIELD (4,		emuptr,						displayWindow)				\
	DO_TO_FIELD (8,		emuptr,						drawWindow)					\
	DO_TO_FIELD (12,	emuptr,						firstWindow)				\
	DO_TO_FIELD (16,	emuptr,						exitWindowID)				\
	DO_TO_FIELD (20,	emuptr,						enterWindowID)				\
	DO_TO_FIELD (24,	emuptr,						exitedWindowID)				\
	DO_TO_FIELD (28,	GraphicStateTypeV1,			gStateV1)					\
	DO_TO_FIELD (28,	GraphicStateTypeV2,			gStateV2)					\
	DO_TO_FIELD (28,	GraphicStateTypeV3,			gStateV3)					\
		/* Gap of shifting fields */											\
	DO_TO_FIELD (98,	UInt8,						uiCurrentFontID)			\
	DO_TO_FIELD (99,	Int16,						_unused2)					\
	DO_TO_FIELD (100,	emuptr,						currentForm)				\
	DO_TO_FIELD (104,	Boolean,					insPtIsEnabled)				\
	DO_TO_FIELD (105,	Boolean,					insPtOn)					\
	DO_TO_FIELD (106,	PointType,					insPtLoc)					\
	DO_TO_FIELD (110,	Int16,						insPtHeight)				\
	DO_TO_FIELD (112,	Int32,						insPtLastTick)				\
	DO_TO_FIELD (116,	emuptr,						insPtBitsBehind)			\
		/* array of 3 ClipboardItems */											\
	DO_TO_FIELD (120,	ClipboardItem,				clipboard)					\
	DO_TO_FIELD (138,	emuptr,						uiCurrentMenu)				\
	DO_TO_FIELD (142,	UInt16,						uiCurrentMenuRscID)			\
	DO_TO_FIELD (144,	FieldUndoType,				undoGlobals)				\
	DO_TO_FIELD (156,	UInt8,						gsiState)					\
	DO_TO_FIELD (157,	Boolean,					gsiIsEnabled)				\
	DO_TO_FIELD (158,	PointType,					gsiLocation)				\
	DO_TO_FIELD (162,	Char,						uiDecimalSeparator)			\


// ========================================
// ===== UIGlobalsTypeV1 =====
// ========================================
// Used in Palm OS 1.0.

#define FOR_UIGlobalsTypeV1_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (UIGlobalsTypeV1, 160, FOR_UIGlobalsTypeV1_FIELDS)

#define FOR_UIGlobalsTypeV1_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						activeWindow)				\
	DO_TO_FIELD (4,		emuptr,						displayWindow)				\
	DO_TO_FIELD (8,		emuptr,						drawWindow)					\
	DO_TO_FIELD (12,	emuptr,						firstWindow)				\
	DO_TO_FIELD (16,	emuptr,						exitWindowID)				\
	DO_TO_FIELD (20,	emuptr,						enterWindowID)				\
	DO_TO_FIELD (24,	emuptr,						exitedWindowID)				\
	DO_TO_FIELD (28,	GraphicStateTypeV1,			gState)						\
	DO_TO_FIELD (48,	emuptr,						eventQ)						\
	DO_TO_FIELD (52,	UInt16,						eventQIndex)				\
	DO_TO_FIELD (54,	UInt16,						eventQLength)				\
	DO_TO_FIELD (56,	Int16,						lastScreenX)				\
	DO_TO_FIELD (58,	Int16,						lastScreenY)				\
	DO_TO_FIELD (60,	Boolean,					lastPenDown)				\
	DO_TO_FIELD (61,	UInt8,						_unused1)					\
	DO_TO_FIELD (62,	Int32,						needNullTickCount)			\
	DO_TO_FIELD (66,	emuptr,						uiCurrentFontPtr)			\
		/* array of 7 emuptrs */												\
	DO_TO_FIELD (70,	emuptr,						uiFontTable)				\
	DO_TO_FIELD (98,	UInt8,						uiCurrentFontID)			\
	DO_TO_FIELD (99,	Int16,						_unused2)					\
	DO_TO_FIELD (100,	emuptr,						currentForm)				\
	DO_TO_FIELD (104,	Boolean,					insPtIsEnabled)				\
	DO_TO_FIELD (105,	Boolean,					insPtOn)					\
	DO_TO_FIELD (106,	PointType,					insPtLoc)					\
	DO_TO_FIELD (110,	Int16,						insPtHeight)				\
	DO_TO_FIELD (112,	Int32,						insPtLastTick)				\
	DO_TO_FIELD (116,	emuptr,						insPtBitsBehind)			\
		/* array of 3 ClipboardItems */											\
	DO_TO_FIELD (120,	ClipboardItem,				clipboard)					\
	DO_TO_FIELD (138,	emuptr,						uiCurrentMenu)				\
	DO_TO_FIELD (142,	FieldUndoType,				undoGlobals)				\
	DO_TO_FIELD (154,	UInt8,						gsiState)					\
	DO_TO_FIELD (155,	Boolean,					gsiIsEnabled)				\
	DO_TO_FIELD (156,	PointType,					gsiLocation)				\


// ========================================
// ===== UIGlobalsTypeV2 =====
// ========================================
// Used in Palm OS 2.0.
// Inserts uiCurrentMenuRscID and adds uiDecimalSeparator.

#define FOR_UIGlobalsTypeV2_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (UIGlobalsTypeV2, 163, FOR_UIGlobalsTypeV2_FIELDS)

#define FOR_UIGlobalsTypeV2_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						activeWindow)				\
	DO_TO_FIELD (4,		emuptr,						displayWindow)				\
	DO_TO_FIELD (8,		emuptr,						drawWindow)					\
	DO_TO_FIELD (12,	emuptr,						firstWindow)				\
	DO_TO_FIELD (16,	emuptr,						exitWindowID)				\
	DO_TO_FIELD (20,	emuptr,						enterWindowID)				\
	DO_TO_FIELD (24,	emuptr,						exitedWindowID)				\
	DO_TO_FIELD (28,	GraphicStateTypeV1,			gState)						\
	DO_TO_FIELD (48,	emuptr,						eventQ)						\
	DO_TO_FIELD (52,	UInt16,						eventQIndex)				\
	DO_TO_FIELD (54,	UInt16,						eventQLength)				\
	DO_TO_FIELD (56,	Int16,						lastScreenX)				\
	DO_TO_FIELD (58,	Int16,						lastScreenY)				\
	DO_TO_FIELD (60,	Boolean,					lastPenDown)				\
	DO_TO_FIELD (61,	UInt8,						_unused1)					\
	DO_TO_FIELD (62,	Int32,						needNullTickCount)			\
	DO_TO_FIELD (66,	emuptr,						uiCurrentFontPtr)			\
		/* array of 7 emuptrs */												\
	DO_TO_FIELD (70,	emuptr,						uiFontTable)				\
	DO_TO_FIELD (98,	UInt8,						uiCurrentFontID)			\
	DO_TO_FIELD (99,	Int16,						_unused2)					\
	DO_TO_FIELD (100,	emuptr,						currentForm)				\
	DO_TO_FIELD (104,	Boolean,					insPtIsEnabled)				\
	DO_TO_FIELD (105,	Boolean,					insPtOn)					\
	DO_TO_FIELD (106,	PointType,					insPtLoc)					\
	DO_TO_FIELD (110,	Int16,						insPtHeight)				\
	DO_TO_FIELD (112,	Int32,						insPtLastTick)				\
	DO_TO_FIELD (116,	emuptr,						insPtBitsBehind)			\
		/* array of 3 ClipboardItems */											\
	DO_TO_FIELD (120,	ClipboardItem,				clipboard)					\
	DO_TO_FIELD (138,	emuptr,						uiCurrentMenu)				\
	DO_TO_FIELD (142,	UInt16,						uiCurrentMenuRscID)			\
	DO_TO_FIELD (144,	FieldUndoType,				undoGlobals)				\
	DO_TO_FIELD (156,	UInt8,						gsiState)					\
	DO_TO_FIELD (157,	Boolean,					gsiIsEnabled)				\
	DO_TO_FIELD (158,	PointType,					gsiLocation)				\
	DO_TO_FIELD (162,	Char,						uiDecimalSeparator)			\


// ========================================
// ===== UIGlobalsTypeV3 =====
// ========================================
// Used in Palm OS 3.0.
// Uses V2 GraphicStateType, replaces uiFontTable with uiSysFontTablePtr...uiUnused2.

#define FOR_UIGlobalsTypeV3_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (UIGlobalsTypeV3, 163, FOR_UIGlobalsTypeV3_FIELDS)

#define FOR_UIGlobalsTypeV3_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		emuptr,						activeWindow)				\
	DO_TO_FIELD (4,		emuptr,						displayWindow)				\
	DO_TO_FIELD (8,		emuptr,						drawWindow)					\
	DO_TO_FIELD (12,	emuptr,						firstWindow)				\
	DO_TO_FIELD (16,	emuptr,						exitWindowID)				\
	DO_TO_FIELD (20,	emuptr,						enterWindowID)				\
	DO_TO_FIELD (24,	emuptr,						exitedWindowID)				\
	DO_TO_FIELD (28,	GraphicStateTypeV2,			gState)						\
	DO_TO_FIELD (56,	emuptr,						eventQ)						\
	DO_TO_FIELD (60,	UInt16,						eventQIndex)				\
	DO_TO_FIELD (62,	UInt16,						eventQLength)				\
	DO_TO_FIELD (64,	Int16,						lastScreenX)				\
	DO_TO_FIELD (66,	Int16,						lastScreenY)				\
	DO_TO_FIELD (68,	Boolean,					lastPenDown)				\
	DO_TO_FIELD (69,	UInt8,						_unused1)					\
	DO_TO_FIELD (70,	Int32,						needNullTickCount)			\
	DO_TO_FIELD (74,	emuptr,						uiCurrentFontPtr)			\
	DO_TO_FIELD (78,	emuptr,						uiSysFontTablePtr)			\
	DO_TO_FIELD (82,	emuptr,						uiAppFontTablePtr)			\
	DO_TO_FIELD (86,	UInt16,						uiNumSysFonts)				\
	DO_TO_FIELD (88,	UInt16,						uiNumAppFonts)				\
	DO_TO_FIELD (90,	UInt32,						uiUnused1)					\
	DO_TO_FIELD (94,	UInt32,						uiUnused2)					\
	DO_TO_FIELD (98,	UInt8,						uiCurrentFontID)			\
	DO_TO_FIELD (99,	Int16,						_unused2)					\
	DO_TO_FIELD (100,	emuptr,						currentForm)				\
	DO_TO_FIELD (104,	Boolean,					insPtIsEnabled)				\
	DO_TO_FIELD (105,	Boolean,					insPtOn)					\
	DO_TO_FIELD (106,	PointType,					insPtLoc)					\
	DO_TO_FIELD (110,	Int16,						insPtHeight)				\
	DO_TO_FIELD (112,	Int32,						insPtLastTick)				\
	DO_TO_FIELD (116,	emuptr,						insPtBitsBehind)			\
		/* array of 3 ClipboardItems */											\
	DO_TO_FIELD (120,	ClipboardItem,				clipboard)					\
	DO_TO_FIELD (138,	emuptr,						uiCurrentMenu)				\
	DO_TO_FIELD (142,	UInt16,						uiCurrentMenuRscID)			\
	DO_TO_FIELD (144,	FieldUndoType,				undoGlobals)				\
	DO_TO_FIELD (156,	UInt8,						gsiState)					\
	DO_TO_FIELD (157,	Boolean,					gsiIsEnabled)				\
	DO_TO_FIELD (158,	PointType,					gsiLocation)				\
	DO_TO_FIELD (162,	Char,						uiDecimalSeparator)			\


// ========================================
// ===== UIGlobalsTypeV31 =====
// ========================================
// Used in Palm OS 3.1.
// Replaces uiUnused1 with uiFontListPtr.

#define FOR_UIGlobalsTypeV31_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (UIGlobalsTypeV31, 163, FOR_UIGlobalsTypeV31_FIELDS)

#define FOR_UIGlobalsTypeV31_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		emuptr,						activeWindow)				\
	DO_TO_FIELD (4,		emuptr,						displayWindow)				\
	DO_TO_FIELD (8,		emuptr,						drawWindow)					\
	DO_TO_FIELD (12,	emuptr,						firstWindow)				\
	DO_TO_FIELD (16,	emuptr,						exitWindowID)				\
	DO_TO_FIELD (20,	emuptr,						enterWindowID)				\
	DO_TO_FIELD (24,	emuptr,						exitedWindowID)				\
	DO_TO_FIELD (28,	GraphicStateTypeV2,			gState)						\
	DO_TO_FIELD (56,	emuptr,						eventQ)						\
	DO_TO_FIELD (60,	UInt16,						eventQIndex)				\
	DO_TO_FIELD (62,	UInt16,						eventQLength)				\
	DO_TO_FIELD (64,	Int16,						lastScreenX)				\
	DO_TO_FIELD (66,	Int16,						lastScreenY)				\
	DO_TO_FIELD (68,	Boolean,					lastPenDown)				\
	DO_TO_FIELD (69,	UInt8,						_unused1)					\
	DO_TO_FIELD (70,	Int32,						needNullTickCount)			\
	DO_TO_FIELD (74,	emuptr,						uiCurrentFontPtr)			\
	DO_TO_FIELD (78,	emuptr,						uiSysFontTablePtr)			\
	DO_TO_FIELD (82,	emuptr,						uiAppFontTablePtr)			\
	DO_TO_FIELD (86,	UInt16,						uiNumSysFonts)				\
	DO_TO_FIELD (88,	UInt16,						uiNumAppFonts)				\
	DO_TO_FIELD (90,	UInt32,						uiFontListPtr)				\
	DO_TO_FIELD (94,	UInt32,						uiUnused2)					\
	DO_TO_FIELD (98,	UInt8,						uiCurrentFontID)			\
	DO_TO_FIELD (99,	Int16,						_unused2)					\
	DO_TO_FIELD (100,	emuptr,						currentForm)				\
	DO_TO_FIELD (104,	Boolean,					insPtIsEnabled)				\
	DO_TO_FIELD (105,	Boolean,					insPtOn)					\
	DO_TO_FIELD (106,	PointType,					insPtLoc)					\
	DO_TO_FIELD (110,	Int16,						insPtHeight)				\
	DO_TO_FIELD (112,	Int32,						insPtLastTick)				\
	DO_TO_FIELD (116,	emuptr,						insPtBitsBehind)			\
		/* array of 3 ClipboardItems */											\
	DO_TO_FIELD (120,	ClipboardItem,				clipboard)					\
	DO_TO_FIELD (138,	emuptr,						uiCurrentMenu)				\
	DO_TO_FIELD (142,	UInt16,						uiCurrentMenuRscID)			\
	DO_TO_FIELD (144,	FieldUndoType,				undoGlobals)				\
	DO_TO_FIELD (156,	UInt8,						gsiState)					\
	DO_TO_FIELD (157,	Boolean,					gsiIsEnabled)				\
	DO_TO_FIELD (158,	PointType,					gsiLocation)				\
	DO_TO_FIELD (162,	Char,						uiDecimalSeparator)			\


// ========================================
// ===== UIGlobalsTypeV32 =====
// ========================================
// Used in Palm OS 3.2.
// Replaces uiUnused2 with rootWindow.

#define FOR_UIGlobalsTypeV32_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (UIGlobalsTypeV32, 163, FOR_UIGlobalsTypeV32_FIELDS)

#define FOR_UIGlobalsTypeV32_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		emuptr,						activeWindow)				\
	DO_TO_FIELD (4,		emuptr,						displayWindow)				\
	DO_TO_FIELD (8,		emuptr,						drawWindow)					\
	DO_TO_FIELD (12,	emuptr,						firstWindow)				\
	DO_TO_FIELD (16,	emuptr,						exitWindowID)				\
	DO_TO_FIELD (20,	emuptr,						enterWindowID)				\
	DO_TO_FIELD (24,	emuptr,						exitedWindowID)				\
	DO_TO_FIELD (28,	GraphicStateTypeV2,			gState)						\
	DO_TO_FIELD (56,	emuptr,						eventQ)						\
	DO_TO_FIELD (60,	UInt16,						eventQIndex)				\
	DO_TO_FIELD (62,	UInt16,						eventQLength)				\
	DO_TO_FIELD (64,	Int16,						lastScreenX)				\
	DO_TO_FIELD (66,	Int16,						lastScreenY)				\
	DO_TO_FIELD (68,	Boolean,					lastPenDown)				\
	DO_TO_FIELD (69,	UInt8,						_unused1)					\
	DO_TO_FIELD (70,	Int32,						needNullTickCount)			\
	DO_TO_FIELD (74,	emuptr,						uiCurrentFontPtr)			\
	DO_TO_FIELD (78,	emuptr,						uiSysFontTablePtr)			\
	DO_TO_FIELD (82,	emuptr,						uiAppFontTablePtr)			\
	DO_TO_FIELD (86,	UInt16,						uiNumSysFonts)				\
	DO_TO_FIELD (88,	UInt16,						uiNumAppFonts)				\
	DO_TO_FIELD (90,	UInt32,						uiFontListPtr)				\
	DO_TO_FIELD (94,	UInt32,						rootWindow)					\
	DO_TO_FIELD (98,	UInt8,						uiCurrentFontID)			\
	DO_TO_FIELD (99,	Int16,						_unused2)					\
	DO_TO_FIELD (100,	emuptr,						currentForm)				\
	DO_TO_FIELD (104,	Boolean,					insPtIsEnabled)				\
	DO_TO_FIELD (105,	Boolean,					insPtOn)					\
	DO_TO_FIELD (106,	PointType,					insPtLoc)					\
	DO_TO_FIELD (110,	Int16,						insPtHeight)				\
	DO_TO_FIELD (112,	Int32,						insPtLastTick)				\
	DO_TO_FIELD (116,	emuptr,						insPtBitsBehind)			\
		/* array of 3 ClipboardItems */											\
	DO_TO_FIELD (120,	ClipboardItem,				clipboard)					\
	DO_TO_FIELD (138,	emuptr,						uiCurrentMenu)				\
	DO_TO_FIELD (142,	UInt16,						uiCurrentMenuRscID)			\
	DO_TO_FIELD (144,	FieldUndoType,				undoGlobals)				\
	DO_TO_FIELD (156,	UInt8,						gsiState)					\
	DO_TO_FIELD (157,	Boolean,					gsiIsEnabled)				\
	DO_TO_FIELD (158,	PointType,					gsiLocation)				\
	DO_TO_FIELD (162,	Char,						uiDecimalSeparator)			\


// ========================================
// ===== UIGlobalsTypeV35 =====
// ========================================
// Used in Palm OS 3.5.
// Replaces GraphicStateTypeV2 with GraphicStateTypeV3, uicState, and
// lastTapTime, replaces _unused1 with tapCount, replaces _unused2 with
// allFormsClosing, added uiOptions and menuCmdBarCurrent.

#define FOR_UIGlobalsTypeV35_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (UIGlobalsTypeV35, 168, FOR_UIGlobalsTypeV35_FIELDS)

#define FOR_UIGlobalsTypeV35_FIELDS(DO_TO_FIELD)								\
	DO_TO_FIELD (0,		emuptr,						activeWindow)				\
	DO_TO_FIELD (4,		emuptr,						displayWindow)				\
	DO_TO_FIELD (8,		emuptr,						drawWindow)					\
	DO_TO_FIELD (12,	emuptr,						firstWindow)				\
	DO_TO_FIELD (16,	emuptr,						exitWindowID)				\
	DO_TO_FIELD (20,	emuptr,						enterWindowID)				\
	DO_TO_FIELD (24,	emuptr,						exitedWindowID)				\
	DO_TO_FIELD (28,	GraphicStateTypeV3,			gState)						\
	DO_TO_FIELD (40,	UIColorStateType,			uicState)					\
	DO_TO_FIELD (52,	UInt32,						lastTapTime)				\
	DO_TO_FIELD (56,	emuptr,						eventQ)						\
	DO_TO_FIELD (60,	UInt16,						eventQIndex)				\
	DO_TO_FIELD (62,	UInt16,						eventQLength)				\
	DO_TO_FIELD (64,	Int16,						lastScreenX)				\
	DO_TO_FIELD (66,	Int16,						lastScreenY)				\
	DO_TO_FIELD (68,	Boolean,					lastPenDown)				\
	DO_TO_FIELD (69,	UInt8,						tapCount)					\
	DO_TO_FIELD (70,	Int32,						needNullTickCount)			\
	DO_TO_FIELD (74,	emuptr,						uiCurrentFontPtr)			\
	DO_TO_FIELD (78,	emuptr,						uiSysFontTablePtr)			\
	DO_TO_FIELD (82,	emuptr,						uiAppFontTablePtr)			\
	DO_TO_FIELD (86,	UInt16,						uiNumSysFonts)				\
	DO_TO_FIELD (88,	UInt16,						uiNumAppFonts)				\
	DO_TO_FIELD (90,	UInt32,						uiFontListPtr)				\
	DO_TO_FIELD (94,	UInt32,						rootWindow)					\
	DO_TO_FIELD (98,	UInt8,						uiCurrentFontID)			\
	DO_TO_FIELD (99,	Int16,						allFormsClosing)			\
	DO_TO_FIELD (100,	emuptr,						currentForm)				\
	DO_TO_FIELD (104,	Boolean,					insPtIsEnabled)				\
	DO_TO_FIELD (105,	Boolean,					insPtOn)					\
	DO_TO_FIELD (106,	PointType,					insPtLoc)					\
	DO_TO_FIELD (110,	Int16,						insPtHeight)				\
	DO_TO_FIELD (112,	Int32,						insPtLastTick)				\
	DO_TO_FIELD (116,	emuptr,						insPtBitsBehind)			\
		/* array of 3 ClipboardItems */											\
	DO_TO_FIELD (120,	ClipboardItem,				clipboard)					\
	DO_TO_FIELD (138,	emuptr,						uiCurrentMenu)				\
	DO_TO_FIELD (142,	UInt16,						uiCurrentMenuRscID)			\
	DO_TO_FIELD (144,	FieldUndoType,				undoGlobals)				\
	DO_TO_FIELD (156,	UInt8,						gsiState)					\
	DO_TO_FIELD (157,	Boolean,					gsiIsEnabled)				\
	DO_TO_FIELD (158,	PointType,					gsiLocation)				\
	DO_TO_FIELD (162,	Char,						uiDecimalSeparator)			\
	DO_TO_FIELD (163,	UInt8,						uiOptions)					\
	DO_TO_FIELD (164,	emuptr,						menuCmdBarCurrent)			\


// ========================================
// ===== UsbHwrType =====
// ========================================

#define FOR_UsbHwrType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (UsbHwrType, 4, FOR_UsbHwrType_FIELDS)

#define FOR_UsbHwrType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		UInt8,						data)						\
	DO_TO_FIELD (1,		UInt8,						___filler0)					\
	DO_TO_FIELD (2,		UInt8,						command)					\
	DO_TO_FIELD (3,		UInt8,						___filler1)					\



// ========================================
// ===== WindowFlagsType =====
// ========================================

#define FOR_WindowFlagsType_STRUCT(DO_TO_STRUCT)								\
	DO_TO_STRUCT (WindowFlagsType, 2, FOR_WindowFlagsType_FIELDS)

#define FOR_WindowFlagsType_FIELDS(DO_TO_FIELD)									\
	DO_TO_FIELD (0,		UInt16,						flags)						\


// ========================================
// ===== WindowType =====
// ========================================

#define FOR_WindowType_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (WindowType, 40, FOR_WindowType_FIELDS)

#define FOR_WindowType_FIELDS(DO_TO_FIELD)										\
	DO_TO_FIELD (0,		Coord,						displayWidthV20)			\
	DO_TO_FIELD (2,		Coord,						displayHeightV20)			\
	DO_TO_FIELD (4,		emuptr,						displayAddrV20)				\
	DO_TO_FIELD (8,		WindowFlagsType,			windowFlags)				\
	DO_TO_FIELD (10,	RectangleType,				windowBounds)				\
	DO_TO_FIELD (18,	AbsRectType,				clippingBounds)				\
	DO_TO_FIELD (26,	emuptr,						bitmapP)					\
	DO_TO_FIELD (30,	FrameBitsType,				frameType)					\
	DO_TO_FIELD (32,	emuptr,						drawStateP)					\
	DO_TO_FIELD (36,	emuptr,						nextWindow)					\


// ========================================
// ===== cjxln =====
// ========================================

#define FOR_cjxln_STRUCT(DO_TO_STRUCT)											\
	DO_TO_STRUCT (cjxln, 8, FOR_cjxln_FIELDS)

#define FOR_cjxln_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		emuptr,							xlnnext)				\
	DO_TO_FIELD (4,		emuptr,							xlnprev)				\


// ========================================
// ===== cj_xgbh =====
// ========================================

#define FOR_cj_xgbh_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (cj_xgbh, 28, FOR_cj_xgbh_FIELDS)

#define FOR_cj_xgbh_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		cjxln,							xgbhln)					\
	DO_TO_FIELD (8,		cjxln,							xgbhwait)				\
	DO_TO_FIELD (16,	Int16,							xgbhpry)				\
	DO_TO_FIELD (18,	UInt8,							xgbhmod)				\
	DO_TO_FIELD (19,	Int8,							xgbhidkey)				\
	DO_TO_FIELD (20,	UInt32,							xgbhtag)				\
	DO_TO_FIELD (24,	UInt32,							xgbhdjp)				\


// ========================================
// ===== cj_xsmb =====
// ========================================

#define FOR_cj_xsmb_STRUCT(DO_TO_STRUCT)										\
	DO_TO_STRUCT (cj_xsmb, 48, FOR_cj_xsmb_FIELDS)

#define FOR_cj_xsmb_FIELDS(DO_TO_FIELD)											\
	DO_TO_FIELD (0,		cj_xgbh,						xsmh)					\
	DO_TO_FIELD (28,	emuptr,							xsmowner)				\
	DO_TO_FIELD (32,	Int32,							xsmival)				\
	DO_TO_FIELD (36,	Int32,							xsmval)					\
	DO_TO_FIELD (40,	Int32,							xsmuse)					\
	DO_TO_FIELD (44,	Int32,							xsmrsv)					\

