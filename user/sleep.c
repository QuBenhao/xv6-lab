//
// Created by BenHao on 2025/6/4.
//
#include <kernel/types.h>
#include <user/user.h>


int main(int argc, char *argv[])
{
    int i;

    if (argc < 2) {
        fprintf(2, "Usage: sleep seconds\n");
        exit(1);
    }
    for (i = 1; i < argc; i++) {
        int second = atoi(argv[i]);
        if (second < 0) {
            fprintf(2, "Invalid argument[%d]: %s\n", i, argv[i]);
            exit(1);
        }
        sleep(second); // Convert seconds to milliseconds
    }


    exit(0);
}