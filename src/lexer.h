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

class Lexer {
public:
  using string_type = std::string_view;
  using int_type = std::int32_t;

  Lexer(const Charbuffer &buffer);

  auto get_token() {
    lex_token(kind_);
    return kind_;
  }

  auto string_value() const {
    return kind_ == token::Label ? std::make_optional(string_value_)
                                 : std::nullopt;
  }

  auto int_value() const {
    return kind_ == token::Integer ? std::make_optional(int_value_)
                                   : std::nullopt;
  }

private:
  auto next_char() { return cur_char_ = *buffer_cursor_++; }

  void lex_token(token::Kind &tok);
  void lex_identifier(token::Kind &tok);
  void lex_integer(token::Kind &tok);

  const Charbuffer &buffer_;
  Charbuffer::iterator buffer_cursor_;
  char cur_char_;

  token::Kind kind_;
  string_type string_value_;
  int_type int_value_;

  std::queue<token::Kind> token_queue_;
};

} // namespace aavm::parser

#endif
