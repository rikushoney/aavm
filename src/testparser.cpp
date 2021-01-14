#include "keyword_map.h"
#include "operand2.h"
#include "parser.h"

#include <fstream>
#include <iostream>
#include <variant>

using namespace aavm;
using namespace aavm::parser;

auto dump_operand2(const ir::Operand2 &operand) {
  std::cout << "Operand2:\n";
  if (std::holds_alternative<ir::Operand2::int_type>(operand.operand)) {
    const auto imm = std::get<ir::Operand2::int_type>(operand.operand);
    std::cout << "\tImmediate: " << imm << "\n";
  } else if (std::holds_alternative<ir::ShiftedRegister>(operand.operand)) {
    const auto reg = std::get<ir::ShiftedRegister>(operand.operand);
    std::cout << "\tRm: " << keyword::to_string(reg.rm).value() << "\n";
    std::cout << "\tOp: " << keyword::to_string(reg.op).value() << "\n";
    if (std::holds_alternative<std::int32_t>(reg.shift)) {
      const auto imm = std::get<std::int32_t>(reg.shift);
      std::cout << "\tImmediate: " << imm << "\n";
    } else if (std::holds_alternative<token::Kind>(reg.shift)) {
      const auto rs = std::get<token::Kind>(reg.shift);
      std::cout << "\tRegister: " << keyword::to_string(rs).value() << "\n";
    }
  }
}

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << "[filename]\n";
    return 1;
  }

  auto filestream = std::fstream{argv[1], std::ios_base::in};
  const auto buffer = Charbuffer{filestream};
  auto parser = Parser{buffer};
  const auto parsed = parser.parse_instructions();

  return 0;
}
