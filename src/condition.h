#ifndef AAVM_IR_CONDITION_H_
#define AAVM_IR_CONDITION_H_

#include "token.h"

namespace aavm::ir::condition {

enum Kind { EQ, NE, CS, CC, MI, PL, VS, VC, HI, LS, GE, LT, GT, LE, AL };

} // namespace aavm::ir::condition

#endif
