#include "parser.h"
#include "instructions.h"
#include "operand2.h"
#include "register.h"
#include "token.h"
#include <memory>

using namespace aavm;
using namespace aavm::parser;
using namespace aavm::ir;
using namespace std::string_view_literals;

constexpr unsigned map_token(token::Kind token) {
  using namespace aavm::token;

  switch (token) {
  case kw_eq:
    return condition::EQ;
  case kw_ne:
    return condition::NE;
  case kw_cs:
    return condition::CS;
  case kw_cc:
    return condition::CC;
  case kw_mi:
    return condition::MI;
  case kw_pl:
    return condition::PL;
  case kw_vs:
    return condition::VS;
  case kw_vc:
    return condition::VC;
  case kw_hi:
    return condition::HI;
  case kw_ls:
    return condition::LS;
  case kw_ge:
    return condition::GE;
  case kw_lt:
    return condition::LT;
  case kw_gt:
    return condition::GT;
  case kw_le:
    return condition::LE;
  case kw_al:
    return condition::AL;
  case kw_r0:
    return Register::R0;
  case kw_r1:
    return Register::R1;
  case kw_r2:
    return Register::R2;
  case kw_r3:
    return Register::R3;
  case kw_r4:
    return Register::R4;
  case kw_r5:
    return Register::R5;
  case kw_r6:
    return Register::R6;
  case kw_r7:
    return Register::R7;
  case kw_r8:
    return Register::R8;
  case kw_r9:
    return Register::R9;
  case kw_r10:
    return Register::R10;
  case kw_r11:
    return Register::R11;
  case kw_r12:
    return Register::R12;
  case kw_r13:
  case kw_sp:
    return Register::SP;
  case kw_r14:
  case kw_lr:
    return Register::LR;
  case kw_r15:
  case kw_pc:
    return Register::PC;
  case kw_add:
    return Instruction::Add;
  case kw_adc:
    return Instruction::Adc;
  case kw_sub:
    return Instruction::Sub;
  case kw_sbc:
    return Instruction::Sbc;
  case kw_rsb:
    return Instruction::Rsb;
  case kw_rsc:
    return Instruction::Rsc;
  case kw_and:
    return Instruction::And;
  case kw_eor:
    return Instruction::Eor;
  case kw_orr:
    return Instruction::Orr;
  case kw_bic:
    return Instruction::Bic;
  case kw_adr:
    return Instruction::Adr;
  case kw_asr:
    return Instruction::Asr;
  case kw_lsl:
    return Instruction::Lsl;
  case kw_lsr:
    return Instruction::Lsr;
  case kw_ror:
    return Instruction::Ror;
  case kw_rrx:
    return Instruction::Rrx;
  case kw_mul:
    return Instruction::Mul;
  case kw_mla:
    return Instruction::Mla;
  case kw_mls:
    return Instruction::Mls;
  case kw_umull:
    return Instruction::Umull;
  case kw_umlal:
    return Instruction::Umlal;
  case kw_smull:
    return Instruction::Smull;
  case kw_smlal:
    return Instruction::Smlal;
  case kw_sdiv:
    return Instruction::Sdiv;
  case kw_udiv:
    return Instruction::Udiv;
  case kw_mov:
    return Instruction::Mov;
  case kw_mvn:
    return Instruction::Mvn;
  case kw_movt:
    return Instruction::Movt;
  case kw_movw:
    return Instruction::Movw;
  case kw_cmp:
    return Instruction::Cmp;
  case kw_cmn:
    return Instruction::Cmn;
  case kw_tst:
    return Instruction::Tst;
  case kw_teq:
    return Instruction::Teq;
  case kw_bfc:
    return Instruction::Bfc;
  case kw_bfi:
    return Instruction::Bfi;
  case kw_sbfx:
    return Instruction::Sbfx;
  case kw_ubfx:
    return Instruction::Ubfx;
  case kw_rbit:
    return Instruction::Rbit;
  case kw_rev:
    return Instruction::Rev;
  case kw_rev16:
    return Instruction::Rev16;
  case kw_revsh:
    return Instruction::Revsh;
  case kw_b:
    return Instruction::B;
  case kw_bl:
    return Instruction::Bl;
  case kw_bx:
    return Instruction::Bx;
  case kw_cbz:
    return Instruction::Cbz;
  case kw_cbnz:
    return Instruction::Cbnz;
  case kw_ldr:
    return Instruction::Ldr;
  case kw_ldrb:
    return Instruction::Ldrb;
  case kw_ldrsb:
    return Instruction::Ldrsb;
  case kw_ldrh:
    return Instruction::Ldrh;
  case kw_ldrsh:
    return Instruction::Ldrsh;
  case kw_str:
    return Instruction::Str;
  case kw_strb:
    return Instruction::Strb;
  case kw_strh:
    return Instruction::Strh;
  case kw_ldm:
    return Instruction::Ldm;
  case kw_ldmia:
    return Instruction::Ldmia;
  case kw_ldmib:
    return Instruction::Ldmib;
  case kw_ldmda:
    return Instruction::Ldmda;
  case kw_ldmdb:
    return Instruction::Ldmdb;
  case kw_stm:
    return Instruction::Stm;
  case kw_stmia:
    return Instruction::Stmia;
  case kw_stmib:
    return Instruction::Stmib;
  case kw_stmda:
    return Instruction::Stmda;
  case kw_stmdb:
    return Instruction::Stmdb;
  case kw_push:
    return Instruction::Push;
  case kw_pop:
    return Instruction::Pop;

  default:
    return 0;
  }
}

