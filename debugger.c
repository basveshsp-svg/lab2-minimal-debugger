#include <sys/user.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

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

    waitpid(pid, &status, 0);
    printf("Child stopped. Single stepping one instruction...\n");

    ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
    waitpid(pid, &status, 0);

    ptrace(PTRACE_GETREGS, pid, NULL, &regs);
    printf("RIP (instruction pointer) = 0x%llx\n", regs.rip);

    ptrace(PTRACE_CONT, pid, NULL, NULL);
    waitpid(pid, &status, 0);

    printf("Child finished execution.\n");
}


    return 0;
}
