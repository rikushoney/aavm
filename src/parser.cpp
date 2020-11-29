#include "parser.h"

#include <assert.h>

using namespace aavm::parser;
using namespace std::string_view_literals;

Parser::Parser(const Charbuffer &buffer) : lexer_{buffer} {
  // prime the lexer
  lexer_.get_token();
}

void Parser::expect(token::Kind kind, std::string_view message) {
  if (lexer_.get_token() != kind) {
    assert(false);
    // did not get expected token
  }
}

void Parser::ensure_newline() {
  if (lexer_.token_kind() != token::Newline) {
    assert(false);
    // expected newline after instruction
  }

  lexer_.get_token();
}

void Parser::parse_instruction() {
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
  return;
}

std::vector<token::Kind> Parser::parse_register_list(int count) {
  auto vec = std::vector<token::Kind>{};

  if (!token::is_register(lexer_.token_kind())) {
    assert(false);
    return vec;
  }

  vec.push_back(lexer_.token_kind());

  //               order here is important!
  for (auto i = 0; lexer_.get_token() != token::Comma && i < count; ++i) {
    if (!token::is_register(lexer_.peek_token())) {
      break;
    }

    vec.push_back(lexer_.get_token());
  }

  return vec;
}

Operand2 Parser::parse_operand2() {
  if (lexer_.token_kind() == token::Numbersym) {
    expect(token::Integer, "expected constant integer value"sv);
    const auto imm = lexer_.int_value().value();
    lexer_.get_token();
    return Operand2::immediate(imm);
  }

  const auto rm = lexer_.token_kind();
  if (!token::is_register(rm)) {
    assert(false);
    // must be register or integer constant
  }

  if (lexer_.get_token() != token::Comma) {
    return Operand2::shifted_register(rm, token::OP_lsl, 0);
  }

  expect(token::Comma, "expected comma"sv);
  expect(token::is_shift_instruction, "expected shift type"sv);
  const auto shift_type = lexer_.token_kind();

  auto shift = Operand2::shift_variant{};
  if (lexer_.get_token() == token::Numbersym) {
    expect(token::Integer, "expected integer value"sv);
    shift = lexer_.int_value().value();
  } else if (token::is_register(lexer_.token_kind())) {
    shift = lexer_.token_kind();
  } else {
    assert(false);
    // error
  }

  lexer_.get_token();
  return Operand2::shifted_register(rm, shift_type, shift);
}

void Parser::parse_arithmetic_instruction() {
  if (lexer_.get_token() == token::S) {
    lexer_.get_token();
  }

  const auto registers = parse_register_list(2);
  if (registers.size() != 2) {
    assert(false);
    // not enough registers
  }

  expect(token::Comma, "expected comma"sv);

  lexer_.get_token();
  parse_operand2();
}
