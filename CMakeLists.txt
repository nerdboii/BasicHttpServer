cmake_minimum_required(VERSION 3.16)
project(DTL_MINIPROJECT)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include headers
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

# Build executable
add_executable(DTL_MINIPROJECT ${SOURCES})

# Set compile options (optional)
target_compile_options(DTL_MINIPROJECT PRIVATE -Wall -Wextra -Wpedantic)

# Set output directory (optional)
set_target_properties(DTL_MINIPROJECT PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)