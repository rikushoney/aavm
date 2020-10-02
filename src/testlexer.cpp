#include "lexer.h"
#include "textbuffer.h"
#include "token.h"

#include <fstream>
#include <iostream>

using namespace aavm;

parser::token::Kind dump_token(parser::Lexer &lex) {
  parser::token::Kind tok{lex.lex_token()};

  switch (tok) {
  case parser::token::Error:
    std::cout << "Error!\n";
    break;
  case parser::token::Eof:
    std::cout << "End of file\n";
    break;
  case parser::token::Newline:
    std::cout << "Newline\n";
    break;
  case parser::token::Label:
    std::cout << "Label \"" << lex.string_value() << "\"\n";
    break;
  case parser::token::Integer:
    std::cout << "Integer " << lex.int_value() << "\n";
    break;
  case parser::token::Comma:
    std::cout << "Comma\n";
    break;
  case parser::token::Exclaim:
    std::cout << "Exclaim\n";
    break;
  case parser::token::Equal:
    std::cout << "Equal\n";
    break;
  case parser::token::Numbersym:
    std::cout << "Number symbol\n";
    break;
  case parser::token::Lbracket:
    std::cout << "Left bracket\n";
    break;
  case parser::token::Rbracket:
    std::cout << "Right bracket\n";
    break;
  case parser::token::Lbrace:
    std::cout << "Left brace\n";
    break;
  case parser::token::Rbrace:
    std::cout << "Right brace\n";
    break;
  case parser::token::Minus:
    std::cout << "Minus\n";
    break;
  case parser::token::Colon:
    std::cout << "Colon\n";
    break;
  case parser::token::Period:
    std::cout << "Period\n";
    break;
  default:
    std::cout << "Unknown!\n";
    break;
  }

  return tok;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: testlexer [filename]\n";
    return -1;
  }

  auto filestream = std::fstream{argv[1], std::ios_base::in};
  auto buffer = Charbuffer{filestream};
  auto lexer = parser::Lexer{buffer};

  parser::token::Kind tok{};
  do {
    tok = dump_token(lexer);

    if (tok == parser::token::Error) {
      return -1;
    }
  } while (tok != parser::token::Eof);

  return 0;
}
