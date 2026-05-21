### C YAML Parser

This is a C project for parsing YAML files with libyaml. It includes testing with unity. Dependency management is handled with CMake. Memory management is done with valgrind.

``` bash
./compile # Compiles with CMake
./test # Runs tests with unity
./check # Compiles, checks memory leaks and runs tests
./valgrind # Runs valgrind on the parser
./parse test.yaml # Parses test.yaml file in /configs
```

Requirements:
- gcc
- CMake
- valgrind