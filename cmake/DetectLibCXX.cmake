include(CheckCXXSourceRuns)

macro(detect_libcxx result_var)
  set(result_var 0)
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_REQUIRED_FLAGS "-stdlib=libc++")
    check_cxx_source_runs("
#include <ciso646>
#ifndef _LIBCPP_VERSION
#error
#endif
int main() { return 0; }
"
    ${result_var})
  endif()
endmacro()
