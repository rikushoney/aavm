cmake_minimum_required(VERSION 3.13.4)
if(${CMAKE_VERSION} VERSION_GREATER "3.15")
  cmake_policy(SET CMP0091 NEW)
endif()

project(aavm CXX)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)

# AAVM requires C++20
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "Setting build type to 'Debug' as none was specified.")
  set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_definitions(AAVM_CLANG=1 AAVM_GCC=0 AAVM_MSVC=0)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  add_compile_definitions(AAVM_CLANG=0 AAVM_GCC=1 AAVM_MSVC=0)
elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  add_compile_definitions(AAVM_CLANG=0 AAVM_GCC=0 AAVM_MSVC=1)
else()
  add_compile_definitions(AAVM_CLANG=0 AAVM_GCC=0 AAVM_MSVC=0)
endif()

if(WIN32)
  add_compile_definitions(AAVM_WINDOWS=1 AAVM_LINUX=0 AAVM_MACOS=0)
elseif(UNIX AND NOT APPLE)
  add_compile_definitions(AAVM_WINDOWS=0 AAVM_LINUX=1 AAVM_MACOS=0)
elseif(APPLE)
  add_compile_definitions(AAVM_WINDOWS=0 AAVM_LINUX=0 AAVM_MACOS=1)
else()
  add_compile_definitions(AAVM_WINDOWS=0 AAVM_LINUX=0 AAVM_MACOS=0)
endif()

# Output compiled binaries and library files to appropriate directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Force MSVC builds to use static linking 
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

option(AAVM_ENABLE_TESTING "Enable building aavm unit tests" ON)

add_subdirectory(third_party)
add_subdirectory(src)

if(AAVM_ENABLE_TESTING)
  enable_testing()
  add_subdirectory(test)
endif()
