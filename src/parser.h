#ifndef AAVM_PARSER_PARSER_H_
#define AAVM_PARSER_PARSER_H_

#include "instruction.h"
#include "instructions.h"
#include "lexer.h"
#include "operand2.h"
#include "register.h"
#include "textbuffer.h"
#include <memory>
#include <optional>
#include <string_view>

namespace aavm::parser {

class Parser {
public:
  Parser(Charbuffer &source) : source_{source}, lexer_{source_} {}

  // TODO: make this private and make the parser return a "module" instead with
  // all parsed instructions and directives etc.
  std::unique_ptr<ir::Instruction> parse_instruction();

private:
  template <typename Pred>
  constexpr auto ensure(Pred &&pred, std::string_view message) {
    if (!pred(lexer_.token_kind())) {
      // assertion failed
      static_cast<void>(message); // emit some message
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
      // assertion failed
      static_cast<void>(message); // emit some message
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

  bool parse_update_flag();
  ir::condition::Kind parse_condition();
  std::optional<unsigned> parse_immediate();
  std::optional<ir::Register::Kind> parse_register();
  std::optional<ir::Operand2> parse_operand2();

  std::unique_ptr<ir::ArithmeticInstruction>
  parse_arithmetic(ir::Instruction::ArithmeticOperation op);
  std::unique_ptr<ir::MoveInstruction>
  parse_shift(ir::Instruction::ShiftOperation op);
  std::unique_ptr<ir::MultiplyInstruction>
  parse_multiply(ir::Instruction::MultiplyOperation op);
  std::unique_ptr<ir::DivideInstruction>
  parse_divide(ir::Instruction::DivideOperation op);
  std::unique_ptr<ir::MoveInstruction>
  parse_move(ir::Instruction::MoveOperation op);
  std::unique_ptr<ir::ComparisonInstruction>
  parse_comparison(ir::Instruction::ComparisonOperation op);
  std::unique_ptr<ir::BitfieldInstruction>
  parse_bitfield(ir::Instruction::BitfieldOperation op);
  std::unique_ptr<ir::ReverseInstruction>
  parse_reverse(ir::Instruction::ReverseOperation op);
  std::unique_ptr<ir::BranchInstruction>
  parse_branch(ir::Instruction::BranchOperation op);
  std::unique_ptr<ir::SingleMemoryInstruction>
  parse_single_memory(ir::Instruction::SingleMemoryOperation op);
  std::unique_ptr<ir::BlockMemoryInstruction>
  parse_block_memory(ir::Instruction::BlockMemoryOperation op);

  Charbuffer &source_;
  Lexer lexer_;
};

} // namespace aavm::parser

#endif
