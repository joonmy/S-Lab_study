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
    }

    int s = open(buffer, O_RDONLY);
    if (s == -1)
    {
        printf("File doesn't exist\n");
        exit(1);
    }
    else
    {
        sfile(buffer);
    }
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

    //Accessing data members of stat struct
    printf("\nFile st_uid %d \n", sfile.st_uid);
    if (S_ISREG(sfile.st_mode))
    {
        printf("regular file");
    }
    else
    {
        printf("not regular file");
    }
}