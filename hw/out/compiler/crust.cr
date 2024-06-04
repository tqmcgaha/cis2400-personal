
// assume syntax is correct

// - function declarations
//   - always assume that the types are int for params and return value
// - variable decl & default init
//   - variables all near the top of function
// - int literals
//   - hexadecimal and decimal
// - function calls
// - all statements are kept trivial with:
//   - one assignment per line (so the LHS is just a variable identifier)
//   - the rhs of an assignment has at most one operand (+,-,/,*,%,==,<=,>=,<,>,!=,~,!,<<,>>)
//     c = 0;
//     c = a;
//     b = b + a;
//     b = c * 3;
//     c = func_call(3, pi);
//     func_call(a, 2, c);
//     b = c < a;

// if/elif/else statements
// while loops

// not allowed
// c += b
// c = a = b
// a = b - a - c
// d = b * b - 4 * a * c
// c = (a + b) * 4;

fn hello(a, b) {
  let c;
  c = 0;

  c = c + a;

  b = b + c;


  print_int(a);
  print_int(b);
  return 0;
}

fn howdy(a, b) {
  let i;

  i = b;
  
  while (i) {
    a = a + 1;
    i = i - 1;
  }

  return 0;
}
