#include "operand2.h"
#include "token.h"

#include <cstdint>

namespace aavm {

class InstructionEncoder;
class InstructionDecoder;

namespace parser {

class Instruction {
  friend InstructionDecoder;
  friend InstructionEncoder;

public:
  using opcode_type = token::Kind;
  using condition_type = token::Kind;
  using register_type = token::Kind;
  constexpr Instruction(opcode_type op, condition_type cond, bool update)
      : op_{op}, cond_{cond}, update_{update} {}

  constexpr Instruction(const Instruction &other)
      : op_{other.op_}, cond_{other.cond_}, update_{other.update_} {}

  constexpr opcode_type opcode() const { return op_; }
  constexpr condition_type condition() const { return cond_; }
  constexpr bool updates_flags() const { return update_; }

private:
  opcode_type op_;
  condition_type cond_;
  bool update_;
};

struct ArithmeticInstruction : public Instruction {
  register_type rd;
  register_type rn;
  Operand2 operand2;

  ArithmeticInstruction(const Instruction &other) : Instruction{other} {}
};

} // namespace parser
} // namespace aavm
