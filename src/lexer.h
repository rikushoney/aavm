#ifndef AAVM_PARSER_LEXER_H_
#define AAVM_PARSER_LEXER_H_

#include "keyword.h"
#include "textbuffer.h"
#include "token.h"
#include <cstddef>
#include <queue>
#include <string_view>

namespace aavm::parser {

struct SourceLocation {
  std::size_t column;
  std::size_t line;
  Charbuffer::iterator cursor;
};

class Lexer {
public:
  Lexer() = delete;
  Lexer(const Charbuffer &text) : text_{text}, cursor_{text_.begin()} {
    // prime the first character
    get_char();
  }

  constexpr auto token_kind() const { return current_token_; }
  constexpr auto int_value() const { return int_value_; }
  constexpr auto string_value() const { return string_value_; }

  // MSVC does not implement std::vector<char>::const_iterator (aka
  // Charbuffer::iterator) as constexpr yet
  /* constexpr */ auto source_location() const {
    return SourceLocation{column_number_, line_number_, cursor_};
  }

  auto get_token() {
    if (!token_queue_.empty()) {
      current_token_ = token_queue_.front();
      token_queue_.pop();
    } else {
      const auto tok = lex_token();
      current_token_ = tok;
    }

    return current_token_;
  }

private:
  int get_char() {
    current_char_ = *cursor_++;
    if (current_char_ == '\n') {
      ++line_number_;
      column_number_ = 0;
    } else {
      ++column_number_;
    }
    return current_char_;
  }

  token::Kind lex_token();
  token::Kind lex_integer();
  token::Kind lex_identifier();

  const Charbuffer &text_;
  Charbuffer::iterator cursor_{};
  int current_char_{};
  std::size_t column_number_{0};
  std::size_t line_number_{1};

  token::Kind current_token_{};
  unsigned int_value_{};
  std::string_view string_value_{};

  std::queue<token::Kind> token_queue_{};
};

} // namespace aavm::parser

#endif
