add_library(storageLib STATIC "src/Utility/RandomGenerator.cpp" 
                "src/DTO/Pallet.cpp")

find_package(Threads REQUIRED)

target_link_libraries(storageLib PRIVATE Threads::Threads)

target_include_directories(storageLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS storageLib
    EXPORT storageLibTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT storageLibTargets
    FILE storageLibConfig.cmake
    NAMESPACE storageLib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/storageLib
)

add_executable(simulation main.cpp)
target_link_libraries(simulation PRIVATE storageLib)

install(TARGETS simulation
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set (DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
set (DOXYGEN_EXTRACT_ALL YES)

add_subdirectory(tests)
