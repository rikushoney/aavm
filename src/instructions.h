#ifndef AAVM_IR_INSTRUCTIONS_H_
#define AAVM_IR_INSTRUCTIONS_H_

#include "instruction.h"
#include "label.h"
#include "operand2.h"
#include "register.h"
#include <vector>

namespace aavm::ir {

struct ArithmeticInstruction : public Instruction {
  Register::Kind rd;
  Register::Kind rn;
  Operand2 src2;
};

struct MultiplyInstruction : public Instruction {
  Register::Kind rd;
  Register::Kind rm;
  Register::Kind rs;
  Register::Kind rn;
  Register::Kind rdlo;
  Register::Kind rdhi;
};

struct DivideInstruction : public Instruction {
  Register::Kind rd;
  Register::Kind rn;
  Register::Kind rm;
};

struct MoveInstruction : public Instruction {
  Register::Kind rd;
  Operand2 src2;
  unsigned imm16;
};

struct ComparisonInstruction : public Instruction {
  Register::Kind rd;
  Register::Kind rn;
  Operand2 src2;
};

struct BitfieldInstruction : public Instruction {
  Register::Kind rd;
  Register::Kind rn;
  unsigned lsb;
  unsigned width;
};

struct ReverseInstruction : public Instruction {
  Register::Kind rd;
  Register::Kind rm;
};

struct BranchInstruction : public Instruction {
  Register::Kind rm;
  Register::Kind rn;
  LabelID label;
};

struct SingleMemoryInstruction : public Instruction {
  Register::Kind rd;
  Register::Kind rn;
  enum IndexMode { PostIndex, Offset, PreIndex } index_mode;
  bool subtract;
  Operand2 src2;
  LabelID label;
  unsigned imm32;
};

struct BlockMemoryInstruction : public Instruction {
  Register::Kind rn;
  bool writeback;
  std::vector<Register::Kind> register_list;
};

} // namespace aavm::ir

#endif
