.PHONY = all clean

all: use_cstring test_suite

test_suite: catch.o test_suite.o test_cstring.o cstring.o
	clang++-15 -g3 -Wall --std=c++20 -o test_suite catch.o test_suite.o test_cstring.o cstring.o

catch.o: catch.cpp catch.hpp
	clang++-15 -g3 -Wall --std=c++23 -c catch.cpp

test_cstring.o: test_cstring.cpp cstring.h catch.cpp catch.hpp
	clang++-15 -g3 -Wall --std=c++23 -c test_cstring.cpp

test_suite.o: test_suite.cpp catch.cpp catch.hpp
	clang++-15 -g3 -Wall --std=c++23 -c test_suite.cpp

# above may be useful, but also should not be copied directly since that is C++

# TODO: add a rule for `use_cstring` executable. Should depend on `use_cstring.o` and `cstring.o`


use_cstring.o: use_cstring.c # TODO: FINISH
	# TODO: FINISH

# TODO: add a rule for cstring.o

clean:
	rm -rf *.o
