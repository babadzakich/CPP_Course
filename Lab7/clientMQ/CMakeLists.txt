add_library(client STATIC 
    "src/MQClient.cpp"
)

target_compile_features(client PUBLIC cxx_std_17)

target_include_directories(client PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

find_package(cppzmq REQUIRED)
find_package(nlohmann_json REQUIRED)
find_package(spdlog REQUIRED)
target_link_libraries(client PUBLIC cppzmq nlohmann_json::nlohmann_json spdlog::spdlog)
install(TARGETS client
    EXPORT clientTargets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

add_executable(clientRunner startClient.cpp)
target_link_libraries(clientRunner PRIVATE client)

add_executable(test_scenario test_scenario.cpp)
target_link_libraries(test_scenario PRIVATE client)

add_executable(stress_test stress_test.cpp)
target_link_libraries(stress_test PRIVATE client)