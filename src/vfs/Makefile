-include ../Makefile.local

CC_NATIVE ?= gcc
CXX_NATIVE ?= g++
LD_NATIVE ?= g++

CC_EMCC ?= emcc
CXX_EMCC ?= emcc
LD_EMCC ?= emcc

PYTHON ?= python3

EMCC_OPT ?= -O3 -flto

WEBIDL_BINDER ?= $(PYTHON) $${EMSDK}/upstream/emscripten/tools/webidl_binder.py

CFLAGS_NATIVE ?= -O2  -g

CXXFLAGS_NATIVE ?= $(CFLAGS_NATIVE)

INCLUDE_EXTRA ?=

LDFLAGS_NATIVE ?=

CFLAGS_COMMON := -Werror -Wextra -Wall -Wno-unused-parameter -Wno-deprecated-declarations -Wno-sign-compare
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
	-s ALLOW_TABLE_GROWTH=1 \
	-s EXPORTED_FUNCTIONS=["_malloc","_free","_main"] \
	-s INITIAL_MEMORY=39976960

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

INCLUDE = -I../common -I../common/zip -I. $(INCLUDE_EXTRA)

INCLUDE_NATIVE = -I../argparse

SOURCE_C = \
	fatfs/ff.c \
	fatfs/ffsystem.c \
	fatfs/ffunicode.c \
	fatfs/fattime.c

SOURCE_CPP = \
	fatfs/diskio.cpp \
	ReaddirContext.cpp \
	FileEntry.cpp \
	CreateZipContext.cpp

SOURCE_TEST =

SOURCE_NATIVE = $(SOURCE_CPP) \
	native/main.cpp \
	native/VfsCli.cpp

SOURCE_WEB = $(SOURCE_CPP) $(WEBIDL_BINDING_CXX) \
	web/Vfs.cpp \
	web/main.cpp

OBJECTS_TEST = \
	$(SOURCE_C:%.c=$(BUILDDIR_TEST)/%.o) \
	$(SOURCE_TEST:%.cpp=$(BUILDDIR_TEST)/%.o) \
	../common/libcommon.a

OBJECTS_NATIVE = \
	$(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o) \
	$(SOURCE_NATIVE:%.cpp=$(BUILDDIR_NATIVE)/%.o) \
	../common/libcommon.a

OBJECTS_WEB_EMCC = \
	$(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o) \
	$(SOURCE_WEB:%.cpp=$(BUILDDIR_EMCC)/%.o) \
	../common/libcommon-wasm.a

BINARY_NATIVE = vfs

BINARY_WEB_EMCC = vfs_web.js
BINARY_WEB_WASM = vfs_web.wasm
BINARY_TEST =

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
	$(WEBIDL_BINDING_JS) \
	$(WEBIDL_BINDING_JS:%.js=%.cpp) \
	parser.out \
	WebIDLGrammar.pkl

bin: $(BINARY_NATIVE)

emscripten: $(BINARY_WEB_EMCC)

test: $(BINARY_TEST)

$(BINARY_NATIVE): $(OBJECTS_NATIVE)
	$(LD_NATIVE) -o $@ $^ $(LDFLAGS_NATIVE)

$(BINARY_WEB_EMCC): $(OBJECTS_WEB_EMCC)
	$(LD_EMCC) -o $@ $^ $(LDFLAGS_EMCC_WEB)

$(BINARY_TEST) : $(OBJECTS_TEST)
	$(LD_NATIVE) -o $@ $^ $(LDFLAGS_TEST)

$(BUILDDIR_NATIVE)/%.o : %.c
	$(MKDIR_NATIVE) && $(CC_NATIVE) $(DEPFLAGS_NATIVE) $(CFLAGS_COMMON) $(CFLAGS_NATIVE) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_EMCC)/%.o : %.c
	$(MKDIR_EMCC) && $(CC_EMCC) $(DEPFLAGS_EMCC) $(CFLAGS_COMMON) $(CFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_TEST)/%.o : %.c
	$(MKDIR_TEST) && $(CC_NATIVE) $(DEPFLAGS_TEST) $(CFLAGS_COMMON) $(CFLAGS_TEST) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_NATIVE)/%.o : %.cpp
	$(MKDIR_NATIVE) && $(CXX_NATIVE) $(DEPFLAGS_NATIVE) $(CXXFLAGS_COMMON) $(CXXFLAGS_NATIVE) $(INCLUDE) $(INCLUDE_NATIVE) -c -o $@ $<

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

