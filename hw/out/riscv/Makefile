
CC = clang-15
CFLAGS = --target=riscv32 -march=rv32i

.PHONY = all

all: cstdlib.o

cstdlib.o: cstdlib.s
	$(CC) $(CFLAGS) -c $<

cstdlib.s: cstdlib.c cstdlib.h
	$(CC) $(CFLAGS) -S $<
