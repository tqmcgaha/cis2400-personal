#ifndef RISCV_CSTDLIB_2400_H_
#define RISCV_CSTDLIB_2400_H_

///////////////////////////////////////////////////////////////////////////////
// built in types
///////////////////////////////////////////////////////////////////////////////
typedef long unsigned int size_t;
typedef long ssize_t;
typedef unsigned char bool;

typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned int uint32_t;
typedef signed int int32_t;

///////////////////////////////////////////////////////////////////////////////
// Compiler Built-ins
///////////////////////////////////////////////////////////////////////////////

// the following four functions are things that gcc requires as builtins
// we arne't using gcc, but for future-proofint and since these are so
// fundamental, we have included them

// https://gcc.gnu.org/onlinedocs/gcc/Standards.html

// src and dest may overlap
void *memmove(void *dest, const void *src, size_t n);

// src and dest may not overlap
void *memcpy(void *dest, const void *src, size_t n);

void *memset(void* dest, int c, size_t n);

// compares the first n bytes (each interpreted as unsigned char) of the memory areas s1 and s2
int memcmp(const void* s1, const void *s2, size_t n);

///////////////////////////////////////////////////////////////////////////////
// stdlib
///////////////////////////////////////////////////////////////////////////////

size_t strlen(const char* str);

///////////////////////////////////////////////////////////////////////////////
// system calls
///////////////////////////////////////////////////////////////////////////////

bool stdout_putc(char c);

bool stdin_getc(char* out);

extern int errno;

#endif  // RISCV_CSTDLIB_H_
