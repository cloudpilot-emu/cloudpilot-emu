-include ../Makefile.local

YARN ?= yarn
ZIP ?= zip
XXD ?= xxd
SED ?= sed

SVG_ORIGINALS = originals
SVG_GENERATED = generated
SVG_SUBDIRS = hard-buttons silkscreen
SVG_ZIP = skins.zip

INCLUDE = -I../common -I../common/zip

SOURCE_CXX_EMCC = main.cpp Skins.cpp
LIBRARY_EMCC = libskin-wasm.a

EXTRA_OBJECT_DEPS = generated/skins.h
GARBAGE_EXTRA ?= generated

include ../Makefile.common

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
	$(XXD) -i $(SVG_GENERATED)/$(SVG_ZIP) | $(SED) 's/unsigned/static const/' | $(SED) 's/char/uint8_t/' > $@

.PHONY: bin emscripten clean test generate