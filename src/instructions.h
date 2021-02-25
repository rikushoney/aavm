#ifndef AAVM_IR_INSTRUCTIONS_H_
#define AAVM_IR_INSTRUCTIONS_H_

#include "compiler.h"
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
  union {
    Operand2 src2;
    const Label *label;
  };

  constexpr ArithmeticInstruction(ArithmeticOperation op, condition::Kind cond,
                                  bool update, Register::Kind rd,
                                  Register::Kind rn, Operand2 src2)
      : Instruction{op, cond, update}, rd{rd}, rn{rn}, src2{src2} {}

  constexpr ArithmeticInstruction(ArithmeticOperation op, condition::Kind cond,
                                  Register::Kind rd, const Label *label)
      : Instruction{op, cond, false}, rd{rd}, label{label} {}
};

struct MultiplyInstruction : public Instruction {
// clang, gcc and msvc support anonymous structs as an extension
#if defined(AAVM_CLANG)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#pragma clang diagnostic ignored "-Wnested-anon-types"
#elif defined(AAVM_GCC)
#pragma gcc diagnostic push
#pragma gcc diagnostic ignored "-Wgnu-anonymous-struct"
#pragma gcc diagnostic ignored "-Wnested-anon-types"
#elif defined(AAVM_MSVC)
#pragma warning(push)
// thing
#endif
  union {
    Register::Kind rd;
    struct {
      Register::Kind rdlo;
      Register::Kind rdhi;
    };
  };
#if defined(AAVM_CLANG)
#pragma clang diagnostic pop
#elif defined(AAVM_GCC)
#pragma gcc diagnostic pop
#elif defined(AAVM_MSVC)
#pragma warning(pop)
#endif

  Register::Kind rm{};
  Register::Kind rs{};
  Register::Kind rn{};

  constexpr MultiplyInstruction(MultiplyOperation op, condition::Kind cond,
                                bool update, Register::Kind rd,
                                Register::Kind rm, Register::Kind rs,
                                Register::Kind rn = Register::R0)
      : Instruction{op, cond, update}, rd{rd}, rm{rm}, rs{rs}, rn{rn} {}

  constexpr MultiplyInstruction(MultiplyOperation op, condition::Kind cond,
                                bool update,
                                std::pair<Register::Kind, Register::Kind> rd,
                                Register::Kind rm, Register::Kind rs)
      : Instruction{op, cond, update}, rdlo{rd.first}, rdhi{rd.second}, rm{rm},
        rs{rs} {}
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
  union {
    Operand2 src2;
    unsigned imm16;
  };

  constexpr MoveInstruction(MoveOperation op, condition::Kind cond, bool update,
                            Register::Kind rd, Operand2 src2)
      : Instruction{op, cond, update}, rd{rd}, src2{src2} {}

  constexpr MoveInstruction(MoveOperation op, condition::Kind cond,
                            Register::Kind rd, unsigned imm16)
      : Instruction{op, cond, false}, rd{rd}, imm16{imm16} {}
};

struct ComparisonInstruction : public Instruction {
  Register::Kind rn{};
  Operand2 src2;

  constexpr ComparisonInstruction(ComparisonOperation op, condition::Kind cond,
                                  Register::Kind rn, Operand2 src2)
      : Instruction{op, cond, true}, rn{rn}, src2{src2} {}
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
  Register::Kind rn{};
  std::variant<Operand2, const Label *, unsigned> source;
  enum class IndexMode { PostIndex, Offset, PreIndex } indexmode{};
  bool subtract{};

  constexpr SingleMemoryInstruction(SingleMemoryOperation op,
                                    condition::Kind cond, Register::Kind rd,
                                    Register::Kind rn, Operand2 src2,
                                    IndexMode mode, bool subtract)
      : Instruction{op, cond, false}, rd{rd}, rn{rn}, source{src2},
        indexmode{mode}, subtract{subtract} {}

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

  /* constexpr */ BlockMemoryInstruction(
      BlockMemoryOperation op, condition::Kind cond, Register::Kind rn,
      bool writeback, const std::vector<Register::Kind> &registers)
      : Instruction{op, cond, false}, rn{rn}, writeback{writeback},
        register_list{registers} {}

  /* constexpr */ BlockMemoryInstruction(
      BlockMemoryOperation op, condition::Kind cond,
      const std::vector<Register::Kind> &registers)
      : Instruction{op, cond, false}, rn{Register::SP}, writeback{true},
        register_list{registers} {}
};

} // namespace aavm::ir

#endif
