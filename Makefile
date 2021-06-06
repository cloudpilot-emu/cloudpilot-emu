all: bin emscripten

bin:
	$(MAKE) -Csrc bin

emscripten:
	$(MAKE) -Csrc emscripten

proto:
	$(MAKE) -Csrc/proto
	$(MAKE) -Cserver/proto

clean:
	$(MAKE) -Csrc clean

.PHONY: all bin emscripten proto clean
