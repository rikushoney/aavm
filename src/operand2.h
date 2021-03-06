#ifndef AAVM_IR_OPERAND2_H_
#define AAVM_IR_OPERAND2_H_

#include "instruction.h"
#include "register.h"
#include <variant>

namespace aavm::ir {

class ShiftedRegister {
public:
  explicit constexpr ShiftedRegister(Register::Kind rm,
                                     Instruction::ShiftOperation sh,
                                     unsigned shamt5)
      : rm_{rm}, sh_{sh}, shamt5_{shamt5}, immediate_{true} {}

  explicit constexpr ShiftedRegister(Register::Kind rm,
                                     Instruction::ShiftOperation sh,
                                     Register::Kind rs)
      : rm_{rm}, sh_{sh}, rs_{rs}, immediate_{false} {}

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

  constexpr auto rm() const { return rm_; }
  constexpr auto sh() const { return sh_; }
  constexpr auto shamt5() const { return shamt5_; }
  constexpr auto rs() const { return rs_; }
  constexpr auto immediate() const { return immediate_; }

private:
  Register::Kind rm_{};
  Instruction::ShiftOperation sh_{};
  union {
    unsigned shamt5_{};
    Register::Kind rs_;
  };
  bool immediate_{};
};

class Operand2 {
public:
  explicit constexpr Operand2(unsigned imm12)
      : imm12_{imm12}, immediate_{true} {}

  explicit constexpr Operand2(ShiftedRegister rm)
      : rm_{rm}, immediate_{false} {}

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

  constexpr auto imm12() const { return imm12_; }
  constexpr auto rm() const { return rm_; }
  constexpr auto immediate() const { return immediate_; }

private:
  union {
    unsigned imm12_{};
    ShiftedRegister rm_;
  };
  bool immediate_{};
};

} // namespace aavm::ir

#endif
