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

struct LexerState {
  token::Kind token;
  unsigned int_value;
  std::string_view string_value;
};

class Lexer {
public:
  Lexer(Charbuffer &text)
      : text_{text}, cursor_{text_.begin()}, current_char_{0},
        column_number_{1}, line_number_{1}, current_token_{token::Error},
        int_value_{0}, string_value_{} {
    // prime the first character
    get_char();
  }

  auto kind() const { return current_token_; }
  auto int_value() const { return int_value_; }
  auto string_value() const { return string_value_; }

  auto source_location() const {
    return SourceLocation{column_number_, line_number_, cursor_};
  }

  auto set_state(LexerState state) {
    current_token_ = state.token;
    int_value_ = state.int_value;
    string_value_ = state.string_value;
  }

  auto save_state() const {
    return LexerState{current_token_, int_value_, string_value_};
  }

  auto get_token() {
    if (!saved_states_.empty()) {
      const auto state = saved_states_.front();
      saved_states_.pop();
      set_state(state);
    } else {
      const auto tok = lex_token();
      current_token_ = tok;
    }

    return current_token_;
  }

  auto peek_token() {
    if (!saved_states_.empty()) {
      return saved_states_.front().token;
    }

    // save the current state
    const auto saved_state = save_state();
    const auto tok = lex_token();
    // push the state after lexing
    saved_states_.push(save_state());
    // restore the state before lexing
    set_state(saved_state);
    return tok;
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

  const Charbuffer &text_;
  Charbuffer::iterator cursor_;
  int current_char_;
  std::size_t column_number_;
  std::size_t line_number_;

  token::Kind current_token_;
  unsigned int_value_;
  std::string_view string_value_;

  std::queue<LexerState> saved_states_;
};

} // namespace aavm::parser

#endif
