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

  assert(token::is_register(lexer_.token_kind()));
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

void Parser::parse_operand2() {
  if (lexer_.token_kind() == token::Numbersym) {
    expect(token::Integer, "expected constant integer value"sv);
    lexer_.get_token();
    return;
  }

  if (!token::is_register(lexer_.token_kind())) {
    assert(false);
    // must be register or integer constant
  }

  expect(token::Comma, "expected comma"sv);

  if (lexer_.get_token() == token::Numbersym) {
    expect(token::Integer, "expected integer value"sv);
    expect(token::Comma, "expected comma"sv);
  } else if (token::is_register(lexer_.token_kind())) {
    expect(token::Comma, "expected comma"sv);
  } else {
    assert(false);
    // error
  }

  expect(token::is_shift_instruction, "expected shift type"sv);
  lexer_.get_token();
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
