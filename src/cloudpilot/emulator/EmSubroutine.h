/* -*- mode: C++; tab-width: 4 -*- */
/* ===================================================================== *\
        Copyright (c) 2001 Palm, Inc. or its subsidiaries.
        All rights reserved.

        This file is part of the Palm OS Emulator.

        This program is free software; you can redistribute it and/or modify
        it under the terms of the GNU General Public License as published by
        the Free Software Foundation; either version 2 of the License, or
        (at your option) any later version.
\* ===================================================================== */

#ifndef EmSubroutine_h
#define EmSubroutine_h

#include <string>
#include <vector>

#include "EmCommon.h"

/*
        Overview:
        ---------

                From time to time, Poser needs to interfere with emulated code
                as it calls from one subroutine to another.  In some cases, Poser
                needs to intercept the call on entry or exit and perform some
                actions.  On rarer occasions, Poser needs to completely replace
                the called subroutine, prohibiting from executing entirely.  And
                on other occasions, Poser needs to be able to call a subroutine
                in the emulated environment.

                The EmSubroutine class helps in these activities.  For the cases
                where it is patching a called subroutine (possibly replacing it
                entirely), Poser needs to read the parameters passed to the subroutine,
                and possibly even provide a function result.  For the cases where
                is calling an emulated subroutine, Poser needs to push parameters
                onto the emulated stack, and then read back any function result
                when the subroutine returns.

                Both of these activities (calling an emulated function, and being
                called from an emulated function) require that Poser know the
                format of the stack (that is, the number, order, and type of the
                parameters pushed or to be pushed onto the stack).

                The EmSubroutine class has methods used to define the stack format.
                When defining the stack format, the EmSubroutine class needs to
                provide one to three pieces of information for each parameter:

                        *	The parameter type.
                        *	An optional parameter name.
                        *	An optional parameter value.


        Calling an emulated function:
        -----------------------------

                When Poser needs to call an emulated function, it takes the following
                steps:

                        *	Creates an EmSubroutine object.

                        *	Specifies the types and values of the parameters with a
                                specification string passed to the object's constructor.

                        *	Sets the values of the parameters with multiple calls
                                to SetParamVal.

                        *	Calls the function by calling Call.

                        *	Reads any return value by calling GetReturnVal.

                For example:

                        Err DmCreateDatabase (UInt16 cardNo, const Char * const nameP,
                                                                UInt32 creator, UInt32 type, Boolean
   resDB)
                        {
                                // Create subroutine object.

                                EmSubroutine	sub (	"Err",
                                                                                "UInt16 cardNo,"
                                                                                "const Char * const
   nameP," "UInt32 creator," "UInt32 type," "Boolean resDB", true);

                                // Set parameter values.

                                sub.SetParamVal ("cardNo", cardNo);
                                sub.SetParamVal ("nameP", nameP);
                                sub.SetParamVal ("creator", creator);
                                sub.SetParamVal ("type", type);
                                sub.SetParamVal ("resDB", resDB);

                                // Call the subroutine.

                                sub.Call (sysTrapDmCreateDatabase);

                                // Get and return subroutine result.

                                Err result;
                                sub.GetReturnVal (result);
                                return result;
                        }


        Being called by an emulated function:
        -------------------------------------

                When Poser needs to respond to an emulated function making a
                subroutine call, it takes the following steps:

                        *	Creates an EmSubroutine object.

                        *	Specifies the types and values of the parameters with a
                                specification string passed to the object's constructor.

                        *	Retrieves the values on the stack with multple calls
                                to GetParamVal.

                        *	Optionally specifies a function return value by calling
                                SetReturnVal.

                For example:

                        CallROMType SysHeadpatch::ExgDoDialog (void)
                        {
                                // Boolean ExgDoDialog (ExgSocketPtr socketP,
                                //						ExgDialogInfoType*
   infoP,
                                //						Err* errP)

                                // If this patch is not supposed to do anything special
                                // right now, just return, saying that the ROM version
                                // should be executed as normal.

                                if (!gAutoAcceptBeamDialogs)
                                        return kExecuteROM;

                                // Create subroutine object.

                                EmSubroutine	sub (	"Boolean",
                                                                                "ExgSocketPtr
   socketP," "ExgDialogInfoType* infoP," "Err* errP", false);

                                // Read the parameter values.

                                void*	socketP;
                                void*	infoP;
                                void*	errP;

                                sub.GetParamVal ("socketP", socketP);
                                sub.GetParamVal ("infoP", infoP);
                                sub.GetParamVal ("errP", errP);

                                // If no socket, just return, saying that the ROM version
                                // should be executed as normal.

                                if (!socketP)
                                        return kExecuteROM;

                                // Set the "errP" parameter to zero.

                                sub.SetParamRef ("errP", (Err) errNone);

                                // Set the function result to "true".

                                sub.SetReturnVal ((Bool) true);

                                // Return, saying that we completely implemented the
                                // function and that the ROM version should not be
                                // executed at all.

                                return kSkipROM;
                        }
*/

