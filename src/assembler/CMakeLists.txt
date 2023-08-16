add_library(assembler 
    assembler.h
    assembler.cpp
    tokenizer.h 
    tokenizer.cpp 
    define_table.h 
    define_table.cpp 
    tokenizer.cpp 
    label_table.h 
    label_table.cpp 
    macro.h 
    macro.cpp 
    macro_generator.h 
    macro_generator.cpp)

target_link_libraries(assembler cpu)
target_include_directories(assembler INTERFACE ..)

add_executable(assembler_exe main.cpp)
target_link_libraries(assembler_exe assembler) 
set_property(TARGET assembler_exe PROPERTY OUTPUT_NAME assembler)