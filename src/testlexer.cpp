#include "keyword_map.h"
#include "lexer.h"
#include "textbuffer.h"
#include "token.h"

#include <fstream>
#include <iostream>

using namespace aavm;
using namespace aavm::parser;

token::Kind dump_token(Lexer &lex) {
  const auto tok = lex.get_token();

  switch (tok) {
  case token::Error:
    std::cout << "Error!\n";
    break;
  case token::Eof:
    std::cout << "End of file\n";
    break;
  case token::Newline:
    std::cout << "Newline\n";
    break;
  case token::Label:
    std::cout << "Label \"" << lex.string_value() << "\"\n";
    break;
  case token::Integer:
    std::cout << "Integer " << lex.int_value() << "\n";
    break;
  case token::Comma:
    std::cout << "Comma\n";
    break;
  case token::Exclaim:
    std::cout << "Exclaim\n";
    break;
  case token::Equal:
    std::cout << "Equal\n";
    break;
  case token::Numbersym:
    std::cout << "Number symbol\n";
    break;
  case token::Lbracket:
    std::cout << "Left bracket\n";
    break;
  case token::Rbracket:
    std::cout << "Right bracket\n";
    break;
  case token::Lbrace:
    std::cout << "Left brace\n";
    break;
  case token::Rbrace:
    std::cout << "Right brace\n";
    break;
  case token::Minus:
    std::cout << "Minus\n";
    break;
  case token::Colon:
    std::cout << "Colon\n";
    break;
  case token::Period:
    std::cout << "Period\n";
    break;
  case token::S:
    std::cout << "S\n";
    break;
  default:
    const auto str = keyword::to_string(tok);
    if (str) {
      if (token::is_condition(tok)) {
        std::cout << "Condition " << str.value() << "\n";
      } else if (token::is_register(tok)) {
        std::cout << "Register " << str.value() << "\n";
      } else if (token::is_instruction(tok)) {
        std::cout << "Instruction " << str.value() << "\n";
      } else if (token::is_directive(tok)) {
        std::cout << "Directive " << str.value() << "\n";
      } else {
        std::cout << str.value() << "\n";
      }
    } else {
      std::cout << "Invalid token " << tok << "\n";
    }

    break;
  }

  return tok;
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << "[filename]\n";
    return 1;
  }

  auto filestream = std::fstream{argv[1], std::ios_base::in};
  const auto buffer = Charbuffer{filestream};
  auto lexer = Lexer{buffer};

  auto tok = token::Kind{};
  do {
    tok = dump_token(lexer);

    if (tok == token::Error) {
      return 1;
    }
  } while (tok != token::Eof);

  return 0;
}
