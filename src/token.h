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
  kw_hs,
  kw_cc,
  kw_lo,
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

  kw_add,
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

  kw_asr,
  kw_lsl,
  kw_lsr,
  kw_ror,
  kw_rrx,

  kw_mul,
  kw_mla,
  kw_mls,
  kw_umull,
  kw_umlal,
  kw_smull,
  kw_smlal,

  kw_sdiv,
  kw_udiv,

  kw_mov,
  kw_mvn,
  kw_movt,
  kw_movw,

  kw_cmp,
  kw_cmn,
  kw_tst,
  kw_teq,

  kw_bfc,
  kw_bfi,
  kw_sbfx,
  kw_ubfx,

  kw_rbit,
  kw_rev,
  kw_rev16,
  kw_revsh,

  kw_b,
  kw_bl,
  kw_bx,
  kw_cbz,
  kw_cbnz,

  kw_ldr,
  kw_ldrb,
  kw_ldrsb,
  kw_ldrh,
  kw_ldrsh,
  kw_str,
  kw_strb,
  kw_strh,

  kw_ldm,
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
  instructions_end_ = kw_pop
};

constexpr auto is_condition(token::Kind token) {
  return token >= conditions_start_ && token <= conditions_end_;
}

constexpr auto is_register(token::Kind token) {
  return token >= registers_start_ && token <= registers_end_;
}

constexpr auto is_instruction(token::Kind token) {
  return token >= instructions_start_ && token <= instructions_end_;
}

} // namespace aavm::token

#endif
