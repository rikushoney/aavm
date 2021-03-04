#ifndef AAVM_IR_INSTRUCTIONS_H_
#define AAVM_IR_INSTRUCTIONS_H_

#include "compiler.h"
#include "fmt/format.h"
#include "instruction.h"
#include "label.h"
#include "operand2.h"
#include "register.h"
#include <type_traits>
#include <utility>
#include <vector>

namespace aavm::ir {

class ArithmeticInstruction : public Instruction {
public:
  constexpr ArithmeticInstruction(ArithmeticOperation op, condition::Kind cond,
                                  bool update, Register::Kind rd,
                                  Register::Kind rn, Operand2 src2)
      : Instruction{op, cond, update}, rd_{rd}, rn_{rn}, src2_{src2} {}

  constexpr ArithmeticInstruction(ArithmeticOperation op, condition::Kind cond,
                                  Register::Kind rd, const Label *label)
      : Instruction{op, cond, false}, rd_{rd}, label_{label} {}

  constexpr auto rd() const { return rd_; }
  constexpr auto rn() const { return rn_; }
  constexpr auto src2() const { return src2_; }
  constexpr auto label() const { return label_; }

private:
  Register::Kind rd_{};
  Register::Kind rn_{};
  union {
    Operand2 src2_;
    const Label *label_;
  };
};

class MultiplyInstruction : public Instruction {
public:
  constexpr MultiplyInstruction(MultiplyOperation op, condition::Kind cond,
                                bool update, Register::Kind rd,
                                Register::Kind rm, Register::Kind rs,
                                Register::Kind rn = Register::R0)
      : Instruction{op, cond, update}, rd_{rd}, rm_{rm}, rs_{rs}, rn_{rn} {}

  constexpr MultiplyInstruction(MultiplyOperation op, condition::Kind cond,
                                bool update,
                                std::pair<Register::Kind, Register::Kind> rd,
                                Register::Kind rm, Register::Kind rs)
      : Instruction{op, cond, update},
        registers_{rd.first, rd.second}, rm_{rm}, rs_{rs} {}

  constexpr auto rd() const { return rd_; }
  constexpr auto rdlo() const { return registers_.first; }
  constexpr auto rdhi() const { return registers_.second; }
  constexpr auto rm() const { return rm_; }
  constexpr auto rs() const { return rs_; }
  constexpr auto rn() const { return rn_; }

private:
  union {
    Register::Kind rd_;
    std::pair<Register::Kind, Register::Kind> registers_;
  };

  Register::Kind rm_{};
  Register::Kind rs_{};
  Register::Kind rn_{};
};

class DivideInstruction : public Instruction {
public:
  constexpr DivideInstruction(DivideOperation op, condition::Kind cond,
                              Register::Kind rd, Register::Kind rn,
                              Register::Kind rm)
      : Instruction{op, cond, false}, rd_{rd}, rn_{rn}, rm_{rm} {}

  constexpr auto rd() const { return rd_; }
  constexpr auto rn() const { return rn_; }
  constexpr auto rm() const { return rm_; }

private:
  Register::Kind rd_{};
  Register::Kind rn_{};
  Register::Kind rm_{};
};

class MoveInstruction : public Instruction {
public:
  constexpr MoveInstruction(MoveOperation op, condition::Kind cond, bool update,
                            Register::Kind rd, Operand2 src2)
      : Instruction{op, cond, update}, rd_{rd}, src2_{src2} {}

  constexpr MoveInstruction(MoveOperation op, condition::Kind cond,
                            Register::Kind rd, unsigned imm16)
      : Instruction{op, cond, false}, rd_{rd}, imm16_{imm16} {}

  constexpr auto rd() const { return rd_; }
  constexpr auto src2() const { return src2_; }
  constexpr auto imm16() const { return imm16_; }

private:
  Register::Kind rd_{};
  union {
    Operand2 src2_;
    unsigned imm16_;
  };
};

class ComparisonInstruction : public Instruction {
public:
  constexpr ComparisonInstruction(ComparisonOperation op, condition::Kind cond,
                                  Register::Kind rn, Operand2 src2)
      : Instruction{op, cond, true}, rn_{rn}, src2_{src2} {}

  constexpr auto rn() const { return rn_; }
  constexpr auto src2() const { return src2_; }

private:
  Register::Kind rn_{};
  Operand2 src2_;
};

class BitfieldInstruction : public Instruction {
public:
  constexpr BitfieldInstruction(BitfieldOperation op, condition::Kind cond,
                                Register::Kind rd, Register::Kind rn,
                                unsigned lsb, unsigned width)
      : Instruction{op, cond, false}, rd_{rd}, rn_{rn}, lsb_{lsb}, width_{
                                                                       width} {}

  constexpr BitfieldInstruction(BitfieldOperation op, condition::Kind cond,
                                Register::Kind rd, unsigned lsb, unsigned width)
      : Instruction{op, cond, false}, rd_{rd}, lsb_{lsb}, width_{width} {}

  constexpr auto rd() const { return rd_; }
  constexpr auto rn() const { return rn_; }
  constexpr auto lsb() const { return lsb_; }
  constexpr auto width() const { return width_; }

private:
  Register::Kind rd_{};
  Register::Kind rn_{};
  unsigned lsb_{};
  unsigned width_{};
};

class ReverseInstruction : public Instruction {
public:
  constexpr ReverseInstruction(ReverseOperation op, condition::Kind cond,
                               Register::Kind rd, Register::Kind rm)
      : Instruction{op, cond, false}, rd_{rd}, rm_{rm} {}

