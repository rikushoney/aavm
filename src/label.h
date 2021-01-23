#ifndef AAVM_IR_LABEL_H_
#define AAVM_IR_LABEL_H_

#include <string_view>

namespace aavm::ir {

using LabelID = unsigned;

struct Label {
  LabelID id;
  std::string_view name;
};

} // namespace aavm::ir

#endif
