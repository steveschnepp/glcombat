CFLAGS=$(shell pkg-config --cflags sdl2 SDL2_image)
LDLIBS=$(shell pkg-config --libs sdl2 SDL2_image)

SRCS=$(wildcard *.c)
OBJS=$(SRCS:.c=.o)
DEPS=$(SRCS:.c=.d)
BINS=$(lastword $(subst /, ,$(dir $(realpath $(lastword $(MAKEFILE_LIST))))))

ifeq ($(OS),Windows_NT)
EXT=.exe
# show stdout
LDLIBS+=-mconsole
endif

EXES=$(BINS:=$(EXT))

$(EXES): main.o
	$(LINK.c) $^ $(LOADLIBES) $(LDLIBS) -o $@

$(EXES): $(OBJS)

OPTIM_LEVEL?=g
CFLAGS+=-g
CFLAGS+=-O$(OPTIM_LEVEL)
LDLIBS+=-lm
CFLAGS+=-DUSDL_APPNAME=$(BINS)
CFLAGS+=-Wall -Werror -pedantic

ifeq ($(GL), 1)
CFLAGS+=-DOPENGL_ENABLED=1
CFLAGS+=$(shell pkg-config --cflags gl glu)
LDLIBS+=$(shell pkg-config --libs gl glu)
endif

ifeq ($(ANTIALIAS), 1)
CFLAGS+=-DANTIALIAS_ENABLED=1
endif

.DEFAULT: all

all: $(EXES) screenshots
	echo OS:$(OS) EXE:$(EXES)

screenshots:
	mkdir -p screenshots

clean: $(SUBDIRS)
	rm -f $(OBJS) $(EXES)
	rm -Rf screenshots

realclean: clean $(SUBDIRS)
	rm -f $(DEPS)

.PHONY: all clean