  constexpr auto rd() const { return rd_; }
  constexpr auto rm() const { return rm_; }

private:
  Register::Kind rd_{};
  Register::Kind rm_{};
};

class BranchInstruction : public Instruction {
public:
  constexpr BranchInstruction(BranchOperation op, condition::Kind cond,
                              const Label *label)
      : Instruction{op, cond, false}, label_{label} {}

  constexpr BranchInstruction(BranchOperation op, condition::Kind cond,
                              Register::Kind rm)
      : Instruction{op, cond, false}, rm_{rm} {}

  constexpr BranchInstruction(BranchOperation op, condition::Kind cond,
                              Register::Kind rn, const Label *label)
      : Instruction{op, cond, false}, label_{label}, rn_{rn} {}

  constexpr auto label() const { return label_; }
  constexpr auto rm() const { return rm_; }
  constexpr auto rn() const { return rn_; }

private:
  const Label *label_{};
  Register::Kind rm_{};
  Register::Kind rn_{};
};

class SingleMemoryInstruction : public Instruction {
public:
  enum class IndexMode { PostIndex, Offset, PreIndex };

  constexpr SingleMemoryInstruction(SingleMemoryOperation op,
                                    condition::Kind cond, Register::Kind rd,
                                    Register::Kind rn, Operand2 src2,
                                    IndexMode mode, bool subtract)
      : Instruction{op, cond, false}, rd_{rd}, rn_{rn}, source_{src2},
        indexmode_{mode}, subtract_{subtract} {}

  constexpr SingleMemoryInstruction(SingleMemoryOperation op,
                                    condition::Kind cond, Register::Kind rd,
                                    const Label *label)
      : Instruction{op, cond, false}, rd_{rd}, source_{label} {}

  constexpr SingleMemoryInstruction(SingleMemoryOperation op,
                                    condition::Kind cond, Register::Kind rd,
                                    unsigned imm32)
      : Instruction{op, cond, false}, rd_{rd}, source_{imm32} {}

  constexpr auto rd() const { return rd_; }
  constexpr auto rn() const { return rn_; }
  constexpr auto source() const { return source_; }
  constexpr auto indexmode() const { return indexmode_; }
  constexpr auto subtract() const { return subtract_; }

private:
  Register::Kind rd_{};
  Register::Kind rn_{};
  std::variant<Operand2, const Label *, unsigned> source_;
  IndexMode indexmode_{};
  bool subtract_{};
};

class BlockMemoryInstruction : public Instruction {
public:
  /* constexpr */ BlockMemoryInstruction(
      BlockMemoryOperation op, condition::Kind cond, Register::Kind rn,
      bool writeback, const std::vector<Register::Kind> &registers)
      : Instruction{op, cond, false}, rn_{rn}, writeback_{writeback},
        register_list_{registers} {}

  /* constexpr */ BlockMemoryInstruction(
      BlockMemoryOperation op, condition::Kind cond,
      const std::vector<Register::Kind> &registers)
      : Instruction{op, cond, false}, rn_{Register::SP}, writeback_{true},
        register_list_{registers} {}

  constexpr auto rn() const { return rn_; }
  constexpr auto writeback() const { return writeback_; }
  constexpr auto &register_list() const { return register_list_; }

private:
  Register::Kind rn_{};
  bool writeback_{};
  std::vector<Register::Kind> register_list_{};
};

using InstructionVariant =
    std::variant<Instruction, ArithmeticInstruction, MultiplyInstruction,
                 DivideInstruction, MoveInstruction, ComparisonInstruction,
                 BitfieldInstruction, ReverseInstruction, BranchInstruction,
                 SingleMemoryInstruction, BlockMemoryInstruction>;

constexpr auto as_arithmetic(const InstructionVariant &instruction) {
  return std::holds_alternative<ArithmeticInstruction>(instruction)
             ? &std::get<ArithmeticInstruction>(instruction)
             : nullptr;
}

constexpr auto as_multiply(const InstructionVariant &instruction) {
  return std::holds_alternative<MultiplyInstruction>(instruction)
             ? &std::get<MultiplyInstruction>(instruction)
             : nullptr;
}

constexpr auto as_divide(const InstructionVariant &instruction) {
  return std::holds_alternative<DivideInstruction>(instruction)
             ? &std::get<DivideInstruction>(instruction)
             : nullptr;
}

constexpr auto as_move(const InstructionVariant &instruction) {
  return std::holds_alternative<MoveInstruction>(instruction)
             ? &std::get<MoveInstruction>(instruction)
             : nullptr;
}

constexpr auto as_comparison(const InstructionVariant &instruction) {
  return std::holds_alternative<ComparisonInstruction>(instruction)
             ? &std::get<ComparisonInstruction>(instruction)
             : nullptr;
}

constexpr auto as_bitfield(const InstructionVariant &instruction) {
  return std::holds_alternative<BitfieldInstruction>(instruction)
             ? &std::get<BitfieldInstruction>(instruction)
             : nullptr;
}

constexpr auto as_reverse(const InstructionVariant &instruction) {
  return std::holds_alternative<ReverseInstruction>(instruction)
             ? &std::get<ReverseInstruction>(instruction)
             : nullptr;
}

constexpr auto as_branch(const InstructionVariant &instruction) {
  return std::holds_alternative<BranchInstruction>(instruction)
             ? &std::get<BranchInstruction>(instruction)
             : nullptr;
}

constexpr auto as_single_memory(const InstructionVariant &instruction) {
  return std::holds_alternative<SingleMemoryInstruction>(instruction)
             ? &std::get<SingleMemoryInstruction>(instruction)
             : nullptr;
}

constexpr auto as_block_memory(const InstructionVariant &instruction) {
  return std::holds_alternative<BlockMemoryInstruction>(instruction)
             ? &std::get<BlockMemoryInstruction>(instruction)
             : nullptr;
}

} // namespace aavm::ir

#endif
