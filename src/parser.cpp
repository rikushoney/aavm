#include "parser.h"
#include "token.h"

#include <assert.h>
#include <utility>

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

std::unique_ptr<Instruction> Parser::parse_instruction() {
  const auto op = lexer_.token_kind();
  assert(token::is_instruction(op));

  auto update = false;
  if (lexer_.get_token() == token::S) {
    update = true;
    lexer_.get_token();
  }

  auto cond = Instruction::condition_type::COND_al;
  if (token::is_condition(lexer_.token_kind())) {
    cond = lexer_.token_kind();
  }

  auto instruction = std::make_unique<Instruction>(op, cond, update);

  if (token::is_arithmetic_instruction(op)) {
    parse_arithmetic_instruction(instruction);
  } else if (token::is_multiply_instruction(op)) {
    parse_multiply_instruction(instruction);
  } else if (token::is_divide_instruction(op)) {
    parse_divide_instruction(instruction);
  } else if (token::is_move_instruction(op)) {
    parse_move_instruction(instruction);
  } else if (token::is_comparison_instruction(op)) {
    parse_comparison_instruction(instruction);
  } else if (token::is_bitfield_instruction(op)) {
    parse_bitfield_instruction(instruction);
  } else if (token::is_reverse_instruction(op)) {
    parse_reverse_instruction(instruction);
  } else if (token::is_branch_instruction(op)) {
    parse_branch_instruction(instruction);
  } else if (token::is_single_memory_instruction(op)) {
    parse_single_memory_instruction(instruction);
  } else if (token::is_block_memory_instruction(op)) {
    parse_multiple_memory_instruction(instruction);
  } else {
    // not a valid instruction
    assert(false);
  }

  return instruction;
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
    expect(token::Integer, "expected integer value"sv);
    const auto imm = lexer_.int_value();
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

  auto shift = shift_variant{};
  if (lexer_.get_token() == token::Numbersym) {
    expect(token::Integer, "expected integer value"sv);
    shift = lexer_.int_value();
  } else if (lexer_.token_kind() == token::Integer) {
    shift = lexer_.int_value();
  } else if (token::is_register(lexer_.token_kind())) {
    shift = lexer_.token_kind();
  } else {
    assert(false);
    // error
  }

  lexer_.get_token();
  return Operand2::shifted_register(rm, shift_type, shift);
}

void Parser::parse_arithmetic_instruction(
    std::unique_ptr<Instruction> &instruction) {
  // look into avoiding the unnecessary copying
  auto instr = std::make_unique<ArithmeticInstruction>(*instruction.get());

  const auto registers = parse_register_list(2);
  if (registers.size() < 2) {
    assert(false);
    // not enough registers
  }

  instr->rd = registers[0];
  instr->rn = registers[1];

  expect(token::Comma, "expected comma"sv);

  lexer_.get_token();

  if (instr->opcode() != Instruction::opcode_type::OP_rrx) {
    instr->operand2 = parse_operand2();
  }

  instruction = std::move(instr);
}
