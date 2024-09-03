-include ../Makefile.local

CC_NATIVE ?= gcc
CXX_NATIVE ?= g++
LD_NATIVE ?= g++

CC_EMCC ?= emcc
CXX_EMCC ?= emcc
LD_EMCC ?= emcc

WASMOPT = wasm-opt
WASMOPT_FLAGS_BASE = --enable-bulk-memory --inlining-optimizing \
	-fimfs 1000 -ifwl -pii 1000  -aimfs 10 -tnh -lmu -O4
WASMOPT_FLAGS_WEBKIT = --no-inline="*execFn_*"  $(WASMOPT_FLAGS_BASE) 
WASMOPT_FLAGS_OTHER = $(WASMOPT_FLAGS_BASE)

WASMSTRIP = wasm-strip
YARN = yarn

CFLAGS_NATIVE ?= -O3  -g $(shell sdl2-config --cflags) -flto
CXXFLAGS_NATIVE ?= $(CFLAGS_NATIVE)

CFLAGS_EMCC = -O3 -flto -fno-rtti -fno-exceptions -g
CXXFLAGS_EMCC = $(CFLAGS_EMCC)
INCLUDE_EXTRA ?=

CFLAGS_TEST ?= -O0 -g -fsanitize=address,undefined
CXXFLAGS_TEST ?= $(CFLAGS_TEST)
LDFLAGS_TEST ?= -fsanitize=address,undefined -lgtest -lgtest_main -lgmock

LDFLAGS_NATIVE ?=  $(shell sdl2-config --libs) -lSDL2_image -flto
LDFLAGS_EMCC = -O3 -Wno-version-check -flto -Wl,-u,fileno -g \
	-s EXIT_RUNTIME=0 \
	-s MODULARIZE=1 \
	-s ENVIRONMENT="web,worker" \
	-s EXPORT_NAME=createModule \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s INITIAL_MEMORY=314572800 \
	-s WASM_BIGINT=1 \
	-s ALLOW_TABLE_GROWTH=1 \
	-s FILESYSTEM=0 \
	-s EXPORTED_FUNCTIONS=["_malloc","_free","_main"] \
	-s BINARYEN_IGNORE_IMPLICIT_TRAPS=1 \
	-s EXTRA_EXPORTED_RUNTIME_METHODS=["callMain","cwrap","ccall","addFunction"]

CFLAGS_COMMON := \
	-Werror -Wextra -Wall -Wno-unused-parameter -Wno-unknown-pragmas \
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

BUILDDIR_TEST = .build-test
DEPDIR_TEST = .deps-test
DEPFLAGS_TEST = -MT $@ -MMD -MP -MF $(DEPDIR_TEST)/$*.d
MKDIR_TEST = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_TEST)/$(dir $<)

SOURCE_C = 						\
	util.c						\
	uarm/sdcard.c 				\
	uarm/patch_dispatch.c		\
	uarm/patches.c				\
	uarm/syscall.c				\
	uarm/syscall_dispatch.c		\
	uarm/MMU.c 					\
	uarm/cp15.c 				\
	uarm/mem.c 					\
	uarm/RAM.c 					\
	uarm/ROM.c 					\
	uarm/gdbstub.c				\
	uarm/vSD.c 					\
	uarm/keys.c 				\
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
	uarm/ac97dev_WM9712L.c		\
	uarm/pace_patch.c 			\
	uarm/pace.c 				\
	uarm/peephole.c				\
	uarm/uae/cpudefs.c 			\
	uarm/uae/cpuemu.c 			\
	uarm/uae/cpustbl.c 			\
	uarm/uae/readcpu.c 			\
	uarm/uae/newcpu.c

SOURCE_CXX_COMMON = 			\
	uarm/socPXA.cpp				\
	main.cpp					\
	MainLoop.cpp				\
	uarm/icache.cpp				\
	uarm/CPU.cpp				\
	uarm/memcpy.cpp				\
	uarm/audio_queue.cpp

SOURCE_CXX_NATIVE = 			\
	$(SOURCE_CXX_COMMON)		\
	Silkscreen.cpp				\
	SdlRenderer.cpp				\
	SdlEventHandler.cpp			\
	SdlAudioDriver.cpp

SOURCE_TEST = \
	test/scheduler.cpp \
	test/queue.cpp

