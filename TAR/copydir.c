#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
char dpath[200] = "C:\\Users\\JunMung\\Desktop\\testplace\\";
// /char dpath[200] = "C:\\Users\\JunMung\\Desktop\\mystudy\\21-2winter\\study\\TAR\\test\\";

void copydir(char path[], char ddir[], char *filename) // (복사할 파일 위치, 붙여넣기 할 위치, 그 파일 이름)
{
    char dir[100];
    strcpy(dir, ddir);
    strcat(dir, filename);
    FILE *fp;
    long lSize;
    char *buffer;
    size_t result;

    fp = fopen(path, "rb");
    if (fp == NULL)
    {
        fputs("File error", stderr);
        return;
    }
    fseek(fp, 0, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    buffer = malloc(sizeof(char) * lSize);
    if (buffer == NULL)
    {
        fputs("Memory error", stderr);
        return;
    }

    result = fread(buffer, 1, lSize, fp);
    if (result != lSize)
    {
        fputs("Reading error", stderr);
        return;
        //exit(3);
    }
    FILE *p;
    p = fopen(dir, "wb");
    fwrite(buffer, 1, lSize, p);
};

void finddir(DIR *dp, int layer, char path[], char ddir[])
{
    dp = opendir(path);
    struct dirent *ep; //dp dir stream 안에 있는 파일들 정보 담을 struct

    if (dp != NULL)
    {
        while (ep = readdir(dp))
        {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
            {
                continue;
            }

            char s[30];
            strcpy(s, path);
            strcat(s, ep->d_name); //./ + subdir
            //printf("%s", s);
            DIR *p = opendir(s);
            if (p != NULL)
            {
                for (int i = 0; i < layer; i++)
                {
                    printf("    ");
                }

                DIR *a;
                char dir[200];
                char s1[10] = "/";
                strcpy(dir, ddir);
                strcat(dir, ep->d_name);
                strcat(dir, s1);
                strcat(s, s1);

                //printf("%s  ", s);
                //printf("%s  ", dir);
                puts(ep->d_name);

                mkdir(dir);
                layer += 1;
                //printf(" %s  => ", dir);
                finddir(a, layer, s, dir);
                layer -= 1;
            }
            else
            {
                for (int i = 0; i < layer; i++)
                {
                    printf("    ");
                }
                puts(ep->d_name);
                char b[100];
                strcpy(b, path);
                strcat(b, ep->d_name);
                //printf(" %s  => ", ddir);
                copydir(b, ddir, ep->d_name);
            }
        }
        closedir(dp);
    }
    else
    {
        perror("Couldn't open the directory\n");
    }
};
int main(void)
{
    DIR *dp;
    char path[200] = "./";
    finddir(dp, 0, path, dpath);
}