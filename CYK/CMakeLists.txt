cmake_minimum_required(VERSION 3.14)
project(CYK LANGUAGES CXX)

# Используем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# -------------------------------------------------------------------
# 1. Поиск зависимостей
# -------------------------------------------------------------------
find_package(GTest REQUIRED)

# -------------------------------------------------------------------
# 2. Создание библиотеки с логикой (CYK_lib)
# -------------------------------------------------------------------
# Собираем все файлы реализации (.cpp) из подпапок
file(GLOB_RECURSE LIB_SOURCES
        "Algorithms/*.cpp"
        "CNF/*.cpp"
        "Logger/*.cpp"
        "Parser/*.cpp"
        "Types/*.cpp"
        "CYKValidator.cpp"
)

# Создаем статическую библиотеку, чтобы не компилировать файлы дважды
add_library(CYK_lib STATIC ${LIB_SOURCES})

# Указываем, где искать заголовочные файлы
# PUBLIC означает, что все, кто линкуется с CYK_lib, тоже увидят эти пути
target_include_directories(CYK_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# -------------------------------------------------------------------
# 3. Основное приложение (CYK_Tool)
# -------------------------------------------------------------------
add_executable(CYK_Tool main.cpp)

# Просто линкуем библиотеку. CMake сам прокинет пути инклюдов и объектные файлы
target_link_libraries(CYK_Tool PRIVATE CYK_lib)

# -------------------------------------------------------------------
# 4. Тесты (CYK_tests)
# -------------------------------------------------------------------
# Находим все файлы тестов в папке tests
file(GLOB TEST_SOURCES "tests/*.cpp")

if(TEST_SOURCES)
    add_executable(CYK_tests ${TEST_SOURCES})

    # Линкуем библиотеку логики, сам GTest и стандартный мейн GTest
    target_link_libraries(CYK_tests
            PRIVATE
            CYK_lib
            GTest::GTest
            GTest::Main
    )

    # Чтобы тесты могли писать #include "tests/Utils.h" или #include "Utils.h"
    target_include_directories(CYK_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/tests)
endif()