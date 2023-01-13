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

#include "EmRegsMediaQ11xx.h"

#include <algorithm>  // swap

#include "ChunkHelper.h"
#include "EmCPU68K.h"  // gCPU68K
#include "EmCommon.h"
#include "EmRegsFrameBuffer.h"
#include "EmSession.h"
#include "EmSystemState.h"
#include "Frame.h"
#include "Logging.h"  // LogAppendMsg
#include "Nibbler.h"
#include "Savestate.h"
#include "SavestateLoader.h"
#include "SavestateProbe.h"
#include "ScreenDimensions.h"

#define LogAppendMsg PRINTF

// #define LOGGING 0
#ifdef LOGGING
    #define PRINTF_LINE logging::printf
    #define PRINTF_BLIT logging::printf
    #define PRINTF logging::printf
#else
    #define PRINTF_LINE(...) ;
    #define PRINTF_BLIT(...) ;
    #define PRINTF(...) ;
#endif

namespace {
    constexpr uint32 SAVESTATE_VERSION = 1;

    uint16 cscolor = 0;

    template <typename T>
    bool IsEven(T t) {
        return ((t & 0x01) == 0);
    }
}  // namespace

/* --------------------------------------------------------------------------- *\
        MediaQ Graphics Engine overview and description
        -----------------------------------------------

        The MediaQ 1100/1132 chip contains a graphics engine used for accelerating
        graphics drawing operations.  It has the following features:

                * Line drawing
                * Rectangle operations (fill, scroll, and BLT)
                * Arbitrary transfer operations (using Windows ROP definitions)
                * Monochrome and color source data
                * Monochrome pattern data
                * Colorization of monochrome source and pattern data
                * Transparency support
                * Clipping support
                * Automatic screen flipping

        There are 20 registers reserved for the graphics accelerator.  The MediaQ
        client generally works by storing necessary values in a subset of those
        registers, and then kicks off the drawing process by updating the register
        that receives the drawing command.  This register (GE00R) is the master
        control register.  It contains all of the top-level information about what
        is to be drawn and how.  As needed, other registers are queried for
        additional values needed for the operation.  Registers not required for
        the drawing operation need not be updated before the drawing command is
        issued.  The drawing operation takes place asynchronously, freeing up the
        MediaQ client for other tasks.

        The graphics chip makes use of two FIFOs: a command FIFO and a source data
        FIFO.  The command FIFO is used to make sure that registers are not
        updated and changed out from under a drawing operation currently in
        progress.  That is, any write access to a register really results in an
        entry being added to the command FIFO.  Entries are then popped off and
        used to update the registers.  If the command register is written to, the
        drawing operation commences.  Any subsequent writes to registers will
        still be added to the FIFO, but will stay there until the drawing
        operation has completed.  If the FIFO is full (it has room only for 16
        commands), the main CPU will stall if it tries to add any more commands.

        Sometimes the drawing operation requires "source data", that is, data from
        main memory.  The MediaQ chip doesn't have direct access to main RAM, and
        so data from there needs to be fed in through a source FIFO.  After a
        drawing command has been started that requires source data, the MediaQ
        client must go into a loop and ensure that this source data FIFO is filled
        with the necessary data.  Note that this source FIFO contains 16 64-bit
        slots.  Therefore, the client needs to make sure that information written
        to the FIFO occurs in multiples of 64-bits (or 8 bytes).

        GEState
        -------

        This Poser-defined structure is a mirror of the registers used by the
        MediaQ graphics acceleration engine.  It breaks out all of the pieces of
        information packed into those registers, and stores each piece into its
        own field for easy access.  This unpacking takes place as needed,
        generally when a graphics drawing command is issued.

        Following is a description of all the registers and the fields within
        those registers.  After that are included some notes on how those
        registers fit into the bigger picture.

        **************************************************************************
        ==========================================================================
                                                                        GE00R
        ==========================================================================

        GE00R[7:0]	rasterOperation
        -----------------------------------------------
                Specifies the transfer mode.  That is, it defines the way in which the
                source data, destination data, and pattern data are combined to
                produce the final destination or output data.  The values that can be
                used here are the same that Microsoft has defined for GDI.

                This register can contain either the ternary ROP codes or the binary
                ROP2 codes, depending on the setting of rop2Select (GE00R[25]).

        GE00R[10:8]	commandType
        -----------------------------------------------
                Specifies the drawing command.  Can be one of three values:

                        0 = NOP
                        2 = BitBLT
                        4 = Line

                All other values are either reserved or undefined.

                Note that BitBLT is used for drawing filled rectangles, scrolling
                images in the screen memory, or copy images from main memory to the
                screen memory.  Monochrome and color source data and patterns are
                supported.

        GE00R[11]	xDirection
        GE00R[12]	yDirection
        -----------------------------------------------
                Defines the direction in which a BitBLT or line drawing operation
                occurs.  Values of zero indicate the positive (right or down)
                direction; values of one indicate the negative (left or up) direction.
                When drawing a line, note that these fields are used only if the
                useXY field is set.

        GE00R[13]	systemMemory
        -----------------------------------------------
                If true, specifies that any source data comes from the source FIFO.
                Otherwise, the data comes from display memory, and xSrc (GE03R[11:0])
                and ySrc (GE03R[27:16]) need to be programmed.

        GE00R[14]	monoSource
        -----------------------------------------------
                Specifies whether the source data is a monochrome (aka, bitonal)
                bitmap where each pixel is represented by a single bit, or that the
                source data is in color format, where each pixel is defined by an 8 or
                16 bit value, depending on the setting of colorDepth (GE0AR[31:30]).

        GE00R[15]	monoPattern
        -----------------------------------------------
                Specifies whether the pattern data is a monochrome (aka, bitonal)
                bitmap where each pixel is represented by a single bit, or that the
                pattern data is in color format, where each pixel is defined by an 8
                or 16 bit value.  Note that in the current chip, this bit *MUST* be
                set to 1, indicating a monochrome pattern.  There doesn't appear to be
                any support or any way to specify a color pattern.

        GE00R[16]	colorTransEnable
        -----------------------------------------------
                Setting this bit enables transparent BitBLT operations when the source
                data is in color format.  It works in conjunction with
                destTransPolarity, colorTransCmpSrc, and destTransColor to determine
                if the destination pixel is updated or stays the same.

        GE00R[17]	destTransPolarity
        -----------------------------------------------
                If color transparency is enabled (that is, colorTransEnable is set),
                determines how transparency is achieved.  Depending on the setting of
                colorTransCmpSrc, either the source data or the dest data is compared
                to destTransColor.  If destTransPolarity is zero, then the destination
                pixel is updated if the result of the comparison is not equal.  If
                destTransPolarity is one, then the destination pixel is update if the
                result is equal.

        GE00R[18]	monoTransEnable
        -----------------------------------------------
                Setting this bit enables transparent BitBLT operations when the source
                data is in monochrome format.  It works in conjunction with
                monoTransPolarity to determine if the destination pixel is updated or
                stays the same.

        GE00R[19]	monoTransPolarity
        -----------------------------------------------
                If monochrome transparency is enabled (that is, monoTransEnable is
                set), determines how transparency is achieved.  If this value is zero,
                then background (that is, zero) bits in the source are treated as
                transparent and don't update the destination.  If this value is one,
                then foreground (that is, one) bits in the source data are treated as
                transparent.

        GE00R[20]	memToScreen
        -----------------------------------------------
                Specifies LINED (memToScreen == 0) or PACKED (memToScreen == 1) blit
                transfer mode.  PACKED Mode allows bitmaps that are compact to be
                transferred more efficiently.  LINE Mode requires each line of bitmap
                to be aligned at 64-bit and thus could require padding and realignment
                if each line of bitmap to be transfer is non-64-bit aligned.

        GE00R[23]	solidSourceColor
        -----------------------------------------------
                If set, the actual color source data is effectively ignored, and
                instead treated as if it were all fgColorMonoSrc.

        GE00R[24]	srcEqualDestStride
        -----------------------------------------------
                Specifies whether or not the dest and source have the same "strides"
                (that is, "rowBytes" in other terminologies).  If the source is the
                display, then the source will always be the same as the dest.  But if
                the source is main memory, then the data comes in through the source
                FIFO, and I'm not sure how stride comes in to play there.

        GE00R[25]	rop2Select
        -----------------------------------------------
                Causes the value in rasterOperation (GE00R[7:0]) to be treated as a
                binary ROP2 value instead of a ternary ROP value.  The documentation
                says that "[3:0] is duplicated to [7:4]".  However, it's not clear to
                me is the programmer is required to write the register in this
                duplicated fashion, or if the hardware aliases the low values into the
                high values in some way.  In either case, I'm not sure why the
                hardware cares.  Why wouldn't it just look at the low 4 bits and
                ignore the upper 4 bits.  My guess is that the hardware performs this
                aliasing on its own, and that the result when looking at all 8 bits
                has the result of acting like a binary ROP2 code (that is, the any
                source data will be ignored -- see comments in PrvUsesSource on how
                the ROP value can be examined to see if the pattern, source or
                destination have any role in determining the final output value).

        GE00R[26]	clipEnable
        -----------------------------------------------
                Enable clipping according to the clipLeft, clipTop, clipRight, and
                clipBottom fields (registers GE05R and GE06R).

        GE00R[27]	autoExecute
        -----------------------------------------------
                Setting this bit enables auto-execution of the command after the
                destination XY register (GE02R) is written.  If it's clear, the
                graphics engine command execution starts the command register (GE00R)
                is written.

        GE00R[30]	solidPattern
        -----------------------------------------------
                If set, then all pattern data is forced to the value of the pattern
                foreground color register (GE12R).

        GE00R[31]	colorTransCmpSrc
        -----------------------------------------------
                Used in conjunction with the transparency-enable bits, determines if
                the color to be compared comes from the source or destination.
                Compare to the source data if it's zero, destination if it's one.

        ==========================================================================
                                                                        GE01R
        ==========================================================================

        GE01R[11:0] (BitBLT)	width
        GE01R[27:16] (BitBLT)	height
        -----------------------------------------------
                When a BitBLT operation has been specified, these fields contain the
                dimensions (in pixels) of the operation.  It is used for both the
                source and destination.

        GE01R[31] (BitBLT)	xyConversion
        -----------------------------------------------
                When a BitBLT operation has been specified, this field controls how
                the starting x/y values are interpreted.  If this bit is clear, then
                the specified x/y values are the first pixel updated.  If this bit is
                set, then the specified x/y values are the top/left corner of the
                rectangle that's updated.

        GE01R[16:0] (Line)	gamma
        -----------------------------------------------
                When a line drawing operation has been specified, contains a value
                identified as "gamma".  This is defined in the documentation as
                "either -(dm >> 1) - 1 or -(dm >> 1), depending on the quadrant",
                where "dm" is the major axis delta.  However, there's no mention of
                which value is used in what situations.  As well, our source code
                writes to this field a value that has the same magnitude of dm.  That
                is dm gets written to its designated field, and -dm gets written to
                gamma.

        GE01R[28:17] (Line)	majorLength
        -----------------------------------------------
                When a line drawing operation has been specified, contains the length
                along the major axis.

        GE01R[29] (Line)	yIsMajor
        -----------------------------------------------
                When a line drawing operation has been specified, specifies which axis
                is the major axis (that is, the "long" axis -- a "mostly horizontal"
                line has X as the major axis, while a "mostly vertical" line has Y as
                the major axis, and a diagonal line can have either defines as its
                major axis).  If set, Y is the major axis.  Otherwise, X is the major
                axis.  Note that this field is used only if the useXY field is set.

        GE01R[30] (Line)	drawLastPixel
        -----------------------------------------------
                When a line drawing operation has been specified, specifies whether or
                not the last pixel in the rasterization process is drawn.

        GE01R[31] (Line)	useXY
        -----------------------------------------------
                When a line drawing operation has been specified, specifies the method
                used to determine in which direction the line should be drawn.  If
                this field is set, then the information about direction and slope is
                taken from the xDirection, yDirection, and yIsMajor fields.  If this
                field is clear, then the information is taken from the quadrant field.

        ==========================================================================
                                                                        GE02R
        ==========================================================================

        GE02R[11:0] (BitBLT)	xDest
        GE02R[27:16] (BitBLT)	yDest
        -----------------------------------------------
                When a BitBLT operation has been specified, these fields contain the
                initial x,y location to receive the blitted data.  Note that these
                values are always the topLeft of the destination area, regardless of
                xDirection and yDirection.

        GE02R[15:13] (BitBLT)	monoPatternXOffset
        GE02R[31:29] (BitBLT)	monoPatternYOffset
        -----------------------------------------------
                When a BitBLT operation has been specified, these fields contain the
                initial x,y offsets into the monochrome pattern being used.

        GE02R[11:0] (Line)	xStart
        -----------------------------------------------
                When a line drawing operation has been specified, specifies starting X
                position of the line.

        GE02R[28:12] (Line)	deltaMajor
        -----------------------------------------------
                When a line drawing operation has been specified, specifies the length
                of the line along the major axis.  See yIsMajor for a definition of
                "major axis".

        GE02R[31:29] (Line)	quadrant
        -----------------------------------------------
                When a line drawing operation has been specified, specifies the
                direction and angle in which the line is drawn.  This is used only
                when the useXY field is clear, and replaces the same information
                provided by xDirection, yDirection, and yIsMajor.  The field contains
                a value from 0 to 7, indicating one of the following 8 quadrants:

                        Quadrant 0 (x-major, positive x and y directions)
                        Quadrant 1 (y-major, positive x and y directions)
                        Quadrant 2 (y-major, negative x and positive y directions)
                        Quadrant 3 (x-major, negative x and positive y directions)
                        Quadrant 4 (x-major, negative x and y directions)
                        Quadrant 5 (y-major, negative x and y directions)
                        Quadrant 6 (y-major, positive x and negative y directions)
                        Quadrant 7 (x-major, positive x and negative y directions)

        ==========================================================================
                                                                        GE03R
        ==========================================================================

        GE03R[11:0] (BitBLT)	xSrc
        GE03R[27:16] (BitBLT)	ySrc
        -----------------------------------------------
                When a BitBLT operation has been specified, these fields contain the
                initial x,y location from which to read the source data.  Only used
                when the source data is coming from display memory and not the source
                FIFO.  That is, used in scrolling operations.  Note that these values
                are always the topLeft of the source area, regardless of xDirection
                and yDirection.

        GE03R[11:0] (Line)	yStart
        -----------------------------------------------
                When a line drawing operation has been specified, specifies starting Y
                position of the line.

        GE03R[28:12] (Line)	deltaMinor
        -----------------------------------------------
                When a line drawing operation has been specified, specifies the length
                of the line along the minor axis.  The "minor axis" is the X or Y axis
                that is not designated as the major axis.  See yIsMajor for a
                definition of "major axis".

        ==========================================================================
                                                                        GE04R
        ==========================================================================

        GE04R[15:0]	destTransColor
        -----------------------------------------------
                When color transparency has been enabled by setting the
                colorTransEnable field, specifies the color that should be compared
                against.

        ==========================================================================
                                                                        GE05R/GE06R
        ==========================================================================

        GE05R[10:0]		clipLeft
        GE05R[25:16]	clipTop
        GE06R[10:0]		clipRight
        GE06R[25:16]	clipBottom
        -----------------------------------------------
                When the clippingEnable bit is set, these four fields specify the
                clipping boundary.  Pixels outside this rectangle are not written.

        ==========================================================================
                                                                        GE07R/GE08R
        ==========================================================================

        GE07R[15:0]		fgColorMonoSrc
        GE08R[15:0]		bgColorMonoSrc
        -----------------------------------------------
                When a monochrome source is being used, these two fields contain the
                fore- and background colors that should be applied to the bits
                representing the pixels.  Any 1 bit is replace with fgColorMonoSrc,
                and any 0 bit is replaces with bgColorMonoSrc.

        ==========================================================================
                                                                        GE09R
        ==========================================================================

        GE09R[9:0] (Lined)	srcLineStride
        -----------------------------------------------
                When Lined mode is being used (see memToScreen (GE00R[20])) and
                srcEqualDestStride is false, contains the number of bytes in a line
                of source data.

        GE09R[27:25] (Lined)	srcBitOffset
        -----------------------------------------------
                When Lined mode is being used (see memToScreen (GE00R[20])), contains
                the bit offset into the byte containing the leftmost pixel.  Makes
                sense only with a monochrome source.

        GE09R[30:28] (Lined)	srcByteOffset
        -----------------------------------------------
                When Lined mode is being used (see memToScreen (GE00R[20])), contains
                the byte offset of the byte containing the leftmost pixel.

        GE09R[2:0] (Packed)	srcLeadingBits
        -----------------------------------------------
                When Packed mode is being used (see memToScreen (GE00R[20])), contains
                the bit offset into the byte containing the leftmost pixel.  Makes
                sense only with a monochrome source.

        GE09R[5:3] (Packed)	srcLeadingBytes
        -----------------------------------------------
                When Packed mode is being used (see memToScreen (GE00R[20])), contains
                the byte offset of the byte containing the leftmost pixel.

        GE09R[15:6] (Packed)	srcNumBytes
        -----------------------------------------------
                When Packed mode is being used (see memToScreen (GE00R[20])), described
                in the manual as: Number of 8 bytes amount that MIU needs to fetch from
                frame buffer.  For off-screen to on-screen packed mode, these 10 bits
                represent the number of 8 bytes needed to be fetched by the MIU; the
                maximum amount it can address is 16 Kbyte, which is the off screen
                monochrome font or monochrome image graphics engine can support for
                packed mode.

        GE09R[27:25] (Packed)	srcTrailingBits
        -----------------------------------------------
                When Packed mode is being used (see memToScreen (GE00R[20])), contains
                the number of unused trailing bits in the last byte containing pixel
                data.  Makes sense only in monochrome mode.

        GE09R[31:28] (Packed)	srcTrailingBytes
        -----------------------------------------------
                When Packed mode is being used (see memToScreen (GE00R[20])), contains
                the number of unused trailing bytes.

        ==========================================================================
                                                                        GE0AR
        ==========================================================================

        GE0AR[9:0]	destLineStride
        -----------------------------------------------
                Specifies the stride (or rowBytes) of the destination bitmap.  The
                stride is the number of bytes between the first pixel of one line and
                the first pixel of the following lines.  This information duplicates
                that stored in the graphic controller, but presumably is stored here
                as well because the controller and accelerator are separate modules.

        GE0AR[28]	monoSrcBitSwap
        -----------------------------------------------
                Determines the interpretation of the bits in a monochrome pattern or
                source bitmap.  If this bit is clear, then bit 7 of a byte is the
                leftmost pixel and bit 0 is the rightmost pixel.  If this bit is set,
                then bit 0 is the leftmost pixel and bit 7 is the rightmost.  Having
                this bit clear is consistent with Windows CE and Palm, and having it
                set is consistent with EPOC.

        GE0AR[29]	rotate90
        -----------------------------------------------
                If set, rotates all drawing operation 90 degrees clockwise.

        GE0AR[31:30]	colorDepth
        -----------------------------------------------
                Determines the number of bits per pixel.  If zero, BPP == 8.  If one,
                BPP == 16.  All other settings are undefined.  This information
                duplicates that stored in the graphic controller, but presumably is
                stored here as well because the controller and accelerator are
                separate modules.

        ==========================================================================
                                                                        GE0BR
        ==========================================================================

        GE0BR[19:0]	baseAddr
        -----------------------------------------------
                Determines the base address of the frame buffer, relative to the start
                of the buffer.  That is, a value of 0x00000 indicates the beginning of
                the buffer, not the beginning of RAM.  This information duplicates
                that stored in the graphic controller, but presumably is stored here
                as well because the controller and accelerator are separate modules.

        GE0BR[29]		testModeEnable
        GE0BR[31:30]	testModeControl
        -----------------------------------------------
                Test enable and control bits.  Unsupported in Poser.

        ==========================================================================
                                                                        GE0CR
        ==========================================================================

        GE0CR[9:0]		cmdLineStart
        GE0CR[21:12]	cmdLineEnd
        -----------------------------------------------
                Specifies the beginning and end of the "command start window".
                However, there is no explanation of what this window is.

        GE0CR[24]	cmdLineControl
        -----------------------------------------------
                Controls whether or not the graphics engine needs to check with
                graphics controller before starting a command.  However, there's no
                explanation of how the graphics controller decides whether or not a
                command should be started.

        GE0CR[27:26]	gc1SwitchControl
        -----------------------------------------------
                Controls automatics switching between the main and alternate windows
                in the graphic controller.  A value of 0 or 2 means that no switching
                is performed.  A value of 1 means to switch to the main window after
                the current drawing command is done.  A value of 3 means to switch to
                the alternate window when the command is done.

        ==========================================================================
                                                                        GE0FR
        ==========================================================================

        GE0FR[31:0]
        -----------------------------------------------
                If testModeEnable is on, this field receives various values, as
                defined by testModeControl.

        ==========================================================================
                                                                        GE10R/GE11R
        ==========================================================================

        GE10R[31:0]	monoPattern1
        GE11R[31:0]	monoPattern2
        -----------------------------------------------
                Together, these fields specify 64 bits of an 8x8 monochrome pattern.
                The low-order byte of monoPattern1 contains the 8 bits for the first
                line, the next-order byte of monoPattern1 contains the 8 bits for the
                next line, etc., all the way up to the high-order byte of monoPattern2
                containing the 8 bits for the last line.

        ==========================================================================
                                                                        GE12R/GE13R
        ==========================================================================

        GE12R[15:0]	fgColorMonoPat
        GE13R[15:0]	bgColorMonoPat
        -----------------------------------------------
                When a monochrome pattern is being used, these two fields contain the
                fore- and background colors that should be applied to the bits
                representing the pixels.  Any 1 bit is replace with fgColorMonoPat,
                and any 0 bit is replaces with bgColorMonoPat.

        **************************************************************************

        Notes:
        -----------------------------------------------
        Transparency
        -----------------------------------------------
                Use destTransPolarity, colorTransCmpSrc, and destTransColor to
                determine if the destination needs to be updated.  All told, it should
                lay out like this:

                        destTransPolarity == 0
                                colorTransCmpSrc == 0
                                        Compare destTransColor to the source pixel.
                                        If ==, do not update the destination.

                                        That is, treat all destTransColor colored pixels
                                        in the source as transparent.

                                colorTransCmpSrc == 1
                                        Compare destTransColor to the destination pixel.
                                        If ==, do not update the destination.

                                        That is, leave all destTransColor colored pixels
                                        in the destination alone.

                        destTransPolarity == 1
                                colorTransCmpSrc == 0
                                        Compare destTransColor to the source pixel.
                                        If !=, do not update the destination.

                                        That is, transfer all destTransColor colored pixels
                                        from the source to the destination.

                                colorTransCmpSrc == 1
                                        Compare destTransColor to the destination pixel.
                                        If !=, do not update the destination.

                                        That is, update all destTransColor colored pixels
                                        in the destination with source pixels.

        ROPs
        -----------------------------------------------
                Take the pattern, source, and destination, and consider each value one
                bit at a time.  The three bits from the three values can form eight
                combinations.  The raster opcode is a bitfield containing the desired
                output from each of the combinations.  Thus:

                         P	 S	 D	 O (output)
                        ---	---	---	---
                         0	 0	 0	 b0
                         0	 0	 1	 b1
                         0	 1	 0	 b2
                         0	 1	 1	 b3
                         1	 0	 0	 b4
                         1	 0	 1	 b5
                         1	 1	 0	 b6
                         1	 1	 1	 b7

                The output is the byte <b7><b6><b5><b4><b3><b2><b1><b0>.  And this
                output value is what becomes the raster opcode.  Thus, if the raster
                opcode is, for example, 0xCC, the table would be:

                         P	 S	 D	 O (output)
                        ---	---	---	---
                         0	 0	 0	 0
                         0	 0	 1	 0
                         0	 1	 0	 1
                         0	 1	 1	 1
                         1	 0	 0	 0
                         1	 0	 1	 0
                         1	 1	 0	 1
                         1	 1	 1	 1

                So, the we look at three bits from the source and they're all zero,
                then the output is zero.  If they're all one, then the output is one.
                If the pattern is one and the source and destination are zero, then
                the output is zero. Overall,

                        Output[x] = ROP[P[x],S[x],D[x]]

                where "x" is 0..7, and "P[x],S[x],D[x]" is a bitfield composed of the
                given bits.

\* --------------------------------------------------------------------------- */

