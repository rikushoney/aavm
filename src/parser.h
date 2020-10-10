#ifndef AAVM_PARSER_H
#define AAVM_PARSER_H

#include "lexer.h"
#include "textbuffer.h"
#include "token.h"

#include <string_view>

namespace aavm::parser {

class Parser {
public:
  Parser(const Charbuffer &buffer);

private:
  void expect(token::Kind kind, std::string_view message);

  void parse_instruction();
  void parse_arithmetic();
  void parse_mov();
  void parse_compare_test();
  void parse_branch();
  void parse_memory();
  void parse_multiple_memory();
  void parse_push_pop();
  void parse_reverse();
  void parse_bitfield();
  void parse_multiply();
  void parse_divide();
  void parse_address();
  void parse_operand2();

  Lexer lexer_;
};

} // namespace aavm::parser

#endif
