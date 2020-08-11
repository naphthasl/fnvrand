FLAGS = -std=c89 -pedantic -Wall -Wextra
CFLAGS = -fPIC -Ofast -g -march=core2 -mtune=generic
LDFLAGS = -shared

SRC = ${wildcard src/*.c}
OBJ = ${SRC:.c=.o}

all: libradix.so a.out

libradix.so: ${OBJ}
	${CC} -o $@ $^ ${LDFLAGS}

libradix.a: ${OBJ}
	ar rcs $@ $^

a.out: main.c libradix.a
	${CC} $^

clean:
	rm -f a.out libradix.* ${OBJ}

.PHONY: all clean
