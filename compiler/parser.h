#ifndef CRUST_PARSER_H_
#define CRUST_PARSER_H_

#define MAX_TOKEN_LENGTH 100
#define MAX_ARGS_COUNT 8

typedef enum {
  ADD, MUL, SUB, DIV, MOD,
  LSHIFT, RSHIFT, BANG, TILDE, OR, AND, XOR,
  LT, GT, EQ, LTEQ, GTEQ,
  ASSIGN,
  LET, IDENT,
  FN_CALL, FN_DECL,
  IF, ELIF, ENDIF, WHILE, ENDWHILE,
  LITERAL,
} crust_token_type;

typedef struct crust_arg_st {
  crust_token_type type;  // should be either an identifier or a literal

  // used by IDENT token
  char name[MAX_TOKEN_LENGTH];

  // used by LITEREAL token
  // at least 64 bits to store any possible 32bit signed or unsigned value
  int64_t literal_value;

} crust_arg;

typedef struct crust_token_st {
  // used by all tokens
  crust_token_type type;

  // used only by literal tokens
  // at least 64 bits to store any possible 32bit signed or unsigned value
  int64_t literal_value;

  // used by call token and the fn token
  int num_args;
  crust_arg args[MAX_ARGS_COUNT];

  // used by IDENT, call, and fn tokens
  char name[MAX_TOKEN_LENGTH]

  // feel free to add other fields
} crust_token;

typedef struct crust_parser_st {
  // TODO: put some stuff here
} crust_parser;

static_assert(sizeof(crust_parser) <= 128);

bool crust_parser_init(curst_parser* to_init, string file_name);

bool crust_parser_next_token(crust_parser* this, crust_token* output);

void crust_parser_destroy(crust_parser* to_destroy);

#endif  // CRUST_PARSER_H_
