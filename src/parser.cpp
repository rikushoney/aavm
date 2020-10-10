#include "parser.h"

using namespace aavm::parser;

Parser::Parser(const Charbuffer &buffer) : lexer_{buffer} {
  lexer_.get_token();
}

void Parser::expect(token::Kind kind, std::string_view message) {
  const auto tok = lexer_.get_token();
  if (tok != kind) {
    // display message (and exit?)
  }
}

void Parser::parse_instruction() {
  const auto tok = lexer_.token_kind();
  if (tok < token::OP_START || tok > token::OP_END) {
    // error not an instruction
    return;
  }

  switch (tok) {
  case token::OP_add:
  case token::OP_adc:
  case token::OP_sub:
  case token::OP_sbc:
  case token::OP_rsb:
  case token::OP_rsc:
  case token::OP_asr:
  case token::OP_lsl:
  case token::OP_lsr:
  case token::OP_ror:
  case token::OP_rrx:
  case token::OP_and:
  case token::OP_eor:
  case token::OP_orr:
  case token::OP_bic:
    parse_arithmetic();
    break;
  case token::OP_mov:
  case token::OP_mvn:
  case token::OP_movt:
  case token::OP_movw:
    parse_mov();
    break;
  case token::OP_cmp:
  case token::OP_cmn:
  case token::OP_tst:
  case token::OP_teq:
    parse_compare_test();
    break;
  case token::OP_b:
  case token::OP_bl:
  case token::OP_bx:
  case token::OP_cbz:
  case token::OP_cbnz:
    parse_branch();
    break;
  case token::OP_ldr:
  case token::OP_ldrb:
  case token::OP_ldrh:
  case token::OP_ldrsb:
  case token::OP_ldrsh:
  case token::OP_str:
  case token::OP_strb:
  case token::OP_strh:
    parse_memory();
    break;
  case token::OP_ldm:
  case token::OP_ldmia:
  case token::OP_ldmib:
  case token::OP_ldmda:
  case token::OP_ldmdb:
  case token::OP_stm:
  case token::OP_stmia:
  case token::OP_stmib:
  case token::OP_stmda:
  case token::OP_stmdb:
    parse_multiple_memory();
    break;
  case token::OP_push:
  case token::OP_pop:
    parse_push_pop();
    break;
  case token::OP_rbit:
  case token::OP_rev:
  case token::OP_rev16:
  case token::OP_revsh:
    parse_reverse();
    break;
  case token::OP_bfc:
  case token::OP_bfi:
  case token::OP_sbfx:
  case token::OP_ubfx:
    parse_bitfield();
    break;
  case token::OP_mul:
  case token::OP_mla:
  case token::OP_mls:
  case token::OP_umull:
  case token::OP_umlal:
  case token::OP_smull:
  case token::OP_smlal:
    parse_multiply();
    break;
  case token::OP_sdiv:
  case token::OP_udiv:
    parse_divide();
  case token::OP_adr:
    parse_address();
    break;
  default:
    // unsupported instruction
    break;
  }
}

// <op>{S} Rd, Rn, <Operand2>
void Parser::parse_arithmetic() {}

// <op>{S} Rd, <Operand2>
// <op> Rd, #<imm16>
void Parser::parse_mov() {}

// <op> Rn, <Operand2>
void Parser::parse_compare_test() {}

// <op> <label>
// <op> Rm
// <op> Rn, <label>
void Parser::parse_branch() {}

// <op>{size} Rd, Rn
// <op>{size} Rd, <label>
// <op> Rd, =imm32
void Parser::parse_memory() {}

// <op>{IA|IB|DA|DB} Rn{!}, <reglist>
void Parser::parse_multiple_memory() {}

// <op> <reglist>
void Parser::parse_push_pop() {}

// <op> Rd, Rm
void Parser::parse_reverse() {}

// <op> Rd, #<lsb>, #<width>
// <op> Rd, Rn, #<lsb>, #<width>
void Parser::parse_bitfield() {}

// <op>{S} Rd, Rm, Rs
// <op>{S} RdLo, RdHi, Rm, Rs
void Parser::parse_multiply() {}

// <op> Rd, Rn, Rm
void Parser::parse_divide() {}

// <op> Rd, <label>
void Parser::parse_address() {}

void Parser::parse_operand2() {}
