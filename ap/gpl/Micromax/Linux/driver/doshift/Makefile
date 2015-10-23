PROG        = lcdoshift
VERS        = 1.1.0
CC          = gcc
CFLAGS      += -Wall
RM          = /bin/rm -f
OBJS        = lcdoshift.c

all:        $(PROG)

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJS) -lusb

install:
	cp lcdoshift /sbin/lcdoshift

clean:
	rm -rf *.mod.c *.o *.markers *.symvers *.order  *.ko .tmp_versions .*.cmd lcdoshift

