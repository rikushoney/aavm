include(FetchContent)

if(NOT ${fmt_POPULATED})
  message(STATUS "Downloading fmt")
endif()

FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG        7.1.3
  GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(fmt)

FetchContent_GetProperties(googletest)
if(AAVM_ENABLE_TESTING)
  FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG        release-1.10.0
    GIT_SHALLOW    TRUE
  )

  if(NOT ${googletest_POPULATED})
    message(STATUS "Downloading googletest")
    FetchContent_Populate(googletest)
    # Patch googletest deprecation warnings
    set(GTEST_FILES_TO_PATCH
      "CMakeLists.txt;googlemock/CMakeLists.txt;googletest/CMakeLists.txt")
    foreach(file ${GTEST_FILES_TO_PATCH})
      file(READ "${googletest_SOURCE_DIR}/${file}" FILE_CONTENTS)
      string(REPLACE
        "cmake_minimum_required(VERSION 2.6.4)"
        "cmake_minimum_required(VERSION 2.8.12)"
        FILE_CONTENTS "${FILE_CONTENTS}")
      string(REPLACE
        "cmake_minimum_required(VERSION 2.8.8)"
        "cmake_minimum_required(VERSION 2.8.12)"
        FILE_CONTENTS "${FILE_CONTENTS}")
      file(WRITE "${googletest_SOURCE_DIR}/${file}" "${FILE_CONTENTS}")
    endforeach()
  endif()
  add_subdirectory("${googletest_SOURCE_DIR}" "${googletest_BINARY_DIR}")
endif()
