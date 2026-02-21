CC = gcc
CFLAGS = -Wall -Werror -std=c11

all:
	$(CC) $(CFLAGS) src/main.c -o irv

clean:
	rm -f irv
