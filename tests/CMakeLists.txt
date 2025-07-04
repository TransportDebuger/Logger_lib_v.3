find_package(GTest REQUIRED)
add_executable(test_simple_logger
    ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_simple_logger.cpp
)
target_link_libraries(test_simple_logger PRIVATE logger3 GTest::gtest_main)
add_test(NAME SimpleLoggerTest COMMAND test_simple_logger)