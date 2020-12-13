#include "keyword_map.h"
#include "operand2.h"
#include "parser.h"

#include <fstream>
#include <iostream>
#include <variant>

using namespace aavm;
using namespace aavm::parser;

auto dump_operand2(const Operand2 &operand) {
  std::cout << "Operand2:\n";
  if (std::holds_alternative<Operand2::int_type>(operand.operand)) {
    const auto imm = std::get<Operand2::int_type>(operand.operand);
    std::cout << "\tImmediate: " << imm << "\n";
  } else if (std::holds_alternative<ShiftedRegister>(operand.operand)) {
    const auto reg = std::get<ShiftedRegister>(operand.operand);
    std::cout << "\tRm: " << keyword::to_string(reg.rm).value() << "\n";
    std::cout << "\tOp: " << keyword::to_string(reg.op).value() << "\n";
    if (std::holds_alternative<std::int32_t>(reg.amount)) {
      const auto imm = std::get<std::int32_t>(reg.amount);
      std::cout << "\tImmediate: " << imm << "\n";
    } else if (std::holds_alternative<token::Kind>(reg.amount)) {
      const auto rs = std::get<token::Kind>(reg.amount);
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
  auto parsed = parser.parse_instruction();

  if (Instruction::is_arithmetic_instruction(parsed->opcode())) {
    const auto instr = static_cast<ArithmeticInstruction &>(*parsed.get());
    std::cout << "Parsed arithmetic instruction:\n";
    std::cout << "Opcode: " << keyword::to_string(parsed->opcode()).value()
              << "\n";
    std::cout << "Rd: " << keyword::to_string(instr.rd).value() << "\n";
    std::cout << "Rn: " << keyword::to_string(instr.rn).value() << "\n";
    dump_operand2(instr.operand2);
  }

  return 0;
}
