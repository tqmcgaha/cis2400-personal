#ifndef CSTRING_H_
#define CSTRING_H_

#include "./cstring.h"

#include <stdlib.h>

char* cstrcpy(char *dest, char *src) {
  unsigned int index = 0;
  while(src[index] != '\0') {
    dest[index] = src[index];
    index++;
  }
  dest[index] = src[index];
  return dest;
}

char* cstrdup(char *str) {
  char* result = malloc((cstrlen(str) + 1) * sizeof(char));
  return cstrcpy(result, str);
}

unsigned int cstrlen(char *str) {
  unsigned int result = 0;
  while (str[result] != '\0') {
    result++;
  }
  return result;
}

char* cstrchr(char *str, char target) {
  unsigned int len = cstrlen(str);
  for (unsigned int i = 0; i < len; i++) {
    if (str[i] == target) {
      return str + i;
    }
  }
  return NULL;
}

char* cstrstr(char *str, char *target) {
  unsigned int len = cstrlen(str);
  unsigned int target_len = cstrlen(target);
  for (unsigned int i = 0; i < len - target_len; i++) {
    bool matches = true;
    for (unsigned int j = i; j < target_len; j++) {
      if (str[j] != target[j-i]) {
        matches = false;
      }
    }
    if (matches) {
      return str + i;
    }
  }
  return NULL;
}

char* cstrpbmrk(char *str, char *target) {
  unsigned int len = cstrlen(str);
  unsigned int target_len = cstrlen(target);
  for (unsigned int i = 0; i < len; i++) {
    for (unsigned int j = 0; j < target_len; j++) {
      if (str[i] == target[j]) {
        return str + i;
      }
    }
  }
  return NULL;
}

int cstrcmp(char *lhs, char *rhs) {
  unsigned int llen = cstrlen(lhs);
  unsigned int rlen = cstrlen(rhs);

  unsigned int min = llen < rlen ? llen : rlen;

  for (unsigned int i = 0; i < min; i++) {
    if (lhs[i] != rhs[i]) {
      return lhs[i] - rhs[i];
    }
  }

  return lhs[min] - rhs[min];
}

void *cmemset(void *s, char c, unsigned int n) {
  char* ptr = (char*) s;
  for (unsigned int i = 0; i < n; i++) {
    ptr[i] = c;
  }
  return s;
}

void *cmemcpy(void *dest, void *src, unsigned int n) {
  char* dest_ptr = (char*) s;
  char* src_ptr = (char*) s;
  for (unsigned int i = 0; i < n; i++) {
    dest_ptr[i] = src_ptr;
  }
  return dest;
}

char* cstrtok_r(char* input, char* delims, char** save_ptr) {
  if (input == NULL) {
    input = *save_ptr;
  }

  char* end = input;
  unsigned int len = cstrlen(input);
  
  if (len == 0) {
    return NULL;
  }

  for (unsigned int = 0; i < len; i++) {
    if (cstrchr(delims, input[i]) != NULL) {
      char* result = malloc((i + 1) * sizeof(char));
      cmemcpy(result, input, i);
      result[i] = '\0';
      *save_ptr = input + i + 1;
      return result;
    }
  }
  *save_ptr = input + len;
  return NULL;
}


#endif  // CSTRING_H_

