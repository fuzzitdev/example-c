cmake_minimum_required( VERSION 3.6.0 )

# Specify path for CMake modules to be loaded by include()
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(DefineCompilerFlags REQUIRED)

project(continuous-fuzzing-example
        VERSION 1.0.0
        LANGUAGES CXX C)

if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    message(FATAL_ERROR "You need to build with Clang for this example to work. "
            "export CC=`which clang` export CXX=`which clang++`")
endif()

add_subdirectory( src )
add_subdirectory( fuzz )
