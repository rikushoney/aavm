#ifndef AAVM_IR_OPERAND2_H_
#define AAVM_IR_OPERAND2_H_

#include "instruction.h"
#include "register.h"
#include <variant>

namespace aavm::ir {

struct ShiftedRegister {
  Register::Kind rm{};
  Instruction::ShiftOperation sh{};
  union {
    unsigned shamt5{};
    Register::Kind rs;
  };
  bool immediate{};

  explicit constexpr ShiftedRegister(Register::Kind rm,
                                     Instruction::ShiftOperation sh,
                                     unsigned shamt5)
      : rm{rm}, sh{sh}, shamt5{shamt5}, immediate{true} {}

  explicit constexpr ShiftedRegister(Register::Kind rm,
                                     Instruction::ShiftOperation sh,
                                     Register::Kind rs)
      : rm{rm}, sh{sh}, rs{rs}, immediate{false} {}

  static constexpr auto immediate_shift(Register::Kind rm,
                                        Instruction::ShiftOperation sh,
                                        unsigned shamt5) {
    return ShiftedRegister{rm, sh, shamt5};
  }

  static constexpr auto register_shift(Register::Kind rm,
                                       Instruction::ShiftOperation sh,
                                       Register::Kind rs) {
    return ShiftedRegister{rm, sh, rs};
  }
};

struct Operand2 {
  union {
    unsigned imm12{};
    ShiftedRegister rm;
  };
  bool immediate{};

  explicit constexpr Operand2(unsigned imm12) : imm12{imm12}, immediate{true} {}

  explicit constexpr Operand2(ShiftedRegister rm) : rm{rm}, immediate{false} {}

  static constexpr auto immediate_value(unsigned imm12) {
    return Operand2{imm12};
  }

  static constexpr auto register_immediate_shift(Register::Kind rm,
                                                 Instruction::ShiftOperation sh,
                                                 unsigned shamt5) {
    return Operand2{ShiftedRegister{rm, sh, shamt5}};
  }

  static constexpr auto register_register_shift(Register::Kind rm,
                                                Instruction::ShiftOperation sh,
                                                Register::Kind rs) {
    return Operand2{ShiftedRegister{rm, sh, rs}};
  }
};

} // namespace aavm::ir

#endif
