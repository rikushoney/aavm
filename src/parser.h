#ifndef AAVM_PARSER_H
#define AAVM_PARSER_H

#include "instruction.h"
#include "lexer.h"
#include "operand2.h"
#include "textbuffer.h"
#include "token.h"

#include <cstdint>
#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>

namespace aavm::parser {

class Parser {
public:
  using int_type = std::int32_t;

  Parser(const Charbuffer &buffer);

  std::vector<std::unique_ptr<ir::Instruction>> parse_instructions();

private:
  template <typename Pred> auto expect(Pred &&p, std::string_view message) {
    if (!p(lexer_.get_token())) {
      return false;
    }

    return true;
  }

  auto expect(token::Kind kind, std::string_view message) {
    return expect([kind](const auto given) { return given == kind; }, message);
  }

  template <typename Pred> auto ensure(Pred &&p, std::string_view message) {
    if (!p(lexer_.token_kind())) {
      return false;
    }

    lexer_.get_token();
    return true;
  }

  auto ensure(token::Kind kind, std::string_view message) {
    return ensure([kind](const auto given) { return given == kind; }, message);
  }

  std::unique_ptr<ir::Instruction> parse_instruction();
  int_type parse_immediate_value();
  void parse_shifted_register(ir::ShiftedRegister &reg);
  std::vector<token::Kind> parse_register_list(std::size_t count);
  ir::Operand2 parse_operand2();
  std::vector<token::Kind> parse_register_range();
  void
  parse_arithmetic_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void
  parse_multiply_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void parse_divide_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void parse_move_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void parse_shift_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void
  parse_comparison_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void
  parse_bitfield_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void parse_reverse_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void parse_branch_instruction(std::unique_ptr<ir::Instruction> &instruction);
  void parse_single_memory_instruction(
      std::unique_ptr<ir::Instruction> &instruction);
  void parse_multiple_memory_instruction(
      std::unique_ptr<ir::Instruction> &instruction);

  Lexer lexer_;
};

} // namespace aavm::parser

#endif
