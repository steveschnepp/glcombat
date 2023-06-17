#CFLAGS=$(shell pkg-config --cflags sdl2 gl glu)
#LDLIBS=$(shell pkg-config --libs sdl2 gl glu)

SRCS=$(wildcard *.c)
BINS=$(SRCS:.c=)

CFLAGS+=-g
CFLAGS+=-O0
CFLAGS+=-I..
LDLIBS+=-lm

all: $(BINS)

sphere: LDLIBS=-lglut -lGL -lm

test_object: test_object.c ../object.c ../object.h

test_map: test_map.c ../map.c ../map.h ../object.c ../object.h

img2map: LDLIBS=-lgd

clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean
