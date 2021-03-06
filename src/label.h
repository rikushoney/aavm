#ifndef AAVM_IR_LABEL_H_
#define AAVM_IR_LABEL_H_

#include <string_view>

namespace aavm::ir {

using LabelID = unsigned;

class Label {
public:
  explicit constexpr Label(LabelID id, std::string_view name)
      : id_{id}, name_{name} {}

  constexpr auto id() const { return id_; }
  constexpr auto name() const { return name_; }

private:
  LabelID id_;
  std::string_view name_;
};

} // namespace aavm::ir

#endif
