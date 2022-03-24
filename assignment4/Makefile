CC := /usr/local/bin/gcc-10

TSAN ?= yes

CFLAGS := -O3  -ggdb -Wall -Werror -pedantic
LDFLAGS := -lpthread

ifeq ($(TSAN),yes)
	CFLAGS := $(CFLAGS) -fsanitize=thread -fPIE
	LDFLAGS := $(LDFLAGS) -fsanitize=thread -pie -static-libtsan
endif

all: tests

%.o: %.c assignment4.h
	$(CC) $(CFLAGS) -c $<

tests: tests.o assignment4.o
	$(CC) $(LDFLAGS) -o $@ $^

.PHONY: clean test

clean:
	-rm *.o tests

test: tests
	./tests

