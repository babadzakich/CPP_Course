add_library(babadzakiMQ STATIC 
    "src/Configuration/ConfigReader.cpp"
    "src/Brocker/BabadzakiMQ.cpp"
    "src/Brocker/Queues/MemoryQueueFIFO.cpp"
    "src/Brocker/Queues/MemoryQueueNone.cpp"
    "src/Brocker/Queues/DiskQueueFIFO.cpp"
    "src/Brocker/Queues/DiskQueueNone.cpp"
)

target_compile_features(babadzakiMQ PUBLIC cxx_std_17)

target_include_directories(babadzakiMQ PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

find_package(cppzmq REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(yaml-cpp REQUIRED)
find_package(spdlog REQUIRED)

target_link_libraries(babadzakiMQ PUBLIC spdlog::spdlog cppzmq nlohmann_json::nlohmann_json yaml-cpp::yaml-cpp)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 9.1)
    target_link_libraries(babadzakiMQ PUBLIC stdc++fs)
endif()

install(TARGETS babadzakiMQ
    EXPORT babadzakiMQTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(EXPORT babadzakiMQTargets
    FILE babadzakiMQConfig.cmake
    NAMESPACE babadzakiMQ::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/babadzakiMQ
)

add_executable(broker src/startMQ.cpp)

target_link_libraries(broker PRIVATE babadzakiMQ)

install(TARGETS broker RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

set (DOXYGEN_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/docs)
set (DOXYGEN_EXTRACT_ALL YES)

# add_subdirectory(tests)
