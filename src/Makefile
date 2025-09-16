SUBDIRS = common skins cloudpilot fstools vfs uarm

all: bin emscripten

bin:
	for subdir in $(SUBDIRS); do $(MAKE) -C$$subdir bin || exit 1; done

emscripten:
	for subdir in $(SUBDIRS); do $(MAKE) -C$$subdir emscripten || exit 1; done

test:
	for subdir in $(SUBDIRS); do $(MAKE) -C$$subdir test || exit 1; done

clean:
	for subdir in $(SUBDIRS); do $(MAKE) -C$$subdir clean || exit 1; done

.PHONY: all bin emscripten clean test
