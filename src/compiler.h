#ifndef AAVM_COMPILER_H_
#define AAVM_COMPILER_H_

namespace aavm {
namespace compiler {

#if defined(AAVM_MSVC)
static constexpr auto msvc = true;
static constexpr auto clang = false;
static constexpr auto gcc = false;
#elif defined(AAVM_CLANG)
static constexpr auto msvc = false;
static constexpr auto clang = true;
static constexpr auto gcc = false;
#elif defined(AAVM_GCC)
static constexpr auto msvc = false;
static constexpr auto clang = false;
static constexpr auto gcc = true;
#else
static constexpr auto msvc = false;
static constexpr auto clang = false;
static constexpr auto gcc = false;
#endif

} // namespace compiler

namespace platform {

#if defined(AAVM_WINDOWS)
static constexpr auto windows = true;
static constexpr auto linux = false;
static constexpr auto macos = false;
#elif defined(AAVM_LINUX)
static constexpr auto windows = false;
static constexpr auto linux = true;
static constexpr auto macos = false;
#elif defined(AAVM_MACOS)
static constexpr auto windows = false;
static constexpr auto linux = false;
static constexpr auto macos = true;
#else
static constexpr auto windows = false;
static constexpr auto linux = false;
static constexpr auto macos = false;
#endif

} // namespace platform
} // namespace aavm

#endif
