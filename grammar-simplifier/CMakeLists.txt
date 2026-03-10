cmake_minimum_required(VERSION 4.0)
project(grammar_simplifier)

set(CMAKE_CXX_STANDARD 20)

add_library(grammar_simplifier STATIC
        EpsilonEliminator.cpp
        EpsilonEliminator.h
)

target_include_directories(grammar_simplifier PUBLIC . ..)

target_link_libraries(grammar_simplifier PRIVATE grammar_parser)

add_executable(grammar_simplifier_tests
        tests/EpsilonEliminatorTest.cpp
)

target_include_directories(grammar_simplifier_tests PRIVATE ..)

target_link_libraries(grammar_simplifier_tests PRIVATE
        grammar_simplifier
        grammar_parser
        GTest::gtest_main
)

find_package(GTest REQUIRED)