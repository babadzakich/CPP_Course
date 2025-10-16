add_library(libconvertor STATIC "src/Json/JsonParser.cpp" 
"src/Toml/TomlParser.cpp"  
"src/Json/JsonBuilder.cpp" 
"src/Toml/TomlBuilder.cpp"
"src/Xml/XmlParser.cpp" "src/Xml/XmlBuilder.cpp")

target_include_directories(libconvertor PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS libconvertor
    EXPORT libconvertorTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT libconvertorTargets
    FILE libconvertorConfig.cmake
    NAMESPACE libconvertor::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/libconvertor
)

add_executable(convertor main.cpp)
target_link_libraries(convertor PRIVATE libconvertor)

install(TARGETS convertor
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

set (DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
set (DOXYGEN_EXTRACT_ALL YES)

add_subdirectory(tests)
