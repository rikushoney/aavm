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
};

struct Operand2 {
  union {
    unsigned imm12{};
    ShiftedRegister rm;
  };
  bool immediate{};

  explicit constexpr Operand2(unsigned imm12) : imm12{imm12}, immediate{true} {}

  explicit constexpr Operand2(ShiftedRegister rm) : rm{rm}, immediate{false} {}
};

} // namespace aavm::ir

#endif
