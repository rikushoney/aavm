#ifndef AAVM_LEXER_H
#define AAVM_LEXER_H

#include "textbuffer.h"
#include "token.h"

#include <cstdint>
#include <string>
#include <string_view>

namespace aavm::parser {

class Lexer {
public:
  Lexer(Charbuffer &buffer);

  token::Kind lex_token();

  auto string_value() const { return string_value_; }
  auto int_value() const { return int_value_; }

private:
  char next_char();

  void lex_identifier(token::Kind &tok);
  void lex_integer(token::Kind &tok);

  Charbuffer &buffer_;
  Charbuffer::iterator buffer_cursor_;
  char cur_char_;

  std::string_view string_value_;
  std::int32_t int_value_;
};

} // namespace aavm::parser

#endif
