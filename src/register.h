#ifndef AAVM_IR_REGISTER_H_
#define AAVM_IR_REGISTER_H_

namespace aavm::ir {

struct Register {
  enum Kind {
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    SP,
    LR,
    PC
  };
};

} // namespace aavm::ir

#endif
