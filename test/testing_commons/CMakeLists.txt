add_library(
    testing_commons 
    commons.cpp 
    commons.h 
    )

target_link_libraries(testing_commons cpu gtest gtest_main)

target_include_directories(testing_commons INTERFACE ..)