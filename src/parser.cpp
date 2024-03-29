#include "parser.h"
#include "compiler.h"
#include "instructions.h"
#include "operand2.h"
#include "register.h"
#include "token.h"
#include <memory>

using namespace aavm;
using namespace aavm::parser;
using namespace aavm::ir;
using namespace std::string_view_literals;

static constexpr auto map_token(token::Kind token) -> unsigned {
  using namespace aavm::token;

  switch (token) {
  case kw_eq:
    return Condition::EQ;
  case kw_ne:
    return Condition::NE;
  case kw_cs:
  case kw_hs:
    return Condition::CS;
  case kw_cc:
  case kw_lo:
    return Condition::CC;
  case kw_mi:
    return Condition::MI;
  case kw_pl:
    return Condition::PL;
  case kw_vs:
    return Condition::VS;
  case kw_vc:
    return Condition::VC;
  case kw_hi:
    return Condition::HI;
  case kw_ls:
    return Condition::LS;
  case kw_ge:
    return Condition::GE;
  case kw_lt:
    return Condition::LT;
  case kw_gt:
    return Condition::GT;
  case kw_le:
    return Condition::LE;
  case kw_al:
    return Condition::AL;
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

  aavm_unreachable();
}

std::unique_ptr<Instruction> Parser::parse_instruction() {
  const auto tok = lexer_.token_kind();

  if (!token::is_instruction(tok)) {
    return {};
  }

  if (tok != token::kw_nop) {
    const auto op = map_token(tok);
    if (op > 0) {
      if (Instruction::is_arithmetic_operation(op)) {
        return parse_arithmetic(
            static_cast<Instruction::ArithmeticOperation>(op),
            lexer_.source_location());
      } else if (Instruction::is_shift_operation(op)) {
        return parse_shift(static_cast<Instruction::ShiftOperation>(op),
                           lexer_.source_location());
      } else if (Instruction::is_multiply_operation(op)) {
        return parse_multiply(static_cast<Instruction::MultiplyOperation>(op),
                              lexer_.source_location());
      } else if (Instruction::is_divide_operation(op)) {
        return parse_divide(static_cast<Instruction::DivideOperation>(op),
                            lexer_.source_location());
      } else if (Instruction::is_move_operation(op)) {
        return parse_move(static_cast<Instruction::MoveOperation>(op),
                          lexer_.source_location());
      } else if (Instruction::is_comparison_operation(op)) {
        return parse_comparison(
            static_cast<Instruction::ComparisonOperation>(op),
            lexer_.source_location());
      } else if (Instruction::is_bitfield_operation(op)) {
        return parse_bitfield(static_cast<Instruction::BitfieldOperation>(op),
                              lexer_.source_location());
      } else if (Instruction::is_reverse_operation(op)) {
        return parse_reverse(static_cast<Instruction::ReverseOperation>(op),
                             lexer_.source_location());
      } else if (Instruction::is_branch_operation(op)) {
        return parse_branch(static_cast<Instruction::BranchOperation>(op),
                            lexer_.source_location());
      } else if (Instruction::is_single_memory_operation(op)) {
        return parse_single_memory(
            static_cast<Instruction::SingleMemoryOperation>(op),
            lexer_.source_location());
      } else if (Instruction::is_block_memory_operation(op)) {
        return parse_block_memory(
            static_cast<Instruction::BlockMemoryOperation>(op),
            lexer_.source_location());
      } else {
        return {};
      }
    }
  }

  // return mov r0, r0
  return std::make_unique<MoveInstruction>(
      Instruction::Mov, Condition::AL, false, Register::Kind::R0,
      Operand2{ShiftedRegister{Register::Kind::R0, Instruction::Lsl, 0u}});
}

const Label *Parser::find_label_or_insert(std::string_view name) {
  for (const auto &l : labels_) {
    if (name == l.name()) {
      return &l;
    }
  }
  return &labels_.emplace_back(
      Label{static_cast<LabelID>(labels_.size() + 1), name});
}

bool Parser::parse_update_flag(const SourceLocation & /*srcloc*/) {
  const auto update = lexer_.token_kind() == token::UpdateFlag;
  if (update) {
    lexer_.get_token();
  }

  return update;
}

Condition::Kind Parser::parse_condition(const SourceLocation & /*srcloc*/) {
  if (is_condition(lexer_.token_kind())) {
    const auto cond = map_token(lexer_.token_kind());
    lexer_.get_token();
    return static_cast<Condition::Kind>(cond);
  }

  return Condition::AL;
}

std::optional<unsigned>
Parser::parse_immediate(bool numbersym, const SourceLocation & /*srcloc*/) {
  if (numbersym && !ensure(token::Numbersym, "expected '#'"sv)) {
    return std::nullopt;
  }

  const auto negate = lexer_.token_kind() == token::Minus;
  if (negate) {
    lexer_.get_token();
  }

  const auto imm = lexer_.int_value();
  if (!ensure(token::Integer, "expected integer"sv)) {
    return std::nullopt;
  }

  // we want to compute the two's compliment of the number but keep the value
  // unsigned since it's up to interpretation at an assembly level anyway
#if AAVM_MSVC
#pragma warning(push)
#pragma warning(disable : 4146)
#endif
  return negate ? static_cast<unsigned>(-imm) : imm;
#if AAVM_MSVC
#pragma warning(pop)
#endif
}

std::optional<Register::Kind>
Parser::parse_register(const SourceLocation & /*srcloc*/) {
  const auto reg = map_token(lexer_.token_kind());
  if (!ensure(token::is_register, "expected register"sv)) {
    return std::nullopt;
  }

  return static_cast<Register::Kind>(reg);
}

std::optional<Operand2>
Parser::parse_operand2(const SourceLocation & /*srcloc*/) {
  if (lexer_.token_kind() == token::Numbersym) {
    const auto imm =
        parse_immediate(/*numbersym*/ true, lexer_.source_location());
    return imm ? std::optional{Operand2{*imm}} : std::nullopt;
  }

  const auto rm = parse_register(lexer_.source_location());
  if (!rm) {
    return std::nullopt;
  }

  if (lexer_.token_kind() != token::Comma) {
    return Operand2{ShiftedRegister{*rm, Instruction::Lsl, 0}};
  }

  if (!expect(token::is_instruction, "expected shift operation"sv)) {
    return std::nullopt;
  }

  const auto sh = map_token(lexer_.token_kind());
  if (!Instruction::is_shift_operation(sh)) {
    return std::nullopt;
  }

  lexer_.get_token();
  if (lexer_.token_kind() == token::Numbersym) {
    const auto shamt5 =
        parse_immediate(/*numbersym*/ true, lexer_.source_location());
    return shamt5 ? std::optional{Operand2{ShiftedRegister{
                        *rm, static_cast<Instruction::ShiftOperation>(sh),
                        *shamt5}}}
                  : std::nullopt;
  } else {
    const auto rs = parse_register(lexer_.source_location());
    return rs ? std::optional{Operand2{ShiftedRegister{
                    *rm, static_cast<Instruction::ShiftOperation>(sh), *rs}}}
              : std::nullopt;
  }
}

std::optional<const Label *>
Parser::parse_label(const SourceLocation & /*srcloc*/) {
  const auto label = lexer_.string_value();
  if (!ensure(token::Label, "expected label"sv)) {
    return std::nullopt;
  }
  return find_label_or_insert(label);
}

std::unique_ptr<ArithmeticInstruction>
Parser::parse_arithmetic(Instruction::ArithmeticOperation op,
                         const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto updates = parse_update_flag(lexer_.source_location());
  const auto cond = parse_condition(lexer_.source_location());

  const auto rd = parse_register(lexer_.source_location());
  if (!rd || !ensure_comma()) {
    return nullptr;
  }
  // special case for ADR
  if (op == Instruction::Adr) {
    const auto label = parse_label(lexer_.source_location());
    return label
               ? std::make_unique<ArithmeticInstruction>(op, cond, *rd, *label)
               : nullptr;
  }
  const auto rn = parse_register(lexer_.source_location());
  if (!rn) {
    return nullptr;
  }
  if (!ensure_comma()) {
    return nullptr;
  }

  const auto src2 = parse_operand2(lexer_.source_location());
  return src2 ? std::make_unique<ArithmeticInstruction>(op, cond, updates, *rd,
                                                        *rn, *src2)
              : nullptr;
}

std::unique_ptr<MoveInstruction>
Parser::parse_shift(Instruction::ShiftOperation op,
                    const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto updates = parse_update_flag(lexer_.source_location());
  const auto cond = parse_condition(lexer_.source_location());

  const auto rd = parse_register(lexer_.source_location());
  if (!rd || !ensure_comma()) {
    return nullptr;
  }
  const auto rm = parse_register(lexer_.source_location());
  if (!rm) {
    return nullptr;
  }
  if (op == Instruction::Rrx) {
    return std::make_unique<MoveInstruction>(
        Instruction::Mov, cond, updates, *rd,
        Operand2{ShiftedRegister{*rm, op, 0}});
  }

  if (!ensure_comma()) {
    return nullptr;
  }

  if (lexer_.token_kind() == token::Numbersym) {
    const auto shamt5 =
        parse_immediate(/*numbersym*/ true, lexer_.source_location());
    return shamt5 ? std::make_unique<MoveInstruction>(
                        Instruction::Mov, cond, updates, *rd,
                        Operand2{ShiftedRegister{*rm, op, *shamt5}})
                  : nullptr;
  }

  const auto rs = parse_register(lexer_.source_location());
  return rs ? std::make_unique<MoveInstruction>(
                  Instruction::Mov, cond, updates, *rd,
                  Operand2{ShiftedRegister{*rm, op, *rs}})
            : nullptr;
}

std::unique_ptr<MultiplyInstruction>
Parser::parse_multiply(Instruction::MultiplyOperation op,
                       const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto updates = parse_update_flag(lexer_.source_location());
  const auto cond = parse_condition(lexer_.source_location());

  switch (op) {
  case Instruction::Mul:
  case Instruction::Mla:
  case Instruction::Mls: {
    const auto rd = parse_register(lexer_.source_location());
    if (!rd || !ensure_comma()) {
      break;
    }
    const auto rm = parse_register(lexer_.source_location());
    if (!rm || !ensure_comma()) {
      break;
    }
    const auto rs = parse_register(lexer_.source_location());

    if (op == Instruction::Mul) {
      return rs ? std::make_unique<MultiplyInstruction>(op, cond, updates, *rd,
                                                        *rm, *rs)
                : nullptr;
    }

    if (!rs || !ensure_comma()) {
      break;
    }
    const auto rn = parse_register(lexer_.source_location());
    return rn ? std::make_unique<MultiplyInstruction>(op, cond, updates, *rs,
                                                      *rm, *rs, *rn)
              : nullptr;
  }
  case Instruction::Umull:
  case Instruction::Smull:
  case Instruction::Umlal:
  case Instruction::Smlal: {
    const auto rdlo = parse_register(lexer_.source_location());
    if (!rdlo || !ensure_comma()) {
      break;
    }
    const auto rdhi = parse_register(lexer_.source_location());
    if (!rdhi || !ensure_comma()) {
      break;
    }
    const auto rm = parse_register(lexer_.source_location());
    if (!rm || !ensure_comma()) {
      break;
    }
    const auto rs = parse_register(lexer_.source_location());

    return rs ? std::make_unique<MultiplyInstruction>(
                    op, cond, updates, std::pair{*rdlo, *rdhi}, *rm, *rs)
              : nullptr;
  }
  default:
    break;
  }

  return nullptr;
}

std::unique_ptr<DivideInstruction>
Parser::parse_divide(Instruction::DivideOperation op,
                     const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto cond = parse_condition(lexer_.source_location());

  const auto rd = parse_register(lexer_.source_location());
  if (!rd || !ensure_comma()) {
    return nullptr;
  }
  const auto rn = parse_register(lexer_.source_location());
  if (!rn || !ensure_comma()) {
    return nullptr;
  }
  const auto rm = parse_register(lexer_.source_location());

  return rm ? std::make_unique<DivideInstruction>(op, cond, *rd, *rn, *rm)
            : nullptr;
}

std::unique_ptr<MoveInstruction>
Parser::parse_move(Instruction::MoveOperation op,
                   const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  switch (op) {
  case Instruction::Mov:
  case Instruction::Mvn: {
    const auto updates = parse_update_flag(lexer_.source_location());
    const auto cond = parse_condition(lexer_.source_location());

    const auto rd = parse_register(lexer_.source_location());
    if (!rd || !ensure_comma()) {
      break;
    }
    const auto src2 = parse_operand2(lexer_.source_location());
    return src2 ? std::make_unique<MoveInstruction>(op, cond, updates, *rd,
                                                    *src2)
                : nullptr;
  }
  case Instruction::Movt:
  case Instruction::Movw: {
    const auto cond = parse_condition(lexer_.source_location());

    const auto rd = parse_register(lexer_.source_location());
    if (!rd || !ensure_comma()) {
      break;
    }
    const auto imm16 =
        parse_immediate(/*numbersym*/ true, lexer_.source_location());
    return imm16 ? std::make_unique<MoveInstruction>(op, cond, *rd, *imm16)
                 : nullptr;
  }
  default:
    break;
  }

  return nullptr;
}

std::unique_ptr<ComparisonInstruction>
Parser::parse_comparison(Instruction::ComparisonOperation op,
                         const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto cond = parse_condition(lexer_.source_location());

  const auto rn = parse_register(lexer_.source_location());
  if (!rn || !ensure_comma()) {
    return nullptr;
  }
  const auto src2 = parse_operand2(lexer_.source_location());
  return src2 ? std::make_unique<ComparisonInstruction>(op, cond, *rn, *src2)
              : nullptr;
}

std::unique_ptr<BitfieldInstruction>
Parser::parse_bitfield(Instruction::BitfieldOperation op,
                       const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto cond = parse_condition(lexer_.source_location());

  const auto rd = parse_register(lexer_.source_location());
  if (!rd || !ensure_comma()) {
    return nullptr;
  }

  if (op == Instruction::Bfc) {
    const auto lsb =
        parse_immediate(/*numbersym*/ true, lexer_.source_location());
    if (!lsb || !ensure_comma()) {
      return nullptr;
    }
    const auto width =
        parse_immediate(/*numbersym*/ true, lexer_.source_location());
    return width ? std::make_unique<BitfieldInstruction>(op, cond, *rd, *lsb,
                                                         *width)
                 : nullptr;
  }

  const auto rn = parse_register(lexer_.source_location());
  if (!rn || !ensure_comma()) {
    return nullptr;
  }
  const auto lsb =
      parse_immediate(/*numbersym*/ true, lexer_.source_location());
  if (!lsb || !ensure_comma()) {
    return nullptr;
  }
  const auto width =
      parse_immediate(/*numbersym*/ true, lexer_.source_location());
  return width ? std::make_unique<BitfieldInstruction>(op, cond, *rd, *rn, *lsb,
                                                       *width)
               : nullptr;
}

std::unique_ptr<ReverseInstruction>
Parser::parse_reverse(Instruction::ReverseOperation op,
                      const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto cond = parse_condition(lexer_.source_location());

  const auto rd = parse_register(lexer_.source_location());
  if (!rd || !ensure_comma()) {
    return nullptr;
  }
  const auto rm = parse_register(lexer_.source_location());
  return rm ? std::make_unique<ReverseInstruction>(op, cond, *rd, *rm)
            : nullptr;
}

std::unique_ptr<BranchInstruction>
Parser::parse_branch(Instruction::BranchOperation op,
                     const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto cond = parse_condition(lexer_.source_location());

  switch (op) {
  case Instruction::B:
  case Instruction::Bl: {
    const auto label = parse_label(lexer_.source_location());
    return label ? std::make_unique<BranchInstruction>(op, cond, *label)
                 : nullptr;
  }
  case Instruction::Bx: {
    const auto rm = parse_register(lexer_.source_location());
    return rm ? std::make_unique<BranchInstruction>(op, cond, *rm) : nullptr;
  }
  case Instruction::Cbz:
  case Instruction::Cbnz: {
    const auto rn = parse_register(lexer_.source_location());
    if (!rn || !ensure_comma()) {
      break;
    }
    const auto label = parse_label(lexer_.source_location());
    return label ? std::make_unique<BranchInstruction>(op, cond, *label)
                 : nullptr;
  }
  default:
    break;
  }

  return nullptr;
}

std::unique_ptr<SingleMemoryInstruction>
Parser::parse_single_memory(Instruction::SingleMemoryOperation op,
                            const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto cond = parse_condition(lexer_.source_location());

  const auto rd = parse_register(lexer_.source_location());
  if (!rd || !ensure_comma()) {
    return nullptr;
  }

  if (lexer_.token_kind() == token::Equal) {
    lexer_.get_token();
    const auto imm32 =
        parse_immediate(/* numbersym */ false, lexer_.source_location());
    return imm32 ? std::make_unique<SingleMemoryInstruction>(op, cond, *rd,
                                                             *imm32)
                 : nullptr;
  } else if (lexer_.token_kind() == token::Label) {
    const auto label = parse_label(lexer_.source_location());
    return label ? std::make_unique<SingleMemoryInstruction>(op, cond, *rd,
                                                             *label)
                 : nullptr;
  }

  if (!ensure(token::Lbracket, "expected opening bracket"sv)) {
    return nullptr;
  }
  const auto rn = parse_register(lexer_.source_location());
  if (!rn) {
    return nullptr;
  }

  if (lexer_.token_kind() == token::Rbracket) {
    lexer_.get_token();
    if (lexer_.token_kind() != token::Comma) {
      return std::make_unique<SingleMemoryInstruction>(
          op, cond, *rd, *rn, Operand2{0u},
          SingleMemoryInstruction::IndexMode::PostIndex, false);
    }
    lexer_.get_token();
    const auto subtract = lexer_.token_kind() == token::Minus;
    if (subtract) {
      lexer_.get_token();
    }
    const auto src2 = parse_operand2(lexer_.source_location());
    return src2 ? std::make_unique<SingleMemoryInstruction>(
                      op, cond, *rd, *rn, *src2,
                      SingleMemoryInstruction::IndexMode::PostIndex, subtract)
                : nullptr;
  }

  if (!ensure_comma()) {
    return nullptr;
  }
  const auto subtract = lexer_.token_kind() == token::Minus;
  if (subtract) {
    lexer_.get_token();
  }
  const auto src2 = parse_operand2(lexer_.source_location());
  if (!src2 || !ensure(token::Rbracket, "expected closing bracket"sv)) {
    return nullptr;
  }
  const auto indexmode = lexer_.token_kind() == token::Exclaim
                             ? SingleMemoryInstruction::IndexMode::PreIndex
                             : SingleMemoryInstruction::IndexMode::Offset;
  return std::make_unique<SingleMemoryInstruction>(op, cond, *rd, *rn, *src2,
                                                   indexmode, subtract);
}

std::unique_ptr<BlockMemoryInstruction>
Parser::parse_block_memory(Instruction::BlockMemoryOperation op,
                           const SourceLocation & /*srcloc*/) {
  lexer_.get_token();

  const auto cond = parse_condition(lexer_.source_location());

  auto rn = std::optional<Register::Kind>{};
  auto writeback = false;
  switch (op) {
  case Instruction::Ldm:
  case Instruction::Ldmia:
  case Instruction::Ldmib:
  case Instruction::Ldmda:
  case Instruction::Ldmdb:
  case Instruction::Stm:
  case Instruction::Stmia:
  case Instruction::Stmib:
  case Instruction::Stmda:
  case Instruction::Stmdb:
    rn = parse_register(lexer_.source_location());
    if (!rn) {
      return nullptr;
    }
    writeback = lexer_.token_kind() == token::Exclaim;
    if (writeback) {
      lexer_.get_token();
    }
    if (!ensure_comma()) {
      return nullptr;
    }
    break;
  case Instruction::Push:
  case Instruction::Pop:
    rn = Register::SP;
    writeback = true;
    break;
  }

  if (!ensure(token::Lbrace, "expected opening brace"sv)) {
    return nullptr;
  }

  auto registers = std::vector<Register::Kind>{};
  while (lexer_.token_kind() != token::Rbrace) {
    const auto reg = parse_register(lexer_.source_location());
    if (!reg) {
      return nullptr;
    }
    if (lexer_.token_kind() == token::Minus) {
      lexer_.get_token();
      const auto reg2 = parse_register(lexer_.source_location());
      if (!reg2) {
        return nullptr;
      }
      const auto last = static_cast<unsigned>(*reg2);
      for (auto i = static_cast<unsigned>(*reg); i <= last; ++i) {
        registers.push_back(static_cast<Register::Kind>(i));
      }
    } else {
      registers.push_back(*reg);
    }
    if (lexer_.token_kind() != token::Rbrace && !ensure_comma()) {
      return nullptr;
    }
  }

  return std::make_unique<BlockMemoryInstruction>(op, cond, *rn, writeback,
                                                  registers);
}
