cmake_minimum_required(VERSION 4.0)
project(guiding_set_provider)

set(CMAKE_CXX_STANDARD 20)

add_library(guiding_set_provider_lib STATIC
        GuidingSetProvider.cpp
        GuidingSetProvider.h
)

target_include_directories(guiding_set_provider_lib PUBLIC . ..)
target_link_libraries(guiding_set_provider_lib PRIVATE grammar_parser)

add_executable(guiding_set_provider main.cpp)
target_link_libraries(guiding_set_provider PRIVATE
        guiding_set_provider_lib
        grammar_parser
)

add_executable(guiding_set_provider_tests
        ./tests/GuidingSetProviderTest.cpp
)
target_include_directories(guiding_set_provider_tests PRIVATE ..)
target_link_libraries(guiding_set_provider_tests PRIVATE
        guiding_set_provider_lib
        grammar_parser
        GTest::gtest_main
)
find_package(GTest REQUIRED)