#include "lexer.h"
#include "character.h"
#include "keyword.h"
#include "token.h"
#include <iterator>

using namespace aavm;
using namespace aavm::parser;

token::Kind Lexer::lex_integer() {
  auto radix = 10; // assume decimal
  int_value_ = 0;

  // check for prefixes that specify base
  if (current_char_ == '0') {
    // numbers that start with "0" imply octal unless followed by an "X" to
    // specify hexidecimal or "B" for binary numbers
    switch (get_char()) {
    case 'x':
    case 'X':
      radix = 16;
      get_char();
      break;
    case 'b':
    case 'B':
      radix = 2;
      get_char();
      break;
    default:
      radix = 8;
      break;
    }
  }

  while (is_xdigit(current_char_)) {
    const auto digit = ctoi(current_char_);
    if (digit >= radix) {
      // digit not valid in base
      return token::Error;
    }

    int_value_ = int_value_ * radix + digit;
    if (int_value_ < digit) {
      // overflow occurred
      return token::Error;
    }

    get_char();
  }

  return token::Integer;
}

token::Kind Lexer::lex_identifier() {
  const auto id_start = std::prev(cursor_);

  while (is_alnum(current_char_) || current_char_ == '_') {
    get_char();
  }

  string_value_ = text_.view(id_start, std::prev(cursor_));
  const auto lowercase_string = to_lower(string_value_);

  // here we assume the identifier is an instruction
  auto maybe_instruction = std::string_view{lowercase_string};

  auto condition = keyword::none;
  if (maybe_instruction.length() > 2) {
    const auto maybe_condition =
        maybe_instruction.substr(maybe_instruction.length() - 2);
    condition = keyword::find(std::string_view{to_lower(maybe_condition)});
    if (condition != keyword::none) {
      maybe_instruction.remove_suffix(condition->first.length());
    }
  }

  const auto updates_flags = to_lower(maybe_instruction.back()) == 's';
  if (updates_flags) {
    maybe_instruction.remove_suffix(1);
  }

  const auto operation = keyword::find(maybe_instruction);

  if (operation != keyword::none) {
    if (updates_flags) {
      token_queue_.push(token::UpdateFlag);
    }

    if (condition != keyword::none) {
      token_queue_.push(condition->second);
    }

    return operation->second;
  }

  return token::Label;
}

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
      if (is_digit(current_char_)) {
        return lex_integer();
      }

      if (is_alpha(current_char_) || current_char_ == '_') {
        return lex_identifier();
      }

      return token::Error;
    }

    get_char();
    return tok;
  }

  return token::Eof;
}
