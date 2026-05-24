### C YAML Parser

A C YAML parser using libyaml. Nested mappings are flattened into dotted-path keys ( `config.stage`). Sequences are not supported.

It includes testing with unity. Dependency management and build with CMake. Memory management with valgrind.

``` bash
./compile            # Compiles with CMake
./test               # Runs tests with Unity
./check              # Compiles, checks memory leaks and runs tests
./memcheck           # Runs valgrind on the parser
./parse <file.yaml>  # Parses <file.yaml> file in /configs
```

Requirements:
- gcc
- CMake
- valgrind