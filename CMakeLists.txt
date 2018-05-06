cmake_minimum_required(VERSION 3.10)
project(result C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(deps)
include(deps/error/error.cmake)
include(deps/panic/panic.cmake)

add_library(result sources/result.h sources/result.c)
target_link_libraries(result PRIVATE panic)
target_link_libraries(result PUBLIC error)

add_executable(main examples/main.c)
target_link_libraries(main PRIVATE result)
target_include_directories(main PRIVATE sources)
