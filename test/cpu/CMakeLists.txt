add_executable(cpu_tests instruction_serializer.cpp)
target_link_libraries(cpu_tests cpu gtest_main)
add_test(NAME cpu_tests COMMAND $<TARGET_FILE:cpu_tests>)