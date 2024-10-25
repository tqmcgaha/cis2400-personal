#include "cstdlib.h"

// this works!
// clang --target=riscv32 -S cstdlib.c

int errno = 0;

// src and dest may overlap
void *memmove(void *dest, const void *src, size_t n) {
  // if (dest > src && dest < (src + n)) {
  if (dest <= src || dest >= (src + n)) {
    return memcpy(dest, src, n);
  }
  // dest shows up in between src and end
  // we need to copy backwards

  const unsigned char* chr_src = src;
  unsigned char* chr_dest = dest;
  for (size_t i = 0; i < n; i++) {
    chr_dest[n - i - 1] = chr_src[n - i - 1];
  }

  return dest;
}

// src and dest may not overlap
void *memcpy(void *dest, const void *src, size_t n) {
  const unsigned char* chr_src = src;
  unsigned char* chr_dest = dest;
  for (size_t i = 0; i < n; i++) {
    chr_dest[i] = chr_src[i];
  }
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

void print_char(char to_print) {
  
}
