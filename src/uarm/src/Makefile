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
	-D_FILE_OFFSET_BITS=64 -Iuarm -I.

CXXFLAGS_COMMON := $(CFLAGS_COMMON) -std=c++17
CFLAGS_COMMON := $(CFLAGS_COMMON) -std=c11

BUILDDIR_NATIVE = .build
DEPDIR_NATIVE = .deps

DEPFLAGS_NATIVE = -MT $@ -MMD -MP -MF $(DEPDIR_NATIVE)/$*.d

MKDIR_NATIVE = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_NATIVE)/$(dir $<)

SOURCE_C = 						\
	util.c						\
	uarm/CPU.c 					\
	uarm/MMU.c 					\
	uarm/cp15.c 				\
	uarm/mem.c 					\
	uarm/RAM.c 					\
	uarm/ROM.c 					\
	uarm/icache.c 				\
	uarm/gdbstub.c				\
	uarm/vSD.c 					\
	uarm/keys.c 				\
	uarm/palmoscalls.c 			\
	uarm/socPXA.c 				\
	uarm/pxa_IC.c 				\
	uarm/pxa_MMC.c 				\
	uarm/pxa_TIMR.c 			\
	uarm/pxa_RTC.c 				\
	uarm/pxa_UART.c 			\
	uarm/pxa_PwrClk.c 			\
	uarm/pxa_I2S.c 				\
	uarm/pxa_GPIO.c 			\
	uarm/pxa_DMA.c 				\
	uarm/pxa_LCD.c 				\
	uarm/pxa_PWM.c 				\
	uarm/pxa_AC97.c 			\
	uarm/pxa_MemCtrl.c 			\
	uarm/pxa_I2C.c 				\
	uarm/pxa_SSP.c 				\
	uarm/pxa255_UDC.c 			\
	uarm/pxa270_UDC.c 			\
	uarm/pxa255_DSP.c 			\
	uarm/pxa270_IMC.c 			\
	uarm/pxa270_KPC.c 			\
	uarm/pxa270_WMMX.c 			\
	uarm/devicePalmTungstenE2.c \
	uarm/mmiodev_DirectNAND.c 	\
	uarm/nand.c 				\
	uarm/ac97dev_WM9712L.c

SOURCE_CXX = 					\
	main.cpp					\
	MainLoop.cpp				\
	SdlRenderer.cpp

OBJECTS_NATIVE_C = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_NATIVE_CXX = $(SOURCE_CXX:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_NATIVE = $(OBJECTS_NATIVE_C) $(OBJECTS_NATIVE_CXX)

BINARY_NATIVE = uarm-bin

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
