#ifndef AAVM_IR_INSTRUCTIONS_H_
#define AAVM_IR_INSTRUCTIONS_H_

#include "instruction.h"
#include "label.h"
#include "operand2.h"
#include "register.h"
#include <utility>
#include <vector>

namespace aavm::ir {

struct ArithmeticInstruction : public Instruction {
  Register::Kind rd{};
  Register::Kind rn{};
  Operand2 src2{};

  constexpr ArithmeticInstruction(ArithmeticOperation op, condition::Kind cond,
                                  bool update, Register::Kind rd,
                                  Register::Kind rn, Operand2 src2)
      : Instruction{op, cond, update}, rd{rd}, rn{rn}, src2{src2} {}
};

struct MultiplyInstruction : public Instruction {
  std::variant<Register::Kind, std::pair<Register::Kind, Register::Kind>> rd{};
  Register::Kind rm{};
  Register::Kind rs{};
  Register::Kind rn{};

  constexpr MultiplyInstruction(MultiplyOperation op, condition::Kind cond,
                                bool update, Register::Kind rd,
                                Register::Kind rm, Register::Kind rs,
                                Register::Kind rn)
      : Instruction{op, cond, update}, rd{rd}, rm{rm}, rs{rs}, rn{rn} {}

  constexpr MultiplyInstruction(MultiplyOperation op, condition::Kind cond,
                                bool update,
                                std::pair<Register::Kind, Register::Kind> rd,
                                Register::Kind rm, Register::Kind rs)
      : Instruction{op, cond, update}, rd{rd}, rm{rm}, rs{rs} {}
};

struct DivideInstruction : public Instruction {
  Register::Kind rd{};
  Register::Kind rn{};
  Register::Kind rm{};

  constexpr DivideInstruction(DivideOperation op, condition::Kind cond,
                              Register::Kind rd, Register::Kind rn,
                              Register::Kind rm)
      : Instruction{op, cond, false}, rd{rd}, rn{rn}, rm{rm} {}
};

struct MoveInstruction : public Instruction {
  Register::Kind rd{};
  std::variant<Operand2, unsigned, const Label *> value{};

  constexpr MoveInstruction(MoveOperation op, condition::Kind cond, bool update,
                            Register::Kind rd, Operand2 src2)
      : Instruction{op, cond, update}, rd{rd}, value{src2} {}

  constexpr MoveInstruction(MoveOperation op, condition::Kind cond,
                            Register::Kind rd, unsigned imm16)
      : Instruction{op, cond, false}, rd{rd}, value{imm16} {}

  constexpr MoveInstruction(MoveOperation op, condition::Kind cond,
                            Register::Kind rd, const Label *label)
      : Instruction{op, cond, false}, rd{rd}, value{label} {}
};

struct ComparisonInstruction : public Instruction {
  Register::Kind rd{};
  Register::Kind rn{};
  Operand2 src2{};

  constexpr ComparisonInstruction(ComparisonOperation op, condition::Kind cond,
                                  Register::Kind rd, Register::Kind rn,
                                  Operand2 src2)
      : Instruction{op, cond, true}, rd{rd}, rn{rn}, src2{src2} {}
};

struct BitfieldInstruction : public Instruction {
  Register::Kind rd{};
  Register::Kind rn{};
  unsigned lsb{};
  unsigned width{};

  constexpr BitfieldInstruction(BitfieldOperation op, condition::Kind cond,
                                Register::Kind rd, Register::Kind rn,
                                unsigned lsb, unsigned width)
      : Instruction{op, cond, false}, rd{rd}, rn{rn}, lsb{lsb}, width{width} {}

  constexpr BitfieldInstruction(BitfieldOperation op, condition::Kind cond,
                                Register::Kind rd, unsigned lsb, unsigned width)
      : Instruction{op, cond, false}, rd{rd}, lsb{lsb}, width{width} {}
};

struct ReverseInstruction : public Instruction {
  Register::Kind rd{};
  Register::Kind rm{};

  constexpr ReverseInstruction(ReverseOperation op, condition::Kind cond,
                               Register::Kind rd, Register::Kind rm)
      : Instruction{op, cond, false}, rd{rd}, rm{rm} {}
};

struct BranchInstruction : public Instruction {
  const Label *label{};
  Register::Kind rm{};
  Register::Kind rn{};

  constexpr BranchInstruction(BranchOperation op, condition::Kind cond,
                              const Label *label)
      : Instruction{op, cond, false}, label{label} {}

  constexpr BranchInstruction(BranchOperation op, condition::Kind cond,
                              Register::Kind rm)
      : Instruction{op, cond, false}, rm{rm} {}

  constexpr BranchInstruction(BranchOperation op, condition::Kind cond,
                              Register::Kind rn, const Label *label)
      : Instruction{op, cond, false}, label{label}, rn{rn} {}
};

struct SingleMemoryInstruction : public Instruction {
  Register::Kind rd{};
  enum IndexMode { PostIndex, Offset, PreIndex } index_mode{};
  bool subtract{};
  std::variant<Register::Kind, const Label *, unsigned> source{};
  Operand2 src2{};

  constexpr SingleMemoryInstruction(SingleMemoryOperation op,
                                    condition::Kind cond, Register::Kind rd,
                                    Register::Kind rn, Operand2 src2,
                                    IndexMode mode, bool subtract)
      : Instruction{op, cond, false}, rd{rd},
        index_mode{mode}, subtract{subtract}, source{rn}, src2{src2} {}

  constexpr SingleMemoryInstruction(SingleMemoryOperation op,
                                    condition::Kind cond, Register::Kind rd,
                                    const Label *label)
      : Instruction{op, cond, false}, rd{rd}, source{label} {}

  constexpr SingleMemoryInstruction(SingleMemoryOperation op,
                                    condition::Kind cond, Register::Kind rd,
                                    unsigned imm32)
      : Instruction{op, cond, false}, rd{rd}, source{imm32} {}
};

struct BlockMemoryInstruction : public Instruction {
  Register::Kind rn{};
  bool writeback{};
  std::vector<Register::Kind> register_list{};

  /* constexpr */ BlockMemoryInstruction(BlockMemoryOperation op,
                                         condition::Kind cond,
                                         Register::Kind rn, bool writeback,
                                         std::vector<Register::Kind> &registers)
      : Instruction{op, cond, false}, rn{rn}, writeback{writeback},
        register_list{registers} {}

  /* constexpr */ BlockMemoryInstruction(BlockMemoryOperation op,
                                         condition::Kind cond,
                                         std::vector<Register::Kind> &registers)
      : Instruction{op, cond, false}, rn{Register::SP}, writeback{true},
        register_list{registers} {}
};

} // namespace aavm::ir

#endif
