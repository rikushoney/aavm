#ifndef AAVM_INSTRUCTION_H
#define AAVM_INSTRUCTION_H

#include "operand2.h"
#include "token.h"

#include <cstdint>
#include <memory>
#include <variant>

namespace aavm {

namespace parser {
class Parser;
}

namespace ir {

class Instruction {
  friend class parser::Parser;

public:
  using int_type = std::int32_t;
  using opcode_type = std::uint32_t;
  using operation_type = parser::token::Kind;
  using condition_type = parser::token::Kind;
  using register_type = parser::token::Kind;

  constexpr Instruction(operation_type op, condition_type cond, bool update)
      : op_{op}, cond_{cond}, update_{update} {}

  constexpr Instruction(const Instruction &other)
      : op_{other.op_}, cond_{other.cond_}, update_{other.update_} {}

  constexpr operation_type operation() const { return op_; }
  constexpr condition_type condition() const { return cond_; }
  constexpr bool updates_flags() const { return update_; }

  virtual opcode_type encode() const;
  static std::unique_ptr<Instruction> decode(opcode_type opcode);

private:
  operation_type op_;
  condition_type cond_;
  bool update_;
};

struct ArithmeticInstruction : public Instruction {
  register_type rd;
  register_type rn;
  Operand2 src2;

  ArithmeticInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct MultiplyInstruction : public Instruction {
  register_type rd;
  register_type rm;
  register_type rs;
  register_type rn;
  register_type rdlo;
  register_type rdhi;

  MultiplyInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct DivideInstruction : public Instruction {
  register_type rd;
  register_type rn;
  register_type rm;

  DivideInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct MoveInstruction : public Instruction {
  using imm16 = std::uint16_t;
  using source_variant = std::variant<Operand2, imm16>;
  register_type rd;
  source_variant src;

  MoveInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct CompareInstruction : public Instruction {
  register_type rn;
  Operand2 src2;

  CompareInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct BitfieldInstruction : public Instruction {
  register_type rd;
  register_type rn;
  int_type lsb;
  int_type width;

  BitfieldInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct ReverseInstruction : public Instruction {
  register_type rd;
  register_type rm;

  ReverseInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct BranchInstruction : public Instruction {
  using offset_variant = std::variant<int_type, std::string_view>;
  offset_variant target;
  register_type rm;
  register_type rn;

  BranchInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct SingleMemoryInstruction : public Instruction {
  using source_variant = std::variant<Operand2, int_type, std::string_view>;
  register_type rd;
  register_type rn;
  source_variant source;
  enum class IndexMode { Post, Offset, Pre } indexmode;

  SingleMemoryInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

struct MultipleMemoryInstruction : public Instruction {
  register_type rn;
  std::vector<register_type> register_list;
  bool writeback;
  MultipleMemoryInstruction(const Instruction &other) : Instruction{other} {}
  opcode_type encode() const override;
};

} // namespace ir
} // namespace aavm

#endif
