CC_NATIVE = gcc
CXX_NATIVE = g++
LD_NATIVE = g++

CC_EMCC = emcc
CXX_EMCC  = emcc
LD_EMCC = emcc

CFLAGS_COMMON = -Werror -Wextra -Wall -Wno-unused-parameter -Wno-pragma-pack \
	-DEMULATION_LEVEL=EMULATION_UNIX
CXXFLAGS_COMMON = -std=c++14 -fno-exceptions

CFLAGS_NATIVE = $(CFLAGS_COMMON) -O0 -fsanitize=address,undefined -g $(shell sdl2-config --cflags)
CXXFLAGS_NATIVE = $(CFLAGS_NATIVE) $(CXXFLAGS_COMMON)
LDFLAGS_NATIVE =  -fsanitize=address,undefined $(shell sdl2-config --libs) -lSDL2_image

CFLAGS_EMCC =  $(CFLAGS_COMMON) -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2
CXXFLAGS_EMCC= $(CFLAGS_EMCC) $(CXXFLAGS_COMMON)
LDFLAGS_EMCC = -Wno-version-check \
	-s EXIT_RUNTIME=1 \
	-s SDL2_IMAGE_FORMATS='["png"]' \
	-s USE_SDL=2 \
	-s USE_SDL_IMAGE=2 \
	-s MODULARIZE=1 \
	-s EXPORT_NAME=createModule \
	-s EXPORTED_RUNTIME_METHODS=["FS"] \
	-s EXTRA_EXPORTED_RUNTIME_METHODS=["callMain","cwrap"]

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
	emulator/ROMStubs.cpp \
	emulator/Logging.cpp \
	emulator/stubs.cpp

SOURCE_SDL = \
	sdl/main.cpp \
	sdl/MainLoop.cpp \
	sdl/Silkscreen.cpp \
	sdl/EventHandler.cpp

OBJECTS_NATIVE = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_EMCC = $(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o)

OBJECTS_SDL_NATIVE = $(SOURCE_SDL:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_SDL_EMCC = $(SOURCE_SDL:%.cpp=$(BUILDDIR_EMCC)/%.o)

BINARY_SDL_NATIVE = cloudpilot
BINARY_SDL_EMCC = cloudpilot.js
BINARY_SDL_WASM = cloudpilot.wasm
WASM_OPT_STAMP = wasm-opt.stamp

GARBAGE = \
	$(BUILDDIR_NATIVE) \
	$(BUILDDIR_EMCC) \
	$(BINARY_SDL_EMCC) \
	$(BINARY_SDL_NATIVE) \
	$(BINARY_SDL_WASM) \
	$(DEPDIR_NATIVE) \
	$(DEPDIR_EMCC) \
	$(WASM_OPT_STAMP)

all: bin emscripten

bin: $(BINARY_SDL_NATIVE)

emscripten: $(BINARY_SDL_EMCC)

wasm-opt: $(WASM_OPT_STAMP)

$(WASM_OPT_STAMP): $(BINARY_SDL_EMCC)
	wasm-opt -O4 --post-emscripten -o $(BINARY_SDL_WASM) $(BINARY_SDL_WASM)
	terser -c -o $(BINARY_SDL_EMCC) < $(BINARY_SDL_EMCC)
	touch $@

$(BINARY_SDL_NATIVE): $(OBJECTS_NATIVE) $(OBJECTS_SDL_NATIVE)
	$(LD_NATIVE) $(LDFLAGS_NATIVE) -o $@ $^

$(BINARY_SDL_EMCC): $(OBJECTS_EMCC) $(OBJECTS_SDL_EMCC)
	$(LD_EMCC) $(LDFLAGS_EMCC) -o $@ $^

$(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR_NATIVE) && $(CC_NATIVE) $(DEPFLAGS_NATIVE) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_SDL:%.cpp=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR_NATIVE) && $(CXX_NATIVE) $(DEPFLAGS_NATIVE) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o): $(BUILDDIR_EMCC)/%.o : %.c
	$(MKDIR_EMCC) && $(CC_EMCC) $(DEPFLAGS_EMCC) $(CFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o) $(SOURCE_SDL:%.cpp=$(BUILDDIR_EMCC)/%.o): $(BUILDDIR_EMCC)/%.o : %.cpp
	$(MKDIR_EMCC) && $(CXX_EMCC) $(DEPFLAGS_EMCC) $(CXXFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

clean:
	-rm -fr $(GARBAGE)

.PHONY: clean all bin emscripten

include $(shell test -e $(DEPDIR_NATIVE) && find $(DEPDIR_NATIVE) -type f)
include $(shell test -e $(DEPDIR_EMCC) && find $(DEPDIR_EMCC) -type f)
