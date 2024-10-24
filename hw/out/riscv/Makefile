CC = clang-15
CXX = g++-12
CFLAGS = --target=riscv32 -march=rv32i
CXXFLAGS = --std=c++23 -g3 -gdwarf-4 -Wall -Wextra

CPP_SOURCES = decoder.cpp loader.cpp disas.cpp
HPP_SOURCES = decoder.hpp loader.hpp

.PHONY = all clean format

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

disas.o: disas.cpp loader.hpp decoder.hpp
	$(CXX) $(CXXFLAGS) -c $<

disas: disas.o loader.o decoder.o
	$(CXX) $(CXXFLAGS) -o $@ $^

format:
	clang-format -i --verbose --style=Chromium $(CPP_SOURCES) $(HPP_SOURCES)

clean:
	rm *.o *.s disas cstdlib.hexdump
