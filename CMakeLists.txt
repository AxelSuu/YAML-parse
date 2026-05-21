cmake_minimum_required(VERSION 3.15)
project(yaml_parser C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Find libyaml via pkg-config
find_package(PkgConfig REQUIRED)
pkg_check_modules(YAML REQUIRED yaml-0.1)

include(FetchContent)
FetchContent_Declare(
    unity
    GIT_REPOSITORY https://github.com/ThrowTheSwitch/Unity.git
    GIT_TAG        v2.6.0
)
FetchContent_MakeAvailable(unity)

# --- Library: all logic except main.c ---
add_library(yaml_parser_lib
    src/parser.c
)
target_include_directories(yaml_parser_lib PUBLIC
    include
    ${YAML_INCLUDE_DIRS}
)
target_link_libraries(yaml_parser_lib PUBLIC
    ${YAML_LIBRARIES}
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