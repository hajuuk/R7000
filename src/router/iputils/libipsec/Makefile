CFLAGS += -include ../include-glibc/glibc-bugs.h -I../include-glibc -I/usr/src/linux/include
CFLAGS += -DIPSEC_DEBUG -DIPSEC -DINET6
CFLAGS += -Wall


OBJ = ipsec_dump_policy.o    key_debug.o   policy_parse.o \
      ipsec_get_policylen.o  pfkey.o       policy_token.o \
      ipsec_strerror.o       pfkey_dump.o

all: libipsec.a

libipsec.a:  $(OBJ)
	$(AR) rcs $@ $(OBJ)

policy_parse.c policy_parse.h: policy_parse.y
	yacc -d -p __libyy policy_parse.y
	mv y.tab.c policy_parse.c
	mv y.tab.h policy_parse.h

policy_token.c: policy_token.l
	lex -P__libyy policy_token.l
	mv lex.__libyy.c policy_token.c

clean:
	@rm -f libipsec.a $(OBJ) policy_parse.c policy_parse.h policy_token.c
