#ifndef AAVM_OPERAND2_H
#define AAVM_OPERAND2_H

#include "token.h"

#include <assert.h>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <variant>

namespace aavm::parser {

struct Operand2 {
  using int_type = std::int32_t;
  using operand_variant = std::variant<int_type, token::Kind>;
  using shift_variant = std::variant<int_type, token::Kind>;

  operand_variant operand;
  std::optional<token::Kind> shift_type;
  std::optional<shift_variant> shift;

  static Operand2 immediate(int_type imm) {
    auto operand = Operand2{};
    operand.operand = imm;
    return operand;
  }

  template <typename Shift>
  static Operand2 shifted_register(token::Kind rm, token::Kind shift_type,
                                   Shift shift) {
    static_assert(std::is_convertible_v<Shift, shift_variant>);
    assert(token::is_register(rm));
    assert(token::is_shift_instruction(shift_type));
    if constexpr (std::is_same_v<Shift, token::Kind>) {
      assert(token::is_register(shift));
    }

    auto operand = Operand2{};
    operand.operand = rm;
    operand.shift_type = shift_type;
    operand.shift = shift;
    return operand;
  }
};

} // namespace aavm::parser

#endif
