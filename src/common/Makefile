-include ../Makefile.local

CFLAGS_COMMON = -Wextra -Wall -Wno-unused-parameter
STD_C = -std=gnu99
INCLUDE = -I.

SOURCE_C = 							\
	zip/miniz.c 					\
	zip/zip.c

SOURCE_CXX = 						\
	CardImage.cpp 					\
	CardVolume.cpp 					\
	CPCrc.cpp 						\
	GunzipContext.cpp 				\
	GzipContext.cpp 				\
	CreateZipContext.cpp 			\
	ZipfileWalker.cpp 				\
	FileUtil.cpp 					\
	Logging.cpp						\
	savestate/Chunk.cpp				\
	savestate/ChunkProbe.cpp

SOURCE_C_NATIVE = $(SOURCE_C)
SOURCE_CXX_NATIVE = 				\
	$(SOURCE_CXX)					\
	Cli.cpp

SOURCE_C_EMCC = $(SOURCE_C)
SOURCE_CXX_EMCC = $(SOURCE_CXX)

SOURCE_C_TEST = $(SOURCE_C)
SOURCE_CXX_TEST = 					\
	$(SOURCE_CXX) 					\
	test/Crc.cpp 					\
	test/GunzipContext.cpp 			\
	test/GzipContext.cpp			\
	test/SaveChunkHelper.cpp		\
	test/LoadChunkHelper.cpp		\
	test/Savestate.cpp				\
	test/SavestateChunk.cpp			\
	test/SavestateChunkProbe.cpp	\
	test/SavestateLoader.cpp		\
	test/SavestateProbe.cpp			\
	test/main.cpp

LIBRARY_NATIVE = libcommon.a
LIBRARY_EMCC = libcommon-wasm.a
BINARY_TEST = test/test

include ../Makefile.common

bin: $(LIBRARY_NATIVE)

emscripten: $(LIBRARY_EMCC)

test: $(BINARY_TEST)
	$(BINARY_TEST)

.PHONY: bin emscripten clean test