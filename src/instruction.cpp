#include "instruction.h"

using namespace aavm;
using namespace aavm::ir;

Instruction::opcode_type ArithmeticInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type MultiplyInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type DivideInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type MoveInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type CompareInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type BitfieldInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type ReverseInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type BranchInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type SingleMemoryInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type MultipleMemoryInstruction::encode() const {
  return Instruction::encode();
}

Instruction::opcode_type Instruction::encode() const {
  // this is an undefined instruction that always executes
  return 0b1110'0110'0000'0000'0000'0000'0001'0000;
}

std::unique_ptr<Instruction> Instruction::decode(opcode_type opcode) {
  return {};
}
