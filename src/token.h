#ifndef AAVM_TOKEN_H
#define AAVM_TOKEN_H

namespace aavm::parser::token {

enum Kind {
  Error,
  Eof,

  Newline, // \n
  Label,
  Integer,

  Comma,     // ,
  Exclaim,   // !
  Equal,     // =
  Numbersym, // #
  Lbracket,  // [
  Rbracket,  // ]
  Lbrace,    // {
  Rbrace,    // }
  Minus,     // -
  Colon,     // :
  Period,    // .

  // TODO: add instructions/conditions/registers/directives
};

}

#endif
