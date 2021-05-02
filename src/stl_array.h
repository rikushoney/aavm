#ifndef AAVM_STL_ARRAY_H_
#define AAVM_STL_ARRAY_H_

#include <array>
#include <cstddef> // for std::size_t
#include <utility> // for std::index_sequence and std::move

namespace aavm::stl {

namespace detail_ {

template <typename T, std::size_t N, std::size_t... I>
constexpr auto make_array_lvalue_(T (&a)[N], std::index_sequence<I...>)
    -> std::array<T, N> {
  return {{a[I]...}};
}

template <typename T, std::size_t N, std::size_t... I>
constexpr auto make_array_rvalue_(T(&&a)[N], std::index_sequence<I...>)
    -> std::array<T, N> {
  return {{std::move(a[I])...}};
}

} // namespace detail_

template <typename T, std::size_t N> constexpr auto make_array(T(&&a)[N]) {
  return detail_::make_array_lvalue_(a, std::make_index_sequence<N>{});
}

template <typename T, std::size_t N> constexpr auto make_array(T (&a)[N]) {
  return detail_::make_array_rvalue_(a, std::make_index_sequence<N>{});
}

} // namespace aavm::stl

#endif
