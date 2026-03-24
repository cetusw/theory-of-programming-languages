cmake_minimum_required(VERSION 4.0)
project(grammar_parser)

add_library(grammar_parser STATIC
        grammar/Grammar.cpp
        grammar/Production.cpp
        grammar/Symbol.cpp
        parser/GrammarParser.cpp
        printer/GrammarPrinter.cpp
        ../guiding-set-provider/GuidingSetProvider.cpp
        ../guiding-set-provider/GuidingSetProvider.h
        ../ll1-generator/table-processing/TableRow.h
        ../ll1-generator/table-processing/TableGenerator.cpp
        ../ll1-generator/table-processing/TableGenerator.h
        ../ll1-generator/table-processing/CsvExporter.cpp
        ../ll1-generator/table-processing/CsvExporter.h
        ../ll1-generator/runner/LL1Runner.cpp
        ../ll1-generator/runner/LL1Runner.h
        ../ll1-generator/table-processing/AddressMap.cpp
        ../ll1-generator/table-processing/AddressMap.h
)

target_include_directories(grammar_parser PUBLIC .)

target_compile_features(grammar_parser PUBLIC cxx_std_20)

add_executable(grammar_parser_tests
        tests/GrammarParserTest.cpp
        tests/GrammarPrinterTest.cpp
)

target_link_libraries(grammar_parser_tests PRIVATE grammar_parser)

find_package(GTest REQUIRED)
target_link_libraries(grammar_parser_tests PRIVATE GTest::gtest_main)
