add_library(
    emulator 
    emulator.h 
    emulator.cpp 
    microcode_executor.h 
    microcode_executor.cpp 
    state.h 
    state.cpp 
    renderer.h 
    renderer.cpp) 
target_link_libraries(emulator decoder cpu)
target_include_directories(emulator INTERFACE ..)

add_executable(emulator_exe main.cpp)
target_link_libraries(emulator_exe emulator)
set_property(TARGET emulator_exe PROPERTY OUTPUT_NAME emulator)