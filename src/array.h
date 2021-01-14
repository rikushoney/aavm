#ifndef AAVM_ARRAY_H
#define AAVM_ARRAY_H

#include <array>
#include <utility>

namespace aavm {

namespace detail {

template <typename T, std::size_t... I>
constexpr auto make_array_impl(T (&a)[sizeof...(I)], std::index_sequence<I...>)
    -> std::array<T, sizeof...(I)> {
  return {{std::move(a[I])...}};
}

} // namespace detail

template <typename T, std::size_t N> constexpr auto make_array(T(&&a)[N]) {
  return detail::make_array_impl(a, std::make_index_sequence<N>{});
}

} // namespace aavm

#endif
