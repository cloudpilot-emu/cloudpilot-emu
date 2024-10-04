-include ../Makefile.local

CC_NATIVE ?= gcc
CXX_NATIVE ?= g++
LD_NATIVE ?= g++

CC_EMCC ?= emcc
CXX_EMCC ?= emcc
LD_EMCC ?= emcc

DEVELOP = 

WASMOPT = wasm-opt
WASMOPT_FLAGS_BASE = --no-inline="*mmuTranslate*" --enable-bulk-memory --inlining-optimizing \
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
	-D_FILE_OFFSET_BITS=64 -I./src/uarm -I./src

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

SOURCE_C = 							\
	src/cputil.c						\
	src/uarm/sdcard.c 				\
	src/uarm/patch_dispatch.c		\
	src/uarm/patches.c				\
	src/uarm/syscall.c				\
	src/uarm/syscall_dispatch.c		\
	src/uarm/MMU.c 					\
	src/uarm/cp15.c 				\
	src/uarm/mem.c 					\
	src/uarm/ram_buffer.c			\
	src/uarm/RAM.c 					\
	src/uarm/ROM.c 					\
	src/uarm/gdbstub.c				\
	src/uarm/vSD.c 					\
	src/uarm/keys.c 				\
	src/uarm/pxa_IC.c 				\
	src/uarm/pxa_MMC.c 				\
	src/uarm/pxa_TIMR.c 			\
	src/uarm/pxa_RTC.c 				\
	src/uarm/pxa_UART.c 			\
	src/uarm/pxa_PwrClk.c 			\
	src/uarm/pxa_I2S.c 				\
	src/uarm/pxa_GPIO.c 			\
	src/uarm/pxa_DMA.c 				\
	src/uarm/pxa_LCD.c 				\
	src/uarm/pxa_PWM.c 				\
	src/uarm/pxa_AC97.c 			\
	src/uarm/pxa_MemCtrl.c 			\
	src/uarm/pxa_I2C.c 				\
	src/uarm/pxa_SSP.c 				\
	src/uarm/pxa255_UDC.c 			\
	src/uarm/pxa270_UDC.c 			\
	src/uarm/pxa255_DSP.c 			\
	src/uarm/pxa270_IMC.c 			\
	src/uarm/pxa270_KPC.c 			\
	src/uarm/pxa270_WMMX.c 			\
	src/uarm/devicePalmTungstenE2.c \
	src/uarm/mmiodev_DirectNAND.c 	\
	src/uarm/nand.c 				\
	src/uarm/ac97dev_WM9712L.c		\
	src/uarm/pace_patch.c 			\
	src/uarm/pace.c 				\
	src/uarm/peephole.c				\
	src/uarm/uae/cpudefs.c 			\
	src/uarm/uae/cpuemu.c 			\
	src/uarm/uae/cpustbl.c 			\
	src/uarm/uae/readcpu.c 			\
	src/uarm/uae/newcpu.c

SOURCE_CXX_COMMON = 				\
	src/uarm/socPXA.cpp				\
	src/main.cpp					\
	src/MainLoop.cpp				\
	src/uarm/icache.cpp				\
	src/uarm/CPU.cpp				\
	src/uarm/memcpy.cpp				\
	src/uarm/audio_queue.cpp

SOURCE_CXX_NATIVE = 				\
	$(SOURCE_CXX_COMMON)			\
	src/Silkscreen.cpp				\
	src/SdlRenderer.cpp				\
	src/SdlEventHandler.cpp			\
	src/SdlAudioDriver.cpp

SOURCE_TEST = 				\
	src/test/scheduler.cpp 	\
	src/test/queue.cpp

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
	if test -z "$(DEVELOP)"; then $(YARN) --cwd ./tools/build-jump-table && $(YARN) --cwd ./tools/build-jump-table build-jump-table "$$(pwd)/$@"; fi

$(OPTIMIZED_BINARIY_WASM_OTHER): $(BINARY_WASM)
	if test -n "$(DEVELOP)"; then cp $^ $@; else $(WASMOPT) $(WASMOPT_FLAGS_OTHER) -o $@ $^; fi
	if test -z "$(DEVELOP)"; then $(WASMSTRIP) $@; fi

$(OPTIMIZED_BINARIY_WASM_WEBKIT): $(BINARY_WASM)
	if test -n "$(DEVELOP)"; then cp $^ $@; else $(WASMOPT) $(WASMOPT_FLAGS_WEBKIT) -o $@ $^; fi
	if test -z "$(DEVELOP)"; then $(WASMSTRIP) $@; fi

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
