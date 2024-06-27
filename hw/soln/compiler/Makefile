

CC = gcc-12
CFLAGS = -g3 -Wall -Wpedantic -Wextra -std=c2x

.PHONY = all clean

all: crust_parser.o crust_ast.o

crust_parser.o: crust_parser.c crust_parser.h 
	$(CC) $(CFLAGS) -o $@ $<

crust_ast.o: crust_ast.c crust_parser.h crust_ast.h
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm *.o
