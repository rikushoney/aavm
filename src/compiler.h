#ifndef AAVM_COMPILER_H_
#define AAVM_COMPILER_H_

#if !(defined(AAVM_MSVC) && defined(AAVM_CLANG) && defined(AAVM_GCC) &&        \
      defined(AAVM_WINDOWS) && defined(AAVM_LINUX) && defined(AAVM_MACOS))
#error "config error"
#endif

namespace aavm {
namespace compiler {

#if AAVM_MSVC
static constexpr auto msvc = true;
static constexpr auto clang = false;
static constexpr auto gcc = false;
#elif AAVM_CLANG
static constexpr auto msvc = false;
static constexpr auto clang = true;
static constexpr auto gcc = false;
#elif AAVM_GCC
static constexpr auto msvc = false;
static constexpr auto clang = false;
static constexpr auto gcc = true;
#else
#warning "unsupported compiler"
static constexpr auto msvc = false;
static constexpr auto clang = false;
static constexpr auto gcc = false;
#endif

} // namespace compiler

namespace platform {

#if AAVM_WINDOWS
static constexpr auto windows = true;
static constexpr auto linux = false;
static constexpr auto macos = false;
#elif AAVM_LINUX
static constexpr auto windows = false;
static constexpr auto linux = true;
static constexpr auto macos = false;
#elif AAVM_MACOS
static constexpr auto windows = false;
static constexpr auto linux = false;
static constexpr auto macos = true;
#else
#warning "unsupported platform"
static constexpr auto windows = false;
static constexpr auto linux = false;
static constexpr auto macos = false;
#endif

} // namespace platform

inline auto aavm_unreachable() {
#if AAVM_GCC || AAVM_CLANG
  __builtin_unreachable();
#elif AAVM_MSVC
  __assume(0);
#else
  (void);
#endif
}

} // namespace aavm

#endif
