get_filename_component(TARGET_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

file(GLOB SOURCES "*.cpp" "*.h")
add_executable(${TARGET_NAME} ${SOURCES})

