LIB = libeffect
BIN = effectest
VER = 0.1
LIBSRC = effect.c test/test.c
SRC = effect_test.c
LIBOBJ = ${LIBSRC:.c=.o}
OBJ = ${SRC:.c=.o}

CC = gcc
CROSS_COMPILE =

CFLAGS += -std=c99 -Wall -Wextra -Wunused -DVERSION=\"${VER}\" -I./include -DTEST_MODE
LDFLAGS += -L. -leffect
#LIBLDFLAGS = -lasound

all: libeffect effectest

$(LIB): ${LIBOBJ}
	@echo "  LD	"$@
	@${CROSS_COMPILE}${CC} ${CFLAGS} -shared -Wl,-soname,$@.so -o $@.so ${LIBOBJ} ${LIBLDFLAGS}

$(BIN): ${OBJ} ${LIB}
	@echo "  LD	"$@
	@${CROSS_COMPILE}${CC} ${CFLAGS} -o $@ ${OBJ} ${LDFLAGS}


%.o: %.c
	@echo "  CC	"$<
	@${CROSS_COMPILE}${CC} ${CFLAGS} -c -fPIC -o $@ $<

clean:
	@rm -rf ${BIN} ${OBJ} ${LIB}.so ${LIBOBJ}


.PHONY: all clean
