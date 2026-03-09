cmake_minimum_required(VERSION 4.0)
project(grammar_parser)

add_library(grammar_parser STATIC
        grammar/Grammar.cpp
        grammar/Production.cpp
        grammar/Symbol.cpp
        parser/GrammarParser.cpp
)

target_include_directories(grammar_parser PUBLIC .)

target_compile_features(grammar_parser PUBLIC cxx_std_20)

add_executable(grammar_parser_tests tests/GrammarParserTest.cpp)

target_link_libraries(grammar_parser_tests PRIVATE grammar_parser)

find_package(GTest REQUIRED)
target_link_libraries(grammar_parser_tests PRIVATE GTest::gtest_main)
