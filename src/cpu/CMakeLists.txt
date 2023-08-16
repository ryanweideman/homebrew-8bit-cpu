add_library(
    cpu 
    cpu.cpp 
    cpu.h 
    instruction_serializer.cpp 
    instruction_serializer.h
    state.cpp
    state.h)

target_include_directories(cpu INTERFACE ..)