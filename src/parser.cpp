#include "parser.h"
#include "instruction.h"
#include "operand2.h"
#include "token.h"

#include <assert.h>
#include <memory>
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

Parser::int_type Parser::parse_immediate_value() {
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
    reg.shift = parse_immediate_value();
  } else if (token::is_register(lexer_.token_kind())) {
    reg.shift = lexer_.token_kind();
  } else {
    assert(false && "expected immediate or register shift");
  }

  lexer_.get_token();
}

std::vector<token::Kind> Parser::parse_register_list(int count) {
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
  if (immediate_follows(lexer_.token_kind())) {
    return ir::Operand2::immediate(parse_immediate_value());
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

  auto cond = ir::Instruction::condition_type::COND_al;
  if (token::is_condition(lexer_.token_kind())) {
    cond = lexer_.token_kind();
    lexer_.get_token();
  }

  auto instruction = std::make_unique<ir::Instruction>(op, cond, update);

  if (token::is_arithmetic_instruction(op)) {
    parse_arithmetic_instruction(instruction);
  } else if (token::is_multiply_instruction(op)) {
    parse_multiply_instruction(instruction);
  } else if (token::is_divide_instruction(op)) {
    parse_divide_instruction(instruction);
  } else if (token::is_move_instruction(op)) {
    parse_move_instruction(instruction);
  } else if (token::is_shift_instruction(op)) {
    parse_shift_instruction(instruction);
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
    assert(false && "not an instruction");
  }

  return instruction;
}

void Parser::parse_arithmetic_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::ArithmeticInstruction>(*instruction.get());

  const auto registers = parse_register_list(2);
  if (registers.size() < 2) {
    assert(false && "not enough registers");
  }

  instr->rd = registers[0];
  instr->rn = registers[1];

  ensure(token::Comma, "expected comma"sv);
  instr->src2 = parse_operand2();

  instruction = std::move(instr);
}

void Parser::parse_multiply_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::MultiplyInstruction>(*instruction.get());

  const auto register_count =
      instruction->opcode() == ir::Instruction::opcode_type::OP_mul ? 3 : 4;

  const auto registers = parse_register_list(register_count);
  if (registers.size() != register_count) {
    assert(false && "not enough registers");
  }

  switch (instr->opcode()) {
  case ir::Instruction::opcode_type::OP_mla:
  case ir::Instruction::opcode_type::OP_mls:
    instr->rn = registers[3];
  case ir::Instruction::opcode_type::OP_mul:
    instr->rd = registers[0];
    instr->rm = registers[1];
    instr->rs = registers[2];
    break;
  case ir::Instruction::opcode_type::OP_umull:
  case ir::Instruction::opcode_type::OP_smull:
  case ir::Instruction::opcode_type::OP_umlal:
  case ir::Instruction::opcode_type::OP_smlal:
    instr->rdlo = registers[0];
    instr->rdhi = registers[1];
    instr->rm = registers[2];
    instr->rs = registers[3];
    break;
  default:
    assert(false && "not multiply instruction");
    break;
  }

  instruction = std::move(instr);
}

void Parser::parse_divide_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::DivideInstruction>(*instruction.get());

  const auto registers = parse_register_list(3);
  if (registers.size() != 3) {
    assert(false && "not enough registers");
  }

  instr->rd = registers[0];
  instr->rn = registers[2];
  instr->rm = registers[3];

  instruction = std::move(instr);
}

void Parser::parse_move_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::MoveInstruction>(*instruction.get());

  const auto registers = parse_register_list(1);
  instr->rd = registers[0];

  switch (instr->opcode()) {
  case ir::Instruction::opcode_type::OP_mov:
  case ir::Instruction::opcode_type::OP_mvn: {
    const auto op2 = parse_operand2();
    instr->src = op2;
    break;
  }
  case ir::Instruction::opcode_type::OP_movt:
  case ir::Instruction::opcode_type::OP_movw: {
    const auto val = parse_immediate_value();
    ensure(token::Integer, "expected imm16"sv);
    if (val < 0 || val > 65535) {
      assert(false && "imm16 must be between 0 and 65535");
    }
    instr->src = static_cast<ir::MoveInstruction::imm16>(val);
    break;
  }
  default:
    assert(false && "not a move instruction");
  }

  instruction = std::move(instr);
}

void Parser::parse_shift_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::MoveInstruction>(*instruction.get());

  const auto registers = parse_register_list(2);
  if (registers.size() != 2) {
    assert(false && "not enough registers");
  }

  instr->rd = registers[0];
  auto src = ir::ShiftedRegister{};
  src.rm = registers[1];
  src.op = instr->opcode();

  expect(token::Comma, "expected comma"sv);

  if (token::is_register(lexer_.token_kind())) {
    src.shift = lexer_.token_kind();
    lexer_.get_token();
  } else if (immediate_follows(lexer_.token_kind())) {
    src.shift = parse_immediate_value();
  } else {
    assert(false && "expected register or immediate value");
  }

  auto op2 = ir::Operand2{};
  op2.operand = src;
  instr->src = op2;
  instr->op_ = ir::Instruction::opcode_type::OP_mov;

  instruction = std::move(instr);
}

void Parser::parse_comparison_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::CompareInstruction>(*instruction.get());

  const auto registers = parse_register_list(1);
  if (registers.size() != 1) {
    assert(false && "not enough registers");
  }

  expect(token::Comma, "expected comma"sv);

  instr->rn = registers[0];
  instr->src2 = parse_operand2();

  instruction = std::move(instr);
}

void Parser::parse_bitfield_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::BitfieldInstruction>(*instruction.get());

  const auto has_rn =
      instruction->opcode() != ir::Instruction::opcode_type::OP_bfc;

  const auto register_count = has_rn ? 2 : 1;

  const auto registers = parse_register_list(register_count);
  if (registers.size() != register_count) {
    assert(false && "not enough registers");
  }

  instr->rd = registers[0];
  if (has_rn) {
    instr->rn = registers[1];
  }

  expect(token::Comma, "expected comma"sv);
  instr->lsb = parse_immediate_value();
  expect(token::Comma, "expected comma"sv);
  instr->width = parse_immediate_value();

  instruction = std::move(instr);
}

void Parser::parse_reverse_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::ReverseInstruction>(*instruction.get());

  const auto registers = parse_register_list(2);
  if (registers.size() != 2) {
    assert(false && "not enough registers");
  }

  instr->rd = registers[0];
  instr->rm = registers[1];

  instruction = std::move(instr);
}

void Parser::parse_branch_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::BranchInstruction>(*instruction.get());

  switch (instruction->opcode()) {
  case ir::Instruction::opcode_type::OP_cbz:
  case ir::Instruction::opcode_type::OP_cbnz: {
    const auto registers = parse_register_list(1);
    if (registers.size() != 1) {
      assert(false && "not enough registers");
    }

    instr->rn = registers[0];
    break;
  }
  case ir::Instruction::opcode_type::OP_bx: {
    const auto registers = parse_register_list(1);
    if (registers.size() != 1) {
      assert(false && "not enough registers");
    }

    instr->rm = registers[0];
    break;
  }
  default:
    break;
  }

  if (instruction->opcode() != ir::Instruction::opcode_type::OP_bx) {
    if (lexer_.token_kind() == token::Label) {
      instr->target = lexer_.string_value();
      lexer_.get_token();
    } else if (immediate_follows(lexer_.token_kind())) {
      instr->target = parse_immediate_value();
    } else {
      assert(false && "expected label or immediate value");
    }
  }

  instruction = std::move(instr);
}

void Parser::parse_single_memory_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}

void Parser::parse_multiple_memory_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {}
