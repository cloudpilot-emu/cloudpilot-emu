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
	-s BINARYEN_IGNORE_IMPLICIT_TRAPS=1 \
	-s EXPORTED_RUNTIME_METHODS=["wasmTable","callMain","HEAPU8","HEAPU32"]

WEBIDL_BINDING_SRC = $(WEBIDL_BINDING_DIR)/uarm.idl

INCLUDE = -I./uarm -I. -I../common -I../common/zip -I../argparse

STD_C ?= -std=gnu11

WASMOPT = wasm-opt
WASMOPT_FLAGS = --enable-bulk-memory --enable-bulk-memory-opt  --enable-sign-ext --enable-nontrapping-float-to-int  --inlining-optimizing \
	-fimfs 1000 -ifwl -pii 1000  -aimfs 10 -tnh -lmu -O4

WASMSTRIP = wasm-strip
YARN = yarn

SOURCE_C = 								\
	cputil.c							\
	uarm/sdcard.c 						\
	uarm/patches.c						\
	uarm/syscall.c						\
	uarm/mem.c 							\
	uarm/memory_buffer.c				\
	uarm/RAM.c 							\
	uarm/ROM.c 							\
	uarm/gdbstub.c						\
	uarm/keys.c 						\
	uarm/timeutil.c						\
	uarm/pxa270_UDC.c 					\
	uarm/pxa270_IMC.c 					\
	uarm/pxa270_KPC.c 					\
	uarm/pxa270_WMMX.c 					\
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
	uarm/pxa_DMA.cpp					\
	uarm/pxa_UART.cpp 					\
	uarm/pxa_SSP.cpp					\
	uarm/pxa_GPIO.cpp 					\
	uarm/pxa_AC97.cpp 					\
	uarm/pxa_I2S.cpp					\
	uarm/pxa_I2C.cpp					\
	uarm/pxa_MemCtrl.cpp				\
	uarm/pxa_PwrClk.cpp 				\
	uarm/pxa_PWM.cpp					\
	uarm/pxa_MMC.cpp					\
	uarm/pxa_RTC.cpp					\
	uarm/pxa255_DSP.cpp					\
	uarm/pxa255_UDC.cpp					\
	uarm/ac97dev_WM9712L.cpp			\
	uarm/nand.cpp 						\
	uarm/mmiodev_DirectNAND.cpp			\
	uarm/vSD.cpp						\
	uarm/devicePalmTungstenE2.cpp 		\
	uarm/cp15.cpp 						\
	uarm/memcpy.cpp						\
	uarm/syscall_dispatch.cpp			\
	uarm/db_installer.cpp				\
	uarm/audio_queue.cpp				\
	uarm/rom_info.cpp					\
	uarm/db_list.cpp					\
	uarm/db_backup.cpp					\
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
	web/main.cpp						\
	web/uarm.cpp						\
	web/bridge.cpp

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
OPTIMIZED_BINARY_WASM = uarm_web_optimized.wasm

GARBAGE_EXTRA = 						\
	$(PROCESSED_BINARY_WASM)			\
	$(PROCESSED_BINARY_WASM).s			\
	$(OPTIMIZED_BINARY_WASM)

include ../Makefile.common

bin: $(BINARY_NATIVE)

test: $(BINARY_TEST)
	$(BINARY_TEST)

emscripten: $(OPTIMIZED_BINARY_WASM)

$(PROCESSED_BINARY_WASM): $(BINARY_EMCC)
	if test -z "$(DEVELOP)"; then $(YARN) --cwd ./tools/build-jump-table && $(YARN) --cwd ./tools/build-jump-table build-jump-table "$$(pwd)/$^" "$$(pwd)/$@"; fi

$(OPTIMIZED_BINARY_WASM): $(PROCESSED_BINARY_WASM)
	if test -n "$(DEVELOP)"; then cp $^ $@; else $(WASMOPT) $(WASMOPT_FLAGS) -o $@ $^; fi
	if test -z "$(DEVELOP)"; then $(WASMSTRIP) $@; fi

.PHONY: bin emscripten clean test