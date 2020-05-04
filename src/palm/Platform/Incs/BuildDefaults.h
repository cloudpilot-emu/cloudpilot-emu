/******************************************************************************
 *
 * Copyright (c) 1994-1999 Palm Computing, Inc. or its subsidiaries.
 * All rights reserved.
 *
 * File: BuildDefaults.h
 *
 * Description:
 *		Build variable defaults for Palm OS.
 *    
 *    This file is included by <PalmTypes.h>.
 *    This file supercedes the old <BuildRules.h> file.
 *
 * History:
 *		07/16/99	SCL	So many changes the old history is obsolete.
 *		07/29/99	kwk	Deleted TEXT_SERVICES & INTERNATIONAL_MGR defines.
 *		09/15/99	kwk	Changed COUNTRY & LANGUAGE defines into #errors.
 *		10/04/99	grant	Added LOCALE define (defaults to #error).
 *		10/05/99	kwk	Added LOCALE_SUFFIX define (defaults to "").
 *
 *****************************************************************************/

#ifndef 	__BUILDDEFAULTS_H__
#define	__BUILDDEFAULTS_H__

#include <BuildDefines.h>

// To override build options in a local component, include <BuildDefines.h>
// first, then define switches as need, and then include <PalmTypes.h>.

// Some projects used to have a local copy of a file called "AppBuildRules.h"
// or "AppBuildRulesMSC.h", which was automatically included by <BuildRules.h>
// to override certain system default compile-time switches.  These local
// "prefix" files can still be used.  The project source files should be changed
// to include <BuildDefines.h>, then "AppBuildRules.MSC.h", then <PalmTypes.h>
// instead of the previous <Pilot.h>


/************************************************************
 * Include the following when running under the CodeWarrior
 *  IDE so that default build options can be overriden. The default
 *  version of this file is in the Incs: directory. If an app wishes
 *  to override the default options, it should create a copy of this
 *  include file in its own local directory. This include file
 *  is never used when building from MPW since MPW can specify compiler
 *  defines from the command line.
 *
 * Other environments can override all of these settings by simply
 *  pre-defining CMD_LINE_BUILD and any other desired settings.
 *************************************************************/

#ifndef CMD_LINE_BUILD				// typically pre-defined only from MPW

	#if !defined(__MWERKS__) && !defined(__PALMOS_TRAPS__) && !defined(_MSC_VER)

		// assume other environments generally build only PalmOS executables
		#define __PALMOS_TRAPS__ 	1

	#endif
	

	#if	__PALMOS_TRAPS__			// defined by CodeWarrior IDE or above

		// Settings to build a PalmOS executable
		#ifndef EMULATION_LEVEL
			#define EMULATION_LEVEL		EMULATION_NONE		// building Palm OS executable
		#endif
		
		#ifndef USE_TRAPS
			#define USE_TRAPS				1						// use Palm OS traps
		#endif

	#endif


#endif



/************************************************************
 * Settings that can be overriden in the makefile (for MPW)
 *	OR (for CodeWarrior) in "AppBuildRules.h".  If there is no
 * local copy of "AppBuildRules.h" within the project directory,
 * the one in the Incs directory will be used instead.
 *************************************************************/ 
// This default option is probably ok for now
#ifndef	EMULATION_LEVEL
	#define	EMULATION_LEVEL		EMULATION_MAC
#endif

// This default option is probably ok for now
#if EMULATION_LEVEL == EMULATION_NONE
	#define	MEMORY_TYPE				MEMORY_LOCAL
#endif

// This default option is probably ok for now
#ifndef MEMORY_TYPE
	#define	MEMORY_TYPE 			MEMORY_LOCAL
#endif

// This default option is probably ok for now
#ifndef ENVIRONMENT
	#define	ENVIRONMENT				ENVIRONMENT_CW
#endif

// This default option is probably ok to leave as a default
#ifndef	PLATFORM_TYPE
	#define	PLATFORM_TYPE			PLATFORM_VIEWER
