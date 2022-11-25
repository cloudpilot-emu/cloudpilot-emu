/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: Event.h
 *
 * Description:
 *        This file defines UI event structures and routines.
 *
 * History:
 *		September 26, 1994	Created by Art Lamb
 *		07/14/99 jesse	Separated from Event.h
 *		09/12/99	gap	Add for new multi-tap implementation
 *		09/14/99	gap	Removed EvtGetTrapState.
 *		10/28/99	kwk	Added EvtKeydownIsVirtual macro.
 *
 *****************************************************************************/

#ifndef __EVENT_H__
#define __EVENT_H__

#include <PalmTypes.h>
#include <CoreTraps.h>
#include <Font.h>
#include <Rect.h>
#include <SysEvent.h>
#include <Window.h>


typedef enum {
	nilEvent = 0,				// system level
	penDownEvent,				// system level
	penUpEvent,					// system level
	penMoveEvent,				// system level
	keyDownEvent,				// system level
	winEnterEvent,				// system level
	winExitEvent,				// system level
	ctlEnterEvent,
	ctlExitEvent,
	ctlSelectEvent,
	ctlRepeatEvent,
	lstEnterEvent,
	lstSelectEvent,
	lstExitEvent,
	popSelectEvent,
	fldEnterEvent,
	fldHeightChangedEvent,
	fldChangedEvent,
	tblEnterEvent,
	tblSelectEvent,
	daySelectEvent,
	menuEvent,
	appStopEvent = 22,			// system level
	frmLoadEvent,
	frmOpenEvent,
	frmGotoEvent,
	frmUpdateEvent,
	frmSaveEvent,
	frmCloseEvent,
	frmTitleEnterEvent,
	frmTitleSelectEvent,
	tblExitEvent,
	sclEnterEvent,
	sclExitEvent,
	sclRepeatEvent,
	tsmConfirmEvent = 35,		// system level
	tsmFepButtonEvent,			// system level
	tsmFepModeEvent,				// system level
//DOLATER - peter: remove this:	frmTitleChangedEvent,		// system level
	attnIndicatorEnterEvent,	// for attention manager's indicator
	attnIndicatorSelectEvent,	// for attention manager's indicator
	
	// add future UI level events in this numeric space
	// to save room for new system level events
	menuCmdBarOpenEvent = 0x0800,
	menuOpenEvent,
	menuCloseEvent,
	frmGadgetEnterEvent,
	frmGadgetMiscEvent,

	// <chg 2-25-98 RM> Equates added for library events
	firstINetLibEvent = 0x1000,
	firstWebLibEvent = 0x1100,
	
	// <chg 10/9/98 SCL> Changed firstUserEvent from 32767 (0x7FFF) to 0x6000
	// Enums are signed ints, so 32767 technically only allowed for ONE event.
	firstUserEvent = 0x6000
} eventsEnum;


