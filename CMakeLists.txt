cmake_minimum_required(VERSION 4.0)
project(TheoryOfProgrammingLanguages)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(grammar-parser)
add_subdirectory(grammar-simplifier)
add_subdirectory(guiding-set-provider)