OBJECTS_NATIVE_C = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_NATIVE_CXX = $(SOURCE_CXX_NATIVE:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_NATIVE = $(OBJECTS_NATIVE_C) $(OBJECTS_NATIVE_CXX)

OBJECTS_TEST_CXX = $(SOURCE_TEST:%.cpp=$(BUILDDIR_TEST)/%.o)
OBJECTS_TEST = $(OBJECTS_TEST_CXX)

OBJECTS_EMCC_C = $(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o)
OBJECTS_EMCC_CXX = $(SOURCE_CXX_COMMON:%.cpp=$(BUILDDIR_EMCC)/%.o)
OBJECTS_EMCC = $(OBJECTS_EMCC_C) $(OBJECTS_EMCC_CXX)

BINARY_NATIVE = uarm-bin
BINARY_EMCC = uarm_web.js
BINARY_WASM = uarm_web.wasm
OPTIMIZED_BINARIY_WASM_OTHER = uarm_web_other.wasm
OPTIMIZED_BINARIY_WASM_WEBKIT = uarm_web_webkit.wasm
OPTIMIZED_BINARIES_WASM = $(OPTIMIZED_BINARIY_WASM_OTHER) $(OPTIMIZED_BINARIY_WASM_WEBKIT)
BINARY_TEST = test/test

INCLUDE = $(INCLUDE_EXTRA)

GARBAGE = \
	$(BINARY_EMCC) \
	$(BINARY_NATIVE) \
	$(BINARY_WASM) \
	$(BINARY_WASM).s \
	$(OPTIMIZED_BINARIES_WASM) \
	$(BINARY_TEST) \
	$(BUILDDIR_NATIVE) \
	$(DEPDIR_NATIVE) \
	$(BUILDDIR_EMCC) \
	$(DEPDIR_EMCC) \
	$(BUILDDIR_TEST) \
	$(DEPDIR_TEST)

bin: $(BINARY_NATIVE)

test: $(BINARY_TEST)
	$(BINARY_TEST)

emscripten: $(OPTIMIZED_BINARIES_WASM)

$(BINARY_NATIVE): $(OBJECTS_NATIVE)
	$(LD_NATIVE) -o $@ $^ $(LDFLAGS_NATIVE)

$(BINARY_EMCC): $(OBJECTS_EMCC)
	$(LD_EMCC) -o $@ $^ $(LDFLAGS_EMCC)

$(BINARY_WASM): $(BINARY_EMCC)
	$(YARN) --cwd ../tools/build-jump-table && $(YARN) --cwd ../tools/build-jump-table build-jump-table "$$(pwd)/$@"

$(OPTIMIZED_BINARIY_WASM_OTHER): $(BINARY_WASM)
	$(WASMOPT) $(WASMOPT_FLAGS_OTHER) -o $@ $^
	$(WASMSTRIP) $@

$(OPTIMIZED_BINARIY_WASM_WEBKIT): $(BINARY_WASM)
	$(WASMOPT) $(WASMOPT_FLAGS_WEBKIT) -o $@ $^
	$(WASMSTRIP) $@

$(BINARY_TEST): $(OBJECTS_TEST)
	$(LD_NATIVE) -o $@ $^ $(LDFLAGS_TEST)

$(OBJECTS_NATIVE_C) : $(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR_NATIVE) && $(CC_NATIVE) $(DEPFLAGS_NATIVE) $(CFLAGS_COMMON) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(OBJECTS_NATIVE_CXX) : $(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR_NATIVE) && $(CXX_NATIVE) $(DEPFLAGS_NATIVE) $(CXXFLAGS_COMMON) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(OBJECTS_TEST_CXX) : $(BUILDDIR_TEST)/%.o : %.cpp
	$(MKDIR_TEST) && $(CXX_NATIVE) $(DEPFLAGS_TEST) $(CXXFLAGS_COMMON) $(CXXFLAGS_TEST) $(INCLUDE) -c -o $@ $<

$(OBJECTS_EMCC_C) : $(BUILDDIR_EMCC)/%.o : %.c
	$(MKDIR_EMCC) && $(CC_EMCC) $(DEPFLAGS_EMCC) $(CFLAGS_COMMON) $(CFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(OBJECTS_EMCC_CXX) : $(BUILDDIR_EMCC)/%.o : %.cpp
	$(MKDIR_EMCC) && $(CXX_EMCC) $(DEPFLAGS_EMCC) $(CXXFLAGS_COMMON) $(CXXFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

clean:
	-rm -fr $(GARBAGE)

.PHONY: clean all bin emscripten test
.SUFFIXES:


include $(shell test -e $(DEPDIR_NATIVE) && find $(DEPDIR_NATIVE) -type f)
include $(shell test -e $(DEPDIR_EMCC) && find $(DEPDIR_EMCC) -type f)
include $(shell test -e $(DEPDIR_TEST) && find $(DEPDIR_TEST) -type f)
