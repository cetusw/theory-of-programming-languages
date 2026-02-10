cmake_minimum_required(VERSION 3.10)
project(theory-of-programming-languages)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Общая библиотека, если нужно использовать один код в разных лабах
# add_subdirectory(common)

 file(GLOB SUBDIRS RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *)
 foreach(DIR ${SUBDIRS})
     if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${DIR} AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${DIR}/CMakeLists.txt)
         add_subdirectory(${DIR})
     endif()
 endforeach()