// Given a register (specified by its field name), return its address
// in emulated space.

#define mq_addressof(x)                                                       \
    (static_cast<emuptr>(((long)fRegs.x.GetPtr()) - ((long)fRegs.GetPtr())) + \
     ((emuptr)this->GetAddressStart()))

// Macro to help the installation of handlers for a register.

#define INSTALL_HANDLER(read, write, reg)                   \
    this->SetHandler((ReadFunction)&EmRegsMediaQ11xx::read, \
                     (WriteFunction)&EmRegsMediaQ11xx::write, mq_addressof(reg), 4)

// Private inline function for reading a LE register.

static inline uint32 PrvReadRegister(uint8* p) {
#if EM_HOST_BYTE_ORDER == EM_LITTLE_ENDIAN

    return *(uint32*)p;

#else

    return (((UInt32)p[0]) << 0) + (((UInt32)p[1]) << 8) + (((UInt32)p[2]) << 16) +
           (((UInt32)p[3]) << 24);

#endif
}

// Private inline function for writing a LE register

static inline void PrvWriteRegister(uint8* p, uint32 value) {
#if EM_HOST_BYTE_ORDER == EM_LITTLE_ENDIAN

    *(uint32*)p = value;

#else

    p[0] = value >> 0;
    p[1] = value >> 8;
    p[2] = value >> 16;
    p[3] = value >> 24;

#endif
}

// Macros to make calling PrvReadRegister and PrvWriteRegister easier.

#define READ_REGISTER(reg) ::PrvReadRegister((uint8*)(fRegs.reg.GetPtr()))
#define WRITE_REGISTER(reg, value) ::PrvWriteRegister((uint8*)(fRegs.reg.GetPtr()), value)

// ROP values

#define ROP_SRCCOPY 0xcc
#define ROP_SRCAND 0x88
// #define ROP_SRCANDNOT			0x77
#define ROP_SRCANDNOT 0x22
#define ROP_SRCXOR 0x66
#define ROP_SRCOR 0xee
#define ROP_SRCCOPYNOT 0x33
#define ROP_SRCORNOT 0xbb
#define ROP_SRCXORNOT 0x99

#define ROP_PATCOPY 0xf0
#define ROP_PATAND 0xa0
// #define ROP_PATANDNOT			0x5f
#define ROP_PATANDNOT 0x0a
#define ROP_PATXOR 0x5a
#define ROP_PATOR 0xfa
#define ROP_PATCOPYNOT 0x0f
#define ROP_PATORNOT 0xaf
#define ROP_PATXORNOT 0xa5

#define ROP_BLACKNESS 0x00

#define CMD_FIFO_MASK (0x0000001F)
#define CMD_FIFO_EMPTY (0x00000010)
#define CMD_SRC_MASK (0x00001F00)
#define CMD_SRC_EMPTY (0x00001000)
#define GE_BUSY_H (0x00010000)

/////////////////////////////////////////////////////////////////////////////
// Graphics Controller  Registers
//
#define GC_CONTROL (0x00)            // Graphics Controll  Control Reg
#define GC_PWR_SEQ_CONTROL (0x01)    // Power Sequencing Control register
#define GC_HTOTAL_HEND (0x02)        // Horizontal Display  Total & End
#define GC_VTOTAL_VEND (0x03)        // Vertical Display Total and End
#define GC_HSYNC (0x04)              // Horizontal Sync Start and End
#define GC_VSYNC (0x05)              // Vertical Sync Start and End
#define GC_HWINDOW (0x08)            // Horizontal Window Start & End
#define GC_VWINDOW (0x09)            // Vertical Window Start & End
#define GC_LINE_CLOCK (0x0B)         // Line Clock Control
#define GC_START_ADDR (0x0C)         // Image Window  Start Address
#define GC_ALTWIN_START_ADDR (0x0D)  // Alt Image Window  Start Address
#define GC_STRIDE (0x0E)             // (Alt) Image Window  Stride
#define GC_FRAME_CLOCK (0x1A)        // Frame Clock Control
#define GC_SIGNALS (0x1B)            // Miscellaneous signal register (read only)
#define GC_HPARAM (0x1C)             // Horizontal parameter register (read only)
#define GC_VPARAM (0x1D)             // Vertical parameter register (read only)

// GC_CONTROL register bit definition

#define GC_ENABLE (1UL << 0)        // Controll enabled
#define GC_H_PIX_DBLNG (1UL << 14)  // Enable Horizontal Pixel Doubling
#define GC_V_PIX_DBLNG (1UL << 15)  // Enable Vertical Scan Line Doubling
#define GC_PIX_DBLNG_MASK (GC_H_PIX_DBLNG | GC_V_PIX_DBLNG)

// Palette masks

#define RED_MASK 0x0000000FC
#define GREEN_MASK 0x00000FC00
#define BLUE_MASK 0x000FC0000

#define kColorDepth8 0
#define kColorDepth16 1

#define kCommandNOP 0
#define kCommandBitBLT 2
#define kCommandLine 4

#define kAllRegisters -1

static constexpr const char* kCommands[] = {
    "NOP", "Undefined", "BitBLT", "Reserved", "Line Draw", "Reserved", "Undefined", "Reserved",
};

static constexpr const char* kDirections[] = {"Positive", "Negative"};

const struct {
    uint16 yIsMajor;    // 0 = x is major, 1 = y is major
    uint16 xDirection;  // 0 = x is positive, 1 = x is negative
    uint16 yDirection;  // 0 = y is positive, 1 = y is negative
    const char* text;   // Description for debugging.
} kQuadrantDecode[] = {{0, 0, 0, "x-major, positive x and y directions"},
                       {1, 0, 0, "y-major, positive x and y directions"},
                       {1, 1, 0, "y-major, negative x and positive y directions"},
                       {0, 1, 0, "x-major, negative x and positive y directions"},
                       {0, 1, 1, "x-major, negative x and y directions"},
                       {1, 1, 1, "y-major, negative x and y directions"},
                       {1, 0, 1, "y-major, positive x and negative y directions"},
                       {0, 0, 1, "x-major, positive x and negative y directions"}};

static constexpr const char* kROPs[] = {"0 BLACKNESS",
                                        "DPSoon",
                                        "DPSona",
                                        "PSon",
                                        "SDPona",
                                        "DPon",
                                        "PDSxnon",
                                        "PDSaon",
                                        "SDPnaa",
                                        "PDSxon",
                                        "DPna",
                                        "PSDnaon",
                                        "SPna",
                                        "PDSnaon",
                                        "PDSonon",
                                        "Pn",
                                        "PDSona",
                                        "DSon NOTSRCERASE",
                                        "SDPxnon",
                                        "SDPaon",
                                        "DPSxnon",
                                        "DPSaon",
                                        "PSDPSanaxx",
                                        "SSPxDSxaxn",
                                        "SPxPDxa",
                                        "SDPSanaxn",
                                        "PDSPaox",
                                        "SDPSxaxn",
                                        "PSDPaox",
                                        "DSPDxaxn",
                                        "PDSox",
                                        "PDSoan",
                                        "DPSnaa",
                                        "SDPxon",
                                        "DSna",
                                        "SPDnaon",
                                        "SPxDSxa",
                                        "PDSPanaxn",
                                        "SDPSaox",
                                        "SDPSxnox",
                                        "DPSxa",
                                        "PSDPSaoxxn",
                                        "DPSana",
                                        "SSPxPDxaxn",
                                        "SPDSoax",
                                        "PSDnox",
                                        "PSDPxox",
                                        "PSDnoan",
                                        "PSna",
                                        "SDPnaon",
                                        "SDPSoox",
                                        "Sn NOTSRCCOPY",
                                        "SPDSaox",
                                        "SPDSxnox",
                                        "SDPox",
                                        "SDPoan",
                                        "PSDPoax",
                                        "SPDnox",
                                        "SPDSxox",
                                        "SPDnoan",
                                        "PSx",
                                        "SPDSonox",
                                        "SPDSnaox",
                                        "PSan",
                                        "PSDnaa",
                                        "DPSxon",
                                        "SDxPDxa",
                                        "SPDSanaxn",
                                        "SDna SRCERASE",
                                        "DPSnaon",
                                        "DSPDaox",
                                        "PSDPxaxn",
                                        "SDPxa",
                                        "PDSPDaoxxn",
                                        "DPSDoax",
                                        "PDSnox",
                                        "SDPana",
                                        "SSPxDSxoxn",
                                        "PDSPxox",
                                        "PDSnoan",
                                        "PDna",
                                        "DSPnaon",
                                        "DPSDaox",
                                        "SPDSxaxn",
                                        "DPSonon",
                                        "Dn DSTINVERT",
                                        "DPSox",
                                        "DPSoan",
                                        "PDSPoax",
                                        "DPSnox",
                                        "DPx PATINVERT",
                                        "DPSDonox",
                                        "DPSDxox",
                                        "DPSnoan",
                                        "DPSDnaox",
                                        "DPan",
                                        "PDSxa",
                                        "DSPDSaoxxn",
                                        "DSPDoax",
                                        "SDPnox",
                                        "SDPSoax",
                                        "DSPnox",
                                        "DSx SRCINVERT",
                                        "SDPSonox",
                                        "DSPDSonoxxn",
                                        "PDSxxn",
                                        "DPSax",
                                        "PSDPSoaxxn",
                                        "SDPax",
                                        "PDSPDoaxxn",
                                        "SDPSnoax",
                                        "PDSxnan",
                                        "PDSana",
                                        "SSDxPDxaxn",
                                        "SDPSxox",
                                        "SDPnoan",
                                        "DSPDxox",
                                        "DSPnoan",
                                        "SDPSnaox",
                                        "DSan",
                                        "PDSax",
                                        "DSPDSoaxxn",
                                        "DPSDnoax",
                                        "SDPxnan",
                                        "SPDSnoax",
                                        "DPSxnan",
                                        "SPxDSxo",
                                        "DPSaan",
                                        "DPSaa",
                                        "SPxDSxon",
                                        "DPSxna",
                                        "SPDSnoaxn",
                                        "SDPxna",
                                        "PDSPnoaxn",
                                        "DSPDSoaxx",
                                        "PDSaxn",
                                        "DSa SRCAND",
                                        "SDPSnaoxn",
                                        "DSPnoa",
                                        "DSPDxoxn",
                                        "SDPnoa",
                                        "SDPSxoxn",
                                        "SSDxPDxax",
                                        "PDSanan",
                                        "PDSxna",
                                        "SDPSnoaxn",
                                        "DPSDPoaxx",
                                        "SPDaxn",
                                        "PSDPSoaxx",
                                        "DPSaxn",
                                        "DPSxx",
                                        "PSDPSonoxx",
                                        "SDPSonoxn",
                                        "DSxn",
                                        "DPSnax",
                                        "SDPSoaxn",
                                        "SPDnax",
                                        "DSPDoaxn",
                                        "DSPDSaoxx",
                                        "PDSxan",
                                        "DPa",
                                        "PDSPnaoxn",
                                        "DPSnoa",
                                        "DPSDxoxn",
                                        "PDSPonoxn",
                                        "PDxn",
                                        "DSPnax",
                                        "PDSPoaxn",
                                        "DPSoa",
                                        "DPSoxn",
                                        "D",
                                        "DPSono",
                                        "SPDSxax",
                                        "DPSDaoxn",
                                        "DSPnao",
                                        "DPno",
                                        "PDSnoa",
                                        "PDSPxoxn",
                                        "SSPxDSxox",
                                        "SDPanan",
                                        "PSDnax",
                                        "DPSDoaxn",
                                        "DPSDPaoxx",
                                        "SDPxan",
                                        "PSDPxax",
                                        "DSPDaoxn",
                                        "DPSnao",
                                        "DSno MERGEPAINT",
                                        "SPDSanax",
                                        "SDxPDxan",
                                        "DPSxo",
                                        "DPSano",
                                        "PSa MERGECOPY",
                                        "SPDSnaoxn",
                                        "SPDSonoxn",
                                        "PSxn",
                                        "SPDnoa",
                                        "SPDSxoxn",
                                        "SDPnax",
                                        "PSDPoaxn",
                                        "SDPoa",
                                        "SPDoxn",
                                        "DPSDxax",
                                        "SPDSaoxn",
                                        "S SRCCOPY",
                                        "SDPono",
                                        "SDPnao",
                                        "SPno",
                                        "PSDnoa",
                                        "PSDPxoxn",
                                        "PDSnax",
                                        "SPDSoaxn",
                                        "SSPxPDxax",
                                        "DPSanan",
                                        "PSDPSaoxx",
                                        "DPSxan",
                                        "PDSPxax",
                                        "SDPSaoxn",
                                        "DPSDanax",
                                        "SPxDSxan",
                                        "SPDnao",
                                        "SDno",
                                        "SDPxo",
                                        "SDPano",
                                        "PDSoa",
                                        "PDSoxn",
                                        "DSPDxax",
                                        "PSDPaoxn",
                                        "SDPSxax",
                                        "PDSPaoxn",
                                        "SDPSanax",
                                        "SPxPDxan",
                                        "SSPxDSxax",
                                        "DSPDSanaxxn",
                                        "DPSao",
                                        "DPSxno",
                                        "SDPao",
                                        "SDPxno",
                                        "DSo SRCPAINT",
                                        "SDPnoo",
                                        "P PATCOPY",
                                        "PDSono",
                                        "PDSnao",
                                        "PSno",
                                        "PSDnao",
                                        "PDno",
                                        "PDSxo",
                                        "PDSano",
                                        "PDSao",
                                        "PDSxno",
                                        "DPo",
                                        "DPSnoo PATPAINT",
                                        "PSo",
                                        "PSDnoo",
                                        "DPSoo",
                                        "1 WHITENESS"};

