CC_NATIVE = gcc
CXX_NATIVE = g++
LD_NATIVE = g++

CC_EMCC = emcc
CXX_EMCC  = emcc
LD_EMCC = emcc

CFLAGS_COMMON = -Werror -Wextra -Wall -Wno-unused-parameter -Wno-pragma-pack
CXXFLAGS_COMMON = -std=c++14 -fno-exceptions

CFLAGS_NATIVE =  $(CFLAGS_COMMON) -O0 -g -fsanitize=address,undefined
CXXFLAGS_NATIVE = $(CFLAGS_NATIVE) $(CXXFLAGS_COMMON)
LDFLAGS_NATIVE = -fsanitize=address,undefined

CFLAGS_EMCC =  $(CFLAGS_COMMON) -O2
CXXFLAGS_EMCC= $(CFLAGS_EMCC) $(CXXFLAGS_COMMON)
LDFLAGS_EMCC = -Wno-version-check -s EXIT_RUNTIME=1 -s NODERAWFS=1

BUILDDIR_NATIVE = .build
DEPDIR_NATIVE = .deps

BUILDDIR_EMCC = .build-emcc
DEPDIR_EMCC = .deps-emcc

DEPFLAGS_NATIVE = -MT $@ -MMD -MP -MF $(DEPDIR_NATIVE)/$*.d
DEPFLAGS_EMCC = -MT $@ -MMD -MP -MF $(DEPDIR_EMCC)/$*.d

MKDIR_NATIVE = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_NATIVE)/$(dir $<)
MKDIR_EMCC = mkdir -p $(dir $@) && mkdir -p $(DEPDIR_EMCC)/$(dir $<)

INCLUDE = \
	-I./hardware \
	-I./uae \
	-I./palm/Device \
	-I./palm/Platform \
	-I./palm/Platform/Core/Hardware/IncsPrv \
	-I./palm/Platform/Core/System/IncsPrv \
	-I./palm/Platform/Incs \
	-I./palm/Platform/Incs/Core \
	-I./palm/Platform/Incs/Core/Hardware \
	-I./palm/Platform/Incs/Core/System \
	-I./palm/Platform/Incs/Core/UI \
	-I./palm/Platform/Incs/Libraries \
	-I.

SOURCE_C = \
	uae/cpudefs.c \
	uae/cpuemu.c \
	uae/cpustbl.c

SOURCE_CPP = \
	uae/readcpu.cpp \
	hardware/EmMemory.cpp \
	hardware/EmHAL.cpp \
	hardware/EmCPU.cpp \
	hardware/EmCPU68K.cpp \
	hardware/EmRegs.cpp \
	hardware/EmBankRegs.cpp \
	hardware/EmSPISlave.cpp \
	hardware/EmUARTDragonball.cpp \
	hardware/EmRegsEZ.cpp \
	hardware/EmSPISlaveADS784x.cpp \
	hardware/EmRegsEZPalmV.cpp \
	Platform.cpp \
	EmDevice.cpp \
	EmLowMem.cpp \
	Frame.cpp \
	EmPoint.cpp \
	EmThreadSafeQueue.cpp \
	EmTransport.cpp \
	MetaMemory.cpp \
	Byteswapping.cpp \
	DebugMgr.cpp \
	EmSession.cpp \
	EmPalmStructs.cpp \
	EmROMReader.cpp \
	EmCommon.cpp \
	StringData.cpp \
	Logging.cpp \
	stubs.cpp \
	main.cpp

OBJECTS_NATIVE = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_EMCC = $(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o)

BINARY_NATIVE = cloudpilot
BINARY_EMCC = cloudpilot.js
OTHER_EMCC = cloudpilot.wasm

all: bin emscripten

bin: $(BINARY_NATIVE)

emscripten: $(BINARY_EMCC)

$(BINARY_NATIVE): $(OBJECTS_NATIVE)
	$(LD_NATIVE) $(LDFLAGS_NATIVE) -o $@ $^

$(BINARY_EMCC): $(OBJECTS_EMCC)
	$(LD_EMCC) $(LDFLAGS_EMCC) -o $@ $^

$(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR_NATIVE) && $(CC_NATIVE) $(DEPFLAGS_NATIVE) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR_NATIVE) && $(CXX_NATIVE) $(DEPFLAGS_NATIVE) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o): $(BUILDDIR_EMCC)/%.o : %.c
	$(MKDIR_EMCC) && $(CC_EMCC) $(DEPFLAGS_EMCC) $(CFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o): $(BUILDDIR_EMCC)/%.o : %.cpp
	$(MKDIR_EMCC) && $(CXX_EMCC) $(DEPFLAGS_EMCC) $(CXXFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

clean:
	-rm -fr $(BUILDDIR_NATIVE) $(BUILDDIR_EMCC) $(BINARY_EMCC) $(BINARY_NATIVE) $(OTHER_EMCC) $(DEPDIR_NATIVE) $(DEPDIR_EMCC)

.PHONY: clean all bin emscripten

include $(shell test -e $(DEPDIR_NATIVE) && find $(DEPDIR_NATIVE) -type f)
include $(shell test -e $(DEPDIR_EMCC) && find $(DEPDIR_EMCC) -type f)
