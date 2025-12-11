cmake_minimum_required(VERSION 3.10)
project(lab6_dungeon_editor)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Настройки для отладки и безопасности
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=return-type")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror=maybe-uninitialized")

# GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.15.0
  TLS_VERIFY false
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Библиотеки проекта
add_library(dungeon_lib 
    src/dungeon.cpp
    src/factory.cpp
    src/npc.cpp
    src/observer.cpp
    src/point.cpp
    src/visitor.cpp
)

add_library(npc_lib 
    src/npc.cpp
    src/point.cpp
)

# Подключаем заголовочные файлы
target_include_directories(dungeon_lib PUBLIC include)
target_include_directories(npc_lib PUBLIC include)

# Основная программа
add_executable(main main.cpp)
target_link_libraries(main PRIVATE dungeon_lib)

# Тесты
enable_testing()
add_executable(dungeon_tests test/tests.cpp)
target_link_libraries(dungeon_tests dungeon_lib gtest_main)
add_test(NAME dungeon_tests COMMAND dungeon_tests)

# Дополнительные цели (опционально)
add_custom_target(run_main COMMAND main)
add_custom_target(run_tests COMMAND dungeon_tests)
add_custom_target(all_tests DEPENDS dungeon_tests)

