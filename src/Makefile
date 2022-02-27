-include Makefile.local

CC_NATIVE ?= gcc
CXX_NATIVE ?= g++
LD_NATIVE ?= g++

CC_EMCC ?= emcc
CXX_EMCC ?= emcc
LD_EMCC ?= emcc

PYTHON ?= python3

EMCC_OPT ?= -O3 -flto

WEBIDL_BINDER ?= $(PYTHON) $${EMSDK}/upstream/emscripten/tools/webidl_binder.py

CFLAGS_NATIVE ?= \
	-O2  -g \
	$(shell sdl2-config --cflags)

CXXFLAGS_NATIVE ?= $(CFLAGS_NATIVE)

INCLUDE_EXTRA ?=

LDFLAGS_NATIVE ?=  \
	$(shell sdl2-config --libs) -lSDL2_image \
	-lreadline -lboost_coroutine -lboost_json

CFLAGS_COMMON := -Werror -Wextra -Wall -Wno-unused-parameter -Wno-pragma-pack \
	-DEMULATION_LEVEL=EMULATION_UNIX
CXXFLAGS_COMMON := $(CFLAGS_COMMON) -std=c++17
CFLAGS_COMMON := $(CFLAGS_COMMON) -std=c99

CFLAGS_EMCC = $(EMCC_OPT) -D_POSIX_SOURCE
CXXFLAGS_EMCC = $(CFLAGS_EMCC)  -fno-rtti -fno-exceptions

CFLAGS_TEST ?= -O0 -g -fsanitize=address,undefined -fsanitize-blacklist=clang_blacklist.txt
CXXFLAGS_TEST ?= $(CFLAGS_TEST)
LDFLAGS_TEST ?= -fsanitize=address,undefined -fsanitize-blacklist=clang_blacklist.txt -lgtest -lgmock

WEBIDL_BINDING_DIR = web/binding
WEBIDL_BINDING_JS = $(WEBIDL_BINDING_DIR)/binding.js
WEBIDL_BINDING_SRC = $(WEBIDL_BINDING_DIR)/binding.idl
WEBIDL_BINDING_CXX = web/binding.cpp

LDFLAGS_EMCC_WEB = $(EMCC_OPT) -Wno-version-check \
	--post-js $(WEBIDL_BINDING_JS) \
	-s NO_EXIT_RUNTIME=1 \
	-s MODULARIZE=1 \
	-s EXPORT_NAME=createModule \
	-s ENVIRONMENT="web,worker" \
	-s ALLOW_MEMORY_GROWTH=1 \
	-s INITIAL_MEMORY=67108864 \
	-s ALLOW_TABLE_GROWTH=1 \
	-s EXPORTED_FUNCTIONS=["_malloc","_free","_main"] \
	-s EXTRA_EXPORTED_RUNTIME_METHODS=["ccall","addFunction","UTF8ToString"]

BUILDDIR_NATIVE = .build
DEPDIR_NATIVE = .deps

BUILDDIR_EMCC = .build-emcc
DEPDIR_EMCC = .deps-emcc

BUILDDIR_TEST = .build-test
DEPDIR_TEST = .deps-test

DEPFLAGS_NATIVE = -MT $@ -MMD -MP -MF $(DEPDIR_NATIVE)/$*.d
DEPFLAGS_EMCC = -MT $@ -MMD -MP -MF $(DEPDIR_EMCC)/$*.d
DEPFLAGS_TEST = -MT $@ -MMD -MP -MF $(DEPDIR_TEST)/$*.d

MKDIR_NATIVE = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_NATIVE)/$(dir $<)
MKDIR_EMCC = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_EMCC)/$(dir $<)
MKDIR_TEST = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_TEST)/$(dir $<)

INCLUDE = \
	-I./emulator/hardware \
	-I./emulator/hardware/clie \
	-I./emulator/hardware/handera \
	-I./emulator/uae \
	-I./emulator/patch \
	-I./emulator/patch/clie \
	-I./emulator/savestate \
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
	-I./emulator/suspend \
	-I./emulator \
	-I./zip \
	-I./nanopb \
	-I./proto \
	$(INCLUDE_EXTRA)

SOURCE_C = \
	emulator/uae/cpudefs.c \
	emulator/uae/cpuemu.c \
	emulator/uae/cpustbl.c \
	emulator/palm/Platform/Core/System/Src/Crc.c \
	nanopb/pb_common.c \
	nanopb/pb_encode.c \
	nanopb/pb_decode.c \
	proto/networking.pb.c \
	zip/zip.c

SOURCE_CPP = \
	$(shell find . -type f -name '*.cpp' -and -not -name $(notdir $(WEBIDL_BINDING_CXX)) | sed 's/^\.\///g') \
	$(WEBIDL_BINDING_CXX)