#endif

#ifndef	ERROR_CHECK_LEVEL
	#define	ERROR_CHECK_LEVEL		#error "ERROR_CHECK_LEVEL not defined; Try including ''PalmOptErrorCheckLevel.h''!"
	// The following allows <PalmOptErrorCheckLevel.h> to redefine ERROR_CHECK_LEVEL
	// without letting it change ERROR_CHECK_LEVEL if it was already defined,
	// such as would be the case when using a project prefix file...
	#define	ERROR_CHECK_LEVEL_OK_TO_REDEFINE
#endif

// This default option is probably ok for now
#ifndef PUBLIC_STUFF_STRIPPED
#ifndef CPU_TYPE
   #if defined(__INTEL__) || defined(__i386__)
      #define  CPU_TYPE          CPU_x86
   #elif defined(__POWERPC__) || defined(__powerpc__)
      #define  CPU_TYPE          CPU_PPC
   #elif defined(__arm__)
      #define  CPU_TYPE          CPU_ARM
   #else
	   #define	CPU_TYPE				CPU_68K
   #endif
#endif
#else  // PUBLIC_STUFF_STRIPPED
#ifndef CPU_TYPE
   #if defined(__INTEL__) || defined(__i386__)
      #define  CPU_TYPE          CPU_x86
   #else
	   #define	CPU_TYPE				CPU_68K
   #endif
#endif
#endif // PUBLIC_STUFF_STRIPPED

#ifndef PUBLIC_STUFF_STRIPPED
// This default option is probably ok for now
#ifndef CPU_ENDIAN
   #if (CPU_TYPE == CPU_x86) || (CPU_TYPE == CPU_ARM)
      #define  CPU_ENDIAN        CPU_ENDIAN_LITTLE
   #else
	   #define	CPU_ENDIAN			CPU_ENDIAN_BIG
   #endif
#endif
#endif // PUBLIC_STUFF_STRIPPED

#ifndef MODEL
	#define	MODEL						#error "MODEL not defined; Try including ''PalmOptModel.h''!"
#endif

// This default option is probably ok to leave as a default
#ifndef	MEMORY_FORCE_LOCK
	#define	MEMORY_FORCE_LOCK		MEMORY_FORCE_LOCK_ON
#endif

// Must be defined (-d or prefix file) before using.  See comment in <BuildDefines.h>.
#ifndef	DEBUG_LEVEL
	#define	DEBUG_LEVEL				#error "DEBUG_LEVEL must be defined before using!"
#endif

#ifndef 	DEFAULT_LIB_ENTRIES
	#define		DEFAULT_LIB_ENTRIES	12			// space for 12 libraries in library table
#endif

#ifndef	USER_MODE
	#define	USER_MODE				#error "USER_MODE not defined; Try including ''PalmOptUserMode.h''!"
#endif

#ifndef	INTERNAL_COMMANDS
	#define	INTERNAL_COMMANDS		#error "INTERNAL_COMMANDS must be defined before using!"
#endif

#ifndef PUBLIC_STUFF_STRIPPED
// DOLATER - this option should be driven strictly by inclusion or exclusion of a PRC file.
// jquinn - This was turned on in the old makefiles. It should be on by default.
// SCL - We should remove all references to this build option and just assume it's "on".
// Mandalay builds with it "on" in all cases, even in the OS; there's no longer an extension.
#endif // PUBLIC_STUFF_STRIPPED
#ifndef	INCLUDE_DES
	#define	INCLUDE_DES				INCLUDE_DES_ON
#endif

// Unless otherwise specified, assume CML_ENCODER is off
#ifndef	CML_ENCODER
	#define 	CML_ENCODER				CML_ENCODER_OFF
#endif

// Derive the path for resource files.
#ifndef RESOURCE_FILE_PREFIX
	#define	RESOURCE_FILE_PREFIX	""
#endif

