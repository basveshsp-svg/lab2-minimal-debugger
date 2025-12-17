# Minimal Debugger

## Overview
This project implements a minimal debugger for Linux ELF binaries using the
ptrace system call. The debugger can control execution, single-step through
instructions, inspect CPU registers, and handle software breakpoints.

## Build Instructions
To compile the debugger, run:

make


This will generate the executable named `debugger`.

## Run Instructions
To run the debugger on an ELF binary, use:

./debugger <path-to-program>

Example:

./debugger /bin/ls


## Features
- Load and execute ELF binaries
- Continue execution
- Single-step execution
- CPU register inspection
- Software breakpoint support

## Test Cases
### Test 1: Load and execute ELF binary
Command:
./debugger /bin/ls

Expected Result:
Debugger takes control of the process.

### Test 2: Continue execution
Expected Result:
Target program runs after debugger continues execution.

### Test 3: Single-step execution
Expected Result:
One instruction is executed and RIP register value is printed.

### Test 4: Register inspection
Expected Result:
CPU registers are successfully read using ptrace.

### Test 5: Software breakpoint
Expected Result:
Debugger prints "Breakpoint hit!" when breakpoint is triggered.



