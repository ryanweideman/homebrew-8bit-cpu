# homebrew-8bit-cpu
[![homebrew-8bit-cpu-test](https://github.com/ryanweideman/homebrew-8bit-cpu/actions/workflows/ci.yml/badge.svg)](https://github.com/ryanweideman/homebrew-8bit-cpu/actions/workflows/ci.yml)

A custom designed 8-bit cpu architecture, assembly language and assembler, and emulator. The aim was to design a fully functional and programmable CPU, and implement the CPU in hardware with the fewest number of discrete integrated circuits possible.

## Features
* 8-bit CISC, load store, microcoded architecture
* Instruction set contains 37 instructions with support for up to 64
* Up to 16 microcoded instructions per assembly instruction
* 32 KB ROM
* 32 KB RAM
* 10 kHz max clock rate (tested)
* LCD Screen Output

## Demos
 _The CPU executing "Hello World!" at a 7 Hz clock rate!_
<img src="/media/hello_world.gif" width="80%"/>

_A sample of the assembly language!_
```
# This program calculates the fibonacci numbers up to 233
# The result is stored in register A

initialize:
    ldi A 1    # Load 1 to register A
    ldi B 1    # Load 1 to register B
    
loop:
    sta A 0    # Store register A into RAM address 0 
    add A      # Add A + B and store result in register A
    bc  done   # if A + B > 255, branch to done
    lda B 0    # Load the previous value of A from RAM at address 0 into register B
    b   loop   # Repeat the calculation

done: 
    lda A 0    # Recover register A from RAM - the highest Fibonacci number less than 255
    b   done   # Endlessly loop
```
_Emulating the above program that calculates the fibonacci numbers up to 233. Check out register A for the results!_
<img src="/media/fibonacci.gif" width="50%"/>

## Build
To build the project, run the following from the root directory of the project
```
cd build
cmake ..
make
```
## Assemble
The assembler executable is available in the ```build/bin/``` directory. Below is an example of assembling the ```hello_world.asm``` file into an output file named ```hello_world.hex```
```
./assembler ../../examples/hello_world.asm hello_world.hex
```
The output hex file can be programmed to the 32 KB ROM chip with a [28C256-EEPROM-Programmer](https://github.com/ryanweideman/28C256-EEPROM-Programmer). 
## Emulate
The emulator executable is also available in the ```build/bin/``` directory. Below is an example command of how ```hello_world.hex``` could be emulated.  
```
./emulator hello_world.hex
```

## Test
The [GoogleTest](https://github.com/google/googletest) framework is used for all testing.  
To run the tests, navigate to the build directory and run the following after building the project:
```
ctest
```