// LOCALE_SUFFIX is used for the name of merged resource files,
// and also sys.tres.
#ifndef LOCALE_SUFFIX
	#define	LOCALE_SUFFIX			""
#endif

#ifndef PUBLIC_STUFF_STRIPPED
#ifndef	LOCALE
	#define	LOCALE					#error "LOCALE not defined; Try including ''PalmOptLocale.h''!"
	// The following allows <PalmOptLocale.h> to redefine LOCALE
	// without letting it change LOCALE if it was already defined,
	// such as would be the case when using a project prefix file...
	#define	LOCALE_OK_TO_REDEFINE
#endif
#endif // PUBLIC_STUFF_STRIPPED


// Set these according to which Shell commands you want to
// link with.  These are often overridden by other components
// in a local "AppBuildRules.h" which is currently obsolete.
#ifndef SHELL_COMMAND_DB
	#define	SHELL_COMMAND_DB			0	// Not Available
#endif

#ifndef SHELL_COMMAND_UI
	#define	SHELL_COMMAND_UI			1	// Available
#endif

#ifndef SHELL_COMMAND_APP
	#define	SHELL_COMMAND_APP			1	// Available
#endif

#ifndef SHELL_COMMAND_EMULATOR
	#define	SHELL_COMMAND_EMULATOR	1	// Available
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef 	DYN_MEM_SIZE_MAX
	#define	DYN_MEM_SIZE_MAX		#error "DYN_MEM_SIZE_MAX is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef 	SMALL_ROM_SIZE
	#define	SMALL_ROM_SIZE			#error "SMALL_ROM_SIZE is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef 	CONSOLE_SERIAL_LIB
	#define	CONSOLE_SERIAL_LIB	#error "CONSOLE_SERIAL_LIB is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef 	PILOT_SERIAL_MGR
	// Palm OS 3.5 code now assumes PILOT_SERIAL_MGR == PILOT_SERIAL_MGR_NEW
	#define	PILOT_SERIAL_MGR		#error "PILOT_SERIAL_MGR is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef 	MEMORY_VERSION
	// Palm OS 3.5 code now assumes MEMORY_VERSION == MEMORY_VERSION_2
	#define	MEMORY_VERSION			#error "MEMORY_VERSION is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef 	GRAPHICS_VERSION
	// Palm OS 3.5 code now assumes GRAPHICS_VERSION == GRAPHICS_VERSION_2
	#define	GRAPHICS_VERSION		#error "GRAPHICS_VERSION is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef 	HW_TARGET
	// Palm OS 3.5 is now hardware independent; HAL should handle this
	#define	HW_TARGET				#error "HW_TARGET is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef 	HW_REV
	// Palm OS 3.5 is now hardware independent; HAL should handle this
	#define	HW_REV					#error "HW_REV is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef	RMP_LIB_INCLUDE
	#define	RMP_LIB_INCLUDE		#error "RMP_LIB_INCLUDE is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
#ifndef	OEM_PRODUCT
	#define	OEM_PRODUCT				#error "OEM_PRODUCT is an obsolete build flag!"
#endif

// Obsolete option; this is left here to catch any stragglers (test code, etc.)...
// LANGUAGE/COUNTRY are no longer supported. If absolutely necessary, use LOCALE instead, but
// build-time dependencies on the target LOCALE are a _really_ bad idea.
#ifdef 	LANGUAGE
	#undef LANGUAGE
#endif
#define	LANGUAGE					#error "LANGUAGE is now obsolete; use LOCALE and include PalmOptLocale.h"

#ifndef	COUNTRY
	#undef COUNTRY
#endif
#define	COUNTRY					#error "COUNTRY is now obsolete; use LOCALE and include PalmOptLocale.h"

// Moved to <PalmTypes.h>:
//#if defined(__GNUC__) && defined(__UNIX__)
//	// Ensure that structure elements are 16-bit aligned
//	#pragma pack(2)
//#endif

#endif
