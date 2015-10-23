CFLAGS += -include ../include-glibc/glibc-bugs.h -I../libipsec -I../include-glibc -I/usr/src/linux/include
CFLAGS += -DIPSEC_DEBUG -DINET6 -DYY_NO_UNPUT -I. -Wall

all: setkey


OBJ=setkey.o parse.o token.o pfkey.o pfkey_dump.o key_debug.o ipsec_strerror.o

setkey: $(OBJ)
	$(CC) -o $@ $(OBJ) -L../libipsec -lipsec -ll

token.o: parse.h

parse.h parse.c: parse.y
	bison -d parse.y -o parse.c

clean:
	@rm -f setkey $(OBJ) parse.c parse.h token.c
