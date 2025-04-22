add_executable(Regex main.cpp)

target_link_libraries(Regex PRIVATE regexlib)

install(TARGETS Regex RUNTIME DESTINATION ${CMAKE_INSTAL_BINDIR})