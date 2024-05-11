#
# OS/CMakeLists.txt
# Made by RyanM on 30/04/2024.
#

cmake_minimum_required(VERSION 3.10)    # Set minimum CMake version
project(mssv)                           # Define the project name

set(CMAKE_C_STANDARD 99)                # Set C standard to C99

# Specify the target to build and the source files
add_executable(mssv
        Assignment/mssv.c
        Assignment/sudoku_validator.c
        Assignment/utilities.c
)

# Include directories to ensure the compiler can find the header files
include_directories(Assignment)

# Link the pthread library for threading support
find_package(Threads REQUIRED)
target_link_libraries(mssv PRIVATE Threads::Threads)

