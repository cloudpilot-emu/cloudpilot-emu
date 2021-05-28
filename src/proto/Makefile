PROTOC = nanopb_generator.py

networking.pb.c: ../../proto/networking.proto
	$(PROTOC) -I $(dir $<) $(notdir $<)

clean:
	-rm -f networking.pb.c networking.pb.h

.PHONY: clean
