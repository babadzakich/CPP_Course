add_library(libcompressor STATIC src/libcompressor.cpp)

target_include_directories(libcompressor PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

find_package(BZip2 REQUIRED)
find_package(ZLIB REQUIRED)
find_package(GTest REQUIRED)

target_link_libraries(libcompressor PUBLIC BZip2::BZip2 ZLIB::ZLIB GTest::GTest)

install(TARGETS libcompressor
    EXPORT libcompressorTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT libcompressorTargets
    FILE libcompressorConfig.cmake
    NAMESPACE libcompressor::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libcompressor
)

set (DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
set (DOXYGEN_EXTRACT_ALL YES)

add_subdirectory(tests)
