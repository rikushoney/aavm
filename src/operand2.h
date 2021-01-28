#ifndef AAVM_IR_OPERAND2_H_
#define AAVM_IR_OPERAND2_H_

#include "instruction.h"
#include "register.h"
#include <variant>

namespace aavm::ir {

struct ShiftedRegister {
  Register::Kind rm;
  Instruction::ShiftOperation sh;
  std::variant<unsigned, Register::Kind> shift_value;
};

struct Operand2 {
  std::variant<unsigned, ShiftedRegister> value;

  static constexpr auto immediate(unsigned imm) { return Operand2{imm}; }

  static constexpr auto
  shifted_register(Register::Kind rm,
                   Instruction::ShiftOperation sh = Instruction::Lsl,
                   unsigned shamt5 = 0) {
    return Operand2{ShiftedRegister{rm, sh, shamt5}};
  }

  static constexpr auto shifted_register(Register::Kind rm,
                                         Instruction::ShiftOperation sh,
                                         Register::Kind rs) {
    return Operand2{ShiftedRegister{rm, sh, rs}};
  }
};

} // namespace aavm::ir

#endif
