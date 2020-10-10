#ifndef AAVM_TOKEN_H
#define AAVM_TOKEN_H

namespace aavm::parser::token {

enum Kind {
  Error,
  Eof,

  Newline, // \n
  Label,
  Integer,

  // punctuation
  Comma,     // ,
  Exclaim,   // !
  Equal,     // =
  Numbersym, // #
  Lbracket,  // [
  Rbracket,  // ]
  Lbrace,    // {
  Rbrace,    // }
  Minus,     // -
  Colon,     // :
  Period,    // .

  S, // optional suffix to update condition flags

  // condition codes
  COND_START,
  COND_eq,
  COND_ne,
  COND_cs,
  COND_cc,
  COND_mi,
  COND_pl,
  COND_vs,
  COND_vc,
  COND_hi,
  COND_ls,
  COND_ge,
  COND_lt,
  COND_gt,
  COND_le,
  COND_al,
  COND_END,

  // registers
  REG_START,
  REG_r0,
  REG_r1,
  REG_r2,
  REG_r3,
  REG_r4,
  REG_r5,
  REG_r6,
  REG_r7,
  REG_r8,
  REG_r9,
  REG_r10,
  REG_r11,
  REG_r12,
  REG_sp,
  REG_lr,
  REG_pc,
  REG_END,

  // instructions
  OP_START,
  // add
  OP_add,
  OP_adc,

  // subtract
  OP_sub,
  OP_sbc,
  OP_rsb,
  OP_rsc,

  // multiply
  OP_mul,
  OP_mla,
  OP_mls,
  OP_umull,
  OP_umlal,
  OP_smull,
  OP_smlal,

  // divide
  OP_sdiv,
  OP_udiv,

  // move
  OP_mov,
  OP_mvn,
  OP_movt,
  OP_movw,

  // shift
  OP_asr,
  OP_lsl,
  OP_lsr,
  OP_ror,
  OP_rrx,

  // compare
  OP_cmp,
  OP_cmn,

  // logical
  OP_tst,
  OP_teq,
  OP_and,
  OP_eor,
  OP_orr,
  OP_orn, // this seems to only be available in thumb mode
  OP_bic,

  // bit field
  OP_bfc,
  OP_bfi,
  OP_sbfx,
  OP_ubfx,

  // reverse
  OP_rbit,
  OP_rev,
  OP_rev16,
  OP_revsh,

  // branch
  OP_b,
  OP_bl,
  OP_bx,
  OP_cbz,
  OP_cbnz,

  // address
  OP_adr,

  // load and store
  OP_ldr,
  OP_ldrb,
  OP_ldrh,
  OP_ldrsb,
  OP_ldrsh,
  OP_str,
  OP_strb,
  OP_strh,
  OP_ldm,
  OP_ldmia,
  OP_ldmib,
  OP_ldmda,
  OP_ldmdb,
  OP_stm,
  OP_stmia,
  OP_stmib,
  OP_stmda,
  OP_stmdb,
  OP_push,
  OP_pop,
  OP_END,

  // directives
  DIR_START,
  DIR_dcb,
  DIR_equ,
  DIR_space,
  DIR_fill,
  DIR_END
};

}

#endif
