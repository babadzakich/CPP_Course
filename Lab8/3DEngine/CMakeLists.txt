add_library(3DEngine STATIC
    "src/SimpleEngine/SimpleEngine.cpp"
    "src/BinaryEngine/BinaryEngine.cpp"
    "src/SpatHashEngine/SpatHashEngine.cpp"
    "src/SOAEngine/SOAEngine.cpp"
    "src/MultithreadEngine/MultithreadEngine.cpp")

find_package(Threads REQUIRED)

target_include_directories(3DEngine PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_link_libraries(3DEngine PUBLIC Threads::Threads)

install(TARGETS 3DEngine
    EXPORT 3DEngineTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT 3DEngineTargets
    FILE 3DEngineConfig.cmake
    NAMESPACE 3DEngine::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/3DEngine
)

add_executable(engine main.cpp)
target_link_libraries(engine PRIVATE 3DEngine)

install(TARGETS engine
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set (DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
set (DOXYGEN_EXTRACT_ALL YES)

# add_subdirectory(tests)
