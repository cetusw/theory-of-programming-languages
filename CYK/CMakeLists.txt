cmake_minimum_required(VERSION 3.10)
project(CYK)

find_package(GTest REQUIRED)

get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB LOGIC_SOURCES "*.cpp" "*.h")
list(FILTER LOGIC_SOURCES EXCLUDE REGEX "main\\.cpp$")
file(GLOB TEST_SOURCES "tests/*.cpp" "tests/*.h")

add_executable(${TARGET_NAME} main.cpp ${LOGIC_SOURCES})

if(TEST_SOURCES)
    add_executable(${TARGET_NAME}_tests ${TEST_SOURCES} ${LOGIC_SOURCES})
    target_link_libraries(${TARGET_NAME}_tests PRIVATE GTest::GTest GTest::Main)
endif()


