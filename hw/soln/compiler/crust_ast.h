#ifndef CRUST_AST_H_
#define CRUST_AST_H_

#define MAX_ARGS 8

typedef struct {
  // a crust program is just
  // a list of functions
  // each DQPayload_t
  // in this Deque should be of type
  // ast_node*
  Deque* functions; 
} crust_ast;

typedef enum {
  BINARY_OP,
  UNARY_OP,
  IDENT,
  LITERAL,
  LET,
  FUNC_CALL,
  FUNC_DECL,
  IF, ELSE, WHILE,
  RETURN,
} ast_node_type;

typedef struct {
  ast_node_type type;
} ast_node;

typedef struct {
  ast_node_type type;
  ast_node* lhs;
  ast_node* rhs;
  char op;
} binary_op_node;

typedef struct {
  ast_node_type type;
  ast_node* lhs;
  ast_node* rhs;
  char op;
} unary_op_node;

typedef struct {
  ast_node_type type;
  char name[MAX_TOKEN_LENGTH + 1];
} ident_node;

typedef struct {
  ast_node_type type;
  int64_t literal_value;
} literal_node;

typedef struct {
  ast_node_type type;
  char name[MAX_TOKEN_LENGTH + 1];
} let_node;

typedef struct {
  ast_node_type type;
  char name[MAX_TOKEN_LENGTH + 1];
  ast_node* args[MAX_ARGS];
} func_call_node;

typedef struct {
  ast_node_type type;
  char name[MAX_TOKEN_LENGTH + 1];
  int num_args;
} func_decl_node;

typedef struct {
  ast_node_type type;
  ast_node* conditional;
  // a list of type: `ast_node*`
  // that has a node for each line of the
  // "if" body. For example:
  // if (conditional) {
  //   body 
  //   body
  //   body
  // }
  // would parse and have the items in the body
  // stored in the body Deque
  Deque* body;
  ast_node* else_node;
} if_node;

typedef struct {
  ast_node_type type;
  // body is similar to the body specified in the
  // if node
  Deque* body;
} else_node;

typedef struct {
  ast_node_type type;
  ast_node* conditional;
  // body is similar to the body specified in the
  // if node
  Deque* body;
} while_node;

typedef struct {
  ast_node_type type;
  // the expression to be returned.
  // e.g. return a; would have this
  // expression point to a node that represents
  // the identified "a".
  ast_node* expression;
} return_node;

static_assert(
              sizeof(crust_ast_node) >= sizeof(binary_op_node) &&
              sizeof(crust_ast_node) >= sizeof(unary_op_node) &&
              sizeof(crust_ast_node) >= sizeof(if_node) &&
              sizeof(crust_ast_node) >= sizeof(else_node) &&
              sizeof(crust_ast_node) >= sizeof(func_decl_node) &&
              sizeof(crust_ast_node) >= sizeof(func_call_node) &&
              sizeof(crust_ast_node) >= sizeof(let_node) &&
              sizeof(crust_ast_node) >= sizeof(return_node) &&
              sizeof(crust_ast_node) >= sizeof(ident_node) &&
              sizeof(crust_ast_node) >= sizeof(literal_node)
             );

bool parse_crust_file(const char* file_name, crust_ast* to_init);

bool print_ast_node(FILE* stream, ast_node* to_print);

#endif  // CRUST_AST_H_
