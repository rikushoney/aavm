#include "keyword_map.h"
#include "parser.h"

#include <fstream>
#include <iostream>
#include <variant>

using namespace aavm;

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "usage: " << argv[0] << "[filename]\n";
    return 1;
  }

  auto filestream = std::fstream{argv[1], std::ios_base::in};
  const auto buffer = Charbuffer{filestream};
  auto parser = parser::Parser{buffer};
  auto parsed = parser.parse_instruction();

  if (parser::Instruction::is_arithmetic_instruction(parsed->opcode())) {
    const auto instruction =
        static_cast<parser::ArithmeticInstruction *>(parsed.get());
    std::cout << "Parsed arithmetic instruction:\n";
    std::cout << "\tOpcode: "
              << parser::keyword::to_string(parsed->opcode()).value() << "\n";
    std::cout << "\tRd: " << parser::keyword::to_string(instruction->rd).value()
              << "\n";
    std::cout << "\tRn: " << parser::keyword::to_string(instruction->rn).value()
              << "\n";
    std::cout << "\tOperand2:\n";
    if (std::holds_alternative<parser::Operand2::int_type>(
            instruction->operand2.operand)) {
      const auto imm =
          std::get<parser::Operand2::int_type>(instruction->operand2.operand);
      std::cout << "\t\tImmediate: " << imm << "\n";
    } else if (std::holds_alternative<parser::ShiftedRegister>(
                   instruction->operand2.operand)) {
      const auto shifted_register =
          std::get<parser::ShiftedRegister>(instruction->operand2.operand);
      std::cout << "\t\tRm: "
                << parser::keyword::to_string(shifted_register.rm).value()
                << "\n";
      std::cout << "\t\tOp: "
                << parser::keyword::to_string(shifted_register.op).value()
                << "\n";
      if (std::holds_alternative<std::int32_t>(shifted_register.amount)) {
        std::cout << "\t\tImmediate: "
                  << std::get<std::int32_t>(shifted_register.amount) << "\n";
      } else if (std::holds_alternative<parser::token::Kind>(
                     shifted_register.amount)) {
        std::cout << "\t\tRegister: "
                  << parser::keyword::to_string(
                         std::get<parser::token::Kind>(shifted_register.amount))
                         .value()
                  << "\n";
      }
    }
  }

  return 0;
}
