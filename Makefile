FLAGS = -std=c89 -pedantic -Wall -Wextra
CFLAGS = -fPIC -Ofast -g -march=core2 -mtune=generic
LDFLAGS = -shared

SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

all: clean test

libradix.so: ${OBJ}
	${CC} -o $@ ${OBJ} ${LDFLAGS}

test: libradix.so main.c
	${CC} main.c -L. -lradix

clean:
	rm -f a.out libradix.so ${OBJ}

.PHONY: all clean
