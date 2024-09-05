#include "./catch.hpp"
#include <cstring>  // Does not do our cstring.h this includes C's <string.h>
#include <cstdlib>  // include's C's <stdlib.h>
#include <iostream> // for C++ printing

extern "C" {
  #include "cstring.h"
}

using namespace std;
using namespace Catch::Matchers;

// Uses some of the C stdlib impleemntations
// in our test code just incase student version
// doesn't work. Allows us to test the student
// functions in isolation.

TEST_CASE("simple literals", "[cstrlen]") {
  char s1[] = "Hello";
  char s2[] = "Xen Gorilla";
  char arr[] = {'X', 'e', 'n', '\0', 'y', 'e', 's'};
  REQUIRE(cstrlen(s1) == 5U);
  REQUIRE(cstrlen(s2) == 11U);
  REQUIRE(cstrlen(arr) == 3U);
}

TEST_CASE("empty string", "[cstrlen]") {
  char str[] = "";
  REQUIRE(cstrlen(str) == 0U);
}

TEST_CASE("Dynamic strings & const", "[cstrlen]") {
  char *s1 = strdup("Seamus");
  char *s2 = strdup("Echoes");
  char *s3 = strdup("Chumbawamba");
  char *s4 = strdup("What shalllll    we use          to   fill       the            emptyyyy           spaces");

  REQUIRE(cstrlen(s1) == 6U);
  REQUIRE(cstrlen(s2) == 6U);
  REQUIRE(cstrlen(s3) == 11U);
  REQUIRE(strlen(s4) == cstrlen(s4));

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);
  REQUIRE(strcmp("What shalllll    we use          to   fill       the            emptyyyy           spaces", s4) == 0);

  free(s1);
  free(s2);
  free(s3);
  free(s4);

  char uninitialized[5];
  uninitialized[0] = 'H';
  uninitialized[1] = 'i';
  uninitialized[2] = '\0';
  // last 2 chars in array are purposefullly unitialized
  // this is done to help check for valgrind errors
  // for accessing something you should not access

  REQUIRE(cstrlen(uninitialized) == 2U);
  REQUIRE(strcmp(uninitialized, "Hi") == 0);
}

TEST_CASE("literals", "[cstrdup]") {
  char s1[] = "Hello";
  char s2[] = "Xen Gorilla";
  char arr[] = {'X', 'e', 'n', '\0', 'y', 'e', 's'};
  char* r1 = cstrdup(s1);
  char* r2 = cstrdup(s2);
  char* r3 = cstrdup(arr);
  REQUIRE(strcmp(r1, "Hello") == 0);
  REQUIRE(strcmp(s1, "Hello") == 0);
  REQUIRE(strcmp(r2, "Xen Gorilla") == 0);
  REQUIRE(strcmp(s2, "Xen Gorilla") == 0);
  REQUIRE(strcmp(r3, "Xen") == 0);
  REQUIRE(strcmp(arr, "Xen") == 0);

  free(r1);
  free(r2);
  free(r3);
}

TEST_CASE("empty string", "[cstrdup]") {
  char str[] = "";
  char *result = cstrdup(str);
  REQUIRE(strcmp("", result) == 0);
  free(result);
}

TEST_CASE("dynamic strings & const", "[cstrdup]") {
  char *s1 = strdup("Seamus");
  char *s2 = strdup("Echoes");
  char *s3 = strdup("Chumbawamba");
  char *s4 = strdup("What shalllll    we use          to   fill       the            emptyyyy           spaces");

  char* r1 = cstrdup(s1);
  char* r2 = cstrdup(s2);
  char* r3 = cstrdup(s3);
  char* r4 = cstrdup(s4);

  REQUIRE(strcmp(s1, r1) == 0);
  REQUIRE(strcmp(s2, r2) == 0);
  REQUIRE(strcmp(s3, r3) == 0);
  REQUIRE(strcmp(s4, r4) == 0);

  REQUIRE(strcmp("Seamus", s1) == 0);
  REQUIRE(strcmp("Echoes", s2) == 0);
  REQUIRE(strcmp("Chumbawamba", s3) == 0);
  REQUIRE(strcmp("What shalllll    we use          to   fill       the            emptyyyy           spaces", s4) == 0);

  free(s1);
  free(s2);
  free(s3);
  free(s4);

  free(r1);
  free(r2);
  free(r3);
  free(r4);

  char uninitialized[5];
  uninitialized[0] = 'H';
  uninitialized[1] = 'i';
  uninitialized[2] = '\0';
  // last 2 chars in array are purposefullly unitialized
  // this is done to help check for valgrind errors
  // for accessing something you should not access

  char* res = cstrdup(uninitialized);
  REQUIRE(strcmp(res, uninitialized) == 0);
  REQUIRE(strcmp(uninitialized, "Hi") == 0);
  free(res);
}