-include ../Makefile.local

YARN = yarn
ZIP = zip
XXD = xxd
SED = sed

CC_NATIVE ?= gcc
CXX_NATIVE ?= g++
AR_NATIVE ?= ar
RANLIB_NATIVE ?= ranlib

CC_EMCC ?= emcc
CXX_EMCC ?= emcc
AR_EMCC ?= emar
RANLIB_EMCC = emranlib

EMCC_OPT ?= -O3 -flto

CFLAGS_NATIVE ?= -O2  -g

CXXFLAGS_NATIVE ?= $(CFLAGS_NATIVE)

INCLUDE_EXTRA ?=

CFLAGS_COMMON := -Werror -Wextra -Wall -Wno-unused-parameter
CXXFLAGS_COMMON := $(CFLAGS_COMMON) -std=c++17
CFLAGS_COMMON := $(CFLAGS_COMMON) -std=c99

CFLAGS_EMCC = $(EMCC_OPT)
CXXFLAGS_EMCC = $(CFLAGS_EMCC) -fno-rtti -fno-exceptions

CFLAGS_TEST ?= -O0 -g -fsanitize=address,undefined -fsanitize-blacklist=clang_blacklist.txt
CXXFLAGS_TEST ?= $(CFLAGS_TEST)
LDFLAGS_TEST ?= -fsanitize=address,undefined -fsanitize-blacklist=clang_blacklist.txt -lgtest -lgmock

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

SVG_ORIGINALS = originals
SVG_GENERATED = generated
SVG_SUBDIRS = hard-buttons silkscreen
SVG_ZIP = skins.zip

INCLUDE = -I../common -I../common/zip

SOURCE_C =
SOURCE_CPP = main.cpp Skins.cpp

OBJECTS_NATIVE = $(SOURCE_C:%.c=$(BUILDDIR_NATIVE)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_NATIVE)/%.o)
OBJECTS_EMCC = $(SOURCE_C:%.c=$(BUILDDIR_EMCC)/%.o) $(SOURCE_CPP:%.cpp=$(BUILDDIR_EMCC)/%.o)

LIBRARY_NATIVE = libskin.a
LIBRARY_EMCC = libskin-wasm.a

GARBAGE = \
	$(BUILDDIR_NATIVE) \
	$(BUILDDIR_EMCC) \
	$(BUILDDIR_TEST) \
	$(LIBRARY_EMCC) \
	$(LIBRARY_NATIVE) \
	$(SVG_GENERATED)

bin:

emscripten: $(LIBRARY_EMCC)

test:

generate: generated/skins.h

node_modules: package.json
	$(YARN) install

generated/skins.h: node_modules $(shell find originals -name '*.svg')
	test -d $(SVG_GENERATED) || mkdir -p $(SVG_GENERATED)
	$(YARN) svgo -rf $(SVG_ORIGINALS) -o $(SVG_GENERATED)
	cd $(SVG_GENERATED) && $(ZIP) -9ro $(SVG_ZIP) $(SVG_SUBDIRS)
	$(XXD) -n skins_zip -i $(SVG_GENERATED)/$(SVG_ZIP) | $(SED) 's/unsigned/static const/' | $(SED) 's/char/uint8_t/' > $@

$(LIBRARY_NATIVE): $(OBJECTS_NATIVE)
	$(AR_NATIVE) cru $@ $^
	$(RANLIB_NATIVE) $@

$(LIBRARY_EMCC): $(OBJECTS_EMCC)
	$(AR_EMCC) -cru $@ $^
	$(RANLIB_EMCC) $@

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

$(BUILDDIR_EMCC)/%.o : %.cpp generated/skins.h
	$(MKDIR_EMCC) && $(CXX_EMCC) $(DEPFLAGS_EMCC) $(CXXFLAGS_COMMON) $(CXXFLAGS_EMCC) $(INCLUDE) -c -o $@ $<

$(BUILDDIR_TEST)/%.o : %.cpp
	$(MKDIR_TEST) && $(CXX_NATIVE) $(DEPFLAGS_TEST) $(CXXFLAGS_COMMON) $(CXXFLAGS_TEST) $(INCLUDE) -c -o $@ $<

clean:
	-rm -fr $(GARBAGE)

.PHONY: clean all bin emscripten test generate
.SUFFIXES:

include $(shell test -e $(DEPDIR_NATIVE) && find $(DEPDIR_NATIVE) -type f)
include $(shell test -e $(DEPDIR_EMCC) && find $(DEPDIR_EMCC) -type f)
include $(shell test -e $(DEPDIR_TEST) && find $(DEPDIR_TEST) -type f)