typedef string EmParamName;
typedef const char* EmParamNameArg;
typedef const char* EmParamDecl;
typedef const char* EmParamListDecl;

enum EmParamType {
    kEm_Unknown,  // Unknown type.
    kEm_Void,     // Return type, and generic pointer type

    kEm_SI1,  // 1-byte signed integer
    kEm_SI2,  // 2-byte signed integer
    kEm_SI4,  // 4-byte signed integer
    kEm_SI8,  // 8-byte signed integer

    kEm_UI1,  // 1-byte unsigned integer
    kEm_UI2,  // 2-byte unsigned integer
    kEm_UI4,  // 4-byte unsigned integer
    kEm_UI8,  // 8-byte unsigned integer

    kEm_FP4,  // 4-byte floating point
    kEm_FP8,  // 8-byte floating point
    kEm_FP16  // 16-byte floating point
};

struct EmParamInfo {
    const char* fTypeName;

    // If the parameter is passed by value, this holds the type of
    // the value.  If the parameter is pass by reference, this holds
    // the type pointed to.

    EmParamType fType;

    // If this is false, the parameter is passed by value, and pushed
    // directly onto the stack.  If this is true, the parameter is
    // passed by reference, and a pointer to the value is pushed onto
    // the stack.
    //
    // If the EmParamInfo is used to record information about a function's
    // return value, fByRef can indicate where the return value is stored.
    // On 68K platforms, pointers are stored in A0, while directly scalar
    // values are stored in D0.

    Bool fByRef;
};

struct EmParam {
    EmParam(void) : fName(), fType(kEm_Void), fStackOffset(0), fByRef(false) {}

    EmParamName fName;

    EmParamType fType;
    long fStackOffset;
    Bool fByRef;
};

typedef vector<EmParam> EmParamList;

class EmSubroutineCPU;

#define kForCalling true
#define kForBeingCalled false

enum { kEmErrUnsupportedType = 1, kEmErrUnknownParameter, kEmErrTypeMismatch };

#define VIRTUAL

class EmSubroutine {
   public:
    EmSubroutine(void);
    VIRTUAL ~EmSubroutine(void);

    VIRTUAL Err DescribeDecl(EmParamDecl, EmParamListDecl);
    VIRTUAL Err AddParam(EmParamDecl);
    VIRTUAL Err PrepareStack(Bool forCalling, Bool forStdArgs);
    VIRTUAL Err PrepareStack(emuptr);
    VIRTUAL Err Reset(void);

    // Read the parameter indicated by the given parameter name
    // and return it as the given type.
    //
    // !!! What to do if the given parameter type is not the same
    // as the return parameter type?

