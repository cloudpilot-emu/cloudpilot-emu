#include "Miscellaneous.h"

#include "EmMemory.h"
#include "EmPalmFunction.h"
#include "UAE.h"

uint32 NextPowerOf2(uint32 n) {
    // Smear down the upper 1 bit to all bits lower than it.

    uint32 n2 = n;

    n2 |= n2 >> 1;
    n2 |= n2 >> 2;
    n2 |= n2 >> 4;
    n2 |= n2 >> 8;
    n2 |= n2 >> 16;

    // Now use itself to clear all the lower bits.

    n2 &= ~(n2 >> 1);

    // If n2 ends up being the same as what we started with, keep it.
    // Otherwise, we need to bump it by a factor of two (round up).

    if (n2 != n) n2 <<= 1;

    return n2;
}

/***********************************************************************
 *
 * FUNCTION:	GetSystemCallContext
 *
 * DESCRIPTION: .
 *
 * PARAMETERS:	None.
 *
 * RETURNED:	Nothing.
 *
 ***********************************************************************/

Bool GetSystemCallContext(emuptr pc, SystemCallContext& context) {
    context.fPC = pc;

    // Determine how the system function is being called.  There are two ways:
    //
    //		* Via SYS_TRAP macro:
    //
    //			TRAP	$F
    //			DC.W	$Axxx
    //
    //		* Via SYS_TRAP_FAST macro:
    //
    //			MOVE.L	struct(LowMemType.fixed.globals.sysDispatchTableP), A1
    //			MOVE.L	((trapNum-sysTrapBase)*4)(A1), A1
    //			JSR 	(A1)	; opcode == 0x4e91
    //
    // The PC is current pointing to either the TRAP $F or the JSR (A1),
    // so we can look at the opcode to determine how we got here.

    uint8* realMem = EmMemGetRealAddress(pc);
    uint16 opcode = EmMemDoGet16(realMem);

    context.fViaTrap = opcode == (m68kTrapInstr + sysDispatchTrapNum);
    context.fViaJsrA1 = opcode == (0x4e91);

    if (context.fViaTrap) {
        // Not all development systems generate the correct dispatch
        // numbers; some leave off the preceding "A".  Make sure it's
        // set so that we can recognize it as a trap dispatch number.
        // (This code is here specifically so that the profiling routines
        //	will work, which check for trap numbers masquerading as function
        //	addresses by checking to see if they are in the sysTrapBase range.)

        context.fTrapWord = EmMemGet16(pc + 2) | sysTrapBase;
        context.fNextPC = pc + 4;
    } else if (context.fViaJsrA1) {
        context.fTrapWord = (EmMemGet16(pc - 2) / 4) | sysTrapBase;
        context.fNextPC = pc + 2;
    } else {
        EmAssert(false);
        return false;
    }

    if (::IsSystemTrap(context.fTrapWord)) {
        context.fTrapIndex = SysTrapIndex(context.fTrapWord);
        context.fExtra = m68k_dreg(regs, 2);
    } else {
        context.fTrapIndex = LibTrapIndex(context.fTrapWord);
        context.fExtra = EmMemGet16(m68k_areg(regs, 7));
    }

    EmAssert((context.fTrapWord >= sysTrapBase) && (context.fTrapWord < sysTrapBase + 0x1000));

    return true;
}

/***********************************************************************
 *
 * FUNCTION:    SeparateList
 *
 * DESCRIPTION: Break up a comma-delimited list of items, returning the
 *				pieces in a StringList.
 *
 * PARAMETERS:  stringList - the StringList to receive the broken-up
 *					pieces of the comma-delimited list.  This collection
 *					is *not* first cleared out, so it's possible to add
 *					to the collection with this function.
 *
 *				str - the string containing the comma-delimited items.
 *
 * RETURNED:    Nothing
 *
 ***********************************************************************/

void SeparateList(StringList& stringList, string str, char delimiter) {
    string::size_type offset;

    while ((offset = str.find(delimiter)) != string::npos) {
        string nextElement = str.substr(0, offset);
        str = str.substr(offset + 1);
        stringList.push_back(nextElement);
    }

    stringList.push_back(str);
}

/***********************************************************************
 *
 * FUNCTION:	EndsWith
 *
 * DESCRIPTION:	Determine if a string end with the given pattern.
 *
 * PARAMETERS:	s - string to test.
 *
 *				p - pattern to test with.
 *
 * RETURNED:	True if "s" ends with "p".
 *
 ***********************************************************************/

Bool EndsWith(const char* s, const char* p) {
    if (strlen(s) < strlen(p)) return false;

    const char* buffer = s + strlen(s) - strlen(p);
    return (strcasecmp(buffer, p) == 0);
}

StMemoryMapper::StMemoryMapper(const void* memory, long size) : fMemory(memory) {
    if (fMemory) Memory::MapPhysicalMemory(fMemory, size);
}

StMemoryMapper::~StMemoryMapper(void) {
    if (fMemory) Memory::UnmapPhysicalMemory(fMemory);
}
