#include "keyword_map.h"
#include "lexer.h"
#include "textbuffer.h"
#include "token.h"

#include <fstream>
#include <iostream>

using namespace aavm;

parser::token::Kind dump_token(parser::Lexer &lex) {
  const auto tok{lex.get_token()};

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
    std::cout << "Label \"" << lex.string_value().value() << "\"\n";
    break;
  case parser::token::Integer:
    std::cout << "Integer " << lex.int_value().value() << "\n";
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
  case parser::token::S:
    std::cout << "S\n";
    break;
  default:
    const auto str = parser::keyword::to_string(tok);
    if (str) {
      if (tok > parser::token::COND_START && tok < parser::token::COND_END) {
        std::cout << "Condition " << str.value() << "\n";
      } else if (tok > parser::token::REG_START &&
                 tok < parser::token::REG_END) {
        std::cout << "Register " << str.value() << "\n";
      } else if (tok > parser::token::OP_START && tok < parser::token::OP_END) {
        std::cout << "Instruction " << str.value() << "\n";
      } else if (tok > parser::token::DIR_START &&
                 tok < parser::token::DIR_END) {
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
    std::cout << "usage: testlexer [filename]\n";
    return -1;
  }

  auto filestream = std::fstream{argv[1], std::ios_base::in};
  const auto buffer = Charbuffer{filestream};
  auto lexer = parser::Lexer{buffer};

  auto tok = parser::token::Kind{};
  do {
    tok = dump_token(lexer);

    if (tok == parser::token::Error) {
      return -1;
    }
  } while (tok != parser::token::Eof);

  return 0;
}
