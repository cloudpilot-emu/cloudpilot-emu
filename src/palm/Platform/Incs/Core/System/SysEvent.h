/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: SysEvent.h
 *
 * Description:
 *        This file defines event structures and routines.
 *
 * History:
 *		September 26, 1994	Created by Art Lamb
 *       05/05/98 art   Add Text Services event.
 *       07/23/98 kwk   Changed UInt16 field in keyDown event to WChar.
 *       08/20/98 kwk   Split tsmEvent into tsmConfirmEvent & tsmFepButtonEvent.
 *       09/07/98 kwk   Added EvtPeekEvent routine declaration.
 *       10/13/98 kwk   Removed EvtPeekEvent until API can be finalized.
 *       03/11/99 grant Fixed types of pointers in SysEventType data fields.
 *       05/31/99 kwk   Added tsmFepModeEvent event.
 *       07/14/99 jesse Moved UI structures & constants to Event.h
 *                      defined ranges for future UI & system events.
 *			07/30/99	kwk	Moved TSM events here from Event.h
 *			09/12/99	gap	Add new multi-tap implementation
 *			09/14/99	gap	Removed EvtGetTrapState.
 *
 *****************************************************************************/

#ifndef __SYSEVENT_H__
#define __SYSEVENT_H__

#include <PalmTypes.h>
#include <CoreTraps.h>
#include <Font.h>
#include <Rect.h>
#include <Window.h>

#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER еее Don't use an enum for this, as convenient as it is, there's namespace
// pollution, size issues, no way to manage componentization, blah blah blah.
#endif // PUBLIC_STUFF_STRIPPED

typedef enum {
	sysEventNilEvent = 0,
	sysEventPenDownEvent,
	sysEventPenUpEvent,
	sysEventPenMoveEvent,
	sysEventKeyDownEvent,
	sysEventWinEnterEvent,
	sysEventWinExitEvent,
	sysEventAppStopEvent = 22,
	sysEventTsmConfirmEvent = 35,
	sysEventTsmFepButtonEvent,
	sysEventTsmFepModeEvent,

	// add future UI level events in this numeric space
	// to save room for new system level events
	sysEventNextUIEvent = 0x0800,

	// <chg 2-25-98 RM> Equates added for library events
	sysEventFirstINetLibEvent = 0x1000,
	sysEventFirstWebLibEvent = 0x1100,
	
	// <chg 10/9/98 SCL> Changed firstUserEvent from 32767 (0x7FFF) to 0x6000
	// Enums are signed ints, so 32767 technically only allowed for ONE event.
	sysEventFirstUserEvent = 0x6000
} SysEventsEnum;



// keyDownEvent modifers
#define shiftKeyMask       0x0001
#define capsLockMask       0x0002
#define numLockMask        0x0004
#define commandKeyMask     0x0008
#define optionKeyMask      0x0010
#define controlKeyMask     0x0020
#define autoRepeatKeyMask  0x0040      // True if generated due to auto-repeat
#define doubleTapKeyMask   0x0080      // True if this is a double-tap event
#define poweredOnKeyMask   0x0100      // True if this is a double-tap event
#define appEvtHookKeyMask  0x0200      // True if this is an app hook key
#define libEvtHookKeyMask  0x0400      // True if this is a library hook key

// define mask for all "virtual" keys
#define virtualKeyMask	(appEvtHookKeyMask | libEvtHookKeyMask | commandKeyMask)


// Event timeouts
#define  evtWaitForever    -1
#define  evtNoWait          0

struct _GenericEventType {
	UInt16         datum[8];
   };

struct _PenUpEventType {
   PointType      start;            // display coord. of stroke start
   PointType      end;              // display coord. of stroke start
   };

struct _KeyDownEventType {
   WChar          chr;              // ascii code
   UInt16         keyCode;          // virtual key code
   UInt16         modifiers;
   };

struct _WinEnterEventType {
   WinHandle      enterWindow;
   WinHandle      exitWindow;
   };

struct _WinExitEventType {
   WinHandle      enterWindow;
   WinHandle      exitWindow;
   };

struct _TSMConfirmType {
	Char *			yomiText;
	UInt16			formID;
   };

struct _TSMFepButtonType {
	UInt16			buttonID;
   };

struct _TSMFepModeEventType {
	UInt16         mode;					// DOLATER kwk - use real type for mode?
   };


// The event record.
typedef struct SysEventType {
   SysEventsEnum  eType;
   Boolean        penDown;
   UInt8          tapCount;
   Coord          screenX;
   Coord          screenY;
   union {
      struct _GenericEventType		generic;

      struct _PenUpEventType			penUp;
      struct _KeyDownEventType		keyDown;
      struct _WinEnterEventType		winEnter;
      struct _WinExitEventType		winExit;
      struct _TSMConfirmType			tsmConfirm;
      struct _TSMFepButtonType		tsmFepButton;
      struct _TSMFepModeEventType	tsmFepMode;

      } data;

} SysEventType;


// Events are stored in the event queue with some extra fields:
typedef  struct {
   SysEventType   event;
   UInt32         id;                  // used to support EvtAddUniqueEvent
   } SysEventStoreType;

#ifndef PUBLIC_STUFF_STRIPPED		// removed header that should be moved into private area

//---------------------------------------------------------------------
// Event Functions
//---------------------------------------------------------------------

#ifdef REMOVE_FOR_EMULATOR
#ifdef __cplusplus
extern "C" {
#endif

/* DOLATER: jwm: This lot should be in a private header file, no? */

void     SysEventInitialize (void)
                     SYS_TRAP(sysTrapEvtInitialize);

void     SysEventAddToQueue (const SysEventType *event)
                     SYS_TRAP(sysTrapEvtAddEventToQueue);

void     SysEventAddUniqueToQueue(const SysEventType *eventP, UInt32 id,
            Boolean inPlace)
                     SYS_TRAP(sysTrapEvtAddUniqueEventToQueue);

void     SysEventCopy (const SysEventType *source, SysEventType *dest)
                     SYS_TRAP(sysTrapEvtCopyEvent);

void     SysEventGet (SysEventType *event, Int32 timeout)
                     SYS_TRAP(sysTrapEvtGetEvent);

Boolean  SysEventAvail (void)
                     SYS_TRAP(sysTrapEvtEventAvail);


#ifdef __cplusplus
}
#endif
#endif


// For Compatibility.. DOLATER... source modules should use EvtGetPen instead.
#endif // PUBLIC_STUFF_STRIPPED
#define     PenGetPoint(a,b,c)    EvtGetPen(a,b,c)



#endif // __SYSEVENT_H__
