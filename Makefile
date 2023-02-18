all: bin emscripten

bin:
	$(MAKE) -Csrc bin

emscripten:
	$(MAKE) -Csrc emscripten

proto:
	$(MAKE) -Csrc/cloudpilot/proto
	$(MAKE) -Cserver/proto

clean:
	$(MAKE) -Csrc clean

test:
	$(MAKE) -Csrc test

.PHONY: all bin emscripten proto clean
