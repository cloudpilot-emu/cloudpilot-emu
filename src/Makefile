-include Makefile.local

CC_NATIVE ?= gcc
CXX_NATIVE ?= g++
LD_NATIVE ?= g++

CC_EMCC ?= emcc
CXX_EMCC ?= emcc
LD_EMCC ?= emcc

PYTHON ?= python

WEBIDL_BINDER ?= $(PYTHON) $${EMSDK}/upstream/emscripten/tools/webidl_binder.py

CFLAGS_NATIVE ?= \
	-O2  -g \
	$(shell sdl2-config --cflags)

CXXFLAGS_NATIVE ?= $(CFLAGS_NATIVE)

LDFLAGS_NATIVE ?=  \
	$(shell sdl2-config --libs) -lSDL2_image \
	-lreadline

CFLAGS_COMMON = -Werror -Wextra -Wall -Wno-unused-parameter -Wno-pragma-pack \
	-DEMULATION_LEVEL=EMULATION_UNIX
CXXFLAGS_COMMON = $(CFLAGS_COMMON) -std=c++14 -fno-exceptions

CFLAGS_EMCC = -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2
CXXFLAGS_EMCC = $(CFLAGS_EMCC)

WEBIDL_BINDING_DIR = web/binding
WEBIDL_BINDING_JS = $(WEBIDL_BINDING_DIR)/binding.js
WEBIDL_BINDING_SRC = $(WEBIDL_BINDING_DIR)/binding.idl
WEBIDL_BINDING_CXX = web/binding.cpp

LDFLAGS_EMCC_SDL = -Wno-version-check \
	-s EXIT_RUNTIME=1 \
	-s SDL2_IMAGE_FORMATS='["png"]' \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-s MODULARIZE=1 \
	-s EXPORT_NAME=createModule \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s EXPORTED_FUNCTIONS=["_malloc","_free","_main"] \
	-s EXPORTED_RUNTIME_METHODS=["FS"] \
	-s EXTRA_EXPORTED_RUNTIME_METHODS=["callMain","cwrap","ccall"]

LDFLAGS_EMCC_WEB = -Wno-version-check \
	--post-js $(WEBIDL_BINDING_JS) \
	-s NO_EXIT_RUNTIME=1 \
	-s MODULARIZE=1 \
	-s EXPORT_NAME=createModule \
	-s EXPORT_ES6=1 \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s EXTRA_EXPORTED_RUNTIME_METHODS=["ccall"]

BUILDDIR_NATIVE = .build
DEPDIR_NATIVE = .deps

BUILDDIR_EMCC = .build-emcc
DEPDIR_EMCC = .deps-emcc

DEPFLAGS_NATIVE = -MT $@ -MMD -MP -MF $(DEPDIR_NATIVE)/$*.d
DEPFLAGS_EMCC = -MT $@ -MMD -MP -MF $(DEPDIR_EMCC)/$*.d

MKDIR_NATIVE = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_NATIVE)/$(dir $<)
MKDIR_EMCC = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_EMCC)/$(dir $<)

INCLUDE = \
	-I./emulator/hardware \
	-I./emulator/uae \
	-I./emulator/patch \
	-I./emulator/palm/Device \
	-I./emulator/palm/Platform \
	-I./emulator/palm/Platform/Core/Hardware/IncsPrv \
	-I./emulator/palm/Platform/Core/System/IncsPrv \
	-I./emulator/palm/Platform/Incs \
	-I./emulator/palm/Platform/Incs/Core \
	-I./emulator/palm/Platform/Incs/Core/Hardware \
	-I./emulator/palm/Platform/Incs/Core/System \
	-I./emulator/palm/Platform/Incs/Core/UI \
	-I./emulator/palm/Platform/Incs/Libraries \
	-I./emulator

SOURCE_C = \
	emulator/uae/cpudefs.c \
	emulator/uae/cpuemu.c \
	emulator/uae/cpustbl.c \
	emulator/palm/Platform/Core/System/Src/Crc.c

SOURCE_CPP = \
	emulator/uae/readcpu.cpp \
	emulator/hardware/EmMemory.cpp \
	emulator/hardware/EmHAL.cpp \
	emulator/hardware/EmCPU.cpp \
	emulator/hardware/EmCPU68K.cpp \
	emulator/hardware/EmBankSRAM.cpp \
	emulator/hardware/EmBankDRAM.cpp \
	emulator/hardware/EmBankDummy.cpp \
	emulator/hardware/EmBankMapped.cpp \
	emulator/hardware/EmRegs.cpp \
	emulator/hardware/EmBankRegs.cpp \
	emulator/hardware/EmSPISlave.cpp \
	emulator/hardware/EmUARTDragonball.cpp \
	emulator/hardware/EmRegsEZ.cpp \
	emulator/hardware/EmSPISlaveADS784x.cpp \
	emulator/hardware/EmRegsEZPalmV.cpp \
	emulator/hardware/EmBankROM.cpp \
	emulator/hardware/EmUAEGlue.cpp \
	emulator/patch/EmPatchModule.cpp \
	emulator/patch/EmPatchModuleSys.cpp \
	emulator/patch/EmPatchModuleHtal.cpp \
	emulator/patch/EmPatchMgr.cpp \
	emulator/EmSystemState.cpp \
	emulator/Miscellaneous.cpp \
	emulator/Marshall.cpp \
	emulator/Platform.cpp \
	emulator/EmDevice.cpp \
	emulator/EmLowMem.cpp \
	emulator/EmPalmOS.cpp \
	emulator/EmSubroutine.cpp \
	emulator/Frame.cpp \
	emulator/EmPoint.cpp \
	emulator/EmThreadSafeQueue.cpp \
	emulator/EmTransport.cpp \
	emulator/MetaMemory.cpp \
	emulator/Byteswapping.cpp \
	emulator/DebugMgr.cpp \
	emulator/EmSession.cpp \
	emulator/EmPalmStructs.cpp \
	emulator/EmROMReader.cpp \
	emulator/EmCommon.cpp \
	emulator/StringData.cpp \
	emulator/EmFileImport.cpp \
	emulator/ROMStubs.cpp \
	emulator/Logging.cpp \
	emulator/stubs.cpp

