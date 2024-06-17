#define _GNU_SOURCE
#include <stdio.h>
#include <sched.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define STACK_SIZE 65536  // 64KB stack size for the child process

int child_function(void *arg) {
    printf("Child process: PID = %d\n", getpid());
    return 0;
}

int main() {
    char *stack = malloc(STACK_SIZE);
    if (stack == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
	// 
    int pid = clone(child_function, stack + STACK_SIZE, CLONE_VM | SIGCHLD, NULL);
    if (pid == -1) {
        perror("clone");
        exit(EXIT_FAILURE);
    }

    // Wait for the child process to terminate
    waitpid(pid, NULL, 0);

    printf("Parent process: PID = %d\n", getpid());

    free(stack);
    return 0;
}
