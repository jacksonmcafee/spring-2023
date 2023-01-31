CC=gcc
DEBUG=-g -O0
CFLAGS=$(DEBUG)

all: mtu-shell.exe

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

mtu-shell.exe: mtu-shell.o
	$(CC) -o $@ $<

clean:
	rm -f *.o mtu-shell.exe \#*\# *~
