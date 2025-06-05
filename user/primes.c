//
// Created by BenHao on 2025/6/4.
//

#include <kernel/types.h>
#include <user/user.h>

void print_prime(int prime) {
    fprintf(1, "prime %d\n", prime);
}

void sieve(int fd, int base) {
    print_prime(base);
    int num;
    int next_fd[2];
    short init = 0;
    while (read(fd, &num, sizeof(num)) > 0) {
        if (num % base != 0) {
            if (init == 0) {
                init = 1;
                if (pipe(next_fd) < 0) {
                    fprintf(2, "pipe failed\n");
                    exit(1);
                }
                if (fork() == 0) {
                    close(fd); // Close read end in child
                    close(next_fd[1]); // Close write end in child
                    sieve(next_fd[0], num); // Continue sieving with the next number
                    exit(0);
                } else {
                    close(next_fd[0]); // Close read end in parent
                }
            } else {
                write(next_fd[1], &num, sizeof(num));
            }
        }
    }
    if (init) {
        close(next_fd[1]); // Close write end after finishing writing
        wait(0); // Wait for the child process to finish
    }
    close(fd);
}

int main(int argc, char *argv[]) {
    int fd[2];
    if (pipe(fd) < 0) {
        fprintf(2, "pipe failed\n");
        exit(1);
    }

    if (fork() == 0) { // Child process
        close(fd[1]); // Close write end in child
        sieve(fd[0], 2); // Start sieving with base 2
        close(fd[0]);
        exit(0);
    } else { // Parent process
        close(fd[0]); // Close read end in parent
        for (int i = 3; i <= 280; ++i) {
            write(fd[1], &i, sizeof(i)); // Write numbers to the pipe
        }
        close(fd[1]); // Close write end after writing all numbers
        wait(0); // Wait for the child process to finish
    }
    exit(0);
}
