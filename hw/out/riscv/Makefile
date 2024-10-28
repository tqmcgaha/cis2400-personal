CC = clang-15
CXX = g++-12
CFLAGS = --target=riscv32 -march=rv32i -nostdlib -Wno-incompatible-library-redeclaration
CXXFLAGS = --std=c++23 -g3 -gdwarf-4 -Wall -Wextra

CPP_SOURCES = decoder.cpp elf_loader.cpp disas.cpp loader.cpp single_linker.cpp simulator.cpp simulator_main.cpp
HPP_SOURCES = decoder.hpp elf_loader.hpp simulator.hpp loader.hpp

.PHONY = all clean format

all: cstdlib.o cstdlib.s cstdlib.hexdump disas c_main test_riscv_main.o handwritten.o single_linker loader penn-sim

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

elf_loader.o: elf_loader.cpp elf_loader.hpp
	$(CXX) $(CXXFLAGS) -c $<

disas.o: disas.cpp elf_loader.hpp decoder.hpp
	$(CXX) $(CXXFLAGS) -c $<

disas: disas.o elf_loader.o decoder.o
	$(CXX) $(CXXFLAGS) -o $@ $^

single_linker.o: single_linker.cpp decoder.hpp elf_loader.hpp riscv_constants.hpp
	$(CXX) $(CXXFLAGS) -c $<

single_linker: single_linker.o decoder.o elf_loader.o
	$(CXX) $(CXXFLAGS) -o $@ $^

loader.o: loader.cpp riscv_constants.hpp decoder.hpp
	$(CXX) $(CXXFLAGS) -c $<

loader_main.o: loader_main.cpp loader.hpp decoder.hpp
	$(CXX) $(CXXFLAGS) -c $<

loader: loader_main.o loader.o decoder.o
	$(CXX) $(CXXFLAGS) -o $@ $^

simulator.o: simulator.cpp simulator.hpp decoder.hpp loader.hpp
	$(CXX) $(CXXFLAGS) -c $<
        
penn-sim.o: penn-sim.cpp simulator.hpp decoder.hpp
	$(CXX) $(CXXFLAGS) -c $<

penn-sim: penn-sim.o simulator.o loader.o decoder.o
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
	rm *.o cstdlib.s disas cstdlib.hexdump c_main
