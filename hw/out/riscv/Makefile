
CC = clang-15
CXX = clang++15
CFLAGS = --target=riscv32 -march=rv32i
CXXFLAGS = --std=c++23 -g3 -gdwarf-4 -Wall -Wextra

.PHONY = all

all: cstdlib.o cstdlib.s cstdlib.hexdump disas

cstdlib.o: cstdlib.s
	$(CC) $(CFLAGS) -c $<

cstdlib.s: cstdlib.c cstdlib.h
	$(CC) $(CFLAGS) -S $<

cstdlib.hexdump: cstdlib.o
	xxd $< > $@

decoder.o: decoder.cpp decoder.hpp
	$(CXX) $(CXXFLAGS) -c $<

loader.o: loader.cpp loader.hpp
	$(CXX) $(CXXFLAGS) -c $<

disas.o: disas.o loader.hpp decoder.hpp
	$(CXX) $(CXXFLAGS) -o $@ $<

disas: disas.o loader.o decoder.o
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm *.o *.s disas cstdlib.hexdump
