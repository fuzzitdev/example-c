cmake_minimum_required( VERSION 3.6.0 )

# Specify path for CMake modules to be loaded by include()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(DefineCompilerFlags REQUIRED)

project(continuous-fuzzing-example
        VERSION 1.0.0
        LANGUAGES CXX C)

add_subdirectory( src )
add_subdirectory( fuzz )
