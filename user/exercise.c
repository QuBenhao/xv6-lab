//
// Created by BenHao on 2025/6/4.
//

#include "kernel/types.h"
#include "user/user.h"

/**
 *  Write a program that uses UNIX system calls to “ping-pong” a byte between two processes
over a pair of pipes, one for each direction. Measure the program’s performance, in ex-
changes per second.
 */

int main(int argc, char *argv[]) {
    if (argc != 2) {
        // times
        write(1, "usage: pingpong times\n", 38);
        exit(1);
    }
    int times = atoi(argv[1]);
    if (times <= 0) {
        write(1, "times must be a positive integer\n", 35);
        exit(1);
    }
    int pipe1[2], pipe2[2];
    if (pipe(pipe1) < 0 || pipe(pipe2) < 0) {
        write(1, "pipe error\n", 12);
        exit(1);
    }
    int pid = fork();
    if (pid < 0) {
        write(1, "fork error\n", 12);
        exit(1);
    }
    char buf[5];
    buf[4] = '\0'; // Null-terminate the buffer for safety
    if (pid == 0) {
        // Child process
        close(pipe1[1]); // Close write end of pipe1
        close(pipe2[0]); // Close read end of pipe2
        for (int i = 0; i < times; i++) {
            // Read from pipe1
            if (read(pipe1[0], buf, 4) != 4) {
                write(1, "read error in child\n", 21);
                exit(1);
            }
            // Write to pipe2
            if (write(pipe2[1], "pong", 4) != 4) {
                write(1, "write error in child\n", 22);
                exit(1);
            }
            fprintf(1, "Child received: %s\n", buf);
        }
        close(pipe1[0]); // Close read end of pipe1
        close(pipe2[1]); // Close write end of pipe2
    } else {
        // Parent process
        close(pipe1[0]); // Close read end of pipe1
        close(pipe2[1]); // Close write end of pipe2
        for (int i = 0; i < times; i++) {
            // Write to pipe1
            if (write(pipe1[1], "ping", 4) != 4) {
                write(1, "write error in parent\n", 23);
                exit(1);
            }
            // Read from pipe2
            if (read(pipe2[0], buf, 4) != 4) {
                write(1, "read error in parent\n", 22);
                exit(1);
            }
            fprintf(1, "Parent received: %s\n", buf);
        }
        close(pipe1[1]); // Close write end of pipe1
        close(pipe2[0]); // Close read end of pipe2
        wait(0); // Wait for child to finish
        fprintf(1, "Parent finished ping-ponging %d times\n", times);
    }
    return 0;
}