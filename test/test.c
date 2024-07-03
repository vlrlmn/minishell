#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

void run_exec(char *cmd, char **args, char **envp) {
    // printf("Executing command: %s\n", cmd);
    execve(cmd, args, envp);
    perror("execve");
    exit(1);
}

void run_pipe() {
    int p[2];
    pid_t pid1, pid2;
    int status;

    if (pipe(p) < 0) {
        perror("pipe");
        exit(1);
    }

    if ((pid1 = fork()) == 0) {

        close(p[0]);
        if (dup2(p[1], STDOUT_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        close(p[1]);
        char *left_cmd = "/bin/echo";
        char *left_args[] = {left_cmd, "hello", NULL};
        run_exec(left_cmd, left_args, NULL);
    } else if (pid1 < 0) {
        perror("fork");
        exit(1);
    }

    if ((pid2 = fork()) == 0) {
        close(p[1]);
        if (dup2(p[0], STDIN_FILENO) < 0) {
            perror("dup2");
            exit(1);
        }
        close(p[0]);
        char *right_cmd = "/bin/ls";
        char *right_args[] = {right_cmd, "-l", NULL};
        run_exec(right_cmd, right_args, NULL);
    } else if (pid2 < 0) {
        perror("fork");
        exit(1);
    }

    close(p[0]);
    close(p[1]);
    waitpid(pid1, &status, 0);
    printf("pid1 exited with status %d\n", WIFEXITED(status) ? WEXITSTATUS(status) : -1);
    waitpid(pid2, &status, 0);
    printf("pid2 exited with status %d\n", WIFEXITED(status) ? WEXITSTATUS(status) : -1);
}

int main() {
    run_pipe();
    return 0;
}
