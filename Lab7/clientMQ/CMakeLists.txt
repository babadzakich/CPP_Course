add_library(client STATIC 
    "src/Client.cpp"
)

target_compile_features(client PUBLIC cxx_std_17)

target_include_directories(client PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

find_package(cppzmq REQUIRED)
target_link_libraries(client PUBLIC cppzmq)
install(TARGETS client
    EXPORT clientTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)