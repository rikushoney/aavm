#ifndef AAVM_INSTRUCTION_ENCODER_H
#define AAVM_INSTRUCTION_ENCODER_H

#include "instruction.h"

#include <cstdint>

namespace aavm {

class InstructionEncoder {
public:
  using opcode_type = std::int32_t;
  opcode_type encode(const ir::Instruction &instruction) const;

private:
  opcode_type encode_arithmetic_instruction(
      const ir::ArithmeticInstruction *instruction) const;
  opcode_type
  encode_multiply_instruction(const ir::MultiplyInstruction *instruction) const;
  opcode_type
  encode_divide_instruction(const ir::DivideInstruction *instruction) const;
  opcode_type
  encode_move_instruction(const ir::MoveInstruction *instruction) const;
  opcode_type encode_comparison_instruction(
      const ir::CompareInstruction *instruction) const;
  opcode_type
  encode_bitfield_instruction(const ir::BitfieldInstruction *instruction) const;
  opcode_type
  encode_reverse_instruction(const ir::ReverseInstruction *instruction) const;
  opcode_type
  encode_branch_instruction(const ir::BranchInstruction *instruction) const;
  opcode_type encode_single_memory_instruction(
      const ir::SingleMemoryInstruction *instruction) const;
  opcode_type encode_multiply_memory_instruction(
      const ir::MultipleMemoryInstruction *instruction) const;
};

} // namespace aavm

#endif
