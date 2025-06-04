//
// Created by BenHao on 2025/6/4.
//

#include <kernel/types.h>
#include <user/user.h>

int main(int argc, char *argv[]) {

    int pipe1[2], pipe2[2];
    int pr = pipe(pipe1);
    if (pr < 0) {
        fprintf(2, "pipe1 failed\n");
        exit(1);
    }
    pr = pipe(pipe2);
    if (pr < 0) {
        fprintf(2, "pipe2 failed\n");
        exit(1);
    }
    int pid = fork();
    char buf[1];
    if (pid < 0) {
        fprintf(2, "fork failed\n");
        exit(1);
    } else if (pid == 0) { // Child process
        close(pipe1[1]); // Close write end of pipe1
        close(pipe2[0]); // Close read end of pipe2

        read(pipe1[0], buf, 1); // Read from pipe1
        int process_id = getpid();
        fprintf(1, "%d: received ping\n", process_id);
        write(pipe2[1], buf, 1); // Write to pipe2
        close(pipe1[0]);
        close(pipe2[1]);
    } else { // Parent process
        close(pipe1[0]); // Close read end of pipe1
        close(pipe2[1]); // Close write end of pipe2
        write(pipe1[1], "x", 1); // Write to pipe1
        read(pipe2[0], buf, 1); // Read from pipe2
        int process_id = getpid();
        fprintf(1, "%d: received pong\n", process_id);
        close(pipe1[1]);
        close(pipe2[0]);
    }
    exit(0);
}