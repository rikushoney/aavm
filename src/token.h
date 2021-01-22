#ifndef AAVM_TOKEN_H_
#define AAVM_TOKEN_H_

namespace aavm::token {

enum Kind {
  Error = 0,
  Eof,

  Newline,
  Label,
  Integer,

  Comma,
  Exclaim,
  Equal,
  Numbersym,
  Lbracket,
  Rbracket,
  Lbrace,
  Rbrace,
  Minus,
  Colon,
  Period,

  UpdateFlag,

  conditions_start_,
  kw_eq = conditions_start_,
  kw_ne,
  kw_cs,
  kw_cc,
  kw_mi,
  kw_pl,
  kw_vs,
  kw_vc,
  kw_hi,
  kw_ls,
  kw_ge,
  kw_lt,
  kw_gt,
  kw_le,
  kw_al,
  conditions_end_ = kw_al,

  registers_start_,
  kw_r0 = registers_start_,
  kw_r1,
  kw_r2,
  kw_r3,
  kw_r4,
  kw_r5,
  kw_r6,
  kw_r7,
  kw_r8,
  kw_r9,
  kw_r10,
  kw_r11,
  kw_r12,
  kw_r13,
  kw_r14,
  kw_r15,
  kw_sp,
  kw_lr,
  kw_pc,
  registers_end_ = kw_pc,

  instructions_start_,
  kw_nop = instructions_start_,

  arithmetic_instructions_start_,
  kw_add = arithmetic_instructions_start_,
  kw_adc,
  kw_sub,
  kw_sbc,
  kw_rsb,
  kw_rsc,
  kw_and,
  kw_eor,
  kw_orr,
  kw_bic,
  kw_adr,
  arithmetic_instructions_end_ = kw_adr,

  shift_instructions_start_,
  kw_asr = shift_instructions_start_,
  kw_lsl,
  kw_lsr,
  kw_ror,
  kw_rrx,
  shift_instructions_end_ = kw_rrx,

  multiply_instructions_start_,
  kw_mul = multiply_instructions_start_,
  kw_mla,
  kw_mls,
  kw_umull,
  kw_umlal,
  kw_smull,
  kw_smlal,
  multiply_instructions_end_ = kw_smlal,

  divide_instructions_start_,
  kw_sdiv = divide_instructions_start_,
  kw_udiv,
  divide_instructions_end_ = kw_udiv,

  move_instructions_start_,
  kw_mov = move_instructions_start_,
  kw_mvn,
  kw_movt,
  kw_movw,
  move_instructions_end_ = kw_movw,

  comparison_instructions_start_,
  kw_cmp,
  kw_cmn,
  kw_tst,
  kw_teq,
  comparison_instructions_end_ = kw_teq,

  bitfield_instructions_start_,
  kw_bfc = bitfield_instructions_start_,
  kw_bfi,
  kw_sbfx,
  kw_ubfx,
  bitfield_instructions_end_ = kw_ubfx,

  reverse_instructions_start_,
  kw_rbit = reverse_instructions_start_,
  kw_rev,
  kw_rev16,
  kw_revsh,
  reverse_instructions_end_ = kw_revsh,

  branch_instructions_start_,
  kw_b = branch_instructions_start_,
  kw_bl,
  kw_bx,
  kw_cbz,
  kw_cbnz,
  branch_instructions_end_ = kw_cbnz,

  single_memory_instructions_start_,
  kw_ldr = single_memory_instructions_start_,
  kw_ldrb,
  kw_ldrsb,
  kw_ldrh,
  kw_ldrsh,
  kw_str,
  kw_strb,
  kw_strh,
  single_memory_instructions_end_ = kw_strh,

  block_memory_instructions_start_,
  kw_ldm = block_memory_instructions_start_,
  kw_ldmia,
  kw_ldmib,
  kw_ldmda,
  kw_ldmdb,
  kw_stm,
  kw_stmia,
  kw_stmib,
  kw_stmda,
  kw_stmdb,
  kw_push,
  kw_pop,
  block_memory_instructions_end_ = kw_pop,

  instructions_end_ = block_memory_instructions_end_
};

constexpr auto is_condition(Kind kind) {
  return kind >= Kind::conditions_start_ && kind <= Kind::conditions_end_;
}

constexpr auto is_register(Kind kind) {
  return kind >= Kind::registers_start_ && kind <= Kind::registers_end_;
}

constexpr auto is_instruction(Kind kind) {
  return kind >= Kind::instructions_start_ && kind <= Kind::instructions_end_;
}

} // namespace aavm::token

#endif
