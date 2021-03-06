#ifndef AAVM_PARSER_PARSER_H_
#define AAVM_PARSER_PARSER_H_

#include "fmt/format.h"
#include "instruction.h"
#include "instructions.h"
#include "label.h"
#include "lexer.h"
#include "operand2.h"
#include "register.h"
#include "textbuffer.h"
#include <memory>
#include <optional>
#include <string_view>
#include <vector>

namespace aavm::parser {

class Parser {
public:
  Parser() = delete;
  Parser(const Charbuffer &source) : lexer_{source} {
    // prime the first token
    lexer_.get_token();
  }

  // TODO: make this private and make the parser return a "module" instead with
  // all parsed instructions and directives etc.
  std::unique_ptr<ir::InstructionVariant> parse_instruction();

private:
  template <typename Pred>
  constexpr auto ensure(Pred &&pred, std::string_view message) {
    if (!pred(lexer_.token_kind())) {
      fmt::print(message);
      return false;
    }

    lexer_.get_token();
    return true;
  }

  constexpr auto ensure(token::Kind token, std::string_view message) {
    return ensure([token](const auto tok) { return token == tok; }, message);
  }

  template <typename Pred>
  constexpr auto expect(Pred &&pred, std::string_view message) {
    lexer_.get_token();
    if (!pred(lexer_.token_kind())) {
      fmt::print(message);
      return false;
    }

    return true;
  }

  constexpr auto expect(token::Kind token, std::string_view message) {
    return expect([token](const auto tok) { return token == tok; }, message);
  }

  constexpr auto ensure_comma() {
    using namespace std::string_view_literals;
    return ensure(token::Comma, "expected comma"sv);
  }

  const ir::Label *find_label(std::string_view name);

  bool parse_update_flag();
  ir::Condition::Kind parse_condition();
  std::optional<unsigned> parse_immediate(bool numbersym = true);
  std::optional<ir::Register::Kind> parse_register();
  std::optional<ir::Operand2> parse_operand2();
  std::optional<const ir::Label *> parse_label();

protected:
  std::unique_ptr<ir::InstructionVariant>
  parse_arithmetic(ir::Instruction::ArithmeticOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_shift(ir::Instruction::ShiftOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_multiply(ir::Instruction::MultiplyOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_divide(ir::Instruction::DivideOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_move(ir::Instruction::MoveOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_comparison(ir::Instruction::ComparisonOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_bitfield(ir::Instruction::BitfieldOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_reverse(ir::Instruction::ReverseOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_branch(ir::Instruction::BranchOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_single_memory(ir::Instruction::SingleMemoryOperation op);
  std::unique_ptr<ir::InstructionVariant>
  parse_block_memory(ir::Instruction::BlockMemoryOperation op);

private:
  Lexer lexer_;
  std::vector<ir::Label> labels_{};
};

} // namespace aavm::parser

namespace aavm {
using Parser = parser::Parser;
}

#endif
