#ifndef CSTRING_H_
#define CSTRING_H_

char* cstrcpy(char *dest, char *src);

char* cstrdup(char *str);

unsigned int cstrlen(char *str);

char* cstrchr(char *str, char target);

char* cstrstr(char *str, char *target);

char* cstrpbmrk(char *str, char *target);

int cstrcmp(char *lhs, char *rhs);

void *cmemset(void *s, char c, unsigned int n);

void *cmemcpy(void *dest, void *src, unsigned int n);

char* cstrtok_r(char* input, char* delims, char** save_ptr);

#endif  // CSTRING_H_

