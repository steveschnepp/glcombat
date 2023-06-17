SRCS=$(wildcard *.map.spec)

all: $(SRCS:.map.spec=.gen.map)

%.obj: %.norm
	../tools/norm2obj $$(cat $^) > $@

%.gen.map: %.map.spec
	../tools/gen_map $$(cat $^) > $@

clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean
