#ifndef AAVM_OPERAND2_H
#define AAVM_OPERAND2_H

#include "keyword_map.h"
#include "token.h"

#include <assert.h>
#include <cstdint>
#include <type_traits>
#include <variant>

namespace aavm::ir {

using shift_variant = std::variant<std::int32_t, parser::token::Kind>;

struct ShiftedRegister {
  parser::token::Kind rm;
  parser::token::Kind op;
  shift_variant shift;
};

struct Operand2 {
  using int_type = std::int32_t;
  using operand_variant = std::variant<int_type, ShiftedRegister>;

  operand_variant operand;

  static auto immediate(int_type imm) {
    auto op2 = Operand2{};
    op2.operand = imm;
    return op2;
  }

  static auto shifted_register(ShiftedRegister regstr) {
    auto op2 = Operand2{};
    op2.operand = regstr;
    return op2;
  }

  template <typename Shift>
  static auto shifted_register(parser::token::Kind rm, parser::token::Kind op,
                               Shift shift) {
    static_assert(std::is_convertible_v<Shift, shift_variant>);
    assert(parser::token::is_register(rm));
    assert(parser::token::is_shift_instruction(op));
    if constexpr (std::is_same_v<Shift, parser::token::Kind>) {
      assert(parser::token::is_register(shift));
    }

    auto reg = ShiftedRegister{};
    reg.rm = rm;
    reg.op = op;
    reg.shift = shift;
    return shifted_register(reg);
  }
};

} // namespace aavm::ir

#endif
