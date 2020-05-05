CC_NATIVE = gcc
CXX_NATIVE = g++
LD_NATIVE = g++

CFLAGS_NATIVE = -Wextra -Wall -Wno-unused-parameter -Wno-pragma-pack -O0 -g -fsanitize=address,undefined
CXXFLAGS_NATIVE = $(CFLAGS_NATIVE)
LDFLAGS_NATIVE = -fsanitize=address,undefined

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

OBJECTS_NATIVE = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o)

BINARY_NATIVE = cloudpilot

MKDIR = mkdir -p $(dir $@)

all: bin

bin: $(BINARY_NATIVE)

$(BINARY_NATIVE): $(OBJECTS_NATIVE)
	$(LD_NATIVE) $(LDFLAGS_NATIVE) -o $@ $^

$(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR) && $(CC_NATIVE) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o): $(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR) && $(CXX_NATIVE) $(CXXFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

clean:
	-rm -fr $(BUILDDIR_NATIVE)

.PHONY: clean all bin
