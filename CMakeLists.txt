cmake_minimum_required(VERSION 3.15)
project(yaml_parser C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
    libyaml
    GIT_REPOSITORY https://github.com/yaml/libyaml.git
)
FetchContent_MakeAvailable(libyaml)

FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
)
FetchContent_MakeAvailable(unity)

# --- Library: all logic except main.c ---
add_library(yaml_parser_lib
    src/parser.c
)
target_include_directories(yaml_parser_lib PUBLIC
    include
)
target_link_libraries(yaml_parser_lib PUBLIC
    yaml
)
target_compile_options(yaml_parser_lib PRIVATE -Wall -Wextra -Werror)

# --- Main executable ---
add_executable(yaml_parser src/main.c)
target_link_libraries(yaml_parser PRIVATE yaml_parser_lib)

# --- Tests ---
enable_testing()

add_executable(test_parser tests/test_parser.c)
target_link_libraries(test_parser PRIVATE unity yaml_parser_lib)
add_test(NAME test_parser COMMAND test_parser)