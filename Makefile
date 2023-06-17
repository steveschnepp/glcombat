CFLAGS=$(shell pkg-config --cflags sdl2 gl glu)
LDLIBS=$(shell pkg-config --libs sdl2 gl glu)
SUBDIRS := tools data
TOPTARGETS := all clean

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
BINS=glcombat

CFLAGS+=-g
CFLAGS+=-O0
LDLIBS+=-lglut -lm

all: $(BINS)  $(SUBDIRS)

glcombat: $(OBJS)
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(SUBDIRS)
	rm -f $(OBJS) $(BINS) $(DEPS)

.PHONY: $(TOPTARGETS) $(SUBDIRS)

-include $(DEPS)

# automatically generate dependency rules

%.d : %.c
	$(CC) $(CCFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"

# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency