add_executable(tests tests.cpp)
target_link_libraries(tests gtest_main cpu)

add_test(NAME tests COMMAND $<TARGET_FILE:tests>)
