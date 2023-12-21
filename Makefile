CC = clang
CFLAGS = -Wextra -Wall -pedantic -std=c99 -ggdb

example: example.c dstring.h
	$(CC) $(CFLAGS) $<

clean:
	rm -f *.o *.out
