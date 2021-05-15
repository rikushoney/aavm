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
  Parser(Lexer &lexer) : lexer_{lexer} {
    // prime the first token
    lexer_.get_token();
  }

  // TODO: make this private and make the parser return a "module" instead with
  // all parsed instructions and directives etc.
  std::unique_ptr<ir::Instruction> parse_instruction();

private:
  template <typename Pred>
  constexpr auto ensure(Pred &&pred, std::string_view message) {
    if (!pred(lexer_.token_kind())) {
      fmt::print("{}\n", message);
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
      fmt::print("{}\n", message);
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

  const ir::Label *find_label_or_insert(std::string_view name);

  bool parse_update_flag(const SourceLocation &srcloc);
  ir::Condition::Kind parse_condition(const SourceLocation &srcloc);
  std::optional<unsigned> parse_immediate(bool numbersym,
                                          const SourceLocation &srcloc);
  std::optional<ir::Register::Kind>
  parse_register(const SourceLocation &srcloc);
  std::optional<ir::Operand2> parse_operand2(const SourceLocation &srcloc);
  std::optional<const ir::Label *> parse_label(const SourceLocation &srcloc);

protected:
  std::unique_ptr<ir::ArithmeticInstruction>
  parse_arithmetic(ir::Instruction::ArithmeticOperation op,
                   const SourceLocation &srcloc);

  std::unique_ptr<ir::MoveInstruction>
  parse_shift(ir::Instruction::ShiftOperation op, const SourceLocation &srcloc);

  std::unique_ptr<ir::MultiplyInstruction>
  parse_multiply(ir::Instruction::MultiplyOperation op,
                 const SourceLocation &srcloc);

  std::unique_ptr<ir::DivideInstruction>
  parse_divide(ir::Instruction::DivideOperation op,
               const SourceLocation &srcloc);

  std::unique_ptr<ir::MoveInstruction>
  parse_move(ir::Instruction::MoveOperation op, const SourceLocation &srcloc);

  std::unique_ptr<ir::ComparisonInstruction>
  parse_comparison(ir::Instruction::ComparisonOperation op,
                   const SourceLocation &srcloc);

  std::unique_ptr<ir::BitfieldInstruction>
  parse_bitfield(ir::Instruction::BitfieldOperation op,
                 const SourceLocation &srcloc);

  std::unique_ptr<ir::ReverseInstruction>
  parse_reverse(ir::Instruction::ReverseOperation op,
                const SourceLocation &srcloc);

  std::unique_ptr<ir::BranchInstruction>
  parse_branch(ir::Instruction::BranchOperation op,
               const SourceLocation &srcloc);

  std::unique_ptr<ir::SingleMemoryInstruction>
  parse_single_memory(ir::Instruction::SingleMemoryOperation op,
                      const SourceLocation &srcloc);

  std::unique_ptr<ir::BlockMemoryInstruction>
  parse_block_memory(ir::Instruction::BlockMemoryOperation op,
                     const SourceLocation &srcloc);

private:
  Lexer &lexer_;
  std::vector<ir::Label> labels_{};
};

} // namespace aavm::parser

namespace aavm {
using Parser = parser::Parser;
}

#endif
