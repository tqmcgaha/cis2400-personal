.PHONY = all clean

all: use_cstring test_suite

test_suite: catch.o test_suite.o test_cstring.o cstring.o
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++20 -o test_suite catch.o test_suite.o test_cstring.o cstring.o

catch.o: catch.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++20 -c catch.cpp

test_cstring.o: test_cstring.cpp cstring.h catch.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++20 -c test_cstring.cpp

test_suite.o: test_suite.cpp catch.cpp catch.hpp
	clang++-15 -g3 -gdwarf-4 -Wall --std=c++20 -c test_suite.cpp

use_cstring: use_cstring.o cstring.o
	clang-15 -g3 -gdwarf-4 -Wall -o use_cstring use_cstring.o cstring.o

use_cstring.o: use_cstring.c cstring.h
	clang-15 -g3 -gdwarf-4 -Wall -c use_cstring.c

cstring.o: cstring.c cstring.h
	clang-15 -g3 -gdwarf-4 -Wall -c cstring.c

clean:
	rm -rf use_cstring test_suite *.o
