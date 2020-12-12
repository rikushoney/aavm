#ifndef AAVM_OPERAND2_H
#define AAVM_OPERAND2_H

#include "token.h"

#include <assert.h>
#include <cstdint>
#include <type_traits>
#include <variant>

namespace aavm::parser {

using shift_variant = std::variant<std::int32_t, token::Kind>;

struct ShiftedRegister {
  token::Kind rm;
  token::Kind op;
  shift_variant amount;
};

struct Operand2 {
  using int_type = std::int32_t;

  using operand_variant = std::variant<int_type, ShiftedRegister>;

  operand_variant operand;

  static auto immediate(int_type imm) {
    auto operand = Operand2{};
    operand.operand = imm;
    return operand;
  }

  template <typename Shift>
  static auto shifted_register(token::Kind rm, token::Kind shift_type,
                               Shift shift) {
    static_assert(std::is_convertible_v<Shift, shift_variant>);
    assert(token::is_register(rm));
    assert(token::is_shift_instruction(shift_type));
    if constexpr (std::is_same_v<Shift, token::Kind>) {
      assert(token::is_register(shift));
    }

    auto operand = Operand2{};
    auto regstr = ShiftedRegister{};
    regstr.rm = rm;
    regstr.op = shift_type;
    regstr.amount = shift;
    operand.operand = regstr;
    return operand;
  }

  static auto shifted_register(ShiftedRegister regstr) {
    auto operand = Operand2{};
    operand.operand = regstr;
    return operand;
  }
};

} // namespace aavm::parser

#endif
