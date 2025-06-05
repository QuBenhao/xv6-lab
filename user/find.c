#include <kernel/types.h>
#include <kernel/stat.h>
#include <user/user.h>
#include "kernel/fs.h"
#include "kernel/fcntl.h"

#define BUFSIZE 512

void find(char *path, char *file_name)
{
    char buf[BUFSIZE], *p;
    int fd;
    struct dirent de;
    struct stat st;

    if ((fd = open(path, O_RDONLY)) < 0)
    {
        fprintf(2, "find: cannot open %s\n", path);
        exit(1);
    }
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "find: cannot stat %s\n", path);
        close(fd);
        exit(1);
    }
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
    {
        fprintf(2, "find: path too long\n");
        close(fd);
        return;
    }
    switch (st.type)
    {
    case T_DEVICE:
    case T_FILE:
        break;
    case T_DIR:
        strcpy(buf, path);
        p = buf + strlen(buf);
        *p++ = '/';
        while (read(fd, &de, sizeof(de)) == sizeof(de))
        {
            if (de.inum == 0)
                continue; // skip empty entries
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = '\0'; // null-terminate the string
            if (stat(buf, &st) < 0)
            {
                fprintf(2, "find: cannot stat %s\n", buf);
                continue;
            }
            if (st.type == T_DIR)
            {
                if (strcmp(de.name, ".") != 0 && strcmp(de.name, "..") != 0)
                {
                    find(buf, file_name); // recursive call for subdirectories
                }
            }
            else if (st.type == T_FILE || st.type == T_DEVICE)
            {
                if (strcmp(de.name, file_name) == 0)
                {
                    printf("%s\n", buf); // print the full path of the found file
                }
            }
        }
        break;
    }
    close(fd);
}

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        fprintf(2, "Usage: find <folder_path> <file_name>\n");
        exit(1);
    }

    char *path = argv[1];
    char *file_name = argv[2];

    if (strlen(path) >= BUFSIZE || strlen(file_name) >= BUFSIZE)
    {
        fprintf(2, "Error: Path or file name too long.\n");
        exit(1);
    }
    if (strlen(path) + 1 + strlen(file_name) + 1 > BUFSIZE)
    {
        fprintf(2, "Error: Path and file name too long.\n");
        exit(1);
    }
    find(path, file_name);

    exit(0);
}