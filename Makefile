CC = gcc

CFLAGS = -std=c00 -Wall -o -pedantic

DEBUG = -g

HEADER = parseline.h
MYTAR = parseline.c

LM = -lm

all: $(MYTAR) $(HEADER)
	$(CC) $(DEBUG) -o parseline $(MYTAR) $(LM)

%.o: %.c
	$(CC) $(DEBUG) $(CFLAGS) -c -o $@ $^

clean:
	$(RM) parseline *.o *.tar