cmake_minimum_required(VERSION 4.0)
project(ll1_generator)

set(CMAKE_CXX_STANDARD 20)

add_library(ll1_generator_lib STATIC
        runner/LL1Runner.cpp
        runner/LL1Runner.h
        table-processing/AddressMap.cpp
        table-processing/AddressMap.h
        table-processing/CsvExporter.cpp
        table-processing/CsvExporter.h
        table-processing/TableGenerator.cpp
        table-processing/TableGenerator.h
        table-processing/TableRow.h
        runner/ParseTreeNode.h
)

target_include_directories(ll1_generator_lib PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(ll1_generator_lib PRIVATE
        guiding_set_provider_lib
        grammar_parser
)

add_executable(ll1_generator main.cpp)
target_link_libraries(ll1_generator PRIVATE
        ll1_generator_lib
)
