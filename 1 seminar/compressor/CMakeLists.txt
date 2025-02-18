find_package(spdlog REQUIRED)

add_executable(compressor src/compressor.cpp)

target_link_libraries(compressor PRIVATE libcompressor spdlog::spdlog)

install(TARGETS compressor RUNTIME DESTINATION ${CMAKE_INSTAL_BINDIR})
