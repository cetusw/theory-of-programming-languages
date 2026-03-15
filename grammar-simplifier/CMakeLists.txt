cmake_minimum_required(VERSION 4.0)
project(grammar_simplifier)

set(CMAKE_CXX_STANDARD 20)

add_library(grammar_simplifier_lib STATIC
        EpsilonEliminator.cpp
        EpsilonEliminator.h
        ChainRuleEliminator.cpp
        ChainRuleEliminator.h
        UselessSymbolEliminator.cpp
        UselessSymbolEliminator.h
        LeftRecursionEliminator.cpp
        LeftRecursionEliminator.h
        GrammarFactorizer.cpp
        GrammarFactorizer.h
)

target_include_directories(grammar_simplifier_lib PUBLIC . ..)
target_link_libraries(grammar_simplifier_lib PRIVATE grammar_parser)

add_executable(grammar_simplifier main.cpp)
target_link_libraries(grammar_simplifier PRIVATE
        grammar_simplifier_lib
        grammar_parser
)

add_executable(grammar_simplifier_tests tests/GrammarSimplifierTest.cpp)
target_include_directories(grammar_simplifier_tests PRIVATE ..)
target_link_libraries(grammar_simplifier_tests PRIVATE
        grammar_simplifier_lib
        grammar_parser
        GTest::gtest_main
)
find_package(GTest REQUIRED)