uint32 EmRegsMediaQ11xx::GetBPP(void) {
    return 1 << ((READ_REGISTER(gcREG[GC_CONTROL]) & 0x00000070) >> 4);
}

uint32 EmRegsMediaQ11xx::GetWidth(void) { return (READ_REGISTER(gcREG[GC_HWINDOW]) >> 16) + 1; }

uint32 EmRegsMediaQ11xx::GetHeight(void) { return (READ_REGISTER(gcREG[GC_VWINDOW]) >> 16) + 1; }

int16 EmRegsMediaQ11xx::GetPitch(void) { return READ_REGISTER(gcREG[GC_STRIDE]) & 0x0000FFFF; }

bool EmRegsMediaQ11xx::FlipX() { return READ_REGISTER(gcREG[0x00]) & (1 << 12); }

bool EmRegsMediaQ11xx::SwapXY() { return READ_REGISTER(gcREG[0x00]) & (1 << 13); }

Bool EmRegsMediaQ11xx::GetXDoubling(void) {
    return (READ_REGISTER(gcREG[GC_CONTROL]) & GC_H_PIX_DBLNG) != 0;
}

Bool EmRegsMediaQ11xx::GetYDoubling(void) {
    return (READ_REGISTER(gcREG[GC_CONTROL]) & GC_V_PIX_DBLNG) != 0;
}

emuptr EmRegsMediaQ11xx::GetFrameBuffer(void) {
    return this->PrvGetVideoBase() + this->PrvGetVideoOffset();
}

// Inline functions.

inline uint32 EmRegsMediaQ11xx::PrvGetVideoOffset(void) {
    return READ_REGISTER(gcREG[GC_START_ADDR]) & 0x0003FFFF;
}

inline emuptr EmRegsMediaQ11xx::PrvGetVideoBase(void) { return fBaseVideoAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::EmRegsMediaQ11xx
// ---------------------------------------------------------------------------

EmRegsMediaQ11xx::EmRegsMediaQ11xx(EmRegsFrameBuffer& framebuffer, emuptr baseRegsAddr,
                                   emuptr baseVideoAddr)
    : fRegs(),
      fBaseRegsAddr(baseRegsAddr),
      fBaseVideoAddr(baseVideoAddr),
      /*	fState (), */
      /*	fByteLanes (), */
      fLastAddress(EmMemNULL),
      fLastSize(0),
      fBlitInProgress(false),
      fCurXOffset(0),
      fCurYOffset(0),
      fUsesPattern(false),
      fUsesSource(false),
      fLeadingSourcePixels(0),
      fTrailingSourcePixels(0),
      /*	fPatternPipe (), */
      fXPattern(0),
      fYPattern(0),
      /*	fSourcePipe (), */
      fSourcePipeIndex(0),
      fSourcePipeMax(0),
      fSourcePipeSkip(0),
      fXSrc(0),
      fYSrc(0),
      fXDest(0),
      fYDest(0),
      framebuffer(framebuffer) {
#if LOG_LINE || LOG_BLIT
    LogGetStdLog()->SetLogSize(32 * 1024L * 1024L);
#endif

    COMPILE_TIME_ASSERT(countof(kCommands) == 8);
    COMPILE_TIME_ASSERT(countof(kDirections) == 2);
    COMPILE_TIME_ASSERT(countof(kQuadrantDecode) == 8);
    COMPILE_TIME_ASSERT(countof(kROPs) == 256);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::~EmRegsMediaQ11xx
// ---------------------------------------------------------------------------

EmRegsMediaQ11xx::~EmRegsMediaQ11xx(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::Initialize
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::Initialize(void) {
    EmRegs::Initialize();

    paletteDirty = true;
    WRITE_REGISTER(dcREG[0x03], 0x00000040);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::Reset
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::Reset(Bool hardwareReset) {
    EmRegs::Reset(hardwareReset);

    if (hardwareReset) {
        WRITE_REGISTER(ccREG[0x00], 0x00000000);
        WRITE_REGISTER(ccREG[0x01], 0x00000000);
        WRITE_REGISTER(ccREG[0x02], 0x00000000);
        WRITE_REGISTER(ccREG[0x03], 0x00000000);
        WRITE_REGISTER(ccREG[0x04], 0x00000000);

        WRITE_REGISTER(mmREG[0x00], 0x00000000);  // (Low nybble only)
        WRITE_REGISTER(mmREG[0x01], 0x00000000);  // (Low nybble only)
        WRITE_REGISTER(mmREG[0x02], 0x00000000);  // (Low nybble only)

        WRITE_REGISTER(inREG[0x00], 0x00000000);
        WRITE_REGISTER(inREG[0x01], 0x00000000);
        WRITE_REGISTER(inREG[0x02], 0x00000000);
        WRITE_REGISTER(inREG[0x03], 0x00000000);

        WRITE_REGISTER(gcREG[0x00], 0x00000000);
        WRITE_REGISTER(gcREG[0x01], 0x00000000);  // (Low byte only)
        WRITE_REGISTER(gcREG[0x06], 0x00000000);  // (Low word only)
        WRITE_REGISTER(gcREG[0x07], 0x00000000);  // (Low word only)

        // GC_CONTROL              (0x00)	// Graphics Controll  Control Reg
        // GC_PWR_SEQ_CONTROL      (0x01)  // Power Sequencing Control register
        // GC_HTOTAL_HEND          (0x02)  // Horizontal Display  Total & End
        // GC_VTOTAL_VEND          (0x03)  // Vertical Display Total and End
        // GC_HSYNC                (0x04)  // Horizontal Sync Start and End
        // GC_VSYNC                (0x05)  // Vertical Sync Start and End
        // GC_HWINDOW              (0x08)  // Horizontal Window Start & End
        // GC_VWINDOW              (0x09)  // Vertical Window Start & End
        // GC_LINE_CLOCK           (0x0B)  // Line Clock Control
        // GC_START_ADDR           (0x0C)  // Image Window  Start Address
        // GC_ALTWIN_START_ADDR    (0x0D)  // Alt Image Window  Start Address
        // GC_STRIDE               (0x0E)  // (Alt) Image Window  Stride
        //	GC_FRAME_CLOCK          (0x1A)  // Frame Clock Control

        // For now, let's initialize all the graphics controller registers to 0.
        for (int i = 0x00; i < 0x20; i++) {
            WRITE_REGISTER(gcREG[i], 0x00000000);
        }

        WRITE_REGISTER(geREG[0x00], 0x00000000);
        WRITE_REGISTER(geREG[0x0C], 0x00000000);

        WRITE_REGISTER(ssREG[0x00], 0x00000000);
        WRITE_REGISTER(ssREG[0x01], 0x00000000);
        WRITE_REGISTER(ssREG[0x04], 0x00000000);  // (Low nybble only)
        WRITE_REGISTER(ssREG[0x08], 0x00000000);  // (Low byte only)
        WRITE_REGISTER(ssREG[0x09], 0x00000000);  // (Low byte only)
        WRITE_REGISTER(ssREG[0x0A], 0x00000000);  // (Low byte only)
        WRITE_REGISTER(ssREG[0x0B], 0x00000000);  // (Low byte only)
        WRITE_REGISTER(ssREG[0x10], 0x00000000);
        WRITE_REGISTER(ssREG[0x11], 0x00000000);
        WRITE_REGISTER(ssREG[0x13], 0x00000000);

        WRITE_REGISTER(spREG[0x00], 0x00000000);
        WRITE_REGISTER(spREG[0x01], 0xF0000000);
        WRITE_REGISTER(spREG[0x02], 0x00000000);
        WRITE_REGISTER(spREG[0x03], 0x00000000);
        WRITE_REGISTER(spREG[0x04], 0x00000000);
        WRITE_REGISTER(spREG[0x05], 0x00000000);
        WRITE_REGISTER(spREG[0x06], 0x00000000);
        WRITE_REGISTER(spREG[0x07], 0x00000000);
        WRITE_REGISTER(spREG[0x08], 0x00000000);

        WRITE_REGISTER(dcREG[0x00], 0x00000000);
        WRITE_REGISTER(dcREG[0x01], 0xF0000000);
        WRITE_REGISTER(dcREG[0x02], 0x00000000);
        WRITE_REGISTER(dcREG[0x03], 0x00000040);
        WRITE_REGISTER(dcREG[0x04], 0x00000000);
        WRITE_REGISTER(dcREG[0x05], 0x00000000);
        WRITE_REGISTER(dcREG[0x06], 0x00000000);
        WRITE_REGISTER(dcREG[0x07], 0x00000000);

        WRITE_REGISTER(pcREG[0x00], 0x01204D51);
        WRITE_REGISTER(pcREG[0x01], 0x02800000);
        WRITE_REGISTER(pcREG[0x02], 0x03800000);
        WRITE_REGISTER(pcREG[0x03], 0x00000000);
        WRITE_REGISTER(pcREG[0x0B], 0x00000100);
        WRITE_REGISTER(pcREG[0x0F], 0x00000100);

        WRITE_REGISTER(uhREG[0x00], 0x00000010);  // (Low byte only)
        WRITE_REGISTER(uhREG[0x01], 0x00000000);
        WRITE_REGISTER(uhREG[0x02], 0x00000000);
        WRITE_REGISTER(uhREG[0x03], 0x00000000);
        WRITE_REGISTER(uhREG[0x04], 0x00000000);
        WRITE_REGISTER(uhREG[0x05], 0x00000000);
        WRITE_REGISTER(uhREG[0x06], 0x00000000);
        WRITE_REGISTER(uhREG[0x07], 0x00000000);
        WRITE_REGISTER(uhREG[0x08], 0x00000000);
        WRITE_REGISTER(uhREG[0x09], 0x00000000);
        WRITE_REGISTER(uhREG[0x0A], 0x00000000);
        WRITE_REGISTER(uhREG[0x0B], 0x00000000);
        WRITE_REGISTER(uhREG[0x0C], 0x00000000);
        WRITE_REGISTER(uhREG[0x0D], 0x00002EDF);
        WRITE_REGISTER(uhREG[0x0E], 0x00000000);
        WRITE_REGISTER(uhREG[0x0F], 0x00000000);
        WRITE_REGISTER(uhREG[0x10], 0x00000000);  // (Low word only)
        WRITE_REGISTER(uhREG[0x11], 0x00000628);
        WRITE_REGISTER(uhREG[0x12], 0x00001102);
        WRITE_REGISTER(uhREG[0x13], 0x00000000);
        WRITE_REGISTER(uhREG[0x14], 0x00000000);
        WRITE_REGISTER(uhREG[0x15], 0x00000000);
        WRITE_REGISTER(uhREG[0x16], 0x00000000);

        WRITE_REGISTER(fpREG[0x00], 0x00000000);
        WRITE_REGISTER(fpREG[0x01], 0x00000000);
        WRITE_REGISTER(fpREG[0x02], 0x0FFCFCFF);
        WRITE_REGISTER(fpREG[0x03], 0x00000000);
        WRITE_REGISTER(fpREG[0x04], 0x00000000);
        WRITE_REGISTER(fpREG[0x05], 0x00000000);
        WRITE_REGISTER(fpREG[0x06], 0x00000000);
        WRITE_REGISTER(fpREG[0x07], 0x00000000);
        WRITE_REGISTER(fpREG[0x0A], 0xF0030000);
        WRITE_REGISTER(fpREG[0x0B], 0x00000000);  // (Low 12-bits only)
        WRITE_REGISTER(fpREG[0x0E], 0x00000000);  // (Low byte only)
        WRITE_REGISTER(fpREG[0x0F], 0x00000000);

        // (No reset values for color palette and source FIFO)

        WRITE_REGISTER(udREG[0x00], 0x00000000);
        WRITE_REGISTER(udREG[0x01], 0x00000000);
        WRITE_REGISTER(udREG[0x02], 0x00000000);
        WRITE_REGISTER(udREG[0x03], 0x00000000);
        WRITE_REGISTER(udREG[0x04], 0x00000000);
        WRITE_REGISTER(udREG[0x05], 0x00000000);
        WRITE_REGISTER(udREG[0x06], 0x00000000);
        WRITE_REGISTER(udREG[0x07], 0x00000000);
        WRITE_REGISTER(udREG[0x08], 0x00000000);
        WRITE_REGISTER(udREG[0x09], 0x00000000);
        WRITE_REGISTER(udREG[0x0A], 0x00000000);
        WRITE_REGISTER(udREG[0x0B], 0x00000000);  // (Low byte only)
        WRITE_REGISTER(udREG[0x0C], 0x00000000);
        WRITE_REGISTER(udREG[0x0D], 0x00000000);
        WRITE_REGISTER(udREG[0x0E], 0x00000000);
        WRITE_REGISTER(udREG[0x0F], 0x00000000);
        WRITE_REGISTER(udREG[0x00], 0x00000000);
        WRITE_REGISTER(udREG[0x01], 0x00000000);
        WRITE_REGISTER(udREG[0x02], 0x00000000);

        // Because DC00R is reset to zero, establish the
        // byteswapping lanes corresponding to that.

        this->PrvUpdateByteLanes();

        fLastAddress = EmMemNULL;
        fLastSize = 0;

        fBlitInProgress = false;
        paletteDirty = true;

        fSourceFifo.Clear();

        this->PrvGetGEState(kAllRegisters);
    }
}

void EmRegsMediaQ11xx::Save(Savestate& savestate) { DoSave(savestate); }

void EmRegsMediaQ11xx::Save(SavestateProbe& savestate) { DoSave(savestate); }

void EmRegsMediaQ11xx::Load(SavestateLoader& loader) {
    Chunk* chunk = loader.GetChunk(ChunkType::regsMQ1xx);
    if (!chunk) {
        logging::printf("unable to restore RegsMediaQ11xx: missing savestate\n");
        loader.NotifyError();

        return;
    }

    const uint32 version = chunk->Get32();
    if (version > SAVESTATE_VERSION) {
        logging::printf("unable to restore RegsMediaQ11xx: unsupported savestate version\n");
        loader.NotifyError();

        return;
    }

    LoadChunkHelper helper(*chunk);
    DoSaveLoad(helper);

    WRITE_REGISTER(dcREG[0x03], 0x00000040);

    this->PrvUpdateByteLanes();
    this->PrvGetGEState(kAllRegisters);

    paletteDirty = true;
}

template <typename T>
void EmRegsMediaQ11xx::DoSave(T& savestate) {
    typename T::chunkT* chunk = savestate.GetChunk(ChunkType::regsMQ1xx);
    if (!chunk) return;

    chunk->Put32(SAVESTATE_VERSION);

    SaveChunkHelper helper(*chunk);
    DoSaveLoad(helper);
}

template <typename T>
void EmRegsMediaQ11xx::DoSaveLoad(T& helper) {
    helper.DoBuffer(fRegs.GetPtr(), fRegs.GetSize())
        .Do32(fLastAddress)
        .Do32(fLastSize)
        .Do(typename T::Pack16() << fCurXOffset << fCurYOffset)
        .Do(typename T::Pack16() << fLeadingSourcePixels << fTrailingSourcePixels)
        .DoBuffer(fPatternPipe, 64 * sizeof(uint16))
        .Do(typename T::Pack16() << fXPattern << fYPattern)
        .DoBuffer(fSourcePipe, 64 * sizeof(uint16))
        .Do(typename T::Pack16() << fSourcePipeIndex << fSourcePipeMax)
        .Do16(fSourcePipeSkip)
        .Do(typename T::Pack16() << fXSrc << fYSrc)
        .Do(typename T::Pack16() << fXDest << fYDest)
        .Do(typename T::BoolPack() << fBlitInProgress << fUsesPattern << fUsesSource);

    fSourceFifo.DoSaveLoad(helper);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::Dispose
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::Dispose(void) { EmRegs::Dispose(); }

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::SetSubBankHandlers
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::SetSubBankHandlers(void) {
    // Install base handlers.

    EmRegs::SetSubBankHandlers();

    // Now add standard/specialized handers for the defined registers.

    INSTALL_HANDLER(MQRead, MQWrite, ccREG[0x00]);
    INSTALL_HANDLER(CC01Read, MQWrite, ccREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, ccREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, ccREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, ccREG[0x04]);

    INSTALL_HANDLER(MQRead, MQWrite, mmREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, mmREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, mmREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, mmREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, mmREG[0x04]);

    INSTALL_HANDLER(MQRead, MQWrite, inREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, inREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, inREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, inREG[0x03]);

    INSTALL_HANDLER(MQRead, invalidateWrite, gcREG[0x00]);  // BPP (et al)
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x04]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x05]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x06]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x07]);
    INSTALL_HANDLER(MQRead, invalidateWrite, gcREG[0x08]);  // Width
    INSTALL_HANDLER(MQRead, invalidateWrite, gcREG[0x09]);  // Height
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x0A]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x0B]);
    INSTALL_HANDLER(MQRead, GC0CWrite, gcREG[0x0C]);  // Base Address
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x0D]);
    INSTALL_HANDLER(MQRead, invalidateWrite, gcREG[0x0E]);  // Stride
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x0F]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x10]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x11]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x12]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x13]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x14]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x15]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x16]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x17]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x18]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x19]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x1A]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x1B]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x1C]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x1D]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x1E]);
    INSTALL_HANDLER(MQRead, MQWrite, gcREG[0x1F]);

    INSTALL_HANDLER(MQRead, GE00Write, geREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x01]);
    INSTALL_HANDLER(MQRead, GE02Write, geREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x04]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x05]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x06]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x07]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x08]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x09]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x0A]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x0B]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x0C]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x0D]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x0E]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x0F]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x10]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x11]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x12]);
    INSTALL_HANDLER(MQRead, MQWrite, geREG[0x13]);

    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x04]);  // Skip some here
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x08]);  // Skip some here
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x09]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x0A]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x0B]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x0C]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x0D]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x10]);  // Skip some here
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x11]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x12]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x13]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x14]);
    INSTALL_HANDLER(MQRead, MQWrite, ssREG[0x15]);

    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x04]);
    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x05]);
    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x06]);
    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x07]);
    INSTALL_HANDLER(MQRead, MQWrite, spREG[0x08]);

    INSTALL_HANDLER(MQRead, DC00Write, dcREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, dcREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, dcREG[0x02]);
    INSTALL_HANDLER(MQRead, NullWrite, dcREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, dcREG[0x04]);
    INSTALL_HANDLER(MQRead, MQWrite, dcREG[0x05]);
    INSTALL_HANDLER(MQRead, MQWrite, dcREG[0x06]);
    INSTALL_HANDLER(MQRead, MQWrite, dcREG[0x07]);

    INSTALL_HANDLER(MQRead, MQWrite, pcREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, pcREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, pcREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, pcREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, pcREG[0x04]);
    INSTALL_HANDLER(MQRead, MQWrite, pcREG[0x05]);
    INSTALL_HANDLER(MQRead, MQWrite, pcREG[0x0B]);  // Skip some here
    INSTALL_HANDLER(MQRead, MQWrite, pcREG[0x0F]);  // Skip some here

    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x04]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x05]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x06]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x07]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x08]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x09]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x0A]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x0B]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x0C]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x0D]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x0E]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x0F]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x10]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x11]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x12]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x13]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x14]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x15]);
    INSTALL_HANDLER(MQRead, MQWrite, uhREG[0x16]);

    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x04]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x05]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x06]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x07]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x08]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x09]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x0A]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x0B]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x0E]);  // Skip some here
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x0F]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x10]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x11]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x12]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x13]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x14]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x15]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x16]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x17]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x18]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x19]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x1A]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x1B]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x1C]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x1D]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x1E]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x1F]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x20]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x21]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x22]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x23]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x24]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x25]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x26]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x27]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x28]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x29]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x2A]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x2B]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x2C]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x2D]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x2E]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x2F]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x30]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x31]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x32]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x33]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x34]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x35]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x36]);
    INSTALL_HANDLER(MQRead, MQWrite, fpREG[0x37]);

    this->SetHandler((ReadFunction)&EmRegsMediaQ11xx::MQRead,
                     (WriteFunction)&EmRegsMediaQ11xx::CPWrite, mq_addressof(cpREG), 0x0400);

    this->SetHandler((ReadFunction)&EmRegsMediaQ11xx::MQRead,
                     (WriteFunction)&EmRegsMediaQ11xx::SourceFifoWrite, mq_addressof(sfREG),
                     0x0400);

    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x00]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x01]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x02]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x03]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x04]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x05]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x06]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x07]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x08]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x09]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x0A]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x0B]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x0C]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x0D]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x0E]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x0F]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x10]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x11]);
    INSTALL_HANDLER(MQRead, MQWrite, udREG[0x12]);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::GetRealAddress
