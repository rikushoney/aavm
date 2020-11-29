#ifndef AAVM_PARSER_H
#define AAVM_PARSER_H

#include "lexer.h"
#include "operand2.h"
#include "textbuffer.h"
#include "token.h"

#include <assert.h>
#include <string_view>
#include <type_traits>
#include <vector>

namespace aavm::parser {

class Parser {
public:
  Parser(const Charbuffer &buffer);

private:
  void expect(token::Kind kind, std::string_view message);
  template <typename Pred> void expect(Pred &&p, std::string_view message) {
    if (!p(lexer_.get_token())) {
      assert(false);
    }
  }

  void ensure_newline();

  std::vector<token::Kind> parse_register_list(int count);
  // TODO: add proper return types and parameters
  Operand2 parse_operand2();
  void parse_instruction();
  void parse_arithmetic_instruction();
  void parse_multiply_instruction();
  void parse_divide_instruction();
  void parse_move_instruction();
  void parse_shift_instruction();
  void parse_compare_instruction();
  void parse_logical_instruction();
  void parse_bitfield_instruction();
  void parse_reverse_instruction();
  void parse_branch_instruction();
  void parse_address_instruction();
  void parse_memory_instruction();
  void parse_multiple_memory_instruction();
  void parse_stack_instruction();

  Lexer lexer_;
};

} // namespace aavm::parser

#endif