    VIRTUAL Err GetParamVal(EmParamNameArg, int8&);
    VIRTUAL Err GetParamVal(EmParamNameArg, int16&);
    VIRTUAL Err GetParamVal(EmParamNameArg, int32&);
    VIRTUAL Err GetParamVal(EmParamNameArg, int64&);
    VIRTUAL Err GetParamVal(EmParamNameArg, uint8&);
    VIRTUAL Err GetParamVal(EmParamNameArg, uint16&);
    VIRTUAL Err GetParamVal(EmParamNameArg, uint32&);
    VIRTUAL Err GetParamVal(EmParamNameArg, uint64&);
    VIRTUAL Err GetParamVal(EmParamNameArg, float&);
    VIRTUAL Err GetParamVal(EmParamNameArg, double&);
    VIRTUAL Err GetParamVal(EmParamNameArg, long double&);

    // Get the value returned by the function.

    VIRTUAL Err GetReturnVal(int8&);
    VIRTUAL Err GetReturnVal(int16&);
    VIRTUAL Err GetReturnVal(int32&);
    VIRTUAL Err GetReturnVal(int64&);
    VIRTUAL Err GetReturnVal(uint8&);
    VIRTUAL Err GetReturnVal(uint16&);
    VIRTUAL Err GetReturnVal(uint32&);
    VIRTUAL Err GetReturnVal(uint64&);
    VIRTUAL Err GetReturnVal(float&);
    VIRTUAL Err GetReturnVal(double&);
    VIRTUAL Err GetReturnVal(long double&);

    // Store the given value into the memory pointed to by the
    // pointer parameter indicated by the given parameter name.
    // These functions are useful for returning results in "pass
    // by reference" parameters.
    //
    // !!! What to do if the specified parameter is not a pointer
    // type.  Of less concern, what if it is not the right type?

    VIRTUAL Err SetParamVal(EmParamNameArg, int8);
    VIRTUAL Err SetParamVal(EmParamNameArg, int16);
    VIRTUAL Err SetParamVal(EmParamNameArg, int32);
    VIRTUAL Err SetParamVal(EmParamNameArg, int64);
    VIRTUAL Err SetParamVal(EmParamNameArg, uint8);
    VIRTUAL Err SetParamVal(EmParamNameArg, uint16);
    VIRTUAL Err SetParamVal(EmParamNameArg, uint32);
    VIRTUAL Err SetParamVal(EmParamNameArg, uint64);
    VIRTUAL Err SetParamVal(EmParamNameArg, float);
    VIRTUAL Err SetParamVal(EmParamNameArg, double);
    VIRTUAL Err SetParamVal(EmParamNameArg, long double);

    // Set the value to be returned by the function.

    VIRTUAL Err SetReturnVal(int8);
    VIRTUAL Err SetReturnVal(int16);
    VIRTUAL Err SetReturnVal(int32);
    VIRTUAL Err SetReturnVal(int64);
    VIRTUAL Err SetReturnVal(uint8);
    VIRTUAL Err SetReturnVal(uint16);
    VIRTUAL Err SetReturnVal(uint32);
    VIRTUAL Err SetReturnVal(uint64);
    VIRTUAL Err SetReturnVal(float);
    VIRTUAL Err SetReturnVal(double);
    VIRTUAL Err SetReturnVal(long double);

    // Call the given system function.  The first form merely
    // calls the indicated function.  The second stores the
    // selector in D2 and then calls the function.
    //
    // !!! Needs to be extended for calling library functions,
    // calling HostControl functions (which take the selector
    // on the stack), and may need to be adjusted according to
    // however ARM functions will be specified.

    VIRTUAL Err Call(uint16 trapWord);
    VIRTUAL Err CallSelector(uint16 trapWord, uint16 selector);

   private:
    uint32 GetReturnRegInteger(void);
    emuptr GetReturnRegPointer(void);

    void SetReturnRegInteger(uint32);
    void SetReturnRegPointer(uint32);

    EmSubroutineCPU* GetCPU(void);
    Bool Is68K(void);

    Err ParseParamDecl(EmParamDecl, EmParam&);
    string GetToken(EmParamDecl, string::size_type& offset);
    EmParamList::iterator FindParam(EmParamNameArg);

   private:
    EmSubroutineCPU* fCPU;
    EmParamList fParams;
    EmParam fReturnType;
    long fStackSize;
};

#endif  // EmSubroutine_h
