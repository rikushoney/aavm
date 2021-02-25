#ifndef AAVM_COMPILER_H_
#define AAVM_COMPILER_H_

namespace aavm {
namespace compiler {

#ifndef AAVM_COMPILER_DETECTED
#define AAVM_COMPILER_DETECTED
#if defined(_MSC_VER)
#define AAVM_MSVC 1
static constexpr auto msvc = true;
#define AAVM_CLANG 0
static constexpr auto clang = false;
#define AAVM_GCC 0
static constexpr auto gcc = false;
#elif defined(__clang__)
#define AAVM_MSVC 0
static constexpr auto msvc = false;
#define AAVM_CLANG 1
static constexpr auto clang = true;
#define AAVM_GCC 0
static constexpr auto gcc = false;
#elif defined(__GNUC__)
#define AAVM_MSVC 0
static constexpr auto msvc = false;
#define AAVM_CLANG 0
static constexpr auto clang = false;
#define AAVM_GCC 1
static constexpr auto gcc = true;
#else
#error "AAVM is not supported with this compiler"
#define AAVM_MSVC 0
static constexpr auto msvc = false;
#define AAVM_CLANG 0
static constexpr auto clang = false;
#define AAVM_GCC 0
static constexpr auto gcc = false;
#endif
#endif

} // namespace compiler

namespace platform {

#ifndef AAVM_PLATFORM_DETECTED
#define AAVM_PLATFORM_DETECTED

#if defined(_WIN32)
#define AAVM_WINDOWS 1
static constexpr auto windows = true;
#define AAVM_LINUX 0
static constexpr auto linux = false;
#define AAVM_MACOS 0
static constexpr auto macos = false;
#elif defined(__linux__)
#define AAVM_WINDOWS 0
static constexpr auto windows = false;
#define AAVM_LINUX 1
static constexpr auto linux = true;
#define AAVM_MACOS 0
static constexpr auto macos = false;
#elif defined(__APPLE__)
#define AAVM_WINDOWS 0
static constexpr auto windows = false;
#define AAVM_LINUX 0
static constexpr auto linux = false;
#define AAVM_MACOS 1
static constexpr auto macos = true;
#else
#warning "AAVM is not supported on this system"
#define AAVM_WINDOWS 0
static constexpr auto windows = false;
#define AAVM_LINUX 0
static constexpr auto linux = false;
#define AAVM_MACOS 0
static constexpr auto macos = false;
#endif

#endif

} // namespace platform
} // namespace aavm

#endif
