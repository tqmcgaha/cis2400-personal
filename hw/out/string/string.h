#ifndef STRING_H_
#define STRING_H_

typedef struct string_st {
  char* data;
  int len;
} string;

string string_duplicate(string to_duplicate);

// do not take ownership
string string_from_cstr(char* cstr);

bool string_equals(string str1, string str2);

void string_free(string *to_free);

bool string_append(string* this, string suffix);

bool string_prepend(string* this, string prefix);

int string_index_of(string input, string target);

// TODO: check java indexof behaviour
bool string_substr(string input, int start, int length, string* output);

bool string_at(string input, int index, char* output_char);

bool string_slice(string* input, int start, int length);

bool string_to_int(string input, int* output);

#endif   // STRING_H_
