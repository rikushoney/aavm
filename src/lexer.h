#ifndef AAVM_LEXER_H
#define AAVM_LEXER_H

#include "textbuffer.h"
#include "token.h"

#include <cstdint>
#include <optional>
#include <queue>
#include <string>
#include <string_view>

namespace aavm::parser {

struct SourceLocation {
  using size_type = std::size_t;

  size_type line_number;
  size_type column_number;
  Charbuffer::iterator text;
};

class Lexer {
public:
  using string_type = std::string_view;
  using int_type = std::int64_t;
  using size_type = std::size_t;

  Lexer(const Charbuffer &buffer);

  auto get_token() {
    if (!token_queue_.empty()) {
      kind_ = token_queue_.front();
      token_queue_.pop();
      return kind_;
    }

    lex_token(kind_);
    return kind_;
  }

  auto peek_token() {
    if (!token_queue_.empty()) {
      return token_queue_.front();
    }

    auto tok = token::Kind{};
    lex_token(tok);
    token_queue_.push(tok);
    return tok;
  }

  auto source_location() const {
    return SourceLocation{line_number_, column_number_, buffer_cursor_};
  }

  auto token_kind() const { return kind_; }

  auto string_value() const { return string_value_; }

  auto int_value() const { return int_value_; }

private:
  auto next_char() {
    cur_char_ = *buffer_cursor_++;
    if (cur_char_ == '\n') {
      ++line_number_;
      column_number_ = 0;
    } else {
      ++column_number_;
    }

    return cur_char_;
  }

  void lex_token(token::Kind &tok);
  void lex_identifier(token::Kind &tok);
  void lex_integer(token::Kind &tok);

  const Charbuffer &buffer_;
  Charbuffer::iterator buffer_cursor_;
  char cur_char_;
  size_type line_number_;
  size_type column_number_;

  token::Kind kind_;
  string_type string_value_;
  int_type int_value_;

  std::queue<token::Kind> token_queue_;
};

} // namespace aavm::parser

#endif
