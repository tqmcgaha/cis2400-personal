: add { a b } a b + ;
: seven { } 7 ;
: square { a } ( this is a comment that starts and ends with the paren ) a a * ;

: cube { a }
  a
  square
  a 
  *
  / multi line function, the / indicates start of a line comment
  ;

// have args be put into registers
// callee/caller saved registers would be good
// need this for calling a function: hash table<func_name, func_signature (Args)>
// have all functions return exactly just the top thing of the local frame
