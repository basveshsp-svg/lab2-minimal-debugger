#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

long set_breakpoint(pid_t pid, long addr) {
    long data = ptrace(PTRACE_PEEKTEXT, pid, (void*)addr, NULL);
    long int3 = (data & ~0xff) | 0xcc;   // replace lowest byte with INT3
    ptrace(PTRACE_POKETEXT, pid, (void*)addr, (void*)int3);
    return data; // return original instruction
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <program>\n", argv[0]);
        return 1;
    }

    pid_t pid = fork();

    if (pid == 0) {
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        execvp(argv[1], &argv[1]);
        perror("exec failed");
        exit(1);
    } else {
    int status;
    struct user_regs_struct regs;

    // Wait for child to stop after exec
    waitpid(pid, &status, 0);
    printf("Child stopped. Setting breakpoint...\n");

    // Get registers to know where we are
    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    long rip = regs.rip;

    // Set breakpoint at current RIP
    long saved_data = set_breakpoint(pid, rip);

    // Continue execution
    ptrace(PTRACE_CONT, pid, NULL, NULL);
    waitpid(pid, &status, 0);

    if (WIFSTOPPED(status)) {
        printf("Breakpoint hit!\n");
    }

    // Restore original instruction
    ptrace(PTRACE_POKETEXT, pid, (void*)rip, (void*)saved_data);

    // Continue execution normally
    ptrace(PTRACE_CONT, pid, NULL, NULL);
    waitpid(pid, &status, 0);

    printf("Child finished execution.\n");
}



    return 0;
}
