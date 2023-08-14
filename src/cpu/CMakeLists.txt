add_library(
    cpu 
    cpu.cpp 
    cpu.h 
    instruction_serializer.cpp 
    instruction_serializer.h)

target_include_directories(cpu INTERFACE ..)