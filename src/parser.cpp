#include "parser.h"
#include "array.h"
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

constexpr auto is_single_memory_load_instruction(token::Kind kind) {
  switch (kind) {
  case token::OP_ldr:
  case token::OP_ldrb:
  case token::OP_ldrh:
  case token::OP_ldrsb:
  case token::OP_ldrsh:
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
    reg.op = token::OP_lsl;
    reg.shift = 0;
  } else {
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
}

std::vector<token::Kind> Parser::parse_register_list(std::size_t count) {
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

std::vector<token::Kind> Parser::parse_register_range() {
  auto vec = std::vector<token::Kind>{};

  const auto register_start = lexer_.token_kind();
  assert(lexer_.get_token() == token::Minus && "expected minus");
  const auto register_end = lexer_.get_token();

  // HACK: this is ghetto
  constexpr auto register_map =
      make_array({token::REG_r0, token::REG_r1, token::REG_r2, token::REG_r3,
                  token::REG_r4, token::REG_r5, token::REG_r6, token::REG_r7,
                  token::REG_r8, token::REG_r9, token::REG_r10, token::REG_r11,
                  token::REG_r12, token::REG_sp, token::REG_lr, token::REG_pc});

  const auto last = static_cast<std::size_t>(register_end - token::REG_r0);

  for (auto i = static_cast<std::size_t>(register_start - token::REG_r0);
       i <= last; ++i) {
    vec.push_back(register_map[i]);
  }

  lexer_.get_token();

  return vec;
}

ir::Operand2 Parser::parse_operand2() {
  const auto subtract = lexer_.token_kind() == token::Minus &&
                        token::is_register(lexer_.peek_token());

  if (!subtract && immediate_follows(lexer_.token_kind())) {
    return ir::Operand2::immediate(parse_immediate_value());
  }

  auto reg = ir::ShiftedRegister{};
  parse_shifted_register(reg);
  reg.subtract = subtract;
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

  ensure(
      [](const auto tok) { return tok == token::Newline || tok == token::Eof; },
      "only single instruction permitted per line"sv);

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
      instruction->operation() == ir::Instruction::operation_type::OP_mul ? 3
                                                                          : 4;

  const auto registers = parse_register_list(register_count);
  if (registers.size() < register_count) {
    assert(false && "not enough registers");
  }

  switch (instr->operation()) {
  case ir::Instruction::operation_type::OP_mla:
  case ir::Instruction::operation_type::OP_mls:
    instr->rn = registers[3];
  case ir::Instruction::operation_type::OP_mul:
    instr->rd = registers[0];
    instr->rm = registers[1];
    instr->rs = registers[2];
    break;
  case ir::Instruction::operation_type::OP_umull:
  case ir::Instruction::operation_type::OP_smull:
  case ir::Instruction::operation_type::OP_umlal:
  case ir::Instruction::operation_type::OP_smlal:
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
  if (registers.size() < 3) {
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

  instr->rd = lexer_.token_kind();
  ensure(token::is_register, "expected register"sv);
  ensure(token::Comma, "expected comma"sv);

  switch (instr->operation()) {
  case ir::Instruction::operation_type::OP_mov:
  case ir::Instruction::operation_type::OP_mvn: {
    const auto op2 = parse_operand2();
    instr->src = op2;
    break;
  }
  case ir::Instruction::operation_type::OP_movt:
  case ir::Instruction::operation_type::OP_movw: {
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
  if (registers.size() < 2) {
    assert(false && "not enough registers");
  }

  instr->rd = registers[0];
  auto src = ir::ShiftedRegister{};
  src.rm = registers[1];
  src.op = instr->operation();

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
  instr->op_ = ir::Instruction::operation_type::OP_mov;

  instruction = std::move(instr);
}

void Parser::parse_comparison_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::CompareInstruction>(*instruction.get());

  instr->rn = lexer_.token_kind();
  ensure(token::is_register, "expected register"sv);

  ensure(token::Comma, "expected comma"sv);

  instr->src2 = parse_operand2();

  instruction = std::move(instr);
}

void Parser::parse_bitfield_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::BitfieldInstruction>(*instruction.get());

  const auto has_rn =
      instruction->operation() != ir::Instruction::operation_type::OP_bfc;

  const auto register_count = has_rn ? 2 : 1;

  const auto registers = parse_register_list(register_count);
  if (registers.size() < register_count) {
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
  if (registers.size() < 2) {
    assert(false && "not enough registers");
  }

  instr->rd = registers[0];
  instr->rm = registers[1];

  instruction = std::move(instr);
}

void Parser::parse_branch_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr = std::make_unique<ir::BranchInstruction>(*instruction.get());

  switch (instruction->operation()) {
  case ir::Instruction::operation_type::OP_cbz:
  case ir::Instruction::operation_type::OP_cbnz: {
    instr->rn = lexer_.token_kind();
    ensure(token::is_register, "expected register"sv);
    break;
  }
  case ir::Instruction::operation_type::OP_bx: {
    instr->rm = lexer_.token_kind();
    ensure(token::is_register, "expected register"sv);
    break;
  }
  default:
    break;
  }

  if (instruction->operation() != ir::Instruction::operation_type::OP_bx) {
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
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr =
      std::make_unique<ir::SingleMemoryInstruction>(*instruction.get());

  instr->rd = lexer_.token_kind();
  ensure(token::is_register, "expected register"sv);
  ensure(token::Comma, "expected comma"sv);

  if (lexer_.token_kind() == token::Equal) {
    if (immediate_follows(lexer_.get_token())) {
      if (instr->operation() != token::OP_ldr) {
        assert(false && "only ldr is supported");
      }
      instr->source = parse_immediate_value();
    } else {
      if (!is_single_memory_load_instruction(instr->operation())) {
        assert(false && "only ldr-like instructions are supported");
      }
      instr->source = lexer_.string_value();
      lexer_.get_token();
    }
  } else {
    ensure(token::Lbracket, "expected opening bracket"sv);
    instr->rn = lexer_.token_kind();
    ensure(token::is_register, "expected register"sv);

    if (lexer_.token_kind() == token::Rbracket) {
      instr->indexmode = ir::SingleMemoryInstruction::IndexMode::Post;
      if (lexer_.get_token() == token::Comma) {
        lexer_.get_token();
        instr->source = parse_operand2();
      } else {
        instr->source = ir::Operand2::immediate(0);
      }
    } else {
      ensure(token::Comma, "expected comma"sv);
      instr->source = parse_operand2();
      ensure(token::Rbracket, "expected closing bracket"sv);
      if (lexer_.token_kind() == token::Exclaim) {
        lexer_.get_token();
        instr->indexmode = ir::SingleMemoryInstruction::IndexMode::Pre;
      } else {
        instr->indexmode = ir::SingleMemoryInstruction::IndexMode::Offset;
      }
    }
  }

  instruction = std::move(instr);
}

void Parser::parse_multiple_memory_instruction(
    std::unique_ptr<ir::Instruction> &instruction) {
  auto instr =
      std::make_unique<ir::MultipleMemoryInstruction>(*instruction.get());

  instr->rn = lexer_.token_kind();
  ensure(token::is_register, "expected register"sv);
  ensure(token::Comma, "expected comma"sv);
  ensure(token::Lbrace, "expected opening brace"sv);

  while (lexer_.token_kind() != token::Rbrace) {
    if (!token::is_register(lexer_.token_kind())) {
      assert(false && "expected register");
    }

    if (lexer_.peek_token() == token::Minus) {
      const auto register_range = parse_register_range();
      instr->register_list.insert(instr->register_list.end(),
                                  register_range.begin(), register_range.end());
      if (lexer_.token_kind() == token::Comma) {
        lexer_.get_token();
      }
    } else {
      instr->register_list.push_back(lexer_.token_kind());
      if (lexer_.get_token() == token::Comma) {
        lexer_.get_token();
      }
    }
  }

  lexer_.get_token();

  instruction = std::move(instr);
}

std::vector<std::unique_ptr<ir::Instruction>> Parser::parse_instructions() {
  auto instructions = std::vector<std::unique_ptr<ir::Instruction>>{};

  while (lexer_.token_kind() != token::Eof) {
    instructions.push_back(parse_instruction());
  }

  return instructions;
}