SOURCE_EMU = \
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
	emulator/hardware/EmRegsVZ.cpp \
	emulator/hardware/EmRegsSZ.cpp \
	emulator/hardware/EmSPISlaveADS784x.cpp \
	emulator/hardware/EmRegsUSBPhilipsPDIUSBD12.cpp \
	emulator/hardware/EmRegsSED1376.cpp \
	emulator/hardware/EmRegsSED1375.cpp \
	emulator/hardware/EmRegsFrameBuffer.cpp \
	emulator/hardware/EmRegsVZPalmM500.cpp \
	emulator/hardware/EmRegsVZPalmM505.cpp \
	emulator/hardware/EmRegsEZPalmV.cpp \
	emulator/hardware/EmRegsEZPalmIIIc.cpp \
	emulator/hardware/EmRegsMediaQ11xx.cpp \
	emulator/hardware/EmRegsVZPalmM125.cpp \
	emulator/hardware/EmRegsVZPalmM130.cpp \
	emulator/hardware/EmRegsVZPalmI705.cpp \
	emulator/hardware/EmRegsPLDPalmI705.cpp \
	emulator/hardware/EmRegsEZPalmM100.cpp \
	emulator/hardware/EmRegsPLDAtlantiC.cpp \
	emulator/hardware/EmRegsVZAtlantiC.cpp \
	emulator/hardware/EmRegsPLDPacifiC.cpp \
	emulator/hardware/EmRegsMediaQ11xxPacifiC.cpp \
	emulator/hardware/EmRegs328.cpp \
	emulator/hardware/EmRegs328PalmPilot.cpp \
	emulator/hardware/EmRegsEZPalmVIIx.cpp \
	emulator/hardware/EmRegsEZPalmVII.cpp \
	emulator/hardware/EmRegsPLDPalmVIIEZ.cpp \
	emulator/hardware/EmBankROM.cpp \
	emulator/hardware/EmUAEGlue.cpp \
	emulator/hardware/clie/EmRegsExpCardCLIE.cpp \
	emulator/hardware/clie/EmRegsUsbCLIE.cpp \
	emulator/hardware/clie/EmRegsFMSound.cpp \
	emulator/hardware/clie/EmRegsEzPegS300.cpp \
	emulator/hardware/clie/EmRegsEzPegS500C.cpp \
	emulator/hardware/clie/EmRegsVZPegNasca.cpp \
	emulator/hardware/clie/EmRegsVZPegVenice.cpp \
	emulator/hardware/clie/EmRegsVZPegYellowStone.cpp \
	emulator/hardware/clie/EmRegsVZPegModena.cpp \
	emulator/hardware/clie/EmRegsSZNaples.cpp \
	emulator/hardware/clie/EmRegsSZRedwood.cpp \
	emulator/hardware/clie/EmRegsVZPegN700C.cpp \
	emulator/hardware/clie/EmRegsUsbPegN700C.cpp \
	emulator/hardware/clie/EmRegsLCDCtrlT2.cpp \
	emulator/hardware/handera/EmRegs330CPLD.cpp \
	emulator/hardware/handera/EmRegsVZHandEra330.cpp \
	emulator/hardware/handera/EmSPISlave330Current.cpp \
	emulator/hardware/handera/EmTRGATA.cpp \
	emulator/hardware/handera/EmTRGCF.cpp \
	emulator/hardware/handera/EmTRGCFIO.cpp \
	emulator/hardware/handera/EmTRGCFMem.cpp \
	emulator/hardware/handera/EmTRGDiskIO.cpp \
	emulator/hardware/handera/EmTRGSD.cpp \
	emulator/patch/EmPatchModule.cpp \
	emulator/patch/EmPatchModuleSys.cpp \
	emulator/patch/EmPatchModuleHtal.cpp \
	emulator/patch/PatchModuleNetlib.cpp \
	emulator/patch/clie/PatchModuleClieSlotDriver.cpp \
	emulator/patch/clie/PatchModuleClieStubAll.cpp \
	emulator/patch/EmPatchMgr.cpp \
	emulator/savestate/Chunk.cpp \
	emulator/savestate/ChunkProbe.cpp \
	emulator/savestate/SavestateProbe.cpp \
	emulator/savestate/SavestateLoader.cpp \
	emulator/savestate/Savestate.cpp \
	emulator/suspend/SuspendManager.cpp \
	emulator/suspend/SuspendContext.cpp \
	emulator/suspend/SuspendContextClipboardPaste.cpp \
	emulator/suspend/SuspendContextClipboardCopy.cpp \
	emulator/suspend/SuspendContextNetworkConnect.cpp \
	emulator/suspend/SuspendContextNetworkRpc.cpp \
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
	emulator/SessionImage.cpp \
	emulator/DbBackup.cpp \
	emulator/CallbackManager.cpp \
	emulator/DbInstaller.cpp \
	emulator/Fifo.cpp \
	emulator/Feature.cpp \
	emulator/ScreenDimensions.cpp \
	emulator/NetworkProxy.cpp \
	emulator/ZipfileWalker.cpp

