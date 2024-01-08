cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_COMPILER g++)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../) 

include(FetchContent)
FetchContent_Declare(
  	googletest
  	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

project(cpu)

add_compile_options(-Wall -Werror -Wextra -Werror=switch-enum)

add_subdirectory(src)
add_subdirectory(test)