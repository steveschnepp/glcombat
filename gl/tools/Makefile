#CFLAGS=$(shell pkg-config --cflags sdl2 gl glu)
#LDLIBS=$(shell pkg-config --libs sdl2 gl glu)

SRCS=$(wildcard *.c)
BINS=$(SRCS:.c=)

ifeq ($(OS),Windows_NT)
EXT=.exe
endif

%$(EXT): %.o
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@
EXES=$(BINS:=$(EXT))

OPTIM_LEVEL?=g
CFLAGS+=-g
CFLAGS+=-O$(OPTIM_LEVEL)
CFLAGS+=-I..
LDLIBS+=-lm

all: $(EXES)

test_object$(EXT): ../object.c ../object.h

test_map$(EXT): ../map.c ../map.h ../object.c ../object.h

img2map$(EXT): LDLIBS=$(shell pkg-config --libs gdlib)
img2map$(EXT): CFLAGS=$(shell pkg-config --libs gdlib)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(SUBDIRS)
	rm -f $(OBJS) $(EXES)

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
