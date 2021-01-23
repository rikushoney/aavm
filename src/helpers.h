#ifndef AAVM_HELPERS_H_
#define AAVM_HELPERS_H_

#include <array>
#include <cstddef>
#include <utility>

namespace aavm {

template <typename T, std::size_t... I>
constexpr auto make_array(T (&a)[sizeof...(I)], std::index_sequence<I...>)
    -> std::array<T, sizeof...(I)> {
  return {{std::move(a[I])...}};
}

template <typename T, std::size_t N> constexpr auto make_array(T(&&a)[N]) {
  return make_array(a, std::make_index_sequence<N>{});
}

} // namespace aavm

#endif
