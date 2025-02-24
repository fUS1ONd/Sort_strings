cmake_minimum_required(VERSION 3.7)
project(sort_strings LANGUAGES C)

set(CMAKE_C_STANDARD 99)

enable_testing()

option(USE_MMAP "Use memory-mapped file I/O" OFF)

set(SOURCES src/file_reader_api.c)

if (USE_MMAP)
    message(STATUS "Using memory-mapped file I/O")
    set(SOURCES src/file_reader_mmap.c)
else()
    message(STATUS "Using standard file I/O")
endif()

add_executable(sort_strings src/main.c src/sortings.c src/comparators.c ${SOURCES})

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

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")

add_custom_target(clean-build
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}
    COMMENT "cleaning build"
)
add_test(NAME test_sort_strings_1
    COMMAND bash -c "./sort_strings 5 ${CMAKE_SOURCE_DIR}/test/1.in bubble asc > ${CMAKE_BINARY_DIR}/1.out && ${CMAKE_COMMAND} -E compare_files ${CMAKE_SOURCE_DIR}/test/1_asc_expected.out ${CMAKE_BINARY_DIR}/1.out"
)

# Добавляем кастомную цель для сборки и запуска тестов
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running all tests"
)
