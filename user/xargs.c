#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: xargs command [args...]\n");
        exit(1);
    }

    if (argc >= MAXARG) {
        fprintf(2, "xargs: too many arguments\n");
        exit(1);
    }

    char *command = argv[1];
    char *exec_argv[MAXARG];
    int base_argc = argc - 1;

    for (int i = 0; i < base_argc; i++) {
        exec_argv[i] = argv[i + 1];
    }

    char line[512];
    char c;
    int pos = 0;

    while (read(0, &c, 1) > 0) {
        if (c == '\n') {
            if (pos > 0) {
                line[pos] = '\0';
                exec_argv[base_argc] = line;
                exec_argv[base_argc + 1] = 0;

                int pid = fork();
                if (pid < 0) {
                    fprintf(2, "xargs: fork failed\n");
                    exit(1);
                } else if (pid == 0) {
                    exec(command, exec_argv);
                    fprintf(2, "xargs: exec %s failed\n", command);
                    exit(1);
                } else {
                    wait(0);
                }
                pos = 0;
            }
        } else {
            if (pos < sizeof(line) - 1) {
                line[pos++] = c;
            }
        }
    }

    if (pos > 0) {
        line[pos] = '\0';
        exec_argv[base_argc] = line;
        exec_argv[base_argc + 1] = 0;

        int pid = fork();
        if (pid < 0) {
            fprintf(2, "xargs: fork failed\n");
            exit(1);
        } else if (pid == 0) {
            exec(command, exec_argv);
            fprintf(2, "xargs: exec %s failed\n", command);
            exit(1);
        } else {
            wait(0);
        }
    }

    exit(0);
}