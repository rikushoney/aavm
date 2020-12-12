#ifndef AAVM_PARSER_H
#define AAVM_PARSER_H

#include "instruction.h"
#include "lexer.h"
#include "operand2.h"
#include "textbuffer.h"
#include "token.h"

#include <assert.h>
#include <memory>
#include <string_view>
#include <type_traits>
#include <vector>

namespace aavm::parser {

class Parser {
public:
  Parser(const Charbuffer &buffer);

  // TODO: make this private
  std::unique_ptr<Instruction> parse_instruction();

private:
  template <typename Pred> void expect(Pred &&p, std::string_view message) {
    if (!p(lexer_.get_token())) {
      assert(false);
    }
  }

  void expect(token::Kind kind, std::string_view message) {
    return expect([kind](const auto given) { return given == kind; }, message);
  }

  template <typename Pred> void ensure(Pred &&p, std::string_view message) {
    if (!p(lexer_.token_kind())) {
      assert(false);
    }

    lexer_.get_token();
  }

  void ensure(token::Kind kind, std::string_view message) {
    return ensure([kind](const auto given) { return given == kind; }, message);
  }

  std::vector<token::Kind> parse_register_list(int count);
  // TODO: add proper return types and parameters
  Operand2 parse_operand2();
  void parse_arithmetic_instruction(std::unique_ptr<Instruction> &instruction);
  void parse_multiply_instruction(std::unique_ptr<Instruction> &instruction);
  void parse_divide_instruction(std::unique_ptr<Instruction> &instruction);
  void parse_move_instruction(std::unique_ptr<Instruction> &instruction);
  void parse_comparison_instruction(std::unique_ptr<Instruction> &instruction);
  void parse_bitfield_instruction(std::unique_ptr<Instruction> &instruction);
  void parse_reverse_instruction(std::unique_ptr<Instruction> &instruction);
  void parse_branch_instruction(std::unique_ptr<Instruction> &instruction);
  void
  parse_single_memory_instruction(std::unique_ptr<Instruction> &instruction);
  void
  parse_multiple_memory_instruction(std::unique_ptr<Instruction> &instruction);

  Lexer lexer_;
};

} // namespace aavm::parser

#endif
