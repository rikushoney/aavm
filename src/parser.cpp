#include "parser.h"
#include "operand2.h"
#include "token.h"

#include <assert.h>
#include <utility>

using namespace aavm;
using namespace aavm::parser;
using namespace std::string_view_literals;

constexpr auto immediate_follows(token::Kind kind) {
  switch (kind) {
  case token::Numbersym:
  case token::Minus:
  case token::Integer:
    return true;
  default:
    return false;
  }
}

Parser::Parser(const Charbuffer &buffer) : lexer_{buffer} {
  // prime the lexer
  lexer_.get_token();
}

Parser::int_type Parser::parse_immediate() {
  // parse immediate in the form:
  // [#]?[-]?<integer>
  if (lexer_.token_kind() == token::Numbersym) {
    lexer_.get_token();
  }

  const auto negate = lexer_.token_kind() == token::Minus;
  if (negate) {
    lexer_.get_token();
  }

  const auto value = lexer_.int_value();
  ensure(token::Integer, "expected integer value"sv);
  return negate ? 0 - value : value;
}

void Parser::parse_shifted_register(ir::ShiftedRegister &reg) {
  // parse shifted register in the form:
  // <rm>(, <shift_op> (<rs>|<immediate>))?
  reg.rm = lexer_.token_kind();
  ensure(token::is_register, "expected register"sv);

  if (lexer_.token_kind() != token::Comma) {
    // if no shift is specified, just shift left by 0 (value stays unchanged)
    reg.op = token::OP_lsl;
    reg.shift = 0;
  }

  expect(token::is_shift_instruction, "expected shift type"sv);
  reg.op = lexer_.token_kind();

  if (immediate_follows(lexer_.get_token())) {
    reg.shift = parse_immediate();
  } else if (token::is_register(lexer_.token_kind())) {
    reg.shift = lexer_.token_kind();
  } else {
    assert(false && "expected immediate or register shift");
  }

  lexer_.get_token();
}

std::vector<token::Kind> Parser::parse_register_list(int count) {
  // parse up to count registers in the form:
  // (<register>,)+<register>
  auto vec = std::vector<token::Kind>{};

  const auto reg = lexer_.token_kind();
  ensure(token::is_register, "expected register"sv);
  vec.push_back(reg);

  for (auto i = 0; lexer_.token_kind() == token::Comma && i < count - 1; ++i) {
    if (!token::is_register(lexer_.get_token())) {
      break;
    }

    vec.push_back(lexer_.token_kind());
    lexer_.get_token();
  }

  return vec;
}

ir::Operand2 Parser::parse_operand2() {
  // parse immediate or shifted register in the form:
  // (<immediate>|<shifted_register>)
  if (immediate_follows(lexer_.token_kind())) {
    return ir::Operand2::immediate(parse_immediate());
  }

  auto reg = ir::ShiftedRegister{};
  parse_shifted_register(reg);
  return ir::Operand2::shifted_register(reg);
}

std::unique_ptr<ir::Instruction> Parser::parse_instruction() {
  const auto op = lexer_.token_kind();
  assert(token::is_instruction(op));

  auto update = false;
  if (lexer_.get_token() == token::S) {
    update = true;
    lexer_.get_token();
  }

  const auto cond = token::is_condition(lexer_.token_kind())
                        ? lexer_.token_kind()
                        : ir::Instruction::condition_type::COND_al;

  auto instruction = std::make_unique<ir::Instruction>(op, cond, update);

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

void Parser::parse_arithmetic_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  // parse arithmetic instruction in the form:
  // <opcode> <rd>, <rn>, <operand2>
  // TODO: look into avoiding the unnecessary copying
  auto instr = std::make_unique<ir::ArithmeticInstruction>(*instruction.get());

  const auto registers = parse_register_list(2);
  if (registers.size() < 2) {
    assert(false && "not enough registers");
  }

  instr->rd = registers[0];
  instr->rn = registers[1];

  ensure(token::Comma, "expected comma"sv);

  if (instr->opcode() != ir::Instruction::opcode_type::OP_rrx) {
    instr->operand2 = parse_operand2();
  }

  instruction = std::move(instr);
}

void Parser::parse_multiply_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_divide_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_move_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_comparison_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_bitfield_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_reverse_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_branch_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_single_memory_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_multiple_memory_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}
