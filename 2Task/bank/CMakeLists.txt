add_executable(bank src/banc.cpp)

target_link_libraries(bank PRIVATE banklib)

install(TARGETS bank RUNTIME DESTINATION ${CMAKE_INSTAL_BINDIR})
