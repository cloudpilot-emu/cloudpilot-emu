CC_NATIVE = gcc
CXX_NATIVE = g++
LD_NATIVE = g++

CC_EMCC = emcc
CXX_EMCC  = emcc
LD_EMCC = emcc

CFLAGS_COMMON = -Werror -Wextra -Wall -Wno-unused-parameter -Wno-pragma-pack
CXXFLAGS_COMMON = -std=c++11 -fno-exceptions

CFLAGS_NATIVE =  $(CFLAGS_COMMON) -O0 -g -fsanitize=address,undefined
CXXFLAGS_NATIVE = $(CFLAGS_NATIVE) $(CXXFLAGS_COMMON)
LDFLAGS_NATIVE = -fsanitize=address,undefined

CFLAGS_EMCC =  $(CFLAGS_COMMON) -O2
CXXFLAGS_EMCC= $(CFLAGS_EMCC) $(CXXFLAGS_COMMON)

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
	EmCommon.cpp \
	stubs.cpp \
	main.cpp

BUILDDIR_NATIVE = .build
BUILDDIR_EMCC = .build-emcc

OBJECTS_NATIVE = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_EMCC = $(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o)

BINARY_NATIVE = cloudpilot
BINARY_EMCC = cloudpilot.js
OTHER_EMCC = cloudpilot.wasm

MKDIR = mkdir -p $(dir $@)

all: bin emscripten

bin: $(BINARY_NATIVE)

emscripten: $(BINARY_EMCC)

$(BINARY_NATIVE): $(OBJECTS_NATIVE)
	$(LD_NATIVE) $(LDFLAGS_NATIVE) -o $@ $^

$(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR) && $(CC_NATIVE) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR) && $(CXX_NATIVE) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(BINARY_EMCC): $(OBJECTS_EMCC)
	$(LD_EMCC) $(LDFLAGS_EMCC) -o $@ $^

$(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o): $(BUILDDIR_EMCC)/%.o : %.c
	$(MKDIR) && $(CC_EMCC) $(CFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o): $(BUILDDIR_EMCC)/%.o : %.cpp
	$(MKDIR) && $(CXX_EMCC) $(CXXFLAGS_EMCC) $(INCLUDE) -c -o $@ $<


clean:
	-rm -fr $(BUILDDIR_NATIVE) $(BUILDDIR_EMCC) $(BINARY_EMCC) $(BINARY_NATIVE) $(OTHER_EMCC)

.PHONY: clean all bin emscripten
