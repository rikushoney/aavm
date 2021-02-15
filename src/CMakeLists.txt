add_library(aavm-parser lexer.cpp parser.cpp)
target_include_directories(aavm-parser PUBLIC ${CMAKE_CURRENT_LIST_DIR})
target_link_libraries(aavm-parser PUBLIC fmt)
