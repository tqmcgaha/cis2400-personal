# Copyright ©2022 Travis McGaha.  All rights reserved.  Permission is
# hereby granted to students registered for University of Pennsylvania
# CIS 2400 for use solely during Fall Semester 2022 for purposes of
# the course.  No other use, copying, distribution, or modification
# is permitted without prior written consent. Copyrights for
# third-party components of this work must be honored.  Instructors
# interested in reusing these course materials should contact the
# author.

# compile everything; this is the default rule that fires if a user
# just types "make" in the same directory as this Makefile

.PHONY = all clean

CC = gcc-12
CXX = g++-12

CFLAGS += -g3 -Wall -Wpedantic -Wextra -std=c2x 
CXXFLAGS += -g3 -Wall -Wpedantic -Wextra -std=c++23

all: test_suite

test_suite: test_suite.o test_deque.o Deque.o catch.o
	$(CXX) $(CXXFLAGS) -o test_suite test_suite.o catch.o test_deque.o Deque.o

Deque.o: Deque.c Deque.h Deque_priv.h
	$(CC) $(CFLAGS) -c Deque.c

HashTable.o: HashTable.c HashTable.h HashTable_priv.h Deque.h
	$(CC) $(CFLAGS) -c HashTable.c

test_suite.o: test_suite.cpp Deque.h catch.hpp
	$(CXX) $(CXXFLAGS) -c test_suite.cpp

test_deque.o: test_deque.cpp Deque.h catch.hpp
	$(CXX) $(CXXFLAGS) -c test_deque.cpp

catch.o: catch.cpp catch.hpp
	$(CXX) $(CXXFLAGS) -c catch.cpp

clean:
	rm -f *.o *~ test_suite rpn
