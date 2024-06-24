#include "./crust_parser.h"

static bool next_word(crust_parser* parser, char token_chars[MAX_TOKEN_LENGTH]) {
  int curr_length = 0;
  char curr = fgetc(parser->file);
  while (curr == (char) EOF || isspace(curr)) {
    if (curr == (char) EOF) {
      return false;
    }
    curr = fgetc(parser->file);
  }

  while (curr != (char) EOF && !isspace(curr)) {
    token_chars[curr_length] = curr;
    curr = fgetc(parser->file);
    curr_length += 1;
  }

  token_chars[curr_length] = '\0';

  return true;
}

bool crust_parser_init(curst_parser* to_init, char* file_name) {
  to_init->file = fopen(file_name, "rb+");
  return to_init->file != NULL;
}

bool crust_parser_next_token(crust_parser* this, crust_token* output) {
  char current_word[MAX_TOKEN_LENGTH] = {0};
  if (!next_word(this, current_word)) {
    return false;
  }

  // handle comments
  while (strcmp(current_word, "//") == 0) {
    char next = fgetc(this->file);
    while (next != (char) EOF && next != '\n') {
     next = fgetc(this->file);
    }
    if (next == (char) EOF) {
      return false;
    }
    if (!next_word(this, current_word)) {
      return false;
    }
  }

  // handle other tokens

  if (strcmp(current_word, "fn") == 0) {
    // function definition
    if (!next_word(this, current_word)) {
      return false;
    }
  }
  
}

void crust_parser_destroy(crust_parser* to_destroy) {
  fclose(to_destroy->file);
}

#endif  // CRUST_PARSER_H_
