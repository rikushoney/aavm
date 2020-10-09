#include "lexer.h"
#include "character.h"
#include "keyword_map.h"

#include <iterator>

using namespace aavm::parser;

Lexer::Lexer(const Charbuffer &buffer)
    : buffer_{buffer}, buffer_cursor_{buffer_.begin()} {
  // prime the first character
  next_char();
}

void Lexer::lex_identifier(token::Kind &tok) {
  const auto tok_start = std::prev(buffer_cursor_);

  while (is_alnum(cur_char_)) {
    next_char();
  }

  string_value_ = buffer_.view(tok_start, std::prev(buffer_cursor_));
  // it will become apparent why we copy this later
  const auto label_name = string_value_;

  const auto kw = keyword::find([str = to_lower(string_value_)](auto kw) {
    return starts_with(str, kw);
  });

  if (kw == keyword::none) {
    tok = token::Label;
    return;
  }

  tok = kw->second;
  string_value_.remove_prefix(kw->first.length());

  // check for the optional 'S'
  if (to_lower(string_value_.front()) == 's') {
    token_queue_.push(token::S);
    string_value_.remove_prefix(1);
  }

  const auto cond = keyword::find(
      [str = to_lower(string_value_)](auto kw) { return str == kw; });

  if (cond != keyword::none) {
    token_queue_.push(cond->second);
    string_value_.remove_prefix(cond->first.length());
  }

  if (!string_value_.empty()) {
    string_value_ = label_name;
    tok = token::Label;
  }
}

void Lexer::lex_integer(token::Kind &tok) {
  auto radix{10};
  int_value_ = 0;

  if (cur_char_ == '0') {
    switch (next_char()) {
    case 'b':
    case 'B':
      radix = 2;
      next_char();
      break;
    case 'x':
    case 'X':
      radix = 16;
      next_char();
      break;
    default:
      radix = 8;
      break;
    }
  }

  while (is_xdigit(cur_char_)) {
    const auto val = ctoi(cur_char_);

    if (val >= radix) {
      // invalid digit!
      tok = token::Error;
      return;
    }

    int_value_ = int_value_ * radix + val;
    if (int_value_ < 0) {
      // overflow!
      tok = token::Error;
      return;
    }

    next_char();
  }

  tok = token::Integer;
}

void Lexer::lex_token(token::Kind &tok) {
  if (!token_queue_.empty()) {
    tok = token_queue_.front();
    token_queue_.pop();
    return;
  }

  while (buffer_cursor_ != buffer_.end()) {
    switch (cur_char_) {
    case '\r':
    case '\t':
    case ' ':
      next_char();
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
      while (next_char() != '\n') {
        // just skip everything until we reach end of line
      }
      tok = token::Newline;
      break;
    default:
      if (is_alpha(cur_char_)) {
        lex_identifier(tok);
        return;
      }

      if (is_digit(cur_char_)) {
        lex_integer(tok);
        return;
      }

      // invalid character!
      tok = token::Error;
      return;
    }

    next_char();
    return;
  }

  tok = token::Eof;
}
