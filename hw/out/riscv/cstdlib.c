#include "cstdlib.h"


// this works!
// clang --target=riscv32 -S cstdlib.c

// src and dest may overlap
void *memmove(void *dest, const void *src, size_t n) {
  return dest;
}

// src and dest may not overlap
void *memcpy(void *dest, const void *src, size_t n) {
  return dest;
}

void *memset(void* dest, int c, size_t n) {
  uint8_t* cpy = dest;
  for (size_t i = 0; i < n; ++i) {
    cpy[i] = (uint8_t) c;
  }
  return dest;
}

int memcmp(const void* s1, const void *s2, size_t n) {
  return 0;
}
