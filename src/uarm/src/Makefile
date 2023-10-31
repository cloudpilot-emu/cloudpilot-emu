-include ../Makefile.local

CC_NATIVE ?= gcc
CXX_NATIVE ?= g++
LD_NATIVE ?= g++

CC_EMCC ?= emcc
CXX_EMCC ?= emcc
LD_EMCC ?= emcc

CFLAGS_NATIVE ?= -O2  -g $(shell sdl2-config --cflags) -flto
CXXFLAGS_NATIVE ?= $(CFLAGS_NATIVE)

CFLAGS_EMCC = -O3 -flto -fno-rtti -fno-exceptions -s USE_SDL=2 -s USE_SDL_IMAGE=2
CXXFLAGS_EMCC = $(CFLAGS_EMCC)

INCLUDE_EXTRA ?=

LDFLAGS_NATIVE ?=  $(shell sdl2-config --libs) -lSDL2_image -flto
LDFLAGS_EMCC = -O3 -Wno-version-check -flto -Wl,-u,fileno \
	-s EXIT_RUNTIME=0 \
	-s MODULARIZE=1 \
	-s EXPORT_NAME=createModule \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s EXPORTED_FUNCTIONS=["_malloc","_free","_main"] \
	-s EXPORTED_RUNTIME_METHODS=["FS"] \
	-s EXTRA_EXPORTED_RUNTIME_METHODS=["callMain","cwrap","ccall"]

CFLAGS_COMMON := \
	-Werror -Wextra -Wall -Wno-unused-parameter \
	-Wno-visibility -Wno-constant-conversion -Wno-constant-logical-operand \
	-D_FILE_OFFSET_BITS=64 -Iuarm -I.

CXXFLAGS_COMMON := $(CFLAGS_COMMON) -std=c++17
CFLAGS_COMMON := $(CFLAGS_COMMON) -std=gnu11

BUILDDIR_NATIVE = .build
DEPDIR_NATIVE = .deps
DEPFLAGS_NATIVE = -MT $@ -MMD -MP -MF $(DEPDIR_NATIVE)/$*.d
MKDIR_NATIVE = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_NATIVE)/$(dir $<)

BUILDDIR_EMCC = .build-emcc
DEPDIR_EMCC = .deps-emcc
DEPFLAGS_EMCC = -MT $@ -MMD -MP -MF $(DEPDIR_EMCC)/$*.d
MKDIR_EMCC = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_EMCC)/$(dir $<)

SOURCE_C = 						\
	util.c						\
	uarm/CPU.c 					\
	uarm/patch_dispatch.c		\
	uarm/syscall.c				\
	uarm/MMU.c 					\
	uarm/cp15.c 				\
	uarm/mem.c 					\
	uarm/RAM.c 					\
	uarm/ROM.c 					\
	uarm/icache.c 				\
	uarm/gdbstub.c				\
	uarm/vSD.c 					\
	uarm/keys.c 				\
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

SOURCE_CXX_COMMON = 			\
	main.cpp					\
	MainLoop.cpp

SOURCE_CXX_NATIVE = 			\
	$(SOURCE_CXX_COMMON)		\
	Silkscreen.cpp				\
	SdlRenderer.cpp				\
	SdlEventHandler.cpp

OBJECTS_NATIVE_C = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_NATIVE_CXX = $(SOURCE_CXX_NATIVE:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_NATIVE = $(OBJECTS_NATIVE_C) $(OBJECTS_NATIVE_CXX)

OBJECTS_EMCC_C = $(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o)
OBJECTS_EMCC_CXX = $(SOURCE_CXX_COMMON:%.cpp=$(BUILDDIR_EMCC)/%.o)
OBJECTS_EMCC = $(OBJECTS_EMCC_C) $(OBJECTS_EMCC_CXX)

BINARY_NATIVE = uarm-bin
BINARY_EMCC = uarm_web.js
BINARY_WASM = uarm_web.wasm

GARBAGE = \
	$(BINARY_EMCC) \
	$(BINARY_NATIVE) \
	$(BINARY_WASM) \
	$(BUILDDIR_NATIVE) \
	$(BINARY_NATIVE) \
	$(DEPDIR_NATIVE) \
	$(BUILDDIR_EMCC) \
	$(BINARY_ENCC) \
	$(DEPDIR_EMCC)

bin: $(BINARY_NATIVE)

emscripten: $(BINARY_EMCC)

$(BINARY_NATIVE): $(OBJECTS_NATIVE)
	$(LD_NATIVE) -o $@ $^ $(LDFLAGS_NATIVE)

$(BINARY_EMCC): $(OBJECTS_EMCC)
	$(LD_EMCC) -o $@ $^ $(LDFLAGS_EMCC)

$(OBJECTS_NATIVE_C) : $(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR_NATIVE) && $(CC_NATIVE) $(DEPFLAGS_NATIVE) $(CFLAGS_COMMON) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(OBJECTS_NATIVE_CXX) : $(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR_NATIVE) && $(CXX_NATIVE) $(DEPFLAGS_NATIVE) $(CXXFLAGS_COMMON) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(OBJECTS_EMCC_C) : $(BUILDDIR_EMCC)/%.o : %.c
	$(MKDIR_EMCC) && $(CC_EMCC) $(DEPFLAGS_EMCC) $(CFLAGS_COMMON) $(CFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(OBJECTS_EMCC_CXX) : $(BUILDDIR_EMCC)/%.o : %.cpp
	$(MKDIR_EMCC) && $(CXX_EMCC) $(DEPFLAGS_EMCC) $(CXXFLAGS_COMMON) $(CXXFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

clean:
	-rm -fr $(GARBAGE)

.PHONY: clean all bin emscripten
.SUFFIXES:


include $(shell test -e $(DEPDIR_NATIVE) && find $(DEPDIR_NATIVE) -type f)
include $(shell test -e $(DEPDIR_EMCC) && find $(DEPDIR_EMCC) -type f)
