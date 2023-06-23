CFLAGS=$(shell pkg-config --cflags sdl2 gl glu)
LDLIBS=$(shell pkg-config --libs sdl2 gl glu)
SUBDIRS := tools data
TOPTARGETS := all clean

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
BINS=glcombat

ifeq ($(OS),Windows_NT)
EXT=.exe
endif
EXES=$(BINS:=$(EXT))

%$(EXT): %.o
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

OPTIM_LEVEL=0
CFLAGS+=-g
CFLAGS+=-Wall -Werror -pedantic
CFLAGS+=-O$(OPTIM_LEVEL)
CFLAGS+=$(CFLAGS_EXTRA)
LDLIBS+=-lm

all: $(SUBDIRS) $(EXES)

$(EXES): $(OBJS)

$(TOPTARGETS): $(SUBDIRS)

$(SUBDIRS):
	$(MAKE) -C $@ $(MAKECMDGOALS)

clean: $(SUBDIRS)
	rm -f $(OBJS) $(EXES)

cleaner: $(SUBDIRS) clean
	[ -z "$(DEPS)" ] || rm -f $(DEPS)

.PHONY: all clean cleaner

.INTERMEDIATE: $(OBJS)
.PHONY: $(TOPTARGETS) $(SUBDIRS)

# automatically generate dependency rules
%.d: %.c
		$(CC) $(CCFLAGS) -MF"$@" -MG -MM -MP -MT"$@" -MT"$(<:.c=.o)" "$<"

# -MF  write the generated dependency rule to a file
# -MG  assume missing headers will be generated and don't stop with an error
# -MM  generate dependency rule for prerequisite, skipping system headers
# -MP  add phony target for each header to prevent errors when header is missing
# -MT  add a target to the generated dependency
-include $(DEPS)