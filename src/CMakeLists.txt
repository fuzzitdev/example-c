add_library( libteststring STATIC
        test_string.cpp test_string.h)

add_executable(test_string main.cpp)
target_link_libraries(test_string libteststring)