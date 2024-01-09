cmake_minimum_required(VERSION 3.28.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)

project(cpu)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

cmake_policy(SET CMP0135 NEW)
include(FetchContent)
FetchContent_Declare(
  	googletest
  	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_compile_options(-Wall -Werror -Wextra -Werror=switch-enum)

include(CTest)
include(GoogleTest)

add_subdirectory(src)
add_subdirectory(test)

enable_testing()