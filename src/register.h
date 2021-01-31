#ifndef AAVM_IR_REGISTER_H_
#define AAVM_IR_REGISTER_H_

namespace aavm::ir {

struct Register {
  enum Kind {
    R0 = 1,
    R1 = 2,
    R2 = 3,
    R3 = 4,
    R4 = 5,
    R5 = 6,
    R6 = 7,
    R7 = 8,
    R8 = 9,
    R9 = 10,
    R10 = 11,
    R11 = 12,
    R12 = 13,
    SP = 14,
    LR = 15,
    PC = 16
  };
};

} // namespace aavm::ir

#endif
