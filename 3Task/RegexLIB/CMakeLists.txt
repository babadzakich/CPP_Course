add_library(regexlib STATIC
        src/Engine.cpp
        src/Regex.cpp
        src/Nodes/Node_Regular.cpp
        src/Nodes/Node_Group.cpp
        src/Nodes/Node_Quantifier.cpp
        src/Nodes/Node_Sequence.cpp
        Include/Nodes/Node_type.h
)

target_include_directories(regexlib PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/Include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

install(TARGETS regexlib
        EXPORT regexlibTargets
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT regexlibTargets
        FILE regexlibConfig.cmake
        NAMESPACE regexlib::
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/regexlib
)

add_subdirectory(test)