// The event record.
typedef struct EventType {
//   eventsEnum		eType;
   Int16		eType;
   Boolean        penDown;
   UInt8          tapCount;
   Int16          screenX;
   Int16          screenY;
   union {
      struct _GenericEventType		generic;
      struct _PenUpEventType        penUp;
      struct _KeyDownEventType      keyDown;
      struct _WinEnterEventType     winEnter;
      struct _WinExitEventType      winExit;
      struct _TSMConfirmType			tsmConfirm;
      struct _TSMFepButtonType		tsmFepButton;
      struct _TSMFepModeEventType	tsmFepMode;

      struct ctlEnter {
         UInt16            controlID;
         struct ControlType *pControl;
         } ctlEnter;

      struct ctlSelect {
         UInt16            controlID;
         struct ControlType *pControl;
         Boolean           on;
         UInt8             reserved1;
         UInt16				value;		// used for slider controls only
         } ctlSelect;

      struct ctlRepeat {
         UInt16            controlID;
         struct ControlType *pControl;
         UInt32            time;
         UInt16				value;		// used for slider controls only
         } ctlRepeat;

      struct ctlExit {
         UInt16            controlID;
         struct ControlType *pControl;
         } ctlExit;

      struct fldEnter {
         UInt16            fieldID;
         struct FieldType 	*pField;
         } fldEnter;

      struct fldHeightChanged {
         UInt16            fieldID;
         struct FieldType 	*pField;
         Int16             newHeight;
         UInt16            currentPos;
         } fldHeightChanged;

      struct fldChanged {
         UInt16            fieldID;
         struct FieldType 	*pField;
         } fldChanged;

      struct fldExit {
         UInt16            fieldID;
         struct FieldType	*pField;
         } fldExit;

      struct lstEnter {
         UInt16            listID;
         struct ListType 	*pList;
         Int16             selection;
         } lstEnter;

      struct lstExit {
         UInt16            listID;
         struct ListType 	*pList;
         } lstExit;

      struct lstSelect {
         UInt16            listID;
         struct ListType 	*pList;
         Int16             selection;
         } lstSelect;

      struct tblEnter {
         UInt16            tableID;
         struct TableType	*pTable;
         Int16             row;
         Int16             column;
         } tblEnter;

      struct tblExit {
         UInt16            tableID;
         struct TableType 	*pTable;
         Int16             row;
         Int16             column;
         } tblExit;

      struct tblSelect {
         UInt16            tableID;
         struct TableType 	*pTable;
         Int16             row;
         Int16             column;
         } tblSelect;

      struct frmLoad {
         UInt16            formID;
         } frmLoad;

      struct frmOpen {
         UInt16            formID;
         } frmOpen;

      struct frmGoto {
         UInt16            formID;
         UInt16            recordNum;        // index of record that contain a match
         UInt16            matchPos;         // postion in record of the match.
         UInt16            matchLen;         // length of match.
         UInt16            matchFieldNum;    // field number string was found int
         UInt32            matchCustom;      // application specific info
         } frmGoto;

      struct frmClose {
         UInt16            formID;
         } frmClose;

      struct frmUpdate {
         UInt16            formID;
         UInt16            updateCode;       // Application specific
         } frmUpdate;

      struct frmTitleEnter {
         UInt16            formID;
         } frmTitleEnter;

      struct frmTitleSelect {
         UInt16            formID;
         } frmTitleSelect;

       struct attnIndicatorEnter {
         UInt16            formID;
      	} attnIndicatorEnter;

     struct attnIndicatorSelect {
         UInt16            formID;
      	} attnIndicatorSelect;

      struct daySelect {
         struct DaySelectorType *pSelector;
         Int16             selection;
         Boolean           useThisDate;
         UInt8             reserved1;
         } daySelect;

      struct menu {
         UInt16            itemID;
         } menu;

      struct popSelect {
         UInt16            controlID;
         struct ControlType *controlP;
         UInt16            listID;
         struct ListType 	*listP;
         Int16             selection;
         Int16             priorSelection;
         } popSelect;

      struct sclEnter {
         UInt16            scrollBarID;
         struct ScrollBarType *pScrollBar;
         } sclEnter;

      struct sclExit {
         UInt16            scrollBarID;
         struct ScrollBarType *pScrollBar;
         Int16             value;
         Int16             newValue;
         } sclExit;

      struct sclRepeat {
         UInt16            scrollBarID;
         struct ScrollBarType *pScrollBar;
         Int16             value;
         Int16             newValue;
         Int32             time;
         } sclRepeat;

		struct menuCmdBarOpen {
			Boolean preventFieldButtons;  // set to stop the field from automatically adding cut/copy/paste
			UInt8   reserved;					// alignment padding
			} menuCmdBarOpen;
		
		struct menuOpen {
			UInt16				menuRscID;
			//struct MenuBarType *pMenu;
			Int16					cause;
			} menuOpen;
		
		struct gadgetEnter {
         UInt16            gadgetID;			// must be same as gadgetMisc
         struct FormGadgetType *	gadgetP;		// must be same as gadgetMisc
			} gadgetEnter;
			
		struct gadgetMisc {
         UInt16            gadgetID;			// must be same as gadgetEnter
         struct FormGadgetType *	gadgetP;		// must be same as gadgetEnter
         UInt16				selector;
         void *				dataP;
			} gadgetMisc;
			
      } data;

} EventType;

typedef EventType *EventPtr;

// Evaluate to true if <eventP> is a pointer to a virtual character key-
// down event. We assume that the caller has already determined the event
// is a keydown. WARNING!!! This macro is only safe to use on Palm OS 3.5
// or later. With earlier versions of the OS, use TxtGlueCharIsVirtual()
// in PalmOSGlue.lib
#define	EvtKeydownIsVirtual(eventP)	(((eventP)->data.keyDown.modifiers & virtualKeyMask) != 0)

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------
// Event Functions
//---------------------------------------------------------------------

void     EvtAddEventToQueue (const EventType *event)
                     SYS_TRAP(sysTrapEvtAddEventToQueue);

void     EvtAddUniqueEventToQueue(const EventType *eventP, UInt32 id,
            Boolean inPlace)
                     SYS_TRAP(sysTrapEvtAddUniqueEventToQueue);

void     EvtCopyEvent (const EventType *source, EventType *dest)
                     SYS_TRAP(sysTrapEvtCopyEvent);

void     EvtGetEvent (EventType *event, Int32 timeout)
                     SYS_TRAP(sysTrapEvtGetEvent);

Boolean  EvtEventAvail (void)
                     SYS_TRAP(sysTrapEvtEventAvail);

void     EvtGetPen(Int16 *pScreenX, Int16 *pScreenY, Boolean *pPenDown)
                  SYS_TRAP(sysTrapEvtGetPen);

#ifdef __cplusplus
}
#endif
#endif


#endif // __EVENT_H__
