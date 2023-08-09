cmake_minimum_required(VERSION 3.16.3)

set(CMAKE_CXX_STANDARD 20)
set(EXECUTABLE_OUTPUT_PATH "../")

list(APPEND flags 
	"-Wall" 
	"-Werror" 
	"-Werror=switch-enum")

project(cpu)

add_executable(
	assembler 
	src/cpu.h
	src/cpu.cpp
	src/assembler/tokenizer.h 
	src/assembler/tokenizer.cpp
	src/assembler/define_table.h 
	src/assembler/define_table.cpp
	src/assembler/tokenizer.cpp
	src/assembler/assembler.cpp 
	src/assembler/instruction_parser.h 
	src/assembler/instruction_parser.cpp 
	src/assembler/label_table.h 
	src/assembler/label_table.cpp 
	src/assembler/macro.h 
	src/assembler/macro.cpp 
	src/assembler/macro_generator.h
	src/assembler/macro_generator.cpp)

target_compile_options(assembler
	PRIVATE
	${flags})

add_executable(decoder_generator 
	src/cpu.h 
	src/cpu.cpp
	src/decoder/decoder.h
	src/decoder/decoder.cpp
	src/decoder/decoder_main.cpp)

target_compile_options(decoder_generator
	PRIVATE
	${flags})

add_executable(emulator 
	src/cpu.h 
	src/cpu.cpp 
	src/decoder/decoder.h
	src/decoder/decoder.cpp
	src/emulator/emulator.h
	src/emulator/emulator.cpp
	src/emulator/microcode_executor.h
	src/emulator/microcode_executor.cpp
	src/emulator/state.h
	src/emulator/state.cpp)

#target_compile_options(emulator
#	PRIVATE
#	${flags})
