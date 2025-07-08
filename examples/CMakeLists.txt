add_executable(basic_usage
        ${CMAKE_CURRENT_SOURCE_DIR}/basic_usage/main.cpp
    )
    target_link_libraries(basic_usage PRIVATE logger3)
    set_target_properties(basic_usage PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/basic_usage
    )