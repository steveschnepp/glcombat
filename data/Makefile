SRCS=$(wildcard *.map.spec)

all: $(SRCS:.map.spec=.gen.map)

%.obj: %.norm
	../tools/norm2obj $$(cat $^) > $@

%.gen.map: %.map.spec
	../tools/gen_map $$(cat $^) > $@

clean:
	rm -f $(OBJS) $(BINS)

cleaner: $(SUBDIRS) clean
	[ -z "$(DEPS)" ] || rm -f $(DEPS)

.PHONY: all clean cleaner

# automatically generate dependency rules
%.d: %.c
		$(CC) $(CCFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"

# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency
-include $(DEPS)
