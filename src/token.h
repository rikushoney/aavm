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

  // registers
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

  // instructions
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

  // directives
  DIR_dcb,
  DIR_equ,
  DIR_space,
  DIR_fill
};

constexpr auto is_condition(Kind tok) {
  switch (tok) {
  case COND_eq:
  case COND_ne:
  case COND_cs:
  case COND_cc:
  case COND_mi:
  case COND_pl:
  case COND_vs:
  case COND_vc:
  case COND_hi:
  case COND_ls:
  case COND_ge:
  case COND_lt:
  case COND_gt:
  case COND_le:
  case COND_al:
    return true;
  default:
    return false;
  }
}

constexpr auto is_register(Kind tok) {
  switch (tok) {
  case REG_r0:
  case REG_r1:
  case REG_r2:
  case REG_r3:
  case REG_r4:
  case REG_r5:
  case REG_r6:
  case REG_r7:
  case REG_r8:
  case REG_r9:
  case REG_r10:
  case REG_r11:
  case REG_r12:
  case REG_sp:
  case REG_lr:
  case REG_pc:
    return true;
  default:
    return false;
  }
}

constexpr auto is_arithmetic_instruction(Kind tok) {
  switch (tok) {
  case OP_add:
  case OP_adc:
  case OP_sub:
  case OP_sbc:
  case OP_rsb:
  case OP_rsc:
    return true;
  default:
    return false;
  }
}

constexpr auto is_multiply_instruction(Kind tok) {
  switch (tok) {
  case OP_mul:
  case OP_mla:
  case OP_mls:
  case OP_umull:
  case OP_umlal:
  case OP_smull:
  case OP_smlal:
    return true;
  default:
    return false;
  }
}

constexpr auto is_divide_instruction(Kind tok) {
  switch (tok) {
  case OP_sdiv:
  case OP_udiv:
    return true;
  default:
    return false;
  }
}

constexpr auto is_move_instruction(Kind tok) {
  switch (tok) {
  case OP_mov:
  case OP_mvn:
  case OP_movt:
  case OP_movw:
    return true;
  default:
    return false;
  }
}

constexpr auto is_shift_instruction(Kind tok) {
  switch (tok) {
  case OP_asr:
  case OP_lsl:
  case OP_lsr:
  case OP_ror:
  case OP_rrx:
    return true;
  default:
    return false;
  }
}

constexpr auto is_compare_instruction(Kind tok) {
  switch (tok) {
  case OP_cmp:
  case OP_cmn:
    return true;
  default:
    return false;
  }
}

constexpr auto is_logical_instruction(Kind tok) {
  switch (tok) {
  case OP_tst:
  case OP_teq:
  case OP_and:
  case OP_eor:
  case OP_orr:
  case OP_orn:
  case OP_bic:
    return true;
  default:
    return false;
  }
}

constexpr auto is_bitfield_instruction(Kind tok) {
  switch (tok) {
  case OP_bfc:
  case OP_bfi:
  case OP_sbfx:
  case OP_ubfx:
    return true;
  default:
    return false;
  }
}

constexpr auto is_reverse_instruction(Kind tok) {
  switch (tok) {
  case OP_rbit:
  case OP_rev:
  case OP_rev16:
  case OP_revsh:
    return true;
  default:
    return false;
  }
}

constexpr auto is_branch_instruction(Kind tok) {
  switch (tok) {
  case OP_b:
  case OP_bl:
  case OP_bx:
  case OP_cbz:
  case OP_cbnz:
    return true;
  default:
    return false;
  }
}

constexpr auto is_memory_instruction(Kind tok) {
  switch (tok) {
  case OP_ldr:
  case OP_ldrb:
  case OP_ldrh:
  case OP_ldrsb:
  case OP_ldrsh:
  case OP_str:
  case OP_strb:
  case OP_strh:
    return true;
  default:
    return false;
  }
}

constexpr auto is_multiple_memory_instruction(Kind tok) {
  switch (tok) {
  case OP_ldm:
  case OP_ldmia:
  case OP_ldmib:
  case OP_ldmda:
  case OP_ldmdb:
  case OP_stm:
  case OP_stmia:
  case OP_stmib:
  case OP_stmda:
  case OP_stmdb:
    return true;
  default:
    return false;
  }
}

constexpr auto is_stack_instruction(Kind tok) {
  switch (tok) {
  case OP_push:
  case OP_pop:
    return true;
  default:
    return false;
  }
}

constexpr auto is_instruction(Kind tok) {
  return is_arithmetic_instruction(tok) || is_multiply_instruction(tok) ||
         is_divide_instruction(tok) || is_move_instruction(tok) ||
         is_shift_instruction(tok) || is_compare_instruction(tok) ||
         is_logical_instruction(tok) || is_bitfield_instruction(tok) ||
         is_reverse_instruction(tok) || is_branch_instruction(tok) ||
         is_memory_instruction(tok) || is_multiple_memory_instruction(tok) ||
         is_stack_instruction(tok) || tok == OP_adr;
}

constexpr auto is_directive(Kind tok) {
  switch (tok) {
  case DIR_dcb:
  case DIR_equ:
  case DIR_space:
  case DIR_fill:
    return true;
  default:
    return false;
  }
}

} // namespace aavm::parser::token

#endif
