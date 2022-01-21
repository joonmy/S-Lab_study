#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
char dpath[200] = "C:\\Users\\JunMung\\Desktop\\testplace\\";
//char dpath[200] = "C:\\Users\\JunMung\\Desktop\\mystudy\\21-2winter\\study\\TAR\\test\\";

void t_archive(FILE* wb, char filepath[], int num) //쓸 파일 stream,  읽을 파일 stream (num == 1 파일, num ==0 디렉토리)
{
    FILE* rb;
    long lSize;
    char* buffer;
    size_t result;
    rb = fopen(filepath, "rb");
    if (rb == NULL) {
        unsigned int file = 0;
        fwrite(&file, sizeof(unsigned int), 1, wb); //1byte에 0 디렉토리임을 표시

        unsigned int lenpath = strlen(filepath);
        fwrite(&lenpath, sizeof(unsigned int), 1, wb); //4byte path길이

        fwrite(filepath, sizeof(char), lenpath, wb); //파일 path

        unsigned int lendata = 0;
        fwrite(&lendata, sizeof(unsigned int), 1, wb); //4byte 데이터 size
        printf("lenpath: %d\n", lenpath);
        printf("file path: %s\n", filepath);
        printf("lendata: %d\n", lendata);
        return;
    }

    fseek(rb, 0, SEEK_END);
    lSize = ftell(rb);
    rewind(rb);

    buffer = malloc(sizeof(char) * lSize);
    if (buffer == NULL) {
        fputs("Memory error", stderr);
        return;
    }

    result = fread(buffer, 1, lSize, rb);
    if (result != lSize) {
        fputs("Reading error", stderr);
        return;
        //exit(3);
    }
    unsigned int file = 1; //4byte 에 1 파일임을 표시
    fwrite(&file, sizeof(unsigned int), 1, wb);

    unsigned int lenpath = strlen(filepath);
    fwrite(&lenpath, sizeof(unsigned int), 1, wb); //4byte path길이

    fwrite(filepath, sizeof(char), lenpath, wb); //파일 path

    unsigned int lendata = lSize;
    fwrite(&lendata, sizeof(unsigned int), 1, wb); //4byte 데이터 size

    fwrite(buffer, sizeof(char), lSize, wb); // N_byte 데이터
    free(buffer);
    printf("lenpath: %d\n", lenpath);
    printf("file path: %s\n", filepath);
    printf("lendata: %d\n\n", lendata);
};

void archivefile(FILE* wb, DIR* dp, int layer, char path[], char dpath[])
{
    dp = opendir(path);
    struct dirent* ep;
    if (dp != NULL) {

        t_archive(wb, path, 0);
        while (ep = readdir(dp)) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0 || strcmp(ep->d_name, ".git") == 0) {
                continue;
            }
            char s[200];
            strcpy(s, path);
            strcat(s, ep->d_name);

            DIR* p = opendir(s);
            if (p != NULL) {
                for (int i = 0; i < layer; i++) {
                    printf("    ");
                }
                printf("*");
                puts(ep->d_name);

                DIR* a;
                char dir[200];
                char s1[10] = "/";
                strcpy(dir, dpath);
                strcat(dir, ep->d_name);
                strcat(dir, s1);
                strcat(s, s1);
                layer += 1;

                archivefile(wb, a, layer, s, dir);
                layer -= 1;
            } else {
                for (int i = 0; i < layer; i++) {
                    printf("    ");
                }
                printf("--");
                puts(ep->d_name);

                char filepath[200];
                strcpy(filepath, path);
                strcat(filepath, ep->d_name);
                t_archive(wb, filepath, 1);
            }
        }
        closedir(dp);
    } else {
        perror("Couldn't open the directory\n");
    }
};
void listfile(DIR* dp, int layer, char path[])
{
    dp = opendir(path);
    struct dirent* ep; //dp dir stream 안에 있는 파일들 정보 담을 struct
    if (dp != NULL) {
        while (ep = readdir(dp)) {
            if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0 || strcmp(ep->d_name, ".git") == 0) {
                continue;
            }

            char s[200];
            strcpy(s, path);
            strcat(s, ep->d_name);

            DIR* p = opendir(s);

            if (p != NULL) {
                for (int i = 0; i < layer; i++) {
                    printf("    ");
                }
                printf("*");
                puts(ep->d_name);

                DIR* a;
                char s1[10] = "/";
                strcat(s, s1);
                layer += 1;
                listfile(a, layer, s);
                layer -= 1;
            } else {
                for (int i = 0; i < layer; i++) {
                    printf("    ");
                }
                printf("--");
                puts(ep->d_name);
            }
        }
        closedir(dp);
    } else {
        perror("Couldn't open the directory\n");
    }
};
void extract(char* filename)
{
    int i;
    unsigned int n, len, size;
    FILE* fp;
    FILE* p;
    char* buffer;
    char s[10] = "\0";

    fp = fopen(filename, "rb");
    if (fp == NULL) {
        fputs("File error", stderr);
        return;
    }
    while (1) {
        for (i = 0; i < 5; i++) {

            switch (i) {
            case 0:
                if (fread(&n, sizeof(unsigned int), 1, fp) == 0) {
                    fclose(fp);
                    printf("finish extract");
                    exit(1);
                    break;
                }
                printf("file or not :%d\n", n);
                free(buffer);
                break;
            case 1:
                if (fread(&len, sizeof(unsigned int), 1, fp) == 0) {
                    break;
                }
                printf("length of the path :%d\n", len);
                break;
            case 2:
                buffer = malloc(sizeof(char) * len + 1);
                if (fread(buffer, len, 1, fp) == 0) {
                    break;
                }
                buffer[len] = '\0';
                printf("path: %s \n", buffer);
                if (n == 0) {
                    mkdir(buffer);
                } else {
                    p = fopen(buffer, "wb");
                }
                free(buffer);
                break;
            case 3:
                if (fread(&size, sizeof(unsigned int), 1, fp) == 0) {
                    break;
                }
                printf("data size :%d\n", size);
                break;
            case 4:
                buffer = malloc(sizeof(char) * size);
                if (fread(buffer, size, 1, fp) == 0) {
                    break;
                }
                printf("data: %s\n", buffer);
                fwrite(buffer, 1, size, p);
                free(buffer);
                fclose(p);
                break;
            default:
                break;
            }
        }
    }
};
int main(int argc, char* argv[])
{
    char buf[100];
    int command;

    FILE* wb;

    DIR* dp;
    if (strcmp(argv[1], "archive") == 0)
        command = 0;
    if (strcmp(argv[1], "list") == 0)
        command = 1;
    if (strcmp(argv[1], "extract") == 0)
        command = 2;

    switch (command) {
    case 0:
        wb = fopen(argv[2], "wb");
        archivefile(wb, dp, 0, argv[3], dpath);
        fclose(wb);
        break;
    case 1:
        listfile(dp, 0, argv[2]);
        break;
    case 2:
        extract(argv[2]);
        break;
    default:
        printf("wrong command!");
        break;
    }
}