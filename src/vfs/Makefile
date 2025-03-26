-include ../Makefile.local

EMCC_INITIAL_MEMORY = 16777216

CFLAGS_COMMON := -Wextra -Wall -Wno-unused-parameter -Wno-deprecated-declarations -Wno-sign-compare

LDFLAGS_TEST_EXTRA = -lgtest_main

INCLUDE = -I../common -I../common/zip -I../argparse -I.

WEBIDL_BINDING_SRC = $(WEBIDL_BINDING_DIR)/vfs.idl

SOURCE_C = \
	fatfs/ff.c \
	fatfs/ffsystem.c \
	fatfs/ffunicode.c \
	fatfs/fattime.c

SOURCE_CXX = \
	fatfs/diskio.cpp \
	VfsUtil.cpp \
	ReaddirContext.cpp \
	FileEntry.cpp \
	ExportZipContext.cpp \
	DeleteRecursiveContext.cpp \
	UnzipContext.cpp \
	FatfsDelegate.cpp \
	ZipfileIterator.cpp \
	GenericCopyContext.cpp \
	RecursiveFsIterator.cpp \
	PasteContext.cpp

SOURCE_C_NATIVE = $(SOURCE_C)
SOURCE_CXX_NATIVE = \
	$(SOURCE_CXX) \
	native/main.cpp \
	native/VfsCli.cpp

SOURCE_C_EMCC = $(SOURCE_C)
SOURCE_CXX_EMCC = \
	$(SOURCE_CXX) \
	web/Vfs.cpp \
	web/main.cpp

SOURCE_C_TEST = $(SOURCE_C)
SOURCE_CXX_TEST = \
	$(SOURCE_CXX) \
	test/UnzipContext.cpp \
	test/DeleteRecursiveContext.cpp \
	test/FSFixture.cpp \
	test/VfsTest.cpp \
	test/PasteContext.cpp \
	test/NormalizePath.cpp

OBJECTS_EXTRA_NATIVE = ../common/libcommon.a
OBJECTS_EXTRA_TEST = ../common/libcommon.a
OBJECTS_EXTRA_EMCC = ../common/libcommon-wasm.a

BINARY_NATIVE = vfs
BINARY_EMCC = vfs_web.wasm
BINARY_TEST = test/test

include ../Makefile.common

bin: $(BINARY_NATIVE)

emscripten: $(BINARY_EMCC)

test: $(BINARY_TEST)
	$(BINARY_TEST)

.PHONY: bin emscripten clean test