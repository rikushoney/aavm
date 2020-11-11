#include "parser.h"

#include <assert.h>

using namespace aavm::parser;

Parser::Parser(const Charbuffer &buffer) : lexer_{buffer} {
  lexer_.get_token();
}

void Parser::expect(token::Kind kind, std::string_view message) {
  const auto tok = lexer_.get_token();
  assert(tok == kind);
  if (tok != kind) {
    // display message (and exit?)
  }
}

bool Parser::parse_instruction() {
  const auto tok = lexer_.token_kind();
  assert(token::is_instruction(tok));

  if (token::is_arithmetic_instruction(tok)) {
    return parse_arithmetic_instruction();
  }

  if (token::is_multiply_instruction(tok)) {
    return parse_multiply_instruction();
  }

  if (token::is_divide_instruction(tok)) {
    return parse_divide_instruction();
  }

  if (token::is_move_instruction(tok)) {
    return parse_move_instruction();
  }

  if (token::is_shift_instruction(tok)) {
    return parse_shift_instruction();
  }

  if (token::is_compare_instruction(tok)) {
    return parse_compare_instruction();
  }

  if (token::is_logical_instruction(tok)) {
    return parse_logical_instruction();
  }

  if (token::is_bitfield_instruction(tok)) {
    return parse_bitfield_instruction();
  }

  if (token::is_reverse_instruction(tok)) {
    return parse_reverse_instruction();
  }

  if (token::is_branch_instruction(tok)) {
    return parse_branch_instruction();
  }

  if (token::is_memory_instruction(tok)) {
    return parse_memory_instruction();
  }

  if (token::is_multiple_memory_instruction(tok)) {
    return parse_multiple_memory_instruction();
  }

  if (token::is_stack_instruction(tok)) {
    return parse_stack_instruction();
  }

  // not an instruction
  return false;
}
