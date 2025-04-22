add_executable(regextest Regextest.cpp)

find_package(GTest REQUIRED)

target_link_libraries(regextest PRIVATE regexlib GTest::GTest)

target_include_directories(regextest PRIVATE ../Include)

add_test(
        NAME Regextest
        COMMAND regextest
            --gtest_output=xml:${CMAKE_BINARY_DIR}/test_result/test_results.xml
)