// ---------------------------------------------------------------------------

uint8* EmRegsMediaQ11xx::GetRealAddress(emuptr address) {
    return (uint8*)fRegs.GetPtr() + (address - this->GetAddressStart());
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::GetAddressStart
// ---------------------------------------------------------------------------

emuptr EmRegsMediaQ11xx::GetAddressStart(void) { return fBaseRegsAddr; }

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::GetAddressRange
// ---------------------------------------------------------------------------

uint32 EmRegsMediaQ11xx::GetAddressRange(void) { return TOTAL_ADDRESS_SPACE_SIZE; }

uint32 EmRegsMediaQ11xx::GetLong(emuptr address) {
    return (address < fBaseRegsAddr + REGISTER_FILE_SIZE) ? EmRegs::GetLong(address) : 0;
}

uint32 EmRegsMediaQ11xx::GetWord(emuptr address) {
    return (address < fBaseRegsAddr + REGISTER_FILE_SIZE) ? EmRegs::GetWord(address) : 0;
}

uint32 EmRegsMediaQ11xx::GetByte(emuptr address) {
    return (address < fBaseRegsAddr + REGISTER_FILE_SIZE) ? EmRegs::GetByte(address) : 0;
}

void EmRegsMediaQ11xx::SetLong(emuptr address, uint32 value) {
    if (address < fBaseRegsAddr + REGISTER_FILE_SIZE) {
        EmRegs::SetLong(address, value);
    }
}

void EmRegsMediaQ11xx::SetWord(emuptr address, uint32 value) {
    if (address < fBaseRegsAddr + REGISTER_FILE_SIZE) {
        EmRegs::SetWord(address, value);
    }
}

void EmRegsMediaQ11xx::SetByte(emuptr address, uint32 value) {
    if (address < fBaseRegsAddr + REGISTER_FILE_SIZE) {
        EmRegs::SetByte(address, value);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::GetLCDScreenOn
// ---------------------------------------------------------------------------

Bool EmRegsMediaQ11xx::GetLCDScreenOn(void) {
    return ((READ_REGISTER(gcREG[GC_CONTROL]) & GC_ENABLE) != 0);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::GetLCDBacklightOn
// ---------------------------------------------------------------------------

Bool EmRegsMediaQ11xx::GetLCDBacklightOn(void) { return true; }

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::GetLCDHasFrame
// ---------------------------------------------------------------------------

Bool EmRegsMediaQ11xx::GetLCDHasFrame(void) { return true; }

uint16 EmRegsMediaQ11xx::GetLCD2bitMapping() { return 0xfa50; }

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::MQRead
// ---------------------------------------------------------------------------

uint32 EmRegsMediaQ11xx::MQRead(emuptr address, int size) {
    // Determine the offset from the beginning of the register memory range
    // to the location we want to read.

    uint32 offset = address - this->GetAddressStart();

    EmAssert(offset < this->GetAddressRange());

    // Determine the index for the register being read (treating the entire
    // memory range as an array of 32-bit values), and determine the delta
    // into the register.

    int index = offset / 4;
    int delta = offset - (index * 4);

    EmAssert(delta >= 0);
    EmAssert(delta < 4);

    // Get the pointer to the register to read from.

    UInt32 value;
    UInt8* p = (UInt8*)fRegs.ccREG[index].GetPtr();

    if (size == 4) {
        // Make sure we're reading the whole register.

        if ((delta & 3) != 0) this->AddressError(address, size, true);

        // Get a pointer to the "bytelanes" array that will sort out
        // any 32-bit or 16-bit byteswapping for us.

        int* bytelanes = &this->fBytelanes[delta];

        // Read the value a byte at a time, reconstructing it according
        // to the byteswapping rules.

        value = (((UInt32)p[bytelanes[0]]) << 24) | (((UInt32)p[bytelanes[1]]) << 16) |
                (((UInt32)p[bytelanes[2]]) << 8) | (((UInt32)p[bytelanes[3]]) << 0);
    } else if (size == 2) {
        // Make sure we're reading from an even address.
        // !!! This is really a characteristic of the host
        // CPU and bus, not the MediaQ chip.

        if ((delta & 1) != 0) this->AddressError(address, size, true);

        // Get a pointer to the "bytelanes" array that will sort out
        // any 32-bit or 16-bit byteswapping for us.

        int* bytelanes = &this->fBytelanes[delta];

        // Read the value a byte at a time, reconstructing it according
        // to the byteswapping rules.

        value = (((UInt32)p[bytelanes[0]]) << 8) | (((UInt32)p[bytelanes[1]]) << 0);
    } else if (size == 1) {
        // Get a pointer to the "bytelanes" array that will sort out
        // any 32-bit or 16-bit byteswapping for us.

        int* bytelanes = &this->fBytelanes[delta];

        // Read the value a byte at a time, reconstructing it according
        // to the byteswapping rules.

        value = (((UInt32)p[bytelanes[0]]) << 0);
    } else {
        EmAssert(false);
        value = 0;
    }

    return value;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::MQWrite
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::MQWrite(emuptr address, int size, uint32 value) {
    // Determine the offset from the beginning of the register memory range
    // to the location we want to update.

    uint32 offset = address - this->GetAddressStart();

    EmAssert(offset < this->GetAddressRange());

    // Determine the index for the register being updated (treating the entire
    // memory range as an array of 32-bit values), and determine the delta
    // into the register.

    int index = offset / 4;
    int delta = offset - (index * 4);

    EmAssert(delta >= 0);
    EmAssert(delta < 4);

    // Get the pointer to the register to update.

    UInt8* p = (UInt8*)fRegs.ccREG[index].GetPtr();

    if (size == 4) {
        // Make sure we're updating the whole register.

        if ((delta & 3) != 0) this->AddressError(address, size, false);

        // Get a pointer to the "bytelanes" array that will sort out
        // any 32-bit or 16-bit byteswapping for us.

        int* bytelanes = &this->fBytelanes[delta];

        // Write the value a byte at a time, using the bytelanes array
        // to sort out the byteswapping for us.

        p[bytelanes[0]] = value >> 24;
        p[bytelanes[1]] = value >> 16;
        p[bytelanes[2]] = value >> 8;
        p[bytelanes[3]] = value >> 0;
    } else if (size == 2) {
        // Make sure we're updating to an even address.
        // !!! This is really a characteristic of the host
        // CPU and bus, not the MediaQ chip.

        if ((delta & 1) != 0) this->AddressError(address, size, false);

        // Get a pointer to the "bytelanes" array that will sort out
        // any 32-bit or 16-bit byteswapping for us.

        int* bytelanes = &this->fBytelanes[delta];

        // Write the value a byte at a time, using the bytelanes array
        // to sort out the byteswapping for us.

        p[bytelanes[0]] = value >> 8;
        p[bytelanes[1]] = value >> 0;
    } else if (size == 1) {
        // Get a pointer to the "bytelanes" array that will sort out
        // any 32-bit or 16-bit byteswapping for us.

        int* bytelanes = &this->fBytelanes[delta];

        // Write the value a byte at a time, using the bytelanes array
        // to sort out the byteswapping for us.

        p[bytelanes[0]] = value >> 0;
    } else {
        EmAssert(false);
    }

    fLastAddress = address;
    fLastSize = size;

    if (index >= 0x080 && index <= 0x093) {
        this->PrvGetGEState(index - 0x080);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::CC01Read
// ---------------------------------------------------------------------------

uint32 EmRegsMediaQ11xx::CC01Read(emuptr address, int size) {
    // Update the Command FIFO size.
    // Update the Source FIFO size.
    // Also make sure the busy bit is clear.

    uint32 CC01 = READ_REGISTER(ccREG[0x01]);

    CC01 = (CC01 & ~CMD_FIFO_MASK) | (CMD_FIFO_EMPTY & CMD_FIFO_MASK);
    CC01 = (CC01 & ~CMD_SRC_MASK) | (CMD_SRC_EMPTY & CMD_SRC_MASK);
    CC01 = CC01 & ~GE_BUSY_H;

    WRITE_REGISTER(ccREG[0x01], CC01);

    // Perform a standard read.

    uint32 result = MQRead(address, size);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::DC00Write
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::DC00Write(emuptr address, int size, uint32 value) {
    // Perform a standard update.

    this->MQWrite(address, size, value);

    this->PrvUpdateByteLanes();
}

void EmRegsMediaQ11xx::GC0CWrite(emuptr address, int size, uint32 value) {
    MQWrite(address, size, value);

    WRITE_REGISTER(gcREG[0x0c], READ_REGISTER(gcREG[0x0c]) & 0x0003ffff);

    gSystemState.MarkScreenDirty();
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::GE00Write
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::GE00Write(emuptr address, int size, uint32 value) {
    emuptr lastAddress = this->fLastAddress;
    int lastSize = this->fLastSize;

    // Perform a standard update.

    this->MQWrite(address, size, value);

    // If a command was issued, execute it (if autoExecute is false).

    uint32 reg = READ_REGISTER(geREG[0]);

    if ((reg & (1 << 27)) == 0) {
        if (size == 4) {
            this->PrvDoCommand();
        } else if (size == 2 && lastSize == 2 && address == (lastAddress + 2)) {
            this->PrvDoCommand();
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::GE02Write
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::GE02Write(emuptr address, int size, uint32 value) {
    emuptr lastAddress = this->fLastAddress;
    int lastSize = this->fLastSize;

    // Perform a standard update.

    this->MQWrite(address, size, value);

    // If a command was issued, execute it (if autoExecute is true).

    uint32 reg = READ_REGISTER(geREG[0]);

    if ((reg & (1 << 27)) != 0) {
        if (size == 4) {
            this->PrvDoCommand();
        } else if (size == 2 && lastSize == 2 && address == (lastAddress + 2)) {
            this->PrvDoCommand();
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::SourceFifoWrite
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::SourceFifoWrite(emuptr address, int size, uint32 value) {
    emuptr lastAddress = this->fLastAddress;
    int lastSize = this->fLastSize;

    // Perform a standard update.

    this->MQWrite(address, size, value);

    // If a complete entry was added, push it onto the FIFO, and give the
    // blitter a chance to run.

    if (size == 4) {
        value = READ_REGISTER(sfREG[(address >> 2) & 0x0FF]);
        fSourceFifo.Push(value);

        this->PrvIncBlitterRun();
    } else if (size == 2 && lastSize == 2 && address == (lastAddress + 2)) {
        value = READ_REGISTER(sfREG[(address >> 2) & 0x0FF]);
        fSourceFifo.Push(value);

        this->PrvIncBlitterRun();
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::invalidateWrite
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::invalidateWrite(emuptr address, int size, uint32 value) {
    // Perform a standard update.

    this->MQWrite(address, size, value);

    // Invalidate the entire LCD area so that it can get redrawn with
    // the new palette information.

    gSystemState.MarkScreenDirty();
}

void EmRegsMediaQ11xx::CPWrite(emuptr address, int size, uint32 value) {
    this->MQWrite(address, size, value);

    gSystemState.MarkScreenDirty();
    paletteDirty = true;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::AddressError
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::AddressError(emuptr address, long size, Bool forRead) {
    EmAssert(false);
    EmAssert(gCPU68K);
    gCPU68K->AddressError(address, size, forRead);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvGetPalette
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvUpdatePalette() {
    if (!paletteDirty) return;

    for (int ii = 0; ii < 256; ++ii) {
        uint32 reg = READ_REGISTER(cpREG[ii]);

        palette[ii] = 0xff000000 | (reg & (RED_MASK | GREEN_MASK | BLUE_MASK));
    }

    paletteDirty = false;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvUpdateByteLanes
// ---------------------------------------------------------------------------
/*
        Figuring out byteswapping:

        Our source code has the following function for writing data to registers:

        void sREGW (volatile UCHAR* addr, ULONG data)
        {
                UInt16	_data16;
                UInt16*	dc00R = (UInt16*) (MMIO_BASE + DC_BASE);

                if (*dc00R & 1)	// no need to swap data (16BPP mode) within word16
                {
                        *((UInt16*) addr)		= (UInt16) data;
   // write without swap
                        *((UInt16*) (addr + 2))	= (UInt16) __rol (data, 16);	// get next word16
                }
                else
                {
                        _data16	= (UInt16) (data);
                        _data16	= __rol (_data16, 8);			// bswap for Big Endian
                        *((UInt16*) addr) = _data16;

                        _data16 = (UInt16) __rol (data, 16);	// get next word16
                        _data16 = __rol (_data16, 8);			// bswap for Big Endian
                        *((UInt16 *)(addr + 2)) = _data16;
                }
        }

        My assumption here is that we want to take a data pattern like 0x01234567
        and end up with it stored in the register as 67 45 23 01.  Working from
        the code above, if byteswapping is off, we execute the following:

                        *((UInt16*) addr)		= (UInt16) data;
   // write without swap
                        *((UInt16*) (addr + 2))	= (UInt16) __rol (data, 16);	// get next word16

        or:

                        *((UInt16*) addr)		= 0x4567;	// write without swap
                        *((UInt16*) (addr + 2))	= 0x0123;	// get next word16

        In order for this to result in the right bytes getting written to the
        right places in memory, 16-bit byteswapping must be on at the hardware
        level.  Confirming with the code in the "else" statement:

                        _data16	= (UInt16) (data);
                        _data16	= __rol (_data16, 8);			// bswap for Big Endian
                        *((UInt16*) addr) = _data16;

                        _data16 = (UInt16) __rol (data, 16);	// get next word16
                        _data16 = __rol (_data16, 8);			// bswap for Big Endian
                        *((UInt16 *)(addr + 2)) = _data16;

        or:

                        _data16	= 0x4567;
                        _data16	= 0x6745;						// bswap for
   Big Endian
                        *((UInt16*) addr) = 0x6745;

                        _data16 = 0x0123;						// get next
   word16 _data16 = 0x2301;						// bswap for Big Endian
                        *((UInt16 *)(addr + 2)) = 0x2301;

        In this case, if 16-bit byteswapping is turned on in the device
        configuration register, then that will counteract the byteswapping
        performed by the hardware.  If, however, 32-bit byteswapping is
        enabled in the device configuration register, I'm not sure what
        will happen.

        Let's take a look at another example.  Registers are often written
        using the geREG16 macro.  This macro is called twice in order to
        update the whole 32-bit register:

                geREG16( GE_WIDTH_L,            width );
                geREG16( GE_HEIGHT_H,           height );

        Where:

                #define GE_WIDTH_L          (GE_WIDTH_HEIGHT)           // 40204h
                #define GE_HEIGHT_H         (GE_WIDTH_HEIGHT + 2)       // 40206h

        There is also a geREG macro used to update the whole 32-bit register
        at once:

                geREG( 	 GE_DEST_XY,  	 	dstAddr );

        I find a number of definitions of these macros.  The two sets that
        appear most likely to be the ones used in our ROM are as follows:

        # mqgeHAL.h

                #define geREG16(id, data)   *(pGE16 + (id)) = (UInt16)(data)
                #define geREG(id, data)     *(pGE + (id)) = (UInt32)(data)

        # mqge.h

                #define geREG16(id, data)   *((UInt16 *)(pGE + id)) = __rol( data, 8 )
            #define geREG(id, data)								\
        {
   \
                        register UInt32	_data32;						\
                        _data32 = (data); \
                        geREG16(id,   		(UInt16)_data32);			\
                        geREG16((id + 2),	(UInt16)(_data32 >> 16) );	\
                }

        The second set appears to be more consistant with our first
        study case, so let's assume that it represents what's going
        on in our ROM.

        What's left unclear is what really happens when writing a 32-bit
        value to a 32-bit register, and how the 64-bit source data FIFO
        is handled.
*/

void EmRegsMediaQ11xx::PrvUpdateByteLanes(void) {
    // Read the register.

    uint32 reg = READ_REGISTER(dcREG[0x00]);

    // Adjust the bytelanes array to reflect the byteswapping options.
    // Without any other adjustments, it appears that the CPU bytelanes
    // are set up to swap 16-bit values with the MediaQ chip.

    this->fBytelanes[0] = 1;
    this->fBytelanes[1] = 0;
    this->fBytelanes[2] = 3;
    this->fBytelanes[3] = 2;

    if ((reg & 1) == 1) {
        // Enable little endian mode. Byte swapping is not performed on data.

    } else if ((reg & 2) == 2) {
        // Enable 32-bit byte swapping for big endian to little endian conversion.
        // byte0 <-> byte3, byte1 <-> byte2

        std::swap(this->fBytelanes[0], this->fBytelanes[3]);
        std::swap(this->fBytelanes[1], this->fBytelanes[2]);
    } else {
        // Enable 16-bit Byte swapping for big endian to little endian conversion.
        // byte0 <-> byte1, byte2 <-> byte3

        std::swap(this->fBytelanes[0], this->fBytelanes[1]);
        std::swap(this->fBytelanes[2], this->fBytelanes[3]);
    }
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvGetGEState
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvGetGEState(int regNum) {
    uint32 reg;

    // ==================== GE00R ====================

    if (regNum == kAllRegisters || regNum == 0) {
        reg = READ_REGISTER(geREG[0x00]);

        fState.rasterOperation = (reg >> 0) & 0x000000FF;
        fState.commandType = (reg >> 8) & 0x00000007;
        fState.xDirection = (reg >> 11) & 0x00000001;
        fState.yDirection = (reg >> 12) & 0x00000001;
        fState.systemMemory = (reg >> 13) & 0x00000001;
        fState.monoSource = (reg >> 14) & 0x00000001;
        fState.monoPattern = (reg >> 15) & 0x00000001;
        fState.colorTransEnable = (reg >> 16) & 0x00000001;
        fState.destTransPolarity = (reg >> 17) & 0x00000001;
        fState.monoTransEnable = (reg >> 18) & 0x00000001;
        fState.monoTransPolarity = (reg >> 19) & 0x00000001;
        fState.memToScreen = (reg >> 20) & 0x00000001;
        fState.solidSourceColor = (reg >> 23) & 0x00000001;
        fState.srcEqualDestStride = (reg >> 24) & 0x00000001;
        fState.rop2Select = (reg >> 25) & 0x00000001;
        fState.clipEnable = (reg >> 26) & 0x00000001;
        fState.autoExecute = (reg >> 27) & 0x00000001;
        fState.solidPattern = (reg >> 30) & 0x00000001;
        fState.colorTransCmpSrc = (reg >> 31) & 0x00000001;
    }

    // ==================== GE01R ====================

    if (regNum == kAllRegisters || regNum == 1) {
        reg = READ_REGISTER(geREG[0x01]);

        // BitBLT

        fState.width = (reg >> 0) & 0x00000FFF;
        fState.height = (reg >> 16) & 0x00000FFF;
        fState.xyConversion = (reg >> 31) & 0x00000001;

        // Line

        fState.gamma = (reg >> 0) & 0x0001FFFF;
        fState.gamma |= 0xFFFE0000;
        fState.majorLength = (reg >> 17) & 0x00000FFF;
        fState.yIsMajor = (reg >> 29) & 0x00000001;
        fState.drawLastPixel = (reg >> 30) & 0x00000001;
        fState.useXY = (reg >> 31) & 0x00000001;
    }

    // ==================== GE02R ====================

    if (regNum == kAllRegisters || regNum == 2) {
        reg = READ_REGISTER(geREG[0x02]);

        // BitBLT

        fState.xDest = (reg >> 0) & 0x00000FFF;
        fState.monoPatternXOffset = (reg >> 13) & 0x00000007;
        fState.yDest = (reg >> 16) & 0x00000FFF;
        fState.monoPatternYOffset = (reg >> 29) & 0x00000007;

        // Line

        fState.xStart = (reg >> 0) & 0x00000FFF;
        fState.deltaMajor = (reg >> 12) & 0x0001FFFF;
        fState.quadrant = (reg >> 29) & 0x00000007;
    }

    // ==================== GE03R ====================

    if (regNum == kAllRegisters || regNum == 3) {
        reg = READ_REGISTER(geREG[0x03]);

        // BitBLT

        fState.xSrc = (reg >> 0) & 0x00000FFF;
        fState.ySrc = (reg >> 16) & 0x00000FFF;

        // Line

        fState.yStart = (reg >> 0) & 0x00000FFF;
        fState.deltaMinor = (reg >> 12) & 0x0001FFFF;
    }

    // ==================== GE04R ====================

    if (regNum == kAllRegisters || regNum == 4) {
        reg = READ_REGISTER(geREG[0x04]);

        fState.destTransColor = (reg >> 0) & 0x0000FFFF;
    }

    // ==================== GE05R ====================

    if (regNum == kAllRegisters || regNum == 5) {
        reg = READ_REGISTER(geREG[0x05]);

        fState.clipLeft = (reg >> 0) & 0x000007FF;
        fState.clipTop = (reg >> 16) & 0x000003FF;
    }

    // ==================== GE06R ====================

    if (regNum == kAllRegisters || regNum == 6) {
        reg = READ_REGISTER(geREG[0x06]);

        fState.clipRight = (reg >> 0) & 0x000007FF;
        fState.clipBottom = (reg >> 16) & 0x000003FF;
    }

    // ==================== GE07R ====================

    if (regNum == kAllRegisters || regNum == 7) {
        reg = READ_REGISTER(geREG[0x07]);

        fState.fgColorMonoSrc = (reg >> 0) & 0x0000FFFF;
    }

    // ==================== GE08R ====================

    if (regNum == kAllRegisters || regNum == 8) {
        reg = READ_REGISTER(geREG[0x08]);

        fState.bgColorMonoSrc = (reg >> 0) & 0x0000FFFF;
    }

    // ==================== GE09R ====================

    if (regNum == kAllRegisters || regNum == 9) {
        reg = READ_REGISTER(geREG[0x09]);

        // Lined Mode

        fState.srcLineStride = (reg >> 0) & 0x000003FF;
        fState.srcBitOffset = (reg >> 25) & 0x00000007;
        fState.srcByteOffset = (reg >> 28) & 0x00000007;

        // Packed Mode

        fState.srcLeadingBits = (reg >> 0) & 0x00000007;
        fState.srcLeadingBytes = (reg >> 3) & 0x00000007;
        fState.srcNumBytes = (reg >> 6) & 0x000003FF;
        fState.srcTrailingBits = (reg >> 25) & 0x00000007;
        fState.srcTrailingBytes = (reg >> 28) & 0x0000000F;
    }

    // ==================== GE0AR ====================

    if (regNum == kAllRegisters || regNum == 10) {
        reg = READ_REGISTER(geREG[0x0A]);

        fState.destLineStride = (reg >> 0) & 0x00000FFF;
        fState.monoSrcBitSwap = (reg >> 28) & 0x00000001;
        fState.rotate90 = (reg >> 29) & 0x00000001;
        fState.colorDepth = (reg >> 30) & 0x00000003;
    }

    // ==================== GE0BR ====================

    if (regNum == kAllRegisters || regNum == 11) {
        reg = READ_REGISTER(geREG[0x0B]);

        fState.baseAddr = (reg >> 0) & 0x000FFFFF;
    }

    // ==================== GE0CR ====================

    if (regNum == kAllRegisters || regNum == 12) {
        reg = READ_REGISTER(geREG[0x0C]);

        fState.cmdLineStart = (reg >> 0) & 0x000003FF;
        fState.cmdLineEnd = (reg >> 12) & 0x000003FF;
        fState.cmdLineControl = (reg >> 24) & 0x00000001;
        fState.gc1SwitchControl = (reg >> 26) & 0x00000003;
    }

    // ==================== GE0FR ====================

    // ==================== GE10R ====================

    if (regNum == kAllRegisters || regNum == 16) {
        reg = READ_REGISTER(geREG[0x10]);

        fState.monoPattern1 = (reg >> 0) & 0xFFFFFFFF;
    }

    // ==================== GE11R ====================

    if (regNum == kAllRegisters || regNum == 17) {
        reg = READ_REGISTER(geREG[0x11]);

        fState.monoPattern2 = (reg >> 0) & 0xFFFFFFFF;
    }

    // ==================== GE12R ====================

    if (regNum == kAllRegisters || regNum == 18) {
        reg = READ_REGISTER(geREG[0x12]);

        fState.fgColorMonoPat = (reg >> 0) & 0x0000FFFF;
    }

    // ==================== GE13R ====================

    if (regNum == kAllRegisters || regNum == 19) {
        reg = READ_REGISTER(geREG[0x13]);

        fState.bgColorMonoPat = (reg >> 0) & 0x0000FFFF;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvDoCommand
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvDoCommand(void) {
    switch (fState.commandType) {
        case kCommandNOP:
            // NOP command -- do nothing
            break;

        case kCommandBitBLT:
            this->PrvDoBitBLT();
            break;

        case kCommandLine:
            this->PrvDoLine();
            break;

        default:
            this->PrvIllegalCommand();
            break;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvDoBitBLT
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvDoBitBLT(void) {
    cscolor += 0xf0f0;

    this->PrvIncBlitterInit();
    this->PrvIncBlitterRun();
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvDoLine
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvDoLine(void) {
    PRINTF_LINE("	PrvDoLine:	&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");

#if LOG_LINE
    this->PrvLogGEState();
#endif

    // Draw a line using the information in the GE registers,
    // using a Bresenham line drawing algorithm.  For a good
    // tutorial on this algorithm, see:
    //
    //	<http://www.gamedev.net/reference/articles/article1275.asp>

    // Figure out our starting and ending points.  The starting
    // point is in the registers.  The ending point needs to
    // be determined by looking at deltaMajor, deltaMinor, the
    // flag that says if the major is the X or Y access, and the
    // X and Y directions.

    uint16 yIsMajor;    // 0 = x is major, 1 = y is major
    uint16 xDirection;  // 0 = x is positive, 1 = x is negative
    uint16 yDirection;  // 0 = y is positive, 1 = y is negative

    if (fState.useXY) {
        yIsMajor = fState.yIsMajor;
        xDirection = fState.xDirection;
        yDirection = fState.yDirection;
    } else {
        yIsMajor = kQuadrantDecode[fState.quadrant].yIsMajor;
        xDirection = kQuadrantDecode[fState.quadrant].xDirection;
        yDirection = kQuadrantDecode[fState.quadrant].yDirection;
    }

    PRINTF_LINE("	yIsMajor:		%d", yIsMajor);
    PRINTF_LINE("	xDirection:		%d", xDirection);
    PRINTF_LINE("	yDirection:		%d", yDirection);

    // Get the starting x,y position.  We'll be updating these
    // variables as we draw the line.

    uint16 x = fState.xStart;
    uint16 y = fState.yStart;

    PRINTF_LINE("	x:				%d", x);
    PRINTF_LINE("	y:				%d", y);

    // Get the direction in which the x,y coordinates needs to
    // be updated.

    int xAdjust = xDirection ? -1 : 1;
    int yAdjust = yDirection ? -1 : 1;

    PRINTF_LINE("	xAdjust:		%d", xAdjust);
    PRINTF_LINE("	yAdjust:		%d", yAdjust);

    // Based on which axis is the major axis, get pointers to
    // the major and minor coordinates and the values that will
    // be used to update them.

    uint16* major;
    uint16* minor;

    int majorAdjust;
    int minorAdjust;

    if (yIsMajor) {
        major = &y;
        minor = &x;

        majorAdjust = yAdjust;
        minorAdjust = xAdjust;
    } else {
        major = &x;
        minor = &y;

        majorAdjust = xAdjust;
        minorAdjust = yAdjust;
    }

    PRINTF_LINE("	major:			%d", *major);
    PRINTF_LINE("	minor:			%d", *minor);

    PRINTF_LINE("	majorAdjust:	%d", majorAdjust);
    PRINTF_LINE("	minorAdjust:	%d", minorAdjust);

    PRINTF_LINE("	gamma:			%d", fState.gamma);
    PRINTF_LINE("	majorLength:	%d", fState.majorLength);
    PRINTF_LINE("	deltaMajor:		%d", fState.deltaMajor);

    PRINTF_LINE("	...................");

    // Initialize "error" (which is used to determine when we
    // need to increment the minor axis coordinate) and "count"
    // (which is used to iterate over the major axis).

    int error = 0;
    uint16 count = 0;

    // Iterate over all the points along the major axis.

    while (count < fState.majorLength) {
        PRINTF_LINE("	count:			%d", count);
        PRINTF_LINE("	major:			%d", *major);
        PRINTF_LINE("	minor:			%d", *minor);

        // Plot a point.

        uint16 src =
            0;  // Dummy value.  The ROP had better not be one that makes use of a source pixel!
        uint16 pen = fState.fgColorMonoPat;
        uint16 dest = this->PrvGetPixel(x, y);

        uint16 output = this->PrvAdjustPixel(pen, src, dest, fState.rasterOperation);

        this->PrvSetPixel(output, x, y);

        // Update the major axis coordinate and adjust the error factor.

        *major += majorAdjust;
        error += 2 * fState.deltaMinor;

        // See if it's time to update the minor access coordinate.

        if (error > fState.deltaMajor) {
            // Update the minor axis coordinate and adjust the error factor.

            *minor += minorAdjust;
            error -= 2 * fState.deltaMajor;
        }

        // Keep track of how many points we've plotted.

        count += 1;
    }

    PRINTF_LINE("	...................");

    // If we need to draw the last pixel, then do it.

    if (fState.drawLastPixel) {
        PRINTF_LINE("	count:			%d", count);
        PRINTF_LINE("	major:			%d", *major);
        PRINTF_LINE("	minor:			%d", *minor);

        PRINTF_LINE("	...................");

        uint16 src =
            0;  // Dummy value.  The ROP had better not be one that makes use of a source pixel!
        uint16 pen = fState.fgColorMonoPat;
        uint16 dest = this->PrvGetPixel(x, y);

        uint16 output = this->PrvAdjustPixel(pen, src, dest, fState.rasterOperation);

        this->PrvSetPixel(output, x, y);
    }

    PRINTF_LINE("	PrvDoLine:	&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvIllegalCommand
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvIllegalCommand(void) {}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvSrcFifoFilledSlots
// ---------------------------------------------------------------------------

inline int EmRegsMediaQ11xx::PrvSrcFifoFilledSlots(void) { return fSourceFifo.Size() / 2; }

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvGetSrcFifoSlot
// ---------------------------------------------------------------------------

inline void EmRegsMediaQ11xx::PrvGetSrcFifoSlot(uint32& a, uint32& b) {
    a = fSourceFifo.Pop();
    b = fSourceFifo.Pop();
}

// ---------------------------------------------------------------------------
//		Drawing Functions
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvAdjustPixel
// ---------------------------------------------------------------------------

uint16 EmRegsMediaQ11xx::PrvAdjustPixel(uint16 pen, uint16 dest, uint8 rOpCode) {
    uint16 result;

    /*
            (From MSDN)

            Each raster-operation code represents a Boolean operation in which the
            values of the pixels in the selected pen and the destination bitmap are
            combined. The following are the two operands used in these operations.

                    Operand		Meaning
                    -------		-------
                    P			Selected pen
                    D			Destination bitmap

            The Boolean operators used in these operations follow.

                    Operator	Meaning
                    --------	-------
                    a			Bitwise AND
                    n			Bitwise NOT (inverse)
                    o			Bitwise OR
                    x			Bitwise exclusive OR (XOR)

            All Boolean operations are presented in reverse Polish notation. For
            example, the following operation replaces the values of the pixels in
            the destination bitmap with a combination of the pixel values of the
            pen and the selected brush:

                    DPo

            Each raster-operation code is a 32-bit integer whose high-order word
            is a Boolean operation index and whose low-order word is the operation
            code. The 16-bit operation index is a zero-extended 8-bit value that
            represents all possible outcomes resulting from the Boolean operation
            on two parameters (in this case, the pen and destination values). For
            example, the operation indexes for the DPo and DPan operations are shown
            in the following list.

                    P	D	DPo	Dpan
                    0	0	0	1
                    0	1	1	1
                    1	0	1	1
                    1	1	1	0

            The following list outlines the drawing modes and the Boolean operations
            that they represent.

                    Raster operation	Boolean operation
                    ----------------	-----------------
                    R2_BLACK			0
                    R2_COPYPEN			P
                    R2_MASKNOTPEN		DPna
                    R2_MASKPEN			DPa
                    R2_MASKPENNOT		PDna
                    R2_MERGENOTPEN		DPno
                    R2_MERGEPEN			DPo
                    R2_MERGEPENNOT		PDno
                    R2_NOP				D
                    R2_NOT				Dn
                    R2_NOTCOPYPEN		Pn
                    R2_NOTMASKPEN		DPan
                    R2_NOTMERGEPEN		DPon
                    R2_NOTXORPEN		DPxn
                    R2_WHITE			1
                    R2_XORPEN			DPx
    */

    switch (rOpCode) {
        case 0: /*  0       */
            result = 0;
            break;

        case 1: /* DPon     */
            result = ~(dest | pen);
            break;

        case 2: /* DPna     */
            result = dest & ~pen;
            break;

        case 3: /* PN       */
            result = ~pen;
            break;

        case 4: /* PDna     */
            result = pen & ~dest;
            break;

        case 5: /* Dn       */
            result = ~dest;
            break;

        case 6: /* DPx      */
            result = dest ^ pen;
            break;

        case 7: /* DPan     */
            result = ~(dest & pen);
            break;

        case 8: /* DPa      */
            result = dest & pen;
            break;

        case 9: /* DPxn     */
            result = ~(dest ^ pen);
            break;

        case 10: /* D        */
            result = dest;
            break;

        case 11: /* DPno     */
            result = dest | ~pen;
            break;

        case 12: /* P        */
            result = pen;
            break;

        case 13: /* PDno     */
            result = pen & ~dest;
            break;

        case 14: /* DPo      */
            result = dest | pen;
            break;

        case 15: /*  1       */
            result = ~0;
            break;

        default:
            logging::printf("illegal ROP");

            result = 0;
            break;
    }

#ifdef _DEBUG
    // Double-check the special cases with the generalized code.

    {
        const int kNumBits = 8 * sizeof(result);

        uint16 result2 = 0;

        for (int bitNumber = 0; bitNumber < kNumBits; ++bitNumber) {
            uint8 index = (((pen >> bitNumber) & 1) << 1) | (((dest >> bitNumber) & 1) << 0);

            result2 |= (((rOpCode >> index) & 1) << bitNumber);
        }

        EmAssert(result == result2);
    }
#endif

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvAdjustPixel
// ---------------------------------------------------------------------------

uint16 EmRegsMediaQ11xx::PrvAdjustPixel(uint16 pattern, uint16 src, uint16 dest, uint8 rOpCode) {
    uint16 result;

    /*
            (From MSDN)

            Each raster-operation code represents a Boolean operation in which
            the values of the pixels in the source, the selected brush, and the
            destination are combined. The following are the three operands used
            in these operations.

                    Operand		Meaning
                    -------		-------
                    D			Destination bitmap
                    P			Selected brush (also called pattern)
                    S			Source bitmap

            Boolean operators used in these operations follow.

                    Operator	Meaning
                    --------	-------
                    a			Bitwise AND
                    n			Bitwise NOT (inverse)
                    o			Bitwise OR
                    x			Bitwise exclusive OR (XOR)

            ---------------------------------------------------------------------

            (From Keith)

            Here's another way to think about it: take the pattern, source, and
            destination, and consider each value one bit at a time.  The three
            bits from the three values can form eight combinations.  The raster
            opcode is a bitfield containing the desired output from each of the
            combinations.  Thus:

                     P	 S	 D	 O (output)
                    ---	---	---	---
                     0	 0	 0	 b0
                     0	 0	 1	 b1
                     0	 1	 0	 b2
                     0	 1	 1	 b3
                     1	 0	 0	 b4
                     1	 0	 1	 b5
                     1	 1	 0	 b6
                     1	 1	 1	 b7

            The output is the byte <b7><b6><b5><b4><b3><b2><b1><b0>.  And this
            output value is what becomes the raster opcode.  Thus, if the raster
            opcode is, for example, 0xCC, the table would be:

                     P	 S	 D	 O (output)
                    ---	---	---	---
                     0	 0	 0	 0
                     0	 0	 1	 0
                     0	 1	 0	 1
                     0	 1	 1	 1
                     1	 0	 0	 0
                     1	 0	 1	 0
                     1	 1	 0	 1
                     1	 1	 1	 1

            So, the we look at three bits from the source and they're all zero,
            then the output is zero.  If they're all one, then the output is
            one.  If the pattern is one and the source and destination are zero,
            then the output is zero. Overall,

                    Output[x] = ROP[P[x],S[x],D[x]]

            where "x" is 0..7, and "P[x],S[x],D[x]" is a bitfield composed of
            the given bits.

            We will use this generalization for the default case.  However, for
            the most common raster ops, we can manipulate all bits of all the
            incumbant values simultaneously.  We special-case those opcodes.
    */

    switch (rOpCode) {
        case 0x00:  // 00000042 0 BLACKNESS
            result = 0;
            break;

            //	case 0x01:	// 00010289 DPSoon �
            //	case 0x02:	// 00020C89 DPSona �
            //	case 0x03:	// 000300AA PSon �
            //	case 0x04:	// 00040C88 SDPona �
            //	case 0x05:	// 000500A9 DPon �
            //	case 0x06:	// 00060865 PDSxnon �
            //	case 0x07:	// 000702C5 PDSaon �
            //	case 0x08:	// 00080F08 SDPnaa �
            //	case 0x09:	// 00090245 PDSxon �
        case 0x0A:  // 000A0329 DPna �
            result = dest & ~pattern;
            break;

            //	case 0x0B:	// 000B0B2A PSDnaon �
            //	case 0x0C:	// 000C0324 SPna �
            //	case 0x0D:	// 000D0B25 PDSnaon �
            //	case 0x0E:	// 000E08A5 PDSonon �
        case 0x0F:  // 000F0001 Pn �
            result = ~pattern;
            break;

            //	case 0x10:	// 00100C85 PDSona �
        case 0x11:  // 001100A6 DSon NOTSRCERASE
            result = ~(dest | src);
            break;

            //	case 0x12:	// 00120868 SDPxnon �
            //	case 0x13:	// 001302C8 SDPaon �
            //	case 0x14:	// 00140869 DPSxnon �
            //	case 0x15:	// 001502C9 DPSaon �
            //	case 0x16:	// 00165CCA PSDPSanaxx �
            //	case 0x17:	// 00171D54 SSPxDSxaxn �
            //	case 0x18:	// 00180D59 SPxPDxa �
            //	case 0x19:	// 00191CC8 SDPSanaxn �
            //	case 0x1A:	// 001A06C5 PDSPaox �
            //	case 0x1B:	// 001B0768 SDPSxaxn �
            //	case 0x1C:	// 001C06CA PSDPaox �
            //	case 0x1D:	// 001D0766 DSPDxaxn �
            //	case 0x1E:	// 001E01A5 PDSox �
            //	case 0x1F:	// 001F0385 PDSoan �
            //	case 0x20:	// 00200F09 DPSnaa �
            //	case 0x21:	// 00210248 SDPxon �
        case 0x22:  // 00220326 DSna �
            result = dest & ~src;
            break;

            //	case 0x23:	// 00230B24 SPDnaon �
            //	case 0x24:	// 00240D55 SPxDSxa �
            //	case 0x25:	// 00251CC5 PDSPanaxn �
            //	case 0x26:	// 002606C8 SDPSaox �
            //	case 0x27:	// 00271868 SDPSxnox �
            //	case 0x28:	// 00280369 DPSxa �
            //	case 0x29:	// 002916CA PSDPSaoxxn �
            //	case 0x2A:	// 002A0CC9 DPSana �
            //	case 0x2B:	// 002B1D58 SSPxPDxaxn �
            //	case 0x2C:	// 002C0784 SPDSoax �
            //	case 0x2D:	// 002D060A PSDnox �
            //	case 0x2E:	// 002E064A PSDPxox �
            //	case 0x2F:	// 002F0E2A PSDnoan �
            //	case 0x30:	// 0030032A PSna �
            //	case 0x31:	// 00310B28 SDPnaon �
            //	case 0x32:	// 00320688 SDPSoox �
        case 0x33:  // 00330008 Sn NOTSRCCOPY
            result = ~src;
            break;

            //	case 0x34:	// 003406C4 SPDSaox �
            //	case 0x35:	// 00351864 SPDSxnox �
            //	case 0x36:	// 003601A8 SDPox �
            //	case 0x37:	// 00370388 SDPoan �
            //	case 0x38:	// 0038078A PSDPoax �
            //	case 0x39:	// 00390604 SPDnox �
            //	case 0x3A:	// 003A0644 SPDSxox �
            //	case 0x3B:	// 003B0E24 SPDnoan �
            //	case 0x3C:	// 003C004A PSx �
            //	case 0x3D:	// 003D18A4 SPDSonox �
            //	case 0x3E:	// 003E1B24 SPDSnaox �
            //	case 0x3F:	// 003F00EA PSan �
            //	case 0x40:	// 00400F0A PSDnaa �
            //	case 0x41:	// 00410249 DPSxon �
            //	case 0x42:	// 00420D5D SDxPDxa �
            //	case 0x43:	// 00431CC4 SPDSanaxn �
        case 0x44:  // 00440328 SDna SRCERASE
            result = src & ~dest;
            break;

            //	case 0x45:	// 00450B29 DPSnaon �
            //	case 0x46:	// 004606C6 DSPDaox �
            //	case 0x47:	// 0047076A PSDPxaxn �
            //	case 0x48:	// 00480368 SDPxa �
            //	case 0x49:	// 004916C5 PDSPDaoxxn �
            //	case 0x4A:	// 004A0789 DPSDoax �
            //	case 0x4B:	// 004B0605 PDSnox �
            //	case 0x4C:	// 004C0CC8 SDPana �
            //	case 0x4D:	// 004D1954 SSPxDSxoxn �
            //	case 0x4E:	// 004E0645 PDSPxox �
            //	case 0x4F:	// 004F0E25 PDSnoan �
            //	case 0x50:	// 00500325 PDna �
            //	case 0x51:	// 00510B26 DSPnaon �
            //	case 0x52:	// 005206C9 DPSDaox �
            //	case 0x53:	// 00530764 SPDSxaxn �
            //	case 0x54:	// 005408A9 DPSonon �
        case 0x55:  // 00550009 Dn DSTINVERT
            result = ~dest;
            break;

            //	case 0x56:	// 005601A9 DPSox �
            //	case 0x57:	// 00570389 DPSoan �
            //	case 0x58:	// 00580785 PDSPoax �
            //	case 0x59:	// 00590609 DPSnox �
        case 0x5A:  // 005A0049 DPx PATINVERT
            result = dest ^ pattern;
            break;

            //	case 0x5B:	// 005B18A9 DPSDonox �
            //	case 0x5C:	// 005C0649 DPSDxox �
            //	case 0x5D:	// 005D0E29 DPSnoan �
            //	case 0x5E:	// 005E1B29 DPSDnaox �
        case 0x5F:  // 005F00E9 DPan �
            result = ~(dest & pattern);
            break;

            //	case 0x60:	// 00600365 PDSxa �
            //	case 0x61:	// 006116C6 DSPDSaoxxn �
            //	case 0x62:	// 00620786 DSPDoax �
            //	case 0x63:	// 00630608 SDPnox �
            //	case 0x64:	// 00640788 SDPSoax �
            //	case 0x65:	// 00650606 DSPnox �
        case 0x66:  // 00660046 DSx SRCINVERT
            result = dest ^ src;
            break;

            //	case 0x67:	// 006718A8 SDPSonox �
            //	case 0x68:	// 006858A6 DSPDSonoxxn �
            //	case 0x69:	// 00690145 PDSxxn �
            //	case 0x6A:	// 006A01E9 DPSax �
            //	case 0x6B:	// 006B178A PSDPSoaxxn �
            //	case 0x6C:	// 006C01E8 SDPax �
            //	case 0x6D:	// 006D1785 PDSPDoaxxn �
            //	case 0x6E:	// 006E1E28 SDPSnoax �
            //	case 0x6F:	// 006F0C65 PDSxnan �
            //	case 0x70:	// 00700CC5 PDSana �
            //	case 0x71:	// 00711D5C SSDxPDxaxn �
            //	case 0x72:	// 00720648 SDPSxox �
            //	case 0x73:	// 00730E28 SDPnoan �
            //	case 0x74:	// 00740646 DSPDxox �
            //	case 0x75:	// 00750E26 DSPnoan �
            //	case 0x76:	// 00761B28 SDPSnaox �
        case 0x77:  // 007700E6 DSan �
            result = ~(dest & src);
            break;

            //	case 0x78:	// 007801E5 PDSax �
            //	case 0x79:	// 00791786 DSPDSoaxxn �
            //	case 0x7A:	// 007A1E29 DPSDnoax �
            //	case 0x7B:	// 007B0C68 SDPxnan �
            //	case 0x7C:	// 007C1E24 SPDSnoax �
            //	case 0x7D:	// 007D0C69 DPSxnan �
            //	case 0x7E:	// 007E0955 SPxDSxo �
            //	case 0x7F:	// 007F03C9 DPSaan �
            //	case 0x80:	// 008003E9 DPSaa �
            //	case 0x81:	// 00810975 SPxDSxon �
            //	case 0x82:	// 00820C49 DPSxna �
            //	case 0x83:	// 00831E04 SPDSnoaxn �
            //	case 0x84:	// 00840C48 SDPxna �
            //	case 0x85:	// 00851E05 PDSPnoaxn �
            //	case 0x86:	// 008617A6 DSPDSoaxx �
            //	case 0x87:	// 008701C5 PDSaxn �
        case 0x88:  // 008800C6 DSa SRCAND
            result = dest & src;
            break;

            //	case 0x89:	// 00891B08 SDPSnaoxn �
            //	case 0x8A:	// 008A0E06 DSPnoa �
            //	case 0x8B:	// 008B0666 DSPDxoxn �
            //	case 0x8C:	// 008C0E08 SDPnoa �
            //	case 0x8D:	// 008D0668 SDPSxoxn �
            //	case 0x8E:	// 008E1D7C SSDxPDxax �
            //	case 0x8F:	// 008F0CE5 PDSanan �
            //	case 0x90:	// 00900C45 PDSxna �
            //	case 0x91:	// 00911E08 SDPSnoaxn �
            //	case 0x92:	// 009217A9 DPSDPoaxx �
            //	case 0x93:	// 009301C4 SPDaxn �
            //	case 0x94:	// 009417AA PSDPSoaxx �
            //	case 0x95:	// 009501C9 DPSaxn �
            //	case 0x96:	// 00960169 DPSxx �
            //	case 0x97:	// 0097588A PSDPSonoxx �
            //	case 0x98:	// 00981888 SDPSonoxn �
        case 0x99:  // 00990066 DSxn �
            result = ~(dest ^ src);
            break;

            //	case 0x9A:	// 009A0709 DPSnax �
            //	case 0x9B:	// 009B07A8 SDPSoaxn �
            //	case 0x9C:	// 009C0704 SPDnax �
            //	case 0x9D:	// 009D07A6 DSPDoaxn �
            //	case 0x9E:	// 009E16E6 DSPDSaoxx �
            //	case 0x9F:	// 009F0345 PDSxan �
        case 0xA0:  // 00A000C9 DPa �
            result = dest & pattern;
            break;

            //	case 0xA1:	// 00A11B05 PDSPnaoxn �
            //	case 0xA2:	// 00A20E09 DPSnoa �
            //	case 0xA3:	// 00A30669 DPSDxoxn �
            //	case 0xA4:	// 00A41885 PDSPonoxn �
        case 0xA5:  // 00A50065 PDxn �
            result = ~(pattern ^ dest);
            break;

            //	case 0xA6:	// 00A60706 DSPnax �
            //	case 0xA7:	// 00A707A5 PDSPoaxn �
            //	case 0xA8:	// 00A803A9 DPSoa �
            //	case 0xA9:	// 00A90189 DPSoxn �
            //	case 0xAA:	// 00AA0029 D �
            //	case 0xAB:	// 00AB0889 DPSono �
            //	case 0xAC:	// 00AC0744 SPDSxax �
            //	case 0xAD:	// 00AD06E9 DPSDaoxn �
            //	case 0xAE:	// 00AE0B06 DSPnao �
        case 0xAF:  // 00AF0229 DPno �
            result = dest | ~pattern;
            break;

            //	case 0xB0:	// 00B00E05 PDSnoa �
            //	case 0xB1:	// 00B10665 PDSPxoxn �
            //	case 0xB2:	// 00B21974 SSPxDSxox �
            //	case 0xB3:	// 00B30CE8 SDPanan �
            //	case 0xB4:	// 00B4070A PSDnax �
            //	case 0xB5:	// 00B507A9 DPSDoaxn �
            //	case 0xB6:	// 00B616E9 DPSDPaoxx �
            //	case 0xB7:	// 00B70348 SDPxan �
            //	case 0xB8:	// 00B8074A PSDPxax �
            //	case 0xB9:	// 00B906E6 DSPDaoxn �
            //	case 0xBA:	// 00BA0B09 DPSnao �
        case 0xBB:  // 00BB0226 DSno MERGEPAINT
            result = dest | ~src;
            break;

            //	case 0xBC:	// 00BC1CE4 SPDSanax �
            //	case 0xBD:	// 00BD0D7D SDxPDxan �
            //	case 0xBE:	// 00BE0269 DPSxo �
            //	case 0xBF:	// 00BF08C9 DPSano �
        case 0xC0:  // 00C000CA PSa MERGECOPY
            result = pattern & src;
            break;

            //	case 0xC1:	// 00C11B04 SPDSnaoxn �
            //	case 0xC2:	// 00C21884 SPDSonoxn �
            //	case 0xC3:	// 00C3006A PSxn �
            //	case 0xC4:	// 00C40E04 SPDnoa �
            //	case 0xC5:	// 00C50664 SPDSxoxn �
            //	case 0xC6:	// 00C60708 SDPnax �
            //	case 0xC7:	// 00C707AA PSDPoaxn �
            //	case 0xC8:	// 00C803A8 SDPoa �
            //	case 0xC9:	// 00C90184 SPDoxn �
            //	case 0xCA:	// 00CA0749 DPSDxax �
            //	case 0xCB:	// 00CB06E4 SPDSaoxn �
        case 0xCC:  // 00CC0020 S SRCCOPY
            result = src;
            break;

            //	case 0xCD:	// 00CD0888 SDPono �
            //	case 0xCE:	// 00CE0B08 SDPnao �
            //	case 0xCF:	// 00CF0224 SPno �
            //	case 0xD0:	// 00D00E0A PSDnoa �
            //	case 0xD1:	// 00D1066A PSDPxoxn �
            //	case 0xD2:	// 00D20705 PDSnax �
            //	case 0xD3:	// 00D307A4 SPDSoaxn �
            //	case 0xD4:	// 00D41D78 SSPxPDxax �
            //	case 0xD5:	// 00D50CE9 DPSanan �
            //	case 0xD6:	// 00D616EA PSDPSaoxx �
            //	case 0xD7:	// 00D70349 DPSxan �
            //	case 0xD8:	// 00D80745 PDSPxax �
            //	case 0xD9:	// 00D906E8 SDPSaoxn �
            //	case 0xDA:	// 00DA1CE9 DPSDanax �
            //	case 0xDB:	// 00DB0D75 SPxDSxan �
            //	case 0xDC:	// 00DC0B04 SPDnao �
            //	case 0xDD:	// 00DD0228 SDno �
            //	case 0xDE:	// 00DE0268 SDPxo �
            //	case 0xDF:	// 00DF08C8 SDPano �
            //	case 0xE0:	// 00E003A5 PDSoa �
            //	case 0xE1:	// 00E10185 PDSoxn �
            //	case 0xE2:	// 00E20746 DSPDxax �
            //	case 0xE3:	// 00E306EA PSDPaoxn �
            //	case 0xE4:	// 00E40748 SDPSxax �
            //	case 0xE5:	// 00E506E5 PDSPaoxn �
            //	case 0xE6:	// 00E61CE8 SDPSanax �
            //	case 0xE7:	// 00E70D79 SPxPDxan �
            //	case 0xE8:	// 00E81D74 SSPxDSxax �
            //	case 0xE9:	// 00E95CE6 DSPDSanaxxn �
            //	case 0xEA:	// 00EA02E9 DPSao �
            //	case 0xEB:	// 00EB0849 DPSxno �
            //	case 0xEC:	// 00EC02E8 SDPao �
            //	case 0xED:	// 00ED0848 SDPxno �
        case 0xEE:  // 00EE0086 DSo SRCPAINT
            result = dest | src;
            break;

            //	case 0xEF:	// 00EF0A08 SDPnoo �
        case 0xF0:  // 00F00021 P PATCOPY
            result = pattern;
            break;

            //	case 0xF1:	// 00F10885 PDSono �
            //	case 0xF2:	// 00F20B05 PDSnao �
            //	case 0xF3:	// 00F3022A PSno �
            //	case 0xF4:	// 00F40B0A PSDnao �
            //	case 0xF5:	// 00F50225 PDno �
            //	case 0xF6:	// 00F60265 PDSxo �
            //	case 0xF7:	// 00F708C5 PDSano �
            //	case 0xF8:	// 00F802E5 PDSao �
            //	case 0xF9:	// 00F90845 PDSxno �
        case 0xFA:  // 00FA0089 DPo �
            result = dest | pattern;
            break;

        case 0xFB:  // 00FB0A09 DPSnoo PATPAINT
            result = dest | (pattern | ~src);
            break;

            //	case 0xFC:	// 00FC008A PSo �
            //	case 0xFD:	// 00FD0A0A PSDnoo �
            //	case 0xFE:	// 00FE02A9 DPSoo �
        case 0xFF:  // 00FF0062 1 WHITENESS
            result = ~0;
            break;

        default: {
            const int kNumBits = 8 * sizeof(result);

            result = 0;

            for (int bitNumber = 0; bitNumber < kNumBits; ++bitNumber) {
                uint8 index = (((pattern >> bitNumber) & 1) << 2) |
                              (((src >> bitNumber) & 1) << 1) | (((dest >> bitNumber) & 1) << 0);

                result |= (((rOpCode >> index) & 1) << bitNumber);
            }
        } break;
    }

#ifdef _DEBUG
    // Double-check the special cases with the generalized code.

    {
        const int kNumBits = 8 * sizeof(result);

        uint16 result2 = 0;

        for (int bitNumber = 0; bitNumber < kNumBits; ++bitNumber) {
            uint8 index = (((pattern >> bitNumber) & 1) << 2) | (((src >> bitNumber) & 1) << 1) |
                          (((dest >> bitNumber) & 1) << 0);

            result2 |= (((rOpCode >> index) & 1) << bitNumber);
        }

        EmAssert(result == result2);
    }
#endif

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvSetPixel
// ---------------------------------------------------------------------------

inline void EmRegsMediaQ11xx::PrvSetPixel(uint16 pixel, uint16 x, uint16 y) {
    emuptr pixelLocation = this->PrvGetPixelLocation(x, y);
    if ((pixelLocation - this->PrvGetVideoBase()) > MMIO_OFFSET) {
        return;
    }

    switch (fState.colorDepth) {
        case kColorDepth8:
            EmMemPut8(pixelLocation, pixel);
            break;

        case kColorDepth16:
            EmMemPut16(pixelLocation, pixel);
            break;

        default:
            EmAssert(false);
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvGetPixel
// ---------------------------------------------------------------------------

inline uint16 EmRegsMediaQ11xx::PrvGetPixel(uint16 x, uint16 y) {
    uint16 result;
    emuptr pixelLocation = this->PrvGetPixelLocation(x, y);
    if ((pixelLocation - this->PrvGetVideoBase()) > MMIO_OFFSET) {
        return 0;
    }

    switch (fState.colorDepth) {
        case kColorDepth8:
            result = EmMemGet8(pixelLocation);
            break;

        case kColorDepth16:
            result = EmMemGet16(pixelLocation);
            break;

        default:
            logging::printf("illegal color depth");

            result = 0;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvGetPixelLocation
// ---------------------------------------------------------------------------

inline emuptr EmRegsMediaQ11xx::PrvGetPixelLocation(uint16 x, uint16 y) {
    int bytesPerPixel;

    switch (fState.colorDepth) {
        case kColorDepth8:
            bytesPerPixel = 1;
            break;

        case kColorDepth16:
            bytesPerPixel = 2;
            break;

        default:
            logging::printf("illegal color depth");

            return this->PrvGetVideoBase();
    }

    if (fState.rotate90) {
        // Rotate 90 deg clockwise.

        /*
                 -----x             y--
                |+--------------------+|
                ||                    ||
                y|    1               ||
                 |                  2 |x
                 |                    |
                 |                    |
                 |                    |
                 |                    |
                 |                    |
                 |                    |
                 |                    |
                 +--------------------+

                        Point 1 is the original point.
                        Point 2 is the rotated point.
        */

        int16 newX = fState.destLineStride * bytesPerPixel - y;
        int16 newY = x;

        x = newX;
        y = newY;
    }

    emuptr frameBuffer = this->PrvGetVideoBase() + fState.baseAddr;
    emuptr scanLine = frameBuffer + (y * fState.destLineStride);
    emuptr scanByte = scanLine + (x * bytesPerPixel);

    return scanByte;
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvIncBlitterInit
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvIncBlitterInit(void) {
    PRINTF_BLIT(
        "	PrvIncBlitterInit:	"
        "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");

#if LOG_BLIT
    this->PrvLogGEState();
#endif

    fSourceFifo.Clear();
    fBlitInProgress = true;

    fCurXOffset = 0;
    fCurYOffset = 0;

    fUsesPattern = this->PrvUsesPattern();
    fUsesSource = this->PrvUsesSource();

    if (!fUsesPattern) PRINTF_BLIT("	PrvIncBlitterInit:	pattern not used...");

    if (!fUsesSource) PRINTF_BLIT("	PrvIncBlitterInit:	source data not used...");

    if (fState.monoSource) {
        if (fState.monoTransEnable) {
            if (fState.monoTransPolarity) {
                PRINTF_BLIT("	PrvIncBlitterInit:	Foreground is transparent...");
            } else {
                PRINTF_BLIT("	PrvIncBlitterInit:	Background is transparent...");
            }
        }
    } else if (fState.colorTransEnable) {
        if (fState.destTransPolarity == 0) {
            if (fState.colorTransCmpSrc == 0) {
                PRINTF_BLIT(
                    "	PrvIncBlitterInit:	Treat all destTransColor pixels in the source as "
                    "transparent...");
            } else {
                PRINTF_BLIT(
                    "	PrvIncBlitterInit:	Leave all destTransColor pixels in the destination "
                    "alone...");
            }
        } else {
            if (fState.colorTransCmpSrc == 0) {
                PRINTF_BLIT(
                    "	PrvIncBlitterInit:	Transfer only destTransColor pixels from the "
                    "source to the destination...");
            } else {
                PRINTF_BLIT(
                    "	PrvIncBlitterInit:	Update only destTransColor pixels in the "
                    "destination with source pixels...");
            }
        }
    }

    this->PrvPatternPipeInit();
    this->PrvSrcPipeInit();
    this->PrvDestPipeInit();

    PRINTF_BLIT(
        "	PrvIncBlitterInit:	"
        "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&");
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvIncBlitterRun
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvIncBlitterRun(void) {
    if (!fBlitInProgress) return;

#ifdef LOGGING
    static long counter = 0;
#endif

    PRINTF_BLIT(
        "	PrvIncBlitterRun:	"
        "**************************************************");
    PRINTF_BLIT("	PrvIncBlitterRun:	counter:	%u", ++counter);
    PRINTF_BLIT(
        "	PrvIncBlitterRun:	"
        "**************************************************");

    while (fBlitInProgress) {
        Bool stalled;

        // Get the relevent pixel values.

        uint16 source = this->PrvSrcPipeNextPixel(stalled);

        // If the source FIFO stalled (no more input data for now),
        // return false, indicating that the BLT operation is still
        // in progress.

        if (stalled) {
            PRINTF_BLIT("	PrvIncBlitterRun:	stalled...");
            break;
        }

        // Continue getting more pixel values.

        uint16 pattern = this->PrvPatternPipeNextPixel();
        uint16 dest = this->PrvDestPipeNextPixel();
        uint16 output = this->PrvAdjustPixel(pattern, source, dest, fState.rasterOperation);

        PRINTF_BLIT("	PrvIncBlitterRun:	pattern:	0x%04X", pattern);
        PRINTF_BLIT("	PrvIncBlitterRun:	source:		0x%04X", source);
        PRINTF_BLIT("	PrvIncBlitterRun:	dest:		0x%04X", dest);
        PRINTF_BLIT("	PrvIncBlitterRun:	output:		0x%04X", output);

        // Write this pixel as long as it's not transparent or clipped out.

        if (!this->PrvTransparent(source, dest, pattern) && !this->PrvClipped()) {
            PRINTF_BLIT("	PrvIncBlitterRun:	setting...");

            this->PrvSetPixel(output, fXDest, fYDest);
        } else {
            PRINTF_BLIT("	PrvIncBlitterRun:	skipping...");
        }

        // Move to the next X/Y position.

        fBlitInProgress = this->PrvNextXY();

        if (!fBlitInProgress) {
            PRINTF_BLIT("	PrvIncBlitterRun:	Completed!");
        }
    }

    PRINTF_BLIT(
        "	PrvIncBlitterRun:	"
        "**************************************************");
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvPatternPipeInit
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvPatternPipeInit(void) {
    fXPattern = fState.monoPatternXOffset;
    fYPattern = fState.monoPatternYOffset;

    PRINTF_BLIT("	PrvPatternPipeInit:	fXPattern:	%u", fXPattern);
    PRINTF_BLIT("	PrvPatternPipeInit:	fYPattern:	%u", fYPattern);

    // Expand the specified pattern into a pixel array.

    if (fUsesPattern) {
        // Force the pattern data to be a solid color, if specified.

        if (fState.solidPattern) {
            PRINTF_BLIT("	PrvPatternPipeInit:	filling with:	0x%04X",
                        fState.fgColorMonoPat);

            for (int ii = 0; ii < 64; ++ii) {
                fPatternPipe[ii] = fState.fgColorMonoPat;
            }
        }

        // If the pattern is specified as a 64-bit array, expand it
        // into a 64-byte array with the fore- and background colors set.

        else if (fState.monoPattern) {
            /*
                    A pattern is an 8x8 structure, for a total of 64 bits.
                    This is represented on the Palm as an 8 element array
                    of 8-bit bytes.  When translated into MediaQ terms, they
                    are transferred to the registers with:

                    gePAT16( GE_MONO_PAT0_L,    *patPtr++ );
                    gePAT16( GE_MONO_PAT0_H,    *patPtr++ );
                            gePAT16( GE_MONO_PAT1_L,    *patPtr++ );
                    gePAT16( GE_MONO_PAT1_H,    *patPtr   );

                    where "patPtr" is a UInt16*.  Thus, when read back out
                    as two DWORDs, the LSB of monoPattern1 contains the first
                    byte of the pattern, the next LSB contains the second
                    byte, all the way up to where the MSB of monoPattern2
                    contains the last byte.
            */

            PRINTF_BLIT("	PrvPatternPipeInit:	expanding:	0x%08X 0x%08X",
                        fState.monoPattern1, fState.monoPattern2);

            this->PrvExpandMono32(fState.monoPattern1, &fPatternPipe[0], fState.fgColorMonoPat,
                                  fState.bgColorMonoPat);

            this->PrvExpandMono32(fState.monoPattern2, &fPatternPipe[32], fState.fgColorMonoPat,
                                  fState.bgColorMonoPat);

            uint16* p = fPatternPipe;
            UNUSED_PARAM(p);

            PRINTF_BLIT(
                "	PrvPatternPipeInit:	0:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X "
                "0x%04X 0x%04X",
                p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
            PRINTF_BLIT(
                "	PrvPatternPipeInit:	1:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X "
                "0x%04X 0x%04X",
                p[8 + 0], p[8 + 1], p[8 + 2], p[8 + 3], p[8 + 4], p[8 + 5], p[8 + 6], p[8 + 7]);
            PRINTF_BLIT(
                "	PrvPatternPipeInit: 2:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X "
                "0x%04X",
                p[16 + 0], p[16 + 1], p[16 + 2], p[16 + 3], p[16 + 4], p[16 + 5], p[16 + 6],
                p[16 + 7]);
            PRINTF_BLIT(
                "	PrvPatternPipeInit: 3:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X "
                "0x%04X",
                p[24 + 0], p[24 + 1], p[24 + 2], p[24 + 3], p[24 + 4], p[24 + 5], p[24 + 6],
                p[24 + 7]);
            PRINTF_BLIT(
                "	PrvPatternPipeInit: 4:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X "
                "0x%04X",
                p[32 + 0], p[32 + 1], p[32 + 2], p[32 + 3], p[32 + 4], p[32 + 5], p[32 + 6],
                p[32 + 7]);
            PRINTF_BLIT(
                "	PrvPatternPipeInit: 5:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X "
                "0x%04X",
                p[40 + 0], p[40 + 1], p[40 + 2], p[40 + 3], p[40 + 4], p[40 + 5], p[40 + 6],
                p[40 + 7]);
            PRINTF_BLIT(
                "	PrvPatternPipeInit: 6:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X "
                "0x%04X",
                p[48 + 0], p[48 + 1], p[48 + 2], p[48 + 3], p[48 + 4], p[48 + 5], p[48 + 6],
                p[48 + 7]);
            PRINTF_BLIT(
                "	PrvPatternPipeInit: 7:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X "
                "0x%04X",
                p[56 + 0], p[56 + 1], p[56 + 2], p[56 + 3], p[56 + 4], p[56 + 5], p[56 + 6],
                p[56 + 7]);
        } else {
            // The monoPattern bit MUST be programmed to 1, according to
            // the docs.  Color patterns don't appear to be supported.

            logging::printf("illegal pattern setup");
        }
    }

    // We're not making use of any pattern data.

    else {
        // Do nothing.  The pattern array will be garbage, but the theory
        // here is that it's not going to be used.
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvPatternPipeNextPixel
// ---------------------------------------------------------------------------

inline uint16 EmRegsMediaQ11xx::PrvPatternPipeNextPixel(void) {
    uint16 result = fPatternPipe[fXPattern + fYPattern * 8];

    PRINTF_BLIT("	PrvPatternPipeNextPixel:	fXPattern:	%u", fXPattern);
    PRINTF_BLIT("	PrvPatternPipeNextPixel:	fYPattern:	%u", fYPattern);
    PRINTF_BLIT("	PrvPatternPipeNextPixel:	result:		0x%04X", result);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvPatternPipeNextX
// ---------------------------------------------------------------------------

inline void EmRegsMediaQ11xx::PrvPatternPipeNextX(void) {
    ++fXPattern;

    if (fXPattern == 8) {
        fXPattern = 0;
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvPatternPipeNextY
// ---------------------------------------------------------------------------

inline void EmRegsMediaQ11xx::PrvPatternPipeNextY(void) {
    ++fYPattern;
    fXPattern = fState.monoPatternXOffset;

    if (fYPattern == 8) {
        fYPattern = 0;
    }
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvSrcPipeInit
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvSrcPipeInit(void) {
    fXSrc = fState.xSrc;
    fYSrc = fState.ySrc;

    if (fState.xyConversion) {
        if (fState.xDirection) fXSrc += fState.width - 1;

        if (fState.yDirection) fYSrc += fState.height - 1;
    }

    if (fUsesSource && !fState.solidSourceColor && fState.systemMemory) {
        fLeadingSourcePixels = this->PrvLeadingPixels();
        fTrailingSourcePixels = this->PrvTrailingPixels();

        PRINTF_BLIT("	PrvSrcPipeInit:	fLeadingSourcePixels:	%u", fLeadingSourcePixels);
        PRINTF_BLIT("	PrvSrcPipeInit:	fTrailingSourcePixels:	%u", fTrailingSourcePixels);
    }

    // Setting both of these variables to the same value will force the
    // source pipe to fill itself from the source data FIFO the first
    // time we access it (assuming that that's where the source data is
    // coming from).

    fSourcePipeIndex = 0;
    fSourcePipeMax = 0;

    fSourcePipeSkip = fLeadingSourcePixels;

    PRINTF_BLIT("	PrvSrcPipeInit:	fXSrc:					%u", fXSrc);
    PRINTF_BLIT("	PrvSrcPipeInit:	fYSrc:					%u", fYSrc);
    PRINTF_BLIT("	PrvSrcPipeInit:	fSourcePipeSkip:		%u", fSourcePipeSkip);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvSrcPipeNextPixel
// ---------------------------------------------------------------------------
uint16 EmRegsMediaQ11xx::PrvSrcPipeNextPixel(Bool& stalled) {
    stalled = false;

    if (!fUsesSource) {
        // Return a dummy value.  The theory here is that it's not
        // going to be used.

        PRINTF_BLIT("	PrvSrcPipeNextPixel:	result:	<dummy>");

        return 0;
    }

    // Force the source data to be a solid color, if specified.

    if (fState.solidSourceColor) {
        PRINTF_BLIT("	PrvSrcPipeNextPixel:	result:	0x%04X (solid)", fState.fgColorMonoSrc);

        return fState.fgColorMonoSrc;
    }

    // If we're getting source data from system memory, then read
    // the data from the source FIFO.

    if (fState.systemMemory) {
        uint16 result;

        // Loop getting source pixels.  Normally, we'll just return
        // the first pixel, but we may need to skip some in case we're
        // in the part of the scanline before the first valid pixel
        // or after the last one.

        while (1) {
            if (fSourcePipeIndex == fSourcePipeMax) {
                this->PrvSrcPipeFill(stalled);

                if (stalled) return 0;  // Return a dummy value

                fSourcePipeIndex = 0;
            }

            result = fSourcePipe[fSourcePipeIndex++];

            if (fSourcePipeSkip == 0) {
                break;
            }

            PRINTF_BLIT("	PrvSrcPipeNextPixel:	skipping...	%u", fSourcePipeSkip);

            --fSourcePipeSkip;
        }

        PRINTF_BLIT("	PrvSrcPipeNextPixel:	result:	0x%04X", result);

        return result;
    }

    // We're getting the source pixel value from the display memory.
    if (fState.memToScreen) {
        uint16 y = fYSrc >= fState.ySrc ? fYSrc - fState.ySrc : 0;

        if (fState.monoSource) {
            emuptr location = this->PrvGetVideoBase() + fState.baseAddr +
                              fState.ySrc * fState.destLineStride + fState.srcLeadingBytes;

            uint32 pixel =
                fState.srcLeadingBits +
                y * (fState.width + fState.srcTrailingBits + fState.srcTrailingBytes * 8) + fXSrc;

            location += pixel / 8;
            if ((location - this->PrvGetVideoBase()) > MMIO_OFFSET) {
                return 0;
            }

            uint8 byte = EmMemGet8(location);
            uint32 shift = pixel % 8;

            if (fState.monoSrcBitSwap) {
                return byte & (0x01 << shift) ? fState.fgColorMonoSrc : fState.bgColorMonoSrc;
            } else {
                return byte & (0x80 >> shift) ? fState.fgColorMonoSrc : fState.bgColorMonoSrc;
            }
        } else {
            uint8 bpp = fState.colorDepth == kColorDepth8 ? 1 : 2;

            emuptr location = this->PrvGetVideoBase() + fState.baseAddr +
                              fState.ySrc * fState.destLineStride + fState.srcLeadingBytes +
                              y * (fState.width * bpp + fState.srcTrailingBytes) + fXSrc * bpp;

            if ((location - this->PrvGetVideoBase()) <= 0x40000) {
                return bpp == 1 ? EmMemGet8(location) : EmMemGet16(location);
            } else {
                return 0;
            }
        }

    } else {
        if (fState.monoSource) {
            uint8 bpp = fState.colorDepth == kColorDepth16 ? 16 : 8;

            uint32 pixel = fState.xSrc * bpp + (fXSrc - fState.xSrc) + fState.srcBitOffset +
                           8 * fState.srcByteOffset;
            emuptr location = this->PrvGetVideoBase() + fState.baseAddr +
                              fYSrc * fState.srcLineStride + pixel / 8;

            // at 16bpp, pixels are fetched in LE words -> byteswap
            if (fState.colorDepth == kColorDepth16) location ^= 1;

            if ((location - this->PrvGetVideoBase()) > MMIO_OFFSET) {
                return 0;
            }

            uint8 byte = EmMemGet8(location);
            uint8 shift = pixel % 8;

            if (fState.monoSrcBitSwap) {
                return byte & (0x01 << shift) ? fState.fgColorMonoSrc : fState.bgColorMonoSrc;
            } else {
                return byte & (0x80 >> shift) ? fState.fgColorMonoSrc : fState.bgColorMonoSrc;
            }
        } else {
            return this->PrvGetPixel(fXSrc, fYSrc);
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvSrcPipeNextX
// ---------------------------------------------------------------------------

inline void EmRegsMediaQ11xx::PrvSrcPipeNextX(void) { fXSrc += (fState.xDirection == 0) ? 1 : -1; }

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvSrcPipeNextY
// ---------------------------------------------------------------------------

inline void EmRegsMediaQ11xx::PrvSrcPipeNextY(void) {
    fYSrc += (fState.yDirection == 0) ? 1 : -1;
    fXSrc = fState.xSrc;

    if (fState.xyConversion) {
        if (fState.xDirection) fXSrc += fState.width - 1;
    }

    fSourcePipeSkip = fTrailingSourcePixels /* + fLeadingSourcePixels*/;

    PRINTF_BLIT("	PrvSrcPipeNextY:	fXSrc:				%u", fXSrc);
    PRINTF_BLIT("	PrvSrcPipeNextY:	fYSrc:				%u", fYSrc);
    PRINTF_BLIT("	PrvSrcPipeNextY:	fSourcePipeSkip:	%u", fSourcePipeSkip);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvSrcPipeFill
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvSrcPipeFill(Bool& stalled) {
    // If there's no data in the FIFO, bail out.

    if (this->PrvSrcFifoFilledSlots() == 0) {
        PRINTF_BLIT("	PrvSrcPipeFill:	stalled...");

        stalled = true;
        return;
    }

    uint16* p = fSourcePipe;
    UNUSED_PARAM(p);

    uint32 source1;
    uint32 source2;

    this->PrvGetSrcFifoSlot(source1, source2);

    // If the source is monochrome, then expand it.

    if (fState.monoSource) {
        fSourcePipeMax = 64;

        PRINTF_BLIT("	PrvSrcPipeFill:	expanding:	0x%08X 0x%08X", source1, source2);

        this->PrvExpandMono32(source1, &fSourcePipe[0], fState.fgColorMonoSrc,
                              fState.bgColorMonoSrc);

        this->PrvExpandMono32(source2, &fSourcePipe[32], fState.fgColorMonoSrc,
                              fState.bgColorMonoSrc);

        PRINTF_BLIT(
            "	PrvSrcPipeFill:	0:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
        PRINTF_BLIT(
            "	PrvSrcPipeFill:	1:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[8 + 0], p[8 + 1], p[8 + 2], p[8 + 3], p[8 + 4], p[8 + 5], p[8 + 6], p[8 + 7]);
        PRINTF_BLIT(
            "	PrvSrcPipeFill:	2:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[16 + 0], p[16 + 1], p[16 + 2], p[16 + 3], p[16 + 4], p[16 + 5], p[16 + 6], p[16 + 7]);
        PRINTF_BLIT(
            "	PrvSrcPipeFill:	3:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[24 + 0], p[24 + 1], p[24 + 2], p[24 + 3], p[24 + 4], p[24 + 5], p[24 + 6], p[24 + 7]);
        PRINTF_BLIT(
            "	PrvSrcPipeFill:	4:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[32 + 0], p[32 + 1], p[32 + 2], p[32 + 3], p[32 + 4], p[32 + 5], p[32 + 6], p[32 + 7]);
        PRINTF_BLIT(
            "	PrvSrcPipeFill:	5:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[40 + 0], p[40 + 1], p[40 + 2], p[40 + 3], p[40 + 4], p[40 + 5], p[40 + 6], p[40 + 7]);
        PRINTF_BLIT(
            "	PrvSrcPipeFill:	6:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[48 + 0], p[48 + 1], p[48 + 2], p[48 + 3], p[48 + 4], p[48 + 5], p[48 + 6], p[48 + 7]);
        PRINTF_BLIT(
            "	PrvSrcPipeFill:	7:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[54 + 0], p[54 + 1], p[54 + 2], p[54 + 3], p[54 + 4], p[54 + 5], p[54 + 6], p[54 + 7]);
    }

    // If the source is in color, then break apart the 64-bit
    // value we read into individual pixels.  We'll get either
    // 4 or 8 pixels, depending on the source depth.

    else if (fState.colorDepth == kColorDepth8) {
        fSourcePipeMax = 8;

        fSourcePipe[0] = (source1 >> 0) & 0x000000FF;
        fSourcePipe[1] = (source1 >> 8) & 0x000000FF;
        fSourcePipe[2] = (source1 >> 16) & 0x000000FF;
        fSourcePipe[3] = (source1 >> 24) & 0x000000FF;

        fSourcePipe[4] = (source2 >> 0) & 0x000000FF;
        fSourcePipe[5] = (source2 >> 8) & 0x000000FF;
        fSourcePipe[6] = (source2 >> 16) & 0x000000FF;
        fSourcePipe[7] = (source2 >> 24) & 0x000000FF;

        PRINTF_BLIT(
            "	PrvSrcPipeFill:	0:	0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X 0x%04X",
            p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);
    } else if (fState.colorDepth == kColorDepth16) {
        fSourcePipeMax = 4;

        fSourcePipe[0] = (source1 >> 0) & 0x0000FFFF;
        fSourcePipe[1] = (source1 >> 16) & 0x0000FFFF;

        fSourcePipe[2] = (source2 >> 0) & 0x0000FFFF;
        fSourcePipe[3] = (source2 >> 16) & 0x0000FFFF;

        PRINTF_BLIT("	PrvSrcPipeFill:	0:	0x%04X 0x%04X 0x%04X 0x%04X", p[0], p[1], p[2],
                    p[3]);
    } else {
        logging::printf("illegal source pipe setup");
    }
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvDestPipeInit
// ---------------------------------------------------------------------------

void EmRegsMediaQ11xx::PrvDestPipeInit(void) {
    fXDest = fState.xDest;
    fYDest = fState.yDest;

    if (fState.xyConversion) {
        if (fState.xDirection) fXDest += fState.width - 1;

        if (fState.yDirection) fYDest += fState.height - 1;
    }

    PRINTF_BLIT("	PrvDestPipeInit:	fXDest:	%u", fXDest);
    PRINTF_BLIT("	PrvDestPipeInit:	fYDest:	%u", fYDest);
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvDestPipeNextPixel
// ---------------------------------------------------------------------------

inline uint16 EmRegsMediaQ11xx::PrvDestPipeNextPixel(void) {
    uint16 result = this->PrvGetPixel(fXDest, fYDest);

    PRINTF_BLIT("	PrvDestPipeNextPixel:	fXDest:	%u", fXDest);
    PRINTF_BLIT("	PrvDestPipeNextPixel:	fYDest:	%u", fYDest);
    PRINTF_BLIT("	PrvDestPipeNextPixel:	result:	0x%04X", result);

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvDestPipeNextX
// ---------------------------------------------------------------------------

inline void EmRegsMediaQ11xx::PrvDestPipeNextX(void) {
    fXDest += (fState.xDirection == 0) ? 1 : -1;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvDestPipeNextY
// ---------------------------------------------------------------------------

inline void EmRegsMediaQ11xx::PrvDestPipeNextY(void) {
    fYDest += (fState.yDirection == 0) ? 1 : -1;
    fXDest = fState.xDest;

    if (fState.xyConversion) {
        if (fState.xDirection) fXDest += fState.width - 1;
    }
}

#pragma mark -

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvNextXY
// ---------------------------------------------------------------------------
// Increment our X and Y counters.  If we we're done, return false.  If we've
// moved to the next line, inform the various pipes that we've done that.  If
// we've merely moved to the next pixel on the same line, inform the pipes
// that we've done that.

inline Bool EmRegsMediaQ11xx::PrvNextXY(void) {
    fCurXOffset += 1;

    if (fCurXOffset == fState.width) {
        fCurXOffset = 0;
        fCurYOffset += 1;

        if (fCurYOffset == fState.height) {
            return false;
        }

        this->PrvPatternPipeNextY();
        this->PrvSrcPipeNextY();
        this->PrvDestPipeNextY();
    } else {
        this->PrvPatternPipeNextX();
        this->PrvSrcPipeNextX();
        this->PrvDestPipeNextX();
    }

    return true;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvTransparent
// ---------------------------------------------------------------------------

Bool EmRegsMediaQ11xx::PrvTransparent(uint16 source, uint16 dest, uint16 pattern) {
    if (fState.monoTransEnable) {
        if (fState.monoSource) {
            if (fState.monoTransPolarity == 0) {
                // Source background is transparent.

                if (source == fState.bgColorMonoSrc) {
                    PRINTF_BLIT("	PrvTransparent:	source == fState.bgColorMonoSrc...");
                    return true;
                }
            } else {
                // Source foreground is transparent.

                if (source == fState.fgColorMonoSrc) {
                    PRINTF_BLIT("	PrvTransparent:	source == fState.fgColorMonoSrc...");
                    return true;
                }
            }
        }

        if (fState.monoPattern) {
            if (fState.monoTransPolarity == 0) {
                // Source background is transparent.

                if (pattern == fState.bgColorMonoPat) {
                    PRINTF_BLIT("	PrvTransparent:	pattern == fState.bgColorMonoPat...");
                    return true;
                }
            } else {
                // Source foreground is transparent.

                if (pattern == fState.fgColorMonoPat) {
                    PRINTF_BLIT("	PrvTransparent:	pattern == fState.fgColorMonoPat...");
                    return true;
                }
            }
        }
    }

    if (fState.colorTransEnable) {
        // Compare to source data.

        if (fState.colorTransCmpSrc == 0) {
            if (!fState.monoSource) {
                // Transparent if source is the same as the test color.

                if (fState.destTransPolarity == 0) {
                    if (source == fState.destTransColor) {
                        PRINTF_BLIT("	PrvTransparent:	source == fState.destTransColor...");
                        return true;
                    }
                }

                // Transparent if source is different from the test color.

                else {
                    if (source != fState.destTransColor) {
                        PRINTF_BLIT("	PrvTransparent:	source != fState.destTransColor...");
                        return true;
                    }
                }
            }
        }

        // Compare to destination data.

        else {
            // Transparent if dest is the same as the test color.

            if (fState.destTransPolarity == 0) {
                if (dest == fState.destTransColor) {
                    PRINTF_BLIT("	PrvTransparent:	dest == fState.destTransColor...");
                    return true;
                }
            }

            // Transparent if dest is different from the test color.

            else {
                if (dest != fState.destTransColor) {
                    PRINTF_BLIT("	PrvTransparent:	dest != fState.destTransColor...");
                    return true;
                }
            }
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvClipped
// ---------------------------------------------------------------------------

inline Bool EmRegsMediaQ11xx::PrvClipped(void) {
    if (fState.clipEnable) {
        if (fXDest < fState.clipLeft || fXDest >= fState.clipRight || fYDest < fState.clipTop ||
            fYDest >= fState.clipBottom) {
            PRINTF_BLIT("	PrvWriteIt:	clipped out...");

            return true;
        }
    }

    return false;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvLeadingPixels
// ---------------------------------------------------------------------------

uint16 EmRegsMediaQ11xx::PrvLeadingPixels(void) {
    uint16 result;

    if (fState.memToScreen) {
        // Packed Mode

        if (fState.monoSource) {
            result = fState.srcLeadingBits + 8 * fState.srcLeadingBytes;
        } else if (fState.colorDepth == kColorDepth8) {
            result = fState.srcLeadingBytes;
        } else if (fState.colorDepth == kColorDepth16) {
            result = fState.srcLeadingBytes / 2;
        } else {
            logging::printf("illegal source setup");
            result = 0;
        }
    } else {
        // Lined Mode

        if (fState.monoSource) {
            result = fState.srcBitOffset + 8 * fState.srcByteOffset;
        } else if (fState.colorDepth == kColorDepth8) {
            result = fState.srcByteOffset;
        } else if (fState.colorDepth == kColorDepth16) {
            result = fState.srcByteOffset / 2;
        } else {
            logging::printf("illegal source setup");
            result = 0;
        }
    }

    if (result >= 64) {
        logging::printf("illegal amount of leading pixels");
        result = 0;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvTrailingPixels
// ---------------------------------------------------------------------------

uint16 EmRegsMediaQ11xx::PrvTrailingPixels(void) {
    uint16 result;

    if (fState.memToScreen) {
        // Packed Mode

        if (fState.monoSource) {
            result = fState.srcTrailingBits + 8 * fState.srcTrailingBytes;
        } else if (fState.colorDepth == kColorDepth8) {
            result = fState.srcTrailingBytes;
        } else if (fState.colorDepth == kColorDepth16) {
            result = fState.srcTrailingBytes / 2;
        } else {
            logging::printf("illegal source setup");
            result = 0;
        }
    } else {
        // Lined Mode

        uint16 bytesPerLine;

        if (!fState.systemMemory || fState.srcEqualDestStride) {
            bytesPerLine = fState.destLineStride;
        } else {
            bytesPerLine = fState.srcLineStride;
        }

        uint16 pixelsPerLine;

        if (fState.monoSource) {
            pixelsPerLine = bytesPerLine * 8;
        } else if (fState.colorDepth == kColorDepth8) {
            pixelsPerLine = bytesPerLine;
        } else if (fState.colorDepth == kColorDepth16) {
            pixelsPerLine = bytesPerLine / 2;
        } else {
            logging::printf("illegal source setup");
            pixelsPerLine = 0;
        }

        result = pixelsPerLine - (fState.width + fLeadingSourcePixels);
    }

    if (result >= 64) {
        logging::printf("illegal amount of leading pixels");
        result = 0;
    }

    return result;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvUsesPattern
// ---------------------------------------------------------------------------
// Return whether or not the specified rasterOperation will require the use
// of a "pattern" value (that is, a pixel value from the pattern registers).
//
// To see if P is used in the output, consider the P, S, D combination
// table again:
//
//			 P	 S	 D	 O (output)
//			---	---	---	---
//			 0	 0	 0	 b0
//			 0	 0	 1	 b1
//			 0	 1	 0	 b2
//			 0	 1	 1	 b3
//			 1	 0	 0	 b4
//			 1	 0	 1	 b5
//			 1	 1	 0	 b6
//			 1	 1	 1	 b7
//
// If b0 == b4, b1 == b5, b2 == b6, and b3 == b7, then P has not figured into
// the result at all.  That is, for any S & D combination, we got the same
// output regardless of what P was.  Therefore, we can compare those bits to
// see if P is relevent to the outcome or not.
//
// We can quickly compare the bits by taking the ROP:
//
//		b7 b6 b5 b4 b3 b2 b1 b0
//
// Making a copy of it:
//
//		b7 b6 b5 b4 b3 b2 b1 b0
//		b7 b6 b5 b4 b3 b2 b1 b0
//
// Shifting the copy:
//
//		b7 b6 b5 b4 b3 b2 b1 b0
//		 0  0  0  0 b7 b6 b5 b4
//
// XORing the two:
//
//		(b7^0) (b6^0) (b5^0) (b4^0) (b3^b7) (b2^b6) (b1^b5) (b0^b4)
//
// And then masking out bits 4, 5, 6, and 7, as they don't include
// results that we want:
//
//		0 0 0 0 (b3^b7) (b2^b6) (b1^b5) (b0^b4)
//
// Here, we have a bitfield containing the results of comparing the bits
// that we're interested in.  We can test the whole result to see if it's
// zero or non-zero.

inline Bool EmRegsMediaQ11xx::PrvUsesPattern(void) {
    uint8 rop = fState.rasterOperation;
    uint8 shifted = rop >> 4;
    uint8 xored = rop ^ shifted;
    uint8 masked = xored & 0x0F;

    return masked != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvUsesSource
// ---------------------------------------------------------------------------
// Return whether or not the specified rasterOperation will require the use
// of a "source" value (that is, a pixel value from a source buffer).
//
// To see if S is used in the output, consider the P, S, D combination
// table again:
//
//			 P	 S	 D	 O (output)
//			---	---	---	---
//			 0	 0	 0	 b0
//			 0	 0	 1	 b1
//			 0	 1	 0	 b2
//			 0	 1	 1	 b3
//			 1	 0	 0	 b4
//			 1	 0	 1	 b5
//			 1	 1	 0	 b6
//			 1	 1	 1	 b7
//
// If b0 == b2, b1 == b3, b4 == b6, and b5 == b7, then S has not figured into
// the result at all.  That is, for any P & D combination, we got the same
// output regardless of what S was.  Therefore, we can compare those bits to
// see if S is relevent to the outcome or not.
//
// We can quickly compare the bits by taking the ROP:
//
//		b7 b6 b5 b4 b3 b2 b1 b0
//
// Making a copy of it:
//
//		b7 b6 b5 b4 b3 b2 b1 b0
//		b7 b6 b5 b4 b3 b2 b1 b0
//
// Shifting the copy:
//
//		b7 b6 b5 b4 b3 b2 b1 b0
//		 0  0 b7 b6 b5 b4 b3 b2
//
// XORing the two:
//
//		(b7^0) (b6^0) (b5^b7) (b4^b6) (b3^b5) (b2^b4) (b1^b3) (b0^b2)
//
// And then masking out bits 2, 3, 6, and 7, as they don't include
// results that we want:
//
//		0 0 (b5^b7) (b4^b6) 0 0 (b1^b3) (b0^b2)
//
// Here, we have a bitfield containing the results of comparing the bits
// that we're interested in.  We can test the whole result to see if it's
// zero or non-zero.

inline Bool EmRegsMediaQ11xx::PrvUsesSource(void) {
    uint8 rop = fState.rasterOperation;
    uint8 shifted = rop >> 2;
    uint8 xored = rop ^ shifted;
    uint8 masked = xored & 0x33;

    return masked != 0;
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvExpandMono8
// ---------------------------------------------------------------------------
// Take a bit pattern, and expand it into an array of color values, where
// elements of the array are set to the foreground color if the bit in the
// pattern is one, and the background color if the bit is zero.

inline void EmRegsMediaQ11xx::PrvExpandMono8(uint8 bits, uint16* results, uint16 fgColor,
                                             uint16 bgColor) {
    if (fState.monoSrcBitSwap) {
        for (int ii = 0; ii < 8; ++ii) {
            if ((bits & (1 << ii)) != 0) {
                results[ii] = fgColor;
            } else {
                results[ii] = bgColor;
            }
        }
    } else {
        for (int ii = 0; ii < 8; ++ii) {
            if ((bits & (0x080 >> ii)) != 0) {
                results[ii] = fgColor;
            } else {
                results[ii] = bgColor;
            }
        }
    }
}

// ---------------------------------------------------------------------------
//		� EmRegsMediaQ11xx::PrvExpandMono32
// ---------------------------------------------------------------------------
// Wrapper to perform the expansion of 4 8-bit bitfields in a DWORD.

inline void EmRegsMediaQ11xx::PrvExpandMono32(uint32 bits, uint16* results, uint16 fgColor,
                                              uint16 bgColor) {
    this->PrvExpandMono8(bits >> 0, &results[0], fgColor, bgColor);
    this->PrvExpandMono8(bits >> 8, &results[8], fgColor, bgColor);
    this->PrvExpandMono8(bits >> 16, &results[16], fgColor, bgColor);
    this->PrvExpandMono8(bits >> 24, &results[24], fgColor, bgColor);
}
