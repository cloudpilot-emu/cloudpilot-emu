-include ../Makefile.local

CFLAGS_COMMON := -Wextra -Wall -Wno-unused-parameter -Wno-deprecated-declarations -Wno-sign-compare

EMCC_INITIAL_MEMORY = 16777216

INCLUDE = -I../common -I../argparse -I.

WEBIDL_BINDING_SRC = \
	$(WEBIDL_BINDING_DIR)/fstools.idl \
	../common/web/gunzip.idl  \
	../common/web/gzip.idl \
	../common/web/create_zip.idl \
	../common/web/zipfile_walker.idl

SOURCE_C = \
	dosfstools/charconv.c \
	dosfstools/common.c \
	dosfstools/io.c \
	dosfstools/device_info.c \
	dosfstools/boot.c \
	dosfstools/check.c \
	dosfstools/fat.c \
	dosfstools/file.c \
	dosfstools/fsck.fat.c \
	dosfstools/lfn.c \
	dosfstools/mkfs.fat.c \
	dosfstools/mkfs.c \
	dosfstools/fsck.c

SOURCE_CXX = card_io.cpp fstools_util.cpp

SOURCE_C_NATIVE = $(SOURCE_C)
SOURCE_CXX_NATIVE = $(SOURCE_CXX) \
	native/main.cpp \
	native/CmdFsck.cpp \
	native/CmdMkfs.cpp \
	native/terminate.cpp

SOURCE_C_EMCC = $(SOURCE_C)
SOURCE_CXX_EMCC = \
	$(SOURCE_CXX) \
	web/FSTools.cpp \
	web/MkfsContext.cpp \
	web/FsckContext.cpp \
	web/terminate.cpp \
	web/main.cpp

OBJECTS_EXTRA_NATIVE = ../common/libcommon.a
OBJECTS_EXTRA_TEST = ../common/libcommon.a
OBJECTS_EXTRA_EMCC = ../common/libcommon-wasm.a 

BINARY_NATIVE = fstools
BINARY_EMCC = fstools_web.wasm

include ../Makefile.common

bin: $(BINARY_NATIVE)

emscripten: $(BINARY_EMCC)

test:

.PHONY: bin emscripten clean test