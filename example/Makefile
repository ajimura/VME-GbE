CFLAGS += -Wall -I../lib/
LDLIBS += ../lib/vmeg_lib.o ../lib/rbcp_lib.o

APS = v265_test v265_test2 v265_clear v265_mon v265_daq

.c.o:
	$(CC) $(CFLAGS) -c $*.c -o $@

all : $(APS)

clean :
	rm -f *.o *~ $(APS)
