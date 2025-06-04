//
// Created by BenHao on 2025/6/4.
//

#include <kernel/types.h>
#include <user/user.h>

void prime(int pipe_fd[2], int base) {
    close(pipe_fd[1]); // Close write end of the pipe
    fprintf(1, "prime %d\n", base);
    int n;
    short init_child = 0;
    int child_fd[2];
    while (read(pipe_fd[0], &n, sizeof(n)) > 0) {
        if (n % base != 0) {
            if (init_child == 0) {
                if (pipe(child_fd) < 0) {
                    fprintf(2, "pipe failed\n");
                    exit(1);
                }
                init_child = 1;
                if (fork() == 0) {
                    // Child process
                    prime(child_fd, n);
                    exit(0);
                }
                close(child_fd[0]); // Close read end of the child pipe
            } else {
                write(child_fd[1], &n, sizeof(n));
            }
        }
    }
    if (init_child) {
        close(child_fd[1]);
        wait(0);
    }
    close(pipe_fd[0]);
}

int main(int argc, char *argv[]) {
    int pid = fork();
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    }
    int pipe_fd[2];
    if (pipe(pipe_fd) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    if (pid == 0) {
        // Child process
        prime(pipe_fd, 2);
    } else {
        close(pipe_fd[0]); // Close read end of the pipe
        for (int i = 3; i <= 280; i++) {
            write(pipe_fd[1], &i, sizeof(i));
        }
        close(pipe_fd[1]);
        wait(0);
    }
    exit(0);
}
