cmake_minimum_required(VERSION 3.7)
project(sort_strings LANGUAGES C)

set(CMAKE_C_STANDARD 99)


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
    COMMENT "Очистка директории build"
)
