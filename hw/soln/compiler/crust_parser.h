#ifndef CRUST_PARSER_H_
#define CRUST_PARSER_H_

#define MAX_TOKEN_LENGTH 100

typedef enum {
  ADD, MUL, SUB, DIV, MOD,
  LSHIFT, RSHIFT, BANG, TILDE, OR, AND, XOR,
  LT, GT, EQ, LTEQ, GTEQ,
  ASSIGN,
  FN,
  LITERAL,
  SEMICOLON,
  COMMA,
  RETURN,
  LET, IDENT,
  IF, ELSE, WHILE,
  LPAREN, RPAREN,
  LBRACE, RBRACE,
  ERROR,
} crust_token_type;

typedef struct crust_token_st {
  // used by all tokens
  crust_token_type type;

  // used only by literal tokens
  // at least 64 bits to store any possible 32bit signed or unsigned value
  int64_t literal_value;

  // used by IDENT token
  char name[MAX_TOKEN_LENGTH];

  // feel free to add other fields
} crust_token;

typedef struct crust_parser_st {
  // TODO: put some stuff here
  FILE* file;
  char buffer;
} crust_parser;

static_assert(sizeof(crust_parser) <= 128);

bool crust_parser_init(curst_parser* to_init, char* file_name);

bool crust_parser_next_token(crust_parser* this, crust_token* output);

void crust_parser_destroy(crust_parser* to_destroy);

#endif  // CRUST_PARSER_H_
