# Interpreter
Programm interprete ASM code using C++ language and libraries and output some grammatical and lexical analyse results, and process opcodes due to the code you have written in your .asm file. 

It made not for all instructions, but works perfect with available. 

Interpreter is able to work with 16 bit registers as old 98 masm.

## Installing 
Originally programm was written in XCode as command line tools, so you can compile it with your terminal C++ compiler using C++14. But I would highly recomend to use XCode compiler instead of g++.

## Work
So as input you pass .asm file with your code and as output you recieve grammatical and lexical analysing results, and the listing with opcodes and offsets.

In your input file, you are able to use only this instructions:

"MOV" "STOS" "ADD" "WAIT" "RETN" "XCHG" "AND" "OR" "JBE"

## Test results


## Build with
C++14

XCode 7.0 (can be used later version)

## Applicatoin base
The programm is based on procedure programming and some use of OOP paradigms.

## Autors
* **Tkachenko Ivan** - *Initial work* - [SteadyCoder](https://github.com/SteadyCoder)
