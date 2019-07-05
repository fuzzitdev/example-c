macro(fuzzer name)
    add_executable(${name} ${name}.cpp)
    set_target_properties(${name}
            PROPERTIES
            COMPILE_FLAGS "-fsanitize=fuzzer"
            LINK_FLAGS "-fsanitize=fuzzer")
    target_link_libraries(${name}
            PRIVATE
            libteststring
            )
    target_include_directories(${name}
            PRIVATE
            ../src)
endmacro()

fuzzer( fuzz_test_string )