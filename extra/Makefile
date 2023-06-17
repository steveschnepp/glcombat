SRCS=$(wildcard *.c)
BINS=$(SRCS:.c=)

CFLAGS+=-g
CFLAGS+=-O0
CFLAGS+=-I..
LDLIBS+=-lm

all: $(BINS)

sphere: LDLIBS=-lglut -lGL -lm

clean:
	rm -f $(OBJS) $(BINS)

.PHONY: all clean