std::unique_ptr<ir::Instruction> Parser::parse_instruction() {
  const auto tok = lexer_.token_kind();

  if (!token::is_instruction(tok)) {
    return {};
  }

  if (tok != token::kw_nop) {
    const auto operation = map_token(tok);
    if (operation > 0) {
      if (Instruction::is_arithmetic_instruction(operation)) {
        return parse_arithmetic(
            static_cast<Instruction::ArithmeticOperation>(operation));
      } else if (Instruction::is_shift_instruction(operation)) {
        return parse_shift(static_cast<Instruction::ShiftOperation>(operation));
      } else if (Instruction::is_multiply_instruction(operation)) {
        return parse_multiply(
            static_cast<Instruction::MultiplyOperation>(operation));
      } else if (Instruction::is_divide_instruction(operation)) {
        return parse_divide(
            static_cast<Instruction::DivideOperation>(operation));
      } else if (Instruction::is_move_instruction(operation)) {
        return parse_move(static_cast<Instruction::MoveOperation>(operation));
      } else if (Instruction::is_comparison_instruction(operation)) {
        return parse_comparison(
            static_cast<Instruction::ComparisonOperation>(operation));
      } else if (Instruction::is_bitfield_instruction(operation)) {
        return parse_bitfield(
            static_cast<Instruction::BitfieldOperation>(operation));
      } else if (Instruction::is_reverse_instruction(operation)) {
        return parse_reverse(
            static_cast<Instruction::ReverseOperation>(operation));
      } else if (Instruction::is_branch_instruction(operation)) {
        return parse_branch(
            static_cast<Instruction::BranchOperation>(operation));
      } else if (Instruction::is_single_memory_instruction(operation)) {
        return parse_single_memory(
            static_cast<Instruction::SingleMemoryOperation>(operation));
      } else if (Instruction::is_block_memory_instruction(operation)) {
        return parse_block_memory(
            static_cast<Instruction::BlockMemoryOperation>(operation));
      } else {
        return {};
      }
    }
  }

  // return mov r0, r0
  return {};
}

std::optional<Operand2> Parser::parse_operand2() {
  auto src2 = Operand2{};

  if (lexer_.token_kind() == token::Numbersym) {
    lexer_.get_token();
    const auto negate = lexer_.token_kind() == token::Minus;
    if (negate) {
      lexer_.get_token();
    }

    src2.value = lexer_.int_value();
    if (!ensure(token::Integer, "expected integer"sv)) {
      return std::nullopt;
    }

    if (negate) {
      // we dont really care that the value is negative (and thus actually
      // signed) its all up to interpretation in the end
      src2.value = static_cast<unsigned>(-std::get<unsigned>(src2.value));
    }
  } else {
    const auto rm = static_cast<Register::Kind>(map_token(lexer_.token_kind()));
    if (!ensure(token::is_register, "expected register or immediate value"sv)) {
      return std::nullopt;
    }

    if (lexer_.token_kind() != token::Comma) {
      src2.value = ShiftedRegister{rm, Instruction::Lsl, 0u};
      return src2;
    }

    const auto op = map_token(lexer_.token_kind());
    if (!ensure(token::is_instruction, "expected shift operation"sv) ||
        !Instruction::is_shift_instruction(op)) {
      return std::nullopt;
    }
    const auto sh = static_cast<Instruction::ShiftOperation>(op);

    if (lexer_.token_kind() == token::Numbersym) {
      lexer_.get_token();
      const auto shamt5 = lexer_.int_value();
      if (!ensure(token::Integer, "expected positive integer shift"sv)) {
        return std::nullopt;
      }
      src2.value = ShiftedRegister{rm, sh, shamt5};
    } else {
      const auto rs = map_token(lexer_.token_kind());
      if (!ensure(token::is_register,
                  "expected register or immediate shift value"sv)) {
        return std::nullopt;
      }
      src2.value = ShiftedRegister{rm, sh, static_cast<Register::Kind>(rs)};
    }
  }

  return src2;
}

std::unique_ptr<ArithmeticInstruction>
Parser::parse_arithmetic(Instruction::ArithmeticOperation op) {
  lexer_.get_token();
  const auto update = lexer_.token_kind() == token::UpdateFlag;
  if (update) {
    lexer_.get_token();
  }

  auto cond = condition::AL;
  if (token::is_condition(lexer_.token_kind())) {
    cond = static_cast<condition::Kind>(map_token(lexer_.token_kind()));
  }

  expect(token::is_register, "expected register"sv);
  const auto rd = static_cast<Register::Kind>(map_token(lexer_.token_kind()));
  expect(token::Comma, "expected comma"sv);
  expect(token::is_register, "expected register"sv);
  const auto rn = static_cast<Register::Kind>(map_token(lexer_.token_kind()));
  expect(token::Comma, "expected comma"sv);
  const auto src2 = parse_operand2();
  if (!src2) {
    return {};
  }

  return std::make_unique<ArithmeticInstruction>(op, cond, update, rd, rn,
                                                 src2.value());
}
