
// This is just an example program that shows how to use `cstrtok_r`
// feel free to modify this if you want to call the functions yourself and test them to make sure they work

#include "./cstring.h"
#include <stdlib.h>
#include <stdio.h>

int main() {
  char* input = "We drift... Like Worried Fire";
  char* save_ptr;

  char* token = cstrtok_r(input, ". ", &save_ptr);
  while (token != NULL) {
    printf("%s\n", token);
    free(token);
    token = cstrtok_r(NULL, ". ", &save_ptr);
  }

  return EXIT_SUCCESS;
}
