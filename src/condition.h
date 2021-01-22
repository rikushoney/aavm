#ifndef AAVM_IR_CONDITION_H_
#define AAVM_IR_CONDITION_H_

#include "token.h"

namespace aavm::ir::condition {

enum Kind {
  EQ = token::kw_eq,
  NE = token::kw_ne,
  CS = token::kw_cs,
  CC = token::kw_cc,
  MI = token::kw_mi,
  PL = token::kw_pl,
  VS = token::kw_vs,
  VC = token::kw_vc,
  HI = token::kw_hi,
  LS = token::kw_ls,
  GE = token::kw_ge,
  LT = token::kw_lt,
  GT = token::kw_gt,
  LE = token::kw_le,
  AL = token::kw_al
};

} // namespace aavm::ir::condition

#endif
