SOURCES = $(wildcard *.c) ./munit/munit.c

CFLAGS = -I -I. -I$(shell realpath `pwd`/../)

# Ignore __flash attribute when compiling for tests
CFLAGS += -D__flash=""

test: build
	./run_tests

build: $(SOURCES)
	gcc -Wall -std=gnu99 $(SOURCES) $(CFLAGS) -o run_tests

clean:
	rm -f run_tests