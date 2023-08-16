add_executable(integration_tests test.cpp)
target_link_libraries(integration_tests cpu gtest_main)
add_test(NAME integration_tests COMMAND $<TARGET_FILE:integration_tests>)