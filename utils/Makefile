CFLAGS += -Wall -I../lib/
LDLIBS += ../lib/vmeg_lib.o ../lib/rbcp_lib.o

APS = vme_test rbcp_read rbcp_write vmeg_delay vmeg_nodelay

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $@

all : $(APS)

clean :
	rm -f *.o *~ $(APS)
