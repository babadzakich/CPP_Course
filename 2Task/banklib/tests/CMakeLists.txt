add_executable(banklibtest banklibtest.cpp)

find_package(GTest REQUIRED)

target_link_libraries(banklibtest PRIVATE banklib GTest::GTest)

target_include_directories(banklibtest PRIVATE ../include)

add_test(
    NAME banklibtest 
    COMMAND banklibtest
        --gtest_output=xml:${CMAKE_BINARY_DIR}/test_result/test_results.xml
)
