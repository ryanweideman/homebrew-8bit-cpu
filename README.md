# homebrew-8bit-cpu
[![homebrew-8bit-cpu-test](https://github.com/ryanweideman/homebrew-8bit-cpu/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ryanweideman/homebrew-8bit-cpu/actions/workflows/c-cpp.yml)

A customed designed 8-bit cpu architecture, assembly language and assembler, and emulator. The aim was to design fully functional and practical CPU, with the ultimate goal of implementing the CPU in hardware with the fewest number of discrete ICs possible.
 
## Features
* 8-bit CISC, load store, microcoded architecture
* Instruction set contains 37 instructions with support for up to 64
* Up to 16 microcoded instructions per assembly instruction
* 32KB ROM
* 32KB RAM
* 10kHz max clock rate (tested)
* LCD Screen Output

<img src="/media/hello_world.gif" width="80%"/>
The CPU executing ```hello_world.asm``` at a 7 Hz clock rate.

## Build
To build the project, run the following from the root directory of the project
~~~
cd build
cmake ..
make
~~~
## Assemble
The assembler executable is available in the ```build/bin/``` directory. Below is an example of assembling the ```hello_world.asm``` file into an output file named ```hello_world.hex```
```
./assembler ../../examples/hello_world.asm hello_world.hex
```
The output hex file can be programmed to the 32KB ROM chip with a [28C256-EEPROM-Programmer](https://github.com/ryanweideman/28C256-EEPROM-Programmer). 
## Emulate
The emulator executable is also available in the ```build/bin/``` directory. Below is an example command of how ```hello_world.hex``` could be emulated.  
```
./emulator hello_world.hex
```
The emulator output is demonstrated below for a sample program.

<img src="/media/emulator.gif" width="30%"/>

## Test
The [GoogleTest](https://github.com/google/googletest) framework is used for all testing.  
To run the tests, navigate to the build directory and run the following after building the project:
```
ctest
```


