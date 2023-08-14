add_library(
    decoder 
    decoder.h   
    decoder.cpp) 
target_link_libraries(decoder cpu)
target_include_directories(decoder INTERFACE..)

add_executable(decoder_exe main.cpp)
target_link_libraries(decoder_exe decoder)
set_property(TARGET decoder_exe PROPERTY OUTPUT_NAME decoder)