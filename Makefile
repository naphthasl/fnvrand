FLAGS = -std=c99 -pedantic -Wall -Wextra
CFLAGS = -fPIC -g
LDFLAGS = -shared

SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

all: clean test

libradix.so: ${OBJ}
	${CC} -o $@ ${OBJ} -shared -fPIC -g

test: libradix.so main.c
	${CC} main.c -L$(ORIGIN) -lradix

clean:
	rm -f a.out libradix.so ${OBJ}

.PHONY: all clean
