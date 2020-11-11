#ifndef AAVM_PARSER_H
#define AAVM_PARSER_H

#include "lexer.h"
#include "textbuffer.h"
#include "token.h"

#include <string_view>

namespace aavm::parser {

class Parser {
public:
  Parser(const Charbuffer &buffer);

private:
  void expect(token::Kind kind, std::string_view message);

  bool parse_instruction();
  bool parse_arithmetic_instruction();
  bool parse_multiply_instruction();
  bool parse_divide_instruction();
  bool parse_move_instruction();
  bool parse_shift_instruction();
  bool parse_compare_instruction();
  bool parse_logical_instruction();
  bool parse_operand2();
  bool parse_bitfield_instruction();
  bool parse_reverse_instruction();
  bool parse_branch_instruction();
  bool parse_address_instruction();
  bool parse_memory_instruction();
  bool parse_multiple_memory_instruction();
  bool parse_stack_instruction();
  bool parse_register_list();

  Lexer lexer_;
};

} // namespace aavm::parser

#endif