SOURCE_TEST = \
	$(SOURCE_EMU) \
	emulator/assert_native.cpp \
	emulator/stacktrace.cpp \
	test/SavestateChunk.cpp \
	test/SavestateChunkProbe.cpp \
	test/SavestateProbe.cpp \
	test/Savestate.cpp \
	test/SavestateLoader.cpp \
	test/SaveChunkHelper.cpp \
	test/LoadChunkHelper.cpp \
	test/Fifo.cpp \
	test/Miscellaneous.cpp \
	test/main.cpp

SOURCE_NATIVE = \
	$(SOURCE_EMU) \
	native/main.cpp \
	native/MainLoop.cpp \
	native/Silkscreen.cpp \
	native/util.cpp \
	native/EventHandler.cpp \
	native/Cli.cpp \
	native/ProxyClient.cpp \
	native/ProxyHandler.cpp \
	emulator/assert_native.cpp \
	emulator/stacktrace.cpp

SOURCE_WEB = \
	$(SOURCE_EMU) \
	emulator/assert_emscripten.cpp \
	web/main.cpp \
	web/Cloudpilot.cpp \
	web/RomInfo.cpp \
	$(WEBIDL_BINDING_CXX)

OBJECTS_TEST = $(SOURCE_C:%.c=$(BUILDDIR_TEST)/%.o) $(SOURCE_TEST:%.cpp=$(BUILDDIR_TEST)/%.o)
OBJECTS_NATIVE = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_NATIVE:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_WEB_EMCC = $(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o) $(SOURCE_WEB:%.cpp=$(BUILDDIR_EMCC)/%.o)

BINARY_NATIVE = cloudpilot

BINARY_WEB_EMCC = cloudpilot_web.js
BINARY_WEB_WASM = cloudpilot_web.wasm
BINARY_TEST = test/test

WASM_OPT_STAMP = wasm-opt.stamp

GARBAGE = \
	$(BUILDDIR_NATIVE) \
	$(BUILDDIR_EMCC) \
	$(BUILDDIR_TEST) \
	$(BINARY_NATIVE) \
	$(BINARY_WEB_EMCC) \
	$(BINARY_WEB_WASM) \
	$(BINARY_TEST) \
	$(DEPDIR_NATIVE) \
	$(DEPDIR_EMCC) \
	$(DEPDIR_TEST) \
	$(WASM_OPT_STAMP) \
	$(WEBIDL_BINDING_JS) \
	$(WEBIDL_BINDING_JS:%.js=%.cpp) \
	parser.out \
	WebIDLGrammar.pkl

all: bin emscripten

bin: $(BINARY_NATIVE)

emscripten: $(BINARY_WEB_EMCC)

test: $(BINARY_TEST)
	$(BINARY_TEST)

$(BINARY_NATIVE): $(OBJECTS_NATIVE)
	$(LD_NATIVE) $(LDFLAGS_NATIVE) -o $@ $^

$(BINARY_WEB_EMCC): $(OBJECTS_WEB_EMCC)
	$(LD_EMCC) $(LDFLAGS_EMCC_WEB) -o $@ $^

$(BINARY_TEST) : $(OBJECTS_TEST)
	$(LD_NATIVE) $(LDFLAGS_TEST) -o $@ $^

$(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR_NATIVE) && $(CC_NATIVE) $(DEPFLAGS_NATIVE) $(CFLAGS_COMMON) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_EMCC)/%.o : %.c
	$(MKDIR_EMCC) && $(CC_EMCC) $(DEPFLAGS_EMCC) $(CFLAGS_COMMON) $(CFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_TEST)/%.o : %.c
	$(MKDIR_TEST) && $(CC_NATIVE) $(DEPFLAGS_TEST) $(CFLAGS_COMMON) $(CFLAGS_TEST) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR_NATIVE) && $(CXX_NATIVE) $(DEPFLAGS_NATIVE) $(CXXFLAGS_COMMON) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_EMCC)/%.o : %.cpp
	$(MKDIR_EMCC) && $(CXX_EMCC) $(DEPFLAGS_EMCC) $(CXXFLAGS_COMMON) $(CXXFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_TEST)/%.o : %.cpp
	$(MKDIR_TEST) && $(CXX_NATIVE) $(DEPFLAGS_TEST) $(CXXFLAGS_COMMON) $(CXXFLAGS_TEST) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_EMCC)/$(WEBIDL_BINDING_CXX:%.cpp=%.o): $(WEBIDL_BINDING_JS)

$(WEBIDL_BINDING_JS): $(WEBIDL_BINDING_SRC)
	$(WEBIDL_BINDER) $(WEBIDL_BINDING_SRC) $(WEBIDL_BINDING_JS:.js=)

clean:
	-rm -fr $(GARBAGE)

.PHONY: clean all bin emscripten test
.SUFFIXES:

include $(shell test -e $(DEPDIR_NATIVE) && find $(DEPDIR_NATIVE) -type f)
include $(shell test -e $(DEPDIR_EMCC) && find $(DEPDIR_EMCC) -type f)
include $(shell test -e $(DEPDIR_TEST) && find $(DEPDIR_TEST) -type f)

