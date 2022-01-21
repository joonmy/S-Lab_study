#include <stdio.h>
#include <stdlib.h>

int main()
{
    char a[100];
    FILE *fp;
    long lSize;
    char *buffer;
    size_t result;

    printf("Enter the file name for showing as a hexademical number\n");
    scanf("%s", &a);

    fp = fopen(a, "rb");
    if (fp == NULL)
    {
        fputs("File error", stderr);
        return 0;
    }
    fseek(fp, 0, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        return 0;
    }

    result = fread(buffer, 1, lSize, fp);
    printf("==%d==", result);
    if (result != lSize)
    {
        fputs("Reading error", stderr);
        return 0;
        //exit(3);
    }
    //fwrite(buffer,1,lSize,fp);

    for (int i = 1; i <= lSize; i++)
    {
        printf("%2x ", buffer[i - 1]);
        if (i % 2 == 0)
        {
            printf(" ");
        }
        if (i % 16 == 0)
        {
            printf("\n");
        }
    }
}