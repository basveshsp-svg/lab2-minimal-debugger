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
        waitpid(pid, &status, 0);
        printf("Child stopped. Debugger ready.\n");
    }

    return 0;
}
