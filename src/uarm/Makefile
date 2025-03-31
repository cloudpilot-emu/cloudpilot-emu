-include ../Makefile.local

CFLAGS_COMMON := \
	-Wextra -Wall -Wno-unused-parameter -Wno-unknown-pragmas \
	-Wno-visibility -Wno-constant-conversion -Wno-constant-logical-operand \
	-D_FILE_OFFSET_BITS=64

CFLAGS_EMCC_EXTRA = -g

LDFLAGS_TEST_EXTRA ?= -lgtest_main

LDFLAGS_EMCC = -O3 -Wno-version-check -flto -Wl,-u,fileno -g \
	--post-js $(WEBIDL_BINDING_JS) \
	-s EXIT_RUNTIME=0 \
	-s MODULARIZE=1 \
	-s ENVIRONMENT="web,worker" \
	-s EXPORT_NAME=createModule \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s INITIAL_MEMORY=33554432 \
	-s WASM_BIGINT=1 \
	-s ALLOW_TABLE_GROWTH=1 \
	-s FILESYSTEM=0 \
	-s EXPORTED_FUNCTIONS=["_malloc","_free","_main"] \
	-s BINARYEN_IGNORE_IMPLICIT_TRAPS=1 \
	-s EXTRA_EXPORTED_RUNTIME_METHODS=["callMain","cwrap","ccall","addFunction"]

WEBIDL_BINDING_SRC = $(WEBIDL_BINDING_DIR)/uarm.idl

INCLUDE = -I./uarm -I. -I../common -I../argparse

STD_C ?= -std=gnu11

WASMOPT = wasm-opt
WASMOPT_FLAGS_BASE = --no-inline="*mmuTranslate*" --enable-bulk-memory --enable-bulk-memory-opt  --enable-sign-ext --enable-nontrapping-float-to-int  --inlining-optimizing \
	-fimfs 1000 -ifwl -pii 1000  -aimfs 10 -tnh -lmu -O4
WASMOPT_FLAGS_WEBKIT = --no-inline="*execFn_*" --no-inline="*cpuCycleArm*" --no-inline="*cpuCycleThumb*" $(WASMOPT_FLAGS_BASE) 
WASMOPT_FLAGS_OTHER = --no-inline="*cpuCycle" $(WASMOPT_FLAGS_BASE)

WASMSTRIP = wasm-strip
YARN = yarn

SOURCE_C = 								\
	cputil.c							\
	uarm/sdcard.c 						\
	uarm/patches.c						\
	uarm/syscall.c						\
	uarm/syscall_dispatch.c				\
	uarm/mem.c 							\
	uarm/ram_buffer.c					\
	uarm/RAM.c 							\
	uarm/ROM.c 							\
	uarm/gdbstub.c						\
	uarm/vSD.c 							\
	uarm/keys.c 						\
	uarm/pxa_MMC.c 						\
	uarm/pxa_RTC.c 						\
	uarm/pxa_UART.c 					\
	uarm/pxa_PwrClk.c 					\
	uarm/pxa_I2S.c 						\
	uarm/pxa_GPIO.c 					\
	uarm/pxa_DMA.c 						\
	uarm/pxa_PWM.c 						\
	uarm/pxa_AC97.c 					\
	uarm/pxa_MemCtrl.c 					\
	uarm/pxa_I2C.c 						\
	uarm/pxa_SSP.c 						\
	uarm/pxa255_UDC.c 					\
	uarm/pxa270_UDC.c 					\
	uarm/pxa255_DSP.c 					\
	uarm/pxa270_IMC.c 					\
	uarm/pxa270_KPC.c 					\
	uarm/pxa270_WMMX.c 					\
	uarm/devicePalmTungstenE2.c 		\
	uarm/mmiodev_DirectNAND.c 			\
	uarm/nand.c 						\
	uarm/ac97dev_WM9712L.c				\
	uarm/pace_patch.c 					\
	uarm/peephole.c						\
	uarm/uae/cpudefs.c 					\
	uarm/uae/cpuemu.c 					\
	uarm/uae/cpustbl.c 					\
	uarm/uae/readcpu.c 					\
	uarm/uae/newcpu.c

