#include "instruction_encoder.h"
#include "instruction.h"
#include "token.h"

using namespace aavm;
using namespace aavm::parser;

InstructionEncoder::opcode_type
InstructionEncoder::encode(const ir::Instruction &instruction) const {
  const auto op = instruction.opcode();

  // why does this look so ugly?
  if (token::is_arithmetic_instruction(op)) {
    return encode_arithmetic_instruction(
        static_cast<const ir::ArithmeticInstruction *>(&instruction));
  } else if (token::is_multiply_instruction(op)) {
    return encode_multiply_instruction(
        static_cast<const ir::MultiplyInstruction *>(&instruction));
  } else if (token::is_divide_instruction(op)) {
    return encode_divide_instruction(
        static_cast<const ir::DivideInstruction *>(&instruction));
  } else if (token::is_move_instruction(op) ||
             token::is_shift_instruction(op)) {
    return encode_move_instruction(
        static_cast<const ir::MoveInstruction *>(&instruction));
  } else if (token::is_comparison_instruction(op)) {
    return encode_comparison_instruction(
        static_cast<const ir::CompareInstruction *>(&instruction));
  } else if (token::is_bitfield_instruction(op)) {
    return encode_bitfield_instruction(
        static_cast<const ir::BitfieldInstruction *>(&instruction));
  } else if (token::is_reverse_instruction(op)) {
    return encode_reverse_instruction(
        static_cast<const ir::ReverseInstruction *>(&instruction));
  } else if (token::is_branch_instruction(op)) {
    return encode_branch_instruction(
        static_cast<const ir::BranchInstruction *>(&instruction));
  } else if (token::is_single_memory_instruction(op)) {
    return encode_single_memory_instruction(
        static_cast<const ir::SingleMemoryInstruction *>(&instruction));
  } else if (token::is_block_memory_instruction(op)) {
    return encode_multiply_memory_instruction(
        static_cast<const ir::MultipleMemoryInstruction *>(&instruction));
  } else {
    assert(false && "not an instruction");
  }

  return {};
}

InstructionEncoder::opcode_type
InstructionEncoder::encode_arithmetic_instruction(
    const ir::ArithmeticInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type InstructionEncoder::encode_multiply_instruction(
    const ir::MultiplyInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type InstructionEncoder::encode_divide_instruction(
    const ir::DivideInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type InstructionEncoder::encode_move_instruction(
    const ir::MoveInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type
InstructionEncoder::encode_comparison_instruction(
    const ir::CompareInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type InstructionEncoder::encode_bitfield_instruction(
    const ir::BitfieldInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type InstructionEncoder::encode_reverse_instruction(
    const ir::ReverseInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type InstructionEncoder::encode_branch_instruction(
    const ir::BranchInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type
InstructionEncoder::encode_single_memory_instruction(
    const ir::SingleMemoryInstruction *instruction) const {
  return {};
}

InstructionEncoder::opcode_type
InstructionEncoder::encode_multiply_memory_instruction(
    const ir::MultipleMemoryInstruction *instruction) const {
  return {};
}
