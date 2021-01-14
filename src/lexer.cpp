#include "lexer.h"
#include "character.h"
#include "keyword_map.h"
#include "token.h"

#include <assert.h>
#include <iterator>

using namespace aavm::parser;

Lexer::Lexer(const Charbuffer &buffer)
    : buffer_{buffer}, buffer_cursor_{buffer_.begin()}, line_number_{1} {
  // prime the first character
  next_char();
}

void Lexer::lex_identifier(token::Kind &tok) {
  const auto tok_start = std::prev(buffer_cursor_);

  while (is_alnum(cur_char_) || cur_char_ == '_' || cur_char_ == '.') {
    next_char();
  }

  string_value_ = buffer_.view(tok_start, std::prev(buffer_cursor_));

  // TODO: does not find blt correctly - finds bl as instruction and then t is
  // an invalid suffix where it should be b with lt suffix
  const auto kw = keyword::find_longest(
      [lowercase = to_lower(string_value_)](const auto kw) {
        return starts_with(lowercase, kw);
      });

  const auto is_valid_register = token::is_register(kw->second) &&
                                 kw->first.length() == string_value_.length();

  tok = kw->second;

  if (is_valid_register) {
    return;
  }

  if (kw == keyword::none || !token::is_instruction(tok)) {
    tok = token::Label;
    return;
  }

  // assume the token is an instruction with optional suffixes
  auto str = string_value_;
  str.remove_prefix(kw->first.length());

  // check for the optional 'S'
  const auto has_update_flag = to_lower(str.front()) == 's';
  if (has_update_flag) {
    token_queue_.push(token::S);
    str.remove_prefix(1);
  }

  const auto cond = keyword::find(
      [lowercase = to_lower(str)](const auto kw) { return lowercase == kw; });

  if (cond != keyword::none) {
    token_queue_.push(cond->second);
    str.remove_prefix(cond->first.length());
  }

  if (!str.empty()) {
    // assumption is incorrect - token is not valid instruction => must be label
    if (cond != keyword::none) {
      token_queue_.pop();
    }

    if (has_update_flag) {
      token_queue_.pop();
    }

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
      assert(false);
      tok = token::Error;
      return;
    }

    int_value_ = int_value_ * radix + val;
    if (int_value_ > 0xFFFFFFFF) {
      // overflow!
      assert(false);
      tok = token::Error;
      return;
    }

    next_char();
  }

  tok = token::Integer;
}

void Lexer::lex_token(token::Kind &tok) {
  while (buffer_cursor_ != buffer_.end()) {
    switch (cur_char_) {
    case '\r':
    case '\t':
    case ' ':
      next_char();
      continue;
    case '\n':
      tok = token::Newline;
      ++line_number_;
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
      ++line_number_;
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
