PROTOC = protoc

networking_pb2.py: ../../proto/networking.proto
	$(PROTOC) --python_out=. --mypy_out=. -I $(dir $<) $(notdir $<)

clean:
	-rm -f networking_pb2.py

.PHONY: clean
