SUBDIRS = src

all: bin emscripten

bin:
	for subdir in $(SUBDIRS); do $(MAKE) -C$$subdir bin; done

emscripten:
	for subdir in $(SUBDIRS); do $(MAKE) -C$$subdir emscripten; done

clean:
	for subdir in $(SUBDIRS); do $(MAKE) -C$$subdir clean; done

.PHONY: all bin emscripten clean
