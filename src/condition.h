#ifndef AAVM_IR_CONDITION_H_
#define AAVM_IR_CONDITION_H_

#include "token.h"

namespace aavm::ir {

struct Condition {
  enum Kind {
    EQ = 1,
    NE = 2,
    CS = 3,
    CC = 4,
    MI = 5,
    PL = 6,
    VS = 7,
    VC = 8,
    HI = 9,
    LS = 10,
    GE = 11,
    LT = 12,
    GT = 13,
    LE = 14,
    AL = 15
  };
};

} // namespace aavm::ir

#endif
