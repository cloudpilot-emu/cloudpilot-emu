-include ../Makefile.local

CC_NATIVE ?= gcc
CXX_NATIVE ?= g++
LD_NATIVE ?= g++

CC_EMCC ?= emcc
CXX_EMCC ?= emcc
LD_EMCC ?= emcc

CFLAGS_NATIVE ?= -O2  -g $(shell sdl2-config --cflags)
CXXFLAGS_NATIVE ?= $(CFLAGS_NATIVE)

INCLUDE_EXTRA ?=

LDFLAGS_NATIVE ?=  $(shell sdl2-config --libs)

CFLAGS_COMMON := \
	-Werror -Wextra -Wall -Wno-unused-parameter  \
	-Wno-visibility -Wno-constant-conversion -Wno-constant-logical-operand \
	-D_FILE_OFFSET_BITS=64

CXXFLAGS_COMMON := $(CFLAGS_COMMON) -std=c++17
CFLAGS_COMMON := $(CFLAGS_COMMON) -std=c11

BUILDDIR_NATIVE = .build
DEPDIR_NATIVE = .deps

DEPFLAGS_NATIVE = -MT $@ -MMD -MP -MF $(DEPDIR_NATIVE)/$*.d

MKDIR_NATIVE = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_NATIVE)/$(dir $<)

SOURCE_C = \
	CPU.c MMU.c cp15.c mem.c RAM.c ROM.c icache.c gdbstub.c vSD.c keys.c palmoscalls.c \
	socPXA.c pxa_IC.c pxa_MMC.c pxa_TIMR.c pxa_RTC.c pxa_UART.c pxa_PwrClk.c pxa_I2S.c pxa_GPIO.c \
	pxa_DMA.c pxa_LCD.c pxa_PWM.c pxa_AC97.c pxa_MemCtrl.c pxa_I2C.c pxa_SSP.c pxa255_UDC.c \
	pxa270_UDC.c pxa255_DSP.c pxa270_IMC.c pxa270_KPC.c pxa270_WMMX.c \
	devicePalmTungstenE2.c mmiodev_DirectNAND.c nand.c ac97dev_WM9712L.c

SOURCE_CXX = main_pc.cpp MainLoop.cpp

OBJECTS_NATIVE_C = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_NATIVE_CXX = $(SOURCE_CXX:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_NATIVE = $(OBJECTS_NATIVE_C) $(OBJECTS_NATIVE_CXX)

BINARY_NATIVE = uarm

GARBAGE = \
	$(BUILDDIR_NATIVE) \
	$(BINARY_NATIVE) \
	$(DEPDIR_NATIVE) \
	$(DEPDIR_EMCC)

bin: $(BINARY_NATIVE)

$(BINARY_NATIVE): $(OBJECTS_NATIVE)
	$(LD_NATIVE) -o $@ $^ $(LDFLAGS_NATIVE)

$(OBJECTS_NATIVE_C) : $(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR_NATIVE) && $(CC_NATIVE) $(DEPFLAGS_NATIVE) $(CFLAGS_COMMON) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(OBJECTS_NATIVE_CXX) : $(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR_NATIVE) && $(CXX_NATIVE) $(DEPFLAGS_NATIVE) $(CXXFLAGS_COMMON) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

clean:
	-rm -fr $(GARBAGE)

.PHONY: clean all bin
.SUFFIXES:
