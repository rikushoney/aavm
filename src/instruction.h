#ifndef AAVM_IR_INSTRUCTION_H_
#define AAVM_IR_INSTRUCTION_H_

#include "condition.h"
#include "token.h"

namespace aavm::ir {

class Instruction {
public:
  constexpr Instruction(unsigned operation, condition::Kind condition,
                        bool updatesflags)
      : op_{operation}, condition_{condition}, updates_{updatesflags} {}

  auto operation() const { return op_; }

  auto condition() const { return condition_; }

  auto updatesflags() const { return updates_; }

  enum ArithmeticOperations {
    Add = token::kw_add,
    Adc = token::kw_adc,
    Sub = token::kw_sub,
    Sbc = token::kw_sbc,
    Rsb = token::kw_rsb,
    Rsc = token::kw_rsc,
    And = token::kw_and,
    Eor = token::kw_eor,
    Orr = token::kw_orr,
    Bic = token::kw_bic,
    Adr = token::kw_adr
  };

  enum ShiftOperations {
    Asr = token::kw_asr,
    Lsl = token::kw_lsl,
    Lsr = token::kw_lsr,
    Ror = token::kw_ror,
    Rrx = token::kw_rrx
  };

  enum MultiplyOperations {
    Mul = token::kw_mul,
    Mla = token::kw_mla,
    Mls = token::kw_mls,
    Umull = token::kw_umull,
    Umlal = token::kw_umlal,
    Smull = token::kw_smull,
    Smlal = token::kw_smlal
  };

  enum DivideOperations { Sdiv = token::kw_sdiv, Udiv = token::kw_udiv };

  enum MoveOperations {
    Mov = token::kw_mov,
    Mvn = token::kw_mvn,
    Movt = token::kw_movt,
    Movw = token::kw_movw
  };

  enum ComparisonOperations {
    Cmp = token::kw_cmp,
    Cmn = token::kw_cmn,
    Tst = token::kw_tst,
    Teq = token::kw_teq
  };

  enum BitfieldOperations {
    Bfc = token::kw_bfc,
    Bfi = token::kw_bfi,
    Sbfx = token::kw_sbfx,
    Ubfx = token::kw_ubfx
  };

  enum ReverseOperations {
    Rbit = token::kw_rbit,
    Rev = token::kw_rev,
    Rev16 = token::kw_rev16,
    Revsh = token::kw_revsh
  };

  enum BranchOperations {
    B = token::kw_b,
    Bl = token::kw_bl,
    Bx = token::kw_bx,
    Cbz = token::kw_cbz,
    Cbnz = token::kw_cbnz
  };

  enum SingleMemoryOperations {
    Ldr = token::kw_ldr,
    Ldrb = token::kw_ldrb,
    Ldrsb = token::kw_ldrsb,
    Ldrh = token::kw_ldrh,
    Ldrsh = token::kw_ldrsh,
    Str = token::kw_str,
    Strb = token::kw_strb,
    Strh = token::kw_strh
  };

  enum BlockMemoryOperations {
    Ldm = token::kw_ldm,
    Ldmia = token::kw_ldmia,
    Ldmib = token::kw_ldmib,
    Ldmda = token::kw_ldmda,
    Ldmdb = token::kw_ldmdb,
    Stm = token::kw_stm,
    Stmia = token::kw_stmia,
    Stmib = token::kw_stmib,
    Stmda = token::kw_stmda,
    Stmdb = token::kw_stmdb,
    Push = token::kw_push,
    Pop = token::kw_pop
  };

  constexpr auto is_arithmetic_instruction(unsigned kind) {
    return kind >= token::Kind::arithmetic_instructions_start_ &&
           kind <= token::Kind::arithmetic_instructions_end_;
  }

  constexpr auto is_shift_instruction(unsigned kind) {
    return kind >= token::Kind::shift_instructions_start_ &&
           kind <= token::Kind::shift_instructions_end_;
  }

  constexpr auto is_multiply_instruction(unsigned kind) {
    return kind >= token::Kind::multiply_instructions_start_ &&
           kind <= token::Kind::multiply_instructions_end_;
  }

  constexpr auto is_divide_instruction(unsigned kind) {
    return kind >= token::Kind::divide_instructions_start_ &&
           kind <= token::Kind::divide_instructions_end_;
  }

  constexpr auto is_move_instruction(unsigned kind) {
    return kind >= token::Kind::move_instructions_start_ &&
           kind <= token::Kind::move_instructions_end_;
  }

  constexpr auto is_comparison_instruction(unsigned kind) {
    return kind >= token::Kind::comparison_instructions_start_ &&
           kind <= token::Kind::comparison_instructions_end_;
  }

  constexpr auto is_bitfield_instruction(unsigned kind) {
    return kind >= token::Kind::bitfield_instructions_start_ &&
           kind <= token::Kind::bitfield_instructions_end_;
  }

  constexpr auto is_reverse_instruction(unsigned kind) {
    return kind >= token::Kind::reverse_instructions_start_ &&
           kind <= token::Kind::reverse_instructions_end_;
  }

  constexpr auto is_branch_instruction(unsigned kind) {
    return kind >= token::Kind::branch_instructions_start_ &&
           kind <= token::Kind::branch_instructions_end_;
  }

  constexpr auto is_single_memory_instruction(unsigned kind) {
    return kind >= token::Kind::single_memory_instructions_start_ &&
           kind <= token::Kind::single_memory_instructions_end_;
  }

  constexpr auto is_block_memory_instruction(unsigned kind) {
    return kind >= token::Kind::block_memory_instructions_start_ &&
           kind <= token::Kind::block_memory_instructions_end_;
  }

private:
  unsigned op_;
  condition::Kind condition_;
  bool updates_;
};

} // namespace aavm::ir

#endif
