OBJS	= shell.o
SOURCE	= shell.c
HEADER	= 
OUT	= tinysh
CC	 = gcc
FLAGS	 = -g -c -Wall
LFLAGS	 = 

all: $(OBJS)
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)

shell.o: shell.c
	$(CC) $(FLAGS) shell.c -std=c99


clean:
	rm -f $(OBJS) $(OUT)
