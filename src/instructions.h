#ifndef AAVM_IR_INSTRUCTIONS_H_
#define AAVM_IR_INSTRUCTIONS_H_

#include "instruction.h"

namespace aavm::ir {

struct ArithmeticInstruction : public Instruction {};

struct ShiftInstruction : public Instruction {};

struct MultiplyInstruction : public Instruction {};

struct DivideInstruction : public Instruction {};

struct MoveInstruction : public Instruction {};

struct ComparisonInstruction : public Instruction {};

struct BitfieldInstruction : public Instruction {};

struct ReverseInstruction : public Instruction {};

struct BranchInstruction : public Instruction {};

struct SingleMemoryInstruction : public Instruction {};

struct BlockMemoryInstruction : public Instruction {};

} // namespace aavm::ir

#endif
