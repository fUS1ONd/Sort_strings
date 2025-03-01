cmake_minimum_required(VERSION 3.7)
project(sort_strings LANGUAGES C)

set(CMAKE_C_STANDARD 99)

set(ALGORITHMS bubble insertion merge radix quick)

enable_testing()

option(USE_MMAP "Use memory-mapped file I/O" OFF)

set(SOURCES src/file_reader_api.c)

if (USE_MMAP)
    message(STATUS "Using memory-mapped file I/O")
    set(SOURCES src/file_reader_mmap.c)
else()
    message(STATUS "Using standard file I/O")
endif()

add_executable(sort_strings src/sort_strings src/sortings.c src/comparators.c ${SOURCES})

target_include_directories(sort_strings PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_compile_options(sort_strings PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Werror
    -fsanitize=address
    -fno-omit-frame-pointer
)

target_link_options(sort_strings PRIVATE -fsanitize=address)

message(STATUS "Build...")

add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "Cleaning build"
)

function(add_invalid_parameters_tests)
    add_test(
        NAME not_enough_parameters
        COMMAND bash -c "./sort_strings 5 ${CMAKE_SOURCE_DIR}/test/1.in bubble > /dev/null 2>&1; test $? -eq 255"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    add_test(
        NAME too_much_parameters
        COMMAND bash -c "./sort_strings 5 ${CMAKE_SOURCE_DIR}/test/1.in bubble asc des > /dev/null 2>&1; test $? -eq 255"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    add_test(
        NAME incorrect_parameter_file
        COMMAND bash -c "./sort_strings 5 chuchumba.in bubble asc > /dev/null 2>&1; test $? -eq 255"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    add_test(
        NAME incorrect_parameter_number_of_strings_equal_minus
        COMMAND bash -c "./sort_strings -1 ${CMAKE_SOURCE_DIR}/test/1.in bubble asc > /dev/null 2>&1; test $? -eq 255"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    add_test(
        NAME incorrect_parameter_number_of_strings_0
        COMMAND bash -c "./sort_strings 0 ${CMAKE_SOURCE_DIR}/test/1.in bubble asc > /dev/null 2>&1; test $? -eq 255"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    add_test(
        NAME incorrect_algo
        COMMAND bash -c "./sort_strings 5 ${CMAKE_SOURCE_DIR}/test/1.in mamba asc > /dev/null 2>&1; test $? -eq 255"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    set_tests_properties(
        not_enough_parameters
        too_much_parameters
        incorrect_parameter_file
        incorrect_parameter_number_of_strings_equal_minus
        incorrect_parameter_number_of_strings_0
        incorrect_algo
        PROPERTIES LABELS "Invalid Parameters"
    )
endfunction(add_invalid_parameters_tests)

add_invalid_parameters_tests()

function(add_sort_test num_of_str input_file algorithm expected_output)
    get_filename_component(test_name ${input_file} NAME_WE)

    set(run_program_cmd "./sort_strings ${num_of_str} ${input_file} ${algorithm} asc > ${CMAKE_BINARY_DIR}/${test_name}_${algorithm}.out")
    
    set(compare_cmd "cmp ${CMAKE_BINARY_DIR}/${test_name}_${algorithm}.out ${expected_output}")

    add_test(
        NAME ${test_name}_${algorithm}
        COMMAND bash -c "${run_program_cmd} && ${compare_cmd}"
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )

    set_tests_properties(
        ${test_name}_${algorithm}
        PROPERTIES LABELS "Basic Asc Sorting"
    )
endfunction()

file(GLOB input_files "${CMAKE_SOURCE_DIR}/test/input/*.in")
foreach(input_file ${input_files})
    get_filename_component(test_name ${input_file} NAME_WE)
    execute_process(
        COMMAND wc -l ${input_file}
        OUTPUT_VARIABLE num_of_str
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    string(REGEX REPLACE "^[ ]*([0-9]+).*" "\\1" num_of_str "${num_of_str}")
    foreach(algorithm ${ALGORITHMS})
        set(expected_output "${CMAKE_SOURCE_DIR}/test/output/${test_name}.out")
        add_sort_test(${num_of_str} ${input_file} ${algorithm} ${expected_output})
    endforeach()
endforeach()

add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests"
)