SOURCE_CXX =		 					\
	uarm/socPXA.cpp						\
	MainLoop.cpp						\
	uarm/patch_dispatch.cpp				\
	uarm/icache.cpp						\
	uarm/CPU.cpp						\
	uarm/pace.cpp 						\
	uarm/MMU.cpp						\
	uarm/pxa_TIMR.cpp 					\
	uarm/pxa_IC.cpp						\
	uarm/pxa_LCD.cpp					\
	uarm/cp15.cpp 						\
	uarm/memcpy.cpp						\
	uarm/audio_queue.cpp				\
	uarm/rom_info.cpp					\
	uarm/savestate/rle.cpp				\
	uarm/savestate/SessionFile.cpp

SOURCE_C_NATIVE = $(SOURCE_C)
SOURCE_CXX_NATIVE =						\
	$(SOURCE_CXX)						\
	native/Silkscreen.cpp				\
	native/SdlRenderer.cpp				\
	native/SdlEventHandler.cpp			\
	native/SdlAudioDriver.cpp			\
	native/Commands.cpp					\
	native/main.cpp

SOURCE_C_EMCC = $(SOURCE_C)
SOURCE_CXX_EMCC = 						\
	$(SOURCE_CXX)						\
	web/main.cpp

SOURCE_C_TEST = 						\
	$(SOURCE_C) 						\
	test/stub.c
SOURCE_CXX_TEST =						\
	$(SOURCE_CXX)						\
	test/scheduler.cpp 					\
	test/queue.cpp

OBJECTS_EXTRA_NATIVE = ../common/libcommon.a
OBJECTS_EXTRA_TEST = ../common/libcommon.a
OBJECTS_EXTRA_EMCC = ../common/libcommon-wasm.a

BINARY_NATIVE = cp-uarm
BINARY_EMCC = uarm_web.wasm
BINARY_TEST = test/test

PROCESSED_BINARY_WASM = uarm_web_processed.wasm
OPTIMIZED_BINARIY_WASM_OTHER = uarm_web_other.wasm
OPTIMIZED_BINARIY_WASM_WEBKIT = uarm_web_webkit.wasm
OPTIMIZED_BINARIES_WASM = $(OPTIMIZED_BINARIY_WASM_OTHER) $(OPTIMIZED_BINARIY_WASM_WEBKIT)

GARBAGE_EXTRA = 						\
	$(PROCESSED_BINARY_WASM)			\
	$(PROCESSED_BINARY_WASM).s			\
	$(OPTIMIZED_BINARIES_WASM)

include ../Makefile.common

bin: $(BINARY_NATIVE)

test: $(BINARY_TEST)
	$(BINARY_TEST)

emscripten: $(OPTIMIZED_BINARIES_WASM)

$(PROCESSED_BINARY_WASM): $(BINARY_EMCC)
	if test -z "$(DEVELOP)"; then $(YARN) --cwd ./tools/build-jump-table && $(YARN) --cwd ./tools/build-jump-table build-jump-table "$$(pwd)/$^" "$$(pwd)/$@"; fi

$(OPTIMIZED_BINARIY_WASM_OTHER): $(PROCESSED_BINARY_WASM)
	if test -n "$(DEVELOP)"; then cp $^ $@; else $(WASMOPT) $(WASMOPT_FLAGS_OTHER) -o $@ $^; fi
	if test -z "$(DEVELOP)"; then $(WASMSTRIP) $@; fi

$(OPTIMIZED_BINARIY_WASM_WEBKIT): $(PROCESSED_BINARY_WASM)
	if test -n "$(DEVELOP)"; then cp $^ $@; else $(WASMOPT) $(WASMOPT_FLAGS_WEBKIT) -o $@ $^; fi
	if test -z "$(DEVELOP)"; then $(WASMSTRIP) $@; fi

.PHONY: bin emscripten clean test