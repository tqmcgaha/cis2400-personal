CC = clang-15
CXX = g++-12
CFLAGS = --target=riscv32 -march=rv32i -nostdlib -Wno-incompatible-library-redeclaration
CXXFLAGS = --std=c++23 -g3 -gdwarf-4 -Wall -Wextra

CPP_SOURCES = decoder.cpp loader.cpp disas.cpp
HPP_SOURCES = decoder.hpp loader.hpp

.PHONY = all clean format

all: cstdlib.o cstdlib.s cstdlib.hexdump disas c_main test_riscv_main.o handwritten.o

cstdlib.o: cstdlib.s
	$(CC) $(CFLAGS) -c $<

handwritten.o: handwritten.s
	$(CC) $(CFLAGS) -c $<

cstdlib.s: cstdlib.c cstdlib.h
	$(CC) $(CFLAGS) -S $<

test_riscv_main: test_riscv_main.o cstdlib.o
	$(CC) $(CFLAGS) -o $@ $^

test_riscv_main.o: test_riscv_main.s
	$(CC) $(CFLAGS) -c $<

test_riscv_main.s: test_riscv_main.c cstdlib.h
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

# c_main is used for trying to figure out
# how elf maps the .text into memory
c_main: c_main.o
	clang-15 -o c_main c_main.o

c_main.o: c_main.c
	clang-15 -c c_main.c

format:
	clang-format -i --verbose --style=Chromium $(CPP_SOURCES) $(HPP_SOURCES)

clean:
	rm *.o *.s disas cstdlib.hexdump c_main
