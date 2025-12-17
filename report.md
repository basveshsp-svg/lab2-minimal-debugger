## 1. Introduction

This assignment implements a minimal debugger for Linux ELF binaries using
the ptrace system call. The goal of the debugger is to demonstrate how a
process can control and inspect another process at runtime using OS-level
debugging primitives, without relying on external tools such as GDB.

The assignment focuses on understanding the interaction between user-space
programs and the operating system kernel. Debuggers are essential tools for
software development, and implementing a minimal debugger helps in
understanding process control, signals, and low-level execution flow.


## 2. Design Overview

The debugger follows a parent–child process model. The debugger process
creates a child using fork(). The child requests tracing permission using
ptrace(PTRACE_TRACEME) and then executes the target ELF binary using execvp().
The parent process acts as the debugger and controls the execution of the
child using ptrace commands.

The parent debugger process waits for state changes in the child using
waitpid(). Whenever the child process stops due to execution events such as
program start or breakpoint hits, the parent regains control and issues
appropriate ptrace commands. This design closely resembles the internal
working of production debuggers like GDB, but in a simplified form.


## 3. ptrace Usage

The ptrace system call enables a process to observe and control another
process. In this implementation, ptrace is the core mechanism used for
debugging. The following ptrace requests are used:

- PTRACE_TRACEME: Invoked by the child process to indicate that it should be
  traced by its parent. This allows the kernel to notify the parent of all
  important execution events.

- PTRACE_CONT: Used by the debugger to resume execution of the stopped child
  process until the next event occurs.

- PTRACE_SINGLESTEP: Allows execution of exactly one CPU instruction before
  stopping again, enabling instruction-level debugging.

- PTRACE_GETREGS: Reads the CPU register state of the child process, which is
  essential for understanding program execution.

- PTRACE_PEEKTEXT and PTRACE_POKETEXT: Used to read and write the child
  process memory, forming the basis of software breakpoint implementation.



## 4. Software Breakpoints

Software breakpoints are implemented by replacing the instruction at a
specific address with the INT3 (0xCC) instruction. When the breakpoint is
hit, the kernel raises a SIGTRAP signal and stops the process. The debugger
then restores the original instruction and continues execution.

When the INT3 instruction is executed, the CPU generates a trap that is
handled by the kernel. The kernel then sends a SIGTRAP signal to the debugger
and stops the child process. This mechanism allows the debugger to regain
control exactly at the breakpoint location.

## 5. Testing and Demonstration

The debugger was tested using standard Linux ELF binaries such as /bin/ls.
The following features were verified:
- Program loading and execution
- Single-step execution
- Register inspection
- Breakpoint handling

Manual testing was performed to validate each feature of the debugger. The
target program was executed multiple times to ensure consistent behavior.
Testing focused on correctness of process control rather than performance or
usability, as the objective of the assignment was functional correctness


## 6. Limitations

This debugger is a minimal implementation and does not support symbolic
debugging, DWARF information, memory inspection, or interactive commands.
Breakpoints are hardcoded and only demonstrate basic functionality.

These limitations are intentional, as the assignment emphasizes core OS
concepts rather than full-scale debugger functionality. Extending the
debugger to support symbols and memory inspection would require parsing ELF
and DWARF metadata, which is beyond the scope of this lab.



## 7. Conclusion

This assignment demonstrates how OS-level debugging can be implemented using
ptrace. Despite being minimal, the debugger satisfies all required features
and provides insight into how full-scale debuggers operate internally.

Through this assignment, practical experience was gained in low-level system
programming and Linux process control. The implementation reinforced concepts
such as system calls, signals, and CPU execution flow.
