C=gcc
CFLAGS=-std=c99 -Wall -Wextra -D_XOPEN_SOURCE=700
LDFLAGS=-lrt

trans: trans.c
	$(CC) $(CFLAGS) -o trans trans.c $(LDFLAGS)

clean:
	rm -f trans
