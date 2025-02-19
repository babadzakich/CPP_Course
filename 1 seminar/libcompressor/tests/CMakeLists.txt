add_executable(libcompressortest libcompressortest.cpp)

target_link_libraries(libcompressortest PRIVATE libcompressor GTest::GTest)

target_include_directories(libcompressortest PRIVATE ../include)

add_test(
    NAME libcompressortest 
    COMMAND libcompressortest
        --gtest_output=xml:${CMAKE_BINARY_DIR}/test_result/test_results.xml
)