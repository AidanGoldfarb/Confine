CC = gcc
CFLAGS = -Wall

all: confine test

confine: confine.c
	$(CC) $(CFLAGS) -o confine confine.c $(LIB)
test: test.c
	$(CC) $(CFLAGS) -o test test.c$(LIB)
