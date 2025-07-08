find_package(GTest REQUIRED)
add_executable(test_simple_logger
    ${CMAKE_CURRENT_SOURCE_DIR}/unit/test_simple_logger.cpp
)
target_link_libraries(test_simple_logger PRIVATE logger3 GTest::gtest_main)
add_test(NAME SimpleLoggerTest COMMAND test_simple_logger)

add_executable(test_log_level unit/test_log_level.cpp)
target_link_libraries(test_log_level PRIVATE logger3 GTest::gtest_main)
add_test(NAME LogLevelTest COMMAND test_log_level)

add_executable(test_standard_formatter
    unit/test_standard_formatter.cpp
)
target_link_libraries(test_standard_formatter PRIVATE logger3 GTest::gtest_main)
add_test(NAME StandardFormatterTest COMMAND test_standard_formatter)

add_executable(test_simple_logger_formatting
    unit/test_simple_logger_formatting.cpp
)
target_link_libraries(test_simple_logger_formatting PRIVATE logger3 GTest::gtest_main)
add_test(NAME SimpleLoggerFormattingTest COMMAND test_simple_logger_formatting)