SOURCE_SDL = \
	sdl/main.cpp \
	sdl/MainLoop.cpp \
	sdl/Silkscreen.cpp \
	sdl/EventHandler.cpp

SOURCE_SDL_NATIVE = $(SOURCE_SDL) sdl/Cli.cpp
SOURCE_SDL_EMCC = $(SOURCE_SDL) sdl/EmscriptenApi.cpp

SOURCE_WEB = \
	web/main.cpp \
	web/Cloudpilot.cpp \
	$(WEBIDL_BINDING_CXX)

OBJECTS_NATIVE = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_EMCC = $(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o)

OBJECTS_SDL_NATIVE = $(SOURCE_SDL_NATIVE:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_SDL_EMCC = $(SOURCE_SDL_EMCC:%.cpp=$(BUILDDIR_EMCC)/%.o)

OBJECTS_WEB_EMCC = $(SOURCE_WEB:%.cpp=$(BUILDDIR_EMCC)/%.o)

BINARY_SDL_NATIVE = cloudpilot
BINARY_SDL_EMCC = cloudpilot.js
BINARY_SDL_WASM = cloudpilot.wasm

BINARY_WEB_EMCC = cloudpilot_web.js
BINARY_WEB_WASM = cloudpilot_web.wasm

WASM_OPT_STAMP = wasm-opt.stamp

GARBAGE = \
	$(BUILDDIR_NATIVE) \
	$(BUILDDIR_EMCC) \
	$(BINARY_SDL_EMCC) \
	$(BINARY_SDL_NATIVE) \
	$(BINARY_SDL_WASM) \
	$(BINARY_WEB_EMCC) \
	$(BINARY_WEB_WASM) \
	$(DEPDIR_NATIVE) \
	$(DEPDIR_EMCC) \
	$(WASM_OPT_STAMP) \
	$(WEBIDL_BINDING_JS) \
	$(WEBIDL_BINDING_JS:%.js=%.cpp) \
	parser.out \
	WebIDLGrammar.pkl

all: bin emscripten

bin: $(BINARY_SDL_NATIVE)

emscripten: $(BINARY_SDL_EMCC) $(BINARY_WEB_EMCC)

wasm-opt: $(WASM_OPT_STAMP)

$(WASM_OPT_STAMP): $(BINARY_SDL_EMCC) $(BINARY_WEB_EMCC)
	for i in $(BINARY_SDL_WASM) $(BINARY_WEB_WASM); do wasm-opt -O4 --post-emscripten -o $$i $$i; done
	for i in $(BINARY_SDL_EMCC) $(BINARY_WEB_EMCC); do terser -c -o $$i < $$i; done
	touch $@

$(BINARY_SDL_NATIVE): $(OBJECTS_NATIVE) $(OBJECTS_SDL_NATIVE)
	$(LD_NATIVE) $(LDFLAGS_NATIVE) -o $@ $^

$(BINARY_SDL_EMCC): $(OBJECTS_EMCC) $(OBJECTS_SDL_EMCC)
	$(LD_EMCC) $(LDFLAGS_EMCC_SDL) -o $@ $^

$(BINARY_WEB_EMCC): $(OBJECTS_EMCC) $(OBJECTS_WEB_EMCC)
	$(LD_EMCC) $(LDFLAGS_EMCC_WEB) -o $@ $^

$(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR_NATIVE) && $(CC_NATIVE) $(DEPFLAGS_NATIVE) $(CFLAGS_COMMON) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_SDL_NATIVE:%.cpp=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR_NATIVE) && $(CXX_NATIVE) $(DEPFLAGS_NATIVE) $(CXXFLAGS_COMMON) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o): $(BUILDDIR_EMCC)/%.o : %.c
	$(MKDIR_EMCC) && $(CC_EMCC) $(DEPFLAGS_EMCC) $(CFLAGS_COMMON) $(CFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o) $(SOURCE_SDL_EMCC:%.cpp=$(BUILDDIR_EMCC)/%.o) $(SOURCE_WEB:%.cpp=$(BUILDDIR_EMCC)/%.o): $(BUILDDIR_EMCC)/%.o : %.cpp
	$(MKDIR_EMCC) && $(CXX_EMCC) $(DEPFLAGS_EMCC) $(CXXFLAGS_COMMON) $(CXXFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_EMCC)/$(WEBIDL_BINDING_CXX:%.cpp=%.o): $(WEBIDL_BINDING_JS)

$(WEBIDL_BINDING_JS): $(WEBIDL_BINDING_SRC)
	$(WEBIDL_BINDER) $(WEBIDL_BINDING_SRC) $(WEBIDL_BINDING_JS:.js=)

clean:
	-rm -fr $(GARBAGE)

.PHONY: clean all bin emscripten

include $(shell test -e $(DEPDIR_NATIVE) && find $(DEPDIR_NATIVE) -type f)
include $(shell test -e $(DEPDIR_EMCC) && find $(DEPDIR_EMCC) -type f)
