#ifndef AAVM_IR_INSTRUCTION_H_
#define AAVM_IR_INSTRUCTION_H_

#include "condition.h"

namespace aavm::ir {

class Instruction {
public:
  constexpr Instruction(unsigned operation, condition::Kind condition,
                        bool updatesflags)
      : op_{operation}, condition_{condition}, updates_{updatesflags} {}

  auto operation() const { return op_; }

  auto condition() const { return condition_; }

  auto updatesflags() const { return updates_; }

  enum ArithmeticOperation {
    arithmetic_operations_start_ = 1,
    Add = 1,
    Adc = 2,
    Sub = 3,
    Sbc = 4,
    Rsb = 5,
    Rsc = 6,
    And = 7,
    Eor = 8,
    Orr = 9,
    Bic = 10,
    Adr = 11,
    arithmetic_operations_end_ = 11
  };

  enum ShiftOperation {
    shift_operations_start_ = 12,
    Asr = 12,
    Lsl = 13,
    Lsr = 14,
    Ror = 15,
    Rrx = 16,
    shift_operations_end_ = 16
  };

  enum MultiplyOperation {
    multiply_operations_start_ = 17,
    Mul = 17,
    Mla = 18,
    Mls = 19,
    Umull = 20,
    Umlal = 21,
    Smull = 22,
    Smlal = 23,
    multiply_operations_end_ = 23
  };

  enum DivideOperation {
    divide_operations_start_ = 24,
    Sdiv = 24,
    Udiv = 25,
    divide_operations_end_ = 25
  };

  enum MoveOperation {
    move_operations_start_ = 26,
    Mov = 26,
    Mvn = 27,
    Movt = 28,
    Movw = 29,
    move_operations_end_ = 29
  };

  enum ComparisonOperation {
    compare_operations_start_ = 30,
    Cmp = 30,
    Cmn = 31,
    Tst = 32,
    Teq = 33,
    compare_operations_end_ = 33
  };

  enum BitfieldOperation {
    bitfield_operations_start_ = 34,
    Bfc = 34,
    Bfi = 35,
    Sbfx = 36,
    Ubfx = 37,
    bitfield_operations_end_ = 37
  };

  enum ReverseOperation {
    reverse_operations_start_ = 38,
    Rbit = 38,
    Rev = 39,
    Rev16 = 40,
    Revsh = 41,
    reverse_operations_end_ = 41
  };

  enum BranchOperation {
    branch_operations_start_ = 42,
    B = 42,
    Bl = 43,
    Bx = 44,
    Cbz = 45,
    Cbnz = 46,
    branch_operations_end_ = 46
  };

  enum SingleMemoryOperation {
    single_memory_operations_start_ = 47,
    Ldr = 47,
    Ldrb = 48,
    Ldrsb = 49,
    Ldrh = 50,
    Ldrsh = 51,
    Str = 52,
    Strb = 53,
    Strh = 54,
    single_memory_operations_end_ = 54
  };

  enum BlockMemoryOperation {
    block_memory_operations_start_ = 55,
    Ldm = 55,
    Ldmia = 56,
    Ldmib = 57,
    Ldmda = 58,
    Ldmdb = 59,
    Stm = 60,
    Stmia = 61,
    Stmib = 62,
    Stmda = 63,
    Stmdb = 64,
    Push = 65,
    Pop = 66,
    block_memory_operations_end_ = 66
  };

  static constexpr auto is_arithmetic_instruction(unsigned op) {
    return op >= arithmetic_operations_start_ &&
           op <= arithmetic_operations_end_;
  }

  static constexpr auto is_shift_instruction(unsigned op) {
    return op >= shift_operations_start_ && op <= shift_operations_end_;
  }

  static constexpr auto is_multiply_instruction(unsigned op) {
    return op >= multiply_operations_start_ && op <= multiply_operations_end_;
  }

  static constexpr auto is_divide_instruction(unsigned op) {
    return op >= divide_operations_start_ && op <= divide_operations_end_;
  }

  static constexpr auto is_move_instruction(unsigned op) {
    return op >= move_operations_start_ && op <= move_operations_end_;
  }

  static constexpr auto is_comparison_instruction(unsigned op) {
    return op >= compare_operations_start_ && op <= compare_operations_end_;
  }

  static constexpr auto is_bitfield_instruction(unsigned op) {
    return op >= bitfield_operations_start_ && op <= bitfield_operations_end_;
  }

  static constexpr auto is_reverse_instruction(unsigned op) {
    return op >= reverse_operations_start_ && op <= reverse_operations_end_;
  }

  static constexpr auto is_branch_instruction(unsigned op) {
    return op >= branch_operations_start_ && op <= branch_operations_end_;
  }

  static constexpr auto is_single_memory_instruction(unsigned op) {
    return op >= single_memory_operations_start_ &&
           op <= single_memory_operations_end_;
  }

  static constexpr auto is_block_memory_instruction(unsigned op) {
    return op >= block_memory_operations_start_ &&
           op <= block_memory_operations_end_;
  }

private:
  unsigned op_;
  condition::Kind condition_;
  bool updates_;
};

} // namespace aavm::ir

#endif
