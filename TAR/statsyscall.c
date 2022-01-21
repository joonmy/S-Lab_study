#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdlib.h>

void sfile(char const filename[]);

int main()
{
    ssize_t read;
    char *buffer = 0;
    size_t buf_size = 0;

    printf("Enter the name of a file to check: \n");
    read = getline(&buffer, &buf_size, stdin);

    if (read <= 0)
    {
        printf("getline failed\n");
        exit(1);
    }

    if (buffer[read - 1] == '\n')
    {
        buffer[read - 1] = 0;
    };

    sfile(buffer);
    free(buffer);
    return 0;
}

void sfile(char const filename[])
{

    struct stat sfile;

    if (stat(filename, &sfile) == -1)
    {
        printf("Error Occurred\n");
    }
    printf("%o", sfile.st_mode);

    printf("  %o", sfile.st_mode & S_IFMT);
    switch (sfile.st_mode & S_IFMT)
    {
    case S_IFREG:
        puts("regular\n");
        break;
    case S_IFDIR:
        puts("directory\n");
        break;
    }
}