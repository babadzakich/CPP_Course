cmake_minimum_required(VERSION 3.5)

project(task_1)

set(CMAKE_EXPORT_COMPILE_COMMANDS YES)

find_program(CLANG_FORMAT "clang-format")

if (CLANG_FORMAT)
    file(GLOB_RECURSE ALL_SOURCE_FILES
        "*.cpp" "*.hpp" "*.c" "*.h"
    )

    add_custom_target(
        format
        COMMAND ${CLANG_FORMAT}
        -i
        -style=file
        ${ALL_SOURCE_FILES}
        COMMENT "Formatting all source files with clang-format..."
    )
else()
    message(WARNING "clang-format not found. The 'format' target will not be available.")
endif()

find_program(CLANG_TIDY "clang-tidy")

if (CLANG_TIDY)
    set(CMAKE_CXX_CLANG_TIDY
        ${CLANG_TIDY}
        -config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
        -extra-arg=-Wno-unknown-warning-option
    )
else()
    message(WARNING "clang-tidy not found. Static analysis will not be performed.")
endif()

find_package(Doxygen REQUIRED)

doxygen_add_docs(docs
    ${CMAKE_SOURCE_DIR}/libcompressor/include
    ${CMAKE_SOURCE_DIR}/libcompressor/src
    ${CMAKE_SOURCE_DIR}/compressor/src
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    COMMENT "Generating API documentation with Doxygen"
    DOXYFILE ${CMAKE_SOURCE_DIR}/Doxyfile
)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}")
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}")

include(GNUInstallDirs)

enable_testing()

add_subdirectory(libcompressor)
add_subdirectory(compressor)

