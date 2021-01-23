#include "lexer.h"
#include "token.h"

using namespace aavm;
using namespace aavm::parser;

token::Kind Lexer::lex_token() {
  auto tok = token::Error;

  while (cursor_ != text_.end()) {
    switch (current_char_) {
    case '\r':
    case '\t':
    case ' ':
      get_char();
      continue;
    case '\n':
      tok = token::Newline;
      break;
    case ',':
      tok = token::Comma;
      break;
    case '!':
      tok = token::Exclaim;
      break;
    case '=':
      tok = token::Equal;
      break;
    case '#':
      tok = token::Numbersym;
      break;
    case '[':
      tok = token::Lbracket;
      break;
    case ']':
      tok = token::Rbracket;
      break;
    case '{':
      tok = token::Lbrace;
      break;
    case '}':
      tok = token::Rbrace;
      break;
    case '-':
      tok = token::Minus;
      break;
    case ':':
      tok = token::Colon;
      break;
    case '.':
      tok = token::Period;
      break;
    case ';':
      while (get_char() != '\n') {
        // skip comment until end of line
      }
      tok = token::Newline;
      break;
    default:
      tok = token::Error;
    }

    get_char();
    return tok;
  }

  return token::Eof;
}
