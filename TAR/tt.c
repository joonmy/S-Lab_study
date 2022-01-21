#include <stdio.h>

int main()
{
    FILE *pFile;
    int a = 123;
    unsigned char buffer[4];
    buffer[0] = a;

    //sprintf(buffer, "%d", a);
    //printf("zz%c", buffer);
    pFile = fopen("name.txt", "wb");
    fwrite(buffer, sizeof(char), sizeof(buffer), pFile);
    fclose(pFile);
    return 0;
}