SRC = $(wildcard src/*.c)
OBJ = ${SRC:.c=.o}

all: test

libradix.so: ${OBJ}
	${CC} -o $@ ${OBJ} -shared

test: libradix.so main.c
	${CC} main.c -lradix

clean:
	rm -f a.out libradix.so ${OBJ}

.PHONY: all clean
