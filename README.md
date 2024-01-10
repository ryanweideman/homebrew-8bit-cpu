# homebrew-8bit-cpu
[![homebrew-8bit-cpu-test](https://github.com/ryanweideman/homebrew-8bit-cpu/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/ryanweideman/homebrew-8bit-cpu/actions/workflows/c-cpp.yml)

A customed designed 8-bit cpu architecture, assembly language and assembler, and emulator.
It is an 8-bit CISC and load store architecture,  32KB ROM and 32KB RAM, built entirely from 74 series TTL chips.
The aim was to design fully functional and practical CPU, with the ultimate goal of implementing the CPU in hardware with the fewest number of discrete ICs possible. 

## Features
* 32KB ROM
* 32KB RAM
* 37 assembly instructions with support for up to 64
* Max of 16 microcode instructions per assembly instruction
* LCD Screen Output

<img src="/media/hello_world.gif" width="80%"/>

## Build
To build the project, run the following from the root directory of the project
~~~
cmake --build build
~~~
## Test
The [GoogleTest](https://github.com/google/googletest) framework is used for all testing.  
To run the tests, simply navigate to the build directory and run the following after building the project:
```
ctest
```
<img src="/media/emulator.gif" width="30